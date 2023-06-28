#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include "../header/analyzer.h"
#include "../header/globals.h"

enum cpu_data {
    core_id=0,
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
    }cpu_enum;

    struct cpu_data_struct {
    int core_id;
   int user;
   int cpu_nice;
   int cpu_system;
   int idle;
   int iowait;
   int irq;
  int  softir;
   int steal;
   int guest;
   int guest_nice;
    };

enum prev_data{
   prev_total=0,
   prev_idle=1,
}prev_data;

void* analyze(void* thread_data1)
{
    struct cpu_data_struct cores_param_array[25];
  
    
      struct threads_data *thread_data = (struct threads_data*)thread_data1;
      
      char received_data[24][70];
      unsigned int core_numbers=(uintptr_t)thread_data->number_of_cores;
      
         long prev_data[core_numbers][2];
         memset(prev_data, 0, sizeof(prev_data)); 
       int data_chars_array[core_numbers][11];
char read_buffer[70];

     while(1)
    {
        
       sem_wait(&thread_data->reader_send_ready);

    
    for(int core=0; core<core_numbers; core++){
    int result = read(thread_data->reader_analyzer[0],&read_buffer,70);
    for (int z = 0; z < 70; z++) // 70 ? !!!!!!!!!!!!!!!!!!!!
    {
       received_data[core][z]=read_buffer[z];
    }
    

    }
    for(int core=0; core<core_numbers; core++){
    char * param;
    param= strtok((char*)received_data[core]," ");
    
        for(int param_id=0; param!=NULL;param_id++){
        data_chars_array[core][param_id]=atoi(param);
        param=strtok(NULL, " ");
    
    }
    }

    
  
for(int core=0;core<core_numbers;core++){
    for(int param_id=0; param_id<sizeof(cpu_enum); param_id++)
    {
        switch (param_id){
        case user:
        {
                cores_param_array[core].user=data_chars_array[core][user];
                break;
            }
            case cpu_nice:
            {
                cores_param_array[core].cpu_nice=data_chars_array[core][cpu_nice];
                break;
            }
           case cpu_system:
            {
                cores_param_array[core].cpu_system=data_chars_array[core][cpu_system];
                break;
            }
            case idle:
            {
                cores_param_array[core].idle=data_chars_array[core][idle];
                break;
            }
            case iowait:
            {
                cores_param_array[core].iowait=data_chars_array[core][iowait];
                break;
            }
            case irq:
            {
                cores_param_array[core].irq=data_chars_array[core][irq];
                break;
            }
            case softir:
            {
                cores_param_array[core].softir=data_chars_array[core][softir];
                break;
            }
             case steal:
            {
                cores_param_array[core].steal=data_chars_array[core][steal];
                break;
            }
             case guest:
            {
                cores_param_array[core].guest=data_chars_array[core][guest];
                break;
            }
             case guest_nice:
            {
                cores_param_array[core].guest_nice=data_chars_array[core][guest_nice];
                break;
            }
            break;
        
        default:
            break;
        }
    }



}
for(int core=0;core<core_numbers;core++){

        long cpu_idle = data_chars_array[core][idle]+data_chars_array[core][iowait];
        long nonidle = data_chars_array[core][user]+data_chars_array[core][cpu_nice]+
        data_chars_array[core][cpu_system]+data_chars_array[core][irq]+
        data_chars_array[core][softir]+data_chars_array[core][steal];

        long total = cpu_idle+ nonidle;
        long totald = total - prev_data[core][prev_total];
        long idled = cpu_idle - prev_data[core][prev_idle];
        if(prev_data[core][prev_idle]!=0 && prev_data[core][prev_total]!=0)
        {
        float cpu_usage = (float)((totald-idled)*100)/totald;
        
        printf("\n zuzyeice procentowe %d =rdzen %.2f" , core,cpu_usage);
        }
        
        prev_data[core][prev_idle]=cpu_idle;
        prev_data[core][prev_total]=total;

}
        

           sem_post(&thread_data->anazyler_read_ready);

    sleep(1);
    }
    
}
