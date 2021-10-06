/*
	 http://blog.vinu.co.in
	 spi.c
	 A bare minimum spi code for raspberry pi.


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
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/spi/spidev.h>

static const char *spiDev0 = "/dev/spidev0.0";
static const char *spiDev1 = "/dev/spidev0.1";
static uint32_t spiSpeed;
static int spiFd;
static const uint8_t spiBPW = 8;
static const uint16_t spiDelay = 0;

void wordToBytes(uint8_t *pResult, uint16_t *pData, uint32_t dataLen)
{
	for (uint32_t offset = 0; offset < dataLen; ++offset)
	{
		uint16_t word = *(pData + offset);
		*pResult = word >> 8;
		*(pResult + 1) = (word & 0xff);
		pResult += 2;
	}
	return;
}

int spi_write(int channel, unsigned char *data, int len)
{
	struct spi_ioc_transfer spi;

	channel &= 1;

	static unsigned char bbb[4096];

	memset(&spi, 0, sizeof(spi));

	spi.tx_buf = (unsigned long)data;
	spi.rx_buf = (unsigned long)bbb;
	spi.len = len;
	spi.delay_usecs = spiDelay;
	spi.speed_hz = spiSpeed;
	spi.bits_per_word = spiBPW;

	return ioctl(spiFd, SPI_IOC_MESSAGE(1), &spi);
}

int spi_write_16(int channel, uint16_t *data, int len)
{
	// Convert 16 bits data to 8 bit (byte) chunks.
	unsigned char dataBytes[len << 1];
	unsigned char *pDataBytes = dataBytes;
	for (uint32_t offset = 0; offset < len; ++offset)
	{
		uint16_t word = *(data + offset);
		*pDataBytes = word >> 8;
		*(pDataBytes + 1) = word & 0xff;
		pDataBytes += 2;
	}

	spi_write(channel, dataBytes, len << 1);
}

int spi_init(int channel, int speed, int mode)
{
	int fd;

	mode &= 3;		// Mode is 0, 1, 2 or 3
	channel &= 1; // Channel is 0 or 1

	if ((fd = open(channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0)
		printf("Unable to open SPI device\n");

	spiSpeed = speed;
	spiFd = fd;

	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
		printf("SPI mode change failure\n");

	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
		printf("SPI BPW change failure\n");

	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
		printf("SPI SPEED change failure\n");

	printf("SPI Device=0x%x\n", fd);
	return fd;
}
