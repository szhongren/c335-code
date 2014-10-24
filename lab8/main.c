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
#include <f3d_uart.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_delay.h>
#include <f3d_mag.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <math.h>

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

  printf("X: ");
  float pitch = atan(accel_data[0]/(sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2))));
  printf("%fdeg\n", pitch * 180 / M_PI);

  printf("Y: ");
  float roll = atan(accel_data[1]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2))));
  printf("%fdeg\n", roll * 180 / M_PI);

  printf("Z: ");
  float yaw = atan(accel_data[2]/(sqrt(pow(accel_data[0], 2) + pow(accel_data[1], 2))));
  printf("%fdeg\n", yaw * 180 / M_PI);
    
  double x_h = mag_data[0] * cos(pitch) + mag_data[2] * sin(pitch);
  double y_h = mag_data[0] * sin(roll) * sin(pitch) + mag_data[1] * cos(roll) - mag_data[2] * sin(roll) * cos(pitch);
  double heading = atan(y_h/x_h);
  printf("%f\n", heading);
  delay(2000);
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
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
  float accel_data[3];
  float mag_data[3];
  f3d_lcd_fillScreen(BLACK);
  
  while (1) {
    f3d_accel_read(accel_data);
    f3d_mag_read(mag_data);
    // display_raw_data(accel_data, mag_data);
    f3d_lcd_drawCircle(10, 64, 80, RED, 0);
    f3d_lcd_drawSemicircle(21, 64, 80, CYAN, 10);
    f3d_lcd_drawSemicircle(21, 64, 80, CYAN, -10);
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
