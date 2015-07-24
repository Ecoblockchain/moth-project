#include <mraa/i2c.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	uint8_t oldAddress = 0x02;
	uint8_t newAddress = 0x06;
	newAddress = newAddress << 1;

	mraa_i2c_context i2c;
	uint8_t buf[2];
	i2c = mraa_i2c_init(1);

	mraa_i2c_address(i2c, oldAddress);
	mraa_i2c_write_byte(i2c, 0x51);
	usleep(80000);
	mraa_i2c_read(i2c, buf, 2);
	int value = (buf[0] << 8) | buf[1];
	printf("before: %i,\t", value);

	usleep(20000);
	uint8_t writebuf[] = {0xaa, 0xa5, newAddress};
	mraa_i2c_write(i2c, writebuf, 3);

	mraa_i2c_address(i2c, newAddress);
	mraa_i2c_write_byte(i2c, 81);
	usleep(80000);
	mraa_i2c_read(i2c, buf, 2);
	value = (buf[0] << 8) | buf[1];
	printf("after: %i\n", value);

	return 0;
}
