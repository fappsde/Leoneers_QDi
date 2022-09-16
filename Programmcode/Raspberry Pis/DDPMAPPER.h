
#ifndef DDPMAPPER_H_
#define DDPMAPPER_H_

#include <Python.h>


#include </usr/local/include/opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <array>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <stdio.h>
//#include "/home/pi/Documents/VL53L5CX_Linux_driver_1.3.0/user/platform/platform.h"

extern "C"{
	
//#include "vl53l5cx_api.h"
#include "platform.h"
#include "platform.c"
//#include "vl53l5cx_api.c"
#ifndef VL53L5CX_API_H_
#define VL53L5CX_API_H_
#endif
#ifndef VL53L5CX_BUFFERS_H_
#define VL53L5CX_BUFFERS_H_
#endif
#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
//#include "vl53l5cx_buffers.h"

#include <thread>


}






	static PyObject  *pName,*pModule, *pDict, *pFunc;//


class DDPMAPPER {
	private:
	

		
		bool inrange(int x, int y, int z);
		int torange(int val, int min, int max);
		void setLidarsensorlow(int sensor);
		void setLidarsensorhigh(int sensor);
		
		
		/*int MPU6050_addr;
		int f_dev; //Device file
		

		float dt; //Loop time (recalculated with each loop)

		struct timespec start,end; //Create a time structure

		bool _first_run = 1; //Variable for whether to set gyro angle to acceleration angle in compFilter*/
	public:
		DDPMAPPER();
		void addvalue(double sensorxfromcarpos, double sensoryfromcarpos,double sensorzfromcarpos, double distance, double sensorangle, double subsensorx,int subsensory,int carposx, int carposy, double carrotz);
		void addvalueb(double sensorxfromcarpos, double sensoryfromcarpos,double sensorzfromcarpos, double distance, double sensorangle, double subsensorx,int subsensory,int carposx, int carposy, double carrotz);
		
		
		void addLidDistance(double sensorxfromcarpos, double sensoryfromcarpos, double distance, double angle);
		
		void setcarpostion(int carposx, int carposy);

		void setcarrotaotion(double carrotz);

		void onDestroy();
		void setupPythonInterface();
		
		void rangethread(int* frontdis,int* frontleftdis,int* frontrightdis, int* nextdir,bool* stopbit);
		void displaythread(bool* stopbit);
		
		int getlidarrange(int side,bool* stopbit);
		void startlidars();
		
		void displaylayer(int layer);
		
		void printlayer(int layer);
		void resetarrayfield();
		
		void estimaterol(double maxdistance, double winkelfromcarcenter, double offnwinkel,int* frontdis);
		
		void startranging(bool displ,bool* stopbit);
		//void stopranging();
		
};
#endif	// DDPMAPPER_H_

