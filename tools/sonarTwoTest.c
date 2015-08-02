#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t sonar_address[] = {0x04, 0x06, 0x08, 0x0a};
const char* sonar_i2c_dev = "/dev/i2c-2";
volatile int sonar_fd;
const uint8_t sonar_ping = 0x51;

void initSonar() {
  printf("WARNING: ignoring sonars 3 and 4\n");
  sonar_fd = open(sonar_i2c_dev, O_RDWR);
  if (sonar_fd < 0) {
    printf("ERROR: couldn't open i2c sonar\n");
  }
}

int pingSonar(int num) {
  if (ioctl(sonar_fd, I2C_SLAVE, sonar_address[num]) < 0) {
    printf("ERROR: couldn't set sonar address\n");
    return -1;
  }
  if (write(sonar_fd, &sonar_ping, 1) != 1) {
    printf("ERROR: couldn't write to sonar\n");
    return -1;
  }
  return 0;
}

int getSonarDistance(int num) {
  int value;
  uint8_t buf[2];
  if (ioctl(sonar_fd, I2C_SLAVE, sonar_address[num]) < 0) {
    printf("ERROR: couldn't set sonar address\n");
    return -1;
  }
  if (read(sonar_fd, buf, 2) != 2) {
    printf("ERROR: couldn't read from sonar\n");
    return -1;
  }
  value =  (buf[0] << 8) | buf[1];
  printf("%i\t", value);
  if (num == 1) printf("\n");
  return value;
}

void* sonar_begin() {
	initSonar();
	pingSonar(0);
	pingSonar(1);
	pingSonar(2);
	pingSonar(3);
	while (1) {
    usleep(80000);
		getSonarDistance(0);
		getSonarDistance(1);
		getSonarDistance(2);
		getSonarDistance(3);
		usleep(20000);
		pingSonar(0);
		pingSonar(1);
		pingSonar(2);
		pingSonar(3);
	}
}

int main() {
  sonar_begin();
}
