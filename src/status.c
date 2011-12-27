#include "status.h"

enum item {
    StatusRuntime,
    StatusFrames,
    StatusDebug
};

static struct status {
    const char *mode;
    gametime_t runtime;
} status = {NULL, 0.0};

static void printstatusf(const char *format, ...);

/**
 * @mode: current mode name
 *
 * Set the game mode name to the status.
 */
void
status_set_mode(const char *mode)
{
    extern struct status status;

    status.mode = mode;
}

/**
 * @runtime: duration of running game in seconds.
 *
 * Set the runtime in seconds to the status.
 */
void
status_set_runtime(const gametime_t runtime)
{
    extern struct status status;

    status.runtime = runtime;
}

/**
 * Print out contens of the status to the status window.
 */
void
status_print(void)
{
    extern struct status status;

    printstatusf(
        "%s: %.2lfs",
        status.mode,
        status.runtime
    );
}

/**
 * @format: Formatstring like in fprintf
 *
 * Prints out message to status line.
 */
static void
printstatusf(const char *format, ...)
{
    extern struct nyancat nc;
    va_list ap;

    werase(nc.ui.status);
    va_start(ap, format);
    vwprintw(nc.ui.status, format, ap);
    va_end(ap);
    wrefresh(nc.ui.status);
}
