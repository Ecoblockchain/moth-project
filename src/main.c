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
#include "sonar.h"
#include "analog.h"

#define MAX_THREADS 6

pthread_t threads[MAX_THREADS];
void *(*funcs[])(void *) = {gps_begin, sonar_begin, imu_begin, lidar_begin, analog_begin, log_begin};

void startAll() {
	int i;
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_create(&threads[i], NULL, funcs[i], NULL);
	}
}

void cancelAll() {
	int i;
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_cancel(threads[i]);
	}
}

int main() {
	int running = 0;
	switch_init();
	lidar_init();

	while (1) {
		if (switch_status() == 1) { // start/stop button
			if (!running) {
				startAll();
				running = 1;
			}
		} else {
			if (running) {
				cancelAll();
				running = 0;
			}
		}
		usleep(50000);
	}
	return 0;
}
