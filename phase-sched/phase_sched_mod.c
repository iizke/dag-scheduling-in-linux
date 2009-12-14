#include "phase_sched.h"
#include "phase_sysfs.h"
#include "phase_dag.h"

struct phase_sched phase_sched;
 
int phase_req_init(struct phase_req *req)
{
    return SUCCESS;
}

int phase_req_free(struct phase_req *req)
{
    return SUCCESS;
}


int cpuload_list_reset(struct cpuload_list *list)
{
    return SUCCESS;
}

int dag_reset(struct phase_dag *dag)
{
    return SUCCESS;
}

static int rebuild_dag(struct phase_dag *dag, struct phase_req *req) 
{
    if (!req)
        return FAIL;
    
    return SUCCESS;
}

static int phase_sched_schedule(struct phase_sched *ps)
{
    return SUCCESS;
}

int phase_sched_do_req(struct phase_sched *ps, struct phase_req *req)
{
    int flag;

    flag = rebuild_dag(&ps->dag, req);
    if (flag != SUCCESS)
        return flag;
    
    flag = phase_sched_schedule(ps);
    if (flag != SUCCESS)
        return flag;

    return SUCCESS;
}

int phase_sched_reset(struct phase_sched *ps)
{
    int flag;
    
    flag = dag_reset(&ps->dag);
    if (flag != SUCCESS)
        return flag;

    flag = cpuload_list_reset(&ps->cpuload_list);
    if (flag != SUCCESS)
        return flag;

    return SUCCESS;
}

static int phase_sched_init(struct phase_sched *ps)
{
    if (!ps)
        return FAIL;
    
    phase_dag_init(&ps->dag);
    phase_req_init(&ps->req);
    cpuload_list_init(&ps->cpuload_list);
    phase_sysfs_init(ps);
    build_phase_sysfs_tree(ps);
    
    return SUCCESS;
}

static int phase_sched_exit(struct phase_sched *ps)
{
    if (!ps)
        return FAIL;
        
    free_phase_sysfs_tree(ps);
    phase_dag_free(&ps->dag);
    cpuload_list_free(&ps->cpuload_list);
    return SUCCESS;
}

int __init phase_sched_mod_init(void)
{
    phase_sched_init(&phase_sched);
    return SUCCESS;
}

void __exit phase_sched_mod_exit(void)
{
    phase_sched_exit(&phase_sched);
}

/**************************************************************/
MODULE_DESCRIPTION("PHASE SCHEDULING MODULE");
MODULE_AUTHOR("http://dag-scheduling-in-linux.googlecode.com");
MODULE_LICENSE("GPLv3");

module_init(phase_sched_mod_init); // initialization function
module_exit(phase_sched_mod_exit); // exit function
/**************************************************************/
