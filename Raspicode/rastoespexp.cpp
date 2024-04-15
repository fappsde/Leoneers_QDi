


#include <iostream>
#include <RASPTOESP.h>



int main(int argc, char **argv)
{
	RASPTOESP device(85);//68);
	//char data[2]{'c'};
	device.printdata("sp:005");
	device.readdata();
	return 0;
}

