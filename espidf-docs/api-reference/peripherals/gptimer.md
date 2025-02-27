# General Purpose Timer (GPTimer)



## Introduction


GPTimer (General Purpose Timer) is the driver of ESP32-S3 Timer Group peripheral. The hardware timer features high resolution and flexible alarm action. The behavior when the internal counter of a timer reaches a specific target value is called a timer alarm. When a timer alarms, a user registered per-timer callback would be called.


Typically, a general purpose timer can be used in scenarios like:


* Free running as a wall clock, fetching a high-resolution timestamp at any time and any places
* Generate period alarms, trigger events periodically
* Generate one-shot alarm, respond in target time




## Functional Overview


The following sections of this document cover the typical steps to install and operate a timer:


* Resource Allocation - covers which parameters should be set up to get a timer handle and how to recycle the resources when GPTimer finishes working.
* Set and Get Count Value - covers how to force the timer counting from a start point and how to get the count value at anytime.
* Set up Alarm Action - covers the parameters that should be set up to enable the alarm event.
* Register Event Callbacks - covers how to hook user specific code to the alarm event callback function.
* Enable and Disable Timer - covers how to enable and disable the timer.
* Start and Stop Timer - shows some typical use cases that start the timer with different alarm behavior.
* Power Management - describes how different source clock selections can affect power consumption.
* IRAM Safe - describes tips on how to make the timer interrupt and IO control functions work better along with a disabled cache.
* Thread Safety - lists which APIs are guaranteed to be thread safe by the driver.
* Kconfig Options - lists the supported Kconfig options that can be used to make a different effect on driver behavior.






### Resource Allocation


Different ESP chips might have different numbers of independent timer groups, and within each group, there could also be several independent timers. 1


A GPTimer instance is represented by `gptimer_handle_t`. The driver behind manages all available hardware resources in a pool, so that you do not need to care about which timer and which group it belongs to.


To install a timer instance, there is a configuration structure that needs to be given in advance: `gptimer_config_t`:


* `gptimer_config_t::clk_src` selects the source clock for the timer. The available clocks are listed in `gptimer_clock_source_t`, you can only pick one of them. For the effect on power consumption of different clock source, please refer to Section Power Management.
* `gptimer_config_t::direction` sets the counting direction of the timer, supported directions are listed in `gptimer_count_direction_t`, you can only pick one of them.
* `gptimer_config_t::resolution_hz` sets the resolution of the internal counter. Each count step is equivalent to **1 / resolution_hz** seconds.
* `gptimer_config::intr_priority` sets the priority of the timer interrupt. If it is set to `0`, the driver will allocate an interrupt with a default priority. Otherwise, the driver will use the given priority.
* Optional `gptimer_config_t::intr_shared` sets whether or not mark the timer interrupt source as a shared one. For the pros/cons of a shared interrupt, you can refer to Interrupt Handling.


With all the above configurations set in the structure, the structure can be passed to `gptimer_new_timer()` which will instantiate the timer instance and return a handle of the timer.


The function can fail due to various errors such as insufficient memory, invalid arguments, etc. Specifically, when there are no more free timers (i.e., all hardware resources have been used up), then `ESP_ERR_NOT_FOUND` will be returned. The total number of available timers is represented by the `SOC_TIMER_GROUP_TOTAL_TIMERS` and its value depends on the ESP chip.


If a previously created GPTimer instance is no longer required, you should recycle the timer by calling `gptimer_del_timer()`. This allows the underlying HW timer to be used for other purposes. Before deleting a GPTimer handle, please disable it by `gptimer_disable()` in advance or make sure it has not enabled yet by `gptimer_enable()`.



#### Creating a GPTimer Handle with Resolution of 1 MHz



```
gptimer_handle_t gptimer = NULL;
gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT,
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1 * 1000 * 1000, // 1MHz, 1 tick = 1us
};
ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

```





### Set and Get Count Value


When the GPTimer is created, the internal counter will be reset to zero by default. The counter value can be updated asynchronously by `gptimer_set_raw_count()`. The maximum count value is dependent on the bit width of the hardware timer, which is also reflected by the SOC macro `SOC_TIMER_GROUP_COUNTER_BIT_WIDTH`. When updating the raw count of an active timer, the timer will immediately start counting from the new value.


Count value can be retrieved by `gptimer_get_raw_count()`, at any time.




### Set up Alarm Action


For most of the use cases of GPTimer, you should set up the alarm action before starting the timer, except for the simple wall-clock scenario, where a free running timer is enough. To set up the alarm action, you should configure several members of `gptimer_alarm_config_t` based on how you make use of the alarm event:


* `gptimer_alarm_config_t::alarm_count` sets the target count value that triggers the alarm event. You should also take the counting direction into consideration when setting the alarm value. Specially, `gptimer_alarm_config_t::alarm_count` and `gptimer_alarm_config_t::reload_count` cannot be set to the same value when `gptimer_alarm_config_t::auto_reload_on_alarm` is true, as keeping reload with a target alarm count is meaningless.
* `gptimer_alarm_config_t::reload_count` sets the count value to be reloaded when the alarm event happens. This configuration only takes effect when `gptimer_alarm_config_t::auto_reload_on_alarm` is set to true.
* `gptimer_alarm_config_t::auto_reload_on_alarm` flag sets whether to enable the auto-reload feature. If enabled, the hardware timer will reload the value of `gptimer_alarm_config_t::reload_count` into counter immediately when an alarm event happens.


To make the alarm configurations take effect, you should call `gptimer_set_alarm_action()`. Especially, if `gptimer_alarm_config_t` is set to `NULL`, the alarm function will be disabled.



Note


If an alarm value is set and the timer has already exceeded this value, the alarm will be triggered immediately.





### Register Event Callbacks


After the timer starts up, it can generate a specific event (e.g., the "Alarm Event") dynamically. If you have some functions that should be called when the event happens, please hook your function to the interrupt service routine by calling `gptimer_register_event_callbacks()`. All supported event callbacks are listed in `gptimer_event_callbacks_t`:


* `gptimer_event_callbacks_t::on_alarm` sets a callback function for alarm events. As this function is called within the ISR context, you must ensure that the function does not attempt to block (e.g., by making sure that only FreeRTOS APIs with `ISR` suffix are called from within the function). The function prototype is declared in `gptimer_alarm_cb_t`.


You can save your own context to `gptimer_register_event_callbacks()` as well, via the parameter `user_data`. The user data will be directly passed to the callback function.


This function lazy installs the interrupt service for the timer but not enable it. So please call this function before `gptimer_enable()`, otherwise the `ESP_ERR_INVALID_STATE` error will be returned. See Section Enable and Disable Timer for more information.




### Enable and Disable Timer


Before doing IO control to the timer, you needs to enable the timer first, by calling `gptimer_enable()`. This function:


* Switches the timer driver state from **init** to **enable**.
* Enables the interrupt service if it has been lazy installed by `gptimer_register_event_callbacks()`.
* Acquires a proper power management lock if a specific clock source (e.g., APB clock) is selected. See Section Power Management for more information.


Calling `gptimer_disable()` does the opposite, that is, put the timer driver back to the **init** state, disable the interrupts service and release the power management lock.




### Start and Stop Timer


The basic IO operation of a timer is to start and stop. Calling `gptimer_start()` can make the internal counter work, while calling `gptimer_stop()` can make the counter stop working. The following illustrates how to start a timer with or without an alarm event.


Calling `gptimer_start()` transits the driver state from **enable** to **run**, and vice versa. You need to make sure the start and stop functions are used in pairs, otherwise, the functions may return `ESP_ERR_INVALID_STATE`. Most of the time, this error means that the timer is already stopped or in the "start protection" state (i.e., `gptimer_start()` is called but not finished).



#### Start Timer as a Wall Clock



```
ESP_ERROR_CHECK(gptimer_enable(gptimer));
ESP_ERROR_CHECK(gptimer_start(gptimer));
// Retrieve the timestamp at any time
uint64_t count;
ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &count));

```




#### Trigger Period Events



```
typedef struct {
    uint64_t event_count;
} example_queue_element_t;

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // Retrieve the count value from event data
    example_queue_element_t ele = {
        .event_count = edata->count_value
    };
    // Optional: send the event data to other task by OS queue
    // Do not introduce complex logics in callbacks
    // Suggest dealing with event data in the main loop, instead of in this callback
    xQueueSendFromISR(queue, &ele, &high_task_awoken);
    // return whether we need to yield at the end of ISR
    return high_task_awoken == pdTRUE;
}

gptimer_alarm_config_t alarm_config = {
    .reload_count = 0, // counter will reload with 0 on alarm event
    .alarm_count = 1000000, // period = 1s @resolution 1MHz
    .flags.auto_reload_on_alarm = true, // enable auto-reload
};
ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

gptimer_event_callbacks_t cbs = {
    .on_alarm = example_timer_on_alarm_cb, // register user callback
};
ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, queue));
ESP_ERROR_CHECK(gptimer_enable(gptimer));
ESP_ERROR_CHECK(gptimer_start(gptimer));

```




#### Trigger One-Shot Event



```
typedef struct {
    uint64_t event_count;
} example_queue_element_t;

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // Stop timer the sooner the better
    gptimer_stop(timer);
    // Retrieve the count value from event data
    example_queue_element_t ele = {
        .event_count = edata->count_value
    };
    // Optional: send the event data to other task by OS queue
    xQueueSendFromISR(queue, &ele, &high_task_awoken);
    // return whether we need to yield at the end of ISR
    return high_task_awoken == pdTRUE;
}

gptimer_alarm_config_t alarm_config = {
    .alarm_count = 1 * 1000 * 1000, // alarm target = 1s @resolution 1MHz
};
ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

gptimer_event_callbacks_t cbs = {
    .on_alarm = example_timer_on_alarm_cb, // register user callback
};
ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, queue));
ESP_ERROR_CHECK(gptimer_enable(gptimer));
ESP_ERROR_CHECK(gptimer_start(gptimer));

```




#### Dynamic Alarm Update


Alarm value can be updated dynamically inside the ISR handler callback, by changing `gptimer_alarm_event_data_t::alarm_value`. Then the alarm value will be updated after the callback function returns.



```
typedef struct {
    uint64_t event_count;
} example_queue_element_t;

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t queue = (QueueHandle_t)user_data;
    // Retrieve the count value from event data
    example_queue_element_t ele = {
        .event_count = edata->count_value
    };
    // Optional: send the event data to other task by OS queue
    xQueueSendFromISR(queue, &ele, &high_task_awoken);
    // reconfigure alarm value
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = edata->alarm_value + 1000000, // alarm in next 1s
    };
    gptimer_set_alarm_action(timer, &alarm_config);
    // return whether we need to yield at the end of ISR
    return high_task_awoken == pdTRUE;
}

gptimer_alarm_config_t alarm_config = {
    .alarm_count = 1000000, // initial alarm target = 1s @resolution 1MHz
};
ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

gptimer_event_callbacks_t cbs = {
    .on_alarm = example_timer_on_alarm_cb, // register user callback
};
ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, queue));
ESP_ERROR_CHECK(gptimer_enable(gptimer));
ESP_ERROR_CHECK(gptimer_start(gptimer, &alarm_config));

```





### Power Management


There are some power management strategies, which might turn off or change the frequency of GPTimer's source clock to save power consumption. For example, during DFS, APB clock will be scaled down. If light-sleep is also enabled, PLL and XTAL clocks will be powered off. Both of them can result in an inaccurate time keeping.


The driver can prevent the above situation from happening by creating different power management lock according to different clock source. The driver increases the reference count of that power management lock in the `gptimer_enable()` and decrease it in the `gptimer_disable()`. So we can ensure the clock source is stable between `gptimer_enable()` and `gptimer_disable()`.




### IRAM Safe


By default, the GPTimer interrupt will be deferred when the cache is disabled because of writing or erasing the flash. Thus the alarm interrupt will not get executed in time, which is not expected in a real-time application.


There is a Kconfig option CONFIG_GPTIMER_ISR_IRAM_SAFE that:


* Enables the interrupt being serviced even when the cache is disabled
* Places all functions that used by the ISR into IRAM 2
* Places driver object into DRAM (in case it is mapped to PSRAM by accident)


This allows the interrupt to run while the cache is disabled, but comes at the cost of increased IRAM consumption.


There is another Kconfig option CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM that can put commonly used IO control functions into IRAM as well. So, these functions can also be executable when the cache is disabled. These IO control functions are as follows:


* `gptimer_start()`
* `gptimer_stop()`
* `gptimer_get_raw_count()`
* `gptimer_set_raw_count()`
* `gptimer_set_alarm_action()`




### Thread Safety


All the APIs provided by the driver are guaranteed to be thread safe, which means you can call them from different RTOS tasks without protection by extra locks. The following functions are allowed to run under ISR context.


* `gptimer_start()`
* `gptimer_stop()`
* `gptimer_get_raw_count()`
* `gptimer_set_raw_count()`
* `gptimer_get_captured_count()`
* `gptimer_set_alarm_action()`




### Kconfig Options


* CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM controls where to place the GPTimer control functions (IRAM or flash).
* CONFIG_GPTIMER_ISR_HANDLER_IN_IRAM controls where to place the GPTimer ISR handler (IRAM or flash).
* CONFIG_GPTIMER_ISR_IRAM_SAFE controls whether the default ISR handler should be masked when the cache is disabled, see Section IRAM Safe for more information.
* CONFIG_GPTIMER_ENABLE_DEBUG_LOG is used to enabled the debug log output. Enable this option will increase the firmware binary size.





## Application Examples


* Typical use cases of GPTimer are listed in the example peripherals/timer_group/gptimer.







## API Reference



### Header File


* components/esp_driver_gptimer/include/driver/gptimer.h
* This header file can be included with:



> ```
> #include "driver/gptimer.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_gptimer` component. To declare that your component depends on `esp_driver_gptimer`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_gptimer
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_gptimer
> 
> ```




### Functions


#### gptimer_new_timer


```


esp_err_t gptimer_new_timer(const gptimer_config_t *config, gptimer_handle_t *ret_timer)
Create a new General Purpose Timer, and return the handle. 

Note
The newly created timer is put in the "init" state.


Parameters

config -- [in] GPTimer configuration 
ret_timer -- [out] Returned timer handle 


Returns

ESP_OK: Create GPTimer successfully
ESP_ERR_INVALID_ARG: Create GPTimer failed because of invalid argument
ESP_ERR_NO_MEM: Create GPTimer failed because out of memory
ESP_ERR_NOT_FOUND: Create GPTimer failed because all hardware timers are used up and no more free one
ESP_FAIL: Create GPTimer failed because of other error 





```

#### gptimer_del_timer


```


esp_err_t gptimer_del_timer(gptimer_handle_t timer)
Delete the GPTimer handle. 

Note
A timer must be in the "init" state before it can be deleted.


Parameters
timer -- [in] Timer handle created by gptimer_new_timer

Returns

ESP_OK: Delete GPTimer successfully
ESP_ERR_INVALID_ARG: Delete GPTimer failed because of invalid argument
ESP_ERR_INVALID_STATE: Delete GPTimer failed because the timer is not in init state
ESP_FAIL: Delete GPTimer failed because of other error 





```

#### gptimer_set_raw_count


```


esp_err_t gptimer_set_raw_count(gptimer_handle_t timer, uint64_t value)
Set GPTimer raw count value. 

Note
When updating the raw count of an active timer, the timer will immediately start counting from the new value. 


Note
This function is allowed to run within ISR context 


Note
If CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Flash Cache is disabled.


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
value -- [in] Count value to be set 


Returns

ESP_OK: Set GPTimer raw count value successfully
ESP_ERR_INVALID_ARG: Set GPTimer raw count value failed because of invalid argument
ESP_FAIL: Set GPTimer raw count value failed because of other error 





```

#### gptimer_get_raw_count


```


esp_err_t gptimer_get_raw_count(gptimer_handle_t timer, uint64_t *value)
Get GPTimer raw count value. 

Note
This function will trigger a software capture event and then return the captured count value. 


Note
With the raw count value and the resolution returned from gptimer_get_resolution, you can convert the count value into seconds. 


Note
This function is allowed to run within ISR context 


Note
If CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Flash Cache is disabled.


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
value -- [out] Returned GPTimer count value 


Returns

ESP_OK: Get GPTimer raw count value successfully
ESP_ERR_INVALID_ARG: Get GPTimer raw count value failed because of invalid argument
ESP_FAIL: Get GPTimer raw count value failed because of other error 





```

#### gptimer_get_resolution


```


esp_err_t gptimer_get_resolution(gptimer_handle_t timer, uint32_t *out_resolution)
Return the real resolution of the timer. 

Note
usually the timer resolution is same as what you configured in the gptimer_config_t::resolution_hz, but some unstable clock source (e.g. RC_FAST) will do a calibration, the real resolution can be different from the configured one.


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
out_resolution -- [out] Returned timer resolution, in Hz 


Returns

ESP_OK: Get GPTimer resolution successfully
ESP_ERR_INVALID_ARG: Get GPTimer resolution failed because of invalid argument
ESP_FAIL: Get GPTimer resolution failed because of other error 





```

#### gptimer_get_captured_count


```


esp_err_t gptimer_get_captured_count(gptimer_handle_t timer, uint64_t *value)
Get GPTimer captured count value. 

Note
The capture action can be issued either by ETM event or by software (see also gptimer_get_raw_count). 


Note
This function is allowed to run within ISR context 


Note
If CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Flash Cache is disabled.


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
value -- [out] Returned captured count value 


Returns

ESP_OK: Get GPTimer captured count value successfully
ESP_ERR_INVALID_ARG: Get GPTimer captured count value failed because of invalid argument
ESP_FAIL: Get GPTimer captured count value failed because of other error 





```

#### gptimer_register_event_callbacks


```


esp_err_t gptimer_register_event_callbacks(gptimer_handle_t timer, const gptimer_event_callbacks_t *cbs, void *user_data)
Set callbacks for GPTimer. 

Note
User registered callbacks are expected to be runnable within ISR context 


Note
The first call to this function needs to be before the call to gptimer_enable


Note
User can deregister a previously registered callback by calling this function and setting the callback member in the cbs structure to NULL.


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
cbs -- [in] Group of callback functions 
user_data -- [in] User data, which will be passed to callback functions directly 


Returns

ESP_OK: Set event callbacks successfully
ESP_ERR_INVALID_ARG: Set event callbacks failed because of invalid argument
ESP_ERR_INVALID_STATE: Set event callbacks failed because the timer is not in init state
ESP_FAIL: Set event callbacks failed because of other error 





```

#### gptimer_set_alarm_action


```


esp_err_t gptimer_set_alarm_action(gptimer_handle_t timer, const gptimer_alarm_config_t *config)
Set alarm event actions for GPTimer. 

Note
This function is allowed to run within ISR context, so you can update new alarm action immediately in any ISR callback. 


Note
If CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Flash Cache is disabled. In this case, please also ensure the gptimer_alarm_config_t instance is placed in the static data section instead of in the read-only data section. e.g.: static gptimer_alarm_config_t alarm_config = { ... };


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
config -- [in] Alarm configuration, especially, set config to NULL means disabling the alarm function 


Returns

ESP_OK: Set alarm action for GPTimer successfully
ESP_ERR_INVALID_ARG: Set alarm action for GPTimer failed because of invalid argument
ESP_FAIL: Set alarm action for GPTimer failed because of other error 





```

#### gptimer_enable


```


esp_err_t gptimer_enable(gptimer_handle_t timer)
Enable GPTimer. 

Note
This function will transit the timer state from "init" to "enable". 


Note
This function will enable the interrupt service, if it's lazy installed in gptimer_register_event_callbacks. 


Note
This function will acquire a PM lock, if a specific source clock (e.g. APB) is selected in the gptimer_config_t, while CONFIG_PM_ENABLE is enabled. 


Note
Enable a timer doesn't mean to start it. See also gptimer_start for how to make the timer start counting.


Parameters
timer -- [in] Timer handle created by gptimer_new_timer

Returns

ESP_OK: Enable GPTimer successfully
ESP_ERR_INVALID_ARG: Enable GPTimer failed because of invalid argument
ESP_ERR_INVALID_STATE: Enable GPTimer failed because the timer is already enabled
ESP_FAIL: Enable GPTimer failed because of other error 





```

#### gptimer_disable


```


esp_err_t gptimer_disable(gptimer_handle_t timer)
Disable GPTimer. 

Note
This function will transit the timer state from "enable" to "init". 


Note
This function will disable the interrupt service if it's installed. 


Note
This function will release the PM lock if it's acquired in the gptimer_enable. 


Note
Disable a timer doesn't mean to stop it. See also gptimer_stop for how to make the timer stop counting.


Parameters
timer -- [in] Timer handle created by gptimer_new_timer

Returns

ESP_OK: Disable GPTimer successfully
ESP_ERR_INVALID_ARG: Disable GPTimer failed because of invalid argument
ESP_ERR_INVALID_STATE: Disable GPTimer failed because the timer is not enabled yet
ESP_FAIL: Disable GPTimer failed because of other error 





```

#### gptimer_start


```


esp_err_t gptimer_start(gptimer_handle_t timer)
Start GPTimer (internal counter starts counting) 

Note
This function will transit the timer state from "enable" to "run". 


Note
This function is allowed to run within ISR context 


Note
If CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Flash Cache is disabled.


Parameters
timer -- [in] Timer handle created by gptimer_new_timer

Returns

ESP_OK: Start GPTimer successfully
ESP_ERR_INVALID_ARG: Start GPTimer failed because of invalid argument
ESP_ERR_INVALID_STATE: Start GPTimer failed because the timer is not enabled or is already in running
ESP_FAIL: Start GPTimer failed because of other error 





```

#### gptimer_stop


```


esp_err_t gptimer_stop(gptimer_handle_t timer)
Stop GPTimer (internal counter stops counting) 

Note
This function will transit the timer state from "run" to "enable". 


Note
This function is allowed to run within ISR context 


Note
If CONFIG_GPTIMER_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Flash Cache is disabled.


Parameters
timer -- [in] Timer handle created by gptimer_new_timer

Returns

ESP_OK: Stop GPTimer successfully
ESP_ERR_INVALID_ARG: Stop GPTimer failed because of invalid argument
ESP_ERR_INVALID_STATE: Stop GPTimer failed because the timer is not in running.
ESP_FAIL: Stop GPTimer failed because of other error 





```



### Structures


#### gptimer_config_t


```


struct gptimer_config_t
General Purpose Timer configuration. 

Public Members
clk_src

gptimer_clock_source_t clk_src
GPTimer clock source 

direction

gptimer_count_direction_t direction
Count direction 

resolution_hz

uint32_t resolution_hz
Counter resolution (working frequency) in Hz, hence, the step size of each count tick equals to (1 / resolution_hz) seconds 

intr_priority

int intr_priority
GPTimer interrupt priority, if set to 0, the driver will try to allocate an interrupt with a relative low priority (1,2,3) 

intr_shared

uint32_t intr_shared
Set true, the timer interrupt number can be shared with other peripherals 

backup_before_sleep

uint32_t backup_before_sleep
If set, the driver will backup/restore the GPTimer registers before/after entering/exist sleep mode. By this approach, the system can power off GPTimer's power domain. This can save power, but at the expense of more RAM being consumed 

flags

struct gptimer_config_t::[anonymous] flags
GPTimer config flags 



```

#### gptimer_event_callbacks_t


```


struct gptimer_event_callbacks_t
Group of supported GPTimer callbacks. 

Note
The callbacks are all running under ISR environment 


Note
When CONFIG_GPTIMER_ISR_IRAM_SAFE is enabled, the callback itself and functions called by it should be placed in IRAM. 


Public Members
on_alarm

gptimer_alarm_cb_t on_alarm
Timer alarm callback 



```

#### gptimer_alarm_config_t


```


struct gptimer_alarm_config_t
General Purpose Timer alarm configuration. 

Public Members
alarm_count

uint64_t alarm_count
Alarm target count value 

reload_count

uint64_t reload_count
Alarm reload count value, effect only when auto_reload_on_alarm is set to true 

auto_reload_on_alarm

uint32_t auto_reload_on_alarm
Reload the count value by hardware, immediately at the alarm event 

flags

struct gptimer_alarm_config_t::[anonymous] flags
Alarm config flags 



```



### Header File


* components/esp_driver_gptimer/include/driver/gptimer_etm.h
* This header file can be included with:



> ```
> #include "driver/gptimer_etm.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_gptimer` component. To declare that your component depends on `esp_driver_gptimer`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_gptimer
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_gptimer
> 
> ```




### Functions


#### gptimer_new_etm_event


```


esp_err_t gptimer_new_etm_event(gptimer_handle_t timer, const gptimer_etm_event_config_t *config, esp_etm_event_handle_t *out_event)
Get the ETM event for GPTimer. 

Note
The created ETM event object can be deleted later by calling esp_etm_del_event


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
config -- [in] GPTimer ETM event configuration 
out_event -- [out] Returned ETM event handle 


Returns

ESP_OK: Get ETM event successfully
ESP_ERR_INVALID_ARG: Get ETM event failed because of invalid argument
ESP_FAIL: Get ETM event failed because of other error 





```

#### gptimer_new_etm_task


```


esp_err_t gptimer_new_etm_task(gptimer_handle_t timer, const gptimer_etm_task_config_t *config, esp_etm_task_handle_t *out_task)
Get the ETM task for GPTimer. 

Note
The created ETM task object can be deleted later by calling esp_etm_del_task


Parameters

timer -- [in] Timer handle created by gptimer_new_timer
config -- [in] GPTimer ETM task configuration 
out_task -- [out] Returned ETM task handle 


Returns

ESP_OK: Get ETM task successfully
ESP_ERR_INVALID_ARG: Get ETM task failed because of invalid argument
ESP_FAIL: Get ETM task failed because of other error 





```



### Structures


#### gptimer_etm_event_config_t


```


struct gptimer_etm_event_config_t
GPTimer ETM event configuration. 

Public Members
event_type

gptimer_etm_event_type_t event_type
GPTimer ETM event type 



```

#### gptimer_etm_task_config_t


```


struct gptimer_etm_task_config_t
GPTimer ETM task configuration. 

Public Members
task_type

gptimer_etm_task_type_t task_type
GPTimer ETM task type 



```



### Header File


* components/esp_driver_gptimer/include/driver/gptimer_types.h
* This header file can be included with:



> ```
> #include "driver/gptimer_types.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_gptimer` component. To declare that your component depends on `esp_driver_gptimer`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_gptimer
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_gptimer
> 
> ```




### Structures


#### gptimer_alarm_event_data_t


```


struct gptimer_alarm_event_data_t
GPTimer alarm event data. 

Public Members
count_value

uint64_t count_value
Current count value 

alarm_value

uint64_t alarm_value
Current alarm value 



```



### Type Definitions


#### gptimer_handle_t


```


typedef struct gptimer_t *gptimer_handle_t
Type of General Purpose Timer handle. 

```

#### gptimer_alarm_cb_t


```


typedef bool (*gptimer_alarm_cb_t)(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
Timer alarm callback prototype. 

Param timer
[in] Timer handle created by gptimer_new_timer

Param edata
[in] Alarm event data, fed by driver 

Param user_ctx
[in] User data, passed from gptimer_register_event_callbacks

Return
Whether a high priority task has been waken up by this function 



```



### Header File


* components/hal/include/hal/timer_types.h
* This header file can be included with:



> ```
> #include "hal/timer_types.h"
> 
> ```




### Type Definitions


#### gptimer_clock_source_t


```


typedef soc_periph_gptimer_clk_src_t gptimer_clock_source_t
GPTimer clock source. 

Note
User should select the clock source based on the power and resolution requirement 


```



### Enumerations


#### gptimer_count_direction_t


```


enum gptimer_count_direction_t
GPTimer count direction. 
Values:
GPTIMER_COUNT_DOWN

enumerator GPTIMER_COUNT_DOWN
Decrease count value 

GPTIMER_COUNT_UP

enumerator GPTIMER_COUNT_UP
Increase count value 


```

#### gptimer_etm_task_type_t


```


enum gptimer_etm_task_type_t
GPTimer specific tasks that supported by the ETM module. 
Values:
GPTIMER_ETM_TASK_START_COUNT

enumerator GPTIMER_ETM_TASK_START_COUNT
Start the counter 

GPTIMER_ETM_TASK_STOP_COUNT

enumerator GPTIMER_ETM_TASK_STOP_COUNT
Stop the counter 

GPTIMER_ETM_TASK_EN_ALARM

enumerator GPTIMER_ETM_TASK_EN_ALARM
Enable the alarm 

GPTIMER_ETM_TASK_RELOAD

enumerator GPTIMER_ETM_TASK_RELOAD
Reload preset value into counter 

GPTIMER_ETM_TASK_CAPTURE

enumerator GPTIMER_ETM_TASK_CAPTURE
Capture current count value into specific register 

GPTIMER_ETM_TASK_MAX

enumerator GPTIMER_ETM_TASK_MAX
Maximum number of tasks 


```

#### gptimer_etm_event_type_t


```


enum gptimer_etm_event_type_t
GPTimer specific events that supported by the ETM module. 
Values:
GPTIMER_ETM_EVENT_ALARM_MATCH

enumerator GPTIMER_ETM_EVENT_ALARM_MATCH
Count value matches the alarm target value 

GPTIMER_ETM_EVENT_MAX

enumerator GPTIMER_ETM_EVENT_MAX
Maximum number of events 


```


1
Different ESP chip series might have different numbers of GPTimer instances. For more details, please refer to **ESP32-S3 Technical Reference Manual** > Chapter **Timer Group (TIMG)** [PDF]. The driver does forbid you from applying for more timers, but it returns error when all available hardware resources are used up. Please always check the return value when doing resource allocation (e.g., `gptimer_new_timer()`).



2
`gptimer_event_callbacks_t::on_alarm` callback and the functions invoked by the callback should also be placed in IRAM, please take care of them by yourself.