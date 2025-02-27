# Touch Sensor




## Introduction


A touch sensor system is built on a substrate which carries electrodes and relevant connections under a protective flat surface. When the surface is touched, the capacitance variation is used to evaluate if the touch was valid.


The sensing pads can be arranged in different combinations (e.g., matrix, slider), so that a larger area or more points can be detected. The touch pad sensing process is under the control of a hardware-implemented finite-state machine (FSM) which is initiated by software or a dedicated hardware timer.


For design, operation, and control registers of a touch sensor, see **ESP32-S3 Technical Reference Manual** > **On-Chip Sensors and Analog Signal Processing** [PDF].


In-depth design details of touch sensors and firmware development guidelines for ESP32-S3 are available in Touch Sensor Application Note.




## Functionality Overview


Description of API is broken down into groups of functions to provide a quick overview of the following features:


* Initialization of touch pad driver
* Configuration of touch pad GPIO pins
* Taking measurements
* Adjusting parameters of measurements
* Filtering measurements
* Touch detection methods
* Setting up interrupts to report touch detection
* Waking up from Sleep mode on interrupt


For detailed description of a particular function, please go to Section API Reference. Practical implementation of this API is covered in Section Application Examples.



### Initialization


Before using a touch pad, you need to initialize the touch pad driver by calling the function `touch_pad_init()`. This function sets several `.._DEFAULT` driver parameters listed in API Reference under **Macros**. It also removes the information about which pads have been touched before, if any, and disables interrupts.


If the driver is not required anymore, deinitialize it by calling `touch_pad_deinit()`.




### Configuration


Enabling the touch sensor functionality for a particular GPIO is done with `touch_pad_config()`. The following 14 capacitive touch pads are supported for ESP32-S3.







| Touch Pad | GPIO Pin |
| --- | --- |
| T0 | Internal channel, not connect to a GPIO |
| T1 | GPIO1 |
| T2 | GPIO2 |
| T3 | GPIO3 |
| T4 | GPIO4 |
| T5 | GPIO5 |
| T6 | GPIO6 |
| T7 | GPIO7 |
| T8 | GPIO8 |
| T9 | GPIO9 |
| T10 | GPIO10 |
| T11 | GPIO11 |
| T12 | GPIO12 |
| T13 | GPIO13 |
| T14 | GPIO14 |


Use the function `touch_pad_set_fsm_mode()` to select if touch pad measurement (operated by FSM) should be started automatically by a hardware timer, or by software. If software mode is selected, use `touch_pad_sw_start()` to start the FSM.




### Touch State Measurements


The following function come in handy to read raw measurements from the sensor:


* `touch_pad_read_raw_data()`


It can also be used, for example, to evaluate a particular touch pad design by checking the range of sensor readings when a pad is touched or released. This information can be then used to establish a touch threshold.


For the demonstration of how to read the touch pad data, check the application example peripherals/touch_sensor/touch_sensor_v2/touch_pad_read.




### Method of Measurements


The touch sensor records the period of time (i.e., the number of clock cycles) over a fixed charge/discharge cycles (specified by `touch_pad_set_charge_discharge_times()`). The count result is the raw data that read from `touch_pad_read_raw_data()`. After finishing one measurement, the touch sensor sleeps until the next measurement start, this interval between two measurements can be set by `touch_pad_set_measurement_interval()`.



Note


If the specified charge and discharge cycles for measurement is too small, the result may be inaccurate, but increasing charge and discharge cycles will increase the power consumption as well. Additionally, the response of the touch sensor will slow down if the total time of the interval and measurement is too long.





### Optimization of Measurements


A touch sensor has several configurable parameters to match the characteristics of a particular touch pad design. For instance, to sense smaller capacity changes, it is possible to narrow down the reference voltage range within which the touch pads are charged/discharged. The high and low reference voltages are set using the function `touch_pad_set_voltage()`.


Besides the ability to discern smaller capacity changes, a positive side effect is reduction of power consumption for low power applications. A likely negative effect is an increase in measurement noise. If the dynamic range of obtained readings is still satisfactory, then further reduction of power consumption might be done by reducing the measurement time with `touch_pad_set_charge_discharge_times()`.


The following list summarizes available measurement parameters and corresponding 'set' functions:


* Touch pad charge / discharge parameters:



> + voltage range: `touch_pad_set_voltage()`
> 	+ speed (slope): `touch_pad_set_cnt_mode()`


* Charge and discharge times of one measurement: `touch_pad_set_charge_discharge_times()`


Relationship between the voltage range (high/low reference voltages), speed (slope), and measurement time is shown in the figure below.



!Touch Pad - relationship between measurement parameters



Touch pad - relationship between measurement parameters






The last chart **Output** represents the touch sensor reading, i.e., the time taken to accumulate the fixed number of cycles.


All functions are provided in pairs to **set** a specific parameter and to **get** the current parameter's value, e.g., `touch_pad_set_voltage()` and `touch_pad_get_voltage()`.




### Filtering of Measurements


If measurements are noisy, you can filter them with provided API functions. The ESP32-S3's touch functionality provide two sets of APIs for doing this.


There is an internal touch channel that is not connected to any external GPIO. The measurements from this denoise pad can be used to filters out interference introduced on all channels, such as noise introduced by the power supply and external EMI.


The denoise parameters are set with the function `touch_pad_denoise_set_config()` and started by with `touch_pad_denoise_enable()`


There is also a configurable hardware implemented IIR-filter (infinite impulse response). This IIR-filter is configured with the function `touch_pad_filter_set_config()` and enabled by calling `touch_pad_filter_enable()`




### Touch Detection


Touch detection is implemented in ESP32's hardware based on the user-configured threshold and raw measurements executed by FSM. Use the functions `touch_pad_get_status()` to check which pads have been touched and `touch_pad_clear_status()` to clear the touch status information.


Hardware touch detection can also be wired to interrupts. This is described in the next section.


If measurements are noisy and capacity changes are small, hardware touch detection might be unreliable. To resolve this issue, instead of using hardware detection/provided interrupts, implement measurement filtering and perform touch detection in your own application. For sample implementation of both methods of touch detection, see peripherals/touch_sensor/touch_sensor_v2/touch_pad_interrupt.




### Touch Triggered Interrupts


Before enabling an interrupt on a touch detection, you should establish a touch detection threshold. Use the functions described in Touch State Measurements to read and display sensor measurements when a pad is touched and released. Apply a filter if measurements are noisy and relative capacity changes are small. Depending on your application and environment conditions, test the influence of temperature and power supply voltage changes on measured values.


Once a detection threshold is established, it can be set during initialization with `touch_pad_config()` or at the runtime with `touch_pad_set_thresh()`.


Finally, configure and manage interrupt calls using the following functions:


* `touch_pad_isr_register()` / `touch_pad_isr_deregister()`
* `touch_pad_intr_enable()` / `touch_pad_intr_disable()`


When interrupts are operational, you can obtain the information from which particular pad an interrupt came by invoking `touch_pad_get_status()` and clear the pad status with `touch_pad_clear_status()`.





## Application Examples


* Touch sensor read example: peripherals/touch_sensor/touch_sensor_v2/touch_pad_read.
* Touch sensor interrupt example: peripherals/touch_sensor/touch_sensor_v2/touch_pad_interrupt.




## API Reference



### Header File


* components/driver/touch_sensor/esp32s3/include/driver/touch_sensor.h
* This header file can be included with:



> ```
> #include "driver/touch_sensor.h"
> 
> ```
* This header file is a part of the API provided by the `driver` component. To declare that your component depends on `driver`, add the following to your CMakeLists.txt:



> ```
> REQUIRES driver
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES driver
> 
> ```




### Functions


#### touch_pad_fsm_start


```


esp_err_t touch_pad_fsm_start(void)
Set touch sensor FSM start. 

Note
Start FSM after the touch sensor FSM mode is set. 


Note
Call this function will reset benchmark of all touch channels. 


Returns

ESP_OK on success 





```

#### touch_pad_fsm_stop


```


esp_err_t touch_pad_fsm_stop(void)
Stop touch sensor FSM. 

Returns

ESP_OK on success 





```

#### touch_pad_sw_start


```


esp_err_t touch_pad_sw_start(void)
Trigger a touch sensor measurement, only support in SW mode of FSM. 

Returns

ESP_OK on success 





```

#### touch_pad_set_charge_discharge_times


```


esp_err_t touch_pad_set_charge_discharge_times(uint16_t charge_discharge_times)
Set charge and discharge times of each measurement. 

Note
This function will specify the charge and discharge times in each measurement period The clock is sourced from SOC_MOD_CLK_RTC_FAST, and its default frequency is SOC_CLK_RC_FAST_FREQ_APPROX The touch sensor will record the total clock cycles of all the charge and discharge cycles as the final result (raw value) 


Note
If the charge and discharge times is too small, it may lead to inaccurate results.


Parameters
charge_discharge_times -- Charge and discharge times, range: 0 ~ 0xffff. No exact typical value can be recommended because the capacity is influenced by the hardware design and how finger touches, but suggest adjusting this value to make the measurement time around 1 ms. 

Returns

ESP_OK Set charge and discharge times success 





```

#### touch_pad_get_charge_discharge_times


```


esp_err_t touch_pad_get_charge_discharge_times(uint16_t *charge_discharge_times)
Get charge and discharge times of each measurement. 

Parameters
charge_discharge_times -- Charge and discharge times 

Returns

ESP_OK Get charge_discharge_times success
ESP_ERR_INVALID_ARG The input parameter is NULL 





```

#### touch_pad_set_measurement_interval


```


esp_err_t touch_pad_set_measurement_interval(uint16_t interval_cycle)
Set the interval between two measurements. 

Note
The touch sensor will sleep between two measurements This function is to set the interval cycle And the interval is clocked from SOC_MOD_CLK_RTC_SLOW, its default frequency is SOC_CLK_RC_SLOW_FREQ_APPROX


Parameters
interval_cycle -- The interval between two measurements sleep_time = interval_cycle / SOC_CLK_RC_SLOW_FREQ_APPROX. The approximate frequency value of RTC_SLOW_CLK can be obtained using rtc_clk_slow_freq_get_hz function. 

Returns

ESP_OK Set interval cycle success 





```

#### touch_pad_get_measurement_interval


```


esp_err_t touch_pad_get_measurement_interval(uint16_t *interval_cycle)
Get the interval between two measurements. 

Parameters
interval_cycle -- The interval between two measurements 

Returns

ESP_OK Get interval cycle success
ESP_ERR_INVALID_ARG The input parameter is NULL 





```

#### touch_pad_set_meas_time


```


esp_err_t touch_pad_set_meas_time(uint16_t sleep_cycle, uint16_t meas_times)
Set touch sensor times of charge and discharge and sleep time. Excessive total time will slow down the touch response. Too small measurement time will not be sampled enough, resulting in inaccurate measurements. 

Note
The touch sensor will measure time of a fixed number of charge/discharge cycles (specified as the second parameter). That means the time (raw value) will increase as the capacity of the touch pad is increasing. The time (raw value) here is the number of clock cycles which is sourced from SOC_MOD_CLK_RTC_FAST and at (SOC_CLK_RC_FAST_FREQ_APPROX) Hz as default 


Note
The greater the duty cycle of the measurement time, the more system power is consumed.


Parameters

sleep_cycle -- The touch sensor will sleep after each measurement. sleep_cycle decide the interval between each measurement. t_sleep = sleep_cycle / SOC_CLK_RC_SLOW_FREQ_APPROX. The approximate frequency value of RTC_SLOW_CLK can be obtained using rtc_clk_slow_freq_get_hz function. 
meas_times -- The times of charge and discharge in each measurement of touch channels. Range: 0 ~ 0xffff. Recommended typical value: Modify this value to make the measurement time around 1 ms. 


Returns

ESP_OK on success 





```

#### touch_pad_get_meas_time


```


esp_err_t touch_pad_get_meas_time(uint16_t *sleep_cycle, uint16_t *meas_times)
Get touch sensor times of charge and discharge and sleep time. 

Parameters

sleep_cycle -- Pointer to accept sleep cycle number 
meas_times -- Pointer to accept measurement times count. 


Returns

ESP_OK on success 





```

#### touch_pad_set_idle_channel_connect


```


esp_err_t touch_pad_set_idle_channel_connect(touch_pad_conn_type_t type)
Set the connection type of touch channels in idle status. When a channel is in measurement mode, other initialized channels are in idle mode. The touch channel is generally adjacent to the trace, so the connection state of the idle channel affects the stability and sensitivity of the test channel. The CONN_HIGHZ(high resistance) setting increases the sensitivity of touch channels. The CONN_GND(grounding) setting increases the stability of touch channels. 

Parameters
type -- Select idle channel connect to high resistance state or ground. 

Returns

ESP_OK on success 





```

#### touch_pad_get_idle_channel_connect


```


esp_err_t touch_pad_get_idle_channel_connect(touch_pad_conn_type_t *type)
Get the connection type of touch channels in idle status. When a channel is in measurement mode, other initialized channels are in idle mode. The touch channel is generally adjacent to the trace, so the connection state of the idle channel affects the stability and sensitivity of the test channel. The CONN_HIGHZ(high resistance) setting increases the sensitivity of touch channels. The CONN_GND(grounding) setting increases the stability of touch channels. 

Parameters
type -- Pointer to connection type. 

Returns

ESP_OK on success 





```

#### touch_pad_set_thresh


```


esp_err_t touch_pad_set_thresh(touch_pad_t touch_num, uint32_t threshold)
Set the trigger threshold of touch sensor. The threshold determines the sensitivity of the touch sensor. The threshold is the original value of the trigger state minus the benchmark value. 

Note
If set "TOUCH_PAD_THRESHOLD_MAX", the touch is never be triggered. 


Parameters

touch_num -- touch pad index 
threshold -- threshold of touch sensor. Should be less than the max change value of touch. 


Returns

ESP_OK on success 





```

#### touch_pad_get_thresh


```


esp_err_t touch_pad_get_thresh(touch_pad_t touch_num, uint32_t *threshold)
Get touch sensor trigger threshold. 

Parameters

touch_num -- touch pad index 
threshold -- pointer to accept threshold 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if argument is wrong 





```

#### touch_pad_set_channel_mask


```


esp_err_t touch_pad_set_channel_mask(uint16_t enable_mask)
Register touch channel into touch sensor scan group. The working mode of the touch sensor is cyclically scanned. This function will set the scan bits according to the given bitmask. 

Note
If set this mask, the FSM timer should be stop firsty. 


Note
The touch sensor that in scan map, should be deinit GPIO function firstly by touch_pad_io_init. 


Parameters
enable_mask -- bitmask of touch sensor scan group. e.g. TOUCH_PAD_NUM14 -> BIT(14) 

Returns

ESP_OK on success 





```

#### touch_pad_get_channel_mask


```


esp_err_t touch_pad_get_channel_mask(uint16_t *enable_mask)
Get the touch sensor scan group bit mask. 

Parameters
enable_mask -- Pointer to bitmask of touch sensor scan group. e.g. TOUCH_PAD_NUM14 -> BIT(14) 

Returns

ESP_OK on success 





```

#### touch_pad_clear_channel_mask


```


esp_err_t touch_pad_clear_channel_mask(uint16_t enable_mask)
Clear touch channel from touch sensor scan group. The working mode of the touch sensor is cyclically scanned. This function will clear the scan bits according to the given bitmask. 

Note
If clear all mask, the FSM timer should be stop firsty. 


Parameters
enable_mask -- bitmask of touch sensor scan group. e.g. TOUCH_PAD_NUM14 -> BIT(14) 

Returns

ESP_OK on success 





```

#### touch_pad_config


```


esp_err_t touch_pad_config(touch_pad_t touch_num)
Configure parameter for each touch channel. 

Note
Touch num 0 is denoise channel, please use touch_pad_denoise_enable to set denoise function 


Parameters
touch_num -- touch pad index 

Returns

ESP_OK Success
ESP_ERR_INVALID_ARG if argument wrong
ESP_FAIL if touch pad not initialized 





```

#### touch_pad_reset


```


esp_err_t touch_pad_reset(void)
Reset the FSM of touch module. 

Note
Call this function after touch_pad_fsm_stop. 


Returns

ESP_OK Success 





```

#### touch_pad_get_current_meas_channel


```


touch_pad_t touch_pad_get_current_meas_channel(void)
Get the current measure channel. 

Note
Should be called when touch sensor measurement is in cyclic scan mode. 


Returns

touch channel number 





```

#### touch_pad_read_intr_status_mask


```


uint32_t touch_pad_read_intr_status_mask(void)
Get the touch sensor interrupt status mask. 

Returns

touch interrupt bit 





```

#### touch_pad_intr_enable


```


esp_err_t touch_pad_intr_enable(touch_pad_intr_mask_t int_mask)
Enable touch sensor interrupt by bitmask. 

Note
This API can be called in ISR handler. 


Parameters
int_mask -- Pad mask to enable interrupts 

Returns

ESP_OK on success 





```

#### touch_pad_intr_disable


```


esp_err_t touch_pad_intr_disable(touch_pad_intr_mask_t int_mask)
Disable touch sensor interrupt by bitmask. 

Note
This API can be called in ISR handler. 


Parameters
int_mask -- Pad mask to disable interrupts 

Returns

ESP_OK on success 





```

#### touch_pad_intr_clear


```


esp_err_t touch_pad_intr_clear(touch_pad_intr_mask_t int_mask)
Clear touch sensor interrupt by bitmask. 

Parameters
int_mask -- Pad mask to clear interrupts 

Returns

ESP_OK on success 





```

#### touch_pad_isr_register


```


esp_err_t touch_pad_isr_register(intr_handler_t fn, void *arg, touch_pad_intr_mask_t intr_mask)
Register touch-pad ISR. The handler will be attached to the same CPU core that this function is running on. 

Parameters

fn -- Pointer to ISR handler 
arg -- Parameter for ISR 
intr_mask -- Enable touch sensor interrupt handler by bitmask. 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Arguments error
ESP_ERR_NO_MEM No memory 





```

#### touch_pad_timeout_set


```


esp_err_t touch_pad_timeout_set(bool enable, uint32_t threshold)
Enable/disable the timeout check and set timeout threshold for all touch sensor channels measurements. If enable: When the touch reading of a touch channel exceeds the measurement threshold, a timeout interrupt will be generated. If disable: the FSM does not check if the channel under measurement times out. 

Note
The threshold compared with touch readings. 


Note
In order to avoid abnormal short circuit of some touch channels. This function should be turned on. Ensure the normal operation of other touch channels.


Parameters

enable -- true(default): Enable the timeout check; false: Disable the timeout check. 
threshold -- For all channels, the maximum value that will not be exceeded during normal operation.


Returns

ESP_OK Success 





```

#### touch_pad_timeout_resume


```


esp_err_t touch_pad_timeout_resume(void)
Call this interface after timeout to make the touch channel resume normal work. Point on the next channel to measure. If this API is not called, the touch FSM will stop the measurement after timeout interrupt. 

Note
Call this API after finishes the exception handling by user.


Returns

ESP_OK Success 





```

#### touch_pad_read_raw_data


```


esp_err_t touch_pad_read_raw_data(touch_pad_t touch_num, uint32_t *raw_data)
get raw data of touch sensor. 

Note
After the initialization is complete, the "raw_data" is max value. You need to wait for a measurement cycle before you can read the correct touch value. 


Parameters

touch_num -- touch pad index 
raw_data -- pointer to accept touch sensor value 


Returns

ESP_OK Success
ESP_FAIL Touch channel 0 haven't this parameter. 





```

#### touch_pad_read_benchmark


```


esp_err_t touch_pad_read_benchmark(touch_pad_t touch_num, uint32_t *benchmark)
get benchmark of touch sensor. 

Note
After initialization, the benchmark value is the maximum during the first measurement period. 


Parameters

touch_num -- touch pad index 
benchmark -- pointer to accept touch sensor benchmark value 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Touch channel 0 haven't this parameter. 





```

#### touch_pad_filter_read_smooth


```


esp_err_t touch_pad_filter_read_smooth(touch_pad_t touch_num, uint32_t *smooth)
Get smoothed data that obtained by filtering the raw data. 

Parameters

touch_num -- touch pad index 
smooth -- pointer to smoothed data 




```

#### touch_pad_reset_benchmark


```


esp_err_t touch_pad_reset_benchmark(touch_pad_t touch_num)
Force reset benchmark to raw data of touch sensor. 

Parameters
touch_num -- touch pad index
TOUCH_PAD_MAX Reset basaline of all channels 



Returns

ESP_OK Success 





```

#### touch_pad_filter_set_config


```


esp_err_t touch_pad_filter_set_config(const touch_filter_config_t *filter_info)
set parameter of touch sensor filter and detection algorithm. For more details on the detection algorithm, please refer to the application documentation. 

Parameters
filter_info -- select filter type and threshold of detection algorithm 

Returns

ESP_OK Success 





```

#### touch_pad_filter_get_config


```


esp_err_t touch_pad_filter_get_config(touch_filter_config_t *filter_info)
get parameter of touch sensor filter and detection algorithm. For more details on the detection algorithm, please refer to the application documentation. 

Parameters
filter_info -- select filter type and threshold of detection algorithm 

Returns

ESP_OK Success 





```

#### touch_pad_filter_enable


```


esp_err_t touch_pad_filter_enable(void)
enable touch sensor filter for detection algorithm. For more details on the detection algorithm, please refer to the application documentation. 

Returns

ESP_OK Success 





```

#### touch_pad_filter_disable


```


esp_err_t touch_pad_filter_disable(void)
disable touch sensor filter for detection algorithm. For more details on the detection algorithm, please refer to the application documentation. 

Returns

ESP_OK Success 





```

#### touch_pad_denoise_set_config


```


esp_err_t touch_pad_denoise_set_config(const touch_pad_denoise_t *denoise)
set parameter of denoise pad (TOUCH_PAD_NUM0). T0 is an internal channel that does not have a corresponding external GPIO. T0 will work simultaneously with the measured channel Tn. Finally, the actual measured value of Tn is the value after subtracting lower bits of T0. The noise reduction function filters out interference introduced simultaneously on all channels, such as noise introduced by power supplies and external EMI. 

Parameters
denoise -- parameter of denoise 

Returns

ESP_OK Success 





```

#### touch_pad_denoise_get_config


```


esp_err_t touch_pad_denoise_get_config(touch_pad_denoise_t *denoise)
get parameter of denoise pad (TOUCH_PAD_NUM0). 

Parameters
denoise -- Pointer to parameter of denoise 

Returns

ESP_OK Success 





```

#### touch_pad_denoise_enable


```


esp_err_t touch_pad_denoise_enable(void)
enable denoise function. T0 is an internal channel that does not have a corresponding external GPIO. T0 will work simultaneously with the measured channel Tn. Finally, the actual measured value of Tn is the value after subtracting lower bits of T0. The noise reduction function filters out interference introduced simultaneously on all channels, such as noise introduced by power supplies and external EMI. 

Returns

ESP_OK Success 





```

#### touch_pad_denoise_disable


```


esp_err_t touch_pad_denoise_disable(void)
disable denoise function. 

Returns

ESP_OK Success 





```

#### touch_pad_denoise_read_data


```


esp_err_t touch_pad_denoise_read_data(uint32_t *data)
Get denoise measure value (TOUCH_PAD_NUM0). 

Parameters
data -- Pointer to receive denoise value 

Returns

ESP_OK Success 





```

#### touch_pad_waterproof_set_config


```


esp_err_t touch_pad_waterproof_set_config(const touch_pad_waterproof_t *waterproof)
set parameter of waterproof function. 
   The waterproof function includes a shielded channel (TOUCH_PAD_NUM14) and a guard channel.
   Guard pad is used to detect the large area of water covering the touch panel.
   Shield pad is used to shield the influence of water droplets covering the touch panel.
   It is generally designed as a grid and is placed around the touch buttons.




Parameters
waterproof -- parameter of waterproof 

Returns

ESP_OK Success 





```

#### touch_pad_waterproof_get_config


```


esp_err_t touch_pad_waterproof_get_config(touch_pad_waterproof_t *waterproof)
get parameter of waterproof function. 

Parameters
waterproof -- parameter of waterproof 

Returns

ESP_OK Success 





```

#### touch_pad_waterproof_enable


```


esp_err_t touch_pad_waterproof_enable(void)
Enable parameter of waterproof function. Should be called after function touch_pad_waterproof_set_config. 

Returns

ESP_OK Success 





```

#### touch_pad_waterproof_disable


```


esp_err_t touch_pad_waterproof_disable(void)
Disable parameter of waterproof function. 

Returns

ESP_OK Success 





```

#### touch_pad_proximity_enable


```


esp_err_t touch_pad_proximity_enable(touch_pad_t touch_num, bool enabled)
Enable/disable proximity function of touch channels. The proximity sensor measurement is the accumulation of touch channel measurements. 

Note
Supports up to three touch channels configured as proximity sensors. 


Parameters

touch_num -- touch pad index 
enabled -- true: enable the proximity function; false: disable the proximity function 


Returns

ESP_OK: Configured correctly.
ESP_ERR_INVALID_ARG: Touch channel number error.
ESP_ERR_NOT_SUPPORTED: Don't support configured. 





```

#### touch_pad_proximity_set_count


```


esp_err_t touch_pad_proximity_set_count(touch_pad_t touch_num, uint32_t count)
Set measure count of proximity channel. The proximity sensor measurement is the accumulation of touch channel measurements. 

Note
All proximity channels use the same count value. So please pass the parameter TOUCH_PAD_MAX. 


Parameters

touch_num -- Touch pad index. In this version, pass the parameter TOUCH_PAD_MAX. 
count -- The cumulative times of measurements for proximity pad. Range: 0 ~ 255. 


Returns

ESP_OK: Configured correctly.
ESP_ERR_INVALID_ARG: Touch channel number error. 





```

#### touch_pad_proximity_get_count


```


esp_err_t touch_pad_proximity_get_count(touch_pad_t touch_num, uint32_t *count)
Get measure count of proximity channel. The proximity sensor measurement is the accumulation of touch channel measurements. 

Note
All proximity channels use the same count value. So please pass the parameter TOUCH_PAD_MAX. 


Parameters

touch_num -- Touch pad index. In this version, pass the parameter TOUCH_PAD_MAX. 
count -- The cumulative times of measurements for proximity pad. Range: 0 ~ 255. 


Returns

ESP_OK: Configured correctly.
ESP_ERR_INVALID_ARG: Touch channel number error. 





```

#### touch_pad_proximity_get_data


```


esp_err_t touch_pad_proximity_get_data(touch_pad_t touch_num, uint32_t *measure_out)
Get the accumulated measurement of the proximity sensor. The proximity sensor measurement is the accumulation of touch channel measurements. 

Parameters

touch_num -- touch pad index 
measure_out -- If the accumulation process does not end, the measure_out is the process value. 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG Touch num is not proximity 





```

#### touch_pad_sleep_channel_get_info


```


esp_err_t touch_pad_sleep_channel_get_info(touch_pad_sleep_channel_t *slp_config)
Get parameter of touch sensor sleep channel. The touch sensor can works in sleep mode to wake up sleep. 

Note
After the sleep channel is configured, Please use special functions for sleep channel. e.g. The user should uses touch_pad_sleep_channel_read_data instead of touch_pad_read_raw_data to obtain the sleep channel reading.


Parameters
slp_config -- touch sleep pad config. 

Returns

ESP_OK Success 





```

#### touch_pad_sleep_channel_enable


```


esp_err_t touch_pad_sleep_channel_enable(touch_pad_t pad_num, bool enable)
Enable/Disable sleep channel function for touch sensor. The touch sensor can works in sleep mode to wake up sleep. 

Note
ESP32S2 only support one sleep channel. 


Note
After the sleep channel is configured, Please use special functions for sleep channel. e.g. The user should uses touch_pad_sleep_channel_read_data instead of touch_pad_read_raw_data to obtain the sleep channel reading.


Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
enable -- true: enable sleep pad for touch sensor; false: disable sleep pad for touch sensor; 


Returns

ESP_OK Success 





```

#### touch_pad_sleep_channel_enable_proximity


```


esp_err_t touch_pad_sleep_channel_enable_proximity(touch_pad_t pad_num, bool enable)
Enable/Disable proximity function for sleep channel. The touch sensor can works in sleep mode to wake up sleep. 

Note
ESP32S2 only support one sleep channel.


Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
enable -- true: enable proximity for sleep channel; false: disable proximity for sleep channel; 


Returns

ESP_OK Success 





```

#### touch_pad_sleep_set_threshold


```


esp_err_t touch_pad_sleep_set_threshold(touch_pad_t pad_num, uint32_t touch_thres)
Set the trigger threshold of touch sensor in deep sleep. The threshold determines the sensitivity of the touch sensor. 

Note
In general, the touch threshold during sleep can use the threshold parameter parameters before sleep.


Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
touch_thres -- touch sleep pad threshold 


Returns

ESP_OK Success 





```

#### touch_pad_sleep_get_threshold


```


esp_err_t touch_pad_sleep_get_threshold(touch_pad_t pad_num, uint32_t *touch_thres)
Get the trigger threshold of touch sensor in deep sleep. The threshold determines the sensitivity of the touch sensor. 

Note
In general, the touch threshold during sleep can use the threshold parameter parameters before sleep.


Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
touch_thres -- touch sleep pad threshold 


Returns

ESP_OK Success 





```

#### touch_pad_sleep_channel_read_benchmark


```


esp_err_t touch_pad_sleep_channel_read_benchmark(touch_pad_t pad_num, uint32_t *benchmark)
Read benchmark of touch sensor sleep channel. 

Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
benchmark -- pointer to accept touch sensor benchmark value 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG parameter is NULL 





```

#### touch_pad_sleep_channel_read_smooth


```


esp_err_t touch_pad_sleep_channel_read_smooth(touch_pad_t pad_num, uint32_t *smooth_data)
Read smoothed data of touch sensor sleep channel. Smoothed data is filtered from the raw data. 

Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
smooth_data -- pointer to accept touch sensor smoothed data 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG parameter is NULL 





```

#### touch_pad_sleep_channel_read_data


```


esp_err_t touch_pad_sleep_channel_read_data(touch_pad_t pad_num, uint32_t *raw_data)
Read raw data of touch sensor sleep channel. 

Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
raw_data -- pointer to accept touch sensor raw data 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG parameter is NULL 





```

#### touch_pad_sleep_channel_reset_benchmark


```


esp_err_t touch_pad_sleep_channel_reset_benchmark(void)
Reset benchmark of touch sensor sleep channel. 

Returns

ESP_OK Success 





```

#### touch_pad_sleep_channel_read_proximity_cnt


```


esp_err_t touch_pad_sleep_channel_read_proximity_cnt(touch_pad_t pad_num, uint32_t *proximity_cnt)
Read proximity count of touch sensor sleep channel. 

Parameters

pad_num -- Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. 
proximity_cnt -- pointer to accept touch sensor proximity count value 


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG parameter is NULL 





```

#### touch_pad_sleep_channel_set_work_time


```


esp_err_t touch_pad_sleep_channel_set_work_time(uint16_t sleep_cycle, uint16_t meas_times)
Change the operating frequency of touch pad in deep sleep state. Reducing the operating frequency can effectively reduce power consumption. If this function is not called, the working frequency of touch in the deep sleep state is the same as that in the wake-up state. 

Parameters

sleep_cycle -- The touch sensor will sleep after each measurement. sleep_cycle decide the interval between each measurement. t_sleep = sleep_cycle / (RTC_SLOW_CLK frequency). The approximate frequency value of RTC_SLOW_CLK can be obtained using rtc_clk_slow_freq_get_hz function. 
meas_times -- The times of charge and discharge in each measure process of touch channels. The timer frequency is 8Mhz. Range: 0 ~ 0xffff. Recommended typical value: Modify this value to make the measurement time around 1ms. 


Returns

ESP_OK Success 





```



### Header File


* components/driver/touch_sensor/include/driver/touch_sensor_common.h
* This header file can be included with:



> ```
> #include "driver/touch_sensor_common.h"
> 
> ```
* This header file is a part of the API provided by the `driver` component. To declare that your component depends on `driver`, add the following to your CMakeLists.txt:



> ```
> REQUIRES driver
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES driver
> 
> ```




### Functions


#### touch_pad_init


```


esp_err_t touch_pad_init(void)
Initialize touch module. 

Note
If default parameter don't match the usage scenario, it can be changed after this function. 


Returns

ESP_OK Success
ESP_ERR_NO_MEM Touch pad init error
ESP_ERR_NOT_SUPPORTED Touch pad is providing current to external XTAL 





```

#### touch_pad_deinit


```


esp_err_t touch_pad_deinit(void)
Un-install touch pad driver. 

Note
After this function is called, other touch functions are prohibited from being called. 


Returns

ESP_OK Success
ESP_FAIL Touch pad driver not initialized 





```

#### touch_pad_io_init


```


esp_err_t touch_pad_io_init(touch_pad_t touch_num)
Initialize touch pad GPIO. 

Parameters
touch_num -- touch pad index 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if argument is wrong 





```

#### touch_pad_set_voltage


```


esp_err_t touch_pad_set_voltage(touch_high_volt_t refh, touch_low_volt_t refl, touch_volt_atten_t atten)
Set touch sensor high voltage threshold of chanrge. The touch sensor measures the channel capacitance value by charging and discharging the channel. So the high threshold should be less than the supply voltage. 

Parameters

refh -- the value of DREFH 
refl -- the value of DREFL 
atten -- the attenuation on DREFH 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if argument is wrong 





```

#### touch_pad_get_voltage


```


esp_err_t touch_pad_get_voltage(touch_high_volt_t *refh, touch_low_volt_t *refl, touch_volt_atten_t *atten)
Get touch sensor reference voltage,. 

Parameters

refh -- pointer to accept DREFH value 
refl -- pointer to accept DREFL value 
atten -- pointer to accept the attenuation on DREFH 


Returns

ESP_OK on success 





```

#### touch_pad_set_cnt_mode


```


esp_err_t touch_pad_set_cnt_mode(touch_pad_t touch_num, touch_cnt_slope_t slope, touch_tie_opt_t opt)
Set touch sensor charge/discharge speed for each pad. If the slope is 0, the counter would always be zero. If the slope is 1, the charging and discharging would be slow, accordingly. If the slope is set 7, which is the maximum value, the charging and discharging would be fast. 

Note
The higher the charge and discharge current, the greater the immunity of the touch channel, but it will increase the system power consumption. 


Parameters

touch_num -- touch pad index 
slope -- touch pad charge/discharge speed 
opt -- the initial voltage 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if argument is wrong 





```

#### touch_pad_get_cnt_mode


```


esp_err_t touch_pad_get_cnt_mode(touch_pad_t touch_num, touch_cnt_slope_t *slope, touch_tie_opt_t *opt)
Get touch sensor charge/discharge speed for each pad. 

Parameters

touch_num -- touch pad index 
slope -- pointer to accept touch pad charge/discharge slope 
opt -- pointer to accept the initial voltage 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if argument is wrong 





```

#### touch_pad_isr_deregister


```


esp_err_t touch_pad_isr_deregister(void (*fn)(void*), void *arg)
Deregister the handler previously registered using touch_pad_isr_handler_register. 

Parameters

fn -- handler function to call (as passed to touch_pad_isr_handler_register) 
arg -- argument of the handler (as passed to touch_pad_isr_handler_register) 


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if a handler matching both fn and arg isn't registered 





```

#### touch_pad_get_wakeup_status


```


esp_err_t touch_pad_get_wakeup_status(touch_pad_t *pad_num)
Get the touch pad which caused wakeup from deep sleep. 

Parameters
pad_num -- pointer to touch pad which caused wakeup 

Returns

ESP_OK Success
ESP_ERR_INVALID_ARG parameter is NULL 





```

#### touch_pad_set_fsm_mode


```


esp_err_t touch_pad_set_fsm_mode(touch_fsm_mode_t mode)
Set touch sensor FSM mode, the test action can be triggered by the timer, as well as by the software. 

Parameters
mode -- FSM mode 

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if argument is wrong 





```

#### touch_pad_get_fsm_mode


```


esp_err_t touch_pad_get_fsm_mode(touch_fsm_mode_t *mode)
Get touch sensor FSM mode. 

Parameters
mode -- pointer to accept FSM mode 

Returns

ESP_OK on success 





```

#### touch_pad_clear_status


```


esp_err_t touch_pad_clear_status(void)
To clear the touch sensor channel active status. 

Note
The FSM automatically updates the touch sensor status. It is generally not necessary to call this API to clear the status. 


Returns

ESP_OK on success 





```

#### touch_pad_get_status


```


uint32_t touch_pad_get_status(void)
Get the touch sensor channel active status mask. The bit position represents the channel number. The 0/1 status of the bit represents the trigger status. 

Returns

The touch sensor status. e.g. Touch1 trigger status is status_mask & (BIT1). 





```

#### touch_pad_meas_is_done


```


bool touch_pad_meas_is_done(void)
Check touch sensor measurement status. 

Returns

True measurement is under way
False measurement done 





```



### GPIO Lookup Macros


Some useful macros can be used to specified the GPIO number of a touch pad channel, or vice versa. e.g.,


1. `TOUCH_PAD_NUM5_GPIO_NUM` is the GPIO number of channel 5 (12);
2. `TOUCH_PAD_GPIO4_CHANNEL` is the channel number of GPIO 4 (channel 0).




### Header File


* components/soc/esp32s3/include/soc/touch_sensor_channel.h
* This header file can be included with:



> ```
> #include "soc/touch_sensor_channel.h"
> 
> ```




### Macros


#### TOUCH_PAD_GPIO1_CHANNEL


```


TOUCH_PAD_GPIO1_CHANNEL

```

#### TOUCH_PAD_NUM1_GPIO_NUM


```


TOUCH_PAD_NUM1_GPIO_NUM

```

#### TOUCH_PAD_GPIO2_CHANNEL


```


TOUCH_PAD_GPIO2_CHANNEL

```

#### TOUCH_PAD_NUM2_GPIO_NUM


```


TOUCH_PAD_NUM2_GPIO_NUM

```

#### TOUCH_PAD_GPIO3_CHANNEL


```


TOUCH_PAD_GPIO3_CHANNEL

```

#### TOUCH_PAD_NUM3_GPIO_NUM


```


TOUCH_PAD_NUM3_GPIO_NUM

```

#### TOUCH_PAD_GPIO4_CHANNEL


```


TOUCH_PAD_GPIO4_CHANNEL

```

#### TOUCH_PAD_NUM4_GPIO_NUM


```


TOUCH_PAD_NUM4_GPIO_NUM

```

#### TOUCH_PAD_GPIO5_CHANNEL


```


TOUCH_PAD_GPIO5_CHANNEL

```

#### TOUCH_PAD_NUM5_GPIO_NUM


```


TOUCH_PAD_NUM5_GPIO_NUM

```

#### TOUCH_PAD_GPIO6_CHANNEL


```


TOUCH_PAD_GPIO6_CHANNEL

```

#### TOUCH_PAD_NUM6_GPIO_NUM


```


TOUCH_PAD_NUM6_GPIO_NUM

```

#### TOUCH_PAD_GPIO7_CHANNEL


```


TOUCH_PAD_GPIO7_CHANNEL

```

#### TOUCH_PAD_NUM7_GPIO_NUM


```


TOUCH_PAD_NUM7_GPIO_NUM

```

#### TOUCH_PAD_GPIO8_CHANNEL


```


TOUCH_PAD_GPIO8_CHANNEL

```

#### TOUCH_PAD_NUM8_GPIO_NUM


```


TOUCH_PAD_NUM8_GPIO_NUM

```

#### TOUCH_PAD_GPIO9_CHANNEL


```


TOUCH_PAD_GPIO9_CHANNEL

```

#### TOUCH_PAD_NUM9_GPIO_NUM


```


TOUCH_PAD_NUM9_GPIO_NUM

```

#### TOUCH_PAD_GPIO10_CHANNEL


```


TOUCH_PAD_GPIO10_CHANNEL

```

#### TOUCH_PAD_NUM10_GPIO_NUM


```


TOUCH_PAD_NUM10_GPIO_NUM

```

#### TOUCH_PAD_GPIO11_CHANNEL


```


TOUCH_PAD_GPIO11_CHANNEL

```

#### TOUCH_PAD_NUM11_GPIO_NUM


```


TOUCH_PAD_NUM11_GPIO_NUM

```

#### TOUCH_PAD_GPIO12_CHANNEL


```


TOUCH_PAD_GPIO12_CHANNEL

```

#### TOUCH_PAD_NUM12_GPIO_NUM


```


TOUCH_PAD_NUM12_GPIO_NUM

```

#### TOUCH_PAD_GPIO13_CHANNEL


```


TOUCH_PAD_GPIO13_CHANNEL

```

#### TOUCH_PAD_NUM13_GPIO_NUM


```


TOUCH_PAD_NUM13_GPIO_NUM

```

#### TOUCH_PAD_GPIO14_CHANNEL


```


TOUCH_PAD_GPIO14_CHANNEL

```

#### TOUCH_PAD_NUM14_GPIO_NUM


```


TOUCH_PAD_NUM14_GPIO_NUM

```



### Header File


* components/hal/include/hal/touch_sensor_types.h
* This header file can be included with:



> ```
> #include "hal/touch_sensor_types.h"
> 
> ```




### Structures


#### touch_pad_denoise


```


struct touch_pad_denoise
Touch sensor denoise configuration 

Public Members
grade

touch_pad_denoise_grade_t grade
Select denoise range of denoise channel. Determined by measuring the noise amplitude of the denoise channel. 

cap_level

touch_pad_denoise_cap_t cap_level
Select internal reference capacitance of denoise channel. Ensure that the denoise readings are closest to the readings of the channel being measured. Use touch_pad_denoise_read_data to get the reading of denoise channel. The equivalent capacitance of the shielded channel can be calculated from the reading of denoise channel. 



```

#### touch_pad_waterproof


```


struct touch_pad_waterproof
Touch sensor waterproof configuration 

Public Members
guard_ring_pad

touch_pad_t guard_ring_pad
Waterproof. Select touch channel use for guard pad. Guard pad is used to detect the large area of water covering the touch panel. 

shield_driver

touch_pad_shield_driver_t shield_driver
Waterproof. Shield channel drive capability configuration. Shield pad is used to shield the influence of water droplets covering the touch panel. When the waterproof function is enabled, Touch14 is set as shield channel by default. The larger the parasitic capacitance on the shielding channel, the higher the drive capability needs to be set. The equivalent capacitance of the shield channel can be estimated through the reading value of the denoise channel(Touch0). 



```

#### touch_filter_config


```


struct touch_filter_config
Touch sensor filter configuration 

Public Members
mode

touch_filter_mode_t mode
Set filter mode. The input of the filter is the raw value of touch reading, and the output of the filter is involved in the judgment of the touch state. 

debounce_cnt

uint32_t debounce_cnt
Set debounce count, such as n. If the measured values continue to exceed the threshold for n+1 times, the touch sensor state changes. Range: 0 ~ 7 

noise_thr

uint32_t noise_thr
Noise threshold coefficient. Higher = More noise resistance. The actual noise should be less than (noise coefficient * touch threshold). Range: 0 ~ 3. The coefficient is 0: 4/8; 1: 3/8; 2: 2/8; 3: 1; 

jitter_step

uint32_t jitter_step
Set jitter filter step size. Range: 0 ~ 15 

smh_lvl

touch_smooth_mode_t smh_lvl
Level of filter applied on the original data against large noise interference. 



```

#### touch_pad_sleep_channel_t


```


struct touch_pad_sleep_channel_t
Touch sensor channel sleep configuration 

Public Members
touch_num

touch_pad_t touch_num
Set touch channel number for sleep pad. Only one touch sensor channel is supported in deep sleep mode. If clear the sleep channel, point this pad to TOUCH_PAD_NUM0

en_proximity

bool en_proximity
enable proximity function for sleep pad 



```



### Macros


#### TOUCH_PAD_BIT_MASK_ALL


```


TOUCH_PAD_BIT_MASK_ALL

```

#### TOUCH_PAD_SLOPE_DEFAULT


```


TOUCH_PAD_SLOPE_DEFAULT

```

#### TOUCH_PAD_TIE_OPT_DEFAULT


```


TOUCH_PAD_TIE_OPT_DEFAULT

```

#### TOUCH_PAD_BIT_MASK_MAX


```


TOUCH_PAD_BIT_MASK_MAX

```

#### TOUCH_PAD_HIGH_VOLTAGE_THRESHOLD


```


TOUCH_PAD_HIGH_VOLTAGE_THRESHOLD

```

#### TOUCH_PAD_LOW_VOLTAGE_THRESHOLD


```


TOUCH_PAD_LOW_VOLTAGE_THRESHOLD

```

#### TOUCH_PAD_ATTEN_VOLTAGE_THRESHOLD


```


TOUCH_PAD_ATTEN_VOLTAGE_THRESHOLD

```

#### TOUCH_PAD_IDLE_CH_CONNECT_DEFAULT


```


TOUCH_PAD_IDLE_CH_CONNECT_DEFAULT

```

#### TOUCH_PAD_THRESHOLD_MAX


```


TOUCH_PAD_THRESHOLD_MAX
If set touch threshold max value, The touch sensor can't be in touched status 

```

#### TOUCH_PAD_SLEEP_CYCLE_DEFAULT


```


TOUCH_PAD_SLEEP_CYCLE_DEFAULT
Excessive total time will slow down the touch response. Too small measurement time will not be sampled enough, resulting in inaccurate measurements.

Note
The greater the duty cycle of the measurement time, the more system power is consumed. The number of sleep cycle in each measure process of touch channels. The timer frequency is RTC_SLOW_CLK (can be 150k or 32k depending on the options). Range: 0 ~ 0xffff 


```

#### TOUCH_PAD_MEASURE_CYCLE_DEFAULT


```


TOUCH_PAD_MEASURE_CYCLE_DEFAULT
The times of charge and discharge in each measure process of touch channels. The timer frequency is 8Mhz. Recommended typical value: Modify this value to make the measurement time around 1ms. Range: 0 ~ 0xffff 

```

#### TOUCH_PAD_INTR_MASK_ALL


```


TOUCH_PAD_INTR_MASK_ALL
All touch interrupt type enable. 

```

#### TOUCH_PROXIMITY_MEAS_NUM_MAX


```


TOUCH_PROXIMITY_MEAS_NUM_MAX
Touch sensor proximity detection configuration 

```

#### TOUCH_DEBOUNCE_CNT_MAX


```


TOUCH_DEBOUNCE_CNT_MAX

```

#### TOUCH_NOISE_THR_MAX


```


TOUCH_NOISE_THR_MAX

```

#### TOUCH_JITTER_STEP_MAX


```


TOUCH_JITTER_STEP_MAX

```



### Type Definitions


#### touch_pad_denoise_t


```


typedef struct touch_pad_denoise touch_pad_denoise_t
Touch sensor denoise configuration 

```

#### touch_pad_waterproof_t


```


typedef struct touch_pad_waterproof touch_pad_waterproof_t
Touch sensor waterproof configuration 

```

#### touch_filter_config_t


```


typedef struct touch_filter_config touch_filter_config_t
Touch sensor filter configuration 

```



### Enumerations


#### touch_pad_t


```


enum touch_pad_t
Touch pad channel 
Values:
TOUCH_PAD_NUM0

enumerator TOUCH_PAD_NUM0
Touch pad channel 0 is GPIO4(ESP32) 

TOUCH_PAD_NUM1

enumerator TOUCH_PAD_NUM1
Touch pad channel 1 is GPIO0(ESP32) / GPIO1(ESP32-S2) 

TOUCH_PAD_NUM2

enumerator TOUCH_PAD_NUM2
Touch pad channel 2 is GPIO2(ESP32) / GPIO2(ESP32-S2) 

TOUCH_PAD_NUM3

enumerator TOUCH_PAD_NUM3
Touch pad channel 3 is GPIO15(ESP32) / GPIO3(ESP32-S2) 

TOUCH_PAD_NUM4

enumerator TOUCH_PAD_NUM4
Touch pad channel 4 is GPIO13(ESP32) / GPIO4(ESP32-S2) 

TOUCH_PAD_NUM5

enumerator TOUCH_PAD_NUM5
Touch pad channel 5 is GPIO12(ESP32) / GPIO5(ESP32-S2) 

TOUCH_PAD_NUM6

enumerator TOUCH_PAD_NUM6
Touch pad channel 6 is GPIO14(ESP32) / GPIO6(ESP32-S2) 

TOUCH_PAD_NUM7

enumerator TOUCH_PAD_NUM7
Touch pad channel 7 is GPIO27(ESP32) / GPIO7(ESP32-S2) 

TOUCH_PAD_NUM8

enumerator TOUCH_PAD_NUM8
Touch pad channel 8 is GPIO33(ESP32) / GPIO8(ESP32-S2) 

TOUCH_PAD_NUM9

enumerator TOUCH_PAD_NUM9
Touch pad channel 9 is GPIO32(ESP32) / GPIO9(ESP32-S2) 

TOUCH_PAD_NUM10

enumerator TOUCH_PAD_NUM10
Touch channel 10 is GPIO10(ESP32-S2) 

TOUCH_PAD_NUM11

enumerator TOUCH_PAD_NUM11
Touch channel 11 is GPIO11(ESP32-S2) 

TOUCH_PAD_NUM12

enumerator TOUCH_PAD_NUM12
Touch channel 12 is GPIO12(ESP32-S2) 

TOUCH_PAD_NUM13

enumerator TOUCH_PAD_NUM13
Touch channel 13 is GPIO13(ESP32-S2) 

TOUCH_PAD_NUM14

enumerator TOUCH_PAD_NUM14
Touch channel 14 is GPIO14(ESP32-S2) 

TOUCH_PAD_MAX

enumerator TOUCH_PAD_MAX


```

#### touch_high_volt_t


```


enum touch_high_volt_t
Touch sensor high reference voltage 
Values:
TOUCH_HVOLT_KEEP

enumerator TOUCH_HVOLT_KEEP
Touch sensor high reference voltage, no change 

TOUCH_HVOLT_2V4

enumerator TOUCH_HVOLT_2V4
Touch sensor high reference voltage, 2.4V 

TOUCH_HVOLT_2V5

enumerator TOUCH_HVOLT_2V5
Touch sensor high reference voltage, 2.5V 

TOUCH_HVOLT_2V6

enumerator TOUCH_HVOLT_2V6
Touch sensor high reference voltage, 2.6V 

TOUCH_HVOLT_2V7

enumerator TOUCH_HVOLT_2V7
Touch sensor high reference voltage, 2.7V 

TOUCH_HVOLT_MAX

enumerator TOUCH_HVOLT_MAX


```

#### touch_low_volt_t


```


enum touch_low_volt_t
Touch sensor low reference voltage 
Values:
TOUCH_LVOLT_KEEP

enumerator TOUCH_LVOLT_KEEP
Touch sensor low reference voltage, no change 

TOUCH_LVOLT_0V5

enumerator TOUCH_LVOLT_0V5
Touch sensor low reference voltage, 0.5V 

TOUCH_LVOLT_0V6

enumerator TOUCH_LVOLT_0V6
Touch sensor low reference voltage, 0.6V 

TOUCH_LVOLT_0V7

enumerator TOUCH_LVOLT_0V7
Touch sensor low reference voltage, 0.7V 

TOUCH_LVOLT_0V8

enumerator TOUCH_LVOLT_0V8
Touch sensor low reference voltage, 0.8V 

TOUCH_LVOLT_MAX

enumerator TOUCH_LVOLT_MAX


```

#### touch_volt_atten_t


```


enum touch_volt_atten_t
Touch sensor high reference voltage attenuation 
Values:
TOUCH_HVOLT_ATTEN_KEEP

enumerator TOUCH_HVOLT_ATTEN_KEEP
Touch sensor high reference voltage attenuation, no change 

TOUCH_HVOLT_ATTEN_1V5

enumerator TOUCH_HVOLT_ATTEN_1V5
Touch sensor high reference voltage attenuation, 1.5V attenuation 

TOUCH_HVOLT_ATTEN_1V

enumerator TOUCH_HVOLT_ATTEN_1V
Touch sensor high reference voltage attenuation, 1.0V attenuation 

TOUCH_HVOLT_ATTEN_0V5

enumerator TOUCH_HVOLT_ATTEN_0V5
Touch sensor high reference voltage attenuation, 0.5V attenuation 

TOUCH_HVOLT_ATTEN_0V

enumerator TOUCH_HVOLT_ATTEN_0V
Touch sensor high reference voltage attenuation, 0V attenuation 

TOUCH_HVOLT_ATTEN_MAX

enumerator TOUCH_HVOLT_ATTEN_MAX


```

#### touch_cnt_slope_t


```


enum touch_cnt_slope_t
Touch sensor charge/discharge speed 
Values:
TOUCH_PAD_SLOPE_0

enumerator TOUCH_PAD_SLOPE_0
Touch sensor charge / discharge speed, always zero 

TOUCH_PAD_SLOPE_1

enumerator TOUCH_PAD_SLOPE_1
Touch sensor charge / discharge speed, slowest 

TOUCH_PAD_SLOPE_2

enumerator TOUCH_PAD_SLOPE_2
Touch sensor charge / discharge speed 

TOUCH_PAD_SLOPE_3

enumerator TOUCH_PAD_SLOPE_3
Touch sensor charge / discharge speed 

TOUCH_PAD_SLOPE_4

enumerator TOUCH_PAD_SLOPE_4
Touch sensor charge / discharge speed 

TOUCH_PAD_SLOPE_5

enumerator TOUCH_PAD_SLOPE_5
Touch sensor charge / discharge speed 

TOUCH_PAD_SLOPE_6

enumerator TOUCH_PAD_SLOPE_6
Touch sensor charge / discharge speed 

TOUCH_PAD_SLOPE_7

enumerator TOUCH_PAD_SLOPE_7
Touch sensor charge / discharge speed, fast 

TOUCH_PAD_SLOPE_MAX

enumerator TOUCH_PAD_SLOPE_MAX


```

#### touch_tie_opt_t


```


enum touch_tie_opt_t
Touch sensor initial charge level 
Values:
TOUCH_PAD_TIE_OPT_LOW

enumerator TOUCH_PAD_TIE_OPT_LOW
Initial level of charging voltage, low level 

TOUCH_PAD_TIE_OPT_HIGH

enumerator TOUCH_PAD_TIE_OPT_HIGH
Initial level of charging voltage, high level 

TOUCH_PAD_TIE_OPT_MAX

enumerator TOUCH_PAD_TIE_OPT_MAX


```

#### touch_fsm_mode_t


```


enum touch_fsm_mode_t
Touch sensor FSM mode 
Values:
TOUCH_FSM_MODE_TIMER

enumerator TOUCH_FSM_MODE_TIMER
To start touch FSM by timer 

TOUCH_FSM_MODE_SW

enumerator TOUCH_FSM_MODE_SW
To start touch FSM by software trigger 

TOUCH_FSM_MODE_MAX

enumerator TOUCH_FSM_MODE_MAX


```

#### touch_trigger_mode_t


```


enum touch_trigger_mode_t
Values:
TOUCH_TRIGGER_BELOW

enumerator TOUCH_TRIGGER_BELOW
Touch interrupt will happen if counter value is less than threshold. 

TOUCH_TRIGGER_ABOVE

enumerator TOUCH_TRIGGER_ABOVE
Touch interrupt will happen if counter value is larger than threshold. 

TOUCH_TRIGGER_MAX

enumerator TOUCH_TRIGGER_MAX


```

#### touch_trigger_src_t


```


enum touch_trigger_src_t
Values:
TOUCH_TRIGGER_SOURCE_BOTH

enumerator TOUCH_TRIGGER_SOURCE_BOTH
wakeup interrupt is generated if both SET1 and SET2 are "touched" 

TOUCH_TRIGGER_SOURCE_SET1

enumerator TOUCH_TRIGGER_SOURCE_SET1
wakeup interrupt is generated if SET1 is "touched" 

TOUCH_TRIGGER_SOURCE_MAX

enumerator TOUCH_TRIGGER_SOURCE_MAX


```

#### touch_pad_intr_mask_t


```


enum touch_pad_intr_mask_t
Values:
TOUCH_PAD_INTR_MASK_DONE

enumerator TOUCH_PAD_INTR_MASK_DONE
Measurement done for one of the enabled channels. 

TOUCH_PAD_INTR_MASK_ACTIVE

enumerator TOUCH_PAD_INTR_MASK_ACTIVE
Active for one of the enabled channels. 

TOUCH_PAD_INTR_MASK_INACTIVE

enumerator TOUCH_PAD_INTR_MASK_INACTIVE
Inactive for one of the enabled channels. 

TOUCH_PAD_INTR_MASK_SCAN_DONE

enumerator TOUCH_PAD_INTR_MASK_SCAN_DONE
Measurement done for all the enabled channels. 

TOUCH_PAD_INTR_MASK_TIMEOUT

enumerator TOUCH_PAD_INTR_MASK_TIMEOUT
Timeout for one of the enabled channels. 

TOUCH_PAD_INTR_MASK_PROXI_MEAS_DONE

enumerator TOUCH_PAD_INTR_MASK_PROXI_MEAS_DONE
For proximity sensor, when the number of measurements reaches the set count of measurements, an interrupt will be generated. 


```

#### touch_pad_denoise_grade_t


```


enum touch_pad_denoise_grade_t
Values:
TOUCH_PAD_DENOISE_BIT12

enumerator TOUCH_PAD_DENOISE_BIT12
Denoise range is 12bit 

TOUCH_PAD_DENOISE_BIT10

enumerator TOUCH_PAD_DENOISE_BIT10
Denoise range is 10bit 

TOUCH_PAD_DENOISE_BIT8

enumerator TOUCH_PAD_DENOISE_BIT8
Denoise range is 8bit 

TOUCH_PAD_DENOISE_BIT4

enumerator TOUCH_PAD_DENOISE_BIT4
Denoise range is 4bit 

TOUCH_PAD_DENOISE_MAX

enumerator TOUCH_PAD_DENOISE_MAX


```

#### touch_pad_denoise_cap_t


```


enum touch_pad_denoise_cap_t
Values:
TOUCH_PAD_DENOISE_CAP_L0

enumerator TOUCH_PAD_DENOISE_CAP_L0
Denoise channel internal reference capacitance is 5pf 

TOUCH_PAD_DENOISE_CAP_L1

enumerator TOUCH_PAD_DENOISE_CAP_L1
Denoise channel internal reference capacitance is 6.4pf 

TOUCH_PAD_DENOISE_CAP_L2

enumerator TOUCH_PAD_DENOISE_CAP_L2
Denoise channel internal reference capacitance is 7.8pf 

TOUCH_PAD_DENOISE_CAP_L3

enumerator TOUCH_PAD_DENOISE_CAP_L3
Denoise channel internal reference capacitance is 9.2pf 

TOUCH_PAD_DENOISE_CAP_L4

enumerator TOUCH_PAD_DENOISE_CAP_L4
Denoise channel internal reference capacitance is 10.6pf 

TOUCH_PAD_DENOISE_CAP_L5

enumerator TOUCH_PAD_DENOISE_CAP_L5
Denoise channel internal reference capacitance is 12.0pf 

TOUCH_PAD_DENOISE_CAP_L6

enumerator TOUCH_PAD_DENOISE_CAP_L6
Denoise channel internal reference capacitance is 13.4pf 

TOUCH_PAD_DENOISE_CAP_L7

enumerator TOUCH_PAD_DENOISE_CAP_L7
Denoise channel internal reference capacitance is 14.8pf 

TOUCH_PAD_DENOISE_CAP_MAX

enumerator TOUCH_PAD_DENOISE_CAP_MAX


```

#### touch_pad_shield_driver_t


```


enum touch_pad_shield_driver_t
Touch sensor shield channel drive capability level 
Values:
TOUCH_PAD_SHIELD_DRV_L0

enumerator TOUCH_PAD_SHIELD_DRV_L0
The max equivalent capacitance in shield channel is 40pf 

TOUCH_PAD_SHIELD_DRV_L1

enumerator TOUCH_PAD_SHIELD_DRV_L1
The max equivalent capacitance in shield channel is 80pf 

TOUCH_PAD_SHIELD_DRV_L2

enumerator TOUCH_PAD_SHIELD_DRV_L2
The max equivalent capacitance in shield channel is 120pf 

TOUCH_PAD_SHIELD_DRV_L3

enumerator TOUCH_PAD_SHIELD_DRV_L3
The max equivalent capacitance in shield channel is 160pf 

TOUCH_PAD_SHIELD_DRV_L4

enumerator TOUCH_PAD_SHIELD_DRV_L4
The max equivalent capacitance in shield channel is 200pf 

TOUCH_PAD_SHIELD_DRV_L5

enumerator TOUCH_PAD_SHIELD_DRV_L5
The max equivalent capacitance in shield channel is 240pf 

TOUCH_PAD_SHIELD_DRV_L6

enumerator TOUCH_PAD_SHIELD_DRV_L6
The max equivalent capacitance in shield channel is 280pf 

TOUCH_PAD_SHIELD_DRV_L7

enumerator TOUCH_PAD_SHIELD_DRV_L7
The max equivalent capacitance in shield channel is 320pf 

TOUCH_PAD_SHIELD_DRV_MAX

enumerator TOUCH_PAD_SHIELD_DRV_MAX


```

#### touch_pad_conn_type_t


```


enum touch_pad_conn_type_t
Touch channel idle state configuration 
Values:
TOUCH_PAD_CONN_HIGHZ

enumerator TOUCH_PAD_CONN_HIGHZ
Idle status of touch channel is high resistance state 

TOUCH_PAD_CONN_GND

enumerator TOUCH_PAD_CONN_GND
Idle status of touch channel is ground connection 

TOUCH_PAD_CONN_MAX

enumerator TOUCH_PAD_CONN_MAX


```

#### touch_filter_mode_t


```


enum touch_filter_mode_t
Touch channel IIR filter coefficient configuration. 

Note
On ESP32S2. There is an error in the IIR calculation. The magnitude of the error is twice the filter coefficient. So please select a smaller filter coefficient on the basis of meeting the filtering requirements. Recommended filter coefficient selection IIR_16. 

Values:
TOUCH_PAD_FILTER_IIR_4

enumerator TOUCH_PAD_FILTER_IIR_4
The filter mode is first-order IIR filter. The coefficient is 4. 

TOUCH_PAD_FILTER_IIR_8

enumerator TOUCH_PAD_FILTER_IIR_8
The filter mode is first-order IIR filter. The coefficient is 8. 

TOUCH_PAD_FILTER_IIR_16

enumerator TOUCH_PAD_FILTER_IIR_16
The filter mode is first-order IIR filter. The coefficient is 16 (Typical value). 

TOUCH_PAD_FILTER_IIR_32

enumerator TOUCH_PAD_FILTER_IIR_32
The filter mode is first-order IIR filter. The coefficient is 32. 

TOUCH_PAD_FILTER_IIR_64

enumerator TOUCH_PAD_FILTER_IIR_64
The filter mode is first-order IIR filter. The coefficient is 64. 

TOUCH_PAD_FILTER_IIR_128

enumerator TOUCH_PAD_FILTER_IIR_128
The filter mode is first-order IIR filter. The coefficient is 128. 

TOUCH_PAD_FILTER_IIR_256

enumerator TOUCH_PAD_FILTER_IIR_256
The filter mode is first-order IIR filter. The coefficient is 256. 

TOUCH_PAD_FILTER_JITTER

enumerator TOUCH_PAD_FILTER_JITTER
The filter mode is jitter filter 

TOUCH_PAD_FILTER_MAX

enumerator TOUCH_PAD_FILTER_MAX


```

#### touch_smooth_mode_t


```


enum touch_smooth_mode_t
Level of filter applied on the original data against large noise interference. 

Note
On ESP32S2. There is an error in the IIR calculation. The magnitude of the error is twice the filter coefficient. So please select a smaller filter coefficient on the basis of meeting the filtering requirements. Recommended filter coefficient selection IIR_2. 

Values:
TOUCH_PAD_SMOOTH_OFF

enumerator TOUCH_PAD_SMOOTH_OFF
No filtering of raw data. 

TOUCH_PAD_SMOOTH_IIR_2

enumerator TOUCH_PAD_SMOOTH_IIR_2
Filter the raw data. The coefficient is 2 (Typical value). 

TOUCH_PAD_SMOOTH_IIR_4

enumerator TOUCH_PAD_SMOOTH_IIR_4
Filter the raw data. The coefficient is 4. 

TOUCH_PAD_SMOOTH_IIR_8

enumerator TOUCH_PAD_SMOOTH_IIR_8
Filter the raw data. The coefficient is 8. 

TOUCH_PAD_SMOOTH_MAX

enumerator TOUCH_PAD_SMOOTH_MAX


```