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
#include "main.h"
#include "cat.h"
#include "world.h"
#include "util.h"
#include "game.h"

enum catmode {
    CatModeNormal,
    CatModeReverse,
    CatModeFly,
};

enum catstate {
    CatStateWalk,

    CatStateJumpInit,
    CatStateJumpUp,

    CatStateGlide,
    CatStateGlideDown,

    CatStateFall,
    CatStateFallFast,

    CatStateFlyUp,
    CatStateFlyStraight,
    CatStateFlyDown,
};

typedef struct {
    int y;
    int x;
    enum catmode mode;
    enum catstate state;
    int jumpcount;
    int hasground;      /* indicates if nyan has ground under her feets */
} cat_t;

typedef struct movement {
    gametime_t delta;
    int y;
    enum catstate state;
    struct movement *next;
} movement_t;

typedef struct {
    int offset;
    char str[8];
} feets_t;

extern coordinate_t screen;

static cat_t cat;

static movement_t move_fall[] = {
    {TICK(3.5),  1, CatStateGlideDown, &move_fall[1]},
    {TICK(2.5),  1, CatStateGlideDown, &move_fall[2]},
    {TICK(2.5),  1, CatStateFall,      &move_fall[3]},
    {TICK(2.5),  1, CatStateFall,      &move_fall[4]},
    {TICK(2.5),  1, CatStateFall,      &move_fall[5]},
    {TICK(2.1),  1, CatStateFall,      &move_fall[6]},
    {TICK(2.1),  1, CatStateFall,      &move_fall[7]},
    {TICK(2.1),  1, CatStateFall,      &move_fall[8]},
    {TICK(1),    1, CatStateFallFast,  &move_fall[8]}
};
static movement_t move_walk[] = {
    {TICK(1),    0, CatStateWalk, &move_walk[1]},
    {TICK(1),    0, CatStateGlide, move_fall}
};
static movement_t move_jump[] = {
    {TICK(2.5), -1, CatStateJumpUp, &move_jump[1]},
    {TICK(3.5), -1, CatStateJumpUp, &move_jump[2]},
    {TICK(4.7),  0, CatStateGlide,   move_fall}
};
/* TODO is this required - or can we do this in cat_move_handler() */
static movement_t move_fly[] = {
    {TICK(3), -1, CatStateFlyUp,        NULL},
    {TICK(2),  0, CatStateFlyStraight,  NULL},
    {TICK(3),  1, CatStateFlyDown,      NULL}
};

/* holds a pointer to curret movement used - this is for example to resume the
 * game after pause */
static movement_t *current = NULL;

static const struct cat_image {
    char *body[3];
    feets_t feets[4];
} cat_images[] = {
    /* normal mode */
    {
        {",-----,", "|%3d/\\/\\", "~|___(%c.%c)"},
        {
            {0, "U U U U"},
            {0, "UU  UU"},
            {1, "U   U"},
            {1, "UU  UU"}
        }
    },
    /* fast fall */
    {
        {"O----O,", "|%3d|\\/\\", "~\\___(%c.%c)"},
        {
            {1, "U   U"},
            {1, "U   U"},
            {1, "U   U"},
            {1, "U   U"}
        }
    }
};

/* hash sign mark sensitive zones
 *  ,-----#
 *  |   /\/#
 * ~|___(o###
 *  UU  UU#
 *
 * holds the offsets that should be applied to cats current position
 */
static const coordinate_t zones[] = {
    {CATHEIGHT - 1, CATWIDTH - 4},
    {CATHEIGHT - 2, CATWIDTH - 2},
    {CATHEIGHT - 2, CATWIDTH - 3},
    {CATHEIGHT - 2, CATWIDTH - 4},
    {CATHEIGHT - 3, CATWIDTH - 3},
    {CATHEIGHT - 4, CATWIDTH - 4},
};

static const char *mode_names[] = {
    "Normal",   /* CatModeNormal */
    "Reverse",  /* CatModeReverse */
    "Fly"       /* CatModeFly */
};

static void cat_print_modename(void);
static void cat_multiplicator_reset_handler(gametime_t, void *);
static void cat_enter_normalmode_handler(gametime_t, void *);
static void cat_collect_objects(void);
static void cat_move_vertical(const int);

/**
 * Initializes the cat with their default properties.
 */
void cat_init(void)
{
    extern cat_t cat;

    cat.x = CAT_XOFFSET;
    cat.y = WORLDHEIGHT / 2 - CATHEIGHT;
    cat.mode = CatModeNormal;
    cat.state = CatStateWalk;
    cat.jumpcount = 0;
    cat.hasground = 0;

    /* set start movement mode */
    current = move_walk;
}

/**
 * Move the cat up.
 */
void cat_jump_up(gametime_t time)
{
    extern cat_t cat;

    switch (cat.mode) {
        case CatModeNormal: /* fall through */
        case CatModeReverse:
            /* jumping from fast fall is not allowed */
            if (cat.jumpcount <= 1 && CatStateFallFast != cat.state) {
                cat.jumpcount++;
                cat.state = CatStateJumpInit;
            }
            break;

        case CatModeFly:
            if (CatStateFlyDown == cat.state) {
                cat.state = CatStateFlyStraight;
            } else {
                cat.state = CatStateFlyUp;
            }
            break;
    }
}

/**
 * Move the cat down.
 */
void cat_jump_down(gametime_t time)
{
    switch (cat.mode) {
        case CatModeFly:
            if (CatStateFlyUp == cat.state) {
                cat.state = CatStateFlyStraight;
            } else {
                cat.state = CatStateFlyDown;
            }
            break;

        case CatModeNormal: /* fall through */
        case CatModeReverse:
            break;
    }
}

/**
 * Retrieves the height of the cat in the world.
 */
int cat_get_height(void)
{
    extern cat_t cat;

    return WORLDHEIGHT - cat.y;
}

/**
 * Moves the cat to the right by given step size.
 */
void cat_move_right(const int steps)
{
    extern cat_t cat;
    int ypos_feets;
    cat.x += steps;

    world_move_screen_to(cat.y - SCREENHEIGHT / 2 + CATHEIGHT / 2, cat.x - CAT_XOFFSET);

    ypos_feets = cat.y + CATHEIGHT;

    /* hang on platform instead of standing if reversemode is on */
    if (CatModeReverse == cat.mode) {
        ypos_feets -= 1;
    }

    /* check if nyan has ground under her feets */
    if (world_has_platform_at(ypos_feets, cat.x + CATWIDTH - 2)
        || world_has_platform_at(ypos_feets, cat.x)
    ) {
        cat.hasground = 1;
    } else {
        cat.hasground = 0;
    }

    /* collection objects */
    cat_collect_objects();

    /* add extra scores for crazy fall down */
    if (FEATURE_SCORE_STUNT && CatStateFallFast == cat.state) {
        game_increment_score(1);
    }
}

/**
 * Handler function to move the cat according to their state.
 */
void cat_move_handler(gametime_t time, void *data)
{
    extern cat_t cat;
    movement_t *move = data;

    /* if none movement is given, use current that is also the last before
     * pause */
    if (NULL == move) {
        move = current;
    }
    switch (cat.mode) {
        case CatModeNormal: /* fall through */
        case CatModeReverse:
            if (CatStateJumpInit == cat.state) {
                move = move_jump;
            } else if (cat.hasground) {
                move = move_walk;
                cat.jumpcount = 0;
            }
            if (CatStateFlyStraight == cat.state) {
                cat.state = CatStateWalk;
            }
            break;

        case CatModeFly:
            if (CatStateFlyUp == cat.state) {
                move = move_fly;
            } else if (CatStateFlyDown == cat.state) {
                move = &move_fly[2];
            } else {
                /* if none state set begin with straight flight */
                move = &move_fly[1];
            }
            break;
    }

    cat.state = move->state;
    /* TODO should we use extra movement data for reverse mode? */
    if (CatModeReverse == cat.mode) {
        cat_move_vertical(-1 *move->y);
    } else {
        cat_move_vertical(move->y);
    }
    queue_add_event(time + move->delta, cat_move_handler, move->next);

    /* save the next movement already as current */
    if (NULL != move->next) {
        current = move->next;
    }
}

/**
 * Print nyan to the world window after all movements where done.
 */
void cat_print(void)
{
    extern nyancat_t nc;
    extern cat_t cat;
    static int frame = 0;
    const int yoffset = cat.y - screen.y;
    const int xoffset = cat.x - screen.x;
    char eye;
    struct cat_image img = (CatStateFallFast == cat.state) ? cat_images[1] : cat_images[0];

    if (cat.hasground) {
        eye = 'o';
    } else if (CatStateFallFast == cat.state) {
        eye = '+';
    } else {
        eye = '0';
    }

    wattron(nc.ui.world, COLOR_PAIR(ColorMagenta));
    mvwprintw(nc.ui.world, yoffset,     xoffset,     img.body[0]);
    mvwprintw(nc.ui.world, yoffset + 1, xoffset,     img.body[1], cat_get_height() * 100 / WORLDHEIGHT);
    mvwprintw(nc.ui.world, yoffset + 2, xoffset - 1, img.body[2], eye, eye);

    if (frame < LENGTH(img.feets)) {
        mvwprintw(nc.ui.world, yoffset + 3, xoffset + img.feets[frame].offset, img.feets[frame].str);
        if (frame == LENGTH(img.feets) - 1) {
            frame = 0;
        } else {
            ++frame;
        }
    }

    cat_print_modename();

    wnoutrefresh(nc.ui.world);
    wattroff(nc.ui.world, COLOR_PAIR(ColorMagenta));
}

/**
 * Print the name of current mode to the upper left or world.
 */
static void cat_print_modename(void)
{
    extern cat_t cat;

    /* find a better way to handle modes with their names */
    mvwprintw(nc.ui.world, 0, 0, "[%s]", mode_names[cat.mode]);
}

/**
 * Event callback handler that resets the score multiplicator.
 */
static void cat_multiplicator_reset_handler(gametime_t time, void *data)
{
    game_unset_multiplicator();
}

/**
 * Event callback handler that switches cat back to nromal mode.
 */
static void cat_enter_normalmode_handler(gametime_t time, void *data)
{
    extern cat_t cat;

    cat.mode = CatModeNormal;
}

/**
 * Check if nyan is over an object in the world and removes it and collect
 * points for it.
 */
static void cat_collect_objects(void)
{
    enum object_type object;

    for (int i = 0; i < LENGTH(zones); ++i) {
        object = world_get_object_at(cat.y + zones[i].y, cat.x + zones[i].x, 1);
        /* in current object placing there should always only one object that
         * could be collected - so return after first object picked up */
        switch (object) {
            case ObjectMilk:
                /* remove old multiplicator reset events */
                queue_remove_event(cat_multiplicator_reset_handler);

                /* add new event */
                queue_add_event(
                    clock_get_relative() + MULTIPLIER_TIMEOUT,
                    cat_multiplicator_reset_handler,
                    NULL
                );
                game_increment_multiplicator(1);
                return;

            case ObjectCandy:
                game_increment_score(1);
                return;

            case ObjectRandom:
                /* remove old events */
                queue_remove_event(cat_enter_normalmode_handler);

                /* add new event */
                queue_add_event(
                    clock_get_relative() + SPECIALMODE_TIMEOUT,
                    cat_enter_normalmode_handler,
                    NULL
                );
                cat.mode = ((int)clock_get_relative() % 2) ? CatModeFly : CatModeReverse;
                return;

            case ObjectNone:    /* fall through */
            case ObjectPlatform:
                break;
        }
    }
}

/**
 * Moves the cat vertical by given offset. If given y offset is negative, the
 * cat is moved upwards.
 */
static void cat_move_vertical(const int y)
{
    extern cat_t cat;

    cat.y += y;

    /* allow the cat to jump out of the upper screen until only her feets are
     * visible */
    if (cat.y < 1 - CATHEIGHT) {
        cat.y = 1 - CATHEIGHT;
        if (CatModeReverse == cat.mode) {
            gamemode_enter(mode_scores);
        }
    } else if (cat.y >= WORLDHEIGHT) {
        cat.y = WORLDHEIGHT;
        /* cat is out of view - game over */
        gamemode_enter(mode_scores);
    }
}
