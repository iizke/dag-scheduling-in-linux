#include "phase_cpu.h"

/** initialization
 * @list cpuload list
 * @return error code
 */
int cpuload_list_init(struct cpuload_list *list)
{
    return SUCCESS;
}

/** move phase_cpu to new id in load-list
 *  @return error code
 */
int cpuload_list_adjust_load(struct cpuload_list *list, struct phase_cpu *cpu, int newload)
{
    return SUCCESS;
}

int cpuload_list_free(struct cpuload_list *list)
{
    return SUCCESS;
}

int cpu_list_get_first_cpu(struct cpu_list *list, struct phase_cpu **cpu)
{
    return SUCCESS;
}

int cpuload_list_get_light_cpu(struct cpuload_list *list, struct phase_cpu **cpu)
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

int cpu_list_reset(struct cpu_list *list)
{
    return SUCCESS;
}

int cpuload_list_reset(struct cpuload_list *list)
{
    int i = 0;
    if (!list)
        return FAIL;
    list->least_load_id = 0;
    for (i = 0; i < MAX_LOADS; i++)
        cpu_list_reset(&list->load_array[i]);
    return SUCCESS;
}

int cpu_list_insert_cpu(struct cpu_list *list, struct phase_cpu *cpu)
{
    return SUCCESS;
}

int cpu_list_remove_cpu(struct cpu_list *list, struct phase_cpu *cpu)
{
    return SUCCESS;
}
