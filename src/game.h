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

void game_init(void);
void game_start(void);
void game_increment_multiplicator(const unsigned int);
void game_unset_multiplicator(void);
unsigned int game_get_multiplicator(void);
void game_increment_score(const unsigned int);
void game_unset_score(void);
unsigned int game_get_score(void);

#endif /* end of include guard: GAME_H */
