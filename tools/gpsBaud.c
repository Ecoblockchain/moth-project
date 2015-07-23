#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

void btoh(unsigned char c, char *str) {
	unsigned char x;
	x = c >> 4;
	str[0] = x + (x > 9? 'A'-10: '0');
	x = c & 0x0f;
	str[1] = x + ( x > 9? 'A' - 10 : '0');
	str[2] = '\0';
}

// initialize the gps
int main(int argc, char * argv[]) {
  int gps;
  char* temp;
  char* string;
  char* newBaud = "9600";
  if (argc > 1) {
    newBaud = argv[1];
  }
  sprintf(string, "PMTK251,%s*", newBaud);
  temp[1] = '\0';
  temp[2] = '\0';
  char c = '\0';
  int p = 0;
	while (string[p] != '*') {
		c = c ^ string[p++];
	}
	btoh(c,temp);
  char* baud;
  sprintf(baud, "$%s%s", string, temp);
  int length = 0;
  for (length = 0; baud[length] != '\0'; length++);
	gps = open("/dev/ttyMFD1", O_RDWR);
	struct termios tio;

	tcgetattr(gps, &tio);
	cfsetspeed(&tio, B4800);
	cfmakeraw(&tio);
	tcsetattr(gps, TCSADRAIN, &tio);
	write(gps, baud, length);

	tcgetattr(gps, &tio);
	cfsetspeed(&tio, B9600);
	cfmakeraw(&tio);
	tcsetattr(gps, TCSADRAIN, &tio);
	write(gps, baud, length);

	tcgetattr(gps, &tio);
	cfsetspeed(&tio, B19200);
	cfmakeraw(&tio);
	tcsetattr(gps, TCSADRAIN, &tio);
	write(gps, baud, length);

	tcgetattr(gps, &tio);
	cfsetspeed(&tio, B38400);
	cfmakeraw(&tio);
	tcsetattr(gps, TCSADRAIN, &tio);
	write(gps, baud, length);

	tcgetattr(gps, &tio);
	cfsetspeed(&tio, B57600);
	cfmakeraw(&tio);
	tcsetattr(gps, TCSADRAIN, &tio);
	write(gps, baud, length);

	tcgetattr(gps, &tio);
	cfsetspeed(&tio, B115200);
	cfmakeraw(&tio);
	tcsetattr(gps, TCSADRAIN, &tio);
	write(gps, baud, length);

  close(gps);
}
