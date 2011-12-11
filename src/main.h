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
        WINDOW *mainwin;
        WINDOW *world;
        WINDOW *status;
    } ui;
    enum mode current_mode;
} nc;

void init_windows(void);
void clear_windows(void);
void show_start_screen(void);
void read_input(void);
void print_statusline(char* str);
void print_licence_message(void);
void set_timer(void);
void set_signals(void);
void signal_handler(int sig);
void game_handler(void);
void cleanup_windows(void);

#endif /* end of include guard: MAIN_H */
