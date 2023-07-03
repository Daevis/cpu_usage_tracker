#ifndef READER_H
#define READER_H
#include <stdio.h>

extern unsigned int lines_to_read(void);
void *read_data(void*);
char* read_file(FILE*);
#endif 
