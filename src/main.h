#ifndef MAIN_H
#define MAIN_H

#define TIMESTEP 900000

enum mode {
    ModeIntro,
    ModeGame,
    ModePause,
    ModeScores
};

struct Nyancat {
    struct {
        WINDOW *mainwin;
        WINDOW *world;
        WINDOW *status;
    } ui;
} nc;

void init_windows(void);
void show_start_screen(void);
void print_statusline(char* str);
void print_licence_message(void);
void set_timer(void);
void set_signals(void);
void signal_handler(int sig);
void cleanup_windows(void);

#endif /* end of include guard: MAIN_H */
