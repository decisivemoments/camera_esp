# Inter-Processor Call (IPC)




Note


IPC stands for an **"Inter-Processor Call"** and **NOT** "Inter-Process Communication" as found on other operating systems.




## Overview


Due to the dual core nature of the ESP32-S3, there are some scenarios where a certain callback must be executed from a particular core such as:


* When allocating an ISR to an interrupt source of a particular core (applies to freeing a particular core's interrupt source as well)
* On particular chips (such as the ESP32), accessing memory that is exclusive to a particular core (such as RTC Fast Memory)
* Reading the registers/state of another core


The IPC (Inter-Processor Call) feature allows a particular core (the calling core) to trigger the execution of a callback function on another core (the target core). The IPC feature allows execution of a callback function on the target core in either a task context, or an interrupt context. Depending on the context that the callback function is executed in, different restrictions apply to the implementation of the callback function.




## IPC in Task Context


The IPC feature implements callback execution in a task context by creating an IPC task for each core during application startup. When the calling core needs to execute a callback on the target core, the callback will execute in the context of the target core's IPC task.


When using IPCs in a task context, users need to consider the following:


* IPC callbacks should ideally be simple and short. An IPC callback **must never block or yield**.
* The IPC tasks are created at the highest possible priority (i.e., `configMAX_PRIORITIES - 1`).



> + If CONFIG_ESP_IPC_USES_CALLERS_PRIORITY is enabled, the target core's IPC task will be lowered to the current priority of the target core before executing the callback.
> 	+ If CONFIG_ESP_IPC_USES_CALLERS_PRIORITY is disabled, the target core will always execute the callback at the highest possible priority.
* Depending on the complexity of the callback, users may need to configure the stack size of the IPC task via CONFIG_ESP_IPC_TASK_STACK_SIZE.
* The IPC feature is internally protected by a mutex. Therefore, simultaneous IPC calls from two or more calling core's are serialized on a first come first serve basis.



### API Usage


Task Context IPC callbacks have the following restrictions:


* The callback must be of the `esp_ipc_func_t` type.
* The callback **must never block or yield** as this will result in the target core's IPC task blocking or yielding.
* The callback must avoid changing any aspect of the IPC task's state, e.g., by calling `vTaskPrioritySet(NULL, x)`.


The IPC feature offers the API listed below to execute a callback in a task context on a target core. The API allows the calling core to block until the callback's execution has completed, or return immediately once the callback's execution has started.


* `esp_ipc_call()` triggers an IPC call on the target core. This function will block until the target core's IPC task **begins** execution of the callback.
* `esp_ipc_call_blocking()` triggers an IPC on the target core. This function will block until the target core's IPC task **completes** execution of the callback.





## IPC in Interrupt Context


In some cases, we need to quickly obtain the state of another core such as in a core dump, GDB stub, various unit tests, and hardware errata workarounds. The IPC ISR feature implements callback execution from a High Priority Interrupt context by reserving a High Priority Interrupt on each core for IPC usage. When a calling core needs to execute a callback on the target core, the callback will execute in the context of the High Priority Interrupt of the target core.


For such scenarios, the IPC ISR feature supports execution of callbacks in a High Priority Interrupt context.


When using IPCs in High Priority Interrupt context, users need to consider the following:


* Since the callback is executed in a High Priority Interrupt context, the callback must be written entirely in assembly. See the API Usage below for more details regarding writing assembly callbacks.
* The priority of the reserved High Priority Interrupt is dependent on the CONFIG_ESP_SYSTEM_CHECK_INT_LEVEL option.





When the callback executes, users need to consider the following:


* The calling core will disable interrupts of priority level 3 and lower.
* Although the priority of the reserved interrupt depends on CONFIG_ESP_SYSTEM_CHECK_INT_LEVEL, during the execution of IPC ISR callback, the target core will disable interrupts of priority level 5 and lower regardless of what CONFIG_ESP_SYSTEM_CHECK_INT_LEVEL is set to.






### API Usage


High Priority Interrupt IPC callbacks have the following restrictions:


* The callback must be of type `esp_ipc_isr_func_t` but implemented entirely in assembly.
* The callback is invoked via the `CALLX0` instruction with register windowing disabled, thus the callback:
	+ must not call any register window related instructions, e.g., `entry` and `retw`.
	+ must not call other C functions as register windowing is disabled.
* The callback should be placed in IRAM at a 4-byte aligned address.
* On invocation of, or after returning from the callback, the registers `a2, a3, a4` are saved/restored automatically, thus can be used in the callback. The callback should **ONLY** use those registers.
	+ `a2` contains the `void *arg` of the callback.
	+ `a3/a4` are free to use as scratch registers.


The IPC feature offers the API listed below to execute a callback in a High Priority Interrupt context:


* `esp_ipc_isr_call()` triggers an IPC call on the target core. This function will busy-wait until the target core **begins** execution of the callback.
* `esp_ipc_isr_call_blocking()` triggers an IPC call on the target core. This function will busy-wait until the target core **completes** execution of the callback.


The following code-blocks demonstrates a High Priority Interrupt IPC callback written in assembly that simply reads the target core's cycle count:



```
/* esp_test_ipc_isr_get_cycle_count_other_cpu(void *arg) */
// this function reads CCOUNT of the target core and stores it in arg.
// use only a2, a3 and a4 regs here.
.section    .iram1, "ax"
.align      4
.global     esp_test_ipc_isr_get_cycle_count_other_cpu
.type       esp_test_ipc_isr_get_cycle_count_other_cpu, @function
// Args:
// a2 - void* arg
esp_test_ipc_isr_get_cycle_count_other_cpu:
rsr.ccount a3
s32i    a3, a2, 0
ret

```



```
unit32_t cycle_count;
esp_ipc_isr_call_blocking(esp_test_ipc_isr_get_cycle_count_other_cpu, (void *)cycle_count);

```



Note


The number of scratch registers available for use is sufficient for most simple use cases. But if your callback requires more scratch registers, `void *arg` can point to a buffer that is used as a register save area. The callback can then save and restore more registers. See the system/ipc/ipc_isr.




Note


For more examples of High Priority Interrupt IPC callbacks, you can refer to components/esp_system/port/arch/xtensa/esp_ipc_isr_routines.S and components/esp_system/test_apps/esp_system_unity_tests/main/port/arch/xtensa/test_ipc_isr.S.



See examples/system/ipc/ipc_isr/xtensa/main/main.c for an example of its use.


The High Priority Interrupt IPC API also provides the following convenience functions that can stall/resume the target core. These APIs utilize the High Priority Interrupt IPC, but supply their own internal callbacks:


* `esp_ipc_isr_stall_other_cpu()` stalls the target core. The calling core disables interrupts of level 3 and lower while the target core will busy-wait with interrupts of level 5 and lower disabled. The target core will busy-wait until `esp_ipc_isr_release_other_cpu()` is called.
* `esp_ipc_isr_release_other_cpu()` resumes the target core.








## API Reference



### Header File


* components/esp_system/include/esp_ipc.h
* This header file can be included with:



> ```
> #include "esp_ipc.h"
> 
> ```




### Functions


#### esp_ipc_call


```


esp_err_t esp_ipc_call(uint32_t cpu_id, esp_ipc_func_t func, void *arg)
Execute a callback on a given CPU. 
Execute a given callback on a particular CPU. The callback must be of type "esp_ipc_func_t" and will be invoked in the context of the target CPU's IPC task.

This function will block the target CPU's IPC task has begun execution of the callback
If another IPC call is ongoing, this function will block until the ongoing IPC call completes
The stack size of the IPC task can be configured via the CONFIG_ESP_IPC_TASK_STACK_SIZE option



Note
In single-core mode, returns ESP_ERR_INVALID_ARG for cpu_id 1.


Parameters

cpu_id -- [in] CPU where the given function should be executed (0 or 1) 
func -- [in] Pointer to a function of type void func(void* arg) to be executed 
arg -- [in] Arbitrary argument of type void* to be passed into the function


Returns

ESP_ERR_INVALID_ARG if cpu_id is invalid
ESP_ERR_INVALID_STATE if the FreeRTOS scheduler is not running
ESP_OK otherwise 





```

#### esp_ipc_call_blocking


```


esp_err_t esp_ipc_call_blocking(uint32_t cpu_id, esp_ipc_func_t func, void *arg)
Execute a callback on a given CPU until and block until it completes. 
This function is identical to esp_ipc_call() except that this function will block until the execution of the callback completes.

Note
In single-core mode, returns ESP_ERR_INVALID_ARG for cpu_id 1.


Parameters

cpu_id -- [in] CPU where the given function should be executed (0 or 1) 
func -- [in] Pointer to a function of type void func(void* arg) to be executed 
arg -- [in] Arbitrary argument of type void* to be passed into the function


Returns

ESP_ERR_INVALID_ARG if cpu_id is invalid
ESP_ERR_INVALID_STATE if the FreeRTOS scheduler is not running
ESP_OK otherwise 





```



### Type Definitions


#### esp_ipc_func_t


```


typedef void (*esp_ipc_func_t)(void *arg)
IPC Callback. 
A callback of this type should be provided as an argument when calling esp_ipc_call() or esp_ipc_call_blocking(). 

```



### Header File


* components/esp_system/include/esp_ipc_isr.h
* This header file can be included with:



> ```
> #include "esp_ipc_isr.h"
> 
> ```




### Functions


#### esp_ipc_isr_call


```


void esp_ipc_isr_call(esp_ipc_isr_func_t func, void *arg)
Execute an ISR callback on the other CPU. 
Execute a given callback on the other CPU in the context of a High Priority Interrupt.

This function will busy-wait in a critical section until the other CPU has started execution of the callback
The callback must be written:
in assembly for XTENSA chips (such as ESP32, ESP32S3). The function is invoked using a CALLX0 instruction and can use only a2, a3, a4 registers. See :doc:IPC in Interrupt Context </api-reference/system/ipc> doc for more details.
in C or assembly for RISCV chips (such as ESP32P4).





Note
This function is not available in single-core mode.


Parameters

func -- [in] Pointer to a function of type void func(void* arg) to be executed 
arg -- [in] Arbitrary argument of type void* to be passed into the function 




```

#### esp_ipc_isr_call_blocking


```


void esp_ipc_isr_call_blocking(esp_ipc_isr_func_t func, void *arg)
Execute an ISR callback on the other CPU and busy-wait until it completes. 
This function is identical to esp_ipc_isr_call() except that this function will busy-wait until the execution of the callback completes.

Note
This function is not available in single-core mode.


Parameters

func -- [in] Pointer to a function of type void func(void* arg) to be executed 
arg -- [in] Arbitrary argument of type void* to be passed into the function 




```

#### esp_ipc_isr_stall_other_cpu


```


void esp_ipc_isr_stall_other_cpu(void)
Stall the other CPU. 
This function will stall the other CPU. The other CPU is stalled by busy-waiting in the context of a High Priority Interrupt. The other CPU will not be resumed until esp_ipc_isr_release_other_cpu() is called.

This function is internally implemented using IPC ISR
This function is used for DPORT workaround.
If the stall feature is paused using esp_ipc_isr_stall_pause(), this function will have no effect



Note
This function is not available in single-core mode. 


Note
It is the caller's responsibility to avoid deadlocking on spinlocks 


```

#### esp_ipc_isr_release_other_cpu


```


void esp_ipc_isr_release_other_cpu(void)
Release the other CPU. 
This function will release the other CPU that was previously stalled from calling esp_ipc_isr_stall_other_cpu()

This function is used for DPORT workaround.
If the stall feature is paused using esp_ipc_isr_stall_pause(), this function will have no effect



Note
This function is not available in single-core mode. 


```

#### esp_ipc_isr_stall_pause


```


void esp_ipc_isr_stall_pause(void)
Puase the CPU stall feature. 
This function will pause the CPU stall feature. Once paused, calls to esp_ipc_isr_stall_other_cpu() and esp_ipc_isr_release_other_cpu() will have no effect. If a IPC ISR call is already in progress, this function will busy-wait until the call completes before pausing the CPU stall feature. 

```

#### esp_ipc_isr_stall_abort


```


void esp_ipc_isr_stall_abort(void)
Abort a CPU stall. 
This function will abort any stalling routine of the other CPU due to a pervious call to esp_ipc_isr_stall_other_cpu(). This function aborts the stall in a non-recoverable manner, thus should only be called in case of a panic().

This function is used in panic handling code 



```

#### esp_ipc_isr_stall_resume


```


void esp_ipc_isr_stall_resume(void)
Resume the CPU stall feature. 
This function will resume the CPU stall feature that was previously paused by calling esp_ipc_isr_stall_pause(). Once resumed, calls to esp_ipc_isr_stall_other_cpu() and esp_ipc_isr_release_other_cpu() will have effect again. 

```



### Macros


#### esp_ipc_isr_asm_call


```


esp_ipc_isr_asm_call(func, arg)
Execute an ISR callback on the other CPU See esp_ipc_isr_call(). 

```

#### esp_ipc_isr_asm_call_blocking


```


esp_ipc_isr_asm_call_blocking(func, arg)
Execute an ISR callback on the other CPU and busy-wait until it completes See esp_ipc_isr_call_blocking(). 

```



### Type Definitions


#### esp_ipc_isr_func_t


```


typedef void (*esp_ipc_isr_func_t)(void *arg)
IPC ISR Callback. 
The callback must be written:
in assembly for XTENSA chips (such as ESP32, ESP32S3).
in C or assembly for RISCV chips (such as ESP32P4).


A callback of this type should be provided as an argument when calling esp_ipc_isr_call() or esp_ipc_isr_call_blocking(). 

```