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

#define BUFF_SIZE 1024

void main(){
	
	char ifile_name[100];
	char r;
	char c[100];
	int n_file;
	
	//ifile_name = (char *) malloc(sizeof(char) * BUFF_SIZE);
		
	n_file = 1;
	
	printf("File collection for infield training ready to start. Press Any Key to continue.\n");
	
	while (kbhit()) getch();         //clear buffer
	fflush (stdin);
	while(!kbhit());
	while (kbhit()) getch();         //clear buffer
	fflush (stdin);
	// 1
	r = 'N';
	do
	{
		printf("Collecting running speed 1 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 2
	r = 'N';
	do
	{
		printf("Collecting running speed 1 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 3
	r = 'N';
	do
	{
		printf("Collecting running speed 2 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 4
	r = 'N';
	do
	{
		printf("Collecting running speed 2 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 5
	r = 'N';
	do
	{
		printf("Collecting running speed 3 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 6
	r = 'N';
	do
	{
		printf("Collecting running speed 3 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 7
	r = 'N';
	do
	{
		printf("Collecting running speed 4 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 8
	r = 'N';
	do
	{
		printf("Collecting running speed 4 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 9
	r = 'N';
	do
	{
		printf("Collecting walking speed 1 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 10
	r = 'N';
	do
	{
		printf("Collecting walking speed 1 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 11
	r = 'N';
	do
	{
		printf("Collecting walking speed 2 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 12
	r = 'N';
	do
	{
		printf("Collecting walking speed 2 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 13
	r = 'N';
	do
	{
		printf("Collecting walking speed 3 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 14
	r = 'N';
	do
	{
		printf("Collecting walking speed 3 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 15
	r = 'N';
	do
	{
		printf("Collecting walking speed 4 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 16
	r = 'N';
	do
	{
		printf("Collecting walking speed 4 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 17
	r = 'N';
	do
	{
		printf("Collecting turn left for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 18
	r = 'N';
	do
	{
		printf("Collecting turn left for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 19
	r = 'N';
	do
	{
		printf("Collecting turn right for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 20
	r = 'N';
	do
	{
		printf("Collecting turn right for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 21
	r = 'N';
	do
	{
		printf("Collecting stairing up speed 1 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 22
	r = 'N';
	do
	{
		printf("Collecting stairing up speed 1 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 23
	r = 'N';
	do
	{
		printf("Collecting stairing up speed 2 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 24
	r = 'N';
	do
	{
		printf("Collecting stairing up speed 2 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 25
	r = 'N';
	do
	{
		printf("Collecting stairing up speed 3 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 26
	r = 'N';
	do
	{
		printf("Collecting stairing up speed 3 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 27
	r = 'N';
	do
	{
		printf("Collecting stairing down speed 1 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 28
	r = 'N';
	do
	{
		printf("Collecting stairing down speed 1 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 29
	r = 'N';
	do
	{
		printf("Collecting stairing down speed 2 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 30
	r = 'N';
	do
	{
		printf("Collecting stairing down speed 2 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 31
	r = 'N';
	do
	{
		printf("Collecting stairing down speed 3 for training. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	// 32
	r = 'N';
	do
	{
		printf("Collecting stairing down speed 3 for testing. Press any key to start.\n");
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		while(!kbhit());
		while (kbhit()) getch();         //clear buffer
		fflush (stdin);
		snprintf(c,100,"%d",n_file);
		strcpy(ifile_name,"file_");
		//printf("%c\n",c[0]);
		concatenate_string(ifile_name,c);
		strcat(ifile_name,".csv");
		data_collection(ifile_name);
		printf("File generated. Do you want to regenerate the file? (Y|N) \n");
		
		r = getchar();
		if (r == '\n') r = getchar();
		while(r != 'n' && r != 'N' && r != 'y' && r != 'Y')
		{
			printf("invalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n') r = getchar();
		}
	}while(r == 'Y' || r == 'y');
	n_file++;
	
	printf("Infield training finished!");
}
