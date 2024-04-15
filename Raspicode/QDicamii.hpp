
#ifndef QDicamii_HPP_
#define QDicamii_HPP_




#include </usr/local/include/opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <tuple>

#include <future>
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
#include </home/pi/QDI_gen2/DDPMAPPER.h>
//#include </home/pi/QDI_gen2/ddpmapper.cpp>






class QDicamii {
	private:
		
		
		int tval;
		/*
		int f_dev; //Device file
		

		float dt; //Loop time (recalculated with each loop)

		struct timespec start,end; //Create a time structure

		bool _first_run = 1; //Variable for whether to set gyro angle to acceleration angle in compFilter*/
	public:
		QDicamii();
		std::tuple<float,float> getposition(float xc,float yc, float xd, float yd, float xb, float yb,float xa, float ya, float h, float b);
		float getdistance(float hight, float h);
		bool inrange(int value, int minv, int maxv);
		std::tuple<int,int> groupcolorvalue(cv::Mat frame, int xc, int yc, int searchwidthineachdir,int stepw);
		int colorvalue(cv::Mat frame, int xc, int yc, int searchwidthineachdir,int stepw, int color);
		//std::vector<std::array<int,2>> crawlforcorner(int xstart, int ystart, int corner, cv::Mat frame, int color);
		cv::Mat filterforgcolor(cv::Mat framein);
		cv::Mat filterforrcolor(cv::Mat framein);
		cv::Mat filterforbcolor(cv::Mat framein);
		
		std::array<std::vector <std::array<int,3>>,2> calculaterinterestingpoints(cv::Mat frame);
		std::array<std::vector <std::array<int,3>>,2> calculatercornerinterestingpoints(cv::Mat frame,int xstart, int ystart,int  size,int color);
		void startcams();
		void getfrontdistance(cv::Mat frame,int xstart, int ystart,int xstop,int miny,int maxydown, int stepdis,int jupdis,int smallstepdis,float* distance,int maxdetects, int cam,bool doextend);
		void getblockdistance(cv::Mat frame,int color,int xstart, int ystart,int xstop,int miny,int maxydown, int stepdis,int jupdis,int smallstepdis,std::vector <std::array <int,2>>* distanceobl,int maxdetects, int cam,bool doextend);

		
		void frontrangethread(int* avdisance,float* schraegfrontversatz, bool* stopbit,bool disp);
		void siderangerthread(float* leftdistance,float* rightdistance,float* leftfrontdistance,float* rightfrontdistance,float* divlenk,int* offsetwithspeed, bool* stopbit,bool disp);
		void ddrangethread(int cam,DDPMAPPER* mapper,bool* datacollected,std::vector <std::array<int,3>>*, bool* stopbit,bool disp);
		void dispthread(bool display, bool* stopbit);
		void capturethread(bool* stopbit);
		void blockthread(int* driveoffset,int* blockdetect,int* revdrive,bool* stopbit,bool disp);
		/*
		void printdata(std::string message);
		int readdata(std::string message);
		int readdata();*/
		
};
#endif	// QDicamii_H_

