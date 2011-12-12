#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"

#include <unistd.h>

WINDOW *test;

void init_world(void)
{
    int r, c;

    test = newpad(nc.ui.space.rows, SPACE_MULTIPLIER * nc.ui.space.cols);
    wclear(test);

    for (r = 0; r < nc.ui.space.rows; r += 3) {
        for (c = 0; c < nc.ui.space.cols * SPACE_MULTIPLIER; ++c) {
            if (c == 0) {
                mvwaddch(test, r, c, 'A');
            } else if (c == nc.ui.space.cols) {
                mvwaddch(test, r, c, '-');
            } else if ((rand() % 4) == 0) {
                mvwaddch(test, r, c, '#');
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

    if (i > SPACE_MULTIPLIER * nc.ui.space.cols - 1) {
        i = 0;
    }
    copywin(test, nc.ui.world, 0, i, 0, 0, nc.ui.space.rows, nc.ui.space.cols-1, 0);
    pnoutrefresh(test, 0, i, 0, 0, nc.ui.space.rows, nc.ui.space.cols-1);
    ++i;
}
