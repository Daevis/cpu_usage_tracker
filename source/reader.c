#define  _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../header/globals.h"
#include "../header/reader.h"

unsigned int lines_to_read(){

  unsigned int lines = 0;
  char ch;
  FILE* fp= fopen("/proc/stat", "r");
  if(fp == NULL){
    logger("Error with opening /proc/stat file");
    printf("Error!");   
    exit(1);             
  }

  for (ch = getc(fp); ch != EOF; ch=getc(fp)){
    if (ch == '\n') 
      lines++;
  }

  fclose(fp); 

  return lines;
}

void *read_data(void* thread_dataPtr){

  struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;
  size_t k = 0;
  unsigned int core_numbers = (unsigned int)thread_data->number_of_cores;
  char* array_data_lines[core_numbers];
  memset(array_data_lines,0,sizeof(array_data_lines));
  
  while(thread_data->kill != 1){  
    
    sem_wait(&thread_data->anazyler_read_ready);
    FILE* fp = fopen("/proc/stat", "r");

    for(unsigned int core = 0; core < core_numbers; core++){

      if(getline(&array_data_lines[core], &k , fp)<0){  
        free(array_data_lines[core]);
        array_data_lines[core]=NULL;
      }

      if(strstr(array_data_lines[core],"intr")){
        break;
      }

      int result = write(thread_data->reader_analyzer[1],array_data_lines[core] ,70);
      if(result == -1){
        logger("Error with sending data to analyzer");
      }

      free(array_data_lines[core]);   
      array_data_lines[core]=NULL;
    }
    fclose(fp);
    
    thread_data->watch(0);
    sem_post(&thread_data->reader_send_ready);
  } 
  return 0;
}
