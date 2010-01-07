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
phase_task_reset(struct phase_task *t)
{
    int flag;

    if (!t)
        return ERR_PHASE_TASK_NULL;
    t->oncpu = NULL;
    t->pid = -1;
    t->index = -1;
    t->task = NULL;
    return SUCCESS;
}

static int
task_pool_reset(struct task_pool *tp)
{
    int i = 0;

    if (!tp)
        return ERR_TASK_POOL_NULL;

    for (i = 0; i < MAX_TASKS; i++) {
        tp->state[i] = TASK_POOL_STATE_UNUSED;
        phase_task_reset(&tp->tasks[i]);
    }
    return SUCCESS;
}

static int
link_pool_reset(struct link_pool *lp)
{
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
    return SUCCESS;
}

static int
phase_task_list_add_task(
                         struct phase_task_list *l,
                         struct phase_task *t,
                         int weight)
{
    if (phase_task_list_get_size(l) >= MAX_TASKS)
        return SUCCESS;

    return SUCCESS;
}

static int
phase_task_list_del_task(struct phase_task_list *l, struct phase_task *t)
{
    return SUCCESS;
}

static int
_phase_dag_add_link(
                    struct phase_dag *dag,
                    struct phase_task *src,
                    struct phase_task *dest,
                    int weight)
{
    phase_task_list_add_task(&src->dest_list, dest, weight);
    phase_task_list_add_task(&dest->src_list, src, weight);
    return SUCCESS;
}

static int
_phase_dag_del_link(
                    struct phase_dag *dag,
                    struct phase_task *src,
                    struct phase_task *dest)
{
    phase_task_list_del_task(&src->dest_list, dest);
    phase_task_list_del_task(&dest->src_list, src);
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
    struct phase_task *src_task = NULL;
    struct phase_task *dest_task = NULL;

    phase_dag_get_task(dag, src_pid, &src_task);
    phase_dag_get_task(dag, dest_pid, &dest_task);
    _phase_dag_add_link(dag, src_task, dest_task, weight);
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
    struct phase_task *src_task = NULL;
    struct phase_task *dest_task = NULL;

    phase_dag_get_task(dag, src_pid, &src_task);
    phase_dag_get_task(dag, dest_pid, &dest_task);
    _phase_dag_del_link(dag, src_task, dest_task);

    return SUCCESS;
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
phase_dag_get_task(
                   struct phase_dag *dag,
                   int pid,
                   struct phase_task **task)
{
    return SUCCESS;
}

/**
 * phase_dag_register_task
 * @dag: phase dag
 * @pid: pid
 * @return: error code
 *
 * Assign PID to a phase_task
 */
int
phase_dag_register_task(struct phase_dag *dag, int pid)
{
    return SUCCESS;
}

