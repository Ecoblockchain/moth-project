#include <mraa/i2c.h>
#include <unistd.h>
#include <stdio.h>

int main() {
  mraa_i2c_context i2c;
  i2c = mraa_i2c_init(6);

  mraa_i2c_frequency(i2c, MRAA_I2C_STD);

  mraa_i2c_address(i2c, 0x68);

  uint8_t buf_out[2] = {0x3e, 0x80};
  mraa_i2c_write(i2c, buf_out, 2);

  while(1) {
  mraa_i2c_write_byte(i2c, 0x1b);
  uint8_t m_buffer[8];
  mraa_i2c_read(i2c, m_buffer, 8);
  int m_temperature;
  int m_rotation[3];


  //temp
    //
    m_temperature = (m_buffer[0] << 8 ) | m_buffer[1];
    // x
    m_rotation[0] = ((m_buffer[2] << 8 ) | m_buffer[3]);
    // y
    m_rotation[1] = ((m_buffer[4] << 8 ) | m_buffer[5]);
    // z
    m_rotation[2] = ((m_buffer[6] << 8 ) | m_buffer[7]);

  printf("%i\t%i\t%i\t%i\n", m_temperature, m_rotation[0], m_rotation[1], m_rotation[2]);
  sleep(1);
}
}
