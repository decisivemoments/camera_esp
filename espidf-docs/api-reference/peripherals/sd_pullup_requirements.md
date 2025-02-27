# SD Pull-up Requirements


Espressif hardware products are designed for multiple use cases which may require different pull states on pins. For this reason, the pull state of particular pins on certain products needs to be adjusted to provide the pull-ups required in the SD bus.


SD pull-up requirements apply to cases where ESP32-S3 uses the SPI or SDMMC controller to communicate with SD cards. When an SD card is operating in SPI mode or 1-bit SD mode, the CMD and DATA (DAT0 - DAT3) lines of the SD bus must be pulled up by 10 kOhm resistors. SD cards and SDIO devices should also have pull-ups on all above-mentioned lines (regardless of whether these lines are connected to the host) in order to prevent them from entering a wrong state.


This document has the following structure:


* Overview of compatibility between the default pull states on pins of Espressif's products and the states required by the SD bus
* Solutions - ideas on how to resolve compatibility issues
* Related information - other relevant information



## Overview of Compatibility


This section provides an overview of compatibility issues that might occur when using SDIO (secure digital input output). Since the SD bus needs to be connected to pull-ups, these issues should be resolved regardless of whether they are related to master (host) or slave (device). Each issue has links to its respective solution. A solution for a host and device may differ.



### Systems on a Chip (SoCs)


ESP32-S3 SDMMC host controller allows using any of GPIOs for any of SD interface signals. However, it is recommended to avoid using strapping GPIOs, GPIOs with internal weak pull-downs and GPIOs commonly used for other purposes to prevent conflicts:


* GPIO0 (strapping pin)
* GPIO45, GPIO46 (strapping pins, internal weak pulldown)
* GPIO26 - GPIO32 (commonly used for SPI Flash and PSRAM)
* GPIO33 - GPIO37 (when using chips and modules with Octal SPI Flash or Octal PSRAM)
* GPIO43, GPIO44 (GPIOs used for UART0 by default)
* GPIO19, GPIO20 (GPIOs used for USB by default)




### Systems in Packages (SIP)




### Modules




### Development Boards


* ESP32-S3-DevKitC-1



> + No Pull-ups
* ESP32-S3-USB-OTG



> + The board may be used in 1-line and 4-line SD mode or SPI mode.
> 	+ Required pull-ups are provided on GPIOs 33-38.
* ESP32-S3-EYE



> + The board is limited to 1-line SD mode.
> 	+ Required pull-ups are provided on GPIOs 38-40.





## Solutions



### No Pull-ups


If you use a development board without pull-ups, you can do the following:


* If your host and slave device are on separate boards, replace one of them with a board that has pull-ups. For the list of Espressif's development boards with pull-ups, go to Development Boards.
* Attach external pull-ups by connecting each pin which requires a pull-up to VDD via a 10 kOhm resistor.





## Related Information