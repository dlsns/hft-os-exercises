#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include<unistd.h>

int num_threads_finished = 0;
pthread_mutex_t num_threads_finished_mutex;
pthread_cond_t m_threads_finished_condition;

#define N 10
#define M 5

void* thread_main(void* arg) {
    int n = (int)((uintptr_t)arg);
    int seconds_to_sleep = (rand() % 10) + 1;
    printf("Thread %d is going to sleep for %d s\r\n", n, seconds_to_sleep);
    sleep(seconds_to_sleep);
    printf("Thread %d finished sleeping\r\n", n);
    
    pthread_mutex_lock(&num_threads_finished_mutex);
    num_threads_finished += 1;
    pthread_mutex_unlock(&num_threads_finished_mutex);
    if(num_threads_finished >= M)
        pthread_cond_signal(&m_threads_finished_condition);
    
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t threads[N];
    pthread_cond_init(&m_threads_finished_condition, NULL);
    pthread_mutex_init(&num_threads_finished_mutex, NULL);

    for(int n = 0; n < N; n++) {
        pthread_create(&threads[n], NULL, thread_main, (void*)((uintptr_t)n));
    }

    pthread_mutex_lock(&num_threads_finished_mutex);
    while(num_threads_finished < M) {
        pthread_cond_wait(&m_threads_finished_condition, &num_threads_finished_mutex);
    }
    pthread_mutex_unlock(&num_threads_finished_mutex);

}