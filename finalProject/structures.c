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

Dude makeNewPlayer(int x, int y, int direction) {
  Dude newDude = { x, y, direction, 1};
  return newDude;
}

State defaultState() {
  State state = {0, BLACK, CYAN, RED, BLUE, WHITE, YELLOW};
  return state;
}

Level initLevel() {
  Level lvl = {1, 13, 
	       {3, 8, 10, 10, 10}, 5, 
	       {1023, 1, 1, 3, 7, 15, 31, 63, 1, 1, 1, 1, 1, 1, 1023}, 15};
  return lvl;
}
