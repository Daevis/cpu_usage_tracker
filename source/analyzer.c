#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../header/analyzer.h"
#include "../header/globals.h"


void* analyze(void* thread_dataPtr){

    struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;
    unsigned int core_numbers=(unsigned int)thread_data->number_of_cores;
    char received_data[core_numbers][70];
    long prev_data[core_numbers][2];
    memset(prev_data, 0, sizeof(prev_data)); 
    int data_chars_array[core_numbers][11];
    char read_buffer[70];
    char * param;

    while(thread_data->kill != 1){

        sem_wait(&thread_data->reader_send_ready);
    
        for(unsigned int core=0; core<core_numbers; core++){
            int result = read(thread_data->reader_analyzer[0],&read_buffer,70);
            if(result == -1){
                logger("Error with reading data from reader");
            }

            for (int z = 0; z < 70; z++){ 
               received_data[core][z]=read_buffer[z];
            }

            param= strtok((char*)received_data[core]," ");

            for(int param_id=0; param!=NULL;param_id++){

                data_chars_array[core][param_id]=atoi(param);
                param=strtok(NULL, " ");
            }
        }
                
        sem_post(&thread_data->analyzer_write_ready);
        sem_wait(&thread_data->printer_read_ready);
        
        for(unsigned int core=0;core<core_numbers;core++){

            long cpu_idle = data_chars_array[core][idle]+data_chars_array[core][iowait];
            long nonidle = data_chars_array[core][user]+data_chars_array[core][cpu_nice]+
            data_chars_array[core][cpu_system]+data_chars_array[core][irq]+
            data_chars_array[core][softir]+data_chars_array[core][steal];

            long total = cpu_idle + nonidle;
            long totald = total - prev_data[core][prev_total];
            long idled = cpu_idle - prev_data[core][prev_idle];

            if(prev_data[core][prev_idle]!=0 && prev_data[core][prev_total]!=0){
                float cpu_usage = (float)((totald-idled)*100)/(float)totald;
                int result = write(thread_data->analyzer_printer[1],&cpu_usage ,4);
                if(result == -1){
                    logger("Error with sending data to printer");
                }
            }
            prev_data[core][prev_idle]=cpu_idle;
            prev_data[core][prev_total]=total;
        }

        thread_data->watch(1);
        sleep(1);
        sem_post(&thread_data->anazyler_read_ready);
    }
    return 0;
}
