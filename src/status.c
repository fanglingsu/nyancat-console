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
#include "status.h"
#include "gamemode.h"
#include "clock.h"
#include "game.h"

enum item {
    StatusRuntime,
    StatusFrames,
    StatusDebug
};

static void printstatusf(const char *, ...);

/**
 * Print out contens of the status to the status window.
 */
void status_print(void)
{
    unsigned short extra_mult = game_get_extra_multiplicator();
    /* multiplicator is 1 or 2 and the appended x and \0 */
    char extra_mult_msg[3];

    sprintf(extra_mult_msg, "%dx", extra_mult);
    printstatusf(
        "%s: % 4.2lfs - %09u x%03u %s%s",
        mode_get_name(),
        clock_get_relative(),
        game_get_score(),
        game_get_multiplicator(),
        game_has_multiplicator_unset_protect() ? "Long Life Milk" : "",
        extra_mult > 1 ? extra_mult_msg : ""
    );
}

/**
 * @format: Formatstring like in fprintf
 *
 * Prints out message to status line.
 */
static void printstatusf(const char *format, ...)
{
    extern nyancat_t nc;
    va_list ap;

    werase(nc.ui.status);
    va_start(ap, format);
    vwprintw(nc.ui.status, format, ap);
    va_end(ap);
    wrefresh(nc.ui.status);
}
