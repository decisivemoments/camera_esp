# LED Control (LEDC)




## Introduction


The LED control (LEDC) peripheral is primarily designed to control the intensity of LEDs, although it can also be used to generate PWM signals for other purposes. It has 8 channels which can generate independent waveforms that can be used, for example, to drive RGB LED devices.


The PWM controller can automatically increase or decrease the duty cycle gradually, allowing for fades without any processor interference.




## Functionality Overview


Setting up a channel of the LEDC is done in three steps. Note that unlike ESP32, ESP32-S3 only supports configuring channels in "low speed" mode.


1. Timer Configuration by specifying the PWM signal's frequency and duty cycle resolution.
2. Channel Configuration by associating it with the timer and GPIO to output the PWM signal.
3. Change PWM Signal that drives the output in order to change LED's intensity. This can be done under the full control of software or with hardware fading functions.


As an optional step, it is also possible to set up an interrupt on fade end.



!Key Settings of LED PWM Controller's API



Key Settings of LED PWM Controller's API







Note


For an initial setup, it is recommended to configure for the timers first (by calling `ledc_timer_config()`), and then for the channels (by calling `ledc_channel_config()`). This ensures the PWM frequency is at the desired value since the appearance of the PWM signal from the IO pad.




### Timer Configuration


Setting the timer is done by calling the function `ledc_timer_config()` and passing the data structure `ledc_timer_config_t` that contains the following configuration settings:


* Speed mode (value must be `LEDC_LOW_SPEED_MODE`)
* Timer number `ledc_timer_t`
* PWM signal frequency in Hz
* Resolution of PWM duty
* Source clock `ledc_clk_cfg_t`





The frequency and the duty resolution are interdependent. The higher the PWM frequency, the lower the duty resolution which is available, and vice versa. This relationship might be important if you are planning to use this API for purposes other than changing the intensity of LEDs. For more details, see Section Supported Range of Frequency and Duty Resolutions.


The source clock can also limit the PWM frequency. The higher the source clock frequency, the higher the maximum PWM frequency can be configured.




Characteristics of ESP32-S3 LEDC source clocks





| Clock name | Clock freq | Clock capabilities |
| --- | --- | --- |
| APB_CLK | 80 MHz | / |
| RC_FAST_CLK | ~ 20 MHz | Dynamic Frequency Scaling compatible, Light sleep compatible |
| XTAL_CLK | 40 MHz | Dynamic Frequency Scaling compatible |



Note


1. On ESP32-S3, if RC_FAST_CLK is chosen as the LEDC clock source, an internal calibration will be performed to get the exact frequency of the clock. This ensures the accuracy of output PWM signal frequency.


2. For ESP32-S3, all timers share one clock source. In other words, it is impossible to use different clock sources for different timers.



The LEDC driver offers a helper function `ledc_find_suitable_duty_resolution()` to find the maximum possible resolution for the timer, given the source clock frequency and the desired PWM signal frequency.


When a timer is no longer needed by any channel, it can be deconfigured by calling the same function `ledc_timer_config()`. The configuration structure `ledc_timer_config_t` passes in should be:


* `ledc_timer_config_t::speed_mode` The speed mode of the timer which wants to be deconfigured belongs to (`ledc_mode_t`)
* `ledc_timer_config_t::timer_num` The ID of the timers which wants to be deconfigured (`ledc_timer_t`)
* `ledc_timer_config_t::deconfigure` Set this to true so that the timer specified can be deconfigured




### Channel Configuration


When the timer is set up, configure the desired channel (one out of `ledc_channel_t`). This is done by calling the function `ledc_channel_config()`.


Similar to the timer configuration, the channel setup function should be passed a structure `ledc_channel_config_t` that contains the channel's configuration parameters.


At this point, the channel should start operating and generating the PWM signal on the selected GPIO, as configured in `ledc_channel_config_t`, with the frequency specified in the timer settings and the given duty cycle. The channel operation (signal generation) can be suspended at any time by calling the function `ledc_stop()`.




### Change PWM Signal


Once the channel starts operating and generating the PWM signal with the constant duty cycle and frequency, there are a couple of ways to change this signal. When driving LEDs, primarily the duty cycle is changed to vary the light intensity.


The following two sections describe how to change the duty cycle using software and hardware fading. If required, the signal's frequency can also be changed; it is covered in Section Change PWM Frequency.



Note


All the timers and channels in the ESP32-S3's LED PWM Controller only support low speed mode. Any change of PWM settings must be explicitly triggered by software (see below).




#### Change PWM Duty Cycle Using Software


To set the duty cycle, use the dedicated function `ledc_set_duty()`. After that, call `ledc_update_duty()` to activate the changes. To check the currently set value, use the corresponding `_get_` function `ledc_get_duty()`.


Another way to set the duty cycle, as well as some other channel parameters, is by calling `ledc_channel_config()` covered in Section Channel Configuration.


The range of the duty cycle values passed to functions depends on selected `duty_resolution` and should be from `0` to `(2 ** duty_resolution)`. For example, if the selected duty resolution is 10, then the duty cycle values can range from 0 to 1024. This provides the resolution of ~ 0.1%.



Warning


On ESP32-S3, when channel's binded timer selects its maximum duty resolution, the duty cycle value cannot be set to `(2 ** duty_resolution)`. Otherwise, the internal duty counter in the hardware will overflow and be messed up.





#### Change PWM Duty Cycle Using Hardware


The LEDC hardware provides the means to gradually transition from one duty cycle value to another. To use this functionality, enable fading with `ledc_fade_func_install()` and then configure it by calling one of the available fading functions:


* `ledc_set_fade_with_time()`
* `ledc_set_fade_with_step()`
* `ledc_set_fade()`


Start fading with `ledc_fade_start()`. A fade can be operated in blocking or non-blocking mode, please check `ledc_fade_mode_t` for the difference between the two available fade modes. Note that with either fade mode, the next fade or fixed-duty update will not take effect until the last fade finishes or is stopped. `ledc_fade_stop()` has to be called to stop a fade that is in progress.


To get a notification about the completion of a fade operation, a fade end callback function can be registered for each channel by calling `ledc_cb_register()` after the fade service being installed. The fade end callback prototype is defined in `ledc_cb_t`, where you should return a boolean value from the callback function, indicating whether a high priority task is woken up by this callback function. It is worth mentioning, the callback and the function invoked by itself should be placed in IRAM, as the interrupt service routine is in IRAM. `ledc_cb_register()` will print a warning message if it finds the addresses of callback and user context are incorrect.


If not required anymore, fading and an associated interrupt can be disabled with `ledc_fade_func_uninstall()`.




#### Change PWM Frequency


The LEDC API provides several ways to change the PWM frequency "on the fly":



> * Set the frequency by calling `ledc_set_freq()`. There is a corresponding function `ledc_get_freq()` to check the current frequency.
> * Change the frequency and the duty resolution by calling `ledc_bind_channel_timer()` to bind some other timer to the channel.
> * Change the channel's timer by calling `ledc_channel_config()`.




#### More Control Over PWM


There are several lower level timer-specific functions that can be used to change PWM settings:


* `ledc_timer_set()`
* `ledc_timer_rst()`
* `ledc_timer_pause()`
* `ledc_timer_resume()`


The first two functions are called "behind the scenes" by `ledc_channel_config()` to provide a startup of a timer after it is configured.





### Use Interrupts


When configuring an LEDC channel, one of the parameters selected within `ledc_channel_config_t` is `ledc_intr_type_t` which triggers an interrupt on fade completion.


For registration of a handler to address this interrupt, call `ledc_isr_register()`.





## Supported Range of Frequency and Duty Resolutions


The LED PWM Controller is designed primarily to drive LEDs. It provides a large flexibility of PWM duty cycle settings. For instance, the PWM frequency of 5 kHz can have the maximum duty resolution of 13 bits. This means that the duty can be set anywhere from 0 to 100% with a resolution of ~ 0.012% (2 \*\* 13 = 8192 discrete levels of the LED intensity). Note, however, that these parameters depend on the clock signal clocking the LED PWM Controller timer which in turn clocks the channel (see timer configuration and the **ESP32-S3 Technical Reference Manual** > **LED PWM Controller (LEDC)** [PDF]).


The LEDC can be used for generating signals at much higher frequencies that are sufficient enough to clock other devices, e.g., a digital camera module. In this case, the maximum available frequency is 40 MHz with duty resolution of 1 bit. This means that the duty cycle is fixed at 50% and cannot be adjusted.


The LEDC API is designed to report an error when trying to set a frequency and a duty resolution that exceed the range of LEDC's hardware. For example, an attempt to set the frequency to 20 MHz and the duty resolution to 3 bits results in the following error reported on a serial monitor:



```
E (196) ledc: requested frequency and duty resolution cannot be achieved, try reducing freq_hz or duty_resolution. div_param=128

```


In such a situation, either the duty resolution or the frequency must be reduced. For example, setting the duty resolution to 2 resolves this issue and makes it possible to set the duty cycle at 25% steps, i.e., at 25%, 50% or 75%.


The LEDC driver also captures and reports attempts to configure frequency/duty resolution combinations that are below the supported minimum, e.g.,:



```
E (196) ledc: requested frequency and duty resolution cannot be achieved, try increasing freq_hz or duty_resolution. div_param=128000000

```


The duty resolution is normally set using `ledc_timer_bit_t`. This enumeration covers the range from 10 to 15 bits. If a smaller duty resolution is required (from 10 down to 1), enter the equivalent numeric values directly.




## Application Example


The LEDC basic example: peripherals/ledc/ledc_basic.


The LEDC change duty cycle and fading control example: peripherals/ledc/ledc_fade.




## API Reference



### Header File


* components/esp_driver_ledc/include/driver/ledc.h
* This header file can be included with:



> ```
> #include "driver/ledc.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_ledc` component. To declare that your component depends on `esp_driver_ledc`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_ledc
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_ledc
> 
> ```




### Functions


#### ledc_channel_config


```


esp_err_t ledc_channel_config(const ledc_channel_config_t *ledc_conf)
LEDC channel configuration Configure LEDC channel with the given channel/output gpio_num/interrupt/source timer/frequency(Hz)/LEDC duty. 

Parameters
ledc_conf -- Pointer of LEDC channel configure struct

Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error 





```

#### ledc_find_suitable_duty_resolution


```


uint32_t ledc_find_suitable_duty_resolution(uint32_t src_clk_freq, uint32_t timer_freq)
Helper function to find the maximum possible duty resolution in bits for ledc_timer_config() 

Parameters

src_clk_freq -- LEDC timer source clock frequency (Hz) (See doxygen comments of ledc_clk_cfg_t or get from esp_clk_tree_src_get_freq_hz) 
timer_freq -- Desired LEDC timer frequency (Hz)


Returns

0 The timer frequency cannot be achieved
Others The largest duty resolution value to be set 





```

#### ledc_timer_config


```


esp_err_t ledc_timer_config(const ledc_timer_config_t *timer_conf)
LEDC timer configuration Configure LEDC timer with the given source timer/frequency(Hz)/duty_resolution. 

Parameters
timer_conf -- Pointer of LEDC timer configure struct

Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_FAIL Can not find a proper pre-divider number base on the given frequency and the current duty_resolution.
ESP_ERR_INVALID_STATE Timer cannot be de-configured because timer is not configured or is not paused 





```

#### ledc_update_duty


```


esp_err_t ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel)
LEDC update channel parameters. 

Note
Call this function to activate the LEDC updated parameters. After ledc_set_duty, we need to call this function to update the settings. And the new LEDC parameters don't take effect until the next PWM cycle. 


Note
ledc_set_duty, ledc_set_duty_with_hpoint and ledc_update_duty are not thread-safe, do not call these functions to control one LEDC channel in different tasks at the same time. A thread-safe version of API is ledc_set_duty_and_update 


Note
If CONFIG_LEDC_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Cache is disabled. 


Note
This function is allowed to run within ISR context.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error 





```

#### ledc_set_pin


```


esp_err_t ledc_set_pin(int gpio_num, ledc_mode_t speed_mode, ledc_channel_t ledc_channel)
Set LEDC output gpio. 

Note
This function only routes the LEDC signal to GPIO through matrix, other LEDC resources initialization are not involved. Please use ledc_channel_config() instead to fully configure a LEDC channel.


Parameters

gpio_num -- The LEDC output gpio 
speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
ledc_channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error 





```

#### ledc_stop


```


esp_err_t ledc_stop(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t idle_level)
LEDC stop. Disable LEDC output, and set idle level. 

Note
If CONFIG_LEDC_CTRL_FUNC_IN_IRAM is enabled, this function will be placed in the IRAM by linker, makes it possible to execute even when the Cache is disabled. 


Note
This function is allowed to run within ISR context.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
idle_level -- Set output idle level after LEDC stops.


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error 





```

#### ledc_set_freq


```


esp_err_t ledc_set_freq(ledc_mode_t speed_mode, ledc_timer_t timer_num, uint32_t freq_hz)
LEDC set channel frequency (Hz) 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
timer_num -- LEDC timer index (0-3), select from ledc_timer_t 
freq_hz -- Set the LEDC frequency


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_FAIL Can not find a proper pre-divider number base on the given frequency and the current duty_resolution. 





```

#### ledc_get_freq


```


uint32_t ledc_get_freq(ledc_mode_t speed_mode, ledc_timer_t timer_num)
LEDC get channel frequency (Hz) 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
timer_num -- LEDC timer index (0-3), select from ledc_timer_t


Returns

0 error
Others Current LEDC frequency 





```

#### ledc_set_duty_with_hpoint


```


esp_err_t ledc_set_duty_with_hpoint(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty, uint32_t hpoint)
LEDC set duty and hpoint value Only after calling ledc_update_duty will the duty update. 

Note
ledc_set_duty, ledc_set_duty_with_hpoint and ledc_update_duty are not thread-safe, do not call these functions to control one LEDC channel in different tasks at the same time. A thread-safe version of API is ledc_set_duty_and_update 


Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
duty -- Set the LEDC duty, the range of duty setting is [0, (2**duty_resolution)] 
hpoint -- Set the LEDC hpoint value, the range is [0, (2**duty_resolution)-1]


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error 





```

#### ledc_get_hpoint


```


int ledc_get_hpoint(ledc_mode_t speed_mode, ledc_channel_t channel)
LEDC get hpoint value, the counter value when the output is set high level. 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t


Returns

LEDC_ERR_VAL if parameter error
Others Current hpoint value of LEDC channel 





```

#### ledc_set_duty


```


esp_err_t ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)
LEDC set duty This function do not change the hpoint value of this channel. if needed, please call ledc_set_duty_with_hpoint. only after calling ledc_update_duty will the duty update. 

Note
ledc_set_duty, ledc_set_duty_with_hpoint and ledc_update_duty are not thread-safe, do not call these functions to control one LEDC channel in different tasks at the same time. A thread-safe version of API is ledc_set_duty_and_update. 


Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
duty -- Set the LEDC duty, the range of duty setting is [0, (2**duty_resolution)]


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error 





```

#### ledc_get_duty


```


uint32_t ledc_get_duty(ledc_mode_t speed_mode, ledc_channel_t channel)
LEDC get duty This function returns the duty at the present PWM cycle. You shouldn't expect the function to return the new duty in the same cycle of calling ledc_update_duty, because duty update doesn't take effect until the next cycle. 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t


Returns

LEDC_ERR_DUTY if parameter error
Others Current LEDC duty 





```

#### ledc_set_fade


```


esp_err_t ledc_set_fade(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty, ledc_duty_direction_t fade_direction, uint32_t step_num, uint32_t duty_cycle_num, uint32_t duty_scale)
LEDC set gradient Set LEDC gradient, After the function calls the ledc_update_duty function, the function can take effect. 

Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
duty -- Set the start of the gradient duty, the range of duty setting is [0, (2**duty_resolution)] 
fade_direction -- Set the direction of the gradient 
step_num -- Set the number of the gradient 
duty_cycle_num -- Set how many LEDC tick each time the gradient lasts 
duty_scale -- Set gradient change amplitude


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error 





```

#### ledc_isr_register


```


esp_err_t ledc_isr_register(void (*fn)(void*), void *arg, int intr_alloc_flags, ledc_isr_handle_t *handle)
Register LEDC interrupt handler, the handler is an ISR. The handler will be attached to the same CPU core that this function is running on. 

Parameters

fn -- Interrupt handler function. 
arg -- User-supplied argument passed to the handler function. 
intr_alloc_flags -- Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info. 
handle -- Pointer to return handle. If non-NULL, a handle for the interrupt will be returned here.


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_ERR_NOT_FOUND Failed to find available interrupt source 





```

#### ledc_timer_set


```


esp_err_t ledc_timer_set(ledc_mode_t speed_mode, ledc_timer_t timer_sel, uint32_t clock_divider, uint32_t duty_resolution, ledc_clk_src_t clk_src)
Configure LEDC settings. 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
timer_sel -- Timer index (0-3), there are 4 timers in LEDC module 
clock_divider -- Timer clock divide value, the timer clock is divided from the selected clock source 
duty_resolution -- Resolution of duty setting in number of bits. The range is [1, SOC_LEDC_TIMER_BIT_WIDTH] 
clk_src -- Select LEDC source clock.


Returns

(-1) Parameter error
Other Current LEDC duty 





```

#### ledc_timer_rst


```


esp_err_t ledc_timer_rst(ledc_mode_t speed_mode, ledc_timer_t timer_sel)
Reset LEDC timer. 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
timer_sel -- LEDC timer index (0-3), select from ledc_timer_t


Returns

ESP_ERR_INVALID_ARG Parameter error
ESP_OK Success 





```

#### ledc_timer_pause


```


esp_err_t ledc_timer_pause(ledc_mode_t speed_mode, ledc_timer_t timer_sel)
Pause LEDC timer counter. 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
timer_sel -- LEDC timer index (0-3), select from ledc_timer_t


Returns

ESP_ERR_INVALID_ARG Parameter error
ESP_OK Success 





```

#### ledc_timer_resume


```


esp_err_t ledc_timer_resume(ledc_mode_t speed_mode, ledc_timer_t timer_sel)
Resume LEDC timer. 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
timer_sel -- LEDC timer index (0-3), select from ledc_timer_t


Returns

ESP_ERR_INVALID_ARG Parameter error
ESP_OK Success 





```

#### ledc_bind_channel_timer


```


esp_err_t ledc_bind_channel_timer(ledc_mode_t speed_mode, ledc_channel_t channel, ledc_timer_t timer_sel)
Bind LEDC channel with the selected timer. 

Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel index (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
timer_sel -- LEDC timer index (0-3), select from ledc_timer_t


Returns

ESP_ERR_INVALID_ARG Parameter error
ESP_OK Success 





```

#### ledc_set_fade_with_step


```


esp_err_t ledc_set_fade_with_step(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, uint32_t scale, uint32_t cycle_num)
Set LEDC fade function. 

Note
Call ledc_fade_func_install() once before calling this function. Call ledc_fade_start() after this to start fading. 


Note
ledc_set_fade_with_step, ledc_set_fade_with_time and ledc_fade_start are not thread-safe, do not call these functions to control one LEDC channel in different tasks at the same time. A thread-safe version of API is ledc_set_fade_step_and_start 


Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel index (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
target_duty -- Target duty of fading [0, (2**duty_resolution)] 
scale -- Controls the increase or decrease step scale. 
cycle_num -- increase or decrease the duty every cycle_num cycles


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_ERR_INVALID_STATE Channel not initialized
ESP_FAIL Fade function init error 





```

#### ledc_set_fade_with_time


```


esp_err_t ledc_set_fade_with_time(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, int max_fade_time_ms)
Set LEDC fade function, with a limited time. 

Note
Call ledc_fade_func_install() once before calling this function. Call ledc_fade_start() after this to start fading. 


Note
ledc_set_fade_with_step, ledc_set_fade_with_time and ledc_fade_start are not thread-safe, do not call these functions to control one LEDC channel in different tasks at the same time. A thread-safe version of API is ledc_set_fade_step_and_start 


Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel index (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
target_duty -- Target duty of fading [0, (2**duty_resolution)] 
max_fade_time_ms -- The maximum time of the fading ( ms ).


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_ERR_INVALID_STATE Channel not initialized
ESP_FAIL Fade function init error 





```

#### ledc_fade_func_install


```


esp_err_t ledc_fade_func_install(int intr_alloc_flags)
Install LEDC fade function. This function will occupy interrupt of LEDC module. 

Parameters
intr_alloc_flags -- Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info.

Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Intr flag error
ESP_ERR_NOT_FOUND Failed to find available interrupt source
ESP_ERR_INVALID_STATE Fade function already installed 





```

#### ledc_fade_func_uninstall


```


void ledc_fade_func_uninstall(void)
Uninstall LEDC fade function. 

```

#### ledc_fade_start


```


esp_err_t ledc_fade_start(ledc_mode_t speed_mode, ledc_channel_t channel, ledc_fade_mode_t fade_mode)
Start LEDC fading. 

Note
Call ledc_fade_func_install() once before calling this function. Call this API right after ledc_set_fade_with_time or ledc_set_fade_with_step before to start fading. 


Note
Starting fade operation with this API is not thread-safe, use with care. 


Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel number 
fade_mode -- Whether to block until fading done. See ledc_types.h ledc_fade_mode_t for more info. Note that this function will not return until fading to the target duty if LEDC_FADE_WAIT_DONE mode is selected.


Returns

ESP_OK Success
ESP_ERR_INVALID_STATE Channel not initialized or fade function not installed.
ESP_ERR_INVALID_ARG Parameter error. 





```

#### ledc_fade_stop


```


esp_err_t ledc_fade_stop(ledc_mode_t speed_mode, ledc_channel_t channel)
Stop LEDC fading. The duty of the channel is guaranteed to be fixed at most one PWM cycle after the function returns. 

Note
This API can be called if a new fixed duty or a new fade want to be set while the last fade operation is still running in progress. 


Note
Call this API will abort the fading operation only if it was started by calling ledc_fade_start with LEDC_FADE_NO_WAIT mode. 


Note
If a fade was started with LEDC_FADE_WAIT_DONE mode, calling this API afterwards has no use in stopping the fade. Fade will continue until it reaches the target duty.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel number


Returns

ESP_OK Success
ESP_ERR_INVALID_STATE Channel not initialized
ESP_ERR_INVALID_ARG Parameter error
ESP_FAIL Fade function init error 





```

#### ledc_set_duty_and_update


```


esp_err_t ledc_set_duty_and_update(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty, uint32_t hpoint)
A thread-safe API to set duty for LEDC channel and return when duty updated. 

Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
duty -- Set the LEDC duty, the range of duty setting is [0, (2**duty_resolution)] 
hpoint -- Set the LEDC hpoint value, the range is [0, (2**duty_resolution)-1]


Returns

ESP_OK Success
ESP_ERR_INVALID_STATE Channel not initialized
ESP_ERR_INVALID_ARG Parameter error
ESP_FAIL Fade function init error 





```

#### ledc_set_fade_time_and_start


```


esp_err_t ledc_set_fade_time_and_start(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, uint32_t max_fade_time_ms, ledc_fade_mode_t fade_mode)
A thread-safe API to set and start LEDC fade function, with a limited time. 

Note
Call ledc_fade_func_install() once, before calling this function. 


Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel index (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
target_duty -- Target duty of fading [0, (2**duty_resolution)] 
max_fade_time_ms -- The maximum time of the fading ( ms ). 
fade_mode -- choose blocking or non-blocking mode


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_ERR_INVALID_STATE Channel not initialized
ESP_FAIL Fade function init error 





```

#### ledc_set_fade_step_and_start


```


esp_err_t ledc_set_fade_step_and_start(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, uint32_t scale, uint32_t cycle_num, ledc_fade_mode_t fade_mode)
A thread-safe API to set and start LEDC fade function. 

Note
Call ledc_fade_func_install() once before calling this function. 


Note
For ESP32, hardware does not support any duty change while a fade operation is running in progress on that channel. Other duty operations will have to wait until the fade operation has finished.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel index (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
target_duty -- Target duty of fading [0, (2**duty_resolution)] 
scale -- Controls the increase or decrease step scale. 
cycle_num -- increase or decrease the duty every cycle_num cycles 
fade_mode -- choose blocking or non-blocking mode


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_ERR_INVALID_STATE Channel not initialized
ESP_FAIL Fade function init error 





```

#### ledc_cb_register


```


esp_err_t ledc_cb_register(ledc_mode_t speed_mode, ledc_channel_t channel, ledc_cbs_t *cbs, void *user_arg)
LEDC callback registration function. 

Note
The callback is called from an ISR, it must never attempt to block, and any FreeRTOS API called must be ISR capable.


Parameters

speed_mode -- Select the LEDC channel group with specified speed mode. Note that not all targets support high speed mode. 
channel -- LEDC channel index (0 - LEDC_CHANNEL_MAX-1), select from ledc_channel_t 
cbs -- Group of LEDC callback functions 
user_arg -- user registered data for the callback function


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Parameter error
ESP_ERR_INVALID_STATE Channel not initialized
ESP_FAIL Fade function init error 





```



### Structures


#### ledc_channel_config_t


```


struct ledc_channel_config_t
Configuration parameters of LEDC channel for ledc_channel_config function. 

Public Members
gpio_num

int gpio_num
the LEDC output gpio_num, if you want to use gpio16, gpio_num = 16 

speed_mode

ledc_mode_t speed_mode
LEDC speed speed_mode, high-speed mode (only exists on esp32) or low-speed mode 

channel

ledc_channel_t channel
LEDC channel (0 - LEDC_CHANNEL_MAX-1) 

intr_type

ledc_intr_type_t intr_type
configure interrupt, Fade interrupt enable or Fade interrupt disable 

timer_sel

ledc_timer_t timer_sel
Select the timer source of channel (0 - LEDC_TIMER_MAX-1) 

duty

uint32_t duty
LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)] 

hpoint

int hpoint
LEDC channel hpoint value, the range is [0, (2**duty_resolution)-1] 

output_invert

unsigned int output_invert
Enable (1) or disable (0) gpio output invert 

flags

struct ledc_channel_config_t::[anonymous] flags
LEDC flags 



```

#### ledc_timer_config_t


```


struct ledc_timer_config_t
Configuration parameters of LEDC timer for ledc_timer_config function. 

Public Members
speed_mode

ledc_mode_t speed_mode
LEDC speed speed_mode, high-speed mode (only exists on esp32) or low-speed mode 

duty_resolution

ledc_timer_bit_t duty_resolution
LEDC channel duty resolution 

timer_num

ledc_timer_t timer_num
The timer source of channel (0 - LEDC_TIMER_MAX-1) 

freq_hz

uint32_t freq_hz
LEDC timer frequency (Hz) 

clk_cfg

ledc_clk_cfg_t clk_cfg
Configure LEDC source clock from ledc_clk_cfg_t. Note that LEDC_USE_RC_FAST_CLK and LEDC_USE_XTAL_CLK are non-timer-specific clock sources. You can not have one LEDC timer uses RC_FAST_CLK as the clock source and have another LEDC timer uses XTAL_CLK as its clock source. All chips except esp32 and esp32s2 do not have timer-specific clock sources, which means clock source for all timers must be the same one. 

deconfigure

bool deconfigure
Set this field to de-configure a LEDC timer which has been configured before Note that it will not check whether the timer wants to be de-configured is binded to any channel. Also, the timer has to be paused first before it can be de-configured. When this field is set, duty_resolution, freq_hz, clk_cfg fields are ignored. 



```

#### ledc_cb_param_t


```


struct ledc_cb_param_t
LEDC callback parameter. 

Public Members
event

ledc_cb_event_t event
Event name 

speed_mode

uint32_t speed_mode
Speed mode of the LEDC channel group 

channel

uint32_t channel
LEDC channel (0 - LEDC_CHANNEL_MAX-1) 

duty

uint32_t duty
LEDC current duty of the channel, the range of duty is [0, (2**duty_resolution)] 



```

#### ledc_cbs_t


```


struct ledc_cbs_t
Group of supported LEDC callbacks. 

Note
The callbacks are all running under ISR environment 


Public Members
fade_cb

ledc_cb_t fade_cb
LEDC fade_end callback function 



```



### Macros


#### LEDC_APB_CLK_HZ


```


LEDC_APB_CLK_HZ
Frequency of one of the LEDC peripheral clock sources, APB_CLK. 

Note
This macro should have no use in your application, we keep it here only for backward compatible 


```

#### LEDC_ERR_DUTY


```


LEDC_ERR_DUTY

```

#### LEDC_ERR_VAL


```


LEDC_ERR_VAL

```



### Type Definitions


#### ledc_isr_handle_t


```


typedef intr_handle_t ledc_isr_handle_t

```

#### ledc_cb_t


```


typedef bool (*ledc_cb_t)(const ledc_cb_param_t *param, void *user_arg)
Type of LEDC event callback. 

Param param
LEDC callback parameter 

Param user_arg
User registered data 

Return
Whether a high priority task has been waken up by this function 



```



### Enumerations


#### ledc_cb_event_t


```


enum ledc_cb_event_t
LEDC callback event type. 
Values:
LEDC_FADE_END_EVT

enumerator LEDC_FADE_END_EVT
LEDC fade end event 


```



### Header File


* components/hal/include/hal/ledc_types.h
* This header file can be included with:



> ```
> #include "hal/ledc_types.h"
> 
> ```




### Type Definitions


#### ledc_clk_cfg_t


```


typedef soc_periph_ledc_clk_src_legacy_t ledc_clk_cfg_t
LEDC clock source configuration struct. 
In theory, the following enumeration shall be placed in LEDC driver's header. However, as the next enumeration, ledc_clk_src_t, makes the use of some of these values and to avoid mutual inclusion of the headers, we must define it here. 

```



### Enumerations


#### ledc_mode_t


```


enum ledc_mode_t
Values:
LEDC_LOW_SPEED_MODE

enumerator LEDC_LOW_SPEED_MODE
LEDC low speed speed_mode 

LEDC_SPEED_MODE_MAX

enumerator LEDC_SPEED_MODE_MAX
LEDC speed limit 


```

#### ledc_intr_type_t


```


enum ledc_intr_type_t
Values:
LEDC_INTR_DISABLE

enumerator LEDC_INTR_DISABLE
Disable LEDC interrupt 

LEDC_INTR_FADE_END

enumerator LEDC_INTR_FADE_END
Enable LEDC interrupt 

LEDC_INTR_MAX

enumerator LEDC_INTR_MAX


```

#### ledc_duty_direction_t


```


enum ledc_duty_direction_t
Values:
LEDC_DUTY_DIR_DECREASE

enumerator LEDC_DUTY_DIR_DECREASE
LEDC duty decrease direction 

LEDC_DUTY_DIR_INCREASE

enumerator LEDC_DUTY_DIR_INCREASE
LEDC duty increase direction 

LEDC_DUTY_DIR_MAX

enumerator LEDC_DUTY_DIR_MAX


```

#### ledc_slow_clk_sel_t


```


enum ledc_slow_clk_sel_t
LEDC global clock sources. 
Values:
LEDC_SLOW_CLK_RC_FAST

enumerator LEDC_SLOW_CLK_RC_FAST
LEDC low speed timer clock source is RC_FAST clock 

LEDC_SLOW_CLK_APB

enumerator LEDC_SLOW_CLK_APB
LEDC low speed timer clock source is 80MHz APB clock 

LEDC_SLOW_CLK_XTAL

enumerator LEDC_SLOW_CLK_XTAL
LEDC low speed timer clock source XTAL clock 

LEDC_SLOW_CLK_RTC8M

enumerator LEDC_SLOW_CLK_RTC8M
Alias of 'LEDC_SLOW_CLK_RC_FAST' 


```

#### ledc_clk_src_t


```


enum ledc_clk_src_t
LEDC timer-specific clock sources. 
Note: Setting numeric values to match ledc_clk_cfg_t values are a hack to avoid collision with LEDC_AUTO_CLK in the driver, as these enums have very similar names and user may pass one of these by mistake. 
Values:
LEDC_APB_CLK

enumerator LEDC_APB_CLK
LEDC timer clock divided from APB clock (80Mhz) 

LEDC_SCLK

enumerator LEDC_SCLK
Selecting this value for LEDC_TICK_SEL_TIMER let the hardware take its source clock from LEDC_APB_CLK_SEL 


```

#### ledc_timer_t


```


enum ledc_timer_t
Values:
LEDC_TIMER_0

enumerator LEDC_TIMER_0
LEDC timer 0 

LEDC_TIMER_1

enumerator LEDC_TIMER_1
LEDC timer 1 

LEDC_TIMER_2

enumerator LEDC_TIMER_2
LEDC timer 2 

LEDC_TIMER_3

enumerator LEDC_TIMER_3
LEDC timer 3 

LEDC_TIMER_MAX

enumerator LEDC_TIMER_MAX


```

#### ledc_channel_t


```


enum ledc_channel_t
Values:
LEDC_CHANNEL_0

enumerator LEDC_CHANNEL_0
LEDC channel 0 

LEDC_CHANNEL_1

enumerator LEDC_CHANNEL_1
LEDC channel 1 

LEDC_CHANNEL_2

enumerator LEDC_CHANNEL_2
LEDC channel 2 

LEDC_CHANNEL_3

enumerator LEDC_CHANNEL_3
LEDC channel 3 

LEDC_CHANNEL_4

enumerator LEDC_CHANNEL_4
LEDC channel 4 

LEDC_CHANNEL_5

enumerator LEDC_CHANNEL_5
LEDC channel 5 

LEDC_CHANNEL_6

enumerator LEDC_CHANNEL_6
LEDC channel 6 

LEDC_CHANNEL_7

enumerator LEDC_CHANNEL_7
LEDC channel 7 

LEDC_CHANNEL_MAX

enumerator LEDC_CHANNEL_MAX


```

#### ledc_timer_bit_t


```


enum ledc_timer_bit_t
Values:
LEDC_TIMER_1_BIT

enumerator LEDC_TIMER_1_BIT
LEDC PWM duty resolution of 1 bits 

LEDC_TIMER_2_BIT

enumerator LEDC_TIMER_2_BIT
LEDC PWM duty resolution of 2 bits 

LEDC_TIMER_3_BIT

enumerator LEDC_TIMER_3_BIT
LEDC PWM duty resolution of 3 bits 

LEDC_TIMER_4_BIT

enumerator LEDC_TIMER_4_BIT
LEDC PWM duty resolution of 4 bits 

LEDC_TIMER_5_BIT

enumerator LEDC_TIMER_5_BIT
LEDC PWM duty resolution of 5 bits 

LEDC_TIMER_6_BIT

enumerator LEDC_TIMER_6_BIT
LEDC PWM duty resolution of 6 bits 

LEDC_TIMER_7_BIT

enumerator LEDC_TIMER_7_BIT
LEDC PWM duty resolution of 7 bits 

LEDC_TIMER_8_BIT

enumerator LEDC_TIMER_8_BIT
LEDC PWM duty resolution of 8 bits 

LEDC_TIMER_9_BIT

enumerator LEDC_TIMER_9_BIT
LEDC PWM duty resolution of 9 bits 

LEDC_TIMER_10_BIT

enumerator LEDC_TIMER_10_BIT
LEDC PWM duty resolution of 10 bits 

LEDC_TIMER_11_BIT

enumerator LEDC_TIMER_11_BIT
LEDC PWM duty resolution of 11 bits 

LEDC_TIMER_12_BIT

enumerator LEDC_TIMER_12_BIT
LEDC PWM duty resolution of 12 bits 

LEDC_TIMER_13_BIT

enumerator LEDC_TIMER_13_BIT
LEDC PWM duty resolution of 13 bits 

LEDC_TIMER_14_BIT

enumerator LEDC_TIMER_14_BIT
LEDC PWM duty resolution of 14 bits 

LEDC_TIMER_BIT_MAX

enumerator LEDC_TIMER_BIT_MAX


```

#### ledc_fade_mode_t


```


enum ledc_fade_mode_t
Values:
LEDC_FADE_NO_WAIT

enumerator LEDC_FADE_NO_WAIT
LEDC fade function will return immediately 

LEDC_FADE_WAIT_DONE

enumerator LEDC_FADE_WAIT_DONE
LEDC fade function will block until fading to the target duty 

LEDC_FADE_MAX

enumerator LEDC_FADE_MAX


```