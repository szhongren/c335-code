/* main.c --- 
 * 
 * Filename: main.c
 *
 * Part of: C335 Final Project
 * Created: 12.5.14
 * Last-Updated: Erin Leonhard (eeleonha) &
 *           By: Zhongren Shao (shaoz)
 */

/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;
extern int nunchuk_flag;

FATFS Fatfs;	/* File system object */
FIL fid;	/* File object */
BYTE Buff[512];	/* File read buffer */
int ret;

char *WAV_FILES[] = {"thermo.wav", "cat.wav", "ahem.wav", "goat.wav", "peeoop.wav"};
int NUM_WAV_FILES = 5;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

// plays the selected audio file
int play_audio_from_file(FRESULT *rc) {
  if (!*rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
  	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it!
    //audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
  	if (next < AUDIOBUFSIZE/2)
  	  bzero(Audiobuf, AUDIOBUFSIZE/2);
  	f_read(&fid, Audiobuf, next, &ret);
  	hd.cksize -= ret;
  	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
  	if (next < AUDIOBUFSIZE/2)
  	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
  	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
  	hd.cksize -= ret;
  	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
}

void win() {
  f3d_lcd_fillScreen(GREEN);
  f3d_lcd_drawCircle(20, 64, 80, YELLOW, 1);
}

// this 'advances' the game forward a step, so we can put everything we need to check for each step of the game in here. already in a while loop that exits when dude reaches door
int gameStep(Level lvl, Dude *dude, State *state) {
  extern int move_flag;
  extern int action_flag;
  static int joystick_flag = 0;
  static int buttons_flag = 0;

  if (dude->x == lvl.doorPos) {
    win();
    return 0;
  } else { 
    if (move_flag) {
      if (!joystick_flag) {
	State old = *state;
	int validMove = 1;
	moveDude(move_flag, lvl, dude, state, &validMove);
	updateScreen(validMove, lvl, *dude, old, *state);
	/* eraseOldDude(*dude); */
	/* dude->x = dude->x + move_flag; */
	/* dude->y = getYPosnOfBlock(lvl, dude->x + state->left); */
	/* drawDude(*dude, state->dudeColor, state->capColor); */
	joystick_flag = 1;
      }
    } else {
      joystick_flag = 0;
    }

    if (action_flag) {
      if (!buttons_flag) {
	// check what action_flag is, 1 for pickup, -1 for drop, then check if the action is valid
	buttons_flag = 1;
      }
    } else {
      action_flag = 0;
    }
    
    // State old = *state;
    // if ...
    

    return 1;
  }
}

int main(void) {

  FRESULT rc;	/* Result code */
  DIR dir;	/* Directory object */
  FILINFO fno;	/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_timer2_init();
  f3d_dac_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_lcd_init();
  f3d_led_init();
  f3d_i2c1_init();
  f3d_nunchuk_init();
  f3d_systick_init(150);
  
  extern int nunchuk_flag;
  int FLAG_MOVE = 0;
 
  Dude player1 = makeNewPlayer(1,1,RIGHT);
  Level lvl1 = initLevel();
  State start = defaultState();

  f_mount(0, &Fatfs);
  f3d_lcd_fillScreen(BLACK);
  initGraphics(lvl1, player1, start); // the old drawScreen

  while (gameStep(lvl1, &player1, &start));
  
  f3d_systick_init(10); // shows when you end the loop by making lights slower

  /* while (player1.x != lvl1.doorPos) { */
  /*   /\* f3d_nunchuk_read(&nun_data); *\/ */
  /*   /\* change = movePlayer(&nun_data); *\/ */
  /*   if (nunchuk_flag) { */
  /*     if (!FLAG_MOVE) { */
  /* 	eraseOldDude(player1); */
  /* 	player1.x = player1.x + nunchuk_flag; */
  /* 	player1.y = getYPosnOfBlock(lvl1, player1.x + start.left); */
  /* 	drawDude(player1, start.dudeColor, start.capColor); */
  /* 	FLAG_MOVE = 1; */
  /*     } */
  /*   } else { */
  /*     FLAG_MOVE = 0; */
  /*   } */
  /* } */

}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
