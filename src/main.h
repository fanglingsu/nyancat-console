#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

enum mode {
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
        WINDOW *empty;
    } ui;
    enum mode current_mode;
} nc;

void error_exit(const char *format, ...);

#endif /* end of include guard: MAIN_H */
