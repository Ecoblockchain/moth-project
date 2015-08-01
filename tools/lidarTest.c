#include <urg_c/urg_sensor.h>
#include <urg_c/urg_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

urg_t lidar;
int ret;
long* length_data;
int length_data_size;

const char* dev = "/dev/ttyACM0";
const int baud = 115200;
char howlong[10000];

int main() {
	if (urg_open(&lidar, URG_SERIAL, dev, baud) < 0) {
		printf("ERROR: unable to open LIDAR\n");
		return 1;
	}

	while (1) {
		length_data = (long*) malloc(sizeof(long) * urg_max_data_size(&lidar));

		if (urg_start_measurement(&lidar, URG_DISTANCE, 1, 0) < 0) {
			printf("ERROR: unable to get measurement from LIDAR\n");
			return 1;
		}

		length_data_size = urg_get_distance(&lidar, length_data, NULL);
		if (length_data_size <= 0) {
			printf("ERROR: unable to get distance data from LIDAR\n");
			return 1;
		}

		int i;
		for (i = 0; i < length_data_size - 1; i++) {
		//		printf("%ld\t", length_data[i]);
		}
		//printf("%ld\n", length_data[length_data_size]);
		printf("%i\n", length_data_size);

		usleep(10000);
	}
}
