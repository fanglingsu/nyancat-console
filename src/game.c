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
#include "gamemode.h"
#include "queue.h"
#include "status.h"
#include "game.h"

static unsigned int score_multiplicator = 1;
static unsigned int score = 0;

/**
 * Moves cat and world every tick.
 */
void game_scroll_handler(gametime_t time, void *data)
{
    cat_move_right(1);
    world_move_screen_right(1);
    gamemode_draw();

    status_print();

    /* readd to the queue */
    queue_add_event(time + TICK(1), game_scroll_handler, NULL);
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
