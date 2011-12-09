#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>
#include <langinfo.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "config.h"

struct Nyancat {
   struct {
        WINDOW *mainwin;
        WINDOW *world;
        WINDOW *status;
   } ui;
} nc;

int main(int argc, const char *argv[])
{
    int ch; /* holds pressed input char */

    puts("Nyancat-Console " VERSION);
    puts("Copyright 2011 Daniel Carl");
    puts("Nyancat-Console comes with NO WARRANTY, to the extent permitted by law.");
    puts("You may redistribute copies of Nyancat-Console under the terms of the GNU");
    puts("General Public License. For more information about these matters, see");
    puts("the file named LICENCE.");

    initscr();
    init_windows();

    show_start_screen();
    print_statusline("Press 'q' to exit");

    while((ch = getch()) != 'q') {
        switch(ch) {
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
            case KEY_UP:
                break;
            case KEY_DOWN:
                break;
        }
    }

    cleanup_windows();
    return EXIT_SUCCESS;
}

/**
 * Initialize the used windows
 */
void init_windows()
{
    if ((nc.ui.mainwin = initscr()) == NULL) {
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
void show_start_screen()
{
    int ch;
    waddstr(nc.ui.world, "Press Enter to start nyancat");
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
