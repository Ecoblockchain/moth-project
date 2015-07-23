#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_FIELDS 12

volatile int arduino;
int values[MAX_FIELDS];

void initArduino() {
  arduino = open("/dev/ttyMFD1", O_RDWR);
  struct termios tio;
  tcgetattr(arduino, &tio);
  cfsetspeed(&tio, B9600);
  cfmakeraw(&tio);
  tcsetattr(arduino, TCSANOW, &tio);
}

int readLine() {
  char c;
  int i = 0;
  char buffer[500];
  char *line;
  char *stop;
  int readMore = 0;

  while (1) {
    while (read(arduino, &c, 1) == -1 || (c != '$' && readMore != 1));
    if (c != '\n') {
      buffer[i] = c;
    } else {
      line = &buffer[1];
      for (i = 0; i < MAX_FIELDS; i++) {
        values[i] = strtol(line, &stop, 10);
        line = &stop[1];
      }
    return 0;
    }
  }

  return 0;
}

int printAll() {
  int i;
  for (i = 0; i < MAX_FIELDS; i++) {
    printf("%i\t", values[i]);
  }
  return 0;
}

int main() {
  initArduino();

  while (1) {
    readLine();
    printAll();
    usleep(10000);
  }
}
