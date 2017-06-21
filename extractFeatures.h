#ifndef extractFeatures_h
#define extractFeatures_h

#include <stdio.h>
/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
/* for fabsf() */
#include <math.h>

void extractFeatures (float *x_data,
                      float *y_data,
                      float *z_data,
                      float *gx_data,
                      float *gy_data,
                      float *gz_data,
                      int n_samples,
                      double *time,
                      float *S_i,
                      int *n_Trough,
                      float **features
                      );

#endif
