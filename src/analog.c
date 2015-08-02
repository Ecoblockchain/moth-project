#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "log.h"

#define NUM_ANALOG 7

char analog_dev[NUM_ANALOG][50];

int analog_init() {
  int i;
  for (i = 0; i < NUM_ANALOG; i++) {
    sprintf(analog_dev[i], "/sys/bus/iio/devices/iio:device0/in_voltage%i_raw", i);
  }
  return 0;
}

int analog_read(int num) {
  int analog_fd;
  char buf[4];

  analog_fd = open(analog_dev[num], O_RDONLY);
  if (analog_fd < 0) {
    printf("ERROR: couldn't open analog device\n");
    return -1;
  }
  if (read(analog_fd, buf, 4) < 0) {
    printf("ERROR: couldn't read from analog device %i\n", num);
    return -1;
  }
  close(analog_fd);

  return atoi(buf);
}
