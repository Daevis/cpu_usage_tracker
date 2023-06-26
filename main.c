
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "header/analyzer.h"
#include "header/printer.h"
#include "header/reader.h"
#include <semaphore.h>

struct threads_data{
    pthread_mutex_t lock;
    sem_t semaphore;
    int reader_analyzer[2];
    int analyzer_printer[2];
}thread_data;

int main(int, char**){
    pthread_t thread_id[3];
    
    if(sem_init(&thread_data.semaphore,0,1) != 0)
     {
        printf("\n semaphore init failed\n");
        return 1;
     }
    pipe(thread_data.reader_analyzer);
    pipe(thread_data.analyzer_printer);
    if (pthread_mutex_init(&thread_data.lock, NULL) != 0)
     {
        printf("\n mutex init failed\n");
        return 1;
     }
    printf("Hello, from cpu_usage_tracker!\n"); 
    pthread_create(&thread_id[0], NULL, read_data, &thread_data);
    pthread_create(&thread_id[1], NULL, analyze, &thread_data);
    pthread_join(thread_id[0],NULL);
        pthread_join(thread_id[1],NULL);

}
