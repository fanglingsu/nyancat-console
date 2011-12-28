/**
 * nyancat-console - a clone of the nyancat lost in space flash game.
 *
 * Copyright (C) 2011 Daniel Carl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */
#include <stdlib.h>
#include "world.h"
#include "config.h"
#include "main.h"
#include "random.h"

typedef struct {
    enum object_type type;
    int y;
    int x;
    int size;
} object_t;

static object_t elements[MAX_PLATFORMS];

/* used for vertical hysteresis in screen movement in world_move_screen_to() */
static int screen_hysteresis_diff;

static object_t world_create_random_platform(const int, const int);

/**
 * Generates platform of nyans world.
 */
void world_init(void)
{
    extern object_t elements[];
    extern nyancat_t nc;
    nc.ui.screen.x = 0;
    nc.ui.screen.y = WORLDHEIGHT / 2;

    /* value doesn't matter but value > SCREENHYSTERESIS + 1 will cause to
     * move screen the first time without hysteresis */
    screen_hysteresis_diff = SCREENHYSTERESIS + 2;

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
    extern object_t elements[];
    extern nyancat_t nc;

    /* vertical hysteresis */
    if (screen_hysteresis_diff > SCREENHYSTERESIS) {
        if (screen_hysteresis_diff <= SCREENHYSTERESIS + 1) {
            nc.ui.screen.y += 1;
        } else {
            nc.ui.screen.y = y;
        }
    } else if (screen_hysteresis_diff < SCREENHYSTERESIS * -1) {
        nc.ui.screen.y -= 1;
    }
    screen_hysteresis_diff = y - nc.ui.screen.y;

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
    extern object_t elements[];
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
    extern object_t elements[];

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
static object_t world_create_random_platform(const int xstart, const int xrange)
{
    object_t obj;

    obj.x = random_range_step(xstart, xstart + xrange, 4);
    /* padding top CATHEIGHT+1 and padding bottom 2 */
    obj.y = random_range_step(CATHEIGHT + 1, WORLDHEIGHT - 2, random_range(2, 3));
    /* make platforms size between [12..24] */
    obj.size = random_range_step(12, 24, 4);

    return obj;
}
