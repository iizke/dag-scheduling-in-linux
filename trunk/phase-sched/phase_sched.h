#ifndef PHASE_SCHED_MOD_H
#define PHASE_SCHED_MOD_H

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "phase_def.h"
#include "phase_sched.h"
#include "dag.h"

// command codes
#define PHASE_SCHED_CMD_ADD    0
#define PHASE_SCHED_CMD_DEL    1

struct phase_sched {
    struct subsystem subsystem;
    struct sysfs_ops phase_ops;
    struct attribute phase_attrs[2];
    struct kobj_type phase_ktype;
    struct phase_req req;
    struct phase_attr req_attr;
    /** cpuload_list: list of cpu load for all cpus */
    struct cpuload_list cpuload_list;
    /** DAG data: information about DAG tree of processes */
    static struct phase_dag dag;
};

struct phase_req {
    int cmd;
    int src_pid;
    int dest_pid;
    int weight;
};

/** phase_sched: global variable */
struct phase_sched phase_sched;

int phase_sched_do_req(struct phase_req *req);
/** reset all data */
int phase_sched_reset(void);

//int rebuild_dag(struct phase_req *req);
//int phase_sched_schedule(struct phase_req *req);

#endif
