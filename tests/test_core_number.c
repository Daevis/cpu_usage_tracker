#include <stdio.h>
#include <sys/sysinfo.h>

#include <assert.h>
#include "../header/reader.h"

int main()
{
    unsigned int cpu_num_reader = cpu_number() - 1;
    printf("This system has %d processors configured and "
        "%d processors available.\n",
        get_nprocs_conf(), get_nprocs());

    unsigned int test_cpu_num = (unsigned int)get_nprocs_conf();
    assert(cpu_num_reader == test_cpu_num);
    return 0;
}
