# Sigma-Delta Modulation (SDM)




## Introduction


ESP32-S3 has a second-order sigma-delta modulator, which can generate independent PDM pulses to multiple channels. Please refer to the TRM to check how many hardware channels are available. 1


Delta-sigma modulation converts an analog voltage signal into a pulse frequency, or pulse density, which can be understood as pulse-density modulation (PDM) (refer to Delta-sigma modulation on Wikipedia).


The main differences comparing to I2S PDM mode and DAC peripheral are:


1. SDM has no clock signal, it is just like the DAC mode of PDM;
2. SDM has no DMA, and it can not change its output density continuously. If you have to, you can update the density in a timer's callback;
3. Based on the former two points, unlike the DAC peripheral, an external active or passive low-pass filter is required additionally to restore the analog wave (See Convert to an Analog Signal (Optional)).


Typically, a Sigma-Delta modulated channel can be used in scenarios like:


* LED dimming
* Simple DAC (8-bit), with the help of an active RC low-pass filter
* Class D amplifier, with the help of a half-bridge or full-bridge circuit plus an LC low-pass filter




## Functional Overview


The following sections of this document cover the typical steps to install and operate an SDM channel:


* Resource Allocation - covers how to initialize and configure an SDM channel and how to recycle the resources when it finishes working.
* Enable and Disable Channel - covers how to enable and disable the channel.
* Set Pulse Density - describes how to set the equivalent duty cycle of the PDM pulses.
* Power Management - describes how different source clock selections can affect power consumption.
* IRAM Safe - lists which functions are supposed to work even when the cache is disabled.
* Thread Safety - lists which APIs are guaranteed to be thread-safe by the driver.
* Kconfig Options - lists the supported Kconfig options that can be used to make a different effect on driver behavior.



### Resource Allocation


In ESP-IDF, the information and attributes of SDM channels are managed and accessed through specific data structures, where the data structure is called `sdm_channel_handle_t`. Each channel is capable to output the binary, hardware-generated signal with the sigma-delta modulation. The driver manages all available channels in a pool so that there is no need to manually assign a fixed channel to a GPIO.


To install an SDM channel, you should call `sdm_new_channel()` to get a channel handle. Channel-specific configurations are passed in the `sdm_config_t` structure:


* `sdm_config_t::gpio_num` sets the GPIO that the PDM pulses output from.
* `sdm_config_t::clk_src` selects the source clock for the SDM module. Note that, all channels should select the same clock source.
* `sdm_config_t::sample_rate_hz` sets the sample rate of the SDM module.
* `sdm_config_t::invert_out` sets whether to invert the output signal.
* `sdm_config_t::io_loop_back` is for debugging purposes only. It enables both the GPIO's input and output ability through the GPIO matrix peripheral.


The function `sdm_new_channel()` can fail due to various errors such as insufficient memory, invalid arguments, etc. Specifically, when there are no more free channels (i.e., all hardware SDM channels have been used up), `ESP_ERR_NOT_FOUND` will be returned.


If a previously created SDM channel is no longer required, you should recycle it by calling `sdm_del_channel()`. It allows the underlying HW channel to be used for other purposes. Before deleting an SDM channel handle, you should disable it by `sdm_channel_disable()` in advance or make sure it has not been enabled yet by `sdm_channel_enable()`.



#### Creating an SDM Channel with a Sample Rate of 1 MHz



```
 sdm_channel_handle_t chan = NULL;
 sdm_config_t config = {
     .clk_src = SDM_CLK_SRC_DEFAULT,
     .sample_rate_hz = 1 * 1000 * 1000,
     .gpio_num = 0,
 };
ESP_ERROR_CHECK(sdm_new_channel(&config, &chan));

```





### Enable and Disable Channel


Before doing further IO control to the SDM channel, you should enable it first, by calling `sdm_channel_enable()`. Internally, this function:


* switches the channel state from **init** to **enable**
* acquires a proper power management lock if a specific clock source (e.g., APB clock) is selected. See also Power Management for more information.


On the contrary, calling `sdm_channel_disable()` does the opposite, that is, put the channel back to the **init** state and releases the power management lock.




### Set Pulse Density


For the output PDM signals, the pulse density decides the output analog voltage that is restored by a low-pass filter. The restored analog voltage from the channel is calculated by `Vout = VDD_IO / 256 * duty + VDD_IO / 2`. The range of the quantized `density` input parameter of `sdm_channel_set_pulse_density()` is from -128 to 127 (8-bit signed integer). Depending on the value of the `density` parameter, the duty cycle of the output signal will be changed accordingly. For example, if a zero value is set, then the output signal's duty will be around 50%.




### Power Management


When power management is enabled (i.e., CONFIG_PM_ENABLE is on), the system will adjust the APB frequency before going into Light-sleep, thus potentially changing the sample rate of the sigma-delta modulator.


However, the driver can prevent the system from changing APB frequency by acquiring a power management lock of type `ESP_PM_APB_FREQ_MAX`. Whenever the driver creates an SDM channel instance that has selected `SDM_CLK_SRC_APB` as its clock source, the driver guarantees that the power management lock is acquired when enabling the channel by `sdm_channel_enable()`. Likewise, the driver releases the lock when `sdm_channel_disable()` is called for that channel.




### IRAM Safe


There is a Kconfig option CONFIG_SDM_CTRL_FUNC_IN_IRAM that can put commonly-used IO control functions into IRAM as well. So that these functions can also be executable when the cache is disabled. These IO control functions are listed as follows:


* `sdm_channel_set_pulse_density()`




### Thread Safety


The factory function `sdm_new_channel()` is guaranteed to be thread-safe by the driver, which means, the user can call it from different RTOS tasks without protection by extra locks.


The following functions are allowed to run under ISR context, the driver uses a critical section to prevent them being called concurrently in both task and ISR.


* `sdm_channel_set_pulse_density()`


Other functions that take the `sdm_channel_handle_t` as the first positional parameter, are not treated as thread-safe. This means the user should avoid calling them from multiple tasks.




### Kconfig Options


* CONFIG_SDM_CTRL_FUNC_IN_IRAM controls where to place the SDM channel control functions (IRAM or Flash), see IRAM Safe for more information.
* CONFIG_SDM_ENABLE_DEBUG_LOG is used to enable the debug log output. Enabling this option increases the firmware binary size.





## Convert to an Analog Signal (Optional)


Typically, if a Sigma-Delta signal is connected to an LED to adjust the brightness, you do not have to add any filter between them, because our eyes have their own low-pass filters for changes in light intensity. However, if you want to check the real voltage or watch the analog waveform, you need to design an analog low-pass filter. Also, it is recommended to use an active filter instead of a passive filter to gain better isolation and not lose too much voltage.


For example, you can take the following Sallen-Key topology Low Pass Filter as a reference.



!Sallen-Key Low Pass Filter



Sallen-Key Low Pass Filter








## Application Example


* 100 Hz sine wave that is modulated with Sigma-Delta: peripherals/sigma_delta/sdm_dac.
* LED driven by a GPIO that is modulated with Sigma-Delta: peripherals/sigma_delta/sdm_led.




## API Reference



### Header File


* components/esp_driver_sdm/include/driver/sdm.h
* This header file can be included with:



> ```
> #include "driver/sdm.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_sdm` component. To declare that your component depends on `esp_driver_sdm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_sdm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_sdm
> 
> ```




### Functions


#### sdm_new_channel


```


esp_err_t sdm_new_channel(const sdm_config_t *config, sdm_channel_handle_t *ret_chan)
Create a new Sigma Delta channel. 

Parameters

config -- [in] SDM configuration 
ret_chan -- [out] Returned SDM channel handle 


Returns

ESP_OK: Create SDM channel successfully
ESP_ERR_INVALID_ARG: Create SDM channel failed because of invalid argument
ESP_ERR_NO_MEM: Create SDM channel failed because out of memory
ESP_ERR_NOT_FOUND: Create SDM channel failed because all channels are used up and no more free one
ESP_FAIL: Create SDM channel failed because of other error 





```

#### sdm_del_channel


```


esp_err_t sdm_del_channel(sdm_channel_handle_t chan)
Delete the Sigma Delta channel. 

Parameters
chan -- [in] SDM channel created by sdm_new_channel

Returns

ESP_OK: Delete the SDM channel successfully
ESP_ERR_INVALID_ARG: Delete the SDM channel failed because of invalid argument
ESP_ERR_INVALID_STATE: Delete the SDM channel failed because the channel is not in init state
ESP_FAIL: Delete the SDM channel failed because of other error 





```

#### sdm_channel_enable


```


esp_err_t sdm_channel_enable(sdm_channel_handle_t chan)
Enable the Sigma Delta channel. 

Note
This function will transit the channel state from init to enable. 


Note
This function will acquire a PM lock, if a specific source clock (e.g. APB) is selected in the sdm_config_t, while CONFIG_PM_ENABLE is enabled.


Parameters
chan -- [in] SDM channel created by sdm_new_channel

Returns

ESP_OK: Enable SDM channel successfully
ESP_ERR_INVALID_ARG: Enable SDM channel failed because of invalid argument
ESP_ERR_INVALID_STATE: Enable SDM channel failed because the channel is already enabled
ESP_FAIL: Enable SDM channel failed because of other error 





```

#### sdm_channel_disable


```


esp_err_t sdm_channel_disable(sdm_channel_handle_t chan)
Disable the Sigma Delta channel. 

Note
This function will do the opposite work to the sdm_channel_enable()


Parameters
chan -- [in] SDM channel created by sdm_new_channel

Returns

ESP_OK: Disable SDM channel successfully
ESP_ERR_INVALID_ARG: Disable SDM channel failed because of invalid argument
ESP_ERR_INVALID_STATE: Disable SDM channel failed because the channel is not enabled yet
ESP_FAIL: Disable SDM channel failed because of other error 





```

#### sdm_channel_set_pulse_density


```


esp_err_t sdm_channel_set_pulse_density(sdm_channel_handle_t chan, int8_t density)
Set the pulse density of the PDM output signal. 

Note
The raw output signal requires a low-pass filter to restore it into analog voltage, the restored analog output voltage could be Vout = VDD_IO / 256 * density + VDD_IO / 2 


Note
This function is allowed to run within ISR context 


Note
This function will be placed into IRAM if CONFIG_SDM_CTRL_FUNC_IN_IRAM is on, so that it's allowed to be executed when Cache is disabled


Parameters

chan -- [in] SDM channel created by sdm_new_channel
density -- [in] Quantized pulse density of the PDM output signal, ranges from -128 to 127. But the range of [-90, 90] can provide a better randomness. 


Returns

ESP_OK: Set pulse density successfully
ESP_ERR_INVALID_ARG: Set pulse density failed because of invalid argument
ESP_FAIL: Set pulse density failed because of other error 





```

#### sdm_channel_set_duty


```


esp_err_t sdm_channel_set_duty(sdm_channel_handle_t chan, int8_t duty)
The alias function of sdm_channel_set_pulse_density, it decides the pulse density of the output signal. 

Note
sdm_channel_set_pulse_density has a more appropriate name compare this alias function, suggest to turn to sdm_channel_set_pulse_density instead


Parameters

chan -- [in] SDM channel created by sdm_new_channel
duty -- [in] Actually it's the quantized pulse density of the PDM output signal


Returns

ESP_OK: Set duty cycle successfully
ESP_ERR_INVALID_ARG: Set duty cycle failed because of invalid argument
ESP_FAIL: Set duty cycle failed because of other error 





```



### Structures


#### sdm_config_t


```


struct sdm_config_t
Sigma Delta channel configuration. 

Public Members
gpio_num

int gpio_num
GPIO number 

clk_src

sdm_clock_source_t clk_src
Clock source 

sample_rate_hz

uint32_t sample_rate_hz
Over sample rate in Hz, it determines the frequency of the carrier pulses 

invert_out

uint32_t invert_out
Whether to invert the output signal 

io_loop_back

uint32_t io_loop_back
For debug/test, the signal output from the GPIO will be fed to the input path as well 

flags

struct sdm_config_t::[anonymous] flags
Extra flags 



```



### Type Definitions


#### sdm_channel_handle_t


```


typedef struct sdm_channel_t *sdm_channel_handle_t
Type of Sigma Delta channel handle. 

```



### Header File


* components/hal/include/hal/sdm_types.h
* This header file can be included with:



> ```
> #include "hal/sdm_types.h"
> 
> ```




### Type Definitions


#### sdm_clock_source_t


```


typedef soc_periph_sdm_clk_src_t sdm_clock_source_t

```


1
Different ESP chip series might have different numbers of SDM channels. Please refer to Chapter GPIO and IOMUX in ESP32-S3 Technical Reference Manual for more details. The driver does not forbid you from applying for more channels, but it will return an error when all available hardware resources are used up. Please always check the return value when doing resource allocation (e.g., `sdm_new_channel()`).