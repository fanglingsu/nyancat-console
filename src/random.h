#ifndef RANDOM_H
#define RANDOM_H

void random_init(void);
int random_range(unsigned floor, unsigned ceil);
int random_range_step(unsigned floor, unsigned ceili, int step);

#endif /* end of include guard: RANDOM_H */
