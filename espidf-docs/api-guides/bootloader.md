# Bootloader



The ESP-IDF Software Bootloader performs the following functions:


1. Minimal initial configuration of internal modules;
2. Initialize Flash Encryption and/or Secure features, if configured;
3. Select the application partition to boot, based on the partition table and ota_data (if any);
4. Load this image to RAM (IRAM & DRAM) and transfer management to the image that was just loaded.


Bootloader is located at the address 0x0 in the flash.


For a full description of the startup process including the ESP-IDF bootloader, see Application Startup Flow.



## Bootloader Compatibility


It is recommended to update to newer versions of ESP-IDF: when they are released. The OTA (over the air) update process can flash new apps in the field but cannot flash a new bootloader. For this reason, the bootloader supports booting apps built from newer versions of ESP-IDF.


The bootloader does not support booting apps from older versions of ESP-IDF. When updating ESP-IDF manually on an existing product that might need to downgrade the app to an older version, keep using the older ESP-IDF bootloader binary as well.



Note


If testing an OTA update for an existing product in production, always test it using the same ESP-IDF bootloader binary that is deployed in production.




### SPI Flash Configuration


Each ESP-IDF application or bootloader .bin file contains a header with CONFIG_ESPTOOLPY_FLASHMODE, CONFIG_ESPTOOLPY_FLASHFREQ, CONFIG_ESPTOOLPY_FLASHSIZE embedded in it. These are used to configure the SPI flash during boot.


The First Stage Bootloader in ROM reads the Second Stage Bootloader header information from flash and uses this information to load the rest of the Second Stage Bootloader from flash. However, at this time the system clock speed is lower than configured and not all flash modes are supported. When the Second Stage Bootloader then runs, it will reconfigure the flash using values read from the currently selected app binary's header (and NOT from the Second Stage Bootloader header). This allows an OTA update to change the SPI flash settings in use.





## Log Level


The default bootloader log level is "Info". By setting the CONFIG_BOOTLOADER_LOG_LEVEL option, it is possible to increase or decrease this level. This log level is separate from the log level used in the app (see Logging library).


Reducing bootloader log verbosity can improve the overall project boot time by a small amount.




## Factory Reset


Sometimes it is desirable to have a way for the device to fall back to a known-good state, in case of some problem with an update.


To roll back to the original "factory" device configuration and clear any user settings, configure the config item CONFIG_BOOTLOADER_FACTORY_RESET in the bootloader.


The factory reset mechanism allows the device to be factory reset in two ways:


* Clear one or more data partitions. The CONFIG_BOOTLOADER_DATA_FACTORY_RESET option allows users to specify which data partitions will be erased when the factory reset is executed.


Users can specify the names of partitions as a comma-delimited list with optional spaces for readability. (Like this: `nvs, phy_init, nvs_custom`).


Make sure that the names of partitions specified in the option are the same as those found in the partition table. Partitions of type "app" cannot be specified here.
* Boot from "factory" app partition. Enabling the CONFIG_BOOTLOADER_OTA_DATA_ERASE option will cause the device to boot from the default "factory" app partition after a factory reset (or if there is no factory app partition in the partition table then the default ota app partition is selected instead). This reset process involves erasing the OTA data partition which holds the currently selected OTA partition slot. The "factory" app partition slot (if it exists) is never updated via OTA, so resetting to this allows reverting to a "known good" firmware application.


Either or both of these configuration options can be enabled independently.


In addition, the following configuration options control the reset condition:


* CONFIG_BOOTLOADER_NUM_PIN_FACTORY_RESET- The input GPIO number used to trigger a factory reset. This GPIO must be pulled low or high (configurable) on reset to trigger this.
* CONFIG_BOOTLOADER_HOLD_TIME_GPIO- this is hold time of GPIO for reset/test mode (by default 5 seconds). The GPIO must be held continuously for this period of time after reset before a factory reset or test partition boot (as applicable) is performed.
* CONFIG_BOOTLOADER_FACTORY_RESET_PIN_LEVEL - configure whether a factory reset should trigger on a high or low level of the GPIO. If the GPIO has an internal pullup then this is enabled before the pin is sampled, consult the ESP32-S3 datasheet for details on pin internal pullups.


If an application needs to know if the factory reset has occurred, users can call the function `bootloader_common_get_rtc_retain_mem_factory_reset_state()`.


* If the status is read as true, the function will return the status, indicating that the factory reset has occurred. The function then resets the status to false for subsequent factory reset judgement.
* If the status is read as false, the function will return the status, indicating that the factory reset has not occurred, or the memory where this status is stored is invalid.


Note that this feature reserves some RTC FAST memory (the same size as the CONFIG_BOOTLOADER_SKIP_VALIDATE_IN_DEEP_SLEEP feature).




## Boot from Test Firmware


It is possible to write a special firmware app for testing in production, and boot this firmware when needed. The project partition table will need a dedicated app partition entry for this testing app, type `app` and subtype `test` (see Partition Tables).


Implementing a dedicated test app firmware requires creating a totally separate ESP-IDF project for the test app (each project in ESP-IDF only builds one app). The test app can be developed and tested independently of the main project, and then integrated at production testing time as a pre-compiled .bin file which is flashed to the address of the main project's test app partition.


To support this functionality in the main project's bootloader, set the configuration item CONFIG_BOOTLOADER_APP_TEST and configure the following three items:


* CONFIG_BOOTLOADER_NUM_PIN_APP_TEST - GPIO number to boot test partition. The selected GPIO will be configured as an input with internal pull-up enabled. This GPIO must be pulled low or high (configurable) on reset to trigger this.


Once the GPIO input is released and the device has been rebooted, the default boot sequence will be enabled again to boot the factory partition or any OTA app partition slot.
* CONFIG_BOOTLOADER_HOLD_TIME_GPIO - this is the hold time of GPIO for reset/test mode (by default 5 seconds). The GPIO must be held continuously for this period of time after reset before a factory reset or test partition boot (as applicable) is performed.
* CONFIG_BOOTLOADER_APP_TEST_PIN_LEVEL - configure whether a test partition boot should trigger on a high or low level of the GPIO. If the GPIO has an internal pull-up, then this is enabled before the pin is sampled. Consult the ESP32-S3 datasheet for details on pin internal pull-ups.




## Rollback


Rollback and anti-rollback features must be configured in the bootloader as well.


Consult the App Rollback and Anti-rollback sections in the OTA API reference document.




## Watchdog


The chips come equipped with two groups of watchdog timers: Main System Watchdog Timer (MWDT_WDT) and RTC Watchdog Timer (RTC_WDT). Both watchdog timer groups are enabled when the chip is powered up. However, in the bootloader, they will both be disabled. If CONFIG_BOOTLOADER_WDT_ENABLE is set (which is the default behavior), RTC_WDT is re-enabled. It tracks the time from the bootloader is enabled until the user's main function is called. In this scenario, RTC_WDT remains operational and will automatically reset the chip if no application successfully starts within 9 seconds. This functionality is particularly useful in preventing lockups caused by an unstable power source during startup.


* The timeout period can be adjusted by setting CONFIG_BOOTLOADER_WDT_TIME_MS and recompiling the bootloader.
* The RTC Watchdog can be disabled in the bootloader by disabling the CONFIG_BOOTLOADER_WDT_ENABLE setting and recompiling the bootloader. This is not recommended.
* See Hardware Watchdog Timers to learn how RTC_WDT is used in the application.




## Bootloader Size


When enabling additional bootloader functions, including Flash Encryption or Secure Boot, and especially if setting a high CONFIG_BOOTLOADER_LOG_LEVEL level, then it is important to monitor the bootloader .bin file's size.


When using the default CONFIG_PARTITION_TABLE_OFFSET value 0x8000, the size limit is 0x8000 bytes.


If the bootloader binary is too large, then the bootloader build will fail with an error "Bootloader binary size [..] is too large for partition table offset". If the bootloader binary is flashed anyhow then the ESP32-S3 will fail to boot - errors will be logged about either invalid partition table or invalid bootloader checksum.


Options to work around this are:


* Set bootloader compiler optimization back to "Size" if it has been changed from this default value.
* Reduce bootloader log level. Setting log level to Warning, Error or None all significantly reduce the final binary size (but may make it harder to debug).
* Set CONFIG_PARTITION_TABLE_OFFSET to a higher value than 0x8000, to place the partition table later in the flash. This increases the space available for the bootloader. If the partition table CSV file contains explicit partition offsets, they will need changing so no partition has an offset lower than `CONFIG_PARTITION_TABLE_OFFSET + 0x1000`. (This includes the default partition CSV files supplied with ESP-IDF.)


When Secure Boot V2 is enabled, there is also an absolute binary size limit of 64 KB (0x10000 bytes) (excluding the 4 KB signature), because the bootloader is first loaded into a fixed size buffer for verification.




## Fast Boot from Deep-Sleep


The bootloader has the CONFIG_BOOTLOADER_SKIP_VALIDATE_IN_DEEP_SLEEP option which allows the wake-up time from Deep-sleep to be reduced (useful for reducing power consumption). This option is available when CONFIG_SECURE_BOOT option is disabled. Reduction of time is achieved due to the lack of image verification. During the first boot, the bootloader stores the address of the application being launched in the RTC FAST memory. And during the awakening, this address is used for booting without any checks, thus fast loading is achieved.




## Custom Bootloader


The current bootloader implementation allows a project to extend it or modify it. There are two ways of doing it: by implementing hooks or by overriding it. Both ways are presented in custom_bootloader folder in ESP-IDF examples:


* bootloader_hooks which presents how to connect some hooks to the bootloader initialization
* bootloader_override which presents how to override the bootloader implementation


In the bootloader space, you cannot use the drivers and functions from other components. If necessary, then the required functionality should be placed in the project's bootloader_components directory (note that this will increase its size).


If the bootloader grows too large then it can collide with the partition table, which is flashed at offset 0x8000 by default. Increase the partition table offset value to place the partition table later in the flash. This increases the space available for the bootloader.