#ifndef CONFIG_H
#define CONFIG_H

#define REAL_NAME "nyancat-console"
#define VERSION "0.0.1"

#define DEBUG 0

/* speed */
#define FPS 15  /* number of frames per second - higher values = faster gameing */
#define SECOND 1000000

#define SPEED 1.0
#define TICK(x) ((x)*0.08/(SPEED))

/* dimension for the viewport of the game */
#define SCREENHEIGHT 24
#define SCREENWIDTH  80
#define WORLDWIDTH   900

/* height of the world for nyan should be >= SCREENHEIGHT */
#define WORLDHEIGHT  24

/*
 ,-----,
 |:::/\/\
~|___(o.o)
  UU  UU
*/
#define CATHIGHT  4
#define CATWIDTH 10

#endif /* end of include guard: CONFIG_H */
