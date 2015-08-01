#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "log.h"

#define NUM_ANALOG 7

int analog_array_nums[] = {ANALOG_0, ANALOG_1, ANALOG_2, ANALOG_3, ANALOG_4, ANALOG_5, ANALOG_6};

int analog_read(int num) {
  int analog_fd;
  char analog_dev[50];
  char buf[4];

  sprintf(analog_dev, "/sys/devices/ocp.3/helper.12/AIN%i", num);
  analog_fd = open(analog_dev, O_RDONLY);
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

void* analog_begin() {
  int i;
  while (1) {
    for (i = 0; i < NUM_ANALOG; i++) {
      save_log_value(analog_array_nums[i], analog_read(i), 1);
      usleep(10000);
    }
  }
}
