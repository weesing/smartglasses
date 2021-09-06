# Smart Glasses Source Code

## Overview

- This folder contains the **source code** for the smart glasses project.
- Display used is the **ST7735S TFT display (0.96" 80x160)**.
- Look into the different sub-folders for the platform used. Currently evaluated platforms includes (but not limited to) **Arduino Nano BLE** and **ESP32 w Micropython**.

## Sub-folders

| Folder           | Description
| -- | --
| arduino_nano_ble | Contains the sketch for using an Arduino BLE Nano MCU to connect to SPI ST7735S TFT (0.96" 80x160) display
| esp32            | Contains the Micropython code for use on an ESP32 to connect to SPI ST7735S TFT (0.96" 80x160) display

# Findings

- Arduino containing the compiled software updates the screen faster.
- ESP32 with Micropython is easier to develop due to the nature of scripting language (Python). BLE is also easier to configure and program (Arduino BLE programming requires more manual steps)
- Arduino BLE reception is limited to 20 bytes per packet (unable to configure so far), whereas ESP32 BLE allows configuration of the length of the packet size.
- Parsing of JSON (communication data format) is easier in Micropython/Python with readily built libraries. Since JSON is more verbose, ESP32's configurable data packet size (_see above_) has an advantage.
- ESP32 has wireless which opens other possibilities for integration with other devices for more info display on the smart glasses.
- Physical dimensions of ESP32 is twice of Arduino BLE Nano in terms of width, which will make the final design a little bit more bulky.
- Arduino IDE programming for ESP32 seems to be a best candidate after considering the above.

# TODO

- Arduino BLE configuration and initialization
- Arduino BLE Nano data receiving and JSON parsing (data format for communication)
