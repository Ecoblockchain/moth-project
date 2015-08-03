#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "ultra.h"
#include "log.h"

const uint8_t ultra_address[] = {0x04, 0x06, 0x08, 0x0a};
const char* ultra_i2c_dev = "/dev/i2c-2";
volatile int ultra_fd;
const uint8_t ultra_ping_code = 0x51;

int ultra_init() {
  ultra_fd = open(ultra_i2c_dev, O_RDWR);
  if (ultra_fd < 0) {
    printf("ERROR: couldn't open i2c ultra\n");
    return -1;
  }
  printf("STATUS: initialized ultrasonic\n");
  return 0;
}

int ultra_ping(int num) {
  if (ioctl(ultra_fd, I2C_SLAVE, ultra_address[num]) < 0) {
    printf("ERROR: couldn't set ultra address\n");
    return -1;
  }
  if (write(ultra_fd, &ultra_ping_code, 1) != 1) {
    printf("ERROR: couldn't write to ultra\n");
    return -1;
  }
  return 0;
}

void ultra_pingAll() {
  ultra_ping(0);
  ultra_ping(1);
  ultra_ping(2);
  ultra_ping(3);
}

int ultra_getDistance(int num) {
  uint8_t buf[2];
  if (ioctl(ultra_fd, I2C_SLAVE, ultra_address[num]) < 0) {
    printf("ERROR: couldn't set ultra address\n");
    return -1;
  }
  if (read(ultra_fd, buf, 2) != 2) {
    printf("ERROR: couldn't read from ultra\n");
    return -1;
  }
  return (buf[0] << 8) | buf[1];
}
