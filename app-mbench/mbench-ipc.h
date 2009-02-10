#ifndef MBENCH_IPC_H
#define MBENCH_IPC_H

#define MSQ_SEND 		1
#define MSQ_RECV 		2
#define SHM_READ		3
#define SHM_WRITE		4
#define ACT_GENLOAD		5
#define ACT_JUMP		6
#define ACT_MIN			MSQ_SEND

/*---------------------------------------------------------------------
 * action_struct
 *
 * Describe an action supported :
 * 			action		|	arg1		|	arg2
 *		----------------+---------------+-----------
 *			SYSV_GET	|	TYPE		|	KEY
 *			MSQ_SEND	|	BUFCOUNT	|	DESTID
 *			MSQ_RECV	|	SYSV_ID		|	SRCID
 *			SHM_READ	|	BUFCOUNT	|	DESTID
 *			MSQ_WRITE	|	BUFCOUNT	|	SRCID
 *			ACT_GENLOAD	|	GENLOADID	|	NLOOPS
 *			ACT_JUMP	|	NSTEPS		|	MODE
 *
 * TYPE		: MSQ, SEM, SHM
 * KEY 		: a number
 *---------------------------------------------------------------------
 * action_script
 *
 * Description: Information about actions of a process
 *---------------------------------------------------------------------
 * action_table
 *
 * Description: Information about actions of all processes
 *--------------------------------------------------------------------*/
struct action_struct {
    int action; // id of action
    int arg1; // arg1 of doing function
    int arg2; // arg2 of doing function
};

struct action_script {
    int nsteps; // number of steps in a loop
    int nloops; // number of loop of these activities; note: -1 = loop forever
    struct action_struct * actions; // array of actions in a loop
};

struct action_table {
    int nprocs; // number of processes
    struct action_script * script; // action-scripts of all processes
};

#endif /* MBENCH-MPI_H */
