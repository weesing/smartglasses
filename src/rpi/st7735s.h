#ifndef __ST7735S_h__
#define __ST7735S_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "st7735s_compat.h"

/* undef if low on mem */
#if !defined (BUFFER) && !defined (BUFFER1) && !defined (HVBUFFER)
  #warning no buffer defined, defining BUFFER1
  #define BUFFER1
#endif

typedef enum { R0, R90, R180, R270 } rotation_t;
typedef enum { ON, OFF } idlemode_t;

typedef struct  {
    union {
        struct {
            uint16_t r:5;
            uint16_t g:6;
            uint16_t b:5;
        } __attribute__((packed));
        uint8_t u[2];
    };
} __attribute__((packed)) color565_t;

extern uint16_t WIDTH, HEIGHT;
extern uint16_t XSTART, XSTART;

extern color565_t color;
extern color565_t bg_color;

void Delay(uint32_t);
void Backlight_Pct(uint8_t p);
void ST7735S_Init(void);
void ST7735S_flush(void);
void ST7735S_Pixel(uint16_t x, uint16_t y);
void ST7735S_bgPixel(uint16_t x, uint16_t y);
void setOrientation(rotation_t r);
void ST7735S_sleepIn(void);
void ST7735S_sleepOut(void);
bool ST7735S_defineScrollArea(uint16_t, uint16_t);
void ST7735S_tearingOn(bool);
void ST7735S_tearingOff(void);
void ST7735S_partialArea(uint16_t, uint16_t);
void ST7735S_normalMode(void);
void ST7735S_scroll(uint8_t);
uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B);
void ST7735S_addrset(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE);
#ifdef __cplusplus
} // extern "C"
#endif

#endif

