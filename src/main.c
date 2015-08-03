#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mraa/gpio.h>
#include "log.h"
#include "gps.h"
#include "imu.h"
#include "lidar.h"
#include "switch.h"
#include "ultra.h"
#include "analog.h"

#define TIME_OFFSET 40872

pthread_t gps_thread, lidar_thread, analog_thread;

int main() {
	switch_init();
	gps_init();
	imu_init();
	ultra_init();
	lidar_init();
	analog_ainit();
	printf("STATUS: everything is initialized\n");

	usleep(10000);

	while (1) {
		while (switch_status() == 0) usleep(200000);
		printf("STATUS: switch on\n");
		pthread_create(&gps_thread, NULL, gps_begin, NULL);
		pthread_create(&lidar_thread, NULL, lidar_begin, NULL);
		pthread_create(&analog_thread, NULL, analog_aread, NULL);

		while (switch_status() == 1) usleep(200000);
		printf("STATUS: switch off\n");
		pthread_cancel(gps_thread);
		pthread_cancel(lidar_thread);
		pthread_cancel(analog_thread);
	}
}
