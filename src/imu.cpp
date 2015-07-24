#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <upm/adxl345.h>
#include <upm/hmc5883l.h>
#include <upm/itg3200.h>
extern "C" {
  #include "log.h"
  #include "shared.h"
}

void* imuRead() {
    std::cout << "entering imuRead!" << std::endl;
    int16_t *raw;
    int16_t *pos;
    int16_t *rot;
    float *acc;
    float *ang;

    upm::Adxl345* accel = new upm::Adxl345(0);
    //upm::Itg3200* gyro = new upm::Itg3200(0);
    upm::Hmc5883l* compass = new upm::Hmc5883l(0);

    while (true) {
        accel->update(); // Update the data
        raw = accel->getRawValues(); // Read raw sensor data
        acc = accel->getAcceleration(); // Read acceleration (g)
        save_log_value(ACC_X, acc[0], 1);
        save_log_value(ACC_Y, acc[1], 1);
        save_log_value(ACC_Z, acc[2], 1);
        save_log_value(RAW_ACC_X, raw[0], 1);
        save_log_value(RAW_ACC_Y, raw[1], 1);
        save_log_value(RAW_ACC_Z, raw[2], 1);

        /*
        gyro->update();
        rot = gyro->getRawValues();
        ang = gyro->getRotation();
        save_log_value(ANG_X, ang[0], 1);
        save_log_value(ANG_Y, ang[1], 1);
        save_log_value(ANG_Z, ang[2], 1);
        save_log_value(RAW_ANG_X, rot[0], 1);
        save_log_value(RAW_ANG_Y, rot[1], 1);
        save_log_value(RAW_ANG_Z, rot[2], 1);
        save_log_value(TEMP, gyro->getTemperature(), 1);
        save_log_value(RAW_TEMP, gyro->getRawTemp(), 1);
        */

        compass->update();
        pos = compass->coordinates();
        save_log_value(COMPASS_X, pos[0], 1);
        save_log_value(COMPASS_Y, pos[1], 1);
        save_log_value(COMPASS_Z, pos[2], 1);
        save_log_value(HEADING, compass->heading(), 1);
        save_log_value(DIRECTION, compass->direction(), 1);
    }
}
