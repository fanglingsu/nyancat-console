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
    cat = newpad(4, 10);
    wclear(cat);
    waddstr(cat, " ,-----,  ");
    waddstr(cat, " |    ʌ-ʌ ");
    waddstr(cat, "~|___(∘.∘)");
    waddstr(cat, "  UU  UU  ");
    waddstr(cat, " U U U U  ");
}

/**
 * Print out the cat window.
 */
void print_cat(void)
{
    copywin(cat, nc.ui.world, 0, 0, 10, 15, 4, 10, 0);
}
