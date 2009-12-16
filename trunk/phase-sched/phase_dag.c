#include "phase_dag.h"
#include "phase_def.h"
 
int phase_dag_init(struct phase_dag *dag)
{
    return SUCCESS;
}

int phase_dag_free(struct phase_dag *dag)
{
    return SUCCESS;
}

int phase_dag_reset(struct phase_dag *dag)
{
    return SUCCESS;
}

int phase_dag_add_link(struct phase_dag *dag, int src_pid, int dest_pid, int weight)
{
    return SUCCESS;
}

int phase_dag_del_link(struct phase_dag *dag, int src_pid, int dest_pid, int weight)
{
    return SUCCESS;
}

int phase_dag_get_task(struct phase_dag *dag, int pid, struct phase_task_struct **task)
{
    return SUCCESS;
}

int phase_dag_register_task(struct phase_dag *dag, int pid)
{
    return SUCCESS;
}
