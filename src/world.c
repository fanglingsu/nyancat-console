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

#include <string.h>

typedef struct {
    enum object_type type;
    int y;
    int x;
    int width;
} object_t;

static object_t platforms[MAX_PLATFORMS];
static enum object_type objects[SCREENWIDTH][WORLDHEIGHT];

/* used for vertical hysteresis in screen movement in world_move_screen_to() */
static int screen_hysteresis_diff;

static object_t world_create_random_platform(const int, const int);
static void world_print_object(const enum object_type, const int, const int, const int);
static void world_objects_place(const int);
static void world_objects_move_left(const int);

/**
 * Generates platform of nyans world.
 */
void world_init(void)
{
    extern object_t platforms[];
    extern nyancat_t nc;
    nc.ui.screen.x = 0;
    nc.ui.screen.y = WORLDHEIGHT / 2;

    /* value doesn't matter but value > SCREENHYSTERESIS + 1 will cause to
     * move screen the first time without hysteresis */
    screen_hysteresis_diff = SCREENHYSTERESIS + 2;

    /* place platforms */
    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        platforms[i] = world_create_random_platform(0, SCREENWIDTH);
    }
    world_objects_place(0);
}

/**
 * Moves the visible screen given step to the right.
 */
void world_move_screen_right(const int steps)
{
    extern nyancat_t nc;

    world_move_screen_to(nc.ui.screen.y, nc.ui.screen.x + steps);
    world_objects_move_left(steps);
}

/**
 * Moves the upper left corner of visible screen to given coordinates.
 */
void world_move_screen_to(const int y, const int x)
{
    extern object_t platforms[];
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
        int x = platforms[i].x - nc.ui.screen.x + platforms[i].width;
        /* create new platform for i that is out of scope but in the first
         * half of the new imginary screen */
        if (x < 0) {
            platforms[i] = world_create_random_platform(nc.ui.screen.x + SCREENWIDTH, SCREENWIDTH / 2);
            continue;
        }
    }
}

/**
 * Prints the world.
 */
void world_print(void)
{
    extern object_t platforms[];
    extern nyancat_t nc;

    werase(nc.ui.world);
    /* print objects */
    for (int x = 0; x < SCREENWIDTH; ++x) {
        for (int y = nc.ui.screen.y; y < nc.ui.screen.y + SCREENHEIGHT; ++y) {
            world_print_object(objects[x][y], y - nc.ui.screen.y, x, 1);
        }
    }
    /* print platforms */
    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        world_print_object(
            ObjectPlatform,
            platforms[i].y - nc.ui.screen.y,
            platforms[i].x - nc.ui.screen.x,
            platforms[i].width
        );
    }
    wnoutrefresh(nc.ui.world);
}

/**
 * Prints the given object type to also given coordinates. The coordinates
 * are mesured from the screen.
 */
static void world_print_object(const enum object_type type, const int y, const int x, const int width)
{
    switch (type) {
        case ObjectPlatform:
            wattron(nc.ui.world, A_BOLD);
            for (int l = 0; l < width; ++l) {
                mvwaddch(nc.ui.world, y, x + l, '^');
            }
            wattroff(nc.ui.world, A_BOLD);
            break;

        case ObjectMilk:
            wattron(nc.ui.world, COLOR_PAIR(ColorYellow) | A_BOLD);
            mvwaddch(nc.ui.world, y - 1, x, ':');
            mvwaddch(nc.ui.world, y,     x, 'M');
            wattroff(nc.ui.world, COLOR_PAIR(ColorYellow) | A_BOLD);
            break;

        case ObjectNone:
            break;
    }
}

/**
 * Inidcates if under givem coordinates is an platform element.
 * Coordinates refer to the world.
 */
int world_has_platform_at(const int y, const int x)
{
    extern object_t platforms[];

    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        if (platforms[i].y != y) {
            continue;
        }
        /* e.x <= x < e.x + e.width */
        if (platforms[i].x <= x
            && x < (platforms[i].x + platforms[i].width)) {
            return 1;
        }
    }

    return 0;
}

/**
 * Inidcates if given objects type is found at given position. The position
 * refers to the coordinates of th world.
 *
 * If removed is 1, the found object will be removed from the matrix.
 */
int world_has_object_at(const enum object_type type, const int y, const int x, const int remove)
{
    extern nyancat_t nc;

    /* objects horizontal position is the distance from left screen border */
    const int worldX = x - nc.ui.screen.x;
    if (type == objects[worldX][y]) {
        if (1 == remove) {
            objects[worldX][y] = ObjectNone;
        }
        return 1;
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

    obj.type = ObjectPlatform;
    obj.x = random_range_step(xstart, xstart + xrange, 4);
    /* padding top CATHEIGHT+1 and padding bottom 2 */
    obj.y = random_range_step(CATHEIGHT + 1, WORLDHEIGHT - 2, random_range(2, 3));
    /* make platforms width between [12..24] */
    obj.width = random_range_step(12, 24, 4);

    return obj;
}

/**
 * Places random objects into the matrix of objects. Objects are placed from
 * givem xstart offest from the screens left border to the right border of
 * screen. To fill the hole screen (and the unvisible parts aboth and under
 * the screen) use xstart = 0.
 */
static void world_objects_place(const int xstart)
{
    static unsigned int count = 0;

    for (int x = xstart; x < SCREENWIDTH; ++x) {
        /* initialize current row row with 0 */
        memset(objects[x], ObjectNone, sizeof(enum object_type) * WORLDHEIGHT);

        /* use modulo because x += 3 doesn't work if only the last column
         * should be generated after the matrix was moved */
        if (0 == (count % 4)) {
            objects[x][random_range_step(0, WORLDHEIGHT - 1, 3)] = ObjectMilk;
        }
        count++;
    }
}

/**
 * Move the whole matrix with the objects given steps to the left.
 */
void world_objects_move_left(const int steps)
{
    const size_t matrix_size = sizeof(enum object_type) * SCREENWIDTH * WORLDHEIGHT;

    /* move the matrix of objects to the left */
    memmove(
        objects,
        objects + steps,
        matrix_size - sizeof(objects[0]) * steps
    );
    /* place objects in the rightmost columns */
    world_objects_place(SCREENWIDTH - steps);
}
