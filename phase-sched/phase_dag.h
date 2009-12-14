#ifndef PHASE_DAG_H
#define PHASE_DAG_H

#include <linux/sched.h>

/** How to mapping pid -> task struct */

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
    //
};
 
int phase_dag_init(struct phase_dag *dag);
int phase_dag_free(struct phase_dag *dag);

#endif

