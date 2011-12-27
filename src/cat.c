#include "main.h"
#include "cat.h"
#include "world.h"
#include "util.h"

enum catmode {
    CatModeRun,
    CatModeJump
};

static struct cat {
    int posY;
    int posX;
    enum catmode mode;
    int modeframes;     /* number of frames in a certain mode */
    int actioncount;    /* number of times a action was called this
                         * is used to not allow to jump twice without
                         * hitting the ground before */
    int hasground;      /* indicates if nyan has ground under her feets */
} cat;

static void cat_move_vertical(const int y);

/**
 * Initializes the cat with their default properties.
 */
void
cat_init(void)
{
    extern struct cat cat;

    cat.posX = CAT_XOFFSET;
    cat.posY = WORLDHEIGHT / 2 - CATHIGHT;
    cat.mode = CatModeRun;
    cat.modeframes = 0;
    cat.actioncount = 0;
    cat.hasground = 0;
}

/**
 * Move the cat up.
 */
void
cat_jump_up(gametime_t time)
{
    extern struct cat cat;
    static const int max_jumps = 2;

    /* remove previous run events */
    if (cat.mode == CatModeRun) {
        queue_remove_event(cat_run_handler);
    }
    /* remove previous jump events */
    queue_remove_event(cat_jump_handler);

    cat.actioncount++;
    cat.modeframes = 0;
    /* already jumped twice */
    if (max_jumps < cat.actioncount) {
        /* init run mode */
        cat.mode = CatModeRun;
        cat.modeframes = 0;
        cat.actioncount = 0;
        cat_run_handler(time, NULL);
        return;
    }
    cat_jump_handler(time, NULL);
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
 * Retreives the height of the cat in the world.
 */
int
cat_get_height(void)
{
    extern struct cat cat;

    return WORLDHEIGHT - cat.posY;
}

void
cat_move_right(const int steps)
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

void
cat_run_handler(gametime_t time, void *data)
{
    extern struct cat cat;

    if (cat.mode == CatModeRun) {
        if (cat.hasground) {
            cat.modeframes = 0;
            cat.actioncount = 0;
        } else {
            /* move down if neighert first feet or last feet is upon a platform */
            cat_move_vertical(1);
            cat.modeframes++;
        }
    }
    /* faster recall after more frames to simulate ground acceleration */
    if (cat.modeframes > 15) {
        queue_add_event(time + TICK(1), cat_run_handler, NULL);
    } else if (cat.modeframes > 5) {
        queue_add_event(time + TICK(2), cat_run_handler, NULL);
    } else {
        queue_add_event(time + TICK(3.5), cat_run_handler, NULL);
    }
}

/**
 * Eventcallback to perform a nice jump.
 */
void
cat_jump_handler(gametime_t time, void *data)
{
    extern struct cat cat;
    if (cat.modeframes > 2) {
        /* enter run mode */
        cat.mode = CatModeRun;
        queue_add_event(time + TICK(2), cat_run_handler, NULL);
    } else if (cat.modeframes >= 2) {
        queue_add_event(time + TICK(3.5), cat_jump_handler, NULL);
    } else {
        cat_move_vertical(-1);
        queue_add_event(time + TICK(3.5), cat_jump_handler, NULL);
    }
    cat.modeframes++;
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
    const int yoffset = cat.posY - nc.ui.screen.y;
    const int xoffset = cat.posX - nc.ui.screen.x;
    const char eye = cat.hasground ? 'o' : '+';
    const struct feets {
        int offset;
        char str[8];
    } feets[] = {
        {0, "U U U U"},
        {0, "UU  UU"},
        {1, "U   U"},
        {1, "UU  UU"},
    };

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
static void
cat_move_vertical(const int y)
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
