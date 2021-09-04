# Smartglasses ESP32 Software

* Uses SPI TFT Color Display 80x160 pixels (ST7735S) (Blue Board)

| ST7735S | ESP32
| -- | --
| VCC | 3v3
| Gnd | Gnd
| SCK | Pin 14 (Default SPI)
| SDA | Pin 13 (Default SPI)
| DC | Pin 2
| RST | Pin 4
| CS | Pin 15
| BLK | None

* The source file `st7735s.py` is the **ST7735S** driver source.
* Ported from https://github.com/cheungbx/st7735-esp8266-micropython to support a new `BLUETAB80x160` configuration that displays with the orientation where the Pins on the board is at the *top*.

## Source
| File | Description
|--|--
| `boot.py` | Bootstrap code. Connection to Wifi. See `wifi_cfg.sample.py`
| `wifi_cfg.sample.py` | A sample source file containing Wifi configuration. Should rename this to `wifi_cfg.py`
| `main.py` | The main entry point.
| `sysfont.py` | Font file for displaying text on screen (credits: https://github.com/GuyCarver/MicroPython/tree/master/esp8266)
| `st7735s.py` | ST7735S display driver. Adapted from repository https://github.com/cheungbx/st7735-esp8266-micropython
