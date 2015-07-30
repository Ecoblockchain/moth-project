#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mraa/gpio.h>
#include "log.h"
#include "gps.h"
#include "imu.h"
#include "switch.h"
#include "sonar.h"

#define MAX_THREADS 4

pthread_t threads[MAX_THREADS];

void startAll() {
	pthread_create(&threads[0], NULL, gps_begin, NULL);
	pthread_create(&threads[1], NULL, sonar_begin, NULL);
	pthread_create(&threads[2], NULL, imu_begin, NULL);
	pthread_create(&threads[3], NULL, log_begin, NULL);
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
