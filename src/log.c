/*
 * log.c
 *
 *  Created on: Jan 28, 2015
 *     Authors: Allen Edwards, James Vaughan
 */

#include "defines.h"
#include "prototypes.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "log.h"

pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
FILE *fp;
char filename[50];
double log_array[LOG_ARRAY_MAX];
int file_open = 0;
char mark[20] = "";

double convert(double number, char direction){
        //printf ("%f.2 -> ",number);
        double latlong = number;
        int degrees = (int)(latlong / 100);
        latlong = latlong - degrees * 100; // these are minutes
        latlong = ( degrees + latlong / 60 );
        if ( direction == 'S'  || direction == 'W' ) latlong = -latlong;
        //printf("%.5f\n",latlong);
        return latlong;
}

double convert2(double number, char direction){
        if ( direction == 'S'  || direction == 'W' ) number = -number;
        //printf("%.5f\n",latlong);
        return number;
}


void save_log_value(int index, double data){
	if(index < 0 || index >= LOG_ARRAY_MAX)return;
	pthread_mutex_lock(&log_lock);
		//printf("** Saving %f.2 in %d\n",data, index);
		log_array[index] = data;
	pthread_mutex_unlock(&log_lock);
}


void open_file(char *time, char *date){
	if (file_open == 1) return;
	char timestamp[15];
	sprintf(timestamp,"20%c%c%c%c%c%c%c%c%c%c.0",date[4],date[5],date[2],date[3],date[0],date[1],time[0],time[1],time[2],time[3]);
	printf("ST Log File Opened  /home/root/log/log_%s\n",timestamp);
	char sys[25];
	strcpy(sys,"date ");
	strcat(sys,timestamp);
	system(sys);
	timestamp[12] = '\0';
	sprintf(filename,"/home/root/log/log_%s.txt",timestamp);
	fp=fopen(filename, "w");
	file_open = 1;
	fprintf(fp, "ROW\tDATE\tTIME\tRAW_AWA\tRAW_AWS\tRAW_BS\tRAW_HDG\tRAW_DEPTH\tRAW_HEEL\tRAW_LEEWAY\tRAW_TWA\tRAW_TWS\tCORRECTED_AWA\tCORRECTED_AWS\tCORRECTED_BS\tCORRECTED_HDG\tCORRECTED_DEPTH\tCORRECTED_HEEL\tCORRECTED_LEEWAY\tCORRECTED_TWS\tCORRECTED_TWA\tCORRECTED_TWD\tCALCULATED_TWS\tCALCULATED_TWA\tCALCULATED_TWD\tTWS\tTWA\tTWD\tTAC_TWD_CORRECTION\tTAC_TWS_CORRECTION\tLATITUDE\tLONGITUDE\tSOG\tCOG\tVARIATION\tCURRENT_KT\tCURRENT_DIR\tMARK\n");
	fclose(fp);
}

void write_log_row(){
	//printf("** Writing to Log Row in %s\n",filename);
	fp=fopen(filename, "a");
	static int j = 1;
	int i = 0;
	fprintf(fp,"%d\t",j++);
	pthread_mutex_lock(&log_lock);
		for (i = 0 ; i < LOG_ARRAY_MAX ; i++){
			fprintf(fp,"%0.6f\t",log_array[i]);
		}
		fprintf(fp,"%s\n",mark);
	pthread_mutex_unlock(&log_lock);
	fclose(fp);
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
    //printf("** %s\n",local_buffer);
    char *array[50];
    char sep[] = "*,";
    str_split(array, local_buffer+3, sep, 50);
	pthread_mutex_lock(&log_lock);
		log_array[TIME] = atof(array[1]);
		log_array[DATE] = atof(array[9]);
		log_array[LATITUDE] = convert(atof(array[3]),array[4][0]);
		log_array[LONGITUDE] = convert(atof(array[5]),array[6][0]);
		log_array[COG] = atof(array[8]);
		log_array[SOG] = atof(array[7]);
		log_array[VARIATION] = convert2(atof(array[10]),array[11][0]);
		//printf("** variation %s %s %.2f\n",array[10],array[11],log_array[VARIATION]);
	pthread_mutex_unlock(&log_lock);
	if (file_open == 0) open_file(array[1],array[9]);
	if (file_open == 1) {
		write_log_row();
		//printf("** Writing line to file\n");
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
