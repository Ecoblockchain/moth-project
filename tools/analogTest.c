#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  int analog_0_fd;
  char analog_0_dev[50];
  sprintf(analog_0_dev, "/sys/devices/ocp.3/helper.12/AIN%i", 0);

  char buf[4];
  int value;

  while (1) {
    analog_0_fd = open(analog_0_dev, O_RDONLY);
    if (analog_0_fd < 0) {
      printf("ERROR: couldn't open analog device\n");
      return -1;
    }

    value = 0;
    if (read(analog_0_fd, buf, 4) < 0) {
      printf("ERROR: couldn't read from analog device\n");
      return -1;
    };
    value = atoi(buf);
    printf("%i\n", value);

    close(analog_0_fd);
    usleep(10000);
  }
}
