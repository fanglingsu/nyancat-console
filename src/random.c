#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "random.h"

/**
 * Initializes the random number generator with time based seed.
 */
void random_init(void)
{
    srand(time(0));
}

/**
 * Retrieves a random number in range from given floor till ceil.
 */
int random_range(unsigned floor, unsigned ceil)
{
    assert(ceil > 0);
    return floor + (int)((ceil + 0.5 - floor) * rand() / (RAND_MAX + 1.0));
}

/**
 * Retrieves a random number in range from given floor till ceil in given
 * steps.
 */
int random_range_step(unsigned floor, unsigned ceil, int step)
{
    assert(step > 0);
    return random_range((int)(floor / step), (int)(ceil / step)) * step;
}
