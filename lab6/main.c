/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */
//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_usr_btn.h>

void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

void LED_control(float deg) {
  int isPos = 1;
  int posLED[5] = {9, 8, 15, 14, 13};
  int negLED[5] = {9, 10, 11, 12, 13};

  if (deg < 0) {
    deg = -deg;
    isPos = 0;
  } 
  
  int n;
  if (deg <= 5)
    n = 0;
  if (deg > 5)
    n = 1;
  if (deg > 100)
    n = 2;
  if (deg > 200)
    n = 3;
  if (deg > 300)
    n = 4;
  if (deg > 400)
    n = 5;
  
  int *LED;
  if (isPos) {
    LED = posLED;
  } else {
    LED = negLED;
  }
  
  int i = 0;
  for (i = 0; i < n; i++) {
    f3d_led_on(LED[i]);
  }
}

int main(void) { 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_led_init();
  f3d_usr_btn_init();
  f3d_uart_init();
  f3d_gyro_init();
  
  float fArr[3];
  int identifier = 0;
  char axis = 'x';
  
  while(1) {
    f3d_gyro_getdata(fArr);

    if (button_read()) {
      switch (axis) {
      case 'x':
	axis = 'y';
	break;
      case 'y':
	axis = 'z';
	break;
      case 'z':
	axis = 'x';
	break;
      }
    } else if (getchar()) {
      axis = getchar();
      setvbuf(stdin, NULL, _IONBF, 0);
      setvbuf(stdout, NULL, _IONBF, 0);
      setvbuf(stderr, NULL, _IONBF, 0);
      f3d_uart_init();
    }
    
    
    switch(axis) {
    case 'x':
      identifier = 0;
      break;
    case 'y':
      identifier = 1;
      break;
    case 'z':
      identifier = 2;
      break;
    }
    LED_control(fArr[identifier]);
    
    printf("%c|%f\n", axis, fArr[identifier]);
    printf("--------------------------------------------------------------------------------\n");
    delay();
    f3d_led_all_off();
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */

// Part 2.2 Assertions
/*
  printf("Assertion Raised at line %d in file %s\n",line,file);
*/
  while (1);
}
#endif

/* main.c ends here */
