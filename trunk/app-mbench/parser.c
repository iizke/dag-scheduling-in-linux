#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//#include "errors.h"
#include "genload.h"
#include "parser.h"

/*---------------------------------------------------------------------
 * parser_arg_mpi function
 *
 * It parses input and return file script describing activities of all
 *			  processes, get number of processes, get index of genload
 *			  functions, ...
 *
 * Command-line options:
 * 		[-n] num : number of processes
 *		[-f] file : file script
 *		[-p] nice : nice value
 * 		[-b] batch: run as batch policy
 *
 *---------------------------------------------------------------------*/
int parser_arg(int argc, char **argv, struct cmdoptions *opts)
{
    int opt;
    int errflag = 0;

    extern char * optarg; // command-line options

    /* Default value */
    opts->nprocs = 2;
    strcpy(opts->script_file, "script.txt");
    opts->nice = 0;
    opts->batch = 0;
    /* parse command line */
    while ((opt = getopt(argc, argv, "n:f:p:o:b")) != EOF) {
        switch (opt) {
        case 'n':
            opts->nprocs = atoi(optarg);
            break;
        case 'f':
            strcpy(opts->script_file, optarg);
            break;
        case 'o':
            strcpy(opts->output_file, optarg);
            break;
        case 'p':
            opts->nice = atoi(optarg);
            break;
        case 'b':
            opts->batch = 1;
            break;
        default:
            errflag = 0;
            break;
        }
    }

    if (errflag) {
        fprintf(stderr, "Error : parsing arg failed \n");
        return -1;
    }
    return 0;
}
