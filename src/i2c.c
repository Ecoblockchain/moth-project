#include <mraa/i2c.h>
#include <unistd.h>
#include <stdio.h>
#include "log.h"
#include "prototypes.h"

#define MAX_SONARS 2

uint8_t sonar[] = {0x02, 0x70, 0x00, 0x00};
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

void* startSonar(void* id) {
  int myID = *((int*) id);
  printf("started thread with id: %i\n", myID);
  while (1) {
    pingSonar(myID);
    usleep(80000);
    updateSonar(myID);
    usleep(80000);
  }
}

void* sonarRead() {
  sleep(1);
	i2c = mraa_i2c_init(1);
  sleep(1);
  mraa_i2c_frequency(i2c, MRAA_I2C_STD);

  pthread_t sonarThreads[MAX_SONARS];
  int* ids[MAX_SONARS];
  int i;
  for (i = 0; i < MAX_SONARS; i++) {
    ids[i] = (int*) malloc(sizeof(int));
    *ids[i] = i;
    pthread_create(&sonarThreads[i], NULL, startSonar, (void*) ids[i]);
  }

  return NULL;
}
