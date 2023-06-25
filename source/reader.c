//#include "../header/reader.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include<unistd.h>



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


return lines;
}
