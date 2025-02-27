# USB Serial/JTAG Controller Console



Generally, ESP chips implement a serial port using UART and can be connected to a serial console emulator on a host/PC via an external USB-UART bridge chip. However, on ESP chips that contain a USB Serial/JTAG Controller, the CDC-ACM portion of the controller implements a serial port that is connected directly to a host/PC, thus does not require an external USB-UART bridge chip.


ESP32-S3 contains a USB Serial/JTAG Controller providing the following functions:


* Bidirectional serial console, which can be used with IDF Monitor or another serial monitor.
* Flashing using `esptool.py` and `idf.py flash`.
* JTAG debugging, performed simultaneously with serial operations using tools like OpenOCD.



Note


The USB Serial/JTAG Controller is a fixed-function USB device that is implemented entirely in hardware, meaning that it cannot be reconfigured to perform any function other than a serial port and JTAG debugging functionality. This is in contrast to the USB OTG controllers in some ESP chips that can be configured to perform the function of multiple types of USB devices.




## Hardware Requirements


Connect ESP32-S3 to the USB port as follows:







| GPIO | USB |
| --- | --- |
| 20 | D+ (green) |
| 19 | D- (white) |
| GND | GND (black) |
| 5V (or externally supplied) | +5V (red) |


Some development boards may offer a USB connector for the USB Serial/JTAG Controller. In that case, no extra connections are required.




## Software Configuration


The USB Serial/JTAG Controller can be used as the serial port by selecting `CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG` in the CONFIG_ESP_CONSOLE_UART option. Once selected, building and flashing the project can occur as usual.


Alternatively, you can access the output through the `usb_serial_jtag` port but make sure `CONFIG_ESP_CONSOLE_SECONDARY_USB_SERIAL_JTAG` is selected in the CONFIG_ESP_CONSOLE_SECONDARY.



Warning


Besides output, if you also want to input or use REPL with the console, please select `CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG`.





## Uploading the Application


The USB Serial/JTAG Controller is able to put the ESP32-S3 into download mode automatically. Simply flash as usual, but specify the USB Serial/JTAG Controller port on your system: `idf.py flash -p PORT`, where `PORT` is the name of the proper port.



Note


The USB Serial/JTAG Controller's serial port usually appears:


* as `/dev/ttyACM*` on Linux
* as `/dev/cu*` on Mac
* as a `COM*` port in the Windows Device Manager





## Limitations


There are several limitations to the USB Serial/JTAG console feature. The significance of these limitations depends on the type of application being developed, and the development workflow.



### USB Pin Reconfiguration


If the application accidentally reconfigures the USB peripheral pins or disables the USB Serial/JTAG Controller, the device disappears from the system. After fixing the issue in the application, you need to manually put the ESP32-S3 into download mode by pulling low GPIO0 and resetting the chip.


If the application enters Deep-sleep mode, the USB Serial/JTAG device disappears from the system.




### Data Buffering


For data transmitted from ESP32-S3 to PC Terminal (e.g., stdout, logs), the ESP32-S3 first writes to a small internal buffer. After this buffer becomes full (for example, if no PC Terminal is connected), the ESP32-S3 does a one-time wait of 50 ms for the PC Terminal to request the data. This can appear as a very brief pause in your application.


For data transmitted from the PC Terminal to ESP32-S3 (e.g., console commands), many PC Terminals wait for the ESP32-S3 to ingest the bytes before allowing you to send more data. This is in contrast to using a USB-to-Serial (UART) bridge chip, which always ingests the bytes and sends them to a (possibly not listening) ESP32-S3.



Note


In rare cases, it is possible that data sent from ESP32-S3 to the host gets 'stuck' in host memory. Sending more data will get it 'unstuck', but if the application does not send more data, depending on the driver, this data needs to be flushed to the host manually. The non-blocking (default) driver and the VFS implementation will flush automatically after a newline. The blocking (interrupt-based) driver will automatically flush when its transmit buffer becomes empty.





### Sleep Mode Considerations


The USB Serial/JTAG controller and its associated USB PHY are driven by particular clocks (e.g., APB and USB PHY clock) and belong to a particular power domain (e.g., digital power domain). Thus, any change to the clock and power domains associated with the USB Serial/JTAG controller, such as entering different sleep modes, can affect the controller's operation.



#### Deep-sleep


When entering Deep-sleep, both the USB Serial/JTAG controller and the USB PHY are powered off, leading to the USB PHY's D+ line no longer being pulled up. As a result:


* When entering Deep-sleep, the USB Serial/JTAG device appears disconnected from the host/PC (even if the USB cable is still physically connected).
* When exiting Deep-sleep, the USB Serial/JTAG device reconnects to the host/PC.




#### Light-sleep


When entering Light-sleep, the APB and USB PHY clock are gated. Thus, the USB Serial/JTAG controller is no longer able to receive or respond to any USB transactions from the connected host (including periodic CDC Data IN transactions). As a result:


* when entering Light-sleep, the USB Serial/JTAG device is unresponsive to the host/PC's USB CDC driver. The host/PC may then report the USB Serial/JTAG device as disconnected or erroneous (even if the USB cable is still physically connected).
* when exiting Light-sleep, it is possible that the host/PC does not re-enumerate (i.e., reconnect) the USB Serial/JTAG device given that the USB PHY's D+ line remains pulled up state during Light-sleep. Users may need to physically disconnect and then reconnect the USB cable.




#### Automatic and Manual Sleep Entry


If users enter sleep manually (via `esp_light_sleep_start()` or `esp_deep_sleep_start()`), users should be cognizant of the fact that USB Serial/JTAG controller does not work during sleep. ESP-IDF **does not add any safety check to reject entry to sleep** even if the USB Serial/JTAG controller is connected. In the case where sleep is entered while the USB Serial/JTAG controller is connected, the connection can be re-established by unplugging and re-plugging the USB cable.


If users enter sleep automatically (via `esp_pm_configure()`), enabling the CONFIG_USJ_NO_AUTO_LS_ON_CONNECTION option allows the ESP32-S3 to automatically detect whether the USB Serial/JTAG controller is currently connected to a host, and prevent automatic entry to sleep as long as the connection persists. However, note that this option increases power consumption.