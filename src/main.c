/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include <mraa/gpio.h>
#include "prototypes.h"

#define MAX_THREADS 2

pthread_t threads[MAX_THREADS];
mraa_gpio_context status;

void startAll() {
	pthread_create(&threads[0], NULL, sonarRead, NULL);
	pthread_create(&threads[1], NULL, gpsRead, NULL);
	mraa_gpio_write(status, 1);
}

void cancelAll() {
	int i;
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_cancel(threads[i]);
	}
	mraa_gpio_write(status, 0);
}

int main() {
	int running = 0;
	status = mraa_gpio_init(15);
	mraa_gpio_dir(status, MRAA_GPIO_OUT_LOW);

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
