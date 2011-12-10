#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "main.h"
#include "config.h"

int main(int argc, const char *argv[])
{
    print_licence_message();
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
    nc.ui.mainwin = initscr();
    if (nc.ui.mainwin == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    /* don't show a carret */
    curs_set(0);
    keypad(nc.ui.mainwin, TRUE);
    intrflush(nc.ui.mainwin, FALSE);
    box(nc.ui.mainwin, 0, 0);

    /* print title */
    mvwaddstr(nc.ui.mainwin, 0, 3, " nyancat ");

    /* line buffering disabled, pass on everty thing to me */
    cbreak();
    noecho();

    /* create sub windows */
    nc.ui.world  = subwin(nc.ui.mainwin, LINES-4, COLS-2, 1, 1);
    nc.ui.status = subwin(nc.ui.mainwin, 1, COLS-2, LINES-2, 1);
}

/**
 * Show the startscreen with gaming instructions.
 */
void show_start_screen(void)
{
    werase(nc.ui.world);
    waddstr(nc.ui.world, "Press Enter to start nyancat\n\n");
    waddstr(nc.ui.world, "Use the cursor keys to move the cat.");
    wrefresh(nc.ui.world);
}

/**
 * Reads keyboard input and performs and do actions.
 */
void read_input(void)
{
    int ch;
    static enum mode lastmode;

    ch = getch();
    if ('q' == ch) {
        nc.current_mode = ModeOver;
    }

    switch (nc.current_mode) {
        case ModeGame:
            /* clear the main window */
            werase(nc.ui.world);
            wrefresh(nc.ui.world);
            switch(ch) {
                case KEY_LEFT:
                    print_statusline("KEY_LEFT pressed");
                    break;
                case KEY_RIGHT:
                    print_statusline("KEY_RIGHT pressed");
                    break;
                case KEY_UP:
                    print_statusline("KEY_UP pressed");
                    break;
                case KEY_DOWN:
                    print_statusline("KEY_DOWN pressed");
                    break;
            }
            break;
        case ModePause:
        case ModeScores:
            lastmode = nc.current_mode;
            break;
        case ModeOver:
            cleanup_windows();
            exit(0);
            break;
        case ModeIntro:
            lastmode = nc.current_mode;
            /* swtich to game mode on enter */
            if (10 == ch) {
                nc.current_mode = ModeGame;
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
    puts("Nyancat " VERSION);
    puts("Copyright 2011 Daniel Carl\n");
    puts("Nyancat comes with NO WARRANTY, to the extent permitted by law.");
    puts("You may redistribute copies of Nyancat under the terms of the");
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
    it.it_interval.tv_usec = TIMESTEP;
    it.it_value.tv_usec    = TIMESTEP;
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
            print_statusline("Signal ALRM recieved by timer");
            return;

        case SIGTERM:
        case SIGINT:
            cleanup_windows();
            puts("Nyancat-console was killed by deadly signal");
            exit(EXIT_SUCCESS);
    }
}

/**
 * Cleanup the environment.
 */
void cleanup_windows(void)
{
    delwin(nc.ui.status);
    delwin(nc.ui.world);
    delwin(nc.ui.mainwin);
    endwin();
    refresh();
}
