#ifndef PHASE_SCHED_MOD_H
#define PHASE_SCHED_MOD_H

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "phase_def.h"

// command codes
#define PHASE_SCHED_ADD_CONN    0
#define PHASE_SCHED_DEL_CONN    1

/** application's request */
struct req_msg {
    int cmd;
    int src_pid;
    int dest_pid;
    int weight;
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
int phase_sched_reset(void);

int rebuild_dag(struct req_msg *msg);
int phase_sched_schedule(struct req_msg *msg);

#endif
