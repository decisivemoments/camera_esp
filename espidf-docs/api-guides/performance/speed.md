# Speed Optimization




## Overview


Optimizing execution speed is a key element of software performance. Code that executes faster can also have other positive effects, e.g., reducing overall power consumption. However, improving execution speed may have trade-offs with other aspects of performance such as Minimizing Binary Size.




## Choose What to Optimize


If a function in the application firmware is executed once per week in the background, it may not matter if that function takes 10 ms or 100 ms to execute. If a function is executed constantly at 10 Hz, it matters greatly if it takes 10 ms or 100 ms to execute.


Most kinds of application firmware only have a small set of functions that require optimal performance. Perhaps those functions are executed very often, or have to meet some application requirements for latency or throughput. Optimization efforts should be targeted at these particular functions.




## Measuring Performance


The first step to improving something is to measure it.



### Basic Performance Measurements


You may be able to measure directly the performance relative to an external interaction with the world, e.g., see the examples wifi/iperf and ethernet/iperf for measuring general network performance. Or you can use an oscilloscope or logic analyzer to measure the timing of an interaction with a device peripheral.


Otherwise, one way to measure performance is to augment the code to take timing measurements:



```
#include "esp_timer.h"

void measure_important_function(void) {
    const unsigned MEASUREMENTS = 5000;
    uint64_t start = esp_timer_get_time();

    for (int retries = 0; retries < MEASUREMENTS; retries++) {
        important_function(); // This is the thing you need to measure
    }

    uint64_t end = esp_timer_get_time();

    printf("%u iterations took %llu milliseconds (%llu microseconds per invocation)\n",
           MEASUREMENTS, (end - start)/1000, (end - start)/MEASUREMENTS);
}

```


Executing the target multiple times can help average out factors, e.g., RTOS context switches, overhead of measurements, etc.


* Using `esp_timer_get_time()` generates "wall clock" timestamps with microsecond precision, but has moderate overhead each time the timing functions are called.
* It is also possible to use the standard Unix `gettimeofday()` and `utime()` functions, although the overhead is slightly higher.
* Otherwise, including `hal/cpu_hal.h` and calling the HAL function `cpu_hal_get_cycle_count()` returns the number of CPU cycles executed. This function has lower overhead than the others, which is good for measuring very short execution times with high precision.


The CPU cycles are counted per-core, so only use this method from an interrupt handler, or a task that is pinned to a single core.
* While performing "microbenchmarks" (i.e., benchmarking only a very small routine of code that runs in less than 1-2 milliseconds), the flash cache performance can sometimes cause big variations in timing measurements depending on the binary. This happens because binary layout can cause different patterns of cache misses in a particular sequence of execution. If the test code is larger, then this effect usually averages out. Executing a small function multiple times when benchmarking can help reduce the impact of flash cache misses. Alternatively, move this code to IRAM (see Targeted Optimizations).




### External Tracing


The Application Level Tracing Library allows measuring code execution with minimal impact on the code itself.




### Tasks


If the option CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS is enabled, then the FreeRTOS API `vTaskGetRunTimeStats()` can be used to retrieve runtime information about the processor time used by each FreeRTOS task.


SEGGER SystemView is an excellent tool for visualizing task execution and looking for performance issues or improvements in the system as a whole.





## Improving Overall Speed


The following optimizations improve the execution of nearly all code, including boot times, throughput, latency, etc:


* Set CONFIG_ESPTOOLPY_FLASHMODE to QIO or QOUT mode (Quad I/O). Both almost double the speed at which code is loaded or executed from flash compared to the default DIO mode. QIO is slightly faster than QOUT if both are supported. Note that both the flash chip model, and the electrical connections between the ESP32-S3 and the flash chip must support quad I/O modes or the SoC will not work correctly.
* Set CONFIG_COMPILER_OPTIMIZATION to `Optimize for performance (-O2)` . This may slightly increase binary size compared to the default setting, but almost certainly increases the performance of some code. Note that if your code contains C or C++ Undefined Behavior, then increasing the compiler optimization level may expose bugs that otherwise are not seen.
* Avoid using floating point arithmetic `float`. Even though ESP32-S3 has a single precision hardware floating point unit, floating point calculations are always slower than integer calculations. If possible then use fixed point representations, a different method of integer representation, or convert part of the calculation to be integer only before switching to floating point.
* Avoid using double precision floating point arithmetic `double`. These calculations are emulated in software and are very slow. If possible then use an integer-based representation, or single-precision floating point.






### Change cache size


On ESP32-S3, increasing the overall speed can be achieved to some degree by increasing the size of cache and thus potentially decreasing the frequency of "cache misses" through the Kconfig option(s) listed below.


* CONFIG_ESP32S3_INSTRUCTION_CACHE_SIZE.
* CONFIG_ESP32S3_DATA_CACHE_SIZE.






Note


Increasing the cache size will also result in reducing the amount of available RAM.





### Reduce Logging Overhead


Although standard output is buffered, it is possible for an application to be limited by the rate at which it can print data to log output once buffers are full. This is particularly relevant for startup time if a lot of output is logged, but such problem can happen at other times as well. There are multiple ways to solve this problem:


* Reduce the volume of log output by lowering the app CONFIG_LOG_DEFAULT_LEVEL (the equivalent bootloader setting is CONFIG_BOOTLOADER_LOG_LEVEL). This also reduces the binary size, and saves some CPU time spent on string formatting.
* Increase the speed of logging output by increasing the CONFIG_ESP_CONSOLE_UART_BAUDRATE. However, if you are using internal USB-CDC, the serial throughput is not dependent on the configured baud rate.







### Not Recommended


The following options also increase execution speed, but are not recommended as they also reduce the debuggability of the firmware application and may increase the severity of any bugs.


* Set CONFIG_COMPILER_OPTIMIZATION_ASSERTION_LEVEL to disabled. This also reduces firmware binary size by a small amount. However, it may increase the severity of bugs in the firmware including security-related bugs. If it is necessary to do this to optimize a particular function, consider adding `#define NDEBUG` at the top of that single source file instead.








## Targeted Optimizations


The following changes increase the speed of a chosen part of the firmware application:


* Move frequently executed code to IRAM. By default, all code in the app is executed from flash cache. This means that it is possible for the CPU to have to wait on a "cache miss" while the next instructions are loaded from flash. Functions which are copied into IRAM are loaded once at boot time, and then always execute at full speed.


IRAM is a limited resource, and using more IRAM may reduce available DRAM, so a strategic approach is needed when moving code to IRAM. See IRAM (Instruction RAM) for more information.
* Jump table optimizations can be re-enabled for individual source files that do not need to be placed in IRAM. For hot paths in large `switch cases`, this improves performance. For instructions on how to add the `-fjump-tables` and `-ftree-switch-conversion` options when compiling individual source files, see Controlling Component Compilation







## Improving Startup Time


In addition to the overall performance improvements shown above, the following options can be tweaked to specifically reduce startup time:


* Minimizing the CONFIG_LOG_DEFAULT_LEVEL and CONFIG_BOOTLOADER_LOG_LEVEL has a large impact on startup time. To enable more logging after the app starts up, set the CONFIG_LOG_MAXIMUM_LEVEL as well, and then call `esp_log_level_set()` to restore higher level logs. The system/startup_time main function shows how to do this.
* If using Deep-sleep mode, setting CONFIG_BOOTLOADER_SKIP_VALIDATE_IN_DEEP_SLEEP allows a faster wake from sleep. Note that if using Secure Boot, this represents a security compromise, as Secure Boot validation are not be performed on wake.
* Setting CONFIG_BOOTLOADER_SKIP_VALIDATE_ON_POWER_ON skips verifying the binary on every boot from the power-on reset. How much time this saves depends on the binary size and the flash settings. Note that this setting carries some risk if the flash becomes corrupt unexpectedly. Read the help text of the config item for an explanation and recommendations if using this option.
* It is possible to save a small amount of time during boot by disabling RTC slow clock calibration. To do so, set CONFIG_RTC_CLK_CAL_CYCLES to 0. Any part of the firmware that uses RTC slow clock as a timing source will be less accurate as a result.
* When external memory is used (CONFIG_SPIRAM enabled), enabling memory test on the external memory (CONFIG_SPIRAM_MEMTEST) can have a large impact on startup time (approximately 1 second per 4 MB of memory tested). Disabling the memory tests will reduce startup time at the expense of testing the external memory.
* When external memory is used (CONFIG_SPIRAM enabled), enabling comprehensive poisoning will increase the startup time (approximately 300 milliseconds per 4 MiB of memory set) since all the memory used as heap (including the external memory) will be set to a default value.





The example project system/startup_time is pre-configured to optimize startup time. The file system/startup_time/sdkconfig.defaults contain all of these settings. You can append these to the end of your project's own `sdkconfig` file to merge the settings, but please read the documentation for each setting first.




## Task Priorities


As ESP-IDF FreeRTOS is a real-time operating system, it is necessary to ensure that high-throughput or low-slatency tasks are granted a high priority in order to run immediately. Priority is set when calling `xTaskCreate()` or `xTaskCreatePinnedToCore()` and can be changed at runtime by calling `vTaskPrioritySet()`.


It is also necessary to ensure that tasks yield CPU (by calling `vTaskDelay()`, `sleep()`, or by blocking on semaphores, queues, task notifications, etc) in order to not starve lower-priority tasks and cause problems for the overall system. The Task Watchdog Timer (TWDT) provides a mechanism to automatically detect if task starvation happens. However, note that a TWDT timeout does not always indicate a problem, because sometimes the correct operation of the firmware requires some long-running computation. In these cases, tweaking the TWDT timeout or even disabling the TWDT may be necessary.



### Built-in Task Priorities


ESP-IDF starts a number of system tasks at fixed priority levels. Some are automatically started during the boot process, while some are started only if the application firmware initializes a particular feature. To optimize performance, structure the task priorities of your application properly to ensure the tasks are not delayed by the system tasks, while also not starving system tasks and impacting other functions of the system.


This may require splitting up a particular task. For example, perform a time-critical operation in a high-priority task or an interrupt handler and do the non-time-critical part in a lower-priority task.


Header components/esp_system/include/esp_task.h contains macros for the priority levels used for built-in ESP-IDF tasks system. See Background Tasks for more details about the system tasks.


Common priorities are:


* Running the Main Task that executes app_main function has minimum priority (1). This task is pinned to Core 0 by default (configurable).
* ESP Timer (High Resolution Timer) system task to manage high precision timer events and execute callbacks has high priority (22, `ESP_TASK_TIMER_PRIO`). This task is pinned to Core 0.
* FreeRTOS Timer Task to handle FreeRTOS timer callbacks is created when the scheduler initializes and has minimum task priority (1, configurable). This task is pinned to Core 0.
* Event Loop Library system task to manage the default system event loop and execute callbacks has high priority (20, `ESP_TASK_EVENT_PRIO`) and it is pinned to Core 0. This configuration is only used if the application calls `esp_event_loop_create_default()`, it is possible to call `esp_event_loop_create()` with a custom task configuration instead.
* lwIP TCP/IP task has high priority (18, `ESP_TASK_TCPIP_PRIO`) and is not pinned to any core (configurable).
* Wi-Fi Driver task has high priority (23) and is pinned to Core 0 by default (configurable).
* Wi-Fi wpa_supplicant component may create dedicated tasks while the Wi-Fi Protected Setup (WPS), WPA2 EAP-TLS, Device Provisioning Protocol (DPP) or BSS Transition Management (BTM) features are in use. These tasks all have low priority (2) and are not pinned to any core.
* Controller && VHCI task has high priority (23, `ESP_TASK_BT_CONTROLLER_PRIO`) and is pinned to Core 0 by default (configurable). The Bluetooth Controller needs to respond to requests with low latency, so it should always be among the highest priority task assigned to a single CPU.
* NimBLE-based Host APIs task has high priority (21) and is pinned to Core 0 by default (configurable).
	+ Stack event callback task ("BTC") has high priority (19).
	+ Stack BTU layer task has high priority (20).
	+ Host HCI host task has high priority (22).
All Bluedroid Tasks are pinned to the same core, which is Core 0 by default (configurable).
* The Ethernet driver creates a task for the MAC to receive Ethernet frames. If using the default config `ETH_MAC_DEFAULT_CONFIG` then the priority is medium-high (15) and the task is not pinned to any core. These settings can be changed by passing a custom `eth_mac_config_t` struct when initializing the Ethernet MAC.
* If using the ESP-MQTT component, it creates a task with default priority 5 (configurable, depending on CONFIG_MQTT_USE_CUSTOM_CONFIG) and not pinned to any core (configurable).
* To see what is the task priority for `mDNS` service, please check Performance Optimization.







### Choosing Task Priorities of the Application



Note


Task execution is always completely suspended when writing to the built-in SPI flash chip. Only IRAM-Safe Interrupt Handlers continues executing.






## Improving Interrupt Performance


ESP-IDF supports dynamic Interrupt Allocation with interrupt preemption. Each interrupt in the system has a priority, and higher-priority interrupts preempts lower priority ones.


Interrupt handlers execute in preference to any task, provided the task is not inside a critical section. For this reason, it is important to minimize the amount of time spent in executing an interrupt handler.


To obtain the best performance for a particular interrupt handler:


* Assign more important interrupts a higher priority using a flag such as `ESP_INTR_FLAG_LEVEL2` or `ESP_INTR_FLAG_LEVEL3` when calling `esp_intr_alloc()`.
* Assign the interrupt on a CPU where built-in Wi-Fi/Bluetooth tasks are not configured to run, which means assigning the interrupt on Core 1 by default, see Built-in Task Priorities. Interrupts are assigned on the same CPU where the `esp_intr_alloc()` function call is made.
* If you are sure the entire interrupt handler can run from IRAM (see IRAM-Safe Interrupt Handlers) then set the `ESP_INTR_FLAG_IRAM` flag when calling `esp_intr_alloc()` to assign the interrupt. This prevents it being temporarily disabled if the application firmware writes to the internal SPI flash.
* Even if the interrupt handler is not IRAM-safe, if it is going to be executed frequently then consider moving the handler function to IRAM anyhow. This minimizes the chance of a flash cache miss when the interrupt code is executed (see Targeted Optimizations). It is possible to do this without adding the `ESP_INTR_FLAG_IRAM` flag to mark the interrupt as IRAM-safe, if only part of the handler is guaranteed to be in IRAM.







## Improving Network Speed


* For Wi-Fi, see How to Improve Wi-Fi Performance and Wi-Fi Buffer Usage
* For lwIP TCP/IP, see Performance Optimization
* The wifi/iperf example contains a configuration that is heavily optimized for Wi-Fi TCP/IP throughput, usually at the expense of higher RAM usage. Append the contents of the files wifi/iperf/sdkconfig.defaults, wifi/iperf/sdkconfig.defaults.esp32s3 and wifi/iperf/sdkconfig.ci.99 to the `sdkconfig` file in your project in order to add all of these options. Note that some of these options may have trade-offs in terms of reduced debuggability, increased firmware size, increased memory usage, or reduced performance of other features. To get the best result, read the documentation pages linked above and use related information to determine exactly which options are best suited for your app.







## Improving I/O Performance


Using standard C library functions like `fread` and `fwrite` instead of platform specific unbuffered syscalls such as `read` and `write` can be slow.These functions are designed to be portable, so they are not necessarily optimized for speed, have a certain overhead and are buffered.


FAT Filesystem Support specific information and tips:


* Maximum size of the R/W request = FatFS cluster size (allocation unit size).
* Use `read` and `write` instead of `fread` and `fwrite`.
* To increase speed of buffered reading functions like `fread` and `fgets`, you can increase a size of the file buffer (Newlib's default is 128 bytes) to a higher number like 4096, 8192 or 16384. This can be done locally via the `setvbuf` function used on a certain file pointer or globally applied to all files via modifying CONFIG_FATFS_VFS_FSTAT_BLKSIZE.



> Note
> 
> 
> Setting a bigger buffer size also increases the heap memory usage.