#include "errors.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>

/*---------------------------------------------------------------------
 * error function
 *
 * Print out message and exit
 *
 *---------------------------------------------------------------------*/
void errors(const char * msg, int line)
{
    fprintf(stderr, "ERROR: [line %d] %s \n", line, msg);
    //	exit(-1);
}

/*---------------------------------------------------------------------
 * sys_error function
 *
 * Create system error message, print out message, and exit
 *
 *---------------------------------------------------------------------*/
void sys_error(const char * msg, int line)
{
    char syserr_msg[256];
    sprintf(syserr_msg, "%s : %s \n", msg, strerror(errno));
    errors(syserr_msg, line);
}
