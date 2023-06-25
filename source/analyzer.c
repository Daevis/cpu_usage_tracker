#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <curses.h>

#include"../header/analyzer.h"

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

void analyze(int cpu_cores,int array[cpu_cores][15])
{
  printf("cc %d",array[0][1]);
}
 // sleep(1);
 /*
    for(int cpu_num=0; cpu_num<cpu_cores; cpu_num++){
        if(array[1][cpu_num]==0)
        {
          break;
        }
    }
/*
        for(int data_num =0; data_num<11; data_num++){
            switch(data_num){
            case 1 :{
            printf(" user   %d" ,array[data_num][cpu_num]);
            break;
            }
            case 2: {
             printf(" nice   %d" ,array[data_num][cpu_num]);
             break;
            }
            case 3:{
                 printf(" system   %d" ,array[data_num][cpu_num]);
              break;
            }
            case 4:{
               printf(" idle   %d" ,array[data_num][cpu_num]);
                break;
            }
            case 5:{
               printf(" iowait   %d" ,array[data_num][cpu_num]);
            break;
            }
            case 6:{
               printf(" irq   %d" ,array[data_num][cpu_num]);
            break;
            }
            case 7:{
               printf(" softir   %d" ,array[data_num][cpu_num]);
                break;
            }
            case 8:{
               printf(" steal   %d" ,array[data_num][cpu_num]);
              break;
            }
            case 9:{
               printf(" guest   %d" ,array[data_num][cpu_num]);
            break;
            }
            case 10:{
               printf(" guest_nice   %d" ,array[data_num][cpu_num]);
              break;
            }
        }}
        
*/
/*
        long zidle = array[idle][cpu_num]+array[iowait][cpu_num];
        long nonidle = array[user][cpu_num]+array[cpu_nice][cpu_num]+
        array[cpu_system][cpu_num]+array[irq][cpu_num]+
        array[softir][cpu_num]+array[steal][cpu_num];

        long total = zidle+ nonidle;
        long totald = total - array2[prev_total][cpu_num];
        long idled = zidle - array2[prev_idle][cpu_num];
        if(array2[prev_idle][cpu_num]!=0 && array2[prev_total][cpu_num]!=0)
        {
        float cpu_perc = ((totald-idled)*100)/totald;
        
        printf("\r zuzyeice procentowe %d =rdzen %f" , cpu_num,cpu_perc);
        
        }
       
        
        else{
        array2[prev_idle][cpu_num]=zidle;
        array2[prev_total][cpu_num]=total;
    }
    printf("\r");
            

    }fflush(stdout); 
};
*/