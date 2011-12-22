#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include "io.h"
#include "main.h"
#include "config.h"
#include "clock.h"

static int io_select(struct timeval *timeout);

/**
 * @time: time when to return if no key is pressed before.
 *
 * Waits for the next keypress or the reaching of timeout, wichever occours
 * first. Returns 0 if the fuction returned cause of the timeout. Otherwise
 * this function returned because of a pressed key.
 */
int
io_wait_for_key(game_time time)
{
    if (0 == time) {
        return io_select(NULL);
    }
    game_time delta;
    struct timeval timeout;

    delta = time - clock_get_relative();

    /* timeout already reached */
    if (delta <= 0) {
        return 0;
    }

    timeout.tv_sec = delta / SECOND;
    timeout.tv_usec = delta - timeout.tv_sec * 1e6;

    return io_select(&timeout);
}

/**
 * Check if input is available on stdin or the timeout reached.
 * If timeout reached return 0 else input available.
 */
static int
io_select(struct timeval *timeout)
{
    int n;
    fd_set input;

    /* initialize the input set to observe stdin (fd 0) */
    FD_ZERO(&input);
    FD_SET(STDIN, &input);

    n = select(FD_SETSIZE, &input, NULL, NULL, timeout);

    /* see if there was an error */
    if (n < 0) {
        error_exit("Could not select from stdin");
    }
    return n;
}
