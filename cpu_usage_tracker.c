#define  _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "analyzer.h"
#include "printer.h"
#include "reader.h"
#include "globals.h"

static volatile sig_atomic_t done = 0;

/**
 * @brief SIGTERM capture
 * 
 * This function is capturing SIGTERM signal
 *  
 * @param signum signal type
 */
void term(int signum){
   done = signum;
}

int main(){

   struct sigaction action;
   memset(&action, 0, sizeof(struct sigaction));
   action.sa_handler = term;
   sigaction(SIGTERM, &action, NULL);
   pthread_t thread_id[THREADS_NUMBER];
   unsigned int cores = cpu_number();
   struct threads_data thread_data;
   thread_data.number_of_cores=cores;
   thread_data.kill = 0;
   thread_data.alive_sign = &alive_sign;  

   if(sem_init(&thread_data.reader_send_ready,0,0) != 0){
      printf("reader_send_ready init failed");
      return 1;
   }
   if(sem_init(&thread_data.anazyler_read_ready,0,1) != 0){
      printf("anazyler_read_ready init failed");
      return 1;
   }
   if(sem_init(&thread_data.analyzer_write_ready,0,0) != 0){
      printf("analyzer write init failed");
      return 1;
   }
   if(sem_init(&thread_data.printer_read_ready,0,1) != 0){
      printf("printer read init failed");
      return 1;
   }
   if(sem_init(&thread_data.send_log,0,0) != 0){
      printf("printer read init failed");
      return 1;
   }
   if(pipe(thread_data.reader_analyzer) != 0){
      printf("pipe reader_analyzer init failed");
      return 1;
   }
   if(pipe(thread_data.analyzer_printer) != 0){
      printf("pipe analyzer_printer init failed");
      return 1;
   }
   if(pthread_create(&thread_id[0], NULL, read_data, &thread_data) != 0){
      printf("thread reader init failed");
      return 1;
   }
   if(pthread_create(&thread_id[1], NULL, analyze, &thread_data) != 0){
      printf("thread analyzer init failed");
      return 1;
   }
   if(pthread_create(&thread_id[2], NULL, printer_gui, &thread_data) != 0){
      printf("thread printer init failed");
      return 1;
   }
   if(pthread_create(&thread_id[3], NULL, watchdog, &thread_data) != 0){
      printf("thread watchdog init failed");
      return 1;
   }
   if(pthread_create(&thread_id[4], NULL, logger, &thread_data) != 0){
      printf("thread printf init failed");
      return 1;
   }

   while (!done){
      unsigned int t = sleep(3); 
      
      while (t > 0){
         t = sleep(t);
         thread_data.kill = 1;
      }
   }
   return 1;
}
