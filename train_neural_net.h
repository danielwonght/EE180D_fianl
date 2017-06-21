#ifndef train_neural_net_h
#define train_neural_net_h


#include "fann.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void train_neural_net(char *ifile_feature_name, char *net_name, int N_INPUT, int N_OUTPUT);
#endif
