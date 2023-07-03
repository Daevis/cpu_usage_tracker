
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <ncurses.h>

#include "../header/globals.h"

void* watchdog(void* thread_dataPtr){

    int* threads_status;
    threads_status=watch(THREADS_NUMBER+1);
    struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;

    while(thread_data->kill != 1){

        sleep(2);
        for(int id = 0; id < THREADS_NUMBER; id++){
            if(threads_status[id] == 0 && thread_data->kill != 1){
                char* stuck_thread = "Stuck";
                switch (id){
                    case 0: {
                        stuck_thread = "Reader";
                        break;
                    }
                    case 1: {
                        stuck_thread = "Analyzer";
                        break;
                    }
                    case 2: {
                        stuck_thread = "Printer";
                        break;
                    }
                    case 3: {
                        stuck_thread = "Logger";
                        break;
                    }
                }
                char message[] = "Program terminated \nResult = stuck thread ";
                strcat(message, stuck_thread );
                thread_data->message = message;
                printf("%s", message);
                exit(0);
            }
            threads_status[id]=0;
        }
    }
    return 0;
}

int* watch(int thread_id){   
    static int threads_stat[THREADS_NUMBER]={0};

    if(thread_id != THREADS_NUMBER + 1)
        threads_stat[thread_id]=1;

    return threads_stat;
}
