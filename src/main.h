#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdio.h>
#include "config.h"
#include "gamemode.h"
#include "queue.h"

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

#define err_msg(...) \
{                   \
    werase(nc.ui.debug); \
    wprintw(nc.ui.debug, __VA_ARGS__); \
    wnoutrefresh(nc.ui.debug); \
    doupdate(); \
}

#ifdef DEBUG
#   define DEBUG_FORMAT "%10s:%20s() %03d: "
#   define DEBUG_ARGS __FILE__, __func__, __LINE__
#   define debug(format, ...) err_msg(DEBUG_FORMAT format, DEBUG_ARGS, __VA_ARGS__)
#else
#   define debug(...)
#endif

enum color {
    ColorWhite,
    ColorMagenta,
    ColorRed
};

typedef struct {
    int x;
    int y;
} coordinate_t;

typedef struct {
    struct {
        WINDOW *world;
        WINDOW *status;
#if DEBUG
        WINDOW *debug;
#endif
        coordinate_t screen;
    } ui;
} nyancat_t;

nyancat_t nc;

/* game modes */
gamemode_t *mode_intro;
gamemode_t *mode_game;
gamemode_t *mode_pause;
gamemode_t *mode_scores;

void error_exit(const char *format, ...);

#endif /* end of include guard: MAIN_H */
