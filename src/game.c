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
#include "main.h"
#include "world.h"
#include "cat.h"
#include "status.h"
#include "game.h"

static unsigned int score_multiplicator;
static unsigned int score;

static void game_scroll_handler(gametime_t, void *);

/**
 * Initializes the game.
 */
void game_init(void)
{
    score_multiplicator = 1;
    score = 0;

    /* initialize the game objects */
    clock_init();
    world_init();
    cat_init();
}

/**
 * Starts the game by adding required scrollhandlers to the event queue.
 */
void game_start(void)
{
    /* register games scroll handler that moves the cat and screen */
    game_scroll_handler(clock_get_relative(), NULL);
    queue_add_event(clock_get_relative(), cat_move_handler, NULL);
}

/**
 * Increaments the score multiplicator by given steps.
 */
void game_increment_multiplicator(const unsigned int steps)
{
    score_multiplicator += steps;
}

/**
 * Unset the scrore multiplicator to it's default value.
 */
void game_unset_multiplicator(void)
{
    score_multiplicator = 1;
}

/**
 * Retreive the current used score multiplicator.
 */
unsigned int game_get_multiplicator(void)
{
    return score_multiplicator;
}

/**
 * Increments the scores by given steps multiplied with current multiplicator.
 */
void game_increment_score(const unsigned int steps)
{
    score += score_multiplicator * steps;
}

/**
 * Unsets all scores to zero.
 */
void game_unset_score(void)
{
    score = 0;
}

/**
 * Retrieves the scores.
 */
unsigned int game_get_score(void)
{
    return score;
}

/**
 * Moves cat and world every tick.
 */
static void game_scroll_handler(gametime_t time, void *data)
{
    cat_move_right(1);
    world_move_screen_right(1);
    gamemode_draw();

    status_print();

    /* readd to the queue */
    queue_add_event(time + TICK(1), game_scroll_handler, NULL);
}
