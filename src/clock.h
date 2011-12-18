#ifndef CLOCK_H
#define CLOCK_H

typedef double game_time;


void clock_init(void);
void clock_freeze(void);
void clock_thaw(void);
game_time clock_get_relative(void);

#endif /* end of include guard: CLOCK_H */
