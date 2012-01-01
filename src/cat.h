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
#ifndef CAT_H
#define CAT_H

#include "clock.h"

#define CATHEIGHT 4
#define CATWIDTH 10

void cat_init(void);
void cat_start(void);
void cat_jump_up(gametime_t);
void cat_jump_down(gametime_t);
int cat_get_height(void);
void cat_move_right(void);
void cat_move_handler(gametime_t, void *);
void cat_print(void);

#endif /* end of include guard: CAT_H */
