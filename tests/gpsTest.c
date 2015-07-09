#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int gps;
	if ((gps = open("/dev/ttyMFD1", O_RDWR)) == -1) {
		printf("error opening gps\n");
		return 1;
	}
	char *string = "$PMTK104*37\r\n";
	if (write(gps, string, 13) == -1) {
		printf("error writing to gps\n");
		return 1;
	}
	if (close(gps) == -1) {
		printf("error closing gps\n");
		return 1;
	}
	printf("success\n");
	return 0;
}
