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

  f3d_uart_init();
  f3d_led_init();
  f3d_systick_init(100);
  f3d_usr_btn_init();
  
  int led = 8;

  while(1) {
    putchar(getchar());

    if(button_read()) {
      f3d_systick_init(10);
      while (1) {
	putchar(getchar());
	if (!button_read()) {
	  f3d_systick_init(100);
	  break;
	} else {
	  systick_delay(1);
	}
      }
    } else {
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
