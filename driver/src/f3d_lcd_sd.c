/*
 * f3d_lcd_sd.c
 *
 
 * Created By: Zhongren Shao (shaoz) and Erin Leonhard (eeleonha)
 * Created Date: 10/17/14
 * Last Edited By: Zhongren Shao (shaoz) and Erin Leonhard (eeleonha)
 * Last Edited Date: 10/28/14
 *
 * Part of: C335 Lab 7, C335 Lab 8
 *
 * Sets up the LCD and its interface for use in other programs 
 */

/* Code: */
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <glcdfont.h>
#include <math.h>

static uint8_t madctlcurrent = MADVAL(MADCTLGRAPHICS);


// Inits the Pins and SPI Links needed to use the LCD
void f3d_lcd_sd_interface_init(void) {
  /* Pin Assignments 
     RS       PB9      OUT 
     RESET    PB10     OUT
     BKL      PB11     OUT
     CS       PB12     OUT 
     SCK      PB13     AF - assigned to SPI2 
     MISO     PB14     AF - assigned to SPI2 
     MOSI     PB15     AF - assigned to SPI2 
   */
  
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  //configuration of MISO, MOSI, SCK
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  /********* CONFIGURE MODE HERE (4.2.1) ***********************/
  // use AF Mode for Pins 13..15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /********** Alternate functions (4.2.2) *******/
  // Select the alternate functions for Pins 13..15
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5);

  /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
  
  /********* Call GPIO_Init function here (4.2.3) **************/
  // Init the Pins we just configured
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
  // Section 4.2 Pin Configuration for CS, RESET, RS, BKL
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | 
    GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  /********* CONFIGURE MODE HERE (4.2.1) ***********************/
  // Use OUT Mode for the above Pins, 9..12
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  /**********Call the GPIO_Init function (4.2.3)  *************/
  // Init the Pins we just configured
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  // Section 4.1 SPI2 configuration
  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(/*something goes here*/SPI2, &SPI_InitStructure);
  SPI_RxFIFOThresholdConfig(/*somthing goes here*/SPI2, SPI_RxFIFOThreshold_QF);
  SPI_Cmd(/*something goes here*/SPI2, ENABLE);
} 


struct lcd_cmdBuf {
  uint8_t command;
  uint8_t delay;
  uint8_t len;
  uint8_t data [16];
};

static const struct lcd_cmdBuf initializers[] = {
  // SWRESET Software reset
  { 0x01, 150, 0, 0},
  // SLPOUT Leave sleep mode
  { 0x11, 150, 0, 0},
  // FRMCTR1, FRMCTR2 Frame Rate configuration -- Normal mode, idle
  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
  { 0xB1, 0, 3, { 0x01, 0x2C, 0x2D }},
  { 0xB2, 0, 3, { 0x01, 0x2C, 0x2D }},
  // FRMCTR3 Frame Rate configureation -- partial mode
  { 0xB3, 0, 6, { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D }},
  // INVCTR Display inversion (no inversion)
  { 0xB4, 0, 1, { 0x07 }},
  /* ... */
  /*! llcmdstop2 !*/
  /*! llcmdstart3 !*/
  // PWCTR1 Power control -4.6V, Auto mode
  { 0xC0, 0, 3, { 0xA2, 0x02, 0x84}},
  // PWCTR2 Power control VGH25 2.4C, VGSEL -10, VGH = 3 * AVDD
  { 0xC1, 0, 1, { 0xC5}},
  // PWCTR3 Power control, opamp current smal, boost frequency
  { 0xC2, 0, 2, { 0x0A, 0x00 }},
  // PWCTR4 Power control, BLK/2, opamp current small and medium low
  { 0xC3, 0, 2, { 0x8A, 0x2A}},
  // PWRCTR5, VMCTR1 Power control
  { 0xC4, 0, 2, { 0x8A, 0xEE}},
  { 0xC5, 0, 1, { 0x0E }},
  // INVOFF Don't invert display
  { 0x20, 0, 0, 0},
  // Memory access directions. row address/col address, bottom to top refesh (10.1.27)
  { ST7735_MADCTL, 0, 1, {MADVAL(MADCTLGRAPHICS)}},
  // Color mode 16 bit (10.1.30
  { ST7735_COLMOD, 0, 1, {0x05}},
  // Column address set 0..127
  { ST7735_CASET, 0, 4, {0x00, 0x00, 0x00, 0x7F }},
  // Row address set 0..159
  { ST7735_RASET, 0, 4, {0x00, 0x00, 0x00, 0x9F }},
  // GMCTRP1 Gamma correction
  { 0xE0, 0, 16, {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }},
  // GMCTRP2 Gamma Polarity corrction
  { 0xE1, 0, 16, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 }},
  // DISPON Display on
  { 0x29, 100, 0, 0},
  // NORON Normal on
  { 0x13, 10, 0, 0},
  // End
  { 0, 0, 0, 0}
};

// Inits the LCD along with the interface
void f3d_lcd_init(void) {
  const struct lcd_cmdBuf *cmd;

  f3d_lcd_sd_interface_init();    // Setup SPI2 Link and configure GPIO pins
  LCD_BKL_ON();                   // Enable Backlight

  // Make sure that the chip select and reset lines are deasserted
  LCD_CS_DEASSERT();              // Deassert Chip Select

  LCD_RESET_DEASSERT();           
  delay(100);
  LCD_RESET_ASSERT();
  delay(100);
  LCD_RESET_DEASSERT();
  delay(100);

  // Write initialization sequence to the lcd
  for (cmd=initializers; cmd->command; cmd++) {
    LcdWrite(LCD_C,&(cmd->command),1);
    if (cmd->len)
      LcdWrite(LCD_D,cmd->data,cmd->len);
    if (cmd->delay) {
      delay(cmd->delay);
    }
  }
}

static void LcdWrite(char dc,const char *data,int nbytes) {
  GPIO_WriteBit(LCD_PORT,GPIO_PIN_DC,dc); // dc 1 = data , 0 = control
  GPIO_ResetBits(LCD_PORT,GPIO_PIN_SCE);
  spiReadWrite(SPILCD,0,data,nbytes,LCDSPEED);
  GPIO_SetBits(LCD_PORT,GPIO_PIN_SCE);
}

static void LcdWrite16(char dc,const uint16_t *data,int cnt) {
  GPIO_WriteBit(LCD_PORT,GPIO_PIN_DC,dc); 
  GPIO_ResetBits(LCD_PORT,GPIO_PIN_SCE);
  spiReadWrite16(SPILCD,0,data,cnt,LCDSPEED);
  GPIO_SetBits(LCD_PORT,GPIO_PIN_SCE);
}

int spiReadWrite(SPI_TypeDef *SPIx,uint8_t *rbuf, const uint8_t *tbuf, int cnt, uint16_t speed) {
  int i;
  int timeout;

  SPIx->CR1 = (SPIx->CR1&~SPI_BaudRatePrescaler_256)|speed;
  for (i = 0; i < cnt; i++){
    if (tbuf) {
      SPI_SendData8(SPIx,*tbuf++);
    } 
    else {
      SPI_SendData8(SPIx,0xff);
    }
    timeout = 100;
    while (SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);
    if (rbuf) {
      *rbuf++ = SPI_ReceiveData8(SPIx);
    } 
    else {
      SPI_ReceiveData8(SPIx);
    }
  }
  return i;
}

int spiReadWrite16(SPI_TypeDef *SPIx,uint8_t *rbuf, const uint16_t *tbuf, int cnt, uint16_t speed) {
  int i;
  
  SPIx->CR1 = (SPIx->CR1&~SPI_BaudRatePrescaler_256)|speed;
  SPI_DataSizeConfig(SPIx, SPI_DataSize_16b);

  for (i = 0; i < cnt; i++){
    if (tbuf) {
      //      printf("data=0x%4x\n\r",*tbuf);
      SPI_I2S_SendData16(SPIx,*tbuf++);
    } 
    else {
      SPI_I2S_SendData16(SPIx,0xffff);
    }
    while (SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);
    if (rbuf) {
      *rbuf++ = SPI_I2S_ReceiveData16(SPIx);
    } 
    else {
      SPI_I2S_ReceiveData16(SPIx);
    }
  }
  SPI_DataSizeConfig(SPIx, SPI_DataSize_8b);

  return i;
}

void f3d_lcd_setAddrWindow ( uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 , uint8_t madctl) {
  madctl = MADVAL ( madctl );
  if ( madctl != madctlcurrent ){
    f3d_lcd_writeCmd(ST7735_MADCTL);
    LcdWrite(LCD_D, &madctl, 1);
    madctlcurrent = madctl ;
  }
  f3d_lcd_writeCmd(ST7735_CASET);
  LcdWrite16(LCD_D,&x0,1);
  LcdWrite16(LCD_D,&x1,1);
  f3d_lcd_writeCmd(ST7735_RASET);
  LcdWrite16(LCD_D,&y0,1);
  LcdWrite16(LCD_D,&y1,1);
  f3d_lcd_writeCmd(ST7735_RAMWR);
}

void f3d_lcd_pushColor(uint16_t *color,int cnt) {
  LcdWrite16(LCD_D,color,cnt);
}

static void f3d_lcd_writeCmd(uint8_t c) {
  LcdWrite(LCD_C,&c,1);
}

void f3d_lcd_fillScreen(uint16_t color) {
  uint8_t x,y;
  f3d_lcd_setAddrWindow (0,0,ST7735_width-1,ST7735_height-1,MADCTLGRAPHICS);
  for (x=0;x<ST7735_width;x++) {
    for (y=0;y<ST7735_height; y++) {
      f3d_lcd_pushColor(&color,1);
    }
  }
}

inline void f3d_lcd_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
  if ((x >= ST7735_width) || (y >= ST7735_height)) return;
  f3d_lcd_setAddrWindow(x,y,x+1,y+1,MADCTLGRAPHICS);
  f3d_lcd_pushColor(&color,1);
}

void f3d_lcd_drawChar(uint8_t x, uint8_t y, unsigned char c, uint16_t color, uint16_t background_color) {
  int i, j;
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 8; j++){ 
      f3d_lcd_drawPixel(x+i,y+j, background_color);
    }
  }
  for (i = 0; i < 5; i++) {
    uint8_t byte = ASCII[c*5 + i];
    for (j = 0; j < 8; j++){
      if (byte & (1)) {
	f3d_lcd_drawPixel(x+i,y+j, color);
      }
      byte >>= 1;
    }
  }
}

void f3d_lcd_drawString(uint8_t x, uint8_t y, char *c, uint16_t color, uint16_t background_color) {
  while (c && *c) {
    f3d_lcd_drawChar(x, y, *c++, color, background_color);
    x += 6;
    if (x + 5 >= ST7735_width) {
      y += 10;
      x = 0;
    }
  }
}

// draws a line from (x1, y1) to (x2, y2)
void f3d_lcd_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
   signed int  x, y, addx, addy, dx, dy;
   signed long P;
   int i;
   dx = abs((signed int)(x2 - x1));
   dy = abs((signed int)(y2 - y1));
   x = x1;
   y = y1;

   if(x1 > x2)
      addx = -1;
   else
      addx = 1;
   if(y1 > y2)
      addy = -1;
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
         f3d_lcd_drawPixel(x, y, color);

         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         f3d_lcd_drawPixel(x, y, color);

         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
      }
   }
}

void f3d_lcd_drawRect(uint8_t x, uint8_t y, uint16_t color, int height, int width, int fill) {
  if (fill) {
    int i = 0;
    for (i = 0; i < width; i++) {
      f3d_lcd_drawLine(x + i, y, x + i, y + height, color);
    }
  } else {
    f3d_lcd_drawLine(x, y, x, y + height, color);
    f3d_lcd_drawLine(x, y, x + width, y, color);
    f3d_lcd_drawLine(x + width, y + height, x , y + height, color);
    f3d_lcd_drawLine(x + width, y + height, x + width, y, color);
  }
}

// draws a circle centered at (x, y) with radius radius
void f3d_lcd_drawCircle(uint8_t radius, uint8_t x, uint8_t y, uint16_t color, uint8_t fill) {
   signed int a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;

   do
   {
      if(fill)
      {
         f3d_lcd_drawLine(x-a, y+b, x+a, y+b, color);
         f3d_lcd_drawLine(x-a, y-b, x+a, y-b, color);
         f3d_lcd_drawLine(x-b, y+a, x+b, y+a, color);
         f3d_lcd_drawLine(x-b, y-a, x+b, y-a, color);
      }
      else
      {
	 f3d_lcd_drawPixel(a+x, b+y, color);
         f3d_lcd_drawPixel(b+x, a+y, color);
         f3d_lcd_drawPixel(x-a, b+y, color);
         f3d_lcd_drawPixel(x-b, a+y, color);
         f3d_lcd_drawPixel(b+x, y-a, color);
         f3d_lcd_drawPixel(a+x, y-b, color);
         f3d_lcd_drawPixel(x-a, y-b, color);
         f3d_lcd_drawPixel(x-b, y-a, color);
      }

      if(P < 0)
         P+= 3 + 2*a++;
      else
         P+= 5 + 2*(a++ - b--);
    } while(a <= b);
} 

// draws (magnitude/PI) part of a semicircle centered at (x, y) with radius radius from the top
void f3d_lcd_drawSemicircle(uint8_t radius, uint8_t x, uint8_t y, uint16_t color, float *magnitude) {
  float radians = 0;
  float newmag = 2 * *magnitude;
  if (newmag >= 0) {
    for(radians = -M_PI/2; radians < -M_PI/2 + newmag; radians += .03) {
      int newx = x + cos(radians) * radius;
      int newy = y + sin(radians) * radius;
      f3d_lcd_drawPixel(newx, newy, color);
    }
  } else {
    for(radians = -M_PI/2; radians > -M_PI/2 + newmag; radians -= .03) {
      int newx = x + cos(radians) * radius;
      int newy = y + sin(radians) * radius;
      f3d_lcd_drawPixel(newx, newy, color);
      }
  }
}

void f3d_lcd_eraseSemicircle(uint8_t radius, uint8_t x, uint8_t y, uint16_t color, float *magnitude) {
  float radians = 0;
  float newmag = 2 * *magnitude;
  if (newmag >= 0) {
    for(radians = -M_PI/2 + newmag; radians < M_PI; radians += .03) {
      int newx = x + cos(radians) * radius;
      int newy = y + sin(radians) * radius;
      f3d_lcd_drawPixel(newx, newy, color);
    }
  } else {
    for(radians = -M_PI/2 + newmag; radians > -M_PI; radians -= .03) {
      int newx = x + cos(radians) * radius;
      int newy = y + sin(radians) * radius;
      f3d_lcd_drawPixel(newx, newy, color);
      }
  }
}

void f3d_lcd_drawSemicircle2(uint8_t radius, uint8_t x, uint8_t y, uint16_t color, float *magnitude) {
  float radians = 0;
  if (*magnitude >= 0) {
    for(radians = -M_PI/2 - *magnitude; radians < -M_PI/2 + *magnitude; radians += .03) {
      int newx = x + cos(radians) * radius;
      int newy = y + sin(radians) * radius;
      f3d_lcd_drawPixel(newx, newy, color);
    }
  } else {
    for(radians = M_PI/2 - *magnitude; radians > M_PI/2 + *magnitude; radians -= .03) {
      int newx = x + cos(radians) * radius;
      int newy = y + sin(radians) * radius;
      f3d_lcd_drawPixel(newx, newy, color);
      }
  }
}

// draws a dot on a circle that has radius radius centered at (x, y) at positon (-magnitude + offset)
void f3d_lcd_placeDotOnCircle(uint8_t radius, uint8_t x, uint8_t y, uint16_t color, float *magnitude, float *offset){
  float newmag = -*magnitude + *offset - M_PI/18;
  int newx = x + cos(newmag) * radius;
  int newy = y + sin(newmag) * radius;
  f3d_lcd_drawCircle(3, newx, newy, color, 1);
}
/* f3d_lcd_sd.c ends here */
