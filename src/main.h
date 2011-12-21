#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdio.h>
#include "mode.h"

#define DEBUG 1

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

Mode *mode_intro;
Mode *mode_game;
Mode *mode_pause;
Mode *mode_scores;
/* not a real mode, shoudl always be NULL @see mode_valid() */
Mode *mode_over;

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
