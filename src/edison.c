#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <mraa/gpio.h>
#include "switch.h"

pthread_t airmar_thread;
const char* airmar_file = "/dev/ttyUSB0";
char* filename; // TODO: make unique log names
int airmar_fd;
char buf[500];
FILE* file;

int open_log() {
  filename = "/home/root/log/log"; // TODO: make unique log names
  file = fopen(filename, "w");
  fprintf(file, "AIRMAR DATA\n"); // TODO: write first line
  fclose(file);
  return 0;
}

int write_log_row(char* string) {
  file = fopen(filename, "a");
  fprintf(file, "%s\n", string);
  fclose(file);
  return 0;
}

void* airmar_begin() {
  airmar_fd = open(airmar_file, O_RDWR);
  if (airmar_fd < 0) {
    printf("ERROR: couldn't open airmar dev\n");
    return NULL;
  }
  while (1) {
  // fill buf with info from airmar
  write_log_row(buf);
  usleep(1); // TODO: find out airmar hz
  }
}

int main() {
	int running = 0;
	switch_init();

	while (1) {
		if (switch_status() == 1) { // start/stop button
			if (!running) {
        pthread_create(&airmar_thread, NULL, airmar_begin, NULL);
				running = 1;
			}
		} else {
			if (running) {
        pthread_cancel(airmar_thread);
				running = 0;
			}
		}
		usleep(50000);
	}
	return 0;
}
