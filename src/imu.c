#include <unistd.h>
#include <math.h>
#include <mraa/i2c.h>
#include "shared.h"
#include "log.h"

//address and id
#define ADXL345_I2C_ADDR 0x53
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_POWER_ON 0x08
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_16G 0x03
#define ADXL345_FULL_RES 0x08
#define ADXL345_XOUT_L 0x32
#define DATA_REG_SIZE 6

#define ITG3200_I2C_ADDR 0x68
#define ITG3200_PWR_MGM 0x3E
#define ITG3200_RESET 0x80
#define ITG3200_TEMP_H 0x1B

#define HMC5883L_I2C_ADDR 0x1E
#define HMC5883L_CONF_REG_B 0x01
#define GA_1_3_REG 0x01 << 5
#define HMC5883L_MODE_REG 0x02
#define HMC5883L_CONT_MODE 0x00
#define HMC5883L_DATA_REG 0x03
#define HMC5883L_X_MSB_REG 0
#define HMC5883L_X_LSB_REG 1
#define HMC5883L_Z_MSB_REG 2
#define HMC5883L_Z_LSB_REG 3
#define HMC5883L_Y_MSB_REG 4
#define HMC5883L_Y_LSB_REG 5
#define SCALE_0_92_MG 0.92


mraa_i2c_context acc_context;
uint8_t acc_buffer[6];
float acc_offsets[6];
float acc_accel[6];
int16_t acc_rawaccel[6];

mraa_i2c_context gyro_context;
uint8_t gyro_buffer[8];
int16_t gyro_rotation[6];
int gyro_offsets[6];
float gyro_angle[6];
int gyro_temperature;

mraa_i2c_context mag_context;
uint8_t mag_rx_tx_buf[6];
int16_t mag_coor[3];
float mag_declination;

void acc_init(int bus) {
  //init bus and reset chip`
  acc_context = mraa_i2c_init(bus);
  mraa_i2c_address(acc_context, ADXL345_I2C_ADDR);

  acc_buffer[0] = ADXL345_POWER_CTL;
  acc_buffer[1] = ADXL345_POWER_ON;
  mraa_i2c_write(acc_context, acc_buffer, 2);

  acc_buffer[0] = ADXL345_DATA_FORMAT;
  acc_buffer[1] = ADXL345_16G | ADXL345_FULL_RES;
  mraa_i2c_write(acc_context, acc_buffer, 2);

  //2.5V sensitivity is 256 LSB/g = 0.00390625 g/bit
  //3.3V x and y sensitivity is 265 LSB/g = 0.003773584 g/bit, z is the same

  acc_offsets[0] = 0.003773584;
  acc_offsets[1] = 0.003773584;
  acc_offsets[2] = 0.00390625;

  acc_update();
}

float* acc_getAcceleration() {
  int i;
  for (i = 0; i < 3; i++) {
    acc_accel[i] = acc_rawaccel[i] * acc_offsets[i];
  }
  return &acc_accel[0];
}

int16_t* acc_getRawValues() {
  return &acc_rawaccel[0];
}

uint8_t acc_getScale() {

  uint8_t result;

  if (mraa_i2c_write_byte(acc_context, ADXL345_DATA_FORMAT) != MRAA_SUCCESS) {
    printf("ERROR: unable to write to accelerometer (1)\n");
  }

  result = mraa_i2c_read_byte(acc_context);

  return pow(2, (result & 0x03) + 1);
}

mraa_result_t acc_update() {
  mraa_result_t result;
  int bytesRead;
  result = mraa_i2c_write_byte(acc_context, ADXL345_XOUT_L);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to write to accelerometer (2)\n");
    return result;
  }
  bytesRead = mraa_i2c_read(acc_context, acc_buffer, DATA_REG_SIZE);
  if (bytesRead != DATA_REG_SIZE) {
    printf("ERROR: unable to read from accelerometer (3)\n");
    return result;
  }

  // x
  acc_rawaccel[0] = ((acc_buffer[1] << 8 ) | acc_buffer[0]);
  // y
  acc_rawaccel[1] = ((acc_buffer[3] << 8 ) | acc_buffer[2]);
  // z
  acc_rawaccel[2] = ((acc_buffer[5] << 8 ) | acc_buffer[4]);

  save_log_value(RAW_ACC_X, acc_rawaccel[0], 1);
  save_log_value(RAW_ACC_Y, acc_rawaccel[1], 1);
  save_log_value(RAW_ACC_Z, acc_rawaccel[2], 1);
  float* acc = acc_getAcceleration();
  save_log_value(ACC_X, acc[0], 1);
  save_log_value(ACC_Y, acc[1], 1);
  save_log_value(ACC_Z, acc[2], 1);

  return MRAA_SUCCESS;
}

mraa_result_t gyro_init(int bus) {
  //init bus and reset chip
  gyro_context = mraa_i2c_init(bus);
  mraa_i2c_address(gyro_context, ITG3200_I2C_ADDR);
  mraa_result_t result;

  gyro_buffer[0] = ITG3200_PWR_MGM;
  gyro_buffer[1] = ITG3200_RESET;
  result = mraa_i2c_write(gyro_context, gyro_buffer, 2);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to write to gyro (4)\n");
    return result;
  }

  gyro_calibrate();
  gyro_update();
  return MRAA_SUCCESS;
}

void gyro_calibrate() {
  int reads = 600;
  int delay = 4000; // 4 milliseconds
  int skip = 5; // initial samples to skip
  int temp[3] = {0};

  int i, j;
  for (i = 0; i < reads; i++) {
    gyro_update();
    if (i > skip) {
      for (j = 0; j < 3; j++) {
        temp[j] += gyro_rotation[j];
      }
    }
    usleep(delay);
  }

  for(i = 0; i < 3; i++) {
    gyro_offsets[i] = (-1) * temp[i] / (reads - skip);
  }
}

float gyro_getTemperature() {
  return 35.0 + (gyro_temperature + 13200.0) / 280.0;
}

float* gyro_getRotation() {
  int i;
  for(i = 0; i < 3; i++){
    gyro_angle[i] = gyro_rotation[i]/14.375;
  }
  return &gyro_angle[0];
}

int16_t* gyro_getRawValues() {
  return &gyro_rotation[0];
}

int16_t gyro_getRawTemp() {
  return gyro_temperature;
}

mraa_result_t gyro_update() {
  mraa_result_t result;
  result = mraa_i2c_write_byte(gyro_context, ITG3200_TEMP_H);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to write to gyro (5)\n");
    return result;
  }

  result = mraa_i2c_read(gyro_context, gyro_buffer, DATA_REG_SIZE);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to read from gyro (6)\n");
    return result;
  }

  //temp
  //
  gyro_temperature = (gyro_buffer[0] << 8 ) | gyro_buffer[1];
  // x
  gyro_rotation[0] = ((gyro_buffer[2] << 8 ) | gyro_buffer[3]) + gyro_offsets[0];
  // y
  gyro_rotation[1] = ((gyro_buffer[4] << 8 ) | gyro_buffer[5]) + gyro_offsets[1];
  // z
  gyro_rotation[2] = ((gyro_buffer[6] << 8 ) | gyro_buffer[7]) + gyro_offsets[2];

  save_log_value(RAW_TEMP, gyro_temperature, 1);
  save_log_value(TEMP, gyro_getTemperature(), 1);
  save_log_value(RAW_ANG_X, gyro_rotation[0], 1);
  save_log_value(RAW_ANG_Y, gyro_rotation[1], 1);
  save_log_value(RAW_ANG_Z, gyro_rotation[2], 1);
  float* ang = gyro_getRotation();
  save_log_value(ANG_X, ang[0], 1);
  save_log_value(ANG_Y, ang[1], 1);
  save_log_value(ANG_Z, ang[2], 1);

  return MRAA_SUCCESS;
}

mraa_result_t mag_init(int bus) {
  mag_context = mraa_i2c_init(bus);
  mraa_i2c_address(mag_context, HMC5883L_I2C_ADDR);
  mraa_result_t result;

  mag_rx_tx_buf[0] = HMC5883L_CONF_REG_B;
  mag_rx_tx_buf[1] = GA_1_3_REG;
  result = mraa_i2c_write(mag_context, mag_rx_tx_buf, 2);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to write to compass (7)\n");
    return result;
  }


  mag_rx_tx_buf[0] = HMC5883L_MODE_REG;
  mag_rx_tx_buf[1] = HMC5883L_CONT_MODE;
  result = mraa_i2c_write(mag_context, mag_rx_tx_buf, 2);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to read from compass (8)\n");
    return result;
  }

  mag_update();
  return MRAA_SUCCESS;
}

float mag_direction() {
  return atan2(mag_coor[1] * SCALE_0_92_MG, mag_coor[0] * SCALE_0_92_MG) + mag_declination;
}

float mag_heading() {
  float dir = mag_direction() * 180/M_PI;
  if(dir < 0){
    dir += 360.0;
  }
  return dir;
}

mraa_result_t mag_update() {
  mraa_result_t result;
  int bytesRead;
  result = mraa_i2c_write_byte(mag_context, HMC5883L_DATA_REG);
  if (result != MRAA_SUCCESS) {
    printf("ERROR: unable to write to compass (9)\n");
    return result;
  }
  bytesRead = mraa_i2c_read(mag_context, mag_rx_tx_buf, DATA_REG_SIZE);
  if (bytesRead != DATA_REG_SIZE) {
    printf("ERROR: unable to read from compass (10)\n");
    return result;
  }

  // x
  mag_coor[0] = (mag_rx_tx_buf[HMC5883L_X_MSB_REG] << 8 ) | mag_rx_tx_buf[HMC5883L_X_LSB_REG];
  // z
  mag_coor[2] = (mag_rx_tx_buf[HMC5883L_Z_MSB_REG] << 8 ) | mag_rx_tx_buf[HMC5883L_Z_LSB_REG];
  // y
  mag_coor[1] = (mag_rx_tx_buf[HMC5883L_Y_MSB_REG] << 8 ) | mag_rx_tx_buf[HMC5883L_Y_LSB_REG];

  save_log_value(RAW_COMPASS_X, mag_coor[0], 1);
  save_log_value(RAW_COMPASS_Y, mag_coor[1], 1);
  save_log_value(RAW_COMPASS_Z, mag_coor[2], 1);
  save_log_value(HEADING, mag_heading(), 1);
  save_log_value(DIRECTION, mag_direction(), 1);

  return MRAA_SUCCESS;
}

int16_t* mag_coordinates() {
  return &mag_coor[0];
}

void mag_set_declination(float dec) {
  mag_declination = dec;
}

float mag_get_declination() {
  return mag_declination;
}

void imu_init(int bus) {
  acc_init(bus);
  //gyro_init(bus);
  printf("gyro disabled\n");
  mag_init(bus);
}

void imu_update() {
  acc_update();
  //gyro_update();
  mag_update();
}
