/*
	 gpio.c
	 http://blog.vinu.co.in
	 
	 A very basic raspberrypi gpio driver written only for this project.
	 So all features are not implemented. The code is written based on 
	 BCM2835-ARM-Peripherals.pdf
	 
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

#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include "rpi_gpio.h"

volatile unsigned int *gpio = MAP_FAILED;

void gpio_mode_output(unsigned int pin)
{
	int x;

	int p = pin;
	while (p > 9)
	{
		p -= 10;
	}

	gpio[GPIO_GPFSEL0 + pin / 10] |= 1UL << (p * 3); //make as output
																									 //gpio[GPIO_GPFSEL0 + pin/10] &= ~( 1 << (p*3) ); //use this to make input.
}

void gpio_set_pin(unsigned int pin)
{
	gpio[GPIO_GPSET0 + pin / 32] = (1UL << ((pin - (pin / 32) * 32)));
}

void gpio_clear_pin(unsigned int pin)
{
	gpio[GPIO_GPCLR0 + pin / 32] = (1UL << ((pin - (pin / 32) * 32)));
}

void gpio_init(void)
{
	int memfd = -1;

	if ((memfd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
	{
		fprintf(stderr, "/dev/mem open error\n");
		return;
	}

	gpio =
			mmap(NULL, 4096, (PROT_READ | PROT_WRITE), MAP_SHARED, memfd,
					 0x20200000);
	if (gpio == MAP_FAILED)
	{
		fprintf(stderr, "mmap failed\n");
		return;
	}
	printf("gpio mmap value = %x\n", gpio);

	//                              gpio = (unsigned int *)GPIO_BASE;
}
