#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"

WINDOW *world_pad;

void init_world(void)
{
    int r, c;

    world_pad = newpad(nc.ui.world.rows, SPACE_MULTIPLIER * nc.ui.world.cols);
    wclear(world_pad);

    for (r = 0; r < nc.ui.world.rows; r += 3) {
        for (c = 0; c < nc.ui.world.cols * SPACE_MULTIPLIER; ++c) {
            if (c == 0) {
                mvwaddch(world_pad, r, c, 'A');
            } else if (c == nc.ui.world.cols) {
                mvwaddch(world_pad, r, c, '-');
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

    if (i > SPACE_MULTIPLIER * nc.ui.world.cols - 1) {
        i = 0;
    }
    copywin(world_pad, nc.ui.world.win, 0, i, 0, 0, nc.ui.world.rows, nc.ui.world.cols-1, 0);
    pnoutrefresh(world_pad, 0, i, 0, 0, nc.ui.world.rows, nc.ui.world.cols-1);
    ++i;
}
