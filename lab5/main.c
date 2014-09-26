/* main.c --- 
 * 
 * Filename: main.c
 * Part of: Lab5
 * Description: runs word count program on STM32 with serialT
 * Author: Zhongren Shao (shaoz), Erin Leonhard (eeleonha) 
 * Created: 9/26/2014
 * Last-Updated: 9/26/2014
 *           By: Authors
 * 
 */

#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_uart.h>     // Pull in include file for the local drivers

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_uart_init();
  
  // does weird thing on multiple loops since it counts the enter from ending
  // the previous loop, and adds 1 to all of the counts
  
  while(1) {
    int c; /* current character */
    int numChars = 0;
    int numWords = 0;
    int numLines = 0;
    int notRepeatedWhitespace = 0; /* don't increment numWords for repeated whitespace */

    while ((c = getchar()) != 0x1b) {
      /* Increment char counter for each new c */
      numChars++;
      if (c == ' ' || c == '\t' ||  c == '\r' || c == '\n' || c == '\f' || c == '\v') {

	/* Increment word counter for each group of whitespace chars */
	if (c == ' ' || c == '\t') {
	  if (notRepeatedWhitespace) { 
	    numWords++;
	    notRepeatedWhitespace = 0;
	  }
	}
	/* Increment line counter for each newline char */
	else  {
	  numLines++;
	  numWords++;
	  notRepeatedWhitespace = 0;
	}

      }
      else {
	notRepeatedWhitespace = 1;
      }
    }
    printf("%d %d %d\n", numLines, numWords, numChars);
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
