# Application Level Tracing




## Overview


ESP-IDF provides a useful feature for application behavior analysis called **Application Level Tracing**. The feature can be enabled in menuconfig and allows transfer of arbitrary data between the host and ESP32-S3 via JTAG interface with minimal overhead on program execution.


Developers can use this library to send application specific state of execution to the host, and receive commands or other types of information in the opposite direction at runtime. The main use cases of this library are:


1. Collecting application specific data, see Application Specific Tracing.
2. Lightweight logging to the host, see Logging to Host.
3. System behaviour analysis, see System Behavior Analysis with SEGGER SystemView.




## API Reference



### Header File


* components/app_trace/include/esp_app_trace.h
* This header file can be included with:



> ```
> #include "esp_app_trace.h"
> 
> ```
* This header file is a part of the API provided by the `app_trace` component. To declare that your component depends on `app_trace`, add the following to your CMakeLists.txt:



> ```
> REQUIRES app_trace
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES app_trace
> 
> ```




### Functions


#### esp_apptrace_init


```


esp_err_t esp_apptrace_init(void)
Initializes application tracing module. 

Note
Should be called before any esp_apptrace_xxx call.


Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_down_buffer_config


```


void esp_apptrace_down_buffer_config(uint8_t *buf, uint32_t size)
Configures down buffer. 

Note
Needs to be called before attempting to receive any data using esp_apptrace_down_buffer_get and esp_apptrace_read. This function does not protect internal data by lock.


Parameters

buf -- Address of buffer to use for down channel (host to target) data. 
size -- Size of the buffer. 




```

#### esp_apptrace_buffer_get


```


uint8_t *esp_apptrace_buffer_get(esp_apptrace_dest_t dest, uint32_t size, uint32_t tmo)
Allocates buffer for trace data. Once the data in the buffer is ready to be sent, esp_apptrace_buffer_put must be called to indicate it. 

Parameters

dest -- Indicates HW interface to send data. 
size -- Size of data to write to trace buffer. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
non-NULL on success, otherwise NULL. 



```

#### esp_apptrace_buffer_put


```


esp_err_t esp_apptrace_buffer_put(esp_apptrace_dest_t dest, uint8_t *ptr, uint32_t tmo)
Indicates that the data in the buffer is ready to be sent. This function is a counterpart of and must be preceded by esp_apptrace_buffer_get. 

Parameters

dest -- Indicates HW interface to send data. Should be identical to the same parameter in call to esp_apptrace_buffer_get. 
ptr -- Address of trace buffer to release. Should be the value returned by call to esp_apptrace_buffer_get. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_write


```


esp_err_t esp_apptrace_write(esp_apptrace_dest_t dest, const void *data, uint32_t size, uint32_t tmo)
Writes data to trace buffer. 

Parameters

dest -- Indicates HW interface to send data. 
data -- Address of data to write to trace buffer. 
size -- Size of data to write to trace buffer. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_vprintf_to


```


int esp_apptrace_vprintf_to(esp_apptrace_dest_t dest, uint32_t tmo, const char *fmt, va_list ap)
vprintf-like function to send log messages to host via specified HW interface. 

Parameters

dest -- Indicates HW interface to send data. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely. 
fmt -- Address of format string. 
ap -- List of arguments.


Returns
Number of bytes written. 



```

#### esp_apptrace_vprintf


```


int esp_apptrace_vprintf(const char *fmt, va_list ap)
vprintf-like function to send log messages to host. 

Parameters

fmt -- Address of format string. 
ap -- List of arguments.


Returns
Number of bytes written. 



```

#### esp_apptrace_flush


```


esp_err_t esp_apptrace_flush(esp_apptrace_dest_t dest, uint32_t tmo)
Flushes remaining data in trace buffer to host. 

Parameters

dest -- Indicates HW interface to flush data on. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_flush_nolock


```


esp_err_t esp_apptrace_flush_nolock(esp_apptrace_dest_t dest, uint32_t min_sz, uint32_t tmo)
Flushes remaining data in trace buffer to host without locking internal data. This is a special version of esp_apptrace_flush which should be called from panic handler. 

Parameters

dest -- Indicates HW interface to flush data on. 
min_sz -- Threshold for flushing data. If current filling level is above this value, data will be flushed. TRAX destinations only. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_read


```


esp_err_t esp_apptrace_read(esp_apptrace_dest_t dest, void *data, uint32_t *size, uint32_t tmo)
Reads host data from trace buffer. 

Parameters

dest -- Indicates HW interface to read the data on. 
data -- Address of buffer to put data from trace buffer. 
size -- Pointer to store size of read data. Before call to this function pointed memory must hold requested size of data 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_down_buffer_get


```


uint8_t *esp_apptrace_down_buffer_get(esp_apptrace_dest_t dest, uint32_t *size, uint32_t tmo)
Retrieves incoming data buffer if any. Once data in the buffer is processed, esp_apptrace_down_buffer_put must be called to indicate it. 

Parameters

dest -- Indicates HW interface to receive data. 
size -- Address to store size of available data in down buffer. Must be initialized with requested value. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
non-NULL on success, otherwise NULL. 



```

#### esp_apptrace_down_buffer_put


```


esp_err_t esp_apptrace_down_buffer_put(esp_apptrace_dest_t dest, uint8_t *ptr, uint32_t tmo)
Indicates that the data in the down buffer is processed. This function is a counterpart of and must be preceded by esp_apptrace_down_buffer_get. 

Parameters

dest -- Indicates HW interface to receive data. Should be identical to the same parameter in call to esp_apptrace_down_buffer_get. 
ptr -- Address of trace buffer to release. Should be the value returned by call to esp_apptrace_down_buffer_get. 
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinitely.


Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_host_is_connected


```


bool esp_apptrace_host_is_connected(esp_apptrace_dest_t dest)
Checks whether host is connected. 

Parameters
dest -- Indicates HW interface to use.

Returns
true if host is connected, otherwise false 



```

#### esp_apptrace_fopen


```


void *esp_apptrace_fopen(esp_apptrace_dest_t dest, const char *path, const char *mode)
Opens file on host. This function has the same semantic as 'fopen' except for the first argument. 

Parameters

dest -- Indicates HW interface to use. 
path -- Path to file. 
mode -- Mode string. See fopen for details.


Returns
non zero file handle on success, otherwise 0 



```

#### esp_apptrace_fclose


```


int esp_apptrace_fclose(esp_apptrace_dest_t dest, void *stream)
Closes file on host. This function has the same semantic as 'fclose' except for the first argument. 

Parameters

dest -- Indicates HW interface to use. 
stream -- File handle returned by esp_apptrace_fopen.


Returns
Zero on success, otherwise non-zero. See fclose for details. 



```

#### esp_apptrace_fwrite


```


size_t esp_apptrace_fwrite(esp_apptrace_dest_t dest, const void *ptr, size_t size, size_t nmemb, void *stream)
Writes to file on host. This function has the same semantic as 'fwrite' except for the first argument. 

Parameters

dest -- Indicates HW interface to use. 
ptr -- Address of data to write. 
size -- Size of an item. 
nmemb -- Number of items to write. 
stream -- File handle returned by esp_apptrace_fopen.


Returns
Number of written items. See fwrite for details. 



```

#### esp_apptrace_fread


```


size_t esp_apptrace_fread(esp_apptrace_dest_t dest, void *ptr, size_t size, size_t nmemb, void *stream)
Read file on host. This function has the same semantic as 'fread' except for the first argument. 

Parameters

dest -- Indicates HW interface to use. 
ptr -- Address to store read data. 
size -- Size of an item. 
nmemb -- Number of items to read. 
stream -- File handle returned by esp_apptrace_fopen.


Returns
Number of read items. See fread for details. 



```

#### esp_apptrace_fseek


```


int esp_apptrace_fseek(esp_apptrace_dest_t dest, void *stream, long offset, int whence)
Set position indicator in file on host. This function has the same semantic as 'fseek' except for the first argument. 

Parameters

dest -- Indicates HW interface to use. 
stream -- File handle returned by esp_apptrace_fopen. 
offset -- Offset. See fseek for details. 
whence -- Position in file. See fseek for details.


Returns
Zero on success, otherwise non-zero. See fseek for details. 



```

#### esp_apptrace_ftell


```


int esp_apptrace_ftell(esp_apptrace_dest_t dest, void *stream)
Get current position indicator for file on host. This function has the same semantic as 'ftell' except for the first argument. 

Parameters

dest -- Indicates HW interface to use. 
stream -- File handle returned by esp_apptrace_fopen.


Returns
Current position in file. See ftell for details. 



```

#### esp_apptrace_fstop


```


int esp_apptrace_fstop(esp_apptrace_dest_t dest)
Indicates to the host that all file operations are complete. This function should be called after all file operations are finished and indicate to the host that it can perform cleanup operations (close open files etc.). 

Parameters
dest -- Indicates HW interface to use.

Returns
ESP_OK on success, otherwise see esp_err_t 



```

#### esp_apptrace_feof


```


int esp_apptrace_feof(esp_apptrace_dest_t dest, void *stream)
Test end-of-file indicator on a stream. This function has the same semantic as 'feof' except for the first argument. 

Parameters

dest -- Indicates HW interface to use. 
stream -- File handle returned by esp_apptrace_fopen.


Returns
Non-Zero if end-of-file indicator is set for stream. See feof for details. 



```

#### esp_gcov_dump


```


void esp_gcov_dump(void)
Triggers gcov info dump. This function waits for the host to connect to target before dumping data. 

```



### Enumerations


#### esp_apptrace_dest_t


```


enum esp_apptrace_dest_t
Application trace data destinations bits. 
Values:
ESP_APPTRACE_DEST_JTAG

enumerator ESP_APPTRACE_DEST_JTAG
JTAG destination. 

ESP_APPTRACE_DEST_TRAX

enumerator ESP_APPTRACE_DEST_TRAX
xxx_TRAX name is obsolete, use more common xxx_JTAG 

ESP_APPTRACE_DEST_UART

enumerator ESP_APPTRACE_DEST_UART
UART destination. 

ESP_APPTRACE_DEST_MAX

enumerator ESP_APPTRACE_DEST_MAX

ESP_APPTRACE_DEST_NUM

enumerator ESP_APPTRACE_DEST_NUM


```



### Header File


* components/app_trace/include/esp_sysview_trace.h
* This header file can be included with:



> ```
> #include "esp_sysview_trace.h"
> 
> ```
* This header file is a part of the API provided by the `app_trace` component. To declare that your component depends on `app_trace`, add the following to your CMakeLists.txt:



> ```
> REQUIRES app_trace
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES app_trace
> 
> ```




### Functions


#### esp_sysview_flush


```


static inline esp_err_t esp_sysview_flush(uint32_t tmo)
Flushes remaining data in SystemView trace buffer to host. 

Parameters
tmo -- Timeout for operation (in us). Use ESP_APPTRACE_TMO_INFINITE to wait indefinetly.

Returns
ESP_OK. 



```

#### esp_sysview_vprintf


```


int esp_sysview_vprintf(const char *format, va_list args)
vprintf-like function to sent log messages to the host. 

Parameters

format -- Address of format string. 
args -- List of arguments.


Returns
Number of bytes written. 



```

#### esp_sysview_heap_trace_start


```


esp_err_t esp_sysview_heap_trace_start(uint32_t tmo)
Starts SystemView heap tracing. 

Parameters
tmo -- Timeout (in us) to wait for the host to be connected. Use -1 to wait forever.

Returns
ESP_OK on success, ESP_ERR_TIMEOUT if operation has been timed out. 



```

#### esp_sysview_heap_trace_stop


```


esp_err_t esp_sysview_heap_trace_stop(void)
Stops SystemView heap tracing. 

Returns
ESP_OK. 



```

#### esp_sysview_heap_trace_alloc


```


void esp_sysview_heap_trace_alloc(void *addr, uint32_t size, const void *callers)
Sends heap allocation event to the host. 

Parameters

addr -- Address of allocated block. 
size -- Size of allocated block. 
callers -- Pointer to array with callstack addresses. Array size must be CONFIG_HEAP_TRACING_STACK_DEPTH. 




```

#### esp_sysview_heap_trace_free


```


void esp_sysview_heap_trace_free(void *addr, const void *callers)
Sends heap de-allocation event to the host. 

Parameters

addr -- Address of de-allocated block. 
callers -- Pointer to array with callstack addresses. Array size must be CONFIG_HEAP_TRACING_STACK_DEPTH. 




```