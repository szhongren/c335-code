/* f3d_nunchuk.c --- 
 * 
 * Filename: f3d_nunchuk.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct 31 09:40:37 2013
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

#include <f3d_i2c.h>
#include <f3d_nunchuk.h>
#include <stdio.h>

#define NUNCHUK_ADDRESS 0xA4

#define NUN_X_MID 140
#define NUN_Y_MID 137

void f3d_nunchuk_init(void) {
  uint8_t realb = 0x00;
  uint8_t buf = 0x55;
  uint8_t buf2 = 0x00;


  // Use for factory Wii Nunchuk controllers
  // f3d_i2c1_write(NUNCHUK_ADDRESS,0x40,&realb);
  // Use for 3 party nunchuks like we have in the lab
  f3d_i2c1_write(NUNCHUK_ADDRESS,0xf0,&buf); 
  f3d_i2c1_write(NUNCHUK_ADDRESS,0xfb,&buf2);
}

void f3d_nunchuk_read(nunchuk_t *n) {
  uint8_t data[6] = {0,0,0,0,0,0};
  int i;
  unsigned short tmp;

  f3d_i2c1_read_nunchuk(0xA5, data, 6);
  n->jx = data[0];
  n->jy = data[1];

  tmp = data[2];
  tmp = tmp << 2;
  n->ax = tmp | ((data[5] & 0x0C) >> 2);

  tmp = data[3];
  tmp = tmp << 2;
  n->ay = tmp | ((data[5] & 0x30) >> 4);

  tmp = data[4];
  tmp = tmp << 2;
  n->az = tmp | ((data[5] & 0xC0) >> 6);

  switch (data[5] & 0x03) {
  case 2:
    n->z = 1;
    n->c = 0;
    break;
  case 1:
    n->z = 0;
    n->c = 1;
    break;
  case 0:
    n->z = 1;
    n->c = 1;
    break;
  case 3:
    n->z = 0;
    n->c = 0;
    break;
  }
  //  printf("n->c=%d n->z=%d n->jx=%d n->jy=%d n->ax=%d n->ay=%d n->az=%d\n",n->c,n->z,n->jx,n->jy,n->ax,n->ay,n->az);
}
void f3d_nunchuk_display_data(nunchuk_t *nun_data) {
  printf("jx: %u\n", nun_data->jx);
  printf("jy: %u\n", nun_data->jy);
  printf("ax: %u\n", nun_data->ax);
  printf("ay: %u\n", nun_data->ay);
  printf("az: %u\n", nun_data->az);
  printf("z: %u\n", nun_data->z);
  printf("c: %u\n", nun_data->c);
}

int f3d_c_pressed(nunchuk_t *nun_data) {
  return nun_data->c;
}

int f3d_z_pressed(nunchuk_t *nun_data) {
  return nun_data->z;
}

int f3d_j_right(nunchuk_t *nun_data) {
  return nun_data->jx > NUN_X_MID + 70;
}

int f3d_j_left(nunchuk_t *nun_data) {
  return nun_data->jx < NUN_X_MID - 70;
}

int f3d_nunchuk_change_mode(nunchuk_t *nun_data) {
  if (f3d_c_pressed(nun_data) || f3d_j_right(nun_data))
    return 1;
  else if (f3d_z_pressed(nun_data) || f3d_j_left(nun_data))
    return -1;
  else
    return 0;
}
/* f3d_nunchuk.c ends here */
