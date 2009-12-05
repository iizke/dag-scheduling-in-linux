#ifndef PHASE_SCHED_MOD_H
#define PHASE_SCHED_MOD_H

#define SUCCESS                 1
#define FAILED                  0

#define PHASE_SCHED_ADD_CONN    0
#define PHASE_SCHED_DEL_CONN    1

/** application's request */
struct req_msg {
    int cmd;
    int src_pid;
    int dest_pid;
    int weight;
};

/** cpu_list: list of cpus */
struct cpu_list {
    int size;
};

/** list of cpu load of all cpus */
struct cpuload_list {
    struct cpu_list load_array[MAX_LOADS];
    int least_load_id;    
};

/** phase_task_struct: task's info in DAG */
struct phase_task_struct {
    int pid;
    struct phase_task_list dest_list;
    struct phase_task_list src_list;
};

/** list of phase_task_struct */
struct phase_task_list {
    int size;
};

/** dag data */
struct dag {
};

/** cpuload_list (shared data): list of cpu load for all cpus */
static struct cpuload_list cpuload_list;
/** DAG data (shared data): information about DAG tree of processes */
static struct dag dag;

/** add a connection between src and dest */
int phase_sched_add_connection(int pid_src, int pid_dest, int weight);
/** remove a connection between src and dest */
int phase_sched_remove_connection(int pid_src, int pid_dest, int weight);
/** reset all data */
int phase_sched_reset();

int rebuild_dag(struct req_msg *msg);
int phase_sched_schedule(struct req_msg *msg);

#endif