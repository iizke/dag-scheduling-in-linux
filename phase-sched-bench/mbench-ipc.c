#include "errors.h"
#include "genload.h"
#include "parser.h"
#include "mbench-ipc.h"
#include <errno.h>
#include <stdio.h>

//int main (int argc, char *argv[])
int main()
{
    printf("Hello world \n");
    printf("Do GENLOAD_HEAVY ... \n");
    do_load(GENLOAD_HEAVY);
    printf("Done. \n");
    return 10;
}
