#include "main.h"
#include "world.h"
#include "cat.h"
#include "gamemode.h"
#include "queue.h"
#include "status.h"
#include "game.h"

/**
 * Moves cat and world every tick.
 */
void game_scroll_handler(gametime_t time, void *data)
{
    cat_move_right(1);
    world_move_screen_right(1);
    gamemode_draw();

    status_print();

    /* readd to the queue */
    queue_add_event(time + TICK(1), game_scroll_handler, NULL);
}
