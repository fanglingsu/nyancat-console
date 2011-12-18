#include <sys/time.h>
#include <time.h>
#include "clock.h"

/* holds current base time that where set in clock_init */
game_time base_time;

static double clock_get_base(void);


/**
 * Initializes the clock by setting base_time to now.
 */
void clock_init(void)
{
    extern game_time base_time;
    base_time = clock_get_base();
}

/**
 * Retrieves the current relative game time.
 */
game_time clock_get_relative(void)
{
    extern game_time base_time;

    return clock_get_base() - base_time;
}

/**
 * Retrieves the current real time.
 */
static game_time clock_get_base(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1.0e-6;
}
