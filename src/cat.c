#include <ncurses.h>
#include "config.h"
#include "main.h"
#include "cat.h"
#include "queue.h"
#include "clock.h"

enum catmode {
    CatModeRun,
    CatModeJump
};

static struct cat {
    int posY;
    int posX;
    enum catmode mode;
} cat;

struct action {
    enum catmode mode;
    int posY;
    int posX;
    gametime_t delta;
};

static struct action jump_action[] = {
    {CatModeJump, -1, 0, TICK(2.5)},
    {CatModeJump, -1, 0, TICK(5)},
    {CatModeJump,  1, 0, TICK(5)},
    {CatModeJump,  1, 0, TICK(2.5)},
    {CatModeRun,   0, 0, 0}
};

static struct action *current_action = NULL;

static void cat_move_by(const int y);

/**
 * Initializes the cat with their default properties.
 */
void
cat_init(void)
{
    extern struct cat cat;

    cat.posX = 8;
    cat.posY = 14;
    cat.mode = CatModeRun;
}

/**
 * Move the cat up. In general all none static movement function calls don't
 * move nyan. Their concern is to decide if a requested mode could captured an
 * switch nyan to that mode.
 *
 * Movement will be done in cat_auto_movement() function instead.
 */
void
cat_jump_up(gametime_t time)
{
    /* do not allow to jump multiple times */
    if (cat.mode == CatModeJump) {
        cat.mode = CatModeRun;
    }
    queue_remove_event(cat_jump_handler);
    cat_jump_handler(time, jump_action);
}

/**
 * Move the cat down.
 */
void
cat_jump_down(void)
{
    /* Not implemented yet. Jump down will make sense if the fly mode will be
     * implemented that allows nyan to fly up and down nearly without any
     * constraints. */
}

/**
 * Eventcallback to perform a nice jump.
 */
void
cat_jump_handler(gametime_t time, void *data)
{
    current_action = data;

    if (current_action->mode != CatModeJump) {
        cat.mode = CatModeRun;
        return;
    }
    cat_move_by(current_action->posY);

    queue_add_event(time + current_action->delta, cat_jump_handler,
                    current_action + 1);
}

/**
 * Print nyan to the world window after all movements where done.
 */
void
cat_print(void)
{
    extern struct nyancat nc;
    extern struct cat cat;
    static int frame = 0;
    const struct feets {
        int offset;
        char str[8];
    } feets[] = {
        {1, "U U U U"},
        {1, "UU  UU"},
        {2, "U   U"},
        {2, "UU  UU"},
    };

    /* handle movements */

    wattron(nc.ui.world, COLOR_PAIR(ColorMagenta));
    mvwprintw(nc.ui.world, cat.posY,     cat.posX + 1, ",-----,");
    mvwprintw(nc.ui.world, cat.posY + 1, cat.posX + 1, "|:::/\\/\\");
    mvwprintw(nc.ui.world, cat.posY + 2, cat.posX,    "~|___(o.o)");

    if (frame < LENGTH(feets)) {
        mvwprintw(nc.ui.world, cat.posY + 3, cat.posX + feets[frame].offset, feets[frame].str);
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
 * Moves the cat by applying given vertical offset. Applying negative values
 * moves nyan to the top.
 */
static void
cat_move_by(const int y)
{
    extern struct cat cat;

    if (y > 0) {
        /* move down */
        if (cat.posY + CATHIGHT + y <= WORLDHEIGHT) {
            cat.posY += y;
        } else {
            /* go to lowest position */
            cat.posY = WORLDHEIGHT - CATHIGHT;
        }
    } else if (y < 0) {
        /* move up */
        if (cat.posY + y >= 0) {
            cat.posY += y;
        } else {
            /* go to upper most position */
            cat.posY = 0;
        }
    }
}
