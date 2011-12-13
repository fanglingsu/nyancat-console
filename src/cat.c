#include <ncurses.h>
#include "config.h"
#include "main.h"
#include "cat.h"

WINDOW *cat;

/**
 * Initialzes the cat window.
 */
void init_cat(void)
{
    cat = newpad(4, 11);
    wclear(cat);
    waddstr(cat, " ,-----,  \n");
    waddstr(cat, " |   /\\/\\ \n");
    waddstr(cat, "~|___(o.o)\n");
    waddstr(cat, "  UU  UU  \n");
}

/**
 * Print out the cat window.
 */
void print_cat(void)
{
    static int x = 14, y = 8;

    copywin(cat, nc.ui.world, 0, 0, y, x, y+5, x+9, 0);
    pnoutrefresh(cat, 0, 0, y, x, y+5, x+9);
}
