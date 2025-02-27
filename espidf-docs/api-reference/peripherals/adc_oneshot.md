# Analog to Digital Converter (ADC) Oneshot Mode Driver




## Introduction


The Analog to Digital Converter is integrated on the chip and is capable of measuring analog signals from specific analog IO pins.


ESP32-S3 has two ADC unit(s), which can be used in scenario(s) like:


* Generate one-shot ADC conversion result


* Generate continuous ADC conversion results


This guide introduces ADC oneshot mode conversion.




## Functional Overview


The following sections of this document cover the typical steps to install and operate an ADC:


* Resource Allocation - covers which parameters should be set up to get an ADC handle and how to recycle the resources when ADC finishes working.
* Unit Configuration - covers the parameters that should be set up to configure the ADC unit, so as to get ADC conversion raw result.
* Read Conversion Result - covers how to get ADC conversion raw result.
* Hardware Limitations - describes the ADC-related hardware limitations.
* Power Management - covers power management-related information.
* IRAM Safe - describes tips on how to read ADC conversion raw results when the cache is disabled.
* Thread Safety - lists which APIs are guaranteed to be thread-safe by the driver.
* Kconfig Options - lists the supported Kconfig options that can be used to make a different effect on driver behavior.



### Resource Allocation


The ADC oneshot mode driver is implemented based on ESP32-S3 SAR ADC module. Different ESP chips might have different numbers of independent ADCs. From the oneshot mode driver's point of view, an ADC instance is represented by `adc_oneshot_unit_handle_t`.


To install an ADC instance, set up the required initial configuration structure `adc_oneshot_unit_init_cfg_t`:


* `adc_oneshot_unit_init_cfg_t::unit_id` selects the ADC. Please refer to the datasheet to know dedicated analog IOs for this ADC.
* `adc_oneshot_unit_init_cfg_t::clk_src` selects the source clock of the ADC. If set to 0, the driver will fall back to using a default clock source, see `adc_oneshot_clk_src_t` to know the details.
* `adc_oneshot_unit_init_cfg_t::ulp_mode` sets if the ADC will be working under ULP mode.


After setting up the initial configurations for the ADC, call `adc_oneshot_new_unit()` with the prepared `adc_oneshot_unit_init_cfg_t`. This function will return an ADC unit handle if the allocation is successful.


This function may fail due to various errors such as invalid arguments, insufficient memory, etc. Specifically, when the to-be-allocated ADC instance is registered already, this function will return `ESP_ERR_NOT_FOUND` error. Number of available ADC(s) is recorded by `SOC_ADC_PERIPH_NUM`.


If a previously created ADC instance is no longer required, you should recycle the ADC instance by calling `adc_oneshot_del_unit()`, related hardware and software resources will be recycled as well.



#### Create an ADC Unit Handle Under Normal Oneshot Mode



```
adc_oneshot_unit_handle_t adc1_handle;
adc_oneshot_unit_init_cfg_t init_config1 = {
    .unit_id = ADC_UNIT_1,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};
ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

```




#### Recycle the ADC Unit



```
ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));

```





### Unit Configuration


After an ADC instance is created, set up the `adc_oneshot_chan_cfg_t` to configure ADC IOs to measure analog signal:


* `adc_oneshot_chan_cfg_t::atten`, ADC attenuation. Refer to TRM > `On-Chip Sensor and Analog Signal Processing`.
* `adc_oneshot_chan_cfg_t::bitwidth`, the bitwidth of the raw conversion result.



Note


For the IO corresponding ADC channel number, check datasheet to know the ADC IOs.


Additionally, `adc_continuous_io_to_channel()` and `adc_continuous_channel_to_io()` can be used to know the ADC channels and ADC IOs.



To make these settings take effect, call `adc_oneshot_config_channel()` with the above configuration structure. You should specify an ADC channel to be configured as well. Function `adc_oneshot_config_channel()` can be called multiple times to configure different ADC channels. The Driver will save each of these channel configurations internally.



#### Configure Two ADC Channels



```
adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_12,
};
ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config));
ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN1, &config));

```





### Read Conversion Result


After above configurations, the ADC is ready to measure the analog signal(s) from the configured ADC channel(s). Call `adc_oneshot_read()` to get the conversion raw result of an ADC channel.


* `adc_oneshot_read()` is safe to use. ADC(s) are shared by some other drivers/peripherals, see Hardware Limitations. This function uses mutexes to avoid concurrent hardware usage. Therefore, this function should not be used in an ISR context. This function may fail when the ADC is in use by other drivers/peripherals, and return `ESP_ERR_TIMEOUT`. Under this condition, the ADC raw result is invalid.


This function will fail due to invalid arguments.


The ADC conversion results read from this function are raw data. To calculate the voltage based on the ADC raw results, this formula can be used:



```
Vout = Dout * Vmax / Dmax       (1)

```


where:







| Vout | Digital output result, standing for the voltage. |
| --- | --- |
| Dout | ADC raw digital reading result. |
| Vmax | Maximum measurable input analog voltage, this is related to the ADC attenuation, please refer to TRM > `On-Chip Sensor and Analog Signal Processing`. |
| Dmax | Maximum of the output ADC raw digital reading result, which is 2^bitwidth, where bitwidth is the :cpp:member::adc_oneshot_chan_cfg_t:bitwidth configured before. |


To do further calibration to convert the ADC raw result to voltage in mV, please refer to calibration doc Analog to Digital Converter (ADC) Calibration Driver.



#### Read Raw Result



```
ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[0][0]));
ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, adc_raw[0][0]);

ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN1, &adc_raw[0][1]));
ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN1, adc_raw[0][1]);

```





### Hardware Limitations


* Random Number Generator (RNG) uses ADC as an input source. When ADC `adc_oneshot_read()` works, the random number generated from RNG will be less random.


* A specific ADC unit can only work under one operating mode at any one time, either continuous mode or oneshot mode. `adc_oneshot_read()` has provided the protection.


* ADC2 is also used by Wi-Fi. `adc_oneshot_read()` has provided protection between the Wi-Fi driver and ADC oneshot mode driver.




### Power Management


When power management is enabled, i.e., CONFIG_PM_ENABLE is on, the system clock frequency may be adjusted when the system is in an idle state. However, the ADC oneshot mode driver works in a polling routine, the `adc_oneshot_read()` will poll the CPU until the function returns. During this period of time, the task in which ADC oneshot mode driver resides will not be blocked. Therefore the clock frequency is stable when reading.




### IRAM Safe


By default, all the ADC oneshot mode driver APIs are not supposed to be run when the Cache is disabled. Cache may be disabled due to many reasons, such as Flash writing/erasing, OTA, etc. If these APIs execute when the Cache is disabled, you will probably see errors like `Illegal Instruction` or `Load/Store Prohibited`.




### Thread Safety


* `adc_oneshot_new_unit()`
* `adc_oneshot_config_channel()`
* `adc_oneshot_read()`


Above functions are guaranteed to be thread-safe. Therefore, you can call them from different RTOS tasks without protection by extra locks.


* `adc_oneshot_del_unit()` is not thread-safe. Besides, concurrently calling this function may result in failures of the above thread-safe APIs.




### Kconfig Options


* CONFIG_ADC_ONESHOT_CTRL_FUNC_IN_IRAM controls where to place the ADC fast read function (IRAM or Flash), see IRAM Safe for more details.





## Application Examples


* ADC oneshot mode example: peripherals/adc/oneshot_read.




## API Reference



### Header File


* components/hal/include/hal/adc_types.h
* This header file can be included with:



> ```
> #include "hal/adc_types.h"
> 
> ```




### Structures


#### adc_digi_pattern_config_t


```


struct adc_digi_pattern_config_t
ADC digital controller pattern configuration. 

Public Members
atten

uint8_t atten
Attenuation of this ADC channel. 

channel

uint8_t channel
ADC channel. 

unit

uint8_t unit
ADC unit. 

bit_width

uint8_t bit_width
ADC output bit width. 



```

#### adc_digi_output_data_t


```


struct adc_digi_output_data_t
ADC digital controller (DMA mode) output data format. Used to analyze the acquired ADC (DMA) data. 

Public Members
data

uint32_t data
ADC real output data info. Resolution: 12 bit. 

reserved12

uint32_t reserved12
Reserved12. 

channel

uint32_t channel
ADC channel index info. If (channel < ADC_CHANNEL_MAX), The data is valid. If (channel > ADC_CHANNEL_MAX), The data is invalid. 

unit

uint32_t unit
ADC unit index info. 0: ADC1; 1: ADC2. 

reserved17_31

uint32_t reserved17_31
Reserved17. 

type2

struct adc_digi_output_data_t::[anonymous]::[anonymous] type2
When the configured output format is 12bit. 

val

uint32_t val
Raw data value 



```



### Type Definitions


#### adc_oneshot_clk_src_t


```


typedef soc_periph_adc_rtc_clk_src_t adc_oneshot_clk_src_t
Clock source type of oneshot mode which uses RTC controller. 

```

#### adc_continuous_clk_src_t


```


typedef soc_periph_adc_digi_clk_src_t adc_continuous_clk_src_t
Clock source type of continuous mode which uses digital controller. 

```



### Enumerations


#### adc_unit_t


```


enum adc_unit_t
ADC unit. 
Values:
ADC_UNIT_1

enumerator ADC_UNIT_1
SAR ADC 1. 

ADC_UNIT_2

enumerator ADC_UNIT_2
SAR ADC 2. 


```

#### adc_channel_t


```


enum adc_channel_t
ADC channels. 
Values:
ADC_CHANNEL_0

enumerator ADC_CHANNEL_0
ADC channel. 

ADC_CHANNEL_1

enumerator ADC_CHANNEL_1
ADC channel. 

ADC_CHANNEL_2

enumerator ADC_CHANNEL_2
ADC channel. 

ADC_CHANNEL_3

enumerator ADC_CHANNEL_3
ADC channel. 

ADC_CHANNEL_4

enumerator ADC_CHANNEL_4
ADC channel. 

ADC_CHANNEL_5

enumerator ADC_CHANNEL_5
ADC channel. 

ADC_CHANNEL_6

enumerator ADC_CHANNEL_6
ADC channel. 

ADC_CHANNEL_7

enumerator ADC_CHANNEL_7
ADC channel. 

ADC_CHANNEL_8

enumerator ADC_CHANNEL_8
ADC channel. 

ADC_CHANNEL_9

enumerator ADC_CHANNEL_9
ADC channel. 


```

#### adc_atten_t


```


enum adc_atten_t
ADC attenuation parameter. Different parameters determine the range of the ADC. 
Values:
ADC_ATTEN_DB_0

enumerator ADC_ATTEN_DB_0
No input attenuation, ADC can measure up to approx. 

ADC_ATTEN_DB_2_5

enumerator ADC_ATTEN_DB_2_5
The input voltage of ADC will be attenuated extending the range of measurement by about 2.5 dB. 

ADC_ATTEN_DB_6

enumerator ADC_ATTEN_DB_6
The input voltage of ADC will be attenuated extending the range of measurement by about 6 dB. 

ADC_ATTEN_DB_12

enumerator ADC_ATTEN_DB_12
The input voltage of ADC will be attenuated extending the range of measurement by about 12 dB. 

ADC_ATTEN_DB_11

enumerator ADC_ATTEN_DB_11
This is deprecated, it behaves the same as ADC_ATTEN_DB_12


```

#### adc_bitwidth_t


```


enum adc_bitwidth_t
Values:
ADC_BITWIDTH_DEFAULT

enumerator ADC_BITWIDTH_DEFAULT
Default ADC output bits, max supported width will be selected. 

ADC_BITWIDTH_9

enumerator ADC_BITWIDTH_9
ADC output width is 9Bit. 

ADC_BITWIDTH_10

enumerator ADC_BITWIDTH_10
ADC output width is 10Bit. 

ADC_BITWIDTH_11

enumerator ADC_BITWIDTH_11
ADC output width is 11Bit. 

ADC_BITWIDTH_12

enumerator ADC_BITWIDTH_12
ADC output width is 12Bit. 

ADC_BITWIDTH_13

enumerator ADC_BITWIDTH_13
ADC output width is 13Bit. 


```

#### adc_ulp_mode_t


```


enum adc_ulp_mode_t
Values:
ADC_ULP_MODE_DISABLE

enumerator ADC_ULP_MODE_DISABLE
ADC ULP mode is disabled. 

ADC_ULP_MODE_FSM

enumerator ADC_ULP_MODE_FSM
ADC is controlled by ULP FSM. 

ADC_ULP_MODE_RISCV

enumerator ADC_ULP_MODE_RISCV
ADC is controlled by ULP RISCV. 


```

#### adc_digi_convert_mode_t


```


enum adc_digi_convert_mode_t
ADC digital controller (DMA mode) work mode. 
Values:
ADC_CONV_SINGLE_UNIT_1

enumerator ADC_CONV_SINGLE_UNIT_1
Only use ADC1 for conversion. 

ADC_CONV_SINGLE_UNIT_2

enumerator ADC_CONV_SINGLE_UNIT_2
Only use ADC2 for conversion. 

ADC_CONV_BOTH_UNIT

enumerator ADC_CONV_BOTH_UNIT
Use Both ADC1 and ADC2 for conversion simultaneously. 

ADC_CONV_ALTER_UNIT

enumerator ADC_CONV_ALTER_UNIT
Use both ADC1 and ADC2 for conversion by turn. e.g. ADC1 -> ADC2 -> ADC1 -> ADC2 ..... 


```

#### adc_digi_output_format_t


```


enum adc_digi_output_format_t
ADC digital controller (DMA mode) output data format option. 
Values:
ADC_DIGI_OUTPUT_FORMAT_TYPE1

enumerator ADC_DIGI_OUTPUT_FORMAT_TYPE1
See adc_digi_output_data_t.type1

ADC_DIGI_OUTPUT_FORMAT_TYPE2

enumerator ADC_DIGI_OUTPUT_FORMAT_TYPE2
See adc_digi_output_data_t.type2


```

#### adc_digi_iir_filter_t


```


enum adc_digi_iir_filter_t
ADC IIR Filter ID. 
Values:
ADC_DIGI_IIR_FILTER_0

enumerator ADC_DIGI_IIR_FILTER_0
Filter 0. 

ADC_DIGI_IIR_FILTER_1

enumerator ADC_DIGI_IIR_FILTER_1
Filter 1. 


```

#### adc_digi_iir_filter_coeff_t


```


enum adc_digi_iir_filter_coeff_t
IIR Filter Coefficient. 
Values:
ADC_DIGI_IIR_FILTER_COEFF_2

enumerator ADC_DIGI_IIR_FILTER_COEFF_2
The filter coefficient is 2. 

ADC_DIGI_IIR_FILTER_COEFF_4

enumerator ADC_DIGI_IIR_FILTER_COEFF_4
The filter coefficient is 4. 

ADC_DIGI_IIR_FILTER_COEFF_8

enumerator ADC_DIGI_IIR_FILTER_COEFF_8
The filter coefficient is 8. 

ADC_DIGI_IIR_FILTER_COEFF_16

enumerator ADC_DIGI_IIR_FILTER_COEFF_16
The filter coefficient is 16. 

ADC_DIGI_IIR_FILTER_COEFF_64

enumerator ADC_DIGI_IIR_FILTER_COEFF_64
The filter coefficient is 64. 


```

#### adc_monitor_id_t


```


enum adc_monitor_id_t
ADC monitor (continuous mode) ID. 
Values:
ADC_MONITOR_0

enumerator ADC_MONITOR_0
The monitor index 0. 

ADC_MONITOR_1

enumerator ADC_MONITOR_1
The monitor index 1. 


```

#### adc_monitor_mode_t


```


enum adc_monitor_mode_t
Monitor config/event mode type. 
Values:
ADC_MONITOR_MODE_HIGH

enumerator ADC_MONITOR_MODE_HIGH
ADC raw_result > threshold value, monitor interrupt will be generated. 

ADC_MONITOR_MODE_LOW

enumerator ADC_MONITOR_MODE_LOW
ADC raw_result < threshold value, monitor interrupt will be generated. 


```



### Header File


* components/esp_adc/include/esp_adc/adc_oneshot.h
* This header file can be included with:



> ```
> #include "esp_adc/adc_oneshot.h"
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


#### adc_oneshot_new_unit


```


esp_err_t adc_oneshot_new_unit(const adc_oneshot_unit_init_cfg_t *init_config, adc_oneshot_unit_handle_t *ret_unit)
Create a handle to a specific ADC unit. 

Note
This API is thread-safe. For more details, see ADC programming guide


Parameters

init_config -- [in] Driver initial configurations 
ret_unit -- [out] ADC unit handle


Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid arguments
ESP_ERR_NO_MEM: No memory
ESP_ERR_NOT_FOUND: The ADC peripheral to be claimed is already in use
ESP_FAIL: Clock source isn't initialised correctly 





```

#### adc_oneshot_config_channel


```


esp_err_t adc_oneshot_config_channel(adc_oneshot_unit_handle_t handle, adc_channel_t channel, const adc_oneshot_chan_cfg_t *config)
Set ADC oneshot mode required configurations. 

Note
This API is thread-safe. For more details, see ADC programming guide


Parameters

handle -- [in] ADC handle 
channel -- [in] ADC channel to be configured 
config -- [in] ADC configurations


Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid arguments 





```

#### adc_oneshot_read


```


esp_err_t adc_oneshot_read(adc_oneshot_unit_handle_t handle, adc_channel_t chan, int *out_raw)
Get one ADC conversion raw result. 

Note
This API is thread-safe. For more details, see ADC programming guide 


Note
This API should NOT be called in an ISR context


Parameters

handle -- [in] ADC handle 
chan -- [in] ADC channel 
out_raw -- [out] ADC conversion raw result


Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid arguments
ESP_ERR_TIMEOUT: Timeout, the ADC result is invalid 





```

#### adc_oneshot_del_unit


```


esp_err_t adc_oneshot_del_unit(adc_oneshot_unit_handle_t handle)
Delete the ADC unit handle. 

Note
This API is thread-safe. For more details, see ADC programming guide


Parameters
handle -- [in] ADC handle

Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid arguments
ESP_ERR_NOT_FOUND: The ADC peripheral to be disclaimed isn't in use 





```

#### adc_oneshot_io_to_channel


```


esp_err_t adc_oneshot_io_to_channel(int io_num, adc_unit_t *const unit_id, adc_channel_t *const channel)
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

#### adc_oneshot_channel_to_io


```


esp_err_t adc_oneshot_channel_to_io(adc_unit_t unit_id, adc_channel_t channel, int *const io_num)
Get GPIO number from the given ADC channel. 

Parameters

unit_id -- [in] ADC unit 
channel -- [in] ADC channel 
io_num -- [out] GPIO number
- -- ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid argument 






```

#### adc_oneshot_get_calibrated_result


```


esp_err_t adc_oneshot_get_calibrated_result(adc_oneshot_unit_handle_t handle, adc_cali_handle_t cali_handle, adc_channel_t chan, int *cali_result)
Convenience function to get ADC calibrated result. 
This is an all-in-one function which does:
oneshot read ADC raw result
calibrate the raw result and convert it into calibrated result (in mV)



Parameters

handle -- [in] ADC oneshot handle, you should call adc_oneshot_new_unit() to get this handle 
cali_handle -- [in] ADC calibration handle, you should call adc_cali_create_scheme_x() in adc_cali_scheme.h to create a handle 
chan -- [in] ADC channel 
cali_result -- [out] Calibrated ADC result (in mV)


Returns

ESP_OK Other return errors from adc_oneshot_read() and adc_cali_raw_to_voltage() 





```



### Structures


#### adc_oneshot_unit_init_cfg_t


```


struct adc_oneshot_unit_init_cfg_t
ADC oneshot driver initial configurations. 

Public Members
unit_id

adc_unit_t unit_id
ADC unit. 

clk_src

adc_oneshot_clk_src_t clk_src
Clock source. 

ulp_mode

adc_ulp_mode_t ulp_mode
ADC controlled by ULP, see adc_ulp_mode_t



```

#### adc_oneshot_chan_cfg_t


```


struct adc_oneshot_chan_cfg_t
ADC channel configurations. 

Public Members
atten

adc_atten_t atten
ADC attenuation. 

bitwidth

adc_bitwidth_t bitwidth
ADC conversion result bits. 



```



### Type Definitions


#### adc_oneshot_unit_handle_t


```


typedef struct adc_oneshot_unit_ctx_t *adc_oneshot_unit_handle_t
Type of ADC unit handle for oneshot mode. 

```