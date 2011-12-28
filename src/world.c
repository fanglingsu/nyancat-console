#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"
#include "random.h"

struct object {
    enum object_type type;
    int y;
    int x;
    int size;
};

static struct object elements[MAX_PLATFORMS];

static struct object world_create_random_platform(const int xstart, const int xrange);

/**
 * Generates platform of nyans world.
 */
void world_init(void)
{
    extern nyancat_t nc;
    nc.ui.screen.x = 0;
    nc.ui.screen.y = WORLDHEIGHT / 2;

    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        elements[i] = world_create_random_platform(0, SCREENWIDTH);
    }
}

/**
 * Moves the visible screen given step to the right.
 */
void world_move_screen_right(const int steps)
{
    extern nyancat_t nc;

    world_move_screen_to(nc.ui.screen.y, nc.ui.screen.x + steps);
}

/**
 * Moves the upper left corner of visible screen to given coordinates.
 */
void world_move_screen_to(const int y, const int x)
{
    extern nyancat_t nc;

    nc.ui.screen.y = y;
    nc.ui.screen.x = x;
    if (nc.ui.screen.y < 0) {
        nc.ui.screen.y = 0;
    } else if (nc.ui.screen.y > WORLDHEIGHT - SCREENHEIGHT) {
        nc.ui.screen.y = WORLDHEIGHT - SCREENHEIGHT;
    }
    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        int x = elements[i].x - nc.ui.screen.x + elements[i].size;
        /* create new platform for i that is out of scope but in the first
         * half of the new imginary screen */
        if (x < 0) {
            elements[i] = world_create_random_platform(nc.ui.screen.x + SCREENWIDTH, SCREENWIDTH / 2);
            continue;
        }
    }
}

/**
 * Prints the world.
 */
void world_print(void)
{
    extern nyancat_t nc;

    werase(nc.ui.world);
    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        for (int k = 0; k < elements[i].size; ++k) {
            mvwaddch(nc.ui.world, elements[i].y - nc.ui.screen.y, elements[i].x - nc.ui.screen.x + k, '#');
        }
    }
    wnoutrefresh(nc.ui.world);
}

/**
 * Inidcates if under givem coordinates is an platform element.
 */
int world_has_element_at(enum object_type type, const int y, const int x)
{
    switch (type) {
        case ObjectPlatform:
            for (int i = 0; i < MAX_PLATFORMS; ++i) {
                if (elements[i].y != y) {
                    continue;
                }
                /* e.x <= x < e.x + e.size */
                if (elements[i].x <= x
                    && x < (elements[i].x + elements[i].size)) {
                    return 1;
                }
            }
            break;
    }

    return 0;
}

/**
 * Builds a random places platform struct. Platforms are placed in the x range
 * between xstart and the next range chars.
 */
static struct object
world_create_random_platform(const int xstart, const int xrange)
{
    struct object obj;

    obj.x = random_range_step(xstart, xstart + xrange, 4);
    /* padding top 4 and padding bottom 2 */
    obj.y = random_range(4, WORLDHEIGHT - 2);
    /* make platforms size between [12..24] */
    obj.size = random_range_step(12, 24, 4);

    return obj;
}
