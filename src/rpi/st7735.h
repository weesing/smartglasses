#ifndef __ST7735_H__
#define __ST7735_H__
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "spi.h"

#define CS_PIN	8UL
#define DC_PIN  24UL
#define RST_PIN	25UL

// Screen resolution in normal orientation
//#define scr_w         160
//#define scr_h         128
#define scr_w         80
#define scr_h         160

typedef enum
{
  NOP = 0x00,
  SWRESET = 0x01,   /* Software Reset */
  RDDID = 0x04,     /* Read Display ID */
  RDDST = 0x09,     /* Read Display Status */
  RDDPM = 0x0a,     /* Read Display Power Mode */
  RDDMADCTL = 0x0b, /* Read Display MADCTL */
  RDDCOLMOD = 0x0c, /* Read Display Pixel Format */
  RDDIM = 0x0d,     /* Read Display Image Mode */
  RDDSM = 0x0e,     /* Read Display Signal Mode */
  RDDSDR = 0x0f,    /* Read Display Self-Diagnostic Result */
  SLPIN = 0x10,     /* Sleep In */
  SLPOUT = 0x11,    /* Sleep Out */
  PTLON = 0x12,     /* Partial Display Mode On */
  NORON = 0x13,     /* Normal Display Mode On */
  INVOFF = 0x20,    /* Display Inversion Off */
  INVON = 0x21,     /* Display Inversion On */
  GAMSET = 0x26,    /* Gamma Set */
  DISPOFF = 0x28,   /* Display Off */
  DISPON = 0x29,    /* Display On */
  CASET = 0x2a,     /* Column Address Set */
  RASET = 0x2b,     /* Row Address Set */
  RAMWR = 0x2c,     /* Memory Write */
  RGBSET = 0x2d,    /* Color Setting 4k, 65k, 262k */
  RAMRD = 0x2e,     /* Memory Read */
  PTLAR = 0x30,     /* Partial Area */
  SCRLAR = 0x33,    /* Scroll Area Set */
  TEOFF = 0x34,     /* Tearing Effect Line OFF */
  TEON = 0x35,      /* Tearing Effect Line ON */
  MADCTL = 0x36,    /* Memory Data Access Control */
  VSCSAD = 0x37,    /* Vertical Scroll Start Address of RAM */
  IDMOFF = 0x38,    /* Idle Mode Off */
  IDMON = 0x39,     /* Idle Mode On */
  COLMOD = 0x3a,    /* Interface Pixel Format */
  RDID1 = 0xda,     /* Read ID1 Value */
  RDID2 = 0xdb,     /* Read ID2 Value */
  RDID3 = 0xdc,     /* Read ID3 Value */
  FRMCTR1 = 0xb1,   /* Frame Rate Control in normal mode, full colors */
  FRMCTR2 = 0xb2,   /* Frame Rate Control in idle mode, 8 colors */
  FRMCTR3 = 0xb3,   /* Frame Rate Control in partial mode, full colors */
  INVCTR = 0xb4,    /* Display Inversion Control */
  PWCTR1 = 0xc0,    /* Power Control 1 */
  PWCTR2 = 0xc1,    /* Power Control 2 */
  PWCTR3 = 0xc2,    /* Power Control 3 in normal mode, full colors */
  PWCTR4 = 0xc3,    /* Power Control 4 in idle mode 8colors */
  PWCTR5 = 0xc4,    /* Power Control 5 in partial mode, full colors */
  PWCTR6 = 0xfc,
  VMCTR1 = 0xc5,    /* VCOM Control 1 */
  VMOFCTR = 0xc7,   /* VCOM Offset Control */
  WRID2 = 0xd1,     /* Write ID2 Value */
  WRID3 = 0xd2,     /* Write ID3 Value */
  NVFCTR1 = 0xd9,   /* NVM Control Status */
  NVFCTR2 = 0xde,   /* NVM Read Command */
  NVFCTR3 = 0xdf,   /* NVM Write Command */
  GMCTRP1 = 0xe0,   /* Gamma '+'Polarity Correction Characteristics Setting */
  GMCTRN1 = 0xe1,   /* Gamma '-'Polarity Correction Characteristics Setting */
  GCV = 0xfc,       /* Gate Pump Clock Frequency Variable */
} ST7735S_Command;

// CS pin macros
#define CS_L() gpio_clear_pin(CS_PIN)
#define CS_H() gpio_set_pin(CS_PIN)

// A0 pin macros
#define DC_L() gpio_clear_pin(DC_PIN)
#define DC_H() gpio_set_pin(DC_PIN)

// RESET pin macros
#define RST_L() gpio_clear_pin(RST_PIN)
#define RST_H() gpio_set_pin(RST_PIN)


#define Delay_US(x) usleep(x)
// Colors for spaces between symbols for debug view
//#define V_SEP COLOR565_YELLOW
//#define H_SEP COLOR565_SIENNA
//#define V_SEP COLOR565_WHITE
//#define H_SEP COLOR565_WHITE

typedef enum
{
  scr_normal = 0,
  scr_CW = 1,
  scr_CCW = 2,
  scr_180 = 3
} ScrOrientation_TypeDef;

extern uint16_t scr_width;
extern uint16_t scr_height;

uint16_t RGB565 (uint8_t R, uint8_t G, uint8_t B);
void ST7735_write (uint8_t data);
void ST7735_InitCommand(uint8_t cmd, uint8_t *argv, uint16_t args);
void ST7735_Init(void);
void ST7735_AddrSet (uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE);
void ST7735_Orientation (uint8_t orientation);
void ST7735_Clear (uint16_t color);
extern void Delay_US (volatile  uint32_t nTime);

#endif
