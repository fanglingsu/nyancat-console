#include "main.h"
#include "status.h"
#include "gamemode.h"
#include "clock.h"

enum item {
    StatusRuntime,
    StatusFrames,
    StatusDebug
};

static void printstatusf(const char *format, ...);

/**
 * Print out contens of the status to the status window.
 */
void status_print(void)
{
    printstatusf(
        "%s: %.2lfs",
        mode_get_name(),
        clock_get_relative()
    );
}

/**
 * @format: Formatstring like in fprintf
 *
 * Prints out message to status line.
 */
static void printstatusf(const char *format, ...)
{
    extern nyancat_t nc;
    va_list ap;

    werase(nc.ui.status);
    va_start(ap, format);
    vwprintw(nc.ui.status, format, ap);
    va_end(ap);
    wrefresh(nc.ui.status);
}
