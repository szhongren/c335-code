/* main.c
 * 
 * Part Of: C335 Lab 8
 *
 * Created By: Zhongren Shao (shaoz) and Erin Leonhard (eeleonha)
 * Created Date: 10/24/14
 * Last Edited By: Zhongren Shao (shaoz) and Erin Leonhard (eeleonha)
 * Last Edited On: 10/28/14
 *
 * Below are the functions that measure data from the accelerometer and magnetometer
 * and provide visualizations for these measurements on the LCD screen.
*/

/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_uart.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_delay.h>
#include <f3d_mag.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <math.h>

// prints raw data ts serialT
void display_raw_data(float accel_data[], float mag_data[]) {
  int i = 0;
  char axis[] = {'x', 'y', 'z'};
    
  printf("Accelerometer\n");
  for (i = 0; i < 3; i++) {
    printf("%c: %f\n", axis[i], accel_data[i]);
  }
  printf("Magnetometer\n");
  for (i = 0; i < 3; i++) {
    printf("%c: %f\n", axis[i], mag_data[i]);
  }

  float pitch = atan(accel_data[0]/(sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2))));
  printf("X: %fdeg\n", pitch * 180 / M_PI);

  float roll = atan(accel_data[1]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2))));
  printf("Y: %fdeg\n", roll * 180 / M_PI);

  float yaw = atan(accel_data[2]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[1], 2))));
  printf("Z: %fdeg\n", yaw * 180 / M_PI);
    
  double x_h = mag_data[0] * cos(pitch) + mag_data[2] * sin(pitch);
  double y_h = mag_data[0] * sin(roll) * sin(pitch) + mag_data[1] * cos(roll) - mag_data[2] * sin(roll) * cos(pitch);
  double heading = atan(y_h/x_h);
  printf("x_h: %f, y_h: %f, Heading: %f\n", x_h, y_h, heading);
  delay(2000);
}

// converts accel raw data to radians
void accel_rawdata_to_radians(float accel_data[], float rads[]) {
  rads[0] = atan(accel_data[0]/(sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2))));
  rads[1] = atan(accel_data[1]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2))));
  rads[2] = atan(accel_data[2]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[1], 2))));
}

// converts mag raw data to heading and offset readings for f3d_lcd_placeDotOnCircle
float mag_rawdata_to_heading(float accel_data[], float mag_data[], float *heading, float *offset) {
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

// paints static parts of accel visual
void accel_visualization() { 
  f3d_lcd_drawCircle(15, 64, 26, RED, 1);
  f3d_lcd_drawCircle(15, 64, 79, GREEN, 1);
  f3d_lcd_drawCircle(15, 64, 132, BLUE, 1);
  f3d_lcd_drawChar(62, 23, 'X', WHITE, RED);
  f3d_lcd_drawChar(62, 76, 'Y', WHITE, GREEN);
  f3d_lcd_drawChar(62, 129, 'Z', WHITE, BLUE);
}

// paints static parts of mag visual
void compass_visualization() {
  f3d_lcd_drawCircle(60, 64, 80, RED, 0);
  f3d_lcd_drawCircle(2, 64, 80, RED, 1);
  f3d_lcd_drawChar(62, 25, 'N', RED, BLACK);
  f3d_lcd_drawChar(62, 130,'S', RED, BLACK);
  f3d_lcd_drawChar(117, 78,'E', RED, BLACK);
  f3d_lcd_drawChar(8, 78, 'W', RED, BLACK);
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
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  
  // arrays for storing raw data
  float accel_data[3];
  float mag_data[3];
  // array for accel in rads
  float accel_rads[3];
  // button things to switch between modes  
  int mode = 0;
  int FLAG_btn_pressed = 0;
  
  f3d_lcd_fillScreen(BLACK);
  accel_visualization();
  
  while (1) {
    // visualization switcher
    if(button_read()) {
      if (!FLAG_btn_pressed) {	
	f3d_lcd_fillScreen(BLACK);
	if (mode == 0) {
	  mode = 1;
	  compass_visualization();
	} else {
	  mode = 0;
	  accel_visualization();
	}
	FLAG_btn_pressed = 1;
      }
    } else {
      FLAG_btn_pressed = 0;
    }
    
    if (mode == 0) { // accelerometer mode
      f3d_accel_read(accel_data);    
      // display_raw_data(accel_data, mag_data);
      accel_rawdata_to_radians(accel_data, accel_rads);
      f3d_lcd_drawSemicircle(21, 64, 26, RED, &accel_rads[0]);
      f3d_lcd_drawSemicircle(21, 64, 79, GREEN, &accel_rads[1]);
      f3d_lcd_drawSemicircle(21, 64, 132, BLUE, &accel_rads[2]);
      delay(200);
      // erase semicircles
      f3d_lcd_drawSemicircle(21, 64, 26, BLACK, &accel_rads[0]);  
      f3d_lcd_drawSemicircle(21, 64, 79, BLACK, &accel_rads[1]); 
      f3d_lcd_drawSemicircle(21, 64, 132, BLACK, &accel_rads[2]);
    } else {// compass mode
      f3d_mag_read(mag_data);
      // display_raw_data(accel_data, mag_data);
      float heading;
      float offset;
      mag_rawdata_to_heading(accel_data, mag_data, &heading, &offset);
      f3d_lcd_placeDotOnCircle(30, 64, 80, WHITE, &heading, &offset);
      delay(200);
      // erase dot
      f3d_lcd_placeDotOnCircle(30, 64, 80, BLACK, &heading, &offset);
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


/* main.c ends here */
