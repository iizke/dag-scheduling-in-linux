#include <linux/stddef.h>
#include "phase_cpu.h"
#include "phase_def.h"

static int
cpu_list_reset(struct cpu_list *list)
{
    if (!list)
        return ERR_CPU_LIST_NULL;
    list->size = 0;
    list->last_cpu = NULL;
    return SUCCESS;
}

static int
phase_cpu_reset(struct phase_cpu *cpu)
{
    if (!cpu)
        return ERR_PHASE_CPU_NULL;
    cpu->load = 0;
    cpu->next = NULL;
    cpu->prev = NULL;
    return SUCCESS;
}

/**
 * cpuload_list_reset
 * @list: cpu load list
 * @return: error code
 *
 * Reset all values in cpuload_list structure
 */
int
cpuload_list_reset(struct cpuload_list *list)
{
    int i = 0;
    if (!list)
        return ERR_CPULOAD_LIST_NULL;
    list->least_load_id = 0;
    for (i = 0; i < MAX_LOADS; i++)
        cpu_list_reset(&list->load_array[i]);
    for (i = 0; i < MAX_CPUS; i++) {
        int next_id = (i + 1) % MAX_CPUS;
        int prev_id = (i + MAX_CPUS - 1) % MAX_CPUS;
        phase_cpu_reset(&list->cpus[i]);
        list->cpus[i].id = i;
        list->cpus[i].next = &list->cpus[next_id];
        list->cpus[i].prev = &list->cpus[prev_id];
    }
    list->load_array[0].last_cpu = &list->cpus[0];
    list->load_array[0].size = MAX_CPUS;
    return SUCCESS;
}

/**
 * initialization
 * @list cpuload list
 * @return error code
 */
int
cpuload_list_init(struct cpuload_list *list)
{
    return cpuload_list_reset(list);
}

static int
cpu_list_del_cpu(struct cpu_list *l, struct phase_cpu *cpu)
{
    if (!l)
        return ERR_CPU_LIST_NULL;
    if (!cpu)
        return ERR_PHASE_CPU_NULL;
    if (cpu->next == NULL)
        return SUCCESS;

    if (l->size <= 1) {
        l->last_cpu = NULL;
        l->size = 0;
    } else {
        struct phase_cpu *next = cpu->next;
        struct phase_cpu *prev = cpu->prev;
        prev->next = next;
        next->prev = prev;
        if (l->last_cpu == cpu)
            l->last_cpu = next;
        l->size--;
    }
    cpu->next = NULL;
    cpu->prev = NULL;
    return SUCCESS;
}

static int
cpu_list_add_cpu(struct cpu_list *l, struct phase_cpu *cpu)
{
    if (!l)
        return ERR_CPU_LIST_NULL;
    if (!cpu)
        return ERR_PHASE_CPU_NULL;
    if (!cpu->next)
        return SUCCESS;

    if (l->last_cpu == NULL) {
        l->last_cpu = cpu;
        l->size = 1;
        cpu->next = cpu;
        cpu->prev = cpu;
    } else {
        cpu->next = l->last_cpu->next;
        l->last_cpu->next = cpu;
        cpu->prev = l->last_cpu;
        cpu->next->prev = cpu;
        l->last_cpu = cpu;
        l->size++;
    }
    return SUCCESS;
}

/**
 * cpuload_list_adjust_load
 * @list: cpu load list
 * @cpu: cpu
 *
 * move phase_cpu to new id in load-list
 * @return error code
 */
int
cpuload_list_adjust_load(
                         struct cpuload_list *list,
                         struct phase_cpu *cpu,
                         int newload)
{
    int curload_id;
    int newload_id;

    if (!list)
        return ERR_CPULOAD_LIST_NULL;
    if (!cpu)
        return ERR_PHASE_CPU_NULL;

    curload_id = cpu->load;
    if (curload_id >= MAX_LOADS)
        curload_id = MAX_LOADS - 1;
    newload_id = newload;
    if (newload_id >= MAX_LOADS)
        newload_id = MAX_LOADS - 1;

    if (curload_id == newload_id)
        return SUCCESS;

    if (curload_id >= 0)
        cpu_list_del_cpu(&list->load_array[curload_id], cpu);

    cpu->load = newload;
    cpu_list_add_cpu(&list->load_array[newload], cpu);

    return SUCCESS;
}

/**
 * cpuload_list_free
 * @list: cpuload list
 * @return: error code
 *
 * Free dynamic allocated memory
 */
int
cpuload_list_free(struct cpuload_list *list)
{
    // nothing to do
    return SUCCESS;
}

/**
 * cpu_list_get_first_cpu
 * @list: cpu list
 * @cpu: (output) cpu
 * @return: error code
 */
static int
cpu_list_get_first_cpu(struct cpu_list *list, struct phase_cpu **cpu)
{
    if (!list)
        return ERR_CPU_LIST_NULL;
    if (!cpu)
        return FAIL;

    if (list->size == 0)
        (*cpu) = NULL;
    else
        (*cpu) = list->last_cpu->next;

    return SUCCESS;
}

/**
 * cpuload_list_get_light_cpu
 * @list: cpuload list
 * @cpu: (output) cpu
 * @return: error code
 *
 * Get the cpu that has the least load
 */
int
cpuload_list_get_light_cpu(struct cpuload_list *list, struct phase_cpu **cpu)
{
    int id = 0;
    if (!list)
        return -1;
    id = list->least_load_id;
    if (id < 0)
        return FAIL;
    cpu_list_get_first_cpu(&list->load_array[id], cpu);

    return SUCCESS;
}
