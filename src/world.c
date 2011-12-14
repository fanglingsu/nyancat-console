#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"

static WINDOW *world;

void init_world(void)
{
    extern WINDOW *world;
    int r, c;

    world = newpad(SCREENHEIGHT, SPACE_MULTIPLIER * SCREENWIDTH);
    wclear(world);

    for (r = 0; r < SCREENHEIGHT; ++r) {
        for (c = 0; c < (SCREENWIDTH * SPACE_MULTIPLIER); ++c) {
            if (c == 0) {
                mvwaddch(world, r, c, '<');
            } else if (c == SCREENWIDTH) {
                mvwaddch(world, r, c, '|');
            } else if (c == (SCREENWIDTH*2 - 1)) {
                mvwaddch(world, r, c, '>');
            } else if ((rand() % 20) == 0) {
                mvwaddch(world, r, c, '#');
            }
        }
    }
}

/**
 * Prints the world.
 */
void print_world(void)
{
    extern WINDOW *world;
    static int i = 0;

    if (i > SPACE_MULTIPLIER * SCREENWIDTH - 1) {
        i = 0;
    }
    copywin(world, nc.ui.world, 0, i, 0, 0, SCREENHEIGHT, SCREENWIDTH-1, 0);
    pnoutrefresh(world, 0, i, 0, 0, SCREENHEIGHT, SCREENWIDTH-1);
    ++i;
}
