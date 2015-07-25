#include <Wire.h>

byte registerMap[8];
int values[4];
int updating;

void setup() {
  Wire.begin(0x7f);
  Wire.onRequest(requestEvent);
  updating = 0;
}

void loop() {
  updateValues();
  storeData();
  delay(10);
}

void requestEvent() {
  while (updating == 1);
  Wire.write(registerMap, 8);
}

void updateValues() {
  for (int i = 0; i < 6; i++) {
    values[i] = analogRead(i);
  }
}

void storeData() {
  updating = 1;
  byte b;
  for (int i = 0; i < 4; i++) {
    b = values[i] >> 8;
    registerMap[i * 2] = b;
    b = values[i];
    registerMap[(i * 2) + 1] = b;
  }
  updating = 0;
}

