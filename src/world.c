#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"
#include "random.h"

static struct world {
    int y;
    int x;
} world;

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
void
world_init(void)
{
    extern struct world world;
    world.x = 0;
    world.y = 0;

    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        elements[i] = world_create_random_platform(0, SCREENWIDTH);
    }
}

void
world_scroll_handler(gametime_t time, void *data)
{
    world.x++;

    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        int x = elements[i].x - world.x + elements[i].size;
        /* create new platform for i that is out of scope but in the first
         * half of the new imginary screen */
        if (x < 0) {
            elements[i] = world_create_random_platform(world.x + SCREENWIDTH, SCREENWIDTH / 2);
            continue;
        }
    }
    queue_add_event(time + TICK(1), world_scroll_handler, NULL);
    gamemode_draw();
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
    extern struct nyancat nc;

    werase(nc.ui.world);
    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        for (int k = 0; k < elements[i].size; ++k) {
            mvwaddch(nc.ui.world, world.y + elements[i].y, elements[i].x - world.x + k, '#');
        }
    }
    wnoutrefresh(nc.ui.world);
}

/**
 * Inidcates if under givem coordinates is an platform element. The given
 * coordinates are calculatet relative to the visible screen.
 */
int
world_has_element_at(enum object_type type, const int y, const int x)
{
    switch (type) {
        case ObjectPlatform:
            for (int i = 0; i < MAX_PLATFORMS; ++i) {
                if (elements[i].y != y) {
                    continue;
                }
                /* e.x <= x < e.x + e.size */
                if ((elements[i].x - world.x) <= x && x < (elements[i].x - world.x + elements[i].size)) {
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
    obj.y = random_range(4, SCREENHEIGHT - 2);
    /* make platforms size between [12..28]*/
    obj.size = random_range_step(16, 28, 4);

    return obj;
}
