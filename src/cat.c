#include <ncurses.h>
#include "config.h"
#include "main.h"
#include "cat.h"

static struct Cat {
    int posY;
    int posX;
    int speed;  /* steps per frame */
} cat = {8, 14, 1};

/**
 * Moves the cat to the position givem with y and x coordinates.
 */
static void cat_move(const int posY, const int posX)
{
    extern struct Cat cat;

    cat.posY = posY;
    cat.posX = posX;
}

/**
 * Move the cat up by the value of their speed.
 */
void cat_move_up(void)
{
    extern struct Cat cat;

    if (cat.posY >= 0 + cat.speed) {
        cat_move(cat.posY - cat.speed, cat.posX);
    } else {
        /* no enough room - move to upper-most position */
        cat_move(0, cat.posX);
    }
}

/**
 * Move the cat down by the value of their speed.
 */
void cat_move_down(void)
{
    extern struct Cat cat;

    if (cat.posY + CATHIGHT + cat.speed <= WORLDHEIGHT) {
        cat_move(cat.posY + cat.speed, cat.posX);
    } else {
        /* no enough room - move to lowest position */
        cat_move(WORLDHEIGHT - CATHIGHT, cat.posX);
    }
}

/**
 * Print nyan to the world window.
 */
void cat_print(void)
{
    extern struct Nyancat nc;
    extern struct Cat cat;
    static int frame = 0;
    int x = cat.posX, y = cat.posY;

    const struct {
        int x;
        char str[8];
    } feets[] = {
        {x + 1, "U U U U"},
        {x + 1, "U U UU"},
        {x + 1, "UU  UU"},
        {x + 1, "UU  U"},
        {x + 2, "U   U"},
        {x + 2, "U   UU"},
        {x + 2, "UU  UU"},
        {x + 2, "UU U U"}
    };

    wattron(nc.ui.world, COLOR_PAIR(ColorMagenta));
    mvwprintw(nc.ui.world, y,     x + 1, ",-----,");
    mvwprintw(nc.ui.world, y + 1, x + 1, "|:::/\\/\\");
    mvwprintw(nc.ui.world, y + 2, x,    "~|___(o.o)");

    if (frame < LENGTH(feets)) {
        mvwprintw(nc.ui.world, y + 3, feets[frame].x, feets[frame].str);
        if (frame == LENGTH(feets) - 1) {
            frame = 0;
        } else {
            ++frame;
        }
    }
    wnoutrefresh(nc.ui.world);
    wattroff(nc.ui.world, COLOR_PAIR(ColorMagenta));
}
