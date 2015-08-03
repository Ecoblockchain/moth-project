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

void parse_rmc(char*);
void save_log_value(int, double, int);
void write_log_2_row(char* analog_buffer);
double getTimeOffset();

extern char filenames[3][130];
extern int files_open;
extern int keep_going;

#endif /* LOG_H_ */
