#ifndef CLOCK_H
#define CLOCK_H

typedef double gametime_t;

void clock_init(void);
void clock_freeze(void);
void clock_thaw(void);
gametime_t clock_get_relative(void);

#endif /* end of include guard: CLOCK_H */
