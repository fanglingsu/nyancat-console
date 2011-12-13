#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"

WINDOW *world_pad;

void init_world(void)
{
    int r, c;

    world_pad = newpad(SCREENHEIGHT, SPACE_MULTIPLIER * SCREENWIDTH);
    wclear(world_pad);

    for (r = 0; r < SCREENHEIGHT; r += 3) {
        for (c = 0; c < (SCREENWIDTH * SPACE_MULTIPLIER); ++c) {
            if (c == 0) {
                mvwaddch(world_pad, r, c, '<');
            } else if (c == SCREENWIDTH) {
                mvwaddch(world_pad, r, c, '|');
            } else if (c == (SCREENWIDTH*2 - 1)) {
                mvwaddch(world_pad, r, c, '>');
            } else if ((rand() % 4) == 0) {
                mvwaddch(world_pad, r, c, '#');
            }
        }
    }
}

/**
 * Prints the world.
 */
void print_world(void)
{
    static int i = 0;

    if (i > SPACE_MULTIPLIER * SCREENWIDTH - 1) {
        i = 0;
    }
    copywin(world_pad, nc.ui.world.win, 0, i, 0, 0, SCREENHEIGHT, SCREENWIDTH-1, 0);
    pnoutrefresh(world_pad, 0, i, 0, 0, SCREENHEIGHT, SCREENWIDTH-1);
    ++i;
}
