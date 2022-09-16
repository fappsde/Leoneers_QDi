

#include </home/pi/QDI_gen2/DDlidarscanner.hpp>

sl::ILidarDriver * lidar;
std::vector<sl::LidarScanMode> scanModes;
 

bool running=true; //Variable to manage stops when signals are received
void DDlidarscanner::signal_handler(int signo){
	if(signo==SIGTERM || signo==SIGINT)
		running = false;
}

void DDlidarscanner::ddmapthread(DDPMAPPER* mapper, bool* stopbit,bool print){
	
	while(*stopbit) {
		sl_lidar_response_measurement_node_hq_t nodes[8192];
		size_t nodeCount = sizeof(nodes)/sizeof(sl_lidar_response_measurement_node_hq_t);
		auto res = lidar->grabScanDataHq(nodes, nodeCount);

		if (IS_FAIL(res)){
			// failed to get scan data
		}else{
			//lidar->ascendScanData(nodes, nodeCount);
			for(int pos=0;pos<(int)nodeCount;pos++){
				double distance=nodes[pos].dist_mm_q2/4.0f;
				double angle=(nodes[pos].angle_z_q14 * 90.f) / 16384.f;
				
				//addvalue(double sensorxfromcarpos, double sensoryfromcarpos,double sensorzfromcarpos, double distance, double sensorangle, double subsensorx,int subsensory,int carposx, int carposy, double carrotz)
				mapper->addLidDistance(10,0,distance/10,angle);
			}
		}
		
			}
	
}

void DDlidarscanner::startLidar(){
	lidar->startScan(false, true,0,&scanModes[3]);
	
}

void DDlidarscanner::setupLidar(){
	
	sl::Result<sl::IChannel*> channel = sl::createSerialPortChannel("/dev/ttyAMA0", 115200);
    ///  Create a LIDAR driver instance
     lidar = *sl::createLidarDriver();
     auto res = (*lidar).connect(*channel);
    if(SL_IS_OK(res)){
        sl_lidar_response_device_info_t deviceInfo;
        res = (*lidar).getDeviceInfo(deviceInfo);
        if(SL_IS_OK(res)){
            printf("Model: %d, Firmware Version: %d.%d, Hardware Version: %d\n",
            deviceInfo.model,
            deviceInfo.firmware_version >> 8, deviceInfo.firmware_version & 0xffu,
            deviceInfo.hardware_version);
        }else{
            fprintf(stderr, "Failed to get device information from LIDAR %08x\r\n", res);
        }
    }else{
        fprintf(stderr, "Failed to connect to LIDAR %08x\r\n", res);
    }
    rplidar_response_device_health_t health;
    
    lidar->getHealth(health);
    std::cout<<health.status<<std::endl;
    
	//scanModes;
	lidar->getAllSupportedScanModes(scanModes);
	
	for(int i=0;i<(int)scanModes.size();i++){
		std::cout<<(scanModes.at(i).id)<<" "<<scanModes.at(i).us_per_sample<<std::endl;
	}
}

DDlidarscanner::DDlidarscanner(){
	
	
}
/*
int main(int argc, char** argv){
	
	///  Create a communication channel instance
    sl::IChannel* _channel;
    sl::Result<sl::IChannel*> channel = sl::createSerialPortChannel("/dev/ttyAMA0", 115200);
    ///  Create a LIDAR driver instance
    sl::ILidarDriver * lidar = *sl::createLidarDriver();
    auto res = (*lidar).connect(*channel);
    if(SL_IS_OK(res)){
        sl_lidar_response_device_info_t deviceInfo;
        res = (*lidar).getDeviceInfo(deviceInfo);
        if(SL_IS_OK(res)){
            printf("Model: %d, Firmware Version: %d.%d, Hardware Version: %d\n",
            deviceInfo.model,
            deviceInfo.firmware_version >> 8, deviceInfo.firmware_version & 0xffu,
            deviceInfo.hardware_version);
        }else{
            fprintf(stderr, "Failed to get device information from LIDAR %08x\r\n", res);
        }
    }else{
        fprintf(stderr, "Failed to connect to LIDAR %08x\r\n", res);
    }
    rplidar_response_device_health_t health;
    
    lidar->getHealth(health);
    std::cout<<health.status<<std::endl;
    // TODO
	std::vector<sl::LidarScanMode> scanModes;
	lidar->getAllSupportedScanModes(scanModes);
	
	for(int i=0;i<scanModes.size();i++){
		std::cout<<(scanModes.at(i).id)<<" "<<scanModes.at(i).us_per_sample<<std::endl;
	}
	sl::LidarScanMode scanMode;
	lidar->startScan(false, true,0,&scanModes[0]);

while(1){
	sl_lidar_response_measurement_node_hq_t nodes[8192];
	size_t nodeCount = sizeof(nodes)/sizeof(sl_lidar_response_measurement_node_hq_t);
	res = lidar->grabScanDataHq(nodes, nodeCount);

	std::cout<<nodeCount<<" ";
	if (IS_FAIL(res))
	{
		// failed to get scan data
	}else{
		lidar->ascendScanData(nodes, nodeCount);
		for(int pos=0;pos<(int)nodeCount;pos++){
			printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
                    (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", 
                    (nodes[pos].angle_z_q14 * 90.f) / 16384.f,
                    nodes[pos].dist_mm_q2/4.0f,
                    nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);*
			float angle_in_degrees = nodes[gr].angle_z_q14 * 90.f / (1 << 14);
			float distance_in_meters = nodes[gr].dist_mm_q2 / 1000.f / (1 << 2);
			printf("Dist: %d, Angle: %d\n",
            angle_in_degrees,
            distance_in_meters);*
		}
		
			
	}
}
		
	
    /// Delete Lidar Driver and channel Instance
     delete lidar;
     delete *channel;
	
	
	*
	
	* ************************************
	 *      SETUP LIDAR & CHECK STATUS    *
	 **************************************
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	std::cout <<"start";
	RPLidar lidar; //Connects to lidar
	//running = lidar.init(argc>1?argv[argc - 1]:"/dev/ttyAMA0");
	running = lidar.init("/dev/ttyAMA0");
	std::cout <<"ready";
	
	* ************************************
	 *               START 			      *
	 **************************************
	if(running) {
		
		lidar.print_status();
		//return 0;
		running = lidar.start();
	}

	
	while(running){
		//Update current scan (one turn of measurements)
		lidar.update();
		lidar.print_scan();
		lidar.print_deltas();
	}
	* ***********************************
	 *              STOP ALL             *
	 *************************************
	return 0;
}*/
