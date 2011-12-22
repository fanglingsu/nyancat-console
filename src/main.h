#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdio.h>
#include "mode.h"
#include "queue.h"

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

#define err_msg(...) \
{                   \
    fprintf(stderr, "%s:%5d %s()", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, ": ");          \
    fprintf(stderr, __VA_ARGS__);   \
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
/* not a real mode, shoudl always be NULL @see mode_valid() */
gamemode_t *mode_over;

enum color {
    ColorWhite,
    ColorMagenta,
    ColorRed
};

struct Nyancat {
    struct {
        WINDOW *world;
        WINDOW *status;
    } ui;
} nc;

void clear_windows(void);
void error_exit(const char *format, ...);

#endif /* end of include guard: MAIN_H */
