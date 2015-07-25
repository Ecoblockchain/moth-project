#include "shared.h"

// read values from IMU
void imuRead() {
  int i;
  for (i = 0; i < 4; i++) {
    imu_update();
    analog_update();
    usleep(9500); // Sleeping for ~40000 gets the timing right for the sonar
  }
}

// read data from sonar and imu
void* i2cRead() {
  uint8_t sonar_id = 0;
  uint8_t mask = 0x03;
  initSonar();

  while (1) {
    updateSonar(sonar_id);
    pingSonar(sonar_id + 2);
    imuRead();
    sonar_id = (sonar_id + 1) & mask;
  }

  return NULL;
}
