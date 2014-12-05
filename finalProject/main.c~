/* main.c --- 
 * 
 * Filename: main.c
 *
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: Erin Leonhard (eeleonha) &
 *           By: Zhongren Shao (shaoz)
 *     Update #: 1
 */

/* Commentary: 
 * The below definitions complete C335 Lab 12.
 */

/* Change log:
 *
 *
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

// this will draw the menu on the LCD screen.
void draw_lcd_menu(void){
  f3d_lcd_fillScreen(BLACK); // background
  int TOP = 5;
  int VERT_SPACING = 12;
  int x_posn = 10;
  int i = 0;
  for (i = 0; i < NUM_WAV_FILES; i++) {
    uint8_t y_posn = VERT_SPACING * i + TOP;
    f3d_lcd_drawString(x_posn, y_posn, WAV_FILES[i], WHITE, BLACK); // file names
  }
}

// indicate which file is currently selected with brackets
void highlight_selected_on_menu(int *file) {
  int TOP = 5;
  int VERT_SPACING = 12;
  int x_posn_1 = 3;
  int x_posn_2 = 120;
  int i = 0;
  // erase all old highlighting and draw new highlighting for current selection
  for (i = 0; i < NUM_WAV_FILES; i++) {
    uint8_t y_posn = VERT_SPACING * i + TOP;
    if (i != *file ) {
      // erase old highlighting
      f3d_lcd_drawChar(x_posn_1, y_posn, '[', BLACK, BLACK); // first bracket
      f3d_lcd_drawChar(x_posn_2, y_posn, ']', BLACK, BLACK); // second bracket
    }
    else {
      // draw new highlighting
      f3d_lcd_drawChar(x_posn_1, y_posn, '[', CYAN, BLACK); // first bracket
      f3d_lcd_drawChar(x_posn_2, y_posn, ']', CYAN, BLACK); // second bracket
    }
  }
}

// handles switching WAV files
int change_file(int *file, int change) {
  if (*file == NUM_WAV_FILES - 1) {
    if (change == 1) {
      *file = 0;
    } else {
      *file += change;
    }
  } else if (*file == 0) {
    if (change == -1) {
      *file = NUM_WAV_FILES - 1;
    } else {
      *file += change;
    }
  } else {
    *file += change;
  }
  // highlight newly selected file
  highlight_selected_on_menu(file);
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

  f_mount(0, &Fatfs);
  int file = 0;
  int change = 0;
  int FLAG_CHNG_STATE = 0;
  draw_lcd_menu();
  highlight_selected_on_menu(&file);
  //nunchuk_t nun_data;

  while (1) {
    // check for request to change files from nunchuk
    //f3d_nunchuk_read(&nun_data);
    //change = f3d_nunchuk_change_mode(&nun_data);
    if (nunchuk_flag) {
      //if (!FLAG_CHNG_STATE) {
	change_file(&file, nunchuk_flag);
    }
	//FLAG_CHNG_STATE = 1;
	// } 
	//} else { 
	//FLAG_CHNG_STATE = 0;
	//}

    // printf("\nOpen %s\n", WAV_FILES[file]);
    // rc = f_open(&fid, WAV_FILES[file], FA_READ);

    switch(file) {
    case 0:
      rc = f_open(&fid, "thermo.wav", FA_READ);
      break;
    case 1:
      rc = f_open(&fid, "cat.wav", FA_READ);
      break;
    case 2:
      rc = f_open(&fid, "ahem.wav", FA_READ);
      break;
    case 3:
      rc = f_open(&fid, "goat.wav", FA_READ);
      break;
    case 4:
      rc = f_open(&fid, "peeoop.wav", FA_READ);
      break;
    }

    // play selected audio
    play_audio_from_file(&rc);
    
    // close file
    //printf("\nClose the file.\n");
    rc = f_close(&fid);
    
    if (rc) die(rc);
  }
  /*
  if (!rc) {
    struct ckhd hd;
    uint32_t waveid;
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
    
    // Play it !
    
    // audioplayerInit(fck.nSamplesPerSec);
    
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
  
  printf("\nClose the file.\n");*/
  //rc = f_close(&fid);
  
  // if (rc) die(rc);
  // while (1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
