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
