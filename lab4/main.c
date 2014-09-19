/* Names: Erin Leonhard (eeleonha) Zhongren Shao (shaoz)
 * 
 * Filename: main.c
 * Part of: Lab4
 * Date Created: 9/19/14
 * Last Modified: 9/19/14
 *
 *
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_usr_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

// makes lights go in circle, pause when button pressed
int main(void) {
  // calls function from f3d_led.c
  f3d_led_init();
  // function from f3d_usr_btn.c
  f3d_usr_btn_init();
  
  f3d_led_all_off();
  int i = 8;
  for (i = 8; i < 16; i++) {
    f3d_led_on(i);
    delay();
    if (button_read()) {
      while(button_read()){
	delay();
      }
    }
    f3d_led_off(i);
  }
  f3d_led_all_on();
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
