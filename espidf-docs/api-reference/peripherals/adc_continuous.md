# Analog to Digital Converter (ADC) Continuous Mode Driver




## Introduction


The Analog to Digital Converter is integrated on the chip and is capable of measuring analog signals from specific analog IO pads. Additionally, the Direct Memory Access (DMA) functionality is utilized to efficiently retrieve ADC conversion results.


ESP32-S3 has two ADC unit(s), which can be used in scenarios like:


* Generate one-shot ADC conversion result
* Generate continuous ADC conversion results


This guide introduces ADC continuous mode conversion.



### Driver Concepts


ADC continuous mode conversion is made up of multiple conversion frames.


* Conversion Frame: One conversion frame contains multiple conversion results. Conversion frame size is configured in `adc_continuous_new_handle()` in bytes.
* Conversion Result: One conversion result contains multiple bytes, see `SOC_ADC_DIGI_RESULT_BYTES`. Its structure is `adc_digi_output_data_t`, including ADC unit, ADC channel, and raw data.


![../../_images/adc_conversion_frame.png](../../_images/adc_conversion_frame.png)



## Functional Overview


The following sections of this document cover the typical steps to install the ADC continuous mode driver, and read ADC conversion results from a group of ADC channels continuously:


* Resource Allocation: covers which parameters should be set up to initialize the ADC continuous mode driver and how to deinitialize it.
* ADC Configurations: describes how to configure the ADC(s) to make it work under continuous mode.
* ADC Control: describes ADC control functions.
* Register Event Callbacks: describes how to hook user-specific code to an ADC continuous mode event callback function.
* Read Conversion Result: covers how to get ADC conversion result.
* Hardware Limitations: describes the ADC-related hardware limitations.
* Power Management: covers power management-related information.
* IRAM Safe: covers the IRAM safe functions.
* Thread Safety: lists which APIs are guaranteed to be thread-safe by the driver.



### Resource Allocation


The ADC continuous mode driver is implemented based on ESP32-S3 SAR ADC module. Different ESP targets might have different numbers of independent ADCs.


To create an ADC continuous mode driver handle, set up the required configuration structure `adc_continuous_handle_cfg_t`:


* `adc_continuous_handle_cfg_t::max_store_buf_size`: set the maximum size of the pool in bytes, and the driver saves ADC conversion result into the pool. If this pool is full, new conversion results will be lost.
* `adc_continuous_handle_cfg_t::conv_frame_size`: set the size of the ADC conversion frame, in bytes.
* `adc_continuous_handle_cfg_t::flags`: set the flags that can change the driver's behavior.


	+ `flush_pool`: auto flush the pool when it's full.


After setting up the above configurations for the ADC, call `adc_continuous_new_handle()` with the prepared `adc_continuous_handle_cfg_t`. This function may fail due to various errors such as invalid arguments, insufficient memory, etc.


Especially, when this function returns `ESP_ERR_NOT_FOUND`, this means there is no free GDMA channel.


If the ADC continuous mode driver is no longer used, you should deinitialize the driver by calling `adc_continuous_deinit()`.



#### IIR filter


Two IIR filters are available when ADC is working in continuous mode. To create an ADC IIR filter, you should set up `adc_continuous_iir_filter_config_t` and call `adc_new_continuous_iir_filter()`.


* `adc_digi_filter_config_t::unit`: ADC unit.
* `adc_digi_filter_config_t::channel`: ADC channel to be filtered.
* `adc_digi_filter_config_t::coeff`: Filter coefficient.


To recycle a filter, you should call `adc_del_continuous_iir_filter()`.



Note


If you use both filters on the same ADC channel, then only the first one will take effect.





#### Monitor


2 monitors are available when ADC is working under continuous mode, you can set one or two threshold(s) of a monitor on a working ADC channel, then the monitor will invoke interrupts every sample loop if conversion result outranges of the threshold. To create an ADC monitor, you need to set up the `adc_monitor_config_t` and call `adc_new_continuous_monitor()`.


* `adc_monitor_config_t::adc_unit`: Configures which ADC unit the channel you want to monitor belongs to.
* `adc_monitor_config_t::channel`: The channel you want to monitor.
* `adc_monitor_config_t::h_threshold`: The high threshold, conversion result larger than this value invokes interrupt, set to -1 if do not use.
* `adc_monitor_config_t::l_threshold`: The low threshold, conversion result less than this value invokes interrupt, set to -1 if do not use.


Once a monitor is created, you can operate it by following APIs to construct your apps.


* `adc_continuous_monitor_enable()`: Enable a monitor.
* `adc_continuous_monitor_disable()`: Disable a monitor.
* `adc_monitor_register_callbacks()`: register user callbacks to take action when the ADC value exceeds of the threshold.
* `adc_del_continuous_monitor()`: Delete a created monitor and free resources.




#### Initialize the ADC Continuous Mode Driver



```
adc_continuous_handle_cfg_t adc_config = {
    .max_store_buf_size = 1024,
    .conv_frame_size = 100,
};
ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config));

```




#### Recycle the ADC Unit



```
ESP_ERROR_CHECK(adc_continuous_deinit());

```





### ADC Configurations


After the ADC continuous mode driver is initialized, set up the `adc_continuous_config_t` to configure ADC IOs to measure analog signal:


* `adc_continuous_config_t::pattern_num`: number of ADC channels that will be used.
* `adc_continuous_config_t::adc_pattern`: list of configs for each ADC channel that will be used, see the description below.
* `adc_continuous_config_t::sample_freq_hz`: expected ADC sampling frequency in Hz.
* `adc_continuous_config_t::conv_mode`: continuous conversion mode.
* `adc_continuous_config_t::format`: conversion output format.


Set `adc_digi_pattern_config_t` with the following process:


* `adc_digi_pattern_config_t::atten`: ADC attenuation. Refer to the On-Chip Sensor and Analog Signal Processing chapter in TRM.
* `adc_digi_pattern_config_t::channel`: the IO corresponding ADC channel number. See the note below.
* `adc_digi_pattern_config_t::unit`: the ADC that the IO is subordinate to.
* `adc_digi_pattern_config_t::bit_width`: the bitwidth of the raw conversion result.



Note


For the IO corresponding ADC channel number, check TRM to acquire the ADC IOs. Besides, `adc_continuous_io_to_channel()` and `adc_continuous_channel_to_io()` can be used to acquire the ADC channels and ADC IOs.



To make these settings take effect, call `adc_continuous_config()` with the configuration structure above. This API may fail due to reasons like `ESP_ERR_INVALID_ARG`. When it returns `ESP_ERR_INVALID_STATE`, this means the ADC continuous mode driver is started, you should not call this API at this moment.


See ADC continuous mode example peripherals/adc/continuous_read to see configuration codes.


To enable/disable the ADC IIR filter, you should call `adc_continuous_iir_filter_enable()` / `adc_continuous_iir_filter_disable()`.


To enable/disable the ADC monitor, you should call `adc_continuous_monitor_enable()` / `adc_continuous_monitor_disable()`.




### ADC Control



#### Start and Stop


Calling `adc_continuous_start()` makes the ADC start to measure analog signals from the configured ADC channels, and generate the conversion results.


On the contrary, calling `adc_continuous_stop()` stops the ADC conversion.



```
ESP_ERROR_CHECK(adc_continuous_stop());

```





### Register Event Callbacks


By calling `adc_continuous_register_event_callbacks()`, you can hook your own function to the driver ISR. Supported event callbacks are listed in `adc_continuous_evt_cbs_t`.


* `adc_continuous_evt_cbs_t::on_conv_done`: this is invoked when one conversion frame finishes.
* `adc_continuous_evt_cbs_t::on_pool_ovf`: this is invoked when the internal pool is full. Newer conversion results will be discarded.


As the above callbacks are called in an ISR context, you should always ensure the callback function is suitable for an ISR context. Blocking logic should not appear in these callbacks. The callback function prototype is declared in `adc_continuous_callback_t`.


You can also register your own context when calling `adc_continuous_register_event_callbacks()` by the parameter `user_data`. This user data will be passed to the callback functions directly.


This function may fail due to reasons like `ESP_ERR_INVALID_ARG`. Especially, when CONFIG_ADC_CONTINUOUS_ISR_IRAM_SAFE is enabled, this error may indicate that the callback functions are not in the internal RAM. Check the error log for more details. Besides, when it fails due to `ESP_ERR_INVALID_STATE`, it indicates that the ADC continuous mode driver is started, and you should not add a callback at this moment.



#### Conversion Done Event


When the driver completes a conversion, it triggers the `adc_continuous_evt_cbs_t::on_conv_done` event and fills the event data. Event data contains a buffer pointer to a conversion frame buffer, together with the size. Refer to `adc_continuous_evt_data_t` to know the event data structure.



Note


It is worth noting that, the data buffer `adc_continuous_evt_data_t::conv_frame_buffer` is maintained by the driver itself. Therefore, never free this piece of memory.




Note


When the Kconfig option CONFIG_ADC_CONTINUOUS_ISR_IRAM_SAFE is enabled, the registered callbacks and the functions called by the callbacks should be placed in IRAM. The involved variables should be placed in internal RAM as well.





#### Pool Overflow Event


The ADC continuous mode driver has an internal pool to save the conversion results. When the pool is full, a pool overflow event will emerge. Under this condition, the driver will not fill in the event data. This usually happens because the speed to read data from the pool by calling `adc_continuous_read()` is much slower than the ADC conversion speed.





### Read Conversion Result


After calling `adc_continuous_start()`, the ADC continuous conversion starts. Call `adc_continuous_read()` to get the conversion results of the ADC channels. You need to provide a buffer to get the raw results.


Function `adc_continuous_read()` tries to read the expected length of conversion results each time.


* When calling `adc_continuous_read()`, you can request to read a conversion result of the specified length. Sometimes, however, the actual available conversion results may be less than the requested length, in which case the function still moves the data from the internal pool into the buffer you provided. Therefore, to learn the number of conversion results actually moved into the buffer, please check the value of `out_length`.
* If there is no conversion result generated in the internal pool, the function will block for `timeout_ms` until the conversion results are generated. If there are still no generated results, the function will return `ESP_ERR_TIMEOUT`.
* If the generated results fill up the internal pool, newly generated results will be lost. Next time when `adc_continuous_read()` is called, this function will return `ESP_ERR_INVALID_STATE` to indicate this situation.


This API aims to give you a chance to read all the ADC continuous conversion results.


The ADC conversion results read from the above function are raw data. To calculate the voltage based on the ADC raw results, this formula can be used:



```
Vout = Dout * Vmax / Dmax       (1)

```


where:







| Vout | Digital output result, standing for the voltage. |
| --- | --- |
| Dout | ADC raw digital reading result. |
| Vmax | Maximum measurable input analog voltage, this is related to the ADC attenuation, please refer to the On-Chip Sensor and Analog Signal Processing chapter in TRM. |
| Dmax | Maximum of the output ADC raw digital reading result, which is 2^bitwidth, where the bitwidth is the `adc_digi_pattern_config_t::bit_width` configured before. |


To do further calibration to convert the ADC raw result to voltage in mV, please refer to Analog to Digital Converter (ADC) Calibration Driver.




### Hardware Limitations


* A specific ADC unit can only work under one operating mode at any one time, either continuous mode or one-shot mode. `adc_continuous_start()` has provided the protection.
* Random Number Generator (RNG) uses ADC as an input source. When ADC continuous mode driver works, the random number generated from RNG will be less random.


* ADC2 DMA functionality is no longer supported to retrieve ADC conversion results due to hardware limitations, as unstable results have been observed. This issue can be found in ESP32S3 Errata. For compatibility, you can enable CONFIG_ADC_CONTINUOUS_FORCE_USE_ADC2_ON_C3_S3 to force use ADC2.




### Power Management


When power management is enabled, i.e., CONFIG_PM_ENABLE is on, the APB clock frequency may be adjusted when the system is in an idle state, thus potentially changing the behavior of ADC continuous conversion.


However, the continuous mode driver can prevent this change by acquiring a power management lock of type `ESP_PM_APB_FREQ_MAX`. The lock is acquired after the continuous conversion is started by `adc_continuous_start()`. Similarly, the lock will be released after `adc_continuous_stop()`. Therefore, `adc_continuous_start()` and `adc_continuous_stop()` should appear in pairs, otherwise, the power management will be out of action.




### IRAM Safe


All the ADC continuous mode driver APIs are not IRAM-safe. They are not supposed to be run when the Cache is disabled. By enabling the Kconfig option CONFIG_ADC_CONTINUOUS_ISR_IRAM_SAFE, the driver's internal ISR handler is IRAM-safe, which means even when the Cache is disabled, the driver will still save the conversion results into its internal pool.




### Thread Safety


ADC continuous mode driver APIs are not guaranteed to be thread-safe. However, the share hardware mutual exclusion is provided by the driver. See Hardware Limitations for more details.





## Application Examples


* ADC continuous mode example: peripherals/adc/continuous_read.




## API Reference



### Header File


* components/esp_adc/include/esp_adc/adc_continuous.h
* This header file can be included with:



> ```
> #include "esp_adc/adc_continuous.h"
> 
> ```
* This header file is a part of the API provided by the `esp_adc` component. To declare that your component depends on `esp_adc`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_adc
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_adc
> 
> ```




### Functions


#### adc_continuous_new_handle


```


esp_err_t adc_continuous_new_handle(const adc_continuous_handle_cfg_t *hdl_config, adc_continuous_handle_t *ret_handle)
Initialize ADC continuous driver and get a handle to it. 

Parameters

hdl_config -- [in] Pointer to ADC initialization config. Refer to adc_continuous_handle_cfg_t. 
ret_handle -- [out] ADC continuous mode driver handle


Returns

ESP_ERR_INVALID_ARG If the combination of arguments is invalid.
ESP_ERR_NOT_FOUND No free interrupt found with the specified flags
ESP_ERR_NO_MEM If out of memory
ESP_OK On success 





```

#### adc_continuous_config


```


esp_err_t adc_continuous_config(adc_continuous_handle_t handle, const adc_continuous_config_t *config)
Set ADC continuous mode required configurations. 

Parameters

handle -- [in] ADC continuous mode driver handle 
config -- [in] Refer to adc_digi_config_t.


Returns

ESP_ERR_INVALID_STATE: Driver state is invalid, you shouldn't call this API at this moment
ESP_ERR_INVALID_ARG: If the combination of arguments is invalid.
ESP_OK: On success 





```

#### adc_continuous_register_event_callbacks


```


esp_err_t adc_continuous_register_event_callbacks(adc_continuous_handle_t handle, const adc_continuous_evt_cbs_t *cbs, void *user_data)
Register callbacks. 

Note
User can deregister a previously registered callback by calling this function and setting the to-be-deregistered callback member in the cbs structure to NULL. 


Note
When CONFIG_ADC_CONTINUOUS_ISR_IRAM_SAFE is enabled, the callback itself and functions called by it should be placed in IRAM. Involved variables (including user_data) should be in internal RAM as well. 


Note
You should only call this API when the ADC continuous mode driver isn't started. Check return value to know this.


Parameters

handle -- [in] ADC continuous mode driver handle 
cbs -- [in] Group of callback functions 
user_data -- [in] User data, which will be delivered to the callback functions directly


Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid arguments
ESP_ERR_INVALID_STATE: Driver state is invalid, you shouldn't call this API at this moment 





```

#### adc_continuous_start


```


esp_err_t adc_continuous_start(adc_continuous_handle_t handle)
Start the ADC under continuous mode. After this, the hardware starts working. 

Parameters
handle -- [in] ADC continuous mode driver handle

Returns

ESP_ERR_INVALID_STATE Driver state is invalid.
ESP_OK On success 





```

#### adc_continuous_read


```


esp_err_t adc_continuous_read(adc_continuous_handle_t handle, uint8_t *buf, uint32_t length_max, uint32_t *out_length, uint32_t timeout_ms)
Read bytes from ADC under continuous mode. 

Parameters

handle -- [in] ADC continuous mode driver handle 
buf -- [out] Conversion result buffer to read from ADC. Suggest convert to adc_digi_output_data_t for ADC Conversion Results. See the subsection Driver Backgrounds in this header file to learn about this concept. 
length_max -- [in] Expected length of the Conversion Results read from the ADC, in bytes. 
out_length -- [out] Real length of the Conversion Results read from the ADC via this API, in bytes. 
timeout_ms -- [in] Time to wait for data via this API, in millisecond.


Returns

ESP_ERR_INVALID_STATE Driver state is invalid. Usually it means the ADC sampling rate is faster than the task processing rate.
ESP_ERR_TIMEOUT Operation timed out
ESP_OK On success 





```

#### adc_continuous_stop


```


esp_err_t adc_continuous_stop(adc_continuous_handle_t handle)
Stop the ADC. After this, the hardware stops working. 

Parameters
handle -- [in] ADC continuous mode driver handle

Returns

ESP_ERR_INVALID_STATE Driver state is invalid.
ESP_OK On success 





```

#### adc_continuous_deinit


```


esp_err_t adc_continuous_deinit(adc_continuous_handle_t handle)
Deinitialize the ADC continuous driver. 

Parameters
handle -- [in] ADC continuous mode driver handle

Returns

ESP_ERR_INVALID_STATE Driver state is invalid.
ESP_OK On success 





```

#### adc_continuous_flush_pool


```


esp_err_t adc_continuous_flush_pool(adc_continuous_handle_t handle)
Flush the driver internal pool. 

Note
This API is not supposed to be called in an ISR context


Parameters
handle -- [in] ADC continuous mode driver handle

Returns

ESP_ERR_INVALID_STATE Driver state is invalid, you should call this API when it's in init state
ESP_ERR_INVALID_ARG: Invalid arguments
ESP_OK On success 





```

#### adc_continuous_io_to_channel


```


esp_err_t adc_continuous_io_to_channel(int io_num, adc_unit_t *const unit_id, adc_channel_t *const channel)
Get ADC channel from the given GPIO number. 

Parameters

io_num -- [in] GPIO number 
unit_id -- [out] ADC unit 
channel -- [out] ADC channel


Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid argument
ESP_ERR_NOT_FOUND: The IO is not a valid ADC pad 





```

#### adc_continuous_channel_to_io


```


esp_err_t adc_continuous_channel_to_io(adc_unit_t unit_id, adc_channel_t channel, int *const io_num)
Get GPIO number from the given ADC channel. 

Parameters

unit_id -- [in] ADC unit 
channel -- [in] ADC channel 
io_num -- [out] GPIO number
- -- ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid argument 






```



### Structures


#### adc_continuous_handle_cfg_t


```


struct adc_continuous_handle_cfg_t
ADC continuous mode driver initial configurations. 

Public Members
max_store_buf_size

uint32_t max_store_buf_size
Max length of the conversion results that driver can store, in bytes. 

conv_frame_size

uint32_t conv_frame_size
Conversion frame size, in bytes. This should be in multiples of SOC_ADC_DIGI_DATA_BYTES_PER_CONV. 

flush_pool

uint32_t flush_pool
Flush the internal pool when the pool is full. 

flags

struct adc_continuous_handle_cfg_t::[anonymous] flags
Driver flags. 



```

#### adc_continuous_config_t


```


struct adc_continuous_config_t
ADC continuous mode driver configurations. 

Public Members
pattern_num

uint32_t pattern_num
Number of ADC channels that will be used. 

adc_pattern

adc_digi_pattern_config_t *adc_pattern
List of configs for each ADC channel that will be used. 

sample_freq_hz

uint32_t sample_freq_hz
The expected ADC sampling frequency in Hz. Please refer to soc/soc_caps.h to know available sampling frequency range 

conv_mode

adc_digi_convert_mode_t conv_mode
ADC DMA conversion mode, see adc_digi_convert_mode_t. 

format

adc_digi_output_format_t format
ADC DMA conversion output format, see adc_digi_output_format_t. 



```

#### adc_continuous_evt_data_t


```


struct adc_continuous_evt_data_t
Event data structure. 

Note
The conv_frame_buffer is maintained by the driver itself, so never free this piece of memory. 


Public Members
conv_frame_buffer

uint8_t *conv_frame_buffer
Pointer to conversion result buffer for one conversion frame. 

size

uint32_t size
Conversion frame size. 



```

#### adc_continuous_evt_cbs_t


```


struct adc_continuous_evt_cbs_t
Group of ADC continuous mode callbacks. 

Note
These callbacks are all running in an ISR environment. 


Note
When CONFIG_ADC_CONTINUOUS_ISR_IRAM_SAFE is enabled, the callback itself and functions called by it should be placed in IRAM. Involved variables should be in internal RAM as well. 


Public Members
on_conv_done

adc_continuous_callback_t on_conv_done
Event callback, invoked when one conversion frame is done. See the subsection Driver Backgrounds in this header file to learn about the conversion frame concept. 

on_pool_ovf

adc_continuous_callback_t on_pool_ovf
Event callback, invoked when the internal pool is full. 



```



### Macros


#### ADC_MAX_DELAY


```


ADC_MAX_DELAY
ADC read max timeout value, it may make the adc_continuous_read block forever if the OS supports. 

```



### Type Definitions


#### adc_continuous_handle_t


```


typedef struct adc_continuous_ctx_t *adc_continuous_handle_t
Type of adc continuous mode driver handle. 

```

#### adc_continuous_callback_t


```


typedef bool (*adc_continuous_callback_t)(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
Prototype of ADC continuous mode event callback. 

Param handle
[in] ADC continuous mode driver handle 

Param edata
[in] Pointer to ADC continuous mode event data 

Param user_data
[in] User registered context, registered when in adc_continuous_register_event_callbacks()

Return
Whether a high priority task is woken up by this function 



```