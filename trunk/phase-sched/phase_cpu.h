#ifndef PHASE_CPU_H
#define PHASE_CPU_H

#include "phase_sched_mod.h"
/** MAX_LOADS: maximum of loads that a processor can hold */
#define MAX_LOADS               50

/** cpu_list: list of phase_cpu */
struct cpu_list {
    int size;
};

struct phase_cpu {
    int load;
};

/** list of cpu load of all cpus */
struct cpuload_list {
    struct cpu_list load_array[MAX_LOADS];
    int least_load_id;
};

/** initialization */
int cpuload_list_init(struct cpuload_list *list);
/** move phase_cpu to new id in load-list */
int cpuload_list_adjust_load(struct cpuload_list *list, struct phase_cpu *cpu, int newload);

#endif
