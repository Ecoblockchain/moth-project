#include "../include/mraa/aio.h"
#include <stdio.h>

void * spRead() {
	mraa_aio_context stringpot;
	float spValue;

	stringpot = mraa_aio_init(0);
	if (stringpot == NULL) {
		printf("Error initializing String Potentiometer\n");
		return NULL;
	}
	printf("Initialized String Potentiometer\n");

	while(1) {
		spValue = mraa_aio_read_float(stringpot);
		printf("String Pot Value: %f\n", spValue);
	}

	mraa_aio_close(stringpot);
	return NULL;
}
