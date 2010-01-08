#include "phase_sched.h"
#include "phase_sysfs.h"
#include "phase_dag.h"
#include <linux/sched.h>

//#define find_task_by_pid(nr)  find_task_by_pid_type(PIDTYPE_PID, nr)
//extern void set_user_nice(struct task_struct *p, long nice);
//extern int set_cpus_allowed(struct task_struct *p, cpumask_t new_mask);

struct phase_sched phase_sched;
 
int
phase_req_init(struct phase_req *req)
{
    if (!req)
        return FAIL;
    req->cmd = -1;
    req->weight = 0;
    req->src_id = -1;
    req->dest_id = -1;
    return SUCCESS;
}

static int
phase_sched_rebuild_dag(struct phase_sched *ps, struct phase_req *req) 
{
    struct phase_task *task = NULL;

    if (!ps)
        return ERR_PHASE_SCHED_NULL;
    if (!req)
        return ERR_PHASE_REQ_NULL;

    switch (req->cmd) {
        case PHASE_SCHED_CMD_ADD:
            _phase_dag_add_link(&ps->dag, ps->req.src_id, ps->req.dest_id, ps->req.weight);
            break;
        case PHASE_SCHED_CMD_DEL:
            _phase_dag_del_link(&ps->dag, ps->req.src_id, ps->req.dest_id);
        case PHASE_SCHED_CMD_NEW:
            phase_dag_register_task(&ps->dag, req->src_id, &task);
            break;
        default:
            break;
    }
    return SUCCESS;
}

static int phase_sched_attach_task2cpu(struct phase_sched *ps, struct phase_task *task)
{
    cpumask_t mask;
    struct phase_cpu *cpu = NULL;
    int newload = 0;
    int flag;

    if (!phase_task_is_batch(task)) {
        flag = cpuload_list_get_light_cpu(&ps->cpuload_list, &cpu);
        if (flag != SUCCESS)
            return flag;

        cpus_clear(mask);
        cpu_set(cpu->id, mask);
        newload = cpu->load;
        if (! task->oncpu)
            newload++;
        else if (cpu->id != task->oncpu->id)
            newload++;
    } else {
        cpus_setall(mask);
        if (task->oncpu) {
            newload = task->oncpu->load - 1;
            cpu = task->oncpu;
        } else {
            cpu = NULL;
            newload = 0;
        }
    }

    set_cpus_allowed(task->task, mask);
    cpuload_list_adjust_load(&ps->cpuload_list, cpu, newload);

    return SUCCESS;
}

static int
schedule_task(struct phase_sched *ps, int index)
{
    struct phase_task * t = NULL;
    if (!ps)
        return ERR_PHASE_SCHED_NULL;

    t = &(ps->dag.task_pool.tasks[index]);

    if (phase_task_get_srclist_size(t) == 0) {
        if (phase_task_get_destlist_size(t) > 0)
            set_user_nice(t->task, PHASE_SCHED_SRC_PRIO);
        else 
            set_user_nice(t->task, PHASE_SCHED_BATCH_PRIO);
        phase_sched_attach_task2cpu(ps, t);
    }

    return SUCCESS;
}

static int
phase_sched_schedule(struct phase_sched *ps)
{
    if (!ps)
        return ERR_PHASE_SCHED_NULL;

    if (ps->req.cmd != PHASE_SCHED_CMD_NEW) {
        schedule_task(ps, ps->req.src_id);
        schedule_task(ps, ps->req.dest_id);
    }

    return SUCCESS;
}

/**
 * phase_sched_do_req
 * @ps: phase sched
 * @req: user request
 * @return: error code
 *
 * Processing user request.
 * Note: src_id/dest_id in user-request is PID, but
 *       src_id/dest_id in phase-sched-request is an index.
 */
int phase_sched_do_req(struct phase_sched *ps, struct phase_req *req)
{
    int flag;

    if (!ps)
        return ERR_PHASE_SCHED_NULL;
    if (req->cmd != PHASE_SCHED_CMD_NEW) {
        ps->req.src_id = phase_dag_find_index_from_pid(&ps->dag, req->src_id);
        ps->req.dest_id = phase_dag_find_index_from_pid(&ps->dag, req->dest_id);
    } else 
        ps->req.src_id = req->src_id;

    ps->req.cmd = req->cmd;
    ps->req.weight = req->weight;

    flag = phase_sched_rebuild_dag(ps, req);
    if (flag != SUCCESS)
        return flag;
    flag = phase_sched_schedule(ps);
    return flag;
}

int phase_sched_reset(struct phase_sched *ps)
{
    int flag;
    
    flag = phase_dag_reset(&ps->dag);
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
