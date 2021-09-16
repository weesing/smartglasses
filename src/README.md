# Smart Glasses Source Code

## Overview

- This folder contains the **source code** for the smart glasses project.
- Display used is the **ST7735S TFT display (0.96" 80x160)**.
- Look into the different sub-folders for the platform used. Currently evaluated platforms includes (but not limited to) **Arduino Nano BLE**, **ESP32 w Micropython** and **Raspberry Pi Zero W**.

## Sub-folders

| Folder           | Description
| -- | --
| arduino_ide | Contains the sketch for using an Arduino BLE Nano MCU or ESP32 to connect to SPI ST7735S TFT (0.96" 80x160) display using the Arduino IDE
| rpi            | Contains the software for mirroring Raspberry Pi framebuffer to the ST7735S TFT display.

# Findings Arduino BLE Nano/ESP32
- Arduino containing the compiled software updates the screen faster.
- ESP32 with Micropython is easier to develop due to the nature of scripting language (Python). BLE is also easier to configure and program (Arduino BLE programming requires more manual steps)
- Arduino BLE reception is limited to 20 bytes per packet (unable to configure so far), whereas ESP32 BLE allows configuration of the length of the packet size.
- Parsing of JSON (communication data format) is easier in Micropython/Python with readily built libraries. Since JSON is more verbose, ESP32's configurable data packet size (_see above_) has an advantage.
- ESP32 has wireless which opens other possibilities for integration with other devices for more info display on the smart glasses.
- Physical dimensions of ESP32 is twice of Arduino BLE Nano in terms of width, which will make the final design a little bit more bulky.
- Arduino IDE programming for ESP32 seems to be a best candidate after considering the above.

# Findings for Raspberry Pi Zero W
- Display is very tiny for displaying even the console.
- There are more possibilities (such as running a full blown API server) on a Raspberry Pi.
- The physical footprint between ESP32 and RPi Zero W is quite similar, but given the extensibility of a full-blown computer, the RPi Zero W option seems to be much better.

# TODO

- Arduino BLE configuration and initialization
- Arduino BLE Nano data receiving and JSON parsing (data format for communication)
