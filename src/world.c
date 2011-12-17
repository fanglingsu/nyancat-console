#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"
#include "random.h"

static WINDOW *world;

/**
 * Generates nyans world one time into a pad window.
 */
void world_init(void)
{
    extern WINDOW *world;
    int column, rand_line, rand_element, mod;
    const char *elements[8] = {
        "###########",
        "#############",
        "###############",
        "#################",
        "#############  ####",
        "######   ############",
        "########   ############",
        "##########   #############",
    };

    world = newpad(SCREENHEIGHT, WORLDWIDTH);
    wclear(world);
    for (column = 0; column < WORLDWIDTH; column += 9) {
        /* number to select element */
        rand_element = random_limited(5);
        /* gnerate a number in [1..3] */
        mod = (rand_element % 3) + 1;
        /* random select the y position for element */
        rand_line = random_limited(SCREENHEIGHT * mod) / mod;
        mvwaddstr(world, rand_line, column, elements[rand_element]);
    }
}

/**
 * Prints the world.
 */
void world_print(void)
{
    extern WINDOW *world;
    static int i = 0;

    /* end of the world reached */
    if (i > WORLDWIDTH) {
        i = 0;
    }
    /* TODO add possibility to fill screen on end of world with the beginning
     * of the world - how can we make this beak unvisible for the gamer? */
    copywin(world, nc.ui.world, 0, i, 0, 0, SCREENHEIGHT, SCREENWIDTH - 1, 0);
    pnoutrefresh(world, 0, i, 0, 0, SCREENHEIGHT, SCREENWIDTH - 1);
    ++i;
}
