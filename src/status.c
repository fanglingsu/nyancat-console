#include "status.h"
#include "main.h"

enum item {
    StatusRuntime,
    StatusFrames,
    StatusDebug
};

static struct Status {
    const char *mode;
    game_time runtime;
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
    extern struct Status status;

    status.mode = mode;
}

/**
 * @runtime: duration of running game in seconds.
 *
 * Set the runtime in seconds to the status.
 */
void
status_set_runtime(const game_time runtime)
{
    extern struct Status status;

    status.runtime = runtime;
}

/**
 * Print out contens of the status to the status window.
 */
void
status_print(void)
{
    extern struct Status status;

    printstatusf("%s: %.2lfs", status.mode, status.runtime);
}

/**
 * @format: Formatstring like in fprintf
 *
 * Prints out message to status line.
 */
static void
printstatusf(const char *format, ...)
{
    extern struct Nyancat nc;
    va_list ap;

    werase(nc.ui.status);
    va_start(ap, format);
    vwprintw(nc.ui.status, format, ap);
    va_end(ap);
    wrefresh(nc.ui.status);
}
