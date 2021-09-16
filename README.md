# Overview
- This is a smart glasses prototyping project to use a MCU (Arduino/ESP32) or Raspberry Pi (SBC).
- **All resources here are WIP.**

# Folders
| Folder | Description
| -- | --
| STLs | Contains the STL files for 3D printing the parts of the smart glasses prototype.
| src | Contains source code for Arduino IDE based programming (suitable for Arduinos and ESP32) and Raspberry Pi.
| design | The design files used for producing the STLs. The current software used for design is Microsoft 3D Builder.

# Showcase
## Initial prototype with Arduino BLE Nano
- Design with Arduino BLE Nano MCU and 0.49" OLED (monochrome) screen.<br/>
<img src="showcase_2.jpg" style="width:200px">
<img src="showcase_3.jpg" style="width:200px">
- Code can be found in the sub-folder `src/arduino_ide`<br/>

## Raspberry Pi Zero W console on the 0.96" TFT SPI display.
- Displaying framebuffer of Raspbian Terminal on the display.<br/>
<img src="demo.gif" style="width:400px"><br/>
<img src="rpi_showcase_1.jpg" style="width:200px">
<img src="rpi_showcase_2.jpg" style="width:200px"><br/>
<img src="rpi_showcase_3.jpg" style="width:400px"><br/>
- Code can be found in the folder `src/rpi`<br/>

