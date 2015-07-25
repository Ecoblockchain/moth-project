#include <Wire.h>

#define SLAVE_ADDRESS 0x33
#define N_REGISTERS 8

byte registerMap[N_REGISTERS];
byte temp_registerMap[N_REGISTERS];
byte receivedCommand;

int values[4];
int updating;

void setup() {
  Serial.begin(9600);
  Wire.begin(0x33);

  Wire.onRequest(requestEvent);
  updating = 0;
}

void loop() {
  for (int i = 0; i < 4; i++) {
   Serial.print(analogRead(i));
   Serial.print("\t");
  }
  Serial.println();

  updateValues();
  storeData();

  delay(10);
}

void requestEvent() {
  while (updating == 1);
  Wire.write(registerMap, N_REGISTERS);
}

void updateValues() {
  for (int i = 0; i < 6; i++) {
    values[i] = analogRead(i);
  }
}

void storeData() {
  updating = 1;
  byte b = values[0] >> 8;
  registerMap[0] = b;
  b = values[0];
  registerMap[1] = b;
  
  b = values[1] >> 8;
  registerMap[2] = b;
  b = values[1];
  registerMap[3] = b;
  
  b = values[2] >> 8;
  registerMap[4] = b;
  b = values[2];
  registerMap[5] = b;
  
  b = values[3] >> 8;
  registerMap[6] = b;
  b = values[3];
  registerMap[7] = b;
  updating = 0;
}

