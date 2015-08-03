#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "log.h"
#include "imu.h"

char* analog_dev = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
char* analog_adev = "/dev/ttyACM1";
volatile int analog_afd;
char analog_buffer[400];
int analog_open = 0;

int analog_ainit() {
  analog_afd = open(analog_adev, O_RDWR);
  struct termios tio;
	tcgetattr(analog_afd, &tio);
	cfsetspeed(&tio, B115200);
	cfmakeraw(&tio);
	tcsetattr(analog_afd, TCSANOW, &tio);
  printf("STATUS: initialized analog (arduino)\n");
  analog_open = 1;
  return 0;
}

void* analog_aread() {
  while (files_open == 0 && analog_open == 0);
  printf("STATUS: entering analog_aread\n");
  char c;
  int i = 0;
  int read_more = 0;
  while (1) {
    while (read(analog_afd, &c, 1) == -1 || (c != '$' && read_more != 1));
    read_more = 1;
    if (c != '\n') {
      analog_buffer[i++] = c;
    } else {
      analog_buffer[i - 2] = '\0';
      i = 0;
      read_more = 0;
      imu_update();
      write_log_2_row(&analog_buffer[1]);
    }
  }
}

int analog_read() {
  int analog_fd;
  char buf[4];

  analog_fd = open(analog_dev, O_RDONLY);
  if (analog_fd < 0) {
    printf("ERROR: couldn't open analog device\n");
    return -1;
  }
  if (read(analog_fd, buf, 4) < 0) {
    printf("ERROR: couldn't read from analog device (beagle)\n");
    return -1;
  }
  close(analog_fd);

  return atoi(buf);
}
