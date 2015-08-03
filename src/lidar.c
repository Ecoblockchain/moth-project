#include <urg_c/urg_sensor.h>
#include <urg_c/urg_utils.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "lidar.h"
#include "analog.h"
#include "log.h"

urg_t lidar;
long* lidar_data;
char lidar_dev[20];
int lidar_baud = 115200;
int lidar_data_size;
FILE* lidar_fp;

int lidar_open = 0;

int lidar_init() {
	sprintf(lidar_dev, "/dev/ttyACM0");
	if (urg_open(&lidar, URG_SERIAL, lidar_dev, lidar_baud) < 0) {
		printf("ERROR: unable to open LIDAR\n");
		return -1;
	}
	printf("STATUS: lidar opened via urg_open\n");

	lidar_data = (long*) malloc(sizeof(long) * urg_max_data_size(&lidar));

	lidar_open = 1;
	printf("STATUS: initialized lidar\n");
	return 0;
}

void* lidar_begin() {
	while (files_open == 0 && lidar_open == 0);
	static int k = 1;
	while (1) 	{
		lidar_fp = fopen(filenames[2], "a");

		if (urg_start_measurement(&lidar, URG_DISTANCE, 1, 0) < 0) {
			printf("ERROR: unable to start measurement on LIDAR\n");
		}

		lidar_data_size = urg_get_distance(&lidar, lidar_data, NULL);
		if (lidar_data_size <= 0) {
			printf("ERROR: unable to get distance data from LIDAR\n");
		}

		fprintf(lidar_fp, "%i\t%0.2f", k++, getTimeOffset());
		int i;
		for (i = 0; i < lidar_data_size; i++) {
			fprintf(lidar_fp, "\t%ld", lidar_data[i]);
		}
		fprintf(lidar_fp, "\n");

		fclose(lidar_fp);
	}
}
