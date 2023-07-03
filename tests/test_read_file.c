#include <stdio.h>
#include <string.h>
#include "../header/reader.h"

int main(){

    FILE* fp = fopen("/proc/stat", "r");
    char* read_cpu_data = read_file(fp);
    char* read_cpu0_data = read_file(fp);
    printf("%s \n", read_cpu_data);
    printf("%s \n", read_cpu0_data);
    fclose(fp);

    if(strstr(read_cpu_data,"cpu") && strstr(read_cpu0_data,"cpu0")){
        return 0;
    }
    
    return 1;
}