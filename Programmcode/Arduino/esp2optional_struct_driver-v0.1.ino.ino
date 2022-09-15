
#include "Wire.h"
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//Hardware pins
#define LED_FRONT_PIN     27
#define LED_REAR_PIN     26
#define LED_INDI_PIN     25

#define lufESPPIN     15
#define luf1PIN     2
#define luf2PIN     4


#define LED_FRONT_COUNT  37
//37
#define LED_REAR_COUNT  23
#define LED_INDI_COUNT  22
#define BRIGHTNESS 50
#define LED_FRONT_BRIGHTNESS 150
#define LED_INDI_BRIGHTNESS 50

#define I2C_DEV_ADDR 0x56


#define LED_INDIINSID_PIN     12
#define LED_INDIINSID_COUNT  4
#define LED_INDIINSID_BRIGHTNESS 100
Adafruit_NeoPixel indiindsstrip(LED_INDIINSID_COUNT, LED_INDIINSID_PIN, NEO_GRBW + NEO_KHZ800);

//Drivers
Adafruit_NeoPixel frontstrip(LED_FRONT_COUNT, LED_FRONT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rearstrip(LED_REAR_COUNT, LED_REAR_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel indistrip(LED_INDI_COUNT, LED_INDI_PIN, NEO_GRB + NEO_KHZ800);



ESP32PWM lufesppwm;
ESP32PWM luf1pwm;
ESP32PWM luf2pwm;

//Configuration
int lufespfreq = 100;
int luf1freq = 100;
int luf2freq = 100;

int speedlimiter=15;//1-90
int speedlimiterrev=-50;//1-90
int lenkunglimiter=90;//1-90

float servopos = 0; 
float escpos = 0; 

uint32_t i = 0;

int systemstatus=-1;//-1 unknown, 0 raspi off, 1 all systems work properly, 255 error dont start, 100 cam0 error, 101 cam1 error, 200 imu error, 201 start imu
int drivesystemidlestatus=-1;//-1 unknown, 0 raspi off, 1 driving, 5 wait for start idle,  201 imu start

//Software 
boolean rechtsblinken=false;
boolean linksblinken=false;

int drivespeed=0;
int nextdrivespeed=0;

long stepstonextdrivespeed=1L;
long stepstonextlenkung=1L;
long lastmillisspeed = 0L;
long lastmillislenkung = 0L;
long ledmilli=0L;

int revint=0;//0normale 1 intiing, 2 reverse



int drivemode=0;//0:idle, 1:drivefwrd, 2:break, 3:breakhard, 99:disabeld, -1:drivereverse, -2:reversebreak
//int prevdrivemode=0;
int lenkung=0;//0:geradeaus, -: links, +:rechts
int nextlenkung=0;

float lufespspeed = 0;//0-1
float luf1speed = 1.5;
float luf2speed = 1.5;


int lblinkstep=0;
int rblinkstep=0;

int breakhelpmod=0;

boolean fullyinitialized=false;

void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.begin(115200);
  
  rearstrip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  rearstrip.show();            // Turn OFF all pixels ASAP
  rearstrip.setBrightness(BRIGHTNESS);

  frontstrip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
           // Turn OFF all pixels ASAP
  frontstrip.setBrightness(LED_FRONT_BRIGHTNESS);
  

  startsequence();
  frontstrip.fill(frontstrip.Color(255,   255,   255));
   frontstrip.show();  


   indistrip.begin();
   indistrip.setBrightness(LED_INDI_BRIGHTNESS);
   indistrip.fill(frontstrip.Color(75,0,130));
   indistrip.show();


  indiindsstrip.begin();
   indiindsstrip.setBrightness(LED_INDIINSID_BRIGHTNESS);
   indiindsstrip.fill(indiindsstrip.Color(75,0,130,255));
   indiindsstrip.show();

   
  lufesppwm.attachPin(lufESPPIN, lufespfreq, 10);// 1KHz 8 bit
  luf1pwm.attachPin(luf1PIN, luf1freq, 10);// 1KHz 8 bit
  luf2pwm.attachPin(luf2PIN, luf2freq, 10);// 1KHz 8 bit

  


  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  #if CONFIG_IDF_TARGET_ESP32
    char message[64];
    snprintf(message, 64, "%u Packets.", i++);
    Wire.slaveWrite((uint8_t *)message, strlen(message));
  #endif


  pinMode(34, INPUT);//frontled
  pinMode(35, INPUT);//sonstwas
}

void loop() {
  

  
  


  if(millis() > stepstonextdrivespeed + lastmillisspeed){
    lastmillisspeed=millis();
    if(nextdrivespeed<=2&&nextdrivespeed>=-2){
      nextdrivespeed=0;
    }
    if(drivespeed<nextdrivespeed){
      drivespeed=drivespeed+1;
      dobreak(&rearstrip,0);
    }else if(drivespeed>nextdrivespeed){
      drivespeed=drivespeed-1;
      dobreak(&rearstrip,1);
    }else{
      stepstonextdrivespeed=1;
      dobreak(&rearstrip,0);
    }
  }
  if(millis() > stepstonextlenkung + lastmillislenkung){
    lastmillislenkung=millis();
    if(lenkung<nextlenkung){
      int add=(nextlenkung-lenkung)/10;
      lenkung=lenkung+1+add;
      //dobreak(&rearstrip,0);
      
    }else if(lenkung>nextlenkung){
      int add=(lenkung-nextlenkung)/10;
      lenkung=lenkung-1-add;
      //dobreak(&rearstrip,1);
    }else{
      stepstonextlenkung=1;
      
    }
    if(lenkung>5){
      dolinksblinken(&rearstrip,true);
      dorechtsblinken(&rearstrip,false);
    }else if(lenkung<-5){
      dolinksblinken(&rearstrip,false);
      dorechtsblinken(&rearstrip,true);
    }else{
      dolinksblinken(&rearstrip,false);
      dorechtsblinken(&rearstrip,false);
    }
  }
  
  
  updatepwm();

  

  //onupdate();

delay(2);//5
}



void startWipe(Adafruit_NeoPixel* strip,uint32_t color, long wait) {
  //if(side==0){
    //Adafruit_NeoPixel strip= rearstrip;
  //}
  /*if(ledmilli==0L){
    //ledmilli=millis();
  }*/
  strip->setPixelColor((17), color);         //  Set pixel's color (in RAM)
  strip->show();
      //delay(wait); 
     // if(millis() > ledmilli + wait){
  int p=0;
  for(int i=0; i<=6; i++) { // For each pixel in strip...
    //if(millis() > ledmilli + wait){
     // ledmilli=millis();
      strip->setPixelColor(6+i, color);         //  Set pixel's color (in RAM)
      strip->setPixelColor(5-i, color);
      strip->show(); 
    //}
      delay(wait); 
      //if(millis() > ledmilli + wait){
        //ledmilli=millis();
    strip->setPixelColor((16-i), color);         //  Set pixel's color (in RAM)
    strip->setPixelColor((18+i), color); 
      strip->show();
      p++;
      if(i>=5){
        ledmilli=0L;
      
      }
       delay(wait);  
  //}
       
  }
}
void frontstart(){
  int rangemin=0;
  int rangemax=85;
  int deltim=0;
  for(int i=0;i<2750;i++){
    int randaddr = random(rangemin, rangemax);
    int randaddg = random(rangemin, rangemax);
    int randaddb = random(rangemin, rangemax);
    
    int randled = random(0, 37);

    int r=randaddr;
    int g=randaddg;
    int b=randaddb;

    frontstrip.setPixelColor(randled, frontstrip.Color(r,   g,   b));
    frontstrip.show();

    rangemin=checkrange((i/10)-30,0,255);
    rangemax=checkrange((i/10)+85,0,255);

    deltim++;
    if(deltim>10){
      tone(luf1PIN, 3686+(i), // C
      1); // half a second
      tone(luf2PIN, 3686+(i), // C
      1);
      //delay(1); 
      deltim=0;
    }
  }
  if(digitalRead(34)==1){
    frontstrip.setBrightness(5);
  }else{
    frontstrip.setBrightness(BRIGHTNESS);
  }
  frontstrip.show();

  
}


void dobreak(Adafruit_NeoPixel* strip,int mod){

//if(breakhelpmod==mod){
  if(mod==0){
    //strip->fill(strip->Color(255,   0,   0));
    strip->setBrightness(BRIGHTNESS);
    strip->show(); 
    breakhelpmod=mod;
  }else if(mod=1){
    //strip->fill(strip->Color(255,   0,   0));
    strip->setBrightness(255);
    strip->show(); 
    breakhelpmod=mod;
  }
//}
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
  
  
  luf1pwm.adjustFrequency(luf1freq, luf1speed);
  luf2pwm.adjustFrequency(luf2freq, luf2speed);
  
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


void dolinksblinken(Adafruit_NeoPixel* strip,boolean swon){
  if(swon){
    strip->setPixelColor((22), rearstrip.Color(255,   175,   0)); 
    strip->setPixelColor((21), rearstrip.Color(255,   175,   0)); 
    strip->setPixelColor((20), rearstrip.Color(0,   0,   0)); 
     for(int i=0; i<=2; i++) {
      strip->setPixelColor((i), rearstrip.Color(255,   175,   0)); 
      
    }strip->show();
    
   }else{
      for(int i=0; i<=2; i++) {
        strip->setPixelColor((i), rearstrip.Color(255,  0,   0)); 
      
      }for(int i=20; i<=22; i++) {
      strip->setPixelColor((i), rearstrip.Color(255,  0,   0)); 
      
      }strip->show();
    }  
}
void dorechtsblinken(Adafruit_NeoPixel* strip,boolean swon){
if(swon){
    strip->setPixelColor((14), rearstrip.Color(1,   0,   0)); 
for(int i=9; i<=13; i++) {
      strip->setPixelColor((i), rearstrip.Color(255,   175,   0)); 
      
}strip->show();
    
  }else{
    
    strip->setPixelColor((14), rearstrip.Color(225,   0,   0)); 
for(int i=9; i<=13; i++) {
      strip->setPixelColor((i), rearstrip.Color(255,  0,   0)); 
      
}strip->show();
  } 
  
}

void dimfrontsidelight(Adafruit_NeoPixel* strip,int side,int dim){
  int newval=5;
  if(dim==0){newval=255;}
  if(side==0){
    for(int i=0; i<=37; i++) {
      strip->setPixelColor((i), rearstrip.Color(newval,  newval,   newval)); 
      if(i==3)i=37-6;
      
    }
    
  }else if (side==1){
    for(int i=14; i<=22; i++) {
      strip->setPixelColor((i), rearstrip.Color(newval,  newval,   newval)); 
      
    }

  }else if (side==2){
    for(int i=4; i<=31; i++) {
      strip->setPixelColor((i), rearstrip.Color(newval,  newval,   newval)); 
      if(i==13)i=22;
    }

  }
  if(digitalRead(34)==1){
    strip->setBrightness(5);
  }else{
    strip->setBrightness(BRIGHTNESS);
  }
  strip->show();

  


  
}

void displaysystemstatus(){
  systemstatus;//-1 unknown, 0 raspi off, 1 all systems work properly, 255 error dont start, 100 cam0 error, 101 cam1 error, 200 imu error, 201 start imu
  drivesystemidlestatus;//-1 unknown, 0 raspi off, 1 driving, 5 wait for start idle,6 finised  201 imu start
  int ledsides=255;
  int ledmidr=0;
  int ledmidg=255;
  int ledmidb=0;
  int ledsidr=0;
  int ledsidg=255;
  int ledsidb=0;
  int ledsidrr=0;
  int ledsidrg=255;
  int ledsidrb=0;

  bool unknown=false;
  if(systemstatus!=1||drivesystemidlestatus!=1){
    ledsides=0;
    ledmidr=250;ledmidb=0;ledmidg=150;ledsidrr=250;ledsidrb=0;ledsidrg=150;ledsidr=250;ledsidb=0;ledsidg=150;

    if(systemstatus==1){ledmidb=250;ledmidg=0;ledmidr=0;}
   else if(systemstatus==0){ledmidr=150;ledmidb=150;ledmidg=0;}
  else if(systemstatus==255){ledmidr=250;ledmidb=150;ledmidg=100;}
  else if(systemstatus==200){ledmidr=250;ledmidb=250;ledmidg=200;}
  else if(systemstatus==201){ledmidr=250;ledmidb=250;ledmidg=0;}
  else if(systemstatus==100){ledsidr=250;ledsidb=250;ledsidg=200;}
  else if(systemstatus==101){ledsidrr=250;ledsidrb=250;ledsidrg=200;}
  else{unknown=true;}
  if(drivesystemidlestatus==5){ledsidrr=0;ledsidrb=250;ledsidrg=0;ledsidr=0;ledsidb=250;ledsidg=0;}
  else if(drivesystemidlestatus==6){
    ledmidr=250;ledmidb=0;ledmidg=0;ledsidrr=0;ledsidrb=0;ledsidrg=250;ledsidr=0;ledsidb=0;ledsidg=250;
  }
  else{if(unknown){ledmidr=250;ledmidb=0;ledmidg=150;ledsidrr=250;ledsidrb=0;ledsidrg=150;ledsidr=250;ledsidb=0;ledsidg=150;}}
  }
  
  


  rearstrip.setPixelColor(17, rearstrip.Color(ledmidr,  ledmidg,   ledmidb)); 
  rearstrip.setPixelColor(5, rearstrip.Color(ledsidr,  ledsidg,   ledsidb)); 
  rearstrip.setPixelColor(6, rearstrip.Color(ledsidrr,  ledsidrg,   ledsidrb)); 
  rearstrip.setPixelColor(16, rearstrip.Color(ledsides,  0,   0)); 
  rearstrip.setPixelColor(18, rearstrip.Color(ledsides,  0,   0)); 
  
}

void startsequence(){
  frontstart();
  startWipe(&rearstrip,rearstrip.Color(255,   0,   0)     , 60L); // Red
  startWipe(&rearstrip,rearstrip.Color(0,   0,   0)     , 50L); // Red
  startWipe(&rearstrip,rearstrip.Color(255,   0,   0)     , 35L); // Red
  startWipe(&rearstrip,rearstrip.Color(0,   0,   0)     , 20L); // Red
  startWipe(&rearstrip,rearstrip.Color(255,   0,   0)     , 10L); // Red
  
  
}
/*
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

  
}*/

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
        nextlenkung=checkrange(nextlint.toInt(),-90,90);
        
        String nextttlint=ptimetolenkung;
        stepstonextlenkung=checkrange(nextttlint.toInt(),-90,90);
       // Serial.println(nextlenkung);
        
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
          //startreverse();
          Serial.println("startrev");
        }
        
        drivemode=ndrvmd;
        
        
       // String nextttlint=ptimetolenkung;
        //stepstonextlenkung=checkrange(nextttlint.toInt(),-90,90);
       // Serial.println(nextlenkung);
        
      }
    }else if(bufstring.indexOf("lufspd:",1)!=-1){//Lueftergeschwindigkeit
      
    }else if(bufstring.indexOf("stp:",1)!=-1){//full stop
      
    }else if(bufstring.indexOf("dimsid:",1)!=-1){//Lenkung
      if(bufstring.indexOf(";",1)>bufstring.length()-2){
        //Serial.print("dimsid");
        String dimside=bufstring.substring(bufstring.indexOf("dimsid:",1)+7,bufstring.indexOf(",",1));
        String dodim=bufstring.substring(bufstring.indexOf(",",1)+1,bufstring.indexOf(";",1));
        //Serial.print(dimside);
        String dimsidelint=dimside;
        int dimsidein=dimsidelint.toInt();
        
        String dodimlint=dodim;
        int dodimin=dodimlint.toInt();

        
        dimfrontsidelight(&frontstrip,dimsidein,dodimin);
        
      }
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
