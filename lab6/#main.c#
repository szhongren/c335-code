/*
 * main.c
 *
 * Last Edited By: Zhongren Shao (shaoz) and Erin Leonhard (eeleonha)
 * Last Edited Date: 10/3/14
 *
 * Part of: C335 Lab 6 
 * Task: Develop an application for the gyroscope that allows a user to select a measurement
 * axis and then will output the current axis on the console along with the measured data
 * while displaying the magnitude of each velocity on the LED displays. 
 */

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
  // boolean: is positive velocity?
  int isPos = 1;
  // LEDs that will light for positive velocity
  int posLED[5] = {9, 8, 15, 14, 13};
  // LEDs that will light for negative velocity
  int negLED[5] = {9, 10, 11, 12, 13};
  
  // determines if velocity is negative
  if (deg < 0) {
    deg = -deg;
    isPos = 0;
  } 
  
  // n determines how many LEDs should light, based on magnitude of velocity
  int n;
  // for deg <= 5, don't light any LEDs
  if (deg <= 5)
    n = 0;
  // for deg > 5, light one LED
  if (deg > 5)
    n = 1;
  // for deg > 100, light two LEDs
  if (deg > 100)
    n = 2;
  // for deg > 200, light three LEDs
  if (deg > 200)
    n = 3;
  // for deg > 300, light four LEDs
  if (deg > 300)
    n = 4;
  // for deg > 400, light five LEDs (one whole side)
  if (deg > 400)
    n = 5;
  
  // point to relevant array of LEDs
  // as determined by direction of velocity (+ or -)
  int *LED;
  if (isPos) {
    LED = posLED;
  } else {
    LED = negLED;
  }
  
  // light the correct number of LEDs from the arrays
  // as determined by the magnitude of the velocity
  int i = 0;
  for (i = 0; i < n; i++) {
    f3d_led_on(LED[i]);
  }
}

// initialize the LEDs, user button, UART, and gyroscope
int main(void) { 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_led_init();
  f3d_usr_btn_init();
  f3d_uart_init();
  f3d_gyro_init();
  
  // array for three axes
  float fArr[3];
  int identifier = 0;
  char axis = 'x';
  
  // read data and user input as given from command line and user button
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
    
    // set index for fArr based on selected axis
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
    // passes current axis to LED_control
    LED_control(fArr[identifier]);
    
    printf("%c|%f\n", axis, fArr[identifier]);
    printf("--------------------------------------------------------------------------------\n");
    delay();
    // turn off all LEDs that were lit
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
