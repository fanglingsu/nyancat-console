#include "world.h"
#include "config.h"
#include "main.h"
#include "random.h"

static struct _world {
    WINDOW *win;
    int y;
    int x;
} world;

/**
 * Generates nyans world one time into a pad window.
 */
void
world_init(void)
{
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

    /* set the defaults in struct */
    world.win = newpad(SCREENHEIGHT, WORLDWIDTH);
    world.y = 0;
    world.x = 0;

    wclear(world.win);
    for (column = 0; column < WORLDWIDTH; column += 9) {
        /* number to select element */
        rand_element = random_limited(5);
        /* gnerate a number in [1..3] */
        mod = (rand_element % 3) + 1;
        /* random select the y position for element */
        rand_line = random_limited(SCREENHEIGHT * mod) / mod;
        mvwaddstr(world.win, rand_line, column, elements[rand_element]);
    }
}

void
world_scroll_handler(gametime_t time, void *data)
{
    world.x++;

    /* end of the world reached */
    if (world.x > WORLDWIDTH) {
        world.x = 0;
    }
    queue_add_event(time + TICK(1), world_scroll_handler, NULL);
    mode_draw();
}

void
world_start_scrolling(void)
{
    queue_add_event(clock_get_relative() + TICK(1), world_scroll_handler, NULL);
}

/**
 * Prints the world.
 */
void
world_print(void)
{
    /* TODO add possibility to fill screen on end of world with the beginning
     * of the world - how can we make this beak unvisible for the gamer? */
    copywin(world.win, nc.ui.world, 0, world.x, 0, 0, SCREENHEIGHT, SCREENWIDTH - 1, 0);
    pnoutrefresh(world.win, 0, world.x, 0, 0, SCREENHEIGHT, SCREENWIDTH - 1);
}
