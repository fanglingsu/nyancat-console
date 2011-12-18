#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))
#define DEBUG_MSG(x) (mvwprintw(stdscr, SCREENHEIGHT + 1, 0, x))

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
    } ui;
    enum mode mode;
} nc;

void error_exit(const char *format, ...);

#endif /* end of include guard: MAIN_H */
