#include <mraa/i2c.h>
#include <unistd.h>
#include <stdio.h>
#include "log.h"
#include "shared.h"

uint8_t sonar[] = {0x04, 0x06, 0x08, 0x0a};
int arrayValues[] = {SONAR_1, SONAR_2, SONAR_3, SONAR_4};
mraa_i2c_context i2c_sonar;

void initSonar() {
  i2c_sonar = mraa_i2c_init(1);
  mraa_i2c_address(i2c_sonar, sonar[id]);
}

mraa_result_t pingSonar(int id) {
  mraa_result_t result;
  result = mraa_i2c_write_byte(i2c_sonar, 0x51);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to write to sonar %i\n", id);
  }
  return result;
}

mraa_result_t updateSonar(int id) {
  uint8_t buf[2];
  int value;
  mraa_result_t result;
  result = mraa_i2c_read(i2c_sonar, buf, 2);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to read from sonar %i\n", id);
    return result;
  }
  value =  (buf[0] << 8) | buf[1];
  parseSonar(arrayValues[id], value);
  return MRAA_SUCCESS;
}
