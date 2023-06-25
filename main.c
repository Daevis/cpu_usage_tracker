#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <pthread.h>
#include "header/analyzer.h"
#include "header/reader.h"



/*
typedef struct matrix {
    int rows;
    int cols;
    int **data;
} matrix;

void *analyzeThread(void *arg)
{
    matrix m = *((matrix *)arg);

    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("[%2d]", m.data[i][j]);
        }

        puts("");
    }

    puts("");
    return NULL;
}
*/

int main(int, char**){
    pthread_t thread_id;
    
    const int cpu_cores = lines_to_read() -7 ;
    enum cpu_data cpu_data;
    
    
    
    
    int arrayf[cpu_cores][11];
    int array2[3][8]={0};

    int array[cpu_cores][15];
    //int (* prarr)[cpu_cores][15];
    //prarr=array;
   
   memset(array, 0, sizeof(array));
      matrix m;
    m.rows = cpu_cores;         // or take user input
    m.cols = 15;
      m.data = malloc(sizeof(int *) * m.rows);
      
    

    size_t k;
    char* line=NULL;
    char* parameter;
    int newline = 33;


    for(int z=0; z<500000; z++){
            int number_of_lines = lines_to_read();
   // sleep(1);

    FILE* fp= fopen("/proc/stat", "r");
        usleep(300000);
int u=0;
    for(number_of_lines; number_of_lines>0; number_of_lines--){
        
        
        int i=0;
      getline(&line, &k , fp);
    //  printf("%s",line);
      if(strstr(line,"intr"))
      {
        break;
      }
         parameter =strtok(line," ");

        while (parameter != NULL)
    {
                    array[u][i] = atoi(parameter);
        printf("%s\n", parameter);
        parameter = strtok(NULL, " ");
        i++;
    
      }
      u++;
    }

for (int i = 0; i < m.rows; i++) {
        m.data[i] = malloc(sizeof(int) * m.cols);

        for (int j = 0; j < m.cols; j++) {
            m.data[i][j] = array[i][j];
        }
    }
        pthread_create(&thread_id, NULL, analyzeThread, &m);

analyze(cpu_cores,array);
 //   printf("siemaon %d",cpu1.user);
    

   fclose(fp);
    }
    free(line);
        pthread_join(thread_id, NULL);

}



