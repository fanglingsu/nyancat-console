/**
 * nyancat-console - a clone of the nyancat lost in space flash game.
 *
 * Copyright (C) 2011-2012 Daniel Carl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include "main.h"
#include "random.h"
#include "config.h"
#include "modes.h"
#include "io.h"

static void init_windows(void);
static void init_colors(void);
static void init_modes(void);
static void print_help_message(void);
static void print_licence_message(void);
static void set_signals(void);
static void signal_handler(const int);
static void loop(void);
static void prepare_for_exit(void);
static void free_memory(void);

int main(int argc, char *argv[])
{
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1) {
        switch (c) {
            case 'h':
                print_help_message();
                prepare_for_exit();
                return EXIT_FAILURE;
            case 'v':
                print_licence_message();
                prepare_for_exit();
                return EXIT_FAILURE;
        }
    }
    random_init();
    set_signals();
    init_windows();
    init_modes();

    loop();

    prepare_for_exit();
    free_memory();
    return EXIT_SUCCESS;
}

/**
 * @format: Formatstring like in fprintf.
 *
 * Signal a fatal error and quit immediately.
 */
void error_exit(const char *format, ...)
{
    va_list ap;

    /* clean all ncurses windows */
    prepare_for_exit();
    free_memory();

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fputc('\n', stderr);
    va_end(ap);

    exit(EXIT_FAILURE);
}

/**
 * Initialize the used windows
 */
static void init_windows(void)
{
    extern nyancat_t nc;

    initscr();
    init_colors();
    cbreak();       /* line buffering disabled, pass on everty thing to me */
    noecho();       /* don't print typed chars */
    curs_set(0);    /* don't show a carret */
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);

    /* create sub windows */
    nc.ui.world = newwin(SCREENHEIGHT, SCREENWIDTH, 0, 0);
    nc.ui.status = newwin(1, SCREENWIDTH, SCREENHEIGHT, 0);
#ifdef DEBUG
    nc.ui.debug = newwin(5, SCREENWIDTH, SCREENHEIGHT + 1, 0);
#endif
}

/**
 * Setup the colors used in the game.
 */
static void init_colors(void)
{
    if (has_colors() != FALSE) {
        start_color();
    }

    if (can_change_color()) {
        init_color(COLOR_MAGENTA, 1000, 600, 1000);
        init_color(COLOR_GREY, 300, 300, 300);
    }
    /* define used color pairs */
    init_pair(ColorMagenta, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(ColorRed, COLOR_RED, COLOR_BLACK);
    init_pair(ColorYellow, COLOR_YELLOW, COLOR_BLACK);
    init_pair(ColorBlue, COLOR_BLUE, COLOR_BLACK);
    init_pair(ColorGrey, COLOR_GREY, COLOR_BLACK);
}

/**
 * Initializes the game modes.
 */
static void init_modes(void)
{
    extern gamemode_t *mode_intro, *mode_game, *mode_pause, *mode_scores;

    mode_intro = mode_create("Intro", NULL, NULL, intro_draw, intro_key_handler);
    mode_scores = mode_create("Scores", scores_enter, NULL, scores_draw, scores_key_handler);
    mode_game = mode_create("Game", game_enter, game_leave, game_draw, game_key_handler);
    mode_pause = mode_create("Pause", pause_enter, NULL, pause_draw, pause_key_handler);
}

/**
 * Prints the help information.
 */
static void print_help_message(void)
{
    puts("Usage: " REAL_NAME " [OPTION...]\n");
    puts("  -h      give this help list");
    puts("  -v      print programm version and licence\n");
    puts("Report bugs at https://github.com/fanglingsu/nyancat-console.");
}

/**
 * Prints the licence message to stdout.
 */
static void print_licence_message(void)
{
    puts(REAL_NAME " " VERSION " Copyright 2011-2012 Daniel Carl\n");
    puts(REAL_NAME " comes with ABSOLUTELY NO WARRANTY; for details");
    puts("see LICENSE file. This is free software, and you are welcome to");
    puts("redistribute it under the terms of the GNU General Public License.");
}

/**
 * Sets up signal handlers we need.
 */
static void set_signals(void)
{
    struct sigaction sa;

    /* fill in sigaction struct */
    sa.sa_handler = &signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    /* set signal handlers */
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
#ifdef SIGWINCH
    sigaction(SIGWINCH, &sa, NULL);
#endif

    /* ignore sigtstp */
    sa.sa_handler = SIG_IGN;
    sigaction(SIGTSTP, &sa, NULL);
}

/**
 * @sig: number of the signal
 *
 * Signal handler called if signal is emmitted.
 */
static void signal_handler(const int sig)
{
    prepare_for_exit();
#ifdef SIGWINCH
    if (SIGWINCH == sig) {
        refresh();
        init_windows();
        gamemode_draw();

        return;
    }
#endif
    free_memory();
    exit(EXIT_FAILURE);
}

/**
 * Main game loop.
 */
static void loop(void)
{
    extern nyancat_t nc;
    gametime_t time;
    int key_ready = 0, ch;

    /* switch to intro mode */
    gamemode_enter(mode_intro);
    gamemode_draw();

    while (gamemode_valid()) {
        /* look for the first to run event */
        time = queue_get_first_time();
        if (time) {
            /* wait for time to run the first event form queue or the first
             * keypress, wichever comes first. */
            key_ready = io_wait_for_key(time);
        } else {
            /* no event queue item found */
            io_wait_for_key(0);
            key_ready = 1;
        }
        if (key_ready > 0) {
            ch = getch();
            /* no redraw call explicite because this should be done bey the
             * key handlers called functions */
            gamemode_key(ch);
        } else {
            /* TODO find a better way for prevening hight cpu usage here. But
             * for now insert a sleep for the minimum ticks used in game */
            usleep(SECOND * TICK(0.1));
            queue_run_until(time);
        }
        doupdate();
    }
}

/**
 * Cleanup the environment.
 */
static void prepare_for_exit(void)
{
    extern nyancat_t nc;

    wrefresh(nc.ui.status);
    delwin(nc.ui.status);

    wrefresh(nc.ui.world);
    delwin(nc.ui.world);

    endwin();

    /* flush open streams */
    fflush(NULL);
}

/**
 * frees memory allocated for the game and modes.
 */
static void free_memory(void)
{
    queue_free();

    free(mode_intro);
    free(mode_scores);
    free(mode_game);
    free(mode_pause);
}
