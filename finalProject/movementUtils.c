/*
 * movementUtils.c
 *
 * Part of: C335 Final Project
 * Created: 12.7.14 Erin Leonhard (eeleonha) and Zhongren Shao (shaoz)
 * Last Updated: 12.7.14
 *
 * The below definitions handle movement through the game.
 */

/* LCD COORDINATES WHEN FLIPPED
 *          _ _ _ _ _ _ _ _
 * (127,0) |               | (127,159)
 *         |               |
 *         |               |
 *         |               |
 *   (0,0) |_ _ _ _ _ _ _ _| (0,159)
 */

#include "structures.h"

#define BLOCKWIDTH 12
#define BLOCKHEIGHT 12
#define DUDEHEIGHT 24

int isObstacleThere(int x, int y, int direction) {
  

}

void moveDudeRight(Dude *dude) {
  int x = dude->x;
  int y = dude->y;
  int direction = dude->direction;
  
  if (isObstacleThere(x, y, direction)) {
    
  }
}

void moveDudeLeft(Dude *dude) {
  int x = dude->x;
  int y = dude->y;
  int direction = dude->direction;
  
}

