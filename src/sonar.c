#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "sonar.h"
#include "log.h"

const uint8_t sonar_address[] = {0x04, 0x06, 0x08, 0x0a};
const int sonar_array_index[] = {SONAR_1, SONAR_2, SONAR_3, SONAR_4};
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
  if (num > 1) return 0; // TEMPORARY
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
  if (num > 1) return 0; // TEMPORARY
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
  save_log_value(sonar_array_index[num], value, 0);
  return value;
}

void* sonar_begin() {
	initSonar();
	pingSonar(0);
	usleep(40000);
	pingSonar(1);
	usleep(40000);
	while (1) {
		getSonarDistance(0);
		pingSonar(2);
		usleep(40000);
		getSonarDistance(1);
		pingSonar(3);
		usleep(40000);
		getSonarDistance(2);
		pingSonar(0);
		usleep(40000);
		getSonarDistance(3);
		pingSonar(1);
		usleep(40000);
	}
}
