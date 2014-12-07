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

Dude makeNewPlayer(int x, int y, int direction) {
  Dude newDude = { x, y, direction };
  return newDude;
}
