

#include </home/pi/QDI_gen2/QDicamii.hpp>


#define PI 3.14159265

using  namespace std;
using namespace cv;

//IUPM iupm;


int avfrontdis=0;

float leftdistance=0;
float rightdistance=0;


cv::VideoCapture capture0(0);
	cv::VideoCapture capture1(1);
	
	cv::Mat frame0n;
	cv::Mat frame1;
	cv::Mat frame0r;
	cv::Mat frame1r;
	cv::Mat frame0g;
	cv::Mat frame1g;
	

std::tuple<float,float> QDicamii::getposition(float xc,float yc, float xd, float yd, float xb, float yb,float xa, float ya, float h, float b){
	float epsylon=0.0;
	
	if(xd <= b/2){	
		 epsylon = 67 * ((b/2)-xc + ((xd-xc)/2))/b ;
	}else{
		
	 epsylon = 67 * ((b/2)-xd + ((xd-xc)/2))/b ;
		}
	
	float c = 11.0/(tan(((58.0*(ya-yd))/h)* PI / 180.0));
	
	
	return std::make_tuple(c,epsylon);
	
}

float QDicamii::getdistance(float hight, float h){
	//float epsylon=0.0;
	
	/*if(xd <= b/2){	
		 epsylon = 67 * ((b/2)-xc + ((xd-xc)/2))/b ;
	}else{
		
	 epsylon = 67 * ((b/2)-xd + ((xd-xc)/2))/b ;
		}*/
	
	float c = 11.0/(tan((((hight))/h)* PI / 180.0));
	
	
	return c;
	
}



bool QDicamii::inrange(int value, int minv, int maxv){
	if(value<minv){
		return false;
	}else if(value>maxv){
		return false;
	}else{
		return true;
	}
	
}

/**
 * 
 * 
 * color 0=both, 1=green, 2=red
 * */
tuple<int,int> QDicamii::groupcolorvalue(Mat frame, int xc, int yc, int searchwidthineachdir,int stepw){
	int avaragegreen=0;
	int avaragered=0;
	int amount=0;
	for(int x=xc-searchwidthineachdir;x<xc+searchwidthineachdir;x=x+stepw){
		if(inrange(x,1,frame.cols-1)){
			for(int y=yc-searchwidthineachdir;y<yc+searchwidthineachdir;y=y+stepw){
				if(inrange(y,1,frame.rows-1)){
				
				amount++;
								
				Vec3b colorp=frame.at <Vec3b>(Point(x,y));
				int gp=colorp[1]-(((colorp[2]/2)+(colorp[0]/2)));
				int rp=(colorp[2]-(((colorp[1]*1)+(colorp[0]*1))))*8;
				gp=gp*3-15;
				rp=rp*3-15;
				if(gp<=0)gp=0;
				if(rp<=0)rp=0;
				if(gp>=255)gp=255;
				if(rp>=255)rp=255;
				avaragegreen=avaragegreen+gp;
					avaragered=avaragered+rp;
				}
				
			}
		}
	}
		 
		
	
	return make_tuple(avaragegreen/amount,avaragered/amount);
}
/**
 * 
 * 
 * color 0=both, 1=green, 2=red, 3=black
 * */
int QDicamii::colorvalue(Mat frame, int xc, int yc, int searchwidthineachdir,int stepw, int color){
	int avaragec=0;
	int amount=0;
	for(int x=xc-searchwidthineachdir;x<xc+searchwidthineachdir;x=x+stepw){
		if(inrange(x,1,frame.cols-1)){
			for(int y=yc-searchwidthineachdir;y<yc+searchwidthineachdir;y=y+stepw){
				if(inrange(y,1,frame.rows-1)){
				
				amount++;
								
				Vec3b colorp=frame.at <Vec3b>(Point(x,y));
				int cp;
				if(color==2){
					cp=(colorp[2]-(((colorp[1]*0.7)+(colorp[0]*0.7))))*8;//cp=colorp[2]-(((colorp[1]/2)+(colorp[0]/2)));
					cp=cp*3-15;
					cp=(cp*-1)+255;
				}else if(color==1){
					cp=(colorp[1]-(((colorp[2]*0.5)+(colorp[0]*0.5))))*9;
					cp=cp*3-15;
					cp=(cp*-1)+255;
				}else if(color==3){
					cp=((colorp[1]-10)*2+(colorp[2]-10)*2+((colorp[0]-10)*2))/10;
					
				}
				
				if(cp<=0)cp=0;
				if(cp>=255)cp=255;
				avaragec=avaragec+cp;
				}
				
			}
		}
	}
		 
		
	
	return avaragec/amount;
}
/*
std::vector<array<int,2>> QDicamii::crawlforcorner(int xstart, int ystart, int corner, Mat frame, int color){

	
	ICDMU icdmu ;
	std::vector<array<int,2>> crawlline{0};
	crawlline=icdmu.runcrawli(xstart,ystart,corner,frame,color);
	if(crawlline.size()>10){
		std::size_t indexc1=0;
		while(indexc1< crawlline.size()){
		
			Vec3b ccolor = frame.at <Vec3b>(Point(0,0));
			ccolor[2]=255;
			ccolor[1]=255;
			ccolor[0]=0;
			frame.at <Vec3b>(Point(crawlline.at(indexc1)[0],crawlline.at(indexc1)[1]))= ccolor;
			indexc1++;
		}
	}
	
	
	return crawlline;
	
}*/
Mat QDicamii::filterforgcolor(Mat framein){
	static Mat frameout=framein;
	
		for(int x=0;x<framein.cols;x++){
			for(int y=0;y<framein.rows;y++){
				Vec3b color = framein.at <Vec3b>(Point(x,y));
				int pr=color[2];
				int pg=color[1];
				int pb=color[0];
				int r=0;
				int g=0;
				int b=0;
				//frame0n.at <Vec3b>(Point(x,y))= pcolor;
				g=(pg-(((pr*0.5)+(pb*0.5))))*9;
				g=g*3-15;
				g=(g*-1)+225;
				
				//if(g<=0)g=0;
				//r=(r-(((g*1.8)+(b/2))))*3;
				//if(r<=0)r=0;
				
				if(r<=0)r=0;
				if(r>=255)r=255;
				if(g<=0)g=0;
				if(g>=255)g=255;
				if(b<=0)b=0;
				if(b>=255)b=255;
				Vec3b gcolor=color;
				gcolor[2]=0;//r
				gcolor[1]=g;//g
				gcolor[0]=0;//b
				frameout.at <Vec3b>(Point(x,y))= gcolor;
				
				
			
				
			}
			
		}
	
	
return frameout;

}
Mat QDicamii::filterforrcolor(Mat framein){
	static Mat frameout=framein;
	
		for(int x=0;x<framein.cols;x++){
			for(int y=0;y<framein.rows;y++){
				Vec3b color = framein.at <Vec3b>(Point(x,y));
				int pr=color[2];
				int pg=color[1];
				int pb=color[0];
				int r=0;
				int g=0;
				int b=0;
				
				
				//filter red
				
				r=(pr-(((pg*0.7)+(pb*0.7))))*8;
				r=r*3-15;
				
				
				r=(r*-1)+255;
				
				
				
				
				if(r<=0)r=0;
				if(r>=255)r=255;
				if(g<=0)g=0;
				if(g>=255)g=255;
				if(b<=0)b=0;
				if(b>=255)b=255;
				Vec3b gcolor=color;
				gcolor[2]=r;//r
				gcolor[1]=g;//g
				gcolor[0]=b;//b
				frameout.at <Vec3b>(Point(x,y))= gcolor;
				
				
			
				
			}
			
		}
	
	
return frameout;

}
Mat QDicamii::filterforbcolor(Mat framein){
	static Mat frameout=framein;
	
		for(int x=0;x<framein.cols;x++){
			for(int y=0;y<framein.rows;y++){
				Vec3b color = framein.at <Vec3b>(Point(x,y));
				int pr=color[2];
				int pg=color[1];
				int pb=color[0];
				int r=0;
				int g=0;
				int b=0;
				
				
				//filter red
				
				pr=(r-(((g*1.8)+(b/2))))*3;
				pr=pr*3-15;
				
				
				//filter black out
				
				r=(pr-15)*2+(pg-15)*2+(pb-15)*2;
				g=(pr-15)*2+(pg-15)*2+(pb-15)*2;
				b=(pr-15)*2+(pg-15)*2+(pb-15)*2;
				
				
				if(r<=0)r=0;
				if(r>=255)r=255;
				if(g<=0)g=0;
				if(g>=255)g=255;
				if(b<=0)b=0;
				if(b>=255)b=255;
				Vec3b gcolor=color;
				gcolor[2]=r;//r
				gcolor[1]=g;//g
				gcolor[0]=b;//b
				frameout.at <Vec3b>(Point(x,y))= gcolor;
				
				
			
				
			}
			
		}
	
	
return frameout;

}



//vector <array<int,2>> inpointts;
array<std::vector <array<int,3>>,2> QDicamii::calculaterinterestingpoints(Mat frame){
	
	cv::Mat frameo=frame;
	//int inipoint[2] {0,0};
	array<std::vector <array<int,3>>,2> inpoints;
	//std::vector <array<int,3>> inpointsred{0};
	const int searchdis=12;//12
	int cornerdistance=2;
	
	
	array<array<array<int,2>,640/searchdis+2> ,3> rows;
	int row=0;
	int col=0;
	int offset=searchdis*2+cornerdistance;
	int topdis=0;
	int maxxfb=300;
//cout << "." << endl;

	for(int x=searchdis*2+cornerdistance+topdis;x<frame.cols-maxxfb;x=x+searchdis){
		//cout << ";" << endl;
		for(int y=searchdis*2+cornerdistance;y<frame.rows-(searchdis*2)-cornerdistance;y=y+searchdis){
			//cout << "." << x<<" "<<y;
			
			if(row==0){
				//cout << "r0" ;
				//int ingroupgreeninf__p;
				//int ingroupredinf__p;
				//tie(ingroupgreeninf__p,ingroupredinf__p) =iupm.getgroupcolorvalue( x+searchdis, y, 6,2,0);
				
				int ingroupgreeninf;
				int ingroupredinf;
				tie(ingroupgreeninf,ingroupredinf) =groupcolorvalue(frame, x+searchdis, y, 6,2);
				rows[row%3][col+1]={ingroupgreeninf,ingroupredinf};
				int ingroupgreeninfb;
				int ingroupredinfb;
				tie(ingroupgreeninfb,ingroupredinfb) =groupcolorvalue(frame, x+searchdis, y+searchdis, 6,2);
				rows[(row+1)%3][col+1]={ingroupgreeninfb,ingroupredinfb};
				if(col==0){
					int ingroupgreenc;
					int ingroupredc;
					tie(ingroupgreenc,ingroupredc) =groupcolorvalue(frame, x+searchdis, y, 6,2);
					rows[row%3][col]={ingroupgreenc,ingroupredc};
					int ingroupgreencb;
					int ingroupredcb;
					tie(ingroupgreencb,ingroupredcb) =groupcolorvalue(frame, x+searchdis, y+searchdis, 6,2);
					rows[(row+1)%3][col]={ingroupgreencb,ingroupredcb};
				}
			}
			if(row>=1){
				//cout << "r1" ;
				int ingroupgreeninfb;
				int ingroupredinfb;
				tie(ingroupgreeninfb,ingroupredinfb) =groupcolorvalue(frame, x+searchdis, y+searchdis, 6,2);
				rows[(row+1)%3][col+1]={ingroupgreeninfb,ingroupredinfb};
				
				
				for(int i=0;i<=1;i++){
					
				int rtl=rows[(row-1)%3][col-1][i];	int rt=rows[(row-1)%3][col][i];	int rtr=rows[(row-1)%3][col+1][i];
				int rcl=rows[(row)%3][col-1][i];	int rc=rows[(row)%3][col][i];	int rcr=rows[(row)%3][col+1][i];
				int rbl=rows[(row+1)%3][col-1][i];	int rb=rows[(row+1)%3][col][i];	int rbr=rows[(row+1)%3][col+1][i];
				
				
				if(col>=1){
					
					//Top left corner
				if(((((rc+rcr+rb+rbr)/5))>(((rbl+rcl+rtl)/3))+10)&&((((rc+rcr+rb+rbr)/5))>(((rtl+rt+rtr)/3))+10)&&(rc+rcr)/2>45&&(rb+rbr)/2>45){
					int ppx=row*searchdis+offset;
					int ppy=col*searchdis+offset;
					
					
					array<int,3> point{ppx,ppy,1};
					inpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=0;
						ccolor[1]=255;
						ccolor[0]=0;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;
					
				}
				
				//Bottom left
				if(((((rc+rcl+rb+rbl)/5))>(((rbr+rcr+rtr)/3))+10)&&((((rc+rcl+rb+rbl)/5))>(((rtr+rt+rtl)/3))+10)&&(rc+rcl)/2>45&&(rb+rbl)/2>45){
					int ppx=row*searchdis+offset;
					int ppy=col*searchdis+offset;
					array<int,3> point{ppx,ppy,2};
					inpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=255;
						ccolor[1]=255;
						ccolor[0]=0;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;
					
				}
				//Top left
				if(((((rc+rcr+rt+rtr)/5))>(((rtl+rcl+rbl)/3))+10)&&((((rc+rcr+rt+rtr)/5))>(((rbl+rb+rbr)/3))+10)&&(rc+rcr)/2>45&&(rt+rtr)/2>45){
					int ppx=row*searchdis+offset;
					int ppy=col*searchdis+offset;
					array<int,3> point{ppx,ppy,3};
					inpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=0;
						ccolor[1]=255;
						ccolor[0]=255;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;
					
				}
				//Top right
				if(((((rc+rcl+rt+rtl)/5))>(((rtr+rcr+rbr)/3))+10)&&((((rc+rcl+rt+rtl)/5))>(((rbr+rb+rbl)/3))+10)&&(rc+rcl)/2>45&&(rt+rtl)/2>45){
					int ppx=row*searchdis+offset;
					int ppy=col*searchdis+offset;
					array<int,3> point{ppx,ppy,4};
					inpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=255;
						ccolor[1]=255;
						ccolor[0]=255;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;
					
				}
			}
		}
			
				
			}
			
			
				
				
				col++;
				
			}
			col=0;
			row++;
		}
		//cv::imshow("Streame", frame);
	
	return inpoints;
	}


array<std::vector <array<int,3>>,2> QDicamii::calculatercornerinterestingpoints(Mat frame, int xstart, int ystart,int  size,int color){//TODO color
	
	cv::Mat frameo=frame;
	array<std::vector <array<int,3>>,2> coinpoints;
	
	const int searchdis=2;
	int cornerdistance=0;
	
	int aread=6;//6
	int areas=1;
	
	int difference=40;//8
					int minlighting=45;//25
	
	
	array<array<array<int,1>,640/searchdis+2> ,3> rows;
	int row=0;
	int col=0;
	int offset=searchdis*2+cornerdistance;
//cout << "." << endl;

	for(int x=searchdis*2+cornerdistance+xstart;x<searchdis*2+cornerdistance+xstart+size;x=x+searchdis){
		//cout << ";" << endl;
		if(inrange(x,1,frame.cols-1)){
		for(int y=searchdis*2+cornerdistance+ystart;y<searchdis*2+cornerdistance+ystart+size;y=y+searchdis){
			//cout << "." << x<<" "<<y;
			if(inrange(y,1,frame.rows-1)){
			
			if(row==0){
				//cout << "r0" ;
				//int ingroupgreeninf__p;
				//int ingroupredinf__p;
				//tie(ingroupgreeninf__p,ingroupredinf__p) =iupm.getgroupcolorvalue( x+searchdis, y, 6,2,0);
				
				//int ingroupgreeninf;
				//int ingroupredinf;
				int colorvaluec=colorvalue(frame, x+searchdis, y, aread,areas,color);
				//tie(ingroupgreeninf,ingroupredinf) =groupcolorvalue(frame, x+searchdis, y, aread,areas);//colorvalue
				rows[row%3][col+1]={colorvaluec};//{ingroupgreeninf,ingroupredinf};
				//int ingroupgreeninfb;
				//int ingroupredinfb;
				int colorvalueb=colorvalue(frame, x+searchdis, y+searchdis, aread,areas,color);
				//tie(ingroupgreeninfb,ingroupredinfb) =groupcolorvalue(frame, x+searchdis, y+searchdis, aread,areas);
				rows[(row+1)%3][col+1]={colorvalueb};//{ingroupgreeninfb,ingroupredinfb};
				if(col==0){
					//int ingroupgreenc;
					//int ingroupredc;
					int colorvaluec=colorvalue(frame, x+searchdis, y, aread,areas,color);
					//tie(ingroupgreenc,ingroupredc) =groupcolorvalue(frame, x+searchdis, y, aread,areas);
					rows[row%3][col]={colorvaluec};//{ingroupgreenc,ingroupredc};
					//int ingroupgreencb;
					//int ingroupredcb;
					int colorvalueb=colorvalue(frame, x+searchdis, y+searchdis, aread,areas,color);
					//tie(ingroupgreencb,ingroupredcb) =groupcolorvalue(frame, x+searchdis, y+searchdis, aread,areas);
					rows[(row+1)%3][col]={colorvalueb};//{ingroupgreencb,ingroupredcb};
				}
			}
			if(row>=1){
				//cout << "r1" ;
				//int ingroupgreeninfb;
				//int ingroupredinfb;
				int colorvalueb=colorvalue(frame, x+searchdis, y+searchdis, aread,areas,color);
				//tie(ingroupgreeninfb,ingroupredinfb) =groupcolorvalue(frame, x+searchdis, y+searchdis, aread,areas);
				rows[(row+1)%3][col+1]={colorvalueb};//{ingroupgreeninfb,ingroupredinfb};
				
				
				for(int i=0;i<=0;i++){
					if(col>=2){//&&col<((size)/searchdis)-1&&row>2&&row<((size)/searchdis)-1){
				int rtl=rows[(row-1)%3][col-1][i];	int rt=rows[(row-1)%3][col][i];	int rtr=rows[(row-1)%3][col+1][i];
				int rcl=rows[(row)%3][col-1][i];	int rc=rows[(row)%3][col][i];	int rcr=rows[(row)%3][col+1][i];
				int rbl=rows[(row+1)%3][col-1][i];	int rb=rows[(row+1)%3][col][i];	int rbr=rows[(row+1)%3][col+1][i];
				
				
				
					
					
					
					
					if(color==1){
						difference=4;//4
						minlighting=20;
					}else{
						difference=10;//4
						minlighting=20;
					}
					//Top left corner
				if(((((rc+rcr+rb+rbr)/5))>(((rbl+rcl+rtl)/3))+difference)&&((((rc+rcr+rb+rbr)/5))>(((rtl+rt+rtr)/3))+difference)&&(rc+rcr)/2>minlighting&&(rb+rbr)/2>minlighting){
					int ppx=x;
					int ppy=y;
					
					
					array<int,3> point{ppx,ppy,1};
					coinpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=0;
						ccolor[1]=255;
						ccolor[0]=0;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						/*frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;*/
					
				}
				//Bottom left
				if(((((rc+rcl+rb+rbl)/5))>(((rbr+rcr+rtr)/3))+difference)&&((((rc+rcl+rb+rbl)/5))>(((rtr+rt+rtl)/3))+difference)&&(rc+rcl)/2>minlighting&&(rb+rbl)/2>minlighting){
					int ppx=x;
					int ppy=y;
					array<int,3> point{ppx,ppy,2};
					coinpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=255;
						ccolor[1]=255;
						ccolor[0]=0;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						/*frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;*/
					
				}
				//Top left
				if(((((rc+rcr+rt+rtr)/5))>(((rtl+rcl+rbl)/3))+difference)&&((((rc+rcr+rt+rtr)/5))>(((rbl+rb+rbr)/3))+difference)&&(rc+rcr)/2>minlighting&&(rt+rtr)/2>minlighting){
					int ppx=x;
					int ppy=y;
					array<int,3> point{ppx,ppy,3};
					coinpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=0;
						ccolor[1]=255;
						ccolor[0]=255;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						/*frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;*/
					
				}
				//Top right
				if(((((rc+rcl+rt+rtl)/5))>(((rtr+rcr+rbr)/3))+difference)&&((((rc+rcl+rt+rtl)/5))>(((rbr+rb+rbl)/3))+difference)&&(rc+rcl)/2>minlighting&&(rt+rtl)/2>minlighting){
					int ppx=x;
					int ppy=y;
					array<int,3> point{ppx,ppy,4};
					coinpoints[i].push_back(point);
					Vec3b ccolor = frame.at <Vec3b>(Point(ppx,ppy));
						ccolor[2]=255;
						ccolor[1]=255;
						ccolor[0]=255;
						frame.at <Vec3b>(Point(ppx,ppy))= ccolor;
						/*frame.at <Vec3b>(Point(ppx,ppy+1))= ccolor;
						frame.at <Vec3b>(Point(ppx,ppy-1))= ccolor;
						frame.at <Vec3b>(Point(ppx+1,ppy))= ccolor;
						frame.at <Vec3b>(Point(ppx-1,ppy))= ccolor;*/
					
				}
			}
		}
		}
			
				
			}
			
			
				
				
				col++;
				
			}
		}
			col=0;
			row++;
		}
	
	return coinpoints;
	}
	
	
	
 void QDicamii::startcams(){
		
		capture0.set(CAP_PROP_FPS,25);
	capture1.set(CAP_PROP_FPS,25);
	capture0.set(CAP_PROP_OPEN_TIMEOUT_MSEC,1);
	capture1.set(CAP_PROP_OPEN_TIMEOUT_MSEC,1);
	capture0.set(CAP_PROP_READ_TIMEOUT_MSEC,1);
	capture1.set(CAP_PROP_READ_TIMEOUT_MSEC,1);
	
	capture0.set(CAP_PROP_BRIGHTNESS,50);
	capture1.set(CAP_PROP_BRIGHTNESS,50);
	
	
	if (!capture0.isOpened()) {
		cout << "E1c1" << endl;
		//Error
	}
	if (!capture1.isOpened()) {
		cout << "E1c2" << endl;
		//Error
	}
	//cv::namedWindow("Stream");
	cout << "P" << endl;
	
	capture0.setExceptionMode(true);
	capture1.setExceptionMode(false);
	}



void QDicamii::getfrontdistance(Mat frame,int xstart, int ystart,int xstop,int miny,int maxydown, int stepdis,int jupdis,int smallstepdis,float* distance,int maxdetects, int cam,bool doextend){
	cv::Mat pframedis=frame;
	int y=ystart;
	vector <int> distances;
	//cout << "inr" << std::to_string(inrange(10,1,frame.cols-1))<<" " << std::to_string(inrange(10,1,frame.rows-1))<< endl;
	bool lastvalid=false;
	bool extend=doextend;
	
	
	if(smallstepdis>0){
		
		for(int x=xstart;x<xstop;x=x+smallstepdis){
			if(inrange(x,1,frame.cols-1)){
				if(lastvalid){
					//cout << "cv" << std::to_string(colorvalue(frame, x, y, 6,2,3))<< endl;
				
					if(colorvalue(frame, x, y, 3,1,3)<30){// x,y, 8,2,3
						int toppix=0;
						int previnttop=colorvalue(frame, x, y, 1,1,3);
						for(int suby=y;suby>miny;suby--){
							int div=colorvalue(frame, x, suby, 1,1,3)-previnttop;
							previnttop=colorvalue(frame, x, suby, 1,1,3);
							if(div<5&&div>-5){
								toppix++;
								Vec3b ccolor = pframedis.at <Vec3b>(Point(x,suby));
								ccolor[2]=255;
								ccolor[1]=255;
								ccolor[0]=255;
								pframedis.at <Vec3b>(Point(x,suby))= ccolor;
							}else{
								suby=0;
							}
						}
						int botpix=0;
						int previntbot=colorvalue(frame, x, y+1, 1,1,3);
						for(int suby=y+1;suby<maxydown;suby=suby+1){
							int botdiv=colorvalue(frame, x, suby, 1,1,3)-previntbot;
							previntbot=colorvalue(frame, x, suby, 1,1,3);
							if(botdiv<5&&botdiv>-5){
								botpix++;
								Vec3b ccolor = pframedis.at <Vec3b>(Point(x,suby));
								ccolor[2]=255;
								ccolor[1]=255;
								ccolor[0]=255;
								pframedis.at <Vec3b>(Point(x,suby))= ccolor;
							}else{
								suby=maxydown+1;
							}
						}
						distances.push_back(botpix+toppix);
						//cout << "dis" << std::to_string(botpix+toppix)<< endl;
						x=x+(stepdis);
					}
				}else{
					x=x+(jupdis);
				}
				lastvalid=true;
			}else {
				lastvalid=false;
			}
			if (x+15>xstop&&distances.size()<=2&&extend){
				xstop=xstop+(150);
				extend=false;
			}
			if(int(distances.size())>=maxdetects){
				x=xstop+151;
			}
			
		}
	}else{
		for(int x=xstart;x>xstop;x=x-1){
			if(inrange(x,1,frame.cols-1)){
				if(lastvalid){
					//cout << "cv" << std::to_string(colorvalue(frame, x, y, 6,2,3))<< endl;
				
					if(colorvalue(frame, x, y, 3,1,3)<30){
						int toppix=0;
						int previnttop=colorvalue(frame, x, y, 1,1,3);
						for(int suby=y;suby>miny;suby--){
							int div=colorvalue(frame, x, suby, 1,1,3)-previnttop;
							previnttop=colorvalue(frame, x, suby, 1,1,3);
							if(div<5&&div>-5){
								toppix++;
								Vec3b ccolor = pframedis.at <Vec3b>(Point(x,suby));
								ccolor[2]=255;
								ccolor[1]=255;
								ccolor[0]=255;
								pframedis.at <Vec3b>(Point(x,suby))= ccolor;
							}else{
								suby=0;
							}
						}
						int botpix=0;
						int previntbot=colorvalue(frame, x, y+1, 1,1,3);
						for(int suby=y+1;suby<maxydown;suby=suby+1){
							int botdiv=colorvalue(frame, x, suby, 1,1,3)-previntbot;
							previntbot=colorvalue(frame, x, suby, 1,1,3);
							if(botdiv<5&&botdiv>-5){
								botpix++;
								Vec3b ccolor = pframedis.at <Vec3b>(Point(x,suby));
								ccolor[2]=255;
								ccolor[1]=255;
								ccolor[0]=255;
								pframedis.at <Vec3b>(Point(x,suby))= ccolor;
							}else{
								suby=maxydown+1;
							}
						}
						distances.push_back(botpix+toppix);
						//cout << "dis" << std::to_string(botpix+toppix)<< endl;
						x=x-(stepdis);
					}
				}else{
					x=x-(jupdis);
				}
				lastvalid=true;
			}else {
				lastvalid=false;
			}
			if (x-15<xstop&&distances.size()<=2&&extend){
				xstop=xstop-(150);
				extend=false;
			}
			if(int(distances.size())>=maxdetects){
				x=-(xstop+151);
			}
		}
		
	}
	int maxv=0;
	int minv=10000;
	int all=0;
	int ammv=0;
	if(distances.size()!=0){
	for(int val: distances){
			all=all+val;
			ammv++;
			
	}
	if(distances.size()>2){
		for(int val: distances){
			if(val>maxv)maxv=val;
			if(val<minv)minv=val;
			
		}
		all=all-maxv-minv;	
		ammv=ammv-2;	
		
	}
	float dis=all/ammv;
	cout << "getdis";
	//cout << "dis"<< std::to_string(cam)<<" " <<" " << std::to_string(dis)<< std::to_string(getdistance(dis,10.0))<< endl;
	*distance=dis;
	}
	
	
	Vec3b ccolor = pframedis.at <Vec3b>(Point(xstart,ystart));
						ccolor[2]=255;
						ccolor[1]=30;
						ccolor[0]=255;
						pframedis.at <Vec3b>(Point(xstart,ystart))= ccolor;
						pframedis.at <Vec3b>(Point(xstart,ystart+1))= ccolor;
						pframedis.at <Vec3b>(Point(xstart+1,ystart))= ccolor;
						pframedis.at <Vec3b>(Point(xstart-1,ystart))= ccolor;
						pframedis.at <Vec3b>(Point(xstart,ystart-1))= ccolor;
	
	
}


void QDicamii::getblockdistance(Mat frame,int color,int xstart, int ystart,int xstop,int miny,int maxydown, int stepdis,int jupdis,int smallstepdis,vector <array <int,2>>* distanceobl,int maxdetects, int cam,bool doextend){
	cv::Mat pframedis=frame;
	cv::Mat pframedata=frame.clone();
	int y=ystart;
	vector <array <int,2>> distances;
	//cout << "inr" << std::to_string(inrange(10,1,frame.cols-1))<<" " << std::to_string(inrange(10,1,frame.rows-1))<< endl;
	bool lastvalid=false;
	bool extend=doextend;
	
	std::vector< int> subdisvector;
	int pwinkelofobj=0;
	std::vector< int> subbreivector;
	
	int posscanminxdis=20;
	int posscanmaxxdis=300;
	
	bool scannedsidepos=false;
	
	
	if(smallstepdis>0){
		
		for(int x=xstart;x<xstop;x=x+smallstepdis){
			if(inrange(x,1,pframedata.cols-1)){
				//if(lastvalid){
					//cout << "cv" << std::to_string(colorvalue(frame, x, y, 6,2,3))<< endl;
					
				
					if(colorvalue(pframedata, x, y, 4,1,color)<45){
						
						
						
						int toppix=0;
						int previnttop=colorvalue(pframedata, x, y, 1,1,color);
						for(int suby=y;suby>miny;suby--){
							//int div=colorvalue(frame, x, suby, 1,1,color)-previnttop;
							previnttop=colorvalue(pframedata, x, suby, 1,1,color);
							//if(div<5&&div>-5){
								toppix++;
								Vec3b ccolor = pframedata.at <Vec3b>(Point(x,suby));
								ccolor[2]=255;
								ccolor[1]=255;
								ccolor[0]=255;
								pframedis.at <Vec3b>(Point(x,suby))= ccolor;
							//}else{
								//suby=0;
							//}
						}
						int botpix=0;
						int previntbot=colorvalue(pframedata, x, y+1, 1,1,color);
						for(int suby=y+1;suby<maxydown;suby++){
							int botdiv=colorvalue(pframedata, x, suby, 1,1,color)-previntbot;
							previntbot=colorvalue(pframedata, x, suby, 1,1,color);
							if(botdiv<10&&botdiv>-10){//8
								botpix++;
								Vec3b ccolor = pframedata.at <Vec3b>(Point(x,suby));
								ccolor[2]=255;
								ccolor[1]=255;
								ccolor[0]=255;
								pframedis.at <Vec3b>(Point(x,suby))= ccolor;
							}else{
								suby=maxydown+1;
							}
						}
						subdisvector.push_back(botpix+toppix);
						
						//lastvalid=false;
						/**breite des blocks messen um die position zu bestimmen*/
						if(lastvalid==false||scannedsidepos==false){
							for(int pss=y-(toppix);pss<(botpix)+y;pss++){
								if(inrange(pss,1,frame.rows-1)){
									if(colorvalue(frame, x-1, pss, 4,1,color)<55){
									int sidpix=0;
									int sidpixr=0;
									int sidpixam=0;
									int sidpixram=0;
									/**nach links suchen**/
									int previntsid=colorvalue(pframedata, x, pss, 1,1,color);
									for(int psx=x;psx>x-posscanminxdis;psx--){
										if(inrange(psx,1,pframedata.cols-1)){
											int siddiv=colorvalue(pframedata, psx, pss, 1,1,color)-previntsid;
											previntsid=colorvalue(pframedata, psx, pss, 1,1,color);
											if(siddiv<12&&siddiv>-12){//8
												sidpix=sidpix+psx;
												sidpixam++;
												Vec3b ccolor = pframedata.at <Vec3b>(Point(psx,pss));
												ccolor[2]=255;
												ccolor[1]=255;
												ccolor[0]=255;
												pframedis.at <Vec3b>(Point(psx,pss))= ccolor;
											}else{
												psx=x-posscanminxdis-1;
											}
										}
									}
									/**nach rechst suchen**/
									previntsid=colorvalue(pframedata, x+1, pss, 1,1,color);
									for(int psx=x+1;psx<x+posscanmaxxdis;psx++){
										if(inrange(psx,1,pframedata.cols-1)){
											int siddiv=colorvalue(pframedata, psx, pss, 1,1,color)-previntsid;
											previntsid=colorvalue(pframedata, psx, pss, 1,1,color);
											if(siddiv<8&&siddiv>-8){
												sidpixr=sidpixr+psx;
												sidpixram++;
												Vec3b ccolor = pframedata.at <Vec3b>(Point(psx,pss));
												ccolor[2]=255;
												ccolor[1]=255;
												ccolor[0]=255;
												pframedis.at <Vec3b>(Point(psx,pss))= ccolor;
											}else{
												psx=x+posscanmaxxdis-1;
											}
										}
									}
									/**mittleren pixel finden**/
									int lspos=0;
									if(sidpixam!=0){
										lspos=sidpix/sidpixam;
									}
									int rspos=0;
									if(sidpixram!=0){
										rspos=sidpixr/sidpixram;
									}
									subbreivector.push_back(lspos+rspos);
									pss=pss+(stepdis);								
								}
							}	
							
						}
						/*** position durch breite*/
						
						int maxval=0;
						int minval=10000;
						int allval=0;
						int ammval=0;
						if(subbreivector.size()!=0){
							scannedsidepos=true;
							for(int val: subbreivector){
								allval=allval+val;
								ammval++;
							}
						if(subbreivector.size()>2){
							for(int val: subbreivector){
							if(val>maxval)maxval=val;
							if(val<minval)minval=val;
						}
						allval=allval-maxval-minval;	
						ammval=ammval-2;	
						
						}
						float brei=0;
						if(ammval>0){
						brei=allval/ammval;
						}
						int offs=0;//-60
						int mul=1;
						int add=0;
						if(cam==1){
							offs=0;
							//mul=-1;
							//add=pframedata.cols+440;
						}
						int pos=offs+(brei)-add;
						if(brei==0){pos=98085530;}
						//cout << "pos:"<< std::to_string(cam)<<" " << std::to_string(pos)<<" " << endl;
						//TD save
						pwinkelofobj=pos;
						}
						
						
						
												
						}
						x=x+(stepdis);
						lastvalid=true;
					}else {
						if(subdisvector.size()>0){
							/**distanz berechnen und mit dem winkel speichern**/
							//distances.push_back(subdisvector);
							int maxv=0;
							int minv=10000;
							int all=0;
							int ammv=0;
							float dis=0;
							if(subdisvector.size()!=0){
								for(int val: subdisvector){
									all=all+val;
									ammv++;
							
								}
								if(subdisvector.size()>2){
									for(int val: subdisvector){
										if(val>maxv)maxv=val;
										if(val<minv)minv=val;
							
									}
									all=all-maxv-minv;	
								ammv=ammv-2;	
							
								}
								
								if(ammv>0){
									dis=all/ammv;
								}
								//cout << "dis"<< std::to_string(cam)<<" " <<" " <<  std::to_string(getdistance(dis,10.0))<< endl;
								//blockdistances.push_back(dis);
							}
							if(dis!=0&&pwinkelofobj!=98085530){
								array<int,2> valuepair ={getdistance(dis,10.0),pwinkelofobj};
								distances.push_back(valuepair);
							}
							
							pwinkelofobj=98085530;
							subdisvector.clear();
						}
						scannedsidepos=false;
						
						
						
					lastvalid=false;
					}
				}else{
					x=x+(jupdis);
				}
				//lastvalid=true;
			/*}else {
				distances.push_back(subdisvector);
				subdisvector.clear();
				lastvalid=false;
			}*/
			if (x+15>xstop&&distances.size()<=2&&extend){
				xstop=xstop+(150);
				extend=false;
			}
			if(int(distances.size())>=maxdetects){
				x=xstop+151;
			}
			
		}
		if(subdisvector.size()>0){
							/**distanz berechnen und mit dem winkel speichern**/
							//distances.push_back(subdisvector);
							int maxv=0;
							int minv=10000;
							int all=0;
							int ammv=0;
							float dis=0;
							if(subdisvector.size()!=0){
								for(int val: subdisvector){
									all=all+val;
									ammv++;
							
								}
								if(subdisvector.size()>2){
									for(int val: subdisvector){
										if(val>maxv)maxv=val;
										if(val<minv)minv=val;
							
									}
									all=all-maxv-minv;	
								ammv=ammv-2;	
							
								}
								
								if(ammv>0){
									dis=all/ammv;
								}
								cout << "dis"<< std::to_string(cam)<<" " <<pwinkelofobj<<" " <<  std::to_string(getdistance(dis,10.0))<< endl;
								//blockdistances.push_back(dis);
							}
							if(dis!=0&&pwinkelofobj!=98085530){
								array<int,2> valuepair ={getdistance(dis,10.0),pwinkelofobj};
								distances.push_back(valuepair);
							}
							
							pwinkelofobj=98085530;
							subdisvector.clear();
						}
	}
	//cout << "bloeckke"<< std::to_string(distances.size())<<endl;
		//vector<int> blockdistances;
	/*for(vector<int> subv:distances){
		
	}*/
	
	
	Vec3b ccolor = pframedis.at <Vec3b>(Point(xstart,ystart));
						ccolor[2]=255;
						ccolor[1]=30;
						ccolor[0]=255;
						pframedis.at <Vec3b>(Point(xstart,ystart))= ccolor;
						pframedis.at <Vec3b>(Point(xstart,ystart+1))= ccolor;
						pframedis.at <Vec3b>(Point(xstart+1,ystart))= ccolor;
						pframedis.at <Vec3b>(Point(xstart-1,ystart))= ccolor;
						pframedis.at <Vec3b>(Point(xstart,ystart-1))= ccolor;
	frame=pframedis.clone();
	
	*distanceobl=distances;
	
}



void QDicamii::frontrangethread(int* avdisance,float* schraegfrontversatz, bool* stopbit,bool disp){
	int c1acdis=0;
	while(stopbit) {
		if (!frame0n.empty()&&!frame1.empty()){	
			cv::Mat pframe0n=frame0n.clone();
			cv::Mat pframe1=frame1.clone();	
			
			float distancesc0=0;
			float distancesc1=0;
			
			getfrontdistance(pframe0n,50,50,200,35,210,20,12,1,&distancesc0,100,0,true);
			getfrontdistance(pframe1,pframe1.cols-50,70,pframe1.cols-150,65,210,20,12,-1,&distancesc1,100,1,true);
			float distancefront=0;
			if(distancesc0>0&&distancesc1>0){
				distancefront=getdistance(((distancesc0+distancesc1)/2),10.0);
			}else if(distancesc0>0){
				distancefront=getdistance((distancesc0),10.0);
			}else if(distancesc1>0){
				distancefront=getdistance((distancesc1),10.0);
			}
			//cout << "dis" << std::to_string(distancefront)<< endl;
			
			
			*schraegfrontversatz=distancesc1-distancesc0;
			*avdisance=distancefront;
			if(disp){
				cv::imshow("Streamfront0", pframe0n);
				cv::imshow("Streamfront1", pframe1);
				cv::imshow("Streamfrontb1", filterforbcolor(pframe1.clone()));
				cv::imshow("Streamfrontb0", filterforbcolor(pframe0n.clone()));
				cv::waitKey(2);
			}
			//getfrontdistance(pframe0n,50,50,200,20,&c1acdis,0);
		}
		
	
	}
	
	
	
	
}

void QDicamii::siderangerthread(float* leftdistance,float* rightdistance,float* leftfrontdistance,float* rightfrontdistance, float* divlenk,int* offsetwithspeed,bool* stopbit,bool disp){
	
	while(*stopbit) {
		if (!frame0n.empty()&&!frame1.empty()){	
			cv::Mat pframe0n=frame0n.clone();
			cv::Mat pframe1=frame1.clone();	
			
			float distancesc0right=0;
			float distancesc1left=0;
			getfrontdistance(pframe0n,pframe0n.cols-5,70,pframe0n.cols-110,45,470,5,2,-1,&distancesc0right,6,0,false);
			getfrontdistance(pframe0n,pframe0n.cols-50-(*offsetwithspeed*3),70,pframe0n.cols-140-(*offsetwithspeed*3),45,470,5,2,-1,rightfrontdistance,6,0,false);
			
			
			getfrontdistance(pframe1,5,75,110,65,470,5,2,1,&distancesc1left,6,1,false);
			getfrontdistance(pframe1,50+(*offsetwithspeed*3),75,140+(*offsetwithspeed*3),65,470,5,2,1,leftfrontdistance,6,1,false);
	
			*rightdistance=0;
			if(distancesc0right>0){
				*rightdistance=getdistance(distancesc0right,10.0);
			}
			*leftdistance=0;
			if(distancesc1left>0){
				*leftdistance=getdistance(distancesc1left,10.0);
			}
			
			float differencedis=(*leftdistance-*rightdistance)/2;
			*divlenk=differencedis;
			if(disp){
			cv::imshow("Streamlenk0", pframe0n);
			cv::imshow("Streamlenk1", pframe1);
			cv::waitKey(2);
		}
			
			//cout << "lenk l: " << std::to_string(*leftdistance)<< " div: "<< std::to_string(differencedis)<< " r: "<< std::to_string(*rightdistance)<< endl;
			
		}
	}
	
}
//val1: cam, val2: distance, val3: dir
void QDicamii::ddrangethread(int cam,DDPMAPPER* mapper,bool* datacollected,std::vector <std::array<int,3>>*, bool* stopbit,bool disp){
	int fr=0;
	int sideoffs=0;
	int mul=1;
	if(cam==0)  {
		sideoffs=-25;
		mul=-1;
	}else if(cam==1){
		sideoffs=-85;
		mul=-1;
	}
	while(*stopbit) {
		cv::Mat pframe0n=frame1.clone();
		if(cam==0)  {
			 pframe0n=frame0n.clone();
		}else if(cam==1){
			pframe0n=frame1.clone();
		}
		if (!pframe0n.empty()){	
			//cv::Mat pframe0n=frame0n.clone();
					
			float distancesc0right=0;
			int cols=pframe0n.cols;
			
			//Mat frame,int xstart, int ystart,int xstop,int miny,int maxydown, int stepdis,int jupdis,int smallstepdis,float* distance,int maxdetects, int cam,bool doextend
			//getfrontdistance(pframe0n,pframe0n.cols-5,70,pframe0n.cols-40,45,470,2,2,-1,&distancesc0right,5,0,false);
			//std::async(launch::async,&QDicamii::getfrontdistance,this,pframe0n,pframe0n.cols-5-0,70,pframe0n.cols-110-0,45,470,5,2,-1,&distancesc0right,6,0,false);
			/*std::async(launch::async,calculaterinterestingpoints,pframe0n);
			std::future<std::array<std::vector <std::array<int,3>>,2>> retinp_c0 = std::async(launch::async,&calculaterinterestingpoints,frame0n);
				std::thread rangerth(&QDicamii::getfrontdistance,this,pframe0n,pframe0n.cols-5-0,70,pframe0n.cols-110-0,45,470,5,2,-1,&distancesc0right,6,0,false);//getfrontdistance(pframe0n,pframe0n.cols-5-i,70,pframe0n.cols-110-i,45,470,5,2,-1,&distancesc0right,6,0,false);
			*/
			//rangerth.join();
			//getfrontdistance(pframe0n,pframe0n.cols-50-(*offsetwithspeed*3),70,pframe0n.cols-140-(*offsetwithspeed*3),45,470,5,2,-1,rightfrontdistance,6,0,false);
			for(int i=0; i<cols-20;i=i+35){
				//std::future<void> ft=std::async(launch::deferred,&QDicamii::getfrontdistance,this,pframe0n,pframe0n.cols-5-i,70,pframe0n.cols-40-i,45,470,5,2,-1,&distancesc0right,6,0,false);
				float val;
				getfrontdistance(pframe0n,pframe0n.cols-5-i,70,pframe0n.cols-40-i,45,470,2,2,-1,&val,5,0,false);
				//addvalue(double sensorxfromcarpos, double sensoryfromcarpos,double sensorzfromcarpos, double distance, double sensorangle, double subsensorx,int subsensory,int carposx, int carposy, double carrotz)
				mapper->addvalue(10,10,15,getdistance(val,10.0),sideoffs+(i/35)*mul, 1 ,3,300,300,90);

			//ft.get();
				//std::thread rangerth(&QDicamii::getfrontdistance,this,pframe0n,pframe0n.cols-5-i,70,pframe0n.cols-110-i,45,470,5,2,-1,&distancesc0right,6,0,false);//getfrontdistance(pframe0n,pframe0n.cols-5-i,70,pframe0n.cols-110-i,45,470,5,2,-1,&distancesc0right,6,0,false);
			}
			
			/*rightdistance=0;
			if(distancesc0right>0){
				*rightdistance=getdistance(distancesc0right,10.0);
			}*/
			
			cout << "nexfrm"<<fr << endl;
			fr++;
			
			if(disp){
			cv::imshow("Streamlenk0", pframe0n);
			
			cv::waitKey(2);
		}
			
			//cout << "lenk l: " << std::to_string(*leftdistance)<< " div: "<< std::to_string(differencedis)<< " r: "<< std::to_string(*rightdistance)<< endl;
			
		}
	}
	
	
	
}
void QDicamii::dispthread(bool display, bool* stopbit){
	while(*stopbit) {
		if (!frame0n.empty()&&!frame1.empty()){	
			
			
			cv::Mat pframe0n=frame0n.clone();
	cv::Mat pframe1=frame1.clone();		
	int c1acdis=0;
	float distancesc0=0;
			float distancesc1=0;
	
	
	//getfrontdistance(pframe0n,50,50,200,20,&c1acdis,0);
	//getfrontdistance(pframe1,pframe1.cols-150,70,pframe1.cols-50,20,&c1acdis,1);
	//getfrontdistance(pframe0n,10,50,200,20,&c1acdis);
	if(display)	{
		cv::imshow("Stream", pframe0n);
		
		cv::imshow("Streamg", filterforgcolor(pframe0n.clone()));
		cv::imshow("Stream2", pframe1);
		
		cv::imshow("Stream2r", filterforrcolor(pframe1.clone()));
		cv::waitKey(2);
	}
		
	} else {
		/*cout << "Cam Err" << endl;
		capture0.release();
		capture1.release();
		capture0.open(0);
		capture1.open(1);*/
	}
}
	
	
	
}

void QDicamii::capturethread(bool* stopbit){
	int i=0;
	
	while(*stopbit) {
		//cout << "P"<<i << endl;
		try{
			cv::Mat pframe0n;
			cv::Mat pframe1;	
			if((capture0.isOpened())&&(capture1.isOpened())){
				
				
			}else{
				cout << "Cam Errbb" << endl;
				capture0.open(0);
				capture1.open(1);
			}
			
			if (!(capture0.read(pframe0n)&&(capture1.read(pframe1))) ){
				cout << "E2" << endl;
				//Error
			}else{
				if (!pframe0n.empty()&&!pframe1.empty()){
					flip(pframe0n,frame0n,-1);
					flip(pframe1,frame1,-1);				
				}else{
					cout << "Cam Err" << endl;
					capture0.release();
					capture1.release();
					capture0.open(0);
					capture1.open(1);
				}	
			}
		}
		catch(const char* e){
			cout << e<< endl;
		}
		
		
	
		//cv::waitKey(1);
		i=i+1;
	}
	
}

void QDicamii::blockthread(int* driveoffset, int* blockdetect,int* revdrive,bool* stopbit,bool disp){
	while(*stopbit){
		if (!frame0n.empty()&&!frame1.empty()){	
			
			
			cv::Mat pframe0n=frame0n.clone();
	cv::Mat pframe1=frame1.clone();	
	std::array<std::vector <std::array<int,3>>,2> inpoints_c0;
	std::array<std::vector <std::array<int,3>>,2> inpoints_c1;
	
	//std::array<std::vector <std::array<int,3>>,2> retinp_c0 = QDicamii::calculaterinterestingpoints(pframe0n);
	//td::array<std::vector <std::array<int,3>>,2> retinp_c1 = QDicamii::calculaterinterestingpoints(frame1);
	//inpoints_c0 = retinp_c0.get();
	std::vector <std::array <int,2>> blockrr;
	std::vector <std::array <int,2>> blockrl;
	std::vector <std::array <int,2>> blockgr;
	std::vector <std::array <int,2>> blockgl;
		getblockdistance(pframe0n,2,10, 100,600,50,400, 4,3,5,&blockrr,10, 0,false);
		getblockdistance(pframe1,2,10, 100,600,50,400, 4,3,5,&blockrl,10, 1,false);

		getblockdistance(pframe0n,1,10, 100,600,50,400, 4,3,5,&blockgr,10, 0,false);
		getblockdistance(pframe1,1,10, 100,600,50,400, 4,3,5,&blockgl,10, 1,false);
		
		int nearestdis=100000;
		int dirofnear=0;
		int colofnear=-1;
		int cam=-1;
		
		for(std::array <int,2> valrr:blockrr){
			if(valrr[0]<nearestdis){nearestdis=valrr[0];dirofnear=valrr[1];colofnear=2;cam=0;}
			
		}
		for(std::array <int,2> valrl:blockrl){
			if(valrl[0]<nearestdis){nearestdis=valrl[0];dirofnear=valrl[1];colofnear=2;cam=1;}
			
		}
		for(std::array <int,2> valgr:blockgr){
			if(valgr[0]<nearestdis){nearestdis=valgr[0];dirofnear=valgr[1];colofnear=1;cam=0;}
			
		}
		for(std::array <int,2> valgl:blockgl){
			if(valgl[0]<nearestdis){nearestdis=valgl[0];dirofnear=valgl[1];colofnear=1;cam=1;}
			
		}
		
		
		int ndir=0;
		int pdec=1;
		if(cam==0){//rechte cam
			
			if(colofnear==2){//rot
				ndir=-900;
				
			}else if(colofnear==1){//grün
				ndir=(dirofnear-750)*-1;//450   //650
				if(nearestdis<40){
					ndir=ndir+20;
				}
				
			}
			
		}else if(cam==1){//linke cam
			
			if(colofnear==2){//rot
				ndir=(dirofnear-50)*-1;
				if(nearestdis<40){
					ndir=ndir-20;
				}
				
			}else if(colofnear==1){//grün
				ndir=900;
				
				
			}
			
		}else{
			pdec=0;
		}
			
			
			/**
			 * rueckwärts, wenn 1. objekt näher als 40 und falsche seite
			 * 2, rand der falschen kamera
			 * gerade vor**/
			 
			 int pdrv=0;
			 if(nearestdis<40){
				 if(colofnear==2&&cam==0){
					 pdrv=-1;
					 
				 }
				 if(colofnear==1&&cam==1){
					 pdrv=1;
					 
				 }
				 
			 }
			*revdrive=pdrv;
		/*
		if(colofnear==2){
			ndir=dirofnear-250;
		}
		if(colofnear==1){
			ndir=dirofnear+250;
		}
		dirofnear=dirofnear/1.5;
		if(colofnear==2&&dirofnear>0){
			ndir=-90;
		}
		if(colofnear==1&&dirofnear>0){
			ndir=-90;
		}*/
		/*
		if(colofnear!=-1){
		if(colofnear==1){
			dirofnear=dirofnear+40;
		}
		if(colofnear==2){
			dirofnear=dirofnear-40;
		}
		
		if(colofnear==2&&dirofnear>0){
			dirofnear=dirofnear*-1;
		}
		if(colofnear==1&&dirofnear<0){
			dirofnear=dirofnear*-1;
		}
		if(colofnear==2&&dirofnear<0){
			dirofnear=dirofnear/2;
		}
		if(colofnear==1&&dirofnear>0){
			dirofnear=dirofnear/2;
		}
		if(dirofnear<50&&dirofnear>-50){
			cv::waitKey(500);
		}
		}*/
		
	
		/*if(colofnear==1){
			if(dirofnear>260){
				dirofnear=50;
			}else if(dirofnear<220){
				dirofnear=-90;
			}else{
				dirofnear=0;
			}
		}
		if(colofnear==2){
			dirofnear=0;
		}*/
		*driveoffset=ndir;
		*blockdetect=pdec;
		
		//cout << "Dis: "<<std::to_string(nearestdis) <<"dir: "<<std::to_string(dirofnear) <<"col: "<<std::to_string(colofnear) << endl;
			
				if(disp)	{
				cv::imshow("Streamblock", pframe0n);
				cv::imshow("Streamblock1", pframe1);
				cv::imshow("Streamg0", filterforgcolor(pframe0n.clone()));
				cv::imshow("Streamg1", filterforgcolor(pframe1.clone()));
				cv::waitKey(2);
				}
			}
		}
	
}


QDicamii::QDicamii(){
	startcams();
	
	
}

/*
int main(int argc, char **argv)
{
	
	startcams();
	
	
	
	
	
	
	
	
	
 
	int i=0;
	
	auto start = std::chrono::system_clock::now();
	bool stopbit=true;
	std::thread dispth(&QDicamii::dispthread,true,&stopbit);
	std::thread capth(&QDicamii::capturethread,&stopbit);
	std::thread frontranger(&QDicamii::frontrangethread,&avfrontdis,&stopbit);
	std::thread sideranger(&QDicamii::siderangerthread,&leftdistance,&rightdistance,&stopbit);
	
	//capth.detach();
	//dispth.detach();
	
	stopbit=false;
	dispth.join();
	capth.join();
	/*
	while(false) {
		cout << "P"<<i << endl;
		
		try{
			//if((&capture0!=nullptr)&&(&capture1!=nullptr)&&
			if((capture0.isOpened())&&(capture1.isOpened())){
				
			}else{
				cout << "Cam Errbb" << endl;
				capture0.open(0);
				capture1.open(1);
			}
			
		if (!(capture0.read(frame0n)&&(capture1.read(frame1))) ){
			cout << "E2" << endl;
			//Error
		}else{
			if (!frame0n.empty()&&!frame1.empty()){
				//Size size = frame1.size();
				//cout << "w" << size.height<<endl;
				
				flip(frame0n,frame0n,-1);
				flip(frame1,frame1,-1);
				
				/  *for(int x=0;x<frame0n.cols;x++){
					for(int y=0;y<frame0n.rows-240;y++){
					Vec3b color = frame0n.at <Vec3b>(Point(x,y));
				
					Vec3b gcolor=color;
					gcolor[2]=0;//r
					gcolor[1]=0;//g
					gcolor[0]=0;//b
					frame0n.at <Vec3b>(Point(x,y))= gcolor;
				}
				}
				for(int x=0;x<frame1.cols;x++){
					for(int y=0;y<frame1.rows-230;y++){
					Vec3b color = frame1.at <Vec3b>(Point(x,y));
				
					Vec3b gcolor=color;
					gcolor[2]=0;//r
					gcolor[1]=0;//g
					gcolor[0]=0;//b
					frame1.at <Vec3b>(Point(x,y))= gcolor;
				}
				}*
			
		
				
				//iupm.setupIUPM(frame0n,0);
				//iupm.setupIUPM(frame1,1);
				vector <array<int,2>> inpoigreenc0;
				std::array<std::vector <std::array<int,3>>,2> inpoints_c0;
				std::array<std::vector <std::array<int,3>>,2> inpoints_c1;
				//tie(inpoigreenc0,inpoiredc0) = 
				//iupm.getam();
				
				//IUPM iupmb;
				
				//inpoints_c1=calculaterinterestingpoints(frame1,iupm);
				//inpoints_c0=calculaterinterestingpoints(frame0n,iupm);
				
				
				std::future<std::array<std::vector <std::array<int,3>>,2>> retinp_c0 = std::async(launch::async,&calculaterinterestingpoints,frame0n,iupm);
				std::future<std::array<std::vector <std::array<int,3>>,2>> retinp_c1 = std::async(launch::async,&calculaterinterestingpoints,frame1,iupm);
				 inpoints_c0 = retinp_c0.get();
				 
				//iupm.getam();
				*
				 * 
				 * 
				 * std::thread lidarside(getlidarrange,1);
	std::thread lidarfront(getlidarrange,0);
	std::thread lidardisp(displaythread);
	lidardisp.join();
	lidarside.join();
	lidarfront.join();*
				
			
			//iupm.getam();
			int r=0;
			int g=0;
			
				for(int ic0=0;ic0<2;ic0++){
				std::vector<std::future<std::array<std::vector <std::array<int,3>>,2>>>  retinpdets_c0;
				if(ic0==0){
					r=inpoints_c0[ic0].size();
				}else{
					g=inpoints_c0[ic0].size();
				}
					std::size_t indexc0=0;
					while(indexc0< inpoints_c0[ic0].size()){
						
						retinpdets_c0.push_back(  std::async(launch::async,&calculatercornerinterestingpoints,frame0n,iupm,inpoints_c0[ic0].at(indexc0)[0]-20,inpoints_c0[ic0].at(indexc0)[1]-20,40,ic0+1));
						
						
						std::array<std::vector <std::array<int,3>>,2> incornerpoints_c0;
						incornerpoints_c0=retinpdets_c0.at(indexc0).get();
						
						if(incornerpoints_c0[0].size()>15){
							int avx=0;
							int avy=0;
							int countam=0;
							for(int ipos=0;ipos<int(incornerpoints_c0[0].size());ipos++){
								avx=avx+incornerpoints_c0[0].at(ipos)[0];
								avy=avy+incornerpoints_c0[0].at(ipos)[1];
								countam++;
							}
							avx=avx/countam;
							avy=avy/countam;
							
							Vec3b color = frame0n.at <Vec3b>(Point(avx,avy));
				
							Vec3b gcolor=color;
							gcolor[2]=220;//r
							gcolor[1]=0;//g
							gcolor[0]=220;//b
							for(int pix=-5;pix<5;pix++){
								frame0n.at <Vec3b>(Point(avx+pix,avy))= gcolor;
							}
							for(int pix=-5;pix<5;pix++){
								frame0n.at <Vec3b>(Point(avx,avy+pix))= gcolor;
							}
						
						}
						
						//retinp_c0.get();
						//calculatercornerinterestingpoints(frame0n,iupm,inpoints_c0[ic0].at(indexc0)[0]-20,inpoints_c0[ic0].at(indexc0)[1]-20,40);
						//int xstart, int ystart, int corner, Mat frame, int color
						//Mat frame, IUPM pIUPM,int xstart, int ystart,int  size
						//crawlforcorner(inpoints_c0[ic0].at(indexc0)[0],inpoints_c0[ic0].at(indexc0)[1],inpoints_c0[ic0].at(indexc0)[2],frame0n,ic0+1);
						indexc0++;
					}
				}
				
						std::vector <std::array<int,3>> corner_c1;
				cout << "dir gr"<< std::to_string(g>r)<< endl;
				inpoints_c1 = retinp_c1.get();
				for(int ic1=0;ic1<2;ic1++){
					std::vector<std::future<std::array<std::vector <std::array<int,3>>,2>>>  retinpdets_c1;
					std::size_t indexc1=0;
					while(indexc1< inpoints_c1[ic1].size()){
						retinpdets_c1.push_back(  std::async(launch::async,&calculatercornerinterestingpoints,frame1,iupm,inpoints_c1[ic1].at(indexc1)[0]-20,inpoints_c1[ic1].at(indexc1)[1]-20,40,ic1+1));
						
						std::array<std::vector <std::array<int,3>>,2> incornerpoints_c1;
						incornerpoints_c1=retinpdets_c1.at(indexc1).get();
						
						
						if(incornerpoints_c1[0].size()>15){
							int avx_c1=0;
							int avy_c1=0;
							int countam_c1=0;
							for(int ipos_c1=0;ipos_c1<int(incornerpoints_c1[0].size());ipos_c1++){
								avx_c1=avx_c1+incornerpoints_c1[0].at(ipos_c1)[0];
								avy_c1=avy_c1+incornerpoints_c1[0].at(ipos_c1)[1];
								countam_c1++;
							}
							avx_c1=avx_c1/countam_c1;
							avy_c1=avy_c1/countam_c1;
							
							array<int,3> point{avx_c1,avy_c1,0};
							corner_c1.push_back(point);
							
							/*Vec3b color = frame1.at <Vec3b>(Point(avx_c1,avy_c1));
				
							Vec3b gcolor=color;
							gcolor[2]=220;//r
							gcolor[1]=0;//g
							gcolor[0]=220;//b
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(avx_c1+pix,avy_c1))= gcolor;
							}
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(avx_c1,avy_c1+pix))= gcolor;
							}*
						
						}
						
						
						//crawlforcorner(inpoints_c1[ic1].at(indexc1)[0],inpoints_c1[ic1].at(indexc1)[1],inpoints_c1[ic1].at(indexc1)[2],frame1,ic1+1);
						indexc1++;
					}
					
				/*
				int botx=0;
						int boty=0;
						
						for(int poscorn=0;poscorn<int(corner_c1.size());poscorn++){
							if(corner_c1.at(poscorn)[0]>=botx&&corner_c1.at(poscorn)[1]>=boty){
								 botx=corner_c1.at(poscorn)[0];
									boty=corner_c1.at(poscorn)[1];
							}
							
						}
						Vec3b color = frame1.at <Vec3b>(Point(botx,boty));
				
							Vec3b gcolor=color;
							gcolor[2]=220;//r
							gcolor[1]=0;//g
							gcolor[0]=220;//b
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(botx+pix,boty))= gcolor;
							}
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(botx,boty+pix))= gcolor;
							}
							int bottx=1000;
						int botty=1000;
						
						for(int poscorn=0;poscorn<int(corner_c1.size());poscorn++){
							if(corner_c1.at(poscorn)[0]<=bottx&&corner_c1.at(poscorn)[1]<=botty){
								 bottx=corner_c1.at(poscorn)[0];
									botty=corner_c1.at(poscorn)[1];
							}
							
						}
						Vec3b colort = frame1.at <Vec3b>(Point(bottx,botty));
				
							Vec3b gcolort=colort;
							gcolort[2]=220;//r
							gcolort[1]=0;//g
							gcolort[0]=220;//b
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(bottx+pix,botty))= gcolort;
							}
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(bottx,botty+pix))= gcolort;
							}
						}
						int botrx=0;
						int botry=1000;
						
						for(int poscorn=0;poscorn<int(corner_c1.size());poscorn++){
							if(corner_c1.at(poscorn)[0]>=botrx&&corner_c1.at(poscorn)[1]<=botry){
								 botrx=corner_c1.at(poscorn)[0];
									botry=corner_c1.at(poscorn)[1];
							}
							
						}
						Vec3b colorr = frame1.at <Vec3b>(Point(botrx,botry));
				
							Vec3b gcolorr=colorr;
							gcolorr[2]=220;//r
							gcolorr[1]=0;//g
							gcolorr[0]=220;//b
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(botrx+pix,botry))= gcolorr;
							}
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(botrx,botry+pix))= gcolorr;
							
						}
						int botrbx=1000;
						int botrby=0;
						
						for(int poscorn=0;poscorn<int(corner_c1.size());poscorn++){
							if(corner_c1.at(poscorn)[0]<=botrbx&&corner_c1.at(poscorn)[1]>=botrby){
								 botrbx=corner_c1.at(poscorn)[0];
									botrby=corner_c1.at(poscorn)[1];
							}
							
						}
						Vec3b colorrb = frame1.at <Vec3b>(Point(botrbx,botrby));
				
							Vec3b gcolorrb=colorrb;
							gcolorrb[2]=220;//r
							gcolorrb[1]=0;//g
							gcolorrb[0]=220;//b
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(botrbx+pix,botrby))= gcolorrb;
							}
							for(int pix=-5;pix<5;pix++){
								frame1.at <Vec3b>(Point(botrbx,botrby+pix))= gcolorrb;
							
						*}
			}else{
				cout << "Cam Err" << endl;
		capture0.release();
		capture1.release();
		capture0.open(0);
		capture1.open(1);
			}
	}
		//IBU q ;
		//q.pfoo();
		//ibunit.foo();	
		
	
		cv::waitKey(1);
		i=i+1;
	}
	catch(const char* e){
		cout << e<< endl;
	}
	} *
	//cout << "dim"<< frame0n.cols << endl;
	//Vec3b color = frame0n.at <Vec3b>(Point(1,1));
	//cout << "dim"<< color << endl;
	
	
	auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
	
    //cout << cv::getBuildInformation() << std::endl;
	return 0;
}*/


/**
 * 
 * 
 * int r=0;
			int g=0;
			
				for(int ic0=0;ic0<2;ic0++){
				std::vector<std::array<std::vector <std::array<int,3>>,2>>  retinpdets_c0;
				if(ic0==0){
					r=inpoints_c0[ic0].size();
				}else{
					g=inpoints_c0[ic0].size();
				}
					std::size_t indexc0=0;
					while(indexc0< inpoints_c0[ic0].size()){
						
						retinpdets_c0.push_back(  calculatercornerinterestingpoints(pframe0n,inpoints_c0[ic0].at(indexc0)[0]-20,inpoints_c0[ic0].at(indexc0)[1]-20,40,ic0+1));
						
						
						std::array<std::vector <std::array<int,3>>,2> incornerpoints_c0;
						incornerpoints_c0=retinpdets_c0.at(indexc0);
						
						if(incornerpoints_c0[0].size()>15){
							int avx=0;
							int avy=0;
							int countam=0;
							for(int ipos=0;ipos<int(incornerpoints_c0[0].size());ipos++){
								avx=avx+incornerpoints_c0[0].at(ipos)[0];
								avy=avy+incornerpoints_c0[0].at(ipos)[1];
								countam++;
							}
							avx=avx/countam;
							avy=avy/countam;
							
							Vec3b color = pframe0n.at <Vec3b>(Point(avx,avy));
				
							Vec3b gcolor=color;
							gcolor[2]=220;//r
							gcolor[1]=0;//g
							gcolor[0]=220;//b
							for(int pix=-5;pix<5;pix++){
								pframe0n.at <Vec3b>(Point(avx+pix,avy))= gcolor;
							}
							for(int pix=-5;pix<5;pix++){
								pframe0n.at <Vec3b>(Point(avx,avy+pix))= gcolor;
							}
						
						}
						
						indexc0++;
					}
				}
				* 
				* **/
