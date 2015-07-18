#include <unistd.h>
#include <mraa/spi.h>
#include <stdio.h>

int main() {
	mraa_spi_context spi;
	spi = mraa_spi_init(5);
	mraa_spi_mode(spi, MRAA_SPI_MODE3);
	unsigned int response = 0;
	printf("Initialized SPI\n");
	uint8_t data[] = {0x68, 0x00};
	uint8_t * recv;

	while (1) {
		printf("Writing - %i-%i", data[0], data[1]);
		recv = mraa_spi_write_buf(spi, data, 2);
		printf("  --  Received - %i-%i\n", recv[0], recv[1]);
		usleep(300000);
	}
	
	return 0;
}
