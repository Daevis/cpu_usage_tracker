#ifndef ANALYZER_H
#define ANALYZER_H

enum cpuData {
    core_id=0,
    user=1,
    cpu_nice=2,
    cpu_system=3,
    idle=4,
    iowait=5,
    irq=6,
    softir=7,
    steal=8,
    guest=9,
    guest_nice=10
};

enum prevData{
   prev_total=0,
   prev_idle=1,
};

void* analyze(void*);

#endif

