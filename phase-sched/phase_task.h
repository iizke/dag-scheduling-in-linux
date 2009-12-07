#ifndef PHASE_TASK_H
#define PHASE_TASK_H

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
};

#endif
