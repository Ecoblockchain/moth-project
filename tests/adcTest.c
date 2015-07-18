#include <unistd.h>
#include <mraa/spi.h>
#include <stdio.h>

void * spRead() {
	mraa_spi_context spi;
	spi = mraa_spi_init(5);
	mraa_spi_mode(spi, MRAA_SPI_MODE3);
	unsigned int response = 0;
	printf("Initialized SPI\n");
	uint8_t data[] = {0x00, 100};
	uint8_t * recv;

	while (1) {
		int i;
		for (i = 90; i < 130; i++) {
			data[1] = i;
			mraa_spi_transfew_buf(spi, data, recv, 2);
			printf("Writing -%i", i);
			printf("RECIEVED-%i-%i\n", recv[0], recv[1]);
			usleep(100000);
		}
		for (i = 130; i < 90; i--) {
			data[1] = i;
			recv = mraa_spi_write_buf(spi, data, 2);
			printf("Writing -%i", i);
			printf("RECIEVED-%i-%i\n", recv[0], recv[1]);
			usleep(100000);
		}
	}

	return NULL;
}

int main() {
	spRead();
	return 0;
}
