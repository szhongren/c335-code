/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
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

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_usr_btn.h>

volatile int systick_flag = 0;
volatile int flag = 0; 

static __IO uint32_t TimingDelay;

void f3d_systick_init(int inter_per_sec) {
  SysTick_Config(SystemCoreClock/inter_per_sec);
}

void systick_delay(int time) {
    TimingDelay = time;
    while (TimingDelay != 0);
}

void SysTick_Handler(void) {
  if (TimingDelay != 0x00)
    TimingDelay--;
  if (!queue_empty(&txbuf)) {
    flush_uart();
  }
}

/* f3d_systick.c ends here */
