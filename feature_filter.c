#include "feature_filter.h"

float compute_mean(float *arr, int n_sample)
{
    float total;
    int i;
    total = 0.0f;
    for (i = 0; i < n_sample; i++) {
        total += arr[i];
    }
    
    return total/((float) n_sample);
}

float compute_std(float *arr, float mean, int n_sample)
{
    int i;
    float total = 0.0f;
    for (i = 0; i < n_sample; i++) {
        total += pow((arr[i] - mean),2);
    }
    return sqrt(total/((float) n_sample));
}

float compute_correlation(float *arr1, float *arr2, int n_sample)
{
    int i;
    float total = 0.0f;
    float mean1, mean2, std1, std2;
    mean1 = compute_mean(arr1, n_sample);
    mean2 = compute_mean(arr2, n_sample);
    std1 = compute_std(arr1, mean1, n_sample);
    std2 = compute_std(arr2, mean2, n_sample);
    for (i = 0; i < n_sample; i++) {
        total += (arr1[i] - mean1)*(arr2[i]-mean2);
    }
    return total/(((float) n_sample) * std1 * std2);
}

float compute_merit(float cor, float intercor, int k)
{
    return (((float)k) * cor)/ sqrt(((float)k)+((float)k)*((float)(k-1))*intercor);
}

int feature_filter(float **feature_all,
                   float *op,
                   int N_SAMPLES,
                   int N_FEATURE,
                   int *idx_default,
                   float **filtered_feature,
                   int *idx_final,
                   int num_train_feature) // return the number of selected features
{
    int i, j, k, idx, num_ff;
    
    if(N_FEATURE == -1) // for test set
    {
        num_ff = num_train_feature;
        for(i = 0; i < num_ff; i++)
        {
            for(j = 0; j < N_SAMPLES; j++)
            {
                filtered_feature[i][j] = feature_all[idx_final[i]][j];
            }
        }
        return num_ff;
    }
    
    float *feature_correlation[N_FEATURE];
    for(i = 0; i < N_FEATURE; i++)
    {
        feature_correlation[i] = (float *)malloc(sizeof(float) * N_FEATURE);
    }
    
    for(i = 0; i < N_FEATURE; i++)
    {
        for(j = 0; j < N_FEATURE; j++)
        {
            if(i == j)
            {
                feature_correlation[i][j] = compute_correlation(feature_all[i], op, N_SAMPLES);
            } else {
                feature_correlation[i][j] = compute_correlation(feature_all[i], feature_all[j], N_SAMPLES);
            }
            if (feature_correlation[i][j] < 0)
            {
                feature_correlation[i][j] = 0.0f - feature_correlation[i][j]; // absolute correlation
            }
        }
    }
    
    /* filter features by CFS */
    float *cor_avg, *intercor_avg, *max_layer;
    int *idx_layer;
    cor_avg = (float *) malloc(sizeof(float) * N_FEATURE);
    intercor_avg = (float *) malloc(sizeof(float) * N_FEATURE);
    idx_layer = (int *) malloc(sizeof(int) * N_FEATURE); // index of feature selected at each layer
    max_layer = (float *) malloc(sizeof(float) * N_FEATURE); // max value at each layer
    for (i = 0; i < N_FEATURE; i++) // preset default value in each layer
    {
        idx_layer[i] = -1;
        max_layer[i] = 0.0f;
        cor_avg[i] = feature_correlation[i][i];
        intercor_avg[i] = 0.0f;
    }
    for(i = 0; i < N_FEATURE; i++) // find the strongest correlation
    {
        if (feature_correlation[i][i] > max_layer[0])
        {
            max_layer[0] = feature_correlation[i][i];
            idx_layer[0] = i;
        }
    }
    int multiplier = 0;
    for (j = 1; j < N_FEATURE; j++)
    {
        max_layer[j] = max_layer[j-1];
        for(i = 0; i < N_FEATURE; i++)
        {
            if(i == idx_layer[j-1]){
                cor_avg[i] = 0.0f;
            } else {
                cor_avg[i] = (cor_avg[idx_layer[j-1]]*((float) j)+feature_correlation[i][i])/((float) (j+1));
                intercor_avg[i] = intercor_avg[idx_layer[j-1]]*((float) multiplier)/ ((float) (multiplier+j));
                for(k = 0; k < j; k++)
                {
                    intercor_avg[i] += feature_correlation[idx_layer[k]][i]/ ((float)(multiplier+j));
                }
            }
            if (compute_merit(cor_avg[i], intercor_avg[i], j+1) > max_layer[j])
            {
                max_layer[j] = compute_merit(cor_avg[i], intercor_avg[i], j+1);
                idx_layer[j] = i;
            }
        }
        if (idx_layer[j] == -1)
        {
            break;
        }
        multiplier += j;
    }
    
    /* find highest layer index */
    for(i = 0; i < N_FEATURE; i++)
    {
        if(idx_layer[i] == -1)
        {
            idx = i - 1;
            break;
        }
    }
    
    if (max_layer[idx] >= 0.85)
    {
        num_ff = idx+1;
        for(i = 0; i < num_ff; i++)
        {
            for(j = 0; j < N_SAMPLES; j++)
            {
                filtered_feature[i][j] = feature_all[idx_layer[i]][j];
            }
        }
        memcpy(idx_final, idx_layer, sizeof(int) * num_ff);
    } else {
        num_ff = num_train_feature;
        for(i = 0; i < num_ff; i++)
        {
            for(j = 0; j < N_SAMPLES; j++)
            {
                filtered_feature[i][j] = feature_all[idx_default[i]][j];
            }
        }
        memcpy(idx_final, idx_default, sizeof(int) * num_ff);
    }
    printf("num_ff: %d\n",num_ff);
    return num_ff;
}
