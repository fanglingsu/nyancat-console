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
#ifndef CONFIG_H
#define CONFIG_H

#define REAL_NAME "nyancat-console"
#define VERSION "0.0.4"

#define DEBUG 1

#define SECOND 1000000
#define SPEED 1.0
#define TICK(x) ((x)*0.05/(SPEED))

/* time in seconds after that the multiplicator will be resetet if no further
 * milk is collected */
#define MULTIPLIER_TIMEOUT 10

/* dimension for the viewport of the game */
#define SCREENHEIGHT 24
#define SCREENWIDTH  80

/* steps that can nyan move vertical befor the screen is moved to */
#define SCREENHYSTERESIS 3

/* distance of cat from left border */
#define CAT_XOFFSET  8

/* plattforms placed on to screen widths */
#define MAX_PLATFORMS 50

/* height of the world for nyan should be >= SCREENHEIGHT */
#define WORLDHEIGHT  96

#endif /* end of include guard: CONFIG_H */
