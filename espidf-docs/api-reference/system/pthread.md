# POSIX Threads Support




## Overview


ESP-IDF is based on FreeRTOS but offers a range of POSIX-compatible APIs that allow easy porting of third-party code. This includes support for common parts of the POSIX Threads `pthread` API.


POSIX Threads are implemented in ESP-IDF as wrappers around equivalent FreeRTOS features. The runtime memory or performance overhead of using the pthreads API is quite low, but not every feature available in either pthreads or FreeRTOS is available via the ESP-IDF pthreads support.


Pthreads can be used in ESP-IDF by including standard `pthread.h` header, which is included in the toolchain libc. An additional ESP-IDF specific header, `esp_pthread.h`, provides additional non-POSIX APIs for using some ESP-IDF features with pthreads.


C++ Standard Library implementations for `std::thread`, `std::mutex`, `std::condition_variable`, etc., are realized using pthreads (via GCC libstdc++). Therefore, restrictions mentioned here also apply to the equivalent C++ standard library functionality.




## RTOS Integration


Unlike many operating systems using POSIX Threads, ESP-IDF is a real-time operating system with a real-time scheduler. This means that a thread will only stop running if a higher priority task is ready to run, the thread blocks on an OS synchronization structure like a mutex, or the thread calls any of the functions `sleep`, `vTaskDelay()`, or `usleep`.



Note


When calling a standard libc or C++ sleep function, such as `usleep` defined in `unistd.h`, the task will only block and yield the core if the sleep time is longer than one FreeRTOS tick period. If the time is shorter, the thread will busy-wait instead of yielding to another RTOS task.



By default, all POSIX Threads have the same RTOS priority, but it is possible to change this by calling a custom API.




## Standard Features


The following standard APIs are implemented in ESP-IDF.


Refer to standard POSIX Threads documentation, or `pthread.h`, for details about the standard arguments and behaviour of each function. Differences or limitations compared to the standard APIs are noted below.



### Thread APIs


* `pthread_create()`
	+ The `attr` argument is supported for setting stack size and detach state only. Other attribute fields are ignored.
	+ Unlike FreeRTOS task functions, the `start_routine` function is allowed to return. A detached type thread is automatically deleted if the function returns. The default joinable type thread will be suspended until `pthread_join()` is called on it.
* `pthread_join()`
* `pthread_detach()`
* `pthread_exit()`
* `sched_yield()`
* `pthread_self()`
	+ An assert will fail if this function is called from a FreeRTOS task which is not a pthread.
* `pthread_equal()`




### Thread Attributes


* `pthread_attr_init()`
* `pthread_attr_destroy()`
	+ This function does not need to free any resources and instead resets the `attr` structure to defaults. The implementation is the same as `pthread_attr_init()`.
* `pthread_attr_getstacksize()` / `pthread_attr_setstacksize()`
* `pthread_attr_getdetachstate()` / `pthread_attr_setdetachstate()`




### Once


* `pthread_once()`


Static initializer constant `PTHREAD_ONCE_INIT` is supported.



Note


This function can be called from tasks created using either pthread or FreeRTOS APIs.





### Mutexes


POSIX Mutexes are implemented as FreeRTOS Mutex Semaphores (normal type for "fast" or "error check" mutexes, and Recursive type for "recursive" mutexes). This means that they have the same priority inheritance behavior as mutexes created with `xSemaphoreCreateMutex()`.


* `pthread_mutex_init()`
* `pthread_mutex_destroy()`
* `pthread_mutex_lock()`
* `pthread_mutex_timedlock()`
* `pthread_mutex_trylock()`
* `pthread_mutex_unlock()`
* `pthread_mutexattr_init()`
* `pthread_mutexattr_destroy()`
* `pthread_mutexattr_gettype()` / `pthread_mutexattr_settype()`


Static initializer constant `PTHREAD_MUTEX_INITIALIZER` is supported, but the non-standard static initializer constants for other mutex types are not supported.



Note


These functions can be called from tasks created using either pthread or FreeRTOS APIs.





### Condition Variables


* `pthread_cond_init()`
	+ The `attr` argument is not implemented and is ignored.
* `pthread_cond_destroy()`
* `pthread_cond_signal()`
* `pthread_cond_broadcast()`
* `pthread_cond_wait()`
* `pthread_cond_timedwait()`


Static initializer constant `PTHREAD_COND_INITIALIZER` is supported.


* The resolution of `pthread_cond_timedwait()` timeouts is the RTOS tick period (see CONFIG_FREERTOS_HZ). Timeouts may be delayed up to one tick period after the requested timeout.



Note


These functions can be called from tasks created using either pthread or FreeRTOS APIs.





### Semaphores


In ESP-IDF, POSIX **unnamed** semaphores are implemented. The accessible API is described below. It implements semaphores as specified in the POSIX standard, unless specified otherwise.


* sem_init()
* sem_destroy()



> + `pshared` is ignored. Semaphores can always be shared between FreeRTOS tasks.
* sem_post()



> + If the semaphore has a value of `SEM_VALUE_MAX` already, `-1` is returned and `errno` is set to `EAGAIN`.
* sem_wait()
* sem_trywait()
* sem_timedwait()



> + The time value passed by abstime will be rounded up to the next FreeRTOS tick.
> 	+ The actual timeout happens after the tick that the time was rounded to and before the following tick.
> 	+ It is possible, though unlikely, that the task is preempted directly after the timeout calculation, delaying the timeout of the following blocking operating system call by the duration of the preemption.
* sem_getvalue()




### Read/Write Locks


The following API functions of the POSIX reader-writer locks specification are implemented:


* pthread_rwlock_init()



> + The `attr` argument is not implemented and is ignored.
* pthread_rwlock_destroy()
* pthread_rwlock_rdlock()
* pthread_rwlock_tryrdlock()
* pthread_rwlock_wrlock()
* pthread_rwlock_trywrlock()
* pthread_rwlock_unlock()


The static initializer constant `PTHREAD_RWLOCK_INITIALIZER` is supported.



Note


These functions can be called from tasks created using either pthread or FreeRTOS APIs.





### Thread-Specific Data


* `pthread_key_create()`
	+ The `destr_function` argument is supported and will be called if a thread function exits normally, calls `pthread_exit()`, or if the underlying task is deleted directly using the FreeRTOS function `vTaskDelete()`.
* `pthread_key_delete()`
* `pthread_setspecific()` / `pthread_getspecific()`



Note


These functions can be called from tasks created using either pthread or FreeRTOS APIs. When calling these functions from tasks created using FreeRTOS APIs, CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS config option must be enabled to ensure the thread-specific data is cleaned up before the task is deleted.




Note


There are other options for thread local storage in ESP-IDF, including options with higher performance. See Thread Local Storage.






## Not Implemented


The `pthread.h` header is a standard header and includes additional APIs and features which are not implemented in ESP-IDF. These include:


* `pthread_cancel()` returns `ENOSYS` if called.
* `pthread_condattr_init()` returns `ENOSYS` if called.


Other POSIX Threads functions (not listed here) are not implemented and will produce either a compiler or a linker error if referenced from an ESP-IDF application. If you identify a useful API that you would like to see implemented in ESP-IDF, please open a feature request on GitHub with the details.




## ESP-IDF Extensions


The API `esp_pthread_set_cfg()` defined in the `esp_pthreads.h` header offers custom extensions to control how subsequent calls to `pthread_create()` behaves. Currently, the following configuration can be set:


* Default stack size of new threads, if not specified when calling `pthread_create()` (overrides CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT).
* Stack memory capabilities determine which kind of memory is used for allocating pthread stacks. The field takes ESP-IDF heap capability flags, as defined in heap/include/esp_heap_caps.h. The memory must be 8-bit accessible (MALLOC_CAP_8BIT), besides other custom flags the user can choose from. The user is responsible for ensuring the correctness of the stack memory capabilities. For more information about memory locations, refer to the documentation of Memory Capabilities.
* RTOS priority of new threads (overrides CONFIG_PTHREAD_TASK_PRIO_DEFAULT).
* Core affinity / core pinning of new threads (overrides CONFIG_PTHREAD_TASK_CORE_DEFAULT).
* FreeRTOS task name for new threads (overrides CONFIG_PTHREAD_TASK_NAME_DEFAULT)





This configuration is scoped to the calling thread (or FreeRTOS task), meaning that `esp_pthread_set_cfg()` can be called independently in different threads or tasks. If the `inherit_cfg` flag is set in the current configuration then any new thread created will inherit the creator's configuration (if that thread calls `pthread_create()` recursively), otherwise the new thread will have the default configuration.




## Examples


* system/pthread demonstrates using the pthreads API to create threads.
* cxx/pthread demonstrates using C++ Standard Library functions with threads.




## API Reference



### Header File


* components/pthread/include/esp_pthread.h
* This header file can be included with:



> ```
> #include "esp_pthread.h"
> 
> ```
* This header file is a part of the API provided by the `pthread` component. To declare that your component depends on `pthread`, add the following to your CMakeLists.txt:



> ```
> REQUIRES pthread
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES pthread
> 
> ```




### Functions


#### esp_pthread_get_default_config


```


esp_pthread_cfg_t esp_pthread_get_default_config(void)
Creates a default pthread configuration based on the values set via menuconfig. 

Returns
A default configuration structure. 



```

#### esp_pthread_set_cfg


```


esp_err_t esp_pthread_set_cfg(const esp_pthread_cfg_t *cfg)
Configure parameters for creating pthread. 
This API allows you to configure how the subsequent pthread_create() call will behave. This call can be used to setup configuration parameters like stack size, priority, configuration inheritance etc.
If the 'inherit' flag in the configuration structure is enabled, then the same configuration is also inherited in the thread subtree.

Note
If cfg->stack_alloc_caps is 0, it is automatically set to valid default stack memory capabilities. If cfg->stack_alloc_caps is non-zero, the developer is responsible for its correctenss. This function only checks that the capabilities are MALLOC_CAP_8BIT, the rest is unchecked. 


Note
Passing non-NULL attributes to pthread_create() will override the stack_size parameter set using this API


Parameters
cfg -- The pthread config parameters

Returns

ESP_OK if configuration was successfully set
ESP_ERR_NO_MEM if out of memory
ESP_ERR_INVALID_ARG if stack_size is less than PTHREAD_STACK_MIN
ESP_ERR_INVALID_ARG if stack_alloc_caps does not include MALLOC_CAP_8BIT 





```

#### esp_pthread_get_cfg


```


esp_err_t esp_pthread_get_cfg(esp_pthread_cfg_t *p)
Get current pthread creation configuration. 
This will retrieve the current configuration that will be used for creating threads.

Parameters
p -- Pointer to the pthread config structure that will be updated with the currently configured parameters

Returns

ESP_OK if the configuration was available
ESP_ERR_NOT_FOUND if a configuration wasn't previously set 





```

#### esp_pthread_init


```


esp_err_t esp_pthread_init(void)
Initialize pthread library. 

```



### Structures


#### esp_pthread_cfg_t


```


struct esp_pthread_cfg_t
pthread configuration structure that influences pthread creation 

Public Members
stack_size

size_t stack_size
The stack size of the pthread 

prio

size_t prio
The thread's priority 

inherit_cfg

bool inherit_cfg
Inherit this configuration further 

thread_name

const char *thread_name
The thread name. 

pin_to_core

int pin_to_core
The core id to pin the thread to. Has the same value range as xCoreId argument of xTaskCreatePinnedToCore. 

stack_alloc_caps

uint32_t stack_alloc_caps
A bit mask of memory capabilities (MALLOC_CAPS*) to use when allocating the stack. The memory must be 8 bit accessible (MALLOC_CAP_8BIT). The developer is responsible for the correctenss of stack_alloc_caps. 



```



### Macros


#### PTHREAD_STACK_MIN


```


PTHREAD_STACK_MIN

```