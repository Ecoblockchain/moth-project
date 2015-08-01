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

pthread_mutex_t log_lock_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_lock_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t time_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t* log_locks[] = {&log_lock_1, &log_lock_2};
struct timespec start_time, current_time;

FILE* fp[3];
char filenames[3][130];
double log_arrays[3][LOG_2_ARRAY_MAX];
int files_open = 0;
int begun = 0;
int log_array_max[] = {LOG_1_ARRAY_MAX, LOG_2_ARRAY_MAX};
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

double getTime() {
  pthread_mutex_lock(&time_lock);
    clock_gettime(CLOCK_MONOTONIC, &current_time);
  pthread_mutex_unlock(&time_lock);
  return (current_time.tv_sec - start_time.tv_sec) + (current_time.tv_nsec - start_time.tv_nsec) / 1E9;
}


void save_log_value(int index, double data, int log) {
	if(index < 0 || index >= log_array_max[log]) return;
	pthread_mutex_lock(log_locks[log]);
		log_arrays[log][index] = data;
	pthread_mutex_unlock(log_locks[log]);
}

void open_files(char *time, char *date){
	if (files_open == 1) return;
	char timestamp[15];
	sprintf(timestamp,"20%c%c%c%c%c%c%c%c%c%c.0",date[4],date[5],date[2],date[3],date[0],date[1],time[0],time[1],time[2],time[3]);
	printf("STATUS: Log File Opened  /root/log/log_%s\n",timestamp);
	char sys[25];
	strcpy(sys,"date ");
	strcat(sys,timestamp);
	system(sys);
	timestamp[12] = '\0';
  struct stat st = {0};
  char folder[35];
  sprintf(folder, "/root/log/log_%s", timestamp);
  if (stat(folder, &st)) {
    mkdir(folder, 0700);
  }
	sprintf(filenames[0],"%s/log_%s_1_10hz.txt", folder, timestamp);
	sprintf(filenames[1],"%s/log_%s_2_100hz.txt", folder, timestamp);
	sprintf(filenames[2],"%s/log_%s_3_lidar.txt", folder, timestamp);
	fp[0]=fopen(filenames[0], "w");
	fp[1]=fopen(filenames[1], "w");
	fp[2]=fopen(filenames[2], "w");
	files_open = 1;
	fprintf(fp[0], "ROW\tDATE\tTIME\tLATITUDE\tLONGITUDE\tSOG\tCOG\tSONAR_1\tSONAR_2\tSONAR_3\tSONAR_4\n");
fprintf(fp[1], "ROW\tTIME\tACC_X\tACC_Y\tACC_Z\tANG_X\tANG_Y\tANG_Z\tTEMP\tHEADING\tDIRECTION\tANALOG_0\tANALOG_1\tANALOG_2\tANALOG_3\tANALOG_4\tANALOG_5\tANALOG_6\tRAW_ACC_X\tRAW_ACC_Y\tRAW_ACC_Z\tRAW_ANG_X\tRAW_ANG_Y\tRAW_ANG_Z\tRAW_TEMP\tRAW_COMPASS_X\tRAW_COMPASS_Y\tRAW_COMPASS_Z\n");
fprintf(fp[2], "ROW\tTIME\tVALUES\n");
	fclose(fp[0]);
	fclose(fp[1]);
	fclose(fp[2]);
}

void write_log_row(int log) {
	fp[log] = fopen(filenames[log], "a");
  if (begun == 0) {
	  clock_gettime(CLOCK_MONOTONIC, &start_time);
    begun = 1;
  }
	static int j[] = {1, 1};
	int i = 0;
	fprintf(fp[log],"%d\t",j[log]++);
	pthread_mutex_lock(log_locks[log]);
		for (i = 0 ; i < log_array_max[log] ; i++){
      if (log == 0 && (i == LATITUDE || i == LONGITUDE)) {
			  fprintf(fp[log],"%0.6f\t",log_arrays[log][i]);
      } else if (log == 1 && i == TIME_2) {
        fprintf(fp[log], "%0.2f\t", getTime());
      } else {
			  fprintf(fp[log],"%0.2f\t",log_arrays[log][i]);
      }
		}
		fprintf(fp[log],"\n");
	pthread_mutex_unlock(log_locks[log]);
	fclose(fp[log]);
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
	pthread_mutex_lock(log_locks[0]);
		log_arrays[0][TIME_1] = atof(array[1]);
		log_arrays[0][DATE] = atof(array[9]);
		log_arrays[0][LATITUDE] = convert(atof(array[3]),array[4][0]);
		log_arrays[0][LONGITUDE] = convert(atof(array[5]),array[6][0]);
		log_arrays[0][COG] = atof(array[8]);
		log_arrays[0][SOG] = atof(array[7]);
	pthread_mutex_unlock(log_locks[0]);
	if (files_open == 0) open_files(array[1],array[9]);
}

void* log_begin() {
  int i;
  while (files_open == 0);
  while (1) {
    write_log_row(0);
    for (i = 0; i < 10; i++) {
      write_log_row(1);
      usleep(10000);
    }
  }
}
