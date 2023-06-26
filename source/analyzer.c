#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../header/analyzer.h"
struct threads_data{
    pthread_mutex_t lock;
    sem_t semaphore;
    int reader_analyzer[2];
    int analyzer_printer[2];
};

void* analyze(void* thread_data1)
{
      struct threads_data *thread_data = (struct threads_data*)thread_data1;
char* ch[24];
     while(1)
    {
       sem_wait(&thread_data->semaphore);


 //   for(int i=0;i<3000;i++){
        while(1){
    int result = read(thread_data->reader_analyzer[0],&ch,192);
    if(result=1)
    {
    printf("odebrane dane %s",&ch);
    }
              sem_post(&thread_data->semaphore);

    }

    sleep(1);
    }
    printf("thread anal end");
}