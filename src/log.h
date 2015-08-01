#ifndef LOG_H_
#define LOG_H_

#define	DATE	0
#define	TIME_1	1
#define	LATITUDE	2
#define	LONGITUDE	3
#define	SOG	4
#define	COG	5
#define SONAR_1 6
#define SONAR_2 7
#define SONAR_3 8
#define SONAR_4 9
#define	LOG_1_ARRAY_MAX	10

#define TIME_2 0
#define ACC_X 1
#define ACC_Y 2
#define ACC_Z 3
#define ANG_X 4
#define ANG_Y 5
#define ANG_Z 6
#define TEMP 7
#define HEADING 8
#define DIRECTION 9
#define ANALOG_0 10
#define ANALOG_1 11
#define ANALOG_2 12
#define ANALOG_3 13
#define ANALOG_4 14
#define ANALOG_5 15
#define ANALOG_6 16
#define RAW_ACC_X 17
#define RAW_ACC_Y 18
#define RAW_ACC_Z 19
#define RAW_ANG_X 20
#define RAW_ANG_Y 21
#define RAW_ANG_Z 22
#define RAW_TEMP 23
#define RAW_COMPASS_X 24
#define RAW_COMPASS_Y 25
#define RAW_COMPASS_Z 26
#define	LOG_2_ARRAY_MAX	27

void parse_rmc(char*);
void save_log_value(int, double, int);
void* log_begin();
double getTime();

extern char filenames[3][100];

#endif /* LOG_H_ */
