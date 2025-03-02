# README

# Arduino How To

- [Download](https://www.arduino.cc/en/software) the Arduino IDE

## Install the ESP3266 Board

- Follow these [instructions](https://support.arduino.cc/hc/en-us/articles/360016466340-Add-third-party-platforms-to-the-Boards-Manager-in-Arduino-IDE#add-or-remove-additional-board-manager-urls) to add an a 3rd party board
- Paste this url where in the Additional Boards Manager Urls window shown in the instructions above: https://arduino.esp8266.com/stable/package_esp8266com_index.json
- Follow the link at the bottom of the page: "You can now install the platform with **Boards Manager** by following the instructions [**here**](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-board-manager/#installing-a-board-package)" 👈 Or click there.
  - You only need the "## Installing a Board Package" section.
  - ℹ️ Just below this is a link to the official [Unofficial list of 3rd party boards support urls](https://github.com/arduino/Arduino/wiki/Unofficial-list-of-3rd-party-boards-support-urls), where the JSON url above was found.
- We are using the ESP8266 microcontroller (MCU), so search for "ESP8266" in the boards manager.

## Select the Newly Installed Board

- Follow these [instructions](https://support.arduino.cc/hc/en-us/articles/4406856349970-Select-board-and-port-in-Arduino-IDE#other-board-and-port) to install an "unknown" (3rd party) board.
- BOARDS option: "NodeMCU 1.0 (ESP-12E Module)"
- PORTS option: The board we want should have an address similar to "/dev/cu.usbserial-3"

## Hello World

We want to check if the IDE is properly configured to compile to the assembly language that's "native" to the chosen board. To do this, we'll compile, upload, and run the Arduino equivalent of a "Hello, World!"

- With the board selected, File > Examples > Basics > Blink
- In the new window, press the upload button
- ![[arduino-button-upload.png]]
- If successful, the built-in LED should toggle on and off every second.

## Setup the ArduinoIoTCloud Lib

- Follow these [instructions](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library/) to install the "ArduinoIoTCloud" lib using the library manager. Be sure to install all dependencies when prompted.
- Find the most example on Github

## Build out the Basic Circuit

- Mount the MCU board and the power supply to the bread board. The usb ports (PSU and MCU board) should be facing outward
- Connect the low V side of the relay to the bread board and MCU board
