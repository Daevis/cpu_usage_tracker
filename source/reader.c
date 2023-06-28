#include "../header/reader.h"
#include <unistd.h>
#include <stdint.h>
#include <semaphore.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../header/globals.h"

int lines_to_read()
{ 
    int lines = 0;
    char ch;
    FILE* fp= fopen("/proc/stat", "r");
    if(fp == NULL)
      {
        printf("Error!");   
        exit(1);             
      }
    
    for (ch = getc(fp); ch != EOF; ch=getc(fp) )
        if (ch == '\n') 
            lines++;
    fclose(fp); 

return lines;
}

void *read_data(void* thread_data1)
{ 
  struct threads_data *thread_data = (struct threads_data*)thread_data1;
    
    while(1)

    {
      
    size_t k=0;
    unsigned int core_numbers =(uintptr_t)thread_data->number_of_cores;

    char* array_data_lines[core_numbers];
    int size = sizeof(&array_data_lines);
    memset(array_data_lines,0,sizeof(array_data_lines));
            FILE* fp= fopen("/proc/stat", "r");
   sem_wait(&thread_data->anazyler_read_ready);
     sem_post(&thread_data->reader_send_ready);



//   MERGE FOR LOOPS?
        for(int core =0; core<core_numbers;core++)
        {

      if(getline(&array_data_lines[core], &k , fp)<0)
      {
        free(array_data_lines[core]);
        array_data_lines[core]=NULL;
      }
     
        }

     fclose(fp);
      sem_post(&thread_data->reader_send_ready);

  for(int core=0; core<core_numbers;core++)
  {
     
     if(strstr(array_data_lines[core],"intr"))
     {break;

     }
 
           write(thread_data->reader_analyzer[1],array_data_lines[core] ,70);

        free(array_data_lines[core]);   
    array_data_lines[core]=NULL;
  }
    
        


 sleep(1);
      

    } 

    return NULL;
      
}