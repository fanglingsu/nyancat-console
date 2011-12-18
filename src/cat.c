#include <ncurses.h>
#include "config.h"
#include "main.h"
#include "cat.h"

enum catmode {
    CatModeRun,
    CatModeJump
};

static struct Cat {
    int posY;
    int posX;
    enum catmode mode;
    /* count the frame nyan is in the current mode this is useful to auto
     * abort a certain mode and to change into another without any user
     * request */
    int mode_frames;
} cat;

static void cat_move_up(void);
static void cat_move_down(void);
static void cat_auto_movement(void);
static int  cat_switch_mode(enum catmode mode);


/**
 * Initializes the cat with their default properties.
 */
void cat_init(void)
{
    extern struct Cat cat;

    cat.posX    = 8;
    cat.posY    = 14;
    cat.mode    = CatModeRun;
    cat.mode_frames = 0;
}

/**
 * Move the cat up. In general all none static movement function calls don't
 * move nyan. Their concern is to decide if a requested mode could captured an
 * switch nyan to that mode.
 *
 * Movement will be done in cat_auto_movement() function instead.
 */
void cat_jump_up(void)
{
    extern struct Cat cat;

    /* don't allow to jump if already jumped */
    if (!cat_switch_mode(CatModeJump)) {
        cat_switch_mode(CatModeRun);
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
 * Print nyan to the world window after all movements where done.
 */
void cat_print(void)
{
    extern struct Nyancat nc;
    extern struct Cat cat;
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
    cat_auto_movement();

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
 * Move the cat up.
 */
static void cat_move_up(void)
{
    extern struct Cat cat;

    if (cat.posY > 0) {
        cat.posY--;
    }
}

/**
 * Move the cat down.
 */
static void cat_move_down(void)
{
    extern struct Cat cat;

    if (cat.posY + CATHIGHT < WORLDHEIGHT) {
        cat.posY++;
    }
}

/**
 * Handles movement of nyan. For example performe moves according to a
 * previous user requested jump.
 */
static void cat_auto_movement(void)
{
    extern struct Cat cat;
    /* number of frame until a certain movement is performed */
    const int up = 6, forward = 12, down = 18;

    if (CatModeJump == cat.mode) {
        /* try to jump in a natural way */
        if (cat.mode_frames < up) {
            if (1 == (cat.mode_frames % 2)) {
                cat_move_up();
            }
        } else if (cat.mode_frames >= up && cat.mode_frames < forward) {
        } else if (cat.mode_frames >= forward && cat.mode_frames < down) {
            if (1 == (cat.mode_frames % 2)) {
                cat_move_down();
            }
        } else {
            cat_switch_mode(CatModeRun);
        }
    } else if (CatModeRun == cat.mode) {
        /* TODO check if nyan has eath under the feets, else fall down until
         * the ground or a plateau is reached */

        /* dont fall to fast */
        if (0 == (cat.mode_frames % 2)) {
            cat_move_down();
        }
    }
    ++cat.mode_frames;
}

/**
 * @mode: mode to switch nyan into.
 *
 * Switches nyan in given mode and resets the mode frames set.
 *
 * @return  int 1 if mode could be switched
 */
static int cat_switch_mode(enum catmode mode)
{
    extern struct Cat cat;
    static int jump_count = 0;

    switch (mode) {
        case CatModeJump:
            /* only allow to jump twice */
            if (CatModeJump == cat.mode && 2 <= ++jump_count) {
                jump_count = 0;
                return 0;
            }
            break;

        default:
            break;
    }

    /* change mode if not previous returned */
    cat.mode = mode;
    cat.mode_frames = 0;

    return 1;
}
