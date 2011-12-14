#include <ncurses.h>
#include "config.h"
#include "main.h"
#include "cat.h"

/**
 * Print nyan to the world window.
 */
void print_cat(void)
{
    extern struct Nyancat nc;
    static int x = 14, y = 8, frame = 0;
    struct {
        int x;
        char str[8];
    } feets[] = {
        {x + 1, "U U U U"},
        {x + 1, "U U UU"},
        {x + 1, "UU  UU"},
        {x + 1, "UU  U"},
        {x + 2, "U   U"},
        {x + 2, "U   UU"},
        {x + 2, "UU  UU"},
        {x + 2, "UU U U"}
    };

    wattron(nc.ui.world, COLOR_PAIR(ColorMagenta));
    mvwprintw(nc.ui.world, y,     x + 1, ",-----,");
    mvwprintw(nc.ui.world, y + 1, x + 1, "|:::/\\/\\");
    mvwprintw(nc.ui.world, y + 2, x,    "~|___(o.o)");

    if (frame < LENGTH(feets)) {
        mvwprintw(nc.ui.world, y + 3, feets[frame].x, feets[frame].str);
        if (frame == LENGTH(feets) - 1) {
            frame = 0;
        } else {
            ++frame;
        }
    }
    wnoutrefresh(nc.ui.world);
    wattroff(nc.ui.world, COLOR_PAIR(ColorMagenta));
}
