#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_network.h"
#include "floatfann.h"

int test_network(char *ifile_name, char *network_name)
{
	FILE *fp;
	/* Generic variables */
	int i,j,n_line;
	int rv;
	
	int n_samples;
	int n_in;
	int n_out;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	
    fp = fopen(ifile_name, "r");
		
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to read from file \'%s\'.\n", 
				ifile_name
		       );
		exit(EXIT_FAILURE);
	}
	
	
	read = getline(&line, &len, fp); //discard header of file
	rv = sscanf(line, "%d %d %d\n", &n_samples,&n_in,&n_out);

	int flag = 0;
	if (n_out == 1)
	{
		n_out++;
		flag = 1;
	}
	
	/* start reading the data from the file into the data structures */
	i = 0;
	
	float *input;
	input = (float *) malloc(sizeof(float) * n_in);
	
	int *group;
	group = (int *) malloc(sizeof(int) * n_out);
	
	char *eptr;
	fann_type *calc_out;
    fann_type input_feature[n_in];
	struct fann *ann;
	ann = fann_create_from_file(network_name);
	
	int row,col;
	int max;
	row = 0;
	col = 0;
    int matrix[n_out][n_out];

	
	for (i = 0; i < n_out; i++)
		for (j = 0; j < n_out; j++)
            matrix[i][j] = 0; 
	
	if (flag)
		n_out--;
	
	
	for(n_line = 0; n_line < n_samples; n_line++){
		read = getline(&line, &len, fp);
		for (i = 0 ; i != n_in ; i++) {
			input[i] = strtof(line, &eptr);
			input_feature[i] = input[i];
			line = eptr;
		}
		
		line = NULL;
		len = 0;
		read = getline(&line, &len, fp);
		for (i = 0 ; i < n_out ; i++) {
			group[i] = strtof(line, &eptr);
			line = eptr;
			if(group[i] == 1)
				row = i;
			if(flag && group[0] == 1)
				row = 0;
			if(flag && group[0] == -1)
				row = 1;
		}
		
		line = NULL;
		len = 0;
		calc_out = fann_run(ann, input_feature);
		max = -100;
		if (!flag)
		{
			for (i = 0; i < n_out; i++) {
				if (calc_out[i] > max) {
					max = calc_out[i];
					col = i;
				}
			}
		}
		
		if(flag)
		{
			if (calc_out[0] > 0)
				col = 0;
			if (calc_out[0] < 0)
				col = 1;
		}
		
	
		matrix[row][col]++;
	}
	
	
	if (flag)
		n_out++;
	
	for (i = 0; i < n_out; i++)
	{
		for (j = 0; j < n_out; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	
	fclose(fp);	
	return 1;
}

