/*
 * gps.c
 *
 *      Authors: Allen Edwards and James Vaughan
 */

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "shared.h"
#include "log.h"

volatile int tty_fd_gps;

// used by verify_nmea
void btoh(unsigned char c, char *str) {
	unsigned char x;
	x = c >> 4;
	str[0] = x + (x > 9? 'A'-10: '0');
	x = c & 0x0f;
	str[1] = x + ( x > 9? 'A' - 10 : '0');
	str[2] = '\0';
}

// used to verify the validity of NMEA sentences
int verify_nmea(char *string){
	if (string[0] != '$')
		return -1;
	int length = strlen(string);
	if (length > 90) {
		printf("ER string  %s too long\n",string);
		return -1;
	}
	if (string[length-1]!= '\n') {
		printf("ER no newline\n");
		return -1;
	}
	if (string[length-2] != '\r') {
		printf("ER no return\n");
		return -1;
	}
	int p = 1; // start after the $, stop before the *
	char temp[3];
	char cs[3];
	cs[2] = '\0';
	cs[0] = string[length-4];
	cs[1] = string[length-3];
	temp[2]='\0';
	temp[1] = '\0';
	char c = '\0';
	while (string[p] != '*') {
		c = c ^ string[p++];
	}
	// compare the checksums
	btoh(c,temp);
	p = strcmp(cs,temp);
	if (p != 0) {
		printf("ER CHECKSUM ERROR in %s, %s != %s\n",string, cs, temp);
		return -1;
	}

	return 0;
}

// initialize the gps
void initGps() {
	char *dataSetting = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n\0";
	char *baudSetting = "$PMTK251,115200*1F\r\n\0";
	char *freqSetting = "$PMTK220,100*2F\r\n\0";
	tty_fd_gps = open("/dev/ttyMFD1", O_RDWR);
	struct termios tio;
	tcgetattr(tty_fd_gps, &tio);
	cfsetspeed(&tio, B9600);
	cfmakeraw(&tio);
	tcsetattr(tty_fd_gps, TCSANOW, &tio);
	write(tty_fd_gps, baudSetting, strlen(baudSetting));
	tcgetattr(tty_fd_gps, &tio);
	cfsetspeed(&tio, B115200);
	cfmakeraw(&tio);
	tcsetattr(tty_fd_gps, TCSADRAIN, &tio);
	write(tty_fd_gps, dataSetting, strlen(dataSetting));
	write(tty_fd_gps, freqSetting, strlen(freqSetting));
}

// read from the gps forever
void* gpsRead() {
	printf("GPS read is running\n");
	char aa;
	char local_buffer[500];
	int idx = 0;
	initGps();
	tcflush(tty_fd_gps, TCIFLUSH);
	int read_more = 0;

	while (1) {
		while (read(tty_fd_gps, &aa, 1) == -1 || (aa != '$' && read_more != 1));   // read 1 character from stream (blocking call)
		read_more = 1;
		if (aa != '\n') {
			local_buffer[idx++] = aa;
			if (idx > 400) {
				// prevent segmentation faults
				idx = 0;
				printf("\nGR ***********************************************\nlocal_buffer overflow in read nmea.\n Connect inputs and Cycle power to recover\n*****************************************\n");
				int flag = 1;
				char old = 'x';
				while (flag == 1) {
					read(tty_fd_gps, &aa, 1);
					if (aa == '\n'  && old == '\r') flag = 0;
					old = aa;
				}
			}
		} else {
			// at end of sentence
			read_more = 0;
			local_buffer[idx] = '\n';
			local_buffer[idx + 1] = '\0';
			idx = 0;
			if(verify_nmea(local_buffer) == 0) {
				if (strstr(local_buffer,",,,,,") && strstr(local_buffer, "RMC")){
					printf("ST GPS Not Ready %s\n",local_buffer);
				} else if (strstr(local_buffer, "RMC")) {
					// GPS Sentence
					printf("GPS: %s", local_buffer);
					parse_rmc(local_buffer);
				}
			}
		}
	}
}
