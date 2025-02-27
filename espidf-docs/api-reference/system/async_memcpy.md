# Asynchronous Memory Copy




## Overview


ESP32-S3 has a DMA engine which can help to offload internal memory copy operations from the CPU in an asynchronous way.


The async memcpy API wraps all DMA configurations and operations. The signature of `esp_async_memcpy()` is almost the same as the standard libc `memcpy` function.


The DMA allows multiple memory copy requests to be queued up before the first one is completed, which allows overlap of computation and memory copy. Moreover, it is still possible to know the exact time when a memory copy request is completed by registering an event callback.


If the async memcpy is constructed upon the AHB GDMA, it is also possible to copy data from/to PSRAM with a proper alignment.




## Configure and Install Driver


There are several ways to install the async memcpy driver, depending on the underlying DMA engine:


* `esp_async_memcpy_install_gdma_ahb()` is used to install the async memcpy driver based on the AHB GDMA engine.
* `esp_async_memcpy_install()` is a generic API to install the async memcpy driver with a default DMA engine. If the SoC has the CP DMA engine, the default DMA engine is CP DMA. Otherwise, the default DMA engine is AHB GDMA.





Driver configuration is described in `async_memcpy_config_t`:


* `backlog`: This is used to configure the maximum number of memory copy transactions that can be queued up before the first one is completed. If this field is set to zero, then the default value 4 will be applied.
* `dma_burst_size`: Set the burst size in a DMA burst transfer.
* `flags`: This is used to enable some special driver features.



```
async_memcpy_config_t config = ASYNC_MEMCPY_DEFAULT_CONFIG();
// update the maximum data stream supported by underlying DMA engine
config.backlog = 8;
async_memcpy_handle_t driver = NULL;
ESP_ERROR_CHECK(esp_async_memcpy_install(&config, &driver)); // install driver with default DMA engine

```




## Send Memory Copy Request


`esp_async_memcpy()` is the API to send memory copy request to DMA engine. It must be called after driver is installed successfully. This API is thread safe, so it can be called from different tasks.


Different from the libc version of `memcpy`, you can optionally pass a callback to `esp_async_memcpy()`, so that you can be notified when the memory copy is finished. Note that the callback is executed in the ISR context, please make sure you will not call any blocking functions in the callback.


The prototype of the callback function is `async_memcpy_isr_cb_t`. The callback function should only return true if it wakes up a high priority task by RTOS APIs like `xSemaphoreGiveFromISR()`.



```
// Callback implementation, running in ISR context
static bool my_async_memcpy_cb(async_memcpy_handle_t mcp_hdl, async_memcpy_event_t *event, void *cb_args)
{
    SemaphoreHandle_t sem = (SemaphoreHandle_t)cb_args;
    BaseType_t high_task_wakeup = pdFALSE;
    xSemaphoreGiveFromISR(semphr, &high_task_wakeup); // high_task_wakeup set to pdTRUE if some high priority task unblocked
    return high_task_wakeup == pdTRUE;
}

// Create a semaphore used to report the completion of async memcpy
SemaphoreHandle_t semphr = xSemaphoreCreateBinary();

// Called from user's context
ESP_ERROR_CHECK(esp_async_memcpy(driver_handle, to, from, copy_len, my_async_memcpy_cb, my_semaphore));
// Do something else here
xSemaphoreTake(my_semaphore, portMAX_DELAY); // Wait until the buffer copy is done

```




## Uninstall Driver


`esp_async_memcpy_uninstall()` is used to uninstall asynchronous memcpy driver. It is not necessary to uninstall the driver after each memcpy operation. If you know your application will not use this driver anymore, then this API can recycle the memory and other hardware resources for you.




## API Reference



### Header File


* components/esp_hw_support/include/esp_async_memcpy.h
* This header file can be included with:



> ```
> #include "esp_async_memcpy.h"
> 
> ```




### Functions


#### esp_async_memcpy_install_gdma_ahb


```


esp_err_t esp_async_memcpy_install_gdma_ahb(const async_memcpy_config_t *config, async_memcpy_handle_t *mcp)
Install async memcpy driver, with AHB-GDMA as the backend. 

Parameters

config -- [in] Configuration of async memcpy 
mcp -- [out] Returned driver handle 


Returns

ESP_OK: Install async memcpy driver successfully
ESP_ERR_INVALID_ARG: Install async memcpy driver failed because of invalid argument
ESP_ERR_NO_MEM: Install async memcpy driver failed because out of memory
ESP_FAIL: Install async memcpy driver failed because of other error 





```

#### esp_async_memcpy_install


```


esp_err_t esp_async_memcpy_install(const async_memcpy_config_t *config, async_memcpy_handle_t *mcp)
Install async memcpy driver with the default DMA backend. 

Note
On chip with CPDMA support, CPDMA is the default choice. On chip with AHB-GDMA support, AHB-GDMA is the default choice.


Parameters

config -- [in] Configuration of async memcpy 
mcp -- [out] Returned driver handle 


Returns

ESP_OK: Install async memcpy driver successfully
ESP_ERR_INVALID_ARG: Install async memcpy driver failed because of invalid argument
ESP_ERR_NO_MEM: Install async memcpy driver failed because out of memory
ESP_FAIL: Install async memcpy driver failed because of other error 





```

#### esp_async_memcpy_uninstall


```


esp_err_t esp_async_memcpy_uninstall(async_memcpy_handle_t mcp)
Uninstall async memcpy driver. 

Parameters
mcp -- [in] Handle of async memcpy driver that returned from esp_async_memcpy_install

Returns

ESP_OK: Uninstall async memcpy driver successfully
ESP_ERR_INVALID_ARG: Uninstall async memcpy driver failed because of invalid argument
ESP_FAIL: Uninstall async memcpy driver failed because of other error 





```

#### esp_async_memcpy


```


esp_err_t esp_async_memcpy(async_memcpy_handle_t mcp, void *dst, void *src, size_t n, async_memcpy_isr_cb_t cb_isr, void *cb_args)
Send an asynchronous memory copy request. 

Note
The callback function is invoked in interrupt context, never do blocking jobs in the callback.


Parameters

mcp -- [in] Handle of async memcpy driver that returned from esp_async_memcpy_install
dst -- [in] Destination address (copy to) 
src -- [in] Source address (copy from) 
n -- [in] Number of bytes to copy 
cb_isr -- [in] Callback function, which got invoked in interrupt context. Set to NULL can bypass the callback. 
cb_args -- [in] User defined argument to be passed to the callback function 


Returns

ESP_OK: Send memory copy request successfully
ESP_ERR_INVALID_ARG: Send memory copy request failed because of invalid argument
ESP_FAIL: Send memory copy request failed because of other error 





```



### Structures


#### async_memcpy_event_t


```


struct async_memcpy_event_t
Async memory copy event data. 

Public Members
data

void *data
Event data 



```

#### async_memcpy_config_t


```


struct async_memcpy_config_t
Type of async memcpy configuration. 

Public Members
backlog

uint32_t backlog
Maximum number of transactions that can be prepared in the background 

sram_trans_align

size_t sram_trans_align
DMA transfer alignment (both in size and address) for SRAM memory 

psram_trans_align

size_t psram_trans_align
DMA transfer alignment (both in size and address) for PSRAM memory 

dma_burst_size

size_t dma_burst_size
DMA transfer burst size, in bytes 

flags

uint32_t flags
Extra flags to control async memcpy feature 



```



### Macros


#### ASYNC_MEMCPY_DEFAULT_CONFIG


```


ASYNC_MEMCPY_DEFAULT_CONFIG()
Default configuration for async memcpy. 

```



### Type Definitions


#### async_memcpy_handle_t


```


typedef struct async_memcpy_context_t *async_memcpy_handle_t
Async memory copy driver handle. 

```

#### async_memcpy_isr_cb_t


```


typedef bool (*async_memcpy_isr_cb_t)(async_memcpy_handle_t mcp_hdl, async_memcpy_event_t *event, void *cb_args)
Type of async memcpy interrupt callback function. 

Note
User can call OS primitives (semaphore, mutex, etc) in the callback function. Keep in mind, if any OS primitive wakes high priority task up, the callback should return true. 


Param mcp_hdl
Handle of async memcpy 

Param event
Event object, which contains related data, reserved for future 

Param cb_args
User defined arguments, passed from esp_async_memcpy function 

Return
Whether a high priority task is woken up by the callback function



```