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

pthread_t gps_thread, lidar_thread;

int main() {
	switch_init();
	lidar_init();
	gps_init();
	imu_init();
	ultra_init();

	while (1) {
		if (switch_status() == 1) {
			pthread_create(&gps_thread, NULL, gps_begin, NULL);
			pthread_create(&lidar_thread, NULL, lidar_begin, NULL);

			while (switch_status() == 1) {
				imu_update();
				write_log_2_row();
				usleep(10000);
			}

			pthread_cancel(gps_thread);
		}

		usleep(200000);
	}
}
