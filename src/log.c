/*
 * log.c
 *
 *  Created on: Jan 28, 2015
 *     Authors: Allen Edwards, James Vaughan
 */

#include "prototypes.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "log.h"

pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
FILE *fp;
char filename[50];
double log_array[LOG_1_ARRAY_MAX];
int file_open = 0;
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


void save_log_value(int index, double data) {
	if(index < 0 || index >= LOG_1_ARRAY_MAX) return;
	pthread_mutex_lock(&log_lock);
		log_array[index] = data;
	pthread_mutex_unlock(&log_lock);
}

double get_log_value(int index) {
	if(index < 0 || index >= LOG_1_ARRAY_MAX) return 0;
	pthread_mutex_lock(&log_lock);
		return log_array[index];
	pthread_mutex_unlock(&log_lock);
}

void open_file(char *time, char *date){
	if (file_open == 1) return;
	char timestamp[15];
	sprintf(timestamp,"20%c%c%c%c%c%c%c%c%c%c",date[4],date[5],date[2],date[3],date[0],date[1],time[0],time[1],time[2],time[3]);
	printf("ST Log File Opened  /home/root/log/log_%s\n",timestamp);
	char sys[25];
	strcpy(sys,"date ");
	strcat(sys,timestamp);
	system(sys);
	timestamp[12] = '\0';
  struct stat st = {0};
  char folder[35];
  sprintf(folder, "/home/root/log/log_%s", timestamp);
  if (stat(folder, &st)) {
    mkdir(folder, 0700);
  }
	sprintf(filename,"%s/log_%s.txt", folder, timestamp);
	fp=fopen(filename, "w");
	file_open = 1;
	fprintf(fp, "ROW\tDATE\tTIME\tLATITUDE\tLONGITUDE\tSOG\tCOG\tSONAR_1\tSONAR_2\tSONAR_3\tSONAR_4\n");
	fclose(fp);
}

void write_log_row(){
	//printf("** Writing to Log Row in %s\n",filename);
	fp=fopen(filename, "a");
	static int j = 1;
	int i = 0;
	fprintf(fp,"%d\t",j++);
	pthread_mutex_lock(&log_lock);
		for (i = 0 ; i < LOG_1_ARRAY_MAX ; i++){
      if (i == LATITUDE || i == LONGITUDE) {
			  fprintf(fp,"%0.6f\t",log_array[i]);
      } else {
			  fprintf(fp,"%0.2f\t",log_array[i]);
      }
		}
		fprintf(fp,"%s\n",mark);
    //printf("%f\t%f\n", log_array[SONAR_1], log_array[SONAR_2]);
	pthread_mutex_unlock(&log_lock);
	fclose(fp);
}

void parseSonar(int index, int value) {
	if(index < 0 || index >= LOG_1_ARRAY_MAX)return;
	pthread_mutex_lock(&log_lock);
		log_array[index] = value;
	pthread_mutex_unlock(&log_lock);
}

void log_error(char *message) {
	fp = fopen(filename, "a");
	pthread_mutex_lock(&log_lock);
		fprintf(fp, "%s\n", message);
	pthread_mutex_lock(&log_lock);
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
	pthread_mutex_lock(&log_lock);
		log_array[TIME_1] = atof(array[1]);
		log_array[DATE] = atof(array[9]);
		log_array[LATITUDE] = convert(atof(array[3]),array[4][0]);
		log_array[LONGITUDE] = convert(atof(array[5]),array[6][0]);
		log_array[COG] = atof(array[8]);
		log_array[SOG] = atof(array[7]);
	pthread_mutex_unlock(&log_lock);
	if (file_open == 0) open_file(array[1],array[9]);
	if (file_open == 1) {
		write_log_row();
	}
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
