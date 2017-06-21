#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>
#include <mraa/i2c.h>
#include <sys/time.h>
#include "extractStrides.h"
#include "extractFeatures.h"
#include "featureDivider.h"
#include "feature_filter.h"
#include "conversion.h"
#include "train_neural_net.h"
#include "test_network.h"
#include "floatfann.h"

#define BUFF_SIZE 1024

int main(int argc, char **argv) {

    const int numNeural = 11;
    const int numFeatures = 109;
    const int defaultDataSize = 200000;
    const int defaultFeatureSize = 10000;
    const float pk_threshold = 1.8;
    
    int numFiles = 0;
    int i, j, k, rv;
    FILE *fp, *fp2;
    char *line = NULL;
    char *fname, *ofname;
    size_t len = 0;
    ssize_t read;
    float ***features;
    
    //array of pointer to hold all features of all data of layer 1 Neural Network
    float ***indv_feature;
    float *op[numNeural*2]; //array of pointers that stored output required by each neural network.
    int n_indv_sample[numNeural*2]; // number of features in input data sets of the 11 neural networks.

    

    system("ls -1v file_*.csv > fnames.txt");
    fp = fopen("fnames.txt","r");
    printf("Opening \'fnames.txt\' to read each file.\n");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file\n");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        numFiles ++;
    }

    int n_Data[numFiles];
    int n_Trough[numFiles];
    int n_Feature[numFiles];
    
    float *S_i[numFiles]; //idx of troughs selected by the extract strides algorithm
    float *x[numFiles], *y[numFiles], *z[numFiles], *gx[numFiles], *gy[numFiles], *gz[numFiles];
    double *t1[numFiles], *t2[numFiles];
    
    //allocate memeory for pointers that store original data
    for (i = 0; i < numFiles; i++)
    {
        S_i[i] = (float *) malloc(sizeof(float) * defaultFeatureSize);
        x[i] = (float *) malloc(sizeof(float) * defaultDataSize);
        y[i] = (float *) malloc(sizeof(float) * defaultDataSize);
        z[i] = (float *) malloc(sizeof(float) * defaultDataSize);
        gx[i] = (float *) malloc(sizeof(float) * defaultDataSize);
        gy[i] = (float *) malloc(sizeof(float) * defaultDataSize);
        gz[i] = (float *) malloc(sizeof(float) * defaultDataSize);
        t1[i] = (double *) malloc(sizeof(double) * defaultDataSize);
        t2[i] = (double *) malloc(sizeof(double) * defaultDataSize);
    }
    
    /*allocate memory for *op[], the array of pointers that stored output required by each neural network.
     *set initial values of n_indv_sample[]
     */
    for (i = 0; i < numNeural*2; i++)
    {
        op[i] = (float *) malloc(sizeof(float) * defaultFeatureSize);
        n_indv_sample[i] = 0;
    }

    //allocate memory for 3d pointer array to store features of each file
    features = (float ***)malloc(numFiles*sizeof(float**));
    for (i = 0; i < numFiles; i++)
    {
        features[i] = (float **)malloc(numFeatures*sizeof(float *));
        for (j = 0; j < numFeatures; j++)
        {
            features[i][j] = (float *)malloc(defaultFeatureSize*sizeof(float *));
        }
    }
         
    //allocate memory for 3d pointer array to store features of each nerual network
    indv_feature = (float ***)malloc((2*numNeural)*sizeof(float **));
    for (i = 0; i < 2*numNeural; i++)
    {
        indv_feature[i] = (float **)malloc(numFeatures*sizeof(float *));
        for (j = 0; j < numFeatures; j++)
        {
            indv_feature[i][j] = (float *)malloc(defaultFeatureSize*sizeof(float *));
        }
    }
         
    
    ofname = (char *) malloc(sizeof(char) * BUFF_SIZE);
    
    rewind(fp);
    
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        
        printf("Discovered file \'%s\'", line);
        
        fname = line;
        fname[strlen(line)-1] = 0;
    
        printf("Attempting to read from file \'%s\'.\n", fname);
        fp2 = fopen(fname, "r");
        if (fp2 == NULL) {
            fprintf(stderr,
                    "Failed to read from file \'%s\'.\n",
                    fname
                    );
            exit(EXIT_FAILURE);
        }
        
        j = 0;
        read = getline(&line, &len, fp2); 
        while ((read= getline(&line, &len, fp2)) != -1) {
            rv = sscanf(line, "%lf,%lf,%f,%f,%f,%f,%f,%f\n", &t1[i][j], &t2[i][j], &x[i][j], &y[i][j], &z[i][j], &gx[i][j], &gy[i][j], &gz[i][j]);
            if (rv != 8) {
                fprintf(stderr,
                        "%s %d \'%s\'. %s.\n",
                        "Failed to read line",
                        i,
                        line,
                        "Exiting"
                        );
                exit(EXIT_FAILURE);
            }
            j++;
        }
        fclose(fp2);
        n_Data[i] = j;
        i++;
    }
    fclose(fp);
    
    for (i = 0; i < numFiles; i++)
    {
        extractStrides(gz[i], t2[i], n_Data[i], pk_threshold, &n_Trough[i], S_i[i]);
        extractFeatures(x[i], y[i], z[i], gx[i], gy[i], gz[i],
                        n_Data[i], t2[i], S_i[i], &n_Trough[i],
                        features[i]);
    }
    
    //fill the array with number of features of each file.
    for (i = 0; i < numFiles; i++)
    {
        n_Feature[i] = n_Trough[i] - 1;
    }
    
    divideFeature(indv_feature, features, &n_Feature[0], &n_indv_sample[0],
                  op, numFeatures, numFiles);
    
    
    /* feature selection */
    
    // preset features
    int num_prefeature = 11;
    int *preset_feature_idx[num_prefeature];
    int temp_0[] = {29, 6};
    int temp_1[] = {1, 5, 7};
    int temp_2[] = {19, 99};
    int temp_3[] = {100, 11, 6, 1};
    int temp_4[] = {6, 7, 99};
    int temp_5[] = {44, 9, 92};
    int temp_6[] = {104, 0};
    int temp_7[] = {7};
    int temp_8[] = {13, 8, 2};
    int temp_9[] = {58, 22, 3, 0};
    int temp_10[] = {107, 0};
    int num_preset_feature[] = {2, 3, 2, 4, 3, 3, 2, 1, 3, 4, 2};
    preset_feature_idx[0] = &temp_0[0];
    preset_feature_idx[1] = &temp_1[0];
    preset_feature_idx[2] = &temp_2[0];
    preset_feature_idx[3] = &temp_3[0];
    preset_feature_idx[4] = &temp_4[0];
    preset_feature_idx[5] = &temp_5[0];
    preset_feature_idx[6] = &temp_6[0];
    preset_feature_idx[7] = &temp_7[0];
    preset_feature_idx[8] = &temp_8[0];
    preset_feature_idx[9] = &temp_9[0];
    preset_feature_idx[10] = &temp_10[0];
    
    /* train section */
    int *num_train_feature, *num_train_output;
    num_train_feature = (int *)malloc(sizeof(int) * numNeural);
    num_train_output = (int *)malloc(sizeof(int) * numNeural);
    float ***train_set = (float ***) malloc(sizeof(float**) * numNeural);
    float ***train_output = (float ***) malloc(sizeof(float**) * numNeural);
    int *idx_final_train[numNeural];
    for(i = 0; i < numNeural; i++)
    {
        idx_final_train[i] = (int *) malloc(sizeof(int) * numFeatures);
        train_set[i] = (float **) malloc(sizeof(float*) * numFeatures);
        train_output[i] = (float **) malloc(sizeof(float*) * 4);
        for(j = 0; j < numFeatures; j++)
        {
            train_set[i][j] = (float *) malloc(sizeof(float) * BUFF_SIZE);
        }
        for(j = 0; j < 4; j++)
        {
            train_output[i][j] = (float *) malloc(sizeof(float) * BUFF_SIZE);
        }
    }
    
    for(i = 0; i < numNeural; i++)
    {
        num_train_feature[i] = feature_filter(indv_feature[2*i],
                                              op[2*i],
                                              n_indv_sample[2*i],
                                              numFeatures,
                                              preset_feature_idx[i],
                                              train_set[i],
                                              idx_final_train[i],
                                              num_preset_feature[i]);
    }
    for(i = 0; i < numNeural; i++)
    {
        num_train_output[i] = conversion_one_to_multiple(op[2*i], 1, n_indv_sample[2*i], train_output[i]);
    }
    /* test section */
    int *num_test_feature, *num_test_output;
    num_test_feature = (int *)malloc(sizeof(int) * numNeural);
    num_test_output = (int *)malloc(sizeof(int) * numNeural);
    float ***test_set = (float ***) malloc(sizeof(float**) * numNeural);
    float ***test_output = (float ***) malloc(sizeof(float**) * numNeural);
    for(i = 0; i < numNeural; i++)
    {
        test_set[i] = (float **) malloc(sizeof(float*) * numFeatures);
        test_output[i] = (float **) malloc(sizeof(float*) * 4);
        for(j = 0; j < numFeatures; j++)
        {
            test_set[i][j] = (float *) malloc(sizeof(float) * BUFF_SIZE);
        }
        for(j = 0; j < 4; j++)
        {
            test_output[i][j] = (float *) malloc(sizeof(float) * BUFF_SIZE);
        }
    }
    for(i = 0; i < numNeural; i++)
    {
        num_test_feature[i] = feature_filter(indv_feature[2*i+1],
                                             op[2*i+1],
                                             n_indv_sample[2*i+1],
                                             -1,
                                             preset_feature_idx[i],
                                             test_set[i],
                                             idx_final_train[i],
                                             num_train_feature[i]);
    }
    for(i = 0; i < numNeural; i++)
    {
        num_test_output[i] = conversion_one_to_multiple(op[2*i+1], 1, n_indv_sample[2*i+1], test_output[i]);
    }
    
    /* indices for selected features */
        memset(ofname, 0, BUFF_SIZE);
        snprintf(ofname, 20, "feature_idx.csv");
        
        printf("Attempting to write to file \'%s\'.\n", ofname);
        fp = fopen(ofname, "w");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to write to file \'%s\'.\n",
                    ofname
                    );
            exit(EXIT_FAILURE);
        }
    
        for (i = 0; i < numNeural; i++) {
            for(j = 0; j < num_train_feature[i]-1; j++)
            {
                fprintf(fp, "%d ", idx_final_train[i][j]);
            }
            fprintf(fp, "%d\n", idx_final_train[i][num_train_feature[i]-1]);
        }
        fclose(fp);
    
    /* create train file */
    for (k = 0; k < numNeural; k++)
    {
        memset(ofname, 0, BUFF_SIZE);
        snprintf(ofname, 15, "train_%d.csv", k+1);
        
        printf("Attempting to write to file \'%s\'.\n", ofname);
        fp = fopen(ofname, "w");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to write to file \'%s\'.\n",
                    ofname
                    );
            exit(EXIT_FAILURE);
        }
        
        fprintf(fp, "%d %d %d\n",
                n_indv_sample[2*k],
                num_train_feature[k],
                num_train_output[k]
                );
        for (i = 0; i < n_indv_sample[2*k]; i++) {
            for(j = 0; j < num_train_feature[k]-1; j++)
            {
                fprintf(fp, "%lf ", train_set[k][j][i]);
            }
            fprintf(fp, "%lf\n", train_set[k][num_train_feature[k]-1][i]);
            
            for(j = 0; j < num_train_output[k]-1; j++)
            {
                fprintf(fp, "%d ", (int) train_output[k][j][i]);
            }
            fprintf(fp, "%d\n", (int) train_output[k][num_train_output[k]-1][i]);
        }
        fclose(fp);
    }
    
    /* create test file */
    for (k = 0; k < numNeural; k++)
    {
        memset(ofname, 0, BUFF_SIZE);
        snprintf(ofname, 15, "test_%d.csv", k+1);
        
        printf("Attempting to write to file \'%s\'.\n", ofname);
        fp = fopen(ofname, "w");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to write to file \'%s\'.\n",
                    ofname
                    );
            exit(EXIT_FAILURE);
        }
        
        fprintf(fp, "%d %d %d\n",
                n_indv_sample[2*k+1],
                num_test_feature[k],
                num_test_output[k]
                );
        for (i = 0; i < n_indv_sample[2*k+1]; i++) {
            for(j = 0; j < num_test_feature[k]-1; j++)
            {
                fprintf(fp, "%lf ", test_set[k][j][i]);
            }
            fprintf(fp, "%lf\n", test_set[k][num_test_feature[k]-1][i]);
            
            for(j = 0; j < num_test_output[k]-1; j++)
            {
                fprintf(fp, "%d ", (int) test_output[k][j][i]);
            }
            fprintf(fp, "%d\n", (int) test_output[k][num_test_output[k]-1][i]);
        }
        fclose(fp);
    }
    
    /* train neural network */
    char *netname;
    netname = (char *) malloc(sizeof(char) * BUFF_SIZE);
    for (k = 0; k < numNeural; k++)
    {
        memset(ofname, 0, BUFF_SIZE);
        memset(netname, 0, BUFF_SIZE);
        snprintf(ofname, 15, "train_%d.csv", k+1);
        snprintf(netname, 20, "network_%d.net", k+1);
        
        printf("Attempting to create network file \'%s\'.\n", netname);
        
        train_neural_net(ofname, netname, num_train_feature[k], num_train_output[k]);
    }
    
    
    /* test neural network */
    for (k = 0; k < numNeural; k++)
    {
        memset(ofname, 0, BUFF_SIZE);
        memset(netname, 0, BUFF_SIZE);
        snprintf(ofname, 15, "test_%d.csv", k+1);
        snprintf(netname, 20, "network_%d.net", k+1);
        
        printf("Attempting to testing network file \'%s\'.\n", netname);
		test_network(ofname,netname);
        
       
    }
    
}

        
        
        
        
    
    
