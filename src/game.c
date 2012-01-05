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
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "world.h"
#include "cat.h"
#include "status.h"
#include "game.h"
#include "util.h"

static float tickbase;
static score_t score_multiplier;
static unsigned short score_extra_multiplier;
static score_t score;
static score_t highscore;
static unsigned short multiplier_unset_protect;
static unsigned short achievements;

static void game_scroll_handler(gametime_t, void *);
static void game_save_hightscore(void);
static void game_load_highscore(void);
static char *game_get_highscore_file(void);

/**
 * Initializes the game.
 */
void game_init(void)
{
    tickbase = 1;
    score_multiplier = 1;
    score_extra_multiplier = 1;
    score = 0;
    highscore = 0;
    achievements = 0;

    multiplier_unset_protect = FALSE;

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
    cat_start();
}

/**
 * Set the value given to TICK() for the games base speed.
 */
void game_set_tickbase(float base)
{
    if (base < 0) {
        tickbase = base * -1;
    } else {
        tickbase = base;
    }
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
 * Increaments the score multiplier by given steps.
 */
void game_increment_multiplier(const unsigned int steps)
{
    score_multiplier += steps;
}

/**
 * Unset the scrore multiplier to it's default value.
 */
void game_unset_multiplier(void)
{
    if (multiplier_unset_protect) {
        return;
    }
    score_multiplier = 1;
}

/**
 * Retreive the current used score multiplier.
 */
score_t game_get_multiplier(void)
{
    return score_multiplier;
}

/**
 * Set the flag to not unset multiplier on call of
 * game_unset_multiplier().
 */
void game_set_multiplier_unset_protect(void)
{
    multiplier_unset_protect = TRUE;
}

/**
 * Remove the protection for multiplier to be unseted by call off
 * game_unset_multiplier().
 */
void game_remove_multiplier_unset_protect(void)
{
    multiplier_unset_protect = FALSE;
}

/**
 * Set an extra multiplier for the scores.
 */
void game_set_extra_multiplier(unsigned short multiplier)
{
    score_extra_multiplier = multiplier;
}

/**
 * Unset the extra multiplier.
 */
void game_unset_extra_multiplier(void)
{
    score_extra_multiplier = 1;
}

/**
 * Retrieves the value of the extra multipicator.
 */
unsigned short game_get_extra_multiplier(void)
{
    return score_extra_multiplier;
}

/**
 * Inidcates if the multiplier unset protection is enabled.
 */
unsigned short game_has_multiplier_unset_protect(void)
{
    return multiplier_unset_protect;
}

/**
 * Increments the scores by given steps multiplied with current multiplier.
 */
void game_increment_score(const unsigned int steps)
{
    score += score_multiplier * score_extra_multiplier * steps;
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
 * Adds given achievement.
 */
void game_add_achievement(enum achievement value)
{
    achievements |= value;
}

/**
 * Inidicates if given achievment was reached.
 */
short game_has_achievement(enum achievement value)
{
    return (achievements & value) == value ? 1 : 0;
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
    queue_add_event(time + TICK(tickbase), game_scroll_handler, NULL);
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
