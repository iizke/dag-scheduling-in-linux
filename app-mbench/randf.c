/*
 * randf.c
 * Collection of random functions
 *
 *  Created on: Feb 13, 2009
 *      Author: roseman
 */

#include <stdlib.h>

/*
 * TODO: srand
 * Generate 0/1, with probability of 1 is density/100
 */
double my_srand(int seed, int density)
{
    double val;
    srand48(seed);
    val = drand48();
    if (val < ((double)density/100))
        return 1;
    return 0;
}
