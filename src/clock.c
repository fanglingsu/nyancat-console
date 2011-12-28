#include <sys/time.h>
#include <stddef.h>
#include "clock.h"

/* holds current base time that where set in clock_init */
gametime_t base_time;

/* holds the relative game time if clock where frozen */
gametime_t frozen_time;

static double clock_get_base(void);

/**
 * Initializes the clock by setting base_time to now.
 */
void clock_init(void)
{
    extern gametime_t base_time;
    base_time = clock_get_base();
}

/**
 * Freezes the clock.
 */
void clock_freeze(void)
{
    extern gametime_t frozen_time;

    if (0 == frozen_time) {
        /* save current relative time as frozen time */
        frozen_time = clock_get_relative();
    }
}

/**
 * Thaws the frozen clock.
 */
void clock_thaw(void)
{
    extern gametime_t frozen_time;

    if (0 != frozen_time) {
        /* calculate new base_time */
        base_time = clock_get_base() - frozen_time;
        frozen_time = 0;
    }
}

/**
 * Retrieves the current relative game time.
 */
gametime_t clock_get_relative(void)
{
    extern gametime_t frozen_time;
    extern gametime_t base_time;

    if (0 == frozen_time) {
        return clock_get_base() - base_time;
    }

    return frozen_time;
}

/**
 * Retrieves the current real time.
 */
static gametime_t clock_get_base(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1.0e-6;
}
