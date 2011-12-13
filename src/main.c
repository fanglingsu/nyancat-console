#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "main.h"
#include "random.h"
#include "world.h"
#include "config.h"
#include "error.h"

int main(int argc, const char *argv[])
{
    init_random();
    set_timer();
    set_signals();
    init_windows();

    show_start_screen();
    print_statusline("Press 'q' to exit");

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
void init_windows(void)
{
    initscr();

    cbreak();       /* line buffering disabled, pass on everty thing to me */
    noecho();
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
void show_start_screen(void)
{
    werase(nc.ui.world);
    waddstr(nc.ui.world, "Press Enter to start " REAL_NAME "\n\n");
    waddstr(nc.ui.world, "Use the cursor keys to move the cat.");
    refresh_world();
}

/**
 * refresh screen so that modified graphic buffers get visible
 */
void refresh_world(void)
{
    /* needed to display graphics properly at startup on some terminals */
    redrawwin(nc.ui.world);
    wrefresh(nc.ui.world);
}

/**
 * Reads keyboard input and decide when to switch to another mode or to change
 * position variables.
 */
void read_input(void)
{
    int ch = getch();
    if ('q' == ch) {
        nc.current_mode = ModeOver;
    }

    switch (nc.current_mode) {
        case ModeGame:
            if ('p' == ch) {
                nc.current_mode = ModePause;
            }
            break;
        case ModePause:
            if ('p' == ch) {
                nc.current_mode = ModeGame;
            }
            break;
        case ModeScores:
            break;
        case ModeOver:
            break;
        case ModeIntro:
            if (10 == ch) {
                nc.current_mode = ModeGame;
                init_world();
            }
            break;
    }
}

/**
 * @str: Message to print out to the status line
 *
 * Prints out message to status line.
 */
void print_statusline(char* str)
{
    werase(nc.ui.status);
    waddstr(nc.ui.status, str);
    wrefresh(nc.ui.status);
}

/**
 * Prints the licence message to stdout.
 */
void print_licence_message(void)
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
void set_timer(void)
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
void set_signals(void)
{
    struct sigaction sa;

    /* fill in sigaction struct */
    sa.sa_handler = signal_handler;
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
void signal_handler(int sig)
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
 * Handlerfunction to perform gaming actions. This function is calles FPS
 * times a second.
 */
void game_handler(void)
{
    switch (nc.current_mode) {
        case ModeGame:
            print_statusline("Game");
            print_world();
            break;
        case ModePause:
            print_statusline("Pause");
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
}

/**
 * Cleanup the environment.
 */
void cleanup_windows(void)
{
    delwin(nc.ui.status);
    delwin(nc.ui.status);
    delwin(nc.ui.world);
    endwin();
    refresh();
}
