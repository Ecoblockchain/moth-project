#include <unistd.h>
#include <upm/adxl345.h>
#include <upm/hmc5883l.h>
#include <upm/itg3200.h>

int main() {
    int16_t *raw;
    float *acc;

    upm::Adxl345* accel = new upm::Adxl345(1);

    while(true){
        accel->update(); // Update the data
        raw = accel->getRawValues(); // Read raw sensor data
        acc = accel->getAcceleration(); // Read acceleration (g)

        fprintf(stdout, "Current scale: 0x%2xg\n", accel->getScale());
        fprintf(stdout, "Raw: %6d %6d %6d\n", raw[0], raw[1], raw[2]);
        fprintf(stdout, "AccX: %5.2f g\n", acc[0]);
        fprintf(stdout, "AccY: %5.2f g\n", acc[1]);
        fprintf(stdout, "AccZ: %5.2f g\n\n", acc[2]);

        usleep(100000);
    }
    return 0;
}
