/*
	http://blog.vinu.co.in
	basic driver for st7735r lcd.
	This is just to initialize the lcd and not for any rendering purpose.
	 
	 This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
		 but WITHOUT ANY WARRANTY; without even the implied warranty of
		 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		 GNU General Public License for more details.

		 You should have received a copy of the GNU General Public License
		 along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include "st7735.h"
#include <inttypes.h>
#include <stdlib.h>
#include "spi.h"
#include "gpio.h"

uint16_t scr_width;
uint16_t scr_height;

void ST7735_write(uint8_t data)
{
	spi_write(0, &data, 1);
	//wiringPiSPIDataW(0, &data, 1) ;
}

void ST7735_cmd(uint8_t cmd)
{
	DC_L();
	ST7735_write(cmd);
}

void ST7735_data(uint8_t data)
{
	DC_H();
	ST7735_write(data);
}

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B)
{
	return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
}

static uint8_t init_cmd[] = {
		1, SWRESET,																			/* software reset */
		1, SLPOUT,																			/* sleep out, turn off sleep mode */
		1, DISPOFF,																			/*  output from frame mem disabled */
		4, FRMCTR1, 0x00, 0b111111, 0b111111,						/* frame frequency normal mode (highest frame rate in normal mode) */
		4, FRMCTR2, 0b1111, 0x01, 0x01,									/* frame frequency idle mode */
		7, FRMCTR3, 0x05, 0x3c, 0x3c, 0x05, 0x3c, 0x3c, /* frame freq partial mode: 1-3 dot inv, 4-6 col inv */
		2, INVCTR, 0x03,																/* display inversion control: 3-bit 0=dot, 1=col */

		4, PWCTR1, 0b11111100, 0x08, 0b10, /* power control */
		2, PWCTR2, 0xc0,
		3, PWCTR3, 0x0d, 0x00,
		3, PWCTR4, 0x8d, 0x2a,
		3, PWCTR5, 0x8d, 0xee, /* partial */

		/* display brightness and gamma */
		2, GCV, 0b11011000,			/* auto gate pump freq, max power save */
		2, NVFCTR1, 0b01000000, /* automatic adjust gate pumping clock for saving power consumption */
		2, VMCTR1, 0b001111,		/* VCOM voltage setting */
		2, VMOFCTR, 0b10000,		/* ligthness of black color 0-0x1f */
		2, GAMSET, 0x08,				/* gamma 1, 2, 4, 8 */

		/* row order, col order, row colum xchange, vert refr order, rgb/bgr, hor refr order, 0, 0 */
		2, MADCTL, 0b00100000,
		5, CASET, 0, 0, 0, scr_h - 1, // See MADCTL
		5, RASET, 0, 0, 0, scr_w - 1, // See MADCTL
		2, COLMOD, 0x05,							/* 3=12bit, 5=16-bit, 6=18-bit  pixel color mode */
		17, GMCTRP1,
		0x02, 0x1c, 0x07, 0x12,
		0x37, 0x32, 0x29, 0x2c,
		0x29, 0x25, 0x2b, 0x39,
		0x00, 0x01, 0x03, 0x10,
		17, GMCTRN1,
		0x03, 0x1d, 0x07, 0x06,
		0x2E, 0x2C, 0x29, 0x2c,
		0x2e, 0x2e, 0x37, 0x3f,
		0x00, 0x00, 0x02, 0x10,
		1, INVON, /* display inversion on/off */
		// 1, INVOFF,
		1, IDMOFF, /* idle mode off */
		1, NORON,	 /* normal display mode on */
		1, DISPON, /* recover from display off, output from frame mem enabled */
};

void ST7735_InitCommand(uint8_t cmd, uint8_t *pArgv, uint16_t nbArgs)
{
	printf("Sending command 0x%x with %d arguements\n", cmd, nbArgs);
	// command
	ST7735_cmd(cmd);
	DC_H();
	// data
	for (uint16_t i = 0; i < nbArgs; ++i)
	{
		ST7735_write(*pArgv++);
		++pArgv;
	}
	Delay_US(20);
	return;
}

void ST7735_Init(void)
{

	//init spi
	printf("ST7735_Init...\n");
	//wiringPiSetup ();
	gpio_init();
	spi_init(0, 30000000, 0);
	// spi_init(0, 2000000, 0);

	gpio_mode_output(CS_PIN);
	gpio_mode_output(DC_PIN);
	gpio_mode_output(RST_PIN);
	gpio_mode_output(23);
	gpio_mode_output(17);

	//toggle test for debugging
	/*while(1) {
//	   CS_H();
//	   CS_L();

//	   DC_H();
//	   DC_L();

	   RST_H();
		usleep(1000);
	   RST_L();
		usleep(1000);

	   }
*/

	// Reset display
	CS_H();
	RST_H();
	Delay_US(5);
	RST_L();
	Delay_US(5);
	RST_H();
	CS_H();
	Delay_US(5);

	CS_L();

	uint8_t *pCmd = init_cmd;
	uint16_t cmdIndex = 0;
	if (sizeof(init_cmd) > 0)
	{
		while (cmdIndex < sizeof(init_cmd))
		{
			uint8_t nbArgs = init_cmd[cmdIndex] - 1;
			uint8_t cmd = init_cmd[cmdIndex + 1];
			uint8_t *pArgv = &(init_cmd[cmdIndex + 2]);
			ST7735_InitCommand(cmd, pArgv, nbArgs);
			cmdIndex += init_cmd[cmdIndex] + 1;
		}
	}

	CS_H();

	ST7735_Orientation(scr_CCW);
	printf("ST7735_Init done!\n");
}

void ST7735_Orientation(uint8_t orientation)
{
	printf("---------- Changing orientation to %d\n", orientation);
	CS_L();
	ST7735_cmd(MADCTL); // Memory data access control:
	switch (orientation)
	{
	case scr_CW:
		scr_width = scr_h;
		scr_height = scr_w;
		ST7735_data(0xa0); // X-Y Exchange,Y-Mirror
		printf("Setting MADCTL to 0x20\n");
		break;
	case scr_CCW:
		scr_width = scr_h;
		scr_height = scr_w;
		//ST7735_data(0x60);	// X-Y Exchange,X-Mirror
		/* row order, col order, row colum xchange, vert refr order, rgb/bgr, hor refr order, 0, 0 */
		ST7735_data(0b00100000); // X-Y Exchange: X mirror
		printf("Setting MADCTL to 0x20\n");
		break;
	case scr_180:
		scr_width = scr_w;
		scr_height = scr_h;
		ST7735_data(0xc0); // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		printf("Setting MADCTL to 0xc0\n");
		break;
	default:
		scr_width = scr_w;
		scr_height = scr_h;
		ST7735_data(0x00); // Normal: Top to Bottom; Left to Right; RGB
		printf("Setting MADCTL to 0x00\n");
		break;
	}
	printf("scr_width=%d,scr_height=%d\n", scr_width, scr_height);
	printf("---------- Orientation changed to %d.\n\n", orientation);
	CS_H();
}

void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE)
{
	ST7735_cmd(CASET); // Column address set
	DC_H();
	ST7735_write(XS >> 8);
	ST7735_write(XS);
	ST7735_write(XE >> 8);
	ST7735_write(XE);

	ST7735_cmd(RASET); // Row address set
	DC_H();
	ST7735_write(YS >> 8);
	ST7735_write(YS);
	ST7735_write(YE >> 8);
	ST7735_write(YE);

	ST7735_cmd(RAMWR); // Memory write
}

void ST7735_Clear(uint16_t color)
{
	uint16_t i;
	uint8_t CH, CL;

	CH = color >> 8;
	CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(0, 0, scr_width - 1, scr_height - 1);
	DC_H();
	for (i = 0; i < scr_width * scr_height; i++)
	{
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}
