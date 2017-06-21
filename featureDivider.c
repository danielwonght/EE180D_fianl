#include "featureDivider.h"

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

/*
 *The function devideFeature put features in 3D pointer array
 *into individual 2D arrays that is required to select features
 *for each neural networks. There are features of 32 files stored
 in the 3D pointer array feature. The order of the files are as
 shown below:
 
 file1: run speed 1 train set
 file2. run speed 1 test set
 file3. run speed 2 train set
 file4. run speed 2 test set
 file5. run speed 3 train set
 file6. run speed 3 test set
 file7. run speed 4 train set
 file8. run speed 4 test set
 file9. walk speed 1 train set
 file10. walk speed 1 test set
 file11. walk speed 2 train set
 file12. walk speed 2 test set
 file13. walk speed 3 train set
 file14. walk speed 3 test set
 file15. walk speed 4 train set
 file16. walk speed 4 test set
 file17. turn left train set
 file18. turn left test set
 file19. turn right train set
 file20. turn right test set
 file21. stair up speed 1 train set
 file22. stair up speed 1 test set
 file23. stair up speed 2 train set
 file24. stair up speed 2 test set
 file25. stair up speed 3 train set
 file26. stair up speed 3 test set
 file27. stair down speed 1 train set
 file28. stair down speed 1 test set
 file29. stair down speed 2 train set
 file30. stair down speed 2 test set
 file31. stair down speed 3 train set
 file32. stair down speed 3 test set
 
 For simplyfication, the index of file will be directly used in
 for loops below without specifically declared as variables.
 */

void divideFeature(float ***indv_feature,
                   float ***feature,
                   int *n_Feature,
                   int *n_indv_feature,
                   float **op,
                   int numFeatures,
                   int numFiles
                   )
{
    int largerFeatureSize = 10000;
    
    int i, j, k;
    int *all_mark[14];
    int mark_all_train[5], mark_all_test[5];
    int mark_run_train[5], mark_run_test[5];
    int mark_walk_train[5], mark_walk_test[5];
    int mark_turn_train[4], mark_turn_test[4];
    int mark_stair_train[4], mark_stair_test[4];
    int mark_up_train[4], mark_up_test[4];
    int mark_down_train[4], mark_down_test[4];
    
    int position_run_train[4] = {0, 2, 4, 6};
    int position_walk_train[4] = {8, 10, 12, 14};
    int position_turn_train[2] = {16, 18};
    int position_up_train[3] = {20, 22, 24};
    int position_down_train[3] = {26, 28, 30};
    
    int IndvArrayFillOrder[14] = {0,1,10,11,12,13,14,15,16,17,18,19,20,21};
    int FillFileOrder[14] = {0,16,0,4,4,8,8,10,10,16,10,13,13,16};
    
    for (i = 0; i < 14; i++)
    {
        all_mark[i] = (int *) malloc(sizeof(int) * 5);
    }
    
    /*put all features of all files together.
     *this array is for 1st layer neural networks
     */
    
    for (j = 0; j < 7; j++)
    {
        for (i = 0; i < numFeatures; i++)
        {
            n_indv_feature[IndvArrayFillOrder[2*j]] = 0;
            n_indv_feature[IndvArrayFillOrder[2*j+1]] = 0;
            for (k = FillFileOrder[2*j]; k < FillFileOrder[2*j+1]; k++)
            {
                indv_feature[IndvArrayFillOrder[2*j]][i] = array_concat(indv_feature[IndvArrayFillOrder[2*j]][i],
                                                                        n_indv_feature[IndvArrayFillOrder[2*j]],
                                                                        feature[2*k][i], n_Feature[2*k],
                                                                        sizeof(float));
                indv_feature[IndvArrayFillOrder[2*j+1]][i] = array_concat(indv_feature[IndvArrayFillOrder[2*j+1]][i],
                                                                          n_indv_feature[IndvArrayFillOrder[2*j+1]],
                                                                          feature[2*k+1][i], n_Feature[2*k+1],
                                                                          sizeof(float));
                n_indv_feature[IndvArrayFillOrder[2*j]] += n_Feature[2*k];
                n_indv_feature[IndvArrayFillOrder[2*j+1]] += n_Feature[2*k+1];
            }
        }
    }
    
    /* copy features just selected into next 8 spaces in the 3D pointer since
     * there are 5 neural networks in the first layer and each needs a train
     * set and test set
     */
    
    for (i = 1; i < 5; i++)
    {
        for (k = 0; k < numFeatures; k++)
        {
            memcpy(indv_feature[i*2], indv_feature[0], n_indv_feature[0]*sizeof(float));
            memcpy(indv_feature[i*2+1], indv_feature[1], n_indv_feature[1]*sizeof(float));
            n_indv_feature[i*2] = n_indv_feature[0];
            n_indv_feature[i*2+1] = n_indv_feature[1];
        }
    }
    
    
    mark_run_train[0] = 0;
    mark_run_test[0] = 0;
    mark_walk_train[0] = 0;
    mark_walk_test[0] = 0;
    for (i = 0; i < 4; i++)
    {
        mark_run_train[i+1] = mark_run_train[i] + n_Feature[position_run_train[i]];
        mark_run_test[i+1] = mark_run_test[i] + n_Feature[position_run_train[i]+1];
        mark_walk_train[i+1] = mark_walk_train[i] + n_Feature[position_walk_train[i]];
        mark_walk_test[i+1] = mark_walk_test[i] + n_Feature[position_walk_train[i]+1];
    }
    
    mark_turn_train[0] = 0;
    mark_turn_test[0] = 0;
    for (i = 0; i < 2; i++)
    {
        mark_turn_train[i+1] = mark_turn_train[i] + n_Feature[position_turn_train[i]];
        mark_turn_test[i+1] = mark_turn_test[i] + n_Feature[position_turn_train[i]+1];
    }
    mark_turn_train[3] = largerFeatureSize;
    mark_turn_test[3] = largerFeatureSize;
    
    
    
    mark_up_train[0] = 0;
    mark_up_test[0] = 0;
    mark_down_train[0] = 0;
    mark_down_test[0] = 0;
    for (i = 0; i < 3; i++)
    {
        mark_up_train[i+1] = mark_up_train[i] + n_Feature[position_up_train[i]];
        mark_up_test[i+1] = mark_up_test[i] + n_Feature[position_up_train[i]+1];
        mark_down_train[i+1] = mark_down_train[i] + n_Feature[position_down_train[i]];
        mark_down_test[i+1] = mark_down_test[i] + n_Feature[position_down_train[i]+1];
    }
    
    mark_all_train[0] = 0;
    mark_all_train[1] = n_indv_feature[10];
    mark_all_train[2] = mark_all_train[1] + n_indv_feature[12];
    mark_all_train[3] = mark_all_train[2] + n_indv_feature[14];
    mark_all_train[4] = mark_all_train[3] + n_indv_feature[16];
    
    mark_all_test[0] = 0;
    mark_all_test[1] = n_indv_feature[11];
    mark_all_test[2] = mark_all_test[1] + n_indv_feature[13];
    mark_all_test[3] = mark_all_test[2] + n_indv_feature[15];
    mark_all_test[4] = mark_all_test[3] + n_indv_feature[17];
    
    mark_stair_train[0] = 0;
    mark_stair_train[1] = n_indv_feature[18];
    mark_stair_train[2] = mark_stair_train[1] + n_indv_feature[20];
    mark_stair_train[3] = largerFeatureSize;
    
    mark_stair_test[0] = 0;
    mark_stair_test[1] = n_indv_feature[19];
    mark_stair_test[2] = mark_stair_test[1] + n_indv_feature[21];
    mark_stair_test[3] = largerFeatureSize;
    
    all_mark[0]  = &mark_all_train[0];
    all_mark[1]  = &mark_all_test[0];
    all_mark[2]  = &mark_run_train[0];
    all_mark[3]  = &mark_run_test[0];
    all_mark[4]  = &mark_walk_train[0];
    all_mark[5]  = &mark_walk_test[0];
    all_mark[6]  = &mark_turn_train[0];
    all_mark[7]  = &mark_turn_test[0];
    all_mark[8]  = &mark_stair_train[0];
    all_mark[9]  = &mark_stair_test[0];
    all_mark[10] = &mark_up_train[0];
    all_mark[11] = &mark_up_test[0];
    all_mark[12] = &mark_down_train[0];
    all_mark[13] = &mark_down_test[0];

    /* start to fill ouput array*/
    for (k = 0; k < 4; k++)
    {
        for (i = 0; i < n_indv_feature[2*k]; i++)
        {
            if(i >= mark_all_train[k] && i < mark_all_train[k+1])
            {
                op[2*k][i] = 1.0f;
            }
            else
            {
                op[2*k][i] = -1.0f;
            }
        }
        
        for (i = 0; i < n_indv_feature[2*k+1]; i++)
        {
            if(i >= mark_all_test[k] && i < mark_all_test[k+1])
            {
                op[2*k+1][i] = 1.0f;
            }
            else
            {
                op[2*k+1][i] = -1.0f;
            }
        }
    }
    
    for (j = 0; j < 14; j++)
    {
        if (j != 8 && j != 9)
        {
            for (i = 0; i < n_indv_feature[j+8]; i++)
            {
                if (i >= all_mark[j][0] && i < all_mark[j][1])
                {
                    op[j+8][i] = 1.0f;
                }
                else if (i >= all_mark[j][1] && i < all_mark[j][2])
                {
                    op[j+8][i] = 2.0f;
                }
                else if (i >= all_mark[j][2] && i < all_mark[j][3])
                {
                    op[j+8][i] = 3.0f;
                }
                else
                {
                    op[j+8][i] = 4.0f;
                }
            }
        }
        else
        {
            for (i = 0; i < n_indv_feature[j+8]; i++)
            {
                if (i >= all_mark[j][0] && i < all_mark[j][1])
                {
                    op[j+8][i] = 1.0f;
                }
                else if (i >= all_mark[j][1] && i < all_mark[j][2])
                {
                    op[j+8][i] = -1.0f;
                }
                else
                {
                    exit(EXIT_FAILURE);
                }
        
            }
    
        }
    }
}





