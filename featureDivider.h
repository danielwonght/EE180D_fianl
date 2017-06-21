#ifndef featureDivider_h
#define featureDivider_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void divideFeature(float ***indv_feature,
                   float ***feature,
                   int *n_Feature,
                   int *n_indv_feature,
                   float **op,
                   int numFeatures,
                   int numFiles
                   );

#endif /* featureDivider_h */
