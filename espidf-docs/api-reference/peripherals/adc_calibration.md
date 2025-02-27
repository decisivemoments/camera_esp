# Analog to Digital Converter (ADC) Calibration Driver




## Introduction


In ESP32-S3, the digital-to-analog converter (ADC) compares the input analog voltage to the reference, and determines each bit of the output digital result. By design, the ADC reference voltage for ESP32-S3 is 1100 mV. However, the true reference voltage can range from 1000 mV to 1200 mV among different chips. This guide introduces the ADC calibration driver to minimize the effect of different reference voltages, and get more accurate output results.




## Functional Overview


The following sections of this document cover the typical steps to install and use the ADC calibration driver:


* Calibration Scheme Creation - covers how to create a calibration scheme handle and delete the calibration scheme handle.
* Result Conversion - covers how to convert ADC raw result to calibrated result.
* Thread Safety - lists which APIs are guaranteed to be thread-safe by the driver.
* Minimize Noise - describes a general way to minimize the noise.






### Calibration Scheme Creation


The ADC calibration driver provides ADC calibration scheme(s). From the calibration driver's point of view, an ADC calibration scheme is created for an ADC calibration handle `adc_cali_handle_t`.


`adc_cali_check_scheme()` can be used to know which calibration scheme is supported on the chip. If you already know the supported schemes, this step can be skipped. Just call the corresponding function to create the scheme handle.


If you use your custom ADC calibration schemes, you could either modify this function `adc_cali_check_scheme()`, or just skip this step and call your custom creation function.



#### ADC Calibration Curve Fitting Scheme


ESP32-S3 supports `ADC_CALI_SCHEME_VER_CURVE_FITTING` scheme. To create this scheme, set up `adc_cali_curve_fitting_config_t` first.


* `adc_cali_curve_fitting_config_t::unit_id`, the ADC that your ADC raw results are from.
* `adc_cali_curve_fitting_config_t::chan`, this member is kept here for extensibility. The calibration scheme only differs by attenuation, there is no difference among different channels.
* `adc_cali_curve_fitting_config_t::atten`, ADC attenuation that your ADC raw results use.
* `adc_cali_curve_fitting_config_t::bitwidth`, bit width of ADC raw result.


After setting up the configuration structure, call `adc_cali_create_scheme_curve_fitting()` to create a Curve Fitting calibration scheme handle. This function may fail due to reasons such as `ESP_ERR_INVALID_ARG` or `ESP_ERR_NO_MEM`.



##### ADC Calibration eFuse Related Failures


When the function `adc_cali_create_scheme_curve_fitting()` returns `ESP_ERR_NOT_SUPPORTED`, this means the calibration scheme required eFuse bits are not correct on your board.


The ADC calibration scheme provided by ESP-IDF is based on the values in certain ADC calibration related on-chip eFuse bits. Espressif guarantees that these bits are burned during module manufacturing, so you don't have to burn these eFuses bits yourself.


If you see such an error, please contact us at Technical Inquiries website.




##### Create Curve Fitting Scheme



```
ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
adc_cali_curve_fitting_config_t cali_config = {
    .unit_id = unit,
    .atten = atten,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
};
ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, &handle));

```


When the ADC calibration is no longer used, please delete the calibration scheme driver from the calibration handle by calling `adc_cali_delete_scheme_curve_fitting()`.




##### Delete Curve Fitting Scheme



```
ESP_LOGI(TAG, "delete %s calibration scheme", "Curve Fitting");
ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(handle));

```





Note


If you want to use your custom calibration schemes, you could provide a creation function to create your calibration scheme handle. Check the function table `adc_cali_scheme_t` in `components/esp_adc/interface/adc_cali_interface.h` to know the ESP ADC calibration interface.





### Result Conversion


After setting up the calibration characteristics, you can call `adc_cali_raw_to_voltage()` to convert the ADC raw result into calibrated result. The calibrated result is in the unit of mV. This function may fail due to an invalid argument. Especially, if this function returns `ESP_ERR_INVALID_STATE`, this means the calibration scheme is not created. You need to create a calibration scheme handle, use `adc_cali_check_scheme()` to know the supported calibration scheme. On the other hand, you could also provide a custom calibration scheme and create the handle.



#### Get Voltage



```
ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, adc_raw[0][0], &voltage[0][0]));
ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, voltage[0][0]);

```





### Thread Safety


The factory function `esp_adc_cali_new_scheme()` is guaranteed to be thread-safe by the driver. Therefore, you can call them from different RTOS tasks without protection by extra locks.


Other functions that take the `adc_cali_handle_t` as the first positional parameter are not thread-safe, you should avoid calling them from multiple tasks.




### Minimize Noise


The ESP32-S3 ADC is sensitive to noise, leading to large discrepancies in ADC readings. Depending on the usage scenario, you may need to connect a bypass capacitor (e.g., a 100 nF ceramic capacitor) to the ADC input pad in use, to minimize noise. Besides, multisampling may also be used to further mitigate the effects of noise.





## API Reference



### Header File


* components/esp_adc/include/esp_adc/adc_cali.h
* This header file can be included with:



> ```
> #include "esp_adc/adc_cali.h"
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


#### adc_cali_check_scheme


```


esp_err_t adc_cali_check_scheme(adc_cali_scheme_ver_t *scheme_mask)
Check the supported ADC calibration scheme. 

Parameters
scheme_mask -- [out] Supported ADC calibration scheme(s)

Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid argument
ESP_ERR_NOT_SUPPORTED: No supported calibration scheme 





```

#### adc_cali_raw_to_voltage


```


esp_err_t adc_cali_raw_to_voltage(adc_cali_handle_t handle, int raw, int *voltage)
Convert ADC raw data to calibrated voltage. 

Parameters

handle -- [in] ADC calibration handle 
raw -- [in] ADC raw data 
voltage -- [out] Calibrated ADC voltage (in mV)


Returns

ESP_OK: On success
ESP_ERR_INVALID_ARG: Invalid argument
ESP_ERR_INVALID_STATE: Invalid state, scheme didn't registered 





```



### Type Definitions


#### adc_cali_handle_t


```


typedef struct adc_cali_scheme_t *adc_cali_handle_t
ADC calibration handle. 

```



### Enumerations


#### adc_cali_scheme_ver_t


```


enum adc_cali_scheme_ver_t
ADC calibration scheme. 
Values:
ADC_CALI_SCHEME_VER_LINE_FITTING

enumerator ADC_CALI_SCHEME_VER_LINE_FITTING
Line fitting scheme. 

ADC_CALI_SCHEME_VER_CURVE_FITTING

enumerator ADC_CALI_SCHEME_VER_CURVE_FITTING
Curve fitting scheme. 


```



### Header File


* components/esp_adc/include/esp_adc/adc_cali_scheme.h
* This header file can be included with:



> ```
> #include "esp_adc/adc_cali_scheme.h"
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