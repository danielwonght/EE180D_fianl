#include "extractFeatures.h"

#define BUFF_SIZE 1024

/*normalize an array of data from accelerometer 
 *with a number of n array elements.*/
void normalize_accel_arr(float *arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        arr[i] = arr[i]/16.0f;
    }
}

/*normalize an array of data from gyroscope
 *with a number of n array elements.*/
void normalize_gyro_arr(float *arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        arr[i] = arr[i]/500.0f;
    }
}

/*Returns the mean of a segment in an array
 with first element's position at variable start 
 and last element at the position before variable 
 end*/
float arr_mean(float *arr, int start, int end)
{
    float total = 0.0;
    int i;
    for (i = start; i < end; i++)
    {
        total += arr[i];
    }
    return total/(float)(end-start+1);
}


/*Return the STD of a segment in an array
 with first element's position at variable start
 and last element at the position before variable
 end. The mean of the segment is varable mean*/
float std (float *arr, float mean, int start, int end)
{
    int i;
    float total = 0.0;
    int N = end - start;
    for (i = start; i < end; i++)
    {
        total += ((arr[i]-mean)*(arr[i]-mean))/N;
    }
    
    return sqrt(total);
}

/*Return the maximum of a segment in an array
 with first element's position at variable start
 and last element at the position before variable
 end*/
float findMax (float *arr, int start, int end)
{
    int i;
    float max = arr[start];
    for (i = start; i < end; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    
    return max;
}

/*Return the minimum of a segment in an array
 with first element's position at variable start
 and last element at the position before variable
 end*/
float findMin (float *arr, int start, int end)
{
    int i;
    float min = arr[start];
    for (i = start; i < end; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    
    return min;
}

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
                      )
{
    int i;
    int segSize; //the number of data in a segment
    int n_interval = *n_Trough - 1; //number of interval between strides
    
    float *x_norm, *y_norm, *z_norm, *gx_norm, *gy_norm, *gz_norm;
    
    x_norm = (float *) malloc(sizeof(float) * (n_samples));
    y_norm = (float *) malloc(sizeof(float) * (n_samples));
    z_norm = (float *) malloc(sizeof(float) * (n_samples));
    gx_norm = (float *) malloc(sizeof(float) * (n_samples));
    gy_norm = (float *) malloc(sizeof(float) * (n_samples));
    gz_norm = (float *) malloc(sizeof(float) * (n_samples));
    
    memcpy(x_norm, x_data, n_samples*sizeof(float));
    memcpy(y_norm, y_data, n_samples*sizeof(float));
    memcpy(z_norm, z_data, n_samples*sizeof(float));
    memcpy(gx_norm, gx_data, n_samples*sizeof(float));
    memcpy(gy_norm, gy_data, n_samples*sizeof(float));
    memcpy(gz_norm, gz_data, n_samples*sizeof(float));
    
    //normalize data on all 6 axis
    normalize_accel_arr(x_norm, n_samples);
    normalize_accel_arr(y_norm, n_samples);
    normalize_accel_arr(z_norm, n_samples);
    normalize_gyro_arr(gx_norm, n_samples);
    normalize_gyro_arr(gy_norm, n_samples);
    normalize_gyro_arr(gz_norm, n_samples);
    
    for (i = 0; i < n_interval; i++)
    {
        /*idx1 and idx2 are two markers that mark 
         the start and end of the array position 
         currently focusing on*/
        int idx1 = (int) S_i[i];
        int idx2 = (int) S_i[i+1];
        
        //Counts the current sub-segment
        int segNum = 1;
        
        //evenly divde a stride segment into 4 sub segments
        segSize = (idx2-idx1+1)/4;
        
        //loop through each sub-segment in a stride segment
        while (segNum != 5)
        {
            /*idx1 and idx2 are two markers that mark
             the start and end of the sub-segment
             currently focus on*/
            int start = idx1+(segNum-1)*segSize;
            int end;
            int rp = segNum-1; //reference postion for inserting data
            
            if (segNum == 4)
            {
                end = idx2;
            }
            else
            {
                end = idx1+segNum*segSize;
            }
            
            //extract mean of each sub-segment
            features[rp+4*0][i] = arr_mean(x_norm, start, end);
            features[rp+4*1][i] = arr_mean(gx_norm, start, end);
            features[rp+4*2][i] = arr_mean(y_norm, start, end);
            features[rp+4*3][i] = arr_mean(gy_norm, start, end);
            features[rp+4*4][i] = arr_mean(z_norm, start, end);
            features[rp+4*5][i] = arr_mean(gz_norm, start, end);
            
            //extract STD of each sub-segment
            features[rp+4*6][i] = std(x_norm, features[rp+4*0][i], start, end);
            features[rp+4*7][i] = std(gx_norm, features[rp+4*1][i], start, end);
            features[rp+4*8][i] = std(y_norm, features[rp+4*2][i], start, end);
            features[rp+4*9][i] = std(gy_norm,  features[rp+4*3][i], start, end);
            features[rp+4*10][i] = std(z_norm, features[rp+4*4][i], start, end);
            features[rp+4*11][i] = std(gz_norm, features[rp+4*5][i], start, end);
            
            //extract maximum and minimum of each sub-segment
            features[rp+4*12][i] = findMax(x_norm, start, end);
            features[rp+4*13][i] = findMin(x_norm, start, end);
            features[rp+4*14][i] = findMax(y_norm, start, end);
            features[rp+4*15][i] = findMin(y_norm, start, end);
            features[rp+4*16][i] = findMax(z_norm, start, end);
            features[rp+4*17][i] = findMin(z_norm, start, end);
            features[rp+4*18][i] = findMax(gx_norm, start, end);
            features[rp+4*19][i] = findMin(gx_norm, start, end);
            features[rp+4*20][i] = findMax(gy_norm, start, end);
            features[rp+4*21][i] = findMin(gy_norm, start, end);
            features[rp+4*22][i] = findMax(gz_norm, start, end);
            features[rp+4*23][i] = findMin(gz_norm, start, end);
            
            segNum++;
        }
        
        //extract mean of each stride segment
        features[96][i] = arr_mean(x_norm, idx1, idx2);
        features[97][i] = arr_mean(gx_norm, idx1, idx2);
        features[98][i] = arr_mean(y_norm, idx1, idx2);
        features[99][i] = arr_mean(gy_norm, idx1, idx2);
        features[100][i] = arr_mean(z_norm, idx1, idx2);
        features[101][i] = arr_mean(gz_norm, idx1, idx2);
        
        //extract STD of each stride segment
        features[102][i] = std(x_norm, features[96][i], idx1, idx2);
        features[103][i] = std(gx_norm, features[97][i], idx1, idx2);
        features[104][i] = std(y_norm, features[98][i], idx1, idx2);
        features[105][i] = std(gy_norm, features[99][i], idx1, idx2);
        features[106][i] = std(z_norm, features[100][i], idx1, idx2);
        features[107][i] = std(gz_norm, features[101][i], idx1, idx2);
        
        //extract time difference of each stride segment
        features[108][i] = time[idx2] - time[idx1];
    }
}















