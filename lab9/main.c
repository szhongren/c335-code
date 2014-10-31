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
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_usr_btn.h>
#include <f3d_uart.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_delay.h>
#include <f3d_mag.h>
#include <f3d_lcd_sd.h>
#include <f3d_nunchuk.h>
#include <f3d_gyro.h>
#include <stdio.h>
#include <math.h>

// converts accel raw data to radians
void accel_rawdata_to_radians(float accel_data[], float rads[]) {
  rads[0] = atan(accel_data[0]/(sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2))));
  rads[1] = atan(accel_data[1]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2))));
  rads[2] = atan(accel_data[2]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[1], 2))));
}

// converts mag raw data to heading and offset readings for f3d_lcd_placeDotOnCircle
void mag_rawdata_to_heading(float accel_data[], float mag_data[], float *heading, float *offset) {
  float pitch = atan(accel_data[0]/(sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2))));
  float roll = atan(accel_data[1]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2))));
  double x_h = mag_data[0] * cos(pitch) + mag_data[2] * sin(pitch);
  double y_h = mag_data[0] * sin(roll) * sin(pitch) + mag_data[1] * cos(roll) - mag_data[2] * sin(roll) * cos(pitch);
  if (x_h > 0) {
    *offset = M_PI/2;
  } else {
    *offset = -M_PI/2;
  }
  *heading = atan(y_h/x_h);
}

void nun_rawdata_to_radians(nunchuk_t *nun_data, float rads[]) {
  float ax = nun_data->ax - 512;
  float ay = nun_data->ay - 512;
  float az = nun_data->az - 512;
  rads[0] = atan(ax/(sqrt(pow(ay, 2) + pow(az, 2))));
  rads[1] = atan(ay/(sqrt(pow(ax, 2) + pow(az, 2))));
  rads[2] = atan(az/(sqrt(pow(ax, 2) + pow(ay, 2))));
}

// paints static parts of gyro visual
void gyro_visualization() {
  char *mode = "GYRO";
  f3d_lcd_drawString(10, 0, mode, WHITE, BLACK);
  f3d_lcd_drawChar(19, 10, 'X', RED, BLACK);
  f3d_lcd_drawChar(61, 10, 'Y', RED, BLACK);
  f3d_lcd_drawChar(103, 10, 'Z', RED, BLACK);
  f3d_lcd_drawLine(0, 20, 128, 20, CYAN);
  f3d_lcd_drawLine(0, 90, 128, 90, CYAN);
  f3d_lcd_drawLine(42, 20, 42, 160, CYAN);
  f3d_lcd_drawLine(84, 20, 84, 160, CYAN);
}

// paints static parts of accel visual
void accel_visualization() { 
  char *mode = "ACCEL";
  f3d_lcd_drawString(10, 0, mode, WHITE, BLACK);
  f3d_lcd_drawCircle(15, 64, 26, RED, 1);
  f3d_lcd_drawCircle(15, 64, 79, GREEN, 1);
  f3d_lcd_drawCircle(15, 64, 132, BLUE, 1);
  f3d_lcd_drawChar(62, 23, 'X', WHITE, RED);
  f3d_lcd_drawChar(62, 76, 'Y', WHITE, GREEN);
  f3d_lcd_drawChar(62, 129, 'Z', WHITE, BLUE);
}

// paints static parts of mag visual
void compass_visualization() {
  char *mode = "COMPASS";
  f3d_lcd_drawString(10, 0, mode, WHITE, BLACK);
  f3d_lcd_drawCircle(60, 64, 80, RED, 0);
  f3d_lcd_drawCircle(2, 64, 80, RED, 1);
  f3d_lcd_drawChar(62, 25, 'N', RED, BLACK);
  f3d_lcd_drawChar(62, 130,'S', RED, BLACK);
  f3d_lcd_drawChar(117, 78,'E', RED, BLACK);
  f3d_lcd_drawChar(8, 78, 'W', RED, BLACK);
}

void nun_accel_visualization() { 
  char *mode = "NUNCHUK";
  f3d_lcd_drawString(10, 0, mode, WHITE, BLACK);
  char *mode2 = "ACCEL";
  f3d_lcd_drawString(10, 9, mode2, WHITE, BLACK);

  f3d_lcd_drawCircle(15, 64, 26, RED, 1);
  f3d_lcd_drawCircle(15, 64, 79, GREEN, 1);
  f3d_lcd_drawCircle(15, 64, 132, BLUE, 1);
  f3d_lcd_drawChar(62, 23, 'X', WHITE, RED);
  f3d_lcd_drawChar(62, 76, 'Y', WHITE, GREEN);
  f3d_lcd_drawChar(62, 129, 'Z', WHITE, BLUE);
}

int change_mode(int *mode, int change) {
  if (*mode == 3) {
    if (change == 1) {
      *mode = 0;
    } else {
      *mode += change;
    }
  } else if (*mode == 0) {
    if (change == -1) {
      *mode = 3;
    } else {
      *mode += change;
    }
  } else {
    *mode += change;
  }
}

int main(void) { 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  // inits
  f3d_usr_btn_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_gyro_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  
  nunchuk_t nun_data;
  int mode = 0;
  int FLAG_btn_pressed = 0;
  f3d_lcd_fillScreen(BLACK);
  gyro_visualization();
  while (1) {
    f3d_nunchuk_read(&nun_data);
    
    int change = f3d_nunchuk_change_mode(&nun_data);
    if (change) {
      if (!FLAG_btn_pressed) {	
	f3d_lcd_fillScreen(BLACK);
	printf("CHANGING from %d ", mode);
	change_mode(&mode, change);
	printf("to %d.\n", mode);
	switch(mode) {
	case 0:
	  gyro_visualization();
	  break;
	case 1:
	  accel_visualization();
	  break;
	case 2:
	  compass_visualization();
	  break;
	case 3:
	  nun_accel_visualization();
	  break;
	}
	FLAG_btn_pressed = 1;
      }
    } else {
      FLAG_btn_pressed = 0;
    }
    
    float fArr[3];
    float accel_data[3];
    float accel_rads[3];
    float mag_data[3];
    float nun_rads[3];
    
    switch(mode) {
    case 0:
      // get gyro readings
      f3d_gyro_getdata(fArr);
      int i = 0;
      int centerBars = 21;
      int nextBar = 42;
      int height;
      int width = 20;	
      for (i = 0; i < 3; i++) {
	height = (int)fArr[i]/10;
      	// draws the bars for each axis, with width of 20
      	if (height < 0) {
      	  f3d_lcd_drawRect(centerBars - width / 2, 90, RED, -height, width, 1);
      	} else {
      	  f3d_lcd_drawRect(centerBars - width / 2, 90 - height, RED, height, width, 1);
      	}
      	centerBars += nextBar;
      	// printf("%n, %f\n", i, fArr[i]);
      }
      delay(200);
      centerBars = 21;
      for (i = 0; i < 3; i++) {
	height = (int)fArr[i]/10;
	if (height < 0) {
      	  f3d_lcd_drawRect(centerBars - width / 2, 91, BLACK, -height + 1, width, 1);
      	} else {
      	  f3d_lcd_drawRect(centerBars - width / 2, 90 - height - 1, BLACK, height, width, 1);
      	}
	centerBars += nextBar;
      }
      break;
    case 1:
      f3d_accel_read(accel_data);    
      accel_rawdata_to_radians(accel_data, accel_rads);
      f3d_lcd_drawSemicircle(21, 64, 26, RED, &accel_rads[0]);
      f3d_lcd_drawSemicircle(21, 64, 79, GREEN, &accel_rads[1]);
      f3d_lcd_drawSemicircle(21, 64, 132, BLUE, &accel_rads[2]);
      delay(200);
      // erase semicircles
      f3d_lcd_drawSemicircle(21, 64, 26, BLACK, &accel_rads[0]);  
      f3d_lcd_drawSemicircle(21, 64, 79, BLACK, &accel_rads[1]); 
      f3d_lcd_drawSemicircle(21, 64, 132, BLACK, &accel_rads[2]);
      break;
    case 2:
      f3d_accel_read(accel_data);
      f3d_mag_read(mag_data);
      float heading;
      float offset;
      mag_rawdata_to_heading(accel_data, mag_data, &heading, &offset);
      f3d_lcd_placeDotOnCircle(30, 64, 80, WHITE, &heading, &offset);
      delay(200);
      // erase dot
      f3d_lcd_placeDotOnCircle(30, 64, 80, BLACK, &heading, &offset);
      break;
    case 3:
      nun_rawdata_to_radians(&nun_data, nun_rads);
      f3d_lcd_drawSemicircle2(21, 64, 26, RED, &nun_rads[0]);
      f3d_lcd_drawSemicircle2(21, 64, 79, GREEN, &nun_rads[1]);
      f3d_lcd_drawSemicircle2(21, 64, 132, BLUE, &nun_rads[2]);
      delay(200);
      // erase semicircles
      f3d_lcd_drawSemicircle2(21, 64, 26, BLACK, &nun_rads[0]);  
      f3d_lcd_drawSemicircle2(21, 64, 79, BLACK, &nun_rads[1]); 
      f3d_lcd_drawSemicircle2(21, 64, 132, BLACK, &nun_rads[2]);
      break;
    }
    delay(100);
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
