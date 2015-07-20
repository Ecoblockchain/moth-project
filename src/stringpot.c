/*
 * stringpot.c
 *
 *      Author: James Vaughan
 */

#include <unistd.h>
#include <mraa/spi.h>
#include <mraa/gpio.h>
#include <stdio.h>
#include "prototypes.h"

void * spRead() {
	const int spi_dev_num = 5;
	int chipSelectPin = 8;
	mraa_spi_context spi;
	mraa_gpio_context chipSelect;

	spi = mraa_spi_init(spi_dev_num);
	chipSelect = mraa_gpio_init(chipSelectPin);
	mraa_gpio_mode(chipSelect, MRAA_GPIO_OUT_HIGH);
	printf("Initialized SPI\n");

	unsigned int response = 0;
	uint8_t data[] = {0x68};
	uint8_t zero[] = {0x00};
	uint8_t * recv;

	while (1) {
		mraa_gpio_write(chipSelect, 0);		
		recv = mraa_spi_write_buf(spi, data, 1);
		response = recv[0];
		response = response & 0x03;
		response = response << 8;
		recv = mraa_spi_write_buf(spi, zero, 1);
		response = response | recv[0];
		printf("value: %i", response);
		usleep(50000);
	}

	return NULL;
}
