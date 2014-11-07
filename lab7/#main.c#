/*
 * main.c
 *
 * Last Edited By: Zhongren Shao (shaoz) and Erin Leonhard (eeleonha)
 * Last Edited Date: 10/17/14
 *
 * Part of: C335 Lab 7
 * Task: Develop an application for the gyroscope that uses the LCD screen to
 * display live data from the gyroscope as output. 
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>

#define TIMER 20000

// size of the LCD is 128 * 160, 

// function that draws a vertical line all the way down that statrts from a
// given Y index
void draw_verLine(uint8_t x, uint8_t y, uint16_t color) {
  int i = 0;
  for (i = y; i < ST7735_height; i++) {
    f3d_lcd_drawPixel(x, i, color);
  }
}

// function that draws a horizontal line across the entire screen at a given Y
// index
void draw_horLine(uint8_t y, uint16_t color) {
  int i = 0;
  for (i = 0; i < ST7735_width; i++) {
    f3d_lcd_drawPixel(i, y, color);
  }
}

// function that draws a bar representing angular velocity for a given axis and
// value from the gyroscope. Also takes arguments for the color and the width
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

  // this delay is needed for it to work for some reason
  delay(50);
}

// draws the static background of the application display
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

// clears the bars that represent the angular velocities so that they can be
// updated
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

  // arr ofr gyro readings
  float fArr[3];
  // setup 'background'
  fill();
  while (1) {
    // clear bars so the display can update
    clearBars();
    // get gyro readings
    f3d_gyro_getdata(fArr);
    int i = 0;
    for (i = 0; i < 3; i++) {
      // draws the bars for each axis, with width of 16
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
