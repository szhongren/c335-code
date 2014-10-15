/* 
 * main.c
 *
 * Last Edited By: Zhongren Shao (shaoz) and Erin Leonhard (eeleonha)
 * Last Edited Date: 10/11/14
 *
 * Part of: C335 Lab 6
 * Purpose: Debug this function using gdb.
 */


/*
  The expected performace of the program is as follows:
  We want to print out the actual bits of numbers! What a great
  way to understand how number representation works.
 */

//The libraries to include
#include <f3d_uart.h>
#include <stdio.h> 
#include <stdint.h>
//The arrays that store the bits

// formats the bits of a char into a string format for printing
int gitBitsChar(unsigned char c, char answer[]){
  int i = 7;
  // convert to bits
  while(i > -1){
    if(c&1) answer[i] = '1';
    else answer[i] = '0';
    c=c>>1; // bit shift right, divide by 2
    i--;
  }
  // fill in end char for a string
  answer[8]=0;
  return 0;
}

// formats the bits of a short to string format for printing
int gitBitsShort(unsigned short s, char answer[]){
  int i = 15;
  // convert to bits
  while(i > -1){
    if(s&1) answer[i] = '1';
    else answer[i] = '0';
    s=s>>1; // bit shift right, divide by 2
    i--;
  }
  // fill in  end char for string
  answer[16]=0;
  return 0;
}

// formats the bits of an int to string format for printing
int gitBitsInt(int yikes, char answer[]){
  int i = 31;
  // convert to bits
  while(i > -1){
    if(yikes&1) answer[i] = '1';
    else answer[i] = '0';
    yikes=yikes>>1;
    i--;
  }
  answer[32]=0;
  return 0;
}

//runs some test on our program
int main(){
  f3d_uart_init();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  char ans1[33];
  char ans2[33];
  char ans3[33];
  char ans4[33];
  while (1){
    gitBitsShort((unsigned short) 3200, ans1);
    gitBitsChar((unsigned char) 'a', ans2);
    gitBitsInt(-94, ans3);
    gitBitsInt((unsigned int) 234, ans4);
    printf("unsigned short 3200= %-32s\n", ans1);
    printf("unsigned a = %-32s\n", ans2);
    printf("-94 = %-32s\n", ans3);
    printf("unsigned 234 = %-32s\n", ans4);
  }
 
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
