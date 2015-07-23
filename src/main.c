/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include "prototypes.h"

#define MAX_THREADS 1

pthread_t threads[MAX_THREADS];

void startAll() {
	pthread_create(&threads[0], NULL, sonarRead, NULL);
	//pthread_create(&threads[1], NULL, gpsRead, NULL);
}

void cancelAll() {
	int i;
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_cancel(threads[i]);
	}
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
