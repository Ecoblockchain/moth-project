#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "log.h"
#include "imu.h"

//char* analog_dev = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
char* analog_adev = "/dev/ttyACM0";
volatile int analog_afd;
int analog_open = 0;

int analog_ainit() {
  printf("DEBUG: start a init\n");
  close(analog_afd);
  analog_afd = open(analog_adev, O_RDWR);
  if (analog_afd < 0) {
    printf("ERROR: couldn't open analog_afd\n");
    return -1;
  }
  printf("DEBUG: opened arduino\n");
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
  analog_ainit();
  printf("STATUS: entering analog_aread\n");
  char c;
  char analog_buffer[500];
  int i = 0;
  int errors = 0;
	tcflush(analog_afd, TCIFLUSH);
  int analog_read_more = 0;

  while (1) {
    while (read(analog_afd, &c, 1) == -1 || (c != '$' && analog_read_more != 1)) {
      if (++errors > 500) {
        printf("ERROR: major problem getting connection to arduino. trying backup plan...\n");
        while (1) {
          imu_update();
          write_log_2_row("-1\t-1\t-1\t-1\t");
        }
      }
    }
    analog_read_more = 1;
    if (c != '\n') {
      analog_buffer[i++] = c;
			if (i > 400) {
				// prevent segmentation faults
				i = 0;
				printf("\nERROR: ***********************************************\nlocal_buffer overflow in read analog.\n Connect inputs and Cycle power to recover\n*****************************************\n");
				int flag = 1;
				char old = 'x';
				while (flag == 1) {
					read(analog_afd, &c, 1);
					if (c == '\n' && old == '\r') flag = 0;
					old = c;
				}
			}
    } else {
      analog_buffer[i - 2] = '\0';
      i = 0;
      analog_read_more = 0;
      imu_update();
      write_log_2_row(&analog_buffer[1]);
      printf("ANALOG: %s\n", analog_buffer);
    }
  }
}

/*
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
*/
