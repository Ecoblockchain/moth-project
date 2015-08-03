#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>


uint8_t sonar[] = {0x04, 0x06, 0x08, 0x0a};
char* i2c_dev = "/dev/i2c-2";
int tty_fd_sonar;
uint8_t ping = 0x51;
uint8_t buf[2];
int value = -1;
int sonar_num = 0;

int main(int argc, char* argv[]) {
  if (argc > 1) {
    sonar_num = argv[1][0] - '0';
  }
  tty_fd_sonar = open(i2c_dev, O_RDWR);
  if (tty_fd_sonar < 0) {
    printf("ERROR: couldn't open i2c device\n");
    return -1;
  }
  if (ioctl(tty_fd_sonar, I2C_SLAVE, sonar[sonar_num]) < 0) {
    printf("ERROR: couldn't set slave address\n");
    return -1;
  }
  while (1) {
    if (write(tty_fd_sonar, &ping, 1) != 1) {
      printf("ERROR: couldn't write to sonar\n");
    }
    usleep(80000);
    if (read(tty_fd_sonar, buf, 2) != 2) {
      printf("ERROR: couldn't read from sonar\n");
    }
    usleep(20000);
    value =  (buf[0] << 8) | buf[1];
    printf("%i\n", value);
  }
  return 0;
}
