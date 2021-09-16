//////////////////////////////////////////////////////////////////////
/*
 * main.c
 * http://blog.vinu.co.in
 * <mail@vinu.co.in>
 *brief: Mapping /dev/fb0 to 160x128 16 bit TFT st7735r from user space.
 *Cpu usage: 4.5 to 5% APROX at 20fbs
//////////////////////////////////////////////////////////////////////
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>*/
///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <sys/time.h>
// #include <time.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <inttypes.h>
#include <stdlib.h>
#include "st7735.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "spi.h"
#include "gpio.h"
#include <endian.h>

#define debug
// #define TEST_FILL
#define SCR_WIDTH 160
#define SCR_HEIGHT 80

//uint16_t fb2[128 * 160];
uint16_t fb2[SCR_HEIGHT * SCR_WIDTH];
uint8_t g_screenOffset[] = {0, 24};

void testScreenFill(void)
{
      uint16_t *p;
      while (1)
      {
            p = fb2;
            struct timeval now;
            gettimeofday(&now, NULL);
            srand(now.tv_usec);
            // srand(time(NULL));
            uint8_t r = rand() % 0xff;
            uint8_t g = rand() % 0xff;
            uint8_t b = rand() % 0xff;
            uint16_t initColor = RGB565(r, g, b);
            // uint16_t initColor = RGB565(0x0, 0x0, 0xff);
#ifdef debug
            printf("Initializing color to 0x%x, address=0x%x\n", initColor, p);
#endif
            for (uint32_t i = 0; i < SCR_WIDTH * SCR_HEIGHT; ++i)
            {
                  *p++ = initColor;
            }
#ifdef debug
            printf("Color initialized, address=0x%x\n", p);
#endif
            p = fb2;

            //clear frame buffer
            CS_L();
            //ST7735_AddrSet (0, 0, 160, 128);
            ST7735_AddrSet(
                g_screenOffset[0],
                g_screenOffset[1],
                g_screenOffset[0] + SCR_WIDTH - 1,
                g_screenOffset[1] + SCR_HEIGHT - 1);
            DC_H();
#ifdef debug
            printf("Copying framebuffer...\n");
#endif

            uint32_t stride = SCR_WIDTH;
            for (uint32_t i = 0; i < SCR_HEIGHT; i++)
            {
                  spi_write_16(0, p, stride);
                  p += stride;
            }

#ifdef debug
            printf("Screen copied\n\n\n");
#endif

            // Uncomment this to have interval between color fills.
            usleep(300000);
      }
}

int main()
{
      int fbfd = 0;

      struct fb_var_screeninfo vinfo;
      long int screensize = 0;
      uint8_t *fbp = 0;
      int i;
      uint16_t *p = fb2;
      int xx = 0;
      int yy = 0;

      fbfd = open("/dev/fb0", O_RDWR);

      ST7735_Init();

      memset(fb2, 0, sizeof(fb2));
      printf("fb2 size = %d bytes\n", sizeof(fb2));

#ifdef TEST_FILL
      testScreenFill();
#endif // #ifdef TEST_FILL

      while (1)
      {
            usleep(50000);

            ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
#ifdef debug
            printf("X = %d, Y = %d, bits per pixel = %d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
#endif
            CS_L();

            //ST7735_AddrSet (0, 4, 160, 124);
            ST7735_AddrSet(
                g_screenOffset[0],
                g_screenOffset[1],
                g_screenOffset[0] + SCR_WIDTH - 1,
                g_screenOffset[1] + SCR_HEIGHT - 1);

            DC_H();

            screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
#ifdef debug
            printf("screensize = %d bytes (X * Y * 32 / 8)\n", screensize);
#endif
            // const int PADDING = 4096;
            int mmapsize = screensize;
#ifdef debug
            printf("mapsize = %d\n", mmapsize);
#endif
            fbp = (char *)mmap(0, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

            p = fb2;
            // 32 bits to 16 bits
            uint8_t *pPixel = (uint8_t *)fbp;
            uint8_t xscale = vinfo.xres / SCR_WIDTH;
            uint8_t yscale = vinfo.yres / SCR_HEIGHT;
#ifdef debug
            printf("Calculated screen scaling X:%.2f, Y:%.2f\n", 1 / xscale, 1 / yscale);
#endif // #ifdef debug
            for (uint16_t row = 0; row < SCR_HEIGHT; ++row)
            {
                  for (uint16_t col = 0; col < SCR_WIDTH; ++col)
                  {
                        uint8_t b = *pPixel;
                        ++pPixel;
                        uint8_t g = *pPixel;
                        ++pPixel;
                        uint8_t r = *pPixel;
                        pPixel += 2; // + alpha
                        *p = RGB565(r, g, b);
                        p += xscale;
                  }
                  p += SCR_WIDTH * (yscale - 1);
            }

            munmap(fbp, screensize);

            p = fb2;
            uint32_t stride = SCR_WIDTH;
            for (uint32_t i = 0; i < SCR_HEIGHT; i++)
            {
                  spi_write_16(0, p, stride);
                  p += stride;
            }
      }

      CS_H();
      printf("done\n");

      close(fbfd);

      return 0;
}
