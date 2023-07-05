#ifndef GLOBALS_H
#define GLOBALS_H
#include <semaphore.h>

#define THREADS_NUMBER 5
#define READ_BUFFER 70
struct threads_data{
    sem_t reader_send_ready;
    sem_t anazyler_read_ready;
    sem_t analyzer_write_ready;
    sem_t printer_read_ready;
    sem_t send_log;
    int reader_analyzer[2];
    int analyzer_printer[2];
    unsigned int number_of_cores;
    int* (*alive_sign)(int);
    int kill;
    char* message;
};

void term(int);
void* watchdog(void*);
int* alive_sign(int);

void* logger(void *);

#endif
