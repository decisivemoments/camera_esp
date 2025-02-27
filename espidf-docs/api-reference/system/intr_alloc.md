# Interrupt Allocation




## Overview


The ESP32-S3 has two cores, with 32 interrupts each. Each interrupt has a fixed priority, most (but not all) interrupts are connected to the interrupt matrix.


Because there are more interrupt sources than interrupts, sometimes it makes sense to share an interrupt in multiple drivers. The `esp_intr_alloc()` abstraction exists to hide all these implementation details.


A driver can allocate an interrupt for a certain peripheral by calling `esp_intr_alloc()` (or `esp_intr_alloc_intrstatus()`). It can use the flags passed to this function to specify the type, priority, and trigger method of the interrupt to allocate. The interrupt allocation code will then find an applicable interrupt, use the interrupt matrix to hook it up to the peripheral, and install the given interrupt handler and ISR to it.


The interrupt allocator presents two different types of interrupts, namely shared interrupts and non-shared interrupts, both of which require different handling. Non-shared interrupts will allocate a separate interrupt for every `esp_intr_alloc()` call, and this interrupt is use solely for the peripheral attached to it, with only one ISR that will get called. Shared interrupts can have multiple peripherals triggering them, with multiple ISRs being called when one of the peripherals attached signals an interrupt. Thus, ISRs that are intended for shared interrupts should check the interrupt status of the peripheral they service in order to check if any action is required.


Non-shared interrupts can be either level- or edge-triggered. Shared interrupts can only be level interrupts due to the chance of missed interrupts when edge interrupts are used.


To illustrate why shard interrupts can only be level-triggered, take the scenario where peripheral A and peripheral B share the same edge-triggered interrupt. Peripheral B triggers an interrupt and sets its interrupt signal high, causing a low-to-high edge, which in turn latches the CPU's interrupt bit and triggers the ISR. The ISR executes, checks that peripheral A did not trigger an interrupt, and proceeds to handle and clear peripheral B's interrupt signal. Before the ISR returns, the CPU clears its interrupt bit latch. Thus, during the entire interrupt handling process, if peripheral A triggers an interrupt, it will be missed due the CPU clearing the interrupt bit latch.


* Allocating an external interrupt will always allocate it on the core that does the allocation.
* Freeing an external interrupt must always happen on the same core it was allocated on.
* Disabling and enabling external interrupts from another core is allowed.
* Multiple external interrupt sources can share an interrupt slot by passing `ESP_INTR_FLAG_SHARED` as a flag to `esp_intr_alloc()`.


Care should be taken when calling `esp_intr_alloc()` from a task which is not pinned to a core. During task switching, these tasks can migrate between cores. Therefore it is impossible to tell which CPU the interrupt is allocated on, which makes it difficult to free the interrupt handle and may also cause debugging difficulties. It is advised to use `xTaskCreatePinnedToCore()` with a specific CoreID argument to create tasks that allocate interrupts. In the case of internal interrupt sources, this is required.




## Multicore Issues


Peripherals that can generate interrupts can be divided in two types:



> * External peripherals, within the ESP32-S3 but outside the Xtensa cores themselves. Most ESP32-S3 peripherals are of this type.
> * Internal peripherals, part of the Xtensa CPU cores themselves.


Interrupt handling differs slightly between these two types of peripherals.



### Internal Peripheral Interrupts


Each Xtensa CPU core has its own set of six internal peripherals:



> * Three timer comparators
> * A performance monitor
> * Two software interrupts


Internal interrupt sources are defined in `esp_intr_alloc.h` as `ETS_INTERNAL_*_INTR_SOURCE`.


These peripherals can only be configured from the core they are associated with. When generating an interrupt, the interrupt they generate is hard-wired to their associated core; it is not possible to have, for example, an internal timer comparator of one core generate an interrupt on another core. That is why these sources can only be managed using a task running on that specific core. Internal interrupt sources are still allocatable using `esp_intr_alloc()` as normal, but they cannot be shared and will always have a fixed interrupt level (namely, the one associated in hardware with the peripheral).




### External Peripheral Interrupts


The remaining interrupt sources are from external peripherals.





## IRAM-Safe Interrupt Handlers


The `ESP_INTR_FLAG_IRAM` flag registers an interrupt handler that always runs from IRAM (and reads all its data from DRAM), and therefore does not need to be disabled during flash erase and write operations.


This is useful for interrupts which need a guaranteed minimum execution latency, as flash write and erase operations can be slow (erases can take tens or hundreds of milliseconds to complete).


It can also be useful to keep an interrupt handler in IRAM if it is called very frequently, to avoid flash cache misses.


Refer to the SPI flash API documentation for more details.




## Multiple Handlers Sharing A Source


Several handlers can be assigned to a same source, given that all handlers are allocated using the `ESP_INTR_FLAG_SHARED` flag. They will all be allocated to the interrupt, which the source is attached to, and called sequentially when the source is active. The handlers can be disabled and freed individually. The source is attached to the interrupt (enabled), if one or more handlers are enabled, otherwise detached. A handler will never be called when disabled, while **its source may still be triggered** if any one of its handler enabled.


Sources attached to non-shared interrupt do not support this feature.


By default, when `ESP_INTR_FLAG_SHARED` flag is specified, the interrupt allocator will allocate only priority level 1 interrupts. Use `ESP_INTR_FLAG_SHARED | ESP_INTR_FLAG_LOWMED` to also allow allocating shared interrupts at priority levels 2 and 3.


Though the framework supports this feature, you have to use it **very carefully**. There usually exist two ways to stop an interrupt from being triggered: **disable the source** or **mask peripheral interrupt status**. ESP-IDF only handles enabling and disabling of the source itself, leaving status and mask bits to be handled by users.


**Status bits shall either be masked before the handler responsible for it is disabled, or be masked and then properly handled in another enabled interrupt**.



Note


Leaving some status bits unhandled without masking them, while disabling the handlers for them, will cause the interrupt(s) to be triggered indefinitely, resulting therefore in a system crash.





## Troubleshooting Interrupt Allocation


On most Espressif SoCs, CPU interrupts are a limited resource. Therefore it is possible for a program to run out of CPU interrupts, for example by initializing several peripheral drivers. Typically, this will result in the driver initialization function returning `ESP_ERR_NOT_FOUND` error code.


If this happens, you can use `esp_intr_dump()` function to print the list of interrupts along with their status. The output of this function typically looks like this:



```
CPU 0 interrupt status:
Int  Level  Type   Status
0     1    Level  Reserved
1     1    Level  Reserved
2     1    Level  Used: RTC_CORE
3     1    Level  Used: TG0_LACT_LEVEL
...

```


The columns of the output have the following meaning:


* `Int`: CPU interrupt input number. This is typically not used in software directly, and is provided for reference only.
* `Level`: Interrupt priority (1-7) of the CPU interrupt. This priority is fixed in hardware, and cannot be changed.
* `Type`: Interrupt type (Level or Edge) of the CPU interrupt. This type is fixed in hardware, and cannot be changed.
* `Status`: One of the possible statuses of the interrupt:
	+ `Reserved`: The interrupt is reserved either at hardware level, or by one of the parts of ESP-IDF. It can not be allocated using `esp_intr_alloc()`.
	+ `Used: <source>`: The interrupt is allocated and connected to a single peripheral.
	+ `Shared: <source1> <source2> ...`: The interrupt is allocated and connected to multiple peripherals. See Multiple Handlers Sharing A Source above.
	+ `Free`: The interrupt is not allocated and can be used by `esp_intr_alloc()`.
* `Free (not general-use)`: The interrupt is not allocated, but is either a high-priority interrupt (priority 4-7) or an edge-triggered interrupt. High-priority interrupts can be allocated using `esp_intr_alloc()` but requires the handlers to be written in Assembly, see High Priority Interrupts. Edge-triggered low- and medium-priority interrupts can also be allocated using `esp_intr_alloc()`, but are not used often since most peripheral interrupts are level-triggered.





If you have confirmed that the application is indeed running out of interrupts, a combination of the following suggestions can help resolve the issue:


* On multi-core targets, try initializing some of the peripheral drivers from a task pinned to the second core. Interrupts are typically allocated on the same core where the peripheral driver initialization function runs. Therefore by running the initialization function on the second core, more interrupt inputs can be used.
* Determine the interrupts which can tolerate higher latency, and allocate them using `ESP_INTR_FLAG_SHARED` flag (optionally ORed with `ESP_INTR_FLAG_LOWMED`). Using this flag for two or more peripherals will let them use a single interrupt input, and therefore save interrupt inputs for other peripherals. See Multiple Handlers Sharing A Source above.
* Some peripheral driver may default to allocating interrupts with `ESP_INTR_FLAG_LEVEL1` flag, so priority 2 and 3 interrupts do not get used by default. If `esp_intr_dump()` shows that some priority 2 or 3 interrupts are available, try changing the interrupt allocation flags when initializing the driver to `ESP_INTR_FLAG_LEVEL2` or `ESP_INTR_FLAG_LEVEL3`.
* Check if some of the peripheral drivers do not need to be used all the time, and initialize or deinitialize them on demand. This can reduce the number of simultaneously allocated interrupts.







## API Reference



### Header File


* components/esp_hw_support/include/esp_intr_types.h
* This header file can be included with:



> ```
> #include "esp_intr_types.h"
> 
> ```




### Macros


#### ESP_INTR_CPU_AFFINITY_TO_CORE_ID


```


ESP_INTR_CPU_AFFINITY_TO_CORE_ID(cpu_affinity)
Convert esp_intr_cpu_affinity_t to CPU core ID. 

```



### Type Definitions


#### intr_handler_t


```


typedef void (*intr_handler_t)(void *arg)
Function prototype for interrupt handler function 

```

#### intr_handle_t


```


typedef struct intr_handle_data_t *intr_handle_t
Handle to an interrupt handler 

```



### Enumerations


#### esp_intr_cpu_affinity_t


```


enum esp_intr_cpu_affinity_t
Interrupt CPU core affinity. 
This type specify the CPU core that the peripheral interrupt is connected to. 
Values:
ESP_INTR_CPU_AFFINITY_AUTO

enumerator ESP_INTR_CPU_AFFINITY_AUTO
Install the peripheral interrupt to ANY CPU core, decided by on which CPU the interrupt allocator is running. 

ESP_INTR_CPU_AFFINITY_0

enumerator ESP_INTR_CPU_AFFINITY_0
Install the peripheral interrupt to CPU core 0. 

ESP_INTR_CPU_AFFINITY_1

enumerator ESP_INTR_CPU_AFFINITY_1
Install the peripheral interrupt to CPU core 1. 


```



### Header File


* components/esp_hw_support/include/esp_intr_alloc.h
* This header file can be included with:



> ```
> #include "esp_intr_alloc.h"
> 
> ```




### Functions


#### esp_intr_mark_shared


```


esp_err_t esp_intr_mark_shared(int intno, int cpu, bool is_in_iram)
Mark an interrupt as a shared interrupt. 
This will mark a certain interrupt on the specified CPU as an interrupt that can be used to hook shared interrupt handlers to.

Parameters

intno -- The number of the interrupt (0-31) 
cpu -- CPU on which the interrupt should be marked as shared (0 or 1) 
is_in_iram -- Shared interrupt is for handlers that reside in IRAM and the int can be left enabled while the flash cache is disabled.


Returns
ESP_ERR_INVALID_ARG if cpu or intno is invalid ESP_OK otherwise 



```

#### esp_intr_reserve


```


esp_err_t esp_intr_reserve(int intno, int cpu)
Reserve an interrupt to be used outside of this framework. 
This will mark a certain interrupt on the specified CPU as reserved, not to be allocated for any reason.

Parameters

intno -- The number of the interrupt (0-31) 
cpu -- CPU on which the interrupt should be marked as shared (0 or 1)


Returns
ESP_ERR_INVALID_ARG if cpu or intno is invalid ESP_OK otherwise 



```

#### esp_intr_alloc


```


esp_err_t esp_intr_alloc(int source, int flags, intr_handler_t handler, void *arg, intr_handle_t *ret_handle)
Allocate an interrupt with the given parameters. 
This finds an interrupt that matches the restrictions as given in the flags parameter, maps the given interrupt source to it and hooks up the given interrupt handler (with optional argument) as well. If needed, it can return a handle for the interrupt as well.
The interrupt will always be allocated on the core that runs this function.
If ESP_INTR_FLAG_IRAM flag is used, and handler address is not in IRAM or RTC_FAST_MEM, then ESP_ERR_INVALID_ARG is returned.

Parameters

source -- The interrupt source. One of the ETS_*_INTR_SOURCE interrupt mux sources, as defined in soc/soc.h, or one of the internal ETS_INTERNAL_*_INTR_SOURCE sources as defined in this header. 
flags -- An ORred mask of the ESP_INTR_FLAG_* defines. These restrict the choice of interrupts that this routine can choose from. If this value is 0, it will default to allocating a non-shared interrupt of level 1, 2 or 3. If this is ESP_INTR_FLAG_SHARED, it will allocate a shared interrupt of level 1. Setting ESP_INTR_FLAG_INTRDISABLED will return from this function with the interrupt disabled. 
handler -- The interrupt handler. Must be NULL when an interrupt of level >3 is requested, because these types of interrupts aren't C-callable. 
arg -- Optional argument for passed to the interrupt handler 
ret_handle -- Pointer to an intr_handle_t to store a handle that can later be used to request details or free the interrupt. Can be NULL if no handle is required.


Returns
ESP_ERR_INVALID_ARG if the combination of arguments is invalid. ESP_ERR_NOT_FOUND No free interrupt found with the specified flags ESP_OK otherwise 



```

#### esp_intr_alloc_intrstatus


```


esp_err_t esp_intr_alloc_intrstatus(int source, int flags, uint32_t intrstatusreg, uint32_t intrstatusmask, intr_handler_t handler, void *arg, intr_handle_t *ret_handle)
Allocate an interrupt with the given parameters. 
This essentially does the same as esp_intr_alloc, but allows specifying a register and mask combo. For shared interrupts, the handler is only called if a read from the specified register, ANDed with the mask, returns non-zero. By passing an interrupt status register address and a fitting mask, this can be used to accelerate interrupt handling in the case a shared interrupt is triggered; by checking the interrupt statuses first, the code can decide which ISRs can be skipped

Parameters

source -- The interrupt source. One of the ETS_*_INTR_SOURCE interrupt mux sources, as defined in soc/soc.h, or one of the internal ETS_INTERNAL_*_INTR_SOURCE sources as defined in this header. 
flags -- An ORred mask of the ESP_INTR_FLAG_* defines. These restrict the choice of interrupts that this routine can choose from. If this value is 0, it will default to allocating a non-shared interrupt of level 1, 2 or 3. If this is ESP_INTR_FLAG_SHARED, it will allocate a shared interrupt of level 1. Setting ESP_INTR_FLAG_INTRDISABLED will return from this function with the interrupt disabled. 
intrstatusreg -- The address of an interrupt status register 
intrstatusmask -- A mask. If a read of address intrstatusreg has any of the bits that are 1 in the mask set, the ISR will be called. If not, it will be skipped. 
handler -- The interrupt handler. Must be NULL when an interrupt of level >3 is requested, because these types of interrupts aren't C-callable. 
arg -- Optional argument for passed to the interrupt handler 
ret_handle -- Pointer to an intr_handle_t to store a handle that can later be used to request details or free the interrupt. Can be NULL if no handle is required.


Returns
ESP_ERR_INVALID_ARG if the combination of arguments is invalid. ESP_ERR_NOT_FOUND No free interrupt found with the specified flags ESP_OK otherwise 



```

#### esp_intr_free


```


esp_err_t esp_intr_free(intr_handle_t handle)
Disable and free an interrupt. 
Use an interrupt handle to disable the interrupt and release the resources associated with it. If the current core is not the core that registered this interrupt, this routine will be assigned to the core that allocated this interrupt, blocking and waiting until the resource is successfully released.

Note
When the handler shares its source with other handlers, the interrupt status bits it's responsible for should be managed properly before freeing it. see esp_intr_disable for more details. Please do not call this function in esp_ipc_call_blocking.


Parameters
handle -- The handle, as obtained by esp_intr_alloc or esp_intr_alloc_intrstatus

Returns
ESP_ERR_INVALID_ARG the handle is NULL ESP_FAIL failed to release this handle ESP_OK otherwise 



```

#### esp_intr_get_cpu


```


int esp_intr_get_cpu(intr_handle_t handle)
Get CPU number an interrupt is tied to. 

Parameters
handle -- The handle, as obtained by esp_intr_alloc or esp_intr_alloc_intrstatus

Returns
The core number where the interrupt is allocated 



```

#### esp_intr_get_intno


```


int esp_intr_get_intno(intr_handle_t handle)
Get the allocated interrupt for a certain handle. 

Parameters
handle -- The handle, as obtained by esp_intr_alloc or esp_intr_alloc_intrstatus

Returns
The interrupt number 



```

#### esp_intr_disable


```


esp_err_t esp_intr_disable(intr_handle_t handle)
Disable the interrupt associated with the handle. 

Note

For local interrupts (ESP_INTERNAL_* sources), this function has to be called on the CPU the interrupt is allocated on. Other interrupts have no such restriction.
When several handlers sharing a same interrupt source, interrupt status bits, which are handled in the handler to be disabled, should be masked before the disabling, or handled in other enabled interrupts properly. Miss of interrupt status handling will cause infinite interrupt calls and finally system crash.




Parameters
handle -- The handle, as obtained by esp_intr_alloc or esp_intr_alloc_intrstatus

Returns
ESP_ERR_INVALID_ARG if the combination of arguments is invalid. ESP_OK otherwise 



```

#### esp_intr_enable


```


esp_err_t esp_intr_enable(intr_handle_t handle)
Enable the interrupt associated with the handle. 

Note
For local interrupts (ESP_INTERNAL_* sources), this function has to be called on the CPU the interrupt is allocated on. Other interrupts have no such restriction.


Parameters
handle -- The handle, as obtained by esp_intr_alloc or esp_intr_alloc_intrstatus

Returns
ESP_ERR_INVALID_ARG if the combination of arguments is invalid. ESP_OK otherwise 



```

#### esp_intr_set_in_iram


```


esp_err_t esp_intr_set_in_iram(intr_handle_t handle, bool is_in_iram)
Set the "in IRAM" status of the handler. 

Note
Does not work on shared interrupts.


Parameters

handle -- The handle, as obtained by esp_intr_alloc or esp_intr_alloc_intrstatus 
is_in_iram -- Whether the handler associated with this handle resides in IRAM. Handlers residing in IRAM can be called when cache is disabled.


Returns
ESP_ERR_INVALID_ARG if the combination of arguments is invalid. ESP_OK otherwise 



```

#### esp_intr_noniram_disable


```


void esp_intr_noniram_disable(void)
Disable interrupts that aren't specifically marked as running from IRAM. 

```

#### esp_intr_noniram_enable


```


void esp_intr_noniram_enable(void)
Re-enable interrupts disabled by esp_intr_noniram_disable. 

```

#### esp_intr_enable_source


```


void esp_intr_enable_source(int inum)
enable the interrupt source based on its number 

Parameters
inum -- interrupt number from 0 to 31 



```

#### esp_intr_disable_source


```


void esp_intr_disable_source(int inum)
disable the interrupt source based on its number 

Parameters
inum -- interrupt number from 0 to 31 



```

#### esp_intr_flags_to_level


```


static inline int esp_intr_flags_to_level(int flags)
Get the lowest interrupt level from the flags. 

Parameters
flags -- The same flags that pass to esp_intr_alloc_intrstatus API 



```

#### esp_intr_level_to_flags


```


static inline int esp_intr_level_to_flags(int level)
Get the interrupt flags from the supplied level (priority) 

Parameters
level -- The interrupt priority level 



```

#### esp_intr_dump


```


esp_err_t esp_intr_dump(FILE *stream)
Dump the status of allocated interrupts. 

Parameters
stream -- The stream to dump to, if NULL then stdout is used 

Returns
ESP_OK on success 



```



### Macros


#### ESP_INTR_FLAG_LEVEL1


```


ESP_INTR_FLAG_LEVEL1
Interrupt allocation flags. 
These flags can be used to specify which interrupt qualities the code calling esp_intr_alloc* needs. Accept a Level 1 interrupt vector (lowest priority) 

```

#### ESP_INTR_FLAG_LEVEL2


```


ESP_INTR_FLAG_LEVEL2
Accept a Level 2 interrupt vector. 

```

#### ESP_INTR_FLAG_LEVEL3


```


ESP_INTR_FLAG_LEVEL3
Accept a Level 3 interrupt vector. 

```

#### ESP_INTR_FLAG_LEVEL4


```


ESP_INTR_FLAG_LEVEL4
Accept a Level 4 interrupt vector. 

```

#### ESP_INTR_FLAG_LEVEL5


```


ESP_INTR_FLAG_LEVEL5
Accept a Level 5 interrupt vector. 

```

#### ESP_INTR_FLAG_LEVEL6


```


ESP_INTR_FLAG_LEVEL6
Accept a Level 6 interrupt vector. 

```

#### ESP_INTR_FLAG_NMI


```


ESP_INTR_FLAG_NMI
Accept a Level 7 interrupt vector (highest priority) 

```

#### ESP_INTR_FLAG_SHARED


```


ESP_INTR_FLAG_SHARED
Interrupt can be shared between ISRs. 

```

#### ESP_INTR_FLAG_EDGE


```


ESP_INTR_FLAG_EDGE
Edge-triggered interrupt. 

```

#### ESP_INTR_FLAG_IRAM


```


ESP_INTR_FLAG_IRAM
ISR can be called if cache is disabled. 

```

#### ESP_INTR_FLAG_INTRDISABLED


```


ESP_INTR_FLAG_INTRDISABLED
Return with this interrupt disabled. 

```

#### ESP_INTR_FLAG_LOWMED


```


ESP_INTR_FLAG_LOWMED
Low and medium prio interrupts. These can be handled in C. 

```

#### ESP_INTR_FLAG_HIGH


```


ESP_INTR_FLAG_HIGH
High level interrupts. Need to be handled in assembly. 

```

#### ESP_INTR_FLAG_LEVELMASK


```


ESP_INTR_FLAG_LEVELMASK
Mask for all level flags 

```

#### ETS_INTERNAL_TIMER0_INTR_SOURCE


```


ETS_INTERNAL_TIMER0_INTR_SOURCE
Platform timer 0 interrupt source. 
The esp_intr_alloc* functions can allocate an int for all ETS_*_INTR_SOURCE interrupt sources that are routed through the interrupt mux. Apart from these sources, each core also has some internal sources that do not pass through the interrupt mux. To allocate an interrupt for these sources, pass these pseudo-sources to the functions. 

```

#### ETS_INTERNAL_TIMER1_INTR_SOURCE


```


ETS_INTERNAL_TIMER1_INTR_SOURCE
Platform timer 1 interrupt source. 

```

#### ETS_INTERNAL_TIMER2_INTR_SOURCE


```


ETS_INTERNAL_TIMER2_INTR_SOURCE
Platform timer 2 interrupt source. 

```

#### ETS_INTERNAL_SW0_INTR_SOURCE


```


ETS_INTERNAL_SW0_INTR_SOURCE
Software int source 1. 

```

#### ETS_INTERNAL_SW1_INTR_SOURCE


```


ETS_INTERNAL_SW1_INTR_SOURCE
Software int source 2. 

```

#### ETS_INTERNAL_PROFILING_INTR_SOURCE


```


ETS_INTERNAL_PROFILING_INTR_SOURCE
Int source for profiling. 

```

#### ETS_INTERNAL_UNUSED_INTR_SOURCE


```


ETS_INTERNAL_UNUSED_INTR_SOURCE
Interrupt is not assigned to any source. 

```

#### ETS_INTERNAL_INTR_SOURCE_OFF


```


ETS_INTERNAL_INTR_SOURCE_OFF
Provides SystemView with positive IRQ IDs, otherwise scheduler events are not shown properly 

```

#### ESP_INTR_ENABLE


```


ESP_INTR_ENABLE(inum)
Enable interrupt by interrupt number 

```

#### ESP_INTR_DISABLE


```


ESP_INTR_DISABLE(inum)
Disable interrupt by interrupt number 

```