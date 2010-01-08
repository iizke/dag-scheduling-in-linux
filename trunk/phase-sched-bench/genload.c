//#include "errors.h"
#include "genload.h"
#include <math.h>

void do_sqrt()
{
    int i;
    double result, tmp;
    result = 1;
    for (i = 1; i < 9999999; i++) {
        tmp = sqrt(sqrt(sqrt(sqrt(i * sqrt(i)))));
        result /= pow(result, tmp);
    }
}

void do_heavy()
{
    int i;
    for (i = 0; i < 10; i++)
        do_sqrt();
}

void do_load(int genloadid)
{
    switch (genloadid) {
    //		case GENLOAD_MIN:
    case GENLOAD_SQRT:
        do_sqrt();
        break;
    case GENLOAD_HEAVY:
        do_heavy();
        break;
    default:
        do_sqrt();
        break;
    }
}
