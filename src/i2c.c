#include <mraa/i2c.h>
#include <unistd.h>
#include <stdio.h>
#include "log.h"
#include "prototypes.h"

uint8_t sonar[] = {0x02, 0x70, 0x70, 0x70};
mraa_i2c_context i2c;

void pingSonar(int id) {
  mraa_i2c_address(i2c, sonar[id]);
  mraa_i2c_write_byte(i2c, 0x51);
}

int updateSonar(int id) {
  mraa_i2c_address(i2c, sonar[id]);
  //return ((int) mraa_i2c_read_byte(i2c) << 8) | mraa_i2c_read_byte(i2c);
  return 500;
}

void* sonarRead() {
  pingSonar(0);
  usleep(40000);
  pingSonar(1);
  usleep(40000);
  while(1) {
    parseSonar(SONAR_1, updateSonar(0));
    pingSonar(2);
    usleep(40000);
    //parseSonar(SONAR_2, updateSonar(1));
    pingSonar(3);
    usleep(40000);
    //parseSonar(SONAR_3, updateSonar(2));
    pingSonar(0);
    usleep(40000);
    //parseSonar(SONAR_4, updateSonar(3));
    pingSonar(1);
    usleep(40000);
  }
}
