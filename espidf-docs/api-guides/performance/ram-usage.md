# Minimizing RAM Usage



In some cases, a firmware application's available RAM may run low or run out entirely. In these cases, it is necessary to tune the memory usage of the firmware application.


In general, firmware should aim to leave some headroom of free internal RAM to deal with extraordinary situations or changes in RAM usage in future updates.



## Background


Before optimizing ESP-IDF RAM usage, it is necessary to understand the basics of ESP32-S3 memory types, the difference between static and dynamic memory usage in C, and the way ESP-IDF uses stack and heap. This information can all be found in Heap Memory Allocation.




## Measuring Static Memory Usage


The idf.py tool can be used to generate reports about the static memory usage of an application, see Measuring Static Sizes.




## Measuring Dynamic Memory Usage


ESP-IDF contains a range of heap APIs for measuring free heap at runtime, see Heap Memory Debugging.



Note


In embedded systems, heap fragmentation can be a significant issue alongside total RAM usage. The heap measurement APIs provide ways to measure the largest free block. Monitoring this value along with the total number of free bytes can give a quick indication of whether heap fragmentation is becoming an issue.





## Reducing Static Memory Usage


* Reducing the static memory usage of the application increases the amount of RAM available for heap at runtime, and vice versa.
* Generally speaking, minimizing static memory usage requires monitoring the `.data` and `.bss` sizes. For tools to do this, see Measuring Static Sizes.
* Internal ESP-IDF functions do not make heavy use of static RAM in C. In many instances (such as Wi-Fi library, Bluetooth controller), static buffers are still allocated from the heap. However, the allocation is performed only once during feature initialization and will be freed if the feature is deinitialized. This approach is adopted to optimize the availability of free memory at various stages of the application's life cycle.


To minimize static memory use:


* Constant data can be stored in flash memory instead of RAM, thus it is recommended to declare structures, buffers, or other variables as `const`. This approach may require modifying firmware functions to accept `const *` arguments instead of mutable pointer arguments. These changes can also help reduce the stack usage of certain functions.
* If using Bluedroid, setting the option CONFIG_BT_BLE_DYNAMIC_ENV_MEMORY will cause Bluedroid to allocate memory on initialization and free it on deinitialization. This does not necessarily reduce the peak memory usage, but changes it from static memory usage to runtime memory usage.
* If using OpenThread, enabling the option CONFIG_OPENTHREAD_PLATFORM_MSGPOOL_MANAGEMENT will cause OpenThread to allocate message pool buffers from PSRAM, which will reduce static memory use.







## Reducing Stack Sizes


In FreeRTOS, task stacks are usually allocated from the heap. The stack size for each task is fixed and passed as an argument to `xTaskCreate()`. Each task can use up to its allocated stack size, but using more than this will cause an otherwise valid program to crash, with a stack overflow or heap corruption.


Therefore, determining the optimum sizes of each task stack, minimizing the required size of each task stack, and minimizing the number of task stacks as whole, can all substantially reduce RAM usage.


To determine the optimum size for a particular task stack, users can consider the following methods:


* At runtime, call the function `uxTaskGetStackHighWaterMark()` with the handle of any task where you think there is unused stack memory. This function returns the minimum lifetime free stack memory in bytes.


	+ The easiest time to call `uxTaskGetStackHighWaterMark()` is from the task itself: call `uxTaskGetStackHighWaterMark(NULL)` to get the current task's high water mark after the time that the task has achieved its peak stack usage, i.e., if there is a main loop, execute the main loop a number of times with all possible states, and then call `uxTaskGetStackHighWaterMark()`.
	+ Often, it is possible to subtract almost the entire value returned here from the total stack size of a task, but allow some safety margin to account for unexpected small increases in stack usage at runtime.
* Call `uxTaskGetSystemState()` at runtime to get a summary of all tasks in the system. This includes their individual stack high watermark values.
* When debugger watchpoints are not being used, users can set the CONFIG_FREERTOS_WATCHPOINT_END_OF_STACK option. This will cause one of the watchpoints to watch the last word of the task's stack. If that word is overwritten (such as in a stack overflow), a panic is triggered immediately. This is slightly more reliable than the default CONFIG_FREERTOS_CHECK_STACKOVERFLOW option of `Check using canary bytes`, because the panic happens immediately, rather than on the next RTOS context switch. Neither option is perfect. In some cases, it is possible that the stack pointer skips the watchpoint or canary bytes and corrupts another region of RAM instead.


To reduce the required size of a particular task stack, users can consider the following methods:


* Avoid stack heavy functions. String formatting functions (like `printf()`) are particularly heavy users of the stack, so any task which does not ever call these can usually have its stack size reduced.


	+ Enabling Newlib Nano Formatting reduces the stack usage of any task that calls `printf()` or other C string formatting functions.
* Avoid allocating large variables on the stack. In C, any large structures or arrays allocated as an automatic variable (i.e., default scope of a C declaration) uses space on the stack. To minimize the sizes of these, allocate them statically and/or see if you can save memory by dynamically allocating them from the heap only when they are needed.
* Avoid deep recursive function calls. Individual recursive function calls do not always add a lot of stack usage each time they are called, but if each function includes large stack-based variables then the overhead can get quite high.


To reduce the total number of tasks, users can consider the following method:


* Combine tasks. If a particular task is never created, the task's stack is never allocated, thus reducing RAM usage significantly. Unnecessary tasks can typically be removed if those tasks can be combined with another task. In an application, tasks can typically be combined or removed if:



> + The work done by the tasks can be structured into multiple functions that are called sequentially.
> 	+ The work done by the tasks can be structured into smaller jobs that are serialized (via a FreeRTOS queue or similar) for execution by a worker task.



### Internal Task Stack Sizes


ESP-IDF allocates a number of internal tasks for housekeeping purposes or operating system functions. Some are created during the startup process, and some are created at runtime when particular features are initialized.


The default stack sizes for these tasks are usually set conservatively high to allow all common usage patterns. Many of the stack sizes are configurable, and it may be possible to reduce them to match the real runtime stack usage of the task.



Important


If internal task stack sizes are set too small, ESP-IDF will crash unpredictably. Even if the root cause is task stack overflow, this is not always clear when debugging. It is recommended that internal stack sizes are only reduced carefully (if at all), with close attention to high water mark free space under load. If reporting an issue that occurs when internal task stack sizes have been reduced, please always include the following information and the specific configuration that is being used.



* Running the Main Task has stack size CONFIG_ESP_MAIN_TASK_STACK_SIZE.
* ESP Timer (High Resolution Timer) system task which executes callbacks has stack size CONFIG_ESP_TIMER_TASK_STACK_SIZE.
* FreeRTOS Timer Task to handle FreeRTOS timer callbacks has stack size CONFIG_FREERTOS_TIMER_TASK_STACK_DEPTH.
* Event Loop Library system task to execute callbacks for the default system event loop has stack size CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE.
* lwIP TCP/IP task has stack size CONFIG_LWIP_TCPIP_TASK_STACK_SIZE.
* Bluetooth® API have task stack sizes CONFIG_BT_BTC_TASK_STACK_SIZE, CONFIG_BT_BTU_TASK_STACK_SIZE.
* NimBLE-based Host APIs has task stack size CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE.
* The Ethernet driver creates a task for the MAC to receive Ethernet frames. If using the default config `ETH_MAC_DEFAULT_CONFIG` then the task stack size is 4 KB. This setting can be changed by passing a custom `eth_mac_config_t` struct when initializing the Ethernet MAC.
* FreeRTOS idle task stack size is configured by CONFIG_FREERTOS_IDLE_TASK_STACKSIZE.
* If using the ESP-MQTT component, it creates a task with stack size configured by CONFIG_MQTT_TASK_STACK_SIZE. MQTT stack size can also be configured using `task_stack` field of `esp_mqtt_client_config_t`.
* To see how to optimize RAM usage when using `mDNS`, please check Minimizing RAM Usage.






Note


Aside from built-in system features such as ESP-timer, if an ESP-IDF feature is not initialized by the firmware, then no associated task is created. In those cases, the stack usage is zero, and the stack-size configuration for the task is not relevant.






## Reducing Heap Usage


For functions that assist in analyzing heap usage at runtime, see Heap Memory Debugging.


Normally, optimizing heap usage consists of analyzing the usage and removing calls to `malloc()` that are not being used, reducing the corresponding sizes, or freeing previously allocated buffers earlier.


There are some ESP-IDF configuration options that can reduce heap usage at runtime:


* lwIP documentation has a section to configure Minimum RAM Usage.
* Wi-Fi Buffer Usage describes options to either reduce the number of static buffers or reduce the maximum number of dynamic buffers in use, so as to minimize memory usage at a possible cost of performance. Note that static Wi-Fi buffers are still allocated from the heap when Wi-Fi is initialized, and will be freed if Wi-Fi is deinitialized.
* Several Mbed TLS configuration options can be used to reduce heap memory usage. See the Reducing Heap Usage docs for details.






Note


There are other configuration options that increases heap usage at runtime if changed from the defaults. These options are not listed above, but the help text for the configuration item will mention if there is some memory impact.





## Optimizing IRAM Usage


The available DRAM at runtime for heap usage is also reduced by the static IRAM usage. Therefore, one way to increase available DRAM is to reduce IRAM usage.


If the app allocates more static IRAM than available, then the app will fail to build, and linker errors such as `section '.iram0.text' will not fit in region 'iram0_0_seg'`, `IRAM0 segment data does not fit`, and `region 'iram0_0_seg' overflowed by 84-bytes` will be seen. If this happens, it is necessary to find ways to reduce static IRAM usage in order to link the application.


To analyze the IRAM usage in the firmware binary, use Measuring Static Sizes. If the firmware failed to link, steps to analyze are shown at Showing Size When Linker Fails.


The following options will reduce IRAM usage of some ESP-IDF features:


* Enable CONFIG_FREERTOS_PLACE_FUNCTIONS_INTO_FLASH. Provided these functions are not incorrectly used from ISRs, this option is safe to enable in all configurations.
* Enable CONFIG_RINGBUF_PLACE_FUNCTIONS_INTO_FLASH. Provided these functions are not incorrectly used from ISRs, this option is safe to enable in all configurations.
* Enable CONFIG_RINGBUF_PLACE_ISR_FUNCTIONS_INTO_FLASH. This option is not safe to use if the ISR ringbuf functions are used from an IRAM interrupt context, e.g., if CONFIG_UART_ISR_IN_IRAM is enabled. For the ESP-IDF drivers where this is the case, you can get an error at run-time when installing the driver in question.
* Disabling Wi-Fi options CONFIG_ESP_WIFI_IRAM_OPT and/or CONFIG_ESP_WIFI_RX_IRAM_OPT options frees available IRAM at the cost of Wi-Fi performance.
* Enabling CONFIG_SPI_FLASH_ROM_IMPL frees some IRAM but means that esp_flash bugfixes and new flash chip support are not available, see SPI Flash API ESP-IDF Version vs Chip-ROM Version for details.
* Disabling CONFIG_ESP_EVENT_POST_FROM_IRAM_ISR prevents posting `esp_event` events from IRAM-Safe Interrupt Handlers but saves some IRAM.
* Disabling CONFIG_SPI_MASTER_ISR_IN_IRAM prevents spi_master interrupts from being serviced while writing to flash, and may otherwise reduce spi_master performance, but saves some IRAM.
* Disabling CONFIG_SPI_SLAVE_ISR_IN_IRAM prevents spi_slave interrupts from being serviced while writing to flash, which saves some IRAM.
* Setting CONFIG_HAL_DEFAULT_ASSERTION_LEVEL to disable assertion for HAL component saves some IRAM, especially for HAL code who calls `HAL_ASSERT` a lot and resides in IRAM.
* Refer to the sdkconfig menu `Auto-detect Flash chips`, and you can disable flash drivers which you do not need to save some IRAM.
* Enable CONFIG_HEAP_PLACE_FUNCTION_INTO_FLASH. Provided that CONFIG_SPI_MASTER_ISR_IN_IRAM is not enabled and the heap functions are not incorrectly used from ISRs, this option is safe to enable in all configurations.






Note


Moving frequently-called functions from IRAM to flash may increase their execution time.




Note


Other configuration options exist that will increase IRAM usage by moving some functionality into IRAM, usually for performance, but the default option is not to do this. These are not listed here. The IRAM size impact of enabling these options is usually noted in the configuration item help text.




### Change cache size


The ESP32-S3 RAM memory available size is dependent on the size of cache. Decreasing the cache size in the Kconfig options listed below will result in increasing the available RAM.


* CONFIG_ESP32S3_INSTRUCTION_CACHE_SIZE
* CONFIG_ESP32S3_DATA_CACHE_SIZE