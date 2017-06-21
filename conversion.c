#include "conversion.h"

#define BUFF_SIZE 1024

int conversion_one_to_multiple(float *output, int N_output, int N_SAMPLES, float **final_output)
// return number of output
{
    int i, j;
    for(j = 0; j < N_SAMPLES; j++)
    {
        if (output[j] > N_output)
        {
            N_output = output[j];
        }
    }
    if(N_output > 1)
    {
        for(i = 0; i < N_output; i++)
        {
            for(j = 0; j < N_SAMPLES; j++)
            {
                final_output[i][j] = -1;
            }
        }
        for(i = 0; i < N_output; i++)
        {
            for(j = 0; j < N_SAMPLES; j++)
            {
                final_output[(int) output[j]-1][j] = 1;
            }
        }
    } else {
        for(i = 0; i < N_SAMPLES; i++)
        {
            final_output[0][i] = output[i];
        }
    }
    return N_output;
}
