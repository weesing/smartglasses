# Overview
This project contains software that mirrors your Raspberry Pi's framebuffer (where a virtual screen resides) to an ST7735S 0.96" screen (80x160 resolution).

The software is part of the effort for display the Raspberry Pi's framebuffer for the smart glasses.

Source code originated & modified from the following repositories:
- https://github.com/vinodstanur/raspberry-pi-frame-buffer-mapping-to-160x128-ST7735R-LCD
- https://github.com/bersch/ST7735S 

# Pins Assignment
*Note: We are using SPI0 channel.*
| ST7355S | Raspberry Pi Zero W
| -- | --
| VCC | 3v
| GND | GND
| SCL | SLCK (GPIO 11)
| SDA | MOSI (GPIO 10)
| CS | GPIO 8 (can be configured in st7735.h)
| RST | GPIO 25 (can be configured in st7735.h)
| DC | GPIO 24 (can be configured in st7735.h)

# How to run
- Connect the ST7735S to your Raspberry Pi as described above.
- Change the framebuffer in `/boot/config.txt` to force the framebuffer width and height to 160 x 80 (you will need root access).
```
... (other settings)
framebuffer_width=160
framebuffer_height=80
... (other settings)
```
- Change to the project folder.
- Run `make`
- Run `./run.sh`

# Screenshots
<img src="demo_screenshot.jpg" alt="drawing" style="width:400px;"/>

# References
- Guide - https://learn.adafruit.com/1-8-tft-display/python-wiring-and-setup
- Python Library - https://github.com/adafruit/Adafruit_CircuitPython_RGB_Display

# Improvements
- To use pixel interpolation to translate higher resolution framebuffer to 160x80 resolution.