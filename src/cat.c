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
#include "main.h"
#include "cat.h"
#include "world.h"
#include "util.h"
#include "game.h"
#include "random.h"

enum catmode {
    CatModeNormal,
    CatModeReverse,
    CatModeFly,
    CatModeBubble,
    CatModeGhost,
    CatModeCrack,
};

typedef struct {
    enum catmode mode;
    char *name;
} catmode_t;

/* initialize cat modes with names */
static const catmode_t catmodes[] = {
    {CatModeNormal,  "Normal"},
    {CatModeReverse, "Reverse"},
    {CatModeFly,     "Fly"},
    {CatModeBubble,  "Bubble"},
    {CatModeGhost,   "Ghost"},
    {CatModeCrack,   "Crack"}
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
    catmode_t mode;
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

/* used to resque events, so that the event queue could be regenerated after
 * the queue was free'd by gamemode switching. */
typedef struct {
    gametime_t time;
    eventhandler_fn callback;
} event_backup;

static event_backup eventbackup_specialmode;
static event_backup eventbackup_gem;
static event_backup eventbackup_milk;

extern coordinate_t screen;

static cat_t cat;

static movement_t move_fall[] = {
    {TICK(3.5), 1, CatStateGlideDown, &move_fall[1]},
    {TICK(2.5), 1, CatStateGlideDown, &move_fall[2]},
    {TICK(2.5), 1, CatStateFall,      &move_fall[3]},
    {TICK(2.5), 1, CatStateFall,      &move_fall[4]},
    {TICK(2.5), 1, CatStateFall,      &move_fall[5]},
    {TICK(2.1), 1, CatStateFall,      &move_fall[6]},
    {TICK(2.1), 1, CatStateFall,      &move_fall[7]},
    {TICK(2.1), 1, CatStateFall,      &move_fall[8]},
    {TICK(1),   1, CatStateFallFast,  &move_fall[8]}
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
static movement_t move_bubblefall[] = {
    {TICK(2.5), 1, CatStateGlideDown, &move_bubblefall[1]},
    {TICK(3.5), 1, CatStateGlideDown, &move_bubblefall[2]},
    {TICK(2.5), 1, CatStateFall,      &move_bubblefall[3]},
    {TICK(2.5), 1, CatStateFall,      &move_bubblefall[4]},
    {TICK(2.5), 1, CatStateFall,      &move_bubblefall[5]},
    {TICK(2.1), 1, CatStateFall,      &move_bubblefall[6]},
    {TICK(2.1), 1, CatStateFall,      &move_bubblefall[7]},
    {TICK(2.1), 1, CatStateFall,      &move_bubblefall[7]}
};
static movement_t move_bubblejump[] = {
    {TICK(2.5), -1, CatStateJumpUp,    &move_bubblejump[1]},
    {TICK(3.5), -1, CatStateJumpUp,    &move_bubblejump[2]},
    {TICK(4.7),  0, CatStateGlide,     move_bubblefall},
};
static movement_t move_bubblewalk[] = {
    {TICK(1), 0, CatStateWalk, &move_bubblewalk[1]},
    {TICK(1), 0, CatStateGlide, move_bubblefall}
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
 * +--012345678
 * 0  #--#--#
 * 1  |   /\/#
 * 2 ~|___(o.##
 * 3  #U# ###
 *
 * holds the offsets that should be applied to cats current position
 */
static const coordinate_t zones[] = {
    {2, 8},
    {3, 6},
    {2, 7},
    {1, 7},
    {0, 6},
    {3, 4},
    {3, 5},
    {0, 0},
    {3, 0},
    {0, 3},
    {3, 2}
};

static void cat_print_modename(void);
static void cat_multiplier_reset_handler(gametime_t, void *);
static void cat_longlifemilk_reset_handler(gametime_t, void *);
static void cat_enter_normalmode_handler(gametime_t, void *);
static void cat_collect_objects(void);
static void cat_apply_gem(enum object_type);
static void cat_apply_specialmode(enum object_type);
static void cat_register_event(gametime_t, eventhandler_fn);
static void cat_move_vertical(const int);

/**
 * Initializes the cat with their default properties.
 */
void cat_init(void)
{
    extern cat_t cat;

    cat.x = CAT_XOFFSET;
    cat.y = WORLDHEIGHT / 2 - CATHEIGHT;
    cat.mode = catmodes[CatModeNormal];
    cat.state = CatStateWalk;
    cat.jumpcount = 0;
    cat.hasground = 0;

    /* set start movement mode */
    current = move_walk;

    eventbackup_specialmode = (event_backup){0, NULL};
    eventbackup_gem = (event_backup){0, NULL};
    eventbackup_milk = (event_backup){0, NULL};
}

/**
 * Registers events that are required to move the cat and look for previous
 * saved events to be restored.
 *
 * This function should be called if the gamemode is entered - started or
 * after puase.
 */
void cat_start(void)
{
    gametime_t time = clock_get_relative();

    queue_add_event(time, cat_move_handler, NULL);

    /* restore events from backups */
    if (eventbackup_gem.time >= time) {
        cat_register_event(
            eventbackup_gem.time,
            eventbackup_gem.callback
        );
    }
    if (eventbackup_specialmode.time >= time) {
        cat_register_event(
            eventbackup_specialmode.time,
            eventbackup_specialmode.callback
        );
    }
    if (eventbackup_milk.time >= time) {
        cat_register_event(
            eventbackup_milk.time,
            eventbackup_milk.callback
        );
    }
}

/**
 * Move the cat up.
 */
void cat_jump_up(gametime_t time)
{
    extern cat_t cat;

    switch (cat.mode.mode) {
        case CatModeNormal: /* fall through */
        case CatModeReverse:
        case CatModeGhost:
        case CatModeCrack:
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

        case CatModeBubble:
            cat.state = CatStateJumpInit;
            break;
    }
}

/**
 * Move the cat down.
 */
void cat_jump_down(gametime_t time)
{
    switch (cat.mode.mode) {
        case CatModeFly:
            if (CatStateFlyUp == cat.state) {
                cat.state = CatStateFlyStraight;
            } else {
                cat.state = CatStateFlyDown;
            }
            break;

        case CatModeNormal: /* fall through */
        case CatModeReverse:
        case CatModeBubble:
        case CatModeGhost:
        case CatModeCrack:
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
 * Moves the cat one step to the right.
 */
void cat_move_right(void)
{
    extern cat_t cat;
    int ypos_feets;
    int world_x, world_y;
    cat.x += 1;

    world_y = cat.y - SCREENHEIGHT/2 + CATHEIGHT/2;
    world_x = cat.x - CAT_XOFFSET;

    /* shake vertically move -1 - +1 on x */
    if (CatModeCrack == cat.mode.mode) {
        world_x += random_range(-1 , 1);
    }
    world_move_screen_to(world_y, world_x);

    ypos_feets = cat.y + CATHEIGHT;

    /* hang on platform instead of standing if reversemode is on */
    if (CatModeReverse == cat.mode.mode) {
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
    switch (cat.mode.mode) {
        case CatModeNormal: /* fall through */
        case CatModeReverse:
        case CatModeGhost:
        case CatModeCrack:
            if (CatStateJumpInit == cat.state) {
                move = move_jump;
            } else if (cat.hasground) {
                move = move_walk;
                cat.jumpcount = 0;
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

        case CatModeBubble:
            if (CatStateJumpInit == cat.state) {
                move = move_bubblejump;
            } else if (cat.hasground) {
                move = move_bubblewalk;
                cat.jumpcount = 0;
            }
            break;
    }

    cat.state = move->state;
    /* TODO should we use extra movement data for reverse mode? */
    if (CatModeReverse == cat.mode.mode) {
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

    wattron(nc.ui.world, COLOR_PAIR(ColorMagenta));

    /* don't show the cat in ghost mode most of the time */
    if (CatModeGhost != cat.mode.mode
        || 0 == random_range(0, GHOST_MODE_FRAMES)
    ) {
        if (CatModeCrack == cat.mode.mode) {
            eye = '@';
        } else if (cat.hasground) {
            eye = 'o';
        } else if (CatStateFallFast == cat.state) {
            eye = '+';
        } else {
            eye = '0';
        }

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
    mvwprintw(nc.ui.world, 0, 0, "[%s]", cat.mode.name);
}

/**
 * Event callback handler that resets the score multiplier.
 */
static void cat_multiplier_reset_handler(gametime_t time, void *data)
{
    game_unset_multiplier();
}

/**
 * Event callback handler that resets the long life milk.
 */
static void cat_longlifemilk_reset_handler(gametime_t time, void *data)
{
    game_remove_multiplier_unset_protect();
}

/**
 * Event callback that reset the extra score multiplier.
 */
static void cat_extramultiplier_reset_handler(gametime_t time, void *data)
{
    game_unset_extra_multiplier();
}

/**
 * Event callback handler that switches cat back to nromal mode.
 */
static void cat_enter_normalmode_handler(gametime_t time, void *data)
{
    extern cat_t cat;

    cat.mode = catmodes[CatModeNormal];
    game_set_tickbase(1);
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
                cat_register_event(
                    clock_get_relative() + MULTIPLIER_TIMEOUT,
                    cat_multiplier_reset_handler
                );
                /* backup events data */
                eventbackup_milk.time = clock_get_relative() + MULTIPLIER_TIMEOUT;
                eventbackup_milk.callback = cat_multiplier_reset_handler;

                game_increment_multiplier(1);
                return;

            case ObjectCandy:
                game_increment_score(1);
                return;

            case ObjectRandom:
                cat_apply_specialmode(object);
                break;

            case ObjectDiamond: /* fall though */
            case ObjectRubin:
                cat_apply_gem(object);
                return;

            case ObjectNone:    /* fall through */
            case ObjectPlatform:
                break;
        }
    }
}

/**
 * Applies changes according to given collected objecttype of a gem.
 */
static void cat_apply_gem(enum object_type object)
{
    gametime_t time = clock_get_relative() + GEMSTONE_TIMEOUT;

    eventbackup_gem.time = time;

    if (ObjectDiamond == object) {
        cat_register_event(time, cat_longlifemilk_reset_handler);
        /* gems aren't combineable */
        game_unset_extra_multiplier();
        game_set_multiplier_unset_protect();

        /* backup event callback */
        eventbackup_gem.callback = cat_longlifemilk_reset_handler;
    } else if (ObjectRubin == object) {
        cat_register_event(time, cat_extramultiplier_reset_handler);
        /* gems aren't combineable */
        game_remove_multiplier_unset_protect();
        game_set_extra_multiplier(2);

        /* backup event callback */
        eventbackup_gem.callback = cat_extramultiplier_reset_handler;
    }
}

/**
 * Applies changes according to given collected objecttype for special modes
 * like the bubble or fly mode.
 */
static void cat_apply_specialmode(enum object_type object)
{
    extern cat_t cat;
    gametime_t time = clock_get_relative() + GEMSTONE_TIMEOUT;

    cat_register_event(time, cat_enter_normalmode_handler);

    /* backup event callback */
    eventbackup_specialmode.callback = cat_enter_normalmode_handler;
    eventbackup_specialmode.time = time;

    /* unset the tickbase */
    game_set_tickbase(1);
    switch (random_range(0, 4)) {
        case 0: cat.mode = catmodes[CatModeFly]; break;
        case 1: cat.mode = catmodes[CatModeReverse]; break;
        case 2: cat.mode = catmodes[CatModeBubble]; break;
        case 3: cat.mode = catmodes[CatModeGhost]; break;
        case 4:
            cat.mode = catmodes[CatModeCrack];
            /* set new tick base */
            game_set_tickbase(0.75);
            break;
    }
}

/**
 * Register a cat event in event queue.
 */
static void cat_register_event(gametime_t time, eventhandler_fn callback)
{
    /* remove old event */
    queue_remove_event(callback);

    /* add new event */
    queue_add_event(time, callback, NULL);
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
        if (CatModeReverse == cat.mode.mode) {
            gamemode_enter(mode_scores);
        }
    } else if (cat.y >= WORLDHEIGHT) {
        cat.y = WORLDHEIGHT;
        /* cat is out of view - game over */
        gamemode_enter(mode_scores);
    }
}
