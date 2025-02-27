# Logging library




## Overview


The logging library provides three ways for setting log verbosity:


* **At compile time**: in menuconfig, set the verbosity level using the option CONFIG_LOG_DEFAULT_LEVEL.
* Optionally, also in menuconfig, set the maximum verbosity level using the option CONFIG_LOG_MAXIMUM_LEVEL. By default, this is the same as the default level, but it can be set higher in order to compile more optional logs into the firmware.
* **At runtime**: all logs for verbosity levels lower than CONFIG_LOG_DEFAULT_LEVEL are enabled by default. The function `esp_log_level_set()` can be used to set a logging level on a per-module basis. Modules are identified by their tags, which are human-readable ASCII zero-terminated strings.
* **At runtime**: if CONFIG_LOG_MASTER_LEVEL is enabled then a `Master logging level` can be set using `esp_log_set_level_master()`. This option adds an additional logging level check for all compiled logs. Note that this will increase application size. This feature is useful if you want to compile a lot of logs that are selectable at runtime, but also want to avoid the performance hit from looking up the tags and their log level when you don't want log output.


There are the following verbosity levels:


* Error (lowest)
* Warning
* Info
* Debug
* Verbose (highest)



Note


The function `esp_log_level_set()` cannot set logging levels higher than specified by CONFIG_LOG_MAXIMUM_LEVEL. To increase log level for a specific file above this maximum at compile time, use the macro LOG_LOCAL_LEVEL (see the details below).





## How to Use Logging Library


In each C file that uses logging functionality, define the TAG variable as shown below:



```
static const char* TAG = "MyModule";

```


Then use one of logging macros to produce output, e.g:



```
ESP_LOGW(TAG, "Baud rate error %.1f%%. Requested: %d baud, actual: %d baud", error * 100, baud_req, baud_real);

```


Several macros are available for different verbosity levels:


* `ESP_LOGE` - Error (lowest)
* `ESP_LOGW` - Warning
* `ESP_LOGI` - Info
* `ESP_LOGD` - Debug
* `ESP_LOGV` - Verbose (highest)


Additionally, there are `ESP_EARLY_LOGx` versions for each of these macros, e.g. `ESP_EARLY_LOGE`. These versions have to be used explicitly in the early startup code only, before heap allocator and syscalls have been initialized. Normal `ESP_LOGx` macros can also be used while compiling the bootloader, but they will fall back to the same implementation as `ESP_EARLY_LOGx` macros.


There are also `ESP_DRAM_LOGx` versions for each of these macros, e.g. `ESP_DRAM_LOGE`. These versions are used in some places where logging may occur with interrupts disabled or with flash cache inaccessible. Use of this macros should be as sparse as possible, as logging in these types of code should be avoided for performance reasons.



Note


Inside critical sections interrupts are disabled so it's only possible to use `ESP_DRAM_LOGx` (preferred) or `ESP_EARLY_LOGx`. Even though it's possible to log in these situations, it's better if your program can be structured not to require it.



To override default verbosity level at file or component scope, define the `LOG_LOCAL_LEVEL` macro.


At file scope, define it before including `esp_log.h`, e.g.:



```
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

```


At component scope, define it in the component CMakeLists:



```
target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLOG_LOCAL_LEVEL=ESP_LOG_VERBOSE")

```


To configure logging output per module at runtime, add calls to the function `esp_log_level_set()` as follows:



```
esp_log_level_set("*", ESP_LOG_ERROR);        // set all components to ERROR level
esp_log_level_set("wifi", ESP_LOG_WARN);      // enable WARN logs from WiFi stack
esp_log_level_set("dhcpc", ESP_LOG_INFO);     // enable INFO logs from DHCP client

```



Note


The "DRAM" and "EARLY" log macro variants documented above do not support per module setting of log verbosity. These macros will always log at the "default" verbosity level, which can only be changed at runtime by calling `esp_log_level("*", level)`.



Even when logs are disabled by using a tag name, they will still require a processing time of around 10.9 microseconds per entry.



### Master Logging Level


To enable the Master logging level feature, the CONFIG_LOG_MASTER_LEVEL option must be enabled. It adds an additional level check for `ESP_LOGx` macros before calling `esp_log_write()`. This allows to set a higher CONFIG_LOG_MAXIMUM_LEVEL, but not inflict a performance hit during normal operation (only when directed). An application may set the master logging level (`esp_log_set_level_master()`) globally to enforce a maximum log level. `ESP_LOGx` macros above this level will be skipped immediately, rather than calling `esp_log_write()` and doing a tag lookup. It is recommended to only use this in an top-level application and not in shared components as this would override the global log level for any user using the component. By default, at startup, the Master logging level is CONFIG_LOG_DEFAULT_LEVEL.


Note that this feature increases application size because the additional check is added into all `ESP_LOGx` macros.


The snippet below shows how it works. Setting the Master logging level to `ESP_LOG_NONE` disables all logging globally. `esp_log_level_set()` does not currently affect logging. But after the Master logging level is released, the logs will be printed as set by `esp_log_level_set()`.



```
// Master logging level is CONFIG_LOG_DEFAULT_LEVEL at start up and = ESP_LOG_INFO
ESP_LOGI("lib_name", "Message for print");          // prints a INFO message
esp_log_level_set("lib_name", ESP_LOG_WARN);        // enables WARN logs from lib_name

esp_log_set_level_master(ESP_LOG_NONE);             // disables all logs globally. esp_log_level_set has no effect at the moment

ESP_LOGW("lib_name", "Message for print");          // no print, Master logging level blocks it
esp_log_level_set("lib_name", ESP_LOG_INFO);        // enable INFO logs from lib_name
ESP_LOGI("lib_name", "Message for print");          // no print, Master logging level blocks it

esp_log_set_level_master(ESP_LOG_INFO);             // enables all INFO logs globally

ESP_LOGI("lib_name", "Message for print");          // prints a INFO message

```




### Logging to Host via JTAG


By default, the logging library uses the vprintf-like function to write formatted output to the dedicated UART. By calling a simple API, all log output may be routed to JTAG instead, making logging several times faster. For details, please refer to Section Logging to Host.




### Thread Safety


The log string is first written into a memory buffer and then sent to the UART for printing. Log calls are thread-safe, i.e., logs of different threads do not conflict with each other.





## Application Example


The logging library is commonly used by most ESP-IDF components and examples. For demonstration of log functionality, check ESP-IDF's examples directory. The most relevant examples that deal with logging are the following:


* system/ota
* storage/sd_card
* protocols/https_request




## API Reference



### Header File


* components/log/include/esp_log.h
* This header file can be included with:



> ```
> #include "esp_log.h"
> 
> ```




### Functions


#### esp_log_set_level_master


```


void esp_log_set_level_master(esp_log_level_t level)
Master log level. 
Optional master log level to check against for ESP_LOGx macros before calling esp_log_write. Allows one to set a higher CONFIG_LOG_MAXIMUM_LEVEL but not impose a performance hit during normal operation (only when instructed). An application may set esp_log_set_level_master(level) to globally enforce a maximum log level. ESP_LOGx macros above this level will be skipped immediately, rather than calling esp_log_write and doing a cache hit.
The tradeoff is increased application size.

Parameters
level -- Master log level 



```

#### esp_log_get_level_master


```


esp_log_level_t esp_log_get_level_master(void)
Returns master log level. 

Returns
Master log level 



```

#### esp_log_level_set


```


void esp_log_level_set(const char *tag, esp_log_level_t level)
Set log level for given tag. 
If logging for given component has already been enabled, changes previous setting.

Note
Note that this function can not raise log level above the level set using CONFIG_LOG_MAXIMUM_LEVEL setting in menuconfig. To raise log level above the default one for a given file, define LOG_LOCAL_LEVEL to one of the ESP_LOG_* values, before including esp_log.h in this file.


Parameters

tag -- Tag of the log entries to enable. Must be a non-NULL zero terminated string. Value "*" resets log level for all tags to the given value.
level -- Selects log level to enable. Only logs at this and lower verbosity levels will be shown. 




```

#### esp_log_level_get


```


esp_log_level_t esp_log_level_get(const char *tag)
Get log level for a given tag, can be used to avoid expensive log statements. 

Parameters
tag -- Tag of the log to query current level. Must be a non-NULL zero terminated string.

Returns
The current log level for the given tag 



```

#### esp_log_set_vprintf


```


vprintf_like_t esp_log_set_vprintf(vprintf_like_t func)
Set function used to output log entries. 
By default, log output goes to UART0. This function can be used to redirect log output to some other destination, such as file or network. Returns the original log handler, which may be necessary to return output to the previous destination.

Note
Please note that function callback here must be re-entrant as it can be invoked in parallel from multiple thread context.


Parameters
func -- new Function used for output. Must have same signature as vprintf.

Returns
func old Function used for output. 



```

#### esp_log_timestamp


```


uint32_t esp_log_timestamp(void)
Function which returns timestamp to be used in log output. 
This function is used in expansion of ESP_LOGx macros. In the 2nd stage bootloader, and at early application startup stage this function uses CPU cycle counter as time source. Later when FreeRTOS scheduler start running, it switches to FreeRTOS tick count.
For now, we ignore millisecond counter overflow.

Returns
timestamp, in milliseconds 



```

#### esp_log_system_timestamp


```


char *esp_log_system_timestamp(void)
Function which returns system timestamp to be used in log output. 
This function is used in expansion of ESP_LOGx macros to print the system time as "HH:MM:SS.sss". The system time is initialized to 0 on startup, this can be set to the correct time with an SNTP sync, or manually with standard POSIX time functions.
Currently, this will not get used in logging from binary blobs (i.e. Wi-Fi & Bluetooth libraries), these will still print the RTOS tick time.

Returns
timestamp, in "HH:MM:SS.sss" 



```

#### esp_log_early_timestamp


```


uint32_t esp_log_early_timestamp(void)
Function which returns timestamp to be used in log output. 
This function uses HW cycle counter and does not depend on OS, so it can be safely used after application crash.

Returns
timestamp, in milliseconds 



```

#### esp_log_write


```


void esp_log_write(esp_log_level_t level, const char *tag, const char *format, ...)
Write message into the log. 
This function is not intended to be used directly. Instead, use one of ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV macros.
This function or these macros should not be used from an interrupt. 

```

#### esp_log_writev


```


void esp_log_writev(esp_log_level_t level, const char *tag, const char *format, va_list args)
Write message into the log, va_list variant. 

This function is provided to ease integration toward other logging framework, so that esp_log can be used as a log sink. 
See also
esp_log_write()



```



### Macros


#### ESP_LOG_BUFFER_HEX_LEVEL


```


ESP_LOG_BUFFER_HEX_LEVEL(tag, buffer, buff_len, level)
Log a buffer of hex bytes at specified level, separated into 16 bytes each line. 

Parameters

tag -- description tag 
buffer -- Pointer to the buffer array 
buff_len -- length of buffer in bytes 
level -- level of the log 




```

#### ESP_LOG_BUFFER_CHAR_LEVEL


```


ESP_LOG_BUFFER_CHAR_LEVEL(tag, buffer, buff_len, level)
Log a buffer of characters at specified level, separated into 16 bytes each line. Buffer should contain only printable characters. 

Parameters

tag -- description tag 
buffer -- Pointer to the buffer array 
buff_len -- length of buffer in bytes 
level -- level of the log 




```

#### ESP_LOG_BUFFER_HEXDUMP


```


ESP_LOG_BUFFER_HEXDUMP(tag, buffer, buff_len, level)
Dump a buffer to the log at specified level. 
The dump log shows just like the one below:  W (195) log_example: 0x3ffb4280   45 53 50 33 32 20 69 73  20 67 72 65 61 74 2c 20  |ESP32 is great, |
 W (195) log_example: 0x3ffb4290   77 6f 72 6b 69 6e 67 20  61 6c 6f 6e 67 20 77 69  |working along wi|
 W (205) log_example: 0x3ffb42a0   74 68 20 74 68 65 20 49  44 46 2e 00              |th the IDF..|


 It is highly recommended to use terminals with over 102 text width.

Parameters

tag -- description tag 
buffer -- Pointer to the buffer array 
buff_len -- length of buffer in bytes 
level -- level of the log 




```

#### ESP_LOG_BUFFER_HEX


```


ESP_LOG_BUFFER_HEX(tag, buffer, buff_len)
Log a buffer of hex bytes at Info level. 

See also
esp_log_buffer_hex_level



Parameters

tag -- description tag 
buffer -- Pointer to the buffer array 
buff_len -- length of buffer in bytes




```

#### ESP_LOG_BUFFER_CHAR


```


ESP_LOG_BUFFER_CHAR(tag, buffer, buff_len)
Log a buffer of characters at Info level. Buffer should contain only printable characters. 

See also
esp_log_buffer_char_level



Parameters

tag -- description tag 
buffer -- Pointer to the buffer array 
buff_len -- length of buffer in bytes




```

#### ESP_EARLY_LOGE


```


ESP_EARLY_LOGE(tag, format, ...)
macro to output logs in startup code, before heap allocator and syscalls have been initialized. Log at ESP_LOG_ERROR level. 
See also
printf,ESP_LOGE,ESP_DRAM_LOGE In the future, we want to become compatible with clang. Hence, we provide two versions of the following macros which are using variadic arguments. The first one is using the GNU extension ##__VA_ARGS__. The second one is using the C++20 feature VA_OPT(,). This allows users to compile their code with standard C++20 enabled instead of the GNU extension. Below C++20, we haven't found any good alternative to using ##__VA_ARGS__. 



```

#### ESP_EARLY_LOGW


```


ESP_EARLY_LOGW(tag, format, ...)
macro to output logs in startup code at ESP_LOG_WARN level. 

See also
ESP_EARLY_LOGE,ESP_LOGE, printf



```

#### ESP_EARLY_LOGI


```


ESP_EARLY_LOGI(tag, format, ...)
macro to output logs in startup code at ESP_LOG_INFO level. 

See also
ESP_EARLY_LOGE,ESP_LOGE, printf



```

#### ESP_EARLY_LOGD


```


ESP_EARLY_LOGD(tag, format, ...)
macro to output logs in startup code at ESP_LOG_DEBUG level. 

See also
ESP_EARLY_LOGE,ESP_LOGE, printf



```

#### ESP_EARLY_LOGV


```


ESP_EARLY_LOGV(tag, format, ...)
macro to output logs in startup code at ESP_LOG_VERBOSE level. 

See also
ESP_EARLY_LOGE,ESP_LOGE, printf



```

#### _ESP_LOG_EARLY_ENABLED


```


_ESP_LOG_EARLY_ENABLED(log_level)

```

#### ESP_LOG_EARLY_IMPL


```


ESP_LOG_EARLY_IMPL(tag, format, log_level, log_tag_letter, ...)

```

#### ESP_LOGE


```


ESP_LOGE(tag, format, ...)

```

#### ESP_LOGW


```


ESP_LOGW(tag, format, ...)

```

#### ESP_LOGI


```


ESP_LOGI(tag, format, ...)

```

#### ESP_LOGD


```


ESP_LOGD(tag, format, ...)

```

#### ESP_LOGV


```


ESP_LOGV(tag, format, ...)

```

#### ESP_LOG_LEVEL


```


ESP_LOG_LEVEL(level, tag, format, ...)
runtime macro to output logs at a specified level.

See also
printf



Parameters

tag -- tag of the log, which can be used to change the log level by esp_log_level_set at runtime. 
level -- level of the output log. 
format -- format of the output log. See printf
... -- variables to be replaced into the log. See printf




```

#### ESP_LOG_LEVEL_LOCAL


```


ESP_LOG_LEVEL_LOCAL(level, tag, format, ...)
runtime macro to output logs at a specified level. Also check the level with LOG_LOCAL_LEVEL. If CONFIG_LOG_MASTER_LEVEL set, also check first against esp_log_get_level_master().

See also
printf, ESP_LOG_LEVEL



```

#### ESP_DRAM_LOGE


```


ESP_DRAM_LOGE(tag, format, ...)
Macro to output logs when the cache is disabled. Log at ESP_LOG_ERROR level. 
Similar to
Usage:
ESP_DRAM_LOGE(DRAM_STR("my_tag"), "format", orESP_DRAM_LOGE(TAG, "format", ...)`, where TAG is a char* that points to a str in the DRAM.
See also
ESP_EARLY_LOGE, the log level cannot be changed per-tag, however esp_log_level_set("*", level) will set the default level which controls these log lines also.



See also
esp_rom_printf,ESP_LOGE



Note
Unlike normal logging macros, it's possible to use this macro when interrupts are disabled or inside an ISR.


Note
Placing log strings in DRAM reduces available DRAM, so only use when absolutely essential.


```

#### ESP_DRAM_LOGW


```


ESP_DRAM_LOGW(tag, format, ...)
macro to output logs when the cache is disabled at ESP_LOG_WARN level. 

See also
ESP_DRAM_LOGW,ESP_LOGW, esp_rom_printf



```

#### ESP_DRAM_LOGI


```


ESP_DRAM_LOGI(tag, format, ...)
macro to output logs when the cache is disabled at ESP_LOG_INFO level. 

See also
ESP_DRAM_LOGI,ESP_LOGI, esp_rom_printf



```

#### ESP_DRAM_LOGD


```


ESP_DRAM_LOGD(tag, format, ...)
macro to output logs when the cache is disabled at ESP_LOG_DEBUG level. 

See also
ESP_DRAM_LOGD,ESP_LOGD, esp_rom_printf



```

#### ESP_DRAM_LOGV


```


ESP_DRAM_LOGV(tag, format, ...)
macro to output logs when the cache is disabled at ESP_LOG_VERBOSE level. 

See also
ESP_DRAM_LOGV,ESP_LOGV, esp_rom_printf



```



### Type Definitions


#### vprintf_like_t


```


typedef int (*vprintf_like_t)(const char*, va_list)

```



### Enumerations


#### esp_log_level_t


```


enum esp_log_level_t
Log level. 
Values:
ESP_LOG_NONE

enumerator ESP_LOG_NONE
No log output 

ESP_LOG_ERROR

enumerator ESP_LOG_ERROR
Critical errors, software module can not recover on its own 

ESP_LOG_WARN

enumerator ESP_LOG_WARN
Error conditions from which recovery measures have been taken 

ESP_LOG_INFO

enumerator ESP_LOG_INFO
Information messages which describe normal flow of events 

ESP_LOG_DEBUG

enumerator ESP_LOG_DEBUG
Extra information which is not necessary for normal use (values, pointers, sizes, etc). 

ESP_LOG_VERBOSE

enumerator ESP_LOG_VERBOSE
Bigger chunks of debugging information, or frequent messages which can potentially flood the output. 


```