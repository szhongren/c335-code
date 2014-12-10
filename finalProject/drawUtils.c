/*
 * drawUtils.c
 *
 * Part of: C335 Final Project
 * Authors: Erin Leonhard (eeleonha) and Zhongren Shao (shaoz)
 * Date Created: 12.5.14
 * 
 * Draw bricks, blocks, and dude.
 */

#include <f3d_lcd_sd.h>
#include "structures.h"


// getYPosnOfBlock
int getYPosnOfBlock(Level lvl, int x) {
  int col = lvl.cols[x];
  int i = 0;
  int y = 0;
  
  for (i = 0; i < 10; i++) {
    if (col == 0) {
      break;
    } else {
      y++;
      col = col >> 1;
    }
  }

  for (i = 0; i < 10; i++) {
    if (lvl.blocks[i] == x) {
      y++;
    }
  }

  return y;
}

// draw brick
void drawBrick(int x, int y, uint16_t color, uint16_t bgColor) {
  static short brick[12] = {0x000, 0xf9f, 0xf9f, 0xf9f, 0xf9f, 0x000,
			    0x000, 0x7fe, 0x7fe, 0x7fe, 0x7fe, 0x000};
  int r = 0;
  for (r = 0; r < 12; r++) {
    // 0x000
    int c = 0;
    for (c = 0; c < 12; c++) {
      if ((brick[r] >> c) & 0x1)
	f3d_lcd_drawPixel(y * 12 + 4 + r, x * 12 + 2 + c, color);
      else
	f3d_lcd_drawPixel(y * 12 + 4 + r, x * 12 + 2 + c, bgColor);
    }
  }
}

// draw block
void drawBlock(int x, int y, uint16_t color, uint16_t bgColor) {
  static short block[12] = {0xfff, 0x801, 0x801, 0x801, 0x801, 0x801,
			    0x801, 0x801, 0x801, 0x801, 0x801, 0xfff};
  int r = 0;
  for (r = 0; r < 12; r++) {
    // 0x000
    int c = 0;
    for (c = 0; c < 12; c++) {
      if ((block[r] >> c) & 0x1)
	f3d_lcd_drawPixel(y * 12 + 4 + r, x * 12 + 2 + c, color);
      else
	f3d_lcd_drawPixel(y * 12 + 4 + r, x * 12 + 2 + c, bgColor);
    }
  }
}


void drawDoor(int x, int y, uint16_t color, uint16_t bgColor) {
  static short door[24] = {0x7fe, 0x402, 0x402, 0x402, 0x402, 0x402, 
			   0x402, 0x402, 0x402, 0x402, 0x482, 0x5c2,
			   0x482, 0x402, 0x402, 0x402, 0x402, 0x402, 
			   0x402, 0x402, 0x402, 0x402, 0x402, 0x7fe};
  int r = 0;
  for (r = 0; r < 24; r++) {
    // 0x000
    int c = 0;
    for (c = 0; c < 12; c++) {
      if ((door[r] >> c) & 0x1)
	f3d_lcd_drawPixel(y * 12 + 4 + r, x * 12 + 2 + c, color);
      else
	f3d_lcd_drawPixel(y * 12 + 4 + r, x * 12 + 2 + c, bgColor);
    }
  }
}

// draw dude
void drawDude(Dude dude, uint16_t color, uint16_t capColor) {
  int x = dude.screenPos;
  int y = dude.y;
  int direction = dude.direction;
  
  static short leftDude[24] = {0x0e0, 0x0f0, 0x7f0, 0x7f0, 0x0f0, 0x1f8,
			       0x3fc, 0x3fc, 0x1f8, 0x0f0, 0x060, 0x060,
			       0x7fe, 0x7fe, 0x060, 0x060, 0x060, 0x060,
			       0x060, 0x0f0, 0x198, 0x30c, 0x606, 0xc03};
  static short rightDude[24] = {0x070, 0x0f0, 0x0fe, 0x0fe, 0x0f0, 0x1f8,
				0x3fc, 0x3fc, 0x1f8, 0x0f0, 0x060, 0x060,
				0x7fe, 0x7fe, 0x060, 0x060, 0x060, 0x060,
				0x060, 0x0f0, 0x198, 0x30c, 0x606, 0xc03};
  int r = 0;
  for (r = 0; r < 24; r++) {
    int c = 0;
    for (c = 0; c < 12; c++) {
      if (direction == RIGHT) {
	if ((leftDude[r] >> c) & 0x1) {
	  if (r < 4) {
	    f3d_lcd_drawPixel(y * 12 + 28 - r, x * 12 + 2 + c, capColor);
	  } else {
	    f3d_lcd_drawPixel(y * 12 + 28 - r, x * 12 + 2 + c, color);
	  }
	}
      } else {
	if ((rightDude[r] >> c) & 0x1) {
	  if (r < 4) {
	    f3d_lcd_drawPixel(y * 12 + 28 - r, x * 12 + 2 + c, capColor);
	  } else {
	    f3d_lcd_drawPixel(y * 12 + 28 - r, x * 12 + 2 + c, color);
	  }
	}
      }
    }
  }

  if (dude.hasBlock) {
    drawBlock(x, y+2, state.blockColor, state.bgColor);
  }
}

int count1s(int x) {
  int i = 0;
  int y = 0;
  
  for (i = 0; i < 10; i++) {
    if (x == 0) {
      break;
    } else {
      y++;
      x = x >> 1;
    }
  }
  return y;
}

// drawScreen
void initGraphics(Level lvl, Dude dude, State state) {
  int x = dude.x;
  int direction = dude.direction;
  // bricks
  int i = 0;
  for (i = 0; i < 13; i++) {
    int j = 0;
    for (j = 0; j < 10; j++) {
      if ((lvl.cols[state.left + i] >> j) & 0x1) {
	drawBrick(i, j, state.brickColor, state.bgColor);
      } else
	break;
      // check to see if we hit a 0, if so just continue
    }
  }
  // blocks
  for (i = 0; i < lvl.numBlocks; i++) {
    int blockX = lvl.blocks[i];
    int numBricks = count1s(lvl.cols[blockX]);
    int stackSize = getYPosnOfBlock(lvl, blockX) - numBricks;
    if (blockX >= state.left && blockX < state.left + 13) {
      int j;
      for  (j = 0; j < stackSize; j++) {
	drawBlock(blockX - state.left, numBricks + j, state.blockColor, state.bgColor);
      }
    }
  }
  // door
  int doorX = lvl.doorPos;
  if (doorX >= state.left && doorX < state.left + 13)
    drawDoor(doorX - state.left, getYPosnOfBlock(lvl, doorX), state.doorColor, state.bgColor);

  if (state.left == 0 || state.left == lvl.numCols - 13)
    drawDude(dude, state.dudeColor, state.capColor);
  // ceiling
  for (i = 0; i < 13; i++) {
    drawBrick(i, 9, state.brickColor, state.bgColor);
  }
}

void redrawScreen(Level lvl, State old, State new) {
  int i, j;
    
  // door
  int doorX = lvl.doorPos;
  int doorY = getYPosnOfBlock(lvl, doorX);
  if (doorX >= old.left && doorX < old.left + 13) {
    clearBlock(doorX - old.left, doorY);
    clearBlock(doorX - old.left, doorY + 1);
  }
  
  // blocks
  for (i = 0; i < lvl.numBlocks; i++) {  // loop through all the blocks in the level
    int blockX = lvl.blocks[i]; // 
    int numBricks = count1s(lvl.cols[blockX]); // count the number of bricks where the block is
    int stackSize = getYPosnOfBlock(lvl, blockX) - numBricks;   
    if (blockX >= old.left && blockX < old.left + 13) { // if block at i was on screen in the last frame
      int j;
      for (j = 0; j < stackSize; j++) {
	clearBlock(blockX - old.left, numBricks + j);
      }
    }
  }

  
  // bricks
  for (i = 0; i < 13; i++) {
    int diff = lvl.cols[old.left + i] ^ lvl.cols[new.left + i];
    if (!diff)
      continue;
    else {
      for (j = 1; j < 9; j++) {
	if (diff >> j == 0) // if the shift produces a zero we know that we have all the differences
	  break;
	else if (diff >> j & 1 == 0) {  // if bit at j is 0, then no difference, so do nothing
	  continue;
	} else {  // otherwise, if the new state has a brick at j, draw it, or clear it if not
	  if (lvl.cols[new.left + i] >> j & 1) {
	    drawBrick(i, j, new.brickColor, new.bgColor);
	  } else {
	    clearBlock(i, j);
	  }
	}
      }
    }
  }
  // blocks
  for (i = 0; i < lvl.numBlocks; i++) {  // loop through all the blocks in the level
    int blockX = lvl.blocks[i]; // 
    int numBricks = count1s(lvl.cols[blockX]); // count the number of bricks where the block is
    int stackSize = getYPosnOfBlock(lvl, blockX) - numBricks;  
    if (blockX >= new.left && blockX < new.left + 13) {
      int j;
      for (j = 0; j < stackSize; j++) {
	drawBlock(blockX - new.left, numBricks + j, new.blockColor, new.bgColor);
      }
    }
  }
  
  if (doorX >= new.left && doorX < new.left + 13) {
    drawDoor(doorX - new.left, doorY, new.doorColor, new.bgColor);
  }
}

//use this to update the screen at the end of every gameStep call
void updateScreen(int validMove, Level lvl, Dude dude, State old, State state) {
  //static char NOTHING = 0, BRICK = 1, BLOCK = 2, DOOR = 3, DUDE = 4;
  // using this to update the screen
  if (!validMove) {
    return;
  } else if (state.left  == old.left) {
    drawDude(dude, state.dudeColor, state.capColor);
  } else {
    redrawScreen(lvl, old, state);
    drawDude(dude, state.dudeColor, state.capColor);
  }
}


// eraseOldDude
void eraseOldDude(Dude player) {
  drawDude(player, BLACK, BLACK);
}

// clearBlock
void clearBlock(int x, int y) {
  drawBlock(x, y, BLACK, BLACK);
}
