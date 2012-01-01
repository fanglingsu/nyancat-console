/**
 * nyancat-console - a clone of the nyancat lost in space flash game.
 *
 * Copyright (C) 2011-2012 Daniel Carl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "io.h"
#include "config.h"

static int io_select(struct timeval *);

/**
 * @time: time when to return if no key is pressed before.
 *
 * Waits for the next keypress or the reaching of timeout, wichever occours
 * first. Returns 0 if the fuction returned cause of the timeout. Otherwise
 * this function returned because of a pressed key.
 */
int io_wait_for_key(gametime_t time)
{
    if (0 == time) {
        return io_select(NULL);
    }
    gametime_t delta;
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
static int io_select(struct timeval *timeout)
{
    int n;
    fd_set input;

    /* initialize the input set to observe stdin (fd 0) */
    FD_ZERO(&input);
    FD_SET(STDIN, &input);

    /*n = select(FD_SETSIZE, &input, NULL, NULL, timeout);*/

    /* repeat it as long as temporary error code EINTR occours */
    n = TEMP_FAILURE_RETRY(select(FD_SETSIZE, &input, NULL, NULL, timeout));

    /* see if there was an error */
    if (n < 0) {
        error_exit("Could not select from stdin");
    }
    return n;
}
