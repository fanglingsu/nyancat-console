/**
 * nyancat-console - a clone of the nyancat lost in space flash game.
 *
 * Copyright (C) 2011-2012 Daniel Carl
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
#include <string.h>
#include "world.h"
#include "config.h"
#include "cat.h"
#include "main.h"
#include "random.h"

typedef struct {
    enum object_type type;
    int y;
    int x;
    int width;
} object_t;

/* holds the coordinates of the upper left corner of visible screen in the world */
coordinate_t screen;

static object_t platforms[MAX_PLATFORMS];
static enum object_type objects[SCREENWIDTH][WORLDHEIGHT];

/* used for vertical hysteresis in screen movement in world_move_screen_to() */
static int screen_hysteresis_diff;

static object_t world_create_random_platform(const int, const int);
static void world_print_object(const enum object_type, const int, const int, const int);
static void world_objects_place(const int);
static void world_objects_move_left(void);

/**
 * Generates platform of nyans world.
 */
void world_init(void)
{
    extern object_t platforms[];

    screen.x = 0;
    screen.y = WORLDHEIGHT / 2;

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
 * Moves the visible screen one step to the right.
 */
void world_move_screen_right(void)
{
    world_move_screen_to(screen.y, screen.x + 1);
    world_objects_move_left();
}

/**
 * Moves the upper left corner of visible screen to given coordinates.
 */
void world_move_screen_to(const int y, const int x)
{
    extern object_t platforms[];

    /* vertical hysteresis */
    if (screen_hysteresis_diff > SCREENHYSTERESIS) {
        if (screen_hysteresis_diff <= SCREENHYSTERESIS + 1) {
            screen.y += 1;
        } else {
            screen.y = y;
        }
    } else if (screen_hysteresis_diff < SCREENHYSTERESIS * -1) {
        screen.y -= 1;
    }
    screen_hysteresis_diff = y - screen.y;

    screen.x = x;
    if (screen.y < 0) {
        screen.y = 0;
    } else if (screen.y > WORLDHEIGHT - SCREENHEIGHT) {
        screen.y = WORLDHEIGHT - SCREENHEIGHT;
    }
    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        int x = platforms[i].x - screen.x + platforms[i].width;
        /* create new platform for i that is out of scope but in the first
         * half of the new imginary screen */
        if (x < 0) {
            platforms[i] = world_create_random_platform(screen.x + SCREENWIDTH, SCREENWIDTH / 2);
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
    wattron(nc.ui.world, A_BOLD);
    /* print objects */
    for (int x = 0; x < SCREENWIDTH; ++x) {
        for (int y = screen.y; y < screen.y + SCREENHEIGHT; ++y) {
            world_print_object(objects[x][y], y - screen.y, x, 1);
        }
    }
    /* print platforms */
    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        world_print_object(
            ObjectPlatform,
            platforms[i].y - screen.y,
            platforms[i].x - screen.x,
            platforms[i].width
        );
    }
    wattroff(nc.ui.world, A_BOLD);
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
            for (int l = 0; l < width; ++l) {
                mvwaddch(nc.ui.world, y, x + l, '^');
            }
            break;

        case ObjectMilk:
            wattron(nc.ui.world, COLOR_PAIR(ColorYellow));
            mvwaddch(nc.ui.world, y,     x, ':');
            mvwaddch(nc.ui.world, y + 1, x, 'M');
            wattroff(nc.ui.world, COLOR_PAIR(ColorYellow));
            break;

        case ObjectCandy:
            wattron(nc.ui.world, COLOR_PAIR(ColorRed));
            mvwaddch(nc.ui.world, y,     x, 'I');
            mvwaddch(nc.ui.world, y + 1, x, 'I');
            wattroff(nc.ui.world, COLOR_PAIR(ColorRed));
            break;

        case ObjectRandom:
            wattron(nc.ui.world, COLOR_PAIR(ColorBlue) | A_REVERSE);
            mvwaddch(nc.ui.world, y,     x, '?');
            mvwaddch(nc.ui.world, y + 1, x, 'R');
            wattroff(nc.ui.world, COLOR_PAIR(ColorBlue) | A_REVERSE);
            break;

        case ObjectDiamond:
            wattron(nc.ui.world, A_REVERSE);
            mvwaddch(nc.ui.world, y,     x, 'A');
            mvwaddch(nc.ui.world, y + 1, x, 'V');
            wattroff(nc.ui.world, A_REVERSE);
            break;

        case ObjectRubin:
            wattron(nc.ui.world, COLOR_PAIR(ColorRed) | A_REVERSE);
            mvwaddch(nc.ui.world, y,     x, 'M');
            mvwaddch(nc.ui.world, y + 1, x, 'W');
            wattroff(nc.ui.world, COLOR_PAIR(ColorRed) | A_REVERSE);
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
 * Retrieves the object at given position in the world.
 *
 * If @remove is true, the found object will be removed from the matrix.
 */
enum object_type world_get_object_at(const int y, const int x, const int remove)
{
    extern nyancat_t nc;
    enum object_type found;
    static const int obj_height = 2;

    /* objects horizontal position is the distance from left screen border */
    const int worldX = x - screen.x;

    /* objects are only a point in matrix but hase a height in real so look
     * for the coordinates in matrix and if not found have a look at the point
     * under it */
    for (int i = 0; i < obj_height; ++i) {
        found = objects[worldX][y - i];
        if (ObjectNone != found) {
            /* have found the object */
            if (remove) {
                objects[worldX][y - i] = ObjectNone;
            }
            return found;
        }
    }
    return ObjectNone;
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
    /* padding top CATHEIGHT+2 and padding bottom 2 */
    obj.y = random_range_step(CATHEIGHT + 2, WORLDHEIGHT - 2, random_range(2, 3));
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

    for (short x = xstart; x < SCREENWIDTH; ++x) {
        /* initialize current row row with 0 */
        memset(objects[x], ObjectNone, sizeof(enum object_type) * WORLDHEIGHT);

        /* TODO this needs rethinking entirely, because it's not configurable
         * and it's hard to adapt the object ratio according to changed
         * WORLDHEIGHT or SCREENWIDTH values. */

        /* use modulo because x += 3 doesn't work if only the last column
         * should be generated after the matrix was moved */
        if (0 == (count % 3)) {
            /* place more object for larger worlds */
            for (short i = 0; i < WORLDHEIGHT / 40; ++i) {
                objects[x][random_range_step(0, WORLDHEIGHT - 2, 3)] = (count % 4) ? ObjectCandy : ObjectMilk;
            }
        }
        count++;
    }

    /* place rare objects */
    if (0 == random_range(0, 35)) {
        objects[SCREENWIDTH - 1][random_range(0, WORLDHEIGHT - 2)] = random_range(ObjectRandom, ObjectRubin);
    }
}

/**
 * Move the whole matrix with the objects one step to the left.
 */
void world_objects_move_left(void)
{
    const size_t matrix_size = sizeof(enum object_type) * SCREENWIDTH * WORLDHEIGHT;

    /* move the matrix of objects to the left */
    memmove(
        objects,
        objects + 1,
        matrix_size - sizeof(objects[0])
    );
    /* place objects in the rightmost columns */
    world_objects_place(SCREENWIDTH - 1);
}
