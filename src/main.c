#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "main.h"
#include "random.h"
#include "world.h"
#include "config.h"
#include "cat.h"

static void init_windows(void);
static void show_start_screen(void);
static void show_scores(void);
static void print_statusline(const char *format, ...);
static void print_licence_message(void);
static void set_timer(void);
static void set_signals(void);
static void signal_handler(int sig);
static void read_input(void);
static void game_handler(void);
static void cleanup_windows(void);

int main(int argc, const char *argv[])
{
    extern struct Nyancat nc;

    random_init();
    set_timer();
    set_signals();
    init_windows();

    /* set initial mode */
    nc.current_mode = ModeIntro;

    while (true) {
        read_input();
    };

    cleanup_windows();
    return EXIT_SUCCESS;
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

/**
 * Show the startscreen with gaming instructions.
 */
static void show_start_screen(void)
{
    extern struct Nyancat nc;

    werase(nc.ui.world);
    waddstr(nc.ui.world, "Press Enter to start " REAL_NAME "\n\n");
    waddstr(nc.ui.world, "Use j, k or the cursor keys to move the cat.");
    wnoutrefresh(nc.ui.world);
}

/**
 * Shows the summary scores screen on end of the game.
 */
static void show_scores(void)
{
    extern struct Nyancat nc;

    werase(nc.ui.world);
    wattron(nc.ui.world, COLOR_PAIR(ColorRed));
    mvwprintw(nc.ui.world, 5, SCREENWIDTH/2 - 5, "GAME OVER!");
    wattroff(nc.ui.world, COLOR_PAIR(ColorRed));
    mvwprintw(nc.ui.world, 7, SCREENWIDTH/2 - 8, "Press q to quit.");
    wnoutrefresh(nc.ui.world);

    /* remove content from status window */
    werase(nc.ui.status);
    wnoutrefresh(nc.ui.status);
}

/**
 * @format: Formatstring like in fprintf
 *
 * Prints out message to status line.
 */
static void print_statusline(const char *format, ...)
{
    extern struct Nyancat nc;
    va_list ap;

    werase(nc.ui.status);
    va_start(ap, format);
    vwprintw(nc.ui.status, format, ap);
    va_end(ap);
    wrefresh(nc.ui.status);
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
 * Sets up the game timer
 */
static void set_timer(void)
{
    struct itimerval it;

    /* clear itimerval struct members */
    timerclear(&it.it_interval);
    timerclear(&it.it_value);

    /* set timer */
    it.it_interval.tv_usec = 1000000/FPS;
    it.it_value.tv_usec    = 1000000/FPS;
    setitimer(ITIMER_REAL, &it, NULL);
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
    sigaction(SIGALRM, &sa, NULL);

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
    switch (sig) {
        case SIGALRM:
            /* received from the timer */
            game_handler();
            return;

        case SIGTERM:
        case SIGINT:
            cleanup_windows();
            print_licence_message();
            exit(EXIT_FAILURE);
    }
}

/**
 * Reads keyboard input and decide when to switch to another mode or to change
 * position variables.
 */
static void read_input(void)
{
    extern struct Nyancat nc;
    int ch = getch();

    switch (nc.current_mode) {
        case ModeGame:
            if ('p' == ch) {
                nc.current_mode = ModePause;
            } else if ('k' == ch || KEY_UP == ch) {
                cat_move_up();
            } else if ('j' == ch || KEY_DOWN == ch) {
                cat_move_down();
            } else if ('q' == ch) {
                nc.current_mode =  ModeScores;
            }
            break;
        case ModePause:
            if ('p' == ch) {
                nc.current_mode = ModeGame;
            }
            break;
        case ModeScores:
            if ('q' == ch) {
                nc.current_mode = ModeOver;
            }
            show_scores();
            break;
        case ModeOver:
            break;
        case ModeIntro:
            if (10 == ch) {
                nc.current_mode = ModeGame;
                world_init();
            }
            break;
    }
}

/**
 * Handlerfunction to perform gaming actions. This function is calles FPS
 * times a second.
 */
static void game_handler(void)
{
    extern struct Nyancat nc;
    static unsigned int i = 0;

    switch (nc.current_mode) {
        case ModeGame:
            ++i;
            print_statusline("Game: % 3d - %d s", i, (i / FPS));
            world_print();
            cat_print();
            break;
        case ModePause:
            print_statusline("Paused: % 3d - %d s", i, (i / FPS));
            break;
        case ModeScores:
            break;
        case ModeOver:
            cleanup_windows();
            print_licence_message();
            exit(EXIT_SUCCESS);
            break;
        case ModeIntro:
            show_start_screen();
            break;
    }
    /* print changes from virtual screen to terminal */
    doupdate();
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
 * Cleanup the environment.
 */
static void cleanup_windows(void)
{
    extern struct Nyancat nc;

    wrefresh(nc.ui.status);
    delwin(nc.ui.status);

    wrefresh(nc.ui.world);
    delwin(nc.ui.world);

    endwin();

    /* flush open streams */
    fflush(NULL);
}
