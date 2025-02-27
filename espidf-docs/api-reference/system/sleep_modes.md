# Sleep Modes




## Overview


ESP32-S3 supports two major power saving modes: Light-sleep and Deep-sleep. According to the features used by an application, there are some sub sleep modes. See Sleep Modes for these sleep modes and sub sleep modes. Additionally, there are some power-down options that can be configured to further reduce the power consumption. See Power-down Options for more details.


There are several wakeup sources in the sleep modes. These sources can also be combined so that the chip will wake up when any of the sources are triggered. Wakeup Sources describes these wakeup sources and configuration APIs in detail.


The configuration of power-down options and wakeup sources are optional. They can be configured at any moment before entering the sleep modes.


Then the application can call sleep start APIs to enter one of the sleep modes. See Entering Sleep for more details. When the wakeup condition is met, the application is awoken from sleep. See Checking Sleep Wakeup Cause on how to get the wakeup cause, and Disable Sleep Wakeup Source on how to handle the wakeup sources after wakeup.




## Sleep Modes


In Light-sleep mode, the digital peripherals, most of the RAM, and CPUs are clock-gated and their supply voltage is reduced. Upon exit from Light-sleep, the digital peripherals, RAM, and CPUs resume operation and their internal states are preserved.


In Deep-sleep mode, the CPUs, most of the RAM, and all digital peripherals that are clocked from APB_CLK are powered off. The only parts of the chip that remain powered on are:



> * RTC controller
> * ULP coprocessor
> * RTC FAST memory
> * RTC SLOW memory



### Wi-Fi/Bluetooth and Sleep Modes


In Deep-sleep and Light-sleep modes, the wireless peripherals are powered down. Before entering Deep-sleep or Light-sleep modes, the application must disable Wi-Fi and Bluetooth using the appropriate calls (i.e., `nimble_port_stop()`, `nimble_port_deinit()`, `esp_bluedroid_disable()`, `esp_bluedroid_deinit()`, `esp_bt_controller_disable()`, `esp_bt_controller_deinit()`, `esp_wifi_stop()`). Wi-Fi and Bluetooth connections are not maintained in Deep-sleep or Light-sleep mode, even if these functions are not called.


If Wi-Fi/Bluetooth connections need to be maintained, enable Wi-Fi/Bluetooth Modem-sleep mode and automatic Light-sleep feature (see Power Management APIs). This allows the system to wake up from sleep automatically when required by the Wi-Fi/Bluetooth driver, thereby maintaining the connection.




### Sub Sleep Modes


Tables below list the sub sleep modes in the first row and the features they support in the first column. Modes that support more features may consume more power during sleep mode. The sleep system automatically selects the mode that satisfies all the features required by the user while consuming least power.


Deep-sleep:




|  | DSLP_ULTRA_LOW | DSLP_DEFAULT | DSLP_8MD256/ |
| --- | --- | --- | --- |
|  |  |  | DSLP_ADC_TSENS |
| ULP/Touch sensor (ESP32-S2 and ESP32-S3 only) | Y | Y | Y |
| RTC IO input/RTC memory at high temperature |  | Y | Y |
| ADC_TSEN_MONITOR |  |  | Y |
| 8MD256 as the clock source for RTC_SLOW_CLK |  |  | Y |


Features:


1. RTC IO input/RTC memory at high temperature (experimental): Use RTC IO as input pins, or use RTC memory at high temperature. The chip can go into ultra low power mode when these features are disabled. Controlled by API `rtc_sleep_enable_ultra_low()`.
2. ADC_TSEN_MONITOR: Use ADC/Temperature Sensor in monitor mode (controlled by ULP). Enabled by API `ulp_adc_init()` or its higher level APIs. Only available for ESP32-S2 and ESP32-S3 chips with monitor mode.
3. 8MD256 as the clock source for RTC_SLOW_CLK: When 8MD256 is selected as the clock source for RTC_SLOW_CLK using the Kconfig option `CONFIG_RTC_CLK_SRC_INT_8MD256`, the chip will automatically enter this sub sleep mode during Deep-sleep mode.


Light-sleep:




|  | LSLP_DEFAULT | LSLP_ADC_TSENS | LSLP_8MD256 | LSLP_LEDC8M/ |
| --- | --- | --- | --- | --- |
|  |  |  |  | LSLP_XTAL_FPU |
| ULP/Touch sensor (ESP32-S2 and ESP32-S3 only) | Y | Y | Y | Y |
| RTC IO input/RTC memory at high temperature | Y | Y | Y | Y |
| ADC_TSEN_MONITOR |  | Y | Y | Y |
| 8MD256 as the clock source for RTC_SLOW_CLK |  |  | Y | Y |
| 8 MHz RC clock source used by digital peripherals |  |  |  | Y |
| Keep the XTAL clock on |  |  |  | Y |


Features: (Also see 8MD256 and ADC_TSEN_MONITOR features for Deep-sleep mode above)


1. 8 MHz RC clock source used by digital peripherals: Currently, only LEDC uses this clock source during Light-sleep mode. When LEDC selects this clock source, this feature is automatically enabled.
2. Keep the XTAL clock on: Keep the XTAL clock on during Light-sleep mode. Controlled by `ESP_PD_DOMAIN_XTAL` power domain.


Default mode of ESP32-S3 already supports ADC_TSEN_MONITOR feature.





## Wakeup Sources


Wakeup sources can be enabled using `esp_sleep_enable_X_wakeup` APIs. Wakeup sources are not disabled after wakeup, you can disable them using `esp_sleep_disable_wakeup_source()` API if you do not need them any more. See Disable Sleep Wakeup Source.


Following are the wakeup sources supported on ESP32-S3.



### Timer


The RTC controller has a built-in timer which can be used to wake up the chip after a predefined amount of time. Time is specified at microsecond precision, but the actual resolution depends on the clock source selected for RTC_SLOW_CLK.


For details on RTC clock options, see **ESP32-S3 Technical Reference Manual** > **ULP Coprocessor** [PDF].


RTC peripherals or RTC memories do not need to be powered on during sleep in this wakeup mode.


`esp_sleep_enable_timer_wakeup()` function can be used to enable sleep wakeup using a timer.




### Touchpad


The RTC IO module contains the logic to trigger wakeup when a touch sensor interrupt occurs. To wakeup from a touch sensor interrupt, users need to configure the touch pad interrupt before the chip enters Deep-sleep or Light-sleep modes.


`esp_sleep_enable_touchpad_wakeup()` function can be used to enable this wakeup source.




### External Wakeup (`ext0`)


The RTC IO module contains the logic to trigger wakeup when one of RTC GPIOs is set to a predefined logic level. RTC IO is part of the RTC peripherals power domain, so RTC peripherals will be kept powered on during Deep-sleep if this wakeup source is requested.


The RTC IO module is enabled in this mode, so internal pullup or pulldown resistors can also be used. They need to be configured by the application using `rtc_gpio_pullup_en()` and `rtc_gpio_pulldown_en()` functions before calling `esp_deep_sleep_start()`.


`esp_sleep_enable_ext0_wakeup()` function can be used to enable this wakeup source.



Warning


After waking up from sleep, the IO pad used for wakeup will be configured as RTC IO. Therefore, before using this pad as digital GPIO, users need to reconfigure it using `rtc_gpio_deinit()` function.





### External Wakeup (`ext1`)


The RTC controller contains the logic to trigger wakeup using multiple RTC GPIOs. One of the following two logic functions can be used to trigger ext1 wakeup:


* wake up if any of the selected pins is high (`ESP_EXT1_WAKEUP_ANY_HIGH`)
* wake up if any of the selected pins is low (`ESP_EXT1_WAKEUP_ANY_LOW`)


This wakeup source is controlled by the RTC controller. Unlike `ext0`, this wakeup source supports wakeup even when the RTC peripheral is powered down. Although the power domain of the RTC peripheral, where RTC IOs are located, is powered down during sleep modes, ESP-IDF will automatically lock the state of the wakeup pin before the system enters sleep modes and unlock upon exiting sleep modes. Therefore, the internal pull-up or pull-down resistors can still be configured for the wakeup pin:



```
esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
rtc_gpio_pullup_dis(gpio_num);
rtc_gpio_pulldown_en(gpio_num);

```


If we turn off the `RTC_PERIPH` domain, we will use the HOLD feature to maintain the pull-up and pull-down on the pins during sleep. HOLD feature will be acted on the pin internally before the system enters sleep modes, and this can further reduce power consumption:



```
rtc_gpio_pullup_dis(gpio_num);
rtc_gpio_pulldown_en(gpio_num);

```


If certain chips lack the `RTC_PERIPH` domain, we can only use the HOLD feature to maintain the pull-up and pull-down on the pins during sleep modes:



```
gpio_pullup_dis(gpio_num);
gpio_pulldown_en(gpio_num);

```


`esp_sleep_enable_ext1_wakeup_io()` function can be used to append ext1 wakeup IO and set corresponding wakeup level.


`esp_sleep_disable_ext1_wakeup_io()` function can be used to remove ext1 wakeup IO.



Note


Due to hardware limitation, when we use more than one IO for EXT1 wakeup, it is not allowed to configure different wakeup levels for the IOs, and there is corresponding inspection mechanism in `esp_sleep_enable_ext1_wakeup_io()`.




Warning


* To use the EXT1 wakeup, the IO pad(s) are configured as RTC IO. Therefore, before using these pads as digital GPIOs, users need to reconfigure them by calling the `rtc_gpio_deinit()` function.
* If the RTC peripherals are configured to be powered down (which is by default), the wakeup IOs will be set to the holding state before entering sleep. Therefore, after the chip wakes up from Light-sleep, please call `rtc_gpio_hold_dis` to disable the hold function to perform any pin re-configuration. For Deep-sleep wakeup, this is already being handled at the application startup stage.





### ULP Coprocessor Wakeup


ULP coprocessor can run while the chip is in sleep mode, and may be used to poll sensors, monitor ADC or touch sensor values, and wake up the chip when a specific event is detected. ULP coprocessor is part of the RTC peripherals power domain, and it runs the program stored in RTC SLOW memory. RTC SLOW memory will be powered on during sleep if this wakeup mode is requested. RTC peripherals will be automatically powered on before ULP coprocessor starts running the program; once the program stops running, RTC peripherals are automatically powered down again.


`esp_sleep_enable_ulp_wakeup()` function can be used to enable this wakeup source.




### GPIO Wakeup (Light-sleep Only)


In addition to EXT0 and EXT1 wakeup sources described above, one more method of wakeup from external inputs is available in Light-sleep mode. With this wakeup source, each pin can be individually configured to trigger wakeup on high or low level using `gpio_wakeup_enable()` function. Unlike EXT0 and EXT1 wakeup sources, which can only be used with RTC IOs, this wakeup source can be used with any IO (RTC or digital).


`esp_sleep_enable_gpio_wakeup()` function can be used to enable this wakeup source.



Warning


Before entering Light-sleep mode, check if any GPIO pin to be driven is part of the VDD_SPI power domain. If so, this power domain must be configured to remain ON during sleep.


For example, on ESP32-WROOM-32 board, GPIO16 and GPIO17 are linked to VDD_SPI power domain. If they are configured to remain high during Light-sleep, the power domain should be configured to remain powered ON. This can be done with `esp_sleep_pd_config()`:



```
esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_ON);

```





### UART Wakeup (Light-sleep Only)


When ESP32-S3 receives UART input from external devices, it is often necessary to wake up the chip when input data is available. The UART peripheral contains a feature which allows waking up the chip from Light-sleep when a certain number of positive edges on RX pin are seen. This number of positive edges can be set using `uart_set_wakeup_threshold()` function. Note that the character which triggers wakeup (and any characters before it) will not be received by the UART after wakeup. This means that the external device typically needs to send an extra character to the ESP32-S3 to trigger wakeup before sending the data.


`esp_sleep_enable_uart_wakeup()` function can be used to enable this wakeup source.


After waking-up from UART, you should send some extra data through the UART port in Active mode, so that the internal wakeup indication signal can be cleared. Otherwises, the next UART wake-up would trigger with two less rising edges than the configured threshold value.




### Disable Sleep Wakeup Source


Previously configured wakeup sources can be disabled later using `esp_sleep_disable_wakeup_source()` API. This function deactivates trigger for the given wakeup source. Additionally, it can disable all triggers if the argument is `ESP_SLEEP_WAKEUP_ALL`.





## Power-down Options


The application can force specific powerdown modes for RTC peripherals and RTC memories. In Deep-sleep mode, we can also isolate some IOs to further reduce current consumption.



### Power-down of RTC Peripherals and Memories


By default, `esp_deep_sleep_start()` and `esp_light_sleep_start()` functions power down all RTC power domains which are not needed by the enabled wakeup sources. To override this behaviour, `esp_sleep_pd_config()` function is provided.


If some variables in the program are placed into RTC SLOW memory (for example, using `RTC_DATA_ATTR` attribute), RTC SLOW memory will be kept powered on by default. This can be overridden using `esp_sleep_pd_config()` function, if desired.




### Power-down of Flash


By default, to avoid potential issues, `esp_light_sleep_start()` function does **not** power down flash. To be more specific, it takes time to power down the flash and during this period the system may be woken up, which then actually powers up the flash before this flash could be powered down completely. As a result, there is a chance that the flash may not work properly.


So, in theory, it is ok if you only wake up the system after the flash is completely powered down. However, in reality, the flash power-down period can be hard to predict (for example, this period can be much longer when you add filter capacitors to the flash's power supply circuit) and uncontrollable (for example, the asynchronous wake-up signals make the actual sleep time uncontrollable).



Warning


If a filter capacitor is added to your flash power supply circuit, please do everything possible to avoid powering down flash.



Therefore, it is recommended not to power down flash when using ESP-IDF. For power-sensitive applications, it is recommended to use Kconfig option CONFIG_ESP_SLEEP_FLASH_LEAKAGE_WORKAROUND to reduce the power consumption of the flash during Light-sleep, instead of powering down the flash.


It is worth mentioning that PSRAM has a similar Kconfig option CONFIG_ESP_SLEEP_PSRAM_LEAKAGE_WORKAROUND.


However, for those who have fully understood the risk and are still willing to power down the flash to further reduce the power consumption, please check the following mechanisms:



> * Setting Kconfig option CONFIG_ESP_SLEEP_POWER_DOWN_FLASH only powers down the flash when the RTC timer is the only wake-up source **and** the sleep time is longer than the flash power-down period.
> * Calling `esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF)` powers down flash when the RTC timer is not enabled as a wakeup source **or** the sleep time is longer than the flash power-down period.



Note


* ESP-IDF does not provide any mechanism that can power down the flash in all conditions when Light-sleep.
* `esp_deep_sleep_start()` function forces power down flash regardless of user configuration.








### Configuring IOs (Deep-sleep Only)


Some ESP32-S3 IOs have internal pullups or pulldowns, which are enabled by default. If an external circuit drives this pin in Deep-sleep mode, current consumption may increase due to current flowing through these pullups and pulldowns.


To isolate a pin to prevent extra current draw, call `rtc_gpio_isolate()` function.


For example, on ESP32-WROVER module, GPIO12 is pulled up externally, and it also has an internal pulldown in the ESP32 chip. This means that in Deep-sleep, some current flows through these external and internal resistors, increasing Deep-sleep current above the minimal possible value.


Add the following code before `esp_deep_sleep_start()` to remove such extra current:



```
rtc_gpio_isolate(GPIO_NUM_12);

```





## Entering Sleep


`esp_light_sleep_start()` or `esp_deep_sleep_start()` functions can be used to enter Light-sleep or Deep-sleep modes correspondingly. After that, the system configures the parameters of RTC controller according to the requested wakeup sources and power-down options.


It is also possible to enter sleep modes with no wakeup sources configured. In this case, the chip will be in sleep modes indefinitely until external reset is applied.



### UART Output Handling


Before entering sleep mode, `esp_deep_sleep_start()` will flush the contents of UART FIFOs.


When entering Light-sleep mode using `esp_light_sleep_start()`, UART FIFOs will not be flushed. Instead, UART output will be suspended, and remaining characters in the FIFO will be sent out after wakeup from Light-sleep.





## Checking Sleep Wakeup Cause


`esp_sleep_get_wakeup_cause()` function can be used to check which wakeup source has triggered wakeup from sleep mode.


For touchpad, it is possible to identify which touch pin has caused wakeup using `esp_sleep_get_touchpad_wakeup_status()` functions.


For ext1 wakeup sources, it is possible to identify which touch pin has caused wakeup using `esp_sleep_get_ext1_wakeup_status()` functions.




## Application Example


* protocols/sntp: the implementation of basic functionality of Deep-sleep, where ESP module is periodically waken up to retrieve time from NTP server.
* wifi/power_save: the usage of Wi-Fi Modem-sleep mode and automatic Light-sleep feature to maintain Wi-Fi connections.
* bluetooth/nimble/power_save: the usage of Bluetooth Modem-sleep mode and automatic Light-sleep feature to maintain Bluetooth connections.
* system/deep_sleep: the usage of various Deep-sleep wakeup triggers and ULP coprocessor programming.
* system/light_sleep: the usage of Light-sleep wakeup triggered by various sources supported by the chip (Timer, GPIO, Touch Sensor, etc.).







## API Reference



### Header File


* components/esp_hw_support/include/esp_sleep.h
* This header file can be included with:



> ```
> #include "esp_sleep.h"
> 
> ```




### Functions


#### esp_sleep_disable_wakeup_source


```


esp_err_t esp_sleep_disable_wakeup_source(esp_sleep_source_t source)
Disable wakeup source. 
This function is used to deactivate wake up trigger for source defined as parameter of the function.

See docs/sleep-modes.rst for details.

Note
This function does not modify wake up configuration in RTC. It will be performed in esp_deep_sleep_start/esp_light_sleep_start function.


Parameters
source -- - number of source to disable of type esp_sleep_source_t 

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if trigger was not active 





```

#### esp_sleep_enable_ulp_wakeup


```


esp_err_t esp_sleep_enable_ulp_wakeup(void)
Enable wakeup by ULP coprocessor. 

Note
On ESP32, ULP wakeup source cannot be used when RTC_PERIPH power domain is forced, to be powered on (ESP_PD_OPTION_ON) or when ext0 wakeup source is used. 


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if additional current by touch (CONFIG_RTC_EXT_CRYST_ADDIT_CURRENT) is enabled.
ESP_ERR_INVALID_STATE if ULP co-processor is not enabled or if wakeup triggers conflict 





```

#### esp_sleep_enable_timer_wakeup


```


esp_err_t esp_sleep_enable_timer_wakeup(uint64_t time_in_us)
Enable wakeup by timer. 

Parameters
time_in_us -- time before wakeup, in microseconds 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if value is out of range (TBD) 





```

#### esp_sleep_enable_touchpad_wakeup


```


esp_err_t esp_sleep_enable_touchpad_wakeup(void)
Enable wakeup by touch sensor. 

Note
On ESP32, touch wakeup source can not be used when RTC_PERIPH power domain is forced to be powered on (ESP_PD_OPTION_ON) or when ext0 wakeup source is used.


Note
The FSM mode of the touch button should be configured as the timer trigger mode.


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if additional current by touch (CONFIG_RTC_EXT_CRYST_ADDIT_CURRENT) is enabled.
ESP_ERR_INVALID_STATE if wakeup triggers conflict 





```

#### esp_sleep_get_touchpad_wakeup_status


```


touch_pad_t esp_sleep_get_touchpad_wakeup_status(void)
Get the touch pad which caused wakeup. 
If wakeup was caused by another source, this function will return TOUCH_PAD_MAX;

Returns
touch pad which caused wakeup 



```

#### esp_sleep_is_valid_wakeup_gpio


```


bool esp_sleep_is_valid_wakeup_gpio(gpio_num_t gpio_num)
Returns true if a GPIO number is valid for use as wakeup source. 

Note
For SoCs with RTC IO capability, this can be any valid RTC IO input pin.


Parameters
gpio_num -- Number of the GPIO to test for wakeup source capability

Returns
True if this GPIO number will be accepted as a sleep wakeup source. 



```

#### esp_sleep_enable_ext0_wakeup


```


esp_err_t esp_sleep_enable_ext0_wakeup(gpio_num_t gpio_num, int level)
Enable wakeup using a pin. 
This function uses external wakeup feature of RTC_IO peripheral. It will work only if RTC peripherals are kept on during sleep.
This feature can monitor any pin which is an RTC IO. Once the pin transitions into the state given by level argument, the chip will be woken up.

Note
This function does not modify pin configuration. The pin is configured in esp_deep_sleep_start/esp_light_sleep_start, immediately before entering sleep mode.


Note
ESP32: ext0 wakeup source can not be used together with touch or ULP wakeup sources.


Parameters

gpio_num -- GPIO number used as wakeup source. Only GPIOs with the RTC functionality can be used. For different SoCs, the related GPIOs are:
ESP32: 0, 2, 4, 12-15, 25-27, 32-39;
ESP32-S2: 0-21;
ESP32-S3: 0-21. 


level -- input level which will trigger wakeup (0=low, 1=high) 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if the selected GPIO is not an RTC GPIO, or the mode is invalid
ESP_ERR_INVALID_STATE if wakeup triggers conflict 





```

#### esp_sleep_enable_ext1_wakeup


```


esp_err_t esp_sleep_enable_ext1_wakeup(uint64_t io_mask, esp_sleep_ext1_wakeup_mode_t level_mode)
Enable wakeup using multiple pins. 
This function uses external wakeup feature of RTC controller. It will work even if RTC peripherals are shut down during sleep.
This feature can monitor any number of pins which are in RTC IOs. Once selected pins go into the state given by level_mode argument, the chip will be woken up.

Note
This function does not modify pin configuration. The pins are configured in esp_deep_sleep_start/esp_light_sleep_start, immediately before entering sleep mode.


Note
Internal pullups and pulldowns don't work when RTC peripherals are shut down. In this case, external resistors need to be added. Alternatively, RTC peripherals (and pullups/pulldowns) may be kept enabled using esp_sleep_pd_config function. If we turn off the RTC_PERIPH domain or certain chips lack the RTC_PERIPH domain, we will use the HOLD feature to maintain the pull-up and pull-down on the pins during sleep. HOLD feature will be acted on the pin internally before the system entering sleep, and this can further reduce power consumption.


Note
Call this func will reset the previous ext1 configuration.


Note
This function will be deprecated in release/v6.0. Please switch to use esp_sleep_enable_ext1_wakeup_io and esp_sleep_disable_ext1_wakeup_io


Parameters

io_mask -- Bit mask of GPIO numbers which will cause wakeup. Only GPIOs which have RTC functionality can be used in this bit map. For different SoCs, the related GPIOs are:
ESP32: 0, 2, 4, 12-15, 25-27, 32-39
ESP32-S2: 0-21
ESP32-S3: 0-21
ESP32-C6: 0-7
ESP32-H2: 7-14 


level_mode -- Select logic function used to determine wakeup condition: When target chip is ESP32:
ESP_EXT1_WAKEUP_ALL_LOW: wake up when all selected GPIOs are low
ESP_EXT1_WAKEUP_ANY_HIGH: wake up when any of the selected GPIOs is high When target chip is ESP32-S2, ESP32-S3, ESP32-C6 or ESP32-H2:
ESP_EXT1_WAKEUP_ANY_LOW: wake up when any of the selected GPIOs is low
ESP_EXT1_WAKEUP_ANY_HIGH: wake up when any of the selected GPIOs is high 




Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if io_mask is zero,, or mode is invalid 





```

#### esp_sleep_enable_ext1_wakeup_io


```


esp_err_t esp_sleep_enable_ext1_wakeup_io(uint64_t io_mask, esp_sleep_ext1_wakeup_mode_t level_mode)
Enable ext1 wakeup pins with IO masks. 
This will append selected IOs to the wakeup IOs, it will not reset previously enabled IOs. To reset specific previously enabled IOs, call esp_sleep_disable_ext1_wakeup_io with the io_mask. To reset all the enabled IOs, call esp_sleep_disable_ext1_wakeup_io(0).
This function uses external wakeup feature of RTC controller. It will work even if RTC peripherals are shut down during sleep.
This feature can monitor any number of pins which are in RTC IOs. Once selected pins go into the state given by level_mode argument, the chip will be woken up.

Note
This function does not modify pin configuration. The pins are configured in esp_deep_sleep_start/esp_light_sleep_start, immediately before entering sleep mode.


Note
Internal pullups and pulldowns don't work when RTC peripherals are shut down. In this case, external resistors need to be added. Alternatively, RTC peripherals (and pullups/pulldowns) may be kept enabled using esp_sleep_pd_config function. If we turn off the RTC_PERIPH domain or certain chips lack the RTC_PERIPH domain, we will use the HOLD feature to maintain the pull-up and pull-down on the pins during sleep. HOLD feature will be acted on the pin internally before the system entering sleep, and this can further reduce power consumption.


Parameters

io_mask -- Bit mask of GPIO numbers which will cause wakeup. Only GPIOs which have RTC functionality can be used in this bit map. For different SoCs, the related GPIOs are:
ESP32: 0, 2, 4, 12-15, 25-27, 32-39
ESP32-S2: 0-21
ESP32-S3: 0-21
ESP32-C6: 0-7
ESP32-H2: 7-14 


level_mode -- Select logic function used to determine wakeup condition: When target chip is ESP32:
ESP_EXT1_WAKEUP_ALL_LOW: wake up when all selected GPIOs are low
ESP_EXT1_WAKEUP_ANY_HIGH: wake up when any of the selected GPIOs is high When target chip is ESP32-S2, ESP32-S3, ESP32-C6 or ESP32-H2:
ESP_EXT1_WAKEUP_ANY_LOW: wake up when any of the selected GPIOs is low
ESP_EXT1_WAKEUP_ANY_HIGH: wake up when any of the selected GPIOs is high 




Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if any of the selected GPIOs is not an RTC GPIO, or mode is invalid
ESP_ERR_NOT_ALLOWED when wakeup level will become different between ext1 IOs if !SOC_PM_SUPPORT_EXT1_WAKEUP_MODE_PER_PIN 





```

#### esp_sleep_disable_ext1_wakeup_io


```


esp_err_t esp_sleep_disable_ext1_wakeup_io(uint64_t io_mask)
Disable ext1 wakeup pins with IO masks. This will remove selected IOs from the wakeup IOs. 

Parameters
io_mask -- Bit mask of GPIO numbers which will cause wakeup. Only GPIOs which have RTC functionality can be used in this bit map. If value is zero, this func will remove all previous ext1 configuration. For different SoCs, the related GPIOs are:
ESP32: 0, 2, 4, 12-15, 25-27, 32-39
ESP32-S2: 0-21
ESP32-S3: 0-21
ESP32-C6: 0-7
ESP32-H2: 7-14



Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if any of the selected GPIOs is not an RTC GPIO. 





```

#### esp_sleep_enable_gpio_wakeup


```


esp_err_t esp_sleep_enable_gpio_wakeup(void)
Enable wakeup from light sleep using GPIOs. 
Each GPIO supports wakeup function, which can be triggered on either low level or high level. Unlike EXT0 and EXT1 wakeup sources, this method can be used both for all IOs: RTC IOs and digital IOs. It can only be used to wakeup from light sleep though.
To enable wakeup, first call gpio_wakeup_enable, specifying gpio number and wakeup level, for each GPIO which is used for wakeup. Then call this function to enable wakeup feature.

Note
On ESP32, GPIO wakeup source can not be used together with touch or ULP wakeup sources.


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if wakeup triggers conflict 





```

#### esp_sleep_enable_uart_wakeup


```


esp_err_t esp_sleep_enable_uart_wakeup(int uart_num)
Enable wakeup from light sleep using UART. 
Use uart_set_wakeup_threshold function to configure UART wakeup threshold.
Wakeup from light sleep takes some time, so not every character sent to the UART can be received by the application.

Note
ESP32 does not support wakeup from UART2.


Parameters
uart_num -- UART port to wake up from 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if wakeup from given UART is not supported 





```

#### esp_sleep_enable_bt_wakeup


```


esp_err_t esp_sleep_enable_bt_wakeup(void)
Enable wakeup by bluetooth. 

Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if wakeup from bluetooth is not supported 





```

#### esp_sleep_disable_bt_wakeup


```


esp_err_t esp_sleep_disable_bt_wakeup(void)
Disable wakeup by bluetooth. 

Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if wakeup from bluetooth is not supported 





```

#### esp_sleep_enable_wifi_wakeup


```


esp_err_t esp_sleep_enable_wifi_wakeup(void)
Enable wakeup by WiFi MAC. 

Returns

ESP_OK on success 





```

#### esp_sleep_disable_wifi_wakeup


```


esp_err_t esp_sleep_disable_wifi_wakeup(void)
Disable wakeup by WiFi MAC. 

Returns

ESP_OK on success 





```

#### esp_sleep_enable_wifi_beacon_wakeup


```


esp_err_t esp_sleep_enable_wifi_beacon_wakeup(void)
Enable beacon wakeup by WiFi MAC, it will wake up the system into modem state. 

Returns

ESP_OK on success 





```

#### esp_sleep_disable_wifi_beacon_wakeup


```


esp_err_t esp_sleep_disable_wifi_beacon_wakeup(void)
Disable beacon wakeup by WiFi MAC. 

Returns

ESP_OK on success 





```

#### esp_sleep_get_ext1_wakeup_status


```


uint64_t esp_sleep_get_ext1_wakeup_status(void)
Get the bit mask of GPIOs which caused wakeup (ext1) 
If wakeup was caused by another source, this function will return 0.

Returns
bit mask, if GPIOn caused wakeup, BIT(n) will be set 



```

#### esp_sleep_pd_config


```


esp_err_t esp_sleep_pd_config(esp_sleep_pd_domain_t domain, esp_sleep_pd_option_t option)
Set power down mode for an RTC power domain in sleep mode. 
If not set set using this API, all power domains default to ESP_PD_OPTION_AUTO.

Parameters

domain -- power domain to configure 
option -- power down option (ESP_PD_OPTION_OFF, ESP_PD_OPTION_ON, or ESP_PD_OPTION_AUTO) 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if either of the arguments is out of range 





```

#### esp_deep_sleep_try_to_start


```


esp_err_t esp_deep_sleep_try_to_start(void)
Enter deep sleep with the configured wakeup options. 

The reason for the rejection can be such as a short sleep time.

Note
In general, the function does not return, but if the sleep is rejected, then it returns from it.


Returns

No return - If the sleep is not rejected.
ESP_ERR_SLEEP_REJECT sleep request is rejected(wakeup source set before the sleep request) 





```

#### esp_deep_sleep_start


```


void esp_deep_sleep_start(void)
Enter deep sleep with the configured wakeup options. 

Note
The function does not do a return (no rejection). Even if wakeup source set before the sleep request it goes to deep sleep anyway. 


```

#### esp_light_sleep_start


```


esp_err_t esp_light_sleep_start(void)
Enter light sleep with the configured wakeup options. 

Returns

ESP_OK on success (returned after wakeup)
ESP_ERR_SLEEP_REJECT sleep request is rejected(wakeup source set before the sleep request)
ESP_ERR_SLEEP_TOO_SHORT_SLEEP_DURATION after deducting the sleep flow overhead, the final sleep duration is too short to cover the minimum sleep duration of the chip, when rtc timer wakeup source enabled 





```

#### esp_deep_sleep_try


```


esp_err_t esp_deep_sleep_try(uint64_t time_in_us)
Enter deep-sleep mode. 
The device will automatically wake up after the deep-sleep time Upon waking up, the device calls deep sleep wake stub, and then proceeds to load application.
Call to this function is equivalent to a call to esp_deep_sleep_enable_timer_wakeup followed by a call to esp_deep_sleep_start.

The reason for the rejection can be such as a short sleep time.

Note
In general, the function does not return, but if the sleep is rejected, then it returns from it.


Parameters
time_in_us -- deep-sleep time, unit: microsecond

Returns

No return - If the sleep is not rejected.
ESP_ERR_SLEEP_REJECT sleep request is rejected(wakeup source set before the sleep request) 





```

#### esp_deep_sleep


```


void esp_deep_sleep(uint64_t time_in_us)
Enter deep-sleep mode. 
The device will automatically wake up after the deep-sleep time Upon waking up, the device calls deep sleep wake stub, and then proceeds to load application.
Call to this function is equivalent to a call to esp_deep_sleep_enable_timer_wakeup followed by a call to esp_deep_sleep_start.

Note
The function does not do a return (no rejection).. Even if wakeup source set before the sleep request it goes to deep sleep anyway.


Parameters
time_in_us -- deep-sleep time, unit: microsecond 



```

#### esp_deep_sleep_register_hook


```


esp_err_t esp_deep_sleep_register_hook(esp_deep_sleep_cb_t new_dslp_cb)
Register a callback to be called from the deep sleep prepare. 

Warning
deepsleep callbacks should without parameters, and MUST NOT, UNDER ANY CIRCUMSTANCES, CALL A FUNCTION THAT MIGHT BLOCK.


Parameters
new_dslp_cb -- Callback to be called

Returns

ESP_OK: Callback registered to the deepsleep misc_modules_sleep_prepare
ESP_ERR_NO_MEM: No more hook space for register the callback 





```

#### esp_deep_sleep_deregister_hook


```


void esp_deep_sleep_deregister_hook(esp_deep_sleep_cb_t old_dslp_cb)
Unregister an deepsleep callback. 

Parameters
old_dslp_cb -- Callback to be unregistered 



```

#### esp_sleep_get_wakeup_cause


```


esp_sleep_wakeup_cause_t esp_sleep_get_wakeup_cause(void)
Get the wakeup source which caused wakeup from sleep. 

Returns
cause of wake up from last sleep (deep sleep or light sleep) 



```

#### esp_wake_deep_sleep


```


void esp_wake_deep_sleep(void)
Default stub to run on wake from deep sleep. 
Allows for executing code immediately on wake from sleep, before the software bootloader or ESP-IDF app has started up.
This function is weak-linked, so you can implement your own version to run code immediately when the chip wakes from sleep.
See docs/deep-sleep-stub.rst for details. 

```

#### esp_set_deep_sleep_wake_stub


```


void esp_set_deep_sleep_wake_stub(esp_deep_sleep_wake_stub_fn_t new_stub)
Install a new stub at runtime to run on wake from deep sleep. 
If implementing esp_wake_deep_sleep() then it is not necessary to call this function.
However, it is possible to call this function to substitute a different deep sleep stub. Any function used as a deep sleep stub must be marked RTC_IRAM_ATTR, and must obey the same rules given for esp_wake_deep_sleep(). 

```

#### esp_set_deep_sleep_wake_stub_default_entry


```


void esp_set_deep_sleep_wake_stub_default_entry(void)
Set wake stub entry to default esp_wake_stub_entry

```

#### esp_get_deep_sleep_wake_stub


```


esp_deep_sleep_wake_stub_fn_t esp_get_deep_sleep_wake_stub(void)
Get current wake from deep sleep stub. 

Returns
Return current wake from deep sleep stub, or NULL if no stub is installed. 



```

#### esp_default_wake_deep_sleep


```


void esp_default_wake_deep_sleep(void)
The default esp-idf-provided esp_wake_deep_sleep() stub. 
See docs/deep-sleep-stub.rst for details. 

```

#### esp_deep_sleep_disable_rom_logging


```


void esp_deep_sleep_disable_rom_logging(void)
Disable logging from the ROM code after deep sleep. 
Using LSB of RTC_STORE4. 

```

#### esp_sleep_cpu_pd_low_init


```


esp_err_t esp_sleep_cpu_pd_low_init(void)
CPU Power down low-level initialize, enable CPU power down during light sleep. 

Returns

ESP_OK on success
ESP_ERR_NO_MEM not enough retention memory 





```

#### esp_sleep_cpu_pd_low_deinit


```


esp_err_t esp_sleep_cpu_pd_low_deinit(void)
CPU Power down low-level deinitialize, disable CPU power down during light sleep. 

Returns

ESP_OK on success
ESP_ERR_NO_MEM not enough retention memory 





```

#### esp_sleep_cpu_retention_init


```


esp_err_t esp_sleep_cpu_retention_init(void)
CPU Power down initialize. 

Returns

ESP_OK on success
ESP_ERR_NO_MEM not enough retention memory 





```

#### esp_sleep_cpu_retention_deinit


```


esp_err_t esp_sleep_cpu_retention_deinit(void)
CPU Power down de-initialize. 

Release system retention memory. 

Returns

ESP_OK on success





```

#### esp_sleep_config_gpio_isolate


```


void esp_sleep_config_gpio_isolate(void)
Configure to isolate all GPIO pins in sleep state. 

```

#### esp_sleep_enable_gpio_switch


```


void esp_sleep_enable_gpio_switch(bool enable)
Enable or disable GPIO pins status switching between slept status and waked status. 

Parameters
enable -- decide whether to switch status or not 



```



### Macros


#### ESP_PD_DOMAIN_RTC8M


```


ESP_PD_DOMAIN_RTC8M

```

#### ESP_SLEEP_POWER_DOWN_CPU


```


ESP_SLEEP_POWER_DOWN_CPU

```



### Type Definitions


#### esp_deep_sleep_cb_t


```


typedef void (*esp_deep_sleep_cb_t)(void)

```

#### esp_sleep_wakeup_cause_t


```


typedef esp_sleep_source_t esp_sleep_wakeup_cause_t

```

#### esp_deep_sleep_wake_stub_fn_t


```


typedef void (*esp_deep_sleep_wake_stub_fn_t)(void)
Function type for stub to run on wake from sleep. 

```



### Enumerations


#### esp_sleep_ext1_wakeup_mode_t


```


enum esp_sleep_ext1_wakeup_mode_t
Logic function used for EXT1 wakeup mode. 
Values:
ESP_EXT1_WAKEUP_ANY_LOW

enumerator ESP_EXT1_WAKEUP_ANY_LOW
Wake the chip when any of the selected GPIOs go low. 

ESP_EXT1_WAKEUP_ANY_HIGH

enumerator ESP_EXT1_WAKEUP_ANY_HIGH
Wake the chip when any of the selected GPIOs go high. 

ESP_EXT1_WAKEUP_ALL_LOW

enumerator ESP_EXT1_WAKEUP_ALL_LOW


```

#### esp_sleep_pd_domain_t


```


enum esp_sleep_pd_domain_t
Power domains which can be powered down in sleep mode. 
Values:
ESP_PD_DOMAIN_RTC_PERIPH

enumerator ESP_PD_DOMAIN_RTC_PERIPH
RTC IO, sensors and ULP co-processor. 

ESP_PD_DOMAIN_XTAL

enumerator ESP_PD_DOMAIN_XTAL
XTAL oscillator. 

ESP_PD_DOMAIN_RC_FAST

enumerator ESP_PD_DOMAIN_RC_FAST
Internal Fast oscillator. 

ESP_PD_DOMAIN_CPU

enumerator ESP_PD_DOMAIN_CPU
CPU core. 

ESP_PD_DOMAIN_VDDSDIO

enumerator ESP_PD_DOMAIN_VDDSDIO
VDD_SDIO. 

ESP_PD_DOMAIN_MODEM

enumerator ESP_PD_DOMAIN_MODEM
MODEM, includes WiFi, Bluetooth and IEEE802.15.4. 

ESP_PD_DOMAIN_MAX

enumerator ESP_PD_DOMAIN_MAX
Number of domains. 


```

#### esp_sleep_pd_option_t


```


enum esp_sleep_pd_option_t
Power down options. 
Values:
ESP_PD_OPTION_OFF

enumerator ESP_PD_OPTION_OFF
Power down the power domain in sleep mode. 

ESP_PD_OPTION_ON

enumerator ESP_PD_OPTION_ON
Keep power domain enabled during sleep mode. 

ESP_PD_OPTION_AUTO

enumerator ESP_PD_OPTION_AUTO
Keep power domain enabled in sleep mode, if it is needed by one of the wakeup options. Otherwise power it down. 


```

#### esp_sleep_source_t


```


enum esp_sleep_source_t
Sleep wakeup cause. 
Values:
ESP_SLEEP_WAKEUP_UNDEFINED

enumerator ESP_SLEEP_WAKEUP_UNDEFINED
In case of deep sleep, reset was not caused by exit from deep sleep. 

ESP_SLEEP_WAKEUP_ALL

enumerator ESP_SLEEP_WAKEUP_ALL
Not a wakeup cause, used to disable all wakeup sources with esp_sleep_disable_wakeup_source. 

ESP_SLEEP_WAKEUP_EXT0

enumerator ESP_SLEEP_WAKEUP_EXT0
Wakeup caused by external signal using RTC_IO. 

ESP_SLEEP_WAKEUP_EXT1

enumerator ESP_SLEEP_WAKEUP_EXT1
Wakeup caused by external signal using RTC_CNTL. 

ESP_SLEEP_WAKEUP_TIMER

enumerator ESP_SLEEP_WAKEUP_TIMER
Wakeup caused by timer. 

ESP_SLEEP_WAKEUP_TOUCHPAD

enumerator ESP_SLEEP_WAKEUP_TOUCHPAD
Wakeup caused by touchpad. 

ESP_SLEEP_WAKEUP_ULP

enumerator ESP_SLEEP_WAKEUP_ULP
Wakeup caused by ULP program. 

ESP_SLEEP_WAKEUP_GPIO

enumerator ESP_SLEEP_WAKEUP_GPIO
Wakeup caused by GPIO (light sleep only on ESP32, S2 and S3) 

ESP_SLEEP_WAKEUP_UART

enumerator ESP_SLEEP_WAKEUP_UART
Wakeup caused by UART (light sleep only) 

ESP_SLEEP_WAKEUP_WIFI

enumerator ESP_SLEEP_WAKEUP_WIFI
Wakeup caused by WIFI (light sleep only) 

ESP_SLEEP_WAKEUP_COCPU

enumerator ESP_SLEEP_WAKEUP_COCPU
Wakeup caused by COCPU int. 

ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG

enumerator ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG
Wakeup caused by COCPU crash. 

ESP_SLEEP_WAKEUP_BT

enumerator ESP_SLEEP_WAKEUP_BT
Wakeup caused by BT (light sleep only) 


```

#### esp_sleep_mode_t


```


enum esp_sleep_mode_t
Sleep mode. 
Values:
ESP_SLEEP_MODE_LIGHT_SLEEP

enumerator ESP_SLEEP_MODE_LIGHT_SLEEP
light sleep mode 

ESP_SLEEP_MODE_DEEP_SLEEP

enumerator ESP_SLEEP_MODE_DEEP_SLEEP
deep sleep mode 


```

#### [anonymous]


```


enum [anonymous]
Values:
ESP_ERR_SLEEP_REJECT

enumerator ESP_ERR_SLEEP_REJECT

ESP_ERR_SLEEP_TOO_SHORT_SLEEP_DURATION

enumerator ESP_ERR_SLEEP_TOO_SHORT_SLEEP_DURATION


```