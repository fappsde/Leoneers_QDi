

#include </home/pi/QDI_gen2/MPU6050-C-CPP-Library-for-Raspberry-Pi/IMUG2.hpp>

MPU6050G2 mpu(104,true);//0x68

float pxpos=0;

float pxrot=0;
float pyrot=0;
float pzrot=0;

float poax, poay, poaz, pogr, pogp, pogy;
float ax, ay, az, gr, gp, gy;

void IMUG2::getposition(){
	
	
}

void IMUG2::getstatus(int* stat){
	
	
}

void IMUG2::posthread(DDPMAPPER* mapper,float* xrot,float* yrot,float* zrot,bool* stopbit){
	
	while(*stopbit){
		
		mpu.getGyroRaw(&gr, &gp, &gy);
		
		*xrot=*xrot+(round((gr - pogr) / 10.0 / 16.4) / 10.0)*1.0714285714;
		*yrot=*yrot+(round((gp - pogp) / 10.0 / 16.4) / 10.0)*1.0714285714;
		*zrot=*zrot+(round((gy - pogy) / 10.0 / 16.4) / 10.0)*3.71134;//*1.0714285714;
		
		mapper->setcarrotaotion(*zrot);
		std::cout<<*zrot<<std::endl;
		cv::waitKey(15);//10 to not overflow bus
		//std::cout << " X: " << *xrot  << "\n";
	}
	
}

void IMUG2::initimu(){
	mpu.getOffsets(&poax, &poay, &poaz, &pogr, &pogp, &pogy,10000/2/3);
	mpu.calc_yaw = true;
	
}

void IMUG2::gyrobackloop(int* driverev, float* zrbot, bool* imuon, bool* stopbit){
	
	///TODO drive back with poscam 
	
	int stehtcount=0;
	int prevx=*zrbot;
	int prevxb=0;
	if(*imuon){
		while(*stopbit){
			for(int i=0;i<300;i++){
				prevx=prevx+*zrbot;
				
			}
			for(int i=0;i<300;i++){
				prevxb=prevxb+*zrbot;
				
			}
			int prevxb=prevxb/300;
			int prevx=prevx/300;
			std::cout << " x: " <<std::to_string( *zrbot )<< " rev: " <<std::to_string( prevx ) << "\n";
			if(prevx-7<prevxb&&prevx+7>prevxb){
				stehtcount++;
				
				
			}else{
				stehtcount=0;
			}
			std::cout << " count: " <<std::to_string( stehtcount ) << "\n";
			
			
			if(stehtcount>10){
				if(*zrbot<-30){
					*driverev=-1;
				}else if(*zrbot>30){
					*driverev=1;
				}else{
					
					*driverev=2;
				}
			}else{
				*driverev=0;
			}
		}
	}
}

IMUG2::IMUG2(){
	//mpu.getOffsets(&poax, &poay, &poaz, &pogr, &pogp, &pogy,10000/2);
	
	//mpu.calc_yaw = true;
}



/*
int main() {
	
	
	bool stop=true;
	
	std::thread dispth(&IMU::posthread,&xrot,&yrot,&zrot,&stop);
	dispth.join();
	
	
}
	/*
	float ax, ay, az, gr, gp, gy; //Variables to store the accel, gyro and angle values

float oax, oay, oaz, ogr, ogp, ogy;
	sleep(1); //Wait for the MPU6050 to stabilize


	//Calculate the offsets
	std::cout << "Calculating the offsets...\n    Please keep the accelerometer level and still\n    This could take a couple of minutes...";
	device.getOffsets(&oax, &oay, &oaz, &ogr, &ogp, &ogy);
	std::cout << "Gyroscope R,P,Y: " << ogr << "," << ogp << "," << ogy << "\nAccelerometer X,Y,Z: " << oax << "," << oay << "," << oaz << "\n";


	//Read the current yaw angle
	device.calc_yaw = true;
	int prevten=0;
		
	for (int i = 0; i < 10000; i++) {
	/*	device.getAngle(0, &gr);
		device.getAngle(1, &gp);
		device.getAngle(2, &gy);
		std::cout << "Current angle around the roll axis: " << gr << "\n";
		std::cout << "Current angle around the pitch axis: " << gp << "\n";
		std::cout << "Current angle around the yaw axis: " << gy << "\n";
		
		
	std::cout << "Accelerometer Readings: X: " << ax << ", Y: " << ay << ", Z: " << az << "\n";*
	device.getAccelRaw(&ax, &ay, &az);
			device.getGyroRaw(&gr, &gp, &gy);
			
			xrot=xrot+(round((gy - ogy) / 10.0 / 16.4) / 10.0);
		/*float tensteps=0;
		for (int ip = 0; ip < 10; ip++) {
			device.getAccelRaw(&ax, &ay, &az);
			device.getGyroRaw(&gr, &gp, &gy);
			int pchange=(round((ay - oay) * 1000.0 / 8192.0) / 1000.0)*100*0.10197162129779;
			tensteps=tensteps+pchange;
			usleep(100);
		}*
		//int pten=tensteps;
		//tensteps=tensteps-prevten;
		//prevten=tensteps;
		
		//std::cout << "Gyroscope Readings: X: " << gr << ", Y: " << gp << ", Z: " << gy << "\n";round((*z - A_OFF_Z) * 1000.0 / ACCEL_SENS) / 1000.0;
		//Strecke = ½ * Beschleunigung * Zeit²
		//s = ½ * a * t²
		//int change=(round((ay - oay) * 1000.0 / 2048.0) / 1000.0)*100;
		//float strecke=(1/2)*((tensteps)/100)*(10*10);
		xpos=gy;//xpos+tensteps/0.02;//gp+2;
		
		//ay+1
		//ay+0.18354
		//ax+1.76492
		//gr-3.81651
		//az-0.854
		//ax+1.7943

	std::cout << " X: " << xrot  << "\n";
		//(250000); //0.25sec
	}

	//Get the current accelerometer values
	device.getAccel(&ax, &ay, &az);
	std::cout << "Accelerometer Readings: X: " << ax << ", Y: " << ay << ", Z: " << az << "\n";

	//Get the current gyroscope values
	device.getGyro(&gr, &gp, &gy);
	std::cout << "Gyroscope Readings: X: " << gr << ", Y: " << gp << ", Z: " << gy << "\n";

	return 0;
}*/


