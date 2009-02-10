#ifndef MBENCH_MPI_H
#define MBENCH_MPI_H

#define ACT_SEND 		1
#define ACT_RECV 		2
#define ACT_GENLOAD		3
#define ACT_JUMP		4
#define ACT_SLEEP		5
#define ACT_SSEND		6
#define ACT_MIN			ACT_SEND

#define DEF_BUFCOUNT		1
#define DEF_GENLOAD		GENLOAD_MIN
#define DEF_NLOOPS		1
#define DEF_NSTEPS		1

#define MAX_PROCS		45
/*---------------------------------------------------------------------
 * action_struct
 *
 * Describe an action supported :
 * 		action		|	arg1		|	arg2
 *		----------------+-----------------------+--------------
 *		ACT_SEND	|	BUFCOUNT	|	DESTID
 *		ACT_RECV	|	BUFCOUNT	|	SRCID
 *		ACT_GENLOAD	|	GENLOADID	|	NLOOPS
 *		ACT_JUMP	|	NSTEPS		|	MODE
 *		ACT_SLEEP	|	SECONDS		|	NULL(0)
 *
 * Default:
 * 		BUFCOUNT 	= DEF_BUFCOUNT
 *		GENLOADID 	= GENLOAD_MIN
 *		NLOOPS		= DEF_NLOOPS
 *		NSTEPS		= DEF_NSTEPS
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
    struct action_struct * next;
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
    struct action_script script[MAX_PROCS]; // action-scripts of all processes
};

#endif /* MBENCH-MPI_H */
