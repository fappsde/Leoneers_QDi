




/*#include <Python.h>


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
	
#include "vl53l5cx_api.h"
#include "platform.h"
#include "platform.c"
#include "vl53l5cx_api.c"
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
#include "vl53l5cx_buffers.h"

#include <thread>


}*/
#include </home/pi/QDI_gen2/DDPMAPPER.h>



//normal layer: 15

std::array<std::array<std::array<int,600>,600>,30> ddparrayfield;
std::vector<std::array<int,4>> relevantpoints;


int rangex=600;
int rangey=600;
int rangez=30;

/*
int lidarsamount=4;
int lidarsides=2;
int startsideadd=0;*/
int ampx=2;
int sensoradd=1;

int carposxaxis=300;
int carposyaxis=300;
int carzrotation=0;

//VL53L5CX_Configuration Dev;
//std::array<std::array<VL53L5CX_Configuration,4>,3> sensorconfigs={Dev,Dev,Dev,Dev,Dev,Dev,Dev,Dev,Dev,Dev,Dev,Dev};
///side(1:front,2:left,3:right,4:rear);sensorxfromcarpos;sensoryfromcarpos;sensorzfromcarpos;sensorangle;orientation
double sensorpositionconfigs[3][4][6]=	{{{1.0,-2.2,-4.0,15.0,-77.5,1},{1.0,-0.7,-4.5,15.0,-55,1},{1.0,0.7,-4.5,15.0,-27.5,3},{1.0,2.2,-4.0,15.0,-5,3}},
										{{2.0,-7.0,9.3,15.0,-139,1},{2.0,-7.0,9.3,15.0,-117.5,3},{3.0,7.0,9.3,15.0,50,1},{3.0,7.0,9.3,15.0,22.5,3}},
										{{4.0,25.0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}}};
//VL53L5CX_Configuration *p_dev;





void DDPMAPPER::onDestroy(){
	Py_Finalize();
	
}


int DDPMAPPER::torange(int val, int min, int max){
	if(val<=min)return min;
	if (val>=max)return max;
	return val;
	
}

bool DDPMAPPER::inrange(int x, int y, int z){
	if(x<0||x>=rangex)return false;
	if(y<0||y>=rangey)return false;
	if(z<0||z>=rangez)return false;
	return true;	
}

void DDPMAPPER::estimaterol(double maxdistance, double winkelfromcarcenter, double offnwinkel, int* frontdis){
	
	
	//sensorxfromcarpos,sensoryfromcarpos,sensorzfromcarpos,(Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*i])/10, sensorangle, ((((i+1)-end)/step)*flipper)-shifter,3,300,300,90);
	
	double averagedistance=0;
	int disamount=0;
	//double farawayside;
				
	double sensorxfromcarpos=2.2;
	double sensoryfromcarpos=-4.0;
	double sensorzfromcarpos=15.0;
	double distance=maxdistance;
	//double sensorangle=-5.0;
	//double subsensorx=3.0;
	//int subsensory=-5;
	int carposx=300;
	int carposy=300;
	double carrotz=90.0;
	
	carrotz=(carrotz*M_PI)/180.0;
	//int xobsc=carposx;
	//int yobsc=carposy;
	int zobsc=sensorzfromcarpos;
	
	double pointsreached=0.0;
	
	
	//double offnwinkel=60.0;//suchwinkel
	
	
	//int obsize=0;
	
	int sensorposx=carposx+(cos(carrotz)*sensorxfromcarpos-sin(carrotz)*sensoryfromcarpos);
	int sensorposy=carposy+(sin(carrotz)*sensorxfromcarpos+cos(carrotz)*sensoryfromcarpos);
	//double absolutedistancefromrobsenter= sqrt(pow(sensorxfromcarpos,2)-(2*sensorxfromcarpos*sensoryfromcarpos)*cos(0.5*M_PI)+pow(sensoryfromcarpos,2));
	//std::cout <<absolutedistancefromrobsenter<<std::endl  ;
	
	double subsensorangle=/*(1*M_PI)-*/(((winkelfromcarcenter)*M_PI)/180.0);//sensorangle+(subsensorx-4.5)*(45.0/8.0))*M_PI)/180.0);//TODO: size
	//std::cout <<subsensorangle<<std::endl  ;
	
	
	double absolutdir=subsensorangle+carrotz;
	//std::cout <<absolutdir<<std::endl  ;
	//double distancefromrobcenter
	//double angleofobjectfromrobzero=acos(()/());
	//angleofobjectfromrobzero
	
	double xfromsensor=(distance*cos(absolutdir));//sin(180-(subsensorangle+90)))/sin(90);
	double yfromsensor=(distance*sin(absolutdir));//sin(subsensorangle))/sin(90);
	
	double xfromsensorlrange=(distance*cos(absolutdir+(((offnwinkel/2)*M_PI)/180.0)));//defines the search size
	double yfromsensorlrange=(distance*sin(absolutdir+(((offnwinkel/2)*M_PI)/180.0)));
	double xfromsensorrrange=(distance*cos(absolutdir-(((offnwinkel/2)*M_PI)/180.0)));
	double yfromsensorrrange=(distance*sin(absolutdir-(((offnwinkel/2)*M_PI)/180.0)));
		//std::cout <<xfromsensor<<yfromsensor<<std::endl  ;
	if(inrange(sensorposx+xfromsensor,sensorposy+yfromsensor,zobsc)){
		//ddparrayfield.at(zobsc).at(sensorposy+yfromsensor).at(sensorposx+xfromsensor)=100;
	}
	
	
	int freeorloc=-1;
	if(inrange(carposx,carposy,zobsc)){
		//ddparrayfield.at(zobsc).at(carposy).at(carposx)=50;
	}
	double lr=2;
	double ll=2;
	for(double i=0;(pointsreached<ll+lr+70)&&i<distance+2;i=i+0.5){
		
		//std::cout <<"pr"<< pointsreached <<"."<< ll+lr ;
		
		if(i<distance)freeorloc=0;
		if(i>=distance)freeorloc=100;
		
		double xn=(xfromsensor/distance*i)+sensorposx;
		double yn=(yfromsensor/distance*i)+sensorposy;
		if(inrange(xn,yn,zobsc)){
			//ddparrayfield.at(zobsc).at(yn).at(xn)=freeorloc;///TODO check previous value
		}
		//std::cout <<(yfromsensor/distance)*i+sensorposy<<","<<(xfromsensor/distance)*i+sensorposx<<std::endl;
		
		double xloc=xn-((xfromsensorlrange/distance*i)+sensorposx);
		double yloc=yn-((yfromsensorlrange/distance*i)+sensorposy);
		
		 ll=sqrt(pow(xloc,2)+pow(yloc,2));
		double xroc=xn-((xfromsensorrrange/distance*i)+sensorposx);
		double yroc=yn-((yfromsensorrrange/distance*i)+sensorposy);
		
		 lr=sqrt(pow(xroc,2)+pow(yroc,2));
		 
		//std::cout <<xloc<<","<<yloc<<"längequer"<<ll<<std::endl;
		//std::cout << ll;
		for(double l=0;l<ll;l=l+0.5){
			int pointx=(xloc/ll*l)+xn;
			int pointy=(yloc/ll*l)+yn;
			int pointz=zobsc;
			//std::cout << l;
			
			if(inrange(pointx,pointy,pointz)){
				//int prevloc=
				if((ddparrayfield.at(pointz).at(pointy).at(pointx))>20){//;//=freeorloc;
				
					pointsreached=pointsreached+(2/(ll));
					averagedistance=averagedistance+i;
	
					disamount++;
				}
				//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;
				//std::cout << ".. ";
				
			}
		}for(double r=0;r<lr;r=r+0.5){
			int pointx=(xroc/lr*r)+xn;
			int pointy=(yroc/lr*r)+yn;
			int pointz=zobsc;
			if(inrange(pointx,pointy,pointz)){
				///TODO check previous value
				if((ddparrayfield.at(pointz).at(pointy).at(pointx))>20){
				//std::cout << ". ";
					pointsreached=pointsreached+(2/(lr));
					averagedistance=averagedistance+i;
	
					disamount++;
				}
				//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;
			}
		}
		pointsreached=pointsreached*ll*0.1;//0.1
		
		
	}
	
	//std::cout << std::endl<< "dis "<<((averagedistance/disamount)-15)*2 <<std::endl;
	int v=int(((averagedistance/disamount)-15)*4);
	*frontdis= (v);
	
}









void DDPMAPPER::resetarrayfield(){
	for(int z=0;z<30;z++){//std::array<std::array<int,600>,600> ddspace:ddparrayfield){
		for(int y=0;y<600;y++){//std::array<int,600> dspace:ddspace){
			for(int x=0;x<600;x++){//int spaceval:dspace){
				ddparrayfield.at(z).at(y).at(x)=-1;
				//spaceval=0;
			}
		}
		
	}
	
	
	
}
void DDPMAPPER::printlayer(int layer){
	for(int y=0;y<600;y++){//std::array<int,600> dspace:ddspace){
			for(int x=0;x<600;x++){//int spaceval:dspace){
				//std::cout << ddparrayfield.at(layer).at(y).at(x) ;
				//spaceval=0;
			}
			//std::cout << std::endl;
		}
}



void DDPMAPPER::addvalue(double sensorxfromcarpos, double sensoryfromcarpos,double sensorzfromcarpos, double distance, double sensorangle, double subsensorx,int subsensory,int carposx, int carposy, double carrotz){
	
	carrotz=(carrotz*M_PI)/180.0;
	//int xobsc=carposx;
	//int yobsc=carposy;
	int zobsc=sensorzfromcarpos;
	//int obsize=0;
	
	int sensorposx=carposx+(cos(carrotz)*sensorxfromcarpos-sin(carrotz)*sensoryfromcarpos);
	int sensorposy=carposy+(sin(carrotz)*sensorxfromcarpos+cos(carrotz)*sensoryfromcarpos);
	//double absolutedistancefromrobsenter= sqrt(pow(sensorxfromcarpos,2)-(2*sensorxfromcarpos*sensoryfromcarpos)*cos(0.5*M_PI)+pow(sensoryfromcarpos,2));
	//std::cout <<absolutedistancefromrobsenter<<std::endl  ;
	
	double subsensorangle=/*(1*M_PI)-*/(((sensorangle+(subsensorx-4.5)*(45.0/8.0))*M_PI)/180.0);
	//std::cout <<subsensorangle<<std::endl  ;
	
	
	double absolutdir=subsensorangle+carrotz;
	//std::cout <<absolutdir<<std::endl  ;
	//double distancefromrobcenter
	//double angleofobjectfromrobzero=acos(()/());
	//angleofobjectfromrobzero
	
	double xfromsensor=(distance*cos(absolutdir));//sin(180-(subsensorangle+90)))/sin(90);
	double yfromsensor=(distance*sin(absolutdir));//sin(subsensorangle))/sin(90);
	
	double xfromsensorlrange=(distance*cos(absolutdir+(((45.0/8.0/1)*M_PI)/180.0)));
	double yfromsensorlrange=(distance*sin(absolutdir+(((45.0/8.0/1.5)*M_PI)/180.0)));
	double xfromsensorrrange=(distance*cos(absolutdir-(((45.0/8.0/1)*M_PI)/180.0)));
	double yfromsensorrrange=(distance*sin(absolutdir-(((45.0/8.0/2)*M_PI)/180.0)));// /2
		//std::cout <<xfromsensor<<yfromsensor<<std::endl  ;
	if(inrange(sensorposx+xfromsensor,sensorposy+yfromsensor,zobsc)){
		ddparrayfield.at(zobsc).at(sensorposy+yfromsensor).at(sensorposx+xfromsensor)=100;
	}
	
	
	int freeorloc=-1;
	if(inrange(carposx,carposy,zobsc)){
		ddparrayfield.at(zobsc).at(carposy).at(carposx)=50;
	}
	//ddparrayfield.at(zobsc).at(sensorposy).at(sensorposx+i)=0;
	for(double i=0;i<distance+2;i=i+0.5){
		
		if(i<distance)freeorloc=0;
		if(i>=distance)freeorloc=100;
		
		double xn=(xfromsensor/distance*i)+sensorposx;
		double yn=(yfromsensor/distance*i)+sensorposy;
		if(inrange(xn,yn,zobsc)){
			ddparrayfield.at(zobsc).at(yn).at(xn)=freeorloc;///TODO check previous value
		}
		//std::cout <<(yfromsensor/distance)*i+sensorposy<<","<<(xfromsensor/distance)*i+sensorposx<<std::endl;
		
		double xloc=xn-((xfromsensorlrange/distance*i)+sensorposx);
		double yloc=yn-((yfromsensorlrange/distance*i)+sensorposy);
		
		double ll=sqrt(pow(xloc,2)+pow(yloc,2));
		double xroc=xn-((xfromsensorrrange/distance*i)+sensorposx);
		double yroc=yn-((yfromsensorrrange/distance*i)+sensorposy);
		
		double lr=sqrt(pow(xroc,2)+pow(yroc,2));
		//std::cout <<xloc<<","<<yloc<<"längequer"<<ll<<std::endl;
		
		for(double l=0;l<ll;l=l+0.5){
			int pointx=(xloc/ll*l)+xn;
			int pointy=(yloc/ll*l)+yn;
			int pointz=zobsc;
			if(inrange(pointx,pointy,pointz)){
				int prevloc=ddparrayfield.at(pointz).at(pointy).at(pointx);
				if(prevloc==-1){ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;
					}///TODO check previous value
				else if(prevloc!=-1){
					if(freeorloc==0){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc-10,0,100);
					}else if(freeorloc==100){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc+20,0,100);
					}
					//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;///TODO check previous value
				}
			}
		}for(double r=0;r<lr;r=r+0.5){
			int pointx=(xroc/lr*r)+xn;
			int pointy=(yroc/lr*r)+yn;
			int pointz=zobsc;
			if(inrange(pointx,pointy,pointz)){
				int prevloc=ddparrayfield.at(pointz).at(pointy).at(pointx);
				if(prevloc==-1){ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;}///TODO check previous value
				else if(prevloc!=-1){
					if(freeorloc==0){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc-10,0,100);
					}else if(freeorloc==100){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc+20,0,100);
					}
					//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;///TODO check previous value
				}
				//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;///TODO check previous value
			}
		}
		
		
	}/*
	ddparrayfield.at(zobsc).at(sensorposy).at(sensorposx+distance)=100;
	
	double xn=(xfromsensor)+sensorposx;	double yn=(yfromsensor)+sensorposy;
	ddparrayfield.at(zobsc).at(yn).at(xn)=100;///TODO check previous value
	
	double xloc=xn-((xfromsensorlrange)+sensorposx);
	double yloc=yn-((yfromsensorlrange)+sensorposy);
		
	double ll=sqrt(pow(xloc,2)+pow(yloc,2));
	double xroc=xn-((xfromsensorrrange)+sensorposx);
	double yroc=yn-((yfromsensorrrange)+sensorposy);
		
	double lr=sqrt(pow(xroc,2)+pow(yroc,2));
	for(double l=0;l<ll;l=l+0.5){
		ddparrayfield.at(zobsc).at((yloc/ll*l)+yn).at((xloc/ll*l)+xn)=100;///TODO check previous value
	}for(double r=0;r<lr;r=r+0.5){
		ddparrayfield.at(zobsc).at((yroc/lr*r)+yn).at((xroc/lr*r)+xn)=100;///TODO check previous value
	}
	*/
	//calc relative pos from sensor
	
	
	//xobsc=xobsc+sensorxfromcarpos+carposx;
	
}
void DDPMAPPER::addvalueb(double sensorxfromcarpos, double sensoryfromcarpos,double sensorzfromcarpos, double distance, double sensorangle, double subsensorx,int subsensory,int carposx, int carposy, double carrotz){
	
	carrotz=(carrotz*M_PI)/180.0;
	//int xobsc=carposx;
	//int yobsc=carposy;
	int zobsc=sensorzfromcarpos;
	//int obsize=0;
	
	int sensorposx=carposx+(cos(carrotz)*sensorxfromcarpos-sin(carrotz)*sensoryfromcarpos);
	int sensorposy=carposy+(sin(carrotz)*sensorxfromcarpos+cos(carrotz)*sensoryfromcarpos);
	//double absolutedistancefromrobsenter= sqrt(pow(sensorxfromcarpos,2)-(2*sensorxfromcarpos*sensoryfromcarpos)*cos(0.5*M_PI)+pow(sensoryfromcarpos,2));
	//std::cout <<absolutedistancefromrobsenter<<std::endl  ;
	
	double subsensorangle=/*(1*M_PI)-*/(((sensorangle+(subsensorx-4.5)*(45.0/8.0))*M_PI)/180.0);
	//std::cout <<subsensorangle<<std::endl  ;
	
	
	double absolutdir=subsensorangle+carrotz;
	
	
	double xfromsensor=(distance*cos(absolutdir));//sin(180-(subsensorangle+90)))/sin(90);
	double yfromsensor=(distance*sin(absolutdir));//sin(subsensorangle))/sin(90);
	
	double xfromsensorlrange=(distance*cos(absolutdir+(((45.0/8.0/1)*M_PI)/180.0)));
	double yfromsensorlrange=(distance*sin(absolutdir+(((45.0/8.0/1.5)*M_PI)/180.0)));
	double xfromsensorrrange=(distance*cos(absolutdir-(((45.0/8.0/1)*M_PI)/180.0)));
	double yfromsensorrrange=(distance*sin(absolutdir-(((45.0/8.0/2)*M_PI)/180.0)));// /2
		//std::cout <<xfromsensor<<yfromsensor<<std::endl  ;
	if(inrange(sensorposx+xfromsensor,sensorposy+yfromsensor,zobsc)){
		ddparrayfield.at(zobsc).at(sensorposy+yfromsensor).at(sensorposx+xfromsensor)=100;
	}
	
	
	int freeorloc=-1;
	if(inrange(carposx,carposy,zobsc)){
		ddparrayfield.at(zobsc).at(carposy).at(carposx)=50;
	}
	//ddparrayfield.at(zobsc).at(sensorposy).at(sensorposx+i)=0;
	for(double i=0;i<distance+2;i=i+0.5){
		
		if(i<distance)freeorloc=0;
		if(i>=distance)freeorloc=100;
		
		double xn=(xfromsensor/distance*i)+sensorposx;
		double yn=(yfromsensor/distance*i)+sensorposy;
		if(inrange(xn,yn,zobsc)){
			ddparrayfield.at(zobsc).at(yn).at(xn)=freeorloc;///TODO check previous value
		}
		//std::cout <<(yfromsensor/distance)*i+sensorposy<<","<<(xfromsensor/distance)*i+sensorposx<<std::endl;
		
		double xloc=xn-((xfromsensorlrange/distance*i)+sensorposx);
		double yloc=yn-((yfromsensorlrange/distance*i)+sensorposy);
		
		double ll=sqrt(pow(xloc,2)+pow(yloc,2));
		double xroc=xn-((xfromsensorrrange/distance*i)+sensorposx);
		double yroc=yn-((yfromsensorrrange/distance*i)+sensorposy);
		
		double lr=sqrt(pow(xroc,2)+pow(yroc,2));
		//std::cout <<xloc<<","<<yloc<<"längequer"<<ll<<std::endl;
		
		for(double l=0;l<ll;l=l+0.5){
			int pointx=(xloc/ll*l)+xn;
			int pointy=(yloc/ll*l)+yn;
			int pointz=zobsc;
			if(inrange(pointx,pointy,pointz)){
				int prevloc=ddparrayfield.at(pointz).at(pointy).at(pointx);
				if(prevloc==-1){ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;
					}///TODO check previous value
				else if(prevloc!=-1){
					if(freeorloc==0){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc-10,0,100);
					}else if(freeorloc==100){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc+20,0,100);
					}
					//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;///TODO check previous value
				}
			}
		}for(double r=0;r<lr;r=r+0.5){
			int pointx=(xroc/lr*r)+xn;
			int pointy=(yroc/lr*r)+yn;
			int pointz=zobsc;
			if(inrange(pointx,pointy,pointz)){
				int prevloc=ddparrayfield.at(pointz).at(pointy).at(pointx);
				if(prevloc==-1){ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;}///TODO check previous value
				else if(prevloc!=-1){
					if(freeorloc==0){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc-10,0,100);
					}else if(freeorloc==100){
						ddparrayfield.at(pointz).at(pointy).at(pointx)=torange(prevloc+20,0,100);
					}
					//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;///TODO check previous value
				}
				//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;///TODO check previous value
			}
		}
		
		
	}
	
}

void DDPMAPPER::addLidDistance(double sensorxfromcarpos, double sensoryfromcarpos, double distance, double angle){
	//carrotz=(carrotz*M_PI)/180.0;
	
	int zobsc=15;
	
	
	
	
	int sensorposx=carposxaxis+(cos((carzrotation*M_PI)/180.0)*sensorxfromcarpos-sin((carzrotation*M_PI)/180.0)*sensoryfromcarpos);
	int sensorposy=carposyaxis+(sin((carzrotation*M_PI)/180.0)*sensorxfromcarpos+cos((carzrotation*M_PI)/180.0)*sensoryfromcarpos);
	
	
	
	
	double absolutdir=((angle+carzrotation)*M_PI)/180.0;
	
	
	double xfromsensor=(distance*cos(absolutdir));
	double yfromsensor=(distance*sin(absolutdir));
	
	/*double xfromsensorlrange=(distance*cos(absolutdir+(((45.0/8.0/1)*M_PI)/180.0)));
	double yfromsensorlrange=(distance*sin(absolutdir+(((45.0/8.0/1.5)*M_PI)/180.0)));
	double xfromsensorrrange=(distance*cos(absolutdir-(((45.0/8.0/1)*M_PI)/180.0)));
	double yfromsensorrrange=(distance*sin(absolutdir-(((45.0/8.0/2)*M_PI)/180.0)));// /2*/
		//std::cout <<xfromsensor<<yfromsensor<<std::endl  ;
	if(inrange(sensorposx+xfromsensor,sensorposy+yfromsensor,zobsc)){
		ddparrayfield.at(zobsc).at(sensorposy+yfromsensor).at(sensorposx+xfromsensor)=100;
	}
	
	
	int freeorloc=-1;
	/*if(inrange(carposx,carposy,zobsc)){
		ddparrayfield.at(zobsc).at(carposy).at(carposx)=50;
	}*/
	//ddparrayfield.at(zobsc).at(sensorposy).at(sensorposx+i)=0;
	for(double i=0;i<distance+2;i=i+0.5){
		
		if(i<distance)freeorloc=0;
		if(i>=distance)freeorloc=100;
		
		double xn=(xfromsensor/distance*i)+sensorposx;
		double yn=(yfromsensor/distance*i)+sensorposy;
		if(inrange(xn,yn,zobsc)){
			//ddparrayfield.at(zobsc).at(yn).at(xn)=freeorloc;///TODO check previous value
			int prevloc=ddparrayfield.at(zobsc).at(yn).at(xn);
			if(prevloc==-1){ddparrayfield.at(zobsc).at(yn).at(xn)=freeorloc;}//TODO check previous value
				else if(prevloc!=-1){
					if(freeorloc==0){
						ddparrayfield.at(zobsc).at(yn).at(xn)=torange(prevloc-10,0,100);
					}else if(freeorloc==100){
						ddparrayfield.at(zobsc).at(yn).at(xn)=torange(prevloc+20,0,100);
					}
					//ddparrayfield.at(pointz).at(pointy).at(pointx)=freeorloc;///TODO check previous value
				}
		}
		
		
	}
	
}
		
void DDPMAPPER::setcarpostion(int carposx, int carposy){
	carposxaxis=carposx+300;
	carposyaxis=carposy+300;
	
	}
	
void DDPMAPPER::setcarrotaotion( double carrotz){
	carzrotation=carrotz;

	
}

void DDPMAPPER::displaylayer(int layer){
	cv::Mat frame=cv::Mat(600,600,CV_8UC3);
	cv::Vec3b color = frame.at <cv::Vec3b>(cv::Point(0,0));
				//int r=color[2];
				//int g=color[1];
				//int b=color[0];
	cv::Vec3b gcolor=color;
	for(int x=0;x<600;x++){
			for(int y=0;y<600;y++){
				
				if(ddparrayfield.at(layer).at(y).at(x)==-1){
					gcolor[2]=0;//r
					gcolor[1]=255;//g
					gcolor[0]=0;//b
				}else{
					
					gcolor[2]=ddparrayfield.at(layer).at(y).at(x)*2.5;//r
					gcolor[1]=0;//g
					gcolor[0]=0;//b
				}
				frame.at <cv::Vec3b>(cv::Point(x,y))= gcolor;
				
				
			
				
			}
			
		}
		float zprot=(carzrotation*M_PI)/180.0;//0*(180/M_PI);//(180*M_PI)/180.0;carrotz=(carrotz*M_PI)/180.0
		
		gcolor[2]=0;//r
			gcolor[1]=0;//g
			gcolor[0]=255;//b
		
		for(int ix=-16;ix<=16;ix++){
			
			
			int xdis= (cos(zprot)*(carposxaxis-300+ix)-sin(zprot)*(carposyaxis-300+11))+300;
			int ydis= (sin(-zprot)*(carposxaxis-300+ix)-cos(-zprot)*(carposyaxis-300+11))+300;
			int xdiss= (cos(zprot)*(carposxaxis-300+ix)-sin(zprot)*(carposyaxis-300-11))+300;
			int ydiss= (sin(-zprot)*(carposxaxis-300+ix)-cos(-zprot)*(carposyaxis-300-11))+300;
			
			//carposx= carposx+(cos(zrot)*x-sin(zrot)*y);
			//carposy= carposy+(sin(zrot)*x-cos(zrot)*y);
			
			
			frame.at <cv::Vec3b>(cv::Point(torange(xdis,0,600),torange(ydis,0,600)))= gcolor;
			frame.at <cv::Vec3b>(cv::Point(torange(xdiss,0,600),torange(ydiss,0,600)))= gcolor;
		}
		for(int iy=-11;iy<=11;iy++){
			
			gcolor[2]=0;//r
			gcolor[1]=0;//g
			gcolor[0]=255;//b
			int xdis= (cos(zprot)*(carposxaxis-300+16)-sin(zprot)*(carposyaxis-300+iy))+300;
			int ydis= (sin(-zprot)*(carposxaxis-300+16)-cos(-zprot)*(carposyaxis-300+iy))+300;
			int xdiss= (cos(zprot)*(carposxaxis-300-16)-sin(zprot)*(carposyaxis-300+iy))+300;
			int ydiss= (sin(-zprot)*(carposxaxis-300-16)-cos(-zprot)*(carposyaxis-300+iy))+300;
			frame.at <cv::Vec3b>(cv::Point(torange(xdis,0,600),torange(ydis,0,600)))= gcolor;
			frame.at <cv::Vec3b>(cv::Point(torange(xdiss,0,600),torange(ydiss,0,600)))= gcolor;
		}
	cv::imshow("2dmap", frame);
	
}



void DDPMAPPER::displaythread(bool* stopbit){
	
	while(*stopbit)
	{
	displaylayer(15);
	cv::waitKey(50);
}
	
}

void DDPMAPPER::rangethread(int* frontdis,int* frontleftdis,int* frontrightdis, int* nextdir, bool* stopbit){
	
	while(*stopbit)
	{
		//Front distance
	estimaterol(200.0,-90,55.0,frontdis);
	//front right distance
	estimaterol(200.0,-53,25.0,frontrightdis);
	//Front left distance
	estimaterol(200.0,-128,25.0,frontleftdis );
	
	//drive left if left >right &&left >front-20
	if(*frontleftdis>*frontrightdis-20){//&&frontleftdis>frontdis-15){
		*nextdir=5;
		
	}
	else if(*frontrightdis>*frontleftdis-20){//&&frontrightdis>frontdis-15){
		*nextdir=-5;
		
	}else{
		*nextdir=0;
	}
	
	
	//drive right if right >left &&right >front-20
	
}
	
}

DDPMAPPER::DDPMAPPER(){
	//setupPythonInterface();
	resetarrayfield();
	
	
}

void DDPMAPPER::startranging(bool displ,bool* stopbit){
	/*std::thread lidarside(getlidarrange,1);
	std::thread lidarfront(DDPMAPPER::getlidarrange,0);
	std::thread lidarranger(DDPMAPPER::rangethread);
	if(displ){
		std::thread lidardisp(DDPMAPPER::displaythread);
		lidardisp.join();
		
	}
	
	
	
	lidarside.join();
	lidarfront.join();
	lidarranger.join();*/
}


