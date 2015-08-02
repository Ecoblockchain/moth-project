#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	uint8_t oldAddress = 0x70;
	uint8_t newAddress = 0x0a;
	newAddress = newAddress << 1;

	const char* sonar_i2c_dev = "/dev/i2c-2";
	volatile int sonar_fd;
	uint8_t writebuf[] = {0xaa, 0xa5, newAddress};

	sonar_fd = open(sonar_i2c_dev, O_RDWR);
	ioctl(sonar_fd, I2C_SLAVE, oldAddress);
	write(sonar_fd, writebuf, 3);

	close(sonar_fd);
	return 0;
}
