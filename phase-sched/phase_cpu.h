#ifndef PHASE_CPU_H
#define PHASE_CPU_H

#include "phase_def.h"

/** MAX_LOADS: maximum of loads that a processor can hold */
#define MAX_LOADS               50
#define MAX_CPUS                8

struct phase_cpu {
    struct phase_cpu *next;
    struct phase_cpu *prev;
    int id;
    int load;
};

/** cpu_list: list of phase_cpu */
struct cpu_list {
    int size;
    struct phase_cpu *last_cpu;
};

/** list of cpu load of all cpus */
struct cpuload_list {
    struct cpu_list load_array[MAX_LOADS];
    struct phase_cpu cpus[MAX_CPUS];
    int least_load_id;
};

/** initialization */
int
cpuload_list_init(struct cpuload_list *list);
/** move phase_cpu to new id in load-list */
int
cpuload_list_adjust_load(
                         struct cpuload_list *list,
                         struct phase_cpu *cpu,
                         int newload);

int
cpuload_list_reset(struct cpuload_list *list);

int
cpuload_list_free(struct cpuload_list *list);

int
cpuload_list_get_light_cpu(struct cpuload_list *list, struct phase_cpu **cpu);

#endif
