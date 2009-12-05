#include "phase_sched_mod.h"

int cpuload_list_reset(struct cpuload_list *list)
{
    return SUCCESS;
}

int dag_reset(struct dag *dag)
{
    return SUCCESS;
}

int rebuild_dag(struct req_msg *msg) 
{
    if (!msg)
        return FAIL;
    
    return SUCCESS;
}

int phase_sched_schedule(struct req_msg *msg)
{
    return SUCCESS;
}

int phase_sched_add_connection(int src_pid, int dest_pid, int weight)
{
    int flag;
    struct req_msg msg;
    
    msg.cmd = PHASE_SCHED_ADD_CONN;
    msg.src_pid = src_pid;
    msg.dest_pid = dest_pid;
    msg.weight = weight;
    flag = rebuild_dag(&msg);
    if (flag != SUCCESS)
        return flag;
    
    flag = phase_sched_schedule(&msg);
    if (flag != SUCCESS)
        return flag;

    return SUCCESS;
}

int phase_sched_remove_connection(int src_pid, int dest_pid, int weight)
{
    int flag;
    struct req_msg msg;
    
    msg.cmd = PHASE_SCHED_DEL_CONN;
    msg.src_pid = src_pid;
    msg.dest_pid = dest_pid;
    msg.weight = weight;
    flag = rebuild_dag(&msg);
    if (flag != SUCCESS)
        return flag;
    
    flag = phase_sched_schedule(&msg);
    if (flag != SUCCESS)
        return flag;

    return SUCCESS;
}

int phase_sched_reset() 
{
    int flag;
    
    flag = dag_reset(&dag);
    if (flag != SUCCESS)
        return flag;

    flag = cpuload_list_reset(&cpuload_list);
    if (flag != SUCCESS)
        return flag;

    return SUCCESS;
}

int __init usb_rtusb_init(void)
{
    return SUCCESS;
}

void __exit phase_sched_mod_exit(void)
{
}

/**************************************************************/
MODULE_DESCRIPTION("PHASE SCHEDULING MODULE");
MODULE_AUTHOR("http://dag-scheduling-in-linux.googlecode.com");
MODULE_LICENSE("GPLv3");

module_init(phase_sched_mod_init); // initialization function
module_exit(phase_sched_mod_exit); // exit function
/**************************************************************/
