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
  } else {
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

void checkForBlockAction(nunchuk_t *nun_data, Level *lvl, Dude *dude) {
  int blockAction = readBlockAct(&nun_data);
   int blockLocation = dude.x;
  if (dude.direction == RIGHT) 
    blockLocation += 1;
  else
    blockLocation -= 1;
  if (! blockAction) return;
    if (blockAction > 0) {
      pickupBlock(blockLocation,lvl,dude);
  } else {
      dropBlock(blockLocation,lvl,dude);
  }
}

void pickupBlock(int blockLocation, Level *lvl, Dude *dude) {
  int blockX = getIndexOfBlockInLevel(blockLocation);
  int move = dude->direction == RIGHT ? 1 : -1;
  if (isObstacleThere(move,*lvl,*dude)) {
    return;
  } else {
    lvl->blocks[blockX] = -1;
    dude->hasBlock = 1;
  }
}

void dropBlock(int blockLocation, Level *lvl, Dude *dude) {
  int blockX = getIndexOfBlockInLevel(lvl,-1);
  int move = dude->direction == RIGHT ? 1 : -1;
  if (isObstacleThere(move,*lvl,*dude)) {
    return;
  } else {
    lvl->blocks[blockX] = dude->x + move;
    dude->hasBlock = 0;
 }
}

int getIndexOfBlockInLevel(Level lvl, int blockX) {
  int i = 0;
  for (i = 0; i < lvl.numBlocks; i++) {
    if (lvl.blocks[i] == blockX)
      break;
  }
  return i;
}

int invalidMove(int move, Level lvl, Dude dude) {
  if ((dude.x <= 1 && move == -1) || (dude.x >= lvl.numCols - 2 && move == 1))
    return 1;
  else if (isObstacleThere(move, lvl, dude))
    return 1;
  else
    return 0;
}

// call this from the first if inside gamestep
void moveDude(int move, Level lvl, Dude *dude, State *state, int *validMove) {
  if (invalidMove(move, lvl, *dude)) {
    *validMove = 0;
    return;
  }
  
  eraseOldDude(*dude);

  if (move == 1) {
    if (state->left == 0) {
      if (dude->screenPos == 6)
	state->left += move;
      else
	dude->screenPos += move;
    } else if (state->left == lvl.numCols - 13) {
      dude->screenPos += move;
    } else
      state->left += move;
  
  } else if (move == -1) {
    if (state->left == 0) {
      dude->screenPos += move;
    } else if (state->left == lvl.numCols - 13) {
      if (dude->screenPos == 6)
	state->left += move;
      else
	dude->screenPos += move;
    } else
      state->left += move;
  }

  dude->direction = move > 0 ? RIGHT : LEFT;
  dude->x += move;
  dude->y = getYPosnOfBlock(lvl, dude->x);
  //drawDude(*dude, state->dudeColor, state->capColor);
}

  // move will be the value we got from the readMove function above, which also means the direction. this function should do all the updating of the values in dude.x, state, etc, and also checking of whether the move is valid or not

int isObstacleThere(int move, Level lvl, Dude dude) {
  int nextX = dude.x + move;
  int currHeight = dude.y;
  int topNextStack = getYPosnOfBlock(lvl, nextX);
  if (topNextStack - currHeight > 1)
    return 1;
  else 
    return 0;
}

/* void moveDudeRight(Dude *dude) { */
/*   int x = dude->x; */
/*   int y = dude->y; */
/*   int direction = dude->direction; */
  
/*   if (isObstacleThere(x, y, direction)) { */
    
/*   } */
/* } */

/* void moveDudeLeft(Dude *dude) { */
/*   int x = dude->x; */
/*   int y = dude->y; */
/*   int direction = dude->direction; */
  
/* } */

