#include <stdio.h>
#include <string.h>
#include "globals.h"
#include <time.h>

/**
 * @brief Logger thread
 * 
 * This function is a thread that logs 
 * some error events
 * 
 * @param thread_dataPtr struct of shared variables between threads
 * @return void* 
 */
void* logger(void* thread_dataPtr){
    
    struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;

    while(thread_data->kill != 1){

        thread_data->alive_sign(3);
        sem_wait(&thread_data->send_log);
        if(!strstr(thread_data->message, "No errors")){
        FILE* flog= fopen("log.txt", "a");
        time_t T = time(NULL);
        struct tm tm = *localtime(&T);
        fprintf(flog, "%02d/%02d/%04d %02d:%02d:%02d \n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,tm.tm_hour, tm.tm_min, tm.tm_sec);
        fprintf(flog, "%s \n", thread_data->message);
        fclose(flog);
        }
    }
    return 0;
}
