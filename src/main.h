#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdio.h>
#include "gamemode.h"
#include "queue.h"

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

#define err_msg(...) \
{                   \
    werase(stdscr); \
    mvprintw(SCREENHEIGHT + 1, 0, "%s:%5d %s()", __FILE__, __LINE__, __FUNCTION__); \
    mvprintw(SCREENHEIGHT + 2, 4, __VA_ARGS__); \
    wnoutrefresh(stdscr); \
    doupdate(); \
}

#if DEBUG
#   define debug(...) err_msg(__VA_ARGS__)
#else
#   define debug(...)
#endif

/* game modes */
gamemode_t *mode_intro;
gamemode_t *mode_game;
gamemode_t *mode_pause;
gamemode_t *mode_scores;
/* not a real mode, shoudl always be NULL @see gamemode_valid() */
gamemode_t *mode_over;

enum color {
    ColorWhite,
    ColorMagenta,
    ColorRed
};

struct nyancat {
    struct {
        WINDOW *world;
        WINDOW *status;
    } ui;
} nc;

void error_exit(const char *format, ...);

#endif /* end of include guard: MAIN_H */
