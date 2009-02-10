#include "genload.h"
#include "parser.h"
#include "errors.h"
#include "mbench-mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <mpi.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define MAX_INT 	32767

void script_usage()
{
}

/*---------------------------------------------------------------------
 * build_action_table function
 *
 * it make action-table from opts (options) of command-line
 * Return : 0 if SUCCESS, -1 if FAIL
 *
 * 		- Open file script
 * 		- read line to get ACTION
 *		- Terminative signal: BEGIN NLOOPS ... END
 *
 * Script forms:
 * 		BEGIN 100
 *		...
 *		ACT_SEND 12 2
 *		...
 *		END
 *
 *---------------------------------------------------------------------*/
int build_action_table(const struct cmdoptions *opts,
        struct action_table *acttbl)
{
    FILE *f; // id of file script when openning
    int i, err = 0, num1 = 0, num2 = 0;
    int count = 0;
    char action[8];//, args[64];
    struct action_struct *next, *cur = NULL;

    f = fopen(opts->script_file, "r");
    if (f == NULL)
        return -1;

    acttbl->nprocs = opts->nprocs;
    for (i = 0; i < MAX_PROCS; i++) {
        acttbl->script[i].actions = NULL;
    }

    /*
     * Read script file to build action table
     */
    while (opts->nprocs >= count) {
        //		int nsteps = 0;
        int cmdcode = ACT_MIN - 1;

        err = fscanf(f, "%s", action);

        if (err == EOF) {
            printf("Read script file completely, go to do now \n");
            break;
        }

        if (!strcmp(action, "begin")) {
            count++;
            fscanf(f, "%d", &num1);
            acttbl->script[count - 1].nloops = num1;
            acttbl->script[count - 1].nsteps = 0;
        } else if (!strcmp(action, "send")) {
            cmdcode = ACT_SEND;
        } else if (!strcmp(action, "sleep")) {
            cmdcode = ACT_SLEEP;
        } else if (!strcmp(action, "recv")) {
            cmdcode = ACT_RECV;
        } else if (!strcmp(action, "jmp")) {
            cmdcode = ACT_JUMP;
        } else if (!strcmp(action, "genload")) {
            cmdcode = ACT_GENLOAD;
        } else if (!strcmp(action, "ssend")) {
            cmdcode = ACT_SSEND;
        } else {
            printf("Can not understand this script \n");
            return -1;
        }

        if (cmdcode >= ACT_MIN) {
            num2 = 0;
            if (cmdcode == ACT_SLEEP)
                fscanf(f, "%d", &num1);
            else
                fscanf(f, "%d %d", &num1, &num2);

            next = malloc(sizeof(struct action_struct));
            if (!next) {
                sys_error("Can't allocate memory", __LINE__);
                return -1;
            }

            next->action = cmdcode;
            next->arg1 = num1;
            next->arg2 = num2;

            if (!acttbl->script[count - 1].actions)
                acttbl->script[count - 1].actions = next;
            else
                cur->next = next;

            cur = next;
            cur->next = NULL;

            acttbl->script[count - 1].nsteps += 1;
            //			printf("Script: %s %d %d \n",action,num1,num2);
        } /* if */
    } /* while */

    /* Maybe count < nprocs, so all remain processes will have scripts like [count-1]-th process */
    for (i = count; i < opts->nprocs; i++) {
        struct action_struct *cur1 = NULL;
        int nsteps;
        acttbl->script[i].nsteps = acttbl->script[count - 1].nsteps;
        nsteps = acttbl->script[i].nsteps;
        acttbl->script[i].nloops = acttbl->script[count - 1].nloops;
        cur = acttbl->script[count - 1].actions;
        while (cur) {
            next = malloc(sizeof(struct action_struct));
            if (!next) {
                sys_error("Can't allocate memory", __LINE__);
                return -1;
            }

            next->action = cur->action;
            next->arg1 = cur->arg1;
            next->arg2 = cur->arg2;

            if (!acttbl->script[i].actions)
                acttbl->script[i].actions = next;
            else
                cur1->next = next;

            cur1 = next;
            cur1->next = NULL;
            cur = cur->next;
        }
    }
    return 0;
}
/*---------------------------------------------------------------------
 * build_process function
 *
 * It does activities described in input
 * Return : 0 if SUCCESS, -1 if FAIL
 *
 *---------------------------------------------------------------------*/
int build_process(const struct action_script *script, int rank)
{
    int i, loop_flag, err = 0;
    //	int *tmp;
    struct action_struct *cur;
    MPI_Status status;

    loop_flag = 1;
    /*	tmp = malloc(sizeof(i)*script->nsteps);

     for (i=0; i<script->nsteps; i++)
     tmp[i] = MAX_INT;
     */
    if (script->nloops < 0)
        loop_flag = -1;

    /*
     * Loop forever if script->nloops < 0
     * else only loop (script->nloops) times
     */
    //	printf("nsteps=%d, nloops=%d, flag=%d \n", script->nsteps, script->nloops,loop_flag);
    for (i = 0; i * loop_flag < loop_flag * script->nloops; i = (i + 1)
            * (loop_flag + 1) / 2) {
        int j;
        cur = script->actions;
        for (j = 0; j < script->nsteps; j++) {
            int arg1, arg2, action;
            int *buf = NULL, k;

            action = cur->action;
            arg1 = cur->arg1;
            arg2 = cur->arg2;
            cur = cur->next;

            switch (action) {
            case ACT_SEND:
                /* call malloc */
                buf = malloc(sizeof(*buf) * arg1);
                err = MPI_Send(&buf, arg1, MPI_INT, arg2, 0, MPI_COMM_WORLD);

                if (err != MPI_SUCCESS)
                    printf("p%d : Send %d , %d : failed \n", rank, arg1, arg2);
                else
                    printf("p%d : Send %d , %d\n", rank, arg1, arg2);
                break;

            case ACT_SSEND:
                /* call malloc */
                buf = malloc(sizeof(*buf) * arg1);
                err = MPI_Send(&buf, arg1, MPI_INT, arg2, 0, MPI_COMM_WORLD);

                if (err != MPI_SUCCESS)
                    printf("p%d : SSend %d , %d : failed \n", rank, arg1, arg2);
                else
                    printf("p%d : SSend %d , %d\n", rank, arg1, arg2);
                break;

            case ACT_RECV:
                /* call malloc ? */
                buf = malloc(sizeof(*buf) * arg1);
                err = MPI_Recv(&buf, arg1, MPI_INT, arg2, 0, MPI_COMM_WORLD,
                &status);

                if (err != MPI_SUCCESS)
                    printf("p%d : Recv %d , %d : failed \n", rank, arg1, arg2);
                else
                    printf("p%d : Recv %d , %d\n", rank, arg1, arg2);

                /*					if (buf)
                 free(buf);
                 printf("after free recv %d\n",buf);
                 buf = NULL;
                 */
                break;

            case ACT_GENLOAD:
                printf("p%d : Genload %d , %d \n", rank, arg1, arg2);
                for (k = 0; k < arg2; k++)
                    do_load(arg1);
                break;

            case ACT_SLEEP:
                printf("p%d : Sleep %d \n", rank, arg1);
                sleep(arg1);
                break;

            case ACT_JUMP:
                /*					if ((tmp[j] > arg2) || (tmp[j] == 0))
                 tmp[j] = arg2;

                 if (tmp[j] == arg2)
                 j += arg1;

                 tmp[j]--;
                 */
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

/*---------------------------------------------------------------------
 * Main function
 *
 * It does :
 *	- Parse input, get file script describing activities of all
 *	  processes, get number of processes, get index of genload
 *	  functions, ...
 *	- Based on input, build processes doing those activities
 *
 *---------------------------------------------------------------------*/
int main(int argc, char ** argv)
{
    struct cmdoptions opts; // options of command line
    struct action_table acttbl; // actions of all processes
    int rank, err, size, status;
    //	char *arg;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    err = parser_arg(argc, argv, &opts);
    if (err) {
        printf("Please read usage carefully \n");
        return 0;
    }

    /*
     * Set nice for each process & Set BATCH policy
     *
     char *strpid, *strnice;
     int pid, nice;
     pid = getpid();
     strpid = itostr(pid);
     //	nice = opts.nice;
     //	strnice = itostr(nice);

     if (!fork()){
     if (rank % 2 == 0)
     err = execlp("schedtool","schedtool", "-v","-a","0",strpid, 0);
     else
     err = execlp("schedtool","schedtool", "-v","-a","1",strpid, 0);

     MPI_Finalize();
     return 0;
     }
     *
     * End setting ...
     */

    wait(&status);

    err = build_action_table(&opts, &acttbl);
    if (err) {
        printf("Error in building action table \n");
        return 0;
    }

    build_process(&acttbl.script[rank], rank);

    MPI_Finalize();

    return 0;
}
