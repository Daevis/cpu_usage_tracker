#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "header/analyzer.h"
#include "header/printer.h"
#include "header/reader.h"
#include "header/globals.h"

#define NON_CPU_LINES 7

int main()
{
   pthread_t thread_id[THREADS_NUMBER];
   unsigned int cores = lines_to_read() - NON_CPU_LINES;
   struct threads_data thread_data;
   thread_data.number_of_cores=cores;

   if(sem_init(&thread_data.reader_send_ready,0,0) != 0){
      printf("\n reader_send_ready init failed\n");
      return 1;
   }
   if(sem_init(&thread_data.anazyler_read_ready,0,1) != 0){
      printf("\n anazyler_read_ready init failed\n");
      return 1;
   }
   if(sem_init(&thread_data.analyzer_write_ready,0,0) != 0){
      printf("\n printer init failed\n");
      return 1;
   }
   if(sem_init(&thread_data.printer_read_ready,0,1) != 0){
   
      printf("\n printer init failed\n");
      return 1;
   }
   
   pipe(thread_data.reader_analyzer);
   pipe(thread_data.analyzer_printer);
   thread_data.watch = &watch;  
   printf("Hello, from cpu_usage_tracker!\n"); 
   pthread_create(&thread_id[0], NULL, read_data, &thread_data);
   pthread_create(&thread_id[1], NULL, analyze, &thread_data);
   pthread_create(&thread_id[2], NULL, printer_gui, &thread_data);
   pthread_create(&thread_id[3], NULL, watchdog, NULL);

   pthread_join(thread_id[0],NULL);
   pthread_join(thread_id[1],NULL);
   pthread_join(thread_id[2],NULL);
   pthread_join(thread_id[3],NULL);
}
