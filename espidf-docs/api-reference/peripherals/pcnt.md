# Pulse Counter (PCNT)



## Introduction


The PCNT (Pulse Counter) module is designed to count the number of rising and/or falling edges of input signals. The ESP32-S3 contains multiple pulse counter units in the module. 1 Each unit is in effect an independent counter with multiple channels, where each channel can increment/decrement the counter on a rising/falling edge. Furthermore, each channel can be configured separately.


PCNT channels can react to signals of **edge** type and **level** type, however for simple applications, detecting the edge signal is usually sufficient. PCNT channels can be configured react to both pulse edges (i.e., rising and falling edge), and can be configured to increase, decrease or do nothing to the unit's counter on each edge. The level signal is the so-called **control signal**, which is used to control the counting mode of the edge signals that are attached to the same channel. By combining the usage of both edge and level signals, a PCNT unit can act as a **quadrature decoder**.


Besides that, PCNT unit is equipped with a separate glitch filter, which is helpful to remove noise from the signal.


Typically, a PCNT module can be used in scenarios like:


* Calculate periodic signal's frequency by counting the pulse numbers within a time slice
* Decode quadrature signals into speed and direction




## Functional Overview


Description of the PCNT functionality is divided into the following sections:


* Resource Allocation - covers how to allocate PCNT units and channels with properly set of configurations. It also covers how to recycle the resources when they finished working.
* Set Up Channel Actions - covers how to configure the PCNT channel to behave on different signal edges and levels.
* Watch Points - describes how to configure PCNT watch points (i.e., tell PCNT unit to trigger an event when the count reaches a certain value).
* Register Event Callbacks - describes how to hook your specific code to the watch point event callback function.
* Set Glitch Filter - describes how to enable and set the timing parameters for the internal glitch filter.
* Enable and Disable Unit - describes how to enable and disable the PCNT unit.
* Unit IO Control - describes IO control functions of PCNT unit, like enable glitch filter, start and stop unit, get and clear count value.
* Power Management - describes what functionality will prevent the chip from going into low power mode.
* IRAM Safe - describes tips on how to make the PCNT interrupt and IO control functions work better along with a disabled cache.
* Thread Safety - lists which APIs are guaranteed to be thread safe by the driver.
* Kconfig Options - lists the supported Kconfig options that can be used to make a different effect on driver behavior.






### Resource Allocation


The PCNT unit and channel are represented by `pcnt_unit_handle_t` and `pcnt_channel_handle_t` respectively. All available units and channels are maintained by the driver in a resource pool, so you do not need to know the exact underlying instance ID.



#### Install PCNT Unit


To install a PCNT unit, there is a configuration structure that needs to be given in advance: `pcnt_unit_config_t`:


* `pcnt_unit_config_t::low_limit` and `pcnt_unit_config_t::high_limit` specify the range for the internal hardware counter. The counter will reset to zero automatically when it crosses either the high or low limit.
* `pcnt_unit_config_t::accum_count` sets whether to create an internal accumulator for the counter. This is helpful when you want to extend the counter's width, which by default is 16 bit at most, defined in the hardware. See also Compensate Overflow Loss for how to use this feature to compensate the overflow loss.
* `pcnt_unit_config_t::intr_priority` sets the priority of the interrupt. If it is set to `0`, the driver will allocate an interrupt with a default priority. Otherwise, the driver will use the given priority.



Note


Since all PCNT units share the same interrupt source, when installing multiple PCNT units make sure that the interrupt priority `pcnt_unit_config_t::intr_priority` is the same for each unit.



Unit allocation and initialization is done by calling a function `pcnt_new_unit()` with `pcnt_unit_config_t` as an input parameter. The function will return a PCNT unit handle only when it runs correctly. Specifically, when there are no more free PCNT units in the pool (i.e., unit resources have been used up), then this function will return `ESP_ERR_NOT_FOUND` error. The total number of available PCNT units is recorded by `SOC_PCNT_UNITS_PER_GROUP` for reference.


If a previously created PCNT unit is no longer needed, it is recommended to recycle the resource by calling `pcnt_del_unit()`. Which in return allows the underlying unit hardware to be used for other purposes. Before deleting a PCNT unit, one should ensure the following prerequisites:


* The unit is in the init state, in other words, the unit is either disabled by `pcnt_unit_disable()` or not enabled yet.
* The attached PCNT channels are all removed by `pcnt_del_channel()`.



```
#define EXAMPLE_PCNT_HIGH_LIMIT 100
#define EXAMPLE_PCNT_LOW_LIMIT  -100

pcnt_unit_config_t unit_config = {
    .high_limit = EXAMPLE_PCNT_HIGH_LIMIT,
    .low_limit = EXAMPLE_PCNT_LOW_LIMIT,
};
pcnt_unit_handle_t pcnt_unit = NULL;
ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

```




#### Install PCNT Channel


To install a PCNT channel, you must initialize a `pcnt_chan_config_t` structure in advance, and then call `pcnt_new_channel()`. The configuration fields of the `pcnt_chan_config_t` structure are described below:


* `pcnt_chan_config_t::edge_gpio_num` and `pcnt_chan_config_t::level_gpio_num` specify the GPIO numbers used by **edge** type signal and **level** type signal. Please note, either of them can be assigned to `-1` if it is not actually used, and thus it will become a **virtual IO**. For some simple pulse counting applications where one of the level/edge signals is fixed (i.e., never changes), you can reclaim a GPIO by setting the signal as a virtual IO on channel allocation. Setting the level/edge signal as a virtual IO causes that signal to be internally routed to a fixed High/Low logic level, thus allowing you to save a GPIO for other purposes.
* `pcnt_chan_config_t::virt_edge_io_level` and `pcnt_chan_config_t::virt_level_io_level` specify the virtual IO level for **edge** and **level** input signal, to ensure a deterministic state for such control signal. Please note, they are only valid when either `pcnt_chan_config_t::edge_gpio_num` or `pcnt_chan_config_t::level_gpio_num` is assigned to `-1`.
* `pcnt_chan_config_t::invert_edge_input` and `pcnt_chan_config_t::invert_level_input` are used to decide whether to invert the input signals before they going into PCNT hardware. The invert is done by GPIO matrix instead of PCNT hardware.
* `pcnt_chan_config_t::io_loop_back` is for debug only, which enables both the GPIO's input and output paths. This can help to simulate the pulse signals by function `gpio_set_level()` on the same GPIO.


Channel allocating and initialization is done by calling a function `pcnt_new_channel()` with the above `pcnt_chan_config_t` as an input parameter plus a PCNT unit handle returned from `pcnt_new_unit()`. This function will return a PCNT channel handle if it runs correctly. Specifically, when there are no more free PCNT channel within the unit (i.e., channel resources have been used up), then this function will return `ESP_ERR_NOT_FOUND` error. The total number of available PCNT channels within the unit is recorded by `SOC_PCNT_CHANNELS_PER_UNIT` for reference. Note that, when install a PCNT channel for a specific unit, one should ensure the unit is in the init state, otherwise this function will return `ESP_ERR_INVALID_STATE` error.


If a previously created PCNT channel is no longer needed, it is recommended to recycle the resources by calling `pcnt_del_channel()`. Which in return allows the underlying channel hardware to be used for other purposes.



```
#define EXAMPLE_CHAN_GPIO_A 0
#define EXAMPLE_CHAN_GPIO_B 2

pcnt_chan_config_t chan_config = {
    .edge_gpio_num = EXAMPLE_CHAN_GPIO_A,
    .level_gpio_num = EXAMPLE_CHAN_GPIO_B,
};
pcnt_channel_handle_t pcnt_chan = NULL;
ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config, &pcnt_chan));

```





### Set Up Channel Actions


The PCNT will increase/decrease/hold its internal count value when the input pulse signal toggles. You can set different actions for edge signal and/or level signal.


* `pcnt_channel_set_edge_action()` function is to set specific actions for rising and falling edge of the signal attached to the `pcnt_chan_config_t::edge_gpio_num`. Supported actions are listed in `pcnt_channel_edge_action_t`.
* `pcnt_channel_set_level_action()` function is to set specific actions for high and low level of the signal attached to the `pcnt_chan_config_t::level_gpio_num`. Supported actions are listed in `pcnt_channel_level_action_t`. This function is not mandatory if the `pcnt_chan_config_t::level_gpio_num` is set to `-1` when allocating PCNT channel by `pcnt_new_channel()`.



```
// decrease the counter on rising edge, increase the counter on falling edge
ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
// keep the counting mode when the control signal is high level, and reverse the counting mode when the control signal is low level
ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

```




### Watch Points


Each PCNT unit can be configured to watch several different values that you are interested in. The value to be watched is also called **Watch Point**. The watch point itself can not exceed the range set in `pcnt_unit_config_t` by `pcnt_unit_config_t::low_limit` and `pcnt_unit_config_t::high_limit`. When the counter reaches either watch point, a watch event will be triggered and notify you by interrupt if any watch event callback has ever registered in `pcnt_unit_register_event_callbacks()`. See Register Event Callbacks for how to register event callbacks.


The watch point can be added and removed by `pcnt_unit_add_watch_point()` and `pcnt_unit_remove_watch_point()`. The commonly-used watch points are: **zero cross**, **maximum/minimum count** and other threshold values. The number of available watch point is limited, `pcnt_unit_add_watch_point()` will return error `ESP_ERR_NOT_FOUND` if it can not find any free hardware resource to save the watch point. You can not add the same watch point for multiple times, otherwise it will return error `ESP_ERR_INVALID_STATE`.


It is recommended to remove the unused watch point by `pcnt_unit_remove_watch_point()` to recycle the watch point resources.



```
// add zero across watch point
ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, 0));
// add high limit watch point
ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, EXAMPLE_PCNT_HIGH_LIMIT));

```



Note


Due to the hardware limitation, after adding a watch point, you should call `pcnt_unit_clear_count()` to make it take effect.





### Register Event Callbacks


When PCNT unit reaches any enabled watch point, specific event will be generated and notify the CPU by interrupt. If you have some function that want to get executed when event happens, you should hook your function to the interrupt service routine by calling `pcnt_unit_register_event_callbacks()`. All supported event callbacks are listed in the `pcnt_event_callbacks_t`:


* `pcnt_event_callbacks_t::on_reach` sets a callback function for watch point event. As this function is called within the ISR context, you must ensure that the function does not attempt to block (e.g., by making sure that only FreeRTOS APIs with `ISR` suffix are called from within the function). The function prototype is declared in `pcnt_watch_cb_t`.


You can save their own context to `pcnt_unit_register_event_callbacks()` as well, via the parameter `user_ctx`. This user data will be directly passed to the callback functions.


In the callback function, the driver will fill in the event data of specific event. For example, the watch point event data is declared as `pcnt_watch_event_data_t`:


* `pcnt_watch_event_data_t::watch_point_value` saves the watch point value that triggers the event.
* `pcnt_watch_event_data_t::zero_cross_mode` saves how the PCNT unit crosses the zero point in the latest time. The possible zero cross modes are listed in the `pcnt_unit_zero_cross_mode_t`. Usually different zero cross mode means different **counting direction** and **counting step size**.


Registering callback function results in lazy installation of interrupt service, thus this function should only be called before the unit is enabled by `pcnt_unit_enable()`. Otherwise, it can return `ESP_ERR_INVALID_STATE` error.



```
static bool example_pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // send watch point to queue, from this interrupt callback
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    // return whether a high priority task has been waken up by this function
    return (high_task_wakeup == pdTRUE);
}

pcnt_event_callbacks_t cbs = {
    .on_reach = example_pcnt_on_reach,
};
QueueHandle_t queue = xQueueCreate(10, sizeof(int));
ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, queue));

```




### Set Glitch Filter


The PCNT unit features filters to ignore possible short glitches in the signals. The parameters that can be configured for the glitch filter are listed in `pcnt_glitch_filter_config_t`:


* `pcnt_glitch_filter_config_t::max_glitch_ns` sets the maximum glitch width, in nano seconds. If a signal pulse's width is smaller than this value, then it will be treated as noise and will not increase/decrease the internal counter.


You can enable the glitch filter for PCNT unit by calling `pcnt_unit_set_glitch_filter()` with the filter configuration provided above. Particularly, you can disable the glitch filter later by calling `pcnt_unit_set_glitch_filter()` with a `NULL` filter configuration.


This function should be called when the unit is in the init state. Otherwise, it will return `ESP_ERR_INVALID_STATE` error.



Note


The glitch filter is clocked from APB. For the counter not to miss any pulses, the maximum glitch width should be longer than one APB_CLK cycle (usually 12.5 ns if APB equals 80 MHz). As the APB frequency would be changed after DFS (Dynamic Frequency Scaling) enabled, which means the filter does not work as expect in that case. So the driver installs a PM lock for PCNT unit during the first time you enable the glitch filter. For more information related to power management strategy used in PCNT driver, please see Power Management.




```
pcnt_glitch_filter_config_t filter_config = {
    .max_glitch_ns = 1000,
};
ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

```




### Enable and Disable Unit


Before doing IO control to the PCNT unit, you need to enable it first, by calling `pcnt_unit_enable()`. Internally, this function:


* switches the PCNT driver state from **init** to **enable**.
* enables the interrupt service if it has been lazy installed in `pcnt_unit_register_event_callbacks()`.
* acquires a proper power management lock if it has been lazy installed in `pcnt_unit_set_glitch_filter()`. See also Power Management for more information.


On the contrary, calling `pcnt_unit_disable()` will do the opposite, that is, put the PCNT driver back to the **init** state, disable the interrupts service and release the power management lock.




### Unit IO Control



#### Start/Stop and Clear


Calling `pcnt_unit_start()` makes the PCNT unit start to work, increase or decrease counter according to pulse signals. On the contrary, calling `pcnt_unit_stop()` will stop the PCNT unit but retain current count value. Instead, clearing counter can only be done by calling `pcnt_unit_clear_count()`.


Note, `pcnt_unit_start()` and `pcnt_unit_stop()` should be called when the unit has been enabled by `pcnt_unit_enable()`. Otherwise, it will return `ESP_ERR_INVALID_STATE` error.




#### Get Count Value


You can read current count value at any time by calling `pcnt_unit_get_count()`. The returned count value is a **signed** integer, where the sign can be used to reflect the direction.



```
int pulse_count = 0;
ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));

```




#### Compensate Overflow Loss


The internal hardware counter will be cleared to zero automatically when it reaches high or low limit. If you want to compensate for that count loss and extend the counter's bit-width, you can:



> 1. Enable `pcnt_unit_config_t::accum_count` when installing the PCNT unit.
> 2. Add the high/low limit as the Watch Points.
> 3. Now, the returned count value from the `pcnt_unit_get_count()` function not only reflects the hardware's count value, but also accumulates the high/low overflow loss to it.



Note


`pcnt_unit_clear_count()` resets the accumulated count value as well.






### Power Management


When power management is enabled (i.e., CONFIG_PM_ENABLE is on), the system will adjust the APB frequency before going into light sleep, thus potentially changing the behavior of PCNT glitch filter and leading to valid signal being treated as noise.


However, the driver can prevent the system from changing APB frequency by acquiring a power management lock of type `ESP_PM_APB_FREQ_MAX`. Whenever you enable the glitch filter by `pcnt_unit_set_glitch_filter()`, the driver guarantees that the power management lock is acquired after the PCNT unit is enabled by `pcnt_unit_enable()`. Likewise, the driver releases the lock after `pcnt_unit_disable()` is called.




### IRAM Safe


By default, the PCNT interrupt will be deferred when the Cache is disabled for reasons like writing/erasing Flash. Thus the alarm interrupt will not get executed in time, which is not expected in a real-time application.


There is a Kconfig option CONFIG_PCNT_ISR_IRAM_SAFE that:


1. Enables the interrupt being serviced even when cache is disabled
2. Places all functions that used by the ISR into IRAM 2
3. Places driver object into DRAM (in case it is mapped to PSRAM by accident)


This allows the interrupt to run while the cache is disabled but comes at the cost of increased IRAM consumption.


There is another Kconfig option CONFIG_PCNT_CTRL_FUNC_IN_IRAM that can put commonly used IO control functions into IRAM as well. So that these functions can also be executable when the cache is disabled. These IO control functions are as follows:


* `pcnt_unit_start()`
* `pcnt_unit_stop()`
* `pcnt_unit_clear_count()`
* `pcnt_unit_get_count()`




### Thread Safety


The factory functions `pcnt_new_unit()` and `pcnt_new_channel()` are guaranteed to be thread safe by the driver, which means, you can call them from different RTOS tasks without protection by extra locks.


The following functions are allowed to run under ISR context, the driver uses a critical section to prevent them being called concurrently in both task and ISR.


* `pcnt_unit_start()`
* `pcnt_unit_stop()`
* `pcnt_unit_clear_count()`
* `pcnt_unit_get_count()`


Other functions that take the `pcnt_unit_handle_t` and `pcnt_channel_handle_t` as the first positional parameter, are not treated as thread safe. This means you should avoid calling them from multiple tasks.




### Kconfig Options


* CONFIG_PCNT_CTRL_FUNC_IN_IRAM controls where to place the PCNT control functions (IRAM or Flash), see IRAM Safe for more information.
* CONFIG_PCNT_ISR_IRAM_SAFE controls whether the default ISR handler can work when cache is disabled, see IRAM Safe for more information.
* CONFIG_PCNT_ENABLE_DEBUG_LOG is used to enabled the debug log output. Enabling this option increases the firmware binary size.





## Application Examples


* Decode the quadrature signals from rotary encoder: peripherals/pcnt/rotary_encoder.




## API Reference



### Header File


* components/esp_driver_pcnt/include/driver/pulse_cnt.h
* This header file can be included with:



> ```
> #include "driver/pulse_cnt.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_pcnt` component. To declare that your component depends on `esp_driver_pcnt`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_pcnt
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_pcnt
> 
> ```




### Functions


#### pcnt_new_unit


```


esp_err_t pcnt_new_unit(const pcnt_unit_config_t *config, pcnt_unit_handle_t *ret_unit)
Create a new PCNT unit, and return the handle. 

Note
The newly created PCNT unit is put in the init state.


Parameters

config -- [in] PCNT unit configuration 
ret_unit -- [out] Returned PCNT unit handle 


Returns

ESP_OK: Create PCNT unit successfully
ESP_ERR_INVALID_ARG: Create PCNT unit failed because of invalid argument (e.g. high/low limit value out of the range)
ESP_ERR_NO_MEM: Create PCNT unit failed because out of memory
ESP_ERR_NOT_FOUND: Create PCNT unit failed because all PCNT units are used up and no more free one
ESP_FAIL: Create PCNT unit failed because of other error 





```

#### pcnt_del_unit


```


esp_err_t pcnt_del_unit(pcnt_unit_handle_t unit)
Delete the PCNT unit handle. 

Note
A PCNT unit can't be in the enable state when this function is invoked. See also pcnt_unit_disable() for how to disable a unit.


Parameters
unit -- [in] PCNT unit handle created by pcnt_new_unit()

Returns

ESP_OK: Delete the PCNT unit successfully
ESP_ERR_INVALID_ARG: Delete the PCNT unit failed because of invalid argument
ESP_ERR_INVALID_STATE: Delete the PCNT unit failed because the unit is not in init state or some PCNT channel is still in working
ESP_FAIL: Delete the PCNT unit failed because of other error 





```

#### pcnt_unit_set_glitch_filter


```


esp_err_t pcnt_unit_set_glitch_filter(pcnt_unit_handle_t unit, const pcnt_glitch_filter_config_t *config)
Set glitch filter for PCNT unit. 

Note
The glitch filter module is clocked from APB, and APB frequency can be changed during DFS, which in return make the filter out of action. So this function will lazy-install a PM lock internally when the power management is enabled. With this lock, the APB frequency won't be changed. The PM lock can be uninstalled in pcnt_del_unit(). 


Note
This function should be called when the PCNT unit is in the init state (i.e. before calling pcnt_unit_enable())


Parameters

unit -- [in] PCNT unit handle created by pcnt_new_unit()
config -- [in] PCNT filter configuration, set config to NULL means disabling the filter function 


Returns

ESP_OK: Set glitch filter successfully
ESP_ERR_INVALID_ARG: Set glitch filter failed because of invalid argument (e.g. glitch width is too big)
ESP_ERR_INVALID_STATE: Set glitch filter failed because the unit is not in the init state
ESP_FAIL: Set glitch filter failed because of other error 





```

#### pcnt_unit_enable


```


esp_err_t pcnt_unit_enable(pcnt_unit_handle_t unit)
Enable the PCNT unit. 

Note
This function will transit the unit state from init to enable. 


Note
This function will enable the interrupt service, if it's lazy installed in pcnt_unit_register_event_callbacks(). 


Note
This function will acquire the PM lock if it's lazy installed in pcnt_unit_set_glitch_filter(). 


Note
Enable a PCNT unit doesn't mean to start it. See also pcnt_unit_start() for how to start the PCNT counter.


Parameters
unit -- [in] PCNT unit handle created by pcnt_new_unit()

Returns

ESP_OK: Enable PCNT unit successfully
ESP_ERR_INVALID_ARG: Enable PCNT unit failed because of invalid argument
ESP_ERR_INVALID_STATE: Enable PCNT unit failed because the unit is already enabled
ESP_FAIL: Enable PCNT unit failed because of other error 





```

#### pcnt_unit_disable


```


esp_err_t pcnt_unit_disable(pcnt_unit_handle_t unit)
Disable the PCNT unit. 

Note
This function will do the opposite work to the pcnt_unit_enable()


Note
Disable a PCNT unit doesn't mean to stop it. See also pcnt_unit_stop() for how to stop the PCNT counter.


Parameters
unit -- [in] PCNT unit handle created by pcnt_new_unit()

Returns

ESP_OK: Disable PCNT unit successfully
ESP_ERR_INVALID_ARG: Disable PCNT unit failed because of invalid argument
ESP_ERR_INVALID_STATE: Disable PCNT unit failed because the unit is not enabled yet
ESP_FAIL: Disable PCNT unit failed because of other error 





```

#### pcnt_unit_start


```


esp_err_t pcnt_unit_start(pcnt_unit_handle_t unit)
Start the PCNT unit, the counter will start to count according to the edge and/or level input signals. 

Note
This function should be called when the unit is in the enable state (i.e. after calling pcnt_unit_enable()) 


Note
This function is allowed to run within ISR context 


Note
This function will be placed into IRAM if CONFIG_PCNT_CTRL_FUNC_IN_IRAM is on, so that it's allowed to be executed when Cache is disabled


Parameters
unit -- [in] PCNT unit handle created by pcnt_new_unit()

Returns

ESP_OK: Start PCNT unit successfully
ESP_ERR_INVALID_ARG: Start PCNT unit failed because of invalid argument
ESP_ERR_INVALID_STATE: Start PCNT unit failed because the unit is not enabled yet
ESP_FAIL: Start PCNT unit failed because of other error 





```

#### pcnt_unit_stop


```


esp_err_t pcnt_unit_stop(pcnt_unit_handle_t unit)
Stop PCNT from counting. 

Note
This function should be called when the unit is in the enable state (i.e. after calling pcnt_unit_enable()) 


Note
The stop operation won't clear the counter. Also see pcnt_unit_clear_count() for how to clear pulse count value. 


Note
This function is allowed to run within ISR context 


Note
This function will be placed into IRAM if CONFIG_PCNT_CTRL_FUNC_IN_IRAM, so that it is allowed to be executed when Cache is disabled


Parameters
unit -- [in] PCNT unit handle created by pcnt_new_unit()

Returns

ESP_OK: Stop PCNT unit successfully
ESP_ERR_INVALID_ARG: Stop PCNT unit failed because of invalid argument
ESP_ERR_INVALID_STATE: Stop PCNT unit failed because the unit is not enabled yet
ESP_FAIL: Stop PCNT unit failed because of other error 





```

#### pcnt_unit_clear_count


```


esp_err_t pcnt_unit_clear_count(pcnt_unit_handle_t unit)
Clear PCNT pulse count value to zero. 

Note
It's recommended to call this function after adding a watch point by pcnt_unit_add_watch_point(), so that the newly added watch point is effective immediately. 


Note
This function is allowed to run within ISR context 


Note
This function will be placed into IRAM if CONFIG_PCNT_CTRL_FUNC_IN_IRAM, so that it's allowed to be executed when Cache is disabled


Parameters
unit -- [in] PCNT unit handle created by pcnt_new_unit()

Returns

ESP_OK: Clear PCNT pulse count successfully
ESP_ERR_INVALID_ARG: Clear PCNT pulse count failed because of invalid argument
ESP_FAIL: Clear PCNT pulse count failed because of other error 





```

#### pcnt_unit_get_count


```


esp_err_t pcnt_unit_get_count(pcnt_unit_handle_t unit, int *value)
Get PCNT count value. 

Note
This function is allowed to run within ISR context 


Note
This function will be placed into IRAM if CONFIG_PCNT_CTRL_FUNC_IN_IRAM, so that it's allowed to be executed when Cache is disabled


Parameters

unit -- [in] PCNT unit handle created by pcnt_new_unit()
value -- [out] Returned count value 


Returns

ESP_OK: Get PCNT pulse count successfully
ESP_ERR_INVALID_ARG: Get PCNT pulse count failed because of invalid argument
ESP_FAIL: Get PCNT pulse count failed because of other error 





```

#### pcnt_unit_register_event_callbacks


```


esp_err_t pcnt_unit_register_event_callbacks(pcnt_unit_handle_t unit, const pcnt_event_callbacks_t *cbs, void *user_data)
Set event callbacks for PCNT unit. 

Note
User registered callbacks are expected to be runnable within ISR context 


Note
The first call to this function needs to be before the call to pcnt_unit_enable


Note
User can deregister a previously registered callback by calling this function and setting the callback member in the cbs structure to NULL.


Parameters

unit -- [in] PCNT unit handle created by pcnt_new_unit()
cbs -- [in] Group of callback functions 
user_data -- [in] User data, which will be passed to callback functions directly 


Returns

ESP_OK: Set event callbacks successfully
ESP_ERR_INVALID_ARG: Set event callbacks failed because of invalid argument
ESP_ERR_INVALID_STATE: Set event callbacks failed because the unit is not in init state
ESP_FAIL: Set event callbacks failed because of other error 





```

#### pcnt_unit_add_watch_point


```


esp_err_t pcnt_unit_add_watch_point(pcnt_unit_handle_t unit, int watch_point)
Add a watch point for PCNT unit, PCNT will generate an event when the counter value reaches the watch point value. 

Parameters

unit -- [in] PCNT unit handle created by pcnt_new_unit()
watch_point -- [in] Value to be watched 


Returns

ESP_OK: Add watch point successfully
ESP_ERR_INVALID_ARG: Add watch point failed because of invalid argument (e.g. the value to be watched is out of the limitation set in pcnt_unit_config_t)
ESP_ERR_INVALID_STATE: Add watch point failed because the same watch point has already been added
ESP_ERR_NOT_FOUND: Add watch point failed because no more hardware watch point can be configured
ESP_FAIL: Add watch point failed because of other error 





```

#### pcnt_unit_remove_watch_point


```


esp_err_t pcnt_unit_remove_watch_point(pcnt_unit_handle_t unit, int watch_point)
Remove a watch point for PCNT unit. 

Parameters

unit -- [in] PCNT unit handle created by pcnt_new_unit()
watch_point -- [in] Watch point value 


Returns

ESP_OK: Remove watch point successfully
ESP_ERR_INVALID_ARG: Remove watch point failed because of invalid argument
ESP_ERR_INVALID_STATE: Remove watch point failed because the watch point was not added by pcnt_unit_add_watch_point() yet
ESP_FAIL: Remove watch point failed because of other error 





```

#### pcnt_new_channel


```


esp_err_t pcnt_new_channel(pcnt_unit_handle_t unit, const pcnt_chan_config_t *config, pcnt_channel_handle_t *ret_chan)
Create PCNT channel for specific unit, each PCNT has several channels associated with it. 

Note
This function should be called when the unit is in init state (i.e. before calling pcnt_unit_enable())


Parameters

unit -- [in] PCNT unit handle created by pcnt_new_unit()
config -- [in] PCNT channel configuration 
ret_chan -- [out] Returned channel handle 


Returns

ESP_OK: Create PCNT channel successfully
ESP_ERR_INVALID_ARG: Create PCNT channel failed because of invalid argument
ESP_ERR_NO_MEM: Create PCNT channel failed because of insufficient memory
ESP_ERR_NOT_FOUND: Create PCNT channel failed because all PCNT channels are used up and no more free one
ESP_ERR_INVALID_STATE: Create PCNT channel failed because the unit is not in the init state
ESP_FAIL: Create PCNT channel failed because of other error 





```

#### pcnt_del_channel


```


esp_err_t pcnt_del_channel(pcnt_channel_handle_t chan)
Delete the PCNT channel. 

Parameters
chan -- [in] PCNT channel handle created by pcnt_new_channel()

Returns

ESP_OK: Delete the PCNT channel successfully
ESP_ERR_INVALID_ARG: Delete the PCNT channel failed because of invalid argument
ESP_FAIL: Delete the PCNT channel failed because of other error 





```

#### pcnt_channel_set_edge_action


```


esp_err_t pcnt_channel_set_edge_action(pcnt_channel_handle_t chan, pcnt_channel_edge_action_t pos_act, pcnt_channel_edge_action_t neg_act)
Set channel actions when edge signal changes (e.g. falling or rising edge occurred). The edge signal is input from the edge_gpio_num configured in pcnt_chan_config_t. We use these actions to control when and how to change the counter value. 

Parameters

chan -- [in] PCNT channel handle created by pcnt_new_channel()
pos_act -- [in] Action on posedge signal 
neg_act -- [in] Action on negedge signal 


Returns

ESP_OK: Set edge action for PCNT channel successfully
ESP_ERR_INVALID_ARG: Set edge action for PCNT channel failed because of invalid argument
ESP_FAIL: Set edge action for PCNT channel failed because of other error 





```

#### pcnt_channel_set_level_action


```


esp_err_t pcnt_channel_set_level_action(pcnt_channel_handle_t chan, pcnt_channel_level_action_t high_act, pcnt_channel_level_action_t low_act)
Set channel actions when level signal changes (e.g. signal level goes from high to low). The level signal is input from the level_gpio_num configured in pcnt_chan_config_t. We use these actions to control when and how to change the counting mode. 

Parameters

chan -- [in] PCNT channel handle created by pcnt_new_channel()
high_act -- [in] Action on high level signal 
low_act -- [in] Action on low level signal 


Returns

ESP_OK: Set level action for PCNT channel successfully
ESP_ERR_INVALID_ARG: Set level action for PCNT channel failed because of invalid argument
ESP_FAIL: Set level action for PCNT channel failed because of other error 





```



### Structures


#### pcnt_watch_event_data_t


```


struct pcnt_watch_event_data_t
PCNT watch event data. 

Public Members
watch_point_value

int watch_point_value
Watch point value that triggered the event 

zero_cross_mode

pcnt_unit_zero_cross_mode_t zero_cross_mode
Zero cross mode 



```

#### pcnt_event_callbacks_t


```


struct pcnt_event_callbacks_t
Group of supported PCNT callbacks. 

Note
The callbacks are all running under ISR environment 


Note
When CONFIG_PCNT_ISR_IRAM_SAFE is enabled, the callback itself and functions callbed by it should be placed in IRAM. 


Public Members
on_reach

pcnt_watch_cb_t on_reach
Called when PCNT unit counter reaches any watch point 



```

#### pcnt_unit_config_t


```


struct pcnt_unit_config_t
PCNT unit configuration. 

Public Members
low_limit

int low_limit
Low limitation of the count unit, should be lower than 0 

high_limit

int high_limit
High limitation of the count unit, should be higher than 0 

intr_priority

int intr_priority
PCNT interrupt priority, if set to 0, the driver will try to allocate an interrupt with a relative low priority (1,2,3) 

accum_count

uint32_t accum_count
Whether to accumulate the count value when overflows at the high/low limit 

flags

struct pcnt_unit_config_t::[anonymous] flags
Extra flags 



```

#### pcnt_chan_config_t


```


struct pcnt_chan_config_t
PCNT channel configuration. 

Public Members
edge_gpio_num

int edge_gpio_num
GPIO number used by the edge signal, input mode with pull up enabled. Set to -1 if unused 

level_gpio_num

int level_gpio_num
GPIO number used by the level signal, input mode with pull up enabled. Set to -1 if unused 

invert_edge_input

uint32_t invert_edge_input
Invert the input edge signal 

invert_level_input

uint32_t invert_level_input
Invert the input level signal 

virt_edge_io_level

uint32_t virt_edge_io_level
Virtual edge IO level, 0: low, 1: high. Only valid when edge_gpio_num is set to -1 

virt_level_io_level

uint32_t virt_level_io_level
Virtual level IO level, 0: low, 1: high. Only valid when level_gpio_num is set to -1 

io_loop_back

uint32_t io_loop_back
For debug/test, the signal output from the GPIO will be fed to the input path as well 

flags

struct pcnt_chan_config_t::[anonymous] flags
Channel config flags 



```

#### pcnt_glitch_filter_config_t


```


struct pcnt_glitch_filter_config_t
PCNT glitch filter configuration. 

Public Members
max_glitch_ns

uint32_t max_glitch_ns
Pulse width smaller than this threshold will be treated as glitch and ignored, in the unit of ns 



```



### Type Definitions


#### pcnt_unit_handle_t


```


typedef struct pcnt_unit_t *pcnt_unit_handle_t
Type of PCNT unit handle. 

```

#### pcnt_channel_handle_t


```


typedef struct pcnt_chan_t *pcnt_channel_handle_t
Type of PCNT channel handle. 

```

#### pcnt_watch_cb_t


```


typedef bool (*pcnt_watch_cb_t)(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
PCNT watch event callback prototype. 

Note
The callback function is invoked from an ISR context, so it should meet the restrictions of not calling any blocking APIs when implementing the callback. e.g. must use ISR version of FreeRTOS APIs.


Param unit
[in] PCNT unit handle 

Param edata
[in] PCNT event data, fed by the driver 

Param user_ctx
[in] User data, passed from pcnt_unit_register_event_callbacks()

Return
Whether a high priority task has been woken up by this function 



```



### Header File


* components/hal/include/hal/pcnt_types.h
* This header file can be included with:



> ```
> #include "hal/pcnt_types.h"
> 
> ```




### Enumerations


#### pcnt_channel_level_action_t


```


enum pcnt_channel_level_action_t
PCNT channel action on control level. 
Values:
PCNT_CHANNEL_LEVEL_ACTION_KEEP

enumerator PCNT_CHANNEL_LEVEL_ACTION_KEEP
Keep current count mode 

PCNT_CHANNEL_LEVEL_ACTION_INVERSE

enumerator PCNT_CHANNEL_LEVEL_ACTION_INVERSE
Invert current count mode (increase -> decrease, decrease -> increase) 

PCNT_CHANNEL_LEVEL_ACTION_HOLD

enumerator PCNT_CHANNEL_LEVEL_ACTION_HOLD
Hold current count value 


```

#### pcnt_channel_edge_action_t


```


enum pcnt_channel_edge_action_t
PCNT channel action on signal edge. 
Values:
PCNT_CHANNEL_EDGE_ACTION_HOLD

enumerator PCNT_CHANNEL_EDGE_ACTION_HOLD
Hold current count value 

PCNT_CHANNEL_EDGE_ACTION_INCREASE

enumerator PCNT_CHANNEL_EDGE_ACTION_INCREASE
Increase count value 

PCNT_CHANNEL_EDGE_ACTION_DECREASE

enumerator PCNT_CHANNEL_EDGE_ACTION_DECREASE
Decrease count value 


```

#### pcnt_unit_zero_cross_mode_t


```


enum pcnt_unit_zero_cross_mode_t
PCNT unit zero cross mode. 
Values:
PCNT_UNIT_ZERO_CROSS_POS_ZERO

enumerator PCNT_UNIT_ZERO_CROSS_POS_ZERO
start from positive value, end to zero, i.e. +N->0 

PCNT_UNIT_ZERO_CROSS_NEG_ZERO

enumerator PCNT_UNIT_ZERO_CROSS_NEG_ZERO
start from negative value, end to zero, i.e. -N->0 

PCNT_UNIT_ZERO_CROSS_NEG_POS

enumerator PCNT_UNIT_ZERO_CROSS_NEG_POS
start from negative value, end to positive value, i.e. -N->+M 

PCNT_UNIT_ZERO_CROSS_POS_NEG

enumerator PCNT_UNIT_ZERO_CROSS_POS_NEG
start from positive value, end to negative value, i.e. +N->-M 


```


1
Different ESP chip series might have different number of PCNT units and channels. Please refer to the [TRM] for details. The driver does not forbid you from applying for more PCNT units and channels, but it returns error when all available hardware resources are used up. Please always check the return value when doing resource allocation (e.g., `pcnt_new_unit()`).



2
`pcnt_event_callbacks_t::on_reach` callback and the functions invoked by itself should also be placed in IRAM, you need to take care of them by themselves.