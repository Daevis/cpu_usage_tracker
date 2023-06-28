
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "header/analyzer.h"
#include "header/printer.h"
#include "header/reader.h"
#include <semaphore.h>
#include "header/globals.h"

#define NON_CPU_LINES 7

int main(int, char**){
    pthread_t thread_id[3];
    int cores=lines_to_read()-NON_CPU_LINES;
    struct threads_data thread_data;
   thread_data.number_of_cores=cores;

    if(sem_init(&thread_data.reader_send_ready,0,1) != 0)
     {
        printf("\n reader_send_ready init failed\n");
        return 1;
     }
      if(sem_init(&thread_data.anazyler_read_ready,0,1) != 0)
     {
        printf("\n anazyler_read_ready init failed\n");
        return 1;
     }
    pipe(thread_data.reader_analyzer);
    pipe(thread_data.analyzer_printer);

    
    

    printf("Hello, from cpu_usage_tracker!\n"); 
    pthread_create(&thread_id[0], NULL, read_data, &thread_data);
    pthread_create(&thread_id[1], NULL, analyze, &thread_data);
    pthread_join(thread_id[0],NULL);
    pthread_join(thread_id[1],NULL);
    

}
