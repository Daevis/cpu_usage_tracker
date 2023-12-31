#define  _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "reader.h"
#define NON_CPU_LINES 7

/**
 * @brief Return CPUs number
 * 
 * This function return number of CPUs read from /proc/stat
 * 
 * @return unsigned int Number of CPUs
 */
unsigned int cpu_number(){

  unsigned int lines = 0;
  int ch;
  FILE* fp= fopen("/proc/stat", "r");
  if(fp == NULL){
    return 0;           
  }

  for (ch = getc(fp); ch != EOF; ch = getc(fp)){
    if (ch == '\n') 
      lines++;
  }

  fclose(fp); 

  return lines - NON_CPU_LINES;
}

/**
 * @brief Read file lines
 * 
 * This function reads given file line by line
 * 
 * @param fp file pointer to read
 * @return char* Read line from file
 */
char* read_file(FILE* fp){

  size_t k = 0;
  char* data_line;
  if(getline(&data_line, &k, fp)<0){  
    free(data_line);
    data_line = NULL;
  }

  return data_line;
}


/**
 * @brief Read data thread
 * 
 * This function is a thread that reads /proc/stat data
 * and send cpu based data to analyzer thread
 * 
 * @param thread_dataPtr struct of shared variables between threads
 * @return void* 
 */
void *read_data(void* thread_dataPtr){

  struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;
  unsigned int core_numbers = (unsigned int)thread_data->number_of_cores;
  char* array_data_lines[core_numbers];
  memset(array_data_lines,0,sizeof(array_data_lines));
  
  while(thread_data->kill != 1){  
    
    sem_wait(&thread_data->anazyler_read_ready);
    FILE* fp = fopen("/proc/stat", "r");

    for(unsigned int core = 0; core < core_numbers; core++){

      array_data_lines[core] = read_file(fp);
      if(strstr(array_data_lines[core],"intr")){
        break;
      }

      long result = write(thread_data->reader_analyzer[1], array_data_lines[core], READ_BUFFER);
      if(result == -1){
        sem_post(&thread_data->send_log);
        thread_data->message = "Error with sending data to analyzer";
      }

      free(array_data_lines[core]);   
      array_data_lines[core] = NULL;
    }

    fclose(fp);
    thread_data->alive_sign(0);
    sem_post(&thread_data->reader_send_ready);
  } 
  return 0;
}
