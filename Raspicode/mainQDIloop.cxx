
 
 
 ///g++ -Wall -pthread -o "%e" "%f" `pkg-config --cflags --libs opencv4   ` -L/home/pi/QDI_gen2/rplidar_sdk-master/output/Linux/Release/  -l:libsl_lidar_sdk.a -I/home/pi/QDI_gen2/rplidar_sdk-master/sdk/src/ -I/home/pi/QDI_gen2/rplidar_sdk-master/sdk/include/ -I/home/pi/QDI_gen2/rplidar_sdk-master/ -L/home/pi/QDI_gen2/rplidar_sdk-master/    -L/home/pi/QDI_gen2/rplidar_a2-master/build/ -lLIB_RPLIDAR -lpigpio -lrt -li2c  -I/usr/include/python3.9/ -lpython3.9 -I/home/pi/QDi/libi2c-master/  -L/home/pi/VL53L5CX_Linux_driver_1.3.0/ -I/home/pi/VL53L5CX_Linux_driver_1.3.0/user/uld-driver/inc/ -I/home/pi/VL53L5CX_Linux_driver_1.3.0/user/platform/ -I/home/pi/VL53L5CX_Linux_driver_1.3.0/user/uld-driver/src/ -pthread -I/home/pi/QDi/Adafruit-PWM-Servo-Driver-Library-master  -L/home/pi/QDi/Adafruit-PWM-Servo-Driver-Library-master  -I/home/pi/QDi/libi2c-master/libi2c  -L/home/pi/QDi/libi2c-master/src/i2c.o -I/home/pi/MPU6050-C-CPP-Library-for-Raspberry-Pi/ -L/home/pi/MPU6050-C-CPP-Library-for-Raspberry-Pi/  -lMPU6050 -L/home/pi/QDI_gen2/MPU6050-C-CPP-Library-for-Raspberry-Pi/  -lMPU6050G2 -L/home/pi/QDi/libi2c-master/src/ -li2c -L/home/pi/QDi/libi2c-master/ -li2c -l:libi2c.a -I/home/pi/rasp2esp/ -lRASPTOESP -lstdc++ -lpthread
 
 /**Grüne:1, links:lenk<0
  * Rot:2, rechst lenk>0**/

#define PI 3.14159265
#include <iostream>
#include </home/pi/rasp2esp/RASPTOESP.h>
//#include <stdlib.h>

//#include </home/pi/QDi/ddpmapper.cpp>
//#include </home/pi/QDi/DDPMAPPER.h>

#include </home/pi/QDI_gen2/QDicamii.hpp>
#include </home/pi/QDI_gen2/QDicamii.cxx>

#include </home/pi/QDI_gen2/DDlidarscanner.hpp>
#include </home/pi/QDI_gen2/DDlidarscanner.cpp>

//#include <Python.h>

#include <thread>

#include </home/pi/QDI_gen2/MPU6050-C-CPP-Library-for-Raspberry-Pi/IMUG2.hpp>
#include </home/pi/QDI_gen2/MPU6050-C-CPP-Library-for-Raspberry-Pi/IMUG2.cpp>

#include </home/pi/QDI_gen2/DDPMAPPER.h>
#include </home/pi/QDI_gen2/ddpmapper.cpp>

//MPU6050 imu(104,true);//0x68

float xrot=0;
float yrot=0;
float zrot=0;
float oax, oay, oaz, ogr, ogp, ogy;


float carposx=0;
float carposy=0;

bool stopbit=true;

int spd=0;
int ttspd=5;
int drct=0;
int ttdrct=3;

int frontdis=0;
int frontleftdis=0;
int frontrightdis=0;

int nextdir=0;

int blockdrivelenkoffset=0;
int blockdetect=0;

int prevdis=0;

float leftdistancecam=0;
float rightdistancecam=0;
float lenkdiv=0;

float leftfrontdistancecam=0;
float rightfrontdistancecam=0;

int lefdissum=0;
int rigdissum=0;
int frontdissum=0;

int dislefrigamsum=0;

float lenkboooster=1;
int stepstoboostzero=0;

float lenkversatzdurchfront=0;

int lowspeedtime=0;


int siderangerspeedoffset=0;//0 drive 0-10, 10->20spd

bool lowspeedfh=true;

int driverev=0;//0-> normal, 1rightreverse, -1 leftreverse, 2straightreverse


bool hinmodus=false;


//static PyObject *pName, *pModule, *pDict, *pFunc;

RASPTOESP device(0x55);
IMUG2 imu;

int sysstat=-1;

int finished=-1;//-1, 1 to disable, 2wait for start


int counttonextmessure=0;
		int lastw=0;
		int counttoback=0;



int torange(int val, int min, int max){
	if(val>=max)return max;
	if(val<=min)return min;
	return val;
	
}
float torfange(float val, float min, float max){
	if(val>=max)return max;
	if(val<=min)return min;
	return val;
	
}
void calculateraldirandadd(float x, float y){
	
	 carposx= carposx+(cos(zrot)*x-sin(zrot)*y);
	 carposy= carposy+(sin(zrot)*x-cos(zrot)*y);
	 
	 std::cout << " x:"<< carposx<<" y:"<<carposy<<"\n";
	
	
	
	
}

/*
void setupPythonInterface(){
	char program[]="/home/pi/QDi/buttondriver.py";
	FILE* fp;
	
	
	setenv("PYTHONPATH",".",1);
	
	//Py_DecodeLocale("pwmdriver",NULL);
	//Py_SetProgramName(program);
	//Ini python
	Py_Initialize();
	
	//run imports
	PyRun_SimpleString("import time ");
	
	//PyRun_SimpleString("import RPi.GPIO as GPIO");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");
	
	
	
	
	fp = fopen(program,"r");
	PyRun_SimpleFile(fp,program);
	
	
	
	pName = PyUnicode_FromString("buttondriver");//,encoding = 'ISO-8859-1');
	pModule = PyImport_Import(pName);
	pDict = PyModule_GetDict(pModule);
	//pFunc =PyDict_GetItemString(pDict,"setupLidar");
	
	//PyRun_SimpleString("print('Preparing')");
	
	//PyObject *pArgs, *pValue;
	//pArgs=PyTuple_New(1);
	
	//pValue=PyLong_FromLong(1);
	
	//PyTuple_SetItem(pArgs,0,pValue);
	
	
		//Call function
	// PyObject_CallObject(pFunc,pArgs);
	
	
	/pArgs=PyTuple_New(1);
	
		//Call function
	pFunc =PyDict_GetItemString(pDict,"setallLidarlow");
	pValue=PyLong_FromLong(1);
	PyTuple_SetItem(pArgs,0,pValue);
	PyObject_CallObject(pFunc,pArgs);*
	
}
bool getbuttonstate(){
	pFunc =PyDict_GetItemString(pDict,"getbuttonstate");
	PyObject *pArgs, *pValue;
	pArgs=PyTuple_New(1);
	
	pValue=PyLong_FromLong(1);
	
	PyTuple_SetItem(pArgs,0,pValue);
	PyObject* res=PyObject_CallObject(pFunc,pArgs);
	long result=PyLong_AsLong(res);
	if(result==1)return true;
	return false;
	
}*/



void startposcamconnection(){
	std::cout<<"ok1";
	char program[]="/home/pi/QDi_gen2/poscamthrd.py";
	FILE* fp;
	
	std::cout<<"ok2";
	
	setenv("PYTHONPATH",".",1);
	std::cout<<"ok3";
	
	//Py_DecodeLocale("pwmdriver",NULL);
	//Py_SetProgramName(program);
	//Ini python
	Py_Initialize();
	
	std::cout<<"ok4";
	
	//run imports
	PyRun_SimpleString("import time ");
	PyRun_SimpleString("import argparse ");
	PyRun_SimpleString("from pmw3901 import PMW3901, PAA5100, BG_CS_FRONT_BCM, BG_CS_BACK_BCM ");
	PyRun_SimpleString("import threading ");
	
	std::cout<<"ok5";

	
	//PyRun_SimpleString("import RPi.GPIO as GPIO");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");
	
	
	std::cout<<"ok6";
	
	
	fp = fopen(program,"r");
	//PyRun_SimpleFile(fp,program);
	
	
	std::cout<<"ok7";
	
	pName = PyUnicode_FromString("poscamthrd");//,encoding = 'ISO-8859-1');
	pModule = PyImport_Import(pName);
	pDict = PyModule_GetDict(pModule);
	
	std::cout<<"ok";
	
	
	
	
	
}
void poscamupdatethread(DDPMAPPER* mapper,bool* stopbit){
	
	while(*stopbit) {
		//std::cout<<"ok";
		pFunc =PyDict_GetItemString(pDict,"getpos");
		PyObject *pArgs, *pValue;
		pArgs=PyTuple_New(1);
		
		pValue=PyLong_FromLong(1);
		
		PyTuple_SetItem(pArgs,0,pValue);
		PyObject* res=PyObject_CallObject(pFunc,pArgs);
		//tuple <int,int> result=PyTuple_SetItem(res,2,res);
		//std::cout<<"ok";
		vector<float> data;
		if (PyTuple_Check(res)) {
			for(Py_ssize_t i = 0; i < PyTuple_Size(res); i++) {
				PyObject *value = PyTuple_GetItem(res, i);
				data.push_back( PyFloat_AsDouble(value) );
			}
		} else {
			if (PyList_Check(res)) {
				for(Py_ssize_t i = 0; i < PyList_Size(res); i++) {
					PyObject *value = PyList_GetItem(res, i);
					data.push_back( PyFloat_AsDouble(value) );
				}
			} else {
				std::cout<<res;
				//throw logic_error("Passed PyObject pointer was not a list or tuple!");
			}
		}
		//std::cout<<"res"<<data.size()<<" "<<res;
		
		if(data.size()>=2){
			//float x=data.at(0);
			//float y=data.at(1);
			//std::cout<<"res"<<x<<" "<<y<<std::endl;
			calculateraldirandadd(data.at(0),data.at(1));
		}
		mapper->setcarpostion( carposx/100,carposy/100);
		
		///TODO print value to mapper
		//calculateraldirandadd(data.at(0),data.at(1));
		//if(result==1)return true;
	}
	
}



void mainloooop(){
	
	//std::cout << std::endl<< "pk "<<frontdis;
	//wait for start signal
	while(true){
	
	
	
	//char ldata1[]="drcn:0,3;";
	//device.printdata(ldata1);
	
	/*
		if(prevdis>0){
			 if ((frontdis-52)<0){
				if ((frontdis-32)<0){
					spd=-90;
					ttspd=0;
					prevdis=-100;
					
					std::string spddt = "spd:" + std::to_string(spd) + ","+std::to_string(ttspd) +";";  
					device.printdata(spddt);
					WaitMs(&p_dev->platform, 100);
				}else{
					spd=12;
					ttspd=10;
					prevdis=frontdis;
				}
				
				
			}else{
				if((frontdis-112)<0){
					spd=10;
				}else{
					spd=frontdis-50;
				}
				ttspd=10;
				prevdis=frontdis;
			}
		}else{
			if ((frontdis-32)<0){
			spd=-30;
			ttspd=0;
			prevdis=-20;
		}else{
			
			ttspd=10;
			prevdis=prevdis+1;
		}
		}*/
	/*if(nextdir==0){
		drct=0;
	}else if(nextdir==-5){
		drct=-90;
	}
	else if(nextdir==5){
		drct=90;
	}*/
	if(lenkdiv>0){
		drct=(lenkdiv/2)*torfange(lenkboooster,1,6);//(((lenkdiv/2)*(lenkdiv/3))/8.5)*torfange(lenkboooster,1,5);
	}else{
		drct=(lenkdiv/2)*torfange(lenkboooster,1,6);//(((lenkdiv/2)*(lenkdiv/3)*-1)/8.5)*torfange(lenkboooster,1,5);
	}
	/*
	if(frontdis<80&&(drct>-60||drct<60)){
		drct=drct*5.5;
	}else if(frontdis<110&&(drct>-60||drct<60)){
		drct=drct*4.5;
	}else if(frontdis<105){
		drct=drct*3.5;
	}
	else if(frontdis<145){
		drct=drct*0.8;
	}
	else if(frontdis>140){
		drct=drct*0.7;
	}*/
	
	/**Lampen ausschalten**/
	lefdissum=lefdissum+leftdistancecam;
	rigdissum=rigdissum+rightdistancecam;
	frontdissum=frontdissum+frontdis;
	
	dislefrigamsum++;
	if(dislefrigamsum>20){
		
		if((frontdissum/dislefrigamsum)<50){
			std::string camdim = "dimsid:2,1;";  
			device.printdata(camdim);
			std::string camdim1 = "dimsid:1,1;";  
			device.printdata(camdim1);
			std::string camdim2 = "dimsid:0,1;";  
			device.printdata(camdim2);
		}else{
			std::string camdim = "dimsid:2,0;";  
			device.printdata(camdim);
			if((lefdissum/dislefrigamsum)<40){
			std::string camdim = "dimsid:1,1;";  
			device.printdata(camdim);
		}else{
			std::string camdim = "dimsid:1,0;";  
			device.printdata(camdim);
		}
		if((rigdissum/dislefrigamsum)<40){
			std::string camdim = "dimsid:0,1;";  
			device.printdata(camdim);
		}else{
			std::string camdim = "dimsid:0,0;";  
			device.printdata(camdim);
		}
		}
		dislefrigamsum=0;
		lefdissum=0;
		rigdissum=0;
		frontdissum=0;
	}
	
	
	/**Notlenkung**/
	/*
	if(leftdistancecam-5<frontdis&&leftdistancecam+5>frontdis){
		if(leftdistancecam>rightdistancecam){
			drct=95;
		}else{
			drct=-95;
		}
	}
	if(rightdistancecam-5<frontdis&&rightdistancecam+5>frontdis){
		if(leftdistancecam>rightdistancecam){
			drct=95;
		}else{
			drct=-95;
		}
	}*/
	//ttdrct=(rightdistancecam+frontdis+leftdistancecam)/90;
	
	//std::cout << std::endl<< "disp "<<frontdis <<"speed "<<spd<<" nexdir "<<nextdir << " left "<<frontleftdis <<" right "<<frontrightdis;
	int pmaxsp=14;
	if(lowspeedfh)pmaxsp=11;
	
	/**Geschwindigkeit bestimmen* 1: bei geringer distanz langsamer fahren, 2: kurven langsamer und vor kurve bremsen*/
	if ((frontdis-52)<0){
		if ((frontdis-32)<0){
			spd=-90;
			ttspd=0;
			std::string spddt = "spd:" + std::to_string(spd) + ","+std::to_string(ttspd) +";";  
			device.printdata(spddt);
			//WaitMs(&p_dev->platform, 100);
		}else{
			spd=11;
			ttspd=10;
		}
	}else{
		if(lowspeedtime>1){
			spd=11;
			lowspeedtime--;
		}else if(drct<-50||drct>50){
		spd=torange(11,10,pmaxsp);
		
	}else if(drct<-40||drct>40){
		spd=torange(12,10,pmaxsp);
		//lowspeedtime=50;
	}else if(drct<-30||drct>30){
		spd=torange(13,10,pmaxsp);
	}else if(drct<-10||drct>10){
		spd=torange(14,10,pmaxsp);
	}else{
		
		int divofsidesr=(rightfrontdistancecam/(rightdistancecam));
		int divofsidesl=(leftfrontdistancecam/(leftdistancecam));
		
		spd=torange(10+((divofsidesr+divofsidesl)/1.8),10,pmaxsp);//18 und max 16, *1.5
	}
	}
	siderangerspeedoffset=torange((spd)*3.5,0,100);//-9,, *3
	
	/**lenkung vor kurve verstärken**/
	float divofsidesr=(rightdistancecam*2/(rightfrontdistancecam));
	float divofsidesl=(leftdistancecam*2/(leftfrontdistancecam));
	if(divofsidesr>divofsidesl&&divofsidesr>1.0){
		lenkboooster=divofsidesr;
		stepstoboostzero=300;
	}
	if(divofsidesl>divofsidesr&&divofsidesl>1.0){
		lenkboooster=divofsidesl;
		stepstoboostzero=300;
	}
	if(stepstoboostzero>0){
		lenkboooster=(lenkboooster)-1/300;
		stepstoboostzero--;
	}else{
		lenkboooster=1;
	}
	
	/**lenkung mit unterschied bei linkem und rechtem crawler**/
	if(rightdistancecam-(rightfrontdistancecam)<0&&leftdistancecam-(leftfrontdistancecam)>0){
		int divv=torange((frontdis*-1)+120,0,25);
		if(divv<5){
			drct=drct+10;
		}else{
			drct=drct+90;
		}
		
	}
	if(rightdistancecam-(rightfrontdistancecam)>0&&leftdistancecam-(leftfrontdistancecam)<0){
		int divv=torange((frontdis*-1)+120,0,25);
		if(divv<5){
			drct=drct-10;
			//
		}else{
			drct=drct-90;
		}
	}
	
	int lenkversatzdurhfrontmessung=(lenkversatzdurchfront*torange((frontdis*-1)+80,0,35)/-30);
	if(lenkversatzdurhfrontmessung>4||lenkversatzdurhfrontmessung<-4){
		drct=drct+lenkversatzdurhfrontmessung;
		
	}
	
	
	/**TODOOO: akzuelle lenkeung mit einbeziehen**/
	if(hinmodus){
	if(drct<0){
		int lenkungdbande=torange((drct*drct*-1)/(blockdetect+1),-100,100);
		int lenkungdblock=torange(blockdrivelenkoffset/2,-130,130);//120,120
		drct=(lenkungdbande)+lenkungdblock;//100-100
		if(blockdetect!=0){
			if((lenkungdbande<-60&&lenkungdblock<-60&&(lenkungdbande-32<lenkungdblock&&lenkungdbande+23>lenkungdblock))||(lenkungdbande>60&&lenkungdblock>60&&(lenkungdbande-23<lenkungdblock&&lenkungdbande+23>lenkungdblock))){
			drct=drct/2;
			//std::cout << std::endl<< "half ";
			}
			
		}
	}else{
		int lenkungdbande=(torange((drct*drct)/(blockdetect+1),-100,100));
		int lenkungdblock=torange(blockdrivelenkoffset/2,-130,130);//120,120
		drct=(lenkungdbande)+lenkungdblock;//100-100
		if(blockdetect!=0){
			if((lenkungdbande<-60&&lenkungdblock<-60&&(lenkungdbande-23<lenkungdblock&&lenkungdbande+23>lenkungdblock))||(lenkungdbande>60&&lenkungdblock>60&&(lenkungdbande-23<lenkungdblock&&lenkungdbande+23>lenkungdblock))){
			drct=drct/2;
			//std::cout << std::endl<< "half ";
			}
			
		}
		//std::cout << std::endl<< "bande "<<std::to_string(lenkungdbande)<< "block "<<std::to_string(lenkungdblock)<<endl;
		//drct=+;//100-100
	}}else{
		
	}
	//drct=((drct)/(blockdetect+1))+(blockdrivelenkoffset);
	
	
	if(counttoback>6){
		if(zrot>40)driverev=1;
		else if(zrot<-40)driverev=-1;
		else driverev=2;
		
	}

	if(finished<1){
	if(driverev==0){
		std::string drmv = "drvm:1,0;";  
		//device.printdata(drmv);
		/**Send to esp**/
		if(finished==1){spd=0;}
		std::string spddt = "spd:" + std::to_string(spd) + ","+std::to_string(ttspd) +";";  
		//char sdpdata[]="spd:"+frontdis+",5;";
		device.printdata(spddt);
		std::string drcndt = "drcn:" + std::to_string(drct) + ","+std::to_string(ttdrct) +";";
		device.printdata(drcndt);
	}else if(driverev==2){
		std::string drcndt = "drcn:0,"+std::to_string(ttdrct) +";";
		device.printdata(drcndt);
		std::string spddt = "drvm:-1,0;";  
		device.printdata(spddt);
		cv::waitKey(500);
		spddt = "spd:-52,0;";  
		device.printdata(spddt);
		
		cv::waitKey(400);
		spddt = "spd:10,0;";  
		device.printdata(spddt);
		spddt = "drvm:1,0;";  
		device.printdata(spddt);
		cv::waitKey(200);
		driverev=0;
		counttoback=0;
	}else if(driverev==1){
		std::string drcndt = "drcn:-60,"+std::to_string(ttdrct) +";";
		device.printdata(drcndt);
		std::string spddt = "drvm:-1,0;";  
		device.printdata(spddt);
		cv::waitKey(500);
		 spddt = "spd:-52,0;";  
		device.printdata(spddt);
		cv::waitKey(400);
		 spddt = "spd:10,0;";  
		device.printdata(spddt);
		spddt = "drvm:1,0;";  
		device.printdata(spddt);
		cv::waitKey(200);
		driverev=0;
		counttoback=0;
	}else if(driverev==-1){
		std::string drcndt = "drcn:60,"+std::to_string(ttdrct) +";";
		device.printdata(drcndt);
		std::string  spddt = "drvm:-1,0;";  
		device.printdata(spddt);
		cv::waitKey(500);
		spddt = "spd:-52,0;";  
		device.printdata(spddt);
		
		cv::waitKey(400);
		 spddt = "spd:10,0;";  
		device.printdata(spddt);
		spddt = "drvm:1,0;";  
		device.printdata(spddt);
		cv::waitKey(200);
		driverev=0;
		counttoback=0;
	}
	}else{
		spd=0;
		std::string spddt = "spd:" + std::to_string(spd) + ","+std::to_string(ttspd) +";";  
		//char sdpdata[]="spd:"+frontdis+",5;";
		device.printdata(spddt);
	}
	
	
	
	
	//std::cout << std::endl<< "lenk "<<std::to_string(drct)<< "lenkvers "<<std::to_string(lenkversatzdurhfrontmessung)<< "lenkboos "<<std::to_string(lenkboooster)<<"leftdis: "<< std::to_string(leftdistancecam) <<"leftfdis: "<< std::to_string(leftfrontdistancecam)<<"dis: "<< std::to_string(frontdis)<<"spd: "<< std::to_string(spd)<<"     zrot: "<< std::to_string(zrot);
	//char c[str.size() + 1];
   //strcpy(c, str.c_str());
	
		//WaitMs(&p_dev->platform, 10);
		
		//std::cout << " z: " << zrot  << "\n";
		//std::cout << " drv: " <<std::to_string( driverev ) << "\n";
		
		if(zrot>360||zrot<-360){//360 für 3 runden
			finished=1;
			std::string drcndt = "sysstat:" + std::to_string(1) + ","+std::to_string(6) +";";
			device.printdata(drcndt);
		}
		
		
		if(finished<1){
		if(counttonextmessure>700){
			std::cout << " last"<< std::to_string( lastw ) << " ak"<< std::to_string( zrot )<<"\n";
			if(zrot-1<lastw&&zrot+1>lastw){
				//counttoback++;
				std::cout << " countplus"<< std::to_string( counttoback )<<"\n";
			}else{
				
			}
			
			lastw=zrot;
			counttonextmessure=0;
		}else{
			//counttonextmessure++;
		}
		}
		
		
	
}
}





int main(int argc, char **argv)
{
	
	
	cv::waitKey(1000);
	//setupPythonInterface();
	
	
	
	
	DDPMAPPER mapper;
	hinmodus=true;
	QDicamii qdicam;
	DDlidarscanner qdilid;
	
	//qdilid.setupLidar();
	//qdilid.startLidar();
	
	bool imuen=true;
	bool uposcam=true;
	
	bool stopbit=true;
	
	for(int pt;pt<10;pt++){
		std::string stat = "sysstat:" + std::to_string(201) + ","+std::to_string(201) +";";
		device.printdata(stat);
		cv::waitKey(1);
	}
	if(imuen==true){
		imu.initimu();
	}
	if(uposcam==true){
		startposcamconnection();
	}
		//0x68
		//sleep(1);
		
		std::thread gyth(&IMUG2::posthread,&imu,&mapper,&xrot,&yrot,&zrot,&imuen);
		//std::thread gythback(&IMU::gyrobackloop,&imu,&driverev,&zrot,&imuen,&stopbit);
	//}
	/*
	//std::thread imuinit(&MPU6050::getOffsets,&imu,&oax, &oay, &oaz, &ogr, &ogp, &ogy);
	for(int pt;pt<10;pt++){
		std::string stat = "sysstat:" + std::to_string(201) + ","+std::to_string(201) +";";
		device.printdata(stat);
		cv::waitKey(1);
	}
	//imuinit.join();
	std::cout << "Gyroscope R,P,Y: " << ogr << "," << ogp << "," << ogy << "\nAccelerometer X,Y,Z: " << oax << "," << oay << "," << oaz << "\n";

	imu.calc_yaw = true;
	while(true){
		float gr, gp, gy;
		imu.getGyroRaw(&gr, &gp, &gy);
		xrot=xrot+(round((gr - ogr) / 10.0 / 16.4) / 10.0);
		yrot=yrot+(round((gp - ogp) / 10.0 / 16.4) / 10.0);
		zrot=zrot+(round((gy - ogy) / 10.0 / 16.4) / 10.0);
	
		std::cout<<"   zrot: "<< std::to_string(zrot);
	}
	
	std::thread gyroth(gyrothread,&imu,&xrot, &yrot, &zrot, ogr, ogp, ogy,&stopbit);*/
	//initialization
	
	/*mapper.resetarrayfield();
	mapper.setupPythonInterface();
	mapper.startlidars();
	mapper.startranging(true,&stopbit);*/
	//mapper.resetarrayfield();
	//mapper.setupPythonInterface();
	
	bool displ=false;
	bool displ2=false;
	bool displ3=false;
	
	bool displdev=false;
	
	std::thread dispth(&QDicamii::dispthread,&qdicam,displ3,&stopbit);
	//std::thread blockith(&QDicamii::blockthread,&qdicam,&blockdrivelenkoffset,&blockdetect,&driverev,&stopbit,displ2);
	std::thread capth(&QDicamii::capturethread,&qdicam,&stopbit);
	//std::thread frontranger(&QDicamii::frontrangethread,&qdicam,&frontdis,&lenkversatzdurchfront,&stopbit,displ);
	//std::thread sideranger(&QDicamii::siderangerthread,&qdicam,&leftdistancecam,&rightdistancecam,&leftfrontdistancecam,&rightfrontdistancecam,&lenkdiv,&siderangerspeedoffset,&stopbit,displ);
	//std::vector <std::array<int,3>> disdata;
	//bool datacol=true;
	//std::thread mapth(&QDicamii::ddrangethread,&qdicam,0,&mapper,&datacol,&disdata,&stopbit,displdev);
	//std::thread mapthc1(&QDicamii::ddrangethread,&qdicam,1,&mapper,&datacol,&disdata,&stopbit,displdev);
	//std::thread mapthlidar(&DDlidarscanner::ddmapthread,&qdilid,&mapper,&stopbit,false);
	
	std::thread posth(poscamupdatethread,&mapper,&stopbit);
	std::thread lidardisp(&DDPMAPPER::displaythread,&mapper,&stopbit);
	
	std::thread mainth(mainloooop);
	
	/***TODO: check status of all systems**/
	std::string drcndt = "sysstat:" + std::to_string(1) + ","+std::to_string(5) +";";
	device.printdata(drcndt);
	
	
	/*bool startwait=true;
	while(startwait){
		int start=getbuttonstate();
		if(start==1){
			startwait=false;
			std::string drcndt = "sysstat:" + std::to_string(1) + ","+std::to_string(1) +";";
	device.printdata(drcndt);
			finished=0;
		}
		//std::cout << " but"<< std::to_string(  )<<"\n";
		
	}*/
	
	//capth.detach();
	//dispth.detach();
	
	//stopbit=false;
	dispth.join();
	capth.join();
	
	/*std::thread lidarside(&DDPMAPPER::getlidarrange,&mapper,1,&stopbit);
	std::thread lidarfront(&DDPMAPPER::getlidarrange,&mapper,0,&stopbit);
	std::thread lidarranger(&DDPMAPPER::rangethread,&mapper,&frontdis,&frontleftdis,&frontrightdis,&nextdir,&stopbit);
	
	if(displ){
		std::thread lidardisp(&DDPMAPPER::displaythread,&mapper,&stopbit);
		lidardisp.join();
		
	}
	
	
	
	lidarside.join();
	lidarfront.join();
	lidarranger.join();*/
	
	return 0;
}

