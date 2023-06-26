#include "../header/reader.h"
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
struct threads_data{
    pthread_mutex_t lock;
    sem_t semaphore;
    int reader_analyzer[2];
    int analyzer_printer[2];
};

int lines_to_read()
{ 
    int lines;
    char ch ;
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
    int num_cores = lines_to_read()-7;
    char* line1[num_cores];
    int size = sizeof(&line1);
    memset(line1,0,sizeof(line1));
   char* line;
            FILE* fp= fopen("/proc/stat", "r");
    int i=0;
   
        for(int z =0; z<num_cores;z++)
        {

      
      getline(&line1[i], &k , fp);
     
      i++;
        }
      int ch=1;

     fclose(fp);
      sem_post(&thread_data->semaphore);

  for(int i=0; i<num_cores;i++)
  {
     
     if(strstr(line1[i],"intr"))
     {break;}
     printf("g %s %ld",line1[i],sizeof(&line1));
          int result = write(thread_data->reader_analyzer[1],line1[i] ,192);
printf("WYSYLANIE  %d",result);
           

  }

 sleep(1);
        
    

    }
      
  printf("thread reader end");
}