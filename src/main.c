#include <ncurses.h>
#include <stdlib.h>
#include "main.h"
#include "config.h"

int main(int argc, const char *argv[])
{
    int ch;

    print_licence_message();
    init_windows();
    show_start_screen();

    print_statusline("Press 'q' to exit");

    while((ch = getch()) != 'q') {
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
    }

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
    int ch;

    waddstr(nc.ui.world, "Press Enter to start nyancat\n\n");
    waddstr(nc.ui.world, "Use the cursor keys to move the cat.");
    wrefresh(nc.ui.world);
    while ((ch = getch())) {
        if (10 == ch) {
            break;
        }
    }
    werase(nc.ui.world);
    wrefresh(nc.ui.world);
}

/**
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
