#ifndef GLOBALS_H
#define GLOBALS_H
#include <semaphore.h>

#define THREADS_NUMBER 4
struct threads_data{
    sem_t reader_send_ready;
    sem_t anazyler_read_ready;
    sem_t analyzer_write_ready;
    sem_t printer_read_ready;
    int reader_analyzer[2];
    int analyzer_printer[2];
    unsigned int number_of_cores;
    int* (*watch)(int);
    int kill;
};


void * watchdog(void*);
int* watch(int);

void logger(char *);

#endif
