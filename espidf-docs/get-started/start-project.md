# Build the Project


Build the project by running:



```
idf.py build

```


This command compiles the application and all ESP-IDF components, then it generates the bootloader, partition table, and application binaries.



```
$ idf.py build
Running cmake in directory /path/to/hello_world/build
Executing "cmake -G Ninja --warn-uninitialized /path/to/hello_world"...
Warn about uninitialized values.
-- Found Git: /usr/bin/git (found version "2.17.0")
-- Building empty aws_iot component due to configuration
-- Component names: ...
-- Component paths: ...

... (more lines of build system output)

[527/527] Generating hello_world.bin
esptool.py v2.3.1

Project build complete. To flash, run this command:
../../../components/esptool_py/esptool/esptool.py -p (PORT) -b 921600 write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x10000 build/hello_world.bin  build 0x1000 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin
or run 'idf.py -p PORT flash'

```


If there are no errors, the build finishes by generating the firmware binary .bin files.




# Flash onto the Device


To flash the binaries that you just built for the ESP32-S3 in the previous step, you need to run the following command:



```
idf.py -p PORT flash

```


Replace `PORT` with your ESP32-S3 board's USB port name. If the `PORT` is not defined, the idf.py will try to connect automatically using the available USB ports.


For more information on `idf.py` arguments, see idf.py.



Note


The option `flash` automatically builds and flashes the project, so running `idf.py build` is not necessary.



Encountered Issues While Flashing? See the "Additional Tips" below. You can also refer to Flashing Troubleshooting page or Establish Serial Connection with ESP32-S3 for more detailed information.



## Normal Operation


When flashing, you will see the output log similar to the following:



```
...
esptool.py esp32s3 -p /dev/ttyUSB0 -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x0 bootloader/bootloader.bin 0x10000 hello_world.bin 0x8000 partition_table/partition-table.bin
esptool.py v3.2-dev
Serial port /dev/ttyUSB0
Connecting....
Chip is ESP32-S3
Features: WiFi, BLE
Crystal is 40MHz
MAC: 7c:df:a1:e0:00:64
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00000000 to 0x00004fff...
Flash will be erased from 0x00010000 to 0x00039fff...
Flash will be erased from 0x00008000 to 0x00008fff...
Compressed 18896 bytes to 11758...
Writing at 0x00000000... (100 %)
Wrote 18896 bytes (11758 compressed) at 0x00000000 in 0.5 seconds (effective 279.9 kbit/s)...
Hash of data verified.
Compressed 168208 bytes to 88178...
Writing at 0x00010000... (16 %)
Writing at 0x0001a80f... (33 %)
Writing at 0x000201f1... (50 %)
Writing at 0x00025dcf... (66 %)
Writing at 0x0002d0be... (83 %)
Writing at 0x00036c07... (100 %)
Wrote 168208 bytes (88178 compressed) at 0x00010000 in 2.4 seconds (effective 569.2 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 103...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (103 compressed) at 0x00008000 in 0.1 seconds (effective 478.9 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
Done

```


If there are no issues by the end of the flash process, the board will reboot and start up the "hello_world" application.


If you would like to use the Eclipse or VS Code IDE instead of running `idf.py`, check out Eclipse Plugin, VSCode Extension.





# Monitor the Output


To check if "hello_world" is indeed running, type `idf.py -p PORT monitor` (Do not forget to replace PORT with your serial port name).


This command launches the IDF Monitor application.



```
$ idf.py -p <PORT> monitor
Running idf_monitor in directory [...]/esp/hello_world/build
Executing "python [...]/esp-idf/tools/idf_monitor.py -b 115200 [...]/esp/hello_world/build/hello_world.elf"...
--- idf_monitor on <PORT> 115200 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
ets Jun  8 2016 00:22:57
...

```


After startup and diagnostic logs scroll up, you should see "Hello world!" printed out by the application.



```
    ...
    Hello world!
    Restarting in 10 seconds...
    This is esp32s3 chip with 2 CPU core(s), WiFi/BLE, silicon revision 0, 2 MB embedded flash
Minimum free heap size: 390684 bytes
    Restarting in 9 seconds...
    Restarting in 8 seconds...
    Restarting in 7 seconds...

```


To exit IDF monitor use the shortcut `Ctrl+]`.



Note


You can combine building, flashing and monitoring into one step by running:



```
idf.py -p PORT flash monitor

```



See also:


* IDF Monitor for handy shortcuts and more details on using IDF monitor.
* idf.py for a full reference of `idf.py` commands and options.


**That is all that you need to get started with ESP32-S3!**


Now you are ready to try some other examples, or go straight to developing your own applications.



Important


Some of examples do not support ESP32-S3 because required hardware is not included in ESP32-S3 so it cannot be supported.


If building an example, please check the README file for the `Supported Targets` table. If this is present including ESP32-S3 target, or the table does not exist at all, the example will work on ESP32-S3.





# Additional Tips



## Permission Denied Issue


With some Linux distributions, you may get the error message similar to `Could not open port <PORT>: Permission denied: '<PORT>'` when flashing the ESP32-S3. This can be solved by adding the current user to the specific group, such as `dialout` or `uucp` group.




## Python Compatibility


ESP-IDF supports Python 3.8 or newer. It is recommended to upgrade your operating system to a recent version satisfying this requirement. Other options include the installation of Python from sources or the use of a Python version management system such as pyenv.




## Start with Board Support Package


To speed up prototyping on some development boards, you can use Board Support Packages (BSPs), which makes initialization of a particular board as easy as few function calls.


A BSP typically supports all of the hardware components provided on development board. Apart from the pinout definition and initialization functions, a BSP ships with drivers for the external components such as sensors, displays, audio codecs etc.


The BSPs are distributed via IDF Component Manager, so they can be found in IDF Component Registry.


**Here is an example of how to add ESP-BOX BSP to your project:**



```
idf.py add-dependency esp-box

```


More examples of BSP usage can be found in BSP examples folder.




## Flash Erase


Erasing the flash is also possible. To erase the entire flash memory you can run the following command:



```
idf.py -p PORT erase-flash

```


For erasing the OTA data, if present, you can run this command:



```
idf.py -p PORT erase-otadata

```


The flash erase command can take a while to be done. Do not disconnect your device while the flash erasing is in progress.