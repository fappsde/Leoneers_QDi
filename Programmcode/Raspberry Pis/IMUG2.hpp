
#ifndef IMUG2_HPP_
#define IMUG2_HPP_

#include </home/pi/QDI_gen2/MPU6050-C-CPP-Library-for-Raspberry-Pi/MPU6050G2.h>
#include </usr/local/include/opencv4/opencv2/opencv.hpp>


class IMUG2 {
	private:
		//void _update();

		float ax, ay, az, gr, gp, gy; //Temporary storage variables used in _update()

		int MPU6050_addr;
	public:
		IMUG2();
		void getposition();
		void getstatus(int* stat);
		void posthread(DDPMAPPER* mapper,float* xrot,float* yrot,float* zrot,bool* stopbit);
		
		void initimu();
		void gyrobackloop(int* driverev, float* zrbot, bool* imuon, bool* stopbit);
		
		/*
		void getAccelRaw(float *x, float *y, float *z);
		void getGyroRaw(float *roll, float *pitch, float *yaw);
		void getAccel(float *x, float *y, float *z);
		void getGyro(float *roll, float *pitch, float *yaw);
		void getOffsets(float *ax_off, float *ay_off, float *az_off, float *gr_off, float *gp_off, float *gy_off);
		int getAngle(int axis, float *result);*/
		
};
#endif	// IMUG2_HPP_
