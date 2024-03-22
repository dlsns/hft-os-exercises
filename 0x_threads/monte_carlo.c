#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

volatile uint64_t counter = 0;
pthread_mutex_t counter_mutex;
#define N 10
#define STEPS_PER_THREAD 1000 * 1000
#define MAX N * STEPS_PER_THREAD

void* thread_main(void* arg) {
    for(uint64_t i = 0; i < STEPS_PER_THREAD; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
}

int main(int argc, char** argv) {
    pthread_t threads[N];
    pthread_mutex_init(&counter_mutex, NULL);

    for(int n = 0; n < N; n++) {
        pthread_create(&threads[n], NULL, thread_main, (void*)((uintptr_t)n));
    }

    for(int n = 0; n < N; n++){
        pthread_join(threads[n], NULL);
    }

    printf("expected %u actual %lu\r\n", MAX, counter);
}