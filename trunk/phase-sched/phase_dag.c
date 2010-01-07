#include <linux/sched.h>

#include "phase_dag.h"
#include "phase_def.h"

/**
 * phase_dag_init
 * @dag: phase DAG
 * @return: error code
 *
 * Initialize elements in phase_dag
 */
int
phase_dag_init(struct phase_dag *dag)
{
    phase_dag_reset(dag);
    return SUCCESS;
}

/**
 * phase_dag_free
 * @dag: phase DAG
 * @return: error code
 *
 * Free dynamic info before existing
 */
int
phase_dag_free(struct phase_dag *dag)
{
    if (!dag)
        return ERR_PHASE_DAG_NULL;
    return SUCCESS;
}

static int
phase_task_list_reset(struct phase_task_list *l)
{
    if (!l)
        return ERR_PHASE_TASK_LIST_NULL;
    l->size = 0;
    l->last_link = NULL;
    return SUCCESS;
}

static int
phase_task_reset(struct phase_task *t)
{
    if (!t)
        return ERR_PHASE_TASK_NULL;
    t->oncpu = NULL;
    t->pid = -1;
    t->index = -1;
    t->task = NULL;
    phase_task_list_reset(&t->src_list);
    phase_task_list_reset(&t->dest_list);
    return SUCCESS;
}

static int
task_pool_reset(struct task_pool *tp)
{
    int i = 0;

    if (!tp)
        return ERR_TASK_POOL_NULL;

    tp->inused_tasks = 0;
    for (i = 0; i < MAX_TASKS; i++) {
        tp->state[i] = TASK_POOL_STATE_UNUSED;
        phase_task_reset(&tp->tasks[i]);
    }
    return SUCCESS;
}

static int
phase_link_reset(struct phase_link *l)
{
    if (!l)
        return ERR_PHASE_LINK_NULL;
    l->dest_index = -1;
    l->src_index = -1;
    l->src_next = NULL;
    l->dest_next = NULL;
    l->src_prev = NULL;
    l->dest_prev = NULL;
    l->state = LINK_POOL_STATE_UNUSED;
    l->weight = 0;
    return SUCCESS;
}

static int
link_pool_reset(struct link_pool *lp)
{
    int i = 0;
    int j = 0;

    if (!lp)
        return ERR_LINK_POOL_NULL;
    for (i = 0; i < MAX_TASKS; i++) {
        for (j = 0; j < MAX_TASKS; j++) {
            phase_link_reset(&lp->links[i][j]);
        }
    }

    return SUCCESS;
}

/**
 * phase_dag_reset
 * @dag: phase DAG
 * @return: error code
 *
 * Reset all elements of the phase_dag structure
 */
int
phase_dag_reset(struct phase_dag *dag)
{
    if (!dag)
        return ERR_PHASE_DAG_NULL;
    task_pool_reset(&dag->task_pool);
    link_pool_reset(&dag->link_pool);
    return SUCCESS;
}

static int
phase_task_list_add_srclink(struct phase_task_list *l, struct phase_link *link)
{
    if (!link)
        return ERR_PHASE_LINK_NULL;
    if (!l)
        return ERR_PHASE_TASK_LIST_NULL;

    if (link->src_next != NULL)
        return SUCCESS;

    if (l->last_link == NULL) {
        l->last_link = link;
        link->src_next = link;
        link->src_prev = link;
        l->size = 1;
    } else {
        link->src_next = l->last_link->src_next;
        l->last_link->src_next = link;
        link->src_prev = l->last_link;
        link->src_next->src_prev = link;
        l->last_link = link;
        l->size++;
    }

    return SUCCESS;
}

static int
phase_task_list_add_destlink(struct phase_task_list *l, struct phase_link *link)
{
    if (!link)
        return ERR_PHASE_LINK_NULL;
    if (!l)
        return ERR_PHASE_TASK_LIST_NULL;

    if (link->dest_next != NULL)
        return SUCCESS;

    if (l->last_link == NULL) {
        l->last_link = link;
        link->dest_next = link;
        link->dest_prev = link;
        l->size = 1;
    } else {
        link->dest_next = l->last_link->dest_next;
        l->last_link->dest_next = link;
        link->dest_prev = l->last_link;
        link->dest_next->dest_prev = link;
        l->last_link = link;
        l->size++;
    }

    return SUCCESS;
}

static int
_phase_dag_add_link(struct phase_dag *dag, int src_id, int dest_id, int weight)
{
    struct phase_link *link = NULL;
    struct phase_task *src_task = NULL;
    struct phase_task *dest_task = NULL;

    if (!dag)
        return ERR_PHASE_DAG_NULL;
    link = &(dag->link_pool.links[src_id][dest_id]);
    link->state = LINK_POOL_STATE_INUSED;
    link->dest_index = dest_id;
    link->src_index = src_id;
    link->weight = weight;
    src_task = &dag->task_pool.tasks[src_id];
    dest_task = &dag->task_pool.tasks[dest_id];
    phase_task_list_add_destlink(&src_task->dest_list, link);
    phase_task_list_add_srclink(&dest_task->src_list, link);
    return SUCCESS;
}

/**
 * phase_dag_add_link
 * @dag: phase dag
 * @src_pid: source pid
 * @dest_pid: destination pid
 * @weight: weight of link
 * @return: error code
 *
 * Add a link between nodes in DAG
 */
int
phase_dag_add_link(struct phase_dag *dag, int src_pid, int dest_pid, int weight)
{
    int src_id = 0;
    int dest_id = 0;
    int flag = SUCCESS;

    src_id = phase_dag_find_index_from_pid(dag, src_pid);
    if (src_id < 0)
        return src_id;
    dest_id = phase_dag_find_index_from_pid(dag, dest_pid);
    if (dest_id < 0)
        return dest_id;
    flag = _phase_dag_add_link(dag, src_id, dest_id, weight);
    return flag;
}

static int
phase_task_list_del_destlink(struct phase_task_list *l, struct phase_link *link)
{
    if (!link)
        return ERR_PHASE_LINK_NULL;
    if (!l)
        return ERR_PHASE_TASK_LIST_NULL;

    if (link->dest_next == NULL)
        return SUCCESS;

    if (l->size <= 1) {
        l->last_link = NULL;
        link->dest_next = NULL;
        link->dest_prev = NULL;
        l->size = 0;
    } else {
        struct phase_link *next = link->dest_next;
        struct phase_link *prev = link->dest_prev;
        prev->dest_next = next;
        next->dest_prev = prev;
        if (l->last_link == link)
            l->last_link = next;
        l->size--;
    }

    return SUCCESS;
}

static int
phase_task_list_del_srclink(struct phase_task_list *l, struct phase_link *link)
{
    if (!link)
        return ERR_PHASE_LINK_NULL;
    if (!l)
        return ERR_PHASE_TASK_LIST_NULL;

    if (link->src_next == NULL)
        return SUCCESS;

    if (l->size <= 1) {
        l->last_link = NULL;
        link->src_next = NULL;
        link->src_prev = NULL;
        l->size = 0;
    } else {
        struct phase_link *next = link->src_next;
        struct phase_link *prev = link->src_prev;
        prev->src_next = next;
        next->src_prev = prev;
        if (l->last_link == link)
            l->last_link = next;
        l->size--;
    }

    return SUCCESS;
}

static int
_phase_dag_del_link(struct phase_dag *dag, int src_id, int dest_id)
{
    struct phase_link *link = NULL;
    struct phase_task *src_task = NULL;
    struct phase_task *dest_task = NULL;

    if (!dag)
        return ERR_PHASE_DAG_NULL;
    link = &dag->link_pool.links[src_id][dest_id];
    link->state = LINK_POOL_STATE_UNUSED;
    src_task = &dag->task_pool.tasks[src_id];
    dest_task = &dag->task_pool.tasks[dest_id];
    phase_task_list_del_destlink(&src_task->dest_list, link);
    phase_task_list_del_srclink(&dest_task->src_list, link);
    return SUCCESS;
}

/**
 * phase_dag_del_link
 * @dag: phase dag
 * @src_pid: source pid
 * @dest_pid: destination pid
 * @return: error code
 *
 * Remove a link in DAG
 */
int
phase_dag_del_link(struct phase_dag *dag, int src_pid, int dest_pid)
{
    int src_id = 0;
    int dest_id = 0;
    int flag = SUCCESS;

    src_id = phase_dag_find_index_from_pid(dag, src_pid);
    if (src_id < 0)
        return src_id;
    dest_id = phase_dag_find_index_from_pid(dag, dest_pid);
    if (dest_id < 0)
        return dest_id;
    flag = _phase_dag_del_link(dag, src_id, dest_id);
    return flag;
}

/**
 * phase_dag_get_task
 * @dag: phase dag
 * @pid: pid
 * @tasK: pointer to appropriate phase_task (output)
 * @return: error code
 *
 * Get phase_task accompany with pid
 */
int
phase_dag_get_task(struct phase_dag *dag, int pid, struct phase_task **task)
{
    int id = phase_dag_find_index_from_pid(dag, pid);
    if (id < 0) {
        (*task) = NULL;
        return id;
    }
    (*task) = &dag->task_pool.tasks[id];
    return SUCCESS;
}

/**
 * phase_dag_register_task
 * @dag: phase dag
 * @pid: pid
 * @task: (output) phase task for pid
 * @return: error code
 *
 * Assign PID to a phase_task
 */
int
phase_dag_register_task(
                        struct phase_dag *dag,
                        int pid,
                        struct phase_task **task)
{
    int id = 0;
    if (!dag)
        return ERR_PHASE_DAG_NULL;
    id = phase_dag_map_pid2index(dag, pid);
    if (id < 0) {
        (*task) = NULL;
        return id;
    }
    dag->task_pool.tasks[id].pid = pid;
    dag->task_pool.tasks[id].index = id;
    dag->task_pool.tasks[id].task = find_task_by_pid(pid);
    return SUCCESS;
}

static int
task_pool_find_empty_task(struct task_pool *tp, int pid)
{
    int i = 0;
    int id = 0;
    for (i = 0; i < MAX_TASKS; i++) {
        id = (pid + i) % MAX_TASKS;
        if (tp->state[id] == TASK_POOL_STATE_UNUSED)
            return id;
    }
    return ERR_TASK_POOL_FULL;
}

/**
 * phase_dag_map_pid2index
 * @dag: phase dag
 * @pid: pid
 * @return: error code or index-value
 *
 * Map pid value to an index value (range from 0 -> MAX_TASKS-1).
 * This index value is not used before, and assign to this pid.
 */
int
phase_dag_map_pid2index(struct phase_dag *dag, int pid)
{
    int ret = 0;

    if (!dag)
        return ERR_PHASE_DAG_NULL;
    if (task_pool_is_full(&dag->task_pool))
        return ERR_TASK_POOL_FULL;

    ret = task_pool_find_empty_task(&dag->task_pool, pid);
    return ret;
}

static int
task_pool_find_task(struct task_pool *tp, int pid)
{
    int i = 0;
    int id = 0;
    for (i = 0; i < MAX_TASKS; i++) {
        id = (pid + i) % MAX_TASKS;
        if ((tp->state[id] == TASK_POOL_STATE_INUSED) && (tp->tasks[id].pid
                        == pid))
            return id;
    }
    return ERR_TASK_POOL_FIND_TASK_FAILED;
}

/**
 * phase_dag_find_index_from_pid
 * @dag: phase dag
 * @pid: pid
 * @return: error code or index
 *
 * Determine the index value of a pid.
 * This pid has to be assigned an index value before.
 */
int
phase_dag_find_index_from_pid(struct phase_dag *dag, int pid)
{
    int ret = 0;

    if (!dag)
        return ERR_PHASE_DAG_NULL;
    ret = task_pool_find_task(&dag->task_pool, pid);
    return ret;
}
