/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include "prototypes.h"

#define MAX_THREADS 2

pthread_t gpsThread;
pthread_t sonarThread;

void startAll() {
	pthread_create(&gpsThread, NULL, gpsRead, NULL);
	pthread_create(&sonarThread, NULL, sonarRead, NULL);
}

void cancelAll() {
	pthread_cancel(gpsThread);
	pthread_cancel(sonarThread);
}

int main() {
	int running = 0;

	while (1) {
		if (1) { // start/stop button
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
	}

	return 0;
}
