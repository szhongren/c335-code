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
  int x = dude.x;
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
}

void drawScreen(Level lvl, Dude dude) {
  int x = dude.x;
  int direction = dude.direction;

  int i = 0;
  for (i = 0; i < 13; i++) {
    int j = 0;
    for (j = 0; j < 10; j++) {
      if ((lvl.cols[i] >> j) & 0x1) {
	drawBrick(i, j, GREEN, BLACK);
      } else
	break;
      // check to see if we hit a 0, if so just continue
    }
  }
}
