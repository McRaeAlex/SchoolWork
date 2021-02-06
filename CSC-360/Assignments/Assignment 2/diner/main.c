#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define N 5 // This is the number of people and forks
// #define RUN_ONCE // Uncomment this line to run it once where they stop after eating

// Since each fork can only be used by one person at a time we use a mutex
pthread_mutex_t forks[N]; 

void* philospher_dine(void* num) {
    // These are indexs into the array of forks
    size_t name = *((size_t*)num);
    size_t second = name;
    size_t first = (name - 1) ? (name - 1) : N; // If its not 0 return name - 1 else return N (This just makes the table a cirlce)

    // If the fork on your left is greater than your right go for the right first
    if (second < first) {
        second = first;
        first = name;
    }

    while (true) {
        sleep(1);
        // try to get the lower value fork
        pthread_mutex_lock(&forks[first]);
        printf("I %ld picked up my first fork %ld\n", name, first);
        // try to get the higher value fork
        pthread_mutex_lock(&forks[second]);
        printf("I %ld picked up my second fork %ld\n", name, second);
        // eat
        printf("I %ld am eating!\n", name);
        sleep(1);
        // release higher
        pthread_mutex_unlock(&forks[second]);
        printf("I %ld put down up my second fork %ld\n", name, second);
        // release lower
        pthread_mutex_unlock(&forks[first]);
        printf("I %ld put down up my first fork %ld\n", name, first);

        #ifdef RUN_ONCE
        return 0;
        #endif
    }
}

int main(int argc, char const *argv[]) {
    // Setup table
    printf("Setting up the table!\n");
    for (size_t i = 0; i < N; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Welcome guests
    printf("Guests are arriving!\n");

    pthread_t philosphers[N];

    for (size_t i = 0; i < N; ++i) {
        printf("Guest %ld has arrived!\n", i);
        pthread_create(&philosphers[i], NULL, philospher_dine, (void*)(&i));
    }

    for (size_t i = 0; i < N; ++i) {
        pthread_join(philosphers[i], NULL);
    }

    return 0;
}
