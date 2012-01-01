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
#ifndef GAME_H
#define GAME_H

#include "clock.h"

typedef unsigned int score_t;

void game_init(void);
void game_start(void);
void game_set_tickbase(float);
void game_save_scores(void);
void game_increment_multiplicator(const unsigned int);
void game_unset_multiplicator(void);
void game_set_multiplicator_unset_protect(void);
void game_remove_multiplicator_unset_protect(void);
void game_set_extra_multiplicator(unsigned short);
void game_unset_extra_multiplicator(void);
unsigned short game_get_extra_multiplicator(void);
unsigned short game_has_multiplicator_unset_protect(void);
unsigned int game_get_multiplicator(void);
void game_increment_score(const unsigned int);
void game_unset_score(void);
score_t game_get_score(void);
score_t game_get_highscore(void);

#endif /* end of include guard: GAME_H */
