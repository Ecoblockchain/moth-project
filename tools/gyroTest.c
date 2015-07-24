#include <mraa/i2c.h>
#include <stdio.h>
#include <unistd.h>

  int m_temperature;
  int m_rotation[3];
  int m_offsets[3];
  mraa_i2c_context i2c;

void update() {
  mraa_i2c_address(i2c, 0x68);
  mraa_i2c_write_byte(i2c, 0x1b);
  uint8_t m_buffer[8];
  mraa_i2c_address(i2c, 0x68);
  read(((int*) i2c)[2], m_buffer, 8);


  //temp

  m_temperature = (m_buffer[0] << 8 ) | m_buffer[1];
  // x
  m_rotation[0] = ((m_buffer[2] << 8 ) | m_buffer[3]) + m_offsets[0];
  // y
  m_rotation[1] = ((m_buffer[4] << 8 ) | m_buffer[5]) + m_offsets[1];
  // z
  m_rotation[2] = ((m_buffer[6] << 8 ) | m_buffer[7]) + m_offsets[2];

  printf("%i\t%i\t%i\t%i\n", m_temperature, m_rotation[0], m_rotation[1], m_rotation[2]);
}

void calibrate() {
  int reads = 600;
  int delay = 4000; // 4 milliseconds
  int skip = 5; // initial samples to skip
  int temp[3] = {0};

  int i;
  for(i = 0; i < reads; i++){

    update();
    if (i > skip){
      int j;
      for (j = 0; j < 3; j++){
        temp[j] += m_rotation[j];
      }
    }
    usleep(delay);
  }

  for(i = 0; i < 3; i++){
    m_offsets[i] = (-1) * temp[i] / (reads - skip);
  }
}

int main() {
  i2c = mraa_i2c_init(0);
  mraa_i2c_address(i2c, 0x68);
  uint8_t buf_out[2] = {0x3e, 0x80};
  mraa_i2c_write(i2c, buf_out, 2);
  int *rot;

  calibrate();
  update();

  while(1){
      update(); // Update the data
      rot = m_rotation;  // Read rotational speed (deg/sec)
      fprintf(stdout, "Raw: %6d %6d %6d\n", rot[0], rot[1], rot[2]);
      sleep(1);
  }

  return 0;
}
