#include <stdio.h>
#include "globals.h"
#include <time.h>

void logger(char* str_log){
    FILE* flog= fopen("log.txt", "a");
    time_t T = time(NULL);
    struct tm tm = *localtime(&T);
    fprintf(flog, "%02d/%02d/%04d %02d:%02d:%02d \n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(flog, "%s \n",str_log);
    fclose(flog);
}
