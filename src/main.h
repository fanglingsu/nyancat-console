/**
 * nyancat-console - a clone of the nyancat lost in space flash game.
 *
 * Copyright (C) 2011 Daniel Carl
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
#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdio.h>
#include "config.h"
#include "gamemode.h"
#include "queue.h"

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

#define err_msg(...) \
{                   \
    werase(nc.ui.debug); \
    wprintw(nc.ui.debug, __VA_ARGS__); \
    wnoutrefresh(nc.ui.debug); \
    doupdate(); \
}

#ifdef DEBUG
#   define DEBUG_FORMAT "%10s:%20s() %03d: "
#   define DEBUG_ARGS __FILE__, __func__, __LINE__
#   define debug(format, ...) err_msg(DEBUG_FORMAT format, DEBUG_ARGS, __VA_ARGS__)
#else
#   define debug(...)
#endif

enum color {
    ColorWhite,
    ColorMagenta,
    ColorRed,
    ColorYellow,
};

typedef struct {
    int y;
    int x;
} coordinate_t;

typedef struct {
    struct {
        WINDOW *world;
        WINDOW *status;
#if DEBUG
        WINDOW *debug;
#endif
        coordinate_t screen;
    } ui;
} nyancat_t;

nyancat_t nc;

/* game modes */
gamemode_t *mode_intro;
gamemode_t *mode_game;
gamemode_t *mode_pause;
gamemode_t *mode_scores;

void error_exit(const char *, ...);

#endif /* end of include guard: MAIN_H */
