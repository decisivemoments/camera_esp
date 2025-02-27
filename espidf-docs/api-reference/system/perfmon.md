# Performance Monitor



The Performance Monitor component provides APIs to use ESP32-S3 internal performance counters to profile functions and applications.



## Application Example


An example which combines performance monitor is provided in `examples/system/perfmon` directory. This example initializes the performance monitor structure and execute them with printing the statistics.




## High-Level API Reference



### Header Files


* perfmon/include/perfmon.h





## API Reference



### Header File


* components/perfmon/include/xtensa_perfmon_access.h
* This header file can be included with:



> ```
> #include "xtensa_perfmon_access.h"
> 
> ```
* This header file is a part of the API provided by the `perfmon` component. To declare that your component depends on `perfmon`, add the following to your CMakeLists.txt:



> ```
> REQUIRES perfmon
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES perfmon
> 
> ```




### Functions


#### xtensa_perfmon_init


```


esp_err_t xtensa_perfmon_init(int id, uint16_t select, uint16_t mask, int kernelcnt, int tracelevel)
Init Performance Monitoor. 
Initialize performance monitor register with define values

Parameters

id -- [in] performance counter number 
select -- [in] select value from PMCTRLx register 
mask -- [in] mask value from PMCTRLx register 
kernelcnt -- [in] kernelcnt value from PMCTRLx register 
tracelevel -- [in] tracelevel value from PMCTRLx register


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if one of the arguments is not correct 





```

#### xtensa_perfmon_reset


```


esp_err_t xtensa_perfmon_reset(int id)
Reset PM counter. 
Reset PM counter. Writes 0 to the PMx register.

Parameters
id -- [in] performance counter number

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if id out of range 





```

#### xtensa_perfmon_start


```


void xtensa_perfmon_start(void)
Start PM counters. 
Start all PM counters synchronously. Write 1 to the PGM register 

```

#### xtensa_perfmon_stop


```


void xtensa_perfmon_stop(void)
Stop PM counters. 
Stop all PM counters synchronously. Write 0 to the PGM register 

```

#### xtensa_perfmon_value


```


uint32_t xtensa_perfmon_value(int id)
Read PM counter. 
Read value of defined PM counter.

Parameters
id -- [in] performance counter number

Returns

Performance counter value 





```

#### xtensa_perfmon_overflow


```


esp_err_t xtensa_perfmon_overflow(int id)
Read PM overflow state. 
Read overflow value of defined PM counter.

Parameters
id -- [in] performance counter number

Returns

ESP_OK if there is no overflow (overflow = 0)
ESP_FAIL if overflow occure (overflow = 1) 





```

#### xtensa_perfmon_dump


```


void xtensa_perfmon_dump(void)
Dump PM values. 
Dump all PM register to the console. 

```



### Header File


* components/perfmon/include/xtensa_perfmon_apis.h
* This header file can be included with:



> ```
> #include "xtensa_perfmon_apis.h"
> 
> ```
* This header file is a part of the API provided by the `perfmon` component. To declare that your component depends on `perfmon`, add the following to your CMakeLists.txt:



> ```
> REQUIRES perfmon
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES perfmon
> 
> ```




### Functions


#### xtensa_perfmon_exec


```


esp_err_t xtensa_perfmon_exec(const xtensa_perfmon_config_t *config)
Execute PM. 
Execute performance counter for dedicated function with defined parameters

Parameters
config -- [in] pointer to the configuration structure

Returns

ESP_OK if no errors
ESP_ERR_INVALID_ARG if one of the required parameters not defined
ESP_FAIL - counter overflow 





```

#### xtensa_perfmon_view_cb


```


void xtensa_perfmon_view_cb(void *params, uint32_t select, uint32_t mask, uint32_t value)
Dump PM results. 
Callback to dump perfmon result to a FILE* stream specified in perfmon_config_t::callback_params. If callback_params is set to NULL, will print to stdout

Parameters

params -- [in] used parameters passed from configuration (callback_params). This parameter expected as FILE* hanle, where data will be stored. If this parameter NULL, then data will be stored to the stdout. 
select -- [in] select value for current counter 
mask -- [in] mask value for current counter 
value -- [in] counter value for current counter 




```



### Structures


#### xtensa_perfmon_config


```


struct xtensa_perfmon_config
Performance monitor configuration structure. 
Structure to configure performance counter to measure dedicated function 

Public Members
repeat_count

int repeat_count
how much times function will be called before the calback will be repeated 

max_deviation

float max_deviation
Difference between min and max counter number 0..1, 0 - no difference, 1 - not used 

call_params

void *call_params
This pointer will be passed to the call_function as a parameter 

call_function

void (*call_function)(void *params)
pointer to the function that have to be called 

callback

void (*callback)(void *params, uint32_t select, uint32_t mask, uint32_t value)
pointer to the function that will be called with result parameters 

callback_params

void *callback_params
parameter that will be passed to the callback 

tracelevel

int tracelevel
trace level for all counters. In case of negative value, the filter will be ignored. If it's >=0, then the perfmon will count only when interrupt level > tracelevel. It's useful to monitor interrupts. 

counters_size

uint32_t counters_size
amount of counter in the list 

select_mask

const uint32_t *select_mask
list of the select/mask parameters 



```



### Type Definitions


#### xtensa_perfmon_config_t


```


typedef struct xtensa_perfmon_config xtensa_perfmon_config_t
Performance monitor configuration structure. 
Structure to configure performance counter to measure dedicated function 

```