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

/**
 * This file contains callback functions used in the modes.
 */
#include "main.h"
#include "status.h"
#include "modes.h"
#include "world.h"
#include "cat.h"
#include "game.h"

static int game_initialized = 0;

/**
 * Print the introduction message on the screen.
 */
void intro_draw(void)
{
    extern nyancat_t nc;

    werase(nc.ui.world);
    waddstr(nc.ui.world, "Press Enter to start " REAL_NAME "\n\n");
    waddstr(nc.ui.world, "Use j, k or the cursor keys to move the cat.");
    wnoutrefresh(nc.ui.world);
    doupdate();
}

/**
 * @key: Pressed key.
 *
 * Handles key presses from user.
 */
void intro_key_handler(gametime_t time, const int key)
{
    if (10 == key) {
        gamemode_enter(mode_game);
    } else if ('q' == key) {
        /* init clock to show a real time in scores mode */
        clock_init();
        gamemode_enter(mode_scores);
    } else {
        gamemode_draw();
    }
}

/**
 * Enter game mode. If first time called the required game objects are
 * initialized.
 */
void game_enter(void)
{
    clock_thaw();

    /* prevent from reinitialisation after pause mode */
    if (!game_initialized) {
        clock_init();
        game_init();
        game_initialized = 1;
    }
    game_start();
}

void game_leave(void)
{
    clock_freeze();
}

/**
 * Draw the next frame of the game.
 */
void game_draw(void)
{
    world_print();
    cat_print();
}

/**
 * Handle keypresses for game mode.
 */
void game_key_handler(gametime_t time, const int key)
{
    switch (key) {
        case 'k':
        case KEY_UP:
            cat_jump_up(time);
            break;

        case 'j':
        case KEY_DOWN:
            cat_jump_down(time);
            break;

        case 'p':
            gamemode_enter(mode_pause);
            break;

        case 'q':
            gamemode_enter(mode_scores);
            break;

        default:
            break;
    }
}

/**
 * Freeze the clock if paus mode is entered.
 */
void pause_enter(void)
{
    pause_draw();
}

void pause_draw(void)
{
    game_draw();
    status_print();
}

/**
 * @key: Key presses by the user.
 *
 * Handles the keypresses within the pause mode.
 */
void pause_key_handler(gametime_t time, const int key)
{
    switch (key) {
        case 'p':
            gamemode_enter(mode_game);
            break;

        case 'q':
            gamemode_enter(mode_scores);
    }
}

/**
 * Enters the score mode.
 */
void scores_enter(void)
{
    scores_draw();
}

/**
 * Drow the scores page on the end of the game.
 */
void scores_draw(void)
{
    extern nyancat_t nc;

    werase(nc.ui.world);
    wattron(nc.ui.world, COLOR_PAIR(ColorRed));
    mvwprintw(nc.ui.world, 5, 2, "GAME OVER!");
    wattroff(nc.ui.world, COLOR_PAIR(ColorRed));
    mvwprintw(nc.ui.world, 7, 2, "Time (in seconds):           % 9.2lf", clock_get_relative());
    mvwprintw(nc.ui.world, 8, 2, "Scores:                      % 9u", game_get_score());
    mvwprintw(nc.ui.world, 9, 2, "High-Scores:                 % 9u", game_get_highscore());
    mvwprintw(nc.ui.world, 12, 2, "Press 'r' to run again or 'q' to quit.");
    wnoutrefresh(nc.ui.world);

    /* remove content from status window */
    werase(nc.ui.status);
    wnoutrefresh(nc.ui.status);
}

/**
 * @key: Pressed key.
 *
 * Handles key presses from user.
 */
void scores_key_handler(gametime_t time, const int key)
{
    if ('r' == key) {
        game_save_scores();
        /* uninitialize to previous game */
        game_initialized = 0;
        gamemode_enter(mode_game);
    } else if ('q' == key) {
        game_save_scores();
        mode_exit();
    }
}
