/* 
 * Filename: structures.h
 *
 * Part of: C335 Final Project
 * Created: 12.7.14
 * Last-Updated: Erin Leonhard (eeleonha) &
 *           By: Zhongren Shao (shaoz)
 *           On: 12.7.14
 */

/* Code: */

#define LEFT 0
#define RIGHT 1

typedef struct dude {
  int x;
  int y;
  int direction;
} Dude;

typedef struct level {
  int startPos;
  int blocks[10];
  int numBlocks;
  int doorPos;
  int length;
  int cols[25];
} Level;

Dude makeNewPlayer(int x, int y, int direction);
