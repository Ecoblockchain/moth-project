#include <unistd.h>
#include <upm/adxl345.h>
#include <upm/hmc5883l.h>
#include <upm/itg3200.h>

int main() {
    int16_t *raw;
    int16_t *rot;
    float *acc;
    float *ang;

    upm::Adxl345* accel = new upm::Adxl345(1);
    //upm::Itg3200* gyro = new upm::Itg3200(1);

    while(true){
        accel->update(); // Update the data
        //gyro->update(); // Update the data
        raw = accel->getRawValues(); // Read raw sensor data
        acc = accel->getAcceleration(); // Read acceleration (g)
        //rot = gyro->getRawValues();
        //ang = gyro->getRotation();

        printf("Current scale: 0x%2xg\t", accel->getScale());
        printf("Raw: %6d %6d %6d\t", raw[0], raw[1], raw[2]);
        printf("AccX: %5.2f g\t", acc[0]);
        printf("AccY: %5.2f g\t", acc[1]);
        printf("AccZ: %5.2f g\t", acc[2]);
        /*
        printf("Raw: %6d %6d %6d\t", rot[0], rot[1], rot[2]);
        printf("AngX: %5.2f\t", ang[0]);
        printf("AngY: %5.2f\t", ang[1]);
        printf("AngZ: %5.2f\t", ang[2]);
        printf("Temp: %5.2f Raw: %6d\t", gyro->getTemperature(), gyro->getRawTemp());
        */

        usleep(100000);
    }
    return 0;
}

/*
#include <unistd.h>
#include <upm/adxl345.h>
#include <upm/hmc5883l.h>
#include <upm/itg3200.h>

int main() {
    int16_t *raw;
    int16_t *pos;
    int16_t *rot;
    float *acc;
    float *ang;

    upm::Adxl345* accel = new upm::Adxl345(1);
    upm::Itg3200* gyro = new upm::Itg3200(1);
    upm::Hmc5883l* compass = new upm::Hmc5883l(1);

    compass->set_declination(0.2094);

    while(true){
        accel->update(); // Update the data
        compass->update(); // Update the data
        gyro->update(); // Update the data
        raw = accel->getRawValues(); // Read raw sensor data
        acc = accel->getAcceleration(); // Read acceleration (g)
        pos = compass->coordinates();
        rot = gyro->getRawValues();
        ang = gyro->getRotation();

        fprintf(stdout, "Raw: %6d %6d %6d\n", rot[0], rot[1], rot[2]);
        fprintf(stdout, "AngX: %5.2f\n", ang[0]);
        fprintf(stdout, "AngY: %5.2f\n", ang[1]);
        fprintf(stdout, "AngZ: %5.2f\n", ang[2]);
        fprintf(stdout, "Temp: %5.2f Raw: %6d\n\n", gyro->getTemperature(), gyro->getRawTemp());

        fprintf(stdout, "coor: %5d %5d %5d ", pos[0], pos[1], pos[2]);
        fprintf(stdout, "heading: %5.2f direction: %3.2f\n\n", compass->heading(), compass->direction());

        fprintf(stdout, "Current scale: 0x%2xg\n", accel->getScale());
        fprintf(stdout, "Raw: %6d %6d %6d\n", raw[0], raw[1], raw[2]);
        fprintf(stdout, "AccX: %5.2f g\n", acc[0]);
        fprintf(stdout, "AccY: %5.2f g\n", acc[1]);
        fprintf(stdout, "AccZ: %5.2f g\n\n", acc[2]);

        usleep(100000);
    }
    return 0;
}
*/
