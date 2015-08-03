/*
 * log.c
 *
 *  Created on: Jan 28, 2015
 *     Authors: Allen Edwards, James Vaughan
 */

#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "log.h"
#include "ultra.h"
#include "analog.h"
#include "imu.h"

pthread_mutex_t time_lock = PTHREAD_MUTEX_INITIALIZER;
struct timespec start_time, current_time;

FILE* fp[3];
char filenames[3][130];
double log_array[LOG_1_ARRAY_MAX];
int files_open = 0;
char mark[20] = "";

double convert(double number, char direction){
  double latlong = number;
  int degrees = (int)(latlong / 100);
  latlong = latlong - degrees * 100; // these are minutes
  latlong = ( degrees + latlong / 60 );
  if ( direction == 'S'  || direction == 'W' ) latlong = -latlong;
  return latlong;
}

double convert2(double number, char direction){
  if ( direction == 'S'  || direction == 'W' ) number = -number;
  return number;
}

double getTimeOffset() {
  pthread_mutex_lock(&time_lock);
    clock_gettime(CLOCK_MONOTONIC, &current_time);
  pthread_mutex_unlock(&time_lock);
  return (current_time.tv_sec - start_time.tv_sec) + (current_time.tv_nsec - start_time.tv_nsec) / 1E9;
}

void open_files(char *time, char *date){
	if (files_open == 1) return;
	char timestamp[15];
	sprintf(timestamp,"20%c%c%c%c%c%c%c%c%c%c",date[4],date[5],date[2],date[3],date[0],date[1],time[0],time[1],time[2],time[3]);
	printf("STATUS: Log File Opened  /root/log/log_%s\n",timestamp);
  struct stat st = {0};
  char folder[35];
  sprintf(folder, "/root/log/log_%s", timestamp);
  if (stat(folder, &st)) mkdir(folder, 0700);
	sprintf(filenames[0],"%s/log_%s_1_10hz.txt", folder, timestamp);
	sprintf(filenames[1],"%s/log_%s_2_100hz.txt", folder, timestamp);
	sprintf(filenames[2],"%s/log_%s_3_lidar.txt", folder, timestamp);
	fp[0]=fopen(filenames[0], "w");
	fp[1]=fopen(filenames[1], "w");
	fp[2]=fopen(filenames[2], "w");
	files_open = 1;
	fprintf(fp[0], "ROW\tDATE\tTIME\tLATITUDE\tLONGITUDE\tSOG\tCOG\tSONAR_1\tSONAR_2\tSONAR_3\tSONAR_4\n");
fprintf(fp[1], "ROW\tTIME\tACC_X\tACC_Y\tACC_Z\tHEADING\tDIRECTION\tANALOG_0\tANALOG_1\tANALOG_2\tANALOG_3\tANALOG_4\tRAW_ACC_X\tRAW_ACC_Y\tRAW_ACC_Z\tRAW_COMPASS_X\tRAW_COMPASS_Y\tRAW_COMPASS_Z\n");
fprintf(fp[2], "ROW\tTIME\tVALUES\n");
	fclose(fp[0]);
	fclose(fp[1]);
	fclose(fp[2]);
}

void write_log_1_row() {
	fp[0] = fopen(filenames[0], "a");
	static int j = 1;
  if (j == 1) clock_gettime(CLOCK_MONOTONIC, &start_time);
	int i = 0;
	fprintf(fp[0],"%d\t",j++);
	for (i = 0 ; i < LOG_1_ARRAY_MAX; i++){
    if (i == LATITUDE || i == LONGITUDE) {
		  fprintf(fp[0],"%0.6f\t",log_array[i]);
    } else if (i >= SONAR_1 && i <= SONAR_4) {
      fprintf(fp[0], "%d\t", ultra_getDistance(i - SONAR_1));
    } else {
		  fprintf(fp[0],"%0.2f\t",log_array[i]);
    }
	}
	fprintf(fp[0],"\n");
	fclose(fp[0]);
  ultra_pingAll();
}

void write_log_2_row(char* analog_buffer) {
  if (files_open == 0) return;
	fp[1] = fopen(filenames[1], "a");
	static int h = 1;
	if (fprintf(fp[1],"%d\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", h++, getTimeOffset(), acc_accel[0], acc_accel[1], acc_accel[2],  mag_heading(), mag_direction(), analog_read(0), analog_buffer, acc_rawaccel[0], acc_rawaccel[1], acc_rawaccel[2], mag_coor[0], mag_coor[1], mag_coor[2]) < 0) {
    printf("ERROR: couldn't write to log 2\n");
	  fclose(fp[1]);
    return;
  }
	fclose(fp[1]);
}

int str_split(char **array, char *buf, char *sep, int max){
  // takes string buf and breaks it up into sub strings which array will point at
  // at most max elements in array
  // input buf will be modified
  char *token;
  int i = 0;
  int size = 0;
  while ( ( i < max -1 ) && ((token = strsep(&buf,sep))!= NULL ) ) {
    array[i++] = token;
  }
  array[i] = NULL;  // set to null
  size = i;
  return size;
}

/***************************************************
 $--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh
 0) $--RMC
 1) Time (UTC)
 2) Status, V = Navigation receiver warning
 3) Latitude
 4) N or S
 5) Longitude
 6) E or W
 7) Speed over ground, knots
 8) Track made good, degrees true
 9) Date, ddmmyy
10) Magnetic Variation, degrees
11) E or W
12) Checksum
******************************************************/

void parse_rmc(char *buffer){
	char local_buffer[100];
  strcpy(local_buffer, buffer);
  char *array[50];
  char sep[] = "*,";
  str_split(array, local_buffer+3, sep, 50);
	log_array[TIME_1] = atof(array[1]);
	log_array[DATE] = atof(array[9]);
	log_array[LATITUDE] = convert(atof(array[3]),array[4][0]);
	log_array[LONGITUDE] = convert(atof(array[5]),array[6][0]);
	log_array[COG] = atof(array[8]);
	log_array[SOG] = atof(array[7]);
	if (files_open == 0) open_files(array[1],array[9]);
  write_log_1_row();
}
