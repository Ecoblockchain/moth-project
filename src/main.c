/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <mraa/gpio.h>
#include "shared.h"
#include "defines.h"

pthread_t threads[MAX_THREADS];
mraa_gpio_context status;
mraa_gpio_context toggler;
int cont;

void startAll() {
	pthread_create(&threads[0], NULL, sonarRead, NULL);
	pthread_create(&threads[1], NULL, gpsRead, NULL);
	status = mraa_gpio_init(STATUS_LED);
	mraa_gpio_dir(status, MRAA_GPIO_OUT_HIGH);
}

void cancelAll() {
	int i;
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_cancel(threads[i]);
	}
	mraa_gpio_write(status, 0);
	mraa_gpio_close(status);
}

void sig_handler(int signo) {
    if (signo == SIGINT) {
				mraa_gpio_close(toggler);
        cont = 0;
    }
}

void begin() {
	int running = 0;
	cont = 1;
	int toggleOn;
	toggler = mraa_gpio_init(TOGGLER);
	mraa_gpio_dir(toggler, MRAA_GPIO_IN);

	signal(SIGINT, sig_handler);

	while (cont) {
		toggleOn = mraa_gpio_read(toggler);
		if (toggleOn) { // start/stop button
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
}

int main(int argc, char* argv[]) {
	int seconds;
	if (argc > 1) {
		seconds = atoi(argv[1]);
		while (seconds > 0) {
			printf("Starting logger in %i seconds. (Press ctrl+c to abort.)\n", seconds);
			seconds--;
			sleep(1);
		}
	}
	begin();
	return 0;
}
