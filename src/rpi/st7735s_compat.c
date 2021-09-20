#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include "rpi_spi.h"
#include "rpi_gpio.h"
#include "st7735s.h"
#include "st7735s_compat.h"

#define CS_PIN 8UL
#define DC_PIN 24UL
#define RES_PIN 25UL
#define BLK_PIN 0UL

void SPI_Init(void)
{
  printf("GPIO:\n");
  printf("\tInitializing\n");
  gpio_init();
  printf("\tCompleted\n");

  printf("SPI:\n");
  printf("\tInitializing\n");
  spi_init(0, 30000000, 0);
  printf("\tCompleted\n");

  printf("Setting GPIOs Modes\n");
  gpio_mode_output(CS_PIN);
  gpio_mode_output(RES_PIN);
  gpio_mode_output(DC_PIN);
  gpio_mode_output(23);
  gpio_mode_output(17);

  // gpio_clear_pin(CS_PIN);
  // gpio_set_pin(RES_PIN);
}

void Pin_CS_Low(void)
{
  gpio_set_pin(CS_PIN);
}

void Pin_CS_High(void)
{
  gpio_clear_pin(CS_PIN);
}

void Pin_RES_High(void)
{
  gpio_set_pin(RES_PIN);
}

void Pin_RES_Low(void)
{
  gpio_clear_pin(RES_PIN);
}

void Pin_DC_High(void)
{
  gpio_set_pin(DC_PIN);
}

void Pin_DC_Low(void)
{
  gpio_clear_pin(DC_PIN);
}
extern uint8_t backlight_pct;
void Pin_BLK_Pct(uint8_t pct)
{
  /* disable
  backlight_pct = pct;
  // or connect to PWM output
  gpio_set_pin(BLK_PIN);
  */
}

void SPI_send(uint16_t len, uint8_t *data)
{
  gpio_clear_pin(CS_PIN);

  spi_write(0, data, len);

  gpio_set_pin(CS_PIN);
}

void SPI_TransmitCmd(uint16_t len, uint8_t *data)
{
  Pin_DC_Low();
  SPI_send(len, data);
}

void SPI_TransmitData(uint16_t len, uint8_t *data)
{
  Pin_DC_High();
  SPI_send(len, data);
}

void SPI_Transmit(uint16_t len, uint8_t *data)
{
  SPI_TransmitCmd(1, data++);
  if (--len)
    SPI_TransmitData(len, data);
}

void _Delay(uint32_t d)
{
  usleep(d * 1000);
}
