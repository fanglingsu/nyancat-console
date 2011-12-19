#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdio.h>

#define DEBUG 0

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

enum gamemode {
    ModeIntro,
    ModeGame,
    ModePause,
    ModeScores,
    ModeOver
};

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
    enum gamemode mode;
} nc;

void error_exit(const char *format, ...);

#endif /* end of include guard: MAIN_H */
