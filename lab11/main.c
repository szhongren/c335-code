/*
 * main.c
 *
 * Part of: C335 Lab 11
 *
 * Authors: Erin Leonhard (eeleonha) and Zhongren Shao (shaoz)
 * Created Date: 11/14/14
 * Last Edited Date: 11/14/14
 *
 * The definitions below implement an LED light sequence and a circular queue
 * echo program using Systick interrupts.
 */


#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_usr_btn.h>
#include <f3d_systick.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

extern int flag;

int next_led(int curr_led) {
  if (curr_led == 15) 
    return 8;
  else
    return curr_led + 1;
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  // initialize everything
  f3d_uart_init();
  f3d_led_init();
  f3d_systick_init(100);
  f3d_usr_btn_init();
  
  int led = 8;

  while(1) {
    // echo program
    putchar(getchar());
    
    // if button pressed, slow down interrupts
    if(button_read()) {
      // 10 interrupts per second
      f3d_systick_init(10);
      while (1) {
	// echo
	putchar(getchar());
	// 100 interrupts per second
	if (!button_read()) {
	  f3d_systick_init(100);
	  break;
	} else {
	  // delay
	  systick_delay(1);
	}
      }
    } else {
      // delay
      systick_delay(1);
    }
    
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif
