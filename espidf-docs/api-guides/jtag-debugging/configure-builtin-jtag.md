# Configure ESP32-S3 Built-in JTAG Interface



ESP32-S3 has a built-in JTAG circuitry and can be debugged without any additional chip. Only an USB cable connected to the D+/D- pins is necessary. The necessary connections are shown in the following section.



## Configure Hardware




ESP32-S3 pins and USB signals




| ESP32-S3 Pin | USB Signal |
| --- | --- |
| GPIO19 | D- |
| GPIO20 | D+ |
| 5V | V_BUS |
| GND | Ground |


Please verify that the ESP32-S3 pins used for USB communication are not connected to some other HW that may disturb the JTAG operation.




## Configure USB Drivers


JTAG communication should work on all supported platforms. Windows users might get LIBUSB_ERROR_NOT_FOUND errors. Please use version 2.8 (or newer) of the ESP-IDF Tools Installer and select the driver "Espressif - WinUSB support for JTAG (ESP32-C3/S3)" in order to resolve this issue. If you do not want to re-run the installer then the same can be achieved with idf-env by running the following command from PowerShell:



```
Invoke-WebRequest 'https://dl.espressif.com/dl/idf-env/idf-env.exe' -OutFile .\idf-env.exe; .\idf-env.exe driver install --espressif

```


On Linux adding OpenOCD udev rules is required and is done by placing the following udev rules file in the `/etc/udev/rules.d` folder.