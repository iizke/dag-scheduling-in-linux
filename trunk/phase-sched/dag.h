#ifndef DAG_H
#define DAG_H

/** How to mapping pid -> task struct */

struct phase_dag {
    //
};
 
int phase_dag_init(struct phase_dag *dag);
int phase_dag_free(struct phase_dag *dag);

#endif
