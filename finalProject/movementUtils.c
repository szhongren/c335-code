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
#include <f3d_nunchuk.h>
#include "structures.h"

int readMove(nunchuk_t *nun_data) {
  if (f3d_j_right(nun_data)) {
    return 1;
  } else if (f3d_j_left(nun_data)) {
    return -1;
n  } else {
    return 0;
  }
}

int readBlockAct(nunchuk_t *nun_data) {
  if (f3d_c_pressed(nun_data))
    return 1;
  else if (f3d_z_pressed(nun_data))
    return -1;
  else 
    return 0;
}

int invalidMove(int move, Level lvl, Dude dude) {
  if (dude.x <= 1 || dude.x >= lvl.numCols - 2)
    return 0;
  else if (isObstacleThere(dude.x, dude.y, dude.direction))
    return 0;
  else
    return 1;
}

// call this from the first if inside gamestep
void moveDude(int move, Level lvl, Dude dude, State state) {
  if (move == 1) {
    dude.direction = RIGHT;
    if (invalidMove(move, lvl, dude))
      return;

    if (state.left == 0) {
      if (dude.screenPos == 6)
	state.left += move;
      else
	dude.screenPos += move;
    } else if (state.left == lvl.numCols - 13) {
      dude.screenPos += move;
    } else
      state.left += move;
  
  } else if (move == -1) {
    dude.direction = LEFT;
    if (invalidMove(move, lvl, dude))
      return;

    if (state.left == 0) {
      dude.screenPos += move;
    } else if (state.left == lvl.numCols - 13) {
      if (dude.screenPos == 6)
	state.left += move;
      else
	dude.screenPos += move;
    } else
      state.left += move;
  }
  dude.x += move;
}

  // move will be the value we got from the readMove function above, which also means the direction. this function should do all the updating of the values in dude.x, stae, etc, and also checking of whether the move is valid or not
}

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

