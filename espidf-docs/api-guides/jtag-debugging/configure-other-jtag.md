# Configure Other JTAG Interfaces



For guidance about which JTAG interface to select when using OpenOCD with ESP32-S3, refer to the section Selecting JTAG Adapter. Then follow the configuration steps below to get it working.



## Configure eFuses


By default, ESP32-S3 JTAG interface is connected to the built-in USB_SERIAL_JTAG peripheral. To use an external JTAG adapter instead, you need to switch the JTAG interface to the GPIO pins. This can be done by burning eFuses using `espefuse.py` tool.


* Burning `DIS_USB_JTAG` eFuse will permanently disable the connection between USB_SERIAL_JTAG and the JTAG port of the ESP32-S3. JTAG interface can then be connected to GPIO39-GPIO42. Note that USB CDC functionality of USB_SERIAL_JTAG will still be usable, i.e., flashing and monitoring over USB CDC will still work.
* Burning `STRAP_JTAG_SEL` eFuse will enable selection of JTAG interface by a strapping pin, GPIO3. If the strapping pin is low when ESP32-S3 is reset, JTAG interface will use GPIO39-GPIO42. If the strapping pin is high, USB_SERIAL_JTAG will be used as the JTAG interface.



Warning


Burning eFuses is an irreversible operation, so please consider the above option before starting the process.





## Configure Hardware


1. Identify all pins/signals on JTAG interface and ESP32-S3 board that should be connected to establish communication.



ESP32-S3 pins and JTAG signals




| ESP32-S3 Pin | JTAG Signal |
| --- | --- |
| MTDO / GPIO40 | TDO |
| MTDI / GPIO41 | TDI |
| MTCK / GPIO39 | TCK |
| MTMS / GPIO42 | TMS |
2. Verify if ESP32-S3 pins used for JTAG communication are not connected to some other hardware that may disturb JTAG operation.
3. Connect identified pin/signals of ESP32-S3 and JTAG interface.




## Configure Drivers


You may need to install driver software to make JTAG work with computer. Refer to documentation of your JTAG adapter for related details.


On Linux, adding OpenOCD udev rules is required and is done by copying the udev rules file into the `/etc/udev/rules.d` directory.




## Connect


Connect JTAG interface to the computer. Power on ESP32-S3 and JTAG interface boards. Check if the JTAG interface is visible on the computer.


To carry on with debugging environment setup, proceed to section Run OpenOCD.