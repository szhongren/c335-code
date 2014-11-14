/*
 * main.c
 *
 * Part Of: C335 Lab 10
 * Authors: Erin Leonhard (eeleonha), Zhongren Shao (shaoz)
 * Created: 11/7/14
 * Last Edited: 11/14/14
 *
 * The definitions below handle drawing images from an SD card that can be 
 * flipped in orientation of display and selected using a nunchuk.
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_delay.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE Buff[128];		/* File read buffer */

// converts accel raw data to radians
void accel_rawdata_to_radians(float accel_data[], float rads[]) {
  rads[0] = atan(accel_data[0]/(sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2))));
  rads[1] = atan(accel_data[1]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2))));
  rads[2] = atan(accel_data[2]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[1], 2))));
}

// handles changing between different visualizations
int change_mode(int *mode, int change) {
  if (*mode == 2) {
    if (change == 1) {
      *mode = 0;
    } else {
      *mode += change;
    }
  } else if (*mode == 0) {
    if (change == -1) {
      *mode = 2;
    } else {
      *mode += change;
    }
  } else {
    *mode += change;
  }
}

int find_quadrant(float accel_rads[]) {
  int x = accel_rads[0];
  int y = accel_rads[1];
  if (abs(x) > abs(y)) {
    if (x > 0) {
      return 2;
    } else {
      return 0;
    }
  } else {
    if (y > 0) {
      return 1;
    } else {
      return 3;
    }
  }
}
int main(void) { 
  char footer[20];
  int count=0;
  int i;

  FRESULT rc;			/* Result code */
  DIR dir;			   	  /* Directory object */
  FILINFO fno;				     	       /* File information object */
  UINT bw, br;
  unsigned int retval;

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_lcd_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_i2c1_init();
  
  // f3d_nunchuk_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  
  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */
  nunchuk_t nun_data;
  int mode = 0;
  // flag to keep track if button has been pressed already
  int FLAG_btn_pressed = 0;
  char *name = "POKE1.BMP";
  // rc = f_open(&Fil, "POKE1.BMP", FA_READ);
  while (1) {
    int change;
    /* f3d_nunchuk_read(&nun_data); */
    /* change = f3d_nunchuk_change_mode(&nun_data); */
    /* if (change) { */
    /*   if (!FLAG_btn_pressed) { */
    /*   change_mode(&mode, change); */
    /*   FLAG_btn_pressed = 1; */
    /*   } */
    /* } else { */
    /*   FLAG_btn_pressed = 0; */
    /* } */

    switch(mode) {
    case 0:
      rc = f_open(&Fil, "POKE1.BMP", FA_READ);
      printf("mode 1\n");
      break;
    case 1:
      rc = f_open(&Fil, "POKE2.BMP", FA_READ);
      printf("mode 2\n");
      break;
    case 2:
      rc = f_open(&Fil, "POKE3.BMP", FA_READ);
      printf("mode 3\n");
      break;
    }
    
    float accel_data[3];
    float accel_rads[3];
    f3d_accel_read(accel_data);  
    accel_rawdata_to_radians(accel_data, accel_rads);
    int direction = find_quadrant(accel_rads);
    draw_pic(&Fil, direction, &br);
    f_close(&Fil);
    
    
    
    
    // ********************************************
    // ***************** CHANGE *******************
    // ********************************************
    while (1) {
      float new_accel_data[3];
      float new_accel_rads[3];
      f3d_accel_read(new_accel_data);  
      accel_rawdata_to_radians(new_accel_data, new_accel_rads);
      int new_direction = find_quadrant(new_accel_rads);
      // f3d_nunchuk_read(&nun_data);
      // change = f3d_nunchuk_change_mode(&nun_data);
      if (new_direction != direction/* || change*/) {
        break;
      }
    }
    // ********************************************
    // ********************************************
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
