#ifndef PHASE_DAG_H
#define PHASE_DAG_H

#include <linux/sched.h>
#include <linux/hash.h>

/** How to mapping pid -> task struct */

#define MAX_TASKS   50
struct phase_task_list;
/** list of phase_task_struct */
struct phase_task_list {
    int size;
};

/** phase_task_struct: task's info in DAG */
struct phase_task_struct {
    int pid;
    struct phase_task_list dest_list;
    struct phase_task_list src_list;
    struct phase_cpu *oncpu;
    struct task_struct *task;
};

struct phase_dag {
    // hash table of phase_task_struct
};

int phase_dag_reset(struct phase_dag *dag);
int phase_dag_init(struct phase_dag *dag);
int phase_dag_free(struct phase_dag *dag);
int phase_dag_add_link(struct phase_dag *dag, int src_pid, int dest_pid, int weight);
int phase_dag_del_link(struct phase_dag *dag, int src_pid, int dest_pid, int weight);
int phase_dag_get_task(struct phase_dag *dag, int pid, struct phase_task_struct **task);
int phase_dag_register_task(struct phase_dag *dag, int pid);

#endif
