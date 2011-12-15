#ifndef CONFIG_H
#define CONFIG_H

#define REAL_NAME "nyancat-console"
#define VERSION "0.0.1"

/* number of frames per second - higher values = faster gameing */
#define FPS 15

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

#define USE_COLORS

#endif /* end of include guard: CONFIG_H */
