#ifndef feature_filter_h
#define feature_filter_h


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int feature_filter(float **feature_all,
                   float *op,
                   int N_SAMPLES,
                   int N_FEATURE,
                   int *idx_default,
                   float **filtered_feature,
                   int *idx_final,
                   int num_train_feature);

#endif
