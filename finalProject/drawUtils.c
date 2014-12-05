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

// draw brick
void drawBrick(int x, int y, uint16_t color, uint16_t bgColor) {
  static short brick[12] = {0x000, 0xf9f, 0xf9f, 0xf9f, 0xf9f, 0x000, 
		     0x000, 0x7fe, 0x7fe, 0x7fe, 0x7fe, 0x000}; 
  int r = 0;
  for (r = 0; r < 12; r++) {
    // 0x000
    int c = 0;
    for (c = 0; c < 12; c++) {
      if ((brick[c] >> r) & 0x1)
	f3d_lcd_drawPixel(x + c, y + r, color);
      else
	f3d_lcd_drawPixel(x + c, y + r, bgColor);
    }
  }
}

// draw block


// draw dude
void drawDude(int x, int y, int direction, uint16_t color, uint16_t capColor) {
  static short leftDude[24] = {0x0e0, 0x0f0, 0x7f0, 0x7f0, 0x0f0, 0x1f8, 
			       0x3fc, 0x3fc, 0x1f8, 0x0f0, 0x060, 0x060, 
			       0x7fe, 0x7fe, 0x060, 0x060, 0x060, 0x060, 
			       0x060, 0x0f0, 0x198, 0x30c, 0x606, 0xc03};
  static short rightDude[24] = {0x070, 0x0f0, 0x0fe, 0x0fe, 0x0f0, 0x1f8, 
				0x3fc, 0x3fc, 0x1f8, 0x0f0, 0x060, 0x060, 
				0x7fe, 0x7fe, 0x060, 0x060, 0x060, 0x060, 
				0x060, 0x0f0, 0x198, 0x30c, 0x606, 0xc03};
  int r = 0;
  for (r = 0; r < 12; r++) {
    int c = 0;
    for (c = 0; c < 24; c++) {
      if ((rightDude[c] >> r) & 0x1) {
	if (c < 4) {
	  f3d_lcd_drawPixel(x + c, y + r, capColor);
	} else {
	  f3d_lcd_drawPixel(x + c, y + r, color);
	}
      }  
    }  
  }
}
