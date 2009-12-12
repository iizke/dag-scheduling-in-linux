/*
 * phase_sysfs.c
 *
 *  Created on: Dec 11, 2009
 *      Author: roseman
 */

/***************************************************************************
 * PHASE SCHEDULING MODULE
 *
 * File: phase_sysfs.c
 *
 * Support interactions between user and kernel
 *
 * Based on sysfs, we assign requests to phase-sched module.
 *     /sys
 *       |---> phase_sched
 *                 |---> req
 *
 * "req" forms: cmd src_pid dest_pid weight
 *
 ***************************************************************************/
#include "phase_sysfs.h"
#include "phase_def.h"
#include "phase_sched.h"

static ssize_t
phase_sched_show(struct kobject *kobj, struct attribute *attr, char *buffer)
{
    struct phase_attr *_attr;
    struct subsystem *subsystem;
    struct kset *kset;
    struct phase_sched * ps;

    if ((!kobj) || (!attr) || (!buffer)) return 0;

    kset = container_of(kobj, struct kset, kobj);
    subsystem = container_of(kset, struct subsystem, kset);
    ps = container_of(subsystem, struct phase_sched, subsystem);
    _attr = container_of(attr, struct phase_attr, attr);

    return _attr->show((void*) ps, buffer);
}

static ssize_t
phase_sched_store(struct kobject *kobj, struct attribute *attr,
                const char *buffer, size_t size)
{
    struct phase_attr *_attr;
    struct kset *kset;
    struct subsystem *subsystem;
    struct phase_sched *phase_sched;

    if ((!kobj) || (!attr) || (!buffer)) return 0;

    kset = container_of(kobj, struct kset, kobj);
    subsystem = container_of(kset, struct subsystem, kset);
    phase_sched = container_of(subsystem, struct phase_sched, subsystem);
    _attr = container_of(attr, struct phase_attr, attr);

    return _attr->store((void*) phase_sched, buffer, size);
}

static void
phase_sched_release(struct kobject *kobj)
{
    return;
}

ssize_t phase_sched_show_req (void* obj, char *buf)
{
    struct phase_sched *ps = obj;
    int ret = sprintf(buf, "%d %d %d %d", 
                    ps->req.cmd, 
                    ps->req.src_pid,
                    ps->req.dest_pid,
                    ps->req.weight);
    return ret;
}

ssize_t phase_sched_store_req (void* obj, const char *buf, size_t size)
{
    struct  phase_sched *ps = obj;
    struct phase_req * req = (struct phase_req*) buf;
    
    if (size != sizeof(struct phase_req)) {
        printk("Input type is not expected, storing request is end \n");
        return 0;
    }
    
    if (!ps)
        return 0;
    
    ps->req.cmd = req->cmd;
    ps->req.src_pid = req->src_pid;
    ps->req.dest_pid = req->dest_pid;
    ps->req.weight = req->weight;
    
    return size;
}

int phase_sysfs_init(struct phase_sched *ps)
{
    if (!ps)
        return FAIL;
    ps->phase_ops.show = phase_sched_show;
    ps->phase_ops.store = phase_sched_store;
    ps->req_attr.attr.name = __stringify(req);
    ps->req_attr.attr.mode = 0666;
    ps->req_attr.attr.owner = THIS_MODULE;
    ps->req_attr.show = phase_sched_show_req;
    ps->req_attr.store = phase_sched_store_req;
    //ps->phase_attrs[0] = &ps->req_attr.attr;
    memcpy(&ps->phase_attrs[0], &(ps->req_attr.attr), sizeof(struct attribute));
    //ps->phase_attrs[1] = NULL;
    memset(&ps->phase_attrs[1], 0, sizeof(struct attribute));
    ps->phase_ktype.release = phase_sched_release;
    ps->phase_ktype.sysfs_ops = &ps->phase_ops;
    *(ps->phase_ktype.default_attrs) = ps->phase_attrs;
    return SUCCESS;
}

int build_phase_sysfs_tree(struct phase_sched *ps)
{
    struct kset *kset = NULL;

    if (!ps)
        return 0;

    kset = &(ps->subsystem.kset);
    kobject_set_name(&(kset->kobj), "phase_sched");
    kset->kobj.ktype = &ps->phase_ktype;
    kset->ktype = NULL;
    subsystem_register(&ps->subsystem);

    return SUCCESS;
}

int free_phase_sysfs_tree(struct phase_sched *ps)
{
    subsystem_unregister(&ps->subsystem);
    return SUCCESS;
}
