# Project Configuration




## Introduction


The esp-idf-kconfig package that ESP-IDF uses is based on kconfiglib, which is a Python extension to the Kconfig system. Kconfig provides a compile-time project configuration mechanism and offers configuration options of several types (e.g., integers, strings, and Booleans). Kconfig files specify dependencies between options, default values of options, the way options are grouped together, etc.


For the full list of available features, please see Kconfig and kconfiglib extensions.




## Project Configuration Menu


Application developers can open a terminal-based project configuration menu with the `idf.py menuconfig` build target.


After being updated, this configuration is saved in the `sdkconfig` file under the project root directory. Based on `sdkconfig`, application build targets will generate the `sdkconfig.h` file under the build directory, and will make the `sdkconfig` options available to the project build system and source files.




## Using `sdkconfig.defaults`


In some cases, for example, when the `sdkconfig` file is under revision control, it may be inconvenient for the build system to change the `sdkconfig` file. The build system offers a solution to prevent it from happening, which is to create the `sdkconfig.defaults` file. This file is never touched by the build system, and can be created manually or automatically. It contains all the options which matter to the given application and are different from the default ones. The format is the same as that of the `sdkconfig` file. `sdkconfig.defaults` can be created manually when one remembers all the changed configuration, or it can be generated automatically by running the `idf.py save-defconfig` command.


Once `sdkconfig.defaults` is created, `sdkconfig` can be deleted or added to the ignore list of the revision control system (e.g., the `.gitignore` file for `git`). Project build targets will automatically create the `sdkconfig` file, populate it with the settings from the `sdkconfig.defaults` file, and configure the rest of the settings to their default values. Note that during the build process, settings from `sdkconfig.defaults` will not override those already in `sdkconfig`. For more information, see Custom Sdkconfig Defaults.




## Kconfig Format Rules


Format rules for Kconfig files are as follows:


* Option names in any menus should have consistent prefixes. The prefix currently should have at least 3 characters.
* The unit of indentation should be 4 spaces. All sub-items belonging to a parent item are indented by one level deeper. For example, `menu` is indented by 0 spaces, `config` `menu` by 4 spaces, `help` in `config` by 8 spaces, and the text under `help` by 12 spaces.
* No trailing spaces are allowed at the end of the lines.
* The maximum length of options is 50 characters.
* The maximum length of lines is 120 characters.



Note


The `help` section of each config in the menu is treated as reStructuredText to generate the reference documentation for each option.




### Format Checker


`kconfcheck` tool in esp-idf-kconfig package is provided for checking Kconfig files against the above format rules. The checker checks all Kconfig and `Kconfig.projbuild` files given as arguments, and generates a new file with suffix `.new` with some suggestions about how to fix issues (if there are any). Please note that the checker cannot correct all format issues and the responsibility of the developer is to final check and make corrections in order to pass the tests. For example, indentations will be corrected if there is not any misleading formatting, but it cannot come up with a common prefix for options inside a menu.


The `esp-idf-kconfig` package is available in ESP-IDF environments, where the checker tool can be invoked by running command `python -m kconfcheck <path_to_kconfig_file>`.


For more information, please refer to esp-idf-kconfig package documentation.





## Backward Compatibility of Kconfig Options


The standard Kconfig tools ignore unknown options in `sdkconfig`. So if a developer has custom settings for options which are renamed in newer ESP-IDF releases, then the given setting for the option would be silently ignored. Therefore, several features have been adopted to avoid this:


1. `kconfgen` is used by the tool chain to pre-process `sdkconfig` files before anything else. For example, `menuconfig` would read them, so the settings for old options is kept and not ignored.
2. `kconfgen` recursively finds all `sdkconfig.rename` files in ESP-IDF directory which contain old and new `Kconfig` option names. Old options are replaced by new ones in the `sdkconfig` file. Renames that should only appear for a single target can be placed in a target-specific rename file `sdkconfig.rename.TARGET`, where `TARGET` is the target name, e.g., `sdkconfig.rename.esp32s2`.
3. `kconfgen` post-processes `sdkconfig` files and generates all build outputs (`sdkconfig.h`, `sdkconfig.cmake`, and `auto.conf`) by adding a list of compatibility statements, i.e., the values of old options are set for new options after modification. If users still use old options in their code, this will prevent it from breaking.
4. Deprecated options and their replacements are automatically generated by `kconfgen`.




## Configuration Options Reference


Subsequent sections contain the list of available ESP-IDF options automatically generated from Kconfig files. Note that due to dependencies between options, some options listed here may not be visible by default in `menuconfig`.


By convention, all option names are upper-case letters with underscores. When Kconfig generates `sdkconfig` and `sdkconfig.h` files, option names are prefixed with `CONFIG_`. So if an option `ENABLE_FOO` is defined in a Kconfig file and selected in `menuconfig`, then the `sdkconfig` and `sdkconfig.h` files will have `CONFIG_ENABLE_FOO` defined. In the following sections, option names are also prefixed with `CONFIG_`, same as in the source code.



### Build type


Contains:


* CONFIG_APP_BUILD_TYPE
* CONFIG_APP_BUILD_TYPE_PURE_RAM_APP
* CONFIG_APP_REPRODUCIBLE_BUILD
* CONFIG_APP_NO_BLOBS



#### CONFIG_APP_BUILD_TYPE



> Application build type
> 
> 
> *Found in:* Build type
> 
> 
> Select the way the application is built.
> 
> 
> By default, the application is built as a binary file in a format compatible with
> the ESP-IDF bootloader. In addition to this application, 2nd stage bootloader is
> also built. Application and bootloader binaries can be written into flash and
> loaded/executed from there.
> 
> 
> Another option, useful for only very small and limited applications, is to only link
> the .elf file of the application, such that it can be loaded directly into RAM over
> JTAG or UART. Note that since IRAM and DRAM sizes are very limited, it is not possible
> to build any complex application this way. However for some kinds of testing and debugging,
> this option may provide faster iterations, since the application does not need to be
> written into flash.
> 
> 
> Note: when APP_BUILD_TYPE_RAM is selected and loaded with JTAG, ESP-IDF does not contain
> all the startup code required to initialize the CPUs and ROM memory (data/bss).
> Therefore it is necessary to execute a bit of ROM code prior to executing the application.
> A gdbinit file may look as follows (for ESP32):
> 
> 
> 
> > # Connect to a running instance of OpenOCD
> > target remote :3333
> > # Reset and halt the target
> > mon reset halt
> > # Run to a specific point in ROM code,
> > # where most of initialization is complete.
> > thb \*0x40007d54
> > c
> > # Load the application into RAM
> > load
> > # Run till app_main
> > tb app_main
> > c
> 
> 
> Execute this gdbinit file as follows:
> 
> 
> 
> > xtensa-esp32-elf-gdb build/app-name.elf -x gdbinit
> 
> 
> Example gdbinit files for other targets can be found in tools/test_apps/system/gdb_loadable_elf/
> 
> 
> When loading the BIN with UART, the ROM will jump to ram and run the app after finishing the ROM
> startup code, so there's no additional startup initialization required. You can use the
> load_ram in esptool.py to load the generated .bin file into ram and execute.
> 
> 
> 
> Example:esptool.py --chip {chip} -p {port} -b {baud} --no-stub load_ram {app.bin}
> 
> 
> 
> 
> Recommended sdkconfig.defaults for building loadable ELF files is as follows.
> CONFIG_APP_BUILD_TYPE_RAM is required, other options help reduce application
> memory footprint.
> 
> 
> 
> > CONFIG_APP_BUILD_TYPE_RAM=y
> > CONFIG_VFS_SUPPORT_TERMIOS=
> > CONFIG_NEWLIB_NANO_FORMAT=y
> > CONFIG_ESP_SYSTEM_PANIC_PRINT_HALT=y
> > CONFIG_ESP_DEBUG_STUBS_ENABLE=
> > CONFIG_ESP_ERR_TO_NAME_LOOKUP=
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Default (binary application + 2nd stage bootloader) (CONFIG_APP_BUILD_TYPE_APP_2NDBOOT)
> > * Build app runs entirely in RAM (EXPERIMENTAL) (CONFIG_APP_BUILD_TYPE_RAM)




#### CONFIG_APP_BUILD_TYPE_PURE_RAM_APP



> Build app without SPI_FLASH/PSRAM support (saves ram)
> 
> 
> *Found in:* Build type
> 
> 
> If this option is enabled, external memory and related peripherals, such as Cache, MMU,
> Flash and PSRAM, won't be initialized. Corresponding drivers won't be introduced either.
> Components that depend on the spi_flash component will also be unavailable, such as
> app_update, etc. When this option is enabled, about 26KB of RAM space can be saved.




#### CONFIG_APP_REPRODUCIBLE_BUILD



> Enable reproducible build
> 
> 
> *Found in:* Build type
> 
> 
> If enabled, all date, time, and path information would be eliminated. A .gdbinit file would be create
> automatically. (or will be append if you have one already)
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_APP_NO_BLOBS



> No Binary Blobs
> 
> 
> *Found in:* Build type
> 
> 
> If enabled, this disables the linking of binary libraries in the application build. Note
> that after enabling this Wi-Fi/Bluetooth will not work.
> 
> 
> 
> Default value:* No (disabled)





### Bootloader config


Contains:


* CONFIG_BOOTLOADER_LOG_LEVEL
* Bootloader manager
* CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION
* CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE
* CONFIG_BOOTLOADER_REGION_PROTECTION_ENABLE
* CONFIG_BOOTLOADER_APP_TEST
* CONFIG_BOOTLOADER_FACTORY_RESET
* CONFIG_BOOTLOADER_HOLD_TIME_GPIO
* CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC
* Serial Flash Configurations
* CONFIG_BOOTLOADER_SKIP_VALIDATE_ALWAYS
* CONFIG_BOOTLOADER_SKIP_VALIDATE_ON_POWER_ON
* CONFIG_BOOTLOADER_SKIP_VALIDATE_IN_DEEP_SLEEP
* CONFIG_BOOTLOADER_WDT_ENABLE
* CONFIG_BOOTLOADER_VDDSDIO_BOOST



#### Bootloader manager


Contains:


* CONFIG_BOOTLOADER_PROJECT_VER
* CONFIG_BOOTLOADER_COMPILE_TIME_DATE



##### CONFIG_BOOTLOADER_COMPILE_TIME_DATE



> Use time/date stamp for bootloader
> 
> 
> *Found in:* Bootloader config > Bootloader manager
> 
> 
> If set, then the bootloader will be built with the current time/date stamp.
> It is stored in the bootloader description
> structure. If not set, time/date stamp will be excluded from bootloader image.
> This can be useful for getting the
> same binary image files made from the same source, but at different times.




##### CONFIG_BOOTLOADER_PROJECT_VER



> Project version
> 
> 
> *Found in:* Bootloader config > Bootloader manager
> 
> 
> Project version. It is placed in "version" field of the esp_bootloader_desc structure.
> The type of this field is "uint32_t".
> 
> 
> 
> Range:* from 0 to 4294967295
> 
> 
> 
> Default value:* 1





#### CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION



> Bootloader optimization Level
> 
> 
> *Found in:* Bootloader config
> 
> 
> This option sets compiler optimization level (gcc -O argument)
> for the bootloader.
> 
> 
> * The default "Size" setting will add the -0s flag to CFLAGS.
> * The "Debug" setting will add the -Og flag to CFLAGS.
> * The "Performance" setting will add the -O2 flag to CFLAGS.
> 
> 
> Note that custom optimization levels may be unsupported.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Size (-Os) (CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION_SIZE)
> > * Debug (-Og) (CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION_DEBUG)
> > * Optimize for performance (-O2) (CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION_PERF)
> > * Debug without optimization (-O0) (Deprecated, will be removed in IDF v6.0) (CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION_NONE)




#### CONFIG_BOOTLOADER_LOG_LEVEL



> Bootloader log verbosity
> 
> 
> *Found in:* Bootloader config
> 
> 
> Specify how much output to see in bootloader logs.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No output (CONFIG_BOOTLOADER_LOG_LEVEL_NONE)
> > * Error (CONFIG_BOOTLOADER_LOG_LEVEL_ERROR)
> > * Warning (CONFIG_BOOTLOADER_LOG_LEVEL_WARN)
> > * Info (CONFIG_BOOTLOADER_LOG_LEVEL_INFO)
> > * Debug (CONFIG_BOOTLOADER_LOG_LEVEL_DEBUG)
> > * Verbose (CONFIG_BOOTLOADER_LOG_LEVEL_VERBOSE)




#### Serial Flash Configurations


Contains:


* CONFIG_BOOTLOADER_FLASH_DC_AWARE
* CONFIG_BOOTLOADER_CACHE_32BIT_ADDR_QUAD_FLASH
* CONFIG_BOOTLOADER_FLASH_XMC_SUPPORT



##### CONFIG_BOOTLOADER_FLASH_DC_AWARE



> Allow app adjust Dummy Cycle bits in SPI Flash for higher frequency (READ HELP FIRST)
> 
> 
> *Found in:* Bootloader config > Serial Flash Configurations
> 
> 
> This will force 2nd bootloader to be loaded by DOUT mode, and will restore Dummy Cycle setting by
> resetting the Flash




##### CONFIG_BOOTLOADER_FLASH_XMC_SUPPORT



> Enable the support for flash chips of XMC (READ DOCS FIRST)
> 
> 
> *Found in:* Bootloader config > Serial Flash Configurations
> 
> 
> Perform the startup flow recommended by XMC. Please consult XMC for the details of this flow.
> XMC chips will be forbidden to be used, when this option is disabled.
> 
> 
> DON'T DISABLE THIS UNLESS YOU KNOW WHAT YOU ARE DOING.
> 
> 
> comment "Features below require specific hardware (READ DOCS FIRST!)"
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_BOOTLOADER_CACHE_32BIT_ADDR_QUAD_FLASH



> Enable cache access to 32-bit-address (over 16MB) range of SPI Flash (READ DOCS FIRST)
> 
> 
> *Found in:* Bootloader config > Serial Flash Configurations
> 
> 
> Enabling this option allows the CPU to access 32-bit-address flash beyond 16M range.
> 1. This option only valid for 4-line flash. Octal flash doesn't need this.
> 2. This option is experimental, which means it can’t use on all flash chips stable, for more
> information, please contact Espressif Business support.





#### CONFIG_BOOTLOADER_VDDSDIO_BOOST



> VDDSDIO LDO voltage
> 
> 
> *Found in:* Bootloader config
> 
> 
> If this option is enabled, and VDDSDIO LDO is set to 1.8V (using eFuse
> or MTDI bootstrapping pin), bootloader will change LDO settings to
> output 1.9V instead. This helps prevent flash chip from browning out
> during flash programming operations.
> 
> 
> This option has no effect if VDDSDIO is set to 3.3V, or if the internal
> VDDSDIO regulator is disabled via eFuse.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 1.8V (CONFIG_BOOTLOADER_VDDSDIO_BOOST_1_8V)
> > * 1.9V (CONFIG_BOOTLOADER_VDDSDIO_BOOST_1_9V)




#### CONFIG_BOOTLOADER_FACTORY_RESET



> GPIO triggers factory reset
> 
> 
> *Found in:* Bootloader config
> 
> 
> Allows to reset the device to factory settings:
> - clear one or more data partitions;
> - boot from "factory" partition.
> The factory reset will occur if there is a GPIO input held at the configured level while
> device starts up. See settings below.
> 
> 
> 
> Default value:* No (disabled)



##### CONFIG_BOOTLOADER_NUM_PIN_FACTORY_RESET



> Number of the GPIO input for factory reset
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_FACTORY_RESET
> 
> 
> The selected GPIO will be configured as an input with internal pull-up enabled (note that on some SoCs.
> not all pins have an internal pull-up, consult the hardware datasheet for details.) To trigger a factory
> reset, this GPIO must be held high or low (as configured) on startup.
> 
> 
> 
> Default value:* 4 if CONFIG_BOOTLOADER_FACTORY_RESET




##### CONFIG_BOOTLOADER_FACTORY_RESET_PIN_LEVEL



> Factory reset GPIO level
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_FACTORY_RESET
> 
> 
> Pin level for factory reset, can be triggered on low or high.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Reset on GPIO low (CONFIG_BOOTLOADER_FACTORY_RESET_PIN_LOW)
> > * Reset on GPIO high (CONFIG_BOOTLOADER_FACTORY_RESET_PIN_HIGH)




##### CONFIG_BOOTLOADER_OTA_DATA_ERASE



> Clear OTA data on factory reset (select factory partition)
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_FACTORY_RESET
> 
> 
> The device will boot from "factory" partition (or OTA slot 0 if no factory partition is present) after a
> factory reset.




##### CONFIG_BOOTLOADER_DATA_FACTORY_RESET



> Comma-separated names of partitions to clear on factory reset
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_FACTORY_RESET
> 
> 
> Allows customers to select which data partitions will be erased while factory reset.
> 
> 
> Specify the names of partitions as a comma-delimited with optional spaces for readability. (Like this:
> "nvs, phy_init, ...")
> Make sure that the name specified in the partition table and here are the same.
> Partitions of type "app" cannot be specified here.
> 
> 
> 
> Default value:* "nvs" if CONFIG_BOOTLOADER_FACTORY_RESET





#### CONFIG_BOOTLOADER_APP_TEST



> GPIO triggers boot from test app partition
> 
> 
> *Found in:* Bootloader config
> 
> 
> Allows to run the test app from "TEST" partition.
> A boot from "test" partition will occur if there is a GPIO input pulled low while device starts up.
> See settings below.



##### CONFIG_BOOTLOADER_NUM_PIN_APP_TEST



> Number of the GPIO input to boot TEST partition
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_APP_TEST
> 
> 
> The selected GPIO will be configured as an input with internal pull-up enabled.
> To trigger a test app, this GPIO must be pulled low on reset.
> After the GPIO input is deactivated and the device reboots, the old application will boot.
> (factory or OTA[x]).
> Note that GPIO34-39 do not have an internal pullup and an external one must be provided.
> 
> 
> 
> Range:* from 0 to 39 if CONFIG_BOOTLOADER_APP_TEST
> 
> 
> 
> Default value:* 18 if CONFIG_BOOTLOADER_APP_TEST




##### CONFIG_BOOTLOADER_APP_TEST_PIN_LEVEL



> App test GPIO level
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_APP_TEST
> 
> 
> Pin level for app test, can be triggered on low or high.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Enter test app on GPIO low (CONFIG_BOOTLOADER_APP_TEST_PIN_LOW)
> > * Enter test app on GPIO high (CONFIG_BOOTLOADER_APP_TEST_PIN_HIGH)





#### CONFIG_BOOTLOADER_HOLD_TIME_GPIO



> Hold time of GPIO for reset/test mode (seconds)
> 
> 
> *Found in:* Bootloader config
> 
> 
> The GPIO must be held low continuously for this period of time after reset
> before a factory reset or test partition boot (as applicable) is performed.
> 
> 
> 
> Default value:* 5 if CONFIG_BOOTLOADER_FACTORY_RESET || CONFIG_BOOTLOADER_APP_TEST




#### CONFIG_BOOTLOADER_REGION_PROTECTION_ENABLE



> Enable protection for unmapped memory regions
> 
> 
> *Found in:* Bootloader config
> 
> 
> Protects the unmapped memory regions of the entire address space from unintended accesses.
> This will ensure that an exception will be triggered whenever the CPU performs a memory
> operation on unmapped regions of the address space.
> 
> 
> 
> Default value:* Yes (enabled)




#### CONFIG_BOOTLOADER_WDT_ENABLE



> Use RTC watchdog in start code
> 
> 
> *Found in:* Bootloader config
> 
> 
> Tracks the execution time of startup code.
> If the execution time is exceeded, the RTC_WDT will restart system.
> It is also useful to prevent a lock up in start code caused by an unstable power source.
> NOTE: Tracks the execution time starts from the bootloader code - re-set timeout, while selecting the
> source for slow_clk - and ends calling app_main.
> Re-set timeout is needed due to WDT uses a SLOW_CLK clock source. After changing a frequency slow_clk a
> time of WDT needs to re-set for new frequency.
> slow_clk depends on RTC_CLK_SRC (INTERNAL_RC or EXTERNAL_CRYSTAL).
> 
> 
> 
> Default value:* Yes (enabled)



##### CONFIG_BOOTLOADER_WDT_DISABLE_IN_USER_CODE



> Allows RTC watchdog disable in user code
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_WDT_ENABLE
> 
> 
> If this option is set, the ESP-IDF app must explicitly reset, feed, or disable the rtc_wdt in
> the app's own code.
> If this option is not set (default), then rtc_wdt will be disabled by ESP-IDF before calling
> the app_main() function.
> 
> 
> Use function wdt_hal_feed() for resetting counter of RTC_WDT.
> For esp32/s2 you can also use rtc_wdt_feed().
> 
> 
> Use function wdt_hal_disable() for disabling RTC_WDT.
> For esp32/s2 you can also use rtc_wdt_disable().
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_BOOTLOADER_WDT_TIME_MS



> Timeout for RTC watchdog (ms)
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_WDT_ENABLE
> 
> 
> Verify that this parameter is correct and more then the execution time.
> Pay attention to options such as reset to factory, trigger test partition and encryption on boot
> - these options can increase the execution time.
> Note: RTC_WDT will reset while encryption operations will be performed.
> 
> 
> 
> Range:* from 0 to 120000
> 
> 
> 
> Default value:* 9000





#### CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE



> Enable app rollback support
> 
> 
> *Found in:* Bootloader config
> 
> 
> After updating the app, the bootloader runs a new app with the "ESP_OTA_IMG_PENDING_VERIFY" state set.
> This state prevents the re-run of this app. After the first boot of the new app in the user code, the
> function should be called to confirm the operability of the app or vice versa about its non-operability.
> If the app is working, then it is marked as valid. Otherwise, it is marked as not valid and rolls back to
> the previous working app. A reboot is performed, and the app is booted before the software update.
> Note: If during the first boot a new app the power goes out or the WDT works, then roll back will happen.
> Rollback is possible only between the apps with the same security versions.
> 
> 
> 
> Default value:* No (disabled)



##### CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK



> Enable app anti-rollback support
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE
> 
> 
> This option prevents rollback to previous firmware/application image with lower security version.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE



###### CONFIG_BOOTLOADER_APP_SECURE_VERSION



> eFuse secure version of app
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE > CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
> 
> 
> The secure version is the sequence number stored in the header of each firmware.
> The security version is set in the bootloader, version is recorded in the eFuse field
> as the number of set ones. The allocated number of bits in the efuse field
> for storing the security version is limited (see BOOTLOADER_APP_SEC_VER_SIZE_EFUSE_FIELD option).
> 
> 
> Bootloader: When bootloader selects an app to boot, an app is selected that has
> a security version greater or equal that recorded in eFuse field.
> The app is booted with a higher (or equal) secure version.
> 
> 
> The security version is worth increasing if in previous versions there is
> a significant vulnerability and their use is not acceptable.
> 
> 
> Your partition table should has a scheme with ota_0 + ota_1 (without factory).
> 
> 
> 
> Default value:* 0 if CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK




###### CONFIG_BOOTLOADER_APP_SEC_VER_SIZE_EFUSE_FIELD



> Size of the efuse secure version field
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE > CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
> 
> 
> The size of the efuse secure version field.
> Its length is limited to 32 bits for ESP32 and 16 bits for ESP32-S2.
> This determines how many times the security version can be increased.
> 
> 
> 
> Range:* from 1 to 16 if CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
> 
> 
> 
> Default value:* 16 if CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK




###### CONFIG_BOOTLOADER_EFUSE_SECURE_VERSION_EMULATE



> Emulate operations with efuse secure version(only test)
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE > CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
> 
> 
> This option allows to emulate read/write operations with all eFuses and efuse secure version.
> It allows to test anti-rollback implementation without permanent write eFuse bits.
> There should be an entry in partition table with following details: emul_efuse, data, efuse, , 0x2000.
> 
> 
> This option enables: EFUSE_VIRTUAL and EFUSE_VIRTUAL_KEEP_IN_FLASH.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK






#### CONFIG_BOOTLOADER_SKIP_VALIDATE_IN_DEEP_SLEEP



> Skip image validation when exiting deep sleep
> 
> 
> *Found in:* Bootloader config
> 
> 
> This option disables the normal validation of an image coming out of
> deep sleep (checksums, SHA256, and signature). This is a trade-off
> between wakeup performance from deep sleep, and image integrity checks.
> 
> 
> Only enable this if you know what you are doing. It should not be used
> in conjunction with using deep_sleep() entry and changing the active OTA
> partition as this would skip the validation upon first load of the new
> OTA partition.
> 
> 
> It is possible to enable this option with Secure Boot if "allow insecure
> options" is enabled, however it's strongly recommended to NOT enable it as
> it may allow a Secure Boot bypass.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_BOOT && CONFIG_SECURE_BOOT_INSECURE




#### CONFIG_BOOTLOADER_SKIP_VALIDATE_ON_POWER_ON



> Skip image validation from power on reset (READ HELP FIRST)
> 
> 
> *Found in:* Bootloader config
> 
> 
> Some applications need to boot very quickly from power on. By default, the entire app binary
> is read from flash and verified which takes up a significant portion of the boot time.
> 
> 
> Enabling this option will skip validation of the app when the SoC boots from power on.
> Note that in this case it's not possible for the bootloader to detect if an app image is
> corrupted in the flash, therefore it's not possible to safely fall back to a different app
> partition. Flash corruption of this kind is unlikely but can happen if there is a serious
> firmware bug or physical damage.
> 
> 
> Following other reset types, the bootloader will still validate the app image. This increases
> the chances that flash corruption resulting in a crash can be detected following soft reset, and
> the bootloader will fall back to a valid app image. To increase the chances of successfully recovering
> from a flash corruption event, keep the option BOOTLOADER_WDT_ENABLE enabled and consider also enabling
> BOOTLOADER_WDT_DISABLE_IN_USER_CODE - then manually disable the RTC Watchdog once the app is running.
> In addition, enable both the Task and Interrupt watchdog timers with reset options set.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_BOOTLOADER_SKIP_VALIDATE_ALWAYS



> Skip image validation always (READ HELP FIRST)
> 
> 
> *Found in:* Bootloader config
> 
> 
> Selecting this option prevents the bootloader from ever validating the app image before
> booting it. Any flash corruption of the selected app partition will make the entire SoC
> unbootable.
> 
> 
> Although flash corruption is a very rare case, it is not recommended to select this option.
> Consider selecting "Skip image validation from power on reset" instead. However, if boot time
> is the only important factor then it can be enabled.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC



> Reserve RTC FAST memory for custom purposes
> 
> 
> *Found in:* Bootloader config
> 
> 
> This option allows the customer to place data in the RTC FAST memory,
> this area remains valid when rebooted, except for power loss.
> This memory is located at a fixed address and is available
> for both the bootloader and the application.
> (The application and bootloader must be compiled with the same option).
> The RTC FAST memory has access only through PRO_CPU.
> 
> 
> 
> Default value:* No (disabled)



##### CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC_IN_CRC



> Include custom memory in the CRC calculation
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC
> 
> 
> This option allows the customer to use the legacy bootloader behavior when the
> RTC FAST memory CRC calculation takes place. When this option is enabled, the
> allocated user custom data will be taken into account in the CRC calculation.
> This means that any change to the custom data would need a CRC update to prevent
> the bootloader from marking this data as corrupted.
> If this option is disabled, the custom data will not be taken into account when
> calculating the RTC FAST memory CRC. The user custom data can be changed freely,
> without the need to update the CRC.
> THIS OPTION MUST BE THE SAME FOR BOTH THE BOOTLOADER AND THE APPLICATION BUILDS.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC




##### CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC_SIZE



> Size in bytes for custom purposes
> 
> 
> *Found in:* Bootloader config > CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC
> 
> 
> This option reserves in RTC FAST memory the area for custom purposes.
> If you want to create your own bootloader and save more information
> in this area of memory, you can increase it. It must be a multiple of 4 bytes.
> This area (rtc_retain_mem_t) is reserved and has access from the bootloader and an application.
> 
> 
> 
> Default value:* 0 if CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC






### Security features


Contains:


* CONFIG_SECURE_BOOT_INSECURE
* CONFIG_SECURE_SIGNED_APPS_SCHEME
* CONFIG_SECURE_SIGNED_ON_BOOT_NO_SECURE_BOOT
* CONFIG_SECURE_FLASH_CHECK_ENC_EN_IN_APP
* CONFIG_SECURE_BOOT_ECDSA_KEY_LEN_SIZE
* CONFIG_SECURE_BOOT_ENABLE_AGGRESSIVE_KEY_REVOKE
* CONFIG_SECURE_FLASH_ENC_ENABLED
* CONFIG_SECURE_BOOT
* CONFIG_SECURE_FLASH_ENCRYPT_ONLY_IMAGE_LEN_IN_APP_PART
* CONFIG_SECURE_BOOT_FLASH_BOOTLOADER_DEFAULT
* CONFIG_SECURE_BOOTLOADER_KEY_ENCODING
* Potentially insecure options
* CONFIG_SECURE_SIGNED_APPS_NO_SECURE_BOOT
* CONFIG_SECURE_BOOT_VERIFICATION_KEY
* CONFIG_SECURE_BOOTLOADER_MODE
* CONFIG_SECURE_BOOT_BUILD_SIGNED_BINARIES
* CONFIG_SECURE_UART_ROM_DL_MODE
* CONFIG_SECURE_SIGNED_ON_UPDATE_NO_SECURE_BOOT



#### CONFIG_SECURE_SIGNED_APPS_NO_SECURE_BOOT



> Require signed app images
> 
> 
> *Found in:* Security features
> 
> 
> Require apps to be signed to verify their integrity.
> 
> 
> This option uses the same app signature scheme as hardware secure boot, but unlike hardware secure boot it
> does not prevent the bootloader from being physically updated. This means that the device can be secured
> against remote network access, but not physical access. Compared to using hardware Secure Boot this option
> is much simpler to implement.




#### CONFIG_SECURE_SIGNED_APPS_SCHEME



> App Signing Scheme
> 
> 
> *Found in:* Security features
> 
> 
> Select the Secure App signing scheme. Depends on the Chip Revision.
> There are two secure boot versions:
> 
> 
> 1. Secure boot V1
> 	* Legacy custom secure boot scheme. Supported in ESP32 SoC.
> 2. Secure boot V2
> 	* RSA based secure boot scheme.
> 	Supported in ESP32-ECO3 (ESP32 Chip Revision 3 onwards), ESP32-S2, ESP32-C3, ESP32-S3 SoCs.
> 	* ECDSA based secure boot scheme. Supported in ESP32-C2 SoC.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * ECDSA (CONFIG_SECURE_SIGNED_APPS_ECDSA_SCHEME)
> > 
> > 
> > Embeds the ECDSA public key in the bootloader and signs the application with an ECDSA key.
> > Refer to the documentation before enabling.
> > * RSA (CONFIG_SECURE_SIGNED_APPS_RSA_SCHEME)
> > 
> > 
> > Appends the RSA-3072 based Signature block to the application.
> > Refer to <Secure Boot Version 2 documentation link> before enabling.
> > * ECDSA (V2) (CONFIG_SECURE_SIGNED_APPS_ECDSA_V2_SCHEME)
> > 
> > 
> > For Secure boot V2 (e.g., ESP32-C2 SoC), appends ECDSA based signature block to the application.
> > Refer to documentation before enabling.




#### CONFIG_SECURE_BOOT_ECDSA_KEY_LEN_SIZE



> ECDSA key size
> 
> 
> *Found in:* Security features
> 
> 
> Select the ECDSA key size. Two key sizes are supported
> 
> 
> * 192 bit key using NISTP192 curve
> * 256 bit key using NISTP256 curve (Recommended)
> 
> 
> The advantage of using 256 bit key is the extra randomness which makes it difficult to be
> bruteforced compared to 192 bit key.
> At present, both key sizes are practically implausible to bruteforce.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Using ECC curve NISTP192 (CONFIG_SECURE_BOOT_ECDSA_KEY_LEN_192_BITS)
> > * Using ECC curve NISTP256 (Recommended) (CONFIG_SECURE_BOOT_ECDSA_KEY_LEN_256_BITS)




#### CONFIG_SECURE_SIGNED_ON_BOOT_NO_SECURE_BOOT



> Bootloader verifies app signatures
> 
> 
> *Found in:* Security features
> 
> 
> If this option is set, the bootloader will be compiled with code to verify that an app is signed before
> booting it.
> 
> 
> If hardware secure boot is enabled, this option is always enabled and cannot be disabled.
> If hardware secure boot is not enabled, this option doesn't add significant security by itself so most
> users will want to leave it disabled.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_SIGNED_APPS_NO_SECURE_BOOT && CONFIG_SECURE_SIGNED_APPS_ECDSA_SCHEME




#### CONFIG_SECURE_SIGNED_ON_UPDATE_NO_SECURE_BOOT



> Verify app signature on update
> 
> 
> *Found in:* Security features
> 
> 
> If this option is set, any OTA updated apps will have the signature verified before being considered valid.
> 
> 
> When enabled, the signature is automatically checked whenever the esp_ota_ops.h APIs are used for OTA
> updates, or esp_image_format.h APIs are used to verify apps.
> 
> 
> If hardware secure boot is enabled, this option is always enabled and cannot be disabled.
> If hardware secure boot is not enabled, this option still adds significant security against network-based
> attackers by preventing spoofing of OTA updates.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_SECURE_SIGNED_APPS_NO_SECURE_BOOT




#### CONFIG_SECURE_BOOT



> Enable hardware Secure Boot in bootloader (READ DOCS FIRST)
> 
> 
> *Found in:* Security features
> 
> 
> Build a bootloader which enables Secure Boot on first boot.
> 
> 
> Once enabled, Secure Boot will not boot a modified bootloader. The bootloader will only load a partition
> table or boot an app if the data has a verified digital signature. There are implications for reflashing
> updated apps once secure boot is enabled.
> 
> 
> When enabling secure boot, JTAG and ROM BASIC Interpreter are permanently disabled by default.
> 
> 
> 
> Default value:* No (disabled)



##### CONFIG_SECURE_BOOT_VERSION



> Select secure boot version
> 
> 
> *Found in:* Security features > CONFIG_SECURE_BOOT
> 
> 
> Select the Secure Boot Version. Depends on the Chip Revision.
> Secure Boot V2 is the new RSA / ECDSA based secure boot scheme.
> 
> 
> 
> > * RSA based scheme is supported in ESP32 (Revision 3 onwards), ESP32-S2, ESP32-C3 (ECO3), ESP32-S3.
> > * ECDSA based scheme is supported in ESP32-C2 SoC.
> 
> 
> Please note that, RSA or ECDSA secure boot is property of specific SoC based on its HW design, supported
> crypto accelerators, die-size, cost and similar parameters. Please note that RSA scheme has requirement
> for bigger key sizes but at the same time it is comparatively faster than ECDSA verification.
> 
> 
> Secure Boot V1 is the AES based (custom) secure boot scheme supported in ESP32 SoC.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Enable Secure Boot version 1 (CONFIG_SECURE_BOOT_V1_ENABLED)
> > 
> > 
> > Build a bootloader which enables secure boot version 1 on first boot.
> > Refer to the Secure Boot section of the ESP-IDF Programmer's Guide for this version before enabling.
> > * Enable Secure Boot version 2 (CONFIG_SECURE_BOOT_V2_ENABLED)
> > 
> > 
> > Build a bootloader which enables Secure Boot version 2 on first boot.
> > Refer to Secure Boot V2 section of the ESP-IDF Programmer's Guide for this version before enabling.





#### CONFIG_SECURE_BOOTLOADER_MODE



> Secure bootloader mode
> 
> 
> *Found in:* Security features
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * One-time flash (CONFIG_SECURE_BOOTLOADER_ONE_TIME_FLASH)
> > 
> > 
> > On first boot, the bootloader will generate a key which is not readable externally or by software. A
> > digest is generated from the bootloader image itself. This digest will be verified on each subsequent
> > boot.
> > 
> > 
> > Enabling this option means that the bootloader cannot be changed after the first time it is booted.
> > * Reflashable (CONFIG_SECURE_BOOTLOADER_REFLASHABLE)
> > 
> > 
> > Generate a reusable secure bootloader key, derived (via SHA-256) from the secure boot signing key.
> > 
> > 
> > This allows the secure bootloader to be re-flashed by anyone with access to the secure boot signing
> > key.
> > 
> > 
> > This option is less secure than one-time flash, because a leak of the digest key from one device
> > allows reflashing of any device that uses it.




#### CONFIG_SECURE_BOOT_BUILD_SIGNED_BINARIES



> Sign binaries during build
> 
> 
> *Found in:* Security features
> 
> 
> Once secure boot or signed app requirement is enabled, app images are required to be signed.
> 
> 
> If enabled (default), these binary files are signed as part of the build process. The file named in
> "Secure boot private signing key" will be used to sign the image.
> 
> 
> If disabled, unsigned app/partition data will be built. They must be signed manually using espsecure.py.
> Version 1 to enable ECDSA Based Secure Boot and Version 2 to enable RSA based Secure Boot.
> (for example, on a remote signing server.)



##### CONFIG_SECURE_BOOT_SIGNING_KEY



> Secure boot private signing key
> 
> 
> *Found in:* Security features > CONFIG_SECURE_BOOT_BUILD_SIGNED_BINARIES
> 
> 
> Path to the key file used to sign app images.
> 
> 
> Key file is an ECDSA private key (NIST256p curve) in PEM format for Secure Boot V1.
> Key file is an RSA private key in PEM format for Secure Boot V2.
> 
> 
> Path is evaluated relative to the project directory.
> 
> 
> You can generate a new signing key by running the following command:
> espsecure.py generate_signing_key secure_boot_signing_key.pem
> 
> 
> See the Secure Boot section of the ESP-IDF Programmer's Guide for this version for details.
> 
> 
> 
> Default value:* "secure_boot_signing_key.pem" if CONFIG_SECURE_BOOT_BUILD_SIGNED_BINARIES





#### CONFIG_SECURE_BOOT_VERIFICATION_KEY



> Secure boot public signature verification key
> 
> 
> *Found in:* Security features
> 
> 
> Path to a public key file used to verify signed images.
> Secure Boot V1: This ECDSA public key is compiled into the bootloader and/or
> app, to verify app images.
> 
> 
> Key file is in raw binary format, and can be extracted from a
> PEM formatted private key using the espsecure.py
> extract_public_key command.
> 
> 
> Refer to the Secure Boot section of the ESP-IDF Programmer's Guide for this version before enabling.




#### CONFIG_SECURE_BOOT_ENABLE_AGGRESSIVE_KEY_REVOKE



> Enable Aggressive key revoke strategy
> 
> 
> *Found in:* Security features
> 
> 
> If this option is set, ROM bootloader will revoke the public key digest burned in efuse block
> if it fails to verify the signature of software bootloader with it.
> Revocation of keys does not happen when enabling secure boot. Once secure boot is enabled,
> key revocation checks will be done on subsequent boot-up, while verifying the software bootloader
> 
> 
> This feature provides a strong resistance against physical attacks on the device.
> 
> 
> NOTE: Once a digest slot is revoked, it can never be used again to verify an image
> This can lead to permanent bricking of the device, in case all keys are revoked
> because of signature verification failure.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_BOOT




#### CONFIG_SECURE_BOOT_FLASH_BOOTLOADER_DEFAULT



> Flash bootloader along with other artifacts when using the default flash command
> 
> 
> *Found in:* Security features
> 
> 
> When Secure Boot V2 is enabled, by default the bootloader is not flashed along with other artifacts
> like the application and the partition table images, i.e. bootloader has to be separately flashed
> using the command idf.py bootloader flash, whereas, the application and partition table can be flashed
> using the command idf.py flash itself.
> Enabling this option allows flashing the bootloader along with the other artifacts
> by invocation of the command idf.py flash.
> 
> 
> If this option is enabled make sure that even the bootloader is signed using the correct secure boot key,
> otherwise the bootloader signature verification would fail, as hash of the public key which is present in
> the bootloader signature would not match with the digest stored into the efuses
> and thus the device will not be able to boot up.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_BOOT_V2_ENABLED && CONFIG_SECURE_BOOT_BUILD_SIGNED_BINARIES




#### CONFIG_SECURE_BOOTLOADER_KEY_ENCODING



> Hardware Key Encoding
> 
> 
> *Found in:* Security features
> 
> 
> In reflashable secure bootloader mode, a hardware key is derived from the signing key (with SHA-256) and
> can be written to eFuse with espefuse.py.
> 
> 
> Normally this is a 256-bit key, but if 3/4 Coding Scheme is used on the device then the eFuse key is
> truncated to 192 bits.
> 
> 
> This configuration item doesn't change any firmware code, it only changes the size of key binary which is
> generated at build time.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No encoding (256 bit key) (CONFIG_SECURE_BOOTLOADER_KEY_ENCODING_256BIT)
> > * 3/4 encoding (192 bit key) (CONFIG_SECURE_BOOTLOADER_KEY_ENCODING_192BIT)




#### CONFIG_SECURE_BOOT_INSECURE



> Allow potentially insecure options
> 
> 
> *Found in:* Security features
> 
> 
> You can disable some of the default protections offered by secure boot, in order to enable testing or a
> custom combination of security features.
> 
> 
> Only enable these options if you are very sure.
> 
> 
> Refer to the Secure Boot section of the ESP-IDF Programmer's Guide for this version before enabling.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_BOOT




#### CONFIG_SECURE_FLASH_ENC_ENABLED



> Enable flash encryption on boot (READ DOCS FIRST)
> 
> 
> *Found in:* Security features
> 
> 
> If this option is set, flash contents will be encrypted by the bootloader on first boot.
> 
> 
> Note: After first boot, the system will be permanently encrypted. Re-flashing an encrypted
> system is complicated and not always possible.
> 
> 
> Read Flash Encryption
> before enabling.
> 
> 
> 
> Default value:* No (disabled)



##### CONFIG_SECURE_FLASH_ENCRYPTION_KEYSIZE



> Size of generated XTS-AES key
> 
> 
> *Found in:* Security features > CONFIG_SECURE_FLASH_ENC_ENABLED
> 
> 
> Size of generated XTS-AES key.
> 
> 
> * AES-128 uses a 256-bit key (32 bytes) derived from 128 bits (16 bytes) burned in half Efuse key block.
> Internally, it calculates SHA256(128 bits)
> * AES-128 uses a 256-bit key (32 bytes) which occupies one Efuse key block.
> * AES-256 uses a 512-bit key (64 bytes) which occupies two Efuse key blocks.
> 
> 
> This setting is ignored if either type of key is already burned to Efuse before the first boot.
> In this case, the pre-burned key is used and no new key is generated.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * AES-128 key derived from 128 bits (SHA256(128 bits)) (CONFIG_SECURE_FLASH_ENCRYPTION_AES128_DERIVED)
> > * AES-128 (256-bit key) (CONFIG_SECURE_FLASH_ENCRYPTION_AES128)
> > * AES-256 (512-bit key) (CONFIG_SECURE_FLASH_ENCRYPTION_AES256)




##### CONFIG_SECURE_FLASH_ENCRYPTION_MODE



> Enable usage mode
> 
> 
> *Found in:* Security features > CONFIG_SECURE_FLASH_ENC_ENABLED
> 
> 
> By default Development mode is enabled which allows ROM download mode to perform flash encryption
> operations (plaintext is sent to the device, and it encrypts it internally and writes ciphertext
> to flash.) This mode is not secure, it's possible for an attacker to write their own chosen plaintext
> to flash.
> 
> 
> Release mode should always be selected for production or manufacturing. Once enabled it's no longer
> possible for the device in ROM Download Mode to use the flash encryption hardware.
> 
> 
> When EFUSE_VIRTUAL is enabled, SECURE_FLASH_ENCRYPTION_MODE_RELEASE is not available.
> For CI tests we use IDF_CI_BUILD to bypass it ("export IDF_CI_BUILD=1").
> We do not recommend bypassing it for other purposes.
> 
> 
> Refer to the Flash Encryption section of the ESP-IDF Programmer's Guide for details.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Development (NOT SECURE) (CONFIG_SECURE_FLASH_ENCRYPTION_MODE_DEVELOPMENT)
> > * Release (CONFIG_SECURE_FLASH_ENCRYPTION_MODE_RELEASE)





#### Potentially insecure options


Contains:


* CONFIG_SECURE_BOOT_V2_ALLOW_EFUSE_RD_DIS
* CONFIG_SECURE_BOOT_ALLOW_SHORT_APP_PARTITION
* CONFIG_SECURE_BOOT_ALLOW_JTAG
* CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_ENC
* CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_CACHE
* CONFIG_SECURE_BOOT_ALLOW_UNUSED_DIGEST_SLOTS
* CONFIG_SECURE_FLASH_REQUIRE_ALREADY_ENABLED
* CONFIG_SECURE_FLASH_SKIP_WRITE_PROTECTION_CACHE



##### CONFIG_SECURE_BOOT_ALLOW_JTAG



> Allow JTAG Debugging
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default), the bootloader will permanently disable JTAG (across entire chip) on first boot
> when either secure boot or flash encryption is enabled.
> 
> 
> Setting this option leaves JTAG on for debugging, which negates all protections of flash encryption
> and some of the protections of secure boot.
> 
> 
> Only set this option in testing environments.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_BOOT_INSECURE || CONFIG_SECURE_FLASH_ENCRYPTION_MODE_DEVELOPMENT




##### CONFIG_SECURE_BOOT_ALLOW_SHORT_APP_PARTITION



> Allow app partition length not 64KB aligned
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default), app partition size must be a multiple of 64KB. App images are padded to 64KB
> length, and the bootloader checks any trailing bytes after the signature (before the next 64KB
> boundary) have not been written. This is because flash cache maps entire 64KB pages into the address
> space. This prevents an attacker from appending unverified data after the app image in the flash,
> causing it to be mapped into the address space.
> 
> 
> Setting this option allows the app partition length to be unaligned, and disables padding of the app
> image to this length. It is generally not recommended to set this option, unless you have a legacy
> partitioning scheme which doesn't support 64KB aligned partition lengths.




##### CONFIG_SECURE_BOOT_V2_ALLOW_EFUSE_RD_DIS



> Allow additional read protecting of efuses
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default, recommended), on first boot the bootloader will burn the WR_DIS_RD_DIS
> efuse when Secure Boot is enabled. This prevents any more efuses from being read protected.
> 
> 
> If this option is set, it will remain possible to write the EFUSE_RD_DIS efuse field after Secure
> Boot is enabled. This may allow an attacker to read-protect the BLK2 efuse (for ESP32) and
> BLOCK4-BLOCK10 (i.e. BLOCK_KEY0-BLOCK_KEY5)(for other chips) holding the public key digest, causing an
> immediate denial of service and possibly allowing an additional fault injection attack to
> bypass the signature protection.
> 
> 
> NOTE: Once a BLOCK is read-protected, the application will read all zeros from that block
> 
> 
> NOTE: If "UART ROM download mode (Permanently disabled (recommended))" or
> "UART ROM download mode (Permanently switch to Secure mode (recommended))" is set,
> then it is __NOT__ possible to read/write efuses using espefuse.py utility.
> However, efuse can be read/written from the application




##### CONFIG_SECURE_BOOT_ALLOW_UNUSED_DIGEST_SLOTS



> Leave unused digest slots available (not revoke)
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default), during startup in the app all unused digest slots will be revoked.
> To revoke unused slot will be called esp_efuse_set_digest_revoke(num_digest) for each digest.
> Revoking unused digest slots makes ensures that no trusted keys can be added later by an attacker.
> If set, it means that you have a plan to use unused digests slots later.
> 
> 
> Note that if you plan to enable secure boot during the first boot up, the bootloader will intentionally
> revoke the unused digest slots while enabling secure boot, even if the above config is enabled because
> keeping the unused key slots un-revoked would a security hazard.
> In case for any development workflow if you need to avoid this revocation, you should enable
> secure boot externally (host based mechanism) rather than enabling it during the boot up,
> so that the bootloader would not need to enable secure boot and thus you could avoid its revocation
> strategy.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_BOOT_INSECURE




##### CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_ENC



> Leave UART bootloader encryption enabled
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default), the bootloader will permanently disable UART bootloader encryption access on
> first boot. If set, the UART bootloader will still be able to access hardware encryption.
> 
> 
> It is recommended to only set this option in testing environments.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_FLASH_ENCRYPTION_MODE_DEVELOPMENT




##### CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_CACHE



> Leave UART bootloader flash cache enabled
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default), the bootloader will permanently disable UART bootloader flash cache access on
> first boot. If set, the UART bootloader will still be able to access the flash cache.
> 
> 
> Only set this option in testing environments.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_FLASH_ENCRYPTION_MODE_DEVELOPMENT




##### CONFIG_SECURE_FLASH_REQUIRE_ALREADY_ENABLED



> Require flash encryption to be already enabled
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default), and flash encryption is not yet enabled in eFuses, the 2nd stage bootloader
> will enable flash encryption: generate the flash encryption key and program eFuses.
> If this option is set, and flash encryption is not yet enabled, the bootloader will error out and
> reboot.
> If flash encryption is enabled in eFuses, this option does not change the bootloader behavior.
> 
> 
> Only use this option in testing environments, to avoid accidentally enabling flash encryption on
> the wrong device. The device needs to have flash encryption already enabled using espefuse.py.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SECURE_FLASH_ENCRYPTION_MODE_DEVELOPMENT




##### CONFIG_SECURE_FLASH_SKIP_WRITE_PROTECTION_CACHE



> Skip write-protection of DIS_CACHE (DIS_ICACHE, DIS_DCACHE)
> 
> 
> *Found in:* Security features > Potentially insecure options
> 
> 
> If not set (default, recommended), on the first boot the bootloader will burn the write-protection of
> DIS_CACHE(for ESP32) or DIS_ICACHE/DIS_DCACHE(for other chips) eFuse when Flash Encryption is enabled.
> Write protection for cache disable efuse prevents the chip from being blocked if it is set by accident.
> App and bootloader use cache so disabling it makes the chip useless for IDF.
> Due to other eFuses are linked with the same write protection bit (see the list below) then
> write-protection will not be done if these SECURE_FLASH_UART_BOOTLOADER_ALLOW_ENC,
> SECURE_BOOT_ALLOW_JTAG or SECURE_FLASH_UART_BOOTLOADER_ALLOW_CACHE options are selected
> to give a chance to turn on the chip into the release mode later.
> 
> 
> List of eFuses with the same write protection bit:
> ESP32: MAC, MAC_CRC, DISABLE_APP_CPU, DISABLE_BT, DIS_CACHE, VOL_LEVEL_HP_INV.
> 
> 
> ESP32-C3: DIS_ICACHE, DIS_USB_JTAG, DIS_DOWNLOAD_ICACHE, DIS_USB_SERIAL_JTAG,
> DIS_FORCE_DOWNLOAD, DIS_TWAI, JTAG_SEL_ENABLE, DIS_PAD_JTAG, DIS_DOWNLOAD_MANUAL_ENCRYPT.
> 
> 
> ESP32-C6: SWAP_UART_SDIO_EN, DIS_ICACHE, DIS_USB_JTAG, DIS_DOWNLOAD_ICACHE,
> DIS_USB_SERIAL_JTAG, DIS_FORCE_DOWNLOAD, DIS_TWAI, JTAG_SEL_ENABLE,
> DIS_PAD_JTAG, DIS_DOWNLOAD_MANUAL_ENCRYPT.
> 
> 
> ESP32-H2: DIS_ICACHE, DIS_USB_JTAG, POWERGLITCH_EN, DIS_FORCE_DOWNLOAD, SPI_DOWNLOAD_MSPI_DIS,
> DIS_TWAI, JTAG_SEL_ENABLE, DIS_PAD_JTAG, DIS_DOWNLOAD_MANUAL_ENCRYPT.
> 
> 
> ESP32-S2: DIS_ICACHE, DIS_DCACHE, DIS_DOWNLOAD_ICACHE, DIS_DOWNLOAD_DCACHE,
> DIS_FORCE_DOWNLOAD, DIS_USB, DIS_TWAI, DIS_BOOT_REMAP, SOFT_DIS_JTAG,
> HARD_DIS_JTAG, DIS_DOWNLOAD_MANUAL_ENCRYPT.
> 
> 
> ESP32-S3: DIS_ICACHE, DIS_DCACHE, DIS_DOWNLOAD_ICACHE, DIS_DOWNLOAD_DCACHE,
> DIS_FORCE_DOWNLOAD, DIS_USB_OTG, DIS_TWAI, DIS_APP_CPU, DIS_PAD_JTAG,
> DIS_DOWNLOAD_MANUAL_ENCRYPT, DIS_USB_JTAG, DIS_USB_SERIAL_JTAG, STRAP_JTAG_SEL, USB_PHY_SEL.





#### CONFIG_SECURE_FLASH_ENCRYPT_ONLY_IMAGE_LEN_IN_APP_PART



> Encrypt only the app image that is present in the partition of type app
> 
> 
> *Found in:* Security features
> 
> 
> If set (default), optimise encryption time for the partition of type APP,
> by only encrypting the app image that is present in the partition,
> instead of the whole partition.
> The image length used for encryption is derived from the image metadata, which
> includes the size of the app image, checksum, hash and also the signature sector
> when secure boot is enabled.
> 
> 
> If not set, the whole partition of type APP would be encrypted,
> which increases the encryption time but might be useful if there
> is any custom data appended to the firmware image.




#### CONFIG_SECURE_FLASH_CHECK_ENC_EN_IN_APP



> Check Flash Encryption enabled on app startup
> 
> 
> *Found in:* Security features
> 
> 
> If set (default), in an app during startup code,
> there is a check of the flash encryption eFuse bit is on
> (as the bootloader should already have set it).
> The app requires this bit is on to continue work otherwise abort.
> 
> 
> If not set, the app does not care if the flash encryption eFuse bit is set or not.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_SECURE_FLASH_ENC_ENABLED




#### CONFIG_SECURE_UART_ROM_DL_MODE



> UART ROM download mode
> 
> 
> *Found in:* Security features
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * UART ROM download mode (Permanently disabled (recommended)) (CONFIG_SECURE_DISABLE_ROM_DL_MODE)
> > 
> > 
> > If set, during startup the app will burn an eFuse bit to permanently disable the UART ROM
> > Download Mode. This prevents any future use of esptool.py, espefuse.py and similar tools.
> > 
> > 
> > Once disabled, if the SoC is booted with strapping pins set for ROM Download Mode
> > then an error is printed instead.
> > 
> > 
> > It is recommended to enable this option in any production application where Flash
> > Encryption and/or Secure Boot is enabled and access to Download Mode is not required.
> > 
> > 
> > It is also possible to permanently disable Download Mode by calling
> > esp_efuse_disable_rom_download_mode() at runtime.
> > * UART ROM download mode (Permanently switch to Secure mode (recommended)) (CONFIG_SECURE_ENABLE_SECURE_ROM_DL_MODE)
> > 
> > 
> > If set, during startup the app will burn an eFuse bit to permanently switch the UART ROM
> > Download Mode into a separate Secure Download mode. This option can only work if
> > Download Mode is not already disabled by eFuse.
> > 
> > 
> > Secure Download mode limits the use of Download Mode functions to update SPI config,
> > changing baud rate, basic flash write and a command to return a summary of currently
> > enabled security features (get_security_info).
> > 
> > 
> > Secure Download mode is not compatible with the esptool.py flasher stub feature,
> > espefuse.py, read/writing memory or registers, encrypted download, or any other
> > features that interact with unsupported Download Mode commands.
> > 
> > 
> > Secure Download mode should be enabled in any application where Flash Encryption
> > and/or Secure Boot is enabled. Disabling this option does not immediately cancel
> > the benefits of the security features, but it increases the potential "attack
> > surface" for an attacker to try and bypass them with a successful physical attack.
> > 
> > 
> > It is also possible to enable secure download mode at runtime by calling
> > esp_efuse_enable_rom_secure_download_mode()
> > 
> > 
> > Note: Secure Download mode is not available for ESP32 (includes revisions till ECO3).
> > * UART ROM download mode (Enabled (not recommended)) (CONFIG_SECURE_INSECURE_ALLOW_DL_MODE)
> > 
> > 
> > This is a potentially insecure option.
> > Enabling this option will allow the full UART download mode to stay enabled.
> > This option SHOULD NOT BE ENABLED for production use cases.





### Application manager


Contains:


* CONFIG_APP_EXCLUDE_PROJECT_NAME_VAR
* CONFIG_APP_EXCLUDE_PROJECT_VER_VAR
* CONFIG_APP_PROJECT_VER_FROM_CONFIG
* CONFIG_APP_RETRIEVE_LEN_ELF_SHA
* CONFIG_APP_COMPILE_TIME_DATE



#### CONFIG_APP_COMPILE_TIME_DATE



> Use time/date stamp for app
> 
> 
> *Found in:* Application manager
> 
> 
> If set, then the app will be built with the current time/date stamp. It is stored in the app description
> structure. If not set, time/date stamp will be excluded from app image. This can be useful for getting the
> same binary image files made from the same source, but at different times.




#### CONFIG_APP_EXCLUDE_PROJECT_VER_VAR



> Exclude PROJECT_VER from firmware image
> 
> 
> *Found in:* Application manager
> 
> 
> The PROJECT_VER variable from the build system will not affect the firmware image.
> This value will not be contained in the esp_app_desc structure.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_APP_EXCLUDE_PROJECT_NAME_VAR



> Exclude PROJECT_NAME from firmware image
> 
> 
> *Found in:* Application manager
> 
> 
> The PROJECT_NAME variable from the build system will not affect the firmware image.
> This value will not be contained in the esp_app_desc structure.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_APP_PROJECT_VER_FROM_CONFIG



> Get the project version from Kconfig
> 
> 
> *Found in:* Application manager
> 
> 
> If this is enabled, then config item APP_PROJECT_VER will be used for the variable PROJECT_VER.
> Other ways to set PROJECT_VER will be ignored.
> 
> 
> 
> Default value:* No (disabled)



##### CONFIG_APP_PROJECT_VER



> Project version
> 
> 
> *Found in:* Application manager > CONFIG_APP_PROJECT_VER_FROM_CONFIG
> 
> 
> Project version
> 
> 
> 
> Default value:* 1 if CONFIG_APP_PROJECT_VER_FROM_CONFIG





#### CONFIG_APP_RETRIEVE_LEN_ELF_SHA



> The length of APP ELF SHA is stored in RAM(chars)
> 
> 
> *Found in:* Application manager
> 
> 
> At startup, the app will read the embedded APP ELF SHA-256 hash value from flash
> and convert it into a string and store it in a RAM buffer.
> This ensures the panic handler and core dump will be able to print this string
> even when cache is disabled.
> The size of the buffer is APP_RETRIEVE_LEN_ELF_SHA plus the null terminator.
> Changing this value will change the size of this buffer, in bytes.
> 
> 
> 
> Range:* from 8 to 64
> 
> 
> 
> Default value:* 9





### Boot ROM Behavior


Contains:


* CONFIG_BOOT_ROM_LOG_SCHEME



#### CONFIG_BOOT_ROM_LOG_SCHEME



> Permanently change Boot ROM output
> 
> 
> *Found in:* Boot ROM Behavior
> 
> 
> Controls the Boot ROM log behavior.
> The rom log behavior can only be changed for once,
> specific eFuse bit(s) will be burned at app boot stage.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Always Log (CONFIG_BOOT_ROM_LOG_ALWAYS_ON)
> > 
> > 
> > Always print ROM logs, this is the default behavior.
> > * Permanently disable logging (CONFIG_BOOT_ROM_LOG_ALWAYS_OFF)
> > 
> > 
> > Don't print ROM logs.
> > * Log on GPIO High (CONFIG_BOOT_ROM_LOG_ON_GPIO_HIGH)
> > 
> > 
> > Print ROM logs when GPIO level is high during start up.
> > The GPIO number is chip dependent,
> > e.g. on ESP32-S2, the control GPIO is GPIO46.
> > * Log on GPIO Low (CONFIG_BOOT_ROM_LOG_ON_GPIO_LOW)
> > 
> > 
> > Print ROM logs when GPIO level is low during start up.
> > The GPIO number is chip dependent,
> > e.g. on ESP32-S2, the control GPIO is GPIO46.





### Serial flasher config


Contains:


* CONFIG_ESPTOOLPY_AFTER
* CONFIG_ESPTOOLPY_BEFORE
* CONFIG_ESPTOOLPY_FLASH_MODE_AUTO_DETECT
* CONFIG_ESPTOOLPY_HEADER_FLASHSIZE_UPDATE
* CONFIG_ESPTOOLPY_NO_STUB
* CONFIG_ESPTOOLPY_OCT_FLASH
* CONFIG_ESPTOOLPY_FLASH_SAMPLE_MODE
* CONFIG_ESPTOOLPY_FLASHSIZE
* CONFIG_ESPTOOLPY_FLASHMODE
* CONFIG_ESPTOOLPY_FLASHFREQ



#### CONFIG_ESPTOOLPY_NO_STUB



> Disable download stub
> 
> 
> *Found in:* Serial flasher config
> 
> 
> The flasher tool sends a precompiled download stub first by default. That stub allows things
> like compressed downloads and more. Usually you should not need to disable that feature




#### CONFIG_ESPTOOLPY_OCT_FLASH



> Enable Octal Flash
> 
> 
> *Found in:* Serial flasher config




#### CONFIG_ESPTOOLPY_FLASH_MODE_AUTO_DETECT



> Choose flash mode automatically (please read help)
> 
> 
> *Found in:* Serial flasher config
> 
> 
> This config option helps decide whether flash is Quad or Octal, but please note some limitations:
> 
> 
> 1. If the flash chip is an Octal one, even if one of "QIO", "QOUT", "DIO", "DOUT" options is
> selected in ESPTOOLPY_FLASHMODE, our code will automatically change the
> mode to "OPI" and the sample mode will be STR.
> 2. If the flash chip is a Quad one, even if "OPI" is selected in ESPTOOLPY_FLASHMODE, our code will
> automatically change the mode to "DIO".
> 3. This option is mainly to improve the out-of-box experience of developers. It doesn't guarantee
> the feature-complete. Some code still rely on ESPTOOLPY_OCT_FLASH. Please do not rely on this option
> when you are pretty sure that you are using Octal flash.
> In this case, please enable ESPTOOLPY_OCT_FLASH option, then you can choose DTR sample mode
> in ESPTOOLPY_FLASH_SAMPLE_MODE. Otherwise, only STR mode is available.
> 4. Enabling this feature reduces available internal RAM size (around 900 bytes).
> If your IRAM space is insufficient and you're aware of your flash type,
> disable this option and select corresponding flash type options.




#### CONFIG_ESPTOOLPY_FLASHMODE



> Flash SPI mode
> 
> 
> *Found in:* Serial flasher config
> 
> 
> Mode the flash chip is flashed in, as well as the default mode for the
> binary to run in.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * QIO (CONFIG_ESPTOOLPY_FLASHMODE_QIO)
> > * QOUT (CONFIG_ESPTOOLPY_FLASHMODE_QOUT)
> > * DIO (CONFIG_ESPTOOLPY_FLASHMODE_DIO)
> > * DOUT (CONFIG_ESPTOOLPY_FLASHMODE_DOUT)
> > * OPI (CONFIG_ESPTOOLPY_FLASHMODE_OPI)




#### CONFIG_ESPTOOLPY_FLASH_SAMPLE_MODE



> Flash Sampling Mode
> 
> 
> *Found in:* Serial flasher config
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * STR Mode (CONFIG_ESPTOOLPY_FLASH_SAMPLE_MODE_STR)
> > * DTR Mode (CONFIG_ESPTOOLPY_FLASH_SAMPLE_MODE_DTR)




#### CONFIG_ESPTOOLPY_FLASHFREQ



> Flash SPI speed
> 
> 
> *Found in:* Serial flasher config
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 120 MHz (READ DOCS FIRST) (CONFIG_ESPTOOLPY_FLASHFREQ_120M)
> > 
> > 
> > 	+ Optional feature for QSPI Flash. Read docs and enable CONFIG_SPI_FLASH_HPM_ENA first!
> > 	+ Flash 120 MHz SDR mode is stable.
> > 	+ Flash 120 MHz DDR mode is an experimental feature, it works when
> > 	the temperature is stable.
> > 	
> > 	
> > 	
> > 	> Risks:If your chip powers on at a certain temperature, then after the temperature
> > 	> increases or decreases by approximately 20 Celsius degrees (depending on the
> > 	> chip), the program will crash randomly.
> > * 80 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_80M)
> > * 64 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_64M)
> > * 60 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_60M)
> > * 48 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_48M)
> > * 40 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_40M)
> > * 32 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_32M)
> > * 30 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_30M)
> > * 26 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_26M)
> > * 24 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_24M)
> > * 20 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_20M)
> > * 16 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_16M)
> > * 15 MHz (CONFIG_ESPTOOLPY_FLASHFREQ_15M)




#### CONFIG_ESPTOOLPY_FLASHSIZE



> Flash size
> 
> 
> *Found in:* Serial flasher config
> 
> 
> SPI flash size, in megabytes
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 1 MB (CONFIG_ESPTOOLPY_FLASHSIZE_1MB)
> > * 2 MB (CONFIG_ESPTOOLPY_FLASHSIZE_2MB)
> > * 4 MB (CONFIG_ESPTOOLPY_FLASHSIZE_4MB)
> > * 8 MB (CONFIG_ESPTOOLPY_FLASHSIZE_8MB)
> > * 16 MB (CONFIG_ESPTOOLPY_FLASHSIZE_16MB)
> > * 32 MB (CONFIG_ESPTOOLPY_FLASHSIZE_32MB)
> > * 64 MB (CONFIG_ESPTOOLPY_FLASHSIZE_64MB)
> > * 128 MB (CONFIG_ESPTOOLPY_FLASHSIZE_128MB)




#### CONFIG_ESPTOOLPY_HEADER_FLASHSIZE_UPDATE



> Detect flash size when flashing bootloader
> 
> 
> *Found in:* Serial flasher config
> 
> 
> If this option is set, flashing the project will automatically detect
> the flash size of the target chip and update the bootloader image
> before it is flashed.
> 
> 
> Enabling this option turns off the image protection against corruption
> by a SHA256 digest. Updating the bootloader image before flashing would
> invalidate the digest.




#### CONFIG_ESPTOOLPY_BEFORE



> Before flashing
> 
> 
> *Found in:* Serial flasher config
> 
> 
> Configure whether esptool.py should reset the ESP32 before flashing.
> 
> 
> Automatic resetting depends on the RTS & DTR signals being
> wired from the serial port to the ESP32. Most USB development
> boards do this internally.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Reset to bootloader (CONFIG_ESPTOOLPY_BEFORE_RESET)
> > * No reset (CONFIG_ESPTOOLPY_BEFORE_NORESET)




#### CONFIG_ESPTOOLPY_AFTER



> After flashing
> 
> 
> *Found in:* Serial flasher config
> 
> 
> Configure whether esptool.py should reset the ESP32 after flashing.
> 
> 
> Automatic resetting depends on the RTS & DTR signals being
> wired from the serial port to the ESP32. Most USB development
> boards do this internally.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Reset after flashing (CONFIG_ESPTOOLPY_AFTER_RESET)
> > * Stay in bootloader (CONFIG_ESPTOOLPY_AFTER_NORESET)





### Partition Table


Contains:


* CONFIG_PARTITION_TABLE_CUSTOM_FILENAME
* CONFIG_PARTITION_TABLE_MD5
* CONFIG_PARTITION_TABLE_OFFSET
* CONFIG_PARTITION_TABLE_TYPE



#### CONFIG_PARTITION_TABLE_TYPE



> Partition Table
> 
> 
> *Found in:* Partition Table
> 
> 
> The partition table to flash to the ESP32. The partition table
> determines where apps, data and other resources are expected to
> be found.
> 
> 
> The predefined partition table CSV descriptions can be found
> in the components/partition_table directory. These are mostly intended
> for example and development use, it's expect that for production use you
> will copy one of these CSV files and create a custom partition CSV for
> your application.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Single factory app, no OTA (CONFIG_PARTITION_TABLE_SINGLE_APP)
> > 
> > 
> > This is the default partition table, designed to fit into a 2MB or
> > larger flash with a single 1MB app partition.
> > 
> > 
> > The corresponding CSV file in the IDF directory is
> > components/partition_table/partitions_singleapp.csv
> > 
> > 
> > This partition table is not suitable for an app that needs OTA
> > (over the air update) capability.
> > * Single factory app (large), no OTA (CONFIG_PARTITION_TABLE_SINGLE_APP_LARGE)
> > 
> > 
> > This is a variation of the default partition table, that expands
> > the 1MB app partition size to 1.5MB to fit more code.
> > 
> > 
> > The corresponding CSV file in the IDF directory is
> > components/partition_table/partitions_singleapp_large.csv
> > 
> > 
> > This partition table is not suitable for an app that needs OTA
> > (over the air update) capability.
> > * Factory app, two OTA definitions (CONFIG_PARTITION_TABLE_TWO_OTA)
> > 
> > 
> > This is a basic OTA-enabled partition table with a factory app
> > partition plus two OTA app partitions. All are 1MB, so this
> > partition table requires 4MB or larger flash size.
> > 
> > 
> > The corresponding CSV file in the IDF directory is
> > components/partition_table/partitions_two_ota.csv
> > * Custom partition table CSV (CONFIG_PARTITION_TABLE_CUSTOM)
> > 
> > 
> > Specify the path to the partition table CSV to use for your project.
> > 
> > 
> > Consult the Partition Table section in the ESP-IDF Programmers Guide
> > for more information.
> > * Single factory app, no OTA, encrypted NVS (CONFIG_PARTITION_TABLE_SINGLE_APP_ENCRYPTED_NVS)
> > 
> > 
> > This is a variation of the default "Single factory app, no OTA" partition table
> > that supports encrypted NVS when using flash encryption. See the Flash Encryption section
> > in the ESP-IDF Programmers Guide for more information.
> > 
> > 
> > The corresponding CSV file in the IDF directory is
> > components/partition_table/partitions_singleapp_encr_nvs.csv
> > * Single factory app (large), no OTA, encrypted NVS (CONFIG_PARTITION_TABLE_SINGLE_APP_LARGE_ENC_NVS)
> > 
> > 
> > This is a variation of the "Single factory app (large), no OTA" partition table
> > that supports encrypted NVS when using flash encryption. See the Flash Encryption section
> > in the ESP-IDF Programmers Guide for more information.
> > 
> > 
> > The corresponding CSV file in the IDF directory is
> > components/partition_table/partitions_singleapp_large_encr_nvs.csv
> > * Factory app, two OTA definitions, encrypted NVS (CONFIG_PARTITION_TABLE_TWO_OTA_ENCRYPTED_NVS)
> > 
> > 
> > This is a variation of the "Factory app, two OTA definitions" partition table
> > that supports encrypted NVS when using flash encryption. See the Flash Encryption section
> > in the ESP-IDF Programmers Guide for more information.
> > 
> > 
> > The corresponding CSV file in the IDF directory is
> > components/partition_table/partitions_two_ota_encr_nvs.csv




#### CONFIG_PARTITION_TABLE_CUSTOM_FILENAME



> Custom partition CSV file
> 
> 
> *Found in:* Partition Table
> 
> 
> Name of the custom partition CSV filename.
> This path is evaluated relative to the project root directory by default.
> However, if the absolute path for the CSV file is provided, then the absolute path is configured.
> 
> 
> 
> Default value:* "partitions.csv"




#### CONFIG_PARTITION_TABLE_OFFSET



> Offset of partition table
> 
> 
> *Found in:* Partition Table
> 
> 
> The address of partition table (by default 0x8000).
> Allows you to move the partition table, it gives more space for the bootloader.
> Note that the bootloader and app will both need to be compiled with the same PARTITION_TABLE_OFFSET value.
> 
> 
> This number should be a multiple of 0x1000.
> 
> 
> Note that partition offsets in the partition table CSV file may need to be changed if this value is set to
> a higher value. To have each partition offset adapt to the configured partition table offset, leave all
> partition offsets blank in the CSV file.
> 
> 
> 
> Default value:* "0x8000"




#### CONFIG_PARTITION_TABLE_MD5



> Generate an MD5 checksum for the partition table
> 
> 
> *Found in:* Partition Table
> 
> 
> Generate an MD5 checksum for the partition table for protecting the
> integrity of the table. The generation should be turned off for legacy
> bootloaders which cannot recognize the MD5 checksum in the partition
> table.
> 
> 
> 
> Default value:* Yes (enabled)





### Compiler options


Contains:


* CONFIG_COMPILER_OPTIMIZATION_ASSERTION_LEVEL
* CONFIG_COMPILER_FLOAT_LIB_FROM
* CONFIG_COMPILER_RT_LIB
* CONFIG_COMPILER_OPTIMIZATION_CHECKS_SILENT
* CONFIG_COMPILER_DISABLE_GCC12_WARNINGS
* CONFIG_COMPILER_DISABLE_GCC13_WARNINGS
* CONFIG_COMPILER_DUMP_RTL_FILES
* CONFIG_COMPILER_WARN_WRITE_STRINGS
* CONFIG_COMPILER_CXX_EXCEPTIONS
* CONFIG_COMPILER_CXX_RTTI
* CONFIG_COMPILER_OPTIMIZATION
* CONFIG_COMPILER_ORPHAN_SECTIONS
* CONFIG_COMPILER_HIDE_PATHS_MACROS
* CONFIG_COMPILER_STACK_CHECK_MODE



#### CONFIG_COMPILER_OPTIMIZATION



> Optimization Level
> 
> 
> *Found in:* Compiler options
> 
> 
> This option sets compiler optimization level (gcc -O argument) for the app.
> 
> 
> * The "Debug" setting will add the -0g flag to CFLAGS.
> * The "Size" setting will add the -0s flag to CFLAGS.
> * The "Performance" setting will add the -O2 flag to CFLAGS.
> * The "None" setting will add the -O0 flag to CFLAGS.
> 
> 
> The "Size" setting cause the compiled code to be smaller and faster, but
> may lead to difficulties of correlating code addresses to source file
> lines when debugging.
> 
> 
> The "Performance" setting causes the compiled code to be larger and faster,
> but will be easier to correlated code addresses to source file lines.
> 
> 
> "None" with -O0 produces compiled code without optimization.
> 
> 
> Note that custom optimization levels may be unsupported.
> 
> 
> Compiler optimization for the IDF bootloader is set separately,
> see the BOOTLOADER_COMPILER_OPTIMIZATION setting.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Debug (-Og) (CONFIG_COMPILER_OPTIMIZATION_DEBUG)
> > * Optimize for size (-Os) (CONFIG_COMPILER_OPTIMIZATION_SIZE)
> > * Optimize for performance (-O2) (CONFIG_COMPILER_OPTIMIZATION_PERF)
> > * Debug without optimization (-O0) (CONFIG_COMPILER_OPTIMIZATION_NONE)




#### CONFIG_COMPILER_OPTIMIZATION_ASSERTION_LEVEL



> Assertion level
> 
> 
> *Found in:* Compiler options
> 
> 
> Assertions can be:
> 
> 
> * Enabled. Failure will print verbose assertion details. This is the default.
> * Set to "silent" to save code size (failed assertions will abort() but user
> needs to use the aborting address to find the line number with the failed assertion.)
> * Disabled entirely (not recommended for most configurations.) -DNDEBUG is added
> to CPPFLAGS in this case.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Enabled (CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_ENABLE)
> > 
> > 
> > Enable assertions. Assertion content and line number will be printed on failure.
> > * Silent (saves code size) (CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_SILENT)
> > 
> > 
> > Enable silent assertions. Failed assertions will abort(), user needs to
> > use the aborting address to find the line number with the failed assertion.
> > * Disabled (sets -DNDEBUG) (CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_DISABLE)
> > 
> > 
> > If assertions are disabled, -DNDEBUG is added to CPPFLAGS.




#### CONFIG_COMPILER_FLOAT_LIB_FROM



> Compiler float lib source
> 
> 
> *Found in:* Compiler options
> 
> 
> In the soft-fp part of libgcc, riscv version is written in C,
> and handles all edge cases in IEEE754, which makes it larger
> and performance is slow.
> 
> 
> RVfplib is an optimized RISC-V library for FP arithmetic on 32-bit
> integer processors, for single and double-precision FP.
> RVfplib is "fast", but it has a few exceptions from IEEE 754 compliance.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * libgcc (CONFIG_COMPILER_FLOAT_LIB_FROM_GCCLIB)
> > * librvfp (CONFIG_COMPILER_FLOAT_LIB_FROM_RVFPLIB)




#### CONFIG_COMPILER_OPTIMIZATION_CHECKS_SILENT



> Disable messages in ESP_RETURN_ON_\* and ESP_EXIT_ON_\* macros
> 
> 
> *Found in:* Compiler options
> 
> 
> If enabled, the error messages will be discarded in following check macros:
> - ESP_RETURN_ON_ERROR
> - ESP_EXIT_ON_ERROR
> - ESP_RETURN_ON_FALSE
> - ESP_EXIT_ON_FALSE
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_COMPILER_HIDE_PATHS_MACROS



> Replace ESP-IDF and project paths in binaries
> 
> 
> *Found in:* Compiler options
> 
> 
> When expanding the __FILE__ and __BASE_FILE__ macros, replace paths inside ESP-IDF
> with paths relative to the placeholder string "IDF", and convert paths inside the
> project directory to relative paths.
> 
> 
> This allows building the project with assertions or other code that embeds file paths,
> without the binary containing the exact path to the IDF or project directories.
> 
> 
> This option passes -fmacro-prefix-map options to the GCC command line. To replace additional
> paths in your binaries, modify the project CMakeLists.txt file to pass custom -fmacro-prefix-map or
> -ffile-prefix-map arguments.
> 
> 
> 
> Default value:* Yes (enabled)




#### CONFIG_COMPILER_CXX_EXCEPTIONS



> Enable C++ exceptions
> 
> 
> *Found in:* Compiler options
> 
> 
> Enabling this option compiles all IDF C++ files with exception support enabled.
> 
> 
> Disabling this option disables C++ exception support in all compiled files, and any libstdc++ code
> which throws an exception will abort instead.
> 
> 
> Enabling this option currently adds an additional ~500 bytes of heap overhead
> when an exception is thrown in user code for the first time.
> 
> 
> 
> Default value:* No (disabled)


Contains:


* CONFIG_COMPILER_CXX_EXCEPTIONS_EMG_POOL_SIZE



##### CONFIG_COMPILER_CXX_EXCEPTIONS_EMG_POOL_SIZE



> Emergency Pool Size
> 
> 
> *Found in:* Compiler options > CONFIG_COMPILER_CXX_EXCEPTIONS
> 
> 
> Size (in bytes) of the emergency memory pool for C++ exceptions. This pool will be used to allocate
> memory for thrown exceptions when there is not enough memory on the heap.
> 
> 
> 
> Default value:* 0 if CONFIG_COMPILER_CXX_EXCEPTIONS





#### CONFIG_COMPILER_CXX_RTTI



> Enable C++ run-time type info (RTTI)
> 
> 
> *Found in:* Compiler options
> 
> 
> Enabling this option compiles all C++ files with RTTI support enabled.
> This increases binary size (typically by tens of kB) but allows using
> dynamic_cast conversion and typeid operator.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_COMPILER_STACK_CHECK_MODE



> Stack smashing protection mode
> 
> 
> *Found in:* Compiler options
> 
> 
> Stack smashing protection mode. Emit extra code to check for buffer overflows, such as stack
> smashing attacks. This is done by adding a guard variable to functions with vulnerable objects.
> The guards are initialized when a function is entered and then checked when the function exits.
> If a guard check fails, program is halted. Protection has the following modes:
> 
> 
> * In NORMAL mode (GCC flag: -fstack-protector) only functions that call alloca, and functions with
> buffers larger than 8 bytes are protected.
> * STRONG mode (GCC flag: -fstack-protector-strong) is like NORMAL, but includes additional functions
> to be protected -- those that have local array definitions, or have references to local frame
> addresses.
> * In OVERALL mode (GCC flag: -fstack-protector-all) all functions are protected.
> 
> 
> Modes have the following impact on code performance and coverage:
> 
> 
> * performance: NORMAL > STRONG > OVERALL
> * coverage: NORMAL < STRONG < OVERALL
> 
> 
> The performance impact includes increasing the amount of stack memory required for each task.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * None (CONFIG_COMPILER_STACK_CHECK_MODE_NONE)
> > * Normal (CONFIG_COMPILER_STACK_CHECK_MODE_NORM)
> > * Strong (CONFIG_COMPILER_STACK_CHECK_MODE_STRONG)
> > * Overall (CONFIG_COMPILER_STACK_CHECK_MODE_ALL)




#### CONFIG_COMPILER_WARN_WRITE_STRINGS



> Enable -Wwrite-strings warning flag
> 
> 
> *Found in:* Compiler options
> 
> 
> Adds -Wwrite-strings flag for the C/C++ compilers.
> 
> 
> For C, this gives string constants the type `const char[]` so that
> copying the address of one into a non-const `char \*` pointer
> produces a warning. This warning helps to find at compile time code
> that tries to write into a string constant.
> 
> 
> For C++, this warns about the deprecated conversion from string
> literals to `char \*`.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_COMPILER_DISABLE_GCC12_WARNINGS



> Disable new warnings introduced in GCC 12
> 
> 
> *Found in:* Compiler options
> 
> 
> Enable this option if use GCC 12 or newer, and want to disable warnings which don't appear with
> GCC 11.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_COMPILER_DISABLE_GCC13_WARNINGS



> Disable new warnings introduced in GCC 13
> 
> 
> *Found in:* Compiler options
> 
> 
> Enable this option if use GCC 13 or newer, and want to disable warnings which don't appear with
> GCC 12.
> 
> 
> 
> Default value:* No (disabled)




#### CONFIG_COMPILER_DUMP_RTL_FILES



> Dump RTL files during compilation
> 
> 
> *Found in:* Compiler options
> 
> 
> If enabled, RTL files will be produced during compilation. These files
> can be used by other tools, for example to calculate call graphs.




#### CONFIG_COMPILER_RT_LIB



> Compiler runtime library
> 
> 
> *Found in:* Compiler options
> 
> 
> Select runtime library to be used by compiler.
> - GCC toolchain supports libgcc only.
> - Clang allows to choose between libgcc or libclang_rt.
> - For host builds ("linux" target), uses the default library.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * libgcc (CONFIG_COMPILER_RT_LIB_GCCLIB)
> > * libclang_rt (CONFIG_COMPILER_RT_LIB_CLANGRT)
> > * Host (CONFIG_COMPILER_RT_LIB_HOST)




#### CONFIG_COMPILER_ORPHAN_SECTIONS



> Orphan sections handling
> 
> 
> *Found in:* Compiler options
> 
> 
> If the linker finds orphan sections, it attempts to place orphan sections after sections of the same
> attribute such as code vs data, loadable vs non-loadable, etc.
> That means that orphan sections could placed between sections defined in IDF linker scripts.
> This could lead to corruption of the binary image. Configure the linker action here.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Place with warning (CONFIG_COMPILER_ORPHAN_SECTIONS_WARNING)
> > 
> > 
> > Places orphan sections without a warning message.
> > * Place silently (CONFIG_COMPILER_ORPHAN_SECTIONS_PLACE)
> > 
> > 
> > Places orphan sections without a warning/error message.





### Component config


Contains:


* ADC and ADC Calibration
* Application Level Tracing
* Bluetooth
* Common ESP-related
* Console Library
* Core dump
* Driver Configurations
* eFuse Bit Manager
* CONFIG_BLE_MESH
* ESP HTTP client
* ESP HTTPS OTA
* ESP HTTPS server
* ESP NETIF Adapter
* ESP PSRAM
* ESP Ringbuf
* ESP System Settings
* ESP Timer (High Resolution Timer)
* ESP-Driver:Analog Comparator Configurations
* ESP-Driver:Camera Controller Configurations
* ESP-Driver:DAC Configurations
* ESP-Driver:GPIO Configurations
* ESP-Driver:GPTimer Configurations
* ESP-Driver:I2C Configurations
* ESP-Driver:I2S Configurations
* ESP-Driver:ISP Configurations
* ESP-Driver:JPEG-Codec Configurations
* ESP-Driver:LEDC Configurations
* ESP-Driver:MCPWM Configurations
* ESP-Driver:Parallel IO Configurations
* ESP-Driver:PCNT Configurations
* ESP-Driver:RMT Configurations
* ESP-Driver:Sigma Delta Modulator Configurations
* ESP-Driver:SPI Configurations
* ESP-Driver:Temperature Sensor Configurations
* ESP-Driver:Touch Sensor Configurations
* ESP-Driver:UART Configurations
* ESP-Driver:USB Serial/JTAG Configuration
* ESP-MQTT Configurations
* ESP-TLS
* Ethernet
* Event Loop Library
* FAT Filesystem support
* FreeRTOS
* GDB Stub
* Hardware Abstraction Layer (HAL) and Low Level (LL)
* Hardware Settings
* Heap memory debugging
* HTTP Server
* IEEE 802.15.4
* IPC (Inter-Processor Call)
* LCD and Touch Panel
* Log output
* LWIP
* Main Flash configuration
* mbedTLS
* Newlib
* NVS
* NVS Security Provider
* OpenThread
* Partition API Configuration
* PHY
* Power Management
* Protocomm
* PThreads
* SoC Settings
* SPI Flash driver
* SPIFFS Configuration
* TCP Transport
* Ultra Low Power (ULP) Co-processor
* Unity unit testing library
* USB-OTG
* Virtual file system
* Wear Levelling
* Wi-Fi
* Wi-Fi Provisioning Manager
* Wireless Coexistence



#### Application Level Tracing


Contains:


* CONFIG_APPTRACE_DESTINATION1
* CONFIG_APPTRACE_DESTINATION2
* FreeRTOS SystemView Tracing
* CONFIG_APPTRACE_GCOV_ENABLE
* CONFIG_APPTRACE_BUF_SIZE
* CONFIG_APPTRACE_PENDING_DATA_SIZE_MAX
* CONFIG_APPTRACE_POSTMORTEM_FLUSH_THRESH
* CONFIG_APPTRACE_ONPANIC_HOST_FLUSH_TMO
* CONFIG_APPTRACE_UART_BAUDRATE
* CONFIG_APPTRACE_UART_RX_GPIO
* CONFIG_APPTRACE_UART_RX_BUFF_SIZE
* CONFIG_APPTRACE_UART_TASK_PRIO
* CONFIG_APPTRACE_UART_TX_MSG_SIZE
* CONFIG_APPTRACE_UART_TX_GPIO
* CONFIG_APPTRACE_UART_TX_BUFF_SIZE



##### CONFIG_APPTRACE_DESTINATION1



> Data Destination 1
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Select destination for application trace: JTAG or none (to disable).
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * JTAG (CONFIG_APPTRACE_DEST_JTAG)
> > * None (CONFIG_APPTRACE_DEST_NONE)




##### CONFIG_APPTRACE_DESTINATION2



> Data Destination 2
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Select destination for application trace: UART(XX) or none (to disable).
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * UART0 (CONFIG_APPTRACE_DEST_UART0)
> > * UART1 (CONFIG_APPTRACE_DEST_UART1)
> > * UART2 (CONFIG_APPTRACE_DEST_UART2)
> > * USB_CDC (CONFIG_APPTRACE_DEST_USB_CDC)
> > * None (CONFIG_APPTRACE_DEST_UART_NONE)




##### CONFIG_APPTRACE_UART_TX_GPIO



> UART TX on GPIO#
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> This GPIO is used for UART TX pin.




##### CONFIG_APPTRACE_UART_RX_GPIO



> UART RX on GPIO#
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> This GPIO is used for UART RX pin.




##### CONFIG_APPTRACE_UART_BAUDRATE



> UART baud rate
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> This baud rate is used for UART.
> 
> 
> The app's maximum baud rate depends on the UART clock source. If Power Management is disabled,
> the UART clock source is the APB clock and all baud rates in the available range will be sufficiently
> accurate. If Power Management is enabled, REF_TICK clock source is used so the baud rate is divided
> from 1MHz. Baud rates above 1Mbps are not possible and values between 500Kbps and 1Mbps may not be
> accurate.




##### CONFIG_APPTRACE_UART_RX_BUFF_SIZE



> UART RX ring buffer size
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Size of the UART input ring buffer.
> This size related to the baudrate, system tick frequency and amount of data to transfer.
> The data placed to this buffer before sent out to the interface.




##### CONFIG_APPTRACE_UART_TX_BUFF_SIZE



> UART TX ring buffer size
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Size of the UART output ring buffer.
> This size related to the baudrate, system tick frequency and amount of data to transfer.




##### CONFIG_APPTRACE_UART_TX_MSG_SIZE



> UART TX message size
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Maximum size of the single message to transfer.




##### CONFIG_APPTRACE_UART_TASK_PRIO



> UART Task Priority
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> UART task priority. In case of high events rate,
> this parameter could be changed up to (configMAX_PRIORITIES-1).
> 
> 
> 
> Range:* from 1 to 32
> 
> 
> 
> Default value:* 1




##### CONFIG_APPTRACE_ONPANIC_HOST_FLUSH_TMO



> Timeout for flushing last trace data to host on panic
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Timeout for flushing last trace data to host in case of panic. In ms.
> Use -1 to disable timeout and wait forever.




##### CONFIG_APPTRACE_POSTMORTEM_FLUSH_THRESH



> Threshold for flushing last trace data to host on panic
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Threshold for flushing last trace data to host on panic in post-mortem mode.
> This is minimal amount of data needed to perform flush. In bytes.




##### CONFIG_APPTRACE_BUF_SIZE



> Size of the apptrace buffer
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Size of the memory buffer for trace data in bytes.




##### CONFIG_APPTRACE_PENDING_DATA_SIZE_MAX



> Size of the pending data buffer
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Size of the buffer for events in bytes. It is useful for buffering events from
> the time critical code (scheduler, ISRs etc). If this parameter is 0 then
> events will be discarded when main HW buffer is full.




##### FreeRTOS SystemView Tracing


Contains:


* CONFIG_APPTRACE_SV_CPU
* CONFIG_APPTRACE_SV_EVT_ISR_ENTER_ENABLE
* CONFIG_APPTRACE_SV_EVT_ISR_EXIT_ENABLE
* CONFIG_APPTRACE_SV_EVT_ISR_TO_SCHED_ENABLE
* CONFIG_APPTRACE_SV_MAX_TASKS
* CONFIG_APPTRACE_SV_EVT_IDLE_ENABLE
* CONFIG_APPTRACE_SV_ENABLE
* CONFIG_APPTRACE_SV_EVT_TASK_CREATE_ENABLE
* CONFIG_APPTRACE_SV_EVT_TASK_START_EXEC_ENABLE
* CONFIG_APPTRACE_SV_EVT_TASK_START_READY_ENABLE
* CONFIG_APPTRACE_SV_EVT_TASK_STOP_EXEC_ENABLE
* CONFIG_APPTRACE_SV_EVT_TASK_STOP_READY_ENABLE
* CONFIG_APPTRACE_SV_EVT_TASK_TERMINATE_ENABLE
* CONFIG_APPTRACE_SV_EVT_TIMER_ENTER_ENABLE
* CONFIG_APPTRACE_SV_EVT_TIMER_EXIT_ENABLE
* CONFIG_APPTRACE_SV_TS_SOURCE
* CONFIG_APPTRACE_SV_EVT_OVERFLOW_ENABLE
* CONFIG_APPTRACE_SV_BUF_WAIT_TMO



###### CONFIG_APPTRACE_SV_ENABLE



> SystemView Tracing Enable
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables supporrt for SEGGER SystemView tracing functionality.




###### CONFIG_APPTRACE_SV_DEST



> SystemView destination
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing > CONFIG_APPTRACE_SV_ENABLE
> 
> 
> SystemView witt transfer data trough defined interface.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Data destination JTAG (CONFIG_APPTRACE_SV_DEST_JTAG)
> > 
> > 
> > Send SEGGER SystemView events through JTAG interface.
> > * Data destination UART (CONFIG_APPTRACE_SV_DEST_UART)
> > 
> > 
> > Send SEGGER SystemView events through UART interface.




###### CONFIG_APPTRACE_SV_CPU



> CPU to trace
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Define the CPU to trace by SystemView.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CPU0 (CONFIG_APPTRACE_SV_DEST_CPU_0)
> > 
> > 
> > Send SEGGER SystemView events for Pro CPU.
> > * CPU1 (CONFIG_APPTRACE_SV_DEST_CPU_1)
> > 
> > 
> > Send SEGGER SystemView events for App CPU.




###### CONFIG_APPTRACE_SV_TS_SOURCE



> Timer to use as timestamp source
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> SystemView needs to use a hardware timer as the source of timestamps
> when tracing. This option selects the timer for it.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CPU cycle counter (CCOUNT) (CONFIG_APPTRACE_SV_TS_SOURCE_CCOUNT)
> > * General Purpose Timer (Timer Group) (CONFIG_APPTRACE_SV_TS_SOURCE_GPTIMER)
> > * esp_timer high resolution timer (CONFIG_APPTRACE_SV_TS_SOURCE_ESP_TIMER)




###### CONFIG_APPTRACE_SV_MAX_TASKS



> Maximum supported tasks
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Configures maximum supported tasks in sysview debug




###### CONFIG_APPTRACE_SV_BUF_WAIT_TMO



> Trace buffer wait timeout
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Configures timeout (in us) to wait for free space in trace buffer.
> Set to -1 to wait forever and avoid lost events.




###### CONFIG_APPTRACE_SV_EVT_OVERFLOW_ENABLE



> Trace Buffer Overflow Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Trace Buffer Overflow" event.




###### CONFIG_APPTRACE_SV_EVT_ISR_ENTER_ENABLE



> ISR Enter Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "ISR Enter" event.




###### CONFIG_APPTRACE_SV_EVT_ISR_EXIT_ENABLE



> ISR Exit Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "ISR Exit" event.




###### CONFIG_APPTRACE_SV_EVT_ISR_TO_SCHED_ENABLE



> ISR Exit to Scheduler Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "ISR to Scheduler" event.




###### CONFIG_APPTRACE_SV_EVT_TASK_START_EXEC_ENABLE



> Task Start Execution Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Task Start Execution" event.




###### CONFIG_APPTRACE_SV_EVT_TASK_STOP_EXEC_ENABLE



> Task Stop Execution Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Task Stop Execution" event.




###### CONFIG_APPTRACE_SV_EVT_TASK_START_READY_ENABLE



> Task Start Ready State Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Task Start Ready State" event.




###### CONFIG_APPTRACE_SV_EVT_TASK_STOP_READY_ENABLE



> Task Stop Ready State Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Task Stop Ready State" event.




###### CONFIG_APPTRACE_SV_EVT_TASK_CREATE_ENABLE



> Task Create Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Task Create" event.




###### CONFIG_APPTRACE_SV_EVT_TASK_TERMINATE_ENABLE



> Task Terminate Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Task Terminate" event.




###### CONFIG_APPTRACE_SV_EVT_IDLE_ENABLE



> System Idle Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "System Idle" event.




###### CONFIG_APPTRACE_SV_EVT_TIMER_ENTER_ENABLE



> Timer Enter Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Timer Enter" event.




###### CONFIG_APPTRACE_SV_EVT_TIMER_EXIT_ENABLE



> Timer Exit Event
> 
> 
> *Found in:* Component config > Application Level Tracing > FreeRTOS SystemView Tracing
> 
> 
> Enables "Timer Exit" event.





##### CONFIG_APPTRACE_GCOV_ENABLE



> GCOV to Host Enable
> 
> 
> *Found in:* Component config > Application Level Tracing
> 
> 
> Enables support for GCOV data transfer to host.



###### CONFIG_APPTRACE_GCOV_DUMP_TASK_STACK_SIZE



> Gcov dump task stack size
> 
> 
> *Found in:* Component config > Application Level Tracing > CONFIG_APPTRACE_GCOV_ENABLE
> 
> 
> Configures stack size of Gcov dump task
> 
> 
> 
> Default value:* 2048 if CONFIG_APPTRACE_GCOV_ENABLE






#### Bluetooth


Contains:


* Bluedroid Options
* CONFIG_BT_ENABLED
* Common Options
* Controller Options
* CONFIG_BT_HCI_LOG_DEBUG_EN
* NimBLE Options
* CONFIG_BT_RELEASE_IRAM



##### CONFIG_BT_ENABLED



> Bluetooth
> 
> 
> *Found in:* Component config > Bluetooth
> 
> 
> Select this option to enable Bluetooth and show the submenu with Bluetooth configuration choices.



###### CONFIG_BT_HOST



> Host
> 
> 
> *Found in:* Component config > Bluetooth > CONFIG_BT_ENABLED
> 
> 
> This helps to choose Bluetooth host stack
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Bluedroid - Dual-mode (CONFIG_BT_BLUEDROID_ENABLED)
> > 
> > 
> > This option is recommended for classic Bluetooth or for dual-mode
> > usecases
> > * NimBLE - BLE only (CONFIG_BT_NIMBLE_ENABLED)
> > 
> > 
> > This option is recommended for BLE only usecases to save on memory
> > * Disabled (CONFIG_BT_CONTROLLER_ONLY)
> > 
> > 
> > This option is recommended when you want to communicate directly with the
> > controller (without any host) or when you are using any other host stack
> > not supported by Espressif (not mentioned here).




###### CONFIG_BT_CONTROLLER



> Controller
> 
> 
> *Found in:* Component config > Bluetooth > CONFIG_BT_ENABLED
> 
> 
> This helps to choose Bluetooth controller stack
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Enabled (CONFIG_BT_CONTROLLER_ENABLED)
> > 
> > 
> > This option is recommended for Bluetooth controller usecases
> > * Disabled (CONFIG_BT_CONTROLLER_DISABLED)
> > 
> > 
> > This option is recommended for Bluetooth Host only usecases





##### Bluedroid Options


Contains:


* CONFIG_BT_BLE_HOST_QUEUE_CONG_CHECK
* CONFIG_BT_BLUEDROID_MEM_DEBUG
* CONFIG_BT_BTU_TASK_STACK_SIZE
* CONFIG_BT_BTC_TASK_STACK_SIZE
* CONFIG_BT_BLE_ENABLED
* BT DEBUG LOG LEVEL
* CONFIG_BT_ACL_CONNECTIONS
* CONFIG_BT_SMP_MAX_BONDS
* CONFIG_BT_ALLOCATION_FROM_SPIRAM_FIRST
* CONFIG_BT_CLASSIC_ENABLED
* CONFIG_BT_HID_ENABLED
* CONFIG_BT_STACK_NO_LOG
* CONFIG_BT_BLE_42_FEATURES_SUPPORTED
* CONFIG_BT_BLE_50_FEATURES_SUPPORTED
* CONFIG_BT_BLE_HIGH_DUTY_ADV_INTERVAL
* CONFIG_BT_MULTI_CONNECTION_ENBALE
* CONFIG_BT_BLE_FEAT_PERIODIC_ADV_SYNC_TRANSFER
* CONFIG_BT_BLE_FEAT_CREATE_SYNC_ENH
* CONFIG_BT_BLUEDROID_ESP_COEX_VSC
* CONFIG_BT_BLE_FEAT_PERIODIC_ADV_ENH
* CONFIG_BT_MAX_DEVICE_NAME_LEN
* CONFIG_BT_BLE_ACT_SCAN_REP_ADV_SCAN
* CONFIG_BT_BLUEDROID_PINNED_TO_CORE_CHOICE
* CONFIG_BT_BLE_ESTAB_LINK_CONN_TOUT
* CONFIG_BT_BLE_RPA_TIMEOUT
* CONFIG_BT_BLE_RPA_SUPPORTED
* CONFIG_BT_BLE_DYNAMIC_ENV_MEMORY
* CONFIG_BT_HFP_WBS_ENABLE



###### CONFIG_BT_BTC_TASK_STACK_SIZE



> Bluetooth event (callback to application) task stack size
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This select btc task stack size
> 
> 
> 
> Default value:* 3072 if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLUEDROID_PINNED_TO_CORE_CHOICE



> The cpu core which Bluedroid run
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Which the cpu core to run Bluedroid. Can choose core0 and core1.
> Can not specify no-affinity.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Core 0 (PRO CPU) (CONFIG_BT_BLUEDROID_PINNED_TO_CORE_0)
> > * Core 1 (APP CPU) (CONFIG_BT_BLUEDROID_PINNED_TO_CORE_1)




###### CONFIG_BT_BTU_TASK_STACK_SIZE



> Bluetooth Bluedroid Host Stack task stack size
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This select btu task stack size
> 
> 
> 
> Default value:* 4352 if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLUEDROID_MEM_DEBUG



> Bluedroid memory debug
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Bluedroid memory debug
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLUEDROID_ESP_COEX_VSC



> Enable Espressif Vendor-specific HCI commands for coexist status configuration
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Enable Espressif Vendor-specific HCI commands for coexist status configuration
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_CLASSIC_ENABLED



> Classic Bluetooth
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> For now this option needs "SMP_ENABLE" to be set to yes
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && ((CONFIG_BT_CONTROLLER_ENABLED && SOC_BT_CLASSIC_SUPPORTED) || CONFIG_BT_CONTROLLER_DISABLED) && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_ENC_KEY_SIZE_CTRL_ENABLED



> configure encryption key size
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED
> 
> 
> This chooses the support status of configuring encryption key size
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Supported by standard HCI command (CONFIG_BT_ENC_KEY_SIZE_CTRL_STD)
> > * Supported by Vendor-specific HCI command (CONFIG_BT_ENC_KEY_SIZE_CTRL_VSC)
> > * Not supported (CONFIG_BT_ENC_KEY_SIZE_CTRL_NONE)




###### CONFIG_BT_CLASSIC_BQB_ENABLED



> Host Qualitifcation support for Classic Bluetooth
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED
> 
> 
> This enables functionalities of Host qualification for Classic Bluetooth.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CLASSIC_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_A2DP_ENABLE



> A2DP
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED
> 
> 
> Advanced Audio Distribution Profile
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CLASSIC_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_SPP_ENABLED



> SPP
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED
> 
> 
> This enables the Serial Port Profile
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CLASSIC_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_L2CAP_ENABLED



> BT L2CAP
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED
> 
> 
> This enables the Logical Link Control and Adaptation Layer Protocol.
> Only supported classic bluetooth.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CLASSIC_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_HFP_ENABLE



> Hands Free/Handset Profile
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED
> 
> 
> Hands Free Unit and Audio Gateway can be included simultaneously
> but they cannot run simultaneously due to internal limitations.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CLASSIC_ENABLED && CONFIG_BT_BLUEDROID_ENABLED


Contains:


* CONFIG_BT_HFP_AG_ENABLE
* CONFIG_BT_HFP_AUDIO_DATA_PATH
* CONFIG_BT_HFP_CLIENT_ENABLE




###### CONFIG_BT_HFP_CLIENT_ENABLE



> Hands Free Unit
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED > CONFIG_BT_HFP_ENABLE
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_HFP_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_HFP_AG_ENABLE



> Audio Gateway
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED > CONFIG_BT_HFP_ENABLE
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_HFP_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_HFP_AUDIO_DATA_PATH



> audio(SCO) data path
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_CLASSIC_ENABLED > CONFIG_BT_HFP_ENABLE
> 
> 
> SCO data path, i.e. HCI or PCM. This option is set using API
> "esp_bredr_sco_datapath_set" in Bluetooth host. Default SCO data
> path can also be set in Bluetooth Controller.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * PCM (CONFIG_BT_HFP_AUDIO_DATA_PATH_PCM)
> > * HCI (CONFIG_BT_HFP_AUDIO_DATA_PATH_HCI)




###### CONFIG_BT_HFP_WBS_ENABLE



> Wide Band Speech
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This enables Wide Band Speech. Should disable it when SCO data path is PCM.
> Otherwise there will be no data transmitted via GPIOs.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_HFP_AUDIO_DATA_PATH_HCI && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_HID_ENABLED



> Classic BT HID
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This enables the BT HID Host
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CLASSIC_ENABLED && CONFIG_BT_BLUEDROID_ENABLED


Contains:


* CONFIG_BT_HID_DEVICE_ENABLED
* CONFIG_BT_HID_HOST_ENABLED




###### CONFIG_BT_HID_HOST_ENABLED



> Classic BT HID Host
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_HID_ENABLED
> 
> 
> This enables the BT HID Host
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_HID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_HID_DEVICE_ENABLED



> Classic BT HID Device
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_HID_ENABLED
> 
> 
> This enables the BT HID Device




###### CONFIG_BT_BLE_ENABLED



> Bluetooth Low Energy
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This enables Bluetooth Low Energy
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTS_ENABLE



> Include GATT server module(GATTS)
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED
> 
> 
> This option can be disabled when the app work only on gatt client mode
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTS_PPCP_CHAR_GAP



> Enable Peripheral Preferred Connection Parameters characteristic in GAP service
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> This enables "Peripheral Preferred Connection Parameters" characteristic (UUID: 0x2A04) in GAP service that has
> connection parameters like min/max connection interval, slave latency and supervision timeout multiplier
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_BLUFI_ENABLE



> Include blufi function
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> This option can be close when the app does not require blufi function.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATT_MAX_SR_PROFILES



> Max GATT Server Profiles
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> Maximum GATT Server Profiles Count
> 
> 
> 
> Range:* from 1 to 32 if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 8 if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATT_MAX_SR_ATTRIBUTES



> Max GATT Service Attributes
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> Maximum GATT Service Attributes Count
> 
> 
> 
> Range:* from 1 to 500 if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 100 if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTS_SEND_SERVICE_CHANGE_MODE



> GATTS Service Change Mode
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> Service change indication mode for GATT Server.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * GATTS manually send service change indication (CONFIG_BT_GATTS_SEND_SERVICE_CHANGE_MANUAL)
> > 
> > 
> > Manually send service change indication through API esp_ble_gatts_send_service_change_indication()
> > * GATTS automatically send service change indication (CONFIG_BT_GATTS_SEND_SERVICE_CHANGE_AUTO)
> > 
> > 
> > Let Bluedroid handle the service change indication internally




###### CONFIG_BT_GATTS_ROBUST_CACHING_ENABLED



> Enable Robust Caching on Server Side
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> This option enables the GATT robust caching feature on the server.
> if turned on, the Client Supported Features characteristic, Database Hash characteristic,
> and Server Supported Features characteristic will be included in the GAP SERVICE.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTS_DEVICE_NAME_WRITABLE



> Allow to write device name by GATT clients
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> Enabling this option allows remote GATT clients to write device name
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTS_APPEARANCE_WRITABLE



> Allow to write appearance by GATT clients
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTS_ENABLE
> 
> 
> Enabling this option allows remote GATT clients to write appearance
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_GATTS_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTC_ENABLE



> Include GATT client module(GATTC)
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED
> 
> 
> This option can be close when the app work only on gatt server mode
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTC_MAX_CACHE_CHAR



> Max gattc cache characteristic for discover
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTC_ENABLE
> 
> 
> Maximum GATTC cache characteristic count
> 
> 
> 
> Range:* from 1 to 500 if CONFIG_BT_GATTC_ENABLE && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 40 if CONFIG_BT_GATTC_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTC_NOTIF_REG_MAX



> Max gattc notify(indication) register number
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTC_ENABLE
> 
> 
> Maximum GATTC notify(indication) register number
> 
> 
> 
> Range:* from 1 to 64 if CONFIG_BT_GATTC_ENABLE && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 5 if CONFIG_BT_GATTC_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTC_CACHE_NVS_FLASH



> Save gattc cache data to nvs flash
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTC_ENABLE
> 
> 
> This select can save gattc cache data to nvs flash
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_GATTC_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_GATTC_CONNECT_RETRY_COUNT



> The number of attempts to reconnect if the connection establishment failed
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_GATTC_ENABLE
> 
> 
> The number of attempts to reconnect if the connection establishment failed
> 
> 
> 
> Range:* from 0 to 255 if CONFIG_BT_GATTC_ENABLE && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 3 if CONFIG_BT_GATTC_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_SMP_ENABLE



> Include BLE security module(SMP)
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED
> 
> 
> This option can be close when the app not used the ble security connect.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_SMP_SLAVE_CON_PARAMS_UPD_ENABLE



> Slave enable connection parameters update during pairing
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_BLE_SMP_ENABLE
> 
> 
> In order to reduce the pairing time, slave actively initiates connection parameters
> update during pairing.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLE_SMP_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_SMP_ID_RESET_ENABLE



> Reset device identity when all bonding records are deleted
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > CONFIG_BT_BLE_ENABLED > CONFIG_BT_BLE_SMP_ENABLE
> 
> 
> There are tracking risks associated with using a fixed or static IRK.
> If enabled this option, Bluedroid will assign a new randomly-generated IRK
> when all pairing and bonding records are deleted. This would decrease the ability
> of a previously paired peer to be used to determine whether a device
> with which it previously shared an IRK is within range.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLE_SMP_ENABLE && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_STACK_NO_LOG



> Disable BT debug logs (minimize bin size)
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This select can save the rodata code size
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### BT DEBUG LOG LEVEL


Contains:


* CONFIG_BT_LOG_A2D_TRACE_LEVEL
* CONFIG_BT_LOG_APPL_TRACE_LEVEL
* CONFIG_BT_LOG_AVCT_TRACE_LEVEL
* CONFIG_BT_LOG_AVDT_TRACE_LEVEL
* CONFIG_BT_LOG_AVRC_TRACE_LEVEL
* CONFIG_BT_LOG_BLUFI_TRACE_LEVEL
* CONFIG_BT_LOG_BNEP_TRACE_LEVEL
* CONFIG_BT_LOG_BTC_TRACE_LEVEL
* CONFIG_BT_LOG_BTIF_TRACE_LEVEL
* CONFIG_BT_LOG_BTM_TRACE_LEVEL
* CONFIG_BT_LOG_GAP_TRACE_LEVEL
* CONFIG_BT_LOG_GATT_TRACE_LEVEL
* CONFIG_BT_LOG_HCI_TRACE_LEVEL
* CONFIG_BT_LOG_HID_TRACE_LEVEL
* CONFIG_BT_LOG_L2CAP_TRACE_LEVEL
* CONFIG_BT_LOG_MCA_TRACE_LEVEL
* CONFIG_BT_LOG_OSI_TRACE_LEVEL
* CONFIG_BT_LOG_PAN_TRACE_LEVEL
* CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL
* CONFIG_BT_LOG_SDP_TRACE_LEVEL
* CONFIG_BT_LOG_SMP_TRACE_LEVEL




###### CONFIG_BT_LOG_HCI_TRACE_LEVEL



> HCI layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for HCI layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_HCI_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_HCI_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_HCI_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_HCI_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_HCI_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_HCI_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_HCI_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_BTM_TRACE_LEVEL



> BTM layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for BTM layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_BTM_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_BTM_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_BTM_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_BTM_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_BTM_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_BTM_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_BTM_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_L2CAP_TRACE_LEVEL



> L2CAP layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for L2CAP layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL



> RFCOMM layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for RFCOMM layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_SDP_TRACE_LEVEL



> SDP layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for SDP layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_SDP_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_SDP_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_SDP_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_SDP_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_SDP_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_SDP_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_SDP_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_GAP_TRACE_LEVEL



> GAP layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for GAP layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_GAP_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_GAP_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_GAP_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_GAP_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_GAP_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_GAP_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_GAP_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_BNEP_TRACE_LEVEL



> BNEP layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for BNEP layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_BNEP_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_BNEP_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_BNEP_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_BNEP_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_BNEP_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_BNEP_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_BNEP_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_PAN_TRACE_LEVEL



> PAN layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for PAN layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_PAN_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_PAN_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_PAN_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_PAN_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_PAN_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_PAN_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_PAN_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_A2D_TRACE_LEVEL



> A2D layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for A2D layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_A2D_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_A2D_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_A2D_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_A2D_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_A2D_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_A2D_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_A2D_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_AVDT_TRACE_LEVEL



> AVDT layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for AVDT layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_AVDT_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_AVDT_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_AVDT_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_AVDT_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_AVDT_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_AVDT_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_AVDT_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_AVCT_TRACE_LEVEL



> AVCT layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for AVCT layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_AVCT_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_AVCT_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_AVCT_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_AVCT_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_AVCT_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_AVCT_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_AVCT_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_AVRC_TRACE_LEVEL



> AVRC layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for AVRC layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_AVRC_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_AVRC_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_AVRC_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_AVRC_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_AVRC_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_AVRC_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_AVRC_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_MCA_TRACE_LEVEL



> MCA layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for MCA layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_MCA_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_MCA_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_MCA_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_MCA_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_MCA_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_MCA_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_MCA_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_HID_TRACE_LEVEL



> HID layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for HID layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_HID_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_HID_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_HID_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_HID_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_HID_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_HID_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_HID_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_APPL_TRACE_LEVEL



> APPL layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for APPL layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_APPL_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_APPL_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_APPL_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_APPL_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_APPL_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_APPL_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_APPL_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_GATT_TRACE_LEVEL



> GATT layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for GATT layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_GATT_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_GATT_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_GATT_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_GATT_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_GATT_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_GATT_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_GATT_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_SMP_TRACE_LEVEL



> SMP layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for SMP layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_SMP_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_SMP_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_SMP_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_SMP_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_SMP_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_SMP_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_SMP_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_BTIF_TRACE_LEVEL



> BTIF layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for BTIF layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_BTIF_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_BTIF_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_BTIF_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_BTIF_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_BTIF_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_BTIF_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_BTIF_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_BTC_TRACE_LEVEL



> BTC layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for BTC layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_BTC_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_BTC_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_BTC_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_BTC_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_BTC_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_BTC_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_BTC_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_OSI_TRACE_LEVEL



> OSI layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for OSI layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_OSI_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_OSI_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_OSI_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_OSI_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_OSI_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_OSI_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_OSI_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_LOG_BLUFI_TRACE_LEVEL



> BLUFI layer
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL
> 
> 
> Define BT trace level for BLUFI layer
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL_WARNING)
> > * API (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL_API)
> > * EVENT (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL_EVENT)
> > * DEBUG (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL_VERBOSE)




###### CONFIG_BT_ACL_CONNECTIONS



> BT/BLE MAX ACL CONNECTIONS(1~9)
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Maximum BT/BLE connection count. The ESP32-C3/S3 chip supports a maximum of 10 instances,
> including ADV, SCAN and connections. The ESP32-C3/S3 chip can connect up to 9 devices if
> ADV or SCAN uses only one. If ADV and SCAN are both used, The ESP32-C3/S3 chip is connected
> to a maximum of 8 devices. Because Bluetooth cannot reclaim used instances once ADV or SCAN
> is used.
> 
> 
> 
> Range:* from 1 to 9 if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 4 if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_MULTI_CONNECTION_ENBALE



> Enable BLE multi-conections
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Enable this option if there are multiple connections
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_ALLOCATION_FROM_SPIRAM_FIRST



> BT/BLE will first malloc the memory from the PSRAM
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This select can save the internal RAM if there have the PSRAM
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_DYNAMIC_ENV_MEMORY



> Use dynamic memory allocation in BT/BLE stack
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This select can make the allocation of memory will become more flexible
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_HOST_QUEUE_CONG_CHECK



> BLE queue congestion check
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> When scanning and scan duplicate is not enabled, if there are a lot of adv packets around
> or application layer handling adv packets is slow, it will cause the controller memory
> to run out. if enabled, adv packets will be lost when host queue is congested.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_SMP_MAX_BONDS



> BT/BLE maximum bond device count
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> The number of security records for peer devices.




###### CONFIG_BT_BLE_ACT_SCAN_REP_ADV_SCAN



> Report adv data and scan response individually when BLE active scan
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Originally, when doing BLE active scan, Bluedroid will not report adv to application layer
> until receive scan response. This option is used to disable the behavior. When enable this option,
> Bluedroid will report adv data or scan response to application layer immediately.
> 
> 
> # Memory reserved at start of DRAM for Bluetooth stack
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_ESTAB_LINK_CONN_TOUT



> Timeout of BLE connection establishment
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Bluetooth Connection establishment maximum time, if connection time exceeds this value, the connection
> establishment fails, ESP_GATTC_OPEN_EVT or ESP_GATTS_OPEN_EVT is triggered.
> 
> 
> 
> Range:* from 1 to 60 if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 30 if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_MAX_DEVICE_NAME_LEN



> length of bluetooth device name
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Bluetooth Device name length shall be no larger than 248 octets, If the broadcast data cannot contain
> the complete device name, then only the shortname will be displayed, the rest parts that can't fit in
> will be truncated.
> 
> 
> 
> Range:* from 32 to 248 if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 32 if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_RPA_SUPPORTED



> Update RPA to Controller
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This enables controller RPA list function.
> For ESP32, ESP32 only support network privacy mode. If this option is enabled, ESP32 will only accept
> advertising packets from peer devices that contain private address, HW will not receive the advertising
> packets contain identity address after IRK changed. If this option is disabled, address resolution will
> be performed in the host, so the functions that require controller to resolve address in the white list
> cannot be used. This option is disabled by default on ESP32, please enable or disable this option according
> to your own needs.
> 
> 
> For other BLE chips, devices support network privacy mode and device privacy mode,
> users can switch the two modes according to their own needs. So this option is enabled by default.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_CONTROLLER_DISABLED && CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_CONTROLLER_DISABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_RPA_TIMEOUT



> Timeout of resolvable private address
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This set RPA timeout of Controller and Host.
> Default is 900 s (15 minutes). Range is 1 s to 1 hour (3600 s).
> 
> 
> 
> Range:* from 1 to 3600 if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED
> 
> 
> 
> Default value:* 900 if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_50_FEATURES_SUPPORTED



> Enable BLE 5.0 features
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Enabling this option activates BLE 5.0 features.
> This option is universally supported in chips that support BLE, except for ESP32.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_BLE_ENABLED && (CONFIG_BT_CONTROLLER_ENABLED || CONFIG_BT_CONTROLLER_DISABLED) && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_42_FEATURES_SUPPORTED



> Enable BLE 4.2 features
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This enables BLE 4.2 features.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLE_ENABLED && (CONFIG_BT_CONTROLLER_ENABLED || CONFIG_BT_CONTROLLER_DISABLED) && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_FEAT_PERIODIC_ADV_SYNC_TRANSFER



> Enable BLE periodic advertising sync transfer feature
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This enables BLE periodic advertising sync transfer feature
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLE_50_FEATURES_SUPPORTED && ((CONFIG_BT_CONTROLLER_ENABLED && SOC_ESP_NIMBLE_CONTROLLER) || CONFIG_BT_CONTROLLER_DISABLED) && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_FEAT_PERIODIC_ADV_ENH



> Enable periodic adv enhancements(adi support)
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Enable the periodic advertising enhancements
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLE_50_FEATURES_SUPPORTED && ((CONFIG_BT_CONTROLLER_ENABLED && SOC_ESP_NIMBLE_CONTROLLER) || CONFIG_BT_CONTROLLER_DISABLED) && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_FEAT_CREATE_SYNC_ENH



> Enable create sync enhancements(reporting disable and duplicate filtering enable support)
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> Enable the create sync enhancements
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED && CONFIG_BT_BLE_50_FEATURES_SUPPORTED && ((CONFIG_BT_CONTROLLER_ENABLED && SOC_ESP_NIMBLE_CONTROLLER) || CONFIG_BT_CONTROLLER_DISABLED) && CONFIG_BT_BLUEDROID_ENABLED




###### CONFIG_BT_BLE_HIGH_DUTY_ADV_INTERVAL



> Enable BLE high duty advertising interval feature
> 
> 
> *Found in:* Component config > Bluetooth > Bluedroid Options
> 
> 
> This enable BLE high duty advertising interval feature
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLE_ENABLED && CONFIG_BT_BLUEDROID_ENABLED





##### NimBLE Options


Contains:


* CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME
* CONFIG_BT_NIMBLE_HS_STOP_TIMEOUT_MS
* CONFIG_BT_NIMBLE_HOST_QUEUE_CONG_CHECK
* BLE Services
* CONFIG_BT_NIMBLE_WHITELIST_SIZE
* CONFIG_BT_NIMBLE_BLE_GATT_BLOB_TRANSFER
* CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
* CONFIG_BT_NIMBLE_ROLE_BROADCASTER
* CONFIG_BT_NIMBLE_ROLE_CENTRAL
* CONFIG_BT_NIMBLE_HIGH_DUTY_ADV_ITVL
* CONFIG_BT_NIMBLE_MESH
* CONFIG_BT_NIMBLE_ROLE_OBSERVER
* CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
* CONFIG_BT_NIMBLE_SECURITY_ENABLE
* CONFIG_BT_NIMBLE_BLUFI_ENABLE
* CONFIG_BT_NIMBLE_ENABLE_CONN_REATTEMPT
* CONFIG_BT_NIMBLE_DYNAMIC_SERVICE
* CONFIG_BT_NIMBLE_USE_ESP_TIMER
* CONFIG_BT_NIMBLE_DEBUG
* CONFIG_BT_NIMBLE_HS_FLOW_CTRL
* CONFIG_BT_NIMBLE_VS_SUPPORT
* CONFIG_BT_NIMBLE_OPTIMIZE_MULTI_CONN
* CONFIG_BT_NIMBLE_ENC_ADV_DATA
* CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE
* GAP Service
* Host-controller Transport
* CONFIG_BT_NIMBLE_GAP_DEVICE_NAME_MAX_LEN
* CONFIG_BT_NIMBLE_MAX_BONDS
* CONFIG_BT_NIMBLE_MAX_CCCDS
* CONFIG_BT_NIMBLE_MAX_CONNECTIONS
* CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM
* CONFIG_BT_NIMBLE_GATT_MAX_PROCS
* CONFIG_BT_NIMBLE_MEM_ALLOC_MODE
* Memory Settings
* CONFIG_BT_NIMBLE_LOG_LEVEL
* CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE
* CONFIG_BT_NIMBLE_CRYPTO_STACK_MBEDTLS
* CONFIG_BT_NIMBLE_NVS_PERSIST
* CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU
* CONFIG_BT_NIMBLE_SMP_ID_RESET
* CONFIG_BT_NIMBLE_RPA_TIMEOUT
* CONFIG_BT_NIMBLE_PINNED_TO_CORE_CHOICE
* CONFIG_BT_NIMBLE_TEST_THROUGHPUT_TEST



###### CONFIG_BT_NIMBLE_MEM_ALLOC_MODE



> Memory allocation strategy
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Allocation strategy for NimBLE host stack, essentially provides ability to
> allocate all required dynamic allocations from,
> 
> 
> * Internal DRAM memory only
> * External SPIRAM memory only
> * Either internal or external memory based on default malloc()
> behavior in ESP-IDF
> * Internal IRAM memory wherever applicable else internal DRAM
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Internal memory (CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL)
> > * External SPIRAM (CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL)
> > * Default alloc mode (CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_DEFAULT)
> > * Internal IRAM (CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_IRAM_8BIT)
> > 
> > 
> > Allows to use IRAM memory region as 8bit accessible region.
> > 
> > 
> > Every unaligned (8bit or 16bit) access will result in an exception
> > and incur penalty of certain clock cycles per unaligned read/write.




###### CONFIG_BT_NIMBLE_LOG_LEVEL



> NimBLE Host log verbosity
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Select NimBLE log level. Please make a note that the selected NimBLE log
> verbosity can not exceed the level set in "Component config --> Log output
> --> Default log verbosity".
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No logs (CONFIG_BT_NIMBLE_LOG_LEVEL_NONE)
> > * Error logs (CONFIG_BT_NIMBLE_LOG_LEVEL_ERROR)
> > * Warning logs (CONFIG_BT_NIMBLE_LOG_LEVEL_WARNING)
> > * Info logs (CONFIG_BT_NIMBLE_LOG_LEVEL_INFO)
> > * Debug logs (CONFIG_BT_NIMBLE_LOG_LEVEL_DEBUG)




###### CONFIG_BT_NIMBLE_MAX_CONNECTIONS



> Maximum number of concurrent connections
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Defines maximum number of concurrent BLE connections. For ESP32, user
> is expected to configure BTDM_CTRL_BLE_MAX_CONN from controller menu
> along with this option. Similarly for ESP32-C3 or ESP32-S3, user is expected to
> configure BT_CTRL_BLE_MAX_ACT from controller menu.
> For ESP32C2, ESP32C6 and ESP32H2, each connection will take about 1k DRAM.
> 
> 
> 
> Range:* from 1 to 9 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 3 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MAX_BONDS



> Maximum number of bonds to save across reboots
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Defines maximum number of bonds to save for peer security and our security
> 
> 
> 
> Default value:* 3 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MAX_CCCDS



> Maximum number of CCC descriptors to save across reboots
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Defines maximum number of CCC descriptors to save
> 
> 
> 
> Default value:* 8 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM



> Maximum number of connection oriented channels
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Defines maximum number of BLE Connection Oriented Channels. When set to (0), BLE COC is not compiled in
> 
> 
> 
> Range:* from 0 to 9 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_PINNED_TO_CORE_CHOICE



> The CPU core on which NimBLE host will run
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> The CPU core on which NimBLE host will run. You can choose Core 0 or Core 1.
> Cannot specify no-affinity
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Core 0 (PRO CPU) (CONFIG_BT_NIMBLE_PINNED_TO_CORE_0)
> > * Core 1 (APP CPU) (CONFIG_BT_NIMBLE_PINNED_TO_CORE_1)




###### CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE



> NimBLE Host task stack size
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This configures stack size of NimBLE host task
> 
> 
> 
> Default value:* 5120 if CONFIG_BLE_MESH && CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED
> * 4096 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ROLE_CENTRAL



> Enable BLE Central role
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enables central role
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ROLE_PERIPHERAL



> Enable BLE Peripheral role
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable peripheral role
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ROLE_BROADCASTER



> Enable BLE Broadcaster role
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enables broadcaster role
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ROLE_OBSERVER



> Enable BLE Observer role
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enables observer role
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_NVS_PERSIST



> Persist the BLE Bonding keys in NVS
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable this flag to make bonding persistent across device reboots
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SMP_ID_RESET



> Reset device identity when all bonding records are deleted
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> There are tracking risks associated with using a fixed or static IRK.
> If enabled this option, Bluedroid will assign a new randomly-generated IRK
> when all pairing and bonding records are deleted. This would decrease the ability
> of a previously paired peer to be used to determine whether a device
> with which it previously shared an IRK is within range.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SECURITY_ENABLE



> Enable BLE SM feature
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable BLE sm feature
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED


Contains:


* CONFIG_BT_NIMBLE_LL_CFG_FEAT_LE_ENCRYPTION
* CONFIG_BT_NIMBLE_SM_LVL
* CONFIG_BT_NIMBLE_SM_LEGACY
* CONFIG_BT_NIMBLE_SM_SC




###### CONFIG_BT_NIMBLE_SM_LEGACY



> Security manager legacy pairing
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_SECURITY_ENABLE
> 
> 
> Enable security manager legacy pairing
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_SECURITY_ENABLE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SM_SC



> Security manager secure connections (4.2)
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_SECURITY_ENABLE
> 
> 
> Enable security manager secure connections
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_SECURITY_ENABLE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SM_SC_DEBUG_KEYS



> Use predefined public-private key pair
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_SECURITY_ENABLE > CONFIG_BT_NIMBLE_SM_SC
> 
> 
> If this option is enabled, SM uses predefined DH key pair as described
> in Core Specification, Vol. 3, Part H, 2.3.5.6.1. This allows to
> decrypt air traffic easily and thus should only be used for debugging.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_SECURITY_ENABLE && CONFIG_BT_NIMBLE_SM_SC && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_LL_CFG_FEAT_LE_ENCRYPTION



> Enable LE encryption
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_SECURITY_ENABLE
> 
> 
> Enable encryption connection
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_SECURITY_ENABLE && CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SM_LVL



> Security level
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_SECURITY_ENABLE
> 
> 
> LE Security Mode 1 Levels:
> 1. No Security
> 2. Unauthenticated pairing with encryption
> 3. Authenticated pairing with encryption
> 4. Authenticated LE Secure Connections pairing with encryption using a 128-bit strength encryption key.
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_SECURITY_ENABLE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_DEBUG



> Enable extra runtime asserts and host debugging
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This enables extra runtime asserts and host debugging
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_DYNAMIC_SERVICE



> Enable dynamic services
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This enables user to add/remove Gatt services at runtime




###### CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME



> BLE GAP default device name
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> The Device Name characteristic shall contain the name of the device as an UTF-8 string.
> This name can be changed by using API ble_svc_gap_device_name_set()
> 
> 
> 
> Default value:* "nimble" if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_GAP_DEVICE_NAME_MAX_LEN



> Maximum length of BLE device name in octets
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Device Name characteristic value shall be 0 to 248 octets in length
> 
> 
> 
> Default value:* 31 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU



> Preferred MTU size in octets
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This is the default value of ATT MTU indicated by the device during an ATT MTU exchange.
> This value can be changed using API ble_att_set_preferred_mtu()
> 
> 
> 
> Default value:* 256 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE



> External appearance of the device
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Standard BLE GAP Appearance value in HEX format e.g. 0x02C0
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### Memory Settings


Contains:


* CONFIG_BT_NIMBLE_TRANSPORT_ACL_FROM_LL_COUNT
* CONFIG_BT_NIMBLE_TRANSPORT_EVT_DISCARD_COUNT
* CONFIG_BT_NIMBLE_MSYS_BUF_FROM_HEAP
* CONFIG_BT_NIMBLE_L2CAP_COC_SDU_BUFF_COUNT
* CONFIG_BT_NIMBLE_MSYS_1_BLOCK_COUNT
* CONFIG_BT_NIMBLE_MSYS_1_BLOCK_SIZE
* CONFIG_BT_NIMBLE_MSYS_2_BLOCK_COUNT
* CONFIG_BT_NIMBLE_MSYS_2_BLOCK_SIZE
* CONFIG_BT_NIMBLE_TRANSPORT_ACL_SIZE
* CONFIG_BT_NIMBLE_TRANSPORT_EVT_COUNT
* CONFIG_BT_NIMBLE_TRANSPORT_EVT_SIZE




###### CONFIG_BT_NIMBLE_MSYS_1_BLOCK_COUNT



> MSYS_1 Block Count
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> MSYS is a system level mbuf registry. For prepare write & prepare
> responses MBUFs are allocated out of msys_1 pool. For NIMBLE_MESH
> enabled cases, this block count is increased by 8 than user defined
> count.
> 
> 
> 
> Default value:* 24 if SOC_ESP_NIMBLE_CONTROLLER && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MSYS_1_BLOCK_SIZE



> MSYS_1 Block Size
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> Dynamic memory size of block 1
> 
> 
> 
> Default value:* 128 if SOC_ESP_NIMBLE_CONTROLLER && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MSYS_2_BLOCK_COUNT



> MSYS_2 Block Count
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> Dynamic memory count
> 
> 
> 
> Default value:* 24 if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MSYS_2_BLOCK_SIZE



> MSYS_2 Block Size
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> Dynamic memory size of block 2
> 
> 
> 
> Default value:* 320 if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MSYS_BUF_FROM_HEAP



> Get Msys Mbuf from heap
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> This option sets the source of the shared msys mbuf memory between
> the Host and the Controller. Allocate the memory from the heap if
> this option is sets, from the mempool otherwise.
> 
> 
> 
> Default value:* Yes (enabled) if BT_LE_MSYS_INIT_IN_CONTROLLER && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_TRANSPORT_ACL_FROM_LL_COUNT



> ACL Buffer count
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> The number of ACL data buffers allocated for host.
> 
> 
> 
> Default value:* 24 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_TRANSPORT_ACL_SIZE



> Transport ACL Buffer size
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> This is the maximum size of the data portion of HCI ACL data packets.
> It does not include the HCI data header (of 4 bytes)
> 
> 
> 
> Default value:* 255 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_TRANSPORT_EVT_SIZE



> Transport Event Buffer size
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> This is the size of each HCI event buffer in bytes. In case of
> extended advertising, packets can be fragmented. 257 bytes is the
> maximum size of a packet.
> 
> 
> 
> Default value:* 257 if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED
> * 70 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_TRANSPORT_EVT_COUNT



> Transport Event Buffer count
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> This is the high priority HCI events' buffer size. High-priority
> event buffers are for everything except advertising reports. If there
> are no free high-priority event buffers then host will try to allocate a
> low-priority buffer instead
> 
> 
> 
> Default value:* 30 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_TRANSPORT_EVT_DISCARD_COUNT



> Discardable Transport Event Buffer count
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> This is the low priority HCI events' buffer size. Low-priority event
> buffers are only used for advertising reports. If there are no free
> low-priority event buffers, then an incoming advertising report will
> get dropped
> 
> 
> 
> Default value:* 8 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_L2CAP_COC_SDU_BUFF_COUNT



> L2cap coc Service Data Unit Buffer count
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Memory Settings
> 
> 
> This is the service data unit buffer count for l2cap coc.
> 
> 
> 
> Default value:* 1 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_GATT_MAX_PROCS



> Maximum number of GATT client procedures
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Maximum number of GATT client procedures that can be executed.
> 
> 
> 
> Default value:* 4 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HS_FLOW_CTRL



> Enable Host Flow control
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable Host Flow control
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HS_FLOW_CTRL_ITVL



> Host Flow control interval
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_HS_FLOW_CTRL
> 
> 
> Host flow control interval in msecs
> 
> 
> 
> Default value:* 1000 if CONFIG_BT_NIMBLE_HS_FLOW_CTRL && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HS_FLOW_CTRL_THRESH



> Host Flow control threshold
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_HS_FLOW_CTRL
> 
> 
> Host flow control threshold, if the number of free buffers are at or
> below this threshold, send an immediate number-of-completed-packets
> event
> 
> 
> 
> Default value:* 2 if CONFIG_BT_NIMBLE_HS_FLOW_CTRL && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HS_FLOW_CTRL_TX_ON_DISCONNECT



> Host Flow control on disconnect
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_HS_FLOW_CTRL
> 
> 
> Enable this option to send number-of-completed-packets event to
> controller after disconnection
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_HS_FLOW_CTRL && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_RPA_TIMEOUT



> RPA timeout in seconds
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Time interval between RPA address change.
> 
> 
> 
> Range:* from 1 to 41400 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 900 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH



> Enable BLE mesh functionality
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable BLE Mesh example present in upstream mynewt-nimble and not maintained by Espressif.
> 
> 
> IDF maintains ESP-BLE-MESH as the official Mesh solution. Please refer to ESP-BLE-MESH guide at:
> :doc:../esp32/api-guides/esp-ble-mesh/ble-mesh-index``
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED


Contains:


* CONFIG_BT_NIMBLE_MESH_PROVISIONER
* CONFIG_BT_NIMBLE_MESH_PROV
* CONFIG_BT_NIMBLE_MESH_GATT_PROXY
* CONFIG_BT_NIMBLE_MESH_FRIEND
* CONFIG_BT_NIMBLE_MESH_LOW_POWER
* CONFIG_BT_NIMBLE_MESH_PROXY
* CONFIG_BT_NIMBLE_MESH_RELAY
* CONFIG_BT_NIMBLE_MESH_DEVICE_NAME
* CONFIG_BT_NIMBLE_MESH_NODE_COUNT




###### CONFIG_BT_NIMBLE_MESH_PROXY



> Enable mesh proxy functionality
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> Enable proxy. This is automatically set whenever NIMBLE_MESH_PB_GATT or
> NIMBLE_MESH_GATT_PROXY is set
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_PROV



> Enable BLE mesh provisioning
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> Enable mesh provisioning
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_PB_ADV



> Enable mesh provisioning over advertising bearer
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH > CONFIG_BT_NIMBLE_MESH_PROV
> 
> 
> Enable this option to allow the device to be provisioned over
> the advertising bearer
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_MESH_PROV && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_PB_GATT



> Enable mesh provisioning over GATT bearer
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH > CONFIG_BT_NIMBLE_MESH_PROV
> 
> 
> Enable this option to allow the device to be provisioned over the GATT
> bearer
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_MESH_PROV && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_GATT_PROXY



> Enable GATT Proxy functionality
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> This option enables support for the Mesh GATT Proxy Service,
> i.e. the ability to act as a proxy between a Mesh GATT Client
> and a Mesh network
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_RELAY



> Enable mesh relay functionality
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> Support for acting as a Mesh Relay Node
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_LOW_POWER



> Enable mesh low power mode
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> Enable this option to be able to act as a Low Power Node
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_FRIEND



> Enable mesh friend functionality
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> Enable this option to be able to act as a Friend Node
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_DEVICE_NAME



> Set mesh device name
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> This value defines Bluetooth Mesh device/node name
> 
> 
> 
> Default value:* "nimble-mesh-node" if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_NODE_COUNT



> Set mesh node count
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> Defines mesh node count.
> 
> 
> 
> Default value:* 1 if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MESH_PROVISIONER



> Enable BLE mesh provisioner
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_MESH
> 
> 
> Enable mesh provisioner.
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_MESH && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_CRYPTO_STACK_MBEDTLS



> Override TinyCrypt with mbedTLS for crypto computations
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable this option to choose mbedTLS instead of TinyCrypt for crypto
> computations.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HS_STOP_TIMEOUT_MS



> BLE host stop timeout in msec
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> BLE Host stop procedure timeout in milliseconds.
> 
> 
> 
> Default value:* 2000 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ENABLE_CONN_REATTEMPT



> Enable connection reattempts on connection establishment error
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable to make the NimBLE host to reattempt GAP connection on connection
> establishment failure.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MAX_CONN_REATTEMPT



> Maximum number connection reattempts
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_ENABLE_CONN_REATTEMPT
> 
> 
> Defines maximum number of connection reattempts.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLE_CONN_REATTEMPT && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 3 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLE_CONN_REATTEMPT && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT



> Enable BLE 5 feature
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable BLE 5 feature
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED


Contains:


* CONFIG_BT_NIMBLE_LL_CFG_FEAT_LE_2M_PHY
* CONFIG_BT_NIMBLE_LL_CFG_FEAT_LE_CODED_PHY
* CONFIG_BT_NIMBLE_EXT_ADV
* CONFIG_BT_NIMBLE_GATT_CACHING
* CONFIG_BT_NIMBLE_BLE_POWER_CONTROL
* CONFIG_BT_NIMBLE_MAX_PERIODIC_ADVERTISER_LIST
* CONFIG_BT_NIMBLE_MAX_PERIODIC_SYNCS
* CONFIG_BT_NIMBLE_PERIODIC_ADV_ENH




###### CONFIG_BT_NIMBLE_LL_CFG_FEAT_LE_2M_PHY



> Enable 2M Phy
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Enable 2M-PHY
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_LL_CFG_FEAT_LE_CODED_PHY



> Enable coded Phy
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Enable coded-PHY
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_EXT_ADV



> Enable extended advertising
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Enable this option to do extended advertising. Extended advertising
> will be supported from BLE 5.0 onwards.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MAX_EXT_ADV_INSTANCES



> Maximum number of extended advertising instances.
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_EXT_ADV
> 
> 
> Change this option to set maximum number of extended advertising
> instances. Minimum there is always one instance of
> advertising. Enter how many more advertising instances you
> want.
> For ESP32C2, ESP32C6 and ESP32H2, each extended advertising instance
> will take about 0.5k DRAM.
> 
> 
> 
> Range:* from 0 to 4 if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 1 if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED
> * 0 if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_EXT_ADV_MAX_SIZE



> Maximum length of the advertising data.
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_EXT_ADV
> 
> 
> Defines the length of the extended adv data. The value should not
> exceed 1650.
> 
> 
> 
> Range:* from 0 to 1650 if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 1650 if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED
> * 0 if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV



> Enable periodic advertisement.
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_EXT_ADV
> 
> 
> Enable this option to start periodic advertisement.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_PERIODIC_ADV_SYNC_TRANSFER



> Enable Transfer Sync Events
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_EXT_ADV > CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV
> 
> 
> This enables controller transfer periodic sync events to host
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV && CONFIG_BT_NIMBLE_EXT_ADV && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MAX_PERIODIC_SYNCS



> Maximum number of periodic advertising syncs
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Set this option to set the upper limit for number of periodic sync
> connections. This should be less than maximum connections allowed by
> controller.
> 
> 
> 
> Range:* from 0 to 8 if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 1 if CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV && CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && CONFIG_BT_NIMBLE_ENABLED
> * 0 if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_MAX_PERIODIC_ADVERTISER_LIST



> Maximum number of periodic advertiser list
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Set this option to set the upper limit for number of periodic advertiser list.
> 
> 
> 
> Range:* from 1 to 5 if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && SOC_ESP_NIMBLE_CONTROLLER && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 5 if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && SOC_ESP_NIMBLE_CONTROLLER && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_BLE_POWER_CONTROL



> Enable support for BLE Power Control
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Set this option to enable the Power Control feature
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT && SOC_BLE_POWER_CONTROL_SUPPORTED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_PERIODIC_ADV_ENH



> Periodic adv enhancements(adi support)
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Enable the periodic advertising enhancements




###### CONFIG_BT_NIMBLE_GATT_CACHING



> Enable GATT caching
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT
> 
> 
> Enable GATT caching


Contains:


* CONFIG_BT_NIMBLE_GATT_CACHING_MAX_CONNS
* CONFIG_BT_NIMBLE_GATT_CACHING_MAX_CHRS
* CONFIG_BT_NIMBLE_GATT_CACHING_MAX_DSCS
* CONFIG_BT_NIMBLE_GATT_CACHING_MAX_SVCS




###### CONFIG_BT_NIMBLE_GATT_CACHING_MAX_CONNS



> Maximum connections to be cached
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_GATT_CACHING
> 
> 
> Set this option to set the upper limit on number of connections to be cached.
> 
> 
> 
> Default value:* 1 if CONFIG_BT_NIMBLE_GATT_CACHING && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_GATT_CACHING_MAX_SVCS



> Maximum number of services per connection
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_GATT_CACHING
> 
> 
> Set this option to set the upper limit on number of services per connection to be cached.
> 
> 
> 
> Default value:* 64 if CONFIG_BT_NIMBLE_GATT_CACHING && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_GATT_CACHING_MAX_CHRS



> Maximum number of characteristics per connection
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_GATT_CACHING
> 
> 
> Set this option to set the upper limit on number of characteristics per connection to be cached.
> 
> 
> 
> Default value:* 64 if CONFIG_BT_NIMBLE_GATT_CACHING && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_GATT_CACHING_MAX_DSCS



> Maximum number of descriptors per connection
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT > CONFIG_BT_NIMBLE_GATT_CACHING
> 
> 
> Set this option to set the upper limit on number of descriptors per connection to be cached.
> 
> 
> 
> Default value:* 64 if CONFIG_BT_NIMBLE_GATT_CACHING && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_WHITELIST_SIZE



> BLE white list size
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> BLE list size
> 
> 
> 
> Range:* from 1 to 15 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED
> 
> 
> 
> Default value:* 12 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_TEST_THROUGHPUT_TEST



> Throughput Test Mode enable
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Enable the throughput test mode
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_BLUFI_ENABLE



> Enable blufi functionality
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Set this option to enable blufi functionality.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_USE_ESP_TIMER



> Enable Esp Timer for Nimble
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> Set this option to use Esp Timer which has higher priority timer instead of FreeRTOS timer
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_BLE_GATT_BLOB_TRANSFER



> Blob transfer
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This option is used when data to be sent is more than 512 bytes. For peripheral role,
> BT_NIMBLE_MSYS_1_BLOCK_COUNT needs to be increased according to the need.




###### GAP Service


Contains:


* GAP Appearance write permissions
* CONFIG_BT_NIMBLE_SVC_GAP_CENT_ADDR_RESOLUTION
* GAP device name write permissions
* CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MAX_CONN_INTERVAL
* CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MIN_CONN_INTERVAL
* CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SLAVE_LATENCY
* CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SUPERVISION_TMO




###### GAP Appearance write permissions


Contains:


* CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE




###### CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE



> Write
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP Appearance write permissions
> 
> 
> Enable write permission (BLE_GATT_CHR_F_WRITE)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE_ENC



> Write with encryption
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP Appearance write permissions > CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE
> 
> 
> Enable write with encryption permission (BLE_GATT_CHR_F_WRITE_ENC)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE_AUTHEN



> Write with authentication
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP Appearance write permissions > CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE
> 
> 
> Enable write with authentication permission (BLE_GATT_CHR_F_WRITE_AUTHEN)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE_AUTHOR



> Write with authorisation
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP Appearance write permissions > CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE
> 
> 
> Enable write with authorisation permission (BLE_GATT_CHR_F_WRITE_AUTHOR)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_SVC_GAP_APPEAR_WRITE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_CENT_ADDR_RESOLUTION



> GAP Characteristic - Central Address Resolution
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service
> 
> 
> Weather or not Central Address Resolution characteristic is supported on
> the device, and if supported, weather or not Central Address Resolution
> is supported.
> 
> 
> * Central Address Resolution characteristic not supported
> * Central Address Resolution not supported
> * Central Address Resolution supported
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Characteristic not supported (CONFIG_BT_NIMBLE_SVC_GAP_CAR_CHAR_NOT_SUPP)
> > * Central Address Resolution not supported (CONFIG_BT_NIMBLE_SVC_GAP_CAR_NOT_SUPP)
> > * Central Address Resolution supported (CONFIG_BT_NIMBLE_SVC_GAP_CAR_SUPP)




###### GAP device name write permissions


Contains:


* CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE




###### CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE



> Write
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP device name write permissions
> 
> 
> Enable write permission (BLE_GATT_CHR_F_WRITE)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE_ENC



> Write with encryption
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP device name write permissions > CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE
> 
> 
> Enable write with encryption permission (BLE_GATT_CHR_F_WRITE_ENC)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE_AUTHEN



> Write with authentication
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP device name write permissions > CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE
> 
> 
> Enable write with authentication permission (BLE_GATT_CHR_F_WRITE_AUTHEN)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE_AUTHOR



> Write with authorisation
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service > GAP device name write permissions > CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE
> 
> 
> Enable write with authorisation permission (BLE_GATT_CHR_F_WRITE_AUTHOR)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_SVC_GAP_NAME_WRITE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MAX_CONN_INTERVAL



> PPCP Connection Interval Max (Unit: 1.25 ms)
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service
> 
> 
> Peripheral Preferred Connection Parameter: Connection Interval maximum value
> Interval Max = value \* 1.25 ms
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MIN_CONN_INTERVAL



> PPCP Connection Interval Min (Unit: 1.25 ms)
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service
> 
> 
> Peripheral Preferred Connection Parameter: Connection Interval minimum value
> Interval Min = value \* 1.25 ms
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SLAVE_LATENCY



> PPCP Slave Latency
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service
> 
> 
> Peripheral Preferred Connection Parameter: Slave Latency
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SUPERVISION_TMO



> PPCP Supervision Timeout (Uint: 10 ms)
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > GAP Service
> 
> 
> Peripheral Preferred Connection Parameter: Supervision Timeout
> Timeout = Value \* 10 ms
> 
> 
> 
> Default value:* 0 if CONFIG_BT_NIMBLE_ENABLED




###### BLE Services


Contains:


* CONFIG_BT_NIMBLE_HID_SERVICE




###### CONFIG_BT_NIMBLE_HID_SERVICE



> HID service
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > BLE Services
> 
> 
> Enable HID service support
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED


Contains:


* CONFIG_BT_NIMBLE_SVC_HID_MAX_RPTS
* CONFIG_BT_NIMBLE_SVC_HID_MAX_INSTANCES




###### CONFIG_BT_NIMBLE_SVC_HID_MAX_INSTANCES



> Maximum HID service instances
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > BLE Services > CONFIG_BT_NIMBLE_HID_SERVICE
> 
> 
> Defines maximum number of HID service instances
> 
> 
> 
> Default value:* 2 if CONFIG_BT_NIMBLE_HID_SERVICE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_SVC_HID_MAX_RPTS



> Maximum HID Report characteristics per service instance
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > BLE Services > CONFIG_BT_NIMBLE_HID_SERVICE
> 
> 
> Defines maximum number of report characteristics per service instance
> 
> 
> 
> Default value:* 3 if CONFIG_BT_NIMBLE_HID_SERVICE && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_VS_SUPPORT



> Enable support for VSC and VSE
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This option is used to enable support for sending Vendor Specific HCI commands and handling
> Vendor Specific HCI Events.




###### CONFIG_BT_NIMBLE_OPTIMIZE_MULTI_CONN



> Enable the optimization of multi-connection
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This option enables the use of vendor-specific APIs for multi-connections, which can
> greatly enhance the stability of coexistence between numerous central and peripheral
> devices. It will prohibit the usage of standard APIs.
> 
> 
> 
> Default value:* No (disabled) if SOC_BLE_MULTI_CONN_OPTIMIZATION && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_ENC_ADV_DATA



> Encrypted Advertising Data
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This option is used to enable encrypted advertising data.




###### CONFIG_BT_NIMBLE_MAX_EADS



> Maximum number of EAD devices to save across reboots
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > CONFIG_BT_NIMBLE_ENC_ADV_DATA
> 
> 
> Defines maximum number of encrypted advertising data key material to save
> 
> 
> 
> Default value:* 10 if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENC_ADV_DATA && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HIGH_DUTY_ADV_ITVL



> Enable BLE high duty advertising interval feature
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> This enable BLE high duty advertising interval feature




###### CONFIG_BT_NIMBLE_HOST_QUEUE_CONG_CHECK



> BLE queue congestion check
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options
> 
> 
> When scanning and scan duplicate is not enabled, if there are a lot of adv packets around
> or application layer handling adv packets is slow, it will cause the controller memory
> to run out. if enabled, adv packets will be lost when host queue is congested.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_NIMBLE_ENABLED && CONFIG_BT_NIMBLE_ENABLED




###### Host-controller Transport


Contains:


* CONFIG_BT_NIMBLE_TRANSPORT_UART
* CONFIG_BT_NIMBLE_HCI_UART_CTS_PIN
* CONFIG_BT_NIMBLE_USE_HCI_UART_FLOW_CTRL
* CONFIG_BT_NIMBLE_HCI_UART_RTS_PIN




###### CONFIG_BT_NIMBLE_TRANSPORT_UART



> Enable Uart Transport
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport
> 
> 
> Use UART transport
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_CONTROLLER_DISABLED && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_TRANSPORT_UART_PORT



> Uart port
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport > CONFIG_BT_NIMBLE_TRANSPORT_UART
> 
> 
> Uart port
> 
> 
> 
> Default value:* 1 if CONFIG_BT_CONTROLLER_DISABLED && CONFIG_BT_NIMBLE_TRANSPORT_UART && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HCI_USE_UART_BAUDRATE



> Uart Hci Baud Rate
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport > CONFIG_BT_NIMBLE_TRANSPORT_UART
> 
> 
> Uart Baud Rate
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 115200 (CONFIG_UART_BAUDRATE_115200)
> > * 230400 (CONFIG_UART_BAUDRATE_230400)
> > * 460800 (CONFIG_UART_BAUDRATE_460800)
> > * 921600 (CONFIG_UART_BAUDRATE_921600)




###### CONFIG_BT_NIMBLE_USE_HCI_UART_PARITY



> Uart PARITY
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport > CONFIG_BT_NIMBLE_TRANSPORT_UART
> 
> 
> Uart Parity
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * None (CONFIG_UART_PARITY_NONE)
> > * Odd (CONFIG_UART_PARITY_ODD)
> > * Even (CONFIG_UART_PARITY_EVEN)




###### CONFIG_BT_NIMBLE_UART_RX_PIN



> UART Rx pin
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport > CONFIG_BT_NIMBLE_TRANSPORT_UART
> 
> 
> Rx pin for Nimble Transport
> 
> 
> 
> Default value:* 5 if CONFIG_BT_CONTROLLER_DISABLED && CONFIG_BT_NIMBLE_TRANSPORT_UART && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_UART_TX_PIN



> UART Tx pin
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport > CONFIG_BT_NIMBLE_TRANSPORT_UART
> 
> 
> Tx pin for Nimble Transport
> 
> 
> 
> Default value:* 4 if CONFIG_BT_CONTROLLER_DISABLED && CONFIG_BT_NIMBLE_TRANSPORT_UART && CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_USE_HCI_UART_FLOW_CTRL



> Uart Flow Control
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport
> 
> 
> Uart Flow Control
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Disable (CONFIG_UART_HW_FLOWCTRL_DISABLE)
> > * Enable hardware flow control (CONFIG_UART_HW_FLOWCTRL_CTS_RTS)




###### CONFIG_BT_NIMBLE_HCI_UART_RTS_PIN



> UART Rts Pin
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport
> 
> 
> UART HCI RTS pin
> 
> 
> 
> Default value:* 19 if CONFIG_BT_NIMBLE_ENABLED




###### CONFIG_BT_NIMBLE_HCI_UART_CTS_PIN



> UART Cts Pin
> 
> 
> *Found in:* Component config > Bluetooth > NimBLE Options > Host-controller Transport
> 
> 
> UART HCI CTS pin
> 
> 
> 
> Default value:* 23 if CONFIG_BT_NIMBLE_ENABLED





##### Controller Options


Contains:


* CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP
* CONFIG_BT_BLE_CCA_MODE
* CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL
* CONFIG_BT_CTRL_BLE_MAX_ACT
* CONFIG_BT_CTRL_BLE_SCAN_DUPL
* CONFIG_BT_CTRL_BLE_STATIC_ACL_TX_BUF_NB
* CONFIG_BT_CTRL_HW_CCA_VAL
* CONFIG_BT_CTRL_COEX_PHY_CODED_TX_RX_TLIM
* CONFIG_BT_CTRL_CE_LENGTH_TYPE
* CONFIG_BT_CTRL_RX_ANTENNA_INDEX
* CONFIG_BT_CTRL_TX_ANTENNA_INDEX
* CONFIG_BT_CTRL_SCAN_BACKOFF_UPPERLIMITMAX
* CONFIG_BT_BLE_ADV_DATA_LENGTH_ZERO_AUX
* CONFIG_BT_CTRL_CHAN_ASS_EN
* CONFIG_BT_CTRL_AGC_RECORRECT_EN
* CONFIG_BT_CTRL_LE_PING_EN
* CONFIG_BT_CTRL_HCI_MODE_CHOICE
* MODEM SLEEP Options
* CONFIG_BT_CTRL_PINNED_TO_CORE_CHOICE
* CONFIG_BT_CTRL_ADV_DUP_FILT_MAX



###### CONFIG_BT_CTRL_BLE_MAX_ACT



> BLE Max Instances
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> BLE maximum activities of bluetooth controller，both of connections,
> scan , sync and adv(periodic adv, multi-adv). Each instance needs to
> consume 828 bytes, you can save RAM by modifying the instance value
> according to actual needs.
> 
> 
> 
> Range:* from 1 to 10 if CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 6 if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_BLE_STATIC_ACL_TX_BUF_NB



> BLE static ACL TX buffer numbers
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> BLE ACL buffer have two methods to be allocated. One is persistent allocating
> (allocate when controller initialise, never free until controller de-initialise)
> another is dynamically allocating (allocate before TX and free after TX).
> 
> 
> 
> Range:* from 0 to 12 if CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 0 if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_PINNED_TO_CORE_CHOICE



> The cpu core which bluetooth controller run
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Specify the cpu core to run bluetooth controller.
> Can not specify no-affinity.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Core 0 (PRO CPU) (CONFIG_BT_CTRL_PINNED_TO_CORE_0)
> > * Core 1 (APP CPU) (CONFIG_BT_CTRL_PINNED_TO_CORE_1)




###### CONFIG_BT_CTRL_HCI_MODE_CHOICE



> HCI mode
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Specify HCI mode as VHCI or UART(H4)
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * VHCI (CONFIG_BT_CTRL_HCI_MODE_VHCI)
> > 
> > 
> > Normal option. Mostly, choose this VHCI when bluetooth host run on ESP32S3 or ESP32C3.
> > * UART(H4) (CONFIG_BT_CTRL_HCI_MODE_UART_H4)
> > 
> > 
> > If use external bluetooth host which run on other hardware and use UART as the HCI interface,
> > choose this option.




###### CONFIG_BT_CTRL_ADV_DUP_FILT_MAX



> The maximum number of 5.0 extend duplicate scan filter
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> The maximum number of suplicate scan filter
> 
> 
> 
> Range:* from 1 to 500 if CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 30 if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_BLE_CCA_MODE



> BLE CCA mode
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Define BT BLE CCA mode
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BT_BLE_CCA_MODE_NONE)
> > * Hardware (CONFIG_BT_BLE_CCA_MODE_HW)
> > * Software (CONFIG_BT_BLE_CCA_MODE_SW)




###### CONFIG_BT_CTRL_HW_CCA_VAL



> CCA threshold value
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> It is the threshold value of HW CCA, if the value is 30, it means CCA threshold is -30 dBm.
> 
> 
> 
> Range:* from 20 to 100 if CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 20 if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_CE_LENGTH_TYPE



> Connection event length determination method
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Specify connection event length determination
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * ORIGINAL (CONFIG_BT_CTRL_CE_LENGTH_TYPE_ORIG)
> > * Use CE parameter for HCI command (CONFIG_BT_CTRL_CE_LENGTH_TYPE_CE)
> > * Use Espressif self-defined method (CONFIG_BT_CTRL_CE_LENGTH_TYPE_SD)




###### CONFIG_BT_CTRL_TX_ANTENNA_INDEX



> default Tx anntena used
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Specify default Tx antenna used for bluetooth
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Antenna 0 (CONFIG_BT_CTRL_TX_ANTENNA_INDEX_0)
> > * Antenna 1 (CONFIG_BT_CTRL_TX_ANTENNA_INDEX_1)




###### CONFIG_BT_CTRL_RX_ANTENNA_INDEX



> default Rx anntena used
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Specify default Rx antenna used for bluetooth
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Antenna 0 (CONFIG_BT_CTRL_RX_ANTENNA_INDEX_0)
> > * Antenna 1 (CONFIG_BT_CTRL_RX_ANTENNA_INDEX_1)




###### CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL



> BLE default Tx power level
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Specify default Tx power level
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * -24dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N24)
> > * -21dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N21)
> > * -18dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N18)
> > * -15dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N15)
> > * -12dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N12)
> > * `-9dBm`
> > (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N9)
> > * `-6dBm`
> > (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N6)
> > * `-3dBm`
> > (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N3)
> > * 0dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_N0)
> > * `+3dBm`
> > (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_P3)
> > * `+6dBm`
> > (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_P6)
> > * `+9dBm`
> > (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_P9)
> > * +12dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_P12)
> > * +15dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_P15)
> > * +18dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_P18)
> > * +21dBm (CONFIG_BT_CTRL_DFT_TX_POWER_LEVEL_P21)




###### CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP



> BLE adv report flow control supported
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> The function is mainly used to enable flow control for advertising reports. When it is enabled,
> advertising reports will be discarded by the controller if the number of unprocessed advertising
> reports exceeds the size of BLE adv report flow control.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_NUM



> BLE adv report flow control number
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP
> 
> 
> The number of unprocessed advertising report that bluetooth host can save.If you set
> BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_NUM to a small value, this may cause adv packets lost.
> If you set BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_NUM to a large value, bluetooth host may cache a
> lot of adv packets and this may cause system memory run out. For example, if you set
> it to 50, the maximum memory consumed by host is 35 \* 50 bytes. Please set
> BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_NUM according to your system free memory and handle adv
> packets as fast as possible, otherwise it will cause adv packets lost.
> 
> 
> 
> Range:* from 50 to 1000 if CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP && CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 100 if CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_BLE_ADV_REPORT_DISCARD_THRSHOLD



> BLE adv lost event threshold value
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP
> 
> 
> When adv report flow control is enabled, The ADV lost event will be generated when the number
> of ADV packets lost in the controller reaches this threshold. It is better to set a larger value.
> If you set BT_CTRL_BLE_ADV_REPORT_DISCARD_THRSHOLD to a small value or printf every adv lost event, it
> may cause adv packets lost more.
> 
> 
> 
> Range:* from 1 to 1000 if CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP && CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 20 if CONFIG_BT_CTRL_BLE_ADV_REPORT_FLOW_CTRL_SUPP && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_BLE_SCAN_DUPL



> BLE Scan Duplicate Options
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> This select enables parameters setting of BLE scan duplicate.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_SCAN_DUPL_TYPE



> Scan Duplicate Type
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_BLE_SCAN_DUPL
> 
> 
> Scan duplicate have three ways. one is "Scan Duplicate By Device Address", This way is to use
> advertiser address filtering. The adv packet of the same address is only allowed to be reported once.
> Another way is "Scan Duplicate By Device Address And Advertising Data". This way is to use advertising
> data and device address filtering. All different adv packets with the same address are allowed to be
> reported. The last way is "Scan Duplicate By Advertising Data". This way is to use advertising data
> filtering. All same advertising data only allow to be reported once even though they are from
> different devices.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Scan Duplicate By Device Address (CONFIG_BT_CTRL_SCAN_DUPL_TYPE_DEVICE)
> > 
> > 
> > Advertising packets with the same address, address type, and advertising type are reported once.
> > * Scan Duplicate By Advertising Data (CONFIG_BT_CTRL_SCAN_DUPL_TYPE_DATA)
> > 
> > 
> > Advertising packets with identical advertising data, address type, and advertising type
> > are reported only once, even if they originate from different devices.
> > * Scan Duplicate By Device Address And Advertising Data (CONFIG_BT_CTRL_SCAN_DUPL_TYPE_DATA_DEVICE)
> > 
> > 
> > Advertising packets with the same address, advertising data, address type,
> > and advertising type are reported only once.




###### CONFIG_BT_CTRL_SCAN_DUPL_CACHE_SIZE



> Maximum number of devices in scan duplicate filter
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_BLE_SCAN_DUPL
> 
> 
> Maximum number of devices which can be recorded in scan duplicate filter.
> When the maximum amount of device in the filter is reached, the cache will be refreshed.
> 
> 
> 
> Range:* from 10 to 1000 if CONFIG_BT_CTRL_BLE_SCAN_DUPL && CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 100 if CONFIG_BT_CTRL_BLE_SCAN_DUPL && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_DUPL_SCAN_CACHE_REFRESH_PERIOD



> Duplicate scan list refresh period (seconds)
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_BLE_SCAN_DUPL
> 
> 
> If the period value is non-zero, the controller will periodically clear the device information
> stored in the scan duuplicate filter. If it is 0, the scan duuplicate filter will not be cleared
> until the scanning is disabled. Duplicate advertisements for this period should not be sent to the
> Host in advertising report events.
> There are two scenarios where the ADV packet will be repeatedly reported:
> 1. The duplicate scan cache is full, the controller will delete the oldest device information and
> add new device information.
> 2. When the refresh period is up, the controller will clear all device information and start filtering
> again.
> 
> 
> 
> Range:* from 0 to 1000 if CONFIG_BT_CTRL_BLE_SCAN_DUPL && CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 0 if CONFIG_BT_CTRL_BLE_SCAN_DUPL && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_BLE_MESH_SCAN_DUPL_EN



> Special duplicate scan mechanism for BLE Mesh scan
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_BLE_SCAN_DUPL
> 
> 
> This enables the BLE scan duplicate for special BLE Mesh scan.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CTRL_BLE_SCAN_DUPL && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_MESH_DUPL_SCAN_CACHE_SIZE



> Maximum number of Mesh adv packets in scan duplicate filter
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_BLE_SCAN_DUPL > CONFIG_BT_CTRL_BLE_MESH_SCAN_DUPL_EN
> 
> 
> Maximum number of adv packets which can be recorded in duplicate scan cache for BLE Mesh.
> When the maximum amount of device in the filter is reached, the cache will be refreshed.
> 
> 
> 
> Range:* from 10 to 1000 if CONFIG_BT_CTRL_BLE_MESH_SCAN_DUPL_EN && CONFIG_BT_CONTROLLER_ENABLED
> 
> 
> 
> Default value:* 100 if CONFIG_BT_CTRL_BLE_MESH_SCAN_DUPL_EN && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_COEX_PHY_CODED_TX_RX_TLIM



> Coexistence: limit on MAX Tx/Rx time for coded-PHY connection
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> When using PHY-Coded in BLE connection, limitation on max tx/rx time can be applied to
> better avoid dramatic performance deterioration of Wi-Fi.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Force Enable (CONFIG_BT_CTRL_COEX_PHY_CODED_TX_RX_TLIM_EN)
> > 
> > 
> > Always enable the limitation on max tx/rx time for Coded-PHY connection
> > * Force Disable (CONFIG_BT_CTRL_COEX_PHY_CODED_TX_RX_TLIM_DIS)
> > 
> > 
> > Disable the limitation on max tx/rx time for Coded-PHY connection




###### MODEM SLEEP Options


Contains:


* CONFIG_BT_CTRL_MODEM_SLEEP
* CONFIG_BT_CTRL_MAIN_XTAL_PU_DURING_LIGHT_SLEEP




###### CONFIG_BT_CTRL_MODEM_SLEEP



> Bluetooth modem sleep
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > MODEM SLEEP Options
> 
> 
> Enable/disable bluetooth controller low power mode.
> Modem sleep is not supported to be used with UART HCI.




###### CONFIG_BT_CTRL_MODEM_SLEEP_MODE_1



> Bluetooth Modem sleep Mode 1
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > MODEM SLEEP Options > CONFIG_BT_CTRL_MODEM_SLEEP
> 
> 
> Mode 1 is the currently supported sleep mode. In this mode,
> bluetooth controller sleeps between and BLE events. A low
> power clock is used to maintain bluetooth reference clock.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_CTRL_MODEM_SLEEP && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_LOW_POWER_CLOCK



> Bluetooth low power clock
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > MODEM SLEEP Options > CONFIG_BT_CTRL_MODEM_SLEEP > CONFIG_BT_CTRL_MODEM_SLEEP_MODE_1
> 
> 
> Select the low power clock source for bluetooth controller
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Main crystal (CONFIG_BT_CTRL_LPCLK_SEL_MAIN_XTAL)
> > 
> > 
> > Main crystal can be used as low power clock for bluetooth modem sleep. If this option is
> > selected, bluetooth modem sleep can work under Dynamic Frequency Scaling(DFS) enabled, and
> > bluetooth can work under light sleep enabled. Main crystal has a relatively better performance
> > than other bluetooth low power clock sources.
> > * External 32kHz crystal (CONFIG_BT_CTRL_LPCLK_SEL_EXT_32K_XTAL)
> > 
> > 
> > External 32kHz crystal has a nominal frequency of 32.768kHz and provides good frequency
> > stability. If used as Bluetooth low power clock, External 32kHz can support Bluetooth
> > modem sleep to be used with both DFS and light sleep.
> > * Internal 150kHz RC oscillator (CONFIG_BT_CTRL_LPCLK_SEL_RTC_SLOW)
> > 
> > 
> > Internal 150kHz RC oscillator. The accuracy of this clock is a lot larger than 500ppm which is required
> > in Bluetooth communication, so don't select this option in scenarios such as BLE connection state.




###### CONFIG_BT_CTRL_MAIN_XTAL_PU_DURING_LIGHT_SLEEP



> power up main XTAL during light sleep
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > MODEM SLEEP Options
> 
> 
> If this option is selected, the main crystal will power up during light sleep when the low power clock
> selects an external 32kHz crystal but the external 32kHz crystal does not exist or the low power clock
> selects the main crystal.
> 
> 
> 
> Default value:* No (disabled) if (CONFIG_BT_CTRL_LPCLK_SEL_MAIN_XTAL || CONFIG_BT_CTRL_LPCLK_SEL_EXT_32K_XTAL) && CONFIG_FREERTOS_USE_TICKLESS_IDLE && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_AGC_RECORRECT_EN



> Enable HW AGC recorrect
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Enable uncoded phy AGC recorrect
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_CODED_AGC_RECORRECT_EN



> Enable coded phy AGC recorrect
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options > CONFIG_BT_CTRL_AGC_RECORRECT_EN
> 
> 
> Enable coded phy AGC recorrect
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CTRL_AGC_RECORRECT_EN && CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_SCAN_BACKOFF_UPPERLIMITMAX



> Disable active scan backoff
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> Disable active scan backoff. The bluetooth spec requires that scanners should run a backoff procedure to
> minimize collision of scan request PDUs from nultiple scanners. If scan backoff is disabled, in active
> scanning, scan request PDU will be sent every time when HW receives scannable ADV PDU.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_BLE_ADV_DATA_LENGTH_ZERO_AUX



> Enable aux packet when ext adv data length is zero
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> When this option is enabled, auxiliary packets will be present in the events of
> 'Non-Connectable and Non-Scannable' regardless of whether the advertising length is 0.
> If this option is not enabled, auxiliary packets will only be present when the advertising length is not 0.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_CHAN_ASS_EN



> Enable channel assessment
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> If this option is enabled, The Controller will records the communication quality
> for each channel and then start a timer to check and update the channel map every 4 seconds.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_CONTROLLER_ENABLED




###### CONFIG_BT_CTRL_LE_PING_EN



> Enable LE Ping procedure
> 
> 
> *Found in:* Component config > Bluetooth > Controller Options
> 
> 
> If this option is disabled, The Controller will not start the LE authenticated payload timer.
> This option is used for some compatibility problems related to LE ping procedure.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_CONTROLLER_ENABLED





##### CONFIG_BT_RELEASE_IRAM



> Release Bluetooth text (READ DOCS FIRST)
> 
> 
> *Found in:* Component config > Bluetooth
> 
> 
> This option release Bluetooth text section and merge Bluetooth data, bss & text into
> a large free heap region when esp_bt_mem_release is called, total saving ~21kB or more of IRAM.
> ESP32-C2 only 3 configurable PMP entries available, rest of them are hard-coded.
> We cannot split the memory into 3 different regions (IRAM, BLE-IRAM, DRAM).
> So this option will disable the PMP (ESP_SYSTEM_PMP_IDRAM_SPLIT)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_ENABLED && BT_LE_RELEASE_IRAM_SUPPORTED




##### Common Options


Contains:


* CONFIG_BT_ALARM_MAX_NUM



###### CONFIG_BT_ALARM_MAX_NUM



> Maximum number of Bluetooth alarms
> 
> 
> *Found in:* Component config > Bluetooth > Common Options
> 
> 
> This option decides the maximum number of alarms which
> could be used by Bluetooth host.
> 
> 
> 
> Default value:* 50





##### CONFIG_BT_HCI_LOG_DEBUG_EN



> Enable Bluetooth HCI debug mode
> 
> 
> *Found in:* Component config > Bluetooth
> 
> 
> This option is used to enable bluetooth debug mode, which saves the hci layer data stream.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BT_BLUEDROID_ENABLED || CONFIG_BT_NIMBLE_ENABLED



###### CONFIG_BT_HCI_LOG_DATA_BUFFER_SIZE



> Size of the cache used for HCI data in Bluetooth HCI debug mode (N\*1024 bytes)
> 
> 
> *Found in:* Component config > Bluetooth > CONFIG_BT_HCI_LOG_DEBUG_EN
> 
> 
> This option is to configure the buffer size of the hci data steam cache in hci debug mode.
> This is a ring buffer, the new data will overwrite the oldest data if the buffer is full.
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_BT_HCI_LOG_DEBUG_EN
> 
> 
> 
> Default value:* 5 if CONFIG_BT_HCI_LOG_DEBUG_EN




###### CONFIG_BT_HCI_LOG_ADV_BUFFER_SIZE



> Size of the cache used for adv report in Bluetooth HCI debug mode (N\*1024 bytes)
> 
> 
> *Found in:* Component config > Bluetooth > CONFIG_BT_HCI_LOG_DEBUG_EN
> 
> 
> This option is to configure the buffer size of the hci adv report cache in hci debug mode.
> This is a ring buffer, the new data will overwrite the oldest data if the buffer is full.
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_BT_HCI_LOG_DEBUG_EN
> 
> 
> 
> Default value:* 8 if CONFIG_BT_HCI_LOG_DEBUG_EN






#### CONFIG_BLE_MESH



> ESP BLE Mesh Support
> 
> 
> *Found in:* Component config
> 
> 
> This option enables ESP BLE Mesh support. The specific features that are
> available may depend on other features that have been enabled in the
> stack, such as Bluetooth Support, Bluedroid Support & GATT support.


Contains:


* BLE Mesh and BLE coexistence support
* CONFIG_BLE_MESH_GATT_PROXY_CLIENT
* CONFIG_BLE_MESH_GATT_PROXY_SERVER
* BLE Mesh NET BUF DEBUG LOG LEVEL
* CONFIG_BLE_MESH_PROV
* CONFIG_BLE_MESH_PROXY
* BLE Mesh specific test option
* BLE Mesh STACK DEBUG LOG LEVEL
* CONFIG_BLE_MESH_NO_LOG
* CONFIG_BLE_MESH_IVU_DIVIDER
* CONFIG_BLE_MESH_FAST_PROV
* CONFIG_BLE_MESH_FREERTOS_STATIC_ALLOC
* CONFIG_BLE_MESH_EXPERIMENTAL
* CONFIG_BLE_MESH_CRPL
* CONFIG_BLE_MESH_RX_SDU_MAX
* CONFIG_BLE_MESH_MODEL_KEY_COUNT
* CONFIG_BLE_MESH_APP_KEY_COUNT
* CONFIG_BLE_MESH_MODEL_GROUP_COUNT
* CONFIG_BLE_MESH_LABEL_COUNT
* CONFIG_BLE_MESH_SUBNET_COUNT
* CONFIG_BLE_MESH_TX_SEG_MAX
* CONFIG_BLE_MESH_RX_SEG_MSG_COUNT
* CONFIG_BLE_MESH_TX_SEG_MSG_COUNT
* CONFIG_BLE_MESH_MEM_ALLOC_MODE
* CONFIG_BLE_MESH_MSG_CACHE_SIZE
* CONFIG_BLE_MESH_NOT_RELAY_REPLAY_MSG
* CONFIG_BLE_MESH_ADV_BUF_COUNT
* CONFIG_BLE_MESH_PB_GATT
* CONFIG_BLE_MESH_PB_ADV
* CONFIG_BLE_MESH_IVU_RECOVERY_IVI
* CONFIG_BLE_MESH_RELAY
* CONFIG_BLE_MESH_SAR_ENHANCEMENT
* CONFIG_BLE_MESH_SETTINGS
* CONFIG_BLE_MESH_ACTIVE_SCAN
* CONFIG_BLE_MESH_DEINIT
* CONFIG_BLE_MESH_USE_DUPLICATE_SCAN
* Support for BLE Mesh Client/Server models
* Support for BLE Mesh Foundation models
* CONFIG_BLE_MESH_NODE
* CONFIG_BLE_MESH_PROVISIONER
* CONFIG_BLE_MESH_FRIEND
* CONFIG_BLE_MESH_LOW_POWER
* CONFIG_BLE_MESH_HCI_5_0
* CONFIG_BLE_MESH_RANDOM_ADV_INTERVAL
* CONFIG_BLE_MESH_IV_UPDATE_TEST
* CONFIG_BLE_MESH_CLIENT_MSG_TIMEOUT



##### CONFIG_BLE_MESH_HCI_5_0



> Support sending 20ms non-connectable adv packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> It is a temporary solution and needs further modifications.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_RANDOM_ADV_INTERVAL



> Support using random adv interval for mesh packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to allow using random advertising interval
> for mesh packets. And this could help avoid collision of
> advertising packets.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_USE_DUPLICATE_SCAN



> Support Duplicate Scan in BLE Mesh
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to allow using specific duplicate scan filter
> in BLE Mesh, and Scan Duplicate Type must be set by choosing the
> option in the Bluetooth Controller section in menuconfig, which is
> "Scan Duplicate By Device Address and Advertising Data".
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_ACTIVE_SCAN



> Support Active Scan in BLE Mesh
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to allow using BLE Active Scan for BLE Mesh.




##### CONFIG_BLE_MESH_MEM_ALLOC_MODE



> Memory allocation strategy
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Allocation strategy for BLE Mesh stack, essentially provides ability to
> allocate all required dynamic allocations from,
> 
> 
> * Internal DRAM memory only
> * External SPIRAM memory only
> * Either internal or external memory based on default malloc()
> behavior in ESP-IDF
> * Internal IRAM memory wherever applicable else internal DRAM
> 
> 
> Recommended mode here is always internal (\*), since that is most preferred
> from security perspective. But if application requirement does not
> allow sufficient free internal memory then alternate mode can be
> selected.
> 
> 
> (\*) In case of ESP32-S2/ESP32-S3, hardware allows encryption of external
> SPIRAM contents provided hardware flash encryption feature is enabled.
> In that case, using external SPIRAM allocation strategy is also safe choice
> from security perspective.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Internal DRAM (CONFIG_BLE_MESH_MEM_ALLOC_MODE_INTERNAL)
> > * External SPIRAM (CONFIG_BLE_MESH_MEM_ALLOC_MODE_EXTERNAL)
> > * Default alloc mode (CONFIG_BLE_MESH_MEM_ALLOC_MODE_DEFAULT)
> > 
> > 
> > Enable this option to use the default memory allocation strategy when
> > external SPIRAM is enabled. See the SPIRAM options for more details.
> > * Internal IRAM (CONFIG_BLE_MESH_MEM_ALLOC_MODE_IRAM_8BIT)
> > 
> > 
> > Allows to use IRAM memory region as 8bit accessible region. Every
> > unaligned (8bit or 16bit) access will result in an exception and
> > incur penalty of certain clock cycles per unaligned read/write.




##### CONFIG_BLE_MESH_FREERTOS_STATIC_ALLOC



> Enable FreeRTOS static allocation
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to use FreeRTOS static allocation APIs for BLE Mesh,
> which provides the ability to use different dynamic memory (i.e. SPIRAM
> or IRAM) for FreeRTOS objects.
> If this option is disabled, the FreeRTOS static allocation APIs will not
> be used, and internal DRAM will be allocated for FreeRTOS objects.
> 
> 
> 
> Default value:* No (disabled) if ESP32_IRAM_AS_8BIT_ACCESSIBLE_MEMORY && CONFIG_BLE_MESH



###### CONFIG_BLE_MESH_FREERTOS_STATIC_ALLOC_MODE



> Memory allocation for FreeRTOS objects
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_FREERTOS_STATIC_ALLOC
> 
> 
> Choose the memory to be used for FreeRTOS objects.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * External SPIRAM (CONFIG_BLE_MESH_FREERTOS_STATIC_ALLOC_EXTERNAL)
> > 
> > 
> > If enabled, BLE Mesh allocates dynamic memory from external SPIRAM for
> > FreeRTOS objects, i.e. mutex, queue, and task stack. External SPIRAM
> > can only be used for task stack when SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY
> > is enabled. See the SPIRAM options for more details.
> > * Internal IRAM (CONFIG_BLE_MESH_FREERTOS_STATIC_ALLOC_IRAM_8BIT)
> > 
> > 
> > If enabled, BLE Mesh allocates dynamic memory from internal IRAM for
> > FreeRTOS objects, i.e. mutex, queue. Note: IRAM region cannot be used
> > as task stack.





##### CONFIG_BLE_MESH_DEINIT



> Support de-initialize BLE Mesh stack
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> If enabled, users can use the function esp_ble_mesh_deinit() to de-initialize
> the whole BLE Mesh stack.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




##### BLE Mesh and BLE coexistence support


Contains:


* CONFIG_BLE_MESH_SUPPORT_BLE_SCAN
* CONFIG_BLE_MESH_SUPPORT_BLE_ADV



###### CONFIG_BLE_MESH_SUPPORT_BLE_ADV



> Support sending normal BLE advertising packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh and BLE coexistence support
> 
> 
> When selected, users can send normal BLE advertising packets
> with specific API.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_BLE_ADV_BUF_COUNT



> Number of advertising buffers for BLE advertising packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh and BLE coexistence support > CONFIG_BLE_MESH_SUPPORT_BLE_ADV
> 
> 
> Number of advertising buffers for BLE packets available.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BLE_MESH_SUPPORT_BLE_ADV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH_SUPPORT_BLE_ADV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_SUPPORT_BLE_SCAN



> Support scanning normal BLE advertising packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh and BLE coexistence support
> 
> 
> When selected, users can register a callback and receive normal BLE
> advertising packets in the application layer.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_FAST_PROV



> Enable BLE Mesh Fast Provisioning
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to allow BLE Mesh fast provisioning solution to be used.
> When there are multiple unprovisioned devices around, fast provisioning can
> greatly reduce the time consumption of the whole provisioning process.
> When this option is enabled, and after an unprovisioned device is provisioned
> into a node successfully, it can be changed to a temporary Provisioner.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_NODE



> Support for BLE Mesh Node
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable the device to be provisioned into a node. This option should be
> enabled when an unprovisioned device is going to be provisioned into a
> node and communicate with other nodes in the BLE Mesh network.




##### CONFIG_BLE_MESH_PROVISIONER



> Support for BLE Mesh Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable the device to be a Provisioner. The option should be enabled when
> a device is going to act as a Provisioner and provision unprovisioned
> devices into the BLE Mesh network.



###### CONFIG_BLE_MESH_WAIT_FOR_PROV_MAX_DEV_NUM



> Maximum number of unprovisioned devices that can be added to device queue
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER
> 
> 
> This option specifies how many unprovisioned devices can be added to device
> queue for provisioning. Users can use this option to define the size of the
> queue in the bottom layer which is used to store unprovisioned device
> information (e.g. Device UUID, address).
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 10 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MAX_PROV_NODES



> Maximum number of devices that can be provisioned by Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER
> 
> 
> This option specifies how many devices can be provisioned by a Provisioner.
> This value indicates the maximum number of unprovisioned devices which can be
> provisioned by a Provisioner. For instance, if the value is 6, it means the
> Provisioner can provision up to 6 unprovisioned devices.
> Theoretically a Provisioner without the limitation of its memory can provision
> up to 32766 unprovisioned devices, here we limit the maximum number to 100
> just to limit the memory used by a Provisioner. The bigger the value is, the
> more memory it will cost by a Provisioner to store the information of nodes.
> 
> 
> 
> Range:* from 1 to 1000 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 10 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PBA_SAME_TIME



> Maximum number of PB-ADV running at the same time by Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER
> 
> 
> This option specifies how many devices can be provisioned at the same time
> using PB-ADV. For examples, if the value is 2, it means a Provisioner can
> provision two unprovisioned devices with PB-ADV at the same time.
> 
> 
> 
> Range:* from 1 to 10 if CONFIG_BLE_MESH_PB_ADV && CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_PB_ADV && CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PBG_SAME_TIME



> Maximum number of PB-GATT running at the same time by Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER
> 
> 
> This option specifies how many devices can be provisioned at the same
> time using PB-GATT. For example, if the value is 2, it means a Provisioner
> can provision two unprovisioned devices with PB-GATT at the same time.
> 
> 
> 
> Range:* from 1 to 5 if CONFIG_BLE_MESH_PB_GATT && CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 1 if CONFIG_BLE_MESH_PB_GATT && CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PROVISIONER_SUBNET_COUNT



> Maximum number of mesh subnets that can be created by Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER
> 
> 
> This option specifies how many subnets per network a Provisioner can create.
> Indeed, this value decides the number of network keys which can be added by a Provisioner.
> 
> 
> 
> Range:* from 1 to 4096 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PROVISIONER_APP_KEY_COUNT



> Maximum number of application keys that can be owned by Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER
> 
> 
> This option specifies how many application keys the Provisioner can have.
> Indeed, this value decides the number of the application keys which can be added by a Provisioner.
> 
> 
> 
> Range:* from 1 to 4096 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PROVISIONER_RECV_HB



> Support receiving Heartbeat messages
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER
> 
> 
> When this option is enabled, Provisioner can call specific functions to enable
> or disable receiving Heartbeat messages and notify them to the application layer.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PROVISIONER_RECV_HB_FILTER_SIZE



> Maximum number of filter entries for receiving Heartbeat messages
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROVISIONER > CONFIG_BLE_MESH_PROVISIONER_RECV_HB
> 
> 
> This option specifies how many heartbeat filter entries Provisioner supports.
> The heartbeat filter (acceptlist or rejectlist) entries are used to store a
> list of SRC and DST which can be used to decide if a heartbeat message will
> be processed and notified to the application layer by Provisioner.
> Note: The filter is an empty rejectlist by default.
> 
> 
> 
> Range:* from 1 to 1000 if CONFIG_BLE_MESH_PROVISIONER_RECV_HB && CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH_PROVISIONER_RECV_HB && CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_PROV



> BLE Mesh Provisioning support
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to support BLE Mesh Provisioning functionality. For
> BLE Mesh, this option should be always enabled.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH



###### CONFIG_BLE_MESH_PROV_EPA



> BLE Mesh enhanced provisioning authentication
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROV
> 
> 
> Enable this option to support BLE Mesh enhanced provisioning authentication
> functionality. This option can increase the security level of provisioning.
> It is recommended to enable this option.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH_PROV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_CERT_BASED_PROV



> Support Certificate-based provisioning
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROV
> 
> 
> Enable this option to support BLE Mesh Certificate-Based Provisioning.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_PROV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_RECORD_FRAG_MAX_SIZE



> Maximum size of the provisioning record fragment that Provisioner can receive
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PROV > CONFIG_BLE_MESH_CERT_BASED_PROV
> 
> 
> This option sets the maximum size of the provisioning record fragment that the
> Provisioner can receive. The range depends on provisioning bearer.
> 
> 
> 
> Range:* from 1 to 57 if CONFIG_BLE_MESH_CERT_BASED_PROV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 56 if CONFIG_BLE_MESH_CERT_BASED_PROV && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_PB_ADV



> Provisioning support using the advertising bearer (PB-ADV)
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to allow the device to be provisioned over the
> advertising bearer. This option should be enabled if PB-ADV is
> going to be used during provisioning procedure.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH



###### CONFIG_BLE_MESH_UNPROVISIONED_BEACON_INTERVAL



> Interval between two consecutive Unprovisioned Device Beacon
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_PB_ADV
> 
> 
> This option specifies the interval of sending two consecutive unprovisioned
> device beacon, users can use this option to change the frequency of sending
> unprovisioned device beacon. For example, if the value is 5, it means the
> unprovisioned device beacon will send every 5 seconds. When the option of
> BLE_MESH_FAST_PROV is selected, the value is better to be 3 seconds, or less.
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_BLE_MESH_NODE && CONFIG_BLE_MESH_PB_ADV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 5 if CONFIG_BLE_MESH_NODE && CONFIG_BLE_MESH_PB_ADV && CONFIG_BLE_MESH
> * 3 if CONFIG_BLE_MESH_FAST_PROV && CONFIG_BLE_MESH_NODE && CONFIG_BLE_MESH_PB_ADV && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_PB_GATT



> Provisioning support using GATT (PB-GATT)
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to allow the device to be provisioned over GATT.
> This option should be enabled if PB-GATT is going to be used during
> provisioning procedure.
> 
> 
> # Virtual option enabled whenever any Proxy protocol is needed




##### CONFIG_BLE_MESH_PROXY



> BLE Mesh Proxy protocol support
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to support BLE Mesh Proxy protocol used by PB-GATT
> and other proxy pdu transmission.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_GATT_PROXY_SERVER



> BLE Mesh GATT Proxy Server
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option enables support for Mesh GATT Proxy Service, i.e. the
> ability to act as a proxy between a Mesh GATT Client and a Mesh network.
> This option should be enabled if a node is going to be a Proxy Server.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH_NODE && CONFIG_BLE_MESH



###### CONFIG_BLE_MESH_NODE_ID_TIMEOUT



> Node Identity advertising timeout
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_GATT_PROXY_SERVER
> 
> 
> This option determines for how long the local node advertises using
> Node Identity. The given value is in seconds. The specification limits
> this to 60 seconds and lists it as the recommended value as well.
> So leaving the default value is the safest option.
> When an unprovisioned device is provisioned successfully and becomes a
> node, it will start to advertise using Node Identity during the time
> set by this option. And after that, Network ID will be advertised.
> 
> 
> 
> Range:* from 1 to 60 if CONFIG_BLE_MESH_GATT_PROXY_SERVER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 60 if CONFIG_BLE_MESH_GATT_PROXY_SERVER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PROXY_FILTER_SIZE



> Maximum number of filter entries per Proxy Client
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_GATT_PROXY_SERVER
> 
> 
> This option specifies how many Proxy Filter entries the local node supports.
> The entries of Proxy filter (whitelist or blacklist) are used to store a
> list of addresses which can be used to decide which messages will be forwarded
> to the Proxy Client by the Proxy Server.
> 
> 
> 
> Range:* from 1 to 32767 if CONFIG_BLE_MESH_GATT_PROXY_SERVER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 4 if CONFIG_BLE_MESH_GATT_PROXY_SERVER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PROXY_PRIVACY



> Support Proxy Privacy
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_GATT_PROXY_SERVER
> 
> 
> The Proxy Privacy parameter controls the privacy of the Proxy Server
> over the connection. The value of the Proxy Privacy parameter is
> controlled by the type of proxy connection, which is dependent on the
> bearer used by the proxy connection.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH_PRB_SRV && CONFIG_BLE_MESH_GATT_PROXY_SERVER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PROXY_SOLIC_PDU_RX



> Support receiving Proxy Solicitation PDU
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_GATT_PROXY_SERVER
> 
> 
> Enable this option to support receiving Proxy Solicitation PDU.




###### CONFIG_BLE_MESH_PROXY_SOLIC_RX_CRPL



> Maximum capacity of solicitation replay protection list
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_GATT_PROXY_SERVER > CONFIG_BLE_MESH_PROXY_SOLIC_PDU_RX
> 
> 
> This option specifies the maximum capacity of the solicitation replay
> protection list. The solicitation replay protection list is used to
> reject Solicitation PDUs that were already processed by a node, which
> will store the solicitation src and solicitation sequence number of
> the received Solicitation PDU message.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BLE_MESH_PROXY_SOLIC_PDU_RX && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_PROXY_SOLIC_PDU_RX && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_GATT_PROXY_CLIENT



> BLE Mesh GATT Proxy Client
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option enables support for Mesh GATT Proxy Client. The Proxy Client
> can use the GATT bearer to send mesh messages to a node that supports the
> advertising bearer.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH



###### CONFIG_BLE_MESH_PROXY_SOLIC_PDU_TX



> Support sending Proxy Solicitation PDU
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_GATT_PROXY_CLIENT
> 
> 
> Enable this option to support sending Proxy Solicitation PDU.




###### CONFIG_BLE_MESH_PROXY_SOLIC_TX_SRC_COUNT



> Maximum number of SSRC that can be used by Proxy Client
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_GATT_PROXY_CLIENT > CONFIG_BLE_MESH_PROXY_SOLIC_PDU_TX
> 
> 
> This option specifies the maximum number of Solicitation Source (SSRC)
> that can be used by Proxy Client for sending a Solicitation PDU.
> A Proxy Client may use the primary address or any of the secondary
> addresses as the SSRC for a Solicitation PDU.
> So for a Proxy Client, it's better to choose the value based on its
> own element count.
> 
> 
> 
> Range:* from 1 to 16 if CONFIG_BLE_MESH_PROXY_SOLIC_PDU_TX && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_PROXY_SOLIC_PDU_TX && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_SETTINGS



> Store BLE Mesh configuration persistently
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> When selected, the BLE Mesh stack will take care of storing/restoring the BLE
> Mesh configuration persistently in flash.
> If the device is a BLE Mesh node, when this option is enabled, the configuration
> of the device will be stored persistently, including unicast address, NetKey,
> AppKey, etc.
> And if the device is a BLE Mesh Provisioner, the information of the device will
> be stored persistently, including the information of provisioned nodes, NetKey,
> AppKey, etc.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH



###### CONFIG_BLE_MESH_STORE_TIMEOUT



> Delay (in seconds) before storing anything persistently
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS
> 
> 
> This value defines in seconds how soon any pending changes are actually
> written into persistent storage (flash) after a change occurs.
> The option allows nodes to delay a certain period of time to save proper
> information to flash. The default value is 0, which means information
> will be stored immediately once there are updates.
> 
> 
> 
> Range:* from 0 to 1000000 if CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 0 if CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_SEQ_STORE_RATE



> How often the sequence number gets updated in storage
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS
> 
> 
> This value defines how often the local sequence number gets updated in
> persistent storage (i.e. flash). e.g. a value of 100 means that the
> sequence number will be stored to flash on every 100th increment.
> If the node sends messages very frequently a higher value makes more
> sense, whereas if the node sends infrequently a value as low as 0
> (update storage for every increment) can make sense. When the stack
> gets initialized it will add sequence number to the last stored one,
> so that it starts off with a value that's guaranteed to be larger than
> the last one used before power off.
> 
> 
> 
> Range:* from 0 to 1000000 if CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 0 if CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_RPL_STORE_TIMEOUT



> Minimum frequency that the RPL gets updated in storage
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS
> 
> 
> This value defines in seconds how soon the RPL (Replay Protection List)
> gets written to persistent storage after a change occurs. If the node
> receives messages frequently, then a large value is recommended. If the
> node receives messages rarely, then the value can be as low as 0 (which
> means the RPL is written into the storage immediately).
> Note that if the node operates in a security-sensitive case, and there is
> a risk of sudden power-off, then a value of 0 is strongly recommended.
> Otherwise, a power loss before RPL being written into the storage may
> introduce message replay attacks and system security will be in a
> vulnerable state.
> 
> 
> 
> Range:* from 0 to 1000000 if CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 0 if CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_SETTINGS_BACKWARD_COMPATIBILITY



> A specific option for settings backward compatibility
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS
> 
> 
> This option is created to solve the issue of failure in recovering
> node information after mesh stack updates. In the old version mesh
> stack, there is no key of "mesh/role" in nvs. In the new version
> mesh stack, key of "mesh/role" is added in nvs, recovering node
> information needs to check "mesh/role" key in nvs and implements
> selective recovery of mesh node information. Therefore, there may
> be failure in recovering node information during node restarting
> after OTA.
> 
> 
> The new version mesh stack adds the option of "mesh/role" because
> we have added the support of storing Provisioner information, while
> the old version only supports storing node information.
> 
> 
> If users are updating their nodes from old version to new version,
> we recommend enabling this option, so that system could set the flag
> in advance before recovering node information and make sure the node
> information recovering could work as expected.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_NODE && CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_SPECIFIC_PARTITION



> Use a specific NVS partition for BLE Mesh
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS
> 
> 
> When selected, the mesh stack will use a specified NVS partition instead of
> default NVS partition. Note that the specified partition must be registered
> with NVS using nvs_flash_init_partition() API, and the partition must exists
> in the csv file.
> When Provisioner needs to store a large amount of nodes' information in the
> flash (e.g. more than 20), this option is recommended to be enabled.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PARTITION_NAME



> Name of the NVS partition for BLE Mesh
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS > CONFIG_BLE_MESH_SPECIFIC_PARTITION
> 
> 
> This value defines the name of the specified NVS partition used by the
> mesh stack.
> 
> 
> 
> Default value:* "ble_mesh" if CONFIG_BLE_MESH_SPECIFIC_PARTITION && CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_USE_MULTIPLE_NAMESPACE



> Support using multiple NVS namespaces by Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS
> 
> 
> When selected, Provisioner can use different NVS namespaces to store
> different instances of mesh information.
> For example, if in the first room, Provisioner uses NetKey A, AppKey
> A and provisions three devices, these information will be treated as
> mesh information instance A. When the Provisioner moves to the second
> room, it uses NetKey B, AppKey B and provisions two devices, then the
> information will be treated as mesh information instance B.
> Here instance A and instance B will be stored in different namespaces.
> With this option enabled, Provisioner needs to use specific functions
> to open the corresponding NVS namespace, restore the mesh information,
> release the mesh information or erase the mesh information.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_PROVISIONER && CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MAX_NVS_NAMESPACE



> Maximum number of NVS namespaces
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_SETTINGS > CONFIG_BLE_MESH_USE_MULTIPLE_NAMESPACE
> 
> 
> This option specifies the maximum NVS namespaces supported by Provisioner.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BLE_MESH_USE_MULTIPLE_NAMESPACE && CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_USE_MULTIPLE_NAMESPACE && CONFIG_BLE_MESH_SETTINGS && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_SUBNET_COUNT



> Maximum number of mesh subnets per network
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option specifies how many subnets a Mesh network can have at the same time.
> Indeed, this value decides the number of the network keys which can be owned by a node.
> 
> 
> 
> Range:* from 1 to 4096 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_APP_KEY_COUNT



> Maximum number of application keys per network
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option specifies how many application keys the device can store per network.
> Indeed, this value decides the number of the application keys which can be owned by a node.
> 
> 
> 
> Range:* from 1 to 4096 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_MODEL_KEY_COUNT



> Maximum number of application keys per model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option specifies the maximum number of application keys to which each model
> can be bound.
> 
> 
> 
> Range:* from 1 to 4096 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_MODEL_GROUP_COUNT



> Maximum number of group address subscriptions per model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option specifies the maximum number of addresses to which each model can
> be subscribed.
> 
> 
> 
> Range:* from 1 to 4096 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_LABEL_COUNT



> Maximum number of Label UUIDs used for Virtual Addresses
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option specifies how many Label UUIDs can be stored.
> Indeed, this value decides the number of the Virtual Addresses can be supported by a node.
> 
> 
> 
> Range:* from 0 to 4096 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_CRPL



> Maximum capacity of the replay protection list
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option specifies the maximum capacity of the replay protection list.
> It is similar to Network message cache size, but has a different purpose.
> The replay protection list is used to prevent a node from replay attack,
> which will store the source address and sequence number of the received
> mesh messages.
> For Provisioner, the replay protection list size should not be smaller than
> the maximum number of nodes whose information can be stored. And the element
> number of each node should also be taken into consideration. For example, if
> Provisioner can provision up to 20 nodes and each node contains two elements,
> then the replay protection list size of Provisioner should be at least 40.
> 
> 
> 
> Range:* from 2 to 65535 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 10 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_NOT_RELAY_REPLAY_MSG



> Not relay replayed messages in a mesh network
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> There may be many expired messages in a complex mesh network that would be
> considered replayed messages.
> Enable this option will refuse to relay such messages, which could help to
> reduce invalid packets in the mesh network.
> However, it should be noted that enabling this option may result in packet
> loss in certain environments.
> Therefore, users need to decide whether to enable this option according to
> the actual usage situation.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_EXPERIMENTAL && CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_MSG_CACHE_SIZE



> Network message cache size
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Number of messages that are cached for the network. This helps prevent
> unnecessary decryption operations and unnecessary relays. This option
> is similar to Replay protection list, but has a different purpose.
> A node is not required to cache the entire Network PDU and may cache
> only part of it for tracking, such as values for SRC/SEQ or others.
> 
> 
> 
> Range:* from 2 to 65535 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 10 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_ADV_BUF_COUNT



> Number of advertising buffers
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Number of advertising buffers available. The transport layer reserves
> ADV_BUF_COUNT - 3 buffers for outgoing segments. The maximum outgoing
> SDU size is 12 times this value (out of which 4 or 8 bytes are used
> for the Transport Layer MIC). For example, 5 segments means the maximum
> SDU size is 60 bytes, which leaves 56 bytes for application layer data
> using a 4-byte MIC, or 52 bytes using an 8-byte MIC.
> 
> 
> 
> Range:* from 6 to 256 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 60 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_IVU_DIVIDER



> Divider for IV Update state refresh timer
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> When the IV Update state enters Normal operation or IV Update
> in Progress, we need to keep track of how many hours has passed
> in the state, since the specification requires us to remain in
> the state at least for 96 hours (Update in Progress has an
> additional upper limit of 144 hours).
> 
> 
> In order to fulfill the above requirement, even if the node might
> be powered off once in a while, we need to store persistently
> how many hours the node has been in the state. This doesn't
> necessarily need to happen every hour (thanks to the flexible
> duration range). The exact cadence will depend a lot on the
> ways that the node will be used and what kind of power source it
> has.
> 
> 
> Since there is no single optimal answer, this configuration
> option allows specifying a divider, i.e. how many intervals
> the 96 hour minimum gets split into. After each interval the
> duration that the node has been in the current state gets
> stored to flash. E.g. the default value of 4 means that the
> state is saved every 24 hours (96 / 4).
> 
> 
> 
> Range:* from 2 to 96 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 4 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_IVU_RECOVERY_IVI



> Recovery the IV index when the latest whole IV update procedure is missed
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> According to Section 3.10.5 of Mesh Specification v1.0.1.
> If a node in Normal Operation receives a Secure Network beacon with an IV index
> equal to the last known IV index+1 and the IV Update Flag set to 0, the node may
> update its IV without going to the IV Update in Progress state, or it may initiate
> an IV Index Recovery procedure (Section 3.10.6), or it may ignore the Secure
> Network beacon. The node makes the choice depending on the time since last IV
> update and the likelihood that the node has missed the Secure Network beacons
> with the IV update Flag.
> When the above situation is encountered, this option can be used to decide whether
> to perform the IV index recovery procedure.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_SAR_ENHANCEMENT



> Segmentation and reassembly enhancement
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to use the enhanced segmentation and reassembly
> mechanism introduced in Bluetooth Mesh Protocol 1.1.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_TX_SEG_MSG_COUNT



> Maximum number of simultaneous outgoing segmented messages
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Maximum number of simultaneous outgoing multi-segment and/or reliable messages.
> The default value is 1, which means the device can only send one segmented
> message at a time. And if another segmented message is going to be sent, it
> should wait for the completion of the previous one.
> If users are going to send multiple segmented messages at the same time, this
> value should be configured properly.
> 
> 
> 
> Range:* from 1 to if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 1 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_RX_SEG_MSG_COUNT



> Maximum number of simultaneous incoming segmented messages
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Maximum number of simultaneous incoming multi-segment and/or reliable messages.
> The default value is 1, which means the device can only receive one segmented
> message at a time. And if another segmented message is going to be received,
> it should wait for the completion of the previous one.
> If users are going to receive multiple segmented messages at the same time, this
> value should be configured properly.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 1 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_RX_SDU_MAX



> Maximum incoming Upper Transport Access PDU length
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Maximum incoming Upper Transport Access PDU length. Leave this to the default
> value, unless you really need to optimize memory usage.
> 
> 
> 
> Range:* from 36 to 384 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 384 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_TX_SEG_MAX



> Maximum number of segments in outgoing messages
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Maximum number of segments supported for outgoing messages.
> This value should typically be fine-tuned based on what
> models the local node supports, i.e. what's the largest
> message payload that the node needs to be able to send.
> This value affects memory and call stack consumption, which
> is why the default is lower than the maximum that the
> specification would allow (32 segments).
> 
> 
> The maximum outgoing SDU size is 12 times this number (out of
> which 4 or 8 bytes is used for the Transport Layer MIC). For
> example, 5 segments means the maximum SDU size is 60 bytes,
> which leaves 56 bytes for application layer data using a
> 4-byte MIC and 52 bytes using an 8-byte MIC.
> 
> 
> Be sure to specify a sufficient number of advertising buffers
> when setting this option to a higher value. There must be at
> least three more advertising buffers (BLE_MESH_ADV_BUF_COUNT)
> as there are outgoing segments.
> 
> 
> 
> Range:* from 2 to 32 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 32 if CONFIG_BLE_MESH




##### CONFIG_BLE_MESH_RELAY



> Relay support
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Support for acting as a Mesh Relay Node. Enabling this option will allow
> a node to support the Relay feature, and the Relay feature can still
> be enabled or disabled by proper configuration messages. Disabling this
> option will let a node not support the Relay feature.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH_NODE && CONFIG_BLE_MESH



###### CONFIG_BLE_MESH_RELAY_ADV_BUF



> Use separate advertising buffers for relay packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_RELAY
> 
> 
> When selected, self-send packets will be put in a high-priority
> queue and relay packets will be put in a low-priority queue.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_RELAY && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_RELAY_ADV_BUF_COUNT



> Number of advertising buffers for relay packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_RELAY > CONFIG_BLE_MESH_RELAY_ADV_BUF
> 
> 
> Number of advertising buffers for relay packets available.
> 
> 
> 
> Range:* from 6 to 256 if CONFIG_BLE_MESH_RELAY_ADV_BUF && CONFIG_BLE_MESH_RELAY && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 60 if CONFIG_BLE_MESH_RELAY_ADV_BUF && CONFIG_BLE_MESH_RELAY && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_LOW_POWER



> Support for Low Power features
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to operate as a Low Power Node. If low power consumption
> is required by a node, this option should be enabled. And once the node
> enters the mesh network, it will try to find a Friend node and establish a
> friendship.



###### CONFIG_BLE_MESH_LPN_ESTABLISHMENT



> Perform Friendship establishment using low power
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> Perform the Friendship establishment using low power with the help of a
> reduced scan duty cycle. The downside of this is that the node may miss
> out on messages intended for it until it has successfully set up Friendship
> with a Friend node.
> When this option is enabled, the node will stop scanning for a period of
> time after a Friend Request or Friend Poll is sent, so as to reduce more
> power consumption.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_AUTO



> Automatically start looking for Friend nodes once provisioned
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> Once provisioned, automatically enable LPN functionality and start looking
> for Friend nodes. If this option is disabled LPN mode needs to be manually
> enabled by calling bt_mesh_lpn_set(true).
> When an unprovisioned device is provisioned successfully and becomes a node,
> enabling this option will trigger the node starts to send Friend Request at
> a certain period until it finds a proper Friend node.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_AUTO_TIMEOUT



> Time from last received message before going to LPN mode
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER > CONFIG_BLE_MESH_LPN_AUTO
> 
> 
> Time in seconds from the last received message, that the node waits out
> before starting to look for Friend nodes.
> 
> 
> 
> Range:* from 0 to 3600 if CONFIG_BLE_MESH_LPN_AUTO && CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 15 if CONFIG_BLE_MESH_LPN_AUTO && CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_RETRY_TIMEOUT



> Retry timeout for Friend requests
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> Time in seconds between Friend Requests, if a previous Friend Request did
> not yield any acceptable Friend Offers.
> 
> 
> 
> Range:* from 1 to 3600 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 6 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_RSSI_FACTOR



> RSSIFactor, used in Friend Offer Delay calculation
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> The contribution of the RSSI, measured by the Friend node, used in Friend
> Offer Delay calculations. 0 = 1, 1 = 1.5, 2 = 2, 3 = 2.5.
> RSSIFactor, one of the parameters carried by Friend Request sent by Low Power
> node, which is used to calculate the Friend Offer Delay.
> 
> 
> 
> Range:* from 0 to 3 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 0 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_RECV_WIN_FACTOR



> ReceiveWindowFactor, used in Friend Offer Delay calculation
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> The contribution of the supported Receive Window used in Friend Offer
> Delay calculations. 0 = 1, 1 = 1.5, 2 = 2, 3 = 2.5.
> ReceiveWindowFactor, one of the parameters carried by Friend Request sent by
> Low Power node, which is used to calculate the Friend Offer Delay.
> 
> 
> 
> Range:* from 0 to 3 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 0 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_MIN_QUEUE_SIZE



> Minimum size of the acceptable friend queue (MinQueueSizeLog)
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> The MinQueueSizeLog field is defined as log_2(N), where N is the minimum
> number of maximum size Lower Transport PDUs that the Friend node can store
> in its Friend Queue. As an example, MinQueueSizeLog value 1 gives N = 2,
> and value 7 gives N = 128.
> 
> 
> 
> Range:* from 1 to 7 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 1 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_RECV_DELAY



> Receive delay requested by the local node
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> The ReceiveDelay is the time between the Low Power node sending a
> request and listening for a response. This delay allows the Friend
> node time to prepare the response. The value is in units of milliseconds.
> 
> 
> 
> Range:* from 10 to 255 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 100 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_POLL_TIMEOUT



> The value of the PollTimeout timer
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> PollTimeout timer is used to measure time between two consecutive
> requests sent by a Low Power node. If no requests are received
> the Friend node before the PollTimeout timer expires, then the
> friendship is considered terminated. The value is in units of 100
> milliseconds, so e.g. a value of 300 means 30 seconds.
> The smaller the value, the faster the Low Power node tries to get
> messages from corresponding Friend node and vice versa.
> 
> 
> 
> Range:* from 10 to 244735 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 300 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_INIT_POLL_TIMEOUT



> The starting value of the PollTimeout timer
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> The initial value of the PollTimeout timer when Friendship is to be
> established for the first time. After this, the timeout gradually
> grows toward the actual PollTimeout, doubling in value for each iteration.
> The value is in units of 100 milliseconds, so e.g. a value of 300 means
> 30 seconds.
> 
> 
> 
> Range:* from 10 to if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_SCAN_LATENCY



> Latency for enabling scanning
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> Latency (in milliseconds) is the time it takes to enable scanning. In
> practice, it means how much time in advance of the Receive Window, the
> request to enable scanning is made.
> 
> 
> 
> Range:* from 0 to 50 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 10 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_GROUPS



> Number of groups the LPN can subscribe to
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> Maximum number of groups to which the LPN can subscribe.
> 
> 
> 
> Range:* from 0 to 16384 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 8 if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LPN_SUB_ALL_NODES_ADDR



> Automatically subscribe all nodes address
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_LOW_POWER
> 
> 
> Automatically subscribe all nodes address when friendship
> established.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_LOW_POWER && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_FRIEND



> Support for Friend feature
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Enable this option to be able to act as a Friend Node.



###### CONFIG_BLE_MESH_FRIEND_RECV_WIN



> Friend Receive Window
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_FRIEND
> 
> 
> Receive Window in milliseconds supported by the Friend node.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 255 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_FRIEND_QUEUE_SIZE



> Minimum number of buffers supported per Friend Queue
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_FRIEND
> 
> 
> Minimum number of buffers available to be stored for each local Friend Queue.
> This option decides the size of each buffer which can be used by a Friend node
> to store messages for each Low Power node.
> 
> 
> 
> Range:* from 2 to 65536 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 16 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_FRIEND_SUB_LIST_SIZE



> Friend Subscription List Size
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_FRIEND
> 
> 
> Size of the Subscription List that can be supported by a Friend node for a
> Low Power node. And Low Power node can send Friend Subscription List Add or
> Friend Subscription List Remove messages to the Friend node to add or remove
> subscription addresses.
> 
> 
> 
> Range:* from 0 to 1023 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 3 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_FRIEND_LPN_COUNT



> Number of supported LPN nodes
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_FRIEND
> 
> 
> Number of Low Power Nodes with which a Friend can have Friendship simultaneously.
> A Friend node can have friendship with multiple Low Power nodes at the same time,
> while a Low Power node can only establish friendship with only one Friend node at
> the same time.
> 
> 
> 
> Range:* from 1 to 1000 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_FRIEND_SEG_RX



> Number of incomplete segment lists per LPN
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > CONFIG_BLE_MESH_FRIEND
> 
> 
> Number of incomplete segment lists tracked for each Friends' LPN.
> In other words, this determines from how many elements can segmented
> messages destined for the Friend queue be received simultaneously.
> 
> 
> 
> Range:* from 1 to 1000 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 1 if CONFIG_BLE_MESH_FRIEND && CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_NO_LOG



> Disable BLE Mesh debug logs (minimize bin size)
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Select this to save the BLE Mesh related rodata code size. Enabling this option
> will disable the output of BLE Mesh debug log.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH && CONFIG_BLE_MESH




##### BLE Mesh STACK DEBUG LOG LEVEL


Contains:


* CONFIG_BLE_MESH_STACK_TRACE_LEVEL



###### CONFIG_BLE_MESH_STACK_TRACE_LEVEL



> BLE_MESH_STACK
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh STACK DEBUG LOG LEVEL
> 
> 
> Define BLE Mesh trace level for BLE Mesh stack.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BLE_MESH_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BLE_MESH_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BLE_MESH_TRACE_LEVEL_WARNING)
> > * INFO (CONFIG_BLE_MESH_TRACE_LEVEL_INFO)
> > * DEBUG (CONFIG_BLE_MESH_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BLE_MESH_TRACE_LEVEL_VERBOSE)





##### BLE Mesh NET BUF DEBUG LOG LEVEL


Contains:


* CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL



###### CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL



> BLE_MESH_NET_BUF
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh NET BUF DEBUG LOG LEVEL
> 
> 
> Define BLE Mesh trace level for BLE Mesh net buffer.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * NONE (CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL_NONE)
> > * ERROR (CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL_ERROR)
> > * WARNING (CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL_WARNING)
> > * INFO (CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL_INFO)
> > * DEBUG (CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL_DEBUG)
> > * VERBOSE (CONFIG_BLE_MESH_NET_BUF_TRACE_LEVEL_VERBOSE)





##### CONFIG_BLE_MESH_CLIENT_MSG_TIMEOUT



> Timeout(ms) for client message response
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Timeout value used by the node to get response of the acknowledged
> message which is sent by the client model.
> This value indicates the maximum time that a client model waits for
> the response of the sent acknowledged messages. If a client model
> uses 0 as the timeout value when sending acknowledged messages, then
> the default value will be used which is four seconds.
> 
> 
> 
> Range:* from 100 to 1200000 if CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 4000 if CONFIG_BLE_MESH




##### Support for BLE Mesh Foundation models


Contains:


* CONFIG_BLE_MESH_BRC_CLI
* CONFIG_BLE_MESH_BRC_SRV
* CONFIG_BLE_MESH_CFG_CLI
* CONFIG_BLE_MESH_DF_CLI
* CONFIG_BLE_MESH_DF_SRV
* CONFIG_BLE_MESH_HEALTH_CLI
* CONFIG_BLE_MESH_HEALTH_SRV
* CONFIG_BLE_MESH_LCD_CLI
* CONFIG_BLE_MESH_LCD_SRV
* CONFIG_BLE_MESH_PRB_CLI
* CONFIG_BLE_MESH_PRB_SRV
* CONFIG_BLE_MESH_ODP_CLI
* CONFIG_BLE_MESH_ODP_SRV
* CONFIG_BLE_MESH_AGG_CLI
* CONFIG_BLE_MESH_AGG_SRV
* CONFIG_BLE_MESH_RPR_CLI
* CONFIG_BLE_MESH_RPR_SRV
* CONFIG_BLE_MESH_SAR_CLI
* CONFIG_BLE_MESH_SAR_SRV
* CONFIG_BLE_MESH_SRPL_CLI
* CONFIG_BLE_MESH_SRPL_SRV
* CONFIG_BLE_MESH_COMP_DATA_1
* CONFIG_BLE_MESH_COMP_DATA_128
* CONFIG_BLE_MESH_MODELS_METADATA_0



###### CONFIG_BLE_MESH_CFG_CLI



> Configuration Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Configuration Client model.




###### CONFIG_BLE_MESH_HEALTH_CLI



> Health Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Health Client model.




###### CONFIG_BLE_MESH_HEALTH_SRV



> Health Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Health Server model.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_BRC_CLI



> Bridge Configuration Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Bridge Configuration Client model.




###### CONFIG_BLE_MESH_BRC_SRV



> Bridge Configuration Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Bridge Configuration Server model.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MAX_BRIDGING_TABLE_ENTRY_COUNT



> Maximum number of Bridging Table entries
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_BRC_SRV
> 
> 
> Maximum number of Bridging Table entries that the Bridge Configuration Server can support.
> 
> 
> 
> Range:* from 16 to 65535 if CONFIG_BLE_MESH_BRC_SRV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 16 if CONFIG_BLE_MESH_BRC_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_BRIDGE_CRPL



> Maximum capacity of bridge replay protection list
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_BRC_SRV
> 
> 
> This option specifies the maximum capacity of the bridge replay
> protection list. The bridge replay protection list is used to
> prevent a bridged subnet from replay attack, which will store the
> source address and sequence number of the received bridge messages.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BLE_MESH_BRC_SRV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 5 if CONFIG_BLE_MESH_BRC_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PRB_CLI



> Mesh Private Beacon Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Mesh Private Beacon Client model.




###### CONFIG_BLE_MESH_PRB_SRV



> Mesh Private Beacon Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Mesh Private Beacon Server model.




###### CONFIG_BLE_MESH_ODP_CLI



> On-Demand Private Proxy Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for On-Demand Private Proxy Client model.




###### CONFIG_BLE_MESH_ODP_SRV



> On-Demand Private Proxy Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for On-Demand Private Proxy Server model.




###### CONFIG_BLE_MESH_SRPL_CLI



> Solicitation PDU RPL Configuration Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Solicitation PDU RPL Configuration Client model.




###### CONFIG_BLE_MESH_SRPL_SRV



> Solicitation PDU RPL Configuration Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Solicitation PDU RPL Configuration Server model.
> Note:
> This option depends on the functionality of receiving Solicitation
> PDU. If the device doesn't support receiving Solicitation PDU, then
> there is no need to enable this server model.




###### CONFIG_BLE_MESH_AGG_CLI



> Opcodes Aggregator Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Opcodes Aggregator Client model.




###### CONFIG_BLE_MESH_AGG_SRV



> Opcodes Aggregator Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Opcodes Aggregator Server model.




###### CONFIG_BLE_MESH_SAR_CLI



> SAR Configuration Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for SAR Configuration Client model.




###### CONFIG_BLE_MESH_SAR_SRV



> SAR Configuration Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for SAR Configuration Server model.




###### CONFIG_BLE_MESH_COMP_DATA_1



> Support Composition Data Page 1
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Composition Data Page 1 contains information about the relationships
> among models.
> Each model either can be a root model or can extend other models.




###### CONFIG_BLE_MESH_COMP_DATA_128



> Support Composition Data Page 128
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Composition Data Page 128 is used to indicate the structure of
> elements, features, and models of a node after the successful
> execution of the Node Address Refresh procedure or the Node
> Composition Refresh procedure, or after the execution of the
> Node Removal procedure followed by the provisioning process.
> Composition Data Page 128 shall be present if the node supports
> the Remote Provisioning Server model; otherwise it is optional.




###### CONFIG_BLE_MESH_MODELS_METADATA_0



> Support Models Metadata Page 0
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> The Models Metadata state contains metadata of a node’s models.
> The Models Metadata state is composed of a number of pages of
> information.
> Models Metadata Page 0 shall be present if the node supports
> the Large Composition Data Server model.




###### CONFIG_BLE_MESH_MODELS_METADATA_128



> Support Models Metadata Page 128
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_MODELS_METADATA_0
> 
> 
> The Models Metadata state contains metadata of a node’s models.
> The Models Metadata state is composed of a number of pages of
> information.
> Models Metadata Page 128 contains metadata for the node’s models
> after the successful execution of the Node Address Refresh
> procedure or the Node Composition Refresh procedure, or after
> the execution of the Node Removal procedure followed by the
> provisioning process.
> Models Metadata Page 128 shall be present if the node supports
> the Remote Provisioning Server model and the node supports the
> Large Composition Data Server model.




###### CONFIG_BLE_MESH_LCD_CLI



> Large Composition Data Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Large Composition Data Client model.




###### CONFIG_BLE_MESH_LCD_SRV



> Large Composition Data Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Large Composition Data Server model.




###### CONFIG_BLE_MESH_RPR_CLI



> Remote Provisioning Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Remote Provisioning Client model




###### CONFIG_BLE_MESH_RPR_CLI_PROV_SAME_TIME



> Maximum number of PB-Remote running at the same time by Provisioner
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_RPR_CLI
> 
> 
> This option specifies how many devices can be provisioned at the same time
> using PB-REMOTE. For example, if the value is 2, it means a Provisioner can
> provision two unprovisioned devices with PB-REMOTE at the same time.
> 
> 
> 
> Range:* from 1 to 5 if CONFIG_BLE_MESH_RPR_CLI && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_RPR_CLI && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_RPR_SRV



> Remote Provisioning Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Remote Provisioning Server model




###### CONFIG_BLE_MESH_RPR_SRV_MAX_SCANNED_ITEMS



> Maximum number of device information can be scanned
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_RPR_SRV
> 
> 
> This option specifies how many device information can a Remote
> Provisioning Server store each time while scanning.
> 
> 
> 
> Range:* from 4 to 255 if CONFIG_BLE_MESH_RPR_SRV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 10 if CONFIG_BLE_MESH_RPR_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_RPR_SRV_ACTIVE_SCAN



> Support Active Scan for remote provisioning
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_RPR_SRV
> 
> 
> Enable this option to support Active Scan for remote provisioning.




###### CONFIG_BLE_MESH_RPR_SRV_MAX_EXT_SCAN



> Maximum number of extended scan procedures
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_RPR_SRV
> 
> 
> This option specifies how many extended scan procedures can be
> started by the Remote Provisioning Server.
> 
> 
> 
> Range:* from 1 to 10 if CONFIG_BLE_MESH_RPR_SRV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 1 if CONFIG_BLE_MESH_RPR_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_DF_CLI



> Directed Forwarding Configuration Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Directed Forwarding Configuration Client model.




###### CONFIG_BLE_MESH_DF_SRV



> Directed Forwarding Configuration Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models
> 
> 
> Enable support for Directed Forwarding Configuration Server model.




###### CONFIG_BLE_MESH_MAX_DISC_TABLE_ENTRY_COUNT



> Maximum number of discovery table entries in a given subnet
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_DF_SRV
> 
> 
> Maximum number of Discovery Table entries supported by the node in a given subnet.
> 
> 
> 
> Range:* from 2 to 255 if CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MAX_FORWARD_TABLE_ENTRY_COUNT



> Maximum number of forward table entries in a given subnet
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_DF_SRV
> 
> 
> Maximum number of Forward Table entries supported by the node in a given subnet.
> 
> 
> 
> Range:* from 2 to 64 if CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MAX_DEPS_NODES_PER_PATH



> Maximum number of dependent nodes per path
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_DF_SRV
> 
> 
> Maximum size of dependent nodes list supported by each forward table entry.
> 
> 
> 
> Range:* from 2 to 64 if CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_PATH_MONITOR_TEST



> Enable Path Monitoring test mode
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_DF_SRV
> 
> 
> The option only removes the Path Use timer; all other behavior of the
> device is not changed.
> If Path Monitoring test mode is going to be used, this option should
> be enabled.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_SUPPORT_DIRECTED_PROXY



> Enable Directed Proxy functionality
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Foundation models > CONFIG_BLE_MESH_DF_SRV
> 
> 
> Support Directed Proxy functionality.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH_GATT_PROXY_SERVER && CONFIG_BLE_MESH_DF_SRV && CONFIG_BLE_MESH





##### Support for BLE Mesh Client/Server models


Contains:


* CONFIG_BLE_MESH_MBT_CLI
* CONFIG_BLE_MESH_MBT_SRV
* CONFIG_BLE_MESH_GENERIC_BATTERY_CLI
* CONFIG_BLE_MESH_GENERIC_DEF_TRANS_TIME_CLI
* CONFIG_BLE_MESH_GENERIC_LEVEL_CLI
* CONFIG_BLE_MESH_GENERIC_LOCATION_CLI
* CONFIG_BLE_MESH_GENERIC_ONOFF_CLI
* CONFIG_BLE_MESH_GENERIC_POWER_LEVEL_CLI
* CONFIG_BLE_MESH_GENERIC_POWER_ONOFF_CLI
* CONFIG_BLE_MESH_GENERIC_PROPERTY_CLI
* CONFIG_BLE_MESH_GENERIC_SERVER
* CONFIG_BLE_MESH_LIGHT_CTL_CLI
* CONFIG_BLE_MESH_LIGHT_HSL_CLI
* CONFIG_BLE_MESH_LIGHT_LC_CLI
* CONFIG_BLE_MESH_LIGHT_LIGHTNESS_CLI
* CONFIG_BLE_MESH_LIGHT_XYL_CLI
* CONFIG_BLE_MESH_LIGHTING_SERVER
* CONFIG_BLE_MESH_SCENE_CLI
* CONFIG_BLE_MESH_SCHEDULER_CLI
* CONFIG_BLE_MESH_SENSOR_CLI
* CONFIG_BLE_MESH_SENSOR_SERVER
* CONFIG_BLE_MESH_TIME_SCENE_SERVER
* CONFIG_BLE_MESH_TIME_CLI



###### CONFIG_BLE_MESH_GENERIC_ONOFF_CLI



> Generic OnOff Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic OnOff Client model.




###### CONFIG_BLE_MESH_GENERIC_LEVEL_CLI



> Generic Level Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic Level Client model.




###### CONFIG_BLE_MESH_GENERIC_DEF_TRANS_TIME_CLI



> Generic Default Transition Time Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic Default Transition Time Client model.




###### CONFIG_BLE_MESH_GENERIC_POWER_ONOFF_CLI



> Generic Power OnOff Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic Power OnOff Client model.




###### CONFIG_BLE_MESH_GENERIC_POWER_LEVEL_CLI



> Generic Power Level Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic Power Level Client model.




###### CONFIG_BLE_MESH_GENERIC_BATTERY_CLI



> Generic Battery Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic Battery Client model.




###### CONFIG_BLE_MESH_GENERIC_LOCATION_CLI



> Generic Location Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic Location Client model.




###### CONFIG_BLE_MESH_GENERIC_PROPERTY_CLI



> Generic Property Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic Property Client model.




###### CONFIG_BLE_MESH_SENSOR_CLI



> Sensor Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Sensor Client model.




###### CONFIG_BLE_MESH_TIME_CLI



> Time Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Time Client model.




###### CONFIG_BLE_MESH_SCENE_CLI



> Scene Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Scene Client model.




###### CONFIG_BLE_MESH_SCHEDULER_CLI



> Scheduler Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Scheduler Client model.




###### CONFIG_BLE_MESH_LIGHT_LIGHTNESS_CLI



> Light Lightness Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Light Lightness Client model.




###### CONFIG_BLE_MESH_LIGHT_CTL_CLI



> Light CTL Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Light CTL Client model.




###### CONFIG_BLE_MESH_LIGHT_HSL_CLI



> Light HSL Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Light HSL Client model.




###### CONFIG_BLE_MESH_LIGHT_XYL_CLI



> Light XYL Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Light XYL Client model.




###### CONFIG_BLE_MESH_LIGHT_LC_CLI



> Light LC Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Light LC Client model.




###### CONFIG_BLE_MESH_GENERIC_SERVER



> Generic server models
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Generic server models.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_SENSOR_SERVER



> Sensor server models
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Sensor server models.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_TIME_SCENE_SERVER



> Time and Scenes server models
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Time and Scenes server models.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_LIGHTING_SERVER



> Lighting server models
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for Lighting server models.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MBT_CLI



> BLOB Transfer Client model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for BLOB Transfer Client model.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MAX_BLOB_RECEIVERS



> Maximum number of simultaneous blob receivers
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models > CONFIG_BLE_MESH_MBT_CLI
> 
> 
> Maximum number of BLOB Transfer Server models that can participating
> in the BLOB transfer with a BLOB Transfer Client model.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_BLE_MESH_MBT_CLI && CONFIG_BLE_MESH
> 
> 
> 
> Default value:* 2 if CONFIG_BLE_MESH_MBT_CLI && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_MBT_SRV



> BLOB Transfer Server model
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > Support for BLE Mesh Client/Server models
> 
> 
> Enable support for BLOB Transfer Server model.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH





##### CONFIG_BLE_MESH_IV_UPDATE_TEST



> Test the IV Update Procedure
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> This option removes the 96 hour limit of the IV Update Procedure and
> lets the state to be changed at any time.
> If IV Update test mode is going to be used, this option should be enabled.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




##### BLE Mesh specific test option


Contains:


* CONFIG_BLE_MESH_DEBUG
* CONFIG_BLE_MESH_SHELL
* CONFIG_BLE_MESH_BQB_TEST
* CONFIG_BLE_MESH_SELF_TEST
* CONFIG_BLE_MESH_TEST_AUTO_ENTER_NETWORK
* CONFIG_BLE_MESH_TEST_USE_WHITE_LIST



###### CONFIG_BLE_MESH_SELF_TEST



> Perform BLE Mesh self-tests
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option
> 
> 
> This option adds extra self-tests which are run every time BLE Mesh
> networking is initialized.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_BQB_TEST



> Enable BLE Mesh specific internal test
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option
> 
> 
> This option is used to enable some internal functions for auto-pts test.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_TEST_AUTO_ENTER_NETWORK



> Unprovisioned device enters mesh network automatically
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option
> 
> 
> With this option enabled, an unprovisioned device can automatically
> enters mesh network using a specific test function without the pro-
> visioning procedure. And on the Provisioner side, a test function
> needs to be invoked to add the node information into the mesh stack.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BLE_MESH_SELF_TEST && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_TEST_USE_WHITE_LIST



> Use white list to filter mesh advertising packets
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option
> 
> 
> With this option enabled, users can use white list to filter mesh
> advertising packets while scanning.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH_SELF_TEST && CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_SHELL



> Enable BLE Mesh shell
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option
> 
> 
> Activate shell module that provides BLE Mesh commands to the console.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_DEBUG



> Enable BLE Mesh debug logs
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option
> 
> 
> Enable debug logs for the BLE Mesh functionality.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH




###### CONFIG_BLE_MESH_DEBUG_NET



> Network layer debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Network layer debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_TRANS



> Transport layer debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Transport layer debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_BEACON



> Beacon debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Beacon-related debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_CRYPTO



> Crypto debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable cryptographic debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_PROV



> Provisioning debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Provisioning debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_ACCESS



> Access layer debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Access layer debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_MODEL



> Foundation model debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Foundation Models debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_ADV



> Advertising debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable advertising debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_LOW_POWER



> Low Power debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Low Power debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_FRIEND



> Friend debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Friend debug logs for the BLE Mesh functionality.




###### CONFIG_BLE_MESH_DEBUG_PROXY



> Proxy debug
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH > BLE Mesh specific test option > CONFIG_BLE_MESH_DEBUG
> 
> 
> Enable Proxy protocol debug logs for the BLE Mesh functionality.





##### CONFIG_BLE_MESH_EXPERIMENTAL



> Make BLE Mesh experimental features visible
> 
> 
> *Found in:* Component config > CONFIG_BLE_MESH
> 
> 
> Make BLE Mesh Experimental features visible.
> Experimental features list:
> - CONFIG_BLE_MESH_NOT_RELAY_REPLAY_MSG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_BLE_MESH





#### Console Library


Contains:


* CONFIG_CONSOLE_SORTED_HELP



##### CONFIG_CONSOLE_SORTED_HELP



> Enable sorted help
> 
> 
> *Found in:* Component config > Console Library
> 
> 
> Instead of listing the commands in the order of registration, the help command lists
> the available commands in sorted order, if this option is enabled.
> 
> 
> 
> Default value:* No (disabled)





#### Driver Configurations


Contains:


* Legacy ADC Driver Configuration
* Legacy DAC Driver Configurations
* Legacy I2S Driver Configurations
* Legacy MCPWM Driver Configurations
* Legacy PCNT Driver Configurations
* Legacy RMT Driver Configurations
* Legacy SDM Driver Configurations
* Legacy Temperature Sensor Driver Configurations
* Legacy Timer Group Driver Configurations
* TWAI Configuration



##### TWAI Configuration


Contains:


* CONFIG_TWAI_ERRATA_FIX_LISTEN_ONLY_DOM
* CONFIG_TWAI_ISR_IN_IRAM



###### CONFIG_TWAI_ISR_IN_IRAM



> Place TWAI ISR function into IRAM
> 
> 
> *Found in:* Component config > Driver Configurations > TWAI Configuration
> 
> 
> Place the TWAI ISR in to IRAM. This will allow the ISR to avoid
> cache misses, and also be able to run whilst the cache is disabled
> (such as when writing to SPI Flash).
> Note that if this option is enabled:
> - Users should also set the ESP_INTR_FLAG_IRAM in the driver
> configuration structure when installing the driver (see docs for
> specifics).
> - Alert logging (i.e., setting of the TWAI_ALERT_AND_LOG flag)
> will have no effect.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_TWAI_ERRATA_FIX_LISTEN_ONLY_DOM



> Add SW workaround for listen only transmits dominant bit errata
> 
> 
> *Found in:* Component config > Driver Configurations > TWAI Configuration
> 
> 
> When in the listen only mode, the TWAI controller must not influence the TWAI bus (i.e., must not send
> any dominant bits). However, while in listen only mode on the ESP32/ESP32-S2/ESP32-S3/ESP32-C3, the
> TWAI controller will still transmit dominant bits when it detects an error (i.e., as part of an active
> error frame). Enabling this option will add a workaround that forces the TWAI controller into an error
> passive state on initialization, thus preventing any dominant bits from being sent.
> 
> 
> 
> Default value:* Yes (enabled)





##### Legacy ADC Driver Configuration


Contains:


* CONFIG_ADC_DISABLE_DAC
* Legacy ADC Calibration Configuration
* CONFIG_ADC_SUPPRESS_DEPRECATE_WARN



###### CONFIG_ADC_DISABLE_DAC



> Disable DAC when ADC2 is used on GPIO 25 and 26
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy ADC Driver Configuration
> 
> 
> If this is set, the ADC2 driver will disable the output of the DAC corresponding to the specified
> channel. This is the default value.
> 
> 
> For testing, disable this option so that we can measure the output of DAC by internal ADC.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_DAC_SUPPORTED




###### CONFIG_ADC_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy ADC Driver Configuration
> 
> 
> Whether to suppress the deprecation warnings when using legacy adc driver (driver/adc.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled)




###### Legacy ADC Calibration Configuration


Contains:


* CONFIG_ADC_CALI_SUPPRESS_DEPRECATE_WARN




###### CONFIG_ADC_CALI_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy ADC Driver Configuration > Legacy ADC Calibration Configuration
> 
> 
> Whether to suppress the deprecation warnings when using legacy adc calibration
> driver (esp_adc_cal.h).
> If you want to continue using the legacy driver, and don't want to see related
> deprecation warnings, you can enable this option.
> 
> 
> 
> Default value:* No (disabled)





##### Legacy DAC Driver Configurations


Contains:


* CONFIG_DAC_SUPPRESS_DEPRECATE_WARN



###### CONFIG_DAC_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy DAC Driver Configurations
> 
> 
> Whether to suppress the deprecation warnings when using legacy dac driver (driver/dac.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled) if SOC_DAC_SUPPORTED





##### Legacy MCPWM Driver Configurations


Contains:


* CONFIG_MCPWM_SUPPRESS_DEPRECATE_WARN



###### CONFIG_MCPWM_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy MCPWM Driver Configurations
> 
> 
> Whether to suppress the deprecation warnings when using legacy MCPWM driver (driver/mcpwm.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled)





##### Legacy Timer Group Driver Configurations


Contains:


* CONFIG_GPTIMER_SUPPRESS_DEPRECATE_WARN



###### CONFIG_GPTIMER_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy Timer Group Driver Configurations
> 
> 
> Whether to suppress the deprecation warnings when using legacy timer group driver (driver/timer.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled)





##### Legacy RMT Driver Configurations


Contains:


* CONFIG_RMT_SUPPRESS_DEPRECATE_WARN



###### CONFIG_RMT_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy RMT Driver Configurations
> 
> 
> Whether to suppress the deprecation warnings when using legacy rmt driver (driver/rmt.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled)





##### Legacy I2S Driver Configurations


Contains:


* CONFIG_I2S_SUPPRESS_DEPRECATE_WARN



###### CONFIG_I2S_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy I2S Driver Configurations
> 
> 
> Whether to suppress the deprecation warnings when using legacy i2s driver (driver/i2s.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled)





##### Legacy PCNT Driver Configurations


Contains:


* CONFIG_PCNT_SUPPRESS_DEPRECATE_WARN



###### CONFIG_PCNT_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy PCNT Driver Configurations
> 
> 
> whether to suppress the deprecation warnings when using legacy PCNT driver (driver/pcnt.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled)





##### Legacy SDM Driver Configurations


Contains:


* CONFIG_SDM_SUPPRESS_DEPRECATE_WARN



###### CONFIG_SDM_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy SDM Driver Configurations
> 
> 
> whether to suppress the deprecation warnings when using legacy SDM driver (driver/sigmadelta.h).
> If you want to continue using the legacy driver, and don't want to see related deprecation warnings,
> you can enable this option.
> 
> 
> 
> Default value:* No (disabled)





##### Legacy Temperature Sensor Driver Configurations


Contains:


* CONFIG_TEMP_SENSOR_SUPPRESS_DEPRECATE_WARN



###### CONFIG_TEMP_SENSOR_SUPPRESS_DEPRECATE_WARN



> Suppress legacy driver deprecated warning
> 
> 
> *Found in:* Component config > Driver Configurations > Legacy Temperature Sensor Driver Configurations
> 
> 
> whether to suppress the deprecation warnings when using legacy temperature sensor driver
> (driver/temp_sensor.h). If you want to continue using the legacy driver,
> and don't want to see related deprecation warnings, you can enable this option.
> 
> 
> 
> Default value:* No (disabled)






#### eFuse Bit Manager


Contains:


* CONFIG_EFUSE_VIRTUAL
* CONFIG_EFUSE_CUSTOM_TABLE



##### CONFIG_EFUSE_CUSTOM_TABLE



> Use custom eFuse table
> 
> 
> *Found in:* Component config > eFuse Bit Manager
> 
> 
> Allows to generate a structure for eFuse from the CSV file.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_EFUSE_CUSTOM_TABLE_FILENAME



> Custom eFuse CSV file
> 
> 
> *Found in:* Component config > eFuse Bit Manager > CONFIG_EFUSE_CUSTOM_TABLE
> 
> 
> Name of the custom eFuse CSV filename. This path is evaluated
> relative to the project root directory.
> 
> 
> 
> Default value:* "main/esp_efuse_custom_table.csv" if CONFIG_EFUSE_CUSTOM_TABLE





##### CONFIG_EFUSE_VIRTUAL



> Simulate eFuse operations in RAM
> 
> 
> *Found in:* Component config > eFuse Bit Manager
> 
> 
> If "n" - No virtual mode. All eFuse operations are real and use eFuse registers.
> If "y" - The virtual mode is enabled and all eFuse operations (read and write) are redirected
> to RAM instead of eFuse registers, all permanent changes (via eFuse) are disabled.
> Log output will state changes that would be applied, but they will not be.
> 
> 
> If it is "y", then SECURE_FLASH_ENCRYPTION_MODE_RELEASE cannot be used.
> Because the EFUSE VIRT mode is for testing only.
> 
> 
> During startup, the eFuses are copied into RAM. This mode is useful for fast tests.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_EFUSE_VIRTUAL_KEEP_IN_FLASH



> Keep eFuses in flash
> 
> 
> *Found in:* Component config > eFuse Bit Manager > CONFIG_EFUSE_VIRTUAL
> 
> 
> In addition to the "Simulate eFuse operations in RAM" option, this option just adds
> a feature to keep eFuses after reboots in flash memory. To use this mode the partition_table
> should have the efuse partition. partition.csv: "efuse_em, data, efuse, , 0x2000,"
> 
> 
> During startup, the eFuses are copied from flash or,
> in case if flash is empty, from real eFuse to RAM and then update flash.
> This mode is useful when need to keep changes after reboot
> (testing secure_boot and flash_encryption).




###### CONFIG_EFUSE_VIRTUAL_LOG_ALL_WRITES



> Log all virtual writes
> 
> 
> *Found in:* Component config > eFuse Bit Manager > CONFIG_EFUSE_VIRTUAL
> 
> 
> If enabled, log efuse burns. This shows changes that would be made.






#### ESP-TLS


Contains:


* CONFIG_ESP_TLS_INSECURE
* CONFIG_ESP_TLS_SERVER_CERT_SELECT_HOOK
* CONFIG_ESP_TLS_LIBRARY_CHOOSE
* CONFIG_ESP_TLS_CLIENT_SESSION_TICKETS
* CONFIG_ESP_DEBUG_WOLFSSL
* CONFIG_ESP_TLS_PSK_VERIFICATION
* CONFIG_ESP_TLS_SERVER_SESSION_TICKETS
* CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY
* CONFIG_ESP_TLS_SERVER_MIN_AUTH_MODE_OPTIONAL
* CONFIG_ESP_TLS_USE_DS_PERIPHERAL



##### CONFIG_ESP_TLS_LIBRARY_CHOOSE



> Choose SSL/TLS library for ESP-TLS (See help for more Info)
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> The ESP-TLS APIs support multiple backend TLS libraries. Currently mbedTLS and WolfSSL are
> supported. Different TLS libraries may support different features and have different resource
> usage. Consult the ESP-TLS documentation in ESP-IDF Programming guide for more details.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * mbedTLS (CONFIG_ESP_TLS_USING_MBEDTLS)
> > * wolfSSL (License info in wolfSSL directory README) (CONFIG_ESP_TLS_USING_WOLFSSL)




##### CONFIG_ESP_TLS_USE_DS_PERIPHERAL



> Use Digital Signature (DS) Peripheral with ESP-TLS
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> Enable use of the Digital Signature Peripheral for ESP-TLS.The DS peripheral
> can only be used when it is appropriately configured for TLS.
> Consult the ESP-TLS documentation in ESP-IDF Programming Guide for more details.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_TLS_CLIENT_SESSION_TICKETS



> Enable client session tickets
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> Enable session ticket support as specified in RFC5077.




##### CONFIG_ESP_TLS_SERVER_SESSION_TICKETS



> Enable server session tickets
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> Enable session ticket support as specified in RFC5077



###### CONFIG_ESP_TLS_SERVER_SESSION_TICKET_TIMEOUT



> Server session ticket timeout in seconds
> 
> 
> *Found in:* Component config > ESP-TLS > CONFIG_ESP_TLS_SERVER_SESSION_TICKETS
> 
> 
> Sets the session ticket timeout used in the tls server.
> 
> 
> 
> Default value:* 86400 if CONFIG_ESP_TLS_SERVER_SESSION_TICKETS





##### CONFIG_ESP_TLS_SERVER_CERT_SELECT_HOOK



> Certificate selection hook
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> Ability to configure and use a certificate selection callback during server handshake,
> to select a certificate to present to the client based on the TLS extensions supplied in
> the client hello (alpn, sni, etc).




##### CONFIG_ESP_TLS_SERVER_MIN_AUTH_MODE_OPTIONAL



> ESP-TLS Server: Set minimum Certificate Verification mode to Optional
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> When this option is enabled, the peer (here, the client) certificate is checked by the server,
> however the handshake continues even if verification failed. By default, the
> peer certificate is not checked and ignored by the server.
> 
> 
> mbedtls_ssl_get_verify_result() can be called after the handshake is complete to
> retrieve status of verification.




##### CONFIG_ESP_TLS_PSK_VERIFICATION



> Enable PSK verification
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> Enable support for pre shared key ciphers, supported for both mbedTLS as well as
> wolfSSL TLS library.




##### CONFIG_ESP_TLS_INSECURE



> Allow potentially insecure options
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> You can enable some potentially insecure options. These options should only be used for testing pusposes.
> Only enable these options if you are very sure.



###### CONFIG_ESP_TLS_SKIP_SERVER_CERT_VERIFY



> Skip server certificate verification by default (WARNING: ONLY FOR TESTING PURPOSE, READ HELP)
> 
> 
> *Found in:* Component config > ESP-TLS > CONFIG_ESP_TLS_INSECURE
> 
> 
> After enabling this option the esp-tls client will skip the server certificate verification
> by default. Note that this option will only modify the default behaviour of esp-tls client
> regarding server cert verification. The default behaviour should only be applicable when
> no other option regarding the server cert verification is opted in the esp-tls config
> (e.g. crt_bundle_attach, use_global_ca_store etc.).
> WARNING : Enabling this option comes with a potential risk of establishing a TLS connection
> with a server which has a fake identity, provided that the server certificate
> is not provided either through API or other mechanism like ca_store etc.





##### CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY



> Enable SMALL_CERT_VERIFY
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> Enables server verification with Intermediate CA cert, does not authenticate full chain
> of trust upto the root CA cert (After Enabling this option client only needs to have Intermediate
> CA certificate of the server to authenticate server, root CA cert is not necessary).
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_TLS_USING_WOLFSSL




##### CONFIG_ESP_DEBUG_WOLFSSL



> Enable debug logs for wolfSSL
> 
> 
> *Found in:* Component config > ESP-TLS
> 
> 
> Enable detailed debug prints for wolfSSL SSL library.





#### ADC and ADC Calibration


Contains:


* ADC Calibration Configurations
* CONFIG_ADC_CONTINUOUS_ISR_IRAM_SAFE
* CONFIG_ADC_DISABLE_DAC_OUTPUT
* CONFIG_ADC_ENABLE_DEBUG_LOG
* CONFIG_ADC_CONTINUOUS_FORCE_USE_ADC2_ON_C3_S3
* CONFIG_ADC_ONESHOT_CTRL_FUNC_IN_IRAM



##### CONFIG_ADC_ONESHOT_CTRL_FUNC_IN_IRAM



> Place ISR version ADC oneshot mode read function into IRAM
> 
> 
> *Found in:* Component config > ADC and ADC Calibration
> 
> 
> Place ISR version ADC oneshot mode read function into IRAM.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ADC_CONTINUOUS_ISR_IRAM_SAFE



> ADC continuous mode driver ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ADC and ADC Calibration
> 
> 
> Ensure the ADC continuous mode ISR is IRAM-Safe. When enabled, the ISR handler
> will be available when the cache is disabled.
> 
> 
> 
> Default value:* No (disabled)




##### ADC Calibration Configurations




##### CONFIG_ADC_DISABLE_DAC_OUTPUT



> Disable DAC when ADC2 is in use
> 
> 
> *Found in:* Component config > ADC and ADC Calibration
> 
> 
> By default, this is set. The ADC oneshot driver will disable the output of the
> corresponding DAC channels:
> ESP32: IO25 and IO26
> ESP32S2: IO17 and IO18
> 
> 
> Disable this option so as to measure the output of DAC by internal ADC, for test usage.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_DAC_SUPPORTED




##### CONFIG_ADC_CONTINUOUS_FORCE_USE_ADC2_ON_C3_S3



> Force use ADC2 continumous mode on ESP32S3 or ESP32C3
> 
> 
> *Found in:* Component config > ADC and ADC Calibration
> 
> 
> On ESP32C3 and ESP32S3, ADC2 Digital Controller is not stable. Therefore,
> ADC2 continuous mode is not suggested on ESP32S3 and ESP32C3
> 
> 
> If you stick to this, you can enable this option to force use ADC2 under above conditions.
> For more details, you can search for errata on espressif website.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ADC_ENABLE_DEBUG_LOG



> Enable ADC debug log
> 
> 
> *Found in:* Component config > ADC and ADC Calibration
> 
> 
> whether to enable the debug log message for ADC driver.
> Note that this option only controls the ADC driver log, will not affect other drivers.
> 
> 
> note: This cannot be used in the ADC legacy driver.
> 
> 
> 
> Default value:* No (disabled)





#### Wireless Coexistence


Contains:


* CONFIG_ESP_COEX_EXTERNAL_COEXIST_ENABLE
* CONFIG_ESP_COEX_SW_COEXIST_ENABLE
* CONFIG_ESP_COEX_POWER_MANAGEMENT



##### CONFIG_ESP_COEX_SW_COEXIST_ENABLE



> Software controls WiFi/Bluetooth coexistence
> 
> 
> *Found in:* Component config > Wireless Coexistence
> 
> 
> If enabled, WiFi & Bluetooth coexistence is controlled by software rather than hardware.
> Recommended for heavy traffic scenarios. Both coexistence configuration options are
> automatically managed, no user intervention is required.
> If only Bluetooth is used, it is recommended to disable this option to reduce binary file
> size.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_ENABLED || CONFIG_IEEE802154_ENABLED || (CONFIG_IEEE802154_ENABLED && CONFIG_BT_ENABLED)




##### CONFIG_ESP_COEX_EXTERNAL_COEXIST_ENABLE



> External Coexistence
> 
> 
> *Found in:* Component config > Wireless Coexistence
> 
> 
> If enabled, HW External coexistence arbitration is managed by GPIO pins.
> It can support three types of wired combinations so far which are 1-wired/2-wired/3-wired.
> User can select GPIO pins in application code with configure interfaces.
> 
> 
> This function depends on BT-off
> because currently we do not support external coex and internal coex simultaneously.




##### CONFIG_ESP_COEX_POWER_MANAGEMENT



> Support power management under coexistence
> 
> 
> *Found in:* Component config > Wireless Coexistence
> 
> 
> If enabled, coexist power management will be enabled.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_COEX_SW_COEXIST_ENABLE





#### Common ESP-related


Contains:


* CONFIG_ESP_ERR_TO_NAME_LOOKUP



##### CONFIG_ESP_ERR_TO_NAME_LOOKUP



> Enable lookup of error code strings
> 
> 
> *Found in:* Component config > Common ESP-related
> 
> 
> Functions esp_err_to_name() and esp_err_to_name_r() return string representations of error codes from a
> pre-generated lookup table. This option can be used to turn off the use of the look-up table in order to
> save memory but this comes at the price of sacrificing distinguishable (meaningful) output string
> representations.
> 
> 
> 
> Default value:* Yes (enabled)





#### ESP-Driver:Analog Comparator Configurations


Contains:


* CONFIG_ANA_CMPR_ISR_IRAM_SAFE
* CONFIG_ANA_CMPR_ENABLE_DEBUG_LOG
* CONFIG_ANA_CMPR_CTRL_FUNC_IN_IRAM



##### CONFIG_ANA_CMPR_ISR_IRAM_SAFE



> Analog comparator ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:Analog Comparator Configurations
> 
> 
> Ensure the Analog Comparator interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled) if SOC_ANA_CMPR_SUPPORTED




##### CONFIG_ANA_CMPR_CTRL_FUNC_IN_IRAM



> Place Analog Comparator control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:Analog Comparator Configurations
> 
> 
> Place Analog Comparator control functions (like ana_cmpr_set_internal_reference) into IRAM,
> so that these functions can be IRAM-safe and able to be called in an IRAM interrupt context.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled) if SOC_ANA_CMPR_SUPPORTED




##### CONFIG_ANA_CMPR_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:Analog Comparator Configurations
> 
> 
> whether to enable the debug log message for Analog Comparator driver.
> Note that, this option only controls the Analog Comparator driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled) if SOC_ANA_CMPR_SUPPORTED





#### ESP-Driver:Camera Controller Configurations


Contains:


* CONFIG_CAM_CTLR_MIPI_CSI_ISR_IRAM_SAFE
* CONFIG_CAM_CTLR_DVP_CAM_ISR_IRAM_SAFE
* CONFIG_CAM_CTLR_ISP_DVP_ISR_IRAM_SAFE



##### CONFIG_CAM_CTLR_MIPI_CSI_ISR_IRAM_SAFE



> CSI ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:Camera Controller Configurations
> 
> 
> Ensure the CSI driver ISR is IRAM-Safe. When enabled, the ISR handler
> will be available when the cache is disabled.
> 
> 
> 
> Default value:* No (disabled) if SOC_MIPI_CSI_SUPPORTED && (SOC_MIPI_CSI_SUPPORTED || SOC_LCDCAM_CAM_SUPPORTED)




##### CONFIG_CAM_CTLR_ISP_DVP_ISR_IRAM_SAFE



> ISP_DVP ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:Camera Controller Configurations
> 
> 
> Ensure the ISP_DVP driver ISR is IRAM-Safe. When enabled, the ISR handler
> will be available when the cache is disabled.
> 
> 
> 
> Default value:* No (disabled) if SOC_MIPI_CSI_SUPPORTED || SOC_LCDCAM_CAM_SUPPORTED




##### CONFIG_CAM_CTLR_DVP_CAM_ISR_IRAM_SAFE



> DVP ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:Camera Controller Configurations
> 
> 
> Ensure the DVP driver ISR is IRAM-Safe. When enabled, the ISR handler
> will be available when the cache is disabled.
> 
> 
> 
> Default value:* No (disabled) if SOC_LCDCAM_CAM_SUPPORTED && (SOC_MIPI_CSI_SUPPORTED || SOC_LCDCAM_CAM_SUPPORTED)





#### ESP-Driver:DAC Configurations


Contains:


* CONFIG_DAC_DMA_AUTO_16BIT_ALIGN
* CONFIG_DAC_ISR_IRAM_SAFE
* CONFIG_DAC_ENABLE_DEBUG_LOG
* CONFIG_DAC_CTRL_FUNC_IN_IRAM



##### CONFIG_DAC_CTRL_FUNC_IN_IRAM



> Place DAC control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:DAC Configurations
> 
> 
> Place DAC control functions (e.g. 'dac_oneshot_output_voltage') into IRAM,
> so that this function can be IRAM-safe and able to be called in the other IRAM interrupt context.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled) if SOC_DAC_SUPPORTED




##### CONFIG_DAC_ISR_IRAM_SAFE



> DAC ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:DAC Configurations
> 
> 
> Ensure the DAC interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled) if SOC_DAC_SUPPORTED




##### CONFIG_DAC_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:DAC Configurations
> 
> 
> whether to enable the debug log message for DAC driver.
> Note that, this option only controls the DAC driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled) if SOC_DAC_SUPPORTED




##### CONFIG_DAC_DMA_AUTO_16BIT_ALIGN



> Align the continuous data to 16 bit automatically
> 
> 
> *Found in:* Component config > ESP-Driver:DAC Configurations
> 
> 
> Whether to left shift the continuous data to align every bytes to 16 bits in the driver.
> On ESP32, although the DAC resolution is only 8 bits,
> the hardware requires 16 bits data in continuous mode.
> By enabling this option, the driver will left shift 8 bits for the input data automatically.
> Only disable this option when you decide to do this step by yourself.
> Note that the driver will allocate a new piece of memory to save the converted data.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_DAC_DMA_16BIT_ALIGN && SOC_DAC_SUPPORTED





#### ESP-Driver:GPIO Configurations


Contains:


* CONFIG_GPIO_CTRL_FUNC_IN_IRAM



##### CONFIG_GPIO_CTRL_FUNC_IN_IRAM



> Place GPIO control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:GPIO Configurations
> 
> 
> Place GPIO control functions (like intr_disable/set_level) into IRAM,
> so that these functions can be IRAM-safe and able to be called in the other IRAM interrupt context.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:GPTimer Configurations


Contains:


* CONFIG_GPTIMER_ENABLE_DEBUG_LOG
* CONFIG_GPTIMER_ISR_IRAM_SAFE
* CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM
* CONFIG_GPTIMER_ISR_HANDLER_IN_IRAM



##### CONFIG_GPTIMER_ISR_HANDLER_IN_IRAM



> Place GPTimer ISR handler into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:GPTimer Configurations
> 
> 
> Place GPTimer ISR handler into IRAM for better performance and fewer cache misses.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM



> Place GPTimer control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:GPTimer Configurations
> 
> 
> Place GPTimer control functions (like start/stop) into IRAM,
> so that these functions can be IRAM-safe and able to be called in the other IRAM interrupt context.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_GPTIMER_ISR_IRAM_SAFE



> GPTimer ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:GPTimer Configurations
> 
> 
> Ensure the GPTimer interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_GPTIMER_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:GPTimer Configurations
> 
> 
> whether to enable the debug log message for GPTimer driver.
> Note that, this option only controls the GPTimer driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:I2C Configurations


Contains:


* CONFIG_I2C_ENABLE_DEBUG_LOG
* CONFIG_I2C_ISR_IRAM_SAFE



##### CONFIG_I2C_ISR_IRAM_SAFE



> I2C ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:I2C Configurations
> 
> 
> Ensure the I2C interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> note: This cannot be used in the I2C legacy driver.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_I2C_ENABLE_DEBUG_LOG



> Enable I2C debug log
> 
> 
> *Found in:* Component config > ESP-Driver:I2C Configurations
> 
> 
> whether to enable the debug log message for I2C driver.
> Note that this option only controls the I2C driver log, will not affect other drivers.
> 
> 
> note: This cannot be used in the I2C legacy driver.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:I2S Configurations


Contains:


* CONFIG_I2S_ENABLE_DEBUG_LOG
* CONFIG_I2S_ISR_IRAM_SAFE



##### CONFIG_I2S_ISR_IRAM_SAFE



> I2S ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:I2S Configurations
> 
> 
> Ensure the I2S interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_I2S_ENABLE_DEBUG_LOG



> Enable I2S debug log
> 
> 
> *Found in:* Component config > ESP-Driver:I2S Configurations
> 
> 
> whether to enable the debug log message for I2S driver.
> Note that, this option only controls the I2S driver log, will not affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:ISP Configurations


Contains:


* CONFIG_ISP_ISR_IRAM_SAFE



##### CONFIG_ISP_ISR_IRAM_SAFE



> ISP driver ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:ISP Configurations
> 
> 
> Ensure the ISP driver ISR is IRAM-Safe. When enabled, the ISR handler
> will be available when the cache is disabled.
> 
> 
> 
> Default value:* No (disabled) if SOC_ISP_SUPPORTED





#### ESP-Driver:JPEG-Codec Configurations


Contains:


* CONFIG_JPEG_ENABLE_DEBUG_LOG



##### CONFIG_JPEG_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:JPEG-Codec Configurations
> 
> 
> whether to enable the debug log message for JPEG driver.
> Note that, this option only controls the JPEG driver log, won't affect other drivers.
> Please also note, enable this option will make jpeg codec process speed much slower.
> 
> 
> 
> Default value:* No (disabled) if SOC_JPEG_CODEC_SUPPORTED





#### ESP-Driver:LEDC Configurations


Contains:


* CONFIG_LEDC_CTRL_FUNC_IN_IRAM



##### CONFIG_LEDC_CTRL_FUNC_IN_IRAM



> Place LEDC control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:LEDC Configurations
> 
> 
> Place LEDC control functions (ledc_update_duty and ledc_stop) into IRAM,
> so that these functions can be IRAM-safe and able to be called in an IRAM context.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:MCPWM Configurations


Contains:


* CONFIG_MCPWM_ENABLE_DEBUG_LOG
* CONFIG_MCPWM_CTRL_FUNC_IN_IRAM
* CONFIG_MCPWM_ISR_IRAM_SAFE



##### CONFIG_MCPWM_ISR_IRAM_SAFE



> Place MCPWM ISR function into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:MCPWM Configurations
> 
> 
> This will ensure the MCPWM interrupt handle is IRAM-Safe, allow to avoid flash
> cache misses, and also be able to run whilst the cache is disabled.
> (e.g. SPI Flash write)
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MCPWM_CTRL_FUNC_IN_IRAM



> Place MCPWM control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:MCPWM Configurations
> 
> 
> Place MCPWM control functions (like set_compare_value) into IRAM,
> so that these functions can be IRAM-safe and able to be called in the other IRAM interrupt context.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MCPWM_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:MCPWM Configurations
> 
> 
> whether to enable the debug log message for MCPWM driver.
> Note that, this option only controls the MCPWM driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:Parallel IO Configurations


Contains:


* CONFIG_PARLIO_ENABLE_DEBUG_LOG
* CONFIG_PARLIO_ISR_IRAM_SAFE



##### CONFIG_PARLIO_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:Parallel IO Configurations
> 
> 
> whether to enable the debug log message for parallel IO driver.
> Note that, this option only controls the parallel IO driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled) if SOC_PARLIO_SUPPORTED




##### CONFIG_PARLIO_ISR_IRAM_SAFE



> Parallel IO ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:Parallel IO Configurations
> 
> 
> Ensure the Parallel IO interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled) if SOC_PARLIO_SUPPORTED





#### ESP-Driver:PCNT Configurations


Contains:


* CONFIG_PCNT_ENABLE_DEBUG_LOG
* CONFIG_PCNT_ISR_IRAM_SAFE
* CONFIG_PCNT_CTRL_FUNC_IN_IRAM



##### CONFIG_PCNT_CTRL_FUNC_IN_IRAM



> Place PCNT control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:PCNT Configurations
> 
> 
> Place PCNT control functions (like start/stop) into IRAM,
> so that these functions can be IRAM-safe and able to be called in the other IRAM interrupt context.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_PCNT_ISR_IRAM_SAFE



> PCNT ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:PCNT Configurations
> 
> 
> Ensure the PCNT interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_PCNT_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:PCNT Configurations
> 
> 
> whether to enable the debug log message for PCNT driver.
> Note that, this option only controls the PCNT driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:RMT Configurations


Contains:


* CONFIG_RMT_ENABLE_DEBUG_LOG
* CONFIG_RMT_RECV_FUNC_IN_IRAM
* CONFIG_RMT_ISR_IRAM_SAFE



##### CONFIG_RMT_ISR_IRAM_SAFE



> RMT ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:RMT Configurations
> 
> 
> Ensure the RMT interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_RMT_RECV_FUNC_IN_IRAM



> Place RMT receive function into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:RMT Configurations
> 
> 
> Place RMT receive function into IRAM,
> so that the receive function can be IRAM-safe and able to be called when the flash cache is disabled.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_RMT_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:RMT Configurations
> 
> 
> whether to enable the debug log message for RMT driver.
> Note that, this option only controls the RMT driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:Sigma Delta Modulator Configurations


Contains:


* CONFIG_SDM_ENABLE_DEBUG_LOG
* CONFIG_SDM_CTRL_FUNC_IN_IRAM



##### CONFIG_SDM_CTRL_FUNC_IN_IRAM



> Place SDM control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:Sigma Delta Modulator Configurations
> 
> 
> Place SDM control functions (like set_duty) into IRAM,
> so that these functions can be IRAM-safe and able to be called in the other IRAM interrupt context.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_SDM_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:Sigma Delta Modulator Configurations
> 
> 
> whether to enable the debug log message for SDM driver.
> Note that, this option only controls the SDM driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:SPI Configurations


Contains:


* CONFIG_SPI_MASTER_ISR_IN_IRAM
* CONFIG_SPI_SLAVE_ISR_IN_IRAM
* CONFIG_SPI_MASTER_IN_IRAM
* CONFIG_SPI_SLAVE_IN_IRAM



##### CONFIG_SPI_MASTER_IN_IRAM



> Place transmitting functions of SPI master into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:SPI Configurations
> 
> 
> Normally only the ISR of SPI master is placed in the IRAM, so that it
> can work without the flash when interrupt is triggered.
> For other functions, there's some possibility that the flash cache
> miss when running inside and out of SPI functions, which may increase
> the interval of SPI transactions.
> Enable this to put `queue\_trans`, `get\_trans\_result` and
> `transmit` functions into the IRAM to avoid possible cache miss.
> 
> 
> This configuration won't be available if CONFIG_FREERTOS_PLACE_FUNCTIONS_INTO_FLASH is enabled.
> 
> 
> During unit test, this is enabled to measure the ideal case of api.




##### CONFIG_SPI_MASTER_ISR_IN_IRAM



> Place SPI master ISR function into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:SPI Configurations
> 
> 
> Place the SPI master ISR in to IRAM to avoid possible cache miss.
> 
> 
> Enabling this configuration is possible only when HEAP_PLACE_FUNCTION_INTO_FLASH
> is disabled since the spi master uses can allocate transactions buffers into DMA
> memory section using the heap component API that ipso facto has to be placed in IRAM.
> 
> 
> Also you can forbid the ISR being disabled during flash writing
> access, by add ESP_INTR_FLAG_IRAM when initializing the driver.




##### CONFIG_SPI_SLAVE_IN_IRAM



> Place transmitting functions of SPI slave into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:SPI Configurations
> 
> 
> Normally only the ISR of SPI slave is placed in the IRAM, so that it
> can work without the flash when interrupt is triggered.
> For other functions, there's some possibility that the flash cache
> miss when running inside and out of SPI functions, which may increase
> the interval of SPI transactions.
> Enable this to put `queue\_trans`, `get\_trans\_result` and
> `transmit` functions into the IRAM to avoid possible cache miss.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_SPI_SLAVE_ISR_IN_IRAM



> Place SPI slave ISR function into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:SPI Configurations
> 
> 
> Place the SPI slave ISR in to IRAM to avoid possible cache miss.
> 
> 
> Also you can forbid the ISR being disabled during flash writing
> access, by add ESP_INTR_FLAG_IRAM when initializing the driver.
> 
> 
> 
> Default value:* Yes (enabled)





#### ESP-Driver:Touch Sensor Configurations


Contains:


* CONFIG_TOUCH_ENABLE_DEBUG_LOG
* CONFIG_TOUCH_CTRL_FUNC_IN_IRAM
* CONFIG_TOUCH_ISR_IRAM_SAFE



##### CONFIG_TOUCH_CTRL_FUNC_IN_IRAM



> Place touch sensor control functions into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:Touch Sensor Configurations
> 
> 
> Place touch sensor oneshot scanning and continuous scanning functions into IRAM,
> so that these function can be IRAM-safe and able to be called when the flash cache is disabled.
> Enabling this option can improve driver performance as well.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_TOUCH_ISR_IRAM_SAFE



> Touch sensor ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:Touch Sensor Configurations
> 
> 
> Ensure the touch sensor interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_TOUCH_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:Touch Sensor Configurations
> 
> 
> Whether to enable the debug log message for touch driver.
> Note that, this option only controls the touch driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





#### ESP-Driver:Temperature Sensor Configurations


Contains:


* CONFIG_TEMP_SENSOR_ENABLE_DEBUG_LOG
* CONFIG_TEMP_SENSOR_ISR_IRAM_SAFE



##### CONFIG_TEMP_SENSOR_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > ESP-Driver:Temperature Sensor Configurations
> 
> 
> whether to enable the debug log message for temperature sensor driver.
> Note that, this option only controls the temperature sensor driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_TEMP_SENSOR_ISR_IRAM_SAFE



> Temperature sensor ISR IRAM-Safe
> 
> 
> *Found in:* Component config > ESP-Driver:Temperature Sensor Configurations
> 
> 
> Ensure the Temperature Sensor interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled) if SOC_TEMPERATURE_SENSOR_INTR_SUPPORT





#### ESP-Driver:UART Configurations


Contains:


* CONFIG_UART_ISR_IN_IRAM



##### CONFIG_UART_ISR_IN_IRAM



> Place UART ISR function into IRAM
> 
> 
> *Found in:* Component config > ESP-Driver:UART Configurations
> 
> 
> If this option is not selected, UART interrupt will be disabled for a long time and
> may cause data lost when doing spi flash operation.





#### ESP-Driver:USB Serial/JTAG Configuration


Contains:


* CONFIG_USJ_ENABLE_USB_SERIAL_JTAG



##### CONFIG_USJ_ENABLE_USB_SERIAL_JTAG



> Enable USB-Serial-JTAG Module
> 
> 
> *Found in:* Component config > ESP-Driver:USB Serial/JTAG Configuration
> 
> 
> The USB-Serial-JTAG module on ESP chips is turned on by default after power-on.
> If your application does not need it and not rely on it to be used as system
> console or use the built-in JTAG for debugging, you can disable this option,
> then the clock of this module will be disabled at startup, which will save
> some power consumption.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_USJ_NO_AUTO_LS_ON_CONNECTION



> Don't enter the automatic light sleep when USB Serial/JTAG port is connected
> 
> 
> *Found in:* Component config > ESP-Driver:USB Serial/JTAG Configuration > CONFIG_USJ_ENABLE_USB_SERIAL_JTAG
> 
> 
> If enabled, the chip will constantly monitor the connection status of the USB Serial/JTAG port. As long
> as the USB Serial/JTAG is connected, a ESP_PM_NO_LIGHT_SLEEP power management lock will be acquired to
> prevent the system from entering light sleep.
> This option can be useful if serial monitoring is needed via USB Serial/JTAG while power management is
> enabled, as the USB Serial/JTAG cannot work under light sleep and after waking up from light sleep.
> Note. This option can only control the automatic Light-Sleep behavior. If esp_light_sleep_start() is
> called manually from the program, enabling this option will not prevent light sleep entry even if the
> USB Serial/JTAG is in use.






#### Ethernet


Contains:


* CONFIG_ETH_TRANSMIT_MUTEX
* CONFIG_ETH_USE_ESP32_EMAC
* CONFIG_ETH_USE_OPENETH
* CONFIG_ETH_USE_SPI_ETHERNET



##### CONFIG_ETH_USE_ESP32_EMAC



> Support ESP32 internal EMAC controller
> 
> 
> *Found in:* Component config > Ethernet
> 
> 
> ESP32 integrates a 10/100M Ethernet MAC controller.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_EMAC_SUPPORTED


Contains:


* CONFIG_ETH_DMA_RX_BUFFER_NUM
* CONFIG_ETH_DMA_TX_BUFFER_NUM
* CONFIG_ETH_IRAM_OPTIMIZATION
* CONFIG_ETH_SOFT_FLOW_CONTROL
* CONFIG_ETH_DMA_BUFFER_SIZE
* CONFIG_ETH_PHY_INTERFACE



###### CONFIG_ETH_PHY_INTERFACE



> PHY interface
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> Select the communication interface between MAC and PHY chip.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Reduced Media Independent Interface (RMII) (CONFIG_ETH_PHY_INTERFACE_RMII)




###### CONFIG_ETH_DMA_BUFFER_SIZE



> Ethernet DMA buffer size (Byte)
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> Set the size of each buffer used by Ethernet MAC DMA.
> !! Important !! Make sure it is 64B aligned for ESP32P4!
> 
> 
> 
> Range:* from 256 to 1600 if CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> 
> Default value:* 512 if CONFIG_ETH_USE_ESP32_EMAC




###### CONFIG_ETH_DMA_RX_BUFFER_NUM



> Amount of Ethernet DMA Rx buffers
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> Number of DMA receive buffers. Each buffer's size is ETH_DMA_BUFFER_SIZE.
> Larger number of buffers could increase throughput somehow.
> 
> 
> 
> Range:* from 3 to 30 if CONFIG_ETH_USE_ESP32_EMAC




###### CONFIG_ETH_DMA_TX_BUFFER_NUM



> Amount of Ethernet DMA Tx buffers
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> Number of DMA transmit buffers. Each buffer's size is ETH_DMA_BUFFER_SIZE.
> Larger number of buffers could increase throughput somehow.
> 
> 
> 
> Range:* from 3 to 30 if CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> 
> Default value:* 10 if CONFIG_ETH_USE_ESP32_EMAC




###### CONFIG_ETH_SOFT_FLOW_CONTROL



> Enable software flow control
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> Ethernet MAC engine on ESP32 doesn't feature a flow control logic.
> The MAC driver can perform a software flow control if you enable this option.
> Note that, if the RX buffer number is small, enabling software flow control will
> cause obvious performance loss.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ETH_DMA_RX_BUFFER_NUM > 15 && CONFIG_ETH_USE_ESP32_EMAC




###### CONFIG_ETH_IRAM_OPTIMIZATION



> Enable IRAM optimization
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_ESP32_EMAC
> 
> 
> If enabled, functions related to RX/TX are placed into IRAM. It can improve Ethernet throughput.
> If disabled, all functions are placed into FLASH.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ETH_USE_ESP32_EMAC





##### CONFIG_ETH_USE_SPI_ETHERNET



> Support SPI to Ethernet Module
> 
> 
> *Found in:* Component config > Ethernet
> 
> 
> ESP-IDF can also support some SPI-Ethernet modules.
> 
> 
> 
> Default value:* Yes (enabled)


Contains:


* CONFIG_ETH_SPI_ETHERNET_DM9051
* CONFIG_ETH_SPI_ETHERNET_KSZ8851SNL
* CONFIG_ETH_SPI_ETHERNET_W5500



###### CONFIG_ETH_SPI_ETHERNET_DM9051



> Use DM9051
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_SPI_ETHERNET
> 
> 
> DM9051 is a fast Ethernet controller with an SPI interface.
> It's also integrated with a 10/100M PHY and MAC.
> Select this to enable DM9051 driver.




###### CONFIG_ETH_SPI_ETHERNET_W5500



> Use W5500 (MAC RAW)
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_SPI_ETHERNET
> 
> 
> W5500 is a HW TCP/IP embedded Ethernet controller.
> TCP/IP stack, 10/100 Ethernet MAC and PHY are embedded in a single chip.
> However the driver in ESP-IDF only enables the RAW MAC mode,
> making it compatible with the software TCP/IP stack.
> Say yes to enable W5500 driver.




###### CONFIG_ETH_SPI_ETHERNET_KSZ8851SNL



> Use KSZ8851SNL
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_SPI_ETHERNET
> 
> 
> The KSZ8851SNL is a single-chip Fast Ethernet controller consisting of
> a 10/100 physical layer transceiver (PHY), a MAC, and a Serial Peripheral Interface (SPI).
> Select this to enable KSZ8851SNL driver.





##### CONFIG_ETH_USE_OPENETH



> Support OpenCores Ethernet MAC (for use with QEMU)
> 
> 
> *Found in:* Component config > Ethernet
> 
> 
> OpenCores Ethernet MAC driver can be used when an ESP-IDF application
> is executed in QEMU. This driver is not supported when running on a
> real chip.
> 
> 
> 
> Default value:* No (disabled)


Contains:


* CONFIG_ETH_OPENETH_DMA_RX_BUFFER_NUM
* CONFIG_ETH_OPENETH_DMA_TX_BUFFER_NUM



###### CONFIG_ETH_OPENETH_DMA_RX_BUFFER_NUM



> Number of Ethernet DMA Rx buffers
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_OPENETH
> 
> 
> Number of DMA receive buffers, each buffer is 1600 bytes.
> 
> 
> 
> Range:* from 1 to 64 if CONFIG_ETH_USE_OPENETH
> 
> 
> 
> Default value:* 4 if CONFIG_ETH_USE_OPENETH




###### CONFIG_ETH_OPENETH_DMA_TX_BUFFER_NUM



> Number of Ethernet DMA Tx buffers
> 
> 
> *Found in:* Component config > Ethernet > CONFIG_ETH_USE_OPENETH
> 
> 
> Number of DMA transmit buffers, each buffer is 1600 bytes.
> 
> 
> 
> Range:* from 1 to 64 if CONFIG_ETH_USE_OPENETH
> 
> 
> 
> Default value:* 1 if CONFIG_ETH_USE_OPENETH





##### CONFIG_ETH_TRANSMIT_MUTEX



> Enable Transmit Mutex
> 
> 
> *Found in:* Component config > Ethernet
> 
> 
> Prevents multiple accesses when Ethernet interface is used as shared resource and multiple
> functionalities might try to access it at a time.
> 
> 
> 
> Default value:* No (disabled)





#### Event Loop Library


Contains:


* CONFIG_ESP_EVENT_LOOP_PROFILING
* CONFIG_ESP_EVENT_POST_FROM_ISR



##### CONFIG_ESP_EVENT_LOOP_PROFILING



> Enable event loop profiling
> 
> 
> *Found in:* Component config > Event Loop Library
> 
> 
> Enables collections of statistics in the event loop library such as the number of events posted
> to/recieved by an event loop, number of callbacks involved, number of events dropped to to a full event
> loop queue, run time of event handlers, and number of times/run time of each event handler.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_EVENT_POST_FROM_ISR



> Support posting events from ISRs
> 
> 
> *Found in:* Component config > Event Loop Library
> 
> 
> Enable posting events from interrupt handlers.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_EVENT_POST_FROM_IRAM_ISR



> Support posting events from ISRs placed in IRAM
> 
> 
> *Found in:* Component config > Event Loop Library > CONFIG_ESP_EVENT_POST_FROM_ISR
> 
> 
> Enable posting events from interrupt handlers placed in IRAM. Enabling this option places API functions
> esp_event_post and esp_event_post_to in IRAM.
> 
> 
> 
> Default value:* Yes (enabled)






#### GDB Stub


Contains:


* CONFIG_ESP_GDBSTUB_SUPPORT_TASKS
* CONFIG_ESP_SYSTEM_GDBSTUB_RUNTIME



##### CONFIG_ESP_SYSTEM_GDBSTUB_RUNTIME



> GDBStub at runtime
> 
> 
> *Found in:* Component config > GDB Stub
> 
> 
> Enable builtin GDBStub.
> This allows to debug the target device using serial port:
> - Run 'idf.py monitor'.
> - Wait for the device to initialize.
> - Press Ctrl+C to interrupt the execution and enter GDB attached to your device for debugging.
> NOTE: all UART input will be handled by GDBStub.




##### CONFIG_ESP_GDBSTUB_SUPPORT_TASKS



> Enable listing FreeRTOS tasks through GDB Stub
> 
> 
> *Found in:* Component config > GDB Stub
> 
> 
> If enabled, GDBStub can supply the list of FreeRTOS tasks to GDB.
> Thread list can be queried from GDB using 'info threads' command.
> Note that if GDB task lists were corrupted, this feature may not work.
> If GDBStub fails, try disabling this feature.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_GDBSTUB_MAX_TASKS



> Maximum number of tasks supported by GDB Stub
> 
> 
> *Found in:* Component config > GDB Stub > CONFIG_ESP_GDBSTUB_SUPPORT_TASKS
> 
> 
> Set the number of tasks which GDB Stub will support.
> 
> 
> 
> Default value:* 32






#### ESP HTTP client


Contains:


* CONFIG_ESP_HTTP_CLIENT_ENABLE_CUSTOM_TRANSPORT
* CONFIG_ESP_HTTP_CLIENT_ENABLE_BASIC_AUTH
* CONFIG_ESP_HTTP_CLIENT_ENABLE_DIGEST_AUTH
* CONFIG_ESP_HTTP_CLIENT_ENABLE_HTTPS



##### CONFIG_ESP_HTTP_CLIENT_ENABLE_HTTPS



> Enable https
> 
> 
> *Found in:* Component config > ESP HTTP client
> 
> 
> This option will enable https protocol by linking esp-tls library and initializing SSL transport
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_HTTP_CLIENT_ENABLE_BASIC_AUTH



> Enable HTTP Basic Authentication
> 
> 
> *Found in:* Component config > ESP HTTP client
> 
> 
> This option will enable HTTP Basic Authentication. It is disabled by default as Basic
> auth uses unencrypted encoding, so it introduces a vulnerability when not using TLS
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_HTTP_CLIENT_ENABLE_DIGEST_AUTH



> Enable HTTP Digest Authentication
> 
> 
> *Found in:* Component config > ESP HTTP client
> 
> 
> This option will enable HTTP Digest Authentication. It is enabled by default, but use of this
> configuration is not recommended as the password can be derived from the exchange, so it introduces
> a vulnerability when not using TLS
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_HTTP_CLIENT_ENABLE_CUSTOM_TRANSPORT



> Enable custom transport
> 
> 
> *Found in:* Component config > ESP HTTP client
> 
> 
> This option will enable injection of a custom tcp_transport handle, so the http operation
> will be performed on top of the user defined transport abstraction (if configured)
> 
> 
> 
> Default value:* No (disabled)





#### HTTP Server


Contains:


* CONFIG_HTTPD_QUEUE_WORK_BLOCKING
* CONFIG_HTTPD_PURGE_BUF_LEN
* CONFIG_HTTPD_LOG_PURGE_DATA
* CONFIG_HTTPD_MAX_REQ_HDR_LEN
* CONFIG_HTTPD_MAX_URI_LEN
* CONFIG_HTTPD_ERR_RESP_NO_DELAY
* CONFIG_HTTPD_WS_SUPPORT



##### CONFIG_HTTPD_MAX_REQ_HDR_LEN



> Max HTTP Request Header Length
> 
> 
> *Found in:* Component config > HTTP Server
> 
> 
> This sets the maximum supported size of headers section in HTTP request packet to be processed by the
> server
> 
> 
> 
> Default value:* 512




##### CONFIG_HTTPD_MAX_URI_LEN



> Max HTTP URI Length
> 
> 
> *Found in:* Component config > HTTP Server
> 
> 
> This sets the maximum supported size of HTTP request URI to be processed by the server
> 
> 
> 
> Default value:* 512




##### CONFIG_HTTPD_ERR_RESP_NO_DELAY



> Use TCP_NODELAY socket option when sending HTTP error responses
> 
> 
> *Found in:* Component config > HTTP Server
> 
> 
> Using TCP_NODEALY socket option ensures that HTTP error response reaches the client before the
> underlying socket is closed. Please note that turning this off may cause multiple test failures
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_HTTPD_PURGE_BUF_LEN



> Length of temporary buffer for purging data
> 
> 
> *Found in:* Component config > HTTP Server
> 
> 
> This sets the size of the temporary buffer used to receive and discard any remaining data that is
> received from the HTTP client in the request, but not processed as part of the server HTTP request
> handler.
> 
> 
> If the remaining data is larger than the available buffer size, the buffer will be filled in multiple
> iterations. The buffer should be small enough to fit on the stack, but large enough to avoid excessive
> iterations.
> 
> 
> 
> Default value:* 32




##### CONFIG_HTTPD_LOG_PURGE_DATA



> Log purged content data at Debug level
> 
> 
> *Found in:* Component config > HTTP Server
> 
> 
> Enabling this will log discarded binary HTTP request data at Debug level.
> For large content data this may not be desirable as it will clutter the log.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_HTTPD_WS_SUPPORT



> WebSocket server support
> 
> 
> *Found in:* Component config > HTTP Server
> 
> 
> This sets the WebSocket server support.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_HTTPD_QUEUE_WORK_BLOCKING



> httpd_queue_work as blocking API
> 
> 
> *Found in:* Component config > HTTP Server
> 
> 
> This makes httpd_queue_work() API to wait until a message space is available on UDP control socket.
> It internally uses a counting semaphore with count set to LWIP_UDP_RECVMBOX_SIZE to achieve this.
> This config will slightly change API behavior to block until message gets delivered on control socket.





#### ESP HTTPS OTA


Contains:


* CONFIG_ESP_HTTPS_OTA_ALLOW_HTTP
* CONFIG_ESP_HTTPS_OTA_DECRYPT_CB



##### CONFIG_ESP_HTTPS_OTA_DECRYPT_CB



> Provide decryption callback
> 
> 
> *Found in:* Component config > ESP HTTPS OTA
> 
> 
> Exposes an additional callback whereby firmware data could be decrypted
> before being processed by OTA update component. This can help to integrate
> external encryption related format and removal of such encapsulation layer
> from firmware image.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_HTTPS_OTA_ALLOW_HTTP



> Allow HTTP for OTA (WARNING: ONLY FOR TESTING PURPOSE, READ HELP)
> 
> 
> *Found in:* Component config > ESP HTTPS OTA
> 
> 
> It is highly recommended to keep HTTPS (along with server certificate validation) enabled.
> Enabling this option comes with potential risk of:
> - Non-encrypted communication channel with server
> - Accepting firmware upgrade image from server with fake identity
> 
> 
> 
> Default value:* No (disabled)





#### ESP HTTPS server


Contains:


* CONFIG_ESP_HTTPS_SERVER_ENABLE



##### CONFIG_ESP_HTTPS_SERVER_ENABLE



> Enable ESP_HTTPS_SERVER component
> 
> 
> *Found in:* Component config > ESP HTTPS server
> 
> 
> Enable ESP HTTPS server component





#### Hardware Settings


Contains:


* 2D-DMA Configurations
* Chip revision
* Crypto DPA Protection
* DW_GDMA Configurations
* ETM Configuration
* GDMA Configurations
* MAC Config
* Main XTAL Config
* Peripheral Control
* RTC Clock Config
* Sleep Config



##### Chip revision


Contains:


* CONFIG_ESP_REV_NEW_CHIP_TEST
* CONFIG_ESP32S3_REV_MIN



###### CONFIG_ESP32S3_REV_MIN



> Minimum Supported ESP32-S3 Revision
> 
> 
> *Found in:* Component config > Hardware Settings > Chip revision
> 
> 
> Required minimum chip revision. ESP-IDF will check for it and
> reject to boot if the chip revision fails the check.
> This ensures the chip used will have some modifications (features, or bugfixes).
> 
> 
> The complied binary will only support chips above this revision,
> this will also help to reduce binary size.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Rev v0.0 (ECO0) (CONFIG_ESP32S3_REV_MIN_0)
> > * Rev v0.1 (ECO1) (CONFIG_ESP32S3_REV_MIN_1)
> > * Rev v0.2 (ECO2) (CONFIG_ESP32S3_REV_MIN_2)




###### CONFIG_ESP_REV_NEW_CHIP_TEST



> Internal test mode
> 
> 
> *Found in:* Component config > Hardware Settings > Chip revision
> 
> 
> For internal chip testing, a small number of new versions chips didn't
> update the version field in eFuse, you can enable this option to force the
> software recognize the chip version based on the rev selected in menuconfig.





##### MAC Config


Contains:


* CONFIG_ESP_MAC_USE_CUSTOM_MAC_AS_BASE_MAC
* CONFIG_ESP32S3_UNIVERSAL_MAC_ADDRESSES



###### CONFIG_ESP32S3_UNIVERSAL_MAC_ADDRESSES



> Number of universally administered (by IEEE) MAC address
> 
> 
> *Found in:* Component config > Hardware Settings > MAC Config
> 
> 
> Configure the number of universally administered (by IEEE) MAC addresses.
> During initialization, MAC addresses for each network interface are generated or derived from a
> single base MAC address.
> If the number of universal MAC addresses is four, all four interfaces (WiFi station, WiFi softap,
> Bluetooth and Ethernet) receive a universally administered MAC address. These are generated
> sequentially by adding 0, 1, 2 and 3 (respectively) to the final octet of the base MAC address.
> If the number of universal MAC addresses is two, only two interfaces (WiFi station and Bluetooth)
> receive a universally administered MAC address. These are generated sequentially by adding 0
> and 1 (respectively) to the base MAC address. The remaining two interfaces (WiFi softap and Ethernet)
> receive local MAC addresses. These are derived from the universal WiFi station and Bluetooth MAC
> addresses, respectively.
> When using the default (Espressif-assigned) base MAC address, either setting can be used. When using
> a custom universal MAC address range, the correct setting will depend on the allocation of MAC
> addresses in this range (either 2 or 4 per device.)
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Two (CONFIG_ESP32S3_UNIVERSAL_MAC_ADDRESSES_TWO)
> > * Four (CONFIG_ESP32S3_UNIVERSAL_MAC_ADDRESSES_FOUR)




###### CONFIG_ESP_MAC_USE_CUSTOM_MAC_AS_BASE_MAC



> Enable using custom mac as base mac
> 
> 
> *Found in:* Component config > Hardware Settings > MAC Config
> 
> 
> When this configuration is enabled, the user can invoke esp_read_mac to obtain the desired type of
> MAC using a custom MAC as the base MAC.
> 
> 
> 
> Default value:* No (disabled)





##### Sleep Config


Contains:


* CONFIG_ESP_SLEEP_GPIO_ENABLE_INTERNAL_RESISTORS
* CONFIG_ESP_SLEEP_CACHE_SAFE_ASSERTION
* CONFIG_ESP_SLEEP_EVENT_CALLBACKS
* CONFIG_ESP_SLEEP_DEBUG
* CONFIG_ESP_SLEEP_WAIT_FLASH_READY_EXTRA_DELAY
* CONFIG_ESP_SLEEP_GPIO_RESET_WORKAROUND
* CONFIG_ESP_SLEEP_POWER_DOWN_FLASH
* CONFIG_ESP_SLEEP_MSPI_NEED_ALL_IO_PU
* CONFIG_ESP_SLEEP_FLASH_LEAKAGE_WORKAROUND
* CONFIG_ESP_SLEEP_PSRAM_LEAKAGE_WORKAROUND



###### CONFIG_ESP_SLEEP_POWER_DOWN_FLASH



> Power down flash in light sleep when there is no SPIRAM
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> If enabled, chip will try to power down flash as part of esp_light_sleep_start(), which costs
> more time when chip wakes up. Can only be enabled if there is no SPIRAM configured.
> 
> 
> This option will power down flash under a strict but relatively safe condition. Also, it is possible to
> power down flash under a relaxed condition by using esp_sleep_pd_config() to set ESP_PD_DOMAIN_VDDSDIO
> to ESP_PD_OPTION_OFF. It should be noted that there is a risk in powering down flash, you can refer
> ESP-IDF Programming Guide/API Reference/System API/Sleep Modes/Power-down of Flash for more details.




###### CONFIG_ESP_SLEEP_FLASH_LEAKAGE_WORKAROUND



> Pull-up Flash CS pin in light sleep
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> All IOs will be set to isolate(floating) state by default during sleep.
> Since the power supply of SPI Flash is not lost during lightsleep, if its CS pin is recognized as
> low level(selected state) in the floating state, there will be a large current leakage, and the
> data in Flash may be corrupted by random signals on other SPI pins.
> Select this option will set the CS pin of Flash to PULL-UP state during sleep, but this will
> increase the sleep current about 10 uA.
> If you are developing with esp32xx modules, you must select this option, but if you are developing
> with chips, you can also pull up the CS pin of SPI Flash in the external circuit to save power
> consumption caused by internal pull-up during sleep.
> (!!! Don't deselect this option if you don't have external SPI Flash CS pin pullups.)




###### CONFIG_ESP_SLEEP_PSRAM_LEAKAGE_WORKAROUND



> Pull-up PSRAM CS pin in light sleep
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> All IOs will be set to isolate(floating) state by default during sleep.
> Since the power supply of PSRAM is not lost during lightsleep, if its CS pin is recognized as
> low level(selected state) in the floating state, there will be a large current leakage, and the
> data in PSRAM may be corrupted by random signals on other SPI pins.
> Select this option will set the CS pin of PSRAM to PULL-UP state during sleep, but this will
> increase the sleep current about 10 uA.
> If you are developing with esp32xx modules, you must select this option, but if you are developing
> with chips, you can also pull up the CS pin of PSRAM in the external circuit to save power
> consumption caused by internal pull-up during sleep.
> (!!! Don't deselect this option if you don't have external PSRAM CS pin pullups.)
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_SPIRAM




###### CONFIG_ESP_SLEEP_MSPI_NEED_ALL_IO_PU



> Pull-up all SPI pins in light sleep
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> To reduce leakage current, some types of SPI Flash/RAM only need to pull up the CS pin
> during light sleep. But there are also some kinds of SPI Flash/RAM that need to pull up
> all pins. It depends on the SPI Flash/RAM chip used.




###### CONFIG_ESP_SLEEP_GPIO_RESET_WORKAROUND



> light sleep GPIO reset workaround
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> esp32c2, esp32c3, esp32s3, esp32c6 and esp32h2 will reset at wake-up if GPIO is received
> a small electrostatic pulse during light sleep, with specific condition
> 
> 
> * GPIO needs to be configured as input-mode only
> * The pin receives a small electrostatic pulse, and reset occurs when the pulse
> voltage is higher than 6 V
> 
> 
> For GPIO set to input mode only, it is not a good practice to leave it open/floating,
> The hardware design needs to controlled it with determined supply or ground voltage
> is necessary.
> 
> 
> This option provides a software workaround for this issue. Configure to isolate all
> GPIO pins in sleep state.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_SLEEP_WAIT_FLASH_READY_EXTRA_DELAY



> Extra delay (in us) after flash powerdown sleep wakeup to wait flash ready
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> When the chip exits sleep, the CPU and the flash chip are powered on at the same time.
> CPU will run rom code (deepsleep) or ram code (lightsleep) first, and then load or execute
> code from flash.
> 
> 
> Some flash chips need sufficient time to pass between power on and first read operation.
> By default, without any extra delay, this time is approximately 900us, although
> some flash chip types need more than that.
> 
> 
> (!!! Please adjust this value according to the Data Sheet of SPI Flash used in your project.)
> In Flash Data Sheet, the parameters that define the Flash ready timing after power-up (minimum
> time from Vcc(min) to CS activeare) usually named tVSL in ELECTRICAL CHARACTERISTICS chapter,
> and the configuration value here should be:
> ESP_SLEEP_WAIT_FLASH_READY_EXTRA_DELAY = tVSL - 900
> 
> 
> For esp32 and esp32s3, the default extra delay is set to 2000us. When optimizing startup time
> for applications which require it, this value may be reduced.
> 
> 
> If you are seeing "flash read err, 1000" message printed to the console after deep sleep reset
> on esp32, or triggered RTC_WDT/LP_WDT after lightsleep wakeup, try increasing this value.
> (For esp32, the delay will be executed in both deep sleep and light sleep wake up flow.
> For chips after esp32, the delay will be executed only in light sleep flow, the delay
> controlled by the EFUSE_FLASH_TPUW in ROM will be executed in deepsleep wake up flow.)
> 
> 
> 
> Range:* from 0 to 5000
> 
> 
> 
> Default value:* 2000
> * 0




###### CONFIG_ESP_SLEEP_CACHE_SAFE_ASSERTION



> Check the cache safety of the sleep wakeup code in sleep process
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> Enabling it will check the cache safety of the code before the flash power is ready after
> light sleep wakeup, and check PM_SLP_IRAM_OPT related code cache safety. This option is
> only for code quality inspection. Enabling it will increase the time overhead of entering
> and exiting sleep. It is not recommended to enable it in the release version.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_ESP_SLEEP_DEBUG



> esp sleep debug
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> Enable esp sleep debug.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_ESP_SLEEP_GPIO_ENABLE_INTERNAL_RESISTORS



> Allow to enable internal pull-up/downs for the Deep-Sleep wakeup IOs
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> When using rtc gpio wakeup source during deepsleep without external pull-up/downs, you may want to
> make use of the internal ones.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_SLEEP_EVENT_CALLBACKS



> Enable registration of sleep event callbacks
> 
> 
> *Found in:* Component config > Hardware Settings > Sleep Config
> 
> 
> If enabled, it allows user to register sleep event callbacks. It is primarily designed for internal
> developers and customers can use PM_LIGHT_SLEEP_CALLBACKS as an alternative.
> 
> 
> NOTE: These callbacks are executed from the IDLE task context hence you cannot have any blocking calls
> in your callbacks.
> 
> 
> NOTE: Enabling these callbacks may change sleep duration calculations based on time spent in
> callback and hence it is highly recommended to keep them as short as possible.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_FREERTOS_USE_TICKLESS_IDLE





##### RTC Clock Config


Contains:


* CONFIG_RTC_CLK_CAL_CYCLES
* CONFIG_RTC_CLK_SRC



###### CONFIG_RTC_CLK_SRC



> RTC clock source
> 
> 
> *Found in:* Component config > Hardware Settings > RTC Clock Config
> 
> 
> Choose which clock is used as RTC clock source.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Internal 136kHz RC oscillator (CONFIG_RTC_CLK_SRC_INT_RC)
> > * External 32kHz crystal (CONFIG_RTC_CLK_SRC_EXT_CRYS)
> > * External 32kHz oscillator at 32K_XP pin (CONFIG_RTC_CLK_SRC_EXT_OSC)
> > * Internal 17.5MHz oscillator, divided by 256 (CONFIG_RTC_CLK_SRC_INT_8MD256)




###### CONFIG_RTC_CLK_CAL_CYCLES



> Number of cycles for RTC_SLOW_CLK calibration
> 
> 
> *Found in:* Component config > Hardware Settings > RTC Clock Config
> 
> 
> When the startup code initializes RTC_SLOW_CLK, it can perform
> calibration by comparing the RTC_SLOW_CLK frequency with main XTAL
> frequency. This option sets the number of RTC_SLOW_CLK cycles measured
> by the calibration routine. Higher numbers increase calibration
> precision, which may be important for applications which spend a lot of
> time in deep sleep. Lower numbers reduce startup time.
> 
> 
> When this option is set to 0, clock calibration will not be performed at
> startup, and approximate clock frequencies will be assumed:
> 
> 
> * 150000 Hz if internal RC oscillator is used as clock source. For this use value 1024.
> * 32768 Hz if the 32k crystal oscillator is used. For this use value 3000 or more.In case more value will help improve the definition of the launch of the crystal.
> If the crystal could not start, it will be switched to internal RC.
> 
> 
> 
> Range:* from 0 to 8190 if CONFIG_RTC_CLK_SRC_EXT_CRYS || CONFIG_RTC_CLK_SRC_EXT_OSC || CONFIG_RTC_CLK_SRC_INT_8MD256
> * from 0 to 32766
> 
> 
> 
> Default value:* 3000 if CONFIG_RTC_CLK_SRC_EXT_CRYS || CONFIG_RTC_CLK_SRC_EXT_OSC || CONFIG_RTC_CLK_SRC_INT_8MD256
> * 1024





##### Peripheral Control


Contains:


* CONFIG_PERIPH_CTRL_FUNC_IN_IRAM



###### CONFIG_PERIPH_CTRL_FUNC_IN_IRAM



> Place peripheral control functions into IRAM
> 
> 
> *Found in:* Component config > Hardware Settings > Peripheral Control
> 
> 
> Place peripheral control functions (e.g. periph_module_reset) into IRAM,
> so that these functions can be IRAM-safe and able to be called in the other IRAM interrupt context.
> 
> 
> 
> Default value:* No (disabled)





##### ETM Configuration


Contains:


* CONFIG_ETM_ENABLE_DEBUG_LOG



###### CONFIG_ETM_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > Hardware Settings > ETM Configuration
> 
> 
> whether to enable the debug log message for ETM core driver.
> Note that, this option only controls the ETM related driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled) if SOC_ETM_SUPPORTED





##### GDMA Configurations


Contains:


* CONFIG_GDMA_ENABLE_DEBUG_LOG
* CONFIG_GDMA_ISR_IRAM_SAFE
* CONFIG_GDMA_CTRL_FUNC_IN_IRAM



###### CONFIG_GDMA_CTRL_FUNC_IN_IRAM



> Place GDMA control functions in IRAM
> 
> 
> *Found in:* Component config > Hardware Settings > GDMA Configurations
> 
> 
> Place GDMA control functions (like start/stop/append/reset) into IRAM,
> so that these functions can be IRAM-safe and able to be called in the other IRAM interrupt context.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_GDMA_ISR_IRAM_SAFE



> GDMA ISR IRAM-Safe
> 
> 
> *Found in:* Component config > Hardware Settings > GDMA Configurations
> 
> 
> This will ensure the GDMA interrupt handler is IRAM-Safe, allow to avoid flash
> cache misses, and also be able to run whilst the cache is disabled.
> (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_GDMA_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > Hardware Settings > GDMA Configurations
> 
> 
> Whether to enable the debug log message for GDMA driver.
> Note that, this option only controls the GDMA driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)





##### DW_GDMA Configurations


Contains:


* CONFIG_DW_GDMA_ENABLE_DEBUG_LOG



###### CONFIG_DW_GDMA_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > Hardware Settings > DW_GDMA Configurations
> 
> 
> Whether to enable the debug log message for DW_GDMA driver.
> Note that, this option only controls the DW_GDMA driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled) if SOC_DW_GDMA_SUPPORTED





##### 2D-DMA Configurations


Contains:


* CONFIG_DMA2D_ISR_IRAM_SAFE
* CONFIG_DMA2D_OPERATION_FUNC_IN_IRAM



###### CONFIG_DMA2D_OPERATION_FUNC_IN_IRAM



> Place 2D-DMA operation functions into IRAM
> 
> 
> *Found in:* Component config > Hardware Settings > 2D-DMA Configurations
> 
> 
> Place 2D-DMA all operation functions, including control functions (e.g. start/stop/append/reset) and setter
> functions (e.g. connect/strategy/callback registration) into IRAM, so that these functions can be IRAM-safe
> and able to be called in the other IRAM interrupt context. It also helps optimizing the performance.
> 
> 
> 
> Default value:* No (disabled) if SOC_DMA2D_SUPPORTED




###### CONFIG_DMA2D_ISR_IRAM_SAFE



> 2D-DMA ISR IRAM-Safe
> 
> 
> *Found in:* Component config > Hardware Settings > 2D-DMA Configurations
> 
> 
> This will ensure the 2D-DMA interrupt handler is IRAM-Safe, allow to avoid flash
> cache misses, and also be able to run whilst the cache is disabled.
> (e.g. SPI Flash write).
> 
> 
> 
> Default value:* No (disabled) if SOC_DMA2D_SUPPORTED





##### Main XTAL Config


Contains:


* CONFIG_XTAL_FREQ_SEL



###### CONFIG_XTAL_FREQ_SEL



> Main XTAL frequency
> 
> 
> *Found in:* Component config > Hardware Settings > Main XTAL Config
> 
> 
> This option selects the operating frequency of the XTAL (crystal) clock used to drive the ESP target.
> The selected value MUST reflect the frequency of the given hardware.
> 
> 
> Note: The XTAL_FREQ_AUTO option allows the ESP target to automatically estimating XTAL clock's
> operating frequency. However, this feature is only supported on the ESP32. The ESP32 uses the
> internal 8MHZ as a reference when estimating. Due to the internal oscillator's frequency being
> temperature dependent, usage of the XTAL_FREQ_AUTO is not recommended in applications that operate
> in high ambient temperatures or use high-temperature qualified chips and modules.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 24 MHz (CONFIG_XTAL_FREQ_24)
> > * 26 MHz (CONFIG_XTAL_FREQ_26)
> > * 32 MHz (CONFIG_XTAL_FREQ_32)
> > * 40 MHz (CONFIG_XTAL_FREQ_40)
> > * 48 MHz (CONFIG_XTAL_FREQ_48)
> > * Autodetect (CONFIG_XTAL_FREQ_AUTO)





##### Crypto DPA Protection


Contains:


* CONFIG_ESP_CRYPTO_DPA_PROTECTION_AT_STARTUP



###### CONFIG_ESP_CRYPTO_DPA_PROTECTION_AT_STARTUP



> Enable crypto DPA protection at startup
> 
> 
> *Found in:* Component config > Hardware Settings > Crypto DPA Protection
> 
> 
> This config controls the DPA (Differential Power Analysis) protection
> knob for the crypto peripherals. DPA protection dynamically adjusts the
> clock frequency of the crypto peripheral. DPA protection helps to make it
> difficult to perform SCA attacks on the crypto peripherals. However,
> there is also associated performance impact based on the security level
> set. Please refer to the TRM for more details.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_CRYPTO_DPA_PROTECTION_SUPPORTED




###### CONFIG_ESP_CRYPTO_DPA_PROTECTION_LEVEL



> DPA protection level
> 
> 
> *Found in:* Component config > Hardware Settings > Crypto DPA Protection > CONFIG_ESP_CRYPTO_DPA_PROTECTION_AT_STARTUP
> 
> 
> Configure the DPA protection security level
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Security level low (CONFIG_ESP_CRYPTO_DPA_PROTECTION_LEVEL_LOW)
> > * Security level medium (CONFIG_ESP_CRYPTO_DPA_PROTECTION_LEVEL_MEDIUM)
> > * Security level high (CONFIG_ESP_CRYPTO_DPA_PROTECTION_LEVEL_HIGH)






#### LCD and Touch Panel


Contains:


* LCD Peripheral Configuration



##### LCD Peripheral Configuration


Contains:


* CONFIG_LCD_DSI_ISR_IRAM_SAFE
* CONFIG_LCD_ENABLE_DEBUG_LOG
* CONFIG_LCD_PANEL_IO_FORMAT_BUF_SIZE
* CONFIG_LCD_RGB_RESTART_IN_VSYNC
* CONFIG_LCD_RGB_ISR_IRAM_SAFE



###### CONFIG_LCD_PANEL_IO_FORMAT_BUF_SIZE



> LCD panel io format buffer size
> 
> 
> *Found in:* Component config > LCD and Touch Panel > LCD Peripheral Configuration
> 
> 
> LCD driver allocates an internal buffer to transform the data into a proper format, because of
> the endian order mismatch. This option is to set the size of the buffer, in bytes.
> 
> 
> 
> Default value:* 32




###### CONFIG_LCD_ENABLE_DEBUG_LOG



> Enable debug log
> 
> 
> *Found in:* Component config > LCD and Touch Panel > LCD Peripheral Configuration
> 
> 
> whether to enable the debug log message for LCD driver.
> Note that, this option only controls the LCD driver log, won't affect other drivers.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LCD_RGB_ISR_IRAM_SAFE



> RGB LCD ISR IRAM-Safe
> 
> 
> *Found in:* Component config > LCD and Touch Panel > LCD Peripheral Configuration
> 
> 
> Ensure the LCD interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> If you want the LCD driver to keep flushing the screen even when cache ops disabled,
> you can enable this option. Note, this will also increase the IRAM usage.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LCD_RGB_RESTART_IN_VSYNC



> Restart transmission in VSYNC
> 
> 
> *Found in:* Component config > LCD and Touch Panel > LCD Peripheral Configuration
> 
> 
> Reset the GDMA channel every VBlank to stop permanent desyncs from happening.
> Only need to enable it when in your application, the DMA can't deliver data
> as fast as the LCD consumes it.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LCD_DSI_ISR_IRAM_SAFE



> DSI LCD ISR IRAM-Safe
> 
> 
> *Found in:* Component config > LCD and Touch Panel > LCD Peripheral Configuration
> 
> 
> Ensure the LCD interrupt is IRAM-Safe by allowing the interrupt handler to be
> executable when the cache is disabled (e.g. SPI Flash write).
> If you want the LCD driver to keep flushing the screen even when cache ops disabled,
> you can enable this option. Note, this will also increase the IRAM usage.
> 
> 
> 
> Default value:* No (disabled) if SOC_MIPI_DSI_SUPPORTED






#### ESP NETIF Adapter


Contains:


* CONFIG_ESP_NETIF_BRIDGE_EN
* CONFIG_ESP_NETIF_L2_TAP
* CONFIG_ESP_NETIF_IP_LOST_TIMER_INTERVAL
* CONFIG_ESP_NETIF_USE_TCPIP_STACK_LIB
* CONFIG_ESP_NETIF_RECEIVE_REPORT_ERRORS



##### CONFIG_ESP_NETIF_IP_LOST_TIMER_INTERVAL



> IP Address lost timer interval (seconds)
> 
> 
> *Found in:* Component config > ESP NETIF Adapter
> 
> 
> The value of 0 indicates the IP lost timer is disabled, otherwise the timer is enabled.
> 
> 
> The IP address may be lost because of some reasons, e.g. when the station disconnects
> from soft-AP, or when DHCP IP renew fails etc. If the IP lost timer is enabled, it will
> be started everytime the IP is lost. Event SYSTEM_EVENT_STA_LOST_IP will be raised if
> the timer expires. The IP lost timer is stopped if the station get the IP again before
> the timer expires.
> 
> 
> 
> Range:* from 0 to 65535
> 
> 
> 
> Default value:* 120




##### CONFIG_ESP_NETIF_USE_TCPIP_STACK_LIB



> TCP/IP Stack Library
> 
> 
> *Found in:* Component config > ESP NETIF Adapter
> 
> 
> Choose the TCP/IP Stack to work, for example, LwIP, uIP, etc.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * LwIP (CONFIG_ESP_NETIF_TCPIP_LWIP)
> > 
> > 
> > lwIP is a small independent implementation of the TCP/IP protocol suite.
> > * Loopback (CONFIG_ESP_NETIF_LOOPBACK)
> > 
> > 
> > Dummy implementation of esp-netif functionality which connects driver transmit
> > to receive function. This option is for testing purpose only




##### CONFIG_ESP_NETIF_RECEIVE_REPORT_ERRORS



> Use esp_err_t to report errors from esp_netif_receive
> 
> 
> *Found in:* Component config > ESP NETIF Adapter
> 
> 
> Enable if esp_netif_receive() should return error code. This is useful to inform upper layers
> that packet input to TCP/IP stack failed, so the upper layers could implement flow control.
> This option is disabled by default due to backward compatibility and will be enabled in v6.0 (IDF-7194)
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_NETIF_L2_TAP



> Enable netif L2 TAP support
> 
> 
> *Found in:* Component config > ESP NETIF Adapter
> 
> 
> A user program can read/write link layer (L2) frames from/to ESP TAP device.
> The ESP TAP device can be currently associated only with Ethernet physical interfaces.



###### CONFIG_ESP_NETIF_L2_TAP_MAX_FDS



> Maximum number of opened L2 TAP File descriptors
> 
> 
> *Found in:* Component config > ESP NETIF Adapter > CONFIG_ESP_NETIF_L2_TAP
> 
> 
> Maximum number of opened File descriptors (FD's) associated with ESP TAP device. ESP TAP FD's take up
> a certain amount of memory, and allowing fewer FD's to be opened at the same time conserves memory.
> 
> 
> 
> Range:* from 1 to 10 if CONFIG_ESP_NETIF_L2_TAP
> 
> 
> 
> Default value:* 5 if CONFIG_ESP_NETIF_L2_TAP




###### CONFIG_ESP_NETIF_L2_TAP_RX_QUEUE_SIZE



> Size of L2 TAP Rx queue
> 
> 
> *Found in:* Component config > ESP NETIF Adapter > CONFIG_ESP_NETIF_L2_TAP
> 
> 
> Maximum number of frames queued in opened File descriptor. Once the queue is full, the newly arriving
> frames are dropped until the queue has enough room to accept incoming traffic (Tail Drop queue
> management).
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_ESP_NETIF_L2_TAP
> 
> 
> 
> Default value:* 20 if CONFIG_ESP_NETIF_L2_TAP





##### CONFIG_ESP_NETIF_BRIDGE_EN



> Enable LwIP IEEE 802.1D bridge
> 
> 
> *Found in:* Component config > ESP NETIF Adapter
> 
> 
> Enable LwIP IEEE 802.1D bridge support in ESP-NETIF. Note that "Number of clients store data in netif"
> (LWIP_NUM_NETIF_CLIENT_DATA) option needs to be properly configured to be LwIP bridge avaiable!
> 
> 
> 
> Default value:* No (disabled)





#### Partition API Configuration




#### PHY


Contains:


* CONFIG_ESP_PHY_CALIBRATION_MODE
* CONFIG_ESP_PHY_PLL_TRACK_DEBUG
* CONFIG_ESP_PHY_ENABLE_CERT_TEST
* CONFIG_ESP_PHY_IMPROVE_RX_11B
* CONFIG_ESP_PHY_ENABLE_USB
* CONFIG_ESP_PHY_MAX_WIFI_TX_POWER
* CONFIG_ESP_PHY_MAC_BB_PD
* CONFIG_ESP_PHY_REDUCE_TX_POWER
* CONFIG_ESP_PHY_CALIBRATION_AND_DATA_STORAGE
* CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION



##### CONFIG_ESP_PHY_CALIBRATION_AND_DATA_STORAGE



> Store phy calibration data in NVS
> 
> 
> *Found in:* Component config > PHY
> 
> 
> If this option is enabled, NVS will be initialized and calibration data will be loaded from there.
> PHY calibration will be skipped on deep sleep wakeup. If calibration data is not found, full
> calibration will be performed and stored in NVS. Normally, only partial calibration will be performed.
> If this option is disabled, full calibration will be performed.
> 
> 
> If it's easy that your board calibrate bad data, choose 'n'.
> Two cases for example, you should choose 'n':
> 1.If your board is easy to be booted up with antenna disconnected.
> 2.Because of your board design, each time when you do calibration, the result are too unstable.
> If unsure, choose 'y'.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION



> Use a partition to store PHY init data
> 
> 
> *Found in:* Component config > PHY
> 
> 
> If enabled, PHY init data will be loaded from a partition.
> When using a custom partition table, make sure that PHY data
> partition is included (type: 'data', subtype: 'phy').
> With default partition tables, this is done automatically.
> If PHY init data is stored in a partition, it has to be flashed there,
> otherwise runtime error will occur.
> 
> 
> If this option is not enabled, PHY init data will be embedded
> into the application binary.
> 
> 
> If unsure, choose 'n'.
> 
> 
> 
> Default value:* No (disabled)


Contains:


* CONFIG_ESP_PHY_DEFAULT_INIT_IF_INVALID
* CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN



###### CONFIG_ESP_PHY_DEFAULT_INIT_IF_INVALID



> Reset default PHY init data if invalid
> 
> 
> *Found in:* Component config > PHY > CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION
> 
> 
> If enabled, PHY init data will be restored to default if
> it cannot be verified successfully to avoid endless bootloops.
> 
> 
> If unsure, choose 'n'.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION




###### CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN



> Support multiple PHY init data bin
> 
> 
> *Found in:* Component config > PHY > CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION
> 
> 
> If enabled, the corresponding PHY init data type can be automatically switched
> according to the country code. China's PHY init data bin is used by default.
> Can be modified by country information in API esp_wifi_set_country().
> The priority of switching the PHY init data type is:
> 1. Country configured by API esp_wifi_set_country()
> and the parameter policy is WIFI_COUNTRY_POLICY_MANUAL.
> 2. Country notified by the connected AP.
> 3. Country configured by API esp_wifi_set_country()
> and the parameter policy is WIFI_COUNTRY_POLICY_AUTO.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION && CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION




###### CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN_EMBED



> Support embedded multiple phy init data bin to app bin
> 
> 
> *Found in:* Component config > PHY > CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION > CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN
> 
> 
> If enabled, multiple phy init data bin will embedded into app bin
> If not enabled, multiple phy init data bin will still leave alone, and need to be flashed by users.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN && CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION




###### CONFIG_ESP_PHY_INIT_DATA_ERROR



> Terminate operation when PHY init data error
> 
> 
> *Found in:* Component config > PHY > CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION > CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN
> 
> 
> If enabled, when an error occurs while the PHY init data is updated,
> the program will terminate and restart.
> If not enabled, the PHY init data will not be updated when an error occurs.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN && CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION





##### CONFIG_ESP_PHY_MAX_WIFI_TX_POWER



> Max WiFi TX power (dBm)
> 
> 
> *Found in:* Component config > PHY
> 
> 
> Set maximum transmit power for WiFi radio. Actual transmit power for high
> data rates may be lower than this setting.
> 
> 
> 
> Range:* from 10 to 20
> 
> 
> 
> Default value:* 20




##### CONFIG_ESP_PHY_MAC_BB_PD



> Power down MAC and baseband of Wi-Fi and Bluetooth when PHY is disabled
> 
> 
> *Found in:* Component config > PHY
> 
> 
> If enabled, the MAC and baseband of Wi-Fi and Bluetooth will be powered
> down when PHY is disabled. Enabling this setting reduces power consumption
> by a small amount but increases RAM use by approximately 4 KB(Wi-Fi only),
> 2 KB(Bluetooth only) or 5.3 KB(Wi-Fi + Bluetooth).
> 
> 
> 
> Default value:* No (disabled) if CONFIG_FREERTOS_USE_TICKLESS_IDLE




##### CONFIG_ESP_PHY_REDUCE_TX_POWER



> Reduce PHY TX power when brownout reset
> 
> 
> *Found in:* Component config > PHY
> 
> 
> When brownout reset occurs, reduce PHY TX power to keep the code running.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_PHY_ENABLE_USB



> Keep the USB PHY enabled when initializing WiFi
> 
> 
> *Found in:* Component config > PHY
> 
> 
> On some ESP targets, the USB PHY can interfere with WiFi thus lowering WiFi performance.
> As a result, on those affected ESP targets, the ESP PHY library's initialization will automatically
> disable the USB PHY to get best WiFi performance.
> This option controls whether or not the ESP PHY library will keep the USB PHY enabled on
> initialization.
> 
> 
> Note: This option can be disabled to increase WiFi performance. However, disabling this option will
> also mean that the USB PHY cannot be used while WiFi is enabled.
> 
> 
> 
> Default value:* Yes (enabled)
> * No (disabled)




##### CONFIG_ESP_PHY_ENABLE_CERT_TEST



> Enable RF certification test functions
> 
> 
> *Found in:* Component config > PHY
> 
> 
> If enabled, you can use RF certification test APIs.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_PHY_CALIBRATION_MODE



> Calibration mode
> 
> 
> *Found in:* Component config > PHY
> 
> 
> Select PHY calibration mode. During RF initialization, the partial calibration
> method is used by default for RF calibration. Full calibration takes about 100ms
> more than partial calibration. If boot duration is not critical, it is suggested
> to use the full calibration method. No calibration method is only used when the
> device wakes up from deep sleep.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Calibration partial (CONFIG_ESP_PHY_RF_CAL_PARTIAL)
> > * Calibration none (CONFIG_ESP_PHY_RF_CAL_NONE)
> > * Calibration full (CONFIG_ESP_PHY_RF_CAL_FULL)




##### CONFIG_ESP_PHY_IMPROVE_RX_11B



> Improve Wi-Fi receive 11b pkts
> 
> 
> *Found in:* Component config > PHY
> 
> 
> This is a workaround to improve Wi-Fi receive 11b pkts for some modules using AC-DC power supply with
> high interference, enable this option will sacrifice Wi-Fi OFDM receive performance.
> But to guarantee 11b receive performance serves as a bottom line in this case.
> 
> 
> 
> Default value:* No (disabled) if SOC_PHY_IMPROVE_RX_11B




##### CONFIG_ESP_PHY_PLL_TRACK_DEBUG



> Enable pll track logging
> 
> 
> *Found in:* Component config > PHY
> 
> 
> If enabled, there will be some logs while pll tracking
> 
> 
> 
> Default value:* No (disabled)





#### Power Management


Contains:


* CONFIG_PM_LIGHTSLEEP_RTC_OSC_CAL_INTERVAL
* CONFIG_PM_SLP_DISABLE_GPIO
* CONFIG_PM_LIGHT_SLEEP_CALLBACKS
* CONFIG_PM_POWER_DOWN_CPU_IN_LIGHT_SLEEP
* CONFIG_PM_POWER_DOWN_PERIPHERAL_IN_LIGHT_SLEEP
* CONFIG_PM_SLP_IRAM_OPT
* CONFIG_PM_RTOS_IDLE_OPT
* CONFIG_PM_ENABLE



##### CONFIG_PM_ENABLE



> Support for power management
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> If enabled, application is compiled with support for power management.
> This option has run-time overhead (increased interrupt latency,
> longer time to enter idle state), and it also reduces accuracy of
> RTOS ticks and timers used for timekeeping.
> Enable this option if application uses power management APIs.
> 
> 
> 
> Default value:* No (disabled) if __DOXYGEN__



###### CONFIG_PM_DFS_INIT_AUTO



> Enable dynamic frequency scaling (DFS) at startup
> 
> 
> *Found in:* Component config > Power Management > CONFIG_PM_ENABLE
> 
> 
> If enabled, startup code configures dynamic frequency scaling.
> Max CPU frequency is set to DEFAULT_CPU_FREQ_MHZ setting,
> min frequency is set to XTAL frequency.
> If disabled, DFS will not be active until the application
> configures it using esp_pm_configure function.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_PM_ENABLE




###### CONFIG_PM_PROFILING



> Enable profiling counters for PM locks
> 
> 
> *Found in:* Component config > Power Management > CONFIG_PM_ENABLE
> 
> 
> If enabled, esp_pm_\* functions will keep track of the amount of time
> each of the power management locks has been held, and esp_pm_dump_locks
> function will print this information.
> This feature can be used to analyze which locks are preventing the chip
> from going into a lower power state, and see what time the chip spends
> in each power saving mode. This feature does incur some run-time
> overhead, so should typically be disabled in production builds.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_PM_ENABLE




###### CONFIG_PM_TRACE



> Enable debug tracing of PM using GPIOs
> 
> 
> *Found in:* Component config > Power Management > CONFIG_PM_ENABLE
> 
> 
> If enabled, some GPIOs will be used to signal events such as RTOS ticks,
> frequency switching, entry/exit from idle state. Refer to pm_trace.c
> file for the list of GPIOs.
> This feature is intended to be used when analyzing/debugging behavior
> of power management implementation, and should be kept disabled in
> applications.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_PM_ENABLE





##### CONFIG_PM_SLP_IRAM_OPT



> Put lightsleep related codes in internal RAM
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> If enabled, about 2.1KB of lightsleep related source code would be in IRAM and chip would sleep
> longer for 310us at 160MHz CPU frequency most each time.
> This feature is intended to be used when lower power consumption is needed
> while there is enough place in IRAM to place source code.




##### CONFIG_PM_RTOS_IDLE_OPT



> Put RTOS IDLE related codes in internal RAM
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> If enabled, about 180Bytes of RTOS_IDLE related source code would be in IRAM and chip would sleep
> longer for 20us at 160MHz CPU frequency most each time.
> This feature is intended to be used when lower power consumption is needed
> while there is enough place in IRAM to place source code.




##### CONFIG_PM_SLP_DISABLE_GPIO



> Disable all GPIO when chip at sleep
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> This feature is intended to disable all GPIO pins at automantic sleep to get a lower power mode.
> If enabled, chips will disable all GPIO pins at automantic sleep to reduce about 200~300 uA current.
> If you want to specifically use some pins normally as chip wakes when chip sleeps,
> you can call 'gpio_sleep_sel_dis' to disable this feature on those pins.
> You can also keep this feature on and call 'gpio_sleep_set_direction' and 'gpio_sleep_set_pull_mode'
> to have a different GPIO configuration at sleep.
> Warning: If you want to enable this option on ESP32, you should enable GPIO_ESP32_SUPPORT_SWITCH_SLP_PULL
> at first, otherwise you will not be able to switch pullup/pulldown mode.




##### CONFIG_PM_LIGHTSLEEP_RTC_OSC_CAL_INTERVAL



> Calibrate the RTC_FAST/SLOW clock every N times of light sleep
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> The value of this option determines the calibration interval of the RTC_FAST/SLOW clock during sleep when
> power management is enabled. When it is configured as N, the RTC_FAST/SLOW clock will be calibrated
> every N times of lightsleep.
> Decreasing this value will increase the time the chip is in the active state, thereby increasing the
> average power consumption of the chip.
> Increasing this value can reduce the average power consumption, but when the external environment changes
> drastically and the chip RTC_FAST/SLOW oscillator frequency drifts, it may cause system instability.
> 
> 
> 
> Range:* from 1 to 128 if CONFIG_PM_ENABLE
> 
> 
> 
> Default value:* 1 if CONFIG_PM_ENABLE




##### CONFIG_PM_POWER_DOWN_CPU_IN_LIGHT_SLEEP



> Power down CPU in light sleep
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> If enabled, the CPU will be powered down in light sleep,
> ESP chips supports saving and restoring CPU's running context before and after light sleep,
> the feature provides applications with seamless CPU powerdowned lightsleep without user awareness.
> But this will takes up some internal memory.
> On esp32c3 soc, enabling this option will consume 1.68 KB of internal RAM
> and will reduce sleep current consumption by about 100 uA.
> On esp32s3 soc, enabling this option will consume 8.58 KB of internal RAM
> and will reduce sleep current consumption by about 650 uA.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_PM_RESTORE_CACHE_TAGMEM_AFTER_LIGHT_SLEEP



> Restore I/D-cache tag memory after power down CPU light sleep
> 
> 
> *Found in:* Component config > Power Management > CONFIG_PM_POWER_DOWN_CPU_IN_LIGHT_SLEEP
> 
> 
> Cache tag memory and CPU both belong to the CPU power domain.
> ESP chips supports saving and restoring Cache tag memory before and after sleep,
> this feature supports accesses to the external memory that was cached before sleep still
> be cached when the CPU wakes up from a powerdowned CPU lightsleep.
> This option controls the restore method for Cache tag memory in lightsleep.
> If this option is enabled, the I/D-cache tag memory will be backuped to the internal RAM
> before sleep and restored upon wakeup.
> Depending on the the cache configuration, if this option is enabled,
> it will consume up to 9 KB of internal RAM.
> If this option is disabled, all cached data won't be kept after sleep,
> the DCache will be writeback before sleep and invalid all cached data after sleep,
> all accesses to external memory(Flash/PSRAM) will be cache missed after waking up,
> resulting in performance degradation due to increased memory accesses latency.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_PM_POWER_DOWN_PERIPHERAL_IN_LIGHT_SLEEP



> Power down Digital Peripheral in light sleep (EXPERIMENTAL)
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> If enabled, digital peripherals will be powered down in light sleep, it will reduce sleep
> current consumption by about 100 uA. Chip will save/restore register context at sleep/wake
> time to keep the system running. Enabling this option will increase static RAM and heap usage,
> the actual cost depends on the peripherals you have initialized. In order to save/restore the
> context of the necessary hardware for FreeRTOS to run, it will need at least 4.55 KB free heap
> at sleep time. Otherwise sleep will not power down the peripherals.
> 
> 
> Note1: Please use this option with caution, the current IDF does not support the retention of
> all peripherals. When the digital peripherals are powered off and a sleep and wake-up is completed,
> the peripherals that have not saved the running context are equivalent to performing a reset.
> !!! Please confirm the peripherals used in your application and their sleep retention support status
> before enabling this option, peripherals sleep retention driver support status is tracked in
> power_management.rst
> 
> 
> Note2: When this option is enabled simultaneously with FREERTOS_USE_TICKLESS_IDLE, since the UART will
> be powered down, the uart FIFO will be flushed before sleep to avoid data loss, however, this has the
> potential to block the sleep process and cause the wakeup time to be skipped, which will cause the tick
> of freertos to not be compensated correctly when returning from sleep and cause the system to crash.
> To avoid this, you can increase FREERTOS_IDLE_TIME_BEFORE_SLEEP threshold in menuconfig.
> 
> 
> 
> Default value:* No (disabled) if SOC_PM_SUPPORT_TOP_PD && SOC_PAU_SUPPORTED




##### CONFIG_PM_LIGHT_SLEEP_CALLBACKS



> Enable registration of pm light sleep callbacks
> 
> 
> *Found in:* Component config > Power Management
> 
> 
> If enabled, it allows user to register entry and exit callbacks which are called before and after
> entering auto light sleep.
> 
> 
> NOTE: These callbacks are executed from the IDLE task context hence you cannot have any blocking calls
> in your callbacks.
> 
> 
> NOTE: Enabling these callbacks may change sleep duration calculations based on time spent in callback and
> hence it is highly recommended to keep them as short as possible
> 
> 
> 
> Default value:* No (disabled) if CONFIG_FREERTOS_USE_TICKLESS_IDLE





#### ESP PSRAM


Contains:


* CONFIG_SPIRAM



##### CONFIG_SPIRAM



> Support for external, SPI-connected RAM
> 
> 
> *Found in:* Component config > ESP PSRAM
> 
> 
> This enables support for an external SPI RAM chip, connected in parallel with the
> main SPI flash chip.



###### SPI RAM config


Contains:


* CONFIG_SPIRAM_ALLOW_BSS_SEG_EXTERNAL_MEMORY
* CONFIG_SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY
* CONFIG_SPIRAM_XIP_FROM_PSRAM
* CONFIG_SPIRAM_ECC_ENABLE
* CONFIG_SPIRAM_BOOT_INIT
* CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL
* CONFIG_SPIRAM_MODE
* CONFIG_SPIRAM_FETCH_INSTRUCTIONS
* CONFIG_SPIRAM_RODATA
* CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL
* CONFIG_SPIRAM_MEMTEST
* CONFIG_SPIRAM_SPEED
* CONFIG_SPIRAM_USE
* CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP
* CONFIG_SPIRAM_TYPE




###### CONFIG_SPIRAM_MODE



> Mode (QUAD/OCT) of SPI RAM chip in use
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Quad Mode PSRAM (CONFIG_SPIRAM_MODE_QUAD)
> > * Octal Mode PSRAM (CONFIG_SPIRAM_MODE_OCT)




###### CONFIG_SPIRAM_TYPE



> Type of SPIRAM chip in use
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Auto-detect (CONFIG_SPIRAM_TYPE_AUTO)
> > * ESP-PSRAM16 or APS1604 (CONFIG_SPIRAM_TYPE_ESPPSRAM16)
> > * ESP-PSRAM32 (CONFIG_SPIRAM_TYPE_ESPPSRAM32)
> > * ESP-PSRAM64 , LY68L6400 or APS6408 (CONFIG_SPIRAM_TYPE_ESPPSRAM64)




###### CONFIG_SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY



> Allow external memory as an argument to xTaskCreateStatic
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Accessing memory in SPIRAM has certain restrictions, so task stacks allocated by xTaskCreate
> are by default allocated from internal RAM.
> 
> 
> This option allows for passing memory allocated from SPIRAM to be passed to xTaskCreateStatic.
> This should only be used for tasks where the stack is never accessed while the cache is disabled.




###### CONFIG_SPIRAM_XIP_FROM_PSRAM



> Enable Executable in place from (XiP) from PSRAM feature
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Helper for selecting both SPIRAM_FETCH_INSTRUCTIONS and SPIRAM_RODATA




###### CONFIG_SPIRAM_FETCH_INSTRUCTIONS



> Move Instructions in Flash to PSRAM
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> If enabled, instructions in flash will be moved into PSRAM on startup.
> If SPIRAM_RODATA is also enabled, code that requires execution during an MSPI1 Flash operation
> can forgo being placed in IRAM. Therefore codes that need to be executing during Flash
> operation can continue working normally.
> 
> 
> This feature is useful for high throughput peripheral involved applications to improve
> the performance during MSPI1 flash operations. PSRAM access speed is faster than Flash access.
> So the performance is better. (see External RAM documentation for more details).




###### CONFIG_SPIRAM_RODATA



> Move Read-Only Data in Flash to PSRAM
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> If enabled, rodata in flash will be moved into PSRAM on startup.
> If SPIRAM_FETCH_INSTRUCTIONS is also enabled, code that requires execution during an MSPI1 Flash operation
> is not necessary to be placed in IRAM. Therefore codes that need to be executing during Flash
> operation can continue working normally.
> 
> 
> This feature is useful for high throughput peripheral involved applications to improve
> the performance during MSPI1 flash operations. PSRAM access speed is faster than Flash access.
> So the performance is better. (see External RAM documentation for more details).




###### CONFIG_SPIRAM_SPEED



> Set RAM clock speed
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Select the speed for the SPI RAM chip.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 120MHz clock speed (READ DOCS FIRST) (CONFIG_SPIRAM_SPEED_120M)
> > 
> > 
> > 	+ Quad PSRAM 120 MHz is stable.
> > 	+ Octal PSRAM 120 MHz is an experimental feature, it works when
> > 	the temperature is stable.
> > 	
> > 	
> > 	
> > 	> Risks:If your chip powers on at a certain temperature, then after the temperature
> > 	> increases or decreases by approximately 20 Celsius degrees (depending on the
> > 	> chip), the accesses to / from PSRAM will crash randomly.
> > * 80MHz clock speed (CONFIG_SPIRAM_SPEED_80M)
> > * 40Mhz clock speed (CONFIG_SPIRAM_SPEED_40M)




###### CONFIG_SPIRAM_ECC_ENABLE



> Enable SPI RAM ECC
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Enable MSPI Error-Correcting Code function when accessing SPIRAM.
> 
> 
> If enabled, 1/16 of the SPI RAM total size will be reserved for error-correcting code.




###### CONFIG_SPIRAM_BOOT_INIT



> Initialize SPI RAM during startup
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> If this is enabled, the SPI RAM will be enabled during initial boot. Unless you
> have specific requirements, you'll want to leave this enabled so memory allocated
> during boot-up can also be placed in SPI RAM.




###### CONFIG_SPIRAM_IGNORE_NOTFOUND



> Ignore PSRAM when not found
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config > CONFIG_SPIRAM_BOOT_INIT
> 
> 
> Normally, if psram initialization is enabled during compile time but not found at runtime, it
> is seen as an error making the CPU panic. If this is enabled, booting will complete
> but no PSRAM will be available. If PSRAM failed to initialize, the following configs may be affected
> and may need to be corrected manually. SPIRAM_TRY_ALLOCATE_WIFI_LWIP will affect some LWIP and WiFi buffer
> default values and range values. Enable SPIRAM_TRY_ALLOCATE_WIFI_LWIP, ESP_WIFI_AMSDU_TX_ENABLED,
> ESP_WIFI_CACHE_TX_BUFFER_NUM and use static WiFi Tx buffer may cause potential memory exhaustion issues.
> Suggest disable SPIRAM_TRY_ALLOCATE_WIFI_LWIP.
> Suggest disable ESP_WIFI_AMSDU_TX_ENABLED.
> Suggest disable ESP_WIFI_CACHE_TX_BUFFER_NUM, need clear CONFIG_FEATURE_CACHE_TX_BUF_BIT of config->feature_caps.
> Suggest change ESP_WIFI_TX_BUFFER from static to dynamic. Also suggest to adjust some buffer numbers to the
> values used without PSRAM case. Such as, ESP_WIFI_STATIC_TX_BUFFER_NUM, ESP_WIFI_DYNAMIC_TX_BUFFER_NUM.




###### CONFIG_SPIRAM_USE



> SPI RAM access method
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> The SPI RAM can be accessed in multiple methods: by just having it available as an unmanaged
> memory region in the CPU's memory map, by integrating it in the heap as 'special' memory
> needing heap_caps_malloc to allocate, or by fully integrating it making malloc() also able to
> return SPI RAM pointers.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Integrate RAM into memory map (CONFIG_SPIRAM_USE_MEMMAP)
> > * Make RAM allocatable using heap_caps_malloc(..., MALLOC_CAP_SPIRAM) (CONFIG_SPIRAM_USE_CAPS_ALLOC)
> > * Make RAM allocatable using malloc() as well (CONFIG_SPIRAM_USE_MALLOC)




###### CONFIG_SPIRAM_MEMTEST



> Run memory test on SPI RAM initialization
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Runs a rudimentary memory test on initialization. Aborts when memory test fails. Disable this for
> slightly faster startup.




###### CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL



> Maximum malloc() size, in bytes, to always put in internal memory
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> If malloc() is capable of also allocating SPI-connected ram, its allocation strategy will prefer to
> allocate chunks less than this size in internal memory, while allocations larger than this will be
> done from external RAM. If allocation from the preferred region fails, an attempt is made to allocate
> from the non-preferred region instead, so malloc() will not suddenly fail when either internal or
> external memory is full.




###### CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP



> Try to allocate memories of WiFi and LWIP in SPIRAM firstly. If failed, allocate internal memory
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Try to allocate memories of WiFi and LWIP in SPIRAM firstly. If failed, try to allocate internal
> memory then.




###### CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL



> Reserve this amount of bytes for data that specifically needs to be in DMA or internal memory
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> Because the external/internal RAM allocation strategy is not always perfect, it sometimes may happen
> that the internal memory is entirely filled up. This causes allocations that are specifically done in
> internal memory, for example the stack for new tasks or memory to service DMA or have memory that's
> also available when SPI cache is down, to fail. This option reserves a pool specifically for requests
> like that; the memory in this pool is not given out when a normal malloc() is called.
> 
> 
> Set this to 0 to disable this feature.
> 
> 
> Note that because FreeRTOS stacks are forced to internal memory, they will also use this memory pool;
> be sure to keep this in mind when adjusting this value.
> 
> 
> Note also that the DMA reserved pool may not be one single contiguous memory region, depending on the
> configured size and the static memory usage of the app.




###### CONFIG_SPIRAM_ALLOW_BSS_SEG_EXTERNAL_MEMORY



> Allow .bss segment placed in external memory
> 
> 
> *Found in:* Component config > ESP PSRAM > CONFIG_SPIRAM > SPI RAM config
> 
> 
> If enabled, variables with EXT_RAM_BSS_ATTR attribute will be placed in SPIRAM instead of internal DRAM.
> BSS section of lwip, net80211, pp, bt libraries will be automatically placed
> in SPIRAM. BSS sections from other object files and libraries can also be placed in SPIRAM through
> linker fragment scheme extram_bss.
> 
> 
> Note that the variables placed in SPIRAM using EXT_RAM_BSS_ATTR will be zero initialized.






#### ESP Ringbuf


Contains:


* CONFIG_RINGBUF_PLACE_FUNCTIONS_INTO_FLASH



##### CONFIG_RINGBUF_PLACE_FUNCTIONS_INTO_FLASH



> Place non-ISR ringbuf functions into flash
> 
> 
> *Found in:* Component config > ESP Ringbuf
> 
> 
> Place non-ISR ringbuf functions (like xRingbufferCreate/xRingbufferSend) into flash.
> This frees up IRAM, but the functions can no longer be called when the cache is disabled.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_RINGBUF_PLACE_ISR_FUNCTIONS_INTO_FLASH



> Place ISR ringbuf functions into flash
> 
> 
> *Found in:* Component config > ESP Ringbuf > CONFIG_RINGBUF_PLACE_FUNCTIONS_INTO_FLASH
> 
> 
> Place ISR ringbuf functions (like xRingbufferSendFromISR/xRingbufferReceiveFromISR) into flash.
> This frees up IRAM, but the functions can no longer be called when the cache is disabled
> or from an IRAM interrupt context.
> 
> 
> This option is not compatible with ESP-IDF drivers which are configured to
> run the ISR from an IRAM context, e.g. CONFIG_UART_ISR_IN_IRAM.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_RINGBUF_PLACE_FUNCTIONS_INTO_FLASH






#### ESP System Settings


Contains:


* CONFIG_ESP_SYSTEM_RTC_EXT_XTAL_BOOTSTRAP_CYCLES
* Brownout Detector
* Cache config
* CONFIG_ESP_CONSOLE_UART
* CONFIG_ESP_CONSOLE_SECONDARY
* CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ
* CONFIG_ESP_CONSOLE_USB_CDC_SUPPORT_ETS_PRINTF
* CONFIG_ESP_SYSTEM_ALLOW_RTC_FAST_MEM_AS_HEAP
* CONFIG_ESP_TASK_WDT_EN
* CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE
* CONFIG_ESP_SYSTEM_HW_PC_RECORD
* CONFIG_ESP_SYSTEM_HW_STACK_GUARD
* CONFIG_ESP_XT_WDT
* CONFIG_ESP_SYSTEM_CHECK_INT_LEVEL
* CONFIG_ESP_INT_WDT
* CONFIG_ESP_MAIN_TASK_AFFINITY
* CONFIG_ESP_MAIN_TASK_STACK_SIZE
* CONFIG_ESP_DEBUG_OCDAWARE
* Memory
* Memory protection
* CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE
* CONFIG_ESP_DEBUG_STUBS_ENABLE
* CONFIG_ESP_SYSTEM_PANIC
* CONFIG_ESP_SYSTEM_PANIC_REBOOT_DELAY_SECONDS
* CONFIG_ESP_PANIC_HANDLER_IRAM
* CONFIG_ESP_SYSTEM_BBPLL_RECALIB
* CONFIG_ESP_CONSOLE_USB_CDC_RX_BUF_SIZE
* CONFIG_ESP_SYSTEM_EVENT_QUEUE_SIZE
* Trace memory
* CONFIG_ESP_CONSOLE_UART_BAUDRATE
* CONFIG_ESP_CONSOLE_UART_NUM
* CONFIG_ESP_CONSOLE_UART_RX_GPIO
* CONFIG_ESP_CONSOLE_UART_TX_GPIO



##### CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ



> CPU frequency
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> CPU frequency to be set on application startup.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 40 MHz (CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_40)
> > * 80 MHz (CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_80)
> > * 160 MHz (CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_160)
> > * 240 MHz (CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_240)




##### Cache config


Contains:


* CONFIG_ESP32S3_DCACHE_ASSOCIATED_WAYS
* CONFIG_ESP32S3_DATA_CACHE_LINE_SIZE
* CONFIG_ESP32S3_DATA_CACHE_SIZE
* CONFIG_ESP32S3_ICACHE_ASSOCIATED_WAYS
* CONFIG_ESP32S3_INSTRUCTION_CACHE_LINE_SIZE
* CONFIG_ESP32S3_INSTRUCTION_CACHE_SIZE



###### CONFIG_ESP32S3_INSTRUCTION_CACHE_SIZE



> Instruction cache size
> 
> 
> *Found in:* Component config > ESP System Settings > Cache config
> 
> 
> Instruction cache size to be set on application startup.
> If you use 16KB instruction cache rather than 32KB instruction cache,
> then the other 16KB will be managed by heap allocator.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 16KB (CONFIG_ESP32S3_INSTRUCTION_CACHE_16KB)
> > * 32KB (CONFIG_ESP32S3_INSTRUCTION_CACHE_32KB)




###### CONFIG_ESP32S3_ICACHE_ASSOCIATED_WAYS



> Instruction cache associated ways
> 
> 
> *Found in:* Component config > ESP System Settings > Cache config
> 
> 
> Instruction cache associated ways to be set on application startup.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 4 ways (CONFIG_ESP32S3_INSTRUCTION_CACHE_4WAYS)
> > * 8 ways (CONFIG_ESP32S3_INSTRUCTION_CACHE_8WAYS)




###### CONFIG_ESP32S3_INSTRUCTION_CACHE_LINE_SIZE



> Instruction cache line size
> 
> 
> *Found in:* Component config > ESP System Settings > Cache config
> 
> 
> Instruction cache line size to be set on application startup.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 16 Bytes (CONFIG_ESP32S3_INSTRUCTION_CACHE_LINE_16B)
> > * 32 Bytes (CONFIG_ESP32S3_INSTRUCTION_CACHE_LINE_32B)




###### CONFIG_ESP32S3_DATA_CACHE_SIZE



> Data cache size
> 
> 
> *Found in:* Component config > ESP System Settings > Cache config
> 
> 
> Data cache size to be set on application startup.
> If you use 32KB data cache rather than 64KB data cache,
> the other 32KB will be added to the heap.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 16KB (CONFIG_ESP32S3_DATA_CACHE_16KB)
> > * 32KB (CONFIG_ESP32S3_DATA_CACHE_32KB)
> > * 64KB (CONFIG_ESP32S3_DATA_CACHE_64KB)




###### CONFIG_ESP32S3_DCACHE_ASSOCIATED_WAYS



> Data cache associated ways
> 
> 
> *Found in:* Component config > ESP System Settings > Cache config
> 
> 
> Data cache associated ways to be set on application startup.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 4 ways (CONFIG_ESP32S3_DATA_CACHE_4WAYS)
> > * 8 ways (CONFIG_ESP32S3_DATA_CACHE_8WAYS)




###### CONFIG_ESP32S3_DATA_CACHE_LINE_SIZE



> Data cache line size
> 
> 
> *Found in:* Component config > ESP System Settings > Cache config
> 
> 
> Data cache line size to be set on application startup.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 16 Bytes (CONFIG_ESP32S3_DATA_CACHE_LINE_16B)
> > * 32 Bytes (CONFIG_ESP32S3_DATA_CACHE_LINE_32B)
> > * 64 Bytes (CONFIG_ESP32S3_DATA_CACHE_LINE_64B)





##### Memory


Contains:


* CONFIG_ESP32S3_RTCDATA_IN_FAST_MEM
* CONFIG_ESP32S3_USE_FIXED_STATIC_RAM_SIZE



###### CONFIG_ESP32S3_RTCDATA_IN_FAST_MEM



> Place RTC_DATA_ATTR and RTC_RODATA_ATTR variables into RTC fast memory segment
> 
> 
> *Found in:* Component config > ESP System Settings > Memory
> 
> 
> This option allows to place .rtc_data and .rtc_rodata sections into
> RTC fast memory segment to free the slow memory region for ULP programs.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_ESP32S3_USE_FIXED_STATIC_RAM_SIZE



> Use fixed static RAM size
> 
> 
> *Found in:* Component config > ESP System Settings > Memory
> 
> 
> If this option is disabled, the DRAM part of the heap starts right after the .bss section,
> within the dram0_0 region. As a result, adding or removing some static variables
> will change the available heap size.
> 
> 
> If this option is enabled, the DRAM part of the heap starts right after the dram0_0 region,
> where its length is set with ESP32S3_FIXED_STATIC_RAM_SIZE
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_ESP32S3_FIXED_STATIC_RAM_SIZE



> Fixed Static RAM size
> 
> 
> *Found in:* Component config > ESP System Settings > Memory > CONFIG_ESP32S3_USE_FIXED_STATIC_RAM_SIZE
> 
> 
> RAM size dedicated for static variables (.data & .bss sections).
> This value is less than the chips total memory, as not all of it can be used for this purpose.
> E.g. parts are used by the software bootloader, and will only be available
> as heap memory after app startup.
> 
> 
> 
> Range:* from 0 to 0x54700 if CONFIG_ESP32S3_USE_FIXED_STATIC_RAM_SIZE
> 
> 
> 
> Default value:* "0x10000" if CONFIG_ESP32S3_USE_FIXED_STATIC_RAM_SIZE





##### Trace memory


Contains:


* CONFIG_ESP32S3_TRAX



###### CONFIG_ESP32S3_TRAX



> Use TRAX tracing feature
> 
> 
> *Found in:* Component config > ESP System Settings > Trace memory
> 
> 
> The esp32-s3 contains a feature which allows you to trace the execution path the processor
> has taken through the program. This is stored in a chunk of 32K (16K for single-processor)
> of memory that can't be used for general purposes anymore. Disable this if you do not know
> what this is.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_ESP32S3_TRAX_TWOBANKS



> Reserve memory for tracing both pro as well as app cpu execution
> 
> 
> *Found in:* Component config > ESP System Settings > Trace memory > CONFIG_ESP32S3_TRAX
> 
> 
> The esp32-s3 contains a feature which allows you to trace the execution path the processor
> has taken through the program. This is stored in a chunk of 32K (16K for single-processor)
> of memory that can't be used for general purposes anymore. Disable this if you do not know
> what this is.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP32S3_TRAX





##### CONFIG_ESP_SYSTEM_PANIC



> Panic handler behaviour
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> If FreeRTOS detects unexpected behaviour or an unhandled exception, the panic handler is
> invoked. Configure the panic handler's action here.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Print registers and halt (CONFIG_ESP_SYSTEM_PANIC_PRINT_HALT)
> > 
> > 
> > Outputs the relevant registers over the serial port and halt the
> > processor. Needs a manual reset to restart.
> > * Print registers and reboot (CONFIG_ESP_SYSTEM_PANIC_PRINT_REBOOT)
> > 
> > 
> > Outputs the relevant registers over the serial port and immediately
> > reset the processor.
> > * Silent reboot (CONFIG_ESP_SYSTEM_PANIC_SILENT_REBOOT)
> > 
> > 
> > Just resets the processor without outputting anything
> > * GDBStub on panic (CONFIG_ESP_SYSTEM_PANIC_GDBSTUB)
> > 
> > 
> > Invoke gdbstub on the serial port, allowing for gdb to attach to it to do a postmortem
> > of the crash.




##### CONFIG_ESP_SYSTEM_PANIC_REBOOT_DELAY_SECONDS



> Panic reboot delay (Seconds)
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> After the panic handler executes, you can specify a number of seconds to
> wait before the device reboots.
> 
> 
> 
> Range:* from 0 to 99
> 
> 
> 
> Default value:* 0




##### CONFIG_ESP_SYSTEM_RTC_EXT_XTAL_BOOTSTRAP_CYCLES



> Bootstrap cycles for external 32kHz crystal
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> To reduce the startup time of an external RTC crystal,
> we bootstrap it with a 32kHz square wave for a fixed number of cycles.
> Setting 0 will disable bootstrapping (if disabled, the crystal may take
> longer to start up or fail to oscillate under some conditions).
> 
> 
> If this value is too high, a faulty crystal may initially start and then fail.
> If this value is too low, an otherwise good crystal may not start.
> 
> 
> To accurately determine if the crystal has started,
> set a larger "Number of cycles for RTC_SLOW_CLK calibration" (about 3000).




##### CONFIG_ESP_SYSTEM_ALLOW_RTC_FAST_MEM_AS_HEAP



> Enable RTC fast memory for dynamic allocations
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This config option allows to add RTC fast memory region to system heap with capability
> similar to that of DRAM region but without DMA. This memory will be consumed first per
> heap initialization order by early startup services and scheduler related code. Speed
> wise RTC fast memory operates on APB clock and hence does not have much performance impact.
> 
> 
> 
> Default value:* Yes (enabled)




##### Memory protection


Contains:


* CONFIG_ESP_SYSTEM_PMP_IDRAM_SPLIT
* CONFIG_ESP_SYSTEM_MEMPROT_FEATURE



###### CONFIG_ESP_SYSTEM_PMP_IDRAM_SPLIT



> Enable IRAM/DRAM split protection
> 
> 
> *Found in:* Component config > ESP System Settings > Memory protection
> 
> 
> If enabled, the CPU watches all the memory access and raises an exception in case
> of any memory violation. This feature automatically splits
> the SRAM memory, using PMP, into data and instruction segments and sets Read/Execute permissions
> for the instruction part (below given splitting address) and Read/Write permissions
> for the data part (above the splitting address). The memory protection is effective
> on all access through the IRAM0 and DRAM0 buses.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_CPU_IDRAM_SPLIT_USING_PMP




###### CONFIG_ESP_SYSTEM_MEMPROT_FEATURE



> Enable memory protection
> 
> 
> *Found in:* Component config > ESP System Settings > Memory protection
> 
> 
> If enabled, the permission control module watches all the memory access and fires the panic handler
> if a permission violation is detected. This feature automatically splits
> the SRAM memory into data and instruction segments and sets Read/Execute permissions
> for the instruction part (below given splitting address) and Read/Write permissions
> for the data part (above the splitting address). The memory protection is effective
> on all access through the IRAM0 and DRAM0 buses.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_SYSTEM_MEMPROT_FEATURE_LOCK



> Lock memory protection settings
> 
> 
> *Found in:* Component config > ESP System Settings > Memory protection > CONFIG_ESP_SYSTEM_MEMPROT_FEATURE
> 
> 
> Once locked, memory protection settings cannot be changed anymore.
> The lock is reset only on the chip startup.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_ESP_SYSTEM_EVENT_QUEUE_SIZE



> System event queue size
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Config system event queue size in different application.
> 
> 
> 
> Default value:* 32




##### CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE



> Event loop task stack size
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Config system event task stack size in different application.
> 
> 
> 
> Default value:* 2304




##### CONFIG_ESP_MAIN_TASK_STACK_SIZE



> Main task stack size
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Configure the "main task" stack size. This is the stack of the task
> which calls app_main(). If app_main() returns then this task is deleted
> and its stack memory is freed.
> 
> 
> 
> Default value:* 3584




##### CONFIG_ESP_MAIN_TASK_AFFINITY



> Main task core affinity
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Configure the "main task" core affinity. This is the used core of the task
> which calls app_main(). If app_main() returns then this task is deleted.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CPU0 (CONFIG_ESP_MAIN_TASK_AFFINITY_CPU0)
> > * CPU1 (CONFIG_ESP_MAIN_TASK_AFFINITY_CPU1)
> > * No affinity (CONFIG_ESP_MAIN_TASK_AFFINITY_NO_AFFINITY)




##### CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE



> Minimal allowed size for shared stack
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Minimal value of size, in bytes, accepted to execute a expression
> with shared stack.
> 
> 
> 
> Default value:* 2048




##### CONFIG_ESP_CONSOLE_UART



> Channel for console output
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Select where to send console output (through stdout and stderr).
> 
> 
> * Default is to use UART0 on pre-defined GPIOs.
> * If "Custom" is selected, UART0 or UART1 can be chosen,
> and any pins can be selected.
> * If "None" is selected, there will be no console output on any UART, except
> for initial output from ROM bootloader. This ROM output can be suppressed by
> GPIO strapping or EFUSE, refer to chip datasheet for details.
> * On chips with USB OTG peripheral, "USB CDC" option redirects output to the
> CDC port. This option uses the CDC driver in the chip ROM.
> This option is incompatible with TinyUSB stack.
> * On chips with an USB serial/JTAG debug controller, selecting the option
> for that redirects output to the CDC/ACM (serial port emulation) component
> of that device.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Default: UART0 (CONFIG_ESP_CONSOLE_UART_DEFAULT)
> > * USB CDC (CONFIG_ESP_CONSOLE_USB_CDC)
> > * USB Serial/JTAG Controller (CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
> > * Custom UART (CONFIG_ESP_CONSOLE_UART_CUSTOM)
> > * None (CONFIG_ESP_CONSOLE_NONE)




##### CONFIG_ESP_CONSOLE_SECONDARY



> Channel for console secondary output
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This secondary option supports output through other specific port like USB_SERIAL_JTAG
> when UART0 port as a primary is selected but not connected. This secondary output currently only supports
> non-blocking mode without using REPL. If you want to output in blocking mode with REPL or
> input through this secondary port, please change the primary config to this port
> in Channel for console output menu.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No secondary console (CONFIG_ESP_CONSOLE_SECONDARY_NONE)
> > * USB_SERIAL_JTAG PORT (CONFIG_ESP_CONSOLE_SECONDARY_USB_SERIAL_JTAG)
> > 
> > 
> > This option supports output through USB_SERIAL_JTAG port when the UART0 port is not connected.
> > The output currently only supports non-blocking mode without using the console.
> > If you want to output in blocking mode with REPL or input through USB_SERIAL_JTAG port,
> > please change the primary config to ESP_CONSOLE_USB_SERIAL_JTAG above.




##### CONFIG_ESP_CONSOLE_UART_NUM



> UART peripheral to use for console output (0-1)
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This UART peripheral is used for console output from the ESP-IDF Bootloader and the app.
> 
> 
> If the configuration is different in the Bootloader binary compared to the app binary, UART
> is reconfigured after the bootloader exits and the app starts.
> 
> 
> Due to an ESP32 ROM bug, UART2 is not supported for console output
> via esp_rom_printf.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * UART0 (CONFIG_ESP_CONSOLE_UART_CUSTOM_NUM_0)
> > * UART1 (CONFIG_ESP_CONSOLE_UART_CUSTOM_NUM_1)




##### CONFIG_ESP_CONSOLE_UART_TX_GPIO



> UART TX on GPIO#
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This GPIO is used for console UART TX output in the ESP-IDF Bootloader and the app (including
> boot log output and default standard output and standard error of the app).
> 
> 
> If the configuration is different in the Bootloader binary compared to the app binary, UART
> is reconfigured after the bootloader exits and the app starts.
> 
> 
> 
> Range:* from 0 to 48 if CONFIG_ESP_CONSOLE_UART_CUSTOM
> 
> 
> 
> Default value:* 43 if CONFIG_ESP_CONSOLE_UART_CUSTOM




##### CONFIG_ESP_CONSOLE_UART_RX_GPIO



> UART RX on GPIO#
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This GPIO is used for UART RX input in the ESP-IDF Bootloader and the app (including
> default default standard input of the app).
> 
> 
> Note: The default ESP-IDF Bootloader configures this pin but doesn't read anything from the UART.
> 
> 
> If the configuration is different in the Bootloader binary compared to the app binary, UART
> is reconfigured after the bootloader exits and the app starts.
> 
> 
> 
> Range:* from 0 to 48 if CONFIG_ESP_CONSOLE_UART_CUSTOM
> 
> 
> 
> Default value:* 44 if CONFIG_ESP_CONSOLE_UART_CUSTOM




##### CONFIG_ESP_CONSOLE_UART_BAUDRATE



> UART console baud rate
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This baud rate is used by both the ESP-IDF Bootloader and the app (including
> boot log output and default standard input/output/error of the app).
> 
> 
> The app's maximum baud rate depends on the UART clock source. If Power Management is disabled,
> the UART clock source is the APB clock and all baud rates in the available range will be sufficiently
> accurate. If Power Management is enabled, REF_TICK clock source is used so the baud rate is divided
> from 1MHz. Baud rates above 1Mbps are not possible and values between 500Kbps and 1Mbps may not be
> accurate.
> 
> 
> If the configuration is different in the Bootloader binary compared to the app binary, UART
> is reconfigured after the bootloader exits and the app starts.
> 
> 
> 
> Range:* from 1200 to 1000000 if CONFIG_PM_ENABLE
> 
> 
> 
> Default value:* 115200




##### CONFIG_ESP_CONSOLE_USB_CDC_RX_BUF_SIZE



> Size of USB CDC RX buffer
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Set the size of USB CDC RX buffer. Increase the buffer size if your application
> is often receiving data over USB CDC.
> 
> 
> 
> Range:* from 4 to 16384 if CONFIG_ESP_CONSOLE_USB_CDC
> 
> 
> 
> Default value:* 64 if CONFIG_ESP_CONSOLE_USB_CDC




##### CONFIG_ESP_CONSOLE_USB_CDC_SUPPORT_ETS_PRINTF



> Enable esp_rom_printf / ESP_EARLY_LOG via USB CDC
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> If enabled, esp_rom_printf and ESP_EARLY_LOG output will also be sent over USB CDC.
> Disabling this option saves about 1kB or RAM.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_CONSOLE_USB_CDC




##### CONFIG_ESP_INT_WDT



> Interrupt watchdog
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This watchdog timer can detect if the FreeRTOS tick interrupt has not been called for a certain time,
> either because a task turned off interrupts and did not turn them on for a long time, or because an
> interrupt handler did not return. It will try to invoke the panic handler first and failing that
> reset the SoC.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_INT_WDT_TIMEOUT_MS



> Interrupt watchdog timeout (ms)
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_INT_WDT
> 
> 
> The timeout of the watchdog, in milliseconds. Make this higher than the FreeRTOS tick rate.
> 
> 
> 
> Range:* from 10 to 10000
> 
> 
> 
> Default value:* 300




###### CONFIG_ESP_INT_WDT_CHECK_CPU1



> Also watch CPU1 tick interrupt
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_INT_WDT
> 
> 
> Also detect if interrupts on CPU 1 are disabled for too long.





##### CONFIG_ESP_TASK_WDT_EN



> Enable Task Watchdog Timer
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> The Task Watchdog Timer can be used to make sure individual tasks are still
> running. Enabling this option will enable the Task Watchdog Timer. It can be
> either initialized automatically at startup or initialized after startup
> (see Task Watchdog Timer API Reference)
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_TASK_WDT_INIT



> Initialize Task Watchdog Timer on startup
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_TASK_WDT_EN
> 
> 
> Enabling this option will cause the Task Watchdog Timer to be initialized
> automatically at startup.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_TASK_WDT_PANIC



> Invoke panic handler on Task Watchdog timeout
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_TASK_WDT_EN > CONFIG_ESP_TASK_WDT_INIT
> 
> 
> If this option is enabled, the Task Watchdog Timer will be configured to
> trigger the panic handler when it times out. This can also be configured
> at run time (see Task Watchdog Timer API Reference)
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_ESP_TASK_WDT_TIMEOUT_S



> Task Watchdog timeout period (seconds)
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_TASK_WDT_EN > CONFIG_ESP_TASK_WDT_INIT
> 
> 
> Timeout period configuration for the Task Watchdog Timer in seconds.
> This is also configurable at run time (see Task Watchdog Timer API Reference)
> 
> 
> 
> Range:* from 1 to 60
> 
> 
> 
> Default value:* 5




###### CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0



> Watch CPU0 Idle Task
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_TASK_WDT_EN > CONFIG_ESP_TASK_WDT_INIT
> 
> 
> If this option is enabled, the Task Watchdog Timer will watch the CPU0
> Idle Task. Having the Task Watchdog watch the Idle Task allows for detection
> of CPU starvation as the Idle Task not being called is usually a symptom of
> CPU starvation. Starvation of the Idle Task is detrimental as FreeRTOS household
> tasks depend on the Idle Task getting some runtime every now and then.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1



> Watch CPU1 Idle Task
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_TASK_WDT_EN > CONFIG_ESP_TASK_WDT_INIT
> 
> 
> If this option is enabled, the Task Watchdog Timer will wach the CPU1
> Idle Task.





##### CONFIG_ESP_XT_WDT



> Initialize XTAL32K watchdog timer on startup
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This watchdog timer can detect oscillation failure of the XTAL32K_CLK. When such a failure
> is detected the hardware can be set up to automatically switch to BACKUP32K_CLK and generate
> an interrupt.



###### CONFIG_ESP_XT_WDT_TIMEOUT



> XTAL32K watchdog timeout period
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_XT_WDT
> 
> 
> Timeout period configuration for the XTAL32K watchdog timer based on RTC_CLK.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_ESP_XT_WDT
> 
> 
> 
> Default value:* 200 if CONFIG_ESP_XT_WDT




###### CONFIG_ESP_XT_WDT_BACKUP_CLK_ENABLE



> Automatically switch to BACKUP32K_CLK when timer expires
> 
> 
> *Found in:* Component config > ESP System Settings > CONFIG_ESP_XT_WDT
> 
> 
> Enable this to automatically switch to BACKUP32K_CLK as the source of RTC_SLOW_CLK when
> the watchdog timer expires.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_XT_WDT





##### CONFIG_ESP_PANIC_HANDLER_IRAM



> Place panic handler code in IRAM
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> If this option is disabled (default), the panic handler code is placed in flash not IRAM.
> This means that if ESP-IDF crashes while flash cache is disabled, the panic handler will
> automatically re-enable flash cache before running GDB Stub or Core Dump. This adds some minor
> risk, if the flash cache status is also corrupted during the crash.
> 
> 
> If this option is enabled, the panic handler code (including required UART functions) is placed
> in IRAM. This may be necessary to debug some complex issues with crashes while flash cache is
> disabled (for example, when writing to SPI flash) or when flash cache is corrupted when an exception
> is triggered.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_DEBUG_STUBS_ENABLE



> OpenOCD debug stubs
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Debug stubs are used by OpenOCD to execute pre-compiled onboard code
> which does some useful debugging stuff, e.g. GCOV data dump.




##### CONFIG_ESP_DEBUG_OCDAWARE



> Make exception and panic handlers JTAG/OCD aware
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> The FreeRTOS panic and unhandled exception handers can detect a JTAG OCD debugger and
> instead of panicking, have the debugger stop on the offending instruction.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_SYSTEM_CHECK_INT_LEVEL



> Interrupt level to use for Interrupt Watchdog and other system checks
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> Interrupt level to use for Interrupt Watchdog, IPC_ISR and other system checks.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Level 5 interrupt (CONFIG_ESP_SYSTEM_CHECK_INT_LEVEL_5)
> > 
> > 
> > Using level 5 interrupt for Interrupt Watchdog, IPC_ISR and other system checks.
> > * Level 4 interrupt (CONFIG_ESP_SYSTEM_CHECK_INT_LEVEL_4)
> > 
> > 
> > Using level 4 interrupt for Interrupt Watchdog, IPC_ISR and other system checks.




##### Brownout Detector


Contains:


* CONFIG_ESP_BROWNOUT_DET



###### CONFIG_ESP_BROWNOUT_DET



> Hardware brownout detect & reset
> 
> 
> *Found in:* Component config > ESP System Settings > Brownout Detector
> 
> 
> The ESP32-S3 has a built-in brownout detector which can detect if the voltage is lower than
> a specific value. If this happens, it will reset the chip in order to prevent unintended
> behaviour.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_BROWNOUT_DET_LVL_SEL



> Brownout voltage level
> 
> 
> *Found in:* Component config > ESP System Settings > Brownout Detector > CONFIG_ESP_BROWNOUT_DET
> 
> 
> The brownout detector will reset the chip when the supply voltage is approximately
> below this level. Note that there may be some variation of brownout voltage level
> between each ESP3-S3 chip.
> 
> 
> #The voltage levels here are estimates, more work needs to be done to figure out the exact voltages
> #of the brownout threshold levels.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 2.44V (CONFIG_ESP_BROWNOUT_DET_LVL_SEL_7)
> > * 2.56V (CONFIG_ESP_BROWNOUT_DET_LVL_SEL_6)
> > * 2.67V (CONFIG_ESP_BROWNOUT_DET_LVL_SEL_5)
> > * 2.84V (CONFIG_ESP_BROWNOUT_DET_LVL_SEL_4)
> > * 2.98V (CONFIG_ESP_BROWNOUT_DET_LVL_SEL_3)
> > * 3.19V (CONFIG_ESP_BROWNOUT_DET_LVL_SEL_2)
> > * 3.30V (CONFIG_ESP_BROWNOUT_DET_LVL_SEL_1)





##### CONFIG_ESP_SYSTEM_HW_STACK_GUARD



> Hardware stack guard
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This config allows to trigger a panic interrupt when Stack Pointer register goes out of allocated stack
> memory bounds.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_ASSIST_DEBUG_SUPPORTED




##### CONFIG_ESP_SYSTEM_BBPLL_RECALIB



> Re-calibration BBPLL at startup
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This configuration helps to address an BBPLL inaccurate issue when boot from certain bootloader version,
> which may increase about the boot-up time by about 200 us. Disable this when your bootloader is built with
> ESP-IDF version v5.2 and above.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_SYSTEM_HW_PC_RECORD



> Hardware PC recording
> 
> 
> *Found in:* Component config > ESP System Settings
> 
> 
> This option will enable the PC recording function of assist_debug module. The PC value of the CPU will be
> recorded to PC record register in assist_debug module in real time. When an exception occurs and the CPU
> is reset, this register will be kept, then we can use the recorded PC to debug the causes of the reset.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_ASSIST_DEBUG_SUPPORTED





#### IPC (Inter-Processor Call)


Contains:


* CONFIG_ESP_IPC_TASK_STACK_SIZE
* CONFIG_ESP_IPC_USES_CALLERS_PRIORITY



##### CONFIG_ESP_IPC_TASK_STACK_SIZE



> Inter-Processor Call (IPC) task stack size
> 
> 
> *Found in:* Component config > IPC (Inter-Processor Call)
> 
> 
> Configure the IPC tasks stack size. An IPC task runs on each core (in dual core mode), and allows for
> cross-core function calls. See IPC documentation for more details. The default IPC stack size should be
> enough for most common simple use cases. However, users can increase/decrease the stack size to their
> needs.
> 
> 
> 
> Range:* from 512 to 65536
> 
> 
> 
> Default value:* 1280
> * 1024




##### CONFIG_ESP_IPC_USES_CALLERS_PRIORITY



> IPC runs at caller's priority
> 
> 
> *Found in:* Component config > IPC (Inter-Processor Call)
> 
> 
> If this option is not enabled then the IPC task will keep behavior same as prior to that of ESP-IDF v4.0,
> hence IPC task will run at (configMAX_PRIORITIES - 1) priority.





#### ESP Timer (High Resolution Timer)


Contains:


* CONFIG_ESP_TIMER_PROFILING
* CONFIG_ESP_TIMER_TASK_AFFINITY
* CONFIG_ESP_TIMER_TASK_STACK_SIZE
* CONFIG_ESP_TIMER_INTERRUPT_LEVEL
* CONFIG_ESP_TIMER_SHOW_EXPERIMENTAL
* CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD
* CONFIG_ESP_TIMER_ISR_AFFINITY



##### CONFIG_ESP_TIMER_PROFILING



> Enable esp_timer profiling features
> 
> 
> *Found in:* Component config > ESP Timer (High Resolution Timer)
> 
> 
> If enabled, esp_timer_dump will dump information such as number of times the timer was started,
> number of times the timer has triggered, and the total time it took for the callback to run.
> This option has some effect on timer performance and the amount of memory used for timer
> storage, and should only be used for debugging/testing purposes.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_TIMER_TASK_STACK_SIZE



> High-resolution timer task stack size
> 
> 
> *Found in:* Component config > ESP Timer (High Resolution Timer)
> 
> 
> Configure the stack size of "timer_task" task. This task is used
> to dispatch callbacks of timers created using ets_timer and esp_timer
> APIs. If you are seing stack overflow errors in timer task, increase
> this value.
> 
> 
> Note that this is not the same as FreeRTOS timer task. To configure
> FreeRTOS timer task size, see "FreeRTOS timer task stack size" option
> in "FreeRTOS".
> 
> 
> 
> Range:* from 2048 to 65536
> 
> 
> 
> Default value:* 3584




##### CONFIG_ESP_TIMER_INTERRUPT_LEVEL



> Interrupt level
> 
> 
> *Found in:* Component config > ESP Timer (High Resolution Timer)
> 
> 
> This sets the interrupt priority level for esp_timer ISR.
> A higher value reduces interrupt latency by minimizing the timer processing delay.
> 
> 
> 
> Range:* from 1 to 1
> 
> 
> 
> Default value:* 1




##### CONFIG_ESP_TIMER_SHOW_EXPERIMENTAL



> show esp_timer's experimental features
> 
> 
> *Found in:* Component config > ESP Timer (High Resolution Timer)
> 
> 
> This shows some hidden features of esp_timer.
> Note that they may break other features, use them with care.




##### CONFIG_ESP_TIMER_TASK_AFFINITY



> esp_timer task core affinity
> 
> 
> *Found in:* Component config > ESP Timer (High Resolution Timer)
> 
> 
> The default settings: timer TASK on CPU0 and timer ISR on CPU0.
> Other settings may help in certain cases, but note that they may break
> other features, use them with care.
> - "CPU0": (default) esp_timer task is processed by CPU0.
> - "CPU1": esp_timer task is processed by CPU1.
> - "No affinity": esp_timer task can be processed by any CPU.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CPU0 (CONFIG_ESP_TIMER_TASK_AFFINITY_CPU0)
> > * CPU1 (CONFIG_ESP_TIMER_TASK_AFFINITY_CPU1)
> > * No affinity (CONFIG_ESP_TIMER_TASK_AFFINITY_NO_AFFINITY)




##### CONFIG_ESP_TIMER_ISR_AFFINITY



> timer interrupt core affinity
> 
> 
> *Found in:* Component config > ESP Timer (High Resolution Timer)
> 
> 
> The default settings: timer TASK on CPU0 and timer ISR on CPU0.
> Other settings may help in certain cases, but note that they may break
> other features, use them with care.
> - "CPU0": (default) timer interrupt is processed by CPU0.
> - "CPU1": timer interrupt is processed by CPU1.
> - "No affinity": timer interrupt can be processed by any CPU. It helps
> to reduce latency but there is a disadvantage it leads to the timer ISR
> running on every core. It increases the CPU time usage for timer ISRs
> by N on an N-core system.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CPU0 (CONFIG_ESP_TIMER_ISR_AFFINITY_CPU0)
> > * CPU1 (CONFIG_ESP_TIMER_ISR_AFFINITY_CPU1)
> > * No affinity (CONFIG_ESP_TIMER_ISR_AFFINITY_NO_AFFINITY)




##### CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD



> Support ISR dispatch method
> 
> 
> *Found in:* Component config > ESP Timer (High Resolution Timer)
> 
> 
> Allows using ESP_TIMER_ISR dispatch method (ESP_TIMER_TASK dispatch method is also avalible).
> - ESP_TIMER_TASK - Timer callbacks are dispatched from a high-priority esp_timer task.
> - ESP_TIMER_ISR - Timer callbacks are dispatched directly from the timer interrupt handler.
> The ISR dispatch can be used, in some cases, when a callback is very simple
> or need a lower-latency.
> 
> 
> 
> Default value:* No (disabled)





#### Wi-Fi


Contains:


* CONFIG_ESP_WIFI_TESTING_OPTIONS
* CONFIG_ESP_WIFI_WPS_SOFTAP_REGISTRAR
* CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
* CONFIG_ESP_WIFI_11KV_SUPPORT
* CONFIG_ESP_WIFI_11R_SUPPORT
* CONFIG_ESP_WIFI_DPP_SUPPORT
* CONFIG_ESP_WIFI_ENTERPRISE_SUPPORT
* CONFIG_ESP_WIFI_MBO_SUPPORT
* CONFIG_ESP_WIFI_SUITE_B_192
* CONFIG_ESP_WIFI_ENABLE_WPA3_OWE_STA
* CONFIG_ESP_WIFI_WAPI_PSK
* CONFIG_ESP_WIFI_ENABLE_DUMP_CTRL_BFRP
* CONFIG_ESP_WIFI_ENABLE_DUMP_HESIGB
* CONFIG_ESP_WIFI_ENABLE_DUMP_MU_CFO
* CONFIG_ESP_WIFI_ENABLE_DUMP_CTRL_NDPA
* CONFIG_ESP_WIFI_ENABLE_WIFI_RX_STATS
* CONFIG_ESP_WIFI_ENABLE_WIFI_TX_STATS
* CONFIG_ESP_WIFI_ENABLE_WPA3_SAE
* CONFIG_ESP_HOST_WIFI_ENABLED
* CONFIG_ESP_WIFI_SOFTAP_BEACON_MAX_LEN
* CONFIG_ESP_WIFI_CACHE_TX_BUFFER_NUM
* CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM
* CONFIG_ESP_WIFI_DYNAMIC_TX_BUFFER_NUM
* CONFIG_ESP_WIFI_RX_MGMT_BUF_NUM_DEF
* CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM
* CONFIG_ESP_WIFI_STATIC_TX_BUFFER_NUM
* CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM
* CONFIG_ESP_WIFI_SLP_DEFAULT_MAX_ACTIVE_TIME
* CONFIG_ESP_WIFI_SLP_DEFAULT_MIN_ACTIVE_TIME
* CONFIG_ESP_WIFI_SLP_DEFAULT_WAIT_BROADCAST_DATA_TIME
* CONFIG_ESP_WIFI_STA_DISCONNECTED_PM_ENABLE
* CONFIG_ESP_WIFI_DEBUG_PRINT
* CONFIG_ESP_WIFI_MGMT_RX_BUFFER
* CONFIG_ESP_WIFI_TX_BUFFER
* CONFIG_ESP_WIFI_MBEDTLS_CRYPTO
* CONFIG_ESP_WIFI_AMPDU_RX_ENABLED
* CONFIG_ESP_WIFI_AMPDU_TX_ENABLED
* CONFIG_ESP_WIFI_AMSDU_TX_ENABLED
* CONFIG_ESP_WIFI_NAN_ENABLE
* CONFIG_ESP_WIFI_CSI_ENABLED
* CONFIG_ESP_WIFI_EXTRA_IRAM_OPT
* CONFIG_ESP_WIFI_FTM_ENABLE
* CONFIG_ESP_WIFI_GCMP_SUPPORT
* CONFIG_ESP_WIFI_GMAC_SUPPORT
* CONFIG_ESP_WIFI_IRAM_OPT
* CONFIG_ESP_WIFI_MGMT_SBUF_NUM
* CONFIG_ESP_WIFI_ENHANCED_LIGHT_SLEEP
* CONFIG_ESP_WIFI_NVS_ENABLED
* CONFIG_ESP_WIFI_RX_IRAM_OPT
* CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
* CONFIG_ESP_WIFI_SLP_IRAM_OPT
* CONFIG_ESP_WIFI_SOFTAP_SUPPORT
* CONFIG_ESP_WIFI_TASK_CORE_ID
* CONFIG_ESP_WIFI_TX_HETB_QUEUE_NUM
* WPS Configuration Options



##### CONFIG_ESP_HOST_WIFI_ENABLED



> Host WiFi Enable
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> 
> Default value:* No (disabled) if SOC_WIRELESS_HOST_SUPPORTED




##### CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM



> Max number of WiFi static RX buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the number of WiFi static RX buffers. Each buffer takes approximately 1.6KB of RAM.
> The static rx buffers are allocated when esp_wifi_init is called, they are not freed
> until esp_wifi_deinit is called.
> 
> 
> WiFi hardware use these buffers to receive all 802.11 frames.
> A higher number may allow higher throughput but increases memory use. If ESP_WIFI_AMPDU_RX_ENABLED
> is enabled, this value is recommended to set equal or bigger than ESP_WIFI_RX_BA_WIN in order to
> achieve better throughput and compatibility with both stations and APs.
> 
> 
> 
> Range:* from 2 to 128 if SOC_WIFI_HE_SUPPORT
> 
> 
> 
> Default value:* 16 if CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP




##### CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM



> Max number of WiFi dynamic RX buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the number of WiFi dynamic RX buffers, 0 means unlimited RX buffers will be allocated
> (provided sufficient free RAM). The size of each dynamic RX buffer depends on the size of
> the received data frame.
> 
> 
> For each received data frame, the WiFi driver makes a copy to an RX buffer and then delivers
> it to the high layer TCP/IP stack. The dynamic RX buffer is freed after the higher layer has
> successfully received the data frame.
> 
> 
> For some applications, WiFi data frames may be received faster than the application can
> process them. In these cases we may run out of memory if RX buffer number is unlimited (0).
> 
> 
> If a dynamic RX buffer limit is set, it should be at least the number of static RX buffers.
> 
> 
> 
> Range:* from 0 to 1024 if CONFIG_LWIP_WND_SCALE
> 
> 
> 
> Default value:* 32




##### CONFIG_ESP_WIFI_TX_BUFFER



> Type of WiFi TX buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select type of WiFi TX buffers:
> 
> 
> If "Static" is selected, WiFi TX buffers are allocated when WiFi is initialized and released
> when WiFi is de-initialized. The size of each static TX buffer is fixed to about 1.6KB.
> 
> 
> If "Dynamic" is selected, each WiFi TX buffer is allocated as needed when a data frame is
> delivered to the Wifi driver from the TCP/IP stack. The buffer is freed after the data frame
> has been sent by the WiFi driver. The size of each dynamic TX buffer depends on the length
> of each data frame sent by the TCP/IP layer.
> 
> 
> If PSRAM is enabled, "Static" should be selected to guarantee enough WiFi TX buffers.
> If PSRAM is disabled, "Dynamic" should be selected to improve the utilization of RAM.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Static (CONFIG_ESP_WIFI_STATIC_TX_BUFFER)
> > * Dynamic (CONFIG_ESP_WIFI_DYNAMIC_TX_BUFFER)




##### CONFIG_ESP_WIFI_STATIC_TX_BUFFER_NUM



> Max number of WiFi static TX buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the number of WiFi static TX buffers. Each buffer takes approximately 1.6KB of RAM.
> The static RX buffers are allocated when esp_wifi_init() is called, they are not released
> until esp_wifi_deinit() is called.
> 
> 
> For each transmitted data frame from the higher layer TCP/IP stack, the WiFi driver makes a
> copy of it in a TX buffer. For some applications especially UDP applications, the upper
> layer can deliver frames faster than WiFi layer can transmit. In these cases, we may run out
> of TX buffers.
> 
> 
> 
> Range:* from 1 to 64 if CONFIG_ESP_WIFI_STATIC_TX_BUFFER
> 
> 
> 
> Default value:* 16 if CONFIG_ESP_WIFI_STATIC_TX_BUFFER




##### CONFIG_ESP_WIFI_CACHE_TX_BUFFER_NUM



> Max number of WiFi cache TX buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the number of WiFi cache TX buffer number.
> 
> 
> For each TX packet from uplayer, such as LWIP etc, WiFi driver needs to allocate a static TX
> buffer and makes a copy of uplayer packet. If WiFi driver fails to allocate the static TX buffer,
> it caches the uplayer packets to a dedicated buffer queue, this option is used to configure the
> size of the cached TX queue.
> 
> 
> 
> Range:* from 16 to 128 if CONFIG_SPIRAM
> 
> 
> 
> Default value:* 32 if CONFIG_SPIRAM




##### CONFIG_ESP_WIFI_DYNAMIC_TX_BUFFER_NUM



> Max number of WiFi dynamic TX buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the number of WiFi dynamic TX buffers. The size of each dynamic TX buffer is not fixed,
> it depends on the size of each transmitted data frame.
> 
> 
> For each transmitted frame from the higher layer TCP/IP stack, the WiFi driver makes a copy
> of it in a TX buffer. For some applications, especially UDP applications, the upper layer
> can deliver frames faster than WiFi layer can transmit. In these cases, we may run out of TX
> buffers.
> 
> 
> 
> Range:* from 1 to 128
> 
> 
> 
> Default value:* 32




##### CONFIG_ESP_WIFI_MGMT_RX_BUFFER



> Type of WiFi RX MGMT buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select type of WiFi RX MGMT buffers:
> 
> 
> If "Static" is selected, WiFi RX MGMT buffers are allocated when WiFi is initialized and released
> when WiFi is de-initialized. The size of each static RX MGMT buffer is fixed to about 500 Bytes.
> 
> 
> If "Dynamic" is selected, each WiFi RX MGMT buffer is allocated as needed when a MGMT data frame is
> received. The MGMT buffer is freed after the MGMT data frame has been processed by the WiFi driver.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Static (CONFIG_ESP_WIFI_STATIC_RX_MGMT_BUFFER)
> > * Dynamic (CONFIG_ESP_WIFI_DYNAMIC_RX_MGMT_BUFFER)




##### CONFIG_ESP_WIFI_RX_MGMT_BUF_NUM_DEF



> Max number of WiFi RX MGMT buffers
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the number of WiFi RX_MGMT buffers.
> 
> 
> For Management buffers, the number of dynamic and static management buffers is the same.
> In order to prevent memory fragmentation, the management buffer type should be set to static first.
> 
> 
> 
> Range:* from 1 to 10
> 
> 
> 
> Default value:* 5




##### CONFIG_ESP_WIFI_CSI_ENABLED



> WiFi CSI(Channel State Information)
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable CSI(Channel State Information) feature. CSI takes about
> CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM KB of RAM. If CSI is not used, it is better to disable
> this feature in order to save memory.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_AMPDU_TX_ENABLED



> WiFi AMPDU TX
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable AMPDU TX feature
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_WIFI_TX_BA_WIN



> WiFi AMPDU TX BA window size
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_AMPDU_TX_ENABLED
> 
> 
> Set the size of WiFi Block Ack TX window. Generally a bigger value means higher throughput but
> more memory. Most of time we should NOT change the default value unless special reason, e.g.
> test the maximum UDP TX throughput with iperf etc. For iperf test in shieldbox, the recommended
> value is 9~12.
> 
> 
> 
> Range:* from 2 to 64 if SOC_WIFI_HE_SUPPORT && CONFIG_ESP_WIFI_AMPDU_TX_ENABLED
> 
> 
> 
> Default value:* 6





##### CONFIG_ESP_WIFI_AMPDU_RX_ENABLED



> WiFi AMPDU RX
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable AMPDU RX feature
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_WIFI_RX_BA_WIN



> WiFi AMPDU RX BA window size
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_AMPDU_RX_ENABLED
> 
> 
> Set the size of WiFi Block Ack RX window. Generally a bigger value means higher throughput and better
> compatibility but more memory. Most of time we should NOT change the default value unless special
> reason, e.g. test the maximum UDP RX throughput with iperf etc. For iperf test in shieldbox, the
> recommended value is 9~12. If PSRAM is used and WiFi memory is preferred to allocate in PSRAM first,
> the default and minimum value should be 16 to achieve better throughput and compatibility with both
> stations and APs.
> 
> 
> 
> Range:* from 2 to 64 if SOC_WIFI_HE_SUPPORT && CONFIG_ESP_WIFI_AMPDU_RX_ENABLED
> 
> 
> 
> Default value:* 16 if CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP && CONFIG_ESP_WIFI_AMPDU_RX_ENABLED





##### CONFIG_ESP_WIFI_AMSDU_TX_ENABLED



> WiFi AMSDU TX
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable AMSDU TX feature
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SPIRAM




##### CONFIG_ESP_WIFI_NVS_ENABLED



> WiFi NVS flash
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable WiFi NVS flash
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_WIFI_TASK_CORE_ID



> WiFi Task Core ID
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Pinned WiFi task to core 0 or core 1.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Core 0 (CONFIG_ESP_WIFI_TASK_PINNED_TO_CORE_0)
> > * Core 1 (CONFIG_ESP_WIFI_TASK_PINNED_TO_CORE_1)




##### CONFIG_ESP_WIFI_SOFTAP_BEACON_MAX_LEN



> Max length of WiFi SoftAP Beacon
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> ESP-MESH utilizes beacon frames to detect and resolve root node conflicts (see documentation). However
> the default length of a beacon frame can simultaneously hold only five root node identifier structures,
> meaning that a root node conflict of up to five nodes can be detected at one time. In the occurrence of
> more root nodes conflict involving more than five root nodes, the conflict resolution process will
> detect five of the root nodes, resolve the conflict, and re-detect more root nodes. This process will
> repeat until all root node conflicts are resolved. However this process can generally take a very long
> time.
> 
> 
> To counter this situation, the beacon frame length can be increased such that more root nodes can be
> detected simultaneously. Each additional root node will require 36 bytes and should be added on top of
> the default beacon frame length of
> 752 bytes. For example, if you want to detect 10 root nodes simultaneously, you need to set the beacon
> frame length as
> 932 (752+36\*5).
> 
> 
> Setting a longer beacon length also assists with debugging as the conflicting root nodes can be
> identified more quickly.
> 
> 
> 
> Range:* from 752 to 1256
> 
> 
> 
> Default value:* 752




##### CONFIG_ESP_WIFI_MGMT_SBUF_NUM



> WiFi mgmt short buffer number
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the number of WiFi management short buffer.
> 
> 
> 
> Range:* from 6 to 32
> 
> 
> 
> Default value:* 32




##### CONFIG_ESP_WIFI_IRAM_OPT



> WiFi IRAM speed optimization
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to place frequently called Wi-Fi library functions in IRAM.
> When this option is disabled, more than 10Kbytes of IRAM memory will be saved
> but Wi-Fi throughput will be reduced.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_WIFI_EXTRA_IRAM_OPT



> WiFi EXTRA IRAM speed optimization
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to place additional frequently called Wi-Fi library functions
> in IRAM. When this option is disabled, more than 5Kbytes of IRAM memory will be saved
> but Wi-Fi throughput will be reduced.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_WIFI_HE_SUPPORT
> * No (disabled)




##### CONFIG_ESP_WIFI_RX_IRAM_OPT



> WiFi RX IRAM speed optimization
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to place frequently called Wi-Fi library RX functions in IRAM.
> When this option is disabled, more than 17Kbytes of IRAM memory will be saved
> but Wi-Fi performance will be reduced.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_WIFI_ENABLE_WPA3_SAE



> Enable WPA3-Personal
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to allow the device to establish a WPA3-Personal connection with eligible AP's.
> PMF (Protected Management Frames) is a prerequisite feature for a WPA3 connection, it needs to be
> explicitly configured before attempting connection. Please refer to the Wi-Fi Driver API Guide
> for details.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_WIFI_ENABLE_SAE_PK



> Enable SAE-PK
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_WPA3_SAE
> 
> 
> Select this option to enable SAE-PK
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT



> Enable WPA3 Personal(SAE) SoftAP
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_WPA3_SAE
> 
> 
> Select this option to enable SAE support in softAP mode.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_ESP_WIFI_ENABLE_WPA3_OWE_STA



> Enable OWE STA
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to allow the device to establish OWE connection with eligible AP's.
> PMF (Protected Management Frames) is a prerequisite feature for a WPA3 connection, it needs to be
> explicitly configured before attempting connection. Please refer to the Wi-Fi Driver API Guide
> for details.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_WIFI_SLP_IRAM_OPT



> WiFi SLP IRAM speed optimization
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to place called Wi-Fi library TBTT process and receive beacon functions in IRAM.
> Some functions can be put in IRAM either by ESP_WIFI_IRAM_OPT and ESP_WIFI_RX_IRAM_OPT, or this one.
> If already enabled ESP_WIFI_IRAM_OPT, the other 7.3KB IRAM memory would be taken by this option.
> If already enabled ESP_WIFI_RX_IRAM_OPT, the other 1.3KB IRAM memory would be taken by this option.
> If neither of them are enabled, the other 7.4KB IRAM memory would be taken by this option.
> Wi-Fi power-save mode average current would be reduced if this option is enabled.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_WIFI_HE_SUPPORT




##### CONFIG_ESP_WIFI_SLP_DEFAULT_MIN_ACTIVE_TIME



> Minimum active time
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Only for station in WIFI_PS_MIN_MODEM or WIFI_PS_MAX_MODEM. When the station enters the active state,
> it will work for at least ESP_WIFI_SLP_DEFAULT_MIN_ACTIVE_TIME. If a data packet is received or sent
> during this period, the time will be refreshed. If the time is up, but the station still has packets
> to receive or send, the time will also be refreshed. unit: milliseconds.
> 
> 
> 
> Range:* from 8 to 60
> 
> 
> 
> Default value:* 50




##### CONFIG_ESP_WIFI_SLP_DEFAULT_MAX_ACTIVE_TIME



> Maximum keep alive time
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Only for station in WIFI_PS_MIN_MODEM or WIFI_PS_MAX_MODEM. If no packet has been
> sent within ESP_WIFI_SLP_DEFAULT_MAX_ACTIVE_TIME, a null data packet will be sent
> to maintain the connection with the AP. unit: seconds.
> 
> 
> 
> Range:* from 10 to 60
> 
> 
> 
> Default value:* 10




##### CONFIG_ESP_WIFI_SLP_DEFAULT_WAIT_BROADCAST_DATA_TIME



> Minimum wait broadcast data time
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Only for station in WIFI_PS_MIN_MODEM or WIFI_PS_MAX_MODEM. When the station knows through the beacon
> that AP will send broadcast packet, it will wait for ESP_WIFI_SLP_DEFAULT_WAIT_BROADCAST_DATA_TIME
> before entering the sleep process. If a broadcast packet is received with more data bits, the time
> will refreshed. unit: milliseconds.
> 
> 
> 
> Range:* from 10 to 30
> 
> 
> 
> Default value:* 15




##### CONFIG_ESP_WIFI_FTM_ENABLE



> WiFi FTM
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable feature Fine Timing Measurement for calculating WiFi Round-Trip-Time (RTT).
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_ESP_WIFI_FTM_INITIATOR_SUPPORT



> FTM Initiator support
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_FTM_ENABLE
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_WIFI_FTM_ENABLE




###### CONFIG_ESP_WIFI_FTM_RESPONDER_SUPPORT



> FTM Responder support
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_FTM_ENABLE
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_WIFI_FTM_ENABLE





##### CONFIG_ESP_WIFI_STA_DISCONNECTED_PM_ENABLE



> Power Management for station at disconnected
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable power_management for station when disconnected.
> Chip will do modem-sleep when rf module is not in use any more.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_WIFI_GCMP_SUPPORT



> WiFi GCMP Support(GCMP128 and GCMP256)
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable GCMP support. GCMP support is compulsory for WiFi Suite-B support.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_GMAC_SUPPORT



> WiFi GMAC Support(GMAC128 and GMAC256)
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable GMAC support. GMAC support is compulsory for WiFi 192 bit certification.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_WIFI_SOFTAP_SUPPORT



> WiFi SoftAP Support
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> WiFi module can be compiled without SoftAP to save code size.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_WIFI_ENHANCED_LIGHT_SLEEP



> WiFi modem automatically receives the beacon
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> The wifi modem automatically receives the beacon frame during light sleep.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_PHY_MAC_BB_PD && SOC_PM_SUPPORT_BEACON_WAKEUP




##### CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT



> Wifi sleep optimize when beacon lost
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable wifi sleep optimization when beacon loss occurs and immediately enter
> sleep mode when the WiFi module detects beacon loss.



###### CONFIG_ESP_WIFI_SLP_BEACON_LOST_TIMEOUT



> Beacon loss timeout
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> Timeout time for close rf phy when beacon loss occurs, Unit: 1024 microsecond.
> 
> 
> 
> Range:* from 5 to 100 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> 
> Default value:* 10 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT




###### CONFIG_ESP_WIFI_SLP_BEACON_LOST_THRESHOLD



> Maximum number of consecutive lost beacons allowed
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> Maximum number of consecutive lost beacons allowed, WiFi keeps Rx state when
> the number of consecutive beacons lost is greater than the given threshold.
> 
> 
> 
> Range:* from 0 to 8 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> 
> Default value:* 3 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT




###### CONFIG_ESP_WIFI_SLP_PHY_ON_DELTA_EARLY_TIME



> Delta early time for RF PHY on
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> Delta early time for rf phy on, When the beacon is lost, the next rf phy on will
> be earlier the time specified by the configuration item, Unit: 32 microsecond.
> 
> 
> 
> Range:* from 0 to 100 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> 
> Default value:* 2 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT




###### CONFIG_ESP_WIFI_SLP_PHY_OFF_DELTA_TIMEOUT_TIME



> Delta timeout time for RF PHY off
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> Delta timeout time for rf phy off, When the beacon is lost, the next rf phy off will
> be delayed for the time specified by the configuration item. Unit: 1024 microsecond.
> 
> 
> 
> Range:* from 0 to 8 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT
> 
> 
> 
> Default value:* 2 if CONFIG_ESP_WIFI_SLP_BEACON_LOST_OPT





##### CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM



> Maximum espnow encrypt peers number
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Maximum number of encrypted peers supported by espnow.
> The number of hardware keys for encryption is fixed. And the espnow and SoftAP share the same
> hardware keys. So this configuration will affect the maximum connection number of SoftAP.
> Maximum espnow encrypted peers number + maximum number of connections of SoftAP = Max hardware
> keys number. When using ESP mesh, this value should be set to a maximum of 6.
> 
> 
> 
> Range:* from 0 to 17
> 
> 
> 
> Default value:* 7




##### CONFIG_ESP_WIFI_NAN_ENABLE



> WiFi Aware
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable WiFi Aware (NAN) feature.
> 
> 
> 
> Default value:* No (disabled) if SOC_WIFI_NAN_SUPPORT




##### CONFIG_ESP_WIFI_MBEDTLS_CRYPTO



> Use MbedTLS crypto APIs
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable the use of MbedTLS crypto APIs.
> The internal crypto support within the supplicant is limited
> and may not suffice for all new security features, including WPA3.
> 
> 
> It is recommended to always keep this option enabled. Additionally,
> note that MbedTLS can leverage hardware acceleration if available,
> resulting in significantly faster cryptographic operations.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_WIFI_MBEDTLS_TLS_CLIENT



> Use MbedTLS TLS client for WiFi Enterprise connection
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_MBEDTLS_CRYPTO
> 
> 
> Select this option to use MbedTLS TLS client for WPA2 enterprise connection.
> Please note that from MbedTLS-3.0 onwards, MbedTLS does not support SSL-3.0
> TLS-v1.0, TLS-v1.1 versions. In case your server is using one of these version,
> it is advisable to update your server.
> Please disable this option for compatibility with older TLS versions.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_ESP_WIFI_EAP_TLS1_3



> Enable EAP-TLS v1.3 Support for WiFi Enterprise connection
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_MBEDTLS_CRYPTO > CONFIG_ESP_WIFI_MBEDTLS_TLS_CLIENT
> 
> 
> Select this option to support EAP with TLS v1.3.
> This configuration still supports compatibility with EAP-TLS v1.2.
> Please note that enabling this configuration will cause every application which
> uses TLS go for TLS1.3 if server supports that. TLS1.3 is still in development in mbedtls
> and there may be interoperability issues with this. Please modify your application to set
> max version as TLS1.2 if you want to enable TLS1.3 only for WiFi connection.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_WIFI_MBEDTLS_TLS_CLIENT && CONFIG_IDF_EXPERIMENTAL_FEATURES && CONFIG_ESP_WIFI_MBEDTLS_CRYPTO





##### CONFIG_ESP_WIFI_WAPI_PSK



> Enable WAPI PSK support
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable WAPI-PSK
> which is a Chinese National Standard Encryption for Wireless LANs (GB 15629.11-2003).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_SUITE_B_192



> Enable NSA suite B support with 192 bit key
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable 192 bit NSA suite-B.
> This is necessary to support WPA3 192 bit security.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_11KV_SUPPORT



> Enable 802.11k, 802.11v APIs Support
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable 802.11k 802.11v APIs(RRM and BTM support).
> Only APIs which are helpful for network assisted roaming
> are supported for now.
> Enable this option with BTM and RRM enabled in sta config
> to make device ready for network assisted roaming.
> BTM: BSS transition management enables an AP to request a station to transition
> to a specific AP, or to indicate to a station a set of preferred APs.
> RRM: Radio measurements enable STAs to understand the radio environment,
> it enables STAs to observe and gather data on radio link performance
> and on the radio environment. Current implementation adds beacon report,
> link measurement, neighbor report.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_ESP_WIFI_SCAN_CACHE



> Keep scan results in cache
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_11KV_SUPPORT
> 
> 
> Keep scan results in cache, if not enabled, those
> will be flushed immediately.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_WIFI_11KV_SUPPORT





##### CONFIG_ESP_WIFI_MBO_SUPPORT



> Enable Multi Band Operation Certification Support
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable WiFi Multiband operation certification support.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_ENABLE_ROAMING_APP



> Advanced support for Wi-Fi Roaming (Experimental)
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable Espressif's roaming app to allow for efficient Wi-Fi roaming.
> This includes configurable periodic environment scans, maintaining a cache of the
> best APs, handling low rssi events etc.
> 
> 
> Risk Warning
> Please note that this feature is still experimental and enabling this potentially can
> lead to unpredictable scanning, connection and roaming attempts.
> We are still working on tuning and optimising this feature to ensure reliable and stable use.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IDF_EXPERIMENTAL_FEATURES



###### Configure roaming App


Contains:


* CONFIG_ESP_WIFI_ROAMING_BACKOFF_TIME
* Roaming Methods
* Roaming triggers
* Scan Configuration
* CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING




###### Roaming triggers


Contains:


* CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR
* CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING




###### CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING



> Use Low RSSI to trigger roaming.
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming triggers
> 
> 
> Enable to use a RSSI threshold to trigger roaming.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_THRESHOLD



> WiFi RSSI threshold to trigger roaming
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming triggers > CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING
> 
> 
> WiFi RSSI threshold to trigger roaming value in dBm (-99 to -1). Values under -30 dbm
> might lead to a flood of low rssi events. This interferes with normal functioning and
> TX/Rx performance.
> 
> 
> 
> Range:* from -99 to -30 if CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* "-60" if CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_OFFSET



> Offset by which to reset the RSSI Threshold after attempt to roam.
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming triggers > CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING
> 
> 
> Decide the offset by which to decrease the Low RSSI threshold set by ESP_WIFI_ROAMING_LOW_RSSI_THRESHOLD
> after each failed attempt to roam. This allows for the station to keep scanning for better AP's after
> the Low RSSI threshold is reached in a stepped manner, rather than only attempting to roam the first time
> the current AP's RSSI breaches the set RSSI threshold.
> Setting 0 here may cause station to be flooded with low rssi events,
> therefore that's not recommended to be kept.
> 
> 
> 
> Range:* from 0 to 99 if CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 5 if CONFIG_ESP_WIFI_ROAMING_LOW_RSSI_ROAMING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR



> Conduct periodic scans to check if a better AP is available
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming triggers
> 
> 
> Conduct periodic scans periodically to check if a better AP is available.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_THRESHOLD



> Threshold at which to begin periodic scanning for a better AP.
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming triggers > CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR
> 
> 
> Threshold at which the station will begin scanning to find an AP with better RSSI.
> 
> 
> 
> Range:* from -99 to -1 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* "-50" if CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_SCAN_MONITOR_INTERVAL



> Time intervals (in seconds) at which station will initiate a scan
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming triggers > CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR
> 
> 
> Intervals at which station will periodically scan to check if better AP is available
> 
> 
> 
> Range:* from 1 to 1500 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 30 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_SCAN_ROAM_RSSI_DIFF



> RSSI difference b/w current AP and candidate AP to initiate connection
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming triggers > CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR
> 
> 
> Minimum RSSI difference b/w current AP and a potential roaming candidate AP
> to trigger a roaming attempt.
> 
> 
> 
> Range:* from 0 to 99 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 15 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_SCAN_MONITOR && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### Roaming Methods


Contains:


* CONFIG_ESP_WIFI_ROAMING_LEGACY_ROAMING
* CONFIG_ESP_WIFI_ROAMING_NETWORK_ASSISTED_ROAM




###### CONFIG_ESP_WIFI_ROAMING_LEGACY_ROAMING



> Support Legacy roaming approach
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming Methods
> 
> 
> Roaming between APs that do not support 802.11kv.
> This will allow station to roam even when connection is not BTM supported,
> by forcefully disconnecting from current AP and connecting to better AP.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_NETWORK_ASSISTED_ROAM



> Support Network Assisted roaming using 802.11kv
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming Methods
> 
> 
> Roaming between APs using network assisted Roaming.
> This involves BSS Transition Management mechanisms outlined in 802.11v.
> Note that this moves the responsibility to the AP's network, and hence isn't
> guaranteed to cause the station to attempt to roam each time.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_WIFI_11KV_SUPPORT && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_NETWORK_ASSISTED_ROAMING_RETRY_COUNT



> Retry count after which to switch to legacy roaming
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Roaming Methods > CONFIG_ESP_WIFI_ROAMING_NETWORK_ASSISTED_ROAM
> 
> 
> Retry threshold after which the station should stop using Network Assisted
> roaming methods and start using legacy roaming instead.
> 
> 
> 
> Range:* from 1 to 5 if CONFIG_ESP_WIFI_ROAMING_NETWORK_ASSISTED_ROAM && CONFIG_ESP_WIFI_ROAMING_LEGACY_ROAMING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 2 if CONFIG_ESP_WIFI_ROAMING_NETWORK_ASSISTED_ROAM && CONFIG_ESP_WIFI_ROAMING_LEGACY_ROAMING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### Scan Configuration


Contains:


* CONFIG_ESP_WIFI_ROAMING_HOME_CHANNEL_DWELL_TIME
* CONFIG_ESP_WIFI_ROAMING_MAX_CANDIDATES
* CONFIG_ESP_WIFI_ROAMING_SCAN_MAX_SCAN_TIME
* CONFIG_ESP_WIFI_ROAMING_SCAN_MIN_SCAN_TIME
* CONFIG_ESP_WIFI_ROAMING_SCAN_CHAN_LIST
* CONFIG_ESP_WIFI_ROAMING_SCAN_EXPIRY_WINDOW




###### CONFIG_ESP_WIFI_ROAMING_SCAN_MIN_SCAN_TIME



> Minimum duration (in milliseconds) of station's per channel active scan
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Scan Configuration
> 
> 
> Minimum duration of active scanning per channel in milliseconds.
> 
> 
> 
> Range:* from 0 to 120 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 10 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_SCAN_MAX_SCAN_TIME



> Maximum duration (in milliseconds) of station's per channel active scan time
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Scan Configuration
> 
> 
> Maximum duration of active scanning per channel in milliseconds.
> 
> 
> 
> Range:* from 30 to 120 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 70 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_HOME_CHANNEL_DWELL_TIME



> Home channel dwell time scanning between consecutive channels
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Scan Configuration
> 
> 
> If connected, duration for which the station will return to it's home channel for Tx/Rx of
> frames stored in buffers between scanning on consecutive channels.
> 
> 
> 
> Range:* from 30 to 150 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 30 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_SCAN_CHAN_LIST



> Preferred channel list for scanning
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Scan Configuration
> 
> 
> Channels your wireless network operates on to allow for faster scanning.
> Specify the channels(between 1-14) in a comma separated manner.
> 
> 
> 
> Default value:* "None" if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_SCAN_EXPIRY_WINDOW



> Scan results expiry window (in seconds)
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Scan Configuration
> 
> 
> Duration for which the results from the most recent scans can be used
> by the roaming app for determining the roaming candidates.
> 
> 
> 
> Range:* from 5 to 20 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 10 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_MAX_CANDIDATES



> Max Candidates in the network
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > Scan Configuration
> 
> 
> Max candidates that can be considered while scanning as a part of the
> network at one time.
> 
> 
> 
> Range:* from 3 to 20 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 3 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_BACKOFF_TIME



> Default time to wait between subsequent roaming attempts.
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App
> 
> 
> Time to wait (in seconds) by station before registering for the RSSI event again
> or start continuous montoring to find better AP.
> 
> 
> 
> Range:* from 0 to 120 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 15 if CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING



> Send periodic neighbor report request to AP for internal list updation
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App
> 
> 
> This option will enable station to keep sending RRM neighbor list request to AP and
> update its internal list.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_WIFI_11KV_SUPPORT && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_RRM_MONITOR_TIME



> Time interval (in seconds) between neighbor report requests to an AP
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING
> 
> 
> Enable this to send periodic neighbor report requests to the AP.
> These neighbor report requests provide information about other APs in the same managed
> network. This information is used for more intelligent roaming.
> 
> 
> 
> Range:* from 0 to 1500 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* 60 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP




###### CONFIG_ESP_WIFI_ROAMING_RRM_MONITOR_THRESHOLD



> Threshold for sending periodic neighbor report requests
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_ROAMING_APP > Configure roaming App > CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING
> 
> 
> The RSSI threshold beyond which we start sending periodic neighbor report requests.
> 
> 
> 
> Range:* from -99 to 0 if CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* "-20" if CONFIG_ESP_WIFI_ROAMING_PERIODIC_RRM_MONITORING && CONFIG_ESP_WIFI_ENABLE_ROAMING_APP





##### CONFIG_ESP_WIFI_DPP_SUPPORT



> Enable DPP support
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable WiFi Easy Connect Support.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_11R_SUPPORT



> Enable 802.11R (Fast Transition) Support
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable WiFi Fast Transition Support.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_WPS_SOFTAP_REGISTRAR



> Add WPS Registrar support in SoftAP mode
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to enable WPS registrar support in softAP mode.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_ENABLE_WIFI_TX_STATS



> Enable Wi-Fi transmission statistics
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable Wi-Fi transmission statistics. Total support 4 access category. Each access category
> will use 346 bytes memory.
> 
> 
> 
> Default value:* No (disabled) if SOC_WIFI_HE_SUPPORT




##### CONFIG_ESP_WIFI_ENABLE_WIFI_RX_STATS



> Enable Wi-Fi reception statistics
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable Wi-Fi reception statistics. Total support 2 access category. Each access category
> will use 190 bytes memory.
> 
> 
> 
> Default value:* No (disabled) if SOC_WIFI_HE_SUPPORT



###### CONFIG_ESP_WIFI_ENABLE_WIFI_RX_MU_STATS



> Enable Wi-Fi DL MU-MIMO and DL OFDMA reception statistics
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENABLE_WIFI_RX_STATS
> 
> 
> Enable Wi-Fi DL MU-MIMO and DL OFDMA reception statistics. Will use 10932 bytes memory.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_WIFI_ENABLE_WIFI_RX_STATS





##### CONFIG_ESP_WIFI_TX_HETB_QUEUE_NUM



> WiFi TX HE TB QUEUE number for STA HE TB PPDU transmission
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Set the maximum number of queue that can be aggregated by the STA in the A-MPDU carried in the
> HE TB PPDU.
> 
> 
> 
> Range:* from 1 to 4 if SOC_WIFI_HE_SUPPORT
> 
> 
> 
> Default value:* 3 if SOC_WIFI_HE_SUPPORT




##### CONFIG_ESP_WIFI_ENABLE_DUMP_HESIGB



> Enable Wi-Fi dump HE-SIGB which is contained in DL HE MU PPDUs
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable Wi-Fi dump HE-SIGB which is contained in DL HE MU PPDUs.
> 
> 
> 
> Default value:* No (disabled) if SOC_WIFI_HE_SUPPORT_5G




##### CONFIG_ESP_WIFI_ENABLE_DUMP_MU_CFO



> Enable Wi-Fi dump MU CFO
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable Wi-Fi dump MU CFO.
> 
> 
> 
> Default value:* No (disabled) if SOC_WIFI_HE_SUPPORT_5G




##### CONFIG_ESP_WIFI_ENABLE_DUMP_CTRL_NDPA



> Enable Wi-Fi dump NDPA frames
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable Wi-Fi dump NDPA frames.
> 
> 
> 
> Default value:* No (disabled) if SOC_WIFI_HE_SUPPORT_5G




##### CONFIG_ESP_WIFI_ENABLE_DUMP_CTRL_BFRP



> Enable Wi-Fi dump BFRP frames
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Enable Wi-Fi dump BFRP frames.
> 
> 
> 
> Default value:* No (disabled) if SOC_WIFI_HE_SUPPORT_5G




##### WPS Configuration Options


Contains:


* CONFIG_ESP_WIFI_WPS_PASSPHRASE
* CONFIG_ESP_WIFI_WPS_STRICT



###### CONFIG_ESP_WIFI_WPS_STRICT



> Strictly validate all WPS attributes
> 
> 
> *Found in:* Component config > Wi-Fi > WPS Configuration Options
> 
> 
> Select this option to enable validate each WPS attribute
> rigorously. Disabling this add the workarounds with various APs.
> Enabling this may cause inter operability issues with some APs.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_ESP_WIFI_WPS_PASSPHRASE



> Get WPA2 passphrase in WPS config
> 
> 
> *Found in:* Component config > Wi-Fi > WPS Configuration Options
> 
> 
> Select this option to get passphrase during WPS configuration.
> This option fakes the virtual display capabilities to get the
> configuration in passphrase mode.
> Not recommended to be used since WPS credentials should not
> be shared to other devices, making it in readable format increases
> that risk, also passphrase requires pbkdf2 to convert in psk.
> 
> 
> 
> Default value:* No (disabled)





##### CONFIG_ESP_WIFI_DEBUG_PRINT



> Print debug messages from WPA Supplicant
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this option to print logging information from WPA supplicant,
> this includes handshake information and key hex dumps depending
> on the project logging level.
> 
> 
> Enabling this could increase the build size ~60kb
> depending on the project logging level.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_TESTING_OPTIONS



> Add DPP testing code
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this to enable unity test for DPP.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_ESP_WIFI_ENTERPRISE_SUPPORT



> Enable enterprise option
> 
> 
> *Found in:* Component config > Wi-Fi
> 
> 
> Select this to enable/disable enterprise connection support.
> 
> 
> disabling this will reduce binary size.
> disabling this will disable the use of any esp_wifi_sta_wpa2_ent_\* (as APIs will be meaningless)
> 
> 
> Note that when using bigger certificates on low-power chips without crypto
> hardware acceleration, it is recommended to adjust the task watchdog timer (TWDT)
> if it is enabled. For precise information on timing requirements, you can check
> performance numbers at https://github.com/espressif/mbedtls/wiki/Performance-Numbers.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_ESP_WIFI_ENT_FREE_DYNAMIC_BUFFER



> Free dynamic buffers during WiFi enterprise connection
> 
> 
> *Found in:* Component config > Wi-Fi > CONFIG_ESP_WIFI_ENTERPRISE_SUPPORT
> 
> 
> Select this configuration to free dynamic buffers during WiFi enterprise connection.
> This will enable chip to reduce heap consumption during WiFi enterprise connection.
> 
> 
> 
> Default value:* No (disabled)






#### Core dump


Contains:


* CONFIG_ESP_COREDUMP_CHECK_BOOT
* CONFIG_ESP_COREDUMP_DATA_FORMAT
* CONFIG_ESP_COREDUMP_CHECKSUM
* CONFIG_ESP_COREDUMP_TO_FLASH_OR_UART
* CONFIG_ESP_COREDUMP_UART_DELAY
* CONFIG_ESP_COREDUMP_FLASH_NO_OVERWRITE
* CONFIG_ESP_COREDUMP_LOGS
* CONFIG_ESP_COREDUMP_DECODE
* CONFIG_ESP_COREDUMP_CAPTURE_DRAM
* CONFIG_ESP_COREDUMP_MAX_TASKS_NUM
* CONFIG_ESP_COREDUMP_STACK_SIZE



##### CONFIG_ESP_COREDUMP_TO_FLASH_OR_UART



> Data destination
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Select place to store core dump: flash, uart or none (to disable core dumps generation).
> 
> 
> Core dumps to Flash are not available if PSRAM is used for task stacks.
> 
> 
> If core dump is configured to be stored in flash and custom partition table is used add
> corresponding entry to your CSV. For examples, please see predefined partition table CSV descriptions
> in the components/partition_table directory.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Flash (CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH)
> > * UART (CONFIG_ESP_COREDUMP_ENABLE_TO_UART)
> > * None (CONFIG_ESP_COREDUMP_ENABLE_TO_NONE)




##### CONFIG_ESP_COREDUMP_DATA_FORMAT



> Core dump data format
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Select the data format for core dump.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Binary format (CONFIG_ESP_COREDUMP_DATA_FORMAT_BIN)
> > * ELF format (CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF)




##### CONFIG_ESP_COREDUMP_CHECKSUM



> Core dump data integrity check
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Select the integrity check for the core dump.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Use CRC32 for integrity verification (CONFIG_ESP_COREDUMP_CHECKSUM_CRC32)
> > * Use SHA256 for integrity verification (CONFIG_ESP_COREDUMP_CHECKSUM_SHA256)




##### CONFIG_ESP_COREDUMP_CAPTURE_DRAM



> Include whole .bss and .data sections and heap data into core dump file
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Storing these sections can help with easier debugging and troubleshooting.
> However, additional storage space will be required in the core dump partition.
> At least 128KB should be reserved, but the actual amount required may vary based
> on the application's DRAM usage.
> Note that sections located in external RAM will not be stored.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF




##### CONFIG_ESP_COREDUMP_CHECK_BOOT



> Check core dump data integrity on boot
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> When enabled, if any data are found on the flash core dump partition,
> they will be checked by calculating their checksum.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH




##### CONFIG_ESP_COREDUMP_LOGS



> Enable coredump logs for debugging
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Enable/disable coredump logs. Logs strings from espcoredump component are
> placed in DRAM. Disabling these helps to save ~5KB of internal memory.




##### CONFIG_ESP_COREDUMP_MAX_TASKS_NUM



> Maximum number of tasks
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Maximum number of tasks snapshots in core dump.




##### CONFIG_ESP_COREDUMP_UART_DELAY



> Delay before print to UART
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Config delay (in ms) before printing core dump to UART.
> Delay can be interrupted by pressing Enter key.
> 
> 
> 
> Default value:* 0 if CONFIG_ESP_COREDUMP_ENABLE_TO_UART




##### CONFIG_ESP_COREDUMP_FLASH_NO_OVERWRITE



> Don't overwrite existing core dump
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Don't overwrite an existing core dump already present in flash.
> Enable this option to only keep the first of multiple core dumps.
> 
> 
> If enabled, the core dump partition must be erased before the first
> core dump can be written.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH




##### CONFIG_ESP_COREDUMP_STACK_SIZE



> Reserved stack size
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Size of the memory to be reserved for core dump stack. If 0 core dump process will run on
> the stack of crashed task/ISR, otherwise special stack will be allocated.
> To ensure that core dump itself will not overflow task/ISR stack set this to the value around 1300-1800
> depending on the chosen checksum calculation method. SHA256 method needs more stack space than CRC32.
> NOTE: It eats DRAM.




##### CONFIG_ESP_COREDUMP_DECODE



> Handling of UART core dumps in IDF Monitor
> 
> 
> *Found in:* Component config > Core dump
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Decode and show summary (info_corefile) (CONFIG_ESP_COREDUMP_DECODE_INFO)
> > * Don't decode (CONFIG_ESP_COREDUMP_DECODE_DISABLE)





#### FAT Filesystem support


Contains:


* CONFIG_FATFS_API_ENCODING
* CONFIG_FATFS_VFS_FSTAT_BLKSIZE
* CONFIG_FATFS_IMMEDIATE_FSYNC
* CONFIG_FATFS_USE_FASTSEEK
* CONFIG_FATFS_LONG_FILENAMES
* CONFIG_FATFS_MAX_LFN
* CONFIG_FATFS_FS_LOCK
* CONFIG_FATFS_VOLUME_COUNT
* CONFIG_FATFS_CHOOSE_CODEPAGE
* CONFIG_FATFS_LINK_LOCK
* CONFIG_FATFS_ALLOC_PREFER_EXTRAM
* CONFIG_FATFS_SECTOR_SIZE
* CONFIG_FATFS_TIMEOUT_MS
* CONFIG_FATFS_USE_DYN_BUFFERS
* CONFIG_FATFS_USE_LABEL
* CONFIG_FATFS_PER_FILE_CACHE



##### CONFIG_FATFS_VOLUME_COUNT



> Number of volumes
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> Number of volumes (logical drives) to use.
> 
> 
> 
> Range:* from 1 to 10
> 
> 
> 
> Default value:* 2




##### CONFIG_FATFS_LONG_FILENAMES



> Long filename support
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> Support long filenames in FAT. Long filename data increases
> memory usage. FATFS can be configured to store the buffer for
> long filename data in stack or heap.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No long filenames (CONFIG_FATFS_LFN_NONE)
> > * Long filename buffer in heap (CONFIG_FATFS_LFN_HEAP)
> > * Long filename buffer on stack (CONFIG_FATFS_LFN_STACK)




##### CONFIG_FATFS_SECTOR_SIZE



> Sector size
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> Specify the size of the sector in bytes for FATFS partition generator.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 512 (CONFIG_FATFS_SECTOR_512)
> > * 4096 (CONFIG_FATFS_SECTOR_4096)




##### CONFIG_FATFS_CHOOSE_CODEPAGE



> OEM Code Page
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> OEM code page used for file name encodings.
> 
> 
> If "Dynamic" is selected, code page can be chosen at runtime using
> f_setcp function. Note that choosing this option will increase
> application size by ~480kB.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Dynamic (all code pages supported) (CONFIG_FATFS_CODEPAGE_DYNAMIC)
> > * US (CP437) (CONFIG_FATFS_CODEPAGE_437)
> > * Arabic (CP720) (CONFIG_FATFS_CODEPAGE_720)
> > * Greek (CP737) (CONFIG_FATFS_CODEPAGE_737)
> > * KBL (CP771) (CONFIG_FATFS_CODEPAGE_771)
> > * Baltic (CP775) (CONFIG_FATFS_CODEPAGE_775)
> > * Latin 1 (CP850) (CONFIG_FATFS_CODEPAGE_850)
> > * Latin 2 (CP852) (CONFIG_FATFS_CODEPAGE_852)
> > * Cyrillic (CP855) (CONFIG_FATFS_CODEPAGE_855)
> > * Turkish (CP857) (CONFIG_FATFS_CODEPAGE_857)
> > * Portuguese (CP860) (CONFIG_FATFS_CODEPAGE_860)
> > * Icelandic (CP861) (CONFIG_FATFS_CODEPAGE_861)
> > * Hebrew (CP862) (CONFIG_FATFS_CODEPAGE_862)
> > * Canadian French (CP863) (CONFIG_FATFS_CODEPAGE_863)
> > * Arabic (CP864) (CONFIG_FATFS_CODEPAGE_864)
> > * Nordic (CP865) (CONFIG_FATFS_CODEPAGE_865)
> > * Russian (CP866) (CONFIG_FATFS_CODEPAGE_866)
> > * Greek 2 (CP869) (CONFIG_FATFS_CODEPAGE_869)
> > * Japanese (DBCS) (CP932) (CONFIG_FATFS_CODEPAGE_932)
> > * Simplified Chinese (DBCS) (CP936) (CONFIG_FATFS_CODEPAGE_936)
> > * Korean (DBCS) (CP949) (CONFIG_FATFS_CODEPAGE_949)
> > * Traditional Chinese (DBCS) (CP950) (CONFIG_FATFS_CODEPAGE_950)




##### CONFIG_FATFS_MAX_LFN



> Max long filename length
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> Maximum long filename length. Can be reduced to save RAM.




##### CONFIG_FATFS_API_ENCODING



> API character encoding
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> Choose encoding for character and string arguments/returns when using
> FATFS APIs. The encoding of arguments will usually depend on text
> editor settings.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * API uses ANSI/OEM encoding (CONFIG_FATFS_API_ENCODING_ANSI_OEM)
> > * API uses UTF-8 encoding (CONFIG_FATFS_API_ENCODING_UTF_8)




##### CONFIG_FATFS_FS_LOCK



> Number of simultaneously open files protected by lock function
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> This option sets the FATFS configuration value _FS_LOCK.
> The option _FS_LOCK switches file lock function to control duplicated file open
> and illegal operation to open objects.
> 
> 
> \* 0: Disable file lock function. To avoid volume corruption, application
> should avoid illegal open, remove and rename to the open objects.
> 
> 
> \* >0: Enable file lock function. The value defines how many files/sub-directories
> can be opened simultaneously under file lock control.
> 
> 
> Note that the file lock control is independent of re-entrancy.
> 
> 
> 
> Range:* from 0 to 65535
> 
> 
> 
> Default value:* 0




##### CONFIG_FATFS_TIMEOUT_MS



> Timeout for acquiring a file lock, ms
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> This option sets FATFS configuration value _FS_TIMEOUT, scaled to milliseconds.
> Sets the number of milliseconds FATFS will wait to acquire a mutex when
> operating on an open file. For example, if one task is performing a lengthy
> operation, another task will wait for the first task to release the lock,
> and time out after amount of time set by this option.
> 
> 
> 
> Default value:* 10000




##### CONFIG_FATFS_PER_FILE_CACHE



> Use separate cache for each file
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> This option affects FATFS configuration value _FS_TINY.
> 
> 
> If this option is set, _FS_TINY is 0, and each open file has its own cache,
> size of the cache is equal to the _MAX_SS variable (512 or 4096 bytes).
> This option uses more RAM if more than 1 file is open, but needs less reads
> and writes to the storage for some operations.
> 
> 
> If this option is not set, _FS_TINY is 1, and single cache is used for
> all open files, size is also equal to _MAX_SS variable. This reduces the
> amount of heap used when multiple files are open, but increases the number
> of read and write operations which FATFS needs to make.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_FATFS_ALLOC_PREFER_EXTRAM



> Prefer external RAM when allocating FATFS buffers
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> When the option is enabled, internal buffers used by FATFS will be allocated
> from external RAM. If the allocation from external RAM fails, the buffer will
> be allocated from the internal RAM.
> Disable this option if optimizing for performance. Enable this option if
> optimizing for internal memory size.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_SPIRAM_USE_CAPS_ALLOC || CONFIG_SPIRAM_USE_MALLOC




##### CONFIG_FATFS_USE_FASTSEEK



> Enable fast seek algorithm when using lseek function through VFS FAT
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> The fast seek feature enables fast backward/long seek operations without
> FAT access by using an in-memory CLMT (cluster link map table).
> Please note, fast-seek is only allowed for read-mode files, if a
> file is opened in write-mode, the seek mechanism will automatically fallback
> to the default implementation.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_FATFS_FAST_SEEK_BUFFER_SIZE



> Fast seek CLMT buffer size
> 
> 
> *Found in:* Component config > FAT Filesystem support > CONFIG_FATFS_USE_FASTSEEK
> 
> 
> If fast seek algorithm is enabled, this defines the size of
> CLMT buffer used by this algorithm in 32-bit word units.
> This value should be chosen based on prior knowledge of
> maximum elements of each file entry would store.
> 
> 
> 
> Default value:* 64 if CONFIG_FATFS_USE_FASTSEEK





##### CONFIG_FATFS_VFS_FSTAT_BLKSIZE



> Default block size
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> If set to 0, the 'newlib' library's default size (BLKSIZ) is used (128 B).
> If set to a non-zero value, the value is used as the block size.
> Default file buffer size is set to this value
> and the buffer is allocated when first attempt of reading/writing to a file is made.
> Increasing this value improves fread() speed, however the heap usage is increased as well.
> 
> 
> NOTE: The block size value is shared by all the filesystem functions
> accessing target media for given file descriptor!
> See 'Improving I/O performance' section of 'Maximizing Execution Speed' documentation page
> for more details.
> 
> 
> 
> Default value:* 0




##### CONFIG_FATFS_IMMEDIATE_FSYNC



> Enable automatic f_sync
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> Enables automatic calling of f_sync() to flush recent file changes after each call of vfs_fat_write(),
> vfs_fat_pwrite(), vfs_fat_link(), vfs_fat_truncate() and vfs_fat_ftruncate() functions.
> This feature improves file-consistency and size reporting accuracy for the FatFS,
> at a price on decreased performance due to frequent disk operations
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_FATFS_USE_LABEL



> Use FATFS volume label
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> Allows FATFS volume label to be specified using f_setlabel
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_FATFS_LINK_LOCK



> Perform the whole link operation under lock
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> If enabled, the whole link operation (including file copying) is performed under lock.
> This ensures that the link operation is atomic, but may cause performance for large files.
> It may create less fragmented file copy.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_FATFS_USE_DYN_BUFFERS



> Use dynamic buffers
> 
> 
> *Found in:* Component config > FAT Filesystem support
> 
> 
> If enabled, the buffers used by FATFS will be allocated separately from the rest of the structure.
> This option is useful when using multiple FATFS instances with different
> sector sizes, as the buffers will be allocated according to the sector size.
> If disabled, the greatest sector size will be used for all FATFS instances.
> (In most cases, this would be the sector size of Wear Levelling library)
> This might cause more memory to be used than necessary.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_WL_SECTOR_SIZE_4096





#### FreeRTOS


Contains:


* Kernel
* Port



##### Kernel


Contains:


* CONFIG_FREERTOS_CHECK_STACKOVERFLOW
* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
* CONFIG_FREERTOS_MAX_TASK_NAME_LEN
* CONFIG_FREERTOS_IDLE_TASK_STACKSIZE
* CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS
* CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE
* CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES
* CONFIG_FREERTOS_HZ
* CONFIG_FREERTOS_TIMER_QUEUE_LENGTH
* CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
* CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
* CONFIG_FREERTOS_TIMER_TASK_PRIORITY
* CONFIG_FREERTOS_TIMER_TASK_STACK_DEPTH
* CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
* CONFIG_FREERTOS_USE_IDLE_HOOK
* CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES
* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
* CONFIG_FREERTOS_USE_TICK_HOOK
* CONFIG_FREERTOS_USE_TICKLESS_IDLE
* CONFIG_FREERTOS_USE_TRACE_FACILITY
* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
* CONFIG_FREERTOS_UNICORE
* CONFIG_FREERTOS_SMP
* CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK



###### CONFIG_FREERTOS_SMP



> Run the Amazon SMP FreeRTOS kernel instead (FEATURE UNDER DEVELOPMENT)
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Amazon has released an SMP version of the FreeRTOS Kernel which can be found via the following link:
> https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/smp
> 
> 
> IDF has added an experimental port of this SMP kernel located in
> components/freertos/FreeRTOS-Kernel-SMP. Enabling this option will cause IDF to use the Amazon SMP
> kernel. Note that THIS FEATURE IS UNDER ACTIVE DEVELOPMENT, users use this at their own risk.
> 
> 
> Leaving this option disabled will mean the IDF FreeRTOS kernel is used instead, which is located in:
> components/freertos/FreeRTOS-Kernel. Both kernel versions are SMP capable, but differ in
> their implementation and features.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_UNICORE



> Run FreeRTOS only on first core
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> This version of FreeRTOS normally takes control of all cores of the CPU. Select this if you only want
> to start it on the first core. This is needed when e.g. another process needs complete control over the
> second core.




###### CONFIG_FREERTOS_HZ



> configTICK_RATE_HZ
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Sets the FreeRTOS tick interrupt frequency in Hz (see configTICK_RATE_HZ documentation for more
> details).
> 
> 
> 
> Range:* from 1 to 1000
> 
> 
> 
> Default value:* 100




###### CONFIG_FREERTOS_OPTIMIZED_SCHEDULER



> configUSE_PORT_OPTIMISED_TASK_SELECTION
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables port specific task selection method. This option can speed up the search of ready tasks
> when scheduling (see configUSE_PORT_OPTIMISED_TASK_SELECTION documentation for more details).




###### CONFIG_FREERTOS_CHECK_STACKOVERFLOW



> configCHECK_FOR_STACK_OVERFLOW
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables FreeRTOS to check for stack overflows (see configCHECK_FOR_STACK_OVERFLOW documentation for
> more details).
> 
> 
> Note: If users do not provide their own `vApplicationStackOverflowHook()` function, a default
> function will be provided by ESP-IDF.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No checking (CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE)
> > 
> > 
> > Do not check for stack overflows (configCHECK_FOR_STACK_OVERFLOW = 0)
> > * Check by stack pointer value (Method 1) (CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL)
> > 
> > 
> > Check for stack overflows on each context switch by checking if the stack pointer is in a valid
> > range. Quick but does not detect stack overflows that happened between context switches
> > (configCHECK_FOR_STACK_OVERFLOW = 1)
> > * Check using canary bytes (Method 2) (CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY)
> > 
> > 
> > Places some magic bytes at the end of the stack area and on each context switch, check if these
> > bytes are still intact. More thorough than just checking the pointer, but also slightly slower.
> > (configCHECK_FOR_STACK_OVERFLOW = 2)




###### CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS



> configNUM_THREAD_LOCAL_STORAGE_POINTERS
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Set the number of thread local storage pointers in each task (see
> configNUM_THREAD_LOCAL_STORAGE_POINTERS documentation for more details).
> 
> 
> Note: In ESP-IDF, this value must be at least 1. Index 0 is reserved for use by the pthreads API
> thread-local-storage. Other indexes can be used for any desired purpose.
> 
> 
> 
> Range:* from 1 to 256
> 
> 
> 
> Default value:* 1




###### CONFIG_FREERTOS_IDLE_TASK_STACKSIZE



> configMINIMAL_STACK_SIZE (Idle task stack size)
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Sets the idle task stack size in bytes (see configMINIMAL_STACK_SIZE documentation for more details).
> 
> 
> Note:
> 
> 
> * ESP-IDF specifies stack sizes in bytes instead of words.
> * The default size is enough for most use cases.
> * The stack size may need to be increased above the default if the app installs idle or thread local
> storage cleanup hooks that use a lot of stack memory.
> * Conversely, the stack size can be reduced to the minimum if non of the idle features are used.
> 
> 
> 
> Range:* from 768 to 32768
> 
> 
> 
> Default value:* 1536




###### CONFIG_FREERTOS_USE_IDLE_HOOK



> configUSE_IDLE_HOOK
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables the idle task application hook (see configUSE_IDLE_HOOK documentation for more details).
> 
> 
> Note:
> 
> 
> * The application must provide the hook function `void vApplicationIdleHook( void );`
> * `vApplicationIdleHook()` is called from FreeRTOS idle task(s)
> * The FreeRTOS idle hook is NOT the same as the ESP-IDF Idle Hook, but both can be enabled
> simultaneously.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK



> Use FreeRTOS minimal idle hook
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables the minimal idle task application hook (see configUSE_IDLE_HOOK documentation for more
> details).
> 
> 
> Note:
> 
> 
> * The application must provide the hook function `void vApplicationPassiveIdleHook( void );`
> * `vApplicationPassiveIdleHook()` is called from FreeRTOS minimal idle task(s)
> 
> 
> 
> Default value:* No (disabled) if CONFIG_FREERTOS_SMP




###### CONFIG_FREERTOS_USE_TICK_HOOK



> configUSE_TICK_HOOK
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables the tick hook (see configUSE_TICK_HOOK documentation for more details).
> 
> 
> Note:
> 
> 
> * The application must provide the hook function `void vApplicationTickHook( void );`
> * `vApplicationTickHook()` is called from FreeRTOS's tick handling function `xTaskIncrementTick()`
> * The FreeRTOS tick hook is NOT the same as the ESP-IDF Tick Interrupt Hook, but both can be enabled
> simultaneously.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_MAX_TASK_NAME_LEN



> configMAX_TASK_NAME_LEN
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Sets the maximum number of characters for task names (see configMAX_TASK_NAME_LEN documentation for
> more details).
> 
> 
> Note: For most uses, the default of 16 characters is sufficient.
> 
> 
> 
> Range:* from 1 to 256
> 
> 
> 
> Default value:* 16




###### CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY



> configENABLE_BACKWARD_COMPATIBILITY
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enable backward compatibility with APIs prior to FreeRTOS v8.0.0. (see
> configENABLE_BACKWARD_COMPATIBILITY documentation for more details).
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME



> configTIMER_SERVICE_TASK_NAME
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Sets the timer task's name (see configTIMER_SERVICE_TASK_NAME documentation for more details).
> 
> 
> 
> Default value:* "Tmr Svc"




###### CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY



> configTIMER_SERVICE_TASK_CORE_AFFINITY
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Sets the timer task's core affinity
> (see configTIMER_SERVICE_TASK_CORE_AFFINITY documentation for more details).
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CPU0 (CONFIG_FREERTOS_TIMER_TASK_AFFINITY_CPU0)
> > * CPU1 (CONFIG_FREERTOS_TIMER_TASK_AFFINITY_CPU1)
> > * No affinity (CONFIG_FREERTOS_TIMER_TASK_NO_AFFINITY)




###### CONFIG_FREERTOS_TIMER_TASK_PRIORITY



> configTIMER_TASK_PRIORITY
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Sets the timer task's priority (see configTIMER_TASK_PRIORITY documentation for more details).
> 
> 
> 
> Range:* from 1 to 25
> 
> 
> 
> Default value:* 1




###### CONFIG_FREERTOS_TIMER_TASK_STACK_DEPTH



> configTIMER_TASK_STACK_DEPTH
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Set the timer task's stack size (see configTIMER_TASK_STACK_DEPTH documentation for more details).
> 
> 
> 
> Range:* from 1536 to 32768
> 
> 
> 
> Default value:* 2048




###### CONFIG_FREERTOS_TIMER_QUEUE_LENGTH



> configTIMER_QUEUE_LENGTH
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Set the timer task's command queue length (see configTIMER_QUEUE_LENGTH documentation for more
> details).
> 
> 
> 
> Range:* from 5 to 20
> 
> 
> 
> Default value:* 10




###### CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE



> configQUEUE_REGISTRY_SIZE
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Set the size of the queue registry (see configQUEUE_REGISTRY_SIZE documentation for more details).
> 
> 
> Note: A value of 0 will disable queue registry functionality
> 
> 
> 
> Range:* from 0 to 20
> 
> 
> 
> Default value:* 0




###### CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES



> configTASK_NOTIFICATION_ARRAY_ENTRIES
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Set the size of the task notification array of each task. When increasing this value, keep in
> mind that this means additional memory for each and every task on the system.
> However, task notifications in general are more light weight compared to alternatives
> such as semaphores.
> 
> 
> 
> Range:* from 1 to 32
> 
> 
> 
> Default value:* 1




###### CONFIG_FREERTOS_USE_TRACE_FACILITY



> configUSE_TRACE_FACILITY
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables additional structure members and functions to assist with execution visualization and tracing
> (see configUSE_TRACE_FACILITY documentation for more details).
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS



> configUSE_STATS_FORMATTING_FUNCTIONS
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel > CONFIG_FREERTOS_USE_TRACE_FACILITY
> 
> 
> Set configUSE_TRACE_FACILITY and configUSE_STATS_FORMATTING_FUNCTIONS to 1 to include the
> `vTaskList()` and `vTaskGetRunTimeStats()` functions in the build (see
> configUSE_STATS_FORMATTING_FUNCTIONS documentation for more details).
> 
> 
> 
> Default value:* No (disabled) if CONFIG_FREERTOS_USE_TRACE_FACILITY




###### CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES



> configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enable list integrity checker
> (see configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES documentation for more details).
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID



> Enable display of xCoreID in vTaskList
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> If enabled, this will include an extra column when vTaskList is called to display the CoreID the task
> is pinned to (0,1) or -1 if not pinned.




###### CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS



> configGENERATE_RUN_TIME_STATS
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables collection of run time statistics for each task (see configGENERATE_RUN_TIME_STATS
> documentation for more details).
> 
> 
> Note: The clock used for run time statistics can be configured in FREERTOS_RUN_TIME_STATS_CLK.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE



> configRUN_TIME_COUNTER_TYPE
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel > CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
> 
> 
> Sets the data type used for the FreeRTOS run time stats. A larger data type can be used to reduce the
> frequency of the counter overflowing.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * uint32_t (CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U32)
> > 
> > 
> > configRUN_TIME_COUNTER_TYPE is set to uint32_t
> > * uint64_t (CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64)
> > 
> > 
> > configRUN_TIME_COUNTER_TYPE is set to uint64_t




###### CONFIG_FREERTOS_USE_TICKLESS_IDLE



> configUSE_TICKLESS_IDLE
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> If power management support is enabled, FreeRTOS will be able to put the system into light sleep mode
> when no tasks need to run for a number of ticks. This number can be set using
> FREERTOS_IDLE_TIME_BEFORE_SLEEP option. This feature is also known as "automatic light sleep".
> 
> 
> Note that timers created using esp_timer APIs may prevent the system from entering sleep mode, even
> when no tasks need to run. To skip unnecessary wake-up initialize a timer with the
> "skip_unhandled_events" option as true.
> 
> 
> If disabled, automatic light sleep support will be disabled.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_PM_ENABLE




###### CONFIG_FREERTOS_IDLE_TIME_BEFORE_SLEEP



> configEXPECTED_IDLE_TIME_BEFORE_SLEEP
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel > CONFIG_FREERTOS_USE_TICKLESS_IDLE
> 
> 
> FreeRTOS will enter light sleep mode if no tasks need to run for this number of ticks.
> You can enable PM_PROFILING feature in esp_pm components and dump the sleep status with
> esp_pm_dump_locks, if the proportion of rejected sleeps is too high, please increase
> this value to improve scheduling efficiency
> 
> 
> 
> Range:* from 2 to 4294967295 if CONFIG_FREERTOS_USE_TICKLESS_IDLE
> 
> 
> 
> Default value:* 3 if CONFIG_FREERTOS_USE_TICKLESS_IDLE




###### CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG



> configUSE_APPLICATION_TASK_TAG
> 
> 
> *Found in:* Component config > FreeRTOS > Kernel
> 
> 
> Enables task tagging functionality and its associated API (see configUSE_APPLICATION_TASK_TAG
> documentation for more details).
> 
> 
> 
> Default value:* No (disabled)





##### Port


Contains:


* CONFIG_FREERTOS_CHECK_MUTEX_GIVEN_BY_OWNER
* CONFIG_FREERTOS_RUN_TIME_STATS_CLK
* CONFIG_FREERTOS_INTERRUPT_BACKTRACE
* CONFIG_FREERTOS_WATCHPOINT_END_OF_STACK
* CONFIG_FREERTOS_ENABLE_STATIC_TASK_CLEAN_UP
* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
* CONFIG_FREERTOS_ISR_STACKSIZE
* CONFIG_FREERTOS_PLACE_FUNCTIONS_INTO_FLASH
* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE
* CONFIG_FREERTOS_CORETIMER
* CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER



###### CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER



> Wrap task functions
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> If enabled, all FreeRTOS task functions will be enclosed in a wrapper function. If a task function
> mistakenly returns (i.e. does not delete), the call flow will return to the wrapper function. The
> wrapper function will then log an error and abort the application. This option is also required for GDB
> backtraces and C++ exceptions to work correctly inside top-level task functions.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_FREERTOS_WATCHPOINT_END_OF_STACK



> Enable stack overflow debug watchpoint
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> FreeRTOS can check if a stack has overflown its bounds by checking either the value of the stack
> pointer or by checking the integrity of canary bytes. (See FREERTOS_CHECK_STACKOVERFLOW for more
> information.) These checks only happen on a context switch, and the situation that caused the stack
> overflow may already be long gone by then. This option will use the last debug memory watchpoint to
> allow breaking into the debugger (or panic'ing) as soon as any of the last 32 bytes on the stack of a
> task are overwritten. The side effect is that using gdb, you effectively have one hardware watchpoint
> less because the last one is overwritten as soon as a task switch happens.
> 
> 
> Another consequence is that due to alignment requirements of the watchpoint, the usable stack size
> decreases by up to 60 bytes. This is because the watchpoint region has to be aligned to its size and
> the size for the stack watchpoint in IDF is 32 bytes.
> 
> 
> This check only triggers if the stack overflow writes within 32 bytes near the end of the stack, rather
> than overshooting further, so it is worth combining this approach with one of the other stack overflow
> check methods.
> 
> 
> When this watchpoint is hit, gdb will stop with a SIGTRAP message. When no JTAG OCD is attached,
> esp-idf will panic on an unhandled debug exception.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS



> Enable thread local storage pointers deletion callbacks
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> ESP-IDF provides users with the ability to free TLSP memory by registering TLSP deletion callbacks.
> These callbacks are automatically called by FreeRTOS when a task is deleted. When this option is turned
> on, the memory reserved for TLSPs in the TCB is doubled to make space for storing the deletion
> callbacks. If the user does not wish to use TLSP deletion callbacks then this option could be turned
> off to save space in the TCB memory.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK



> Enable task pre-deletion hook
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> Enable this option to make FreeRTOS call a user provided hook function right before it deletes a task
> (i.e., frees/releases a dynamically/statically allocated task's memory). This is useful if users want
> to know when a task is actually deleted (in case the task's deletion is delegated to the IDLE task).
> 
> 
> If this config option is enabled, users must define a `void vTaskPreDeletionHook( void \* pxTCB )`
> hook function in their application.




###### CONFIG_FREERTOS_ENABLE_STATIC_TASK_CLEAN_UP



> Enable static task clean up hook (DEPRECATED)
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> THIS OPTION IS DEPRECATED. Use FREERTOS_TASK_PRE_DELETION_HOOK instead.
> 
> 
> Enable this option to make FreeRTOS call the static task clean up hook when a task is deleted.
> 
> 
> Note: Users will need to provide a `void vPortCleanUpTCB ( void \*pxTCB )` callback
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_CHECK_MUTEX_GIVEN_BY_OWNER



> Check that mutex semaphore is given by owner task
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> If enabled, assert that when a mutex semaphore is given, the task giving the semaphore is the task
> which is currently holding the mutex.




###### CONFIG_FREERTOS_ISR_STACKSIZE



> ISR stack size
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> The interrupt handlers have their own stack. The size of the stack can be defined here. Each processor
> has its own stack, so the total size occupied will be twice this.
> 
> 
> 
> Range:* from 2096 to 32768 if CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF
> * from 1536 to 32768
> 
> 
> 
> Default value:* 2096 if CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF
> * 1536




###### CONFIG_FREERTOS_INTERRUPT_BACKTRACE



> Enable backtrace from interrupt to task context
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> If this option is enabled, interrupt stack frame will be modified to point to the code of the
> interrupted task as its return address. This helps the debugger (or the panic handler) show a backtrace
> from the interrupt to the task which was interrupted. This also works for nested interrupts: higher
> level interrupt stack can be traced back to the lower level interrupt. This option adds 4 instructions
> to the interrupt dispatching code.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_FREERTOS_CORETIMER



> Tick timer source (Xtensa Only)
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> FreeRTOS needs a timer with an associated interrupt to use as the main tick source to increase
> counters, run timers and do pre-emptive multitasking with. There are multiple timers available to do
> this, with different interrupt priorities.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Timer 0 (int 6, level 1) (CONFIG_FREERTOS_CORETIMER_0)
> > 
> > 
> > Select this to use timer 0
> > * Timer 1 (int 15, level 3) (CONFIG_FREERTOS_CORETIMER_1)
> > 
> > 
> > Select this to use timer 1
> > * SYSTIMER 0 (level 1) (CONFIG_FREERTOS_CORETIMER_SYSTIMER_LVL1)
> > 
> > 
> > Select this to use systimer with the 1 interrupt priority.
> > * SYSTIMER 0 (level 3) (CONFIG_FREERTOS_CORETIMER_SYSTIMER_LVL3)
> > 
> > 
> > Select this to use systimer with the 3 interrupt priority.




###### CONFIG_FREERTOS_RUN_TIME_STATS_CLK



> Choose the clock source for run time stats
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> Choose the clock source for FreeRTOS run time stats. Options are CPU0's CPU Clock or the ESP Timer.
> Both clock sources are 32 bits. The CPU Clock can run at a higher frequency hence provide a finer
> resolution but will overflow much quicker. Note that run time stats are only valid until the clock
> source overflows.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Use ESP TIMER for run time stats (CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER)
> > 
> > 
> > ESP Timer will be used as the clock source for FreeRTOS run time stats. The ESP Timer runs at a
> > frequency of 1MHz regardless of Dynamic Frequency Scaling. Therefore the ESP Timer will overflow in
> > approximately 4290 seconds.
> > * Use CPU Clock for run time stats (CONFIG_FREERTOS_RUN_TIME_STATS_USING_CPU_CLK)
> > 
> > 
> > CPU Clock will be used as the clock source for the generation of run time stats. The CPU Clock has
> > a frequency dependent on ESP_DEFAULT_CPU_FREQ_MHZ and Dynamic Frequency Scaling (DFS). Therefore
> > the CPU Clock frequency can fluctuate between 80 to 240MHz. Run time stats generated using the CPU
> > Clock represents the number of CPU cycles each task is allocated and DOES NOT reflect the amount of
> > time each task runs for (as CPU clock frequency can change). If the CPU clock consistently runs at
> > the maximum frequency of 240MHz, it will overflow in approximately 17 seconds.




###### CONFIG_FREERTOS_PLACE_FUNCTIONS_INTO_FLASH



> Place FreeRTOS functions into Flash
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> When enabled the selected Non-ISR FreeRTOS functions will be placed into Flash memory instead of IRAM.
> This saves up to 8KB of IRAM depending on which functions are used.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE



> Tests compliance with Vanilla FreeRTOS port\*_CRITICAL calls
> 
> 
> *Found in:* Component config > FreeRTOS > Port
> 
> 
> If enabled, context of port\*_CRITICAL calls (ISR or Non-ISR) would be checked to be in compliance with
> Vanilla FreeRTOS. e.g Calling port\*_CRITICAL from ISR context would cause assert failure
> 
> 
> 
> Default value:* No (disabled)






#### Hardware Abstraction Layer (HAL) and Low Level (LL)


Contains:


* CONFIG_HAL_DEFAULT_ASSERTION_LEVEL
* CONFIG_HAL_LOG_LEVEL
* CONFIG_HAL_SYSTIMER_USE_ROM_IMPL
* CONFIG_HAL_WDT_USE_ROM_IMPL



##### CONFIG_HAL_DEFAULT_ASSERTION_LEVEL



> Default HAL assertion level
> 
> 
> *Found in:* Component config > Hardware Abstraction Layer (HAL) and Low Level (LL)
> 
> 
> Set the assert behavior / level for HAL component.
> HAL component assert level can be set separately,
> but the level can't exceed the system assertion level.
> e.g. If the system assertion is disabled, then the HAL
> assertion can't be enabled either. If the system assertion
> is enable, then the HAL assertion can still be disabled
> by this Kconfig option.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Same as system assertion level (CONFIG_HAL_ASSERTION_EQUALS_SYSTEM)
> > * Disabled (CONFIG_HAL_ASSERTION_DISABLE)
> > * Silent (CONFIG_HAL_ASSERTION_SILENT)
> > * Enabled (CONFIG_HAL_ASSERTION_ENABLE)




##### CONFIG_HAL_LOG_LEVEL



> HAL layer log verbosity
> 
> 
> *Found in:* Component config > Hardware Abstraction Layer (HAL) and Low Level (LL)
> 
> 
> Specify how much output to see in HAL logs.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No output (CONFIG_HAL_LOG_LEVEL_NONE)
> > * Error (CONFIG_HAL_LOG_LEVEL_ERROR)
> > * Warning (CONFIG_HAL_LOG_LEVEL_WARN)
> > * Info (CONFIG_HAL_LOG_LEVEL_INFO)
> > * Debug (CONFIG_HAL_LOG_LEVEL_DEBUG)
> > * Verbose (CONFIG_HAL_LOG_LEVEL_VERBOSE)




##### CONFIG_HAL_SYSTIMER_USE_ROM_IMPL



> Use ROM implementation of SysTimer HAL driver
> 
> 
> *Found in:* Component config > Hardware Abstraction Layer (HAL) and Low Level (LL)
> 
> 
> Enable this flag to use HAL functions from ROM instead of ESP-IDF.
> 
> 
> If keeping this as "n" in your project, you will have less free IRAM.
> If making this as "y" in your project, you will increase free IRAM,
> but you will lose the possibility to debug this module, and some new
> features will be added and bugs will be fixed in the IDF source
> but cannot be synced to ROM.
> 
> 
> 
> Default value:* Yes (enabled) if ESP_ROM_HAS_HAL_SYSTIMER




##### CONFIG_HAL_WDT_USE_ROM_IMPL



> Use ROM implementation of WDT HAL driver
> 
> 
> *Found in:* Component config > Hardware Abstraction Layer (HAL) and Low Level (LL)
> 
> 
> Enable this flag to use HAL functions from ROM instead of ESP-IDF.
> 
> 
> If keeping this as "n" in your project, you will have less free IRAM.
> If making this as "y" in your project, you will increase free IRAM,
> but you will lose the possibility to debug this module, and some new
> features will be added and bugs will be fixed in the IDF source
> but cannot be synced to ROM.
> 
> 
> 
> Default value:* Yes (enabled)





#### Heap memory debugging


Contains:


* CONFIG_HEAP_ABORT_WHEN_ALLOCATION_FAILS
* CONFIG_HEAP_TASK_TRACKING
* CONFIG_HEAP_PLACE_FUNCTION_INTO_FLASH
* CONFIG_HEAP_CORRUPTION_DETECTION
* CONFIG_HEAP_TRACING_DEST
* CONFIG_HEAP_TRACING_STACK_DEPTH
* CONFIG_HEAP_USE_HOOKS
* CONFIG_HEAP_TRACE_HASH_MAP
* CONFIG_HEAP_TLSF_USE_ROM_IMPL



##### CONFIG_HEAP_CORRUPTION_DETECTION



> Heap corruption detection
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Enable heap poisoning features to detect heap corruption caused by out-of-bounds access to heap memory.
> 
> 
> See the "Heap Memory Debugging" page of the IDF documentation
> for a description of each level of heap corruption detection.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Basic (no poisoning) (CONFIG_HEAP_POISONING_DISABLED)
> > * Light impact (CONFIG_HEAP_POISONING_LIGHT)
> > * Comprehensive (CONFIG_HEAP_POISONING_COMPREHENSIVE)




##### CONFIG_HEAP_TRACING_DEST



> Heap tracing
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Enables the heap tracing API defined in esp_heap_trace.h.
> 
> 
> This function causes a moderate increase in IRAM code side and a minor increase in heap function
> (malloc/free/realloc) CPU overhead, even when the tracing feature is not used.
> So it's best to keep it disabled unless tracing is being used.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Disabled (CONFIG_HEAP_TRACING_OFF)
> > * Standalone (CONFIG_HEAP_TRACING_STANDALONE)
> > * Host-based (CONFIG_HEAP_TRACING_TOHOST)




##### CONFIG_HEAP_TRACING_STACK_DEPTH



> Heap tracing stack depth
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Number of stack frames to save when tracing heap operation callers.
> 
> 
> More stack frames uses more memory in the heap trace buffer (and slows down allocation), but
> can provide useful information.




##### CONFIG_HEAP_USE_HOOKS



> Use allocation and free hooks
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Enable the user to implement function hooks triggered for each successful allocation and free.




##### CONFIG_HEAP_TASK_TRACKING



> Enable heap task tracking
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Enables tracking the task responsible for each heap allocation.
> 
> 
> This function depends on heap poisoning being enabled and adds four more bytes of overhead for each block
> allocated.




##### CONFIG_HEAP_TRACE_HASH_MAP



> Use hash map mechanism to access heap trace records
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Enable this flag to use a hash map to increase performance in handling
> heap trace records.
> 
> 
> Heap trace standalone supports storing records as a list, or a list + hash map.
> 
> 
> Using only a list takes less memory, but calls to 'free' will get slower as the
> list grows. This is particularly affected when using HEAP_TRACE_ALL mode.
> 
> 
> By using a list + hash map, calls to 'free' remain fast, at the cost of
> additional memory to store the hash map.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_HEAP_TRACING_STANDALONE



###### CONFIG_HEAP_TRACE_HASH_MAP_IN_EXT_RAM



> Place hash map in external RAM
> 
> 
> *Found in:* Component config > Heap memory debugging > CONFIG_HEAP_TRACE_HASH_MAP
> 
> 
> When enabled this configuration forces the hash map to be placed in external RAM.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_HEAP_TRACE_HASH_MAP




###### CONFIG_HEAP_TRACE_HASH_MAP_SIZE



> The number of entries in the hash map
> 
> 
> *Found in:* Component config > Heap memory debugging > CONFIG_HEAP_TRACE_HASH_MAP
> 
> 
> Defines the number of entries in the heap trace hashmap. Each entry takes 8 bytes.
> The bigger this number is, the better the performance. Recommended range: 200 - 2000.
> 
> 
> 
> Default value:* 512 if CONFIG_HEAP_TRACE_HASH_MAP





##### CONFIG_HEAP_ABORT_WHEN_ALLOCATION_FAILS



> Abort if memory allocation fails
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> When enabled, if a memory allocation operation fails it will cause a system abort.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_HEAP_TLSF_USE_ROM_IMPL



> Use ROM implementation of heap tlsf library
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Enable this flag to use heap functions from ROM instead of ESP-IDF.
> 
> 
> If keeping this as "n" in your project, you will have less free IRAM.
> If making this as "y" in your project, you will increase free IRAM,
> but you will lose the possibility to debug this module, and some new
> features will be added and bugs will be fixed in the IDF source
> but cannot be synced to ROM.
> 
> 
> 
> Default value:* Yes (enabled) if ESP_ROM_HAS_HEAP_TLSF




##### CONFIG_HEAP_PLACE_FUNCTION_INTO_FLASH



> Force the entire heap component to be placed in flash memory
> 
> 
> *Found in:* Component config > Heap memory debugging
> 
> 
> Enable this flag to save up RAM space by placing the heap component in the flash memory
> 
> 
> Note that it is only safe to enable this configuration if no functions from esp_heap_caps.h
> or esp_heap_trace.h are called from ISR.





#### IEEE 802.15.4


Contains:


* CONFIG_IEEE802154_ENABLED



##### CONFIG_IEEE802154_ENABLED



> IEEE802154 Enable
> 
> 
> *Found in:* Component config > IEEE 802.15.4
> 
> 
> 
> Default value:* Yes (enabled) if SOC_IEEE802154_SUPPORTED



###### CONFIG_IEEE802154_RX_BUFFER_SIZE



> The number of 802.15.4 receive buffers
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> The number of 802.15.4 receive buffers
> 
> 
> 
> Range:* from 2 to 100 if CONFIG_IEEE802154_ENABLED
> 
> 
> 
> Default value:* 20 if CONFIG_IEEE802154_ENABLED




###### CONFIG_IEEE802154_CCA_MODE



> Clear Channel Assessment (CCA) mode
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> configure the CCA mode
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Carrier sense only (CONFIG_IEEE802154_CCA_CARRIER)
> > 
> > 
> > configure the CCA mode to Energy above threshold
> > * Energy above threshold (CONFIG_IEEE802154_CCA_ED)
> > 
> > 
> > configure the CCA mode to Energy above threshold
> > * Carrier sense OR energy above threshold (CONFIG_IEEE802154_CCA_CARRIER_OR_ED)
> > 
> > 
> > configure the CCA mode to Carrier sense OR energy above threshold
> > * Carrier sense AND energy above threshold (CONFIG_IEEE802154_CCA_CARRIER_AND_ED)
> > 
> > 
> > configure the CCA mode to Carrier sense AND energy above threshold




###### CONFIG_IEEE802154_CCA_THRESHOLD



> CCA detection threshold
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> set the CCA threshold, in dB
> 
> 
> 
> Range:* from -120 to 0 if CONFIG_IEEE802154_ENABLED
> 
> 
> 
> Default value:* "-60" if CONFIG_IEEE802154_ENABLED




###### CONFIG_IEEE802154_PENDING_TABLE_SIZE



> Pending table size
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> set the pending table size
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_IEEE802154_ENABLED
> 
> 
> 
> Default value:* 20 if CONFIG_IEEE802154_ENABLED




###### CONFIG_IEEE802154_MULTI_PAN_ENABLE



> Enable multi-pan feature for frame filter
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> Enable IEEE802154 multi-pan
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_ENABLED




###### CONFIG_IEEE802154_TIMING_OPTIMIZATION



> Enable throughput optimization
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> Enabling this option increases throughput by ~5% at the expense of ~2.1k
> IRAM code size increase.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_ENABLED




###### CONFIG_IEEE802154_SLEEP_ENABLE



> Enable IEEE802154 light sleep
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> Enabling this option allows the IEEE802.15.4 module to be powered down during automatic light sleep,
> which reduces current consumption.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_PM_ENABLE && CONFIG_IEEE802154_ENABLED




###### CONFIG_IEEE802154_DEBUG



> Enable IEEE802154 Debug
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED
> 
> 
> Enabling this option allows different kinds of IEEE802154 debug output.
> All IEEE802154 debug features increase the size of the final binary.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_ENABLED


Contains:


* CONFIG_IEEE802154_RECORD_ABORT
* CONFIG_IEEE802154_RECORD_CMD
* CONFIG_IEEE802154_RECORD_EVENT
* CONFIG_IEEE802154_RECORD_STATE
* CONFIG_IEEE802154_TXRX_STATISTIC
* CONFIG_IEEE802154_ASSERT




###### CONFIG_IEEE802154_ASSERT



> Enrich the assert information with IEEE802154 state and event
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG
> 
> 
> Enabling this option to add some probe codes in the driver, and these informations
> will be printed when assert.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_DEBUG




###### CONFIG_IEEE802154_RECORD_EVENT



> Enable record event information for debugging
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG
> 
> 
> Enabling this option to record event, when assert, the recorded event will be printed.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_DEBUG




###### CONFIG_IEEE802154_RECORD_EVENT_SIZE



> Record event table size
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG > CONFIG_IEEE802154_RECORD_EVENT
> 
> 
> set the record event table size
> 
> 
> 
> Range:* from 1 to 50 if CONFIG_IEEE802154_RECORD_EVENT
> 
> 
> 
> Default value:* 30 if CONFIG_IEEE802154_RECORD_EVENT




###### CONFIG_IEEE802154_RECORD_STATE



> Enable record state information for debugging
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG
> 
> 
> Enabling this option to record state, when assert, the recorded state will be printed.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_DEBUG




###### CONFIG_IEEE802154_RECORD_STATE_SIZE



> Record state table size
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG > CONFIG_IEEE802154_RECORD_STATE
> 
> 
> set the record state table size
> 
> 
> 
> Range:* from 1 to 50 if CONFIG_IEEE802154_RECORD_STATE
> 
> 
> 
> Default value:* 10 if CONFIG_IEEE802154_RECORD_STATE




###### CONFIG_IEEE802154_RECORD_CMD



> Enable record command information for debugging
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG
> 
> 
> Enabling this option to record the command, when assert, the recorded
> command will be printed.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_DEBUG




###### CONFIG_IEEE802154_RECORD_CMD_SIZE



> Record command table size
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG > CONFIG_IEEE802154_RECORD_CMD
> 
> 
> set the record command table size
> 
> 
> 
> Range:* from 1 to 50 if CONFIG_IEEE802154_RECORD_CMD
> 
> 
> 
> Default value:* 10 if CONFIG_IEEE802154_RECORD_CMD




###### CONFIG_IEEE802154_RECORD_ABORT



> Enable record abort information for debugging
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG
> 
> 
> Enabling this option to record the abort, when assert, the recorded
> abort will be printed.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_DEBUG




###### CONFIG_IEEE802154_RECORD_ABORT_SIZE



> Record abort table size
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG > CONFIG_IEEE802154_RECORD_ABORT
> 
> 
> set the record abort table size
> 
> 
> 
> Range:* from 1 to 50 if CONFIG_IEEE802154_RECORD_ABORT
> 
> 
> 
> Default value:* 10 if CONFIG_IEEE802154_RECORD_ABORT




###### CONFIG_IEEE802154_TXRX_STATISTIC



> Enable record tx/rx packets information for debugging
> 
> 
> *Found in:* Component config > IEEE 802.15.4 > CONFIG_IEEE802154_ENABLED > CONFIG_IEEE802154_DEBUG
> 
> 
> Enabling this option to record the tx and rx
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IEEE802154_DEBUG






#### Log output


Contains:


* CONFIG_LOG_DEFAULT_LEVEL
* CONFIG_LOG_MASTER_LEVEL
* CONFIG_LOG_TIMESTAMP_SOURCE
* CONFIG_LOG_MAXIMUM_LEVEL
* CONFIG_LOG_COLORS



##### CONFIG_LOG_DEFAULT_LEVEL



> Default log verbosity
> 
> 
> *Found in:* Component config > Log output
> 
> 
> Specify how much output to see in logs by default.
> You can set lower verbosity level at runtime using
> esp_log_level_set function.
> 
> 
> By default, this setting limits which log statements
> are compiled into the program. For example, selecting
> "Warning" would mean that changing log level to "Debug"
> at runtime will not be possible. To allow increasing log
> level above the default at runtime, see the next option.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No output (CONFIG_LOG_DEFAULT_LEVEL_NONE)
> > * Error (CONFIG_LOG_DEFAULT_LEVEL_ERROR)
> > * Warning (CONFIG_LOG_DEFAULT_LEVEL_WARN)
> > * Info (CONFIG_LOG_DEFAULT_LEVEL_INFO)
> > * Debug (CONFIG_LOG_DEFAULT_LEVEL_DEBUG)
> > * Verbose (CONFIG_LOG_DEFAULT_LEVEL_VERBOSE)




##### CONFIG_LOG_MAXIMUM_LEVEL



> Maximum log verbosity
> 
> 
> *Found in:* Component config > Log output
> 
> 
> This config option sets the highest log verbosity that it's possible to select
> at runtime by calling esp_log_level_set(). This level may be higher than
> the default verbosity level which is set when the app starts up.
> 
> 
> This can be used enable debugging output only at a critical point, for a particular
> tag, or to minimize startup time but then enable more logs once the firmware has
> loaded.
> 
> 
> Note that increasing the maximum available log level will increase the firmware
> binary size.
> 
> 
> This option only applies to logging from the app, the bootloader log level is
> fixed at compile time to the separate "Bootloader log verbosity" setting.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Same as default (CONFIG_LOG_MAXIMUM_EQUALS_DEFAULT)
> > * Error (CONFIG_LOG_MAXIMUM_LEVEL_ERROR)
> > * Warning (CONFIG_LOG_MAXIMUM_LEVEL_WARN)
> > * Info (CONFIG_LOG_MAXIMUM_LEVEL_INFO)
> > * Debug (CONFIG_LOG_MAXIMUM_LEVEL_DEBUG)
> > * Verbose (CONFIG_LOG_MAXIMUM_LEVEL_VERBOSE)




##### CONFIG_LOG_MASTER_LEVEL



> Enable global master log level
> 
> 
> *Found in:* Component config > Log output
> 
> 
> Enables an additional global "master" log level check that occurs
> before a log tag cache lookup. This is useful if you want to
> compile in a lot of logs that are selectable at runtime, but avoid the
> performance hit during periods where you don't want log output. Examples
> include remote log forwarding, or disabling logs during a time-critical
> or CPU-intensive section and re-enabling them later. Results in
> larger program size depending on number of logs compiled in.
> 
> 
> If enabled, defaults to LOG_DEFAULT_LEVEL and can be set using
> esp_log_set_level_master().
> This check takes precedence over ESP_LOG_LEVEL_LOCAL.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LOG_COLORS



> Use ANSI terminal colors in log output
> 
> 
> *Found in:* Component config > Log output
> 
> 
> Enable ANSI terminal color codes in bootloader output.
> 
> 
> In order to view these, your terminal program must support ANSI color codes.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LOG_TIMESTAMP_SOURCE



> Log Timestamps
> 
> 
> *Found in:* Component config > Log output
> 
> 
> Choose what sort of timestamp is displayed in the log output:
> 
> 
> * Milliseconds since boot is calulated from the RTOS tick count multiplied
> by the tick period. This time will reset after a software reboot.
> e.g. (90000)
> * System time is taken from POSIX time functions which use the chip's
> RTC and high resoultion timers to maintain an accurate time. The system time is
> initialized to 0 on startup, it can be set with an SNTP sync, or with
> POSIX time functions. This time will not reset after a software reboot.
> e.g. (00:01:30.000)
> * NOTE: Currently this will not get used in logging from binary blobs
> (i.e WiFi & Bluetooth libraries), these will always print
> milliseconds since boot.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Milliseconds Since Boot (CONFIG_LOG_TIMESTAMP_SOURCE_RTOS)
> > * System Time (CONFIG_LOG_TIMESTAMP_SOURCE_SYSTEM)





#### LWIP


Contains:


* CONFIG_LWIP_CHECK_THREAD_SAFETY
* Checksums
* CONFIG_LWIP_DHCP_COARSE_TIMER_SECS
* DHCP server
* CONFIG_LWIP_DHCP_OPTIONS_LEN
* CONFIG_LWIP_DHCP_DISABLE_CLIENT_ID
* CONFIG_LWIP_DHCP_DISABLE_VENDOR_CLASS_ID
* CONFIG_LWIP_DHCP_DOES_ARP_CHECK
* CONFIG_LWIP_DHCP_RESTORE_LAST_IP
* DNS
* CONFIG_LWIP_PPP_CHAP_SUPPORT
* CONFIG_LWIP_L2_TO_L3_COPY
* CONFIG_LWIP_IPV6_DHCP6
* CONFIG_LWIP_IP4_FRAG
* CONFIG_LWIP_IP6_FRAG
* CONFIG_LWIP_IP_FORWARD
* CONFIG_LWIP_NETBUF_RECVINFO
* CONFIG_LWIP_IPV4
* CONFIG_LWIP_AUTOIP
* CONFIG_LWIP_IPV6
* CONFIG_LWIP_ENABLE_LCP_ECHO
* CONFIG_LWIP_ESP_LWIP_ASSERT
* CONFIG_LWIP_DEBUG
* CONFIG_LWIP_IRAM_OPTIMIZATION
* CONFIG_LWIP_EXTRA_IRAM_OPTIMIZATION
* CONFIG_LWIP_ENABLE
* CONFIG_LWIP_STATS
* CONFIG_LWIP_TIMERS_ONDEMAND
* CONFIG_LWIP_DNS_SUPPORT_MDNS_QUERIES
* CONFIG_LWIP_PPP_MPPE_SUPPORT
* CONFIG_LWIP_PPP_MSCHAP_SUPPORT
* CONFIG_LWIP_PPP_NOTIFY_PHASE_SUPPORT
* CONFIG_LWIP_PPP_PAP_SUPPORT
* CONFIG_LWIP_PPP_DEBUG_ON
* CONFIG_LWIP_PPP_SERVER_SUPPORT
* CONFIG_LWIP_PPP_SUPPORT
* CONFIG_LWIP_IP4_REASSEMBLY
* CONFIG_LWIP_IP6_REASSEMBLY
* CONFIG_LWIP_SLIP_SUPPORT
* CONFIG_LWIP_SO_LINGER
* CONFIG_LWIP_SO_RCVBUF
* CONFIG_LWIP_SO_REUSE
* CONFIG_LWIP_NETIF_STATUS_CALLBACK
* CONFIG_LWIP_TCPIP_CORE_LOCKING
* CONFIG_LWIP_NETIF_API
* CONFIG_LWIP_PPP_VJ_HEADER_COMPRESSION
* Hooks
* ICMP
* CONFIG_LWIP_LOCAL_HOSTNAME
* CONFIG_LWIP_ND6
* LWIP RAW API
* CONFIG_LWIP_TCPIP_TASK_PRIO
* CONFIG_LWIP_IPV6_ND6_NUM_NEIGHBORS
* CONFIG_LWIP_IPV6_MEMP_NUM_ND6_QUEUE
* CONFIG_LWIP_MAX_SOCKETS
* CONFIG_LWIP_BRIDGEIF_MAX_PORTS
* CONFIG_LWIP_NUM_NETIF_CLIENT_DATA
* CONFIG_LWIP_ESP_GRATUITOUS_ARP
* CONFIG_LWIP_ESP_MLDV6_REPORT
* SNTP
* CONFIG_LWIP_USE_ONLY_LWIP_SELECT
* CONFIG_LWIP_NETIF_LOOPBACK
* TCP
* CONFIG_LWIP_TCPIP_TASK_AFFINITY
* CONFIG_LWIP_TCPIP_TASK_STACK_SIZE
* CONFIG_LWIP_TCPIP_RECVMBOX_SIZE
* CONFIG_LWIP_IP_REASS_MAX_PBUFS
* CONFIG_LWIP_IP_DEFAULT_TTL
* UDP
* CONFIG_LWIP_IPV6_RDNSS_MAX_DNS_SERVERS



##### CONFIG_LWIP_ENABLE



> Enable LwIP stack
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Builds normally if selected. Excludes LwIP from build if unselected, even if it is a
> dependency of a component or application.
> Some applications can switch their IP stacks, e.g., when switching between chip
> and Linux targets (LwIP stack vs. Linux IP stack). Since the LwIP dependency cannot
> easily be excluded based on a Kconfig option, it has to be a dependency in all cases.
> This switch allows the LwIP stack to be built selectively, even if it is a dependency.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_LOCAL_HOSTNAME



> Local netif hostname
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> The default name this device will report to other devices on the network.
> Could be updated at runtime with esp_netif_set_hostname()
> 
> 
> 
> Default value:* "espressif"




##### CONFIG_LWIP_NETIF_API



> Enable usage of standard POSIX APIs in LWIP
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> If this feature is enabled, standard POSIX APIs: if_indextoname(), if_nametoindex()
> could be used to convert network interface index to name
> instead of IDF specific esp-netif APIs (such as esp_netif_get_netif_impl_name())
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_TCPIP_TASK_PRIO



> LWIP TCP/IP Task Priority
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> LWIP tcpip task priority. In case of high throughput, this parameter
> could be changed up to (configMAX_PRIORITIES-1).
> 
> 
> 
> Range:* from 1 to 24
> 
> 
> 
> Default value:* 18




##### CONFIG_LWIP_TCPIP_CORE_LOCKING



> Enable tcpip core locking
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> If Enable tcpip core locking,Creates a global mutex that is held
> during TCPIP thread operations.Can be locked by client code to perform
> lwIP operations without changing into TCPIP thread using callbacks.
> See LOCK_TCPIP_CORE() and UNLOCK_TCPIP_CORE().
> 
> 
> If disable tcpip core locking,TCP IP will perform tasks through context switching
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_LWIP_TCPIP_CORE_LOCKING_INPUT



> Enable tcpip core locking input
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_TCPIP_CORE_LOCKING
> 
> 
> when LWIP_TCPIP_CORE_LOCKING is enabled, this lets tcpip_input() grab the
> mutex for input packets as well, instead of allocating a message and passing
> it to tcpip_thread.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_TCPIP_CORE_LOCKING





##### CONFIG_LWIP_CHECK_THREAD_SAFETY



> Checks that lwip API runs in expected context
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable to check that the project does not violate lwip thread safety.
> If enabled, all lwip functions that require thread awareness run an assertion
> to verify that the TCP/IP core functionality is either locked or accessed
> from the correct thread.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_DNS_SUPPORT_MDNS_QUERIES



> Enable mDNS queries in resolving host name
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> If this feature is enabled, standard API such as gethostbyname
> support .local addresses by sending one shot multicast mDNS
> query
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_L2_TO_L3_COPY



> Enable copy between Layer2 and Layer3 packets
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> If this feature is enabled, all traffic from layer2(WIFI Driver) will be
> copied to a new buffer before sending it to layer3(LWIP stack), freeing
> the layer2 buffer.
> Please be notified that the total layer2 receiving buffer is fixed and
> ESP32 currently supports 25 layer2 receiving buffer, when layer2 buffer
> runs out of memory, then the incoming packets will be dropped in hardware.
> The layer3 buffer is allocated from the heap, so the total layer3 receiving
> buffer depends on the available heap size, when heap runs out of memory,
> no copy will be sent to layer3 and packet will be dropped in layer2.
> Please make sure you fully understand the impact of this feature before
> enabling it.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_IRAM_OPTIMIZATION



> Enable LWIP IRAM optimization
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> If this feature is enabled, some functions relating to RX/TX in LWIP will be
> put into IRAM, it can improve UDP/TCP throughput by >10% for single core mode,
> it doesn't help too much for dual core mode. On the other hand, it needs about
> 10KB IRAM for these optimizations.
> 
> 
> If this feature is disabled, all lwip functions will be put into FLASH.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_EXTRA_IRAM_OPTIMIZATION



> Enable LWIP IRAM optimization for TCP part
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> If this feature is enabled, some tcp part functions relating to RX/TX in LWIP will be
> put into IRAM, it can improve TCP throughput. On the other hand, it needs about 17KB
> IRAM for these optimizations.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_TIMERS_ONDEMAND



> Enable LWIP Timers on demand
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> If this feature is enabled, IGMP and MLD6 timers will be activated only
> when joining groups or receiving QUERY packets.
> 
> 
> This feature will reduce the power consumption for applications which do not
> use IGMP and MLD6.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_ND6



> LWIP NDP6 Enable/Disable
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> This option is used to disable the Network Discovery Protocol (NDP) if it is not required.
> Please use this option with caution, as the NDP is essential for IPv6 functionality within a local network.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_LWIP_FORCE_ROUTER_FORWARDING



> LWIP Force Router Forwarding Enable/Disable
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_ND6
> 
> 
> This option is used to set the the router flag for the NA packets.
> When enabled, the router flag in NA packet will always set to 1,
> otherwise, never set router flag for NA packets.
> 
> 
> 
> Default value:* No (disabled)





##### CONFIG_LWIP_MAX_SOCKETS



> Max number of open sockets
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Sockets take up a certain amount of memory, and allowing fewer
> sockets to be open at the same time conserves memory. Specify
> the maximum amount of sockets here. The valid value is from 1
> to 16.
> 
> 
> 
> Range:* from 1 to 16
> 
> 
> 
> Default value:* 10




##### CONFIG_LWIP_USE_ONLY_LWIP_SELECT



> Support LWIP socket select() only (DEPRECATED)
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> This option is deprecated. Do not use this option, use VFS_SUPPORT_SELECT instead.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_SO_LINGER



> Enable SO_LINGER processing
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows SO_LINGER processing.
> l_onoff = 1,l_linger can set the timeout.
> 
> 
> If l_linger=0, When a connection is closed, TCP will terminate the connection.
> This means that TCP will discard any data packets stored in the socket send buffer
> and send an RST to the peer.
> 
> 
> If l_linger!=0,Then closesocket() calls to block the process until
> the remaining data packets has been sent or timed out.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_SO_REUSE



> Enable SO_REUSEADDR option
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows binding to a port which remains in
> TIME_WAIT.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_LWIP_SO_REUSE_RXTOALL



> SO_REUSEADDR copies broadcast/multicast to all matches
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_SO_REUSE
> 
> 
> Enabling this option means that any incoming broadcast or multicast
> packet will be copied to all of the local sockets that it matches
> (may be more than one if SO_REUSEADDR is set on the socket.)
> 
> 
> This increases memory overhead as the packets need to be copied,
> however they are only copied per matching socket. You can safely
> disable it if you don't plan to receive broadcast or multicast
> traffic on more than one socket at a time.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_LWIP_SO_RCVBUF



> Enable SO_RCVBUF option
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows checking for available data on a netconn.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_NETBUF_RECVINFO



> Enable IP_PKTINFO option
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows checking for the destination address
> of a received IPv4 Packet.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_IP_DEFAULT_TTL



> The value for Time-To-Live used by transport layers
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Set value for Time-To-Live used by transport layers.
> 
> 
> 
> Range:* from 1 to 255
> 
> 
> 
> Default value:* 64




##### CONFIG_LWIP_IP4_FRAG



> Enable fragment outgoing IP4 packets
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows fragmenting outgoing IP4 packets if their size
> exceeds MTU.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_IP6_FRAG



> Enable fragment outgoing IP6 packets
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows fragmenting outgoing IP6 packets if their size
> exceeds MTU.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_IP4_REASSEMBLY



> Enable reassembly incoming fragmented IP4 packets
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows reassemblying incoming fragmented IP4 packets.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_IP6_REASSEMBLY



> Enable reassembly incoming fragmented IP6 packets
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows reassemblying incoming fragmented IP6 packets.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_IP_REASS_MAX_PBUFS



> The maximum amount of pbufs waiting to be reassembled
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Set the maximum amount of pbufs waiting to be reassembled.
> 
> 
> 
> Range:* from 10 to 100
> 
> 
> 
> Default value:* 10




##### CONFIG_LWIP_IP_FORWARD



> Enable IP forwarding
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows packets forwarding across multiple interfaces.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_LWIP_IPV4_NAPT



> Enable NAT
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_IP_FORWARD
> 
> 
> Enabling this option allows Network Address and Port Translation.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_IP_FORWARD




###### CONFIG_LWIP_IPV4_NAPT_PORTMAP



> Enable NAT Port Mapping
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_IP_FORWARD > CONFIG_LWIP_IPV4_NAPT
> 
> 
> Enabling this option allows Port Forwarding or Port mapping.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_LWIP_IPV4_NAPT





##### CONFIG_LWIP_STATS



> Enable LWIP statistics
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows LWIP statistics
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_ESP_GRATUITOUS_ARP



> Send gratuitous ARP periodically
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable this option allows to send gratuitous ARP periodically.
> 
> 
> This option solve the compatibility issues.If the ARP table of the AP is old, and the AP
> doesn't send ARP request to update it's ARP table, this will lead to the STA sending IP packet fail.
> Thus we send gratuitous ARP periodically to let AP update it's ARP table.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_LWIP_GARP_TMR_INTERVAL



> GARP timer interval(seconds)
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_ESP_GRATUITOUS_ARP
> 
> 
> Set the timer interval for gratuitous ARP. The default value is 60s
> 
> 
> 
> Default value:* 60





##### CONFIG_LWIP_ESP_MLDV6_REPORT



> Send mldv6 report periodically
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable this option allows to send mldv6 report periodically.
> 
> 
> This option solve the issue that failed to receive multicast data.
> Some routers fail to forward multicast packets.
> To solve this problem, send multicast mdlv6 report to routers regularly.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_LWIP_MLDV6_TMR_INTERVAL



> mldv6 report timer interval(seconds)
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_ESP_MLDV6_REPORT
> 
> 
> Set the timer interval for mldv6 report. The default value is 30s
> 
> 
> 
> Default value:* 40





##### CONFIG_LWIP_TCPIP_RECVMBOX_SIZE



> TCPIP task receive mail box size
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Set TCPIP task receive mail box size. Generally bigger value means higher throughput
> but more memory. The value should be bigger than UDP/TCP mail box size.
> 
> 
> 
> Range:* from 6 to 1024 if CONFIG_LWIP_WND_SCALE
> 
> 
> 
> Default value:* 32




##### CONFIG_LWIP_DHCP_DOES_ARP_CHECK



> DHCP: Perform ARP check on any offered address
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option performs a check (via ARP request) if the offered IP address
> is not already in use by another host on the network.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_DHCP_DISABLE_CLIENT_ID



> DHCP: Disable Use of HW address as client identification
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> This option could be used to disable DHCP client identification with its MAC address.
> (Client id is used by DHCP servers to uniquely identify clients and are included
> in the DHCP packets as an option 61)
> Set this option to "y" in order to exclude option 61 from DHCP packets.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_DHCP_DISABLE_VENDOR_CLASS_ID



> DHCP: Disable Use of vendor class identification
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> This option could be used to disable DHCP client vendor class identification.
> Set this option to "y" in order to exclude option 60 from DHCP packets.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_DHCP_RESTORE_LAST_IP



> DHCP: Restore last IP obtained from DHCP server
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> When this option is enabled, DHCP client tries to re-obtain last valid IP address obtained from DHCP
> server. Last valid DHCP configuration is stored in nvs and restored after reset/power-up. If IP is still
> available, there is no need for sending discovery message to DHCP server and save some time.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_DHCP_OPTIONS_LEN



> DHCP total option length
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Set total length of outgoing DHCP option msg. Generally bigger value means it can carry more
> options and values. If your code meets LWIP_ASSERT due to option value is too long.
> Please increase the LWIP_DHCP_OPTIONS_LEN value.
> 
> 
> 
> Range:* from 68 to 255
> 
> 
> 
> Default value:* 68




##### CONFIG_LWIP_NUM_NETIF_CLIENT_DATA



> Number of clients store data in netif
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Number of clients that may store data in client_data member array of struct netif.
> 
> 
> 
> Range:* from 0 to 256
> 
> 
> 
> Default value:* 0




##### CONFIG_LWIP_DHCP_COARSE_TIMER_SECS



> DHCP coarse timer interval(s)
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Set DHCP coarse interval in seconds.
> A higher value will be less precise but cost less power consumption.
> 
> 
> 
> Range:* from 1 to 10
> 
> 
> 
> Default value:* 1




##### DHCP server


Contains:


* CONFIG_LWIP_DHCPS



###### CONFIG_LWIP_DHCPS



> DHCPS: Enable IPv4 Dynamic Host Configuration Protocol Server (DHCPS)
> 
> 
> *Found in:* Component config > LWIP > DHCP server
> 
> 
> Enabling this option allows the device to run the DHCP server
> (to dynamically assign IPv4 addresses to clients).
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_LWIP_DHCPS_LEASE_UNIT



> Multiplier for lease time, in seconds
> 
> 
> *Found in:* Component config > LWIP > DHCP server > CONFIG_LWIP_DHCPS
> 
> 
> The DHCP server is calculating lease time multiplying the sent
> and received times by this number of seconds per unit.
> The default is 60, that equals one minute.
> 
> 
> 
> Range:* from 1 to 3600
> 
> 
> 
> Default value:* 60




###### CONFIG_LWIP_DHCPS_MAX_STATION_NUM



> Maximum number of stations
> 
> 
> *Found in:* Component config > LWIP > DHCP server > CONFIG_LWIP_DHCPS
> 
> 
> The maximum number of DHCP clients that are connected to the server.
> After this number is exceeded, DHCP server removes of the oldest device
> from it's address pool, without notification.
> 
> 
> 
> Range:* from 1 to 64
> 
> 
> 
> Default value:* 8




###### CONFIG_LWIP_DHCPS_STATIC_ENTRIES



> Enable ARP static entries
> 
> 
> *Found in:* Component config > LWIP > DHCP server > CONFIG_LWIP_DHCPS
> 
> 
> Enabling this option allows DHCP server to support temporary static ARP entries
> for DHCP Client. This will help the DHCP server to send the DHCP OFFER and DHCP ACK using IP unicast.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_LWIP_AUTOIP



> Enable IPV4 Link-Local Addressing (AUTOIP)
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows the device to self-assign an address
> in the 169.256/16 range if none is assigned statically or via DHCP.
> 
> 
> See RFC 3927.
> 
> 
> 
> Default value:* No (disabled)


Contains:


* CONFIG_LWIP_AUTOIP_TRIES
* CONFIG_LWIP_AUTOIP_MAX_CONFLICTS
* CONFIG_LWIP_AUTOIP_RATE_LIMIT_INTERVAL



###### CONFIG_LWIP_AUTOIP_TRIES



> DHCP Probes before self-assigning IPv4 LL address
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_AUTOIP
> 
> 
> DHCP client will send this many probes before self-assigning a
> link local address.
> 
> 
> From LWIP help: "This can be set as low as 1 to get an AutoIP
> address very quickly, but you should be prepared to handle a
> changing IP address when DHCP overrides AutoIP." (In the case of
> ESP-IDF, this means multiple SYSTEM_EVENT_STA_GOT_IP events.)
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_LWIP_AUTOIP
> 
> 
> 
> Default value:* 2 if CONFIG_LWIP_AUTOIP




###### CONFIG_LWIP_AUTOIP_MAX_CONFLICTS



> Max IP conflicts before rate limiting
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_AUTOIP
> 
> 
> If the AUTOIP functionality detects this many IP conflicts while
> self-assigning an address, it will go into a rate limited mode.
> 
> 
> 
> Range:* from 1 to 100 if CONFIG_LWIP_AUTOIP
> 
> 
> 
> Default value:* 9 if CONFIG_LWIP_AUTOIP




###### CONFIG_LWIP_AUTOIP_RATE_LIMIT_INTERVAL



> Rate limited interval (seconds)
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_AUTOIP
> 
> 
> If rate limiting self-assignment requests, wait this long between
> each request.
> 
> 
> 
> Range:* from 5 to 120 if CONFIG_LWIP_AUTOIP
> 
> 
> 
> Default value:* 20 if CONFIG_LWIP_AUTOIP





##### CONFIG_LWIP_IPV4



> Enable IPv4
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable IPv4 stack. If you want to use IPv6 only TCP/IP stack, disable this.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_LWIP_IPV6



> Enable IPv6
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable IPv6 function. If not use IPv6 function, set this option to n.
> If disabling LWIP_IPV6 then some other components (asio) will
> no longer be available.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_LWIP_IPV6_AUTOCONFIG



> Enable IPV6 stateless address autoconfiguration (SLAAC)
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_IPV6
> 
> 
> Enabling this option allows the devices to IPV6 stateless address autoconfiguration (SLAAC).
> 
> 
> See RFC 4862.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LWIP_IPV6_NUM_ADDRESSES



> Number of IPv6 addresses on each network interface
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_IPV6
> 
> 
> The maximum number of IPv6 addresses on each interface. Any additional
> addresses will be discarded.
> 
> 
> 
> Default value:* 3




###### CONFIG_LWIP_IPV6_FORWARD



> Enable IPv6 forwarding between interfaces
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_IPV6
> 
> 
> Forwarding IPv6 packets between interfaces is only required when acting as
> a router.
> 
> 
> 
> Default value:* No (disabled)





##### CONFIG_LWIP_IPV6_RDNSS_MAX_DNS_SERVERS



> Use IPv6 Router Advertisement Recursive DNS Server Option
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Use IPv6 Router Advertisement Recursive DNS Server Option (as per RFC 6106) to
> copy a defined maximum number of DNS servers to the DNS module.
> Set this option to a number of desired DNS servers advertised in the RA protocol.
> This feature is disabled when set to 0.
> 
> 
> 
> Default value:* 0 if CONFIG_LWIP_IPV6_AUTOCONFIG




##### CONFIG_LWIP_IPV6_DHCP6



> Enable DHCPv6 stateless address autoconfiguration
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable DHCPv6 for IPv6 stateless address autoconfiguration.
> Note that the dhcpv6 client has to be started using dhcp6_enable_stateless(netif);
> Note that the stateful address autoconfiguration is not supported.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_IPV6_AUTOCONFIG




##### CONFIG_LWIP_NETIF_STATUS_CALLBACK



> Enable status callback for network interfaces
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable callbacks when the network interface is up/down and addresses are changed.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_LWIP_NETIF_LOOPBACK



> Support per-interface loopback
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option means that if a packet is sent with a destination
> address equal to the interface's own IP address, it will "loop back" and
> be received by this interface.
> Disabling this option disables support of loopback interface in lwIP
> 
> 
> 
> Default value:* Yes (enabled)


Contains:


* CONFIG_LWIP_LOOPBACK_MAX_PBUFS



###### CONFIG_LWIP_LOOPBACK_MAX_PBUFS



> Max queued loopback packets per interface
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_NETIF_LOOPBACK
> 
> 
> Configure the maximum number of packets which can be queued for
> loopback on a given interface. Reducing this number may cause packets
> to be dropped, but will avoid filling memory with queued packet data.
> 
> 
> 
> Range:* from 0 to 16
> 
> 
> 
> Default value:* 8





##### TCP


Contains:


* CONFIG_LWIP_TCP_WND_DEFAULT
* CONFIG_LWIP_TCP_SND_BUF_DEFAULT
* CONFIG_LWIP_TCP_ACCEPTMBOX_SIZE
* CONFIG_LWIP_TCP_RECVMBOX_SIZE
* CONFIG_LWIP_TCP_RTO_TIME
* CONFIG_LWIP_MAX_ACTIVE_TCP
* CONFIG_LWIP_TCP_FIN_WAIT_TIMEOUT
* CONFIG_LWIP_MAX_LISTENING_TCP
* CONFIG_LWIP_TCP_MAXRTX
* CONFIG_LWIP_TCP_SYNMAXRTX
* CONFIG_LWIP_TCP_MSL
* CONFIG_LWIP_TCP_MSS
* CONFIG_LWIP_TCP_OVERSIZE
* CONFIG_LWIP_TCP_QUEUE_OOSEQ
* CONFIG_LWIP_WND_SCALE
* CONFIG_LWIP_TCP_HIGH_SPEED_RETRANSMISSION
* CONFIG_LWIP_TCP_TMR_INTERVAL



###### CONFIG_LWIP_MAX_ACTIVE_TCP



> Maximum active TCP Connections
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> The maximum number of simultaneously active TCP
> connections. The practical maximum limit is
> determined by available heap memory at runtime.
> 
> 
> Changing this value by itself does not substantially
> change the memory usage of LWIP, except for preventing
> new TCP connections after the limit is reached.
> 
> 
> 
> Range:* from 1 to 1024
> 
> 
> 
> Default value:* 16




###### CONFIG_LWIP_MAX_LISTENING_TCP



> Maximum listening TCP Connections
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> The maximum number of simultaneously listening TCP
> connections. The practical maximum limit is
> determined by available heap memory at runtime.
> 
> 
> Changing this value by itself does not substantially
> change the memory usage of LWIP, except for preventing
> new listening TCP connections after the limit is reached.
> 
> 
> 
> Range:* from 1 to 1024
> 
> 
> 
> Default value:* 16




###### CONFIG_LWIP_TCP_HIGH_SPEED_RETRANSMISSION



> TCP high speed retransmissions
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Speed up the TCP retransmission interval. If disabled,
> it is recommended to change the number of SYN retransmissions to 6,
> and TCP initial rto time to 3000.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_LWIP_TCP_MAXRTX



> Maximum number of retransmissions of data segments
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set maximum number of retransmissions of data segments.
> 
> 
> 
> Range:* from 3 to 12
> 
> 
> 
> Default value:* 12




###### CONFIG_LWIP_TCP_SYNMAXRTX



> Maximum number of retransmissions of SYN segments
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set maximum number of retransmissions of SYN segments.
> 
> 
> 
> Range:* from 3 to 12
> 
> 
> 
> Default value:* 12




###### CONFIG_LWIP_TCP_MSS



> Maximum Segment Size (MSS)
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set maximum segment size for TCP transmission.
> 
> 
> Can be set lower to save RAM, the default value 1460(ipv4)/1440(ipv6) will give best throughput.
> IPv4 TCP_MSS Range: 576 <= TCP_MSS <= 1460
> IPv6 TCP_MSS Range: 1220<= TCP_MSS <= 1440
> 
> 
> 
> Range:* from 536 to 1460
> 
> 
> 
> Default value:* 1440




###### CONFIG_LWIP_TCP_TMR_INTERVAL



> TCP timer interval(ms)
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set TCP timer interval in milliseconds.
> 
> 
> Can be used to speed connections on bad networks.
> A lower value will redeliver unacked packets faster.
> 
> 
> 
> Default value:* 250




###### CONFIG_LWIP_TCP_MSL



> Maximum segment lifetime (MSL)
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set maximum segment lifetime in milliseconds.
> 
> 
> 
> Default value:* 60000




###### CONFIG_LWIP_TCP_FIN_WAIT_TIMEOUT



> Maximum FIN segment lifetime
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set maximum segment lifetime in milliseconds.
> 
> 
> 
> Default value:* 20000




###### CONFIG_LWIP_TCP_SND_BUF_DEFAULT



> Default send buffer size
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set default send buffer size for new TCP sockets.
> 
> 
> Per-socket send buffer size can be changed at runtime
> with lwip_setsockopt(s, TCP_SNDBUF, ...).
> 
> 
> This value must be at least 2x the MSS size, and the default
> is 4x the default MSS size.
> 
> 
> Setting a smaller default SNDBUF size can save some RAM, but
> will decrease performance.
> 
> 
> 
> Range:* from 2440 to 1024000 if CONFIG_LWIP_WND_SCALE
> 
> 
> 
> Default value:* 5760




###### CONFIG_LWIP_TCP_WND_DEFAULT



> Default receive window size
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set default TCP receive window size for new TCP sockets.
> 
> 
> Per-socket receive window size can be changed at runtime
> with lwip_setsockopt(s, TCP_WINDOW, ...).
> 
> 
> Setting a smaller default receive window size can save some RAM,
> but will significantly decrease performance.
> 
> 
> 
> Range:* from 2440 to 1024000 if CONFIG_LWIP_WND_SCALE
> 
> 
> 
> Default value:* 5760




###### CONFIG_LWIP_TCP_RECVMBOX_SIZE



> Default TCP receive mail box size
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set TCP receive mail box size. Generally bigger value means higher throughput
> but more memory. The recommended value is: LWIP_TCP_WND_DEFAULT/TCP_MSS + 2, e.g. if
> LWIP_TCP_WND_DEFAULT=14360, TCP_MSS=1436, then the recommended receive mail box size is
> (14360/1436 + 2) = 12.
> 
> 
> TCP receive mail box is a per socket mail box, when the application receives packets
> from TCP socket, LWIP core firstly posts the packets to TCP receive mail box and the
> application then fetches the packets from mail box. It means LWIP can caches maximum
> LWIP_TCP_RECCVMBOX_SIZE packets for each TCP socket, so the maximum possible cached TCP packets
> for all TCP sockets is LWIP_TCP_RECCVMBOX_SIZE multiples the maximum TCP socket number. In other
> words, the bigger LWIP_TCP_RECVMBOX_SIZE means more memory.
> On the other hand, if the receiv mail box is too small, the mail box may be full. If the
> mail box is full, the LWIP drops the packets. So generally we need to make sure the TCP
> receive mail box is big enough to avoid packet drop between LWIP core and application.
> 
> 
> 
> Range:* from 6 to 1024 if CONFIG_LWIP_WND_SCALE
> 
> 
> 
> Default value:* 6




###### CONFIG_LWIP_TCP_ACCEPTMBOX_SIZE



> Default TCP accept mail box size
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set TCP accept mail box size. Generally bigger value means supporting larger backlogs
> but more memory. The recommended value is 6, but applications can set it to a lower value
> if listening servers are meant to have a smaller backlog.
> 
> 
> TCP accept mail box is a per socket mail box, when the application listens for connections
> with a given listening TCP socket. If the mailbox is full, LWIP will send a RST packet and
> the client will fail to connect.
> 
> 
> 
> Range:* from 1 to 255 if CONFIG_LWIP_WND_SCALE
> 
> 
> 
> Default value:* 6




###### CONFIG_LWIP_TCP_QUEUE_OOSEQ



> Queue incoming out-of-order segments
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Queue incoming out-of-order segments for later use.
> 
> 
> Disable this option to save some RAM during TCP sessions, at the expense
> of increased retransmissions if segments arrive out of order.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_LWIP_TCP_OOSEQ_TIMEOUT



> Timeout for each pbuf queued in TCP OOSEQ, in RTOs.
> 
> 
> *Found in:* Component config > LWIP > TCP > CONFIG_LWIP_TCP_QUEUE_OOSEQ
> 
> 
> The timeout value is TCP_OOSEQ_TIMEOUT \* RTO.
> 
> 
> 
> Range:* from 1 to 30
> 
> 
> 
> Default value:* 6




###### CONFIG_LWIP_TCP_OOSEQ_MAX_PBUFS



> The maximum number of pbufs queued on OOSEQ per pcb
> 
> 
> *Found in:* Component config > LWIP > TCP > CONFIG_LWIP_TCP_QUEUE_OOSEQ
> 
> 
> If LWIP_TCP_OOSEQ_MAX_PBUFS = 0, TCP will not control the number of OOSEQ pbufs.
> 
> 
> In a poor network environment, many out-of-order tcp pbufs will be received.
> These out-of-order pbufs will be cached in the TCP out-of-order queue which will
> cause Wi-Fi/Ethernet fail to release RX buffer in time.
> It is possible that all RX buffers for MAC layer are used by OOSEQ.
> 
> 
> Control the number of out-of-order pbufs to ensure
> that the MAC layer has enough RX buffer to receive packets.
> 
> 
> In the Wi-Fi scenario, recommended OOSEQ PBUFS Range:
> 0 <= TCP_OOSEQ_MAX_PBUFS <= CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM/(MAX_TCP_NUMBER + 1)
> 
> 
> In the Ethernet scenario,recommended Ethernet OOSEQ PBUFS Range:
> 0 <= TCP_OOSEQ_MAX_PBUFS <= CONFIG_ETH_DMA_RX_BUFFER_NUM/(MAX_TCP_NUMBER + 1)
> 
> 
> Within the recommended value range, the larger the value, the better the performance.
> 
> 
> MAX_TCP_NUMBER represent Maximum number of TCP connections in Wi-Fi(STA+SoftAP) and Ethernet scenario.
> 
> 
> 
> Range:* from 0 to 12
> 
> 
> 
> Default value:* 0 if CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP && CONFIG_LWIP_TCP_QUEUE_OOSEQ




###### CONFIG_LWIP_TCP_SACK_OUT



> Support sending selective acknowledgements
> 
> 
> *Found in:* Component config > LWIP > TCP > CONFIG_LWIP_TCP_QUEUE_OOSEQ
> 
> 
> TCP will support sending selective acknowledgements (SACKs).
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LWIP_TCP_OVERSIZE



> Pre-allocate transmit PBUF size
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Allows enabling "oversize" allocation of TCP transmission pbufs ahead of time,
> which can reduce the length of pbuf chains used for transmission.
> 
> 
> This will not make a difference to sockets where Nagle's algorithm
> is disabled.
> 
> 
> Default value of MSS is fine for most applications, 25% MSS may save
> some RAM when only transmitting small amounts of data. Disabled will
> have worst performance and fragmentation characteristics, but uses
> least RAM overall.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * MSS (CONFIG_LWIP_TCP_OVERSIZE_MSS)
> > * 25% MSS (CONFIG_LWIP_TCP_OVERSIZE_QUARTER_MSS)
> > * Disabled (CONFIG_LWIP_TCP_OVERSIZE_DISABLE)




###### CONFIG_LWIP_WND_SCALE



> Support TCP window scale
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Enable this feature to support TCP window scaling.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP




###### CONFIG_LWIP_TCP_RCV_SCALE



> Set TCP receiving window scaling factor
> 
> 
> *Found in:* Component config > LWIP > TCP > CONFIG_LWIP_WND_SCALE
> 
> 
> Enable this feature to support TCP window scaling.
> 
> 
> 
> Range:* from 0 to 14 if CONFIG_LWIP_WND_SCALE
> 
> 
> 
> Default value:* 0 if CONFIG_LWIP_WND_SCALE




###### CONFIG_LWIP_TCP_RTO_TIME



> Default TCP rto time
> 
> 
> *Found in:* Component config > LWIP > TCP
> 
> 
> Set default TCP rto time for a reasonable initial rto.
> In bad network environment, recommend set value of rto time to 1500.
> 
> 
> 
> Default value:* 1500





##### UDP


Contains:


* CONFIG_LWIP_UDP_RECVMBOX_SIZE
* CONFIG_LWIP_MAX_UDP_PCBS



###### CONFIG_LWIP_MAX_UDP_PCBS



> Maximum active UDP control blocks
> 
> 
> *Found in:* Component config > LWIP > UDP
> 
> 
> The maximum number of active UDP "connections" (ie
> UDP sockets sending/receiving data).
> The practical maximum limit is determined by available
> heap memory at runtime.
> 
> 
> 
> Range:* from 1 to 1024
> 
> 
> 
> Default value:* 16




###### CONFIG_LWIP_UDP_RECVMBOX_SIZE



> Default UDP receive mail box size
> 
> 
> *Found in:* Component config > LWIP > UDP
> 
> 
> Set UDP receive mail box size. The recommended value is 6.
> 
> 
> UDP receive mail box is a per socket mail box, when the application receives packets
> from UDP socket, LWIP core firstly posts the packets to UDP receive mail box and the
> application then fetches the packets from mail box. It means LWIP can caches maximum
> UDP_RECCVMBOX_SIZE packets for each UDP socket, so the maximum possible cached UDP packets
> for all UDP sockets is UDP_RECCVMBOX_SIZE multiples the maximum UDP socket number. In other
> words, the bigger UDP_RECVMBOX_SIZE means more memory.
> On the other hand, if the receiv mail box is too small, the mail box may be full. If the
> mail box is full, the LWIP drops the packets. So generally we need to make sure the UDP
> receive mail box is big enough to avoid packet drop between LWIP core and application.
> 
> 
> 
> Range:* from 6 to 64
> 
> 
> 
> Default value:* 6





##### Checksums


Contains:


* CONFIG_LWIP_CHECKSUM_CHECK_ICMP
* CONFIG_LWIP_CHECKSUM_CHECK_IP
* CONFIG_LWIP_CHECKSUM_CHECK_UDP



###### CONFIG_LWIP_CHECKSUM_CHECK_IP



> Enable LWIP IP checksums
> 
> 
> *Found in:* Component config > LWIP > Checksums
> 
> 
> Enable checksum checking for received IP messages
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LWIP_CHECKSUM_CHECK_UDP



> Enable LWIP UDP checksums
> 
> 
> *Found in:* Component config > LWIP > Checksums
> 
> 
> Enable checksum checking for received UDP messages
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LWIP_CHECKSUM_CHECK_ICMP



> Enable LWIP ICMP checksums
> 
> 
> *Found in:* Component config > LWIP > Checksums
> 
> 
> Enable checksum checking for received ICMP messages
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_LWIP_TCPIP_TASK_STACK_SIZE



> TCP/IP Task Stack Size
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Configure TCP/IP task stack size, used by LWIP to process multi-threaded TCP/IP operations.
> Setting this stack too small will result in stack overflow crashes.
> 
> 
> 
> Range:* from 2048 to 65536
> 
> 
> 
> Default value:* 3072




##### CONFIG_LWIP_TCPIP_TASK_AFFINITY



> TCP/IP task affinity
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Allows setting LwIP tasks affinity, i.e. whether the task is pinned to
> CPU0, pinned to CPU1, or allowed to run on any CPU.
> Currently this applies to "TCP/IP" task and "Ping" task.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No affinity (CONFIG_LWIP_TCPIP_TASK_AFFINITY_NO_AFFINITY)
> > * CPU0 (CONFIG_LWIP_TCPIP_TASK_AFFINITY_CPU0)
> > * CPU1 (CONFIG_LWIP_TCPIP_TASK_AFFINITY_CPU1)




##### CONFIG_LWIP_PPP_SUPPORT



> Enable PPP support
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable PPP stack. Now only PPP over serial is possible.
> 
> 
> 
> Default value:* No (disabled)


Contains:


* CONFIG_LWIP_PPP_ENABLE_IPV6



###### CONFIG_LWIP_PPP_ENABLE_IPV6



> Enable IPV6 support for PPP connections (IPV6CP)
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_PPP_SUPPORT
> 
> 
> Enable IPV6 support in PPP for the local link between the DTE (processor) and DCE (modem).
> There are some modems which do not support the IPV6 addressing in the local link.
> If they are requested for IPV6CP negotiation, they may time out.
> This would in turn fail the configuration for the whole link.
> If your modem is not responding correctly to PPP Phase Network, try to disable IPV6 support.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_LWIP_PPP_SUPPORT && CONFIG_LWIP_IPV6





##### CONFIG_LWIP_IPV6_MEMP_NUM_ND6_QUEUE



> Max number of IPv6 packets to queue during MAC resolution
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Config max number of IPv6 packets to queue during MAC resolution.
> 
> 
> 
> Range:* from 3 to 20
> 
> 
> 
> Default value:* 3




##### CONFIG_LWIP_IPV6_ND6_NUM_NEIGHBORS



> Max number of entries in IPv6 neighbor cache
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Config max number of entries in IPv6 neighbor cache
> 
> 
> 
> Range:* from 3 to 10
> 
> 
> 
> Default value:* 5




##### CONFIG_LWIP_PPP_NOTIFY_PHASE_SUPPORT



> Enable Notify Phase Callback
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable to set a callback which is called on change of the internal PPP state machine.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_PPP_PAP_SUPPORT



> Enable PAP support
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable Password Authentication Protocol (PAP) support
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_PPP_CHAP_SUPPORT



> Enable CHAP support
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable Challenge Handshake Authentication Protocol (CHAP) support
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_PPP_MSCHAP_SUPPORT



> Enable MSCHAP support
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable Microsoft version of the Challenge-Handshake Authentication Protocol (MSCHAP) support
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_PPP_MPPE_SUPPORT



> Enable MPPE support
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable Microsoft Point-to-Point Encryption (MPPE) support
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_PPP_SERVER_SUPPORT



> Enable PPP server support
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable to use PPP server
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_PPP_VJ_HEADER_COMPRESSION



> Enable VJ IP Header compression
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable support for VJ header compression.
> Please disable this if you're using NAPT on PPP interface,
> since the compressed IP header might not be correctly interpreted
> in NAT causing the compressed packet to be dropped.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_ENABLE_LCP_ECHO



> Enable LCP ECHO
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable LCP echo keepalive requests
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT



###### CONFIG_LWIP_LCP_ECHOINTERVAL



> Echo interval (s)
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_ENABLE_LCP_ECHO
> 
> 
> Interval in seconds between keepalive LCP echo requests, 0 to disable.
> 
> 
> 
> Range:* from 0 to 1000000 if CONFIG_LWIP_ENABLE_LCP_ECHO
> 
> 
> 
> Default value:* 3 if CONFIG_LWIP_ENABLE_LCP_ECHO




###### CONFIG_LWIP_LCP_MAXECHOFAILS



> Maximum echo failures
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_ENABLE_LCP_ECHO
> 
> 
> Number of consecutive unanswered echo requests before failure is indicated.
> 
> 
> 
> Range:* from 0 to 100000 if CONFIG_LWIP_ENABLE_LCP_ECHO
> 
> 
> 
> Default value:* 3 if CONFIG_LWIP_ENABLE_LCP_ECHO





##### CONFIG_LWIP_PPP_DEBUG_ON



> Enable PPP debug log output
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable PPP debug log output
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_PPP_SUPPORT




##### CONFIG_LWIP_SLIP_SUPPORT



> Enable SLIP support (new/experimental)
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable SLIP stack. Now only SLIP over serial is possible.
> 
> 
> SLIP over serial support is experimental and unsupported.
> 
> 
> 
> Default value:* No (disabled)


Contains:


* CONFIG_LWIP_SLIP_DEBUG_ON



###### CONFIG_LWIP_SLIP_DEBUG_ON



> Enable SLIP debug log output
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_SLIP_SUPPORT
> 
> 
> Enable SLIP debug log output
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_SLIP_SUPPORT





##### ICMP


Contains:


* CONFIG_LWIP_ICMP
* CONFIG_LWIP_BROADCAST_PING
* CONFIG_LWIP_MULTICAST_PING



###### CONFIG_LWIP_ICMP



> ICMP: Enable ICMP
> 
> 
> *Found in:* Component config > LWIP > ICMP
> 
> 
> Enable ICMP module for check network stability
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_LWIP_MULTICAST_PING



> Respond to multicast pings
> 
> 
> *Found in:* Component config > LWIP > ICMP
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LWIP_BROADCAST_PING



> Respond to broadcast pings
> 
> 
> *Found in:* Component config > LWIP > ICMP
> 
> 
> 
> Default value:* No (disabled)





##### LWIP RAW API


Contains:


* CONFIG_LWIP_MAX_RAW_PCBS



###### CONFIG_LWIP_MAX_RAW_PCBS



> Maximum LWIP RAW PCBs
> 
> 
> *Found in:* Component config > LWIP > LWIP RAW API
> 
> 
> The maximum number of simultaneously active LWIP
> RAW protocol control blocks. The practical maximum
> limit is determined by available heap memory at runtime.
> 
> 
> 
> Range:* from 1 to 1024
> 
> 
> 
> Default value:* 16





##### SNTP


Contains:


* CONFIG_LWIP_SNTP_STARTUP_DELAY
* CONFIG_LWIP_SNTP_MAX_SERVERS
* CONFIG_LWIP_SNTP_UPDATE_DELAY
* CONFIG_LWIP_DHCP_GET_NTP_SRV



###### CONFIG_LWIP_SNTP_MAX_SERVERS



> Maximum number of NTP servers
> 
> 
> *Found in:* Component config > LWIP > SNTP
> 
> 
> Set maximum number of NTP servers used by LwIP SNTP module.
> First argument of sntp_setserver/sntp_setservername functions
> is limited to this value.
> 
> 
> 
> Range:* from 1 to 16
> 
> 
> 
> Default value:* 1




###### CONFIG_LWIP_DHCP_GET_NTP_SRV



> Request NTP servers from DHCP
> 
> 
> *Found in:* Component config > LWIP > SNTP
> 
> 
> If enabled, LWIP will add 'NTP' to Parameter-Request Option sent via DHCP-request.
> DHCP server might reply with an NTP server address in option 42.
> SNTP callback for such replies should be set accordingly (see sntp_servermode_dhcp() func.)
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LWIP_DHCP_MAX_NTP_SERVERS



> Maximum number of NTP servers aquired via DHCP
> 
> 
> *Found in:* Component config > LWIP > SNTP > CONFIG_LWIP_DHCP_GET_NTP_SRV
> 
> 
> Set maximum number of NTP servers aquired via DHCP-offer.
> Should be less or equal to "Maximum number of NTP servers", any extra servers would be just ignored.
> 
> 
> 
> Range:* from 1 to 16 if CONFIG_LWIP_DHCP_GET_NTP_SRV
> 
> 
> 
> Default value:* 1 if CONFIG_LWIP_DHCP_GET_NTP_SRV




###### CONFIG_LWIP_SNTP_UPDATE_DELAY



> Request interval to update time (ms)
> 
> 
> *Found in:* Component config > LWIP > SNTP
> 
> 
> This option allows you to set the time update period via SNTP.
> Default is 1 hour. Must not be below 15 seconds by specification.
> (SNTPv4 RFC 4330 enforces a minimum update time of 15 seconds).
> 
> 
> 
> Range:* from 15000 to 4294967295
> 
> 
> 
> Default value:* 3600000




###### CONFIG_LWIP_SNTP_STARTUP_DELAY



> Enable SNTP startup delay
> 
> 
> *Found in:* Component config > LWIP > SNTP
> 
> 
> It is recommended (RFC 4330) to delay the initial request after by a random timeout from 1 to 5 minutes
> to reduce potential load of NTP servers after simultaneous power-up of many devices.
> This option disables this initial delay. Please use this option with care, it could improve
> a single device responsiveness but might cause peaks on the network after reset.
> Another option to address responsiveness of devices while using the initial random delay
> is to adjust LWIP_SNTP_MAXIMUM_STARTUP_DELAY.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_LWIP_SNTP_MAXIMUM_STARTUP_DELAY



> Maximum startup delay (ms)
> 
> 
> *Found in:* Component config > LWIP > SNTP > CONFIG_LWIP_SNTP_STARTUP_DELAY
> 
> 
> RFC 4330 recommends a startup delay before sending the initial request.
> LWIP calculates this delay to a random number of milliseconds between 0 and this value.
> 
> 
> 
> Range:* from 100 to 300000
> 
> 
> 
> Default value:* 5000





##### DNS


Contains:


* CONFIG_LWIP_FALLBACK_DNS_SERVER_SUPPORT
* CONFIG_LWIP_DNS_MAX_SERVERS



###### CONFIG_LWIP_DNS_MAX_SERVERS



> Maximum number of DNS servers
> 
> 
> *Found in:* Component config > LWIP > DNS
> 
> 
> Set maximum number of DNS servers.
> If fallback DNS servers are supported,
> the number of DNS servers needs to be greater than or equal to 3.
> 
> 
> 
> Range:* from 1 to 4
> 
> 
> 
> Default value:* 3




###### CONFIG_LWIP_FALLBACK_DNS_SERVER_SUPPORT



> Enable DNS fallback server support
> 
> 
> *Found in:* Component config > LWIP > DNS
> 
> 
> Enable this feature to support DNS fallback server.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_LWIP_FALLBACK_DNS_SERVER_ADDRESS



> DNS fallback server address
> 
> 
> *Found in:* Component config > LWIP > DNS > CONFIG_LWIP_FALLBACK_DNS_SERVER_SUPPORT
> 
> 
> This option allows you to config dns fallback server address.
> 
> 
> 
> Default value:* "114.114.114.114" if CONFIG_LWIP_FALLBACK_DNS_SERVER_SUPPORT





##### CONFIG_LWIP_BRIDGEIF_MAX_PORTS



> Maximum number of bridge ports
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Set maximum number of ports a bridge can consists of.
> 
> 
> 
> Range:* from 1 to 63
> 
> 
> 
> Default value:* 7




##### CONFIG_LWIP_ESP_LWIP_ASSERT



> Enable LWIP ASSERT checks
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enable this option keeps LWIP assertion checks enabled.
> It is recommended to keep this option enabled.
> 
> 
> If asserts are disabled for the entire project, they are also disabled
> for LWIP and this option is ignored.




##### Hooks


Contains:


* CONFIG_LWIP_HOOK_ND6_GET_GW
* CONFIG_LWIP_HOOK_IP6_INPUT
* CONFIG_LWIP_HOOK_IP6_ROUTE
* CONFIG_LWIP_HOOK_IP6_SELECT_SRC_ADDR
* CONFIG_LWIP_HOOK_NETCONN_EXTERNAL_RESOLVE
* CONFIG_LWIP_HOOK_TCP_ISN



###### CONFIG_LWIP_HOOK_TCP_ISN



> TCP ISN Hook
> 
> 
> *Found in:* Component config > LWIP > Hooks
> 
> 
> Enables to define a TCP ISN hook to randomize initial sequence
> number in TCP connection.
> The default TCP ISN algorithm used in IDF (standardized in RFC 6528)
> produces ISN by combining an MD5 of the new TCP id and a stable
> secret with the current time.
> This is because the lwIP implementation (tcp_next_iss) is not
> very strong, as it does not take into consideration any platform
> specific entropy source.
> 
> 
> Set to LWIP_HOOK_TCP_ISN_CUSTOM to provide custom implementation.
> Set to LWIP_HOOK_TCP_ISN_NONE to use lwIP implementation.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No hook declared (CONFIG_LWIP_HOOK_TCP_ISN_NONE)
> > * Default implementation (CONFIG_LWIP_HOOK_TCP_ISN_DEFAULT)
> > * Custom implementation (CONFIG_LWIP_HOOK_TCP_ISN_CUSTOM)




###### CONFIG_LWIP_HOOK_IP6_ROUTE



> IPv6 route Hook
> 
> 
> *Found in:* Component config > LWIP > Hooks
> 
> 
> Enables custom IPv6 route hook.
> Setting this to "default" provides weak implementation
> stub that could be overwritten in application code.
> Setting this to "custom" provides hook's declaration
> only and expects the application to implement it.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No hook declared (CONFIG_LWIP_HOOK_IP6_ROUTE_NONE)
> > * Default (weak) implementation (CONFIG_LWIP_HOOK_IP6_ROUTE_DEFAULT)
> > * Custom implementation (CONFIG_LWIP_HOOK_IP6_ROUTE_CUSTOM)




###### CONFIG_LWIP_HOOK_ND6_GET_GW



> IPv6 get gateway Hook
> 
> 
> *Found in:* Component config > LWIP > Hooks
> 
> 
> Enables custom IPv6 route hook.
> Setting this to "default" provides weak implementation
> stub that could be overwritten in application code.
> Setting this to "custom" provides hook's declaration
> only and expects the application to implement it.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No hook declared (CONFIG_LWIP_HOOK_ND6_GET_GW_NONE)
> > * Default (weak) implementation (CONFIG_LWIP_HOOK_ND6_GET_GW_DEFAULT)
> > * Custom implementation (CONFIG_LWIP_HOOK_ND6_GET_GW_CUSTOM)




###### CONFIG_LWIP_HOOK_IP6_SELECT_SRC_ADDR



> IPv6 source address selection Hook
> 
> 
> *Found in:* Component config > LWIP > Hooks
> 
> 
> Enables custom IPv6 source address selection.
> Setting this to "default" provides weak implementation
> stub that could be overwritten in application code.
> Setting this to "custom" provides hook's declaration
> only and expects the application to implement it.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No hook declared (CONFIG_LWIP_HOOK_IP6_SELECT_SRC_ADDR_NONE)
> > * Default (weak) implementation (CONFIG_LWIP_HOOK_IP6_SELECT_SRC_ADDR_DEFAULT)
> > * Custom implementation (CONFIG_LWIP_HOOK_IP6_SELECT_SRC_ADDR_CUSTOM)




###### CONFIG_LWIP_HOOK_NETCONN_EXTERNAL_RESOLVE



> Netconn external resolve Hook
> 
> 
> *Found in:* Component config > LWIP > Hooks
> 
> 
> Enables custom DNS resolve hook.
> Setting this to "default" provides weak implementation
> stub that could be overwritten in application code.
> Setting this to "custom" provides hook's declaration
> only and expects the application to implement it.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No hook declared (CONFIG_LWIP_HOOK_NETCONN_EXT_RESOLVE_NONE)
> > * Default (weak) implementation (CONFIG_LWIP_HOOK_NETCONN_EXT_RESOLVE_DEFAULT)
> > * Custom implementation (CONFIG_LWIP_HOOK_NETCONN_EXT_RESOLVE_CUSTOM)




###### CONFIG_LWIP_HOOK_IP6_INPUT



> IPv6 packet input
> 
> 
> *Found in:* Component config > LWIP > Hooks
> 
> 
> Enables custom IPv6 packet input.
> Setting this to "default" provides weak implementation
> stub that could be overwritten in application code.
> Setting this to "custom" provides hook's declaration
> only and expects the application to implement it.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No hook declared (CONFIG_LWIP_HOOK_IP6_INPUT_NONE)
> > * Default (weak) implementation (CONFIG_LWIP_HOOK_IP6_INPUT_DEFAULT)
> > * Custom implementation (CONFIG_LWIP_HOOK_IP6_INPUT_CUSTOM)





##### CONFIG_LWIP_DEBUG



> Enable LWIP Debug
> 
> 
> *Found in:* Component config > LWIP
> 
> 
> Enabling this option allows different kinds of lwIP debug output.
> 
> 
> All lwIP debug features increase the size of the final binary.
> 
> 
> 
> Default value:* No (disabled)


Contains:


* CONFIG_LWIP_API_LIB_DEBUG
* CONFIG_LWIP_BRIDGEIF_FDB_DEBUG
* CONFIG_LWIP_BRIDGEIF_FW_DEBUG
* CONFIG_LWIP_BRIDGEIF_DEBUG
* CONFIG_LWIP_DHCP_DEBUG
* CONFIG_LWIP_DHCP_STATE_DEBUG
* CONFIG_LWIP_DNS_DEBUG
* CONFIG_LWIP_ETHARP_DEBUG
* CONFIG_LWIP_ICMP_DEBUG
* CONFIG_LWIP_ICMP6_DEBUG
* CONFIG_LWIP_IP_DEBUG
* CONFIG_LWIP_IP6_DEBUG
* CONFIG_LWIP_NAPT_DEBUG
* CONFIG_LWIP_NETIF_DEBUG
* CONFIG_LWIP_PBUF_DEBUG
* CONFIG_LWIP_SNTP_DEBUG
* CONFIG_LWIP_SOCKETS_DEBUG
* CONFIG_LWIP_TCP_DEBUG
* CONFIG_LWIP_UDP_DEBUG
* CONFIG_LWIP_DEBUG_ESP_LOG



###### CONFIG_LWIP_DEBUG_ESP_LOG



> Route LWIP debugs through ESP_LOG interface
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> Enabling this option routes all enabled LWIP debugs through ESP_LOGD.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_NETIF_DEBUG



> Enable netif debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_PBUF_DEBUG



> Enable pbuf debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_ETHARP_DEBUG



> Enable etharp debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_API_LIB_DEBUG



> Enable api lib debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_SOCKETS_DEBUG



> Enable socket debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_IP_DEBUG



> Enable IP debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_ICMP_DEBUG



> Enable ICMP debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG && CONFIG_LWIP_ICMP




###### CONFIG_LWIP_DHCP_STATE_DEBUG



> Enable DHCP state tracking
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_DHCP_DEBUG



> Enable DHCP debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_IP6_DEBUG



> Enable IP6 debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_ICMP6_DEBUG



> Enable ICMP6 debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_TCP_DEBUG



> Enable TCP debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_UDP_DEBUG



> Enable UDP debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_SNTP_DEBUG



> Enable SNTP debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_DNS_DEBUG



> Enable DNS debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_NAPT_DEBUG



> Enable NAPT debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG && CONFIG_LWIP_IPV4_NAPT




###### CONFIG_LWIP_BRIDGEIF_DEBUG



> Enable bridge generic debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_BRIDGEIF_FDB_DEBUG



> Enable bridge FDB debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG




###### CONFIG_LWIP_BRIDGEIF_FW_DEBUG



> Enable bridge forwarding debug messages
> 
> 
> *Found in:* Component config > LWIP > CONFIG_LWIP_DEBUG
> 
> 
> 
> Default value:* No (disabled) if CONFIG_LWIP_DEBUG






#### mbedTLS


Contains:


* CONFIG_MBEDTLS_ASYMMETRIC_CONTENT_LEN
* Certificate Bundle
* Certificates
* CONFIG_MBEDTLS_CHACHA20_C
* CONFIG_MBEDTLS_DHM_C
* CONFIG_MBEDTLS_ECP_C
* CONFIG_MBEDTLS_ECDH_C
* CONFIG_MBEDTLS_ECJPAKE_C
* CONFIG_MBEDTLS_ECP_DP_BP256R1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_BP384R1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_BP512R1_ENABLED
* CONFIG_MBEDTLS_CMAC_C
* CONFIG_MBEDTLS_ECP_DP_CURVE25519_ENABLED
* CONFIG_MBEDTLS_ECDSA_DETERMINISTIC
* CONFIG_MBEDTLS_HARDWARE_ECDSA_VERIFY
* CONFIG_MBEDTLS_HARDWARE_ECDSA_SIGN
* CONFIG_MBEDTLS_ERROR_STRINGS
* CONFIG_MBEDTLS_ECP_FIXED_POINT_OPTIM
* CONFIG_MBEDTLS_HARDWARE_AES
* CONFIG_MBEDTLS_HARDWARE_ECC
* CONFIG_MBEDTLS_ATCA_HW_ECDSA_SIGN
* CONFIG_MBEDTLS_ATCA_HW_ECDSA_VERIFY
* CONFIG_MBEDTLS_HARDWARE_MPI
* CONFIG_MBEDTLS_HARDWARE_SHA
* CONFIG_MBEDTLS_DEBUG
* CONFIG_MBEDTLS_ECP_RESTARTABLE
* CONFIG_MBEDTLS_HAVE_TIME
* CONFIG_MBEDTLS_RIPEMD160_C
* CONFIG_MBEDTLS_ECP_DP_SECP192K1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_SECP192R1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_SECP224K1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_SECP224R1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_SECP256K1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_SECP256R1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_SECP384R1_ENABLED
* CONFIG_MBEDTLS_ECP_DP_SECP521R1_ENABLED
* CONFIG_MBEDTLS_SHA512_C
* CONFIG_MBEDTLS_THREADING_C
* CONFIG_MBEDTLS_HKDF_C
* mbedTLS v3.x related
* CONFIG_MBEDTLS_MEM_ALLOC_MODE
* CONFIG_MBEDTLS_ECP_NIST_OPTIM
* CONFIG_MBEDTLS_POLY1305_C
* CONFIG_MBEDTLS_SSL_ALPN
* CONFIG_MBEDTLS_SSL_PROTO_DTLS
* CONFIG_MBEDTLS_SSL_PROTO_GMTSSL1_1
* CONFIG_MBEDTLS_SSL_PROTO_TLS1_2
* CONFIG_MBEDTLS_SSL_RENEGOTIATION
* Symmetric Ciphers
* TLS Key Exchange Methods
* CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN
* CONFIG_MBEDTLS_TLS_MODE
* CONFIG_MBEDTLS_CLIENT_SSL_SESSION_TICKETS
* CONFIG_MBEDTLS_SERVER_SSL_SESSION_TICKETS
* CONFIG_MBEDTLS_ROM_MD5
* CONFIG_MBEDTLS_USE_CRYPTO_ROM_IMPL
* CONFIG_MBEDTLS_DYNAMIC_BUFFER



##### CONFIG_MBEDTLS_MEM_ALLOC_MODE



> Memory allocation strategy
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Allocation strategy for mbedTLS, essentially provides ability to
> allocate all required dynamic allocations from,
> 
> 
> * Internal DRAM memory only
> * External SPIRAM memory only
> * Either internal or external memory based on default malloc()
> behavior in ESP-IDF
> * Custom allocation mode, by overwriting calloc()/free() using
> mbedtls_platform_set_calloc_free() function
> * Internal IRAM memory wherever applicable else internal DRAM
> 
> 
> Recommended mode here is always internal (\*), since that is most preferred
> from security perspective. But if application requirement does not
> allow sufficient free internal memory then alternate mode can be
> selected.
> 
> 
> (\*) In case of ESP32-S2/ESP32-S3, hardware allows encryption of external
> SPIRAM contents provided hardware flash encryption feature is enabled.
> In that case, using external SPIRAM allocation strategy is also safe choice
> from security perspective.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Internal memory (CONFIG_MBEDTLS_INTERNAL_MEM_ALLOC)
> > * External SPIRAM (CONFIG_MBEDTLS_EXTERNAL_MEM_ALLOC)
> > * Default alloc mode (CONFIG_MBEDTLS_DEFAULT_MEM_ALLOC)
> > * Custom alloc mode (CONFIG_MBEDTLS_CUSTOM_MEM_ALLOC)
> > * Internal IRAM (CONFIG_MBEDTLS_IRAM_8BIT_MEM_ALLOC)
> > 
> > 
> > Allows to use IRAM memory region as 8bit accessible region.
> > 
> > 
> > TLS input and output buffers will be allocated in IRAM section which is 32bit aligned
> > memory. Every unaligned (8bit or 16bit) access will result in an exception
> > and incur penalty of certain clock cycles per unaligned read/write.




##### CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN



> TLS maximum message content length
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Maximum TLS message length (in bytes) supported by mbedTLS.
> 
> 
> 16384 is the default and this value is required to comply
> fully with TLS standards.
> 
> 
> However you can set a lower value in order to save RAM. This
> is safe if the other end of the connection supports Maximum
> Fragment Length Negotiation Extension (max_fragment_length,
> see RFC6066) or you know for certain that it will never send a
> message longer than a certain number of bytes.
> 
> 
> If the value is set too low, symptoms are a failed TLS
> handshake or a return value of MBEDTLS_ERR_SSL_INVALID_RECORD
> (-0x7200).




##### CONFIG_MBEDTLS_ASYMMETRIC_CONTENT_LEN



> Asymmetric in/out fragment length
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> If enabled, this option allows customizing TLS in/out fragment length
> in asymmetric way. Please note that enabling this with default values
> saves 12KB of dynamic memory per TLS connection.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_MBEDTLS_SSL_IN_CONTENT_LEN



> TLS maximum incoming fragment length
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_ASYMMETRIC_CONTENT_LEN
> 
> 
> This defines maximum incoming fragment length, overriding default
> maximum content length (MBEDTLS_SSL_MAX_CONTENT_LEN).
> 
> 
> 
> Range:* from 512 to 16384
> 
> 
> 
> Default value:* 16384




###### CONFIG_MBEDTLS_SSL_OUT_CONTENT_LEN



> TLS maximum outgoing fragment length
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_ASYMMETRIC_CONTENT_LEN
> 
> 
> This defines maximum outgoing fragment length, overriding default
> maximum content length (MBEDTLS_SSL_MAX_CONTENT_LEN).
> 
> 
> 
> Range:* from 512 to 16384
> 
> 
> 
> Default value:* 4096





##### CONFIG_MBEDTLS_DYNAMIC_BUFFER



> Using dynamic TX/RX buffer
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Using dynamic TX/RX buffer. After enabling this option, mbedTLS will
> allocate TX buffer when need to send data and then free it if all data
> is sent, allocate RX buffer when need to receive data and then free it
> when all data is used or read by upper layer.
> 
> 
> By default, when SSL is initialized, mbedTLS also allocate TX and
> RX buffer with the default value of "MBEDTLS_SSL_OUT_CONTENT_LEN" or
> "MBEDTLS_SSL_IN_CONTENT_LEN", so to save more heap, users can set
> the options to be an appropriate value.



###### CONFIG_MBEDTLS_DYNAMIC_FREE_CONFIG_DATA



> Free private key and DHM data after its usage
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_DYNAMIC_BUFFER
> 
> 
> Free private key and DHM data after its usage in handshake process.
> 
> 
> The option will decrease heap cost when handshake, but also lead to problem:
> 
> 
> Because all certificate, private key and DHM data are freed so users should register
> certificate and private key to ssl config object again.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_DYNAMIC_BUFFER




###### CONFIG_MBEDTLS_DYNAMIC_FREE_CA_CERT



> Free SSL CA certificate after its usage
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_DYNAMIC_BUFFER > CONFIG_MBEDTLS_DYNAMIC_FREE_CONFIG_DATA
> 
> 
> Free CA certificate after its usage in the handshake process.
> This option will decrease the heap footprint for the TLS handshake, but may lead to a problem:
> If the respective ssl object needs to perform the TLS handshake again,
> the CA certificate should once again be registered to the ssl object.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_DYNAMIC_FREE_CONFIG_DATA





##### CONFIG_MBEDTLS_DEBUG



> Enable mbedTLS debugging
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable mbedTLS debugging functions at compile time.
> 
> 
> If this option is enabled, you can include
> "mbedtls/esp_debug.h" and call mbedtls_esp_enable_debug_log()
> at runtime in order to enable mbedTLS debug output via the ESP
> log mechanism.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_MBEDTLS_DEBUG_LEVEL



> Set mbedTLS debugging level
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_DEBUG
> 
> 
> Set mbedTLS debugging level
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Warning (CONFIG_MBEDTLS_DEBUG_LEVEL_WARN)
> > * Info (CONFIG_MBEDTLS_DEBUG_LEVEL_INFO)
> > * Debug (CONFIG_MBEDTLS_DEBUG_LEVEL_DEBUG)
> > * Verbose (CONFIG_MBEDTLS_DEBUG_LEVEL_VERBOSE)





##### mbedTLS v3.x related


Contains:


* DTLS-based configurations
* CONFIG_MBEDTLS_PKCS7_C
* CONFIG_MBEDTLS_SSL_CONTEXT_SERIALIZATION
* CONFIG_MBEDTLS_X509_TRUSTED_CERT_CALLBACK
* CONFIG_MBEDTLS_SSL_KEEP_PEER_CERTIFICATE
* CONFIG_MBEDTLS_SSL_CID_PADDING_GRANULARITY
* CONFIG_MBEDTLS_SSL_PROTO_TLS1_3
* CONFIG_MBEDTLS_ECDH_LEGACY_CONTEXT
* CONFIG_MBEDTLS_SSL_VARIABLE_BUFFER_LENGTH



###### CONFIG_MBEDTLS_SSL_PROTO_TLS1_3



> Support TLS 1.3 protocol
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related




###### TLS 1.3 related configurations


Contains:


* CONFIG_MBEDTLS_SSL_TLS1_3_KEXM_EPHEMERAL
* CONFIG_MBEDTLS_SSL_TLS1_3_COMPATIBILITY_MODE
* CONFIG_MBEDTLS_SSL_TLS1_3_KEXM_PSK_EPHEMERAL
* CONFIG_MBEDTLS_SSL_TLS1_3_KEXM_PSK




###### CONFIG_MBEDTLS_SSL_TLS1_3_COMPATIBILITY_MODE



> TLS 1.3 middlebox compatibility mode
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 > TLS 1.3 related configurations
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_SSL_PROTO_TLS1_3




###### CONFIG_MBEDTLS_SSL_TLS1_3_KEXM_PSK



> TLS 1.3 PSK key exchange mode
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 > TLS 1.3 related configurations
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_SSL_PROTO_TLS1_3




###### CONFIG_MBEDTLS_SSL_TLS1_3_KEXM_EPHEMERAL



> TLS 1.3 ephemeral key exchange mode
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 > TLS 1.3 related configurations
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_SSL_PROTO_TLS1_3




###### CONFIG_MBEDTLS_SSL_TLS1_3_KEXM_PSK_EPHEMERAL



> TLS 1.3 PSK ephemeral key exchange mode
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 > TLS 1.3 related configurations
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_SSL_PROTO_TLS1_3




###### CONFIG_MBEDTLS_SSL_VARIABLE_BUFFER_LENGTH



> Variable SSL buffer length
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related
> 
> 
> This enables the SSL buffer to be resized automatically
> based on the negotiated maximum fragment length in each direction.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_ECDH_LEGACY_CONTEXT



> Use a backward compatible ECDH context (Experimental)
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related
> 
> 
> Use the legacy ECDH context format.
> Define this option only if you enable MBEDTLS_ECP_RESTARTABLE or if you
> want to access ECDH context fields directly.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_ECDH_C && CONFIG_MBEDTLS_ECP_RESTARTABLE




###### CONFIG_MBEDTLS_X509_TRUSTED_CERT_CALLBACK



> Enable trusted certificate callbacks
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related
> 
> 
> Enables users to configure the set of trusted certificates
> through a callback instead of a linked list.
> 
> 
> See mbedTLS documentation for required API and more details.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_SSL_CONTEXT_SERIALIZATION



> Enable serialization of the TLS context structures
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related
> 
> 
> Enable serialization of the TLS context structures
> This is a local optimization in handling a single, potentially long-lived connection.
> 
> 
> See mbedTLS documentation for required API and more details.
> Disabling this option will save some code size.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_SSL_KEEP_PEER_CERTIFICATE



> Keep peer certificate after handshake completion
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related
> 
> 
> Keep the peer's certificate after completion of the handshake.
> Disabling this option will save about 4kB of heap and some code size.
> 
> 
> See mbedTLS documentation for required API and more details.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_PKCS7_C



> Enable PKCS #7
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related
> 
> 
> Enable PKCS #7 core for using PKCS #7-formatted signatures.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_SSL_CID_PADDING_GRANULARITY



> Record plaintext padding
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related
> 
> 
> Controls the use of record plaintext padding in TLS 1.3 and
> when using the Connection ID extension in DTLS 1.2.
> 
> 
> The padding will always be chosen so that the length of the
> padded plaintext is a multiple of the value of this option.
> 
> 
> 
> Notes:A value of 1 means that no padding will be used for outgoing records.
> On systems lacking division instructions, a power of two should be preferred.
> 
> 
> 
> Range:* from 0 to 32 if CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 || CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID
> 
> 
> 
> Default value:* 16 if CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 || CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID




###### DTLS-based configurations


Contains:


* CONFIG_MBEDTLS_SSL_DTLS_SRTP
* CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID




###### CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID



> Support for the DTLS Connection ID extension
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > DTLS-based configurations
> 
> 
> Enable support for the DTLS Connection ID extension which allows to
> identify DTLS connections across changes in the underlying transport.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_SSL_PROTO_DTLS




###### CONFIG_MBEDTLS_SSL_CID_IN_LEN_MAX



> Maximum length of CIDs used for incoming DTLS messages
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > DTLS-based configurations > CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID
> 
> 
> Maximum length of CIDs used for incoming DTLS messages
> 
> 
> 
> Range:* from 0 to 32 if CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID && CONFIG_MBEDTLS_SSL_PROTO_DTLS
> 
> 
> 
> Default value:* 32 if CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID && CONFIG_MBEDTLS_SSL_PROTO_DTLS




###### CONFIG_MBEDTLS_SSL_CID_OUT_LEN_MAX



> Maximum length of CIDs used for outgoing DTLS messages
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > DTLS-based configurations > CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID
> 
> 
> Maximum length of CIDs used for outgoing DTLS messages
> 
> 
> 
> Range:* from 0 to 32 if CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID && CONFIG_MBEDTLS_SSL_PROTO_DTLS
> 
> 
> 
> Default value:* 32 if CONFIG_MBEDTLS_SSL_DTLS_CONNECTION_ID && CONFIG_MBEDTLS_SSL_PROTO_DTLS




###### CONFIG_MBEDTLS_SSL_DTLS_SRTP



> Enable support for negotiation of DTLS-SRTP (RFC 5764)
> 
> 
> *Found in:* Component config > mbedTLS > mbedTLS v3.x related > DTLS-based configurations
> 
> 
> Enable support for negotiation of DTLS-SRTP (RFC 5764) through the use_srtp extension.
> 
> 
> See mbedTLS documentation for required API and more details.
> Disabling this option will save some code size.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_SSL_PROTO_DTLS





##### Certificate Bundle


Contains:


* CONFIG_MBEDTLS_CERTIFICATE_BUNDLE



###### CONFIG_MBEDTLS_CERTIFICATE_BUNDLE



> Enable trusted root certificate bundle
> 
> 
> *Found in:* Component config > mbedTLS > Certificate Bundle
> 
> 
> Enable support for large number of default root certificates
> 
> 
> When enabled this option allows user to store default as well
> as customer specific root certificates in compressed format rather
> than storing full certificate. For the root certificates the public key and the subject name
> will be stored.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_DEFAULT_CERTIFICATE_BUNDLE



> Default certificate bundle options
> 
> 
> *Found in:* Component config > mbedTLS > Certificate Bundle > CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Use the full default certificate bundle (CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEFAULT_FULL)
> > * Use only the most common certificates from the default bundles (CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEFAULT_CMN)
> > 
> > 
> > Use only the most common certificates from the default bundles, reducing the size with 50%,
> > while still having around 99% coverage.
> > * Do not use the default certificate bundle (CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEFAULT_NONE)




###### CONFIG_MBEDTLS_CUSTOM_CERTIFICATE_BUNDLE



> Add custom certificates to the default bundle
> 
> 
> *Found in:* Component config > mbedTLS > Certificate Bundle > CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_CUSTOM_CERTIFICATE_BUNDLE_PATH



> Custom certificate bundle path
> 
> 
> *Found in:* Component config > mbedTLS > Certificate Bundle > CONFIG_MBEDTLS_CERTIFICATE_BUNDLE > CONFIG_MBEDTLS_CUSTOM_CERTIFICATE_BUNDLE
> 
> 
> Name of the custom certificate directory or file. This path is evaluated
> relative to the project root directory.




###### CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEPRECATED_LIST



> Add deprecated root certificates
> 
> 
> *Found in:* Component config > mbedTLS > Certificate Bundle > CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
> 
> 
> Include the deprecated list of root certificates in the bundle.
> This list gets updated when a certificate is removed from the Mozilla's
> NSS root certificate store. This config can be enabled if you would like
> to ensure that none of the certificates that were deployed in the product
> are affected because of the update to bundle. In turn, enabling this
> config keeps expired, retracted certificates in the bundle and it may
> pose a security risk.
> 
> 
> * Deprecated cert list may grow based based on sync with upstream bundle
> * Deprecated certs would be be removed in ESP-IDF (next) major release




###### CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_MAX_CERTS



> Maximum no of certificates allowed in certificate bundle
> 
> 
> *Found in:* Component config > mbedTLS > Certificate Bundle > CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
> 
> 
> 
> Default value:* 200





##### CONFIG_MBEDTLS_ECP_RESTARTABLE



> Enable mbedTLS ecp restartable
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable "non-blocking" ECC operations that can return early and be resumed.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_CMAC_C



> Enable CMAC mode for block ciphers
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable the CMAC (Cipher-based Message Authentication Code) mode for
> block ciphers.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_HARDWARE_AES



> Enable hardware AES acceleration
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable hardware accelerated AES encryption & decryption.
> 
> 
> Note that if the ESP32 CPU is running at 240MHz, hardware AES does not
> offer any speed boost over software AES.



###### CONFIG_MBEDTLS_AES_USE_INTERRUPT



> Use interrupt for long AES operations
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_AES
> 
> 
> Use an interrupt to coordinate long AES operations.
> 
> 
> This allows other code to run on the CPU while an AES operation is pending.
> Otherwise the CPU busy-waits.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_AES_INTERRUPT_LEVEL



> AES hardware interrupt level
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_AES > CONFIG_MBEDTLS_AES_USE_INTERRUPT
> 
> 
> This config helps to set the interrupt priority level for the AES peripheral.
> Value 0 (default) means that there is no preference regarding the interrupt
> priority level and any level from 1 to 3 can be selected (based on the availability).
> Note: Higher value indicates high interrupt priority.
> 
> 
> 
> Range:* from 0 to 3
> 
> 
> 
> Default value:* 0




###### CONFIG_MBEDTLS_HARDWARE_GCM



> Enable partially hardware accelerated GCM
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_AES
> 
> 
> Enable partially hardware accelerated GCM. GHASH calculation is still done
> in software.
> 
> 
> If MBEDTLS_HARDWARE_GCM is disabled and MBEDTLS_HARDWARE_AES is enabled then
> mbedTLS will still use the hardware accelerated AES block operation, but
> on a single block at a time.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_AES_SUPPORT_GCM && CONFIG_MBEDTLS_HARDWARE_AES




###### CONFIG_MBEDTLS_GCM_SUPPORT_NON_AES_CIPHER



> Enable support for non-AES ciphers in GCM operation
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_AES
> 
> 
> Enable this config to support fallback to software definitions for a non-AES
> cipher GCM operation as we support hardware acceleration only for AES cipher.
> Some of the non-AES ciphers used in a GCM operation are DES, ARIA, CAMELLIA,
> CHACHA20, BLOWFISH.
> 
> 
> If this config is disabled, performing a non-AES cipher GCM operation with
> the config MBEDTLS_HARDWARE_AES enabled will result in calculation of an
> AES-GCM operation instead for the given input values and thus could lead
> to failure in certificate validation which would ultimately lead to a SSL
> handshake failure.
> 
> 
> This config being by-default enabled leads to an increase in binary size
> footprint of ~2.5KB.
> In case you are sure that your use case (for example, client and server
> configurations in case of a TLS handshake) would not involve any GCM
> operations using a non-AES cipher, you can safely disable this config,
> leading to reduction in binary size footprint.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_MBEDTLS_HARDWARE_MPI



> Enable hardware MPI (bignum) acceleration
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable hardware accelerated multiple precision integer operations.
> 
> 
> Hardware accelerated multiplication, modulo multiplication,
> and modular exponentiation for up to SOC_RSA_MAX_BIT_LEN bit results.
> 
> 
> These operations are used by RSA.



###### CONFIG_MBEDTLS_LARGE_KEY_SOFTWARE_MPI



> Fallback to software implementation for larger MPI values
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_MPI
> 
> 
> Fallback to software implementation for RSA key lengths
> larger than SOC_RSA_MAX_BIT_LEN. If this is not active
> then the ESP will be unable to process keys greater
> than SOC_RSA_MAX_BIT_LEN.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_MPI_USE_INTERRUPT



> Use interrupt for MPI exp-mod operations
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_MPI
> 
> 
> Use an interrupt to coordinate long MPI operations.
> 
> 
> This allows other code to run on the CPU while an MPI operation is pending.
> Otherwise the CPU busy-waits.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_MPI_INTERRUPT_LEVEL



> MPI hardware interrupt level
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_MPI > CONFIG_MBEDTLS_MPI_USE_INTERRUPT
> 
> 
> This config helps to set the interrupt priority level for the MPI peripheral.
> Value 0 (default) means that there is no preference regarding the interrupt
> priority level and any level from 1 to 3 can be selected (based on the availability).
> Note: Higher value indicates high interrupt priority.
> 
> 
> 
> Range:* from 0 to 3
> 
> 
> 
> Default value:* 0





##### CONFIG_MBEDTLS_HARDWARE_SHA



> Enable hardware SHA acceleration
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable hardware accelerated SHA1, SHA256, SHA384 & SHA512 in mbedTLS.
> 
> 
> Due to a hardware limitation, on the ESP32 hardware acceleration is only
> guaranteed if SHA digests are calculated one at a time. If more
> than one SHA digest is calculated at the same time, one will
> be calculated fully in hardware and the rest will be calculated
> (at least partially calculated) in software. This happens automatically.
> 
> 
> SHA hardware acceleration is faster than software in some situations but
> slower in others. You should benchmark to find the best setting for you.




##### CONFIG_MBEDTLS_HARDWARE_ECC



> Enable hardware ECC acceleration
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable hardware accelerated ECC point multiplication and point verification for points
> on curve SECP192R1 and SECP256R1 in mbedTLS
> 
> 
> 
> Default value:* Yes (enabled) if SOC_ECC_SUPPORTED



###### CONFIG_MBEDTLS_ECC_OTHER_CURVES_SOFT_FALLBACK



> Fallback to software implementation for curves not supported in hardware
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HARDWARE_ECC
> 
> 
> Fallback to software implementation of ECC point multiplication and point verification
> for curves not supported in hardware.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_HARDWARE_ECC





##### CONFIG_MBEDTLS_ROM_MD5



> Use MD5 implementation in ROM
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Use ROM MD5 in mbedTLS.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_HARDWARE_ECDSA_SIGN



> Enable ECDSA signing using on-chip ECDSA peripheral
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable hardware accelerated ECDSA peripheral to sign data
> on curve SECP192R1 and SECP256R1 in mbedTLS.
> 
> 
> Note that for signing, the private key has to be burnt in an efuse key block
> with key purpose set to ECDSA_KEY.
> If no key is burnt, it will report an error
> 
> 
> The key should be burnt in little endian format. espefuse.py utility handles it internally
> but care needs to be taken while burning using esp_efuse APIs
> 
> 
> 
> Default value:* No (disabled) if SOC_ECDSA_SUPPORTED




##### CONFIG_MBEDTLS_HARDWARE_ECDSA_VERIFY



> Enable ECDSA signature verification using on-chip ECDSA peripheral
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable hardware accelerated ECDSA peripheral to verify signature
> on curve SECP192R1 and SECP256R1 in mbedTLS.
> 
> 
> 
> Default value:* Yes (enabled) if SOC_ECDSA_SUPPORTED




##### CONFIG_MBEDTLS_ATCA_HW_ECDSA_SIGN



> Enable hardware ECDSA sign acceleration when using ATECC608A
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> This option enables hardware acceleration for ECDSA sign function, only
> when using ATECC608A cryptoauth chip.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_ATCA_HW_ECDSA_VERIFY



> Enable hardware ECDSA verify acceleration when using ATECC608A
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> This option enables hardware acceleration for ECDSA sign function, only
> when using ATECC608A cryptoauth chip.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_HAVE_TIME



> Enable mbedtls time support
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable use of time.h functions (time() and gmtime()) by mbedTLS.
> 
> 
> This option doesn't require the system time to be correct, but enables
> functionality that requires relative timekeeping - for example periodic
> expiry of TLS session tickets or session cache entries.
> 
> 
> Disabling this option will save some firmware size, particularly if
> the rest of the firmware doesn't call any standard timekeeeping
> functions.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_MBEDTLS_PLATFORM_TIME_ALT



> Enable mbedtls time support: platform-specific
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HAVE_TIME
> 
> 
> Enabling this config will provide users with a function
> "mbedtls_platform_set_time()" that allows to set an alternative
> time function pointer.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_HAVE_TIME_DATE



> Enable mbedtls certificate expiry check
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_HAVE_TIME
> 
> 
> Enables X.509 certificate expiry checks in mbedTLS.
> 
> 
> If this option is disabled (default) then X.509 certificate
> "valid from" and "valid to" timestamp fields are ignored.
> 
> 
> If this option is enabled, these fields are compared with the
> current system date and time. The time is retrieved using the
> standard time() and gmtime() functions. If the certificate is not
> valid for the current system time then verification will fail with
> code MBEDTLS_X509_BADCERT_FUTURE or MBEDTLS_X509_BADCERT_EXPIRED.
> 
> 
> Enabling this option requires adding functionality in the firmware
> to set the system clock to a valid timestamp before using TLS. The
> recommended way to do this is via ESP-IDF's SNTP functionality, but
> any method can be used.
> 
> 
> In the case where only a small number of certificates are trusted by
> the device, please carefully consider the tradeoffs of enabling this
> option. There may be undesired consequences, for example if all
> trusted certificates expire while the device is offline and a TLS
> connection is required to update. Or if an issue with the SNTP
> server means that the system time is invalid for an extended period
> after a reset.
> 
> 
> 
> Default value:* No (disabled)





##### CONFIG_MBEDTLS_ECDSA_DETERMINISTIC



> Enable deterministic ECDSA
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Standard ECDSA is "fragile" in the sense that lack of entropy when signing
> may result in a compromise of the long-term signing key.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_SHA512_C



> Enable the SHA-384 and SHA-512 cryptographic hash algorithms
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable MBEDTLS_SHA512_C adds support for SHA-384 and SHA-512.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_TLS_MODE



> TLS Protocol Role
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> mbedTLS can be compiled with protocol support for the TLS
> server, TLS client, or both server and client.
> 
> 
> Reducing the number of TLS roles supported saves code size.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Server & Client (CONFIG_MBEDTLS_TLS_SERVER_AND_CLIENT)
> > * Server (CONFIG_MBEDTLS_TLS_SERVER_ONLY)
> > * Client (CONFIG_MBEDTLS_TLS_CLIENT_ONLY)
> > * None (CONFIG_MBEDTLS_TLS_DISABLED)




##### TLS Key Exchange Methods


Contains:


* CONFIG_MBEDTLS_KEY_EXCHANGE_DHE_RSA
* CONFIG_MBEDTLS_KEY_EXCHANGE_ECJPAKE
* CONFIG_MBEDTLS_PSK_MODES
* CONFIG_MBEDTLS_KEY_EXCHANGE_RSA
* CONFIG_MBEDTLS_KEY_EXCHANGE_ELLIPTIC_CURVE



###### CONFIG_MBEDTLS_PSK_MODES



> Enable pre-shared-key ciphersuites
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods
> 
> 
> Enable to show configuration for different types of pre-shared-key TLS authentatication methods.
> 
> 
> Leaving this options disabled will save code size if they are not used.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_KEY_EXCHANGE_PSK



> Enable PSK based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_PSK_MODES
> 
> 
> Enable to support symmetric key PSK (pre-shared-key) TLS key exchange modes.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_PSK_MODES




###### CONFIG_MBEDTLS_KEY_EXCHANGE_DHE_PSK



> Enable DHE-PSK based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_PSK_MODES
> 
> 
> Enable to support Diffie-Hellman PSK (pre-shared-key) TLS authentication modes.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_PSK_MODES && CONFIG_MBEDTLS_DHM_C




###### CONFIG_MBEDTLS_KEY_EXCHANGE_ECDHE_PSK



> Enable ECDHE-PSK based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_PSK_MODES
> 
> 
> Enable to support Elliptic-Curve-Diffie-Hellman PSK (pre-shared-key) TLS authentication modes.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_PSK_MODES && CONFIG_MBEDTLS_ECDH_C




###### CONFIG_MBEDTLS_KEY_EXCHANGE_RSA_PSK



> Enable RSA-PSK based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_PSK_MODES
> 
> 
> Enable to support RSA PSK (pre-shared-key) TLS authentication modes.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_PSK_MODES




###### CONFIG_MBEDTLS_KEY_EXCHANGE_RSA



> Enable RSA-only based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods
> 
> 
> Enable to support ciphersuites with prefix TLS-RSA-WITH-
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_KEY_EXCHANGE_DHE_RSA



> Enable DHE-RSA based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods
> 
> 
> Enable to support ciphersuites with prefix TLS-DHE-RSA-WITH-
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_DHM_C




###### CONFIG_MBEDTLS_KEY_EXCHANGE_ELLIPTIC_CURVE



> Support Elliptic Curve based ciphersuites
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods
> 
> 
> Enable to show Elliptic Curve based ciphersuite mode options.
> 
> 
> Disabling all Elliptic Curve ciphersuites saves code size and
> can give slightly faster TLS handshakes, provided the server supports
> RSA-only ciphersuite modes.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_KEY_EXCHANGE_ECDHE_RSA



> Enable ECDHE-RSA based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_KEY_EXCHANGE_ELLIPTIC_CURVE
> 
> 
> Enable to support ciphersuites with prefix TLS-ECDHE-RSA-WITH-
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA



> Enable ECDHE-ECDSA based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_KEY_EXCHANGE_ELLIPTIC_CURVE
> 
> 
> Enable to support ciphersuites with prefix TLS-ECDHE-RSA-WITH-
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA



> Enable ECDH-ECDSA based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_KEY_EXCHANGE_ELLIPTIC_CURVE
> 
> 
> Enable to support ciphersuites with prefix TLS-ECDHE-RSA-WITH-
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_KEY_EXCHANGE_ECDH_RSA



> Enable ECDH-RSA based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods > CONFIG_MBEDTLS_KEY_EXCHANGE_ELLIPTIC_CURVE
> 
> 
> Enable to support ciphersuites with prefix TLS-ECDHE-RSA-WITH-
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_KEY_EXCHANGE_ECJPAKE



> Enable ECJPAKE based ciphersuite modes
> 
> 
> *Found in:* Component config > mbedTLS > TLS Key Exchange Methods
> 
> 
> Enable to support ciphersuites with prefix TLS-ECJPAKE-WITH-
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_ECJPAKE_C && CONFIG_MBEDTLS_ECP_DP_SECP256R1_ENABLED





##### CONFIG_MBEDTLS_SSL_RENEGOTIATION



> Support TLS renegotiation
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> The two main uses of renegotiation are (1) refresh keys on long-lived
> connections and (2) client authentication after the initial handshake.
> If you don't need renegotiation, disabling it will save code size and
> reduce the possibility of abuse/vulnerability.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_SSL_PROTO_TLS1_2



> Support TLS 1.2 protocol
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_SSL_PROTO_GMTSSL1_1



> Support GM/T SSL 1.1 protocol
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Provisions for GM/T SSL 1.1 support
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_SSL_PROTO_DTLS



> Support DTLS protocol (all versions)
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Requires TLS 1.2 to be enabled for DTLS 1.2
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_SSL_ALPN



> Support ALPN (Application Layer Protocol Negotiation)
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Disabling this option will save some code size if it is not needed.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_CLIENT_SSL_SESSION_TICKETS



> TLS: Client Support for RFC 5077 SSL session tickets
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Client support for RFC 5077 session tickets. See mbedTLS documentation for more details.
> Disabling this option will save some code size.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_SERVER_SSL_SESSION_TICKETS



> TLS: Server Support for RFC 5077 SSL session tickets
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Server support for RFC 5077 session tickets. See mbedTLS documentation for more details.
> Disabling this option will save some code size.
> 
> 
> 
> Default value:* Yes (enabled)




##### Symmetric Ciphers


Contains:


* CONFIG_MBEDTLS_AES_C
* CONFIG_MBEDTLS_BLOWFISH_C
* CONFIG_MBEDTLS_CAMELLIA_C
* CONFIG_MBEDTLS_CCM_C
* CONFIG_MBEDTLS_DES_C
* CONFIG_MBEDTLS_GCM_C
* CONFIG_MBEDTLS_NIST_KW_C
* CONFIG_MBEDTLS_XTEA_C



###### CONFIG_MBEDTLS_AES_C



> AES block cipher
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_CAMELLIA_C



> Camellia block cipher
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_DES_C



> DES block cipher (legacy, insecure)
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> Enables the DES block cipher to support 3DES-based TLS ciphersuites.
> 
> 
> 3DES is vulnerable to the Sweet32 attack and should only be enabled
> if absolutely necessary.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_BLOWFISH_C



> Blowfish block cipher (read help)
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> Enables the Blowfish block cipher (not used for TLS sessions.)
> 
> 
> The Blowfish cipher is not used for mbedTLS TLS sessions but can be
> used for other purposes. Read up on the limitations of Blowfish (including
> Sweet32) before enabling.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_XTEA_C



> XTEA block cipher
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> Enables the XTEA block cipher.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_MBEDTLS_CCM_C



> CCM (Counter with CBC-MAC) block cipher modes
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> Enable Counter with CBC-MAC (CCM) modes for AES and/or Camellia ciphers.
> 
> 
> Disabling this option saves some code size.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_GCM_C



> GCM (Galois/Counter) block cipher modes
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> Enable Galois/Counter Mode for AES and/or Camellia ciphers.
> 
> 
> This option is generally faster than CCM.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_NIST_KW_C



> NIST key wrapping (KW) and KW padding (KWP)
> 
> 
> *Found in:* Component config > mbedTLS > Symmetric Ciphers
> 
> 
> Enable NIST key wrapping and key wrapping padding.
> 
> 
> 
> Default value:* No (disabled)





##### CONFIG_MBEDTLS_RIPEMD160_C



> Enable RIPEMD-160 hash algorithm
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable the RIPEMD-160 hash algorithm.
> 
> 
> 
> Default value:* No (disabled)




##### Certificates


Contains:


* CONFIG_MBEDTLS_PEM_PARSE_C
* CONFIG_MBEDTLS_PEM_WRITE_C
* CONFIG_MBEDTLS_X509_CRL_PARSE_C
* CONFIG_MBEDTLS_X509_CSR_PARSE_C



###### CONFIG_MBEDTLS_PEM_PARSE_C



> Read & Parse PEM formatted certificates
> 
> 
> *Found in:* Component config > mbedTLS > Certificates
> 
> 
> Enable decoding/parsing of PEM formatted certificates.
> 
> 
> If your certificates are all in the simpler DER format, disabling
> this option will save some code size.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_PEM_WRITE_C



> Write PEM formatted certificates
> 
> 
> *Found in:* Component config > mbedTLS > Certificates
> 
> 
> Enable writing of PEM formatted certificates.
> 
> 
> If writing certificate data only in DER format, disabling this
> option will save some code size.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_X509_CRL_PARSE_C



> X.509 CRL parsing
> 
> 
> *Found in:* Component config > mbedTLS > Certificates
> 
> 
> Support for parsing X.509 Certificate Revocation Lists.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_MBEDTLS_X509_CSR_PARSE_C



> X.509 CSR parsing
> 
> 
> *Found in:* Component config > mbedTLS > Certificates
> 
> 
> Support for parsing X.509 Certificate Signing Requests
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_MBEDTLS_ECP_C



> Elliptic Curve Ciphers
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_DHM_C



> Diffie-Hellman-Merkle key exchange (DHM)
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable DHM. Needed to use DHE-xxx TLS ciphersuites.
> 
> 
> Note that the security of Diffie-Hellman key exchanges depends on
> a suitable prime being used for the exchange. Please see detailed
> warning text about this in file mbedtls/dhm.h file.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_ECDH_C



> Elliptic Curve Diffie-Hellman (ECDH)
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable ECDH. Needed to use ECDHE-xxx TLS ciphersuites.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_MBEDTLS_ECDSA_C



> Elliptic Curve DSA
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_ECDH_C
> 
> 
> Enable ECDSA. Needed to use ECDSA-xxx TLS ciphersuites.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_MBEDTLS_ECJPAKE_C



> Elliptic curve J-PAKE
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable ECJPAKE. Needed to use ECJPAKE-xxx TLS ciphersuites.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_ECP_DP_SECP192R1_ENABLED



> Enable SECP192R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP192R1 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_SECP224R1_ENABLED



> Enable SECP224R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP224R1 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_SECP256R1_ENABLED



> Enable SECP256R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP256R1 Elliptic Curve.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_ECP_DP_SECP384R1_ENABLED



> Enable SECP384R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP384R1 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_SECP521R1_ENABLED



> Enable SECP521R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP521R1 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_SECP192K1_ENABLED



> Enable SECP192K1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP192K1 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_SECP224K1_ENABLED



> Enable SECP224K1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP224K1 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_SECP256K1_ENABLED



> Enable SECP256K1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for SECP256K1 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_BP256R1_ENABLED



> Enable BP256R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> support for DP Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_BP384R1_ENABLED



> Enable BP384R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> support for DP Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_BP512R1_ENABLED



> Enable BP512R1 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> support for DP Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_DP_CURVE25519_ENABLED



> Enable CURVE25519 curve
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for CURVE25519 Elliptic Curve.




##### CONFIG_MBEDTLS_ECP_NIST_OPTIM



> NIST 'modulo p' optimisations
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> NIST 'modulo p' optimisations increase Elliptic Curve operation performance.
> 
> 
> Disabling this option saves some code size.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_ECP_FIXED_POINT_OPTIM



> Enable fixed-point multiplication optimisations
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> This configuration option enables optimizations to speedup (about 3 ~ 4 times) the ECP
> fixed point multiplication using pre-computed tables in the flash memory.
> Disabling this configuration option saves flash footprint (about 29KB if all Elliptic Curve selected)
> in the application binary.
> 
> 
> # end of Elliptic Curve options
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_POLY1305_C



> Poly1305 MAC algorithm
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for Poly1305 MAC algorithm.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_CHACHA20_C



> Chacha20 stream cipher
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for Chacha20 stream cipher.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_MBEDTLS_CHACHAPOLY_C



> ChaCha20-Poly1305 AEAD algorithm
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_CHACHA20_C
> 
> 
> Enable support for ChaCha20-Poly1305 AEAD algorithm.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_CHACHA20_C && CONFIG_MBEDTLS_POLY1305_C





##### CONFIG_MBEDTLS_HKDF_C



> HKDF algorithm (RFC 5869)
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable support for the Hashed Message Authentication Code
> (HMAC)-based key derivation function (HKDF).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MBEDTLS_THREADING_C



> Enable the threading abstraction layer
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> If you do intend to use contexts between threads, you will need to enable
> this layer to prevent race conditions.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_MBEDTLS_THREADING_ALT



> Enable threading alternate implementation
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_THREADING_C
> 
> 
> Enable threading alt to allow your own alternate threading implementation.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_MBEDTLS_THREADING_C




###### CONFIG_MBEDTLS_THREADING_PTHREAD



> Enable threading pthread implementation
> 
> 
> *Found in:* Component config > mbedTLS > CONFIG_MBEDTLS_THREADING_C
> 
> 
> Enable the pthread wrapper layer for the threading layer.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MBEDTLS_THREADING_C





##### CONFIG_MBEDTLS_ERROR_STRINGS



> Enable error code to error string conversion
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enables mbedtls_strerror() for converting error codes to error strings.
> Disabling this config can save some code/rodata size as the error
> string conversion implementation is replaced with an empty stub.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MBEDTLS_USE_CRYPTO_ROM_IMPL



> Use ROM implementation of the crypto algorithm
> 
> 
> *Found in:* Component config > mbedTLS
> 
> 
> Enable this flag to use mbedtls crypto algorithm from ROM instead of ESP-IDF.
> 
> 
> This configuration option saves flash footprint in the application binary.
> Note that the version of mbedtls crypto algorithm library in ROM is v2.16.12.
> We have done the security analysis of the mbedtls revision in ROM (v2.16.12)
> and ensured that affected symbols have been patched (removed). If in the future
> mbedtls revisions there are security issues that also affects the version in
> ROM (v2.16.12) then we shall patch the relevant symbols. This would increase
> the flash footprint and hence care must be taken to keep some reserved space
> for the application binary in flash layout.
> 
> 
> 
> Default value:* No (disabled) if ESP_ROM_HAS_MBEDTLS_CRYPTO_LIB && CONFIG_IDF_EXPERIMENTAL_FEATURES





#### ESP-MQTT Configurations


Contains:


* CONFIG_MQTT_CUSTOM_OUTBOX
* CONFIG_MQTT_TRANSPORT_SSL
* CONFIG_MQTT_TRANSPORT_WEBSOCKET
* CONFIG_MQTT_PROTOCOL_311
* CONFIG_MQTT_PROTOCOL_5
* CONFIG_MQTT_TASK_CORE_SELECTION_ENABLED
* CONFIG_MQTT_USE_CUSTOM_CONFIG
* CONFIG_MQTT_OUTBOX_EXPIRED_TIMEOUT_MS
* CONFIG_MQTT_REPORT_DELETED_MESSAGES
* CONFIG_MQTT_SKIP_PUBLISH_IF_DISCONNECTED
* CONFIG_MQTT_OUTBOX_DATA_ON_EXTERNAL_MEMORY
* CONFIG_MQTT_MSG_ID_INCREMENTAL



##### CONFIG_MQTT_PROTOCOL_311



> Enable MQTT protocol 3.1.1
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> If not, this library will use MQTT protocol 3.1
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MQTT_PROTOCOL_5



> Enable MQTT protocol 5.0
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> If not, this library will not support MQTT 5.0
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MQTT_TRANSPORT_SSL



> Enable MQTT over SSL
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Enable MQTT transport over SSL with mbedtls
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_MQTT_TRANSPORT_WEBSOCKET



> Enable MQTT over Websocket
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Enable MQTT transport over Websocket.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_MQTT_TRANSPORT_WEBSOCKET_SECURE



> Enable MQTT over Websocket Secure
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_TRANSPORT_WEBSOCKET
> 
> 
> Enable MQTT transport over Websocket Secure.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_MQTT_MSG_ID_INCREMENTAL



> Use Incremental Message Id
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Set this to true for the message id (2.3.1 Packet Identifier) to be generated
> as an incremental number rather then a random value (used by default)
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MQTT_SKIP_PUBLISH_IF_DISCONNECTED



> Skip publish if disconnected
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Set this to true to avoid publishing (enqueueing messages) if the client is disconnected.
> The MQTT client tries to publish all messages by default, even in the disconnected state
> (where the qos1 and qos2 packets are stored in the internal outbox to be published later)
> The MQTT_SKIP_PUBLISH_IF_DISCONNECTED option allows applications to override this behaviour
> and not enqueue publish packets in the disconnected state.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MQTT_REPORT_DELETED_MESSAGES



> Report deleted messages
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Set this to true to post events for all messages which were deleted from the outbox
> before being correctly sent and confirmed.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MQTT_USE_CUSTOM_CONFIG



> MQTT Using custom configurations
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Custom MQTT configurations.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_MQTT_TCP_DEFAULT_PORT



> Default MQTT over TCP port
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> Default MQTT over TCP port
> 
> 
> 
> Default value:* 1883 if CONFIG_MQTT_USE_CUSTOM_CONFIG




###### CONFIG_MQTT_SSL_DEFAULT_PORT



> Default MQTT over SSL port
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> Default MQTT over SSL port
> 
> 
> 
> Default value:* 8883 if CONFIG_MQTT_USE_CUSTOM_CONFIG && CONFIG_MQTT_TRANSPORT_SSL




###### CONFIG_MQTT_WS_DEFAULT_PORT



> Default MQTT over Websocket port
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> Default MQTT over Websocket port
> 
> 
> 
> Default value:* 80 if CONFIG_MQTT_USE_CUSTOM_CONFIG && CONFIG_MQTT_TRANSPORT_WEBSOCKET




###### CONFIG_MQTT_WSS_DEFAULT_PORT



> Default MQTT over Websocket Secure port
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> Default MQTT over Websocket Secure port
> 
> 
> 
> Default value:* 443 if CONFIG_MQTT_USE_CUSTOM_CONFIG && CONFIG_MQTT_TRANSPORT_WEBSOCKET && CONFIG_MQTT_TRANSPORT_WEBSOCKET_SECURE




###### CONFIG_MQTT_BUFFER_SIZE



> Default MQTT Buffer Size
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> This buffer size using for both transmit and receive
> 
> 
> 
> Default value:* 1024 if CONFIG_MQTT_USE_CUSTOM_CONFIG




###### CONFIG_MQTT_TASK_STACK_SIZE



> MQTT task stack size
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> MQTT task stack size
> 
> 
> 
> Default value:* 6144 if CONFIG_MQTT_USE_CUSTOM_CONFIG




###### CONFIG_MQTT_DISABLE_API_LOCKS



> Disable API locks
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> Default config employs API locks to protect internal structures. It is possible to disable
> these locks if the user code doesn't access MQTT API from multiple concurrent tasks
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MQTT_USE_CUSTOM_CONFIG




###### CONFIG_MQTT_TASK_PRIORITY



> MQTT task priority
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> MQTT task priority. Higher number denotes higher priority.
> 
> 
> 
> Default value:* 5 if CONFIG_MQTT_USE_CUSTOM_CONFIG




###### CONFIG_MQTT_POLL_READ_TIMEOUT_MS



> MQTT transport poll read timeut
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> Timeout when polling underlying transport for read.
> 
> 
> 
> Default value:* 1000 if CONFIG_MQTT_USE_CUSTOM_CONFIG




###### CONFIG_MQTT_EVENT_QUEUE_SIZE



> Number of queued events.
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_USE_CUSTOM_CONFIG
> 
> 
> A value higher than 1 enables multiple queued events.
> 
> 
> 
> Default value:* 1 if CONFIG_MQTT_USE_CUSTOM_CONFIG





##### CONFIG_MQTT_TASK_CORE_SELECTION_ENABLED



> Enable MQTT task core selection
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> This will enable core selection



###### CONFIG_MQTT_TASK_CORE_SELECTION



> Core to use ?
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations > CONFIG_MQTT_TASK_CORE_SELECTION_ENABLED
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Core 0 (CONFIG_MQTT_USE_CORE_0)
> > * Core 1 (CONFIG_MQTT_USE_CORE_1)





##### CONFIG_MQTT_OUTBOX_DATA_ON_EXTERNAL_MEMORY



> Use external memory for outbox data
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Set to true to use external memory for outbox data.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_MQTT_USE_CUSTOM_CONFIG




##### CONFIG_MQTT_CUSTOM_OUTBOX



> Enable custom outbox implementation
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Set to true if a specific implementation of message outbox is needed (e.g. persistent outbox in NVM or
> similar).
> Note: Implementation of the custom outbox must be added to the mqtt component. These CMake commands
> could be used to append the custom implementation to lib-mqtt sources:
> idf_component_get_property(mqtt mqtt COMPONENT_LIB)
> set_property(TARGET ${mqtt} PROPERTY SOURCES ${PROJECT_DIR}/custom_outbox.c APPEND)
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_MQTT_OUTBOX_EXPIRED_TIMEOUT_MS



> Outbox message expired timeout[ms]
> 
> 
> *Found in:* Component config > ESP-MQTT Configurations
> 
> 
> Messages which stays in the outbox longer than this value before being published will be discarded.
> 
> 
> 
> Default value:* 30000 if CONFIG_MQTT_USE_CUSTOM_CONFIG





#### Newlib


Contains:


* CONFIG_NEWLIB_NANO_FORMAT
* CONFIG_NEWLIB_STDIN_LINE_ENDING
* CONFIG_NEWLIB_STDOUT_LINE_ENDING
* CONFIG_NEWLIB_TIME_SYSCALL



##### CONFIG_NEWLIB_STDOUT_LINE_ENDING



> Line ending for UART output
> 
> 
> *Found in:* Component config > Newlib
> 
> 
> This option allows configuring the desired line endings sent to UART
> when a newline ('n', LF) appears on stdout.
> Three options are possible:
> 
> 
> CRLF: whenever LF is encountered, prepend it with CR
> 
> 
> LF: no modification is applied, stdout is sent as is
> 
> 
> CR: each occurrence of LF is replaced with CR
> 
> 
> This option doesn't affect behavior of the UART driver (drivers/uart.h).
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CRLF (CONFIG_NEWLIB_STDOUT_LINE_ENDING_CRLF)
> > * LF (CONFIG_NEWLIB_STDOUT_LINE_ENDING_LF)
> > * CR (CONFIG_NEWLIB_STDOUT_LINE_ENDING_CR)




##### CONFIG_NEWLIB_STDIN_LINE_ENDING



> Line ending for UART input
> 
> 
> *Found in:* Component config > Newlib
> 
> 
> This option allows configuring which input sequence on UART produces
> a newline ('n', LF) on stdin.
> Three options are possible:
> 
> 
> CRLF: CRLF is converted to LF
> 
> 
> LF: no modification is applied, input is sent to stdin as is
> 
> 
> CR: each occurrence of CR is replaced with LF
> 
> 
> This option doesn't affect behavior of the UART driver (drivers/uart.h).
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * CRLF (CONFIG_NEWLIB_STDIN_LINE_ENDING_CRLF)
> > * LF (CONFIG_NEWLIB_STDIN_LINE_ENDING_LF)
> > * CR (CONFIG_NEWLIB_STDIN_LINE_ENDING_CR)




##### CONFIG_NEWLIB_NANO_FORMAT



> Enable 'nano' formatting options for printf/scanf family
> 
> 
> *Found in:* Component config > Newlib
> 
> 
> In most chips the ROM contains parts of newlib C library, including printf/scanf family
> of functions. These functions have been compiled with so-called "nano"
> formatting option. This option doesn't support 64-bit integer formats and C99
> features, such as positional arguments.
> 
> 
> For more details about "nano" formatting option, please see newlib readme file,
> search for '--enable-newlib-nano-formatted-io':
> https://sourceware.org/git/?p=newlib-cygwin.git;a=blob_plain;f=newlib/README;hb=HEAD
> 
> 
> If this option is enabled and the ROM contains functions from newlib-nano, the build system
> will use functions available in ROM, reducing the application binary size.
> Functions available in ROM run faster than functions which run from flash. Functions available
> in ROM can also run when flash instruction cache is disabled.
> 
> 
> Some chips (e.g. ESP32-C6) has the full formatting versions of printf/scanf in ROM instead of
> the nano versions and in this building with newlib nano might actually increase the size of
> the binary. Which functions are present in ROM can be seen from ROM caps:
> ESP_ROM_HAS_NEWLIB_NANO_FORMAT and ESP_ROM_HAS_NEWLIB_NORMAL_FORMAT.
> 
> 
> If you need 64-bit integer formatting support or C99 features, keep this
> option disabled.




##### CONFIG_NEWLIB_TIME_SYSCALL



> Timers used for gettimeofday function
> 
> 
> *Found in:* Component config > Newlib
> 
> 
> This setting defines which hardware timers are used to
> implement 'gettimeofday' and 'time' functions in C library.
> 
> 
> * If both high-resolution (systimer for all targets except ESP32)and RTC timers are used, timekeeping will continue in deep sleep.
> Time will be reported at 1 microsecond resolution.
> This is the default, and the recommended option.
> * If only high-resolution timer (systimer) is used, gettimeofday willprovide time at microsecond resolution.
> Time will not be preserved when going into deep sleep mode.
> * If only RTC timer is used, timekeeping will continue indeep sleep, but time will be measured at 6.(6) microsecond
> resolution. Also the gettimeofday function itself may take
> longer to run.
> * If no timers are used, gettimeofday and time functionsreturn -1 and set errno to ENOSYS; they are defined as weak,
> so they could be overridden.
> If you want to customize gettimeofday() and other time functions,
> please choose this option and refer to the 'time.c' source file
> for the exact prototypes of these functions.
> * When RTC is used for timekeeping, two RTC_STORE registers areused to keep time in deep sleep mode.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * RTC and high-resolution timer (CONFIG_NEWLIB_TIME_SYSCALL_USE_RTC_HRT)
> > * RTC (CONFIG_NEWLIB_TIME_SYSCALL_USE_RTC)
> > * High-resolution timer (CONFIG_NEWLIB_TIME_SYSCALL_USE_HRT)
> > * None (CONFIG_NEWLIB_TIME_SYSCALL_USE_NONE)





#### NVS


Contains:


* CONFIG_NVS_LEGACY_DUP_KEYS_COMPATIBILITY
* CONFIG_NVS_ENCRYPTION
* CONFIG_NVS_COMPATIBLE_PRE_V4_3_ENCRYPTION_FLAG
* CONFIG_NVS_ASSERT_ERROR_CHECK



##### CONFIG_NVS_ENCRYPTION



> Enable NVS encryption
> 
> 
> *Found in:* Component config > NVS
> 
> 
> This option enables encryption for NVS. When enabled, XTS-AES is used to encrypt
> the complete NVS data, except the page headers. It requires XTS encryption keys
> to be stored in an encrypted partition (enabling flash encryption is mandatory here)
> or to be derived from an HMAC key burnt in eFuse.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_SECURE_FLASH_ENC_ENABLED




##### CONFIG_NVS_COMPATIBLE_PRE_V4_3_ENCRYPTION_FLAG



> NVS partition encrypted flag compatible with ESP-IDF before v4.3
> 
> 
> *Found in:* Component config > NVS
> 
> 
> Enabling this will ignore "encrypted" flag for NVS partitions. NVS encryption
> scheme is different than hardware flash encryption and hence it is not recommended
> to have "encrypted" flag for NVS partitions. This was not being checked in pre v4.3
> IDF. Hence, if you have any devices where this flag is kept enabled in partition
> table then enabling this config will allow to have same behavior as pre v4.3 IDF.




##### CONFIG_NVS_ASSERT_ERROR_CHECK



> Use assertions for error checking
> 
> 
> *Found in:* Component config > NVS
> 
> 
> This option switches error checking type between assertions (y) or return codes (n).
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_NVS_LEGACY_DUP_KEYS_COMPATIBILITY



> Enable legacy nvs_set function behavior when same key is reused with different data types
> 
> 
> *Found in:* Component config > NVS
> 
> 
> Enabling this option will switch the nvs_set() family of functions to the legacy mode:
> when called repeatedly with the same key but different data type, the existing value
> in the NVS remains active and the new value is just stored, actually not accessible through
> corresponding nvs_get() call for the key given. Use this option only when your application
> relies on such NVS API behaviour.
> 
> 
> 
> Default value:* No (disabled)





#### NVS Security Provider


Contains:


* CONFIG_NVS_SEC_HMAC_EFUSE_KEY_ID
* CONFIG_NVS_SEC_KEY_PROTECTION_SCHEME



##### CONFIG_NVS_SEC_KEY_PROTECTION_SCHEME



> NVS Encryption: Key Protection Scheme
> 
> 
> *Found in:* Component config > NVS Security Provider
> 
> 
> This choice defines the default NVS encryption keys protection scheme;
> which will be used for the default NVS partition.
> Users can use the corresponding scheme registration APIs to register other
> schemes for the default as well as other NVS partitions.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Using Flash Encryption (CONFIG_NVS_SEC_KEY_PROTECT_USING_FLASH_ENC)
> > 
> > 
> > Protect the NVS Encryption Keys using Flash Encryption
> > Requires a separate 'nvs_keys' partition (which will be encrypted by flash encryption)
> > for storing the NVS encryption keys
> > * Using HMAC peripheral (CONFIG_NVS_SEC_KEY_PROTECT_USING_HMAC)
> > 
> > 
> > Derive and protect the NVS Encryption Keys using the HMAC peripheral
> > Requires the specified eFuse block (NVS_SEC_HMAC_EFUSE_KEY_ID or the v2 API argument)
> > to be empty or pre-written with a key with the purpose ESP_EFUSE_KEY_PURPOSE_HMAC_UP




##### CONFIG_NVS_SEC_HMAC_EFUSE_KEY_ID



> eFuse key ID storing the HMAC key
> 
> 
> *Found in:* Component config > NVS Security Provider
> 
> 
> eFuse block key ID storing the HMAC key for deriving the NVS encryption keys
> 
> 
> Note: The eFuse block key ID required by the HMAC scheme
> (CONFIG_NVS_SEC_KEY_PROTECT_USING_HMAC) is set using this config when the default
> NVS partition is initialized with nvs_flash_init(). The eFuse block key ID can
> also be set at runtime by passing the appropriate value to the NVS security scheme
> registration APIs.
> 
> 
> 
> Range:* from 0 to 6 if CONFIG_NVS_SEC_KEY_PROTECT_USING_HMAC
> 
> 
> 
> Default value:* 6 if CONFIG_NVS_SEC_KEY_PROTECT_USING_HMAC





#### OpenThread


Contains:


* CONFIG_OPENTHREAD_PLATFORM_MSGPOOL_MANAGEMENT
* CONFIG_OPENTHREAD_DEVICE_TYPE
* CONFIG_OPENTHREAD_RADIO_TYPE
* CONFIG_OPENTHREAD_BORDER_ROUTER
* CONFIG_OPENTHREAD_COMMISSIONER
* CONFIG_OPENTHREAD_CSL_DEBUG_ENABLE
* CONFIG_OPENTHREAD_CSL_ENABLE
* CONFIG_OPENTHREAD_DIAG
* CONFIG_OPENTHREAD_DNS_CLIENT
* CONFIG_OPENTHREAD_DUA_ENABLE
* CONFIG_OPENTHREAD_JOINER
* CONFIG_OPENTHREAD_LINK_METRICS
* CONFIG_OPENTHREAD_MACFILTER_ENABLE
* CONFIG_OPENTHREAD_CLI
* CONFIG_OPENTHREAD_SPINEL_ONLY
* CONFIG_OPENTHREAD_RX_ON_WHEN_IDLE
* CONFIG_OPENTHREAD_RADIO_STATS_ENABLE
* CONFIG_OPENTHREAD_SRP_CLIENT
* CONFIG_OPENTHREAD_TIME_SYNC
* CONFIG_OPENTHREAD_NCP_VENDOR_HOOK
* CONFIG_OPENTHREAD_MAC_MAX_CSMA_BACKOFFS_DIRECT
* CONFIG_OPENTHREAD_ENABLED
* OpenThread version message
* CONFIG_OPENTHREAD_XTAL_ACCURACY
* CONFIG_OPENTHREAD_CSL_UNCERTAIN
* CONFIG_OPENTHREAD_CSL_ACCURACY
* CONFIG_OPENTHREAD_NUM_MESSAGE_BUFFERS
* CONFIG_OPENTHREAD_RCP_TRANSPORT
* CONFIG_OPENTHREAD_MLE_MAX_CHILDREN
* CONFIG_OPENTHREAD_TMF_ADDR_CACHE_ENTRIES
* CONFIG_OPENTHREAD_SPINEL_RX_FRAME_BUFFER_SIZE
* CONFIG_OPENTHREAD_UART_BUFFER_SIZE
* Thread Address Query Config
* Thread Operational Dataset
* CONFIG_OPENTHREAD_DNS64_CLIENT



##### CONFIG_OPENTHREAD_ENABLED



> OpenThread
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable OpenThread and show the submenu with OpenThread configuration choices.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_OPENTHREAD_LOG_LEVEL_DYNAMIC



> Enable dynamic log level control
> 
> 
> *Found in:* Component config > OpenThread > CONFIG_OPENTHREAD_ENABLED
> 
> 
> Select this option to enable dynamic log level control for OpenThread
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_OPENTHREAD_ENABLED




###### CONFIG_OPENTHREAD_CONSOLE_TYPE



> OpenThread console type
> 
> 
> *Found in:* Component config > OpenThread > CONFIG_OPENTHREAD_ENABLED
> 
> 
> Select OpenThread console type
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * OpenThread console type UART (CONFIG_OPENTHREAD_CONSOLE_TYPE_UART)
> > * OpenThread console type USB Serial/JTAG Controller (CONFIG_OPENTHREAD_CONSOLE_TYPE_USB_SERIAL_JTAG)




###### CONFIG_OPENTHREAD_LOG_LEVEL



> OpenThread log verbosity
> 
> 
> *Found in:* Component config > OpenThread > CONFIG_OPENTHREAD_ENABLED
> 
> 
> Select OpenThread log level.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No logs (CONFIG_OPENTHREAD_LOG_LEVEL_NONE)
> > * Error logs (CONFIG_OPENTHREAD_LOG_LEVEL_CRIT)
> > * Warning logs (CONFIG_OPENTHREAD_LOG_LEVEL_WARN)
> > * Notice logs (CONFIG_OPENTHREAD_LOG_LEVEL_NOTE)
> > * Info logs (CONFIG_OPENTHREAD_LOG_LEVEL_INFO)
> > * Debug logs (CONFIG_OPENTHREAD_LOG_LEVEL_DEBG)





##### Thread Operational Dataset


Contains:


* CONFIG_OPENTHREAD_NETWORK_EXTPANID
* CONFIG_OPENTHREAD_MESH_LOCAL_PREFIX
* CONFIG_OPENTHREAD_NETWORK_CHANNEL
* CONFIG_OPENTHREAD_NETWORK_MASTERKEY
* CONFIG_OPENTHREAD_NETWORK_NAME
* CONFIG_OPENTHREAD_NETWORK_PANID
* CONFIG_OPENTHREAD_NETWORK_PSKC



###### CONFIG_OPENTHREAD_NETWORK_NAME



> OpenThread network name
> 
> 
> *Found in:* Component config > OpenThread > Thread Operational Dataset
> 
> 
> 
> Default value:* "OpenThread-ESP"




###### CONFIG_OPENTHREAD_MESH_LOCAL_PREFIX



> OpenThread mesh local prefix, format <address>/<plen>
> 
> 
> *Found in:* Component config > OpenThread > Thread Operational Dataset
> 
> 
> A string in the format "<address>/<plen>", where <address> is an IPv6
> address and <plen> is a prefix length. For example "fd00:db8:a0:0::/64"
> 
> 
> 
> Default value:* "fd00:db8:a0:0::/64"




###### CONFIG_OPENTHREAD_NETWORK_CHANNEL



> OpenThread network channel
> 
> 
> *Found in:* Component config > OpenThread > Thread Operational Dataset
> 
> 
> 
> Range:* from 11 to 26
> 
> 
> 
> Default value:* 15




###### CONFIG_OPENTHREAD_NETWORK_PANID



> OpenThread network pan id
> 
> 
> *Found in:* Component config > OpenThread > Thread Operational Dataset
> 
> 
> 
> Range:* from 0 to 0xFFFE
> 
> 
> 
> Default value:* "0x1234"




###### CONFIG_OPENTHREAD_NETWORK_EXTPANID



> OpenThread extended pan id
> 
> 
> *Found in:* Component config > OpenThread > Thread Operational Dataset
> 
> 
> The OpenThread network extended pan id in hex string format
> 
> 
> 
> Default value:* dead00beef00cafe




###### CONFIG_OPENTHREAD_NETWORK_MASTERKEY



> OpenThread network key
> 
> 
> *Found in:* Component config > OpenThread > Thread Operational Dataset
> 
> 
> The OpenThread network network key in hex string format
> 
> 
> 
> Default value:* 00112233445566778899aabbccddeeff




###### CONFIG_OPENTHREAD_NETWORK_PSKC



> OpenThread pre-shared commissioner key
> 
> 
> *Found in:* Component config > OpenThread > Thread Operational Dataset
> 
> 
> The OpenThread pre-shared commissioner key in hex string format
> 
> 
> 
> Default value:* 104810e2315100afd6bc9215a6bfac53





##### CONFIG_OPENTHREAD_RADIO_TYPE



> Config the Thread radio type
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Configure how OpenThread connects to the 15.4 radio
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Native 15.4 radio (CONFIG_OPENTHREAD_RADIO_NATIVE)
> > 
> > 
> > Select this to use the native 15.4 radio.
> > * Connect via UART (CONFIG_OPENTHREAD_RADIO_SPINEL_UART)
> > 
> > 
> > Select this to connect to a Radio Co-Processor via UART.
> > * Connect via SPI (CONFIG_OPENTHREAD_RADIO_SPINEL_SPI)
> > 
> > 
> > Select this to connect to a Radio Co-Processor via SPI.




##### CONFIG_OPENTHREAD_DEVICE_TYPE



> Config the Thread device type
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> OpenThread can be configured to different device types (FTD, MTD, Radio)
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Full Thread Device (CONFIG_OPENTHREAD_FTD)
> > 
> > 
> > Select this to enable Full Thread Device which can act as router and leader in a Thread network.
> > * Minimal Thread Device (CONFIG_OPENTHREAD_MTD)
> > 
> > 
> > Select this to enable Minimal Thread Device which can only act as end device in a Thread network.
> > This will reduce the code size of the OpenThread stack.
> > * Radio Only Device (CONFIG_OPENTHREAD_RADIO)
> > 
> > 
> > Select this to enable Radio Only Device which can only forward 15.4 packets to the host.
> > The OpenThread stack will be run on the host and OpenThread will have minimal footprint on the
> > radio only device.




##### CONFIG_OPENTHREAD_RCP_TRANSPORT



> The RCP transport type
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * UART RCP (CONFIG_OPENTHREAD_RCP_UART)
> > 
> > 
> > Select this to enable UART connection to host.
> > * SPI RCP (CONFIG_OPENTHREAD_RCP_SPI)
> > 
> > 
> > Select this to enable SPI connection to host.




##### OpenThread version message


Contains:


* CONFIG_OPENTHREAD_PACKAGE_NAME
* CONFIG_OPENTHREAD_PLATFORM_INFO



###### CONFIG_OPENTHREAD_PACKAGE_NAME



> OpenThread package name
> 
> 
> *Found in:* Component config > OpenThread > OpenThread version message
> 
> 
> The OpenThread package name.
> 
> 
> 
> Default value:* "openthread-esp32" if CONFIG_OPENTHREAD_ENABLED




###### CONFIG_OPENTHREAD_PLATFORM_INFO



> platform information
> 
> 
> *Found in:* Component config > OpenThread > OpenThread version message
> 
> 
> The OpenThread platform information.
> 
> 
> 
> Default value:* "esp32s3" if CONFIG_OPENTHREAD_ENABLED





##### CONFIG_OPENTHREAD_NCP_VENDOR_HOOK



> Enable vendor command for RCP
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this to enable OpenThread NCP vendor commands.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_OPENTHREAD_RADIO




##### CONFIG_OPENTHREAD_CLI



> Enable Openthread Command-Line Interface
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable Command-Line Interface in OpenThread.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_DIAG



> Enable diag
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable Diag in OpenThread. This will enable diag mode and a series of diag commands
> in the OpenThread command line. These commands allow users to manipulate low-level features of the storage
> and 15.4 radio.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_COMMISSIONER



> Enable Commissioner
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable commissioner in OpenThread. This will enable the device to act as a
> commissioner in the Thread network. A commissioner checks the pre-shared key from a joining device with
> the Thread commissioning protocol and shares the network parameter with the joining device upon success.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED



###### CONFIG_OPENTHREAD_COMM_MAX_JOINER_ENTRIES



> The size of max commissioning joiner entries
> 
> 
> *Found in:* Component config > OpenThread > CONFIG_OPENTHREAD_COMMISSIONER
> 
> 
> 
> Default value:* 2 if CONFIG_OPENTHREAD_COMMISSIONER





##### CONFIG_OPENTHREAD_JOINER



> Enable Joiner
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable Joiner in OpenThread. This allows a device to join the Thread network with a
> pre-shared key using the Thread commissioning protocol.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_SRP_CLIENT



> Enable SRP Client
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable SRP Client in OpenThread. This allows a device to register SRP services to SRP
> Server.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_OPENTHREAD_ENABLED



###### CONFIG_OPENTHREAD_SRP_CLIENT_MAX_SERVICES



> Specifies number of service entries in the SRP client service pool
> 
> 
> *Found in:* Component config > OpenThread > CONFIG_OPENTHREAD_SRP_CLIENT
> 
> 
> Set the max buffer size of service entries in the SRP client service pool.
> 
> 
> 
> Default value:* 5 if CONFIG_OPENTHREAD_SRP_CLIENT





##### CONFIG_OPENTHREAD_DNS_CLIENT



> Enable DNS Client
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable DNS Client in OpenThread.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_BORDER_ROUTER



> Enable Border Router
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable border router features in OpenThread.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_PLATFORM_MSGPOOL_MANAGEMENT



> Allocate message pool buffer from PSRAM
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> If enabled, the message pool is managed by platform defined logic.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED && (CONFIG_SPIRAM_USE_CAPS_ALLOC || CONFIG_SPIRAM_USE_MALLOC)




##### CONFIG_OPENTHREAD_NUM_MESSAGE_BUFFERS



> The number of openthread message buffers
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> 
> Default value:* 65 if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_SPINEL_RX_FRAME_BUFFER_SIZE



> The size of openthread spinel rx frame buffer
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> 
> Default value:* 1024 if (CONFIG_OPENTHREAD_MTD || CONFIG_OPENTHREAD_RADIO) && (CONFIG_OPENTHREAD_ENABLED || CONFIG_OPENTHREAD_SPINEL_ONLY)
> * 2048 if (CONFIG_OPENTHREAD_FTD || CONFIG_OPENTHREAD_SPINEL_ONLY) && (CONFIG_OPENTHREAD_ENABLED || CONFIG_OPENTHREAD_SPINEL_ONLY)




##### CONFIG_OPENTHREAD_MAC_MAX_CSMA_BACKOFFS_DIRECT



> Maximum backoffs times before declaring a channel access failure.
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> The maximum number of backoffs the CSMA-CA algorithm will attempt before declaring a channel access
> failure.
> 
> 
> 
> Default value:* 4 if CONFIG_OPENTHREAD_ENABLED || CONFIG_OPENTHREAD_SPINEL_ONLY




##### CONFIG_OPENTHREAD_MLE_MAX_CHILDREN



> The size of max MLE children entries
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> 
> Default value:* 10 if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_TMF_ADDR_CACHE_ENTRIES



> The size of max TMF address cache entries
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> 
> Default value:* 20 if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_DNS64_CLIENT



> Use dns64 client
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to acquire NAT64 address from dns servers.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED && CONFIG_LWIP_IPV4



###### CONFIG_OPENTHREAD_DNS_SERVER_ADDR



> DNS server address (IPv4)
> 
> 
> *Found in:* Component config > OpenThread > CONFIG_OPENTHREAD_DNS64_CLIENT
> 
> 
> Set the DNS server IPv4 address.
> 
> 
> 
> Default value:* "8.8.8.8" if CONFIG_OPENTHREAD_DNS64_CLIENT





##### CONFIG_OPENTHREAD_UART_BUFFER_SIZE



> The uart received buffer size of openthread
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Set the OpenThread UART buffer size.
> 
> 
> 
> Default value:* 2048 if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_LINK_METRICS



> Enable link metrics feature
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable link metrics feature
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_MACFILTER_ENABLE



> Enable mac filter feature
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable mac filter feature
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_CSL_ENABLE



> Enable CSL feature
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable CSL feature
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_XTAL_ACCURACY



> The accuracy of the XTAL
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> The device's XTAL accuracy, in ppm.
> 
> 
> 
> Default value:* 130




##### CONFIG_OPENTHREAD_CSL_ACCURACY



> The current CSL rx/tx scheduling drift, in units of ± ppm
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> The current accuracy of the clock used for scheduling CSL operations
> 
> 
> 
> Default value:* 1 if CONFIG_OPENTHREAD_CSL_ENABLE




##### CONFIG_OPENTHREAD_CSL_UNCERTAIN



> The CSL Uncertainty in units of 10 us.
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> The fixed uncertainty of the Device for scheduling CSL Transmissions in units of 10 microseconds.
> 
> 
> 
> Default value:* 1 if CONFIG_OPENTHREAD_CSL_ENABLE




##### CONFIG_OPENTHREAD_CSL_DEBUG_ENABLE



> Enable CSL debug
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to set rx on when sleep in CSL feature, only for debug
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_CSL_ENABLE




##### CONFIG_OPENTHREAD_DUA_ENABLE



> Enable Domain Unicast Address feature
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Only used for Thread1.2 certification
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_TIME_SYNC



> Enable the time synchronization service feature
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable time synchronization feature, the devices in the same Thread network could
> sync to the same network time.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_ENABLED




##### CONFIG_OPENTHREAD_RADIO_STATS_ENABLE



> Enable Radio Statistics feature
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable the radio statistics feature, you can use radio command to print some radio
> Statistics information.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_OPENTHREAD_FTD || CONFIG_OPENTHREAD_MTD




##### CONFIG_OPENTHREAD_SPINEL_ONLY



> Enable OpenThread External Radio Spinel feature
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable the OpenThread Radio Spinel for external protocol stack, such as Zigbee.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_OPENTHREAD_RX_ON_WHEN_IDLE



> Enable OpenThread radio capability rx on when idle
> 
> 
> *Found in:* Component config > OpenThread
> 
> 
> Select this option to enable OpenThread radio capability rx on when idle. Do not support this feature when
> SW coexistence is enabled.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ESP_COEX_SW_COEXIST_ENABLE




##### Thread Address Query Config


Contains:


* CONFIG_OPENTHREAD_ADDRESS_QUERY_RETRY_DELAY
* CONFIG_OPENTHREAD_ADDRESS_QUERY_MAX_RETRY_DELAY
* CONFIG_OPENTHREAD_ADDRESS_QUERY_TIMEOUT



###### CONFIG_OPENTHREAD_ADDRESS_QUERY_TIMEOUT



> Timeout value (in seconds) for a address notification response after sending an address query.
> 
> 
> *Found in:* Component config > OpenThread > Thread Address Query Config
> 
> 
> 
> Default value:* 3 if CONFIG_OPENTHREAD_FTD || CONFIG_OPENTHREAD_MTD




###### CONFIG_OPENTHREAD_ADDRESS_QUERY_RETRY_DELAY



> Initial retry delay for address query (in seconds).
> 
> 
> *Found in:* Component config > OpenThread > Thread Address Query Config
> 
> 
> 
> Default value:* 15 if CONFIG_OPENTHREAD_FTD || CONFIG_OPENTHREAD_MTD




###### CONFIG_OPENTHREAD_ADDRESS_QUERY_MAX_RETRY_DELAY



> Maximum retry delay for address query (in seconds).
> 
> 
> *Found in:* Component config > OpenThread > Thread Address Query Config
> 
> 
> 
> Default value:* 120 if CONFIG_OPENTHREAD_FTD || CONFIG_OPENTHREAD_MTD






#### Protocomm


Contains:


* CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_0
* CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_1
* CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_2



##### CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_0



> Support protocomm security version 0 (no security)
> 
> 
> *Found in:* Component config > Protocomm
> 
> 
> Enable support of security version 0.
> Disabling this option saves some code size.
> Consult the Enabling protocomm security version section of the
> Protocomm documentation in ESP-IDF Programming guide for more details.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_1



> Support protocomm security version 1 (Curve25519 key exchange + AES-CTR encryption/decryption)
> 
> 
> *Found in:* Component config > Protocomm
> 
> 
> Enable support of security version 1.
> Disabling this option saves some code size.
> Consult the Enabling protocomm security version section of the
> Protocomm documentation in ESP-IDF Programming guide for more details.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_2



> Support protocomm security version 2 (SRP6a-based key exchange + AES-GCM encryption/decryption)
> 
> 
> *Found in:* Component config > Protocomm
> 
> 
> Enable support of security version 2.
> Disabling this option saves some code size.
> Consult the Enabling protocomm security version section of the
> Protocomm documentation in ESP-IDF Programming guide for more details.
> 
> 
> 
> Default value:* Yes (enabled)





#### PThreads


Contains:


* CONFIG_PTHREAD_TASK_NAME_DEFAULT
* CONFIG_PTHREAD_TASK_CORE_DEFAULT
* CONFIG_PTHREAD_TASK_PRIO_DEFAULT
* CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT
* CONFIG_PTHREAD_STACK_MIN



##### CONFIG_PTHREAD_TASK_PRIO_DEFAULT



> Default task priority
> 
> 
> *Found in:* Component config > PThreads
> 
> 
> Priority used to create new tasks with default pthread parameters.
> 
> 
> 
> Range:* from 0 to 255
> 
> 
> 
> Default value:* 5




##### CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT



> Default task stack size
> 
> 
> *Found in:* Component config > PThreads
> 
> 
> Stack size used to create new tasks with default pthread parameters.
> 
> 
> 
> Default value:* 3072




##### CONFIG_PTHREAD_STACK_MIN



> Minimum allowed pthread stack size
> 
> 
> *Found in:* Component config > PThreads
> 
> 
> Minimum allowed pthread stack size set in attributes passed to pthread_create
> 
> 
> 
> Default value:* 768




##### CONFIG_PTHREAD_TASK_CORE_DEFAULT



> Default pthread core affinity
> 
> 
> *Found in:* Component config > PThreads
> 
> 
> The default core to which pthreads are pinned.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * No affinity (CONFIG_PTHREAD_DEFAULT_CORE_NO_AFFINITY)
> > * Core 0 (CONFIG_PTHREAD_DEFAULT_CORE_0)
> > * Core 1 (CONFIG_PTHREAD_DEFAULT_CORE_1)




##### CONFIG_PTHREAD_TASK_NAME_DEFAULT



> Default name of pthreads
> 
> 
> *Found in:* Component config > PThreads
> 
> 
> The default name of pthreads.
> 
> 
> 
> Default value:* "pthread"





#### SoC Settings


Contains:


* MMU Config



##### MMU Config





#### Main Flash configuration


Contains:


* Optional and Experimental Features (READ DOCS FIRST)
* SPI Flash behavior when brownout



##### SPI Flash behavior when brownout


Contains:


* CONFIG_SPI_FLASH_BROWNOUT_RESET_XMC



###### CONFIG_SPI_FLASH_BROWNOUT_RESET_XMC



> Enable sending reset when brownout for XMC flash chips
> 
> 
> *Found in:* Component config > Main Flash configuration > SPI Flash behavior when brownout
> 
> 
> When this option is selected, the patch will be enabled for XMC.
> Follow the recommended flow by XMC for better stability.
> 
> 
> DO NOT DISABLE UNLESS YOU KNOW WHAT YOU ARE DOING.





##### Optional and Experimental Features (READ DOCS FIRST)


Contains:


* CONFIG_SPI_FLASH_AUTO_SUSPEND
* CONFIG_SPI_FLASH_HPM
* CONFIG_SPI_FLASH_SUSPEND_TSUS_VAL_US
* CONFIG_SPI_FLASH_HPM_DC



###### CONFIG_SPI_FLASH_HPM



> High Performance Mode (READ DOCS FIRST, > 80MHz)
> 
> 
> *Found in:* Component config > Main Flash configuration > Optional and Experimental Features (READ DOCS FIRST)
> 
> 
> Whether the High Performance Mode of Flash is enabled. As an optional feature, user needs to manually
> enable this option as a confirmation. To be back-compatible with earlier IDF versionn, this option is
> automatically enabled with warning when Flash running > 80Mhz.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Enable (CONFIG_SPI_FLASH_HPM_ENA)
> > * Auto (Not recommended) (CONFIG_SPI_FLASH_HPM_AUTO)
> > * Disabled (CONFIG_SPI_FLASH_HPM_DIS)




###### CONFIG_SPI_FLASH_HPM_DC



> Support HPM using DC (READ DOCS FIRST)
> 
> 
> *Found in:* Component config > Main Flash configuration > Optional and Experimental Features (READ DOCS FIRST)
> 
> 
> This feature needs your bootloader to be compiled DC-aware (BOOTLOADER_FLASH_DC_AWARE=y). Otherwise the
> chip will not be able to boot after a reset.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Auto (Enable when bootloader support enabled (BOOTLOADER_FLASH_DC_AWARE)) (CONFIG_SPI_FLASH_HPM_DC_AUTO)
> > * Disable (READ DOCS FIRST) (CONFIG_SPI_FLASH_HPM_DC_DISABLE)




###### CONFIG_SPI_FLASH_AUTO_SUSPEND



> Auto suspend long erase/write operations (READ DOCS FIRST)
> 
> 
> *Found in:* Component config > Main Flash configuration > Optional and Experimental Features (READ DOCS FIRST)
> 
> 
> This option is disabled by default because it is supported only
> for specific flash chips and for specific Espressif chips.
> To evaluate if you can use this feature refer to
> Optional Features for Flash > Auto Suspend & Resume of the ESP-IDF Programming Guide.
> 
> 
> CAUTION: If you want to OTA to an app with this feature turned on, please make
> sure the bootloader has the support for it. (later than IDF v4.3)
> 
> 
> If you are using an official Espressif module, please contact Espressif Business support
> to check if the module has the flash that support this feature installed.
> Also refer to Concurrency Constraints for Flash on SPI1 > Flash Auto Suspend Feature
> before enabling this option.




###### CONFIG_SPI_FLASH_SUSPEND_TSUS_VAL_US



> SPI flash tSUS value (refer to chapter AC CHARACTERISTICS)
> 
> 
> *Found in:* Component config > Main Flash configuration > Optional and Experimental Features (READ DOCS FIRST)
> 
> 
> This config is used for setting Tsus parameter. Tsus means CS# high to next command after
> suspend. You can refer to the chapter of AC CHARACTERISTICS of flash datasheet.






#### SPI Flash driver


Contains:


* Auto-detect flash chips
* CONFIG_SPI_FLASH_BYPASS_BLOCK_ERASE
* CONFIG_SPI_FLASH_ENABLE_ENCRYPTED_READ_WRITE
* CONFIG_SPI_FLASH_ENABLE_COUNTERS
* CONFIG_SPI_FLASH_ROM_DRIVER_PATCH
* CONFIG_SPI_FLASH_YIELD_DURING_ERASE
* CONFIG_SPI_FLASH_CHECK_ERASE_TIMEOUT_DISABLED
* CONFIG_SPI_FLASH_WRITE_CHUNK_SIZE
* CONFIG_SPI_FLASH_OVERRIDE_CHIP_DRIVER_LIST
* CONFIG_SPI_FLASH_SIZE_OVERRIDE
* CONFIG_SPI_FLASH_ROM_IMPL
* CONFIG_SPI_FLASH_VERIFY_WRITE
* CONFIG_SPI_FLASH_DANGEROUS_WRITE



##### CONFIG_SPI_FLASH_VERIFY_WRITE



> Verify SPI flash writes
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> If this option is enabled, any time SPI flash is written then the data will be read
> back and verified. This can catch hardware problems with SPI flash, or flash which
> was not erased before verification.



###### CONFIG_SPI_FLASH_LOG_FAILED_WRITE



> Log errors if verification fails
> 
> 
> *Found in:* Component config > SPI Flash driver > CONFIG_SPI_FLASH_VERIFY_WRITE
> 
> 
> If this option is enabled, if SPI flash write verification fails then a log error line
> will be written with the address, expected & actual values. This can be useful when
> debugging hardware SPI flash problems.




###### CONFIG_SPI_FLASH_WARN_SETTING_ZERO_TO_ONE



> Log warning if writing zero bits to ones
> 
> 
> *Found in:* Component config > SPI Flash driver > CONFIG_SPI_FLASH_VERIFY_WRITE
> 
> 
> If this option is enabled, any SPI flash write which tries to set zero bits in the flash to
> ones will log a warning. Such writes will not result in the requested data appearing identically
> in flash once written, as SPI NOR flash can only set bits to one when an entire sector is erased.
> After erasing, individual bits can only be written from one to zero.
> 
> 
> Note that some software (such as SPIFFS) which is aware of SPI NOR flash may write one bits as an
> optimisation, relying on the data in flash becoming a bitwise AND of the new data and any existing data.
> Such software will log spurious warnings if this option is enabled.





##### CONFIG_SPI_FLASH_ENABLE_COUNTERS



> Enable operation counters
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> This option enables the following APIs:
> 
> 
> * esp_flash_reset_counters
> * esp_flash_dump_counters
> * esp_flash_get_counters
> 
> 
> These APIs may be used to collect performance data for spi_flash APIs
> and to help understand behaviour of libraries which use SPI flash.




##### CONFIG_SPI_FLASH_ROM_DRIVER_PATCH



> Enable SPI flash ROM driver patched functions
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> Enable this flag to use patched versions of SPI flash ROM driver functions.
> This option should be enabled, if any one of the following is true: (1) need to write
> to flash on ESP32-D2WD; (2) main SPI flash is connected to non-default pins; (3) main
> SPI flash chip is manufactured by ISSI.




##### CONFIG_SPI_FLASH_ROM_IMPL



> Use esp_flash implementation in ROM
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> Enable this flag to use new SPI flash driver functions from ROM instead of ESP-IDF.
> 
> 
> If keeping this as "n" in your project, you will have less free IRAM.
> But you can use all of our flash features.
> 
> 
> If making this as "y" in your project, you will increase free IRAM.
> But you may miss out on some flash features and support for new flash chips.
> 
> 
> Currently the ROM cannot support the following features:
> 
> 
> * SPI_FLASH_AUTO_SUSPEND (C3, S3)




##### CONFIG_SPI_FLASH_DANGEROUS_WRITE



> Writing to dangerous flash regions
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> SPI flash APIs can optionally abort or return a failure code
> if erasing or writing addresses that fall at the beginning
> of flash (covering the bootloader and partition table) or that
> overlap the app partition that contains the running app.
> 
> 
> It is not recommended to ever write to these regions from an IDF app,
> and this check prevents logic errors or corrupted firmware memory from
> damaging these regions.
> 
> 
> Note that this feature \*does not\* check calls to the esp_rom_xxx SPI flash
> ROM functions. These functions should not be called directly from IDF
> applications.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Aborts (CONFIG_SPI_FLASH_DANGEROUS_WRITE_ABORTS)
> > * Fails (CONFIG_SPI_FLASH_DANGEROUS_WRITE_FAILS)
> > * Allowed (CONFIG_SPI_FLASH_DANGEROUS_WRITE_ALLOWED)




##### CONFIG_SPI_FLASH_BYPASS_BLOCK_ERASE



> Bypass a block erase and always do sector erase
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> Some flash chips can have very high "max" erase times, especially for block erase (32KB or 64KB).
> This option allows to bypass "block erase" and always do sector erase commands.
> This will be much slower overall in most cases, but improves latency for other code to run.




##### CONFIG_SPI_FLASH_YIELD_DURING_ERASE



> Enables yield operation during flash erase
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> This allows to yield the CPUs between erase commands.
> Prevents starvation of other tasks.
> Please use this configuration together with `SPI\_FLASH\_ERASE\_YIELD\_DURATION\_MS` and
> `SPI\_FLASH\_ERASE\_YIELD\_TICKS` after carefully checking flash datasheet to avoid a
> watchdog timeout.
> For more information, please check SPI Flash API reference documenation
> under section OS Function.



###### CONFIG_SPI_FLASH_ERASE_YIELD_DURATION_MS



> Duration of erasing to yield CPUs (ms)
> 
> 
> *Found in:* Component config > SPI Flash driver > CONFIG_SPI_FLASH_YIELD_DURING_ERASE
> 
> 
> If a duration of one erase command is large
> then it will yield CPUs after finishing a current command.




###### CONFIG_SPI_FLASH_ERASE_YIELD_TICKS



> CPU release time (tick) for an erase operation
> 
> 
> *Found in:* Component config > SPI Flash driver > CONFIG_SPI_FLASH_YIELD_DURING_ERASE
> 
> 
> Defines how many ticks will be before returning to continue a erasing.





##### CONFIG_SPI_FLASH_WRITE_CHUNK_SIZE



> Flash write chunk size
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> Flash write is broken down in terms of multiple (smaller) write operations.
> This configuration options helps to set individual write chunk size, smaller
> value here ensures that cache (and non-IRAM resident interrupts) remains
> disabled for shorter duration.




##### CONFIG_SPI_FLASH_SIZE_OVERRIDE



> Override flash size in bootloader header by ESPTOOLPY_FLASHSIZE
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> SPI Flash driver uses the flash size configured in bootloader header by default.
> Enable this option to override flash size with latest ESPTOOLPY_FLASHSIZE value from
> the app header if the size in the bootloader header is incorrect.




##### CONFIG_SPI_FLASH_CHECK_ERASE_TIMEOUT_DISABLED



> Flash timeout checkout disabled
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> This option is helpful if you are using a flash chip whose timeout is quite large or unpredictable.




##### CONFIG_SPI_FLASH_OVERRIDE_CHIP_DRIVER_LIST



> Override default chip driver list
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> This option allows the chip driver list to be customized, instead of using the default list provided by
> ESP-IDF.
> 
> 
> When this option is enabled, the default list is no longer compiled or linked. Instead, the
> default_registered_chips structure must be provided by the user.
> 
> 
> See example: custom_chip_driver under examples/storage for more details.




##### Auto-detect flash chips


Contains:


* CONFIG_SPI_FLASH_SUPPORT_BOYA_CHIP
* CONFIG_SPI_FLASH_SUPPORT_GD_CHIP
* CONFIG_SPI_FLASH_SUPPORT_ISSI_CHIP
* CONFIG_SPI_FLASH_SUPPORT_MXIC_CHIP
* CONFIG_SPI_FLASH_SUPPORT_MXIC_OPI_CHIP
* CONFIG_SPI_FLASH_SUPPORT_TH_CHIP
* CONFIG_SPI_FLASH_SUPPORT_WINBOND_CHIP



###### CONFIG_SPI_FLASH_SUPPORT_ISSI_CHIP



> ISSI
> 
> 
> *Found in:* Component config > SPI Flash driver > Auto-detect flash chips
> 
> 
> Enable this to support auto detection of ISSI chips if chip vendor not directly
> given by `chip\_drv` member of the chip struct. This adds support for variant
> chips, however will extend detecting time.




###### CONFIG_SPI_FLASH_SUPPORT_MXIC_CHIP



> MXIC
> 
> 
> *Found in:* Component config > SPI Flash driver > Auto-detect flash chips
> 
> 
> Enable this to support auto detection of MXIC chips if chip vendor not directly
> given by `chip\_drv` member of the chip struct. This adds support for variant
> chips, however will extend detecting time.




###### CONFIG_SPI_FLASH_SUPPORT_GD_CHIP



> GigaDevice
> 
> 
> *Found in:* Component config > SPI Flash driver > Auto-detect flash chips
> 
> 
> Enable this to support auto detection of GD (GigaDevice) chips if chip vendor not
> directly given by `chip\_drv` member of the chip struct. If you are using Wrover
> modules, please don't disable this, otherwise your flash may not work in 4-bit
> mode.
> 
> 
> This adds support for variant chips, however will extend detecting time and image
> size. Note that the default chip driver supports the GD chips with product ID
> 60H.




###### CONFIG_SPI_FLASH_SUPPORT_WINBOND_CHIP



> Winbond
> 
> 
> *Found in:* Component config > SPI Flash driver > Auto-detect flash chips
> 
> 
> Enable this to support auto detection of Winbond chips if chip vendor not directly
> given by `chip\_drv` member of the chip struct. This adds support for variant
> chips, however will extend detecting time.




###### CONFIG_SPI_FLASH_SUPPORT_BOYA_CHIP



> BOYA
> 
> 
> *Found in:* Component config > SPI Flash driver > Auto-detect flash chips
> 
> 
> Enable this to support auto detection of BOYA chips if chip vendor not directly
> given by `chip\_drv` member of the chip struct. This adds support for variant
> chips, however will extend detecting time.




###### CONFIG_SPI_FLASH_SUPPORT_TH_CHIP



> TH
> 
> 
> *Found in:* Component config > SPI Flash driver > Auto-detect flash chips
> 
> 
> Enable this to support auto detection of TH chips if chip vendor not directly
> given by `chip\_drv` member of the chip struct. This adds support for variant
> chips, however will extend detecting time.




###### CONFIG_SPI_FLASH_SUPPORT_MXIC_OPI_CHIP



> mxic (opi)
> 
> 
> *Found in:* Component config > SPI Flash driver > Auto-detect flash chips
> 
> 
> Enable this to support auto detection of Octal MXIC chips if chip vendor not directly
> given by `chip\_drv` member of the chip struct. This adds support for variant
> chips, however will extend detecting time.





##### CONFIG_SPI_FLASH_ENABLE_ENCRYPTED_READ_WRITE



> Enable encrypted partition read/write operations
> 
> 
> *Found in:* Component config > SPI Flash driver
> 
> 
> This option enables flash read/write operations to encrypted partition/s. This option
> is kept enabled irrespective of state of flash encryption feature. However, in case
> application is not using flash encryption feature and is in need of some additional
> memory from IRAM region (~1KB) then this config can be disabled.





#### SPIFFS Configuration


Contains:


* Debug Configuration
* CONFIG_SPIFFS_USE_MAGIC
* CONFIG_SPIFFS_GC_STATS
* CONFIG_SPIFFS_PAGE_CHECK
* CONFIG_SPIFFS_FOLLOW_SYMLINKS
* CONFIG_SPIFFS_MAX_PARTITIONS
* CONFIG_SPIFFS_USE_MTIME
* CONFIG_SPIFFS_GC_MAX_RUNS
* CONFIG_SPIFFS_OBJ_NAME_LEN
* CONFIG_SPIFFS_META_LENGTH
* SPIFFS Cache Configuration
* CONFIG_SPIFFS_PAGE_SIZE
* CONFIG_SPIFFS_MTIME_WIDE_64_BITS



##### CONFIG_SPIFFS_MAX_PARTITIONS



> Maximum Number of Partitions
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> Define maximum number of partitions that can be mounted.
> 
> 
> 
> Range:* from 1 to 10
> 
> 
> 
> Default value:* 3




##### SPIFFS Cache Configuration


Contains:


* CONFIG_SPIFFS_CACHE



###### CONFIG_SPIFFS_CACHE



> Enable SPIFFS Cache
> 
> 
> *Found in:* Component config > SPIFFS Configuration > SPIFFS Cache Configuration
> 
> 
> Enables/disable memory read caching of nucleus file system
> operations.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_SPIFFS_CACHE_WR



> Enable SPIFFS Write Caching
> 
> 
> *Found in:* Component config > SPIFFS Configuration > SPIFFS Cache Configuration > CONFIG_SPIFFS_CACHE
> 
> 
> Enables memory write caching for file descriptors in hydrogen.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_SPIFFS_CACHE_STATS



> Enable SPIFFS Cache Statistics
> 
> 
> *Found in:* Component config > SPIFFS Configuration > SPIFFS Cache Configuration > CONFIG_SPIFFS_CACHE
> 
> 
> Enable/disable statistics on caching. Debug/test purpose only.
> 
> 
> 
> Default value:* No (disabled)





##### CONFIG_SPIFFS_PAGE_CHECK



> Enable SPIFFS Page Check
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> Always check header of each accessed page to ensure consistent state.
> If enabled it will increase number of reads from flash, especially
> if cache is disabled.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_SPIFFS_GC_MAX_RUNS



> Set Maximum GC Runs
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> Define maximum number of GC runs to perform to reach desired free pages.
> 
> 
> 
> Range:* from 1 to 10000
> 
> 
> 
> Default value:* 10




##### CONFIG_SPIFFS_GC_STATS



> Enable SPIFFS GC Statistics
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> Enable/disable statistics on gc. Debug/test purpose only.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_SPIFFS_PAGE_SIZE



> SPIFFS logical page size
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> Logical page size of SPIFFS partition, in bytes. Must be multiple
> of flash page size (which is usually 256 bytes).
> Larger page sizes reduce overhead when storing large files, and
> improve filesystem performance when reading large files.
> Smaller page sizes reduce overhead when storing small (< page size)
> files.
> 
> 
> 
> Range:* from 256 to 1024
> 
> 
> 
> Default value:* 256




##### CONFIG_SPIFFS_OBJ_NAME_LEN



> Set SPIFFS Maximum Name Length
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> Object name maximum length. Note that this length include the
> zero-termination character, meaning maximum string of characters
> can at most be SPIFFS_OBJ_NAME_LEN - 1.
> 
> 
> SPIFFS_OBJ_NAME_LEN + SPIFFS_META_LENGTH should not exceed
> SPIFFS_PAGE_SIZE - 64.
> 
> 
> 
> Range:* from 1 to 256
> 
> 
> 
> Default value:* 32




##### CONFIG_SPIFFS_FOLLOW_SYMLINKS



> Enable symbolic links for image creation
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> If this option is enabled, symbolic links are taken into account
> during partition image creation.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_SPIFFS_USE_MAGIC



> Enable SPIFFS Filesystem Magic
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> Enable this to have an identifiable spiffs filesystem.
> This will look for a magic in all sectors to determine if this
> is a valid spiffs system or not at mount time.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_SPIFFS_USE_MAGIC_LENGTH



> Enable SPIFFS Filesystem Length Magic
> 
> 
> *Found in:* Component config > SPIFFS Configuration > CONFIG_SPIFFS_USE_MAGIC
> 
> 
> If this option is enabled, the magic will also be dependent
> on the length of the filesystem. For example, a filesystem
> configured and formatted for 4 megabytes will not be accepted
> for mounting with a configuration defining the filesystem as 2 megabytes.
> 
> 
> 
> Default value:* Yes (enabled)





##### CONFIG_SPIFFS_META_LENGTH



> Size of per-file metadata field
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> This option sets the number of extra bytes stored in the file header.
> These bytes can be used in an application-specific manner.
> Set this to at least 4 bytes to enable support for saving file
> modification time.
> 
> 
> SPIFFS_OBJ_NAME_LEN + SPIFFS_META_LENGTH should not exceed
> SPIFFS_PAGE_SIZE - 64.
> 
> 
> 
> Default value:* 4




##### CONFIG_SPIFFS_USE_MTIME



> Save file modification time
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> If enabled, then the first 4 bytes of per-file metadata will be used
> to store file modification time (mtime), accessible through
> stat/fstat functions.
> Modification time is updated when the file is opened.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_SPIFFS_MTIME_WIDE_64_BITS



> The time field occupies 64 bits in the image instead of 32 bits
> 
> 
> *Found in:* Component config > SPIFFS Configuration
> 
> 
> If this option is not set, the time field is 32 bits (up to 2106 year),
> otherwise it is 64 bits and make sure it matches SPIFFS_META_LENGTH.
> If the chip already has the spiffs image with the time field = 32 bits
> then this option cannot be applied in this case.
> Erase it first before using this option.
> To resolve the Y2K38 problem for the spiffs, use a toolchain with
> 64-bit time_t support.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_SPIFFS_META_LENGTH >= 8




##### Debug Configuration


Contains:


* CONFIG_SPIFFS_DBG
* CONFIG_SPIFFS_API_DBG
* CONFIG_SPIFFS_CACHE_DBG
* CONFIG_SPIFFS_CHECK_DBG
* CONFIG_SPIFFS_TEST_VISUALISATION
* CONFIG_SPIFFS_GC_DBG



###### CONFIG_SPIFFS_DBG



> Enable general SPIFFS debug
> 
> 
> *Found in:* Component config > SPIFFS Configuration > Debug Configuration
> 
> 
> Enabling this option will print general debug messages to the console.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_SPIFFS_API_DBG



> Enable SPIFFS API debug
> 
> 
> *Found in:* Component config > SPIFFS Configuration > Debug Configuration
> 
> 
> Enabling this option will print API debug messages to the console.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_SPIFFS_GC_DBG



> Enable SPIFFS Garbage Cleaner debug
> 
> 
> *Found in:* Component config > SPIFFS Configuration > Debug Configuration
> 
> 
> Enabling this option will print GC debug messages to the console.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_SPIFFS_CACHE_DBG



> Enable SPIFFS Cache debug
> 
> 
> *Found in:* Component config > SPIFFS Configuration > Debug Configuration
> 
> 
> Enabling this option will print cache debug messages to the console.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_SPIFFS_CHECK_DBG



> Enable SPIFFS Filesystem Check debug
> 
> 
> *Found in:* Component config > SPIFFS Configuration > Debug Configuration
> 
> 
> Enabling this option will print Filesystem Check debug messages
> to the console.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_SPIFFS_TEST_VISUALISATION



> Enable SPIFFS Filesystem Visualization
> 
> 
> *Found in:* Component config > SPIFFS Configuration > Debug Configuration
> 
> 
> Enable this option to enable SPIFFS_vis function in the API.
> 
> 
> 
> Default value:* No (disabled)






#### TCP Transport


Contains:


* Websocket



##### Websocket


Contains:


* CONFIG_WS_TRANSPORT



###### CONFIG_WS_TRANSPORT



> Enable Websocket Transport
> 
> 
> *Found in:* Component config > TCP Transport > Websocket
> 
> 
> Enable support for creating websocket transport.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_WS_BUFFER_SIZE



> Websocket transport buffer size
> 
> 
> *Found in:* Component config > TCP Transport > Websocket > CONFIG_WS_TRANSPORT
> 
> 
> Size of the buffer used for constructing the HTTP Upgrade request during connect
> 
> 
> 
> Default value:* 1024




###### CONFIG_WS_DYNAMIC_BUFFER



> Using dynamic websocket transport buffer
> 
> 
> *Found in:* Component config > TCP Transport > Websocket > CONFIG_WS_TRANSPORT
> 
> 
> If enable this option, websocket transport buffer will be freed after connection
> succeed to save more heap.
> 
> 
> 
> Default value:* No (disabled)






#### Ultra Low Power (ULP) Co-processor


Contains:


* CONFIG_ULP_ROM_PRINT_ENABLE
* CONFIG_ULP_COPROC_ENABLED
* ULP Debugging Options
* ULP RISC-V Settings



##### CONFIG_ULP_COPROC_ENABLED



> Enable Ultra Low Power (ULP) Co-processor
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor
> 
> 
> Enable this feature if you plan to use the ULP Co-processor.
> Once this option is enabled, further ULP co-processor configuration will appear in the menu.
> 
> 
> 
> Default value:* No (disabled)



###### CONFIG_ULP_COPROC_TYPE



> ULP Co-processor type
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor > CONFIG_ULP_COPROC_ENABLED
> 
> 
> Choose the ULP Coprocessor type: ULP FSM (Finite State Machine) or ULP RISC-V.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * ULP FSM (Finite State Machine) (CONFIG_ULP_COPROC_TYPE_FSM)
> > * ULP RISC-V (CONFIG_ULP_COPROC_TYPE_RISCV)
> > * LP core RISC-V (CONFIG_ULP_COPROC_TYPE_LP_CORE)




###### CONFIG_ULP_COPROC_RESERVE_MEM



> RTC slow memory reserved for coprocessor
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor > CONFIG_ULP_COPROC_ENABLED
> 
> 
> Bytes of memory to reserve for ULP Co-processor firmware & data.
> Data is reserved at the beginning of RTC slow memory.
> 
> 
> 
> Range:* from 32 to 8176 if CONFIG_ULP_COPROC_ENABLED
> 
> 
> 
> Default value:* 4096 if CONFIG_ULP_COPROC_ENABLED





##### ULP RISC-V Settings


Contains:


* CONFIG_ULP_RISCV_UART_BAUDRATE
* CONFIG_ULP_RISCV_INTERRUPT_ENABLE
* CONFIG_ULP_RISCV_I2C_RW_TIMEOUT



###### CONFIG_ULP_RISCV_INTERRUPT_ENABLE



> Enable ULP RISC-V interrupts
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor > ULP RISC-V Settings
> 
> 
> Turn on this setting to enabled interrupts on the ULP RISC-V core.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_ULP_COPROC_TYPE_RISCV




###### CONFIG_ULP_RISCV_UART_BAUDRATE



> Baudrate used by the bitbanged ULP RISC-V UART driver
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor > ULP RISC-V Settings
> 
> 
> The accuracy of the bitbanged UART driver is limited, it is not
> recommend to increase the value above 19200.
> 
> 
> 
> Default value:* 9600 if CONFIG_ULP_COPROC_TYPE_RISCV




###### CONFIG_ULP_RISCV_I2C_RW_TIMEOUT



> Set timeout for ULP RISC-V I2C transaction timeout in ticks.
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor > ULP RISC-V Settings
> 
> 
> Set the ULP RISC-V I2C read/write timeout. Set this value to -1
> if the ULP RISC-V I2C read and write APIs should wait forever.
> Please note that the tick rate of the ULP co-processor would be
> different than the OS tick rate of the main core and therefore
> can have different timeout value depending on which core the API
> is invoked on.
> 
> 
> 
> Range:* from -1 to 4294967295 if CONFIG_ULP_COPROC_TYPE_RISCV
> 
> 
> 
> Default value:* 500 if CONFIG_ULP_COPROC_TYPE_RISCV





##### CONFIG_ULP_ROM_PRINT_ENABLE



> Enable print utilities from LP ROM
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor
> 
> 
> Set this option to enable printf functionality from LP ROM. This option
> can help reduce the LP core binary size by not linking printf functionality
> from RAM code.
> Note: For LP ROM prints to work properly, make sure that the LP core boots
> from the LP ROM.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_ULP_COPROC_TYPE_LP_CORE && ESP_ROM_HAS_LP_ROM




##### ULP Debugging Options


Contains:


* CONFIG_ULP_PANIC_OUTPUT_ENABLE
* CONFIG_ULP_HP_UART_CONSOLE_PRINT



###### CONFIG_ULP_PANIC_OUTPUT_ENABLE



> Enable panic handler which outputs over LP UART
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor > ULP Debugging Options
> 
> 
> Set this option to enable panic handler functionality. If this option is
> enabled then the LP Core will output a panic dump over LP UART,
> similar to what the main core does. Output depends on LP UART already being
> initialized and configured.
> Disabling this option will reduce the LP core binary size by not
> linking in panic handler functionality.




###### CONFIG_ULP_HP_UART_CONSOLE_PRINT



> Route lp_core_printf to the console HP-UART
> 
> 
> *Found in:* Component config > Ultra Low Power (ULP) Co-processor > ULP Debugging Options
> 
> 
> Set this option to route lp_core_printf to the console HP-UART.
> This allows you to easily view print outputs from the LP core, without
> having to connect to the LP-UART. This option comes with the following
> limitations:
> 
> 
> 1. There is no mutual exclusion between the HP-Core and the LP-Core accessing
> the HP-UART, which means that if both cores are logging heavily the output
> strings might get mangled together.
> 2. The HP-UART can only work while the HP-Core is running, which means that
> if the HP-Core is in deep sleep, the LP-Core will not be able to print to the
> console HP-UART.
> 
> 
> Due to these limitations it is only recommended to use this option for easy debugging.
> For more serious use-cases you should use the LP-UART.






#### Unity unit testing library


Contains:


* CONFIG_UNITY_ENABLE_COLOR
* CONFIG_UNITY_ENABLE_IDF_TEST_RUNNER
* CONFIG_UNITY_ENABLE_FIXTURE
* CONFIG_UNITY_ENABLE_BACKTRACE_ON_FAIL
* CONFIG_UNITY_ENABLE_64BIT
* CONFIG_UNITY_ENABLE_DOUBLE
* CONFIG_UNITY_ENABLE_FLOAT



##### CONFIG_UNITY_ENABLE_FLOAT



> Support for float type
> 
> 
> *Found in:* Component config > Unity unit testing library
> 
> 
> If not set, assertions on float arguments will not be available.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_UNITY_ENABLE_DOUBLE



> Support for double type
> 
> 
> *Found in:* Component config > Unity unit testing library
> 
> 
> If not set, assertions on double arguments will not be available.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_UNITY_ENABLE_64BIT



> Support for 64-bit integer types
> 
> 
> *Found in:* Component config > Unity unit testing library
> 
> 
> If not set, assertions on 64-bit integer types will always fail.
> If this feature is enabled, take care not to pass pointers (which are 32 bit)
> to UNITY_ASSERT_EQUAL, as that will cause pointer-to-int-cast warnings.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_UNITY_ENABLE_COLOR



> Colorize test output
> 
> 
> *Found in:* Component config > Unity unit testing library
> 
> 
> If set, Unity will colorize test results using console escape sequences.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_UNITY_ENABLE_IDF_TEST_RUNNER



> Include ESP-IDF test registration/running helpers
> 
> 
> *Found in:* Component config > Unity unit testing library
> 
> 
> If set, then the following features will be available:
> 
> 
> * TEST_CASE macro which performs automatic registration of test functions
> * Functions to run registered test functions: unity_run_all_tests,
> unity_run_tests_with_filter, unity_run_single_test_by_name.
> * Interactive menu which lists test cases and allows choosing the tests to
> be run, available via unity_run_menu function.
> 
> 
> Disable if a different test registration mechanism is used.
> 
> 
> 
> Default value:* Yes (enabled)




##### CONFIG_UNITY_ENABLE_FIXTURE



> Include Unity test fixture
> 
> 
> *Found in:* Component config > Unity unit testing library
> 
> 
> If set, unity_fixture.h header file and associated source files are part of
> the build. These provide an optional set of macros and functions to
> implement test groups.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_UNITY_ENABLE_BACKTRACE_ON_FAIL



> Print a backtrace when a unit test fails
> 
> 
> *Found in:* Component config > Unity unit testing library
> 
> 
> If set, the unity framework will print the backtrace information before
> jumping back to the test menu. The jumping is usually occurs in assert
> functions such as TEST_ASSERT, TEST_FAIL etc.
> 
> 
> 
> Default value:* No (disabled)





#### USB-OTG


Contains:


* CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK
* CONFIG_USB_HOST_HW_BUFFER_BIAS
* CONFIG_USB_HOST_CONTROL_TRANSFER_MAX_SIZE
* Root Hub configuration
* CONFIG_USB_HOST_EXT_HUB_SUPPORT



##### CONFIG_USB_HOST_CONTROL_TRANSFER_MAX_SIZE



> Largest size (in bytes) of transfers to/from default endpoints
> 
> 
> *Found in:* Component config > USB-OTG
> 
> 
> Each USB device attached is allocated a dedicated buffer for its OUT/IN transfers to/from the device's
> control endpoint. The maximum size of that buffer is determined by this option. The limited size of the
> transfer buffer have the following implications:
> - The maximum length of control transfers is limited
> - Device's with configuration descriptors larger than this limit cannot be supported
> 
> 
> 
> Default value:* 256




##### CONFIG_USB_HOST_HW_BUFFER_BIAS



> Hardware FIFO size biasing
> 
> 
> *Found in:* Component config > USB-OTG
> 
> 
> The underlying hardware has size adjustable FIFOs to cache USB packets on reception (IN) or for
> transmission (OUT). The size of these FIFOs will affect the largest MPS (maximum packet size) and the
> maximum number of packets that can be cached at any one time. The hardware contains the following
> FIFOS: RX (for all IN packets), Non-periodic TX (for Bulk and Control OUT packets), and Periodic TX
> (for Interrupt and Isochronous OUT packets). This configuration option allows biasing the FIFO sizes
> towards a particular use case, which may be necessary for devices that have endpoints with large MPS.
> The MPS limits for each biasing are listed below:
> 
> 
> Balanced:
> - IN (all transfer types), 408 bytes
> - OUT non-periodic (Bulk/Control), 192 bytes (i.e., 3 x 64 byte packets)
> - OUT periodic (Interrupt/Isochronous), 192 bytes
> 
> 
> Bias IN:
> - IN (all transfer types), 600 bytes
> - OUT non-periodic (Bulk/Control), 64 bytes (i.e., 1 x 64 byte packets)
> - OUT periodic (Interrupt/Isochronous), 128 bytes
> 
> 
> Bias Periodic OUT:
> - IN (all transfer types), 128 bytes
> - OUT non-periodic (Bulk/Control), 64 bytes (i.e., 1 x 64 byte packets)
> - OUT periodic (Interrupt/Isochronous), 600 bytes
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Balanced (CONFIG_USB_HOST_HW_BUFFER_BIAS_BALANCED)
> > * Bias IN (CONFIG_USB_HOST_HW_BUFFER_BIAS_IN)
> > * Periodic OUT (CONFIG_USB_HOST_HW_BUFFER_BIAS_PERIODIC_OUT)




##### Root Hub configuration


Contains:


* CONFIG_USB_HOST_DEBOUNCE_DELAY_MS
* CONFIG_USB_HOST_RESET_HOLD_MS
* CONFIG_USB_HOST_RESET_RECOVERY_MS
* CONFIG_USB_HOST_SET_ADDR_RECOVERY_MS



###### CONFIG_USB_HOST_DEBOUNCE_DELAY_MS



> Debounce delay in ms
> 
> 
> *Found in:* Component config > USB-OTG > Root Hub configuration
> 
> 
> On connection of a USB device, the USB 2.0 specification requires a "debounce interval with a minimum
> duration of 100ms" to allow the connection to stabilize (see USB 2.0 chapter 7.1.7.3 for more details).
> During the debounce interval, no new connection/disconnection events are registered.
> 
> 
> The default value is set to 250 ms to be safe.
> 
> 
> 
> Default value:* 250




###### CONFIG_USB_HOST_RESET_HOLD_MS



> Reset hold in ms
> 
> 
> *Found in:* Component config > USB-OTG > Root Hub configuration
> 
> 
> The reset signaling can be generated on any Hub or Host Controller port by request from the USB System
> Software. The USB 2.0 specification requires that "the reset signaling must be driven for a minimum of
> 10ms" (see USB 2.0 chapter 7.1.7.5 for more details). After the reset, the hub port will transition to
> the Enabled state (refer to Section 11.5).
> 
> 
> The default value is set to 30 ms to be safe.
> 
> 
> 
> Default value:* 30




###### CONFIG_USB_HOST_RESET_RECOVERY_MS



> Reset recovery delay in ms
> 
> 
> *Found in:* Component config > USB-OTG > Root Hub configuration
> 
> 
> After a port stops driving the reset signal, the USB 2.0 specification requires that the "USB System
> Software guarantees a minimum of 10 ms for reset recovery" before the attached device is expected to
> respond to data transfers (see USB 2.0 chapter 7.1.7.3 for more details). The device may ignore any
> data transfers during the recovery interval.
> 
> 
> The default value is set to 30 ms to be safe.
> 
> 
> 
> Default value:* 30




###### CONFIG_USB_HOST_SET_ADDR_RECOVERY_MS



> SetAddress() recovery time in ms
> 
> 
> *Found in:* Component config > USB-OTG > Root Hub configuration
> 
> 
> "After successful completion of the Status stage, the device is allowed a SetAddress() recovery
> interval of 2 ms. At the end of this interval, the device must be able to accept Setup packets
> addressed to the new address. Also, at the end of the recovery interval, the device must not respond to
> tokens sent to the old address (unless, of course, the old and new address is the same)." See USB 2.0
> chapter 9.2.6.3 for more details.
> 
> 
> The default value is set to 10 ms to be safe.
> 
> 
> 
> Default value:* 10





##### CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK



> Enable enumeration filter callback
> 
> 
> *Found in:* Component config > USB-OTG
> 
> 
> The enumeration filter callback is called before enumeration of each newly attached device. This callback
> allows users to control whether a device should be enumerated, and what configuration number to use when
> enumerating a device.
> 
> 
> If enabled, the enumeration filter callback can be set via 'usb_host_config_t' when calling
> 'usb_host_install()'.
> 
> 
> 
> Default value:* No (disabled)




##### CONFIG_USB_HOST_EXT_HUB_SUPPORT



> Support USB HUB (Experimental)
> 
> 
> *Found in:* Component config > USB-OTG
> 
> 
> Feature is under development.
> 
> 
> 
> Default value:* No (disabled) if CONFIG_IDF_EXPERIMENTAL_FEATURES





#### Virtual file system


Contains:


* CONFIG_VFS_SUPPORT_IO



##### CONFIG_VFS_SUPPORT_IO



> Provide basic I/O functions
> 
> 
> *Found in:* Component config > Virtual file system
> 
> 
> If enabled, the following functions are provided by the VFS component.
> 
> 
> open, close, read, write, pread, pwrite, lseek, fstat, fsync, ioctl, fcntl
> 
> 
> Filesystem drivers can then be registered to handle these functions
> for specific paths.
> 
> 
> Disabling this option can save memory when the support for these functions
> is not required.
> 
> 
> Note that the following functions can still be used with socket file descriptors
> when this option is disabled:
> 
> 
> close, read, write, ioctl, fcntl.
> 
> 
> 
> Default value:* Yes (enabled)



###### CONFIG_VFS_SUPPORT_DIR



> Provide directory related functions
> 
> 
> *Found in:* Component config > Virtual file system > CONFIG_VFS_SUPPORT_IO
> 
> 
> If enabled, the following functions are provided by the VFS component.
> 
> 
> stat, link, unlink, rename, utime, access, truncate, rmdir, mkdir,
> opendir, closedir, readdir, readdir_r, seekdir, telldir, rewinddir
> 
> 
> Filesystem drivers can then be registered to handle these functions
> for specific paths.
> 
> 
> Disabling this option can save memory when the support for these functions
> is not required.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_VFS_SUPPORT_SELECT



> Provide select function
> 
> 
> *Found in:* Component config > Virtual file system > CONFIG_VFS_SUPPORT_IO
> 
> 
> If enabled, select function is provided by the VFS component, and can be used
> on peripheral file descriptors (such as UART) and sockets at the same time.
> 
> 
> If disabled, the default select implementation will be provided by LWIP for
> sockets only.
> 
> 
> Disabling this option can reduce code size if support for "select" on UART file
> descriptors is not required.




###### CONFIG_VFS_SUPPRESS_SELECT_DEBUG_OUTPUT



> Suppress select() related debug outputs
> 
> 
> *Found in:* Component config > Virtual file system > CONFIG_VFS_SUPPORT_IO > CONFIG_VFS_SUPPORT_SELECT
> 
> 
> Select() related functions might produce an inconveniently lot of
> debug outputs when one sets the default log level to DEBUG or higher.
> It is possible to suppress these debug outputs by enabling this
> option.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_VFS_SELECT_IN_RAM



> Make VFS driver select() callbacks IRAM-safe
> 
> 
> *Found in:* Component config > Virtual file system > CONFIG_VFS_SUPPORT_IO > CONFIG_VFS_SUPPORT_SELECT
> 
> 
> If enabled, VFS driver select() callback function will be placed in IRAM.
> 
> 
> 
> Default value:* No (disabled)




###### CONFIG_VFS_SUPPORT_TERMIOS



> Provide termios.h functions
> 
> 
> *Found in:* Component config > Virtual file system > CONFIG_VFS_SUPPORT_IO
> 
> 
> Disabling this option can save memory when the support for termios.h is not required.
> 
> 
> 
> Default value:* Yes (enabled)




###### CONFIG_VFS_MAX_COUNT



> Maximum Number of Virtual Filesystems
> 
> 
> *Found in:* Component config > Virtual file system > CONFIG_VFS_SUPPORT_IO
> 
> 
> Define maximum number of virtual filesystems that can be registered.
> 
> 
> 
> Range:* from 1 to 20
> 
> 
> 
> Default value:* 8




###### Host File System I/O (Semihosting)


Contains:


* CONFIG_VFS_SEMIHOSTFS_MAX_MOUNT_POINTS




###### CONFIG_VFS_SEMIHOSTFS_MAX_MOUNT_POINTS



> Host FS: Maximum number of the host filesystem mount points
> 
> 
> *Found in:* Component config > Virtual file system > CONFIG_VFS_SUPPORT_IO > Host File System I/O (Semihosting)
> 
> 
> Define maximum number of host filesystem mount points.
> 
> 
> 
> Default value:* 1






#### Wear Levelling


Contains:


* CONFIG_WL_SECTOR_MODE
* CONFIG_WL_SECTOR_SIZE



##### CONFIG_WL_SECTOR_SIZE



> Wear Levelling library sector size
> 
> 
> *Found in:* Component config > Wear Levelling
> 
> 
> Sector size used by wear levelling library.
> You can set default sector size or size that will
> fit to the flash device sector size.
> 
> 
> With sector size set to 4096 bytes, wear levelling library is more
> efficient. However if FAT filesystem is used on top of wear levelling
> library, it will need more temporary storage: 4096 bytes for each
> mounted filesystem and 4096 bytes for each opened file.
> 
> 
> With sector size set to 512 bytes, wear levelling library will perform
> more operations with flash memory, but less RAM will be used by FAT
> filesystem library (512 bytes for the filesystem and 512 bytes for each
> file opened).
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * 512 (CONFIG_WL_SECTOR_SIZE_512)
> > * 4096 (CONFIG_WL_SECTOR_SIZE_4096)




##### CONFIG_WL_SECTOR_MODE



> Sector store mode
> 
> 
> *Found in:* Component config > Wear Levelling
> 
> 
> Specify the mode to store data into flash:
> 
> 
> * In Performance mode a data will be stored to the RAM and then
> stored back to the flash. Compared to the Safety mode, this operation is
> faster, but if power will be lost when erase sector operation is in
> progress, then the data from complete flash device sector will be lost.
> * In Safety mode data from complete flash device sector will be read from
> flash, modified, and then stored back to flash.
> Compared to the Performance mode, this operation is slower, but if
> power is lost during erase sector operation, then the data from full
> flash device sector will not be lost.
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * Performance (CONFIG_WL_SECTOR_MODE_PERF)
> > * Safety (CONFIG_WL_SECTOR_MODE_SAFE)





#### Wi-Fi Provisioning Manager


Contains:


* CONFIG_WIFI_PROV_BLE_BONDING
* CONFIG_WIFI_PROV_BLE_SEC_CONN
* CONFIG_WIFI_PROV_BLE_FORCE_ENCRYPTION
* CONFIG_WIFI_PROV_KEEP_BLE_ON_AFTER_PROV
* CONFIG_WIFI_PROV_SCAN_MAX_ENTRIES
* CONFIG_WIFI_PROV_AUTOSTOP_TIMEOUT
* CONFIG_WIFI_PROV_STA_SCAN_METHOD



##### CONFIG_WIFI_PROV_SCAN_MAX_ENTRIES



> Max Wi-Fi Scan Result Entries
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager
> 
> 
> This sets the maximum number of entries of Wi-Fi scan results that will be kept by the provisioning manager
> 
> 
> 
> Range:* from 1 to 255
> 
> 
> 
> Default value:* 16




##### CONFIG_WIFI_PROV_AUTOSTOP_TIMEOUT



> Provisioning auto-stop timeout
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager
> 
> 
> Time (in seconds) after which the Wi-Fi provisioning manager will auto-stop after connecting to
> a Wi-Fi network successfully.
> 
> 
> 
> Range:* from 5 to 600
> 
> 
> 
> Default value:* 30




##### CONFIG_WIFI_PROV_BLE_BONDING



> Enable BLE bonding
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager
> 
> 
> This option is applicable only when provisioning transport is BLE.




##### CONFIG_WIFI_PROV_BLE_SEC_CONN



> Enable BLE Secure connection flag
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager
> 
> 
> Used to enable Secure connection support when provisioning transport is BLE.
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_BT_NIMBLE_ENABLED




##### CONFIG_WIFI_PROV_BLE_FORCE_ENCRYPTION



> Force Link Encryption during characteristic Read / Write
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager
> 
> 
> Used to enforce link encryption when attempting to read / write characteristic




##### CONFIG_WIFI_PROV_KEEP_BLE_ON_AFTER_PROV



> Keep BT on after provisioning is done
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager



###### CONFIG_WIFI_PROV_DISCONNECT_AFTER_PROV



> Terminate connection after provisioning is done
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager > CONFIG_WIFI_PROV_KEEP_BLE_ON_AFTER_PROV
> 
> 
> 
> Default value:* Yes (enabled) if CONFIG_WIFI_PROV_KEEP_BLE_ON_AFTER_PROV





##### CONFIG_WIFI_PROV_STA_SCAN_METHOD



> Wifi Provisioning Scan Method
> 
> 
> *Found in:* Component config > Wi-Fi Provisioning Manager
> 
> 
> Available options:
> 
> 
> 
> > > 
> > 
> > 
> > * All Channel Scan (CONFIG_WIFI_PROV_STA_ALL_CHANNEL_SCAN)
> > 
> > 
> > Scan will end after scanning the entire channel. This option is useful in Mesh WiFi Systems.
> > * Fast Scan (CONFIG_WIFI_PROV_STA_FAST_SCAN)
> > 
> > 
> > Scan will end after an AP matching with the SSID has been detected.






### CONFIG_IDF_EXPERIMENTAL_FEATURES



> Make experimental features visible
> 
> 
> *Found in:*
> 
> 
> By enabling this option, ESP-IDF experimental feature options will be visible.
> 
> 
> Note you should still enable a certain experimental feature option to use it, and you
> should read the corresponding risk warning and known issue list carefully.
> 
> 
> Current experimental feature list:
> 
> 
> * CONFIG_ESPTOOLPY_FLASHFREQ_120M && CONFIG_ESPTOOLPY_FLASH_SAMPLE_MODE_DTR
> * CONFIG_SPIRAM_SPEED_120M && CONFIG_SPIRAM_MODE_OCT
> * CONFIG_BOOTLOADER_CACHE_32BIT_ADDR_QUAD_FLASH
> * CONFIG_MBEDTLS_USE_CRYPTO_ROM_IMPL
> * CONFIG_ESP_WIFI_EAP_TLS1_3
> * CONFIG_ESP_WIFI_ENABLE_ROAMING_APP
> 
> 
> 
> Default value:* No (disabled)




### Deprecated options and their replacements


* CONFIG_A2DP_ENABLE (CONFIG_BT_A2DP_ENABLE)
* CONFIG_A2D_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_A2D_TRACE_LEVEL)
	+ CONFIG_A2D_TRACE_LEVEL_NONE
	+ CONFIG_A2D_TRACE_LEVEL_ERROR
	+ CONFIG_A2D_TRACE_LEVEL_WARNING
	+ CONFIG_A2D_TRACE_LEVEL_API
	+ CONFIG_A2D_TRACE_LEVEL_EVENT
	+ CONFIG_A2D_TRACE_LEVEL_DEBUG
	+ CONFIG_A2D_TRACE_LEVEL_VERBOSE
* CONFIG_ADC2_DISABLE_DAC (CONFIG_ADC_DISABLE_DAC)
* CONFIG_APPL_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_APPL_TRACE_LEVEL)
	+ CONFIG_APPL_TRACE_LEVEL_NONE
	+ CONFIG_APPL_TRACE_LEVEL_ERROR
	+ CONFIG_APPL_TRACE_LEVEL_WARNING
	+ CONFIG_APPL_TRACE_LEVEL_API
	+ CONFIG_APPL_TRACE_LEVEL_EVENT
	+ CONFIG_APPL_TRACE_LEVEL_DEBUG
	+ CONFIG_APPL_TRACE_LEVEL_VERBOSE
* CONFIG_APP_ANTI_ROLLBACK (CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK)
* CONFIG_APP_ROLLBACK_ENABLE (CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE)
* CONFIG_APP_SECURE_VERSION (CONFIG_BOOTLOADER_APP_SECURE_VERSION)
* CONFIG_APP_SECURE_VERSION_SIZE_EFUSE_FIELD (CONFIG_BOOTLOADER_APP_SEC_VER_SIZE_EFUSE_FIELD)
* CONFIG_AVCT_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_AVCT_TRACE_LEVEL)
	+ CONFIG_AVCT_TRACE_LEVEL_NONE
	+ CONFIG_AVCT_TRACE_LEVEL_ERROR
	+ CONFIG_AVCT_TRACE_LEVEL_WARNING
	+ CONFIG_AVCT_TRACE_LEVEL_API
	+ CONFIG_AVCT_TRACE_LEVEL_EVENT
	+ CONFIG_AVCT_TRACE_LEVEL_DEBUG
	+ CONFIG_AVCT_TRACE_LEVEL_VERBOSE
* CONFIG_AVDT_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_AVDT_TRACE_LEVEL)
	+ CONFIG_AVDT_TRACE_LEVEL_NONE
	+ CONFIG_AVDT_TRACE_LEVEL_ERROR
	+ CONFIG_AVDT_TRACE_LEVEL_WARNING
	+ CONFIG_AVDT_TRACE_LEVEL_API
	+ CONFIG_AVDT_TRACE_LEVEL_EVENT
	+ CONFIG_AVDT_TRACE_LEVEL_DEBUG
	+ CONFIG_AVDT_TRACE_LEVEL_VERBOSE
* CONFIG_AVRC_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_AVRC_TRACE_LEVEL)
	+ CONFIG_AVRC_TRACE_LEVEL_NONE
	+ CONFIG_AVRC_TRACE_LEVEL_ERROR
	+ CONFIG_AVRC_TRACE_LEVEL_WARNING
	+ CONFIG_AVRC_TRACE_LEVEL_API
	+ CONFIG_AVRC_TRACE_LEVEL_EVENT
	+ CONFIG_AVRC_TRACE_LEVEL_DEBUG
	+ CONFIG_AVRC_TRACE_LEVEL_VERBOSE
* CONFIG_BLE_ACTIVE_SCAN_REPORT_ADV_SCAN_RSP_INDIVIDUALLY (CONFIG_BT_BLE_ACT_SCAN_REP_ADV_SCAN)
* CONFIG_BLE_ESTABLISH_LINK_CONNECTION_TIMEOUT (CONFIG_BT_BLE_ESTAB_LINK_CONN_TOUT)
* CONFIG_BLE_HOST_QUEUE_CONGESTION_CHECK (CONFIG_BT_BLE_HOST_QUEUE_CONG_CHECK)
* CONFIG_BLE_MESH_GATT_PROXY (CONFIG_BLE_MESH_GATT_PROXY_SERVER)
* CONFIG_BLE_SMP_ENABLE (CONFIG_BT_BLE_SMP_ENABLE)
* CONFIG_BLUEDROID_MEM_DEBUG (CONFIG_BT_BLUEDROID_MEM_DEBUG)
* CONFIG_BLUEDROID_PINNED_TO_CORE_CHOICE (CONFIG_BT_BLUEDROID_PINNED_TO_CORE_CHOICE)
	+ CONFIG_BLUEDROID_PINNED_TO_CORE_0
	+ CONFIG_BLUEDROID_PINNED_TO_CORE_1
* CONFIG_BLUFI_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_BLUFI_TRACE_LEVEL)
	+ CONFIG_BLUFI_TRACE_LEVEL_NONE
	+ CONFIG_BLUFI_TRACE_LEVEL_ERROR
	+ CONFIG_BLUFI_TRACE_LEVEL_WARNING
	+ CONFIG_BLUFI_TRACE_LEVEL_API
	+ CONFIG_BLUFI_TRACE_LEVEL_EVENT
	+ CONFIG_BLUFI_TRACE_LEVEL_DEBUG
	+ CONFIG_BLUFI_TRACE_LEVEL_VERBOSE
* CONFIG_BNEP_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_BNEP_TRACE_LEVEL)
* CONFIG_BROWNOUT_DET (CONFIG_ESP_BROWNOUT_DET)
* CONFIG_BROWNOUT_DET_LVL_SEL (CONFIG_ESP_BROWNOUT_DET_LVL_SEL)
	+ CONFIG_BROWNOUT_DET_LVL_SEL_7
	+ CONFIG_BROWNOUT_DET_LVL_SEL_6
	+ CONFIG_BROWNOUT_DET_LVL_SEL_5
	+ CONFIG_BROWNOUT_DET_LVL_SEL_4
	+ CONFIG_BROWNOUT_DET_LVL_SEL_3
	+ CONFIG_BROWNOUT_DET_LVL_SEL_2
	+ CONFIG_BROWNOUT_DET_LVL_SEL_1
* CONFIG_BTC_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_BTC_TRACE_LEVEL)
	+ CONFIG_BTC_TRACE_LEVEL_NONE
	+ CONFIG_BTC_TRACE_LEVEL_ERROR
	+ CONFIG_BTC_TRACE_LEVEL_WARNING
	+ CONFIG_BTC_TRACE_LEVEL_API
	+ CONFIG_BTC_TRACE_LEVEL_EVENT
	+ CONFIG_BTC_TRACE_LEVEL_DEBUG
	+ CONFIG_BTC_TRACE_LEVEL_VERBOSE
* CONFIG_BTC_TASK_STACK_SIZE (CONFIG_BT_BTC_TASK_STACK_SIZE)
* CONFIG_BTH_LOG_SDP_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_SDP_TRACE_LEVEL)
	+ CONFIG_SDP_TRACE_LEVEL_NONE
	+ CONFIG_SDP_TRACE_LEVEL_ERROR
	+ CONFIG_SDP_TRACE_LEVEL_WARNING
	+ CONFIG_SDP_TRACE_LEVEL_API
	+ CONFIG_SDP_TRACE_LEVEL_EVENT
	+ CONFIG_SDP_TRACE_LEVEL_DEBUG
	+ CONFIG_SDP_TRACE_LEVEL_VERBOSE
* CONFIG_BTIF_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_BTIF_TRACE_LEVEL)
	+ CONFIG_BTIF_TRACE_LEVEL_NONE
	+ CONFIG_BTIF_TRACE_LEVEL_ERROR
	+ CONFIG_BTIF_TRACE_LEVEL_WARNING
	+ CONFIG_BTIF_TRACE_LEVEL_API
	+ CONFIG_BTIF_TRACE_LEVEL_EVENT
	+ CONFIG_BTIF_TRACE_LEVEL_DEBUG
	+ CONFIG_BTIF_TRACE_LEVEL_VERBOSE
* CONFIG_BTM_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_BTM_TRACE_LEVEL)
	+ CONFIG_BTM_TRACE_LEVEL_NONE
	+ CONFIG_BTM_TRACE_LEVEL_ERROR
	+ CONFIG_BTM_TRACE_LEVEL_WARNING
	+ CONFIG_BTM_TRACE_LEVEL_API
	+ CONFIG_BTM_TRACE_LEVEL_EVENT
	+ CONFIG_BTM_TRACE_LEVEL_DEBUG
	+ CONFIG_BTM_TRACE_LEVEL_VERBOSE
* CONFIG_BTU_TASK_STACK_SIZE (CONFIG_BT_BTU_TASK_STACK_SIZE)
* CONFIG_BT_NIMBLE_ACL_BUF_COUNT (CONFIG_BT_NIMBLE_TRANSPORT_ACL_FROM_LL_COUNT)
* CONFIG_BT_NIMBLE_ACL_BUF_SIZE (CONFIG_BT_NIMBLE_TRANSPORT_ACL_SIZE)
* CONFIG_BT_NIMBLE_HCI_EVT_BUF_SIZE (CONFIG_BT_NIMBLE_TRANSPORT_EVT_SIZE)
* CONFIG_BT_NIMBLE_HCI_EVT_HI_BUF_COUNT (CONFIG_BT_NIMBLE_TRANSPORT_EVT_COUNT)
* CONFIG_BT_NIMBLE_HCI_EVT_LO_BUF_COUNT (CONFIG_BT_NIMBLE_TRANSPORT_EVT_DISCARD_COUNT)
* CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT (CONFIG_BT_NIMBLE_MSYS_1_BLOCK_COUNT)
* CONFIG_BT_NIMBLE_SM_SC_LVL (CONFIG_BT_NIMBLE_SM_LVL)
* CONFIG_BT_NIMBLE_TASK_STACK_SIZE (CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE)
* CONFIG_CLASSIC_BT_ENABLED (CONFIG_BT_CLASSIC_ENABLED)
* CONFIG_CONSOLE_UART (CONFIG_ESP_CONSOLE_UART)
	+ CONFIG_CONSOLE_UART_DEFAULT
	+ CONFIG_CONSOLE_UART_CUSTOM
	+ CONFIG_CONSOLE_UART_NONE, CONFIG_ESP_CONSOLE_UART_NONE
* CONFIG_CONSOLE_UART_BAUDRATE (CONFIG_ESP_CONSOLE_UART_BAUDRATE)
* CONFIG_CONSOLE_UART_NUM (CONFIG_ESP_CONSOLE_UART_NUM)
	+ CONFIG_CONSOLE_UART_CUSTOM_NUM_0
	+ CONFIG_CONSOLE_UART_CUSTOM_NUM_1
* CONFIG_CONSOLE_UART_RX_GPIO (CONFIG_ESP_CONSOLE_UART_RX_GPIO)
* CONFIG_CONSOLE_UART_TX_GPIO (CONFIG_ESP_CONSOLE_UART_TX_GPIO)
* CONFIG_CXX_EXCEPTIONS (CONFIG_COMPILER_CXX_EXCEPTIONS)
* CONFIG_CXX_EXCEPTIONS_EMG_POOL_SIZE (CONFIG_COMPILER_CXX_EXCEPTIONS_EMG_POOL_SIZE)
* CONFIG_EFUSE_SECURE_VERSION_EMULATE (CONFIG_BOOTLOADER_EFUSE_SECURE_VERSION_EMULATE)
* CONFIG_ENABLE_STATIC_TASK_CLEAN_UP_HOOK (CONFIG_FREERTOS_ENABLE_STATIC_TASK_CLEAN_UP)
* CONFIG_ESP32_APPTRACE_ONPANIC_HOST_FLUSH_TMO (CONFIG_APPTRACE_ONPANIC_HOST_FLUSH_TMO)
* CONFIG_ESP32_APPTRACE_PENDING_DATA_SIZE_MAX (CONFIG_APPTRACE_PENDING_DATA_SIZE_MAX)
* CONFIG_ESP32_APPTRACE_POSTMORTEM_FLUSH_TRAX_THRESH (CONFIG_APPTRACE_POSTMORTEM_FLUSH_THRESH)
* CONFIG_ESP32_CORE_DUMP_DECODE (CONFIG_ESP_COREDUMP_DECODE)
	+ CONFIG_ESP32_CORE_DUMP_DECODE_INFO
	+ CONFIG_ESP32_CORE_DUMP_DECODE_DISABLE
* CONFIG_ESP32_CORE_DUMP_MAX_TASKS_NUM (CONFIG_ESP_COREDUMP_MAX_TASKS_NUM)
* CONFIG_ESP32_CORE_DUMP_STACK_SIZE (CONFIG_ESP_COREDUMP_STACK_SIZE)
* CONFIG_ESP32_CORE_DUMP_UART_DELAY (CONFIG_ESP_COREDUMP_UART_DELAY)
* CONFIG_ESP32_DEBUG_STUBS_ENABLE (CONFIG_ESP_DEBUG_STUBS_ENABLE)
* CONFIG_ESP32_GCOV_ENABLE (CONFIG_APPTRACE_GCOV_ENABLE)
* CONFIG_ESP32_PHY_CALIBRATION_AND_DATA_STORAGE (CONFIG_ESP_PHY_CALIBRATION_AND_DATA_STORAGE)
* CONFIG_ESP32_PHY_DEFAULT_INIT_IF_INVALID (CONFIG_ESP_PHY_DEFAULT_INIT_IF_INVALID)
* CONFIG_ESP32_PHY_INIT_DATA_ERROR (CONFIG_ESP_PHY_INIT_DATA_ERROR)
* CONFIG_ESP32_PHY_INIT_DATA_IN_PARTITION (CONFIG_ESP_PHY_INIT_DATA_IN_PARTITION)
* CONFIG_ESP32_PHY_MAC_BB_PD (CONFIG_ESP_PHY_MAC_BB_PD)
* CONFIG_ESP32_PHY_MAX_WIFI_TX_POWER (CONFIG_ESP_PHY_MAX_WIFI_TX_POWER)
* CONFIG_ESP32_PTHREAD_STACK_MIN (CONFIG_PTHREAD_STACK_MIN)
* CONFIG_ESP32_PTHREAD_TASK_CORE_DEFAULT (CONFIG_PTHREAD_TASK_CORE_DEFAULT)
	+ CONFIG_ESP32_DEFAULT_PTHREAD_CORE_NO_AFFINITY
	+ CONFIG_ESP32_DEFAULT_PTHREAD_CORE_0
	+ CONFIG_ESP32_DEFAULT_PTHREAD_CORE_1
* CONFIG_ESP32_PTHREAD_TASK_NAME_DEFAULT (CONFIG_PTHREAD_TASK_NAME_DEFAULT)
* CONFIG_ESP32_PTHREAD_TASK_PRIO_DEFAULT (CONFIG_PTHREAD_TASK_PRIO_DEFAULT)
* CONFIG_ESP32_PTHREAD_TASK_STACK_SIZE_DEFAULT (CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT)
* CONFIG_ESP32_REDUCE_PHY_TX_POWER (CONFIG_ESP_PHY_REDUCE_TX_POWER)
* CONFIG_ESP32_RTC_XTAL_BOOTSTRAP_CYCLES (CONFIG_ESP_SYSTEM_RTC_EXT_XTAL_BOOTSTRAP_CYCLES)
* CONFIG_ESP32_SUPPORT_MULTIPLE_PHY_INIT_DATA_BIN (CONFIG_ESP_PHY_MULTIPLE_INIT_DATA_BIN)
* CONFIG_ESP32_WIFI_AMPDU_RX_ENABLED (CONFIG_ESP_WIFI_AMPDU_RX_ENABLED)
* CONFIG_ESP32_WIFI_AMPDU_TX_ENABLED (CONFIG_ESP_WIFI_AMPDU_TX_ENABLED)
* CONFIG_ESP32_WIFI_AMSDU_TX_ENABLED (CONFIG_ESP_WIFI_AMSDU_TX_ENABLED)
* CONFIG_ESP32_WIFI_CACHE_TX_BUFFER_NUM (CONFIG_ESP_WIFI_CACHE_TX_BUFFER_NUM)
* CONFIG_ESP32_WIFI_CSI_ENABLED (CONFIG_ESP_WIFI_CSI_ENABLED)
* CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM (CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM)
* CONFIG_ESP32_WIFI_DYNAMIC_TX_BUFFER_NUM (CONFIG_ESP_WIFI_DYNAMIC_TX_BUFFER_NUM)
* CONFIG_ESP32_WIFI_ENABLE_WPA3_OWE_STA (CONFIG_ESP_WIFI_ENABLE_WPA3_OWE_STA)
* CONFIG_ESP32_WIFI_ENABLE_WPA3_SAE (CONFIG_ESP_WIFI_ENABLE_WPA3_SAE)
* CONFIG_ESP32_WIFI_IRAM_OPT (CONFIG_ESP_WIFI_IRAM_OPT)
* CONFIG_ESP32_WIFI_MGMT_SBUF_NUM (CONFIG_ESP_WIFI_MGMT_SBUF_NUM)
* CONFIG_ESP32_WIFI_NVS_ENABLED (CONFIG_ESP_WIFI_NVS_ENABLED)
* CONFIG_ESP32_WIFI_RX_BA_WIN (CONFIG_ESP_WIFI_RX_BA_WIN)
* CONFIG_ESP32_WIFI_RX_IRAM_OPT (CONFIG_ESP_WIFI_RX_IRAM_OPT)
* CONFIG_ESP32_WIFI_SOFTAP_BEACON_MAX_LEN (CONFIG_ESP_WIFI_SOFTAP_BEACON_MAX_LEN)
* CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM (CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM)
* CONFIG_ESP32_WIFI_STATIC_TX_BUFFER_NUM (CONFIG_ESP_WIFI_STATIC_TX_BUFFER_NUM)
* CONFIG_ESP32_WIFI_SW_COEXIST_ENABLE (CONFIG_ESP_COEX_SW_COEXIST_ENABLE)
* CONFIG_ESP32_WIFI_TASK_CORE_ID (CONFIG_ESP_WIFI_TASK_CORE_ID)
	+ CONFIG_ESP32_WIFI_TASK_PINNED_TO_CORE_0
	+ CONFIG_ESP32_WIFI_TASK_PINNED_TO_CORE_1
* CONFIG_ESP32_WIFI_TX_BA_WIN (CONFIG_ESP_WIFI_TX_BA_WIN)
* CONFIG_ESP32_WIFI_TX_BUFFER (CONFIG_ESP_WIFI_TX_BUFFER)
	+ CONFIG_ESP32_WIFI_STATIC_TX_BUFFER
	+ CONFIG_ESP32_WIFI_DYNAMIC_TX_BUFFER
* CONFIG_ESP_GRATUITOUS_ARP (CONFIG_LWIP_ESP_GRATUITOUS_ARP)
* CONFIG_ESP_SYSTEM_PD_FLASH (CONFIG_ESP_SLEEP_POWER_DOWN_FLASH)
* CONFIG_ESP_SYSTEM_PM_POWER_DOWN_CPU (CONFIG_PM_POWER_DOWN_CPU_IN_LIGHT_SLEEP)
* CONFIG_ESP_TASK_WDT (CONFIG_ESP_TASK_WDT_INIT)
* CONFIG_ESP_WIFI_EXTERNAL_COEXIST_ENABLE (CONFIG_ESP_COEX_EXTERNAL_COEXIST_ENABLE)
* CONFIG_ESP_WIFI_SW_COEXIST_ENABLE (CONFIG_ESP_COEX_SW_COEXIST_ENABLE)
* CONFIG_EVENT_LOOP_PROFILING (CONFIG_ESP_EVENT_LOOP_PROFILING)
* CONFIG_EXTERNAL_COEX_ENABLE (CONFIG_ESP_COEX_EXTERNAL_COEXIST_ENABLE)
* CONFIG_FLASH_ENCRYPTION_ENABLED (CONFIG_SECURE_FLASH_ENC_ENABLED)
* CONFIG_FLASH_ENCRYPTION_UART_BOOTLOADER_ALLOW_CACHE (CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_CACHE)
* CONFIG_FLASH_ENCRYPTION_UART_BOOTLOADER_ALLOW_ENCRYPT (CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_ENC)
* CONFIG_GAP_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_GAP_TRACE_LEVEL)
	+ CONFIG_GAP_TRACE_LEVEL_NONE
	+ CONFIG_GAP_TRACE_LEVEL_ERROR
	+ CONFIG_GAP_TRACE_LEVEL_WARNING
	+ CONFIG_GAP_TRACE_LEVEL_API
	+ CONFIG_GAP_TRACE_LEVEL_EVENT
	+ CONFIG_GAP_TRACE_LEVEL_DEBUG
	+ CONFIG_GAP_TRACE_LEVEL_VERBOSE
* CONFIG_GARP_TMR_INTERVAL (CONFIG_LWIP_GARP_TMR_INTERVAL)
* CONFIG_GATTC_CACHE_NVS_FLASH (CONFIG_BT_GATTC_CACHE_NVS_FLASH)
* CONFIG_GATTC_ENABLE (CONFIG_BT_GATTC_ENABLE)
* CONFIG_GATTS_ENABLE (CONFIG_BT_GATTS_ENABLE)
* CONFIG_GATTS_SEND_SERVICE_CHANGE_MODE (CONFIG_BT_GATTS_SEND_SERVICE_CHANGE_MODE)
	+ CONFIG_GATTS_SEND_SERVICE_CHANGE_MANUAL
	+ CONFIG_GATTS_SEND_SERVICE_CHANGE_AUTO
* CONFIG_GATT_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_GATT_TRACE_LEVEL)
	+ CONFIG_GATT_TRACE_LEVEL_NONE
	+ CONFIG_GATT_TRACE_LEVEL_ERROR
	+ CONFIG_GATT_TRACE_LEVEL_WARNING
	+ CONFIG_GATT_TRACE_LEVEL_API
	+ CONFIG_GATT_TRACE_LEVEL_EVENT
	+ CONFIG_GATT_TRACE_LEVEL_DEBUG
	+ CONFIG_GATT_TRACE_LEVEL_VERBOSE
* CONFIG_GDBSTUB_MAX_TASKS (CONFIG_ESP_GDBSTUB_MAX_TASKS)
* CONFIG_GDBSTUB_SUPPORT_TASKS (CONFIG_ESP_GDBSTUB_SUPPORT_TASKS)
* CONFIG_HCI_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_HCI_TRACE_LEVEL)
	+ CONFIG_HCI_TRACE_LEVEL_NONE
	+ CONFIG_HCI_TRACE_LEVEL_ERROR
	+ CONFIG_HCI_TRACE_LEVEL_WARNING
	+ CONFIG_HCI_TRACE_LEVEL_API
	+ CONFIG_HCI_TRACE_LEVEL_EVENT
	+ CONFIG_HCI_TRACE_LEVEL_DEBUG
	+ CONFIG_HCI_TRACE_LEVEL_VERBOSE
* CONFIG_HFP_AG_ENABLE (CONFIG_BT_HFP_AG_ENABLE)
* CONFIG_HFP_AUDIO_DATA_PATH (CONFIG_BT_HFP_AUDIO_DATA_PATH)
	+ CONFIG_HFP_AUDIO_DATA_PATH_PCM
	+ CONFIG_HFP_AUDIO_DATA_PATH_HCI
* CONFIG_HFP_CLIENT_ENABLE (CONFIG_BT_HFP_CLIENT_ENABLE)
* CONFIG_HFP_ENABLE (CONFIG_BT_HFP_ENABLE)
* CONFIG_HID_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_HID_TRACE_LEVEL)
	+ CONFIG_HID_TRACE_LEVEL_NONE
	+ CONFIG_HID_TRACE_LEVEL_ERROR
	+ CONFIG_HID_TRACE_LEVEL_WARNING
	+ CONFIG_HID_TRACE_LEVEL_API
	+ CONFIG_HID_TRACE_LEVEL_EVENT
	+ CONFIG_HID_TRACE_LEVEL_DEBUG
	+ CONFIG_HID_TRACE_LEVEL_VERBOSE
* CONFIG_INT_WDT (CONFIG_ESP_INT_WDT)
* CONFIG_INT_WDT_CHECK_CPU1 (CONFIG_ESP_INT_WDT_CHECK_CPU1)
* CONFIG_INT_WDT_TIMEOUT_MS (CONFIG_ESP_INT_WDT_TIMEOUT_MS)
* CONFIG_IPC_TASK_STACK_SIZE (CONFIG_ESP_IPC_TASK_STACK_SIZE)
* CONFIG_L2CAP_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_L2CAP_TRACE_LEVEL)
	+ CONFIG_L2CAP_TRACE_LEVEL_NONE
	+ CONFIG_L2CAP_TRACE_LEVEL_ERROR
	+ CONFIG_L2CAP_TRACE_LEVEL_WARNING
	+ CONFIG_L2CAP_TRACE_LEVEL_API
	+ CONFIG_L2CAP_TRACE_LEVEL_EVENT
	+ CONFIG_L2CAP_TRACE_LEVEL_DEBUG
	+ CONFIG_L2CAP_TRACE_LEVEL_VERBOSE
* CONFIG_L2_TO_L3_COPY (CONFIG_LWIP_L2_TO_L3_COPY)
* CONFIG_LOG_BOOTLOADER_LEVEL (CONFIG_BOOTLOADER_LOG_LEVEL)
	+ CONFIG_LOG_BOOTLOADER_LEVEL_NONE
	+ CONFIG_LOG_BOOTLOADER_LEVEL_ERROR
	+ CONFIG_LOG_BOOTLOADER_LEVEL_WARN
	+ CONFIG_LOG_BOOTLOADER_LEVEL_INFO
	+ CONFIG_LOG_BOOTLOADER_LEVEL_DEBUG
	+ CONFIG_LOG_BOOTLOADER_LEVEL_VERBOSE
* CONFIG_MAC_BB_PD (CONFIG_ESP_PHY_MAC_BB_PD)
* CONFIG_MAIN_TASK_STACK_SIZE (CONFIG_ESP_MAIN_TASK_STACK_SIZE)
* CONFIG_MCA_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_MCA_TRACE_LEVEL)
	+ CONFIG_MCA_TRACE_LEVEL_NONE
	+ CONFIG_MCA_TRACE_LEVEL_ERROR
	+ CONFIG_MCA_TRACE_LEVEL_WARNING
	+ CONFIG_MCA_TRACE_LEVEL_API
	+ CONFIG_MCA_TRACE_LEVEL_EVENT
	+ CONFIG_MCA_TRACE_LEVEL_DEBUG
	+ CONFIG_MCA_TRACE_LEVEL_VERBOSE
* CONFIG_MCPWM_ISR_IN_IRAM (CONFIG_MCPWM_ISR_IRAM_SAFE)
* CONFIG_NIMBLE_ATT_PREFERRED_MTU (CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU)
* CONFIG_NIMBLE_CRYPTO_STACK_MBEDTLS (CONFIG_BT_NIMBLE_CRYPTO_STACK_MBEDTLS)
* CONFIG_NIMBLE_DEBUG (CONFIG_BT_NIMBLE_DEBUG)
* CONFIG_NIMBLE_GAP_DEVICE_NAME_MAX_LEN (CONFIG_BT_NIMBLE_GAP_DEVICE_NAME_MAX_LEN)
* CONFIG_NIMBLE_HS_FLOW_CTRL (CONFIG_BT_NIMBLE_HS_FLOW_CTRL)
* CONFIG_NIMBLE_HS_FLOW_CTRL_ITVL (CONFIG_BT_NIMBLE_HS_FLOW_CTRL_ITVL)
* CONFIG_NIMBLE_HS_FLOW_CTRL_THRESH (CONFIG_BT_NIMBLE_HS_FLOW_CTRL_THRESH)
* CONFIG_NIMBLE_HS_FLOW_CTRL_TX_ON_DISCONNECT (CONFIG_BT_NIMBLE_HS_FLOW_CTRL_TX_ON_DISCONNECT)
* CONFIG_NIMBLE_L2CAP_COC_MAX_NUM (CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM)
* CONFIG_NIMBLE_MAX_BONDS (CONFIG_BT_NIMBLE_MAX_BONDS)
* CONFIG_NIMBLE_MAX_CCCDS (CONFIG_BT_NIMBLE_MAX_CCCDS)
* CONFIG_NIMBLE_MAX_CONNECTIONS (CONFIG_BT_NIMBLE_MAX_CONNECTIONS)
* CONFIG_NIMBLE_MEM_ALLOC_MODE (CONFIG_BT_NIMBLE_MEM_ALLOC_MODE)
	+ CONFIG_NIMBLE_MEM_ALLOC_MODE_INTERNAL
	+ CONFIG_NIMBLE_MEM_ALLOC_MODE_EXTERNAL
	+ CONFIG_NIMBLE_MEM_ALLOC_MODE_DEFAULT
* CONFIG_NIMBLE_MESH (CONFIG_BT_NIMBLE_MESH)
* CONFIG_NIMBLE_MESH_DEVICE_NAME (CONFIG_BT_NIMBLE_MESH_DEVICE_NAME)
* CONFIG_NIMBLE_MESH_FRIEND (CONFIG_BT_NIMBLE_MESH_FRIEND)
* CONFIG_NIMBLE_MESH_GATT_PROXY (CONFIG_BT_NIMBLE_MESH_GATT_PROXY)
* CONFIG_NIMBLE_MESH_LOW_POWER (CONFIG_BT_NIMBLE_MESH_LOW_POWER)
* CONFIG_NIMBLE_MESH_PB_ADV (CONFIG_BT_NIMBLE_MESH_PB_ADV)
* CONFIG_NIMBLE_MESH_PB_GATT (CONFIG_BT_NIMBLE_MESH_PB_GATT)
* CONFIG_NIMBLE_MESH_PROV (CONFIG_BT_NIMBLE_MESH_PROV)
* CONFIG_NIMBLE_MESH_PROXY (CONFIG_BT_NIMBLE_MESH_PROXY)
* CONFIG_NIMBLE_MESH_RELAY (CONFIG_BT_NIMBLE_MESH_RELAY)
* CONFIG_NIMBLE_NVS_PERSIST (CONFIG_BT_NIMBLE_NVS_PERSIST)
* CONFIG_NIMBLE_PINNED_TO_CORE_CHOICE (CONFIG_BT_NIMBLE_PINNED_TO_CORE_CHOICE)
	+ CONFIG_NIMBLE_PINNED_TO_CORE_0
	+ CONFIG_NIMBLE_PINNED_TO_CORE_1
* CONFIG_NIMBLE_ROLE_BROADCASTER (CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
* CONFIG_NIMBLE_ROLE_CENTRAL (CONFIG_BT_NIMBLE_ROLE_CENTRAL)
* CONFIG_NIMBLE_ROLE_OBSERVER (CONFIG_BT_NIMBLE_ROLE_OBSERVER)
* CONFIG_NIMBLE_ROLE_PERIPHERAL (CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
* CONFIG_NIMBLE_RPA_TIMEOUT (CONFIG_BT_NIMBLE_RPA_TIMEOUT)
* CONFIG_NIMBLE_SM_LEGACY (CONFIG_BT_NIMBLE_SM_LEGACY)
* CONFIG_NIMBLE_SM_SC (CONFIG_BT_NIMBLE_SM_SC)
* CONFIG_NIMBLE_SM_SC_DEBUG_KEYS (CONFIG_BT_NIMBLE_SM_SC_DEBUG_KEYS)
* CONFIG_NIMBLE_SVC_GAP_APPEARANCE (CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE)
* CONFIG_NIMBLE_SVC_GAP_DEVICE_NAME (CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME)
* CONFIG_NIMBLE_TASK_STACK_SIZE (CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE)
* CONFIG_NO_BLOBS (CONFIG_APP_NO_BLOBS)
* CONFIG_OPTIMIZATION_ASSERTION_LEVEL (CONFIG_COMPILER_OPTIMIZATION_ASSERTION_LEVEL)
	+ CONFIG_OPTIMIZATION_ASSERTIONS_ENABLED
	+ CONFIG_OPTIMIZATION_ASSERTIONS_SILENT
	+ CONFIG_OPTIMIZATION_ASSERTIONS_DISABLED
* CONFIG_OPTIMIZATION_COMPILER (CONFIG_COMPILER_OPTIMIZATION)
	+ CONFIG_OPTIMIZATION_LEVEL_DEBUG, CONFIG_COMPILER_OPTIMIZATION_LEVEL_DEBUG, CONFIG_COMPILER_OPTIMIZATION_DEFAULT
	+ CONFIG_OPTIMIZATION_LEVEL_RELEASE, CONFIG_COMPILER_OPTIMIZATION_LEVEL_RELEASE
* CONFIG_OSI_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_OSI_TRACE_LEVEL)
	+ CONFIG_OSI_TRACE_LEVEL_NONE
	+ CONFIG_OSI_TRACE_LEVEL_ERROR
	+ CONFIG_OSI_TRACE_LEVEL_WARNING
	+ CONFIG_OSI_TRACE_LEVEL_API
	+ CONFIG_OSI_TRACE_LEVEL_EVENT
	+ CONFIG_OSI_TRACE_LEVEL_DEBUG
	+ CONFIG_OSI_TRACE_LEVEL_VERBOSE
* CONFIG_OTA_ALLOW_HTTP (CONFIG_ESP_HTTPS_OTA_ALLOW_HTTP)
* CONFIG_PAN_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_PAN_TRACE_LEVEL)
	+ CONFIG_PAN_TRACE_LEVEL_NONE
	+ CONFIG_PAN_TRACE_LEVEL_ERROR
	+ CONFIG_PAN_TRACE_LEVEL_WARNING
	+ CONFIG_PAN_TRACE_LEVEL_API
	+ CONFIG_PAN_TRACE_LEVEL_EVENT
	+ CONFIG_PAN_TRACE_LEVEL_DEBUG
	+ CONFIG_PAN_TRACE_LEVEL_VERBOSE
* CONFIG_PM_POWER_DOWN_TAGMEM_IN_LIGHT_SLEEP (CONFIG_PM_RESTORE_CACHE_TAGMEM_AFTER_LIGHT_SLEEP)
* CONFIG_POST_EVENTS_FROM_IRAM_ISR (CONFIG_ESP_EVENT_POST_FROM_IRAM_ISR)
* CONFIG_POST_EVENTS_FROM_ISR (CONFIG_ESP_EVENT_POST_FROM_ISR)
* CONFIG_PPP_CHAP_SUPPORT (CONFIG_LWIP_PPP_CHAP_SUPPORT)
* CONFIG_PPP_DEBUG_ON (CONFIG_LWIP_PPP_DEBUG_ON)
* CONFIG_PPP_MPPE_SUPPORT (CONFIG_LWIP_PPP_MPPE_SUPPORT)
* CONFIG_PPP_MSCHAP_SUPPORT (CONFIG_LWIP_PPP_MSCHAP_SUPPORT)
* CONFIG_PPP_NOTIFY_PHASE_SUPPORT (CONFIG_LWIP_PPP_NOTIFY_PHASE_SUPPORT)
* CONFIG_PPP_PAP_SUPPORT (CONFIG_LWIP_PPP_PAP_SUPPORT)
* CONFIG_PPP_SUPPORT (CONFIG_LWIP_PPP_SUPPORT)
* CONFIG_REDUCE_PHY_TX_POWER (CONFIG_ESP_PHY_REDUCE_TX_POWER)
* CONFIG_RFCOMM_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_RFCOMM_TRACE_LEVEL)
	+ CONFIG_RFCOMM_TRACE_LEVEL_NONE
	+ CONFIG_RFCOMM_TRACE_LEVEL_ERROR
	+ CONFIG_RFCOMM_TRACE_LEVEL_WARNING
	+ CONFIG_RFCOMM_TRACE_LEVEL_API
	+ CONFIG_RFCOMM_TRACE_LEVEL_EVENT
	+ CONFIG_RFCOMM_TRACE_LEVEL_DEBUG
	+ CONFIG_RFCOMM_TRACE_LEVEL_VERBOSE
* CONFIG_SEMIHOSTFS_MAX_MOUNT_POINTS (CONFIG_VFS_SEMIHOSTFS_MAX_MOUNT_POINTS)
* CONFIG_SMP_INITIAL_TRACE_LEVEL (CONFIG_BT_LOG_SMP_TRACE_LEVEL)
	+ CONFIG_SMP_TRACE_LEVEL_NONE
	+ CONFIG_SMP_TRACE_LEVEL_ERROR
	+ CONFIG_SMP_TRACE_LEVEL_WARNING
	+ CONFIG_SMP_TRACE_LEVEL_API
	+ CONFIG_SMP_TRACE_LEVEL_EVENT
	+ CONFIG_SMP_TRACE_LEVEL_DEBUG
	+ CONFIG_SMP_TRACE_LEVEL_VERBOSE
* CONFIG_SMP_SLAVE_CON_PARAMS_UPD_ENABLE (CONFIG_BT_SMP_SLAVE_CON_PARAMS_UPD_ENABLE)
* CONFIG_SPI_FLASH_32BIT_ADDR_ENABLE (CONFIG_BOOTLOADER_CACHE_32BIT_ADDR_QUAD_FLASH)
* CONFIG_SPI_FLASH_QUAD_32BIT_ADDR_ENABLE (CONFIG_BOOTLOADER_CACHE_32BIT_ADDR_QUAD_FLASH)
* CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS (CONFIG_SPI_FLASH_DANGEROUS_WRITE)
	+ CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS_ABORTS
	+ CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS_FAILS
	+ CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS_ALLOWED
* CONFIG_STACK_CHECK_MODE (CONFIG_COMPILER_STACK_CHECK_MODE)
	+ CONFIG_STACK_CHECK_NONE
	+ CONFIG_STACK_CHECK_NORM
	+ CONFIG_STACK_CHECK_STRONG
	+ CONFIG_STACK_CHECK_ALL
* CONFIG_SUPPORT_TERMIOS (CONFIG_VFS_SUPPORT_TERMIOS)
* CONFIG_SUPPRESS_SELECT_DEBUG_OUTPUT (CONFIG_VFS_SUPPRESS_SELECT_DEBUG_OUTPUT)
* CONFIG_SW_COEXIST_ENABLE (CONFIG_ESP_COEX_SW_COEXIST_ENABLE)
* CONFIG_SYSTEM_EVENT_QUEUE_SIZE (CONFIG_ESP_SYSTEM_EVENT_QUEUE_SIZE)
* CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE (CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE)
* CONFIG_SYSVIEW_BUF_WAIT_TMO (CONFIG_APPTRACE_SV_BUF_WAIT_TMO)
* CONFIG_SYSVIEW_ENABLE (CONFIG_APPTRACE_SV_ENABLE)
* CONFIG_SYSVIEW_EVT_IDLE_ENABLE (CONFIG_APPTRACE_SV_EVT_IDLE_ENABLE)
* CONFIG_SYSVIEW_EVT_ISR_ENTER_ENABLE (CONFIG_APPTRACE_SV_EVT_ISR_ENTER_ENABLE)
* CONFIG_SYSVIEW_EVT_ISR_EXIT_ENABLE (CONFIG_APPTRACE_SV_EVT_ISR_EXIT_ENABLE)
* CONFIG_SYSVIEW_EVT_ISR_TO_SCHEDULER_ENABLE (CONFIG_APPTRACE_SV_EVT_ISR_TO_SCHED_ENABLE)
* CONFIG_SYSVIEW_EVT_OVERFLOW_ENABLE (CONFIG_APPTRACE_SV_EVT_OVERFLOW_ENABLE)
* CONFIG_SYSVIEW_EVT_TASK_CREATE_ENABLE (CONFIG_APPTRACE_SV_EVT_TASK_CREATE_ENABLE)
* CONFIG_SYSVIEW_EVT_TASK_START_EXEC_ENABLE (CONFIG_APPTRACE_SV_EVT_TASK_START_EXEC_ENABLE)
* CONFIG_SYSVIEW_EVT_TASK_START_READY_ENABLE (CONFIG_APPTRACE_SV_EVT_TASK_START_READY_ENABLE)
* CONFIG_SYSVIEW_EVT_TASK_STOP_EXEC_ENABLE (CONFIG_APPTRACE_SV_EVT_TASK_STOP_EXEC_ENABLE)
* CONFIG_SYSVIEW_EVT_TASK_STOP_READY_ENABLE (CONFIG_APPTRACE_SV_EVT_TASK_STOP_READY_ENABLE)
* CONFIG_SYSVIEW_EVT_TASK_TERMINATE_ENABLE (CONFIG_APPTRACE_SV_EVT_TASK_TERMINATE_ENABLE)
* CONFIG_SYSVIEW_EVT_TIMER_ENTER_ENABLE (CONFIG_APPTRACE_SV_EVT_TIMER_ENTER_ENABLE)
* CONFIG_SYSVIEW_EVT_TIMER_EXIT_ENABLE (CONFIG_APPTRACE_SV_EVT_TIMER_EXIT_ENABLE)
* CONFIG_SYSVIEW_MAX_TASKS (CONFIG_APPTRACE_SV_MAX_TASKS)
* CONFIG_SYSVIEW_TS_SOURCE (CONFIG_APPTRACE_SV_TS_SOURCE)
	+ CONFIG_SYSVIEW_TS_SOURCE_CCOUNT
	+ CONFIG_SYSVIEW_TS_SOURCE_ESP_TIMER
* CONFIG_TASK_WDT (CONFIG_ESP_TASK_WDT_INIT)
* CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU0 (CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0)
* CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU1 (CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1)
* CONFIG_TASK_WDT_PANIC (CONFIG_ESP_TASK_WDT_PANIC)
* CONFIG_TASK_WDT_TIMEOUT_S (CONFIG_ESP_TASK_WDT_TIMEOUT_S)
* CONFIG_TCPIP_RECVMBOX_SIZE (CONFIG_LWIP_TCPIP_RECVMBOX_SIZE)
* CONFIG_TCPIP_TASK_AFFINITY (CONFIG_LWIP_TCPIP_TASK_AFFINITY)
	+ CONFIG_TCPIP_TASK_AFFINITY_NO_AFFINITY
	+ CONFIG_TCPIP_TASK_AFFINITY_CPU0
	+ CONFIG_TCPIP_TASK_AFFINITY_CPU1
* CONFIG_TCPIP_TASK_STACK_SIZE (CONFIG_LWIP_TCPIP_TASK_STACK_SIZE)
* CONFIG_TCP_MAXRTX (CONFIG_LWIP_TCP_MAXRTX)
* CONFIG_TCP_MSL (CONFIG_LWIP_TCP_MSL)
* CONFIG_TCP_MSS (CONFIG_LWIP_TCP_MSS)
* CONFIG_TCP_OVERSIZE (CONFIG_LWIP_TCP_OVERSIZE)
	+ CONFIG_TCP_OVERSIZE_MSS
	+ CONFIG_TCP_OVERSIZE_QUARTER_MSS
	+ CONFIG_TCP_OVERSIZE_DISABLE
* CONFIG_TCP_QUEUE_OOSEQ (CONFIG_LWIP_TCP_QUEUE_OOSEQ)
* CONFIG_TCP_RECVMBOX_SIZE (CONFIG_LWIP_TCP_RECVMBOX_SIZE)
* CONFIG_TCP_SND_BUF_DEFAULT (CONFIG_LWIP_TCP_SND_BUF_DEFAULT)
* CONFIG_TCP_SYNMAXRTX (CONFIG_LWIP_TCP_SYNMAXRTX)
* CONFIG_TCP_WND_DEFAULT (CONFIG_LWIP_TCP_WND_DEFAULT)
* CONFIG_TIMER_QUEUE_LENGTH (CONFIG_FREERTOS_TIMER_QUEUE_LENGTH)
* CONFIG_TIMER_TASK_PRIORITY (CONFIG_FREERTOS_TIMER_TASK_PRIORITY)
* CONFIG_TIMER_TASK_STACK_DEPTH (CONFIG_FREERTOS_TIMER_TASK_STACK_DEPTH)
* CONFIG_TIMER_TASK_STACK_SIZE (CONFIG_ESP_TIMER_TASK_STACK_SIZE)
* CONFIG_UDP_RECVMBOX_SIZE (CONFIG_LWIP_UDP_RECVMBOX_SIZE)
* CONFIG_WARN_WRITE_STRINGS (CONFIG_COMPILER_WARN_WRITE_STRINGS)
* CONFIG_WPA_11KV_SUPPORT (CONFIG_ESP_WIFI_11KV_SUPPORT)
* CONFIG_WPA_11R_SUPPORT (CONFIG_ESP_WIFI_11R_SUPPORT)
* CONFIG_WPA_DEBUG_PRINT (CONFIG_ESP_WIFI_DEBUG_PRINT)
* CONFIG_WPA_DPP_SUPPORT (CONFIG_ESP_WIFI_DPP_SUPPORT)
* CONFIG_WPA_MBEDTLS_CRYPTO (CONFIG_ESP_WIFI_MBEDTLS_CRYPTO)
* CONFIG_WPA_MBEDTLS_TLS_CLIENT (CONFIG_ESP_WIFI_MBEDTLS_TLS_CLIENT)
* CONFIG_WPA_MBO_SUPPORT (CONFIG_ESP_WIFI_MBO_SUPPORT)
* CONFIG_WPA_SCAN_CACHE (CONFIG_ESP_WIFI_SCAN_CACHE)
* CONFIG_WPA_SUITE_B_192 (CONFIG_ESP_WIFI_SUITE_B_192)
* CONFIG_WPA_TESTING_OPTIONS (CONFIG_ESP_WIFI_TESTING_OPTIONS)
* CONFIG_WPA_WAPI_PSK (CONFIG_ESP_WIFI_WAPI_PSK)
* CONFIG_WPA_WPS_SOFTAP_REGISTRAR (CONFIG_ESP_WIFI_WPS_SOFTAP_REGISTRAR)
* CONFIG_WPA_WPS_STRICT (CONFIG_ESP_WIFI_WPS_STRICT)