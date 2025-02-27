# Temperature Sensor




## Introduction


The ESP32-S3 has a built-in sensor used to measure the chip's internal temperature. The temperature sensor module contains an 8-bit Sigma-Delta analog-to-digital converter (ADC) and a digital-to-analog converter (DAC) to compensate for the temperature measurement.


Due to restrictions of hardware, the sensor has predefined measurement ranges with specific measurement errors. See the table below for details.







| Predefined Range (°C) | Error (°C) |
| --- | --- |
| 50 ~ 125 | < 3 |
| 20 ~ 100 | < 2 |
| -10 ~ 80 | < 1 |
| -30 ~ 50 | < 2 |
| -40 ~ 20 | < 3 |



Note


The temperature sensor is designed primarily to measure the temperature changes inside the chip. The internal temperature of a chip is usually higher than the ambient temperature, and is affected by factors such as the microcontroller's clock frequency or I/O load, and the external thermal environment.





## Functional Overview


The description of the temperature sensor functionality is divided into the following sections:


* Resource Allocation - covers which parameters should be set up to get a temperature sensor handle and how to recycle the resources when the temperature sensor finishes working.
* Enable and Disable Temperature Sensor - covers how to enable and disable the temperature sensor.
* Get Temperature Value - covers how to get the real-time temperature value.
* Power Management - covers how the temperature sensor is affected when changing power mode (e.g., Light-sleep mode).
* Thread Safety - covers how to make the driver to be thread-safe.






### Resource Allocation


The ESP32-S3 has just one built-in temperature sensor hardware. The temperature sensor instance is represented by `temperature_sensor_handle_t`, which is also the bond of the context. By using `temperature_sensor_handle_t`, the temperature sensor properties can be accessed and modified in different function calls to control and manage the temperature sensor. The variable would always be the parameter of the temperature APIs with the information of hardware and configurations, so you can just create a pointer of type `temperature_sensor_handle_t` and passing to APIs as needed.


In order to install a built-in temperature sensor instance, the first thing is to evaluate the temperature range in your detection environment. For example, if the testing environment is in a room, the range you evaluate might be 10 °C ~ 30 °C; if the testing in a lamp bulb, the range you evaluate might be 60 °C ~ 110 °C. Based on that, configuration structure `temperature_sensor_config_t` should be defined in advance:


* `range_min`: The minimum value of the testing range you have evaluated.
* `range_max`: The maximum value of the testing range you have evaluated.


After the ranges are set, the structure could be passed to `temperature_sensor_install()`, which will instantiate the temperature sensor instance and return a handle.


As mentioned above, different measure ranges have different measurement errors. You do not need to care about the measurement error because we have an internal mechanism to choose the minimum error according to the given range.


If the temperature sensor is no longer needed, you need to call `temperature_sensor_uninstall()` to free the temperature sensor resource.



#### Creating a Temperature Sensor Handle


* Step 1: Evaluate the testing range. In this example, the range is 20 °C ~ 50 °C.
* Step 2: Configure the range and obtain a handle.



```
temperature_sensor_handle_t temp_handle = NULL;
temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(20, 50);
ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_handle));

```





### Enable and Disable Temperature Sensor


1. Enable the temperature sensor by calling `temperature_sensor_enable()`. The internal temperature sensor circuit will start to work. The driver state will transit from init to enable.
2. To Disable the temperature sensor, please call `temperature_sensor_disable()`.




### Get Temperature Value


After the temperature sensor is enabled by `temperature_sensor_enable()`, you can get the current temperature by calling `temperature_sensor_get_celsius()`.



```
// Enable temperature sensor
ESP_ERROR_CHECK(temperature_sensor_enable(temp_handle));
// Get converted sensor data
float tsens_out;
ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_handle, &tsens_out));
printf("Temperature in %f °C\n", tsens_out);
// Disable the temperature sensor if it is not needed and save the power
ESP_ERROR_CHECK(temperature_sensor_disable(temp_handle));

```




### Power Management


As the temperature sensor does not use the APB clock, it will keep working no matter if the power management is enabled with `CONFIG_PM_ENABLE`.




### Thread Safety


In the temperature sensor driver, we do not add any protection to ensure the thread safety, because typically this driver is only supposed to be used in one task. If you have to use this driver in different tasks, please add extra locks to protect it.





## Unexpected Behaviors


1. The value you get from the chip is usually different from the ambient temperature. It is because the temperature sensor is built inside the chip. To some extent, it measures the temperature of the chip.
2. When installing the temperature sensor, the driver may print `the boundary you gave cannot meet the range of internal temperature sensor`. It is because the built-in temperature sensor has a testing limit. The error comes from the incorrect configuration of `temperature_sensor_config_t` as follow:



> 1. Totally out of range, like 200 °C ~ 300 °C.
> 	2. Cross the boundary of each predefined measurement. like 40 °C ~ 110 °C.




## Application Example


* Temperature sensor reading example: peripherals/temperature_sensor/temp_sensor.







## API Reference



### Header File


* components/esp_driver_tsens/include/driver/temperature_sensor.h
* This header file can be included with:



> ```
> #include "driver/temperature_sensor.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_tsens` component. To declare that your component depends on `esp_driver_tsens`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_tsens
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_tsens
> 
> ```




### Functions


#### temperature_sensor_install


```


esp_err_t temperature_sensor_install(const temperature_sensor_config_t *tsens_config, temperature_sensor_handle_t *ret_tsens)
Install temperature sensor driver. 

Parameters

tsens_config -- Pointer to config structure. 
ret_tsens -- Return the pointer of temperature sensor handle. 


Returns

ESP_OK if succeed 





```

#### temperature_sensor_uninstall


```


esp_err_t temperature_sensor_uninstall(temperature_sensor_handle_t tsens)
Uninstall the temperature sensor driver. 

Parameters
tsens -- The handle created by temperature_sensor_install(). 

Returns

ESP_OK if succeed. 





```

#### temperature_sensor_enable


```


esp_err_t temperature_sensor_enable(temperature_sensor_handle_t tsens)
Enable the temperature sensor. 

Parameters
tsens -- The handle created by temperature_sensor_install(). 

Returns

ESP_OK Success
ESP_ERR_INVALID_STATE if temperature sensor is enabled already. 





```

#### temperature_sensor_disable


```


esp_err_t temperature_sensor_disable(temperature_sensor_handle_t tsens)
Disable temperature sensor. 

Parameters
tsens -- The handle created by temperature_sensor_install(). 

Returns

ESP_OK Success
ESP_ERR_INVALID_STATE if temperature sensor is not enabled yet. 





```

#### temperature_sensor_get_celsius


```


esp_err_t temperature_sensor_get_celsius(temperature_sensor_handle_t tsens, float *out_celsius)
Read temperature sensor data that is converted to degrees Celsius. 

Note
Should not be called from interrupt.


Parameters

tsens -- The handle created by temperature_sensor_install(). 
out_celsius -- The measure output value. 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG invalid arguments
ESP_ERR_INVALID_STATE Temperature sensor is not enabled yet.
ESP_FAIL Parse the sensor data into ambient temperature failed (e.g. out of the range). 





```



### Structures


#### temperature_sensor_config_t


```


struct temperature_sensor_config_t
Configuration of measurement range for the temperature sensor. 

Note
If you see the log the boundary you gave cannot meet the range of internal temperature sensor. You may need to refer to predefined range listed doc api-reference/peripherals/Temperature sensor. 


Public Members
range_min

int range_min
the minimum value of the temperature you want to test 

range_max

int range_max
the maximum value of the temperature you want to test 

clk_src

temperature_sensor_clk_src_t clk_src
the clock source of the temperature sensor. 



```



### Macros


#### TEMPERATURE_SENSOR_CONFIG_DEFAULT


```


TEMPERATURE_SENSOR_CONFIG_DEFAULT(min, max)
temperature_sensor_config_t default constructor 

```



### Type Definitions


#### temperature_sensor_handle_t


```


typedef struct temperature_sensor_obj_t *temperature_sensor_handle_t
Type of temperature sensor driver handle. 

```



### Header File


* components/hal/include/hal/temperature_sensor_types.h
* This header file can be included with:



> ```
> #include "hal/temperature_sensor_types.h"
> 
> ```




### Type Definitions


#### temperature_sensor_clk_src_t


```


typedef soc_periph_temperature_sensor_clk_src_t temperature_sensor_clk_src_t
temperature sensor clock source 

```



### Enumerations


#### temperature_sensor_etm_event_type_t


```


enum temperature_sensor_etm_event_type_t
temperature sensor event types enum 
Values:
TEMPERATURE_SENSOR_EVENT_OVER_LIMIT

enumerator TEMPERATURE_SENSOR_EVENT_OVER_LIMIT
Temperature sensor over limit event 

TEMPERATURE_SENSOR_EVENT_MAX

enumerator TEMPERATURE_SENSOR_EVENT_MAX
Maximum number of temperature sensor events 


```

#### temperature_sensor_etm_task_type_t


```


enum temperature_sensor_etm_task_type_t
temperature sensor task types enum 
Values:
TEMPERATURE_SENSOR_TASK_START

enumerator TEMPERATURE_SENSOR_TASK_START
Temperature sensor start task 

TEMPERATURE_SENSOR_TASK_STOP

enumerator TEMPERATURE_SENSOR_TASK_STOP
Temperature sensor stop task 

TEMPERATURE_SENSOR_TASK_MAX

enumerator TEMPERATURE_SENSOR_TASK_MAX
Maximum number of temperature sensor tasks 


```