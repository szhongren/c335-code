/* 
 * Filename: structures.c
 *
 * Part of: C335 Final Project
 * Created: 12.7.14
 * Last-Updated: Erin Leonhard (eeleonha) &
 *           By: Zhongren Shao (shaoz)
 *           On: 12.7.14
 */

/* Code: */
#include "structures.h"
#include <f3d_lcd_sd.h>
#define BRICKS1 1
#define BRICKS2 3
#define BRICKS3 7
#define BRICKS4 15
#define BRICKS5 31
#define BRICKS6 63
#define BRICKS7 127
#define BRICKS8 255
#define BRICKS9 511


Dude makeNewPlayer(int x, int y, int direction) {
  Dude newDude = { x, y, direction, 1};
  return newDude;
}

State defaultState() {
  State state = {0, BLACK, CYAN, RED, BLUE, WHITE, YELLOW};
  return state;
}

Level initLevel() {
  Level lvl = {1, 18, 
	       {8, 9, 10, 10, 11}, 5, 
	       {BRICKS9, BRICKS1, BRICKS1, BRICKS2, BRICKS3, 
		BRICKS4, BRICKS5, BRICKS6, BRICKS1, BRICKS1, 
		BRICKS1, BRICKS1, BRICKS1, BRICKS1, BRICKS2,
		BRICKS2, BRICKS3, BRICKS3, BRICKS3, BRICKS9 }, 
	       20};
  return lvl;
}
