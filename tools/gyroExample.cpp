#include <unistd.h>
#include <upm/adxl345.h>
#include <upm/hmc5883l.h>
#include <upm/itg3200.h>

int main() {

  int16_t *rot;
      float *ang;
      // Note: Sensor not supported on Intel Edison with Arduino breakout
      upm::Itg3200* gyro = new upm::Itg3200(1);
      while(true){
          gyro->update(); // Update the data
          rot = gyro->getRawValues(); // Read raw sensor data
          ang = gyro->getRotation(); // Read rotational speed (deg/sec)
          fprintf(stdout, "Raw: %6d %6d %6d\n", rot[0], rot[1], rot[2]);
          fprintf(stdout, "AngX: %5.2f\n", ang[0]);
          fprintf(stdout, "AngY: %5.2f\n", ang[1]);
          fprintf(stdout, "AngZ: %5.2f\n", ang[2]);
          fprintf(stdout, "Temp: %5.2f Raw: %6d\n", gyro->getTemperature(), gyro->getRawTemp());
          sleep(1);
      }

    return 0;
}
