#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "twc_stretchy_buffer.h"

#define ARRAY_SIZE 100
#define NUM_THREADS 100
//#define TWO_THOUSAND_ONE

typedef struct {
    float date;
    float value;
} data_t;

/* array */ data_t *data;

void read_in_file() {
    FILE *csv = fopen("stremflow_time_series.csv", "r");
    if (!csv) {
        printf("Failed to open the file");
        exit(1);
    }

    char *buffer = malloc(sizeof(char) * 30);
    size_t size = sizeof(char) * 30;

    getline(&buffer, &size, csv); // remove the first

    while (true) {
        // Read in the line from the file
        if (getline(&buffer, &size, csv) == -1) {
            break;
        }

        // Parse the line
        char *date = strtok(buffer, ",");
        char *val = strtok(NULL, "\n");
        int value = atoi(val);

        // parse the date and convert to int then float
        int day = atoi(strtok(date, "/"));
        int month = atoi(strtok(NULL, "/"));
        int year = atoi(strtok(NULL, ""));

        #ifdef TWO_THOUSAND_ONE
        if (year != 2001) {
            continue;
        }
        #endif

        data_t temp;
        temp.date = (float)array_size(data);
        //(float)((10000 * year) + (100 * month) + day);
        temp.value = (float)value;

        // Add it to the list of them
        array_push(data, temp);
    }
    free(buffer);
    fclose(csv);
}

void get_dists(int first, int second, int n, float *result, float *slop,
               float *intercept) {
    data_t a = data[first];
    data_t b = data[second];

    // Create the line
    float slope = (b.value - a.value) / (b.date - a.date);
    float d = b.value - (slope * b.date);

    float sum = 0.0f;
    for (size_t i = 0; i < n && i < array_size(data); i++) {
        data_t temp = data[i];
        float dist = fabs(temp.value - (slope * temp.date + d));
        sum += dist;
    }

    //printf("Slope: %f Inter: %f SARS: %f\n", slope, d, sum);

    *slop = slope;
    *intercept = d;
    *result = sum;
}

struct params {
    int i;
    int j;
    int n;
    float *result;
    float *slope;
    float *intercept;
};

void *wrapper(void *param) {
    struct params *values = (struct params *)param;
    get_dists(values->i, values->j, values->n, values->result, values->slope,
              values->intercept);
    free(param);
    return 0;
}

void do_calc(int n) {
    size_t number_of_pairs = (n * (n - 1) / 2);
    pthread_t threads[number_of_pairs];
    float results[number_of_pairs];
    float slopes[number_of_pairs];
    float intercepts[number_of_pairs];

    int count = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            struct params *temp = malloc(sizeof(struct params));
            temp->i = i;
            temp->j = j;
            temp->n = n;
            temp->result = &results[count];
            temp->slope = &slopes[count];
            temp->intercept = &intercepts[count];
            if (count > NUM_THREADS) { // Basically we run out of threads so wait for the
                          // earlier threads
                pthread_join(threads[count - NUM_THREADS], NULL);
                threads[count - NUM_THREADS] = -1;
            }
            pthread_create(&threads[count], NULL, wrapper, temp);
            count++;
        }
    }

    for (size_t i = 0; i < number_of_pairs; ++i) {
        if (threads[i] != -1) {
            pthread_join(threads[i], NULL);
        }
    }

    float min = 10000000000000.0f;
    size_t index = 0;
    for (size_t i = 0; i < number_of_pairs; ++i) {
        if (results[i] < min) {
            min = results[i];
            index = i;
        }
    }

    printf("Best fit for n = %d has slope %f with intercept %f and sum of abs "
           "dists of %f\n",
           n, slopes[index], intercepts[index], min);
}

int main() {
    data = NULL;

    read_in_file();

    do_calc(array_size(data));

    array_free(data);
    return 0;
}
