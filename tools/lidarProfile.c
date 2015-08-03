#include <urg_c/urg_sensor.h>
#include <urg_c/urg_utils.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

urg_t lidar;
long* length_data;
int length_data_size;
FILE* lidar_fp;

const char* dev = "/dev/ttyACM0";
const int baud = 115200;
int lidar_open = 0;

int lidar_init() {
	if (urg_open(&lidar, URG_SERIAL, dev, baud) < 0) {
		printf("ERROR: unable to open LIDAR\n");
		return -1;
	}

	length_data = (long*) malloc(sizeof(long) * urg_max_data_size(&lidar));

	lidar_open = 1;
	return 0;
}

int main() {
	lidar_init();
	static int k = 1;

	int i;
	for (i = 0; i < 100; i++) {
		lidar_fp = fopen("/root/lidarProfileTest", "w");

		if (urg_start_measurement(&lidar, URG_DISTANCE, 1, 0) < 0) {
			printf("ERROR: unable to start measurement on LIDAR\n");
		}

		length_data_size = urg_get_distance(&lidar, length_data, NULL);
		if (length_data_size <= 0) {
			printf("ERROR: unable to get distance data from LIDAR\n"); }

		fprintf(lidar_fp, "%i\t%0.2f", k++, 1.00);
		int i;
		for (i = 0; i < length_data_size; i++) {
			fprintf(lidar_fp, "\t%ld", length_data[i]);
		}
		fprintf(lidar_fp, "\n");

		fclose(lidar_fp);
	}
	return 0;
}
