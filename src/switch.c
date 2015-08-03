#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "switch.h"

const char* gpio_export = "/sys/class/gpio/export";
const char* gpio_unexport = "/sys/class/gpio/unexport";
const char* switch_dev_direction = "/sys/class/gpio/gpio20/direction";
const char* switch_dev_value = "/sys/class/gpio/gpio20/value";

int switch_init() {
  int fd;
  char buf[] = "20";

  fd = open(gpio_export, O_WRONLY);
  if (fd < 0) {
    printf("ERROR: couldn't initialize switch gpio\n");
    return -1;
  }
  if (write(fd, buf, 2) != 2) {
    printf("WARNING: couldn't initialize switch (It might already be initialized.)\n");
  }
  close(fd);

  buf[0] = 'i';
  buf[1] = 'n';
  fd = open(switch_dev_direction, O_RDWR);
  if (fd < 0) {
    printf("ERROR: couldn't initialize switch gpio\n");
    close(fd);
    return -1;
  }
  if (write(fd, buf, 2) != 2) {
    printf("ERROR: couldn't set switch pin direction\n");
    close(fd);
    return -1;
  }
  close(fd);

  return 0;
}

int switch_status() {
  int switch_fd;
  char buf;

  switch_fd = open(switch_dev_value, O_RDONLY);
  if (switch_fd < 0) {
    printf("ERROR: couldn't open switch gpio\n");
    close(switch_fd);
    return -1;
  }
  if (read(switch_fd, &buf, 1) != 1) {
    printf("ERROR: couldn't read switch pin value\n");
    close(switch_fd);
    return -1;
  }
  close(switch_fd);

  if (buf == '1') return 1;
  return 0;
}
