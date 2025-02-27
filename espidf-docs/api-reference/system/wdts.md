# Watchdogs




## Overview


ESP-IDF supports multiple types of watchdogs:


* Hardware Watchdog Timers
* Interrupt Watchdog Timer (IWDT)
* Task Watchdog Timer (TWDT)
* XTAL32K Watchdog Timer (Crystal 32K Watchdog Timer, i.e., XTWDT)





The Interrupt Watchdog is responsible for ensuring that ISRs (Interrupt Service Routines) are not blocked for a prolonged period of time. The TWDT is responsible for detecting instances of tasks running without yielding for a prolonged period.


The various watchdog timers can be enabled using the Project Configuration Menu. However, the TWDT can also be enabled during runtime.




## Hardware Watchdog Timers


The chips have two groups of watchdog timers:


* Main System Watchdog Timer (MWDT_WDT) - used by Interrupt Watchdog Timer (IWDT) and Task Watchdog Timer (TWDT).
* RTC Watchdog Timer (RTC_WDT) - used to track the boot time from power-up until the user's main function (by default RTC Watchdog is disabled immediately before the user's main function).





Refer to the Watchdog section to understand how watchdogs are utilized in the bootloader.


The app's behaviour can be adjusted so the RTC Watchdog remains enabled after app startup. The Watchdog would need to be explicitly reset (i.e., fed) or disabled by the app to avoid the chip reset. To do this, set the CONFIG_BOOTLOADER_WDT_DISABLE_IN_USER_CODE option, modify the app as needed, and then recompile the app. In this case, the following APIs should be used:


* `wdt_hal_disable()`: see To Disable RTC_WDT
* `wdt_hal_feed()`: see To Reset the RTC_WDT Counter





If RTC_WDT is not reset/disabled in time, the chip will be automatically reset. See RTC Watchdog Timeout for more information.




## Interrupt Watchdog Timer (IWDT)


The purpose of the IWDT is to ensure that interrupt service routines (ISRs) are not blocked from running for a prolonged period of time (i.e., the IWDT timeout period). Preventing ISRs from running in a timely manner is undesirable as it can increase ISR latency, and also prevent task switching (as task switching is executed form an ISR). The things that can block ISRs from running include:


* Disabling interrupts
* Critical Sections (also disables interrupts)
* Other same/higher priority ISRs which block same/lower priority ISRs from running


The IWDT utilizes the MWDT_WDT watchdog timer in Timer Group 1 as its underlying hardware timer and leverages the FreeRTOS tick interrupt on each CPU to feed the watchdog timer. If the tick interrupt on a particular CPU is not run at within the IWDT timeout period, it is indicative that something is blocking ISRs from being run on that CPU (see the list of reasons above).


When the IWDT times out, the default action is to invoke the panic handler and display the panic reason as `Interrupt wdt timeout on CPU0` or `Interrupt wdt timeout on CPU1` (as applicable). Depending on the panic handler's configured behavior (see CONFIG_ESP_SYSTEM_PANIC), users can then debug the source of the IWDT timeout (via the backtrace, OpenOCD, gdbstub etc) or simply reset the chip (which may be preferred in a production environment).


If for whatever reason the panic handler is unable to run after an IWDT timeout, the IWDT has a second stage timeout that will hard-reset the chip (i.e., a system reset).



### Configuration


* The IWDT is enabled by default via the CONFIG_ESP_INT_WDT option.
* The IWDT's timeout is configured by setting the CONFIG_ESP_INT_WDT_TIMEOUT_MS option.



> + Note that the default timeout is higher if PSRAM support is enabled, as a critical section or interrupt routine that accesses a large amount of PSRAM takes longer to complete in some circumstances.
> 	+ The timeout should always at least twice longer than the period between FreeRTOS ticks (see CONFIG_FREERTOS_HZ).




### Tuning


If you find the IWDT timeout is triggered because an interrupt or critical section is running longer than the timeout period, consider rewriting the code:


* Critical sections should be made as short as possible. Any non-critical code/computation should be placed outside the critical section.
* Interrupt handlers should also perform the minimum possible amount of computation. Users can consider deferring any computation to a task by having the ISR push data to a task using queues.


Neither critical sections or interrupt handlers should ever block waiting for another event to occur. If changing the code to reduce the processing time is not possible or desirable, it is possible to increase the CONFIG_ESP_INT_WDT_TIMEOUT_MS setting instead.





## Task Watchdog Timer (TWDT)


The Task Watchdog Timer (TWDT) is used to monitor particular tasks, ensuring that they are able to execute within a given timeout period. The TWDT primarily watches the Idle Tasks of each CPU, however any task can subscribe to be watched by the TWDT. By watching the Idle Tasks of each CPU, the TWDT can detect instances of tasks running for a prolonged period of time without yielding. This can be an indicator of poorly written code that spinloops on a peripheral, or a task that is stuck in an infinite loop.


The TWDT is built around the MWDT_WDT watchdog timer in Timer Group 0. When a timeout occurs, an interrupt is triggered.


Users can define the function `esp_task_wdt_isr_user_handler` in the user code, in order to receive the timeout event and extend the default behavior.



### Usage


The following functions can be used to watch tasks using the TWDT:


* `esp_task_wdt_init()` to initialize the TWDT and subscribe the idle tasks.
* `esp_task_wdt_add()` subscribes other tasks to the TWDT.
* Once subscribed, `esp_task_wdt_reset()` should be called from the task to feed the TWDT.
* `esp_task_wdt_delete()` unsubscribes a previously subscribed task.
* `esp_task_wdt_deinit()` unsubscribes the idle tasks and deinitializes the TWDT.


In the case where applications need to watch at a more granular level (i.e., ensure that a particular functions/stub/code-path is called), the TWDT allows subscription of `users`.


* `esp_task_wdt_add_user()` to subscribe an arbitrary user of the TWDT. This function returns a user handle to the added user.
* `esp_task_wdt_reset_user()` must be called using the user handle in order to prevent a TWDT timeout.
* `esp_task_wdt_delete_user()` unsubscribes an arbitrary user of the TWDT.




### Configuration


The default timeout period for the TWDT is set using config item CONFIG_ESP_TASK_WDT_TIMEOUT_S. This should be set to at least as long as you expect any single task needs to monopolize the CPU (for example, if you expect the app will do a long intensive calculation and should not yield to other tasks). It is also possible to change this timeout at runtime by calling `esp_task_wdt_init()`.



Note


Erasing large flash areas can be time consuming and can cause a task to run continuously, thus triggering a TWDT timeout. The following two methods can be used to avoid this:


* Increase CONFIG_ESP_TASK_WDT_TIMEOUT_S in menuconfig for a larger watchdog timeout period.
* You can also call `esp_task_wdt_init()` to increase the watchdog timeout period before erasing a large flash area.


For more information, you can refer to SPI Flash API.



The following config options control TWDT configuration. They are all enabled by default:


* CONFIG_ESP_TASK_WDT_EN - enables TWDT feature. If this option is disabled, TWDT cannot be used, even if initialized at runtime.
* CONFIG_ESP_TASK_WDT_INIT - the TWDT is initialized automatically during startup. If this option is disabled, it is still possible to initialize the Task WDT at runtime by calling `esp_task_wdt_init()`.
* CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0 - CPU0 Idle task is subscribed to the TWDT during startup. If this option is disabled, it is still possible to subscribe the idle task by calling `esp_task_wdt_init()` again.
* CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1 - CPU1 Idle task is subscribed to the TWDT during startup.






Note


On a TWDT timeout the default behaviour is to simply print a warning and a backtrace before continuing running the app. If you want a timeout to cause a panic and a system reset then this can be configured through CONFIG_ESP_TASK_WDT_PANIC.






## XTAL32K Watchdog Timer (XTWDT)


One of the optional clock inputs to the ESP32-S3 is an external 32 kHz crystal oscillator (XTAL32K) that is used as a clock source (`XTAL32K_CLK`) to various subsystems (such as the RTC).


The XTWDT is a dedicated watchdog timer used to ensure that the XTAL32K is functioning correctly. When `XTAL32K_CLK` works as the clock source of `RTC_SLOW_CLK` and stops oscillating, the XTWDT will detect this and generate an interrupt. It also provides functionality for automatically switching over to the internal, but less accurate oscillator as the `RTC_SLOW_CLK` source.


Since the switch to the backup clock is done in hardware it can also happen during Deep Sleep. This means that even if `XTAL32K_CLK` stops functioning while the chip is in Deep Sleep, waiting for a timer to expire, it is still able to wake-up as planned.


If the `XTAL32K_CLK` starts functioning normally again, you can call `esp_xt_wdt_restore_clk` to switch back to this clock source and re-enable the watchdog timer.



### Configuration


* When the external 32 KHz crystal or oscillator is selected (CONFIG_RTC_CLK_SRC) the XTWDT can be enabled via the CONFIG_ESP_XT_WDT configuration option.
* The timeout is configured by setting the CONFIG_ESP_XT_WDT_TIMEOUT option.
* The automatic backup clock functionality is enabled via the ref:CONFIG_ESP_XT_WDT_BACKUP_CLK_ENABLE configuration option.





## JTAG & Watchdogs


While debugging using OpenOCD, the CPUs are halted every time a breakpoint is reached. However if the watchdog timers continue to run when a breakpoint is encountered, they will eventually trigger a reset making it very difficult to debug code. Therefore OpenOCD will disable the hardware timers of both the interrupt and task watchdogs at every breakpoint. Moreover, OpenOCD will not re-enable them upon leaving the breakpoint. This means that interrupt watchdog and task watchdog functionality will essentially be disabled. No warnings or panics from either watchdogs will be generated when the ESP32-S3 is connected to OpenOCD via JTAG.




## API Reference



### Task Watchdog


A full example using the Task Watchdog is available in esp-idf: system/task_watchdog




### Header File


* components/esp_system/include/esp_task_wdt.h
* This header file can be included with:



> ```
> #include "esp_task_wdt.h"
> 
> ```




### Functions


#### esp_task_wdt_init


```


esp_err_t esp_task_wdt_init(const esp_task_wdt_config_t *config)
Initialize the Task Watchdog Timer (TWDT) 
This function configures and initializes the TWDT. This function will subscribe the idle tasks if configured to do so. For other tasks, users can subscribe them using esp_task_wdt_add() or esp_task_wdt_add_user(). This function won't start the timer if no task have been registered yet.

Note
esp_task_wdt_init() must only be called after the scheduler is started. Moreover, it must not be called by multiple tasks simultaneously. 


Parameters
config -- [in] Configuration structure 

Returns

ESP_OK: Initialization was successful
ESP_ERR_INVALID_STATE: Already initialized
Other: Failed to initialize TWDT 





```

#### esp_task_wdt_reconfigure


```


esp_err_t esp_task_wdt_reconfigure(const esp_task_wdt_config_t *config)
Reconfigure the Task Watchdog Timer (TWDT) 
The function reconfigures the running TWDT. It must already be initialized when this function is called.

Note
esp_task_wdt_reconfigure() must not be called by multiple tasks simultaneously.


Parameters
config -- [in] Configuration structure

Returns

ESP_OK: Reconfiguring was successful
ESP_ERR_INVALID_STATE: TWDT not initialized yet
Other: Failed to initialize TWDT 





```

#### esp_task_wdt_deinit


```


esp_err_t esp_task_wdt_deinit(void)
Deinitialize the Task Watchdog Timer (TWDT) 
This function will deinitialize the TWDT, and unsubscribe any idle tasks. Calling this function whilst other tasks are still subscribed to the TWDT, or when the TWDT is already deinitialized, will result in an error code being returned.

Note
esp_task_wdt_deinit() must not be called by multiple tasks simultaneously. 


Returns

ESP_OK: TWDT successfully deinitialized
Other: Failed to deinitialize TWDT 





```

#### esp_task_wdt_add


```


esp_err_t esp_task_wdt_add(TaskHandle_t task_handle)
Subscribe a task to the Task Watchdog Timer (TWDT) 
This function subscribes a task to the TWDT. Each subscribed task must periodically call esp_task_wdt_reset() to prevent the TWDT from elapsing its timeout period. Failure to do so will result in a TWDT timeout.

Parameters
task_handle -- Handle of the task. Input NULL to subscribe the current running task to the TWDT 

Returns

ESP_OK: Successfully subscribed the task to the TWDT
Other: Failed to subscribe task 





```

#### esp_task_wdt_add_user


```


esp_err_t esp_task_wdt_add_user(const char *user_name, esp_task_wdt_user_handle_t *user_handle_ret)
Subscribe a user to the Task Watchdog Timer (TWDT) 
This function subscribes a user to the TWDT. A user of the TWDT is usually a function that needs to run periodically. Each subscribed user must periodically call esp_task_wdt_reset_user() to prevent the TWDT from elapsing its timeout period. Failure to do so will result in a TWDT timeout.

Parameters

user_name -- [in] String to identify the user 
user_handle_ret -- [out] Handle of the user 


Returns

ESP_OK: Successfully subscribed the user to the TWDT
Other: Failed to subscribe user 





```

#### esp_task_wdt_reset


```


esp_err_t esp_task_wdt_reset(void)
Reset the Task Watchdog Timer (TWDT) on behalf of the currently running task. 
This function will reset the TWDT on behalf of the currently running task. Each subscribed task must periodically call this function to prevent the TWDT from timing out. If one or more subscribed tasks fail to reset the TWDT on their own behalf, a TWDT timeout will occur.

Returns

ESP_OK: Successfully reset the TWDT on behalf of the currently running task
Other: Failed to reset 





```

#### esp_task_wdt_reset_user


```


esp_err_t esp_task_wdt_reset_user(esp_task_wdt_user_handle_t user_handle)
Reset the Task Watchdog Timer (TWDT) on behalf of a user. 
This function will reset the TWDT on behalf of a user. Each subscribed user must periodically call this function to prevent the TWDT from timing out. If one or more subscribed users fail to reset the TWDT on their own behalf, a TWDT timeout will occur.

Parameters
user_handle -- [in] User handle
ESP_OK: Successfully reset the TWDT on behalf of the user
Other: Failed to reset 





```

#### esp_task_wdt_delete


```


esp_err_t esp_task_wdt_delete(TaskHandle_t task_handle)
Unsubscribes a task from the Task Watchdog Timer (TWDT) 
This function will unsubscribe a task from the TWDT. After being unsubscribed, the task should no longer call esp_task_wdt_reset().

Parameters
task_handle -- [in] Handle of the task. Input NULL to unsubscribe the current running task. 

Returns

ESP_OK: Successfully unsubscribed the task from the TWDT
Other: Failed to unsubscribe task 





```

#### esp_task_wdt_delete_user


```


esp_err_t esp_task_wdt_delete_user(esp_task_wdt_user_handle_t user_handle)
Unsubscribes a user from the Task Watchdog Timer (TWDT) 
This function will unsubscribe a user from the TWDT. After being unsubscribed, the user should no longer call esp_task_wdt_reset_user().

Parameters
user_handle -- [in] User handle 

Returns

ESP_OK: Successfully unsubscribed the user from the TWDT
Other: Failed to unsubscribe user 





```

#### esp_task_wdt_status


```


esp_err_t esp_task_wdt_status(TaskHandle_t task_handle)
Query whether a task is subscribed to the Task Watchdog Timer (TWDT) 
This function will query whether a task is currently subscribed to the TWDT, or whether the TWDT is initialized.

Parameters
task_handle -- [in] Handle of the task. Input NULL to query the current running task. 

Returns
:
ESP_OK: The task is currently subscribed to the TWDT
ESP_ERR_NOT_FOUND: The task is not subscribed
ESP_ERR_INVALID_STATE: TWDT was never initialized 





```

#### esp_task_wdt_isr_user_handler


```


void esp_task_wdt_isr_user_handler(void)
User ISR callback placeholder. 
This function is called by task_wdt_isr function (ISR for when TWDT times out). It can be defined in user code to handle TWDT events.

Note
It has the same limitations as the interrupt function. Do not use ESP_LOGx functions inside. 


```

#### esp_task_wdt_print_triggered_tasks


```


esp_err_t esp_task_wdt_print_triggered_tasks(task_wdt_msg_handler msg_handler, void *opaque, int *cpus_fail)
Prints or retrieves information about tasks/users that triggered the Task Watchdog Timeout. 
This function provides various operations to handle tasks/users that did not reset the Task Watchdog in time. It can print detailed information about these tasks/users, such as their names, associated CPUs, and whether they have been reset. Additionally, it can retrieve the total length of the printed information or the CPU affinity of the failing tasks.

Note

If msg_handler is not provided, the information will be printed to console using ESP_EARLY_LOGE.
If msg_handler is provided, the function will send the printed information to the provided message handler function.
If cpus_fail is provided, the function will store the CPU affinity of the failing tasks in the provided integer.
During the execution of this function, logging is allowed in critical sections, as TWDT timeouts are considered fatal errors. 




Parameters

msg_handler -- [in] Optional message handler function that will be called for each printed line. 
opaque -- [in] Optional pointer to opaque data that will be passed to the message handler function. 
cpus_fail -- [out] Optional pointer to an integer where the CPU affinity of the failing tasks will be stored.


Returns

ESP_OK: The function executed successfully.
ESP_FAIL: No triggered tasks were found, and thus no information was printed or retrieved.





```



### Structures


#### esp_task_wdt_config_t


```


struct esp_task_wdt_config_t
Task Watchdog Timer (TWDT) configuration structure. 

Public Members
timeout_ms

uint32_t timeout_ms
TWDT timeout duration in milliseconds 

idle_core_mask

uint32_t idle_core_mask
Bitmask of the core whose idle task should be subscribed on initialization where 1 << i means that core i's idle task will be monitored by the TWDT 

trigger_panic

bool trigger_panic
Trigger panic when timeout occurs 



```



### Type Definitions


#### esp_task_wdt_user_handle_t


```


typedef struct esp_task_wdt_user_handle_s *esp_task_wdt_user_handle_t
Task Watchdog Timer (TWDT) user handle. 

```

#### task_wdt_msg_handler


```


typedef void (*task_wdt_msg_handler)(void *opaque, const char *msg)

```