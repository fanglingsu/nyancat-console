#include "main.h"
#include "cat.h"
#include "world.h"
#include "util.h"

enum catmode {
    CatModeNormal
};

enum catstate {
    CatStateWalk,
    CatStateJumpInit,
    CatStateJumpUp,
    CatStateGlide,
    CatStateGlideDown,
    CatStateFall,
    CatStateFallFast
};

static struct cat {
    int posY;
    int posX;
    enum catmode mode;
    enum catstate state;
    int jumpcount;
    int hasground;      /* indicates if nyan has ground under her feets */
} cat;

typedef struct movement {
    gametime_t delta;
    int y;
    enum catstate state;
    struct movement *next;
} movement_t;

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
    {TICK(4.7),    0, CatStateGlide,   move_fall}
};

static void cat_move_vertical(const int y);

/**
 * Initializes the cat with their default properties.
 */
void cat_init(void)
{
    extern struct cat cat;

    cat.posX = CAT_XOFFSET;
    cat.posY = WORLDHEIGHT / 2 - CATHIGHT;
    cat.mode = CatModeNormal;
    cat.state = CatStateWalk;
    cat.jumpcount = 0;
    cat.hasground = 0;
}

/**
 * Move the cat up.
 */
void cat_jump_up(gametime_t time)
{
    extern struct cat cat;

    /* jumping from fast fall is not allowed */
    if (cat.jumpcount <= 1 && CatStateFallFast != cat.state) {
        cat.jumpcount++;
        cat.state = CatStateJumpInit;
    }
}

/**
 * Move the cat down.
 */
void cat_jump_down(void)
{
    /* Not implemented yet. Jump down will make sense if the fly mode will be
     * implemented that allows nyan to fly up and down nearly without any
     * constraints. */
}

/**
 * Retreives the height of the cat in the world.
 */
int cat_get_height(void)
{
    extern struct cat cat;

    return WORLDHEIGHT - cat.posY;
}

/**
 * Moves the cat to the right by given step size.
 */
void cat_move_right(const int steps)
{
    extern struct cat cat;

    cat.posX += steps;

    world_move_screen_to(cat.posY - SCREENHEIGHT / 2, cat.posX - CAT_XOFFSET);

    /* check if nyan has ground under her feets */
    if (world_has_element_at(ObjectPlatform, cat.posY + CATHIGHT, cat.posX + CATWIDTH - 2)
        || world_has_element_at(ObjectPlatform, cat.posY + CATHIGHT, cat.posX)
    ) {
        cat.hasground = 1;
    } else {
        cat.hasground = 0;
    }
}

/**
 * Handler function to move the cat according to their state.
 */
void cat_move_handler(gametime_t time, void *data)
{
    extern struct cat cat;
    movement_t *move = data;

    /* use movment data according to state if called first time */
    if (NULL == move) {
        if (CatStateJumpInit == cat.state) {
            move = move_jump;
        } else if (cat.hasground) {
            move = move_walk;
        } else {
            move = move_fall;
        }
    }
    if (CatStateJumpInit == cat.state) {
        move = move_jump;
    } else if (cat.hasground) {
        move = move_walk;
        cat.jumpcount = 0;
    }

    cat.state = move->state;
    cat_move_vertical(move->y);
    queue_add_event(time + move->delta, cat_move_handler, move->next);
}

/**
 * Print nyan to the world window after all movements where done.
 */
void cat_print(void)
{
    extern nyancat_t nc;
    extern struct cat cat;
    static int frame = 0;
    const int yoffset = cat.posY - nc.ui.screen.y;
    const int xoffset = cat.posX - nc.ui.screen.x;
    char eye;
    const struct feets {
        int offset;
        char str[8];
    } feets[] = {
        {0, "U U U U"},
        {0, "UU  UU"},
        {1, "U   U"},
        {1, "UU  UU"},
    };

    if (cat.hasground) {
        eye = 'o';
    } else if (CatStateFallFast == cat.state) {
        eye = '+';
    } else {
        eye = '0';
    }

    wattron(nc.ui.world, COLOR_PAIR(ColorMagenta));
    mvwprintw(nc.ui.world, yoffset,     xoffset,      ",-----,");
    mvwprintw(nc.ui.world, yoffset + 1, xoffset,      "|%3d/\\/\\", cat_get_height() * 100 / WORLDHEIGHT);
    mvwprintw(nc.ui.world, yoffset + 2, xoffset - 1, "~|___(%c.%c)", eye, eye);

    if (frame < LENGTH(feets)) {
        mvwprintw(nc.ui.world, yoffset + 3, xoffset + feets[frame].offset, feets[frame].str);
        if (frame == LENGTH(feets) - 1) {
            frame = 0;
        } else {
            ++frame;
        }
    }
    wnoutrefresh(nc.ui.world);
    wattroff(nc.ui.world, COLOR_PAIR(ColorMagenta));
}

/**
 * Moves the cat vertical by given offset. If given y offset is negative, the
 * cat is moved upwards.
 */
static void cat_move_vertical(const int y)
{
    extern struct cat cat;

    cat.posY += y;

    if (cat.posY < 0) {
        cat.posY = 0;
    } else if (cat.posY >= WORLDHEIGHT) {
        cat.posY = WORLDHEIGHT;
        /* cat is out of view - game over */
        gamemode_enter(mode_scores);
    }
}
