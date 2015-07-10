#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

volatile int gps;

// Initialize GPS and set baud rate
void initGps(){
	gps = open("/dev/ttyMFD1", O_RDWR);
	struct termios tio;
	tcgetattr(gps, &tio);
	cfsetspeed(&tio, B9600);
	cfmakeraw(&tio);
	tcsetattr(gps, TCSANOW, &tio);
}

int main() {
	initGps();
	char *c;
	
	// Continuously read in and print out data
	while (1) {
		while (read(gps, c, 1) == -1);
		printf("%c", *c);
	}
}
