#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include<unistd.h>


// Open a file in read mode
enum cpu_data {
    user=1,
    cpu_nice=2,
    cpu_system=3,
    idle=4,
    iowait=5,
    irq=6,
    softir=7,
    steal=8,
    guest=9,
    guest_nice=10
    //char id;
    };
enum cpu_usage_calc{
  prev_idle=0,
  prev_total=1,
} ;

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


printf("%d Hello, from cpu_usage_tracker!\n", lines);
return lines;
}

struct prev_data{
  long prevtotal,previdle;
};
void analyze2(int cpu_cores,int array[11][cpu_cores], int array2[8][cpu_cores])
{
  
  sleep(1);
    for(int cpu_num=0; cpu_num<cpu_cores; cpu_num++){
        if(array[1][cpu_num]==0)
        {
          break;
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
        
        printf(" zuzyeice procentowe %d =rdzen %f" , cpu_num,cpu_perc);
        }
        else{
        array2[prev_idle][cpu_num]=zidle;
        array2[prev_total][cpu_num]=total;
    }
    }
};
/*
 void analyze(struct cpu_data *cpu, struct prev_data *prev_data){
   
    printf("cpu usage %ld", cpu->user+cpu->nice);

    long idle = cpu->idle+cpu->iowait;
    long nonidle = cpu->user+cpu->nice+cpu->system+cpu->irq+cpu->softir+cpu->steal;
    long total = idle+nonidle;
    long totald= total-prev_data->prevtotal;
    long idled=idle-prev_data->previdle;
printf("hmgfm  %ld %ld  %ld", totald,idled , prev_data->prevtotal);
   float  cpu_usage_perc= (((totald - idled)*100)/totald);


printf("  cpu usage percentage  %f\n", cpu_usage_perc);
prev_data->prevtotal= total;
prev_data->previdle= idle;
}
*/
int main(int, char**){
    struct prev_data prev_data= {0,  0};
    const int cpu_cores = lines_to_read() -7 ;
    enum cpu_data cpu_data;
    int array[11][cpu_cores];
    int array2[8][3]={0};
    
    size_t k;
    char* line;
    char* parameter;
    int newline = 33;
    for(int z=0; z<500000; z++){
            int number_of_lines = lines_to_read();

    FILE* fp= fopen("/proc/stat", "r");
        sleep(1);
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
                    array[i][u] = atoi(parameter);
        printf("%s\n", parameter);
        parameter = strtok(NULL, " ");
        i++;
    
      }
      u++;
    }
    sleep(1);
analyze2(cpu_cores,array,array2);
 //   printf("siemaon %d",cpu1.user);
    
   fclose(fp);
    }
    free(line);
}



