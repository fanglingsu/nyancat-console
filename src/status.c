#include "status.h"
#include "main.h"

enum item {
    StatusRuntime,
    StatusFrames,
    StatusDebug
};

static struct Status {
    enum gamemode mode;
    game_time runtime;
    int frames;
    char debug[90];
} status = {ModeIntro, 0.0, 0, ""};

static void printstatusf(const char *format, ...);

/**
 * @mode: current mode of game
 *
 * Set the game mode to the status.
 */
void status_set_mode(const enum gamemode mode)
{
    extern struct Status status;

    status.mode = mode;
}

/**
 * @runtime: duration of running game in seconds.
 *
 * Set the runtime in seconds to the status.
 */
void status_set_runtime(const game_time runtime)
{
    extern struct Status status;

    status.runtime = runtime;
}

/**
 * @frames: number of frames used since starting the game.
 *
 * Set the used frame numbers.
 */
void status_set_frames(const int frames)
{
    extern struct Status status;

    status.frames = frames;
}

/**
 * Print out contens of the status to the status window.
 */
void status_print(void)
{
    extern struct Status status;

    printstatusf(
        "%s: % 3d - %.2lfs %s",
        status.mode == ModePause ? "Paused" : "Game",
        status.frames,
        status.runtime,
        status.debug
    );
}

/**
 * @format: Formatstring like in fprintf
 *
 * Prints out message to status line.
 */
static void printstatusf(const char *format, ...)
{
    extern struct Nyancat nc;
    va_list ap;

    werase(nc.ui.status);
    va_start(ap, format);
    vwprintw(nc.ui.status, format, ap);
    va_end(ap);
    wrefresh(nc.ui.status);
}
