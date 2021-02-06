#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int jobs[] = {89, 29, 72, 45, 39, 2, 9, 23, 54, 85};
const int NUM_JOBS = 10;

void FCFS() {
    int time = 0;
    int head = 0;
    for (int i = 0; i < NUM_JOBS; ++i) {
        time += abs(head - jobs[i]);
        head = jobs[i];
        printf("Servicing %d\n", head);
    }
    printf("Time to run FCFS: %d\n", time);
}

void SSTF() {
    int time = 0;
    int head = 0;

    bool serviced[10] = {0};
    for (int i = 0; i < NUM_JOBS; ++i) {
        // Go through and find the closest then 
        int closest_val = abs(head - jobs[0]);
        int closest_index = 0;
        for (int j = 0; j < NUM_JOBS; ++j) {
            if (serviced[j] == false) {
                int val = abs(head - jobs[j]);
                if (val < closest_val) {
                    closest_val = val;
                    closest_index = j;
                }
            }
        }
        serviced[closest_index] = true;
        time += closest_val;
        head = jobs[closest_index];
        printf("Servicing %d\n", head);
    }

    printf("Time to run SSTF: %d\n", time);
}

int main() {
    printf("JOBS: %d\n", NUM_JOBS);
    FCFS();
    SSTF();
    return 0;
}