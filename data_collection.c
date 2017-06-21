#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <mraa/i2c.h>
#include <sys/time.h>
#include <termios.h>
#include <fcntl.h>
#include "LSM9DS0.h"
#include "data_collection.h"

#define MILLION 1000000.0f

#include <termios.h>
#include <stdio.h>

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

int kbhit()
{
          struct termios oldt, newt;
          int ch;
          int oldf;

          tcgetattr(STDIN_FILENO, &oldt);
          newt = oldt;
          newt.c_lflag &= ~(ICANON | ECHO);
          tcsetattr(STDIN_FILENO, TCSANOW, &newt);
          oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
          fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

          ch = getchar();

          tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
          fcntl(STDIN_FILENO, F_SETFL, oldf);

          if(ch != EOF)
          {
            ungetc(ch, stdin);
            return 1;
          }

          return 0;
}


double parse_tv(struct timeval *tv)
{
    return (double) tv->tv_sec + (double) tv->tv_usec/MILLION;
}

void data_collection(char *file_name)
{
	mraa_i2c_context accel, gyro;
	accel_scale_t 		a_scale = A_SCALE_16G;
    gyro_scale_t 		g_scale = G_SCALE_500DPS;
	
	
	data_t              accel_data, gyro_data;
	float a_res, g_res;
	
	FILE *fp;
	struct timeval 		tv_before, tv_after;
	
	float *b_x, *b_y, *b_z, *b_gx, *b_gy, *b_gz;
	double *b_t_before, *b_t_after;
	int buffer_size = 15000;
	int num_b = 0;//number of data in buffer in current round
    int i,fd;
	
    b_t_before = (double *) malloc(sizeof(double) * buffer_size);
    b_t_after = (double *) malloc(sizeof(double) * buffer_size);
    b_x = (float *) malloc(sizeof(float) * buffer_size);
    b_y = (float *) malloc(sizeof(float) * buffer_size);
    b_z = (float *) malloc(sizeof(float) * buffer_size);
    b_gx = (float *) malloc(sizeof(float) * buffer_size);
    b_gy = (float *) malloc(sizeof(float) * buffer_size);
    b_gz = (float *) malloc(sizeof(float) * buffer_size);	
	
	
	accel = accel_init();
	set_accel_scale(accel, a_scale);
	a_res = calc_accel_res(a_scale);
	
	gyro = gyro_init();
	set_gyro_scale(gyro, g_scale);
	g_res = calc_gyro_res(g_scale);
	
	fp = fopen(file_name, "w");

	
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				"data.csv"
		       );
		exit(EXIT_FAILURE);
	}
	
	
	fd = fileno(fp);
    flock(fd, LOCK_EX);
	
	fprintf(fp,"%s,%s,%s,%s,%s,%s,%s,%s\n",
                        "timestamp_before",
                        "timestamp_after",
                        "accel_x", "accel_y", "accel_z",
                        "gyro_x", "gyro_y", "gryo_z"
						);
	printf("Data collection starting. Press any key to Stop or Wait till it finish.\n");
	

	while (kbhit()) getch();         //clear buffer
	fflush (stdin);
	while(!kbhit() && num_b < buffer_size){
		
		
		gettimeofday(&tv_before, NULL);
        accel_data = read_accel(accel, a_res);
        gyro_data = read_gyro(gyro, g_res);
        gettimeofday(&tv_after, NULL);
		
		b_x[num_b] = accel_data.x;
        b_y[num_b] = accel_data.y;
        b_z[num_b] = accel_data.z;
        b_gx[num_b] = gyro_data.x;
        b_gy[num_b] = gyro_data.y;
        b_gz[num_b] = gyro_data.z;
        b_t_before[num_b] = parse_tv(&tv_before);
        b_t_after[num_b] = parse_tv(&tv_after);
		num_b++;
	}
	while (kbhit()) getch();         //clear buffer
		fflush (stdin);
	for (i = 0; i < num_b; i++)
    {
        fprintf(fp, "%20.10lf,%20.10lf,%8.4lf,%8.4lf,%8.4lf,%8.4lf,%8.4lf,%8.4lf\n", 
									b_t_before[i], b_t_after[i], b_x[i], b_y[i], b_z[i], b_gx[i], b_gy[i], b_gz[i]);
    }
            
	fclose(fp);
	
	
}

void concatenate_string(char *original, char *add)
{
   while(*original)
      original++;
 
   while(*add)
   {
      *original = *add;
      add++;
      original++;
   }
   *original = '\0';
}