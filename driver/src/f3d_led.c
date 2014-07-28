/* led.c --- 
 * 
 * Filename: led.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 10:53:06 2013
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
#include <stm32f30x.h>
#include <f3d_led.h>

void f3d_led_init(void) {
  // initialize the port and pins for the 8 leds
}

void f3d_led_on(int led) {
  // enable the led specified by led parameter
  // led = 0-7 representing the 8 leds
}
 
void f3d_led_off(int led) {
  // disable the led specified by led parameter
  // led = 0-7 representing the 8 leds
} 

void f3d_led_all_on(void) {
  // turn on all 8 leds 
} 

void f3d_led_all_off(void) {
  // turn off all 8 leds
} 

/* led.c ends here */

