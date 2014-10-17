/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>

#define TIMER 20000

// 128 * 160, 

void draw_verLine(uint8_t x, uint8_t y, uint16_t color) {
  int i = 0;
  for (i = y; i < ST7735_height; i++) {
    f3d_lcd_drawPixel(x, i, color);
  }
}
void draw_horLine(uint8_t y, uint16_t color) {
  int i = 0;
  for (i = 0; i < ST7735_width; i++) {
    f3d_lcd_drawPixel(i, y, color);
  }
}

void drawBar(float val, int axis, uint16_t color, int width) {
  int mid = 90;
  int flag;
  switch(axis) {
  case 0: // x
    flag = 21 - (width / 2);
    break;
  case 1: // y
    flag = 63 - (width / 2);
    break;
  case 2: // z
    flag = 105 - (width / 2);
    break;
  }

  int height = (int)val / 10;
  
  int x, y;

  if (height < 0) {
    for (y = mid; y < mid - height; y++) {
      for (x = flag; x < flag + width; x++) {
	f3d_lcd_drawPixel(x, y, color);
      }
    }
  } else {
    for (y = mid; y > mid - height; y--) {
      for (x = flag; x < flag + width; x++) {
	f3d_lcd_drawPixel(x, y, color);
      }
    }
  }
  delay(50);
}

void fill(void) { 
  f3d_lcd_fillScreen(BLACK);
  f3d_lcd_drawChar(21, 10, 'X', RED, BLACK);
  f3d_lcd_drawChar(63, 10, 'Y', RED, BLACK);
  f3d_lcd_drawChar(105, 10, 'Z', RED, BLACK);
  draw_horLine(20, CYAN);
  draw_horLine(90, CYAN);
  draw_verLine(42, 20, CYAN);
  draw_verLine(84, 20, CYAN);
}

void clearBars(void) {
  int x, y;
  for (y = 21; y < ST7735_height; y++) {
    for (x = 0; x < ST7735_width; x++) {
      if (x == 42 || x == 84 || y == 90) {
	continue;
      }
      f3d_lcd_drawPixel(x, y, BLACK);
    }
  }
}

int main(void) { 
  //Initialization functions here!
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_lcd_init();
  f3d_gyro_init();
  f3d_uart_init();

  float fArr[3];
  fill();
  while (1) {
    clearBars();
  
    f3d_gyro_getdata(fArr);
    int i = 0;
    for (i = 0; i < 3; i++) { 
      drawBar(fArr[i], i, RED, 16);
      // printf("%n, %f\n", i, fArr[i]);
    }
    

    delay();
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
