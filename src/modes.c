/**
 * This file contains callback functions used in the modes.
 */
#include "main.h"
#include "status.h"
#include "modes.h"
#include "config.h"
#include "clock.h"
#include "world.h"
#include "cat.h"

/**
 * Print the introduction message on the screen.
 */
void intro_draw(void)
{
    extern struct Nyancat nc;

    werase(nc.ui.world);
    waddstr(nc.ui.world, "Press Enter to start " REAL_NAME "\n\n");
    waddstr(nc.ui.world, "Use j, k or the cursor keys to move the cat.");
    wnoutrefresh(nc.ui.world);
    doupdate();
}

/**
 * @key: Pressed key.
 *
 * Handles key presses from user.
 */
void intro_key_handler(game_time time, const int key)
{
    if (10 == key) {
        mode_enter(mode_game);
    }
}

/**
 * Enter game mode. If first time called the required game objects are
 * initialized.
 */
void game_enter(void)
{
    static int initialized = 0;

    /* prevent from reinitialisation after pause mode */
    if (!initialized) {
        world_init();
        cat_init();
        world_start_scrolling();
        initialized = 1;
    }
}

/**
 * Draw the next frame of the game.
 */
void game_draw(void)
{
    world_print();
    cat_print();
    status_print();
}

/**
 * Handle keypresses for game mode.
 */
void game_key_handler(game_time time, const int key)
{
    switch (key) {
        case KEY_UP:
            cat_jump_up(time);
            break;

        case KEY_DOWN:
            cat_jump_down();
            break;

        case 'p':
            mode_enter(mode_pause);
            break;

        case 'q':
            mode_enter(mode_scores);
            break;

        default:
            break;
    }
}

/**
 * Freeze the clock if paus mode is entered.
 */
void pause_enter(void)
{
    clock_freeze();
}

/**
 * Thaw the clock on leaving the pause mode.
 */
void pause_leave(void)
{
    clock_thaw();
}

/**
 * @key: Key presses by the user.
 *
 * Handles the keypresses within the pause mode.
 */
void pause_key_handler(game_time time, const int key)
{
    switch (key) {
        case 'p':
            mode_enter(mode_game);
            break;

        case 'q':
            mode_enter(mode_scores);
    }
}

/**
 * Drow the scores page on the end of the game.
 */
void scores_draw(void)
{
    extern struct Nyancat nc;

    werase(nc.ui.world);
    wattron(nc.ui.world, COLOR_PAIR(ColorRed));
    mvwprintw(nc.ui.world, 5, 2, "GAME OVER!");
    wattroff(nc.ui.world, COLOR_PAIR(ColorRed));
    mvwprintw(nc.ui.world, 7, 2, "You have nyaned for %.2lfs", clock_get_relative());
    mvwprintw(nc.ui.world, 9, 2, "Press q to quit.");
    wnoutrefresh(nc.ui.world);

    /* remove content from status window */
    werase(nc.ui.status);
    wnoutrefresh(nc.ui.status);
}

/**
 * @key: Pressed key.
 *
 * Handles key presses from user.
 */
void scores_key_handler(game_time time, const int key)
{
    if ('q' == key) {
        mode_exit();
    }
}
