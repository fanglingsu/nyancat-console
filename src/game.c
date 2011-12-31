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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "world.h"
#include "cat.h"
#include "status.h"
#include "game.h"
#include "util.h"

static score_t score_multiplicator;
static score_t score;
static score_t highscore;

static void game_scroll_handler(gametime_t, void *);
static void game_save_hightscore(void);
static void game_load_highscore(void);
static char *game_get_highscore_file(void);

/**
 * Initializes the game.
 */
void game_init(void)
{
    score_multiplicator = 1;
    score = 0;
    highscore = 0;

    /* initialize the game objects */
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
 * Persists the highscores into file.
 */
void game_save_scores(void)
{
    if (score >= highscore) {
        highscore = score;
        game_save_hightscore();
    }
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
score_t game_get_multiplicator(void)
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
score_t game_get_score(void)
{
    return score;
}

/**
 * Retreives the highscores.
 */
score_t game_get_highscore(void)
{
    game_load_highscore();

    if (score > highscore) {
        highscore = score;
    }

    return highscore;
}

/**
 * Moves cat and world every tick.
 */
static void game_scroll_handler(gametime_t time, void *data)
{
    cat_move_right();
    world_move_screen_right();
    gamemode_draw();

    status_print();

    /* readd to the queue */
    queue_add_event(time + TICK(1), game_scroll_handler, NULL);
}

/**
 * Saves the highscores to a file.
 */
static void game_save_hightscore(void)
{
    char *file = game_get_highscore_file();
    char *num;

    if (!file) {
        return;
    }

    FILE *fp = fopen(file, "w");
    free(file);
    if (!fp) {
        return;
    }

    /* converts the integer to string with 10 digits + \0 */
    num = xmalloc(sizeof(score_t) * 11);
    sprintf(num, "%d", highscore);

    fputs(num, fp);
    free(num);
    fclose(fp);
}

/**
 * Loads the highscores from file.
 */
static void game_load_highscore(void)
{
    char *num;
    char *file = game_get_highscore_file();

    if (!file) {
        return;
    }

    FILE *fp = fopen(file, "r");
    free(file);
    if (!fp) {
        return;
    }

    /* allocate memory for the read number 10 digits + \0 */
    num = xmalloc(sizeof(score_t) * 11);
    num = fgets(num, 11, fp);
    if (NULL == num) {
        highscore = 0;
    } else {
        highscore = atoi(num);
    }
    free(num);
    fclose(fp);
}

/**
 * Retrieves the path to the highscore file.
 *
 * Don't forget to free the memory.
 */
static char *game_get_highscore_file(void)
{
    const char *home = getenv("HOME");
    const char *file = ".nyancat-console";

    char *filepath = xmalloc(strlen(home) + strlen(file) + 2);

    sprintf(filepath, "%s/%s", home, file);
    return filepath;
}
