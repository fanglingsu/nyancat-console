#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>

enum mode {
    ModeIntro,
    ModeGame,
    ModePause,
    ModeScores,
    ModeOver
};

struct Nyancat {
    struct {
        WINDOW *world;
        WINDOW *status;
        WINDOW *empty;
    } ui;
    enum mode current_mode;
} nc;

void refresh_world(void);
void error_exit(const char *format, ...);

#endif /* end of include guard: MAIN_H */
