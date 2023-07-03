#define  _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "analyzer.h"
#include "printer.h"
#include "reader.h"
#include "globals.h"

volatile sig_atomic_t done = 0;
 
void term(int signum){
   done = signum;
}

int main(){
   struct sigaction action;
   memset(&action, 0, sizeof(struct sigaction));
   action.sa_handler = term;
   sigaction(SIGTERM, &action, NULL);
   
   pthread_t thread_id[THREADS_NUMBER];
   unsigned int cores = lines_to_read();
   struct threads_data thread_data;
   thread_data.number_of_cores=cores;
   thread_data.kill = 0;
   if(sem_init(&thread_data.reader_send_ready,0,0) != 0){
      logger("reader_send_ready init failed");
      return 1;
   }
   if(sem_init(&thread_data.anazyler_read_ready,0,1) != 0){
      logger("anazyler_read_ready init failed");
      return 1;
   }
   if(sem_init(&thread_data.analyzer_write_ready,0,0) != 0){
      logger("analyzer write init failed");
      return 1;
   }
   if(sem_init(&thread_data.printer_read_ready,0,1) != 0){
      logger("printer read init failed");
      return 1;
   }
   if(sem_init(&thread_data.send_log,0,0) != 0){
      logger("printer read init failed");
      return 1;
   }
   
   pipe(thread_data.reader_analyzer);
   pipe(thread_data.analyzer_printer);
   thread_data.watch = &watch;  
   pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
   pthread_create(&thread_id[0], NULL, read_data, &thread_data);
   pthread_create(&thread_id[1], NULL, analyze, &thread_data);
   pthread_create(&thread_id[2], NULL, printer_gui, &thread_data);
   pthread_create(&thread_id[3], NULL, watchdog, &thread_data);
   pthread_create(&thread_id[4], NULL, logger, &thread_data);

   //logger("Threads, semaphores and pipes created");

   while (!done){
      unsigned int t = sleep(3); 
      
      while (t > 0){
         logger("Received SIGTERM signal");
         t = sleep(t);
         thread_data.kill = 1;
         pthread_join(thread_id[0],NULL);
         pthread_join(thread_id[1],NULL);
         
         pthread_join(thread_id[3],NULL);
         pthread_join(thread_id[4],NULL);
         pthread_cancel(thread_id[2]);
         pthread_join(thread_id[2],NULL);
      }
   }
   return 0;
}
