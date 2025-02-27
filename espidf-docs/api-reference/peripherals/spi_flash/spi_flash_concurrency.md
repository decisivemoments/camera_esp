# Concurrency Constraints for Flash on SPI1



The SPI0/1 bus is shared between the instruction & data cache (for firmware execution) and the SPI1 peripheral (controlled by the drivers including this SPI Flash driver). Hence, operations to SPI1 will cause significant influence to the whole system. This kind of operations include calling SPI Flash API or other drivers on SPI1 bus, any operations like read/write/erase or other user defined SPI operations, regardless to the main flash or other SPI slave devices.


On ESP32-S3, the config option CONFIG_SPI_FLASH_AUTO_SUSPEND allows the cache to read flash concurrently with SPI1 operations. This is an optional feature that depends on special SPI Flash models, hence disabled by default. See Flash Auto Suspend Feature for more details.


If this option is disabled, the caches must be disabled while reading/writing/erasing operations. There are some constraints using driver on the SPI1 bus, see When the Caches Are Disabled. These constraints will cause more IRAM/DRAM usages.


On ESP32-S3, the config options CONFIG_SPIRAM_XIP_FROM_PSRAM (disabled by default) allows the cache to read/write PSRAM concurrently with SPI1 operations. See XIP from PSRAM Feature for more details.


If these options are disabled, the caches must be disabled while reading/writing/erasing operations. There are some constraints using driver on the SPI1 bus, see When the Caches Are Disabled. These constraints will cause more IRAM/DRAM usages.



## When the Caches Are Disabled


Under this condition, all CPUs should always execute code and access data from internal RAM. The APIs documented in this file will disable the caches automatically and transparently.



Note


When CONFIG_SPIRAM_XIP_FROM_PSRAM is enabled, these APIs will not disable the caches.



The way that these APIs disable the caches suspends all the other tasks. Besides, all non-IRAM-safe interrupts will be disabled. The other core will be polling in a busy loop. These will be restored until the Flash operation completes.


See also OS Functions and SPI Bus Lock.


There are no such constraints and impacts for flash chips on other SPI buses than SPI0/1.


For differences between internal RAM (e.g., IRAM, DRAM) and flash cache, please refer to the application memory layout documentation.



### IRAM-Safe Interrupt Handlers


For interrupt handlers which need to execute when the cache is disabled (e.g., for low latency operations), set the `ESP_INTR_FLAG_IRAM` flag when the interrupt handler is registered.


You must ensure that all data and functions accessed by these interrupt handlers, including the ones that handlers call, are located in IRAM or DRAM. See How to Place Code in IRAM.


If a function or symbol is not correctly put into IRAM/DRAM, and the interrupt handler reads from the flash cache during a flash operation, it will cause a crash due to Illegal Instruction exception (for code which should be in IRAM) or garbage data to be read (for constant data which should be in DRAM).



Note


When working with strings in ISRs, it is not advised to use `printf` and other output functions. For debugging purposes, use `ESP_DRAM_LOGE()` and similar macros when logging from ISRs. Make sure that both `TAG` and format string are placed into `DRAM` in that case.





### Non-IRAM-Safe Interrupt Handlers


If the `ESP_INTR_FLAG_IRAM` flag is not set when registering, the interrupt handler will not get executed when the caches are disabled. Once the caches are restored, the non-IRAM-safe interrupts will be re-enabled. After this moment, the interrupt handler will run normally again. This means that as long as caches are disabled, users will not see the corresponding hardware event happening.





## Flash Auto Suspend Feature



Important


1. The flash chip you are using should have a suspend/resume feature.
2. The MSPI hardware should support the auto-suspend feature, i.e., hardware can send suspend command automatically.


If you use suspend feature on an unsupported chip, it may cause a severe crash. Therefore, we strongly suggest you reading the flash chip datasheets first. Ensure the flash chip satisfies the following conditions at minimum.


1. With the current software implementation, SUS bit in status registers should in SR2 bit7 or SR bit15.
2. With the current software implementation, suspend command should be 75H, with resume command being 7AH.
3. When the flash is successfully suspended, all address of the flash, except from the section/block being erased, can be read correctly. At this state，resume can be sent immediately as well.
4. When the flash is successfully resumed, another suspend can be sent immediately at this state.



When CONFIG_SPI_FLASH_AUTO_SUSPEND is enabled, the caches will be kept enabled. They would be disabled if CONFIG_SPI_FLASH_AUTO_SUSPEND is disabled. The hardware handles the arbitration between SPI0 and SPI1. If the SPI1 operation is short, such as a reading operation, the CPU and the cache will wait until the SPI1 operation is completed. However, during processes like erasing, page programming, or status register writing (e.g., `SE`, `PP`, and `WRSR`), an auto suspend will happen, interrupting the ongoing flash operation. This allows the CPU to access data from the cache and flash within limited time.


This approach allows certain code/variables to be stored in flash/PSRAM instead of IRAM/DRAM, while still being executable during flash erasing. This reduces the usage of IRAM/DRAM.


Please note that this feature comes with the overhead of flash suspend/resume. Frequent interruptions during flash erasing can significantly prolong the erasing process. To avoid this, you may use FreeRTOS task priorities to ensure that only real-time critical tasks are executed at a higher priority than flash erasing, allowing the flash erasing to complete in reasonable time.


There are three kinds of code:


1. Critical code: inside IRAM/DRAM. This kind of code usually has high performance requirements, related to cache/flash/PSRAM, or is called very often.
2. Cached code: inside flash/PSRAM. This kind of code has lower performance requirements or is called less often. They will execute during erasing, with some overhead.
3. Low-priority code: inside flash/PSRAM and disabled during erasing. This kind of code should be forbidden from being executed to avoid affecting the flash erasing, by setting a lower task priority than the erasing task.


Regarding the flash suspend feature usage and corresponding response time delay, please also see the system/flash_suspend example.



Note


The flash auto suspend feature relies heavily on strict timing. You can see it as a kind of optimization plan, which means that you cannot use it in every situation, like high requirement of real-time system or triggering interrupt very frequently (e.g., LCD flush, bluetooth, Wi-Fi, etc.). You should take following steps to evaluate what kind of ISR can be used together with flash suspend.





As you can see from the diagram, two key values should be noted:


1. ISR time: The ISR time cannot be very long, at least not larger than the value you set in `IWDT`. But it will significantly lengthen the erasing/writing completion time.
2. ISR interval: ISR cannot be triggered very often. The most important time is the **ISR interval minus ISR time** (from point b to point c in the diagram). During this time, SPI1 will send resume command to restart the operation. However, it needs a time `tsus` for preparation, and the typical value of `tsus` is about **40 us**. If SPI1 cannot resume the operation but another suspend command comes, it will cause CPU starve and `TWDT` may be triggered.


The `tsus` time mentioned in point 2 can be found by looking through the flash datasheets, usually in the AC CHARACTERISTICS section. Users needs to make sure that the `tsus` value obtained from the datasheets is not greater than the CONFIG_SPI_FLASH_SUSPEND_TSUS_VAL_US value in Kconfig.


Furthermore, the flash suspend might be delayed. If both the CPU and the cache access the flash via SPI0 frequently and SPI1 sends the suspend command frequently as well, the efficiency of MSPI data transfer will be influenced. So, we have a **lock** inside to prevent this. When SPI1 sends the suspend command, SPI0 will take over memory SPI bus and take the lock. After SPI0 finishes sending data, it will retain control of the memory SPI bus until the lock delay period time finishes. During this lock delay period, if there is any other SPI0 transaction, then the SPI0 transaction will be proceeded and a new lock delay period will start. Otherwise, SPI0 will release the memory bus and start SPI0/1 arbitration.





## XIP from PSRAM Feature


If CONFIG_SPIRAM_XIP_FROM_PSRAM is enabled, the flash `.text` sections (used for instructions) and the flash `.rodata` sections (used for read only data) will be placed in PSRAM.


The corresponding virtual memory range will be mapped to PSRAM.


If both of the above options are enabled, the Cache won't be disabled during an SPI1 Flash operation. You don't need to make sure ISRs, ISR callbacks and involved data are placed in internal RAM.