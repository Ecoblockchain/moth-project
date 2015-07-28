#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

volatile int tty_fd_airmar;

// initialize the gps
void initAirmar() {
	tty_fd_airmar = open("/dev/ttyMFD1", O_RDWR);
	struct termios tio;

	// set Edison UART to 9600 baud
	tcgetattr(tty_fd_airmar, &tio);
	cfsetspeed(&tio, B9600);
	cfmakeraw(&tio);
	tcsetattr(tty_fd_airmar, TCSANOW, &tio);
}

int main() {
  initAirmar();
  char c;
  while (1) {
    while(read(tty_fd_airmar, &c, 1) == -1);
    printf("%c", c);
  }
}
