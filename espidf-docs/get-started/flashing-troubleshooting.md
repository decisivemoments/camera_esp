# Flashing Troubleshooting




## Failed to Connect


If you run the given command and see errors such as "Failed to connect", there might be several reasons for this. One of the reasons might be issues encountered by `esptool.py`, the utility that is called by the build system to reset the chip, interact with the ROM bootloader, and flash firmware. One simple solution to try is to manually reset as described below. If it does not help, you can find more details about possible issues in the esptool troubleshooting page.


`esptool.py` resets ESP32-S3 automatically by asserting DTR and RTS control lines of the USB-to-UART bridge, i.e., FTDI or CP210x (for more information, see Establish Serial Connection with ESP32-S3). The DTR and RTS control lines are in turn connected to `GPIO0` and `CHIP_PU` (EN) pins of ESP32-S3, thus changes in the voltage levels of DTR and RTS will boot ESP32-S3 into Firmware Download mode. As an example, check the schematic for the ESP32 DevKitC development board.


In general, you should have no problems with the official esp-idf development boards. However, `esptool.py` is not able to reset your hardware automatically in the following cases:


* Your hardware does not have the DTR and RTS lines connected to `GPIO0` and `CHIP_PU`.
* The DTR and RTS lines are configured differently.
* There are no such serial control lines at all.


Depending on the kind of hardware you have, it may also be possible to manually put your ESP32-S3 board into Firmware Download mode (reset).


* For development boards produced by Espressif, this information can be found in the respective getting started guides or user guides. For example, to manually reset an ESP-IDF development board, hold down the `Boot` button (`GPIO0`) and press the `EN` button (`CHIP_PU`).
* For other types of hardware, try pulling `GPIO0` down.