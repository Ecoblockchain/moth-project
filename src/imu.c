#include <stdio.h>
#include <mraa/i2c.h>

// Addresses of sensors


void * imuRead() {
	mraa_i2c_context imu;
	imu = mraa_i2c_init(0);
}
