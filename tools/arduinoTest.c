#include <mraa/i2c.h>
#include <stdio.h>
#include <unistd.h>

int convert(uint8_t high, uint8_t low) {
  return (high << 8) | low;
}

int main() {
  mraa_i2c_context i2c;
  i2c = mraa_i2c_init(1);
  mraa_i2c_address(i2c, 0x33);
  uint8_t buf[8];
  int i;

  while (1) {
    mraa_i2c_read(i2c, buf, 8);
    for (i = 0; i < 8; i += 2) {
      printf("%i\t", convert(buf[i], buf[i + 1]));
    }
    printf("\n");
    usleep(10000);
  }
}
