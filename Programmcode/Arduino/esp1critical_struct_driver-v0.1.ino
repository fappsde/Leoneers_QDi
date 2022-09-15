
#include "Wire.h"
#include <ESP32Servo.h>
//#include <Adafruit_NeoPixel.h>
/*#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif*/

//Hardware pins

Servo servorear;
Servo servof;
//Servo servoR;
Servo esc;

//int SERVORPIN     5
int servorpin =5;
int SERVOFPIN =   18;
int SERVORPIN =   5;
int ESCPIN =    19;

int LIDARPIN =    33;


#define I2C_DEV_ADDR 0x55

//Drivers


//ESP32PWM servorpwm;
//ESP32PWM servofpwm;
//ESP32PWM escpwm;

ESP32PWM lidarpwm;



//Configuration


int speedlimiter=15;//1-90
int speedlimiterrev=-50;//1-90
int lenkunglimiter=90;//1-90

float servorpos = 0; 
float servofpos = 0; 
float escpos = 0; 

uint32_t i = 0;

int systemstatus=-1;//-1 unknown, 0 raspi off, 1 all systems work properly, 255 error dont start, 100 cam0 error, 101 cam1 error, 200 imu error, 201 start imu
int drivesystemidlestatus=-1;//-1 unknown, 0 raspi off, 1 driving, 5 wait for start idle,  201 imu start

//Software 


int drivespeed=0;
int nextdrivespeed=0;

long stepstonextdrivespeed=1L;
long stepstonextlenkung_rear=1L;
long stepstonextlenkung_front=1L;
long lastmillisspeed = 0L;
long lastmillislenkung_rear = 0L;
long lastmillislenkung_front = 0L;


int revint=0;//0normale 1 intiing, 2 reverse

int lidarfreq = 100;
float lidarspeed = 0.0;

int drivemode=0;//0:idle, 1:drivefwrd, 2:break, 3:breakhard, 99:disabeld, -1:drivereverse, -2:reversebreak
//int prevdrivemode=0;
int lenkung_front=0;//0:geradeaus, -: links, +:rechts
int nextlenkung_front=0;
int lenkung_rear=0;//0:geradeaus, -: links, +:rechts
int nextlenkung_rear=0;




int breakhelpmod=0;

boolean fullyinitialized=false;

void setup() {

  Serial.begin(115200);
  
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  
  

//startsequence();

  esc.setPeriodHertz(50);    // standard 50 hz servo
  esc.attach(ESCPIN, 1000, 2000);
  servorear.setPeriodHertz(50);    // standard 50 hz servo
  servorear.attach(servorpin, 1180, 1780);// attaches the servo on pin 18 to the servo object using default min/max of 1000us and 2000us
  servof.setPeriodHertz(50);    // standard 50 hz servo
  servof.attach(SERVOFPIN, 1050, 1590);
  //Values for original servo:  1050, 1590
  
  // attaches the servo on pin 18 to the servo object using default min/max of 1000us and 2000us
  //servoR.setPeriodHertz(50);    // standard 50 hz servo
  //servoR.attach(SERVORPIN, 1050, 1590);
  // different servos may require different min/max settings for an accurate 0 to 180 sweep
  
  servorear.write(90);
  servof.write(90);
  //servoR.write(90);
  esc.write(90);


  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  #if CONFIG_IDF_TARGET_ESP32
    char message[64];
    snprintf(message, 64, "%u Packets.", i++);
    Wire.slaveWrite((uint8_t *)message, strlen(message));
  #endif

  lidarpwm.attachPin(LIDARPIN, lidarfreq, 10);// 1KHz 8 bit

  
  lidarpwm.adjustFrequency(lidarfreq, lidarspeed);

  pinMode(34, INPUT);//frontled
  pinMode(35, INPUT);//sonstwas
}

void loop() {
  // put your main code here, to run repeatedly:

  
  


  if(millis() > stepstonextdrivespeed + lastmillisspeed){
    lastmillisspeed=millis();
    if(nextdrivespeed<=2&&nextdrivespeed>=-2){
      nextdrivespeed=0;
    }
    if(drivespeed<nextdrivespeed){
      drivespeed=drivespeed+1;
    }else if(drivespeed>nextdrivespeed){
      drivespeed=drivespeed-1;
    }else{
      stepstonextdrivespeed=1;
    }
  }
  if(millis() > stepstonextlenkung_front + lastmillislenkung_front){
    lastmillislenkung_front=millis();
    if(lenkung_front<nextlenkung_front){
      int add=(nextlenkung_front-lenkung_front)/10;
      lenkung_front=lenkung_front+1+add;
      //dobreak(&rearstrip,0);
      
    }else if(lenkung_front>nextlenkung_front){
      int add=(lenkung_front-nextlenkung_front)/10;
      lenkung_front=lenkung_front-1-add;
      
      //dobreak(&rearstrip,1);
    }else{
      stepstonextlenkung_front=1;
      
    }
   
  }
  if(millis() > stepstonextlenkung_rear + lastmillislenkung_rear){
    lastmillislenkung_rear=millis();
    if(lenkung_rear<nextlenkung_rear){
      int add=(nextlenkung_rear-lenkung_rear)/10;
      lenkung_rear=lenkung_rear+1+add;
      //dobreak(&rearstrip,0);
      
    }else if(lenkung_rear>nextlenkung_rear){
      int add=(lenkung_rear-nextlenkung_rear)/10;
      lenkung_rear=lenkung_rear-1-add;
      
      //dobreak(&rearstrip,1);
    }else{
      stepstonextlenkung_rear=1;
      
    }
   
  }
  
  
  updatepwm();

  

  //onupdate();

delay(2);//5
}






int checkrange(int value, int minv, int maxv){
  if(value>maxv){
    return maxv;
  }
  if(value<minv){
    return minv;
  }
  return value;
}

void updatepwm(){
  if(drivemode>=0){
    esc.write(checkrange(drivespeed,-90,speedlimiter)+90);
  }else{
    if(revint!=1){
      esc.write(checkrange(drivespeed,speedlimiterrev,speedlimiter)+90);//speedlimiterrev
    }
  }
  servof.write(checkrange(lenkung_front,-lenkunglimiter,lenkunglimiter)+95);
 // servoR.write(checkrange(lenkung,-lenkunglimiter,lenkunglimiter)+95);
  servorear.write((checkrange(lenkung_rear*-1,-lenkunglimiter,lenkunglimiter)+95));

 
  
}
//Driving 
/*
void setdriveSpeed(){

  
}

void break(int steps,int delayt){
  
}

void onupdate(){
  
  
  }
*/





void displaysystemstatus(){
  systemstatus;//-1 unknown, 0 raspi off, 1 all systems work properly, 255 error dont start, 100 cam0 error, 101 cam1 error, 200 imu error, 201 start imu
  drivesystemidlestatus;//-1 unknown, 0 raspi off, 1 driving, 5 wait for start idle,6 finised  201 imu start
  

  bool unknown=false;
  
  
  


  
  
}

void startsequence(){
 
  
  
}

void startreverse(){
  revint=1;
  for(int i=95;i>40;i--){
    esc.write(i);
    delay(1);
  }
  delay(800);
  for(int i=40;i<95;i++){
    esc.write(i);
    delay(1);
  }
  delay(800);
  for(int i=95;i>50;i--){
    esc.write(i);
    delay(1);
  }

  delay(300);
  revint=2;

  
}

void onRequest(){
  Wire.print(i++);
  Wire.print(" Packets.");
  Serial.println("onRequest");
}

void onReceive(int len){
  //Serial.printf("onReceive[%d]: ", len);
  String bufferstring="";
  uint8_t buff;
  int pos=0;
  while(Wire.available()){
    
     buff=Wire.read();
     pos++;
     
    //size_t bufflen = sizeof(buff);
    //strncat( bufferstring, buff, bufflen );
     // 'str' is now a string
    //Serial.write(buff);
    bufferstring+=(char) buff;
    //Serial.println(bufferstring);
    
    
    }
    String bufstring=String(bufferstring);
    bufstring.trim();
    
    
   // Serial.println(bufferstring+bufstring+bufferstring.charAt(1)+bufstring.indexOf("spd:",1)+bufstring.startsWith("spd:")+bufferstring.length());
    //Serial.println(bufferstring.charAt(1));
    //Serial.println(bufferstring+bufstring+bufstring.indexOf("drcn:",1)+bufstring.startsWith("drcn:")+bufferstring.length());
    
    
    if(bufstring.indexOf("spd:",1)!=-1){//Geschwindigkeit
      //Serial.println("Speed");
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        String newspeed=bufstring.substring(bufstring.indexOf("sdp:",1)+7,bufstring.indexOf(",",1));
        String nextspint=newspeed;
        nextdrivespeed=checkrange(nextspint.toInt(),-90,90);
        String timetospeed=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));

        String nextttsint=timetospeed;
        stepstonextdrivespeed=checkrange(nextttsint.toInt(),-90,90);
       // Serial.println(stepstonextdrivespeed);
        
      }
      
    }else if(bufstring.indexOf("drcn:",1)!=-1){//Lenkung
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        String pnewlenkung=bufstring.substring(bufstring.indexOf("drcn:",1)+5,bufstring.indexOf(",",1));
        String ptimetolenkung=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));
        
        String nextlint=pnewlenkung;
        nextlenkung_front=checkrange(nextlint.toInt(),-90,90);
        nextlenkung_rear=checkrange(nextlint.toInt(),-90,90);
        
        String nextttlint=ptimetolenkung;
        stepstonextlenkung_front=checkrange(nextttlint.toInt(),-90,90);
        stepstonextlenkung_rear=checkrange(nextttlint.toInt(),-90,90);
       // Serial.println(nextlenkung);
        
      }
    }else if(bufstring.indexOf("frontdrcn:",1)!=-1){//Lenkung
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        String pnewlenkung=bufstring.substring(bufstring.indexOf("frontdrcn:",1)+10,bufstring.indexOf(",",1));
        String ptimetolenkung=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));
        
        String nextlint=pnewlenkung;
        nextlenkung_front=checkrange(nextlint.toInt(),-90,90);
       
        
        String nextttlint=ptimetolenkung;
        stepstonextlenkung_front=checkrange(nextttlint.toInt(),-90,90);
        
      }
    }else if(bufstring.indexOf("reardrcn:",1)!=-1){//Lenkung
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        String pnewlenkung=bufstring.substring(bufstring.indexOf("reardrcn:",1)+9,bufstring.indexOf(",",1));
        String ptimetolenkung=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));
        
        String nextlint=pnewlenkung;
        nextlenkung_rear=checkrange(nextlint.toInt(),-90,90);
       
        
        String nextttlint=ptimetolenkung;
        stepstonextlenkung_rear=checkrange(nextttlint.toInt(),-90,90);
        
      }
    }else if(bufstring.indexOf("drvm:",1)!=-1){//Fahrmodus  //0:idle, 1:drivefwrd, 2:break, 3:breakhard, 99:disabeld, -1:drivereverse, -2:reversebreak
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        String pdrvmd=bufstring.substring(bufstring.indexOf("drvm:",1)+5,bufstring.indexOf(",",1));
        String p=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));
        
        String drvmdstr=pdrvmd;
        int ndrvmd=drvmdstr.toInt();
        if(ndrvmd==-1&&drivemode>=0){
            //start reverse
            drivemode=ndrvmd;
          startreverse();
          Serial.println("startrev");
        }
        
        drivemode=ndrvmd;
        
        
       // String nextttlint=ptimetolenkung;
        //stepstonextlenkung=checkrange(nextttlint.toInt(),-90,90);
       // Serial.println(nextlenkung);
        
      }
    }else if(bufstring.indexOf("lidar:",1)!=-1){//Lidar speed 0-100
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        String plidspd=bufstring.substring(bufstring.indexOf("lidar:",1)+6,bufstring.indexOf(",",1));
        String p=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));
        
        String lidspdstr=plidspd;
        lidarspeed=lidspdstr.toInt()/100;
        
        lidarpwm.adjustFrequency(lidarfreq, lidarspeed);
            
          Serial.println("lidarset");
               
      }
    }else if(bufstring.indexOf("stp:",1)!=-1){//full stop
      
    }
  else if(bufstring.indexOf("sysstat:",1)!=-1){//Lenkung
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        
        String sysstat=bufstring.substring(bufstring.indexOf("sysstat:",1)+8,bufstring.indexOf(",",1));
        String drivstat=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));
        //Serial.print(dimside);
        String sysstatstr=sysstat;
        int sysstatint=sysstatstr.toInt();
        
        String drivstatstr=drivstat;
        int drivstatint=drivstatstr.toInt();
        
        systemstatus=sysstatint;//-1 unknown, 0 raspi off, 1 all systems work properly, 255 error dont start, 100 cam0 error, 101 cam1 error, 200 imu error, 201 start imu
        drivesystemidlestatus=drivstatint;//-1 unknown, 0 raspi off, 1 driving, 5 wait for start idle,  201 imu start
        displaysystemstatus();
     }
  }
  //Serial.println();
}
