#include <mraa/i2c.h>
#include <unistd.h>
#include <stdio.h>
#include "log.h"
#include "shared.h"

uint8_t sonar[] = {0x02, 0x04, 0x00, 0x00};
int arrayValues[] = {SONAR_1, SONAR_2, SONAR_3, SONAR_4};
mraa_i2c_context i2c;

void pingSonar(int id) {
  mraa_i2c_address(i2c, sonar[id]);
  mraa_i2c_write_byte(i2c, 0x51);
}

void updateSonar(int id) {
  uint8_t buf[2];
  int value;
  mraa_i2c_address(i2c, sonar[id]);
  mraa_i2c_read(i2c, buf, 2);
  value =  (buf[0] << 8) | buf[1];
  parseSonar(arrayValues[id], value);
}

void* sonarRead() {
	i2c = mraa_i2c_init(1);
  mraa_i2c_frequency(i2c, MRAA_I2C_STD);

  pingSonar(0);
  usleep(40000);
  pingSonar(1);
  usleep(40000);
  while (1) {
    updateSonar(0);
    pingSonar(2);
    usleep(40000);
    updateSonar(1);
    pingSonar(3);
    usleep(40000);
    updateSonar(2);
    pingSonar(0);
    usleep(40000);
    updateSonar(3);
    pingSonar(1);
    usleep(40000);
  }

  return NULL;
}
