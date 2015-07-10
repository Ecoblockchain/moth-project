#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

volatile int gps;

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
	/* char *string = "$PMTK104*37\r\n"; */
	/* if (write(gps, string, 13) == -1) { */
	/* 	printf("error writing to gps\n"); */
	/* 	return 1; */
	/* } */

	char * c;
	while (1) {
		while (read(gps, c, 1) == -1);
			printf("READ: %c\n", c);
	}

	if (close(gps) == -1) {
		printf("error closing gps\n");
		return 1;
	}
	/* printf("success writing to gps\n"); */

	printf("Closed GPS connection\n");	
	return 0;
}
