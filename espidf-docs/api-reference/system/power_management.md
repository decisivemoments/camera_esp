# Power Management




## Overview


Power management algorithm included in ESP-IDF can adjust the advanced peripheral bus (APB) frequency, CPU frequency, and put the chip into Light-sleep mode to run an application at smallest possible power consumption, given the requirements of application components.


Application components can express their requirements by creating and acquiring power management locks.


For example:


* Driver for a peripheral clocked from APB can request the APB frequency to be set to 80 MHz while the peripheral is used.
* RTOS can request the CPU to run at the highest configured frequency while there are tasks ready to run.
* A peripheral driver may need interrupts to be enabled, which means it has to request disabling Light-sleep.


Since requesting higher APB or CPU frequencies or disabling Light-sleep causes higher current consumption, please keep the usage of power management locks by components to a minimum.




## Configuration


Power management can be enabled at compile time, using the option CONFIG_PM_ENABLE.


Enabling power management features comes at the cost of increased interrupt latency. Extra latency depends on a number of factors, such as the CPU frequency, single/dual core mode, whether or not frequency switch needs to be done. Minimum extra latency is 0.2 us (when the CPU frequency is 240 MHz and frequency scaling is not enabled). Maximum extra latency is 40 us (when frequency scaling is enabled, and a switch from 40 MHz to 80 MHz is performed on interrupt entry).


Dynamic frequency scaling (DFS) and automatic Light-sleep can be enabled in an application by calling the function `esp_pm_configure()`. Its argument is a structure defining the frequency scaling settings, `esp_pm_config_t`. In this structure, three fields need to be initialized:



> * `max_freq_mhz`: Maximum CPU frequency in MHz, i.e., the frequency used when the `ESP_PM_CPU_FREQ_MAX` lock is acquired. This field is usually set to the default CPU frequency.


* `min_freq_mhz`: Minimum CPU frequency in MHz, indicating the frequency used when not holding the power management lock.


	+ `light_sleep_enable`: Whether the system should automatically enter Light-sleep when no locks are acquired (`true`/`false`).


Alternatively, if you enable the option CONFIG_PM_DFS_INIT_AUTO in menuconfig, the maximum CPU frequency will be determined by the CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ setting, and the minimum CPU frequency will be locked to the XTAL frequency.






Note


Automatic Light-sleep is based on FreeRTOS Tickless Idle functionality. If automatic Light-sleep is requested while the option CONFIG_FREERTOS_USE_TICKLESS_IDLE is not enabled in menuconfig, `esp_pm_configure()` will return the error ESP_ERR_NOT_SUPPORTED.




Note


In Light-sleep, peripherals are clock gated, and interrupts (from GPIOs and internal peripherals) will not be generated. A wakeup source described in the Sleep Modes documentation can be used to trigger wakeup from the Light-sleep state.



For example, the EXT0 and EXT1 wakeup sources can be used to wake up the chip via a GPIO.




## Power Management Locks


Applications have the ability to acquire/release locks in order to control the power management algorithm. When an application acquires a lock, the power management algorithm operation is restricted in a way described below. When the lock is released, such restrictions are removed.


Power management locks have acquire/release counters. If the lock has been acquired a number of times, it needs to be released the same number of times to remove associated restrictions.


ESP32-S3 supports three types of locks described in the table below.







| Lock | Description |
| --- | --- |
| `ESP_PM_CPU_FREQ_MAX` | Requests CPU frequency to be at the maximum value set with `esp_pm_configure()`. For ESP32-S3, this value can be set to 80 MHz, 160 MHz, or 240 MHz. |
| `ESP_PM_APB_FREQ_MAX` | Requests the APB frequency to be at the maximum supported value. For ESP32-S3, this is 80 MHz. |
| `ESP_PM_NO_LIGHT_SLEEP` | Disables automatic switching to Light-sleep. |




## ESP32-S3 Power Management Algorithm


The table below shows how CPU and APB frequencies will be switched if dynamic frequency scaling is enabled. You can specify the maximum CPU frequency with either `esp_pm_configure()` or CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ.








| Max CPU Frequency Set | Lock Acquisition | CPU and APB Frequencies |
| --- | --- | --- |
| 240 | `ESP_PM_CPU_FREQ_MAX` acquired | CPU: 240 MHz APB: 80 MHz |
| `ESP_PM_APB_FREQ_MAX` acquired, `ESP_PM_CPU_FREQ_MAX` not acquired | CPU: 80 MHz APB: 80 MHz |
| None | Min values for both frequencies set with `esp_pm_configure()` |
| 160 | `ESP_PM_CPU_FREQ_MAX` acquired | CPU: 160 MHz APB: 80 MHz |
| `ESP_PM_APB_FREQ_MAX` acquired, `ESP_PM_CPU_FREQ_MAX` not acquired | CPU: 80 MHz APB: 80 MHz |
| None | Min values for both frequencies set with `esp_pm_configure()` |
| 80 | Any of `ESP_PM_CPU_FREQ_MAX` or `ESP_PM_APB_FREQ_MAX` acquired | CPU: 80 MHz APB: 80 MHz |
| None | Min values for both frequencies set with `esp_pm_configure()` |


If none of the locks are acquired, and Light-sleep is enabled in a call to `esp_pm_configure()`, the system will go into Light-sleep mode. The duration of Light-sleep will be determined by:


* FreeRTOS tasks blocked with finite timeouts
* Timers registered with High resolution timer APIs


Light-sleep duration is chosen to wake up the chip before the nearest event (task being unblocked, or timer elapses).


To skip unnecessary wake-up, you can consider initializing an `esp_timer` with the `skip_unhandled_events` option as `true`. Timers with this flag will not wake up the system and it helps to reduce consumption.




## Dynamic Frequency Scaling and Peripheral Drivers


When DFS is enabled, the APB frequency can be changed multiple times within a single RTOS tick. The APB frequency change does not affect the operation of some peripherals, while other peripherals may have issues. For example, Timer Group peripheral timers keeps counting, however, the speed at which they count changes proportionally to the APB frequency.


Peripheral clock sources such as `REF_TICK`, `XTAL`, `RC_FAST` (i.e., `RTC_8M`), their frequencies will not be inflenced by APB frequency. And therefore, to ensure the peripheral behaves consistently during DFS, it is recommended to select one of these clocks as the peripheral clock source. For more specific guidelines, please refer to the "Power Management" section of each peripheral's "API Reference > Peripherals API" page.


Currently, the following peripheral drivers are aware of DFS and use the `ESP_PM_APB_FREQ_MAX` lock for the duration of the transaction:


* SPI master
* I2C
* I2S (If the APLL clock is used, then it will use the `ESP_PM_NO_LIGHT_SLEEP` lock)
* SDMMC


The following drivers hold the `ESP_PM_APB_FREQ_MAX` lock while the driver is enabled:


* **SPI slave**: between calls to `spi_slave_initialize()` and `spi_slave_free()`.
* **GPTimer**: between calls to `gptimer_enable()` and `gptimer_disable()`.
* **Ethernet**: between calls to `esp_eth_driver_install()` and `esp_eth_driver_uninstall()`.
* **WiFi**: between calls to `esp_wifi_start()` and `esp_wifi_stop()`. If modem sleep is enabled, the lock will be released for the periods of time when radio is disabled.
* **TWAI**: between calls to `twai_driver_install()` and `twai_driver_uninstall()` (only when the clock source is set to `TWAI_CLK_SRC_APB`).
* **Bluetooth**: between calls to `esp_bt_controller_enable()` and `esp_bt_controller_disable()`. If Bluetooth Modem-sleep is enabled, the `ESP_PM_APB_FREQ_MAX` lock will be released for the periods of time when radio is disabled. However the `ESP_PM_NO_LIGHT_SLEEP` lock will still be held.





The following peripheral drivers are not aware of DFS yet. Applications need to acquire/release locks themselves, when necessary:


* PCNT
* Sigma-delta
* The legacy timer group driver
* MCPWM







## Light-sleep Peripheral Power Down




## API Reference



### Header File


* components/esp_pm/include/esp_pm.h
* This header file can be included with:



> ```
> #include "esp_pm.h"
> 
> ```
* This header file is a part of the API provided by the `esp_pm` component. To declare that your component depends on `esp_pm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_pm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_pm
> 
> ```




### Functions


#### esp_pm_configure


```


esp_err_t esp_pm_configure(const void *config)
Set implementation-specific power management configuration. 

Parameters
config -- pointer to implementation-specific configuration structure (e.g. esp_pm_config_esp32) 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if the configuration values are not correct
ESP_ERR_NOT_SUPPORTED if certain combination of values is not supported, or if CONFIG_PM_ENABLE is not enabled in sdkconfig 





```

#### esp_pm_get_configuration


```


esp_err_t esp_pm_get_configuration(void *config)
Get implementation-specific power management configuration. 

Parameters
config -- pointer to implementation-specific configuration structure (e.g. esp_pm_config_esp32) 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if the pointer is null 





```

#### esp_pm_lock_create


```


esp_err_t esp_pm_lock_create(esp_pm_lock_type_t lock_type, int arg, const char *name, esp_pm_lock_handle_t *out_handle)
Initialize a lock handle for certain power management parameter. 
When lock is created, initially it is not taken. Call esp_pm_lock_acquire to take the lock.
This function must not be called from an ISR.

Note
If the lock_type argument is not valid, it will cause an abort.


Parameters

lock_type -- Power management constraint which the lock should control 
arg -- argument, value depends on lock_type, see esp_pm_lock_type_t 
name -- arbitrary string identifying the lock (e.g. "wifi" or "spi"). Used by the esp_pm_dump_locks function to list existing locks. May be set to NULL. If not set to NULL, must point to a string which is valid for the lifetime of the lock. 
out_handle -- [out] handle returned from this function. Use this handle when calling esp_pm_lock_delete, esp_pm_lock_acquire, esp_pm_lock_release. Must not be NULL.


Returns

ESP_OK on success
ESP_ERR_NO_MEM if the lock structure can not be allocated
ESP_ERR_INVALID_ARG if out_handle is NULL
ESP_ERR_NOT_SUPPORTED if CONFIG_PM_ENABLE is not enabled in sdkconfig 





```

#### esp_pm_lock_acquire


```


esp_err_t esp_pm_lock_acquire(esp_pm_lock_handle_t handle)
Take a power management lock. 
Once the lock is taken, power management algorithm will not switch to the mode specified in a call to esp_pm_lock_create, or any of the lower power modes (higher numeric values of 'mode').
The lock is recursive, in the sense that if esp_pm_lock_acquire is called a number of times, esp_pm_lock_release has to be called the same number of times in order to release the lock.
This function may be called from an ISR.
This function is not thread-safe w.r.t. calls to other esp_pm_lock_* functions for the same handle.

Parameters
handle -- handle obtained from esp_pm_lock_create function 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if the handle is invalid
ESP_ERR_NOT_SUPPORTED if CONFIG_PM_ENABLE is not enabled in sdkconfig 





```

#### esp_pm_lock_release


```


esp_err_t esp_pm_lock_release(esp_pm_lock_handle_t handle)
Release the lock taken using esp_pm_lock_acquire. 
Call to this functions removes power management restrictions placed when taking the lock.
Locks are recursive, so if esp_pm_lock_acquire is called a number of times, esp_pm_lock_release has to be called the same number of times in order to actually release the lock.
This function may be called from an ISR.
This function is not thread-safe w.r.t. calls to other esp_pm_lock_* functions for the same handle.

Parameters
handle -- handle obtained from esp_pm_lock_create function 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if the handle is invalid
ESP_ERR_INVALID_STATE if lock is not acquired
ESP_ERR_NOT_SUPPORTED if CONFIG_PM_ENABLE is not enabled in sdkconfig 





```

#### esp_pm_lock_delete


```


esp_err_t esp_pm_lock_delete(esp_pm_lock_handle_t handle)
Delete a lock created using esp_pm_lock. 
The lock must be released before calling this function.
This function must not be called from an ISR.

Parameters
handle -- handle obtained from esp_pm_lock_create function 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if the handle argument is NULL
ESP_ERR_INVALID_STATE if the lock is still acquired
ESP_ERR_NOT_SUPPORTED if CONFIG_PM_ENABLE is not enabled in sdkconfig 





```

#### esp_pm_dump_locks


```


esp_err_t esp_pm_dump_locks(FILE *stream)
Dump the list of all locks to stderr
This function dumps debugging information about locks created using esp_pm_lock_create to an output stream.
This function must not be called from an ISR. If esp_pm_lock_acquire/release are called while this function is running, inconsistent results may be reported.

Parameters
stream -- stream to print information to; use stdout or stderr to print to the console; use fmemopen/open_memstream to print to a string buffer. 

Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if CONFIG_PM_ENABLE is not enabled in sdkconfig 





```



### Structures


#### esp_pm_config_t


```


struct esp_pm_config_t
Power management config. 
Pass a pointer to this structure as an argument to esp_pm_configure function. 

Public Members
max_freq_mhz

int max_freq_mhz
Maximum CPU frequency, in MHz 

min_freq_mhz

int min_freq_mhz
Minimum CPU frequency to use when no locks are taken, in MHz 

light_sleep_enable

bool light_sleep_enable
Enter light sleep when no locks are taken 



```



### Type Definitions


#### esp_pm_config_esp32_t


```


typedef esp_pm_config_t esp_pm_config_esp32_t
backward compatibility newer chips no longer require this typedef 

```

#### esp_pm_config_esp32s2_t


```


typedef esp_pm_config_t esp_pm_config_esp32s2_t

```

#### esp_pm_config_esp32s3_t


```


typedef esp_pm_config_t esp_pm_config_esp32s3_t

```

#### esp_pm_config_esp32c3_t


```


typedef esp_pm_config_t esp_pm_config_esp32c3_t

```

#### esp_pm_config_esp32c2_t


```


typedef esp_pm_config_t esp_pm_config_esp32c2_t

```

#### esp_pm_config_esp32c6_t


```


typedef esp_pm_config_t esp_pm_config_esp32c6_t

```

#### esp_pm_lock_handle_t


```


typedef struct esp_pm_lock *esp_pm_lock_handle_t
Opaque handle to the power management lock. 

```



### Enumerations


#### esp_pm_lock_type_t


```


enum esp_pm_lock_type_t
Power management constraints. 
Values:
ESP_PM_CPU_FREQ_MAX

enumerator ESP_PM_CPU_FREQ_MAX
Require CPU frequency to be at the maximum value set via esp_pm_configure. Argument is unused and should be set to 0. 

ESP_PM_APB_FREQ_MAX

enumerator ESP_PM_APB_FREQ_MAX
Require APB frequency to be at the maximum value supported by the chip. Argument is unused and should be set to 0. 

ESP_PM_NO_LIGHT_SLEEP

enumerator ESP_PM_NO_LIGHT_SLEEP
Prevent the system from going into light sleep. Argument is unused and should be set to 0. 

ESP_PM_LOCK_MAX

enumerator ESP_PM_LOCK_MAX


```