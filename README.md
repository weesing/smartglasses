# Overview
- This is a smart glasses prototyping project to use a MCU (Arduino/ESP32) or Raspberry Pi (SBC).
- **All resources here are WIP.**

# Folders
| Folder | Description
| -- | --
| STLs | Contains the STL files for 3D printing the parts of the smart glasses prototype.
| src | Contains source code for Arduino IDE based programming (suitable for Arduinos and ESP32) and Raspberry Pi.
| design | The design files used for producing the STLs. The current software used for design is Microsoft 3D Builder.

# Basic design
<img src="basic_design.jpg" style="width:400px">

# Showcase
## Initial prototype with Arduino BLE Nano
<ul>
<li>Design with Arduino BLE Nano MCU and 0.49" OLED (monochrome) screen.
  <div class="row">
    <div class="col-md-12">
    <img src="showcase_2.jpg" style="width:200px">
    <img src="showcase_3.jpg" style="width:200px">
    </div>
  </div>
</li>
<li>Code can be found in the sub-folder <code>src/arduino_ide</code></li>
</ul>

## Raspberry Pi Zero W console on the 0.96" TFT SPI display.
<ul>
<li>Displaying framebuffer of Raspbian Terminal on the display.</li>
  <div class="row">
    <div class="col-md-12">
    <img src="demo.gif" style="width:400px">
    </div>
    <div class="col-md-12">
    <img src="rpi_showcase_1.jpg" style="width:200px">
    <img src="rpi_showcase_2.jpg" style="width:200px">
    </div>
    <div class="col-md-12">
    <img src="rpi_showcase_3.jpg" style="width:400px">
    </div>
  </div>
<li>Code can be found in the folder <code>src/rpi</code></li>
</ul>