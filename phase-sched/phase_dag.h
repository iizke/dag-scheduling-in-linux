#ifndef PHASE_DAG_H
#define PHASE_DAG_H

#include <linux/sched.h>
#include <linux/hash.h>

/** How to mapping pid -> task struct */

#define MAX_TASKS   50
struct phase_link;

struct phase_task_list {
    int size;
    struct phase_link *last_link;
};

#define phase_task_list_get_size(p) ((p)->size)

/** phase_task: task's info in DAG */
struct phase_task {
    int pid;
    int index;
    struct phase_cpu *oncpu;
    struct task_struct *task;
    struct phase_task_list src_list;
    struct phase_task_list dest_list;
};

#define phase_task_get_srclist_size(p)  ((p)->src_list.size)
#define phase_task_get_destlist_size(p)  ((p)->dest_list.size)
#define phase_task_is_batch(p)  ((phase_task_get_srclist_size(p) == 0) && (phase_task_get_destlist_size(p) == 0))

struct phase_link {
    struct phase_link *src_next;
    struct phase_link *src_prev;
    struct phase_link *dest_next;
    struct phase_link *dest_prev;
    int src_index;
    int dest_index;
    int weight;
    int state;
};

struct task_pool {
    struct phase_task tasks[MAX_TASKS];
    int inused_tasks;
    char state[MAX_TASKS];
};

#define task_pool_is_full(tp)   ((tp)->inused_tasks == MAX_TASKS)

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
                _phase_dag_add_link(
                                   struct phase_dag *dag,
                                   int src_id,
                                   int dest_id,
                                   int weight);

int
                phase_dag_add_link(
                                   struct phase_dag *dag,
                                   int src_pid,
                                   int dest_pid,
                                   int weight);

int
_phase_dag_del_link(struct phase_dag *dag, int src_id, int dest_id);

int
phase_dag_del_link(struct phase_dag *dag, int src_pid, int dest_pid);

int
phase_dag_get_task(struct phase_dag *dag, int pid, struct phase_task **task);

int
phase_dag_register_task(
                        struct phase_dag *dag,
                        int pid,
                        struct phase_task **task);

int
phase_dag_map_pid2index(struct phase_dag *dag, int pid);

int
phase_dag_find_index_from_pid(struct phase_dag *dag, int pid);

#endif

