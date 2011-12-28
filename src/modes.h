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
#ifndef MODES_H
#define MODES_H

#include "clock.h"

void intro_draw(void);
void intro_key_handler(gametime_t, const int);

void game_enter(void);
void game_leave(void);
void game_draw(void);
void game_key_handler(gametime_t, const int);

void pause_enter(void);
void pause_key_handler(gametime_t, const int);

void scores_draw(void);
void scores_key_handler(gametime_t, const int);

#endif /* end of include guard: MODES_H */
