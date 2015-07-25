#ifndef SHARED_H_
#define SHARED_H_

#include <stdint.h>
#include <mraa.h>

// gps
int str_split(char**, char*, char*, int);
int verify_nmea(char *);

// log file
extern struct timespec start_time, current_time;
void parse_rmc(char*);
void save_log_value(int, double, int);
void write_log();
void log_error(char *message);
void parseSonar(int index, int value);
void printError(char* message);

// main
void* gpsRead();
void* i2cRead();
void* logWriter();

// sonar
void initSonar();
mraa_result_t pingSonar(int bus);
mraa_result_t updateSonar(int bus);

// imu
void imu_init(int bus);
void imu_update();
mraa_result_t acc_update();
mraa_result_t gyro_update();
void gyro_calibrate();
mraa_result_t mag_update();

// analog/arduino
void arduino_init();
void analog_update();

#endif /* SHARED_H_ */
