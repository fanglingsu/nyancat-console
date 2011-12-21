#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "main.h"
#include "random.h"
#include "world.h"
#include "config.h"
#include "cat.h"
#include "status.h"
#include "clock.h"
#include "mode.h"
#include "modes.h"
#include "io.h"

static void init_windows(void);
static void init_modes(void);
static void print_licence_message(void);
static void set_signals(void);
static void signal_handler(int sig);
static void loop(void);
static void cleanup_windows(void);

int main(int argc, const char *argv[])
{
    clock_init();
    random_init();
    set_signals();
    init_windows();
    clear_windows();
    init_modes();

    loop();

    cleanup_windows();
    return EXIT_SUCCESS;
}

/**
 * @format: Formatstring like in fprintf.
 *
 * Signal a fatal error and quit immediately.
 */
void error_exit(const char *format, ...)
{
    va_list ap;

    /* clean all ncurses windows */
    cleanup_windows();

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fputc('\n', stderr);
    va_end(ap);

    exit(EXIT_FAILURE);
}

/**
 * Initialize the used windows
 */
static void init_windows(void)
{
    extern struct Nyancat nc;

    initscr();
    if (has_colors() != FALSE) {
        start_color();
    }

    if (can_change_color()) {
        init_color(COLOR_MAGENTA, 1000, 600, 1000);
    }
    /* define used color pairs */
    init_pair(ColorMagenta, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(ColorRed, COLOR_RED, COLOR_BLACK);

    cbreak();       /* line buffering disabled, pass on everty thing to me */
    noecho();       /* don't print typed chars */
    curs_set(0);    /* don't show a carret */
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);

    /* create sub windows */
    nc.ui.world  = newwin(SCREENHEIGHT, SCREENWIDTH, 0, 0);
    nc.ui.status = newwin(1, SCREENWIDTH, SCREENHEIGHT, 0);
}

void clear_windows(void)
{
    wclear(nc.ui.world);
    wnoutrefresh(nc.ui.world);
    wclear(nc.ui.status);
    wnoutrefresh(nc.ui.status);
}

/**
 * Initializes the game modes.
 */
static void init_modes(void)
{
    extern Mode *mode_intro, *mode_game, *mode_pause, *mode_scores;

    mode_intro = mode_create("Intro", NULL, NULL, intro_draw, intro_key_handler);
    mode_scores = mode_create("Scores", scores_draw, NULL, NULL, scores_key_handler);
    mode_game = mode_create("Game", game_enter, NULL, game_draw, game_key_handler);
    mode_pause = mode_create("Pause", pause_enter, pause_leave, NULL, pause_key_handler);
}

/**
 * Prints the licence message to stdout.
 */
static void print_licence_message(void)
{
    puts(REAL_NAME " " VERSION);
    puts("Copyright 2011 Daniel Carl\n");
    puts(REAL_NAME " comes with NO WARRANTY, to the extent permitted by law.");
    puts("You may redistribute copies of " REAL_NAME "  under the terms of the");
    puts("GNU General Public License. For more information about these");
    puts("matters, see the file named LICENCE.");
}

/**
 * Sets up signal handlers we need.
 */
static void set_signals(void)
{
    struct sigaction sa;

    /* fill in sigaction struct */
    sa.sa_handler = &signal_handler;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);

    /* set signal handlers */
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT,  &sa, NULL);

    /* ignore sigtstp */
    sa.sa_handler = SIG_IGN;
    sigaction(SIGTSTP, &sa, NULL);
}

/**
 * @sig: number of the signal
 *
 * Signal handler called if signal is emmitted.
 */
static void signal_handler(int sig)
{
    cleanup_windows();
    print_licence_message();
    exit(EXIT_FAILURE);
}

/**
 * Main game loop.
 */
static void loop(void)
{
    extern struct Nyancat nc;
    game_time time;
    int key_ready = 0, ch;

    /* switch to intro mode */
    mode_enter(mode_intro);
    mode_draw();

    while (mode_valid()) {
        /* collect data for status bar but draw callback decide if to write to
         * window or not */
        status_set_mode(mode_get_name());
        status_set_runtime(clock_get_relative());

        /* look for the first to run event */
        time = queue_get_first_time();
        if (time) {
            /* wait for time to run the first event form queue or the first
             * keypress, wichever comes first. */
            key_ready = io_wait_for_key(time);
        } else {
            /* no event queue item found */
            io_wait_for_key(0);
            key_ready = 1;
        }
        if (key_ready > 0) {
            ch = getch();
            mode_key(ch);
            mode_draw();
        } else {
            queue_run_until(time);
        }
        doupdate();
    }
}

/**
 * Cleanup the environment.
 */
static void cleanup_windows(void)
{
    extern struct Nyancat nc;
    extern Mode *mode_intro, *mode_game, *mode_pause, *mode_scores, *mode_over;
    free(mode_intro);
    free(mode_game);
    free(mode_pause);
    free(mode_scores);
    free(mode_over);
    queue_free();

    wrefresh(nc.ui.status);
    delwin(nc.ui.status);

    wrefresh(nc.ui.world);
    delwin(nc.ui.world);

    endwin();

    /* flush open streams */
    fflush(NULL);
}
