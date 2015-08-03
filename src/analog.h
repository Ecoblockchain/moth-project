#ifndef ANALOG_H_
#define ANALOG_H_

int analog_read(int num);
void* analog_aread();
int analog_init();
int analog_ainit();

extern char* analog_adev;

#endif
