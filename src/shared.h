#ifndef SHARED_H_
#define SHARED_H_

#include <stdint.h>
#include <mraa.h>

// gps
int str_split(char**, char*, char*, int);
int verify_nmea(char *);

// log file
void parse_rmc(char*);
void save_log_value(int, double, int);
void write_log();
void log_error(char *message);
void parseSonar(int index, int value);

// main
void* gpsRead();
void* spRead();
void* i2cRead();
void* imuRead();
void* logWriter();

// i2c
void i2c_init();
void i2c_read(int bus, uint8_t* data, int length);
uint8_t i2c_read_byte(int bus);
void i2c_write(int bus, uint8_t* data, int length);
void i2c_write_byte(int bus, uint8_t data);
void i2c_address(int bus, uint8_t address);

void initSonar();
void pingSonar(int bus);
void updateSonar(int bus);

void imu_init(int bus);
mraa_result_t acc_update();
mraa_result_t gyro_update();
void gyro_calibrate();
mraa_result_t mag_update();


#endif /* SHARED_H_ */
