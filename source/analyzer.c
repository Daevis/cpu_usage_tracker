#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "analyzer.h"
#include "globals.h"


/**
 * @brief Analyze data thread
 * 
 * This function is a thread to calculate average cpu usage in %
 * and sends it to printer thread
 * 
 * @param thread_dataPtr struct of shared variables between threads
 * @return void* 
 */
void* analyze(void* thread_dataPtr){

    struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;
    unsigned int core_numbers=(unsigned int)thread_data->number_of_cores;
    char received_data[core_numbers][READ_BUFFER];
    long prev_data[core_numbers][PREV_PARAMS];
    memset(prev_data, 0, sizeof(prev_data)); 
    int data_chars_array[core_numbers][CPU_PARAMETERS];
    char read_buffer[READ_BUFFER];
    char * param;

    while(thread_data->kill != 1){

        sem_wait(&thread_data->reader_send_ready);
    
        for(unsigned int core = 0; core < core_numbers; core++){

            long result = read(thread_data->reader_analyzer[0], &read_buffer, READ_BUFFER);
            if(result == -1){
                thread_data->message = "Error with reading data from reader";
            }

            for (int character = 0; character < READ_BUFFER; character++){ 
               received_data[core][character]=read_buffer[character];
            }

            param = strtok((char*)received_data[core]," ");

            for(int param_id = 0; param != NULL; param_id++){

                data_chars_array[core][param_id] = atoi(param);
                param = strtok(NULL, " ");
            }
        }

        sem_wait(&thread_data->printer_read_ready);
        sem_post(&thread_data->analyzer_write_ready);
        
        for(unsigned int core = 1; core < core_numbers; core++){

            long cpu_idle = data_chars_array[core][idle] + data_chars_array[core][iowait];
            long nonidle = data_chars_array[core][user] + data_chars_array[core][cpu_nice] +
                            data_chars_array[core][cpu_system] + data_chars_array[core][irq] +
                            data_chars_array[core][softir] + data_chars_array[core][steal];

            long total = cpu_idle + nonidle;
        
            if(prev_data[core][prev_idle] != 0 && prev_data[core][prev_total] != 0){
                long totald = total - prev_data[core][prev_total];
                long idled = cpu_idle - prev_data[core][prev_idle];
                float cpu_usage = (float)((totald-idled) * 100) / (float)totald;
                long result = write(thread_data->analyzer_printer[1], &cpu_usage, sizeof(cpu_usage));
                if(result == -1){
                    sem_post(&thread_data->send_log);
                    thread_data->message = "Error with sending data to printer";
                }
            }
            prev_data[core][prev_idle] = cpu_idle;
            prev_data[core][prev_total] = total;
        }

        sleep(1);                      
        thread_data->alive_sign(1);
        sem_post(&thread_data->anazyler_read_ready);
    }
    return 0;
}
