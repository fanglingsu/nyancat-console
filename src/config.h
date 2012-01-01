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
#ifndef CONFIG_H
#define CONFIG_H

#define REAL_NAME "nyancat-console"
#define VERSION "0.0.7"

#define DEBUG 0

#define SECOND 1000000
#define SPEED 1.0
#define TICK(x) ((x)*0.05/(SPEED))

/* if defined it gives extra points for every frame nyan keeps in fast fall
 * mode */
#define FEATURE_SCORE_STUNT 1

/* number of frames nyan is invisible in the middle. 5 means from 6 frames
 * nyan is 5 frames invisible */
#define GHOST_MODE_FRAMES 5

/* time in seconds after that the multiplier will be resetet if no further
 * milk is collected */
#define MULTIPLIER_TIMEOUT 10

/* time in seconde after that special cat modes will fade */
#define SPECIALMODE_TIMEOUT 20

/* time in seconds the gems has effect */
#define GEMSTONE_TIMEOUT 45

/* dimension for the viewport of the game */
#define SCREENHEIGHT 24
#define SCREENWIDTH  80

/* steps that can nyan move vertical befor the screen is moved to */
#define SCREENHYSTERESIS 3

/* distance of cat from left border */
#define CAT_XOFFSET  8

/* plattforms placed on to screen widths */
#define MAX_PLATFORMS 50

/* length restrictions for generated platform elements */
#define PLATFORM_MIN_WIDTH 12
#define PLATFORM_MAX_WIDTH 24

/* height of the world for nyan should be >= SCREENHEIGHT */
#define WORLDHEIGHT  96

#endif /* end of include guard: CONFIG_H */
