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
#include "decision_maker.h"
#define BUFF_SIZE 1024
#define ARRAY_CONCAT(TYPE, A, An, B, Bn) \
(TYPE *)array_concat((const void *)(A), (An), (const void *)(B), (Bn), sizeof(TYPE));

void *array_concat(const void *a, size_t an,
                   const void *b, size_t bn, size_t s)
{
    char *p = malloc(s * (an + bn));
    memcpy(p, a, an*s);
    memcpy(p + an*s, b, bn*s);
    return p;
}



sig_atomic_t volatile run_flag = 1;

void do_when_interrupted(int sig)
{
    if (sig == SIGINT)
        run_flag = 0;
}

int main() {
    
    const int numNeural = 11;
    const int numFeatures = 109;
    const int default_SAMPLE_Size = 100000;
    const int FeatureArrSize = 100;
    const float pk_threshold = 1.8;	// pk-threshold value
    const int maxNumSelectFeature = 20;
    
    int N_SAMPLES = 5000;
    int i, j, k, fd, zz;
    int rv;
    /* Variables for reading file line by line */
    char *ifile_name, *idx_file;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int N_Data;
    ifile_name = "data.csv";
    idx_file = "feature_idx.csv";
    
    float *x, *y, *z, *gx, *gy, *gz;
    double *t1, *t2;
    float *feature[numFeatures];
    int *selectedFeature[numNeural];
    int n_selFeature[numNeural];
    
    float *S_i; 	// indicies of the start of each trough
    int n_S; 	// number of troughs
    
    double *time_interval;
    
    int *result_running;
    int *result_runSpeed;
    
    t1 = (double *) malloc(sizeof(double) * default_SAMPLE_Size);
    t2 = (double *) malloc(sizeof(double) * default_SAMPLE_Size);
    x = (float *) malloc(sizeof(float) * default_SAMPLE_Size);
    y = (float *) malloc(sizeof(float) * default_SAMPLE_Size);
    z = (float *) malloc(sizeof(float) * default_SAMPLE_Size);
    gx = (float *) malloc(sizeof(float) * default_SAMPLE_Size);
    gy = (float *) malloc(sizeof(float) * default_SAMPLE_Size);
    gz = (float *) malloc(sizeof(float) * default_SAMPLE_Size);
    S_i = (float *) malloc(sizeof(float) * default_SAMPLE_Size);
    
    result_running = (int *) malloc(sizeof(int) * FeatureArrSize);
    result_runSpeed = (int *) malloc(sizeof(int) * FeatureArrSize);
    
    for (i = 0; i < numFeatures; i++)
    {
        feature[i] = (float *) malloc(sizeof(float) * FeatureArrSize);
    }
    
    for (i = 0; i < numNeural; i++)
    {
        selectedFeature[i] = (int *) malloc(sizeof(int) * maxNumSelectFeature);
    }
    
    /* read idx file */
    printf("Attempting to read from file \'%s\'.\n", idx_file);
    fp = fopen(idx_file, "r");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to read from file \'%s\'.\n",
                idx_file
                );
        exit(EXIT_FAILURE);
    }
    
    i = 0;
    const char s[2] = " ";
    char *token;
    zz = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        /* parse the data */
        token = strtok(line, s);
        i = 0;
        while(token != NULL)
        {
            rv = sscanf(token, "%d", &selectedFeature[zz][i]);
            if (rv != 1){
                fprintf(stderr,
                        "%s %d \'%s\'. %s.\n",
                        "Failed to read token in feature",
                        i,
                        token,
                        "Exiting"
                        );
                exit(EXIT_FAILURE);
            }
            token = strtok(NULL, s);
            i++;
        }
        n_selFeature[zz] = i;
        zz++;
    }
    
    
    /* run recognition */
    int N_FEATURE_RUN = n_selFeature[0];
    float *feature_run[N_FEATURE_RUN];
    for(i = 0; i < N_FEATURE_RUN; i++)
    {
        feature_run[i] = (float *) malloc(sizeof(float) * N_SAMPLES);
    }
    int *location_run;
    location_run = (int *) malloc(sizeof(int) * N_SAMPLES);
    /* walk recognition */
    int N_FEATURE_WALK = n_selFeature[1];
    float *feature_walk[N_FEATURE_WALK];
    for(i = 0; i < N_FEATURE_WALK; i++)
    {
        feature_walk[i] = (float *) malloc(sizeof(float) * N_SAMPLES);
    }
    int *location_walk;
    location_walk = (int *) malloc(sizeof(int) * N_SAMPLES);
    /* turn recognition */
    int N_FEATURE_TURN = n_selFeature[2];
    float *feature_turn[N_FEATURE_TURN];
    for(i = 0; i < N_FEATURE_TURN; i++)
    {
        feature_turn[i] = (float *) malloc(sizeof(float) * N_SAMPLES);
    }
    int *location_turn;
    location_turn = (int *) malloc(sizeof(int) * N_SAMPLES);
    /* stair recogniation */
    int N_FEATURE_STAIR = n_selFeature[3];
    float *feature_stair[N_FEATURE_STAIR];
    for(i = 0; i < N_FEATURE_STAIR; i++)
    {
        feature_stair[i] = (float *) malloc(sizeof(float) * N_SAMPLES);
    }
    int *location_stair;
    location_stair = (int *) malloc(sizeof(int) * N_SAMPLES);
    /* backup */
    int N_FEATURE_BACKUP = n_selFeature[4];
    float *feature_backup[N_FEATURE_BACKUP];
    for(i = 0; i < N_FEATURE_BACKUP; i++)
    {
        feature_backup[i] = (float *) malloc(sizeof(float) * N_SAMPLES);
    }
    int *location_backup;
    location_backup = (int *) malloc(sizeof(int) * N_SAMPLES);
    int N_BU;
    /* layer 1 */
    int *location_layer_1;
    location_layer_1 = (int *) malloc(sizeof(int) * N_SAMPLES);
    int N_R; // run
    int N_W; // walk
    int N_T; // turn
    int N_S; // stair
    /* layer 2 */
    /* run speed */
    int N_FEATURE_RUNSPEED = n_selFeature[5];
    float *feature_runspeed[N_FEATURE_RUNSPEED];
    int *location_runspeed;
    /* Walking speed */
    int N_FEATURE_WALKSPEED = n_selFeature[6];
    float *feature_walkspeed[N_FEATURE_WALKSPEED];
    int *location_walkspeed;
    /* Turning direction */
    int N_FEATURE_TURNDIR = n_selFeature[7];
    float *feature_turndir[N_FEATURE_TURNDIR];
    int *location_turndir;
    /* Stair ascent or descent */
    int N_UP;
    int N_DOWN;
    int N_FEATURE_AD = n_selFeature[8];
    int N_FEATURE_UPSPEED = n_selFeature[9];
    int N_FEATURE_DOWNSPEED = n_selFeature[10];
    float *feature_ad[N_FEATURE_AD], *feature_upspeed[N_FEATURE_UPSPEED],
    *feature_downspeed[N_FEATURE_DOWNSPEED];
    int *location_ad;
    /* layer 3 */
    /* Stair ascent speed */
    int N_FEATURE_US = n_selFeature[9];;
    float *feature_us[N_FEATURE_US];
    int *location_us;
    /* Stair descent speed */
    int N_FEATURE_DS = n_selFeature[10];;
    float *feature_ds[N_FEATURE_DS];
    int *location_ds;
    
    char *display[BUFF_SIZE];
    for(i = 0; i < BUFF_SIZE; i++)
    {
        display[i] = (char *)malloc(sizeof(char) * BUFF_SIZE);
    }
    
    printf("finish reading index file!\n");

    
    /* loop running */
    while(run_flag)
    {
        fp = fopen(ifile_name, "r");
        
        usleep(1000);
        
        if (fp != NULL) {
            
            fd = fileno(fp);
            flock(fd, LOCK_EX);
    
            /* count the number of lines in the file */
            read = getline(&line, &len, fp); //discard header of file
            N_Data = 0;
            while ((read = getline(&line, &len, fp)) != -1) {
                N_Data++;
            }
    
            /* go back to the start of the file so that the data can be read */
            rewind(fp);
            read = getline(&line, &len, fp); //discard header of file
    
            /* start reading the data from the file into the data structures */
            i = 0;
    
            while ((read = getline(&line, &len, fp)) != -1) {
                /* parse the data */
                rv = sscanf(line, "%lf,%lf,%f,%f,%f,%f,%f,%f\n", &t1[i], &t2[i], &x[i], &y[i], &z[i], &gx[i], &gy[i], &gz[i]);
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
        
                i++;
            }
            
            fclose(fp);
            
            remove(ifile_name);
            
            printf("removed one file!\n");
            j = extractStrides(gz, t2, N_Data, pk_threshold, &n_S, S_i);
            
            if (j == 0 || n_S == 1)
            {
                printf("No Strides detected\n");
                printf("---------------------------------\n");
                continue;
            }
            extractFeatures(x, y, z, gx, gy, gz, N_Data, t2, S_i, &n_S, feature);
            
            /************************/
            /* run recognition */
            N_SAMPLES = n_S - 1;
            for(i = 0; i < N_SAMPLES; i++)
            {
                for(j = 0; j < n_selFeature[0]; j++)
                {
                    feature_run[j][i] = feature[selectedFeature[0][j]][i];
                }
            }
            location_predictor("network_1.net", N_SAMPLES, N_FEATURE_RUN, 1, feature_run, location_run);

            /* walk recognition */
            for(i = 0; i < N_SAMPLES; i++)
            {
                for(j = 0; j < n_selFeature[1]; j++)
                {
                    feature_walk[j][i] = feature[selectedFeature[1][j]][i];
                }
            }
            location_predictor("network_2.net", N_SAMPLES, N_FEATURE_WALK, 1, feature_walk, location_walk);
            
            /* turn recognition */
            for(i = 0; i < N_SAMPLES; i++)
            {
                for(j = 0; j < n_selFeature[2]; j++)
                {
                    feature_turn[j][i] = feature[selectedFeature[2][j]][i];
                }
            }
            location_predictor("network_3.net", N_SAMPLES, N_FEATURE_TURN, 1, feature_turn, location_turn);
            
            /* stair recogniation */
            for(i = 0; i < N_SAMPLES; i++)
            {
                for(j = 0; j < n_selFeature[3]; j++)
                {
                    feature_stair[j][i] = feature[selectedFeature[3][j]][i];
                }
            }
            location_predictor("network_4.net", N_SAMPLES, N_FEATURE_STAIR, 1, feature_stair, location_stair);
            
            /* backup */
            combination_layer_1(location_run,
                                location_walk,
                                location_turn,
                                location_stair,
                                N_SAMPLES,
                                location_layer_1);
            N_BU = 0;
            j = 0;
            for(i = 0; i < N_SAMPLES; i++)
            {
                if(location_layer_1[i] == -1)
                {
                    for (k = 0; k < n_selFeature[4]; k++)
                    {
                        feature_backup[k][j] = feature[selectedFeature[4][k]][i];
                    }
                    N_BU++;
                    j++;
                }
            }
            location_predictor2("network_5.net", N_BU, N_FEATURE_BACKUP, 4, feature_backup, location_backup);
            
            j = 0;
            for(i = 0; i < N_SAMPLES; i++)
            {
                if(location_layer_1[i] == -1)
                {
                    location_layer_1[i] = location_backup[j];
                    j++;
                }
            }
            
            /***************************************/
            /** determine the location in layer 1 **/
            /***************************************/
            N_R = 0; // run
            N_W = 0; // walk
            N_T = 0; // turn
            N_S = 0; // stair
            for(i = 0; i < N_SAMPLES; i++)
            {
                switch(location_layer_1[i]){
                    case 1:
                        N_R++;
                        display[i] = "---Running---";
                        break;
                    case 2:
                        N_W++;
                        display[i] = "---Walking---";
                        break;
                    case 3:
                        N_T++;
                        display[i] = "---Making a turn---";
                        break;
                    case 4:
                        N_S++;
                        display[i] = "---Stair---";
                        break;
                    default:
                        display[i] = "---Error exists in Layer [1]---";
                }
            }
            
            /***************************************/
            /** determine the location in layer 2 **/
            /***************************************/
            
            /* Running speed */
            if(N_R != 0)
            {
                for(i = 0; i < N_FEATURE_RUNSPEED; i++)
                {
                    feature_runspeed[i] = (float *) malloc(sizeof(float) * N_R);
                }
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 1)
                    {
                        for (k = 0; k < n_selFeature[5]; k++)
                        {
                            feature_runspeed[k][j] = feature[selectedFeature[5][k]][i];
                        }
                        j++;
                    }
                }
                location_runspeed = (int *) malloc(sizeof(int) * N_R);
                location_predictor2("network_6.net", N_R, N_FEATURE_RUNSPEED, 4, feature_runspeed, location_runspeed);
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 1)
                    {
                        switch(location_runspeed[j]){
                            case 1:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---At Speed of lvl [1]---", strlen("---At Speed of lvl [1]---")+1, sizeof(char));
                                break;
                            case 2:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---At Speed of lvl [2]---", strlen("---At Speed of lvl [2]---")+1, sizeof(char));
                                break;
                            case 3:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---At Speed of lvl [3]---", strlen("---At Speed of lvl [3]---")+1, sizeof(char));
                                break;
                            case 4:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---At Speed of lvl [4]---", strlen("---At Speed of lvl [4]---")+1, sizeof(char));
                            break;
                            default:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---Error exists in Layer [2]---", strlen("---Error exists in Layer [2]---")+1, sizeof(char));
                        }
                        j++;
                    }
                }
                
                for(i = 0; i < N_FEATURE_RUNSPEED; i++)
                {
                    free(feature_runspeed[i]);
                }
                free(location_runspeed);
                
            }
            
            /* Walking speed */
            if(N_W != 0)
            {
                for(i = 0; i < N_FEATURE_WALKSPEED; i++)
                {
                    feature_walkspeed[i] = (float *) malloc(sizeof(float) * N_W);
                }
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 2)
                    {
                        for (k = 0; k < n_selFeature[6]; k++)
                        {
                            feature_walkspeed[k][j] = feature[selectedFeature[6][k]][i];
                        }
                        j++;
                    }
                }
                location_walkspeed = (int *) malloc(sizeof(int) * N_W);
                location_predictor2("network_7.net", N_W, N_FEATURE_WALKSPEED, 4, feature_walkspeed, location_walkspeed);
                
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 2)
                    {
                        switch(location_walkspeed[j]){
                            case 1:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [1]---", strlen("---At Speed of lvl [1]---")+1, sizeof(char));
                                break;
                            case 2:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [2]---", strlen("---At Speed of lvl [2]---")+1, sizeof(char));
                                break;
                            case 3:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [3]---", strlen("---At Speed of lvl [3]---")+1, sizeof(char));
                                break;
                            case 4:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [4]---", strlen("---At Speed of lvl [4]---")+1, sizeof(char));
                                break;
                            default:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---Error exists in Layer [2]---", strlen("---Error exists in Layer [2]---")+1, sizeof(char));
                        }
                        j++;
                    }
                }
                
                for(i = 0; i < N_FEATURE_WALKSPEED; i++)
                {
                    free(feature_walkspeed[i]);
                }
                free(location_walkspeed);
                
            }
            
            /* Turning direction */
            if(N_T != 0)
            {
                for(i = 0; i < N_FEATURE_TURNDIR; i++)
                {
                    feature_turndir[i] = (float *) malloc(sizeof(float) * N_T);
                }
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 3)
                    {
                        for (k = 0; k < n_selFeature[7]; k++)
                        {
                            feature_turndir[k][j] = feature[selectedFeature[7][k]][i];
                        }
                        j++;
                    }
                }
                location_turndir = (int *) malloc(sizeof(int) * N_T);
                location_predictor2("network_8.net", N_T, N_FEATURE_TURNDIR, 2, feature_turndir, location_turndir);
                
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 3)
                    {
                        switch(location_turndir[j]){
                            case 1:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---To the left---", strlen("---To the left---")+1, sizeof(char));
                                break;
                            case 2:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---To the right---", strlen("---To the right---")+1, sizeof(char));
                                break;
                            default:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---Error exists in Layer [2]---", strlen("---Error exists in Layer [2]---")+1, sizeof(char));
                        }
                        j++;
                    }
                }
                
                for(i = 0; i < N_FEATURE_TURNDIR; i++)
                {
                    free(feature_turndir[i]);
                }
                free(location_turndir);
                
            }
            
            /* Stair ascent or descent */
            N_UP = 0;
            N_DOWN = 0;
            if(N_S != 0)
            {
                for(i = 0; i < N_FEATURE_AD; i++)
                {
                    feature_ad[i] = (float *) malloc(sizeof(float) * N_S);
                }
                for(i = 0; i < N_FEATURE_UPSPEED; i++)
                {
                    feature_upspeed[i] = (float *) malloc(sizeof(float) * N_S);
                }
                for(i = 0; i < N_FEATURE_DOWNSPEED; i++)
                {
                    feature_downspeed[i] = (float *) malloc(sizeof(float) * N_S);
                }
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 4)
                    {
                        for (k = 0; k < n_selFeature[8]; k++)
                        {
                            feature_ad[k][j] = feature[selectedFeature[8][k]][i];
                        }
                        for (k = 0; k < n_selFeature[9]; k++)
                        {
                            feature_upspeed[k][j] = feature[selectedFeature[9][k]][i];
                        }
                        for (k = 0; k < n_selFeature[10]; k++)
                        {
                            feature_downspeed[k][j] = feature[selectedFeature[10][k]][i];
                        }
                        j++;
                    }
                }
                location_ad = (int *) malloc(sizeof(int) * N_S);
                location_predictor("network_9.net", N_S, N_FEATURE_AD, 1, feature_ad, location_ad);
                
                j = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 4)
                    {
                        switch(location_ad[j]){
                            case 1:
                            N_UP++;
                            display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---Ascent---", strlen("---Ascent---")+1, sizeof(char));
                            break;
                            case -1:
                            N_DOWN++;
                            display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---Descent---", strlen("---Descent---")+1, sizeof(char));
                            break;
                            default:
                            display[i] = array_concat(display[i], strlen(display[i]),
                                                      "---Error exists in Layer [2]---", strlen("---Error exists in Layer [2]---")+1, sizeof(char));
                        }
                        j++;
                    }
                }
                
                for(i = 0; i < N_FEATURE_AD; i++)
                {
                    free(feature_ad[i]);
                }
                
            }
            
            /***************************************/
            /** determine the location in layer 3 **/
            /***************************************/
            
            /* Stair ascent speed */
            if(N_UP != 0)
            {
                for(i = 0; i < N_FEATURE_US; i++)
                {
                    feature_us[i] = (float *) malloc(sizeof(float) * N_UP);
                }
                j = 0;
                for(i = 0; i < N_S; i++)
                {
                    if(location_ad[i] == 1)
                    {
                        for (k = 0; k < n_selFeature[9]; k++)
                        {
                            feature_us[k][j] = feature_upspeed[k][i];
                        }
                        j++;
                    }
                }
                /*int *location_us;*/
                location_us = (int *) malloc(sizeof(int) * N_UP);
                location_predictor2("network_10.net", N_UP, N_FEATURE_US, 3, feature_us, location_us);
                
                j = 0;
                k = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 4)
                    {
                        if(location_ad[j] == 1)
                        {
                            switch(location_us[k]){
                                case 1:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [1]---", strlen("---At Speed of lvl [1]---")+1, sizeof(char));
                                break;
                                case 2:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [2]---", strlen("---At Speed of lvl [2]---")+1, sizeof(char));
                                break;
                                case 3:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [3]---", strlen("---At Speed of lvl [3]---")+1, sizeof(char));
                                break;
                                default:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---Error exists in Layer [3]---", strlen("---Error exists in Layer [3]---")+1, sizeof(char));
                            }
                            k++;
                        }
                        j++;
                    }
                }
                
                for(i = 0; i < N_FEATURE_US; i++)
                {
                    free(feature_us[i]);
                }
                free(location_us);
                
            }
            
            /* Stair descent speed */
            if(N_DOWN != 0)
            {
                for(i = 0; i < N_FEATURE_DS; i++)
                {
                    feature_ds[i] = (float *) malloc(sizeof(float) * N_DOWN);
                }
                j = 0;
                for(i = 0; i < N_S; i++)
                {
                    if(location_ad[i] == -1)
                    {
                        for (k = 0; k < n_selFeature[10]; k++)
                        {
                            feature_ds[k][j] = feature_downspeed[k][i];
                        }
                        j++;
                    }
                }
                location_ds = (int *) malloc(sizeof(int) * N_DOWN);
                location_predictor2("network_11.net", N_DOWN, N_FEATURE_DS, 3, feature_ds, location_ds);
                
                j = 0;
                k = 0;
                for(i = 0; i < N_SAMPLES; i++)
                {
                    if(location_layer_1[i] == 4)
                    {
                        if(location_ad[j] == -1)
                        {
                            switch(location_ds[k]){
                                case 1:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [1]---", strlen("---At Speed of lvl [1]---")+1, sizeof(char));
                                break;
                                case 2:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [2]---", strlen("---At Speed of lvl [2]---")+1, sizeof(char));
                                break;
                                case 3:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---At Speed of lvl [3]---", strlen("---At Speed of lvl [3]---")+1, sizeof(char));
                                break;
                                default:
                                display[i] = array_concat(display[i], strlen(display[i]),
                                                          "---Error exists in Layer [3]---", strlen("---Error exists in Layer [3]---")+1, sizeof(char));
                            }
                            k++;
                        }
                        j++;
                    }
                }
                
                for(i = 0; i < N_FEATURE_DS; i++)
                {
                    free(feature_ds[i]);
                }
                free(location_ds);
                
            }
            
            /* FREE MEMORY*/
            if(N_S != 0)
            {
                free(location_ad);
                {
                    for(i = 0; i < N_FEATURE_DOWNSPEED; i++)
                    {
                        free(feature_downspeed[i]);
                    }
                    for(i = 0; i < N_FEATURE_UPSPEED; i++)
                    {
                        free(feature_upspeed[i]);
                    }
                }
            }
            
            for(i = 0; i < N_SAMPLES; i++)
            {
                printf("%s\n", display[i]);
            }
            printf("---------------------------------\n");
        }
    }
    return 0;
}
