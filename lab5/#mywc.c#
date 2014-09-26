/* my version of wc */
#include <stdio.h>

void main() {

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
