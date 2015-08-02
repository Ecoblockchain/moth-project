#ifndef IMU_H_
#define IMU_H_

#include <stdint.h>

#define ADXL345_I2C_ADDR 0x53
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_POWER_ON 0x08
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_16G 0x03
#define ADXL345_FULL_RES 0x08
#define ADXL345_XOUT_L 0x32

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

int imu_init();
int imu_update();

extern float acc_accel[3];
extern int16_t acc_rawaccel[3];

float gyro_getTemperature();
int16_t gyro_getRawTemp();
extern float gyro_angle[3];
extern int16_t gyro_rotation[3];


float mag_direction();
float mag_heading();
extern int16_t mag_coor[3];

#endif
