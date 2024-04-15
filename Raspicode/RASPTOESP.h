
#ifndef RASPTOESP_H_
#define RASPTOESP_H_

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
	#include <linux/i2c-dev.h>
	#include <i2c/smbus.h>
}
#include <cmath>
#include <thread>








class RASPTOESP {
	private:
		
		int MPU6050_addr;
		int f_dev; //Device file
		

		float dt; //Loop time (recalculated with each loop)

		struct timespec start,end; //Create a time structure

		bool _first_run = 1; //Variable for whether to set gyro angle to acceleration angle in compFilter
	public:
		RASPTOESP(int8_t addr);
		void printdata(std::string message);
		int readdata(std::string message);
		int readdata();
		
};
#endif	// _RASPTOESP_H_

