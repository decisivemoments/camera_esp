# Using Debugger



This section covers the steps to configure and run a debugger using various methods, including:


* Eclipse
* Command Line
* Idf.py Debug Targets


For how to run a debugger from VS Code, see Configuration for Visual Studio Code Debug.



## Eclipse



Note


It is recommended to first check if debugger works using Idf.py Debug Targets or from Command Line and then move to using Eclipse.



Eclipse is an integrated development environment (IDE) that provides a powerful set of tools for developing and debugging software applications. For ESP-IDF applications, IDF Eclipse plugin provides two ways of debugging:


1. ESP-IDF GDB OpenOCD Debugging
2. GDB Hardware Debugging


By default, Eclipse supports OpenOCD Debugging via the GDB Hardware Debugging plugin, which requires starting the OpenOCD server from the command line and configuring the GDB client from Eclipse to start with the debugging. This approach can be time-consuming and error-prone.


To make the debugging process easier, the IDF Eclipse plugin has a customized ESP-IDF GDB OpenOCD Debugging functionality. This functionality supports configuring the OpenOCD server and GDB client from within Eclipse. All the required configuration parameters will be pre-filled by the plugin, and you can start debugging with just a click of a button.


Therefore, it is recommended to use the ESP-IDF GDB OpenOCD Debugging via the IDF Eclipse plugin.


**GDB Hardware Debugging**



Note


This approach is recommended only if you are unable to debug using ESP-IDF GDB OpenOCD Debugging for some reason.



To install the `GDB Hardware Debugging` plugin, open Eclipse and select `Help` > `Install` New Software.


After installation is complete, follow these steps to configure the debugging session. Please note that some configuration parameters are generic, while others are project-specific. This will be shown below by configuring debugging for "blink" example project. If not done already, add this project to Eclipse workspace following Eclipse Plugin. The source of get-started/blink application is available in examples directory of ESP-IDF repository.


1. In Eclipse, go to `Run` > `Debug Configuration`. A new window will open. In the left pane of the window, double-click `GDB Hardware Debugging` (or select `GDB Hardware Debugging` and press the `New` button) to create a new configuration.
2. In a form that will show up on the right, enter the `Name:` of this configuration, e.g., "Blink checking".
3. On the `Main` tab below, under `Project:`, press the `Browse` button and select the `blink` project.
4. In the next line under `C/C++ Application:`, press the `Browse` button and select the `blink.elf` file. If `blink.elf` is not there, it is likely that this project has not been built yet. Refer to the Eclipse Plugin for instructions.
5. Finally, under `Build (if required) before launching` click `Disable auto build`.


A sample window with settings entered in points 1 - 5 is shown below.



!Configuration of GDB Hardware Debugging - Main tab



Configuration of GDB Hardware Debugging - Main tab
6. Click the `Debugger` tab. In field `GDB Command`, enter `xtensa-esp32s3-elf-gdb` to invoke the debugger.
7. Change the default configuration of the `Remote host` by entering `3333` under the `Port number`.


Configuration entered in points 6 and 7 is shown on the following picture.



!Configuration of GDB Hardware Debugging - Debugger tab



Configuration of GDB Hardware Debugging - Debugger tab
8. The last tab that requires changing the default configuration is `Startup`. Under `Initialization Commands` uncheck `Reset and Delay (seconds)` and `Halt`. Then, in the entry field below, enter the following lines:



```
mon reset halt
maintenance flush register-cache
set remote hardware-watchpoint-limit 2

```



Note


To automatically update the image in the flash before starting a new debug session, add the following command lines to the beginning of the `Initialization Commands` textbox:



```
mon reset halt
mon program_esp ${workspace_loc:blink/build/blink.bin} 0x10000 verify

```



For description of `program_esp` command, see Upload Application for Debugging.
9. Uncheck the `Load image` option under `Load Image and Symbols`.
10. Further down on the same tab, establish an initial breakpoint to halt CPUs after they are reset by debugger. The plugin will set this breakpoint at the beginning of the function entered under `Set break point at:`. Checkout this option and enter `app_main` in provided field.
11. Checkout `Resume` option. This will make the program to resume after `mon reset halt` is invoked per point 8. The program will then stop at breakpoint inserted at `app_main`.


Configuration described in points 8 - 11 is shown below.



!Configuration of GDB Hardware Debugging - Startup tab



Configuration of GDB Hardware Debugging - Startup tab






If the `Startup` sequence looks convoluted and respective `Initialization Commands` are unclear, check What Is the Meaning of Debugger's Startup Commands? for additional explanation.
12. If you have completed the Configuring ESP32-S3 Target steps described above, so the target is running and ready to talk to debugger, go right to debugging by pressing `Debug` button. Otherwise press `Apply` to save changes, go back to Configuring ESP32-S3 Target and return here to start debugging.


Once all configuration steps 1-12 are satisfied, the new Eclipse perspective called "Debug" will open, as shown in the example picture below.



!Debug Perspective in Eclipse



Debug Perspective in Eclipse






If you are not quite sure how to use GDB, check Eclipse example debugging session in section Debugging Examples.




## Command Line


1. Begin by completing the steps described under Configuring ESP32-S3 Target. This is prerequisite to start a debugging session.


2. Open a new terminal session and go to the directory that contains the project for debugging, e.g.,



```
cd ~/esp/blink

```


3. When launching a debugger, you will need to provide a couple of configuration parameters and commands. Instead of entering them one by one in the command line, create a configuration file and name it `gdbinit`:



```
target remote :3333
set remote hardware-watchpoint-limit 2
mon reset halt
maintenance flush register-cache
thb app_main
c

```


Save this file in the current directory.


For more details on what is inside `gdbinit` file, see What Is the Meaning of Debugger's Startup Commands?


4. Now you are ready to launch GDB. Type the following in terminal:



```
xtensa-esp32s3-elf-gdb -x gdbinit build/blink.elf

```


5. If the previous steps have been done correctly, you will see a similar log concluded with the `(gdb)` prompt:



```
user-name@computer-name:~/esp/blink$ xtensa-esp32s3-elf-gdb -x gdbinit build/blink.elf
GNU gdb (crosstool-NG crosstool-ng-1.22.0-61-gab8375a) 7.10
Copyright (C) 2015 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=x86_64-build_pc-linux-gnu --target=xtensa-esp32s3-elf".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from build/blink.elf...done.
0x400d10d8 in esp_vApplicationIdleHook () at /home/user-name/esp/esp-idf/components/esp32s3/./freertos_hooks.c:52
52          asm("waiti 0");
JTAG tap: esp32s3.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
JTAG tap: esp32s3.slave tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
esp32s3: Debug controller was reset (pwrstat=0x5F, after clear 0x0F).
esp32s3: Core was reset (pwrstat=0x5F, after clear 0x0F).
Target halted. PRO_CPU: PC=0x5000004B (active)    APP_CPU: PC=0x00000000
esp32s3: target state: halted
esp32s3: Core was reset (pwrstat=0x1F, after clear 0x0F).
Target halted. PRO_CPU: PC=0x40000400 (active)    APP_CPU: PC=0x40000400
esp32s3: target state: halted
Hardware assisted breakpoint 1 at 0x400db717: file /home/user-name/esp/blink/main/./blink.c, line 43.
0x0:    0x00000000
Target halted. PRO_CPU: PC=0x400DB717 (active)    APP_CPU: PC=0x400D10D8
[New Thread 1073428656]
[New Thread 1073413708]
[New Thread 1073431316]
[New Thread 1073410672]
[New Thread 1073408876]
[New Thread 1073432196]
[New Thread 1073411552]
[Switching to Thread 1073411996]

Temporary breakpoint 1, app_main () at /home/user-name/esp/blink/main/./blink.c:43
43      xTaskCreate(&blink_task, "blink_task", 512, NULL, 5, NULL);
(gdb)

```


Note the third-to-last line, which shows debugger halting at breakpoint established in `gdbinit` file at function `app_main()`. Since the processor is halted, the LED should not be blinking. If this is what you see as well, you are ready to start debugging.


If you are not sure how to use GDB, check Command Line example debugging session in section Debugging Examples.




## Idf.py Debug Targets


It is also possible to execute the described debugging tools conveniently from `idf.py`. These commands are supported:


1. `idf.py openocd`


Runs OpenOCD in a console with configuration defined in the environment or via command line. It uses default script directory defined as `OPENOCD_SCRIPTS` environmental variable, which is automatically added from an Export script (`export.sh` or `export.bat`).
It is possible to override the script location using command line argument `--openocd-scripts`.


To configure the JTAG configuration for the current board, please use the environmental variable `OPENOCD_COMMANDS` or `--openocd-commands` command line argument. If none of the above is defined, OpenOCD is started with `-f board/esp32s3-builtin.cfg` board definition.
2. `idf.py gdb`


Starts the GDB the same way as the Command Line, but generates the initial GDB scripts referring to the current project elf file.
3. `idf.py gdbtui`


The same as 2, but starts the gdb with `tui` argument, allowing for a simple source code view.
4. `idf.py gdbgui`


Starts gdbgui debugger frontend enabling out-of-the-box debugging in a browser window. To enable this option, run the install script with the "--enable-gdbgui" argument, e.g., `install.sh --enable-gdbgui`.


You can combine these debugging actions on a single command line, allowing for convenient setup of blocking and non-blocking actions in one step. `idf.py` implements a simple logic to move the background actions (such as openocd) to the beginning and the interactive ones (such as gdb, monitor) to the end of the action list.


An example of a very useful combination is:



```
idf.py openocd gdbgui monitor

```


The above command runs OpenOCD in the background, starts gdbgui to open a browser window with active debugger frontend and opens a serial monitor in the active console.