#ifndef PHASE_DAG_H
#define PHASE_DAG_H

#include <linux/sched.h>
#include <linux/hash.h>

/** How to mapping pid -> task struct */

#define MAX_TASKS   50
struct phase_task;

/** phase_task: task's info in DAG */
struct phase_task {
    int pid;
    int index;
    struct phase_cpu *oncpu;
    struct task_struct *task;
};

struct phase_link {
    struct phase_link *next;
    int src_index;
    int dest_index;
    int weight;
    int state;
};

struct task_pool {
    struct phase_task tasks[MAX_TASKS];
    char state[MAX_TASKS];
};

struct link_pool {
    struct phase_link links[MAX_TASKS][MAX_TASKS];
};

struct phase_dag {
    // hash table of phase_task
    struct task_pool task_pool;
    struct link_pool link_pool;
};

int
phase_dag_reset(struct phase_dag *dag);

int
phase_dag_init(struct phase_dag *dag);

int
phase_dag_free(struct phase_dag *dag);

int
phase_dag_add_link(
                   struct phase_dag *dag,
                   int src_pid,
                   int dest_pid,
                   int weight);

int
phase_dag_del_link(struct phase_dag *dag, int src_pid, int dest_pid);

int
phase_dag_get_task(
                   struct phase_dag *dag,
                   int pid,
                   struct phase_task **task);

int
phase_dag_register_task(struct phase_dag *dag, int pid);

int
map_pid2index(struct phase_dag *dag, int pid);

#endif

