#ifndef CAT_H
#define CAT_H

#include "clock.h"

void cat_init(void);
void cat_jump_up(gametime_t time);
void cat_jump_down(void);
int cat_get_height(void);
void cat_move_right(const int steps);
void cat_run_handler(gametime_t time, void *data);
void cat_jump_handler(gametime_t time, void *data);
void cat_print(void);

#endif /* end of include guard: CAT_H */
