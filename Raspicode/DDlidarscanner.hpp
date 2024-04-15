
#ifndef DDlidarscanner_HPP_
#define DDlidarscanner_HPP_




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

#include <csignal>
#include "/home/pi/QDI_gen2/rplidar_a2-master/include/lidar/RPLidar.hpp"
#include "/home/pi/QDI_gen2/rplidar_sdk-master/sdk/include/sl_lidar.h" 
#include "/home/pi/QDI_gen2/rplidar_sdk-master/sdk/include/sl_lidar_driver.h"
#include </home/pi/QDI_gen2/rplidar_sdk-master/sdk/include/rplidar.h>
//#include </home/pi/QDI_gen2/ddpmapper.cpp>







class DDlidarscanner {
	private:
		void signal_handler(int signo);
		
		
	public:
		DDlidarscanner();
		
		void startLidar();
		
		void setupLidar();
		void ddmapthread(DDPMAPPER* mapper, bool* stopbit,bool print);
		
		
};
#endif	// DDlidarscanner_H_

