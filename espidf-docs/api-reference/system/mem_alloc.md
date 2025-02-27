# Heap Memory Allocation




## Stack and Heap


ESP-IDF applications use the common computer architecture patterns of **stack** (dynamic memory allocated by program control flow), **heap** (dynamic memory allocated by function calls), and **static memory** (memory allocated at compile time).


Because ESP-IDF is a multi-threaded RTOS environment, each RTOS task has its own stack. By default, each of these stacks is allocated from the heap when the task is created. See `xTaskCreateStatic()` for the alternative where stacks are statically allocated.


Because ESP32-S3 uses multiple types of RAM, it also contains multiple heaps with different capabilities. A capabilities-based memory allocator allows apps to make heap allocations for different purposes.


For most purposes, the C Standard Library's `malloc()` and `free()` functions can be used for heap allocation without any special consideration. However, in order to fully make use of all of the memory types and their characteristics, ESP-IDF also has a capabilities-based heap memory allocator. If you want to have a memory with certain properties (e.g., DMA-Capable Memory or executable-memory), you can create an OR-mask of the required capabilities and pass that to `heap_caps_malloc()`.




## Memory Capabilities


The ESP32-S3 contains multiple types of RAM:


* DRAM (Data RAM) is memory that is connected to CPU's data bus and is used to hold data. This is the most common kind of memory accessed as a heap.
* IRAM (Instruction RAM) is memory that is connected to the CPU's instruction bus and usually holds executable data only (i.e., instructions). If accessed as generic memory, all accesses must be aligned to 32-Bit Accessible Memory.
* D/IRAM is RAM that is connected to CPU's data bus and instruction bus, thus can be used either Instruction or Data RAM.


For more details on these internal memory types, see Memory Types.


It is also possible to connect external SPI RAM to the ESP32-S3. The external RAM is integrated into the ESP32-S3's memory map via the cache, and accessed similarly to DRAM.


All DRAM memory is single-byte accessible, thus all DRAM heaps possess the `MALLOC_CAP_8BIT` capability. Users can call `heap_caps_get_free_size(MALLOC_CAP_8BIT)` to get the free size of all DRAM heaps.


When calling `malloc()`, the ESP-IDF `malloc()` internally calls `heap_caps_malloc_default(size)`. This will allocate memory with the capability `MALLOC_CAP_DEFAULT`, which is byte-addressable.


Because `malloc()` uses the capabilities-based allocation system, memory allocated using `heap_caps_malloc()` can be freed by calling the standard `free()` function.




## Available Heap



### DRAM


At startup, the DRAM heap contains all data memory that is not statically allocated by the app. Reducing statically-allocated buffers increases the amount of available free heap.


To find the amount of statically allocated memory, use the idf.py size command.



Note


At runtime, the available heap DRAM may be less than calculated at compile time, because, at startup, some memory is allocated from the heap before the FreeRTOS scheduler is started (including memory for the stacks of initial FreeRTOS tasks).





### IRAM


At startup, the IRAM heap contains all instruction memory that is not used by the app executable code.


The idf.py size command can be used to find the amount of IRAM used by the app.




### D/IRAM


Some memory in the ESP32-S3 is available as either DRAM or IRAM. If memory is allocated from a D/IRAM region, the free heap size for both types of memory will decrease.




### Heap Sizes


At startup, all ESP-IDF apps log a summary of all heap addresses (and sizes) at level Info:



```
I (252) heap_init: Initializing. RAM available for dynamic allocation:
I (259) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (265) heap_init: At 3FFB2EC8 len 0002D138 (180 KiB): DRAM
I (272) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (278) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (284) heap_init: At 4008944C len 00016BB4 (90 KiB): IRAM

```




### Finding Available Heap


See Heap Information.





## Special Capabilities



### DMA-Capable Memory


Use the `MALLOC_CAP_DMA` flag to allocate memory which is suitable for use with hardware DMA engines (for example SPI and I2S). This capability flag excludes any external PSRAM.




### 32-Bit Accessible Memory


If a certain memory structure is only addressed in 32-bit units, for example, an array of ints or pointers, it can be useful to allocate it with the `MALLOC_CAP_32BIT` flag. This also allows the allocator to give out IRAM memory, which is sometimes unavailable for a normal `malloc()` call. This can help to use all the available memory in the ESP32-S3.


Please note that on ESP32-S3 series chips, `MALLOC_CAP_32BIT` cannot be used for storing floating-point variables. This is because `MALLOC_CAP_32BIT` may return instruction RAM and the floating-point assembly instructions on ESP32-S3 cannot access instruction RAM.


Memory allocated with `MALLOC_CAP_32BIT` can **only** be accessed via 32-bit reads and writes, any other type of access will generate a fatal LoadStoreError exception.




### External SPI Memory


When external RAM is enabled, external SPI RAM can be allocated using standard `malloc` calls, or via `heap_caps_malloc(MALLOC_CAP_SPIRAM)`, depending on the configuration. See Configuring External RAM for more details.





## Thread Safety


Heap functions are thread-safe, meaning they can be called from different tasks simultaneously without any limitations.


It is technically possible to call `malloc`, `free`, and related functions from interrupt handler (ISR) context (see Calling Heap-Related Functions from ISR). However, this is not recommended, as heap function calls may delay other interrupts. It is strongly recommended to refactor applications so that any buffers used by an ISR are pre-allocated outside of the ISR. Support for calling heap functions from ISRs may be removed in a future update.




## Calling Heap-Related Functions from ISR


The following functions from the heap component can be called from the interrupt handler (ISR):


* `heap_caps_malloc()`
* `heap_caps_malloc_default()`
* `heap_caps_realloc_default()`
* `heap_caps_malloc_prefer()`
* `heap_caps_realloc_prefer()`
* `heap_caps_calloc_prefer()`
* `heap_caps_free()`
* `heap_caps_realloc()`
* `heap_caps_calloc()`
* `heap_caps_aligned_alloc()`
* `heap_caps_aligned_free()`



Note


However, this practice is strongly discouraged.





## Heap Tracing & Debugging


The following features are documented on the Heap Memory Debugging page:


* Heap Information (free space, etc.)
* Heap Allocation and Free Function Hooks
* Heap Corruption Detection
* Heap Tracing (memory leak detection, monitoring, etc.)




## Implementation Notes


Knowledge about the regions of memory in the chip comes from the "SoC" component, which contains memory layout information for the chip, and the different capabilities of each region. Each region's capabilities are prioritized, so that (for example) dedicated DRAM and IRAM regions are used for allocations ahead of the more versatile D/IRAM regions.


Each contiguous region of memory contains its own memory heap. The heaps are created using the multi_heap functionality. `multi_heap` allows any contiguous region of memory to be used as a heap.


The heap capabilities allocator uses knowledge of the memory regions to initialize each individual heap. Allocation functions in the heap capabilities API will find the most appropriate heap for the allocation based on desired capabilities, available space, and preferences for each region's use, and then calling `multi_heap_malloc()` for the heap situated in that particular region.


Calling `free()` involves finding the particular heap corresponding to the freed address, and then call `multi_heap_free()` on that particular `multi_heap` instance.




## API Reference - Heap Allocation



### Header File


* components/heap/include/esp_heap_caps.h
* This header file can be included with:



> ```
> #include "esp_heap_caps.h"
> 
> ```




### Functions


#### heap_caps_register_failed_alloc_callback


```


esp_err_t heap_caps_register_failed_alloc_callback(esp_alloc_failed_hook_t callback)
registers a callback function to be invoked if a memory allocation operation fails 

Parameters
callback -- caller defined callback to be invoked 

Returns
ESP_OK if callback was registered. 



```

#### heap_caps_malloc


```


void *heap_caps_malloc(size_t size, uint32_t caps)
Allocate a chunk of memory which has the given capabilities. 
Equivalent semantics to libc malloc(), for capability-aware memory.

Parameters

size -- Size, in bytes, of the amount of memory to allocate 
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory to be returned


Returns
A pointer to the memory allocated on success, NULL on failure 



```

#### heap_caps_free


```


void heap_caps_free(void *ptr)
Free memory previously allocated via heap_caps_malloc() or heap_caps_realloc(). 
Equivalent semantics to libc free(), for capability-aware memory.
In IDF, free(p) is equivalent to heap_caps_free(p).

Parameters
ptr -- Pointer to memory previously returned from heap_caps_malloc() or heap_caps_realloc(). Can be NULL. 



```

#### heap_caps_realloc


```


void *heap_caps_realloc(void *ptr, size_t size, uint32_t caps)
Reallocate memory previously allocated via heap_caps_malloc() or heap_caps_realloc(). 
Equivalent semantics to libc realloc(), for capability-aware memory.
In IDF, realloc(p, s) is equivalent to heap_caps_realloc(p, s, MALLOC_CAP_8BIT).
'caps' parameter can be different to the capabilities that any original 'ptr' was allocated with. In this way, realloc can be used to "move" a buffer if necessary to ensure it meets a new set of capabilities.

Parameters

ptr -- Pointer to previously allocated memory, or NULL for a new allocation. 
size -- Size of the new buffer requested, or 0 to free the buffer. 
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory desired for the new allocation.


Returns
Pointer to a new buffer of size 'size' with capabilities 'caps', or NULL if allocation failed. 



```

#### heap_caps_aligned_alloc


```


void *heap_caps_aligned_alloc(size_t alignment, size_t size, uint32_t caps)
Allocate an aligned chunk of memory which has the given capabilities. 
Equivalent semantics to libc aligned_alloc(), for capability-aware memory. 

Parameters

alignment -- How the pointer received needs to be aligned must be a power of two 
size -- Size, in bytes, of the amount of memory to allocate 
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory to be returned


Returns
A pointer to the memory allocated on success, NULL on failure 



```

#### heap_caps_aligned_free


```


void heap_caps_aligned_free(void *ptr)
Used to deallocate memory previously allocated with heap_caps_aligned_alloc. 

Note
This function is deprecated, please consider using heap_caps_free() instead 


Parameters
ptr -- Pointer to the memory allocated 



```

#### heap_caps_aligned_calloc


```


void *heap_caps_aligned_calloc(size_t alignment, size_t n, size_t size, uint32_t caps)
Allocate an aligned chunk of memory which has the given capabilities. The initialized value in the memory is set to zero. 

Parameters

alignment -- How the pointer received needs to be aligned must be a power of two 
n -- Number of continuing chunks of memory to allocate 
size -- Size, in bytes, of a chunk of memory to allocate 
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory to be returned


Returns
A pointer to the memory allocated on success, NULL on failure 



```

#### heap_caps_calloc


```


void *heap_caps_calloc(size_t n, size_t size, uint32_t caps)
Allocate a chunk of memory which has the given capabilities. The initialized value in the memory is set to zero. 
Equivalent semantics to libc calloc(), for capability-aware memory.
In IDF, calloc(p) is equivalent to heap_caps_calloc(p, MALLOC_CAP_8BIT).

Parameters

n -- Number of continuing chunks of memory to allocate 
size -- Size, in bytes, of a chunk of memory to allocate 
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory to be returned


Returns
A pointer to the memory allocated on success, NULL on failure 



```

#### heap_caps_get_total_size


```


size_t heap_caps_get_total_size(uint32_t caps)
Get the total size of all the regions that have the given capabilities. 
This function takes all regions capable of having the given capabilities allocated in them and adds up the total space they have.

Parameters
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory

Returns
total size in bytes 



```

#### heap_caps_get_free_size


```


size_t heap_caps_get_free_size(uint32_t caps)
Get the total free size of all the regions that have the given capabilities. 
This function takes all regions capable of having the given capabilities allocated in them and adds up the free space they have.

Note
Note that because of heap fragmentation it is probably not possible to allocate a single block of memory of this size. Use heap_caps_get_largest_free_block() for this purpose.


Parameters
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory

Returns
Amount of free bytes in the regions 



```

#### heap_caps_get_minimum_free_size


```


size_t heap_caps_get_minimum_free_size(uint32_t caps)
Get the total minimum free memory of all regions with the given capabilities. 
This adds all the low watermarks of the regions capable of delivering the memory with the given capabilities.

Note
Note the result may be less than the global all-time minimum available heap of this kind, as "low watermarks" are tracked per-region. Individual regions' heaps may have reached their "low watermarks" at different points in time. However, this result still gives a "worst case" indication for all-time minimum free heap.


Parameters
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory

Returns
Amount of free bytes in the regions 



```

#### heap_caps_get_largest_free_block


```


size_t heap_caps_get_largest_free_block(uint32_t caps)
Get the largest free block of memory able to be allocated with the given capabilities. 
Returns the largest value of s for which heap_caps_malloc(s, caps) will succeed.

Parameters
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory

Returns
Size of the largest free block in bytes. 



```

#### heap_caps_monitor_local_minimum_free_size_start


```


esp_err_t heap_caps_monitor_local_minimum_free_size_start(void)
Start monitoring the value of minimum_free_bytes from the moment this function is called instead of from startup. 

Note
This allows to detect local lows of the minimum_free_bytes value that wouldn't be detected otherwise.


Returns
esp_err_t ESP_OK if the function executed properly ESP_FAIL if called when monitoring already active 



```

#### heap_caps_monitor_local_minimum_free_size_stop


```


esp_err_t heap_caps_monitor_local_minimum_free_size_stop(void)
Stop monitoring the value of minimum_free_bytes. After this call the minimum_free_bytes value calculated from startup will be returned in heap_caps_get_info and heap_caps_get_minimum_free_size. 

Returns
esp_err_t ESP_OK if the function executed properly ESP_FAIL if called when monitoring not active 



```

#### heap_caps_get_info


```


void heap_caps_get_info(multi_heap_info_t *info, uint32_t caps)
Get heap info for all regions with the given capabilities. 
Calls multi_heap_info() on all heaps which share the given capabilities. The information returned is an aggregate across all matching heaps. The meanings of fields are the same as defined for multi_heap_info_t, except that minimum_free_bytes has the same caveats described in heap_caps_get_minimum_free_size().

Parameters

info -- Pointer to a structure which will be filled with relevant heap metadata. 
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory 




```

#### heap_caps_print_heap_info


```


void heap_caps_print_heap_info(uint32_t caps)
Print a summary of all memory with the given capabilities. 
Calls multi_heap_info on all heaps which share the given capabilities, and prints a two-line summary for each, then a total summary.

Parameters
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory 



```

#### heap_caps_check_integrity_all


```


bool heap_caps_check_integrity_all(bool print_errors)
Check integrity of all heap memory in the system. 
Calls multi_heap_check on all heaps. Optionally print errors if heaps are corrupt.
Calling this function is equivalent to calling heap_caps_check_integrity with the caps argument set to MALLOC_CAP_INVALID.

Note
Please increase the value of CONFIG_ESP_INT_WDT_TIMEOUT_MS when using this API with PSRAM enabled.


Parameters
print_errors -- Print specific errors if heap corruption is found.

Returns
True if all heaps are valid, False if at least one heap is corrupt. 



```

#### heap_caps_check_integrity


```


bool heap_caps_check_integrity(uint32_t caps, bool print_errors)
Check integrity of all heaps with the given capabilities. 
Calls multi_heap_check on all heaps which share the given capabilities. Optionally print errors if the heaps are corrupt.
See also heap_caps_check_integrity_all to check all heap memory in the system and heap_caps_check_integrity_addr to check memory around a single address.

Note
Please increase the value of CONFIG_ESP_INT_WDT_TIMEOUT_MS when using this API with PSRAM capability flag.


Parameters

caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory 
print_errors -- Print specific errors if heap corruption is found.


Returns
True if all heaps are valid, False if at least one heap is corrupt. 



```

#### heap_caps_check_integrity_addr


```


bool heap_caps_check_integrity_addr(intptr_t addr, bool print_errors)
Check integrity of heap memory around a given address. 
This function can be used to check the integrity of a single region of heap memory, which contains the given address.
This can be useful if debugging heap integrity for corruption at a known address, as it has a lower overhead than checking all heap regions. Note that if the corrupt address moves around between runs (due to timing or other factors) then this approach won't work, and you should call heap_caps_check_integrity or heap_caps_check_integrity_all instead.

Note
The entire heap region around the address is checked, not only the adjacent heap blocks.


Parameters

addr -- Address in memory. Check for corruption in region containing this address. 
print_errors -- Print specific errors if heap corruption is found.


Returns
True if the heap containing the specified address is valid, False if at least one heap is corrupt or the address doesn't belong to a heap region. 



```

#### heap_caps_malloc_extmem_enable


```


void heap_caps_malloc_extmem_enable(size_t limit)
Enable malloc() in external memory and set limit below which malloc() attempts are placed in internal memory. 
When external memory is in use, the allocation strategy is to initially try to satisfy smaller allocation requests with internal memory and larger requests with external memory. This sets the limit between the two, as well as generally enabling allocation in external memory.

Parameters
limit -- Limit, in bytes. 



```

#### heap_caps_malloc_prefer


```


void *heap_caps_malloc_prefer(size_t size, size_t num, ...)
Allocate a chunk of memory as preference in decreasing order. 

AttentionThe variable parameters are bitwise OR of MALLOC_CAP_* flags indicating the type of memory. This API prefers to allocate memory with the first parameter. If failed, allocate memory with the next parameter. It will try in this order until allocating a chunk of memory successfully or fail to allocate memories with any of the parameters.




Parameters

size -- Size, in bytes, of the amount of memory to allocate 
num -- Number of variable parameters


Returns
A pointer to the memory allocated on success, NULL on failure 



```

#### heap_caps_realloc_prefer


```


void *heap_caps_realloc_prefer(void *ptr, size_t size, size_t num, ...)
Reallocate a chunk of memory as preference in decreasing order. 

Parameters

ptr -- Pointer to previously allocated memory, or NULL for a new allocation. 
size -- Size of the new buffer requested, or 0 to free the buffer. 
num -- Number of variable parameters


Returns
Pointer to a new buffer of size 'size', or NULL if allocation failed. 



```

#### heap_caps_calloc_prefer


```


void *heap_caps_calloc_prefer(size_t n, size_t size, size_t num, ...)
Allocate a chunk of memory as preference in decreasing order. 

Parameters

n -- Number of continuing chunks of memory to allocate 
size -- Size, in bytes, of a chunk of memory to allocate 
num -- Number of variable parameters


Returns
A pointer to the memory allocated on success, NULL on failure 



```

#### heap_caps_dump


```


void heap_caps_dump(uint32_t caps)
Dump the full structure of all heaps with matching capabilities. 
Prints a large amount of output to serial (because of locking limitations, the output bypasses stdout/stderr). For each (variable sized) block in each matching heap, the following output is printed on a single line:

Block address (the data buffer returned by malloc is 4 bytes after this if heap debugging is set to Basic, or 8 bytes otherwise).
Data size (the data size may be larger than the size requested by malloc, either due to heap fragmentation or because of heap debugging level).
Address of next block in the heap.
If the block is free, the address of the next free block is also printed.



Parameters
caps -- Bitwise OR of MALLOC_CAP_* flags indicating the type of memory 



```

#### heap_caps_dump_all


```


void heap_caps_dump_all(void)
Dump the full structure of all heaps. 
Covers all registered heaps. Prints a large amount of output to serial.
Output is the same as for heap_caps_dump. 

```

#### heap_caps_get_allocated_size


```


size_t heap_caps_get_allocated_size(void *ptr)
Return the size that a particular pointer was allocated with. 

Note
The app will crash with an assertion failure if the pointer is not valid.


Parameters
ptr -- Pointer to currently allocated heap memory. Must be a pointer value previously returned by heap_caps_malloc, malloc, calloc, etc. and not yet freed.

Returns
Size of the memory allocated at this block. 



```

#### heap_caps_walk


```


void heap_caps_walk(uint32_t caps, heap_caps_walker_cb_t walker_func, void *user_data)
Function called to walk through the heaps with the given set of capabilities. 

Parameters

caps -- The set of capabilities assigned to the heaps to walk through 
walker_func -- Callback called for each block of the heaps being traversed 
user_data -- Opaque pointer to user defined data 




```

#### heap_caps_walk_all


```


void heap_caps_walk_all(heap_caps_walker_cb_t walker_func, void *user_data)
Function called to walk through all heaps defined by the heap component. 

Parameters

walker_func -- Callback called for each block of the heaps being traversed 
user_data -- Opaque pointer to user defined data 




```



### Structures


#### walker_heap_info


```


struct walker_heap_info
Structure used to store heap related data passed to the walker callback function. 

Public Members
start

intptr_t start
Start address of the heap in which the block is located. 

end

intptr_t end
End address of the heap in which the block is located. 



```

#### walker_block_info


```


struct walker_block_info
Structure used to store block related data passed to the walker callback function. 

Public Members
ptr

void *ptr
Pointer to the block data. 

size

size_t size
The size of the block. 

used

bool used
Block status. True: used, False: free. 



```



### Macros


#### HEAP_IRAM_ATTR


```


HEAP_IRAM_ATTR

```

#### MALLOC_CAP_EXEC


```


MALLOC_CAP_EXEC
Flags to indicate the capabilities of the various memory systems. 
Memory must be able to run executable code 

```

#### MALLOC_CAP_32BIT


```


MALLOC_CAP_32BIT
Memory must allow for aligned 32-bit data accesses. 

```

#### MALLOC_CAP_8BIT


```


MALLOC_CAP_8BIT
Memory must allow for 8/16/...-bit data accesses. 

```

#### MALLOC_CAP_DMA


```


MALLOC_CAP_DMA
Memory must be able to accessed by DMA. 

```

#### MALLOC_CAP_PID2


```


MALLOC_CAP_PID2
Memory must be mapped to PID2 memory space (PIDs are not currently used) 

```

#### MALLOC_CAP_PID3


```


MALLOC_CAP_PID3
Memory must be mapped to PID3 memory space (PIDs are not currently used) 

```

#### MALLOC_CAP_PID4


```


MALLOC_CAP_PID4
Memory must be mapped to PID4 memory space (PIDs are not currently used) 

```

#### MALLOC_CAP_PID5


```


MALLOC_CAP_PID5
Memory must be mapped to PID5 memory space (PIDs are not currently used) 

```

#### MALLOC_CAP_PID6


```


MALLOC_CAP_PID6
Memory must be mapped to PID6 memory space (PIDs are not currently used) 

```

#### MALLOC_CAP_PID7


```


MALLOC_CAP_PID7
Memory must be mapped to PID7 memory space (PIDs are not currently used) 

```

#### MALLOC_CAP_SPIRAM


```


MALLOC_CAP_SPIRAM
Memory must be in SPI RAM. 

```

#### MALLOC_CAP_INTERNAL


```


MALLOC_CAP_INTERNAL
Memory must be internal; specifically it should not disappear when flash/spiram cache is switched off. 

```

#### MALLOC_CAP_DEFAULT


```


MALLOC_CAP_DEFAULT
Memory can be returned in a non-capability-specific memory allocation (e.g. malloc(), calloc()) call. 

```

#### MALLOC_CAP_IRAM_8BIT


```


MALLOC_CAP_IRAM_8BIT
Memory must be in IRAM and allow unaligned access. 

```

#### MALLOC_CAP_RETENTION


```


MALLOC_CAP_RETENTION
Memory must be able to accessed by retention DMA. 

```

#### MALLOC_CAP_RTCRAM


```


MALLOC_CAP_RTCRAM
Memory must be in RTC fast memory. 

```

#### MALLOC_CAP_TCM


```


MALLOC_CAP_TCM
Memory must be in TCM memory. 

```

#### MALLOC_CAP_DMA_DESC_AHB


```


MALLOC_CAP_DMA_DESC_AHB
Memory must be capable of containing AHB DMA descriptors. 

```

#### MALLOC_CAP_DMA_DESC_AXI


```


MALLOC_CAP_DMA_DESC_AXI
Memory must be capable of containing AXI DMA descriptors. 

```

#### MALLOC_CAP_CACHE_ALIGNED


```


MALLOC_CAP_CACHE_ALIGNED
Memory must be aligned to the cache line size of any intermediate caches. 

```

#### MALLOC_CAP_INVALID


```


MALLOC_CAP_INVALID
Memory can't be used / list end marker. 

```



### Type Definitions


#### esp_alloc_failed_hook_t


```


typedef void (*esp_alloc_failed_hook_t)(size_t size, uint32_t caps, const char *function_name)
callback called when an allocation operation fails, if registered 

Param size
in bytes of failed allocation 

Param caps
capabilities requested of failed allocation 

Param function_name
function which generated the failure 



```

#### walker_heap_into_t


```


typedef struct walker_heap_info walker_heap_into_t
Structure used to store heap related data passed to the walker callback function. 

```

#### walker_block_info_t


```


typedef struct walker_block_info walker_block_info_t
Structure used to store block related data passed to the walker callback function. 

```

#### heap_caps_walker_cb_t


```


typedef bool (*heap_caps_walker_cb_t)(walker_heap_into_t heap_info, walker_block_info_t block_info, void *user_data)
Function callback used to get information of memory block during calls to heap_caps_walk or heap_caps_walk_all. 

Param heap_info
See walker_heap_into_t 

Param block_info
See walker_block_info_t 

Param user_data
Opaque pointer to user defined data

Return
True to proceed with the heap traversal False to stop the traversal of the current heap and continue with the traversal of the next heap (if any) 



```




## API Reference - Initialisation



### Header File


* components/heap/include/esp_heap_caps_init.h
* This header file can be included with:



> ```
> #include "esp_heap_caps_init.h"
> 
> ```




### Functions


#### heap_caps_init


```


void heap_caps_init(void)
Initialize the capability-aware heap allocator. 
This is called once in the IDF startup code. Do not call it at other times. 

```

#### heap_caps_enable_nonos_stack_heaps


```


void heap_caps_enable_nonos_stack_heaps(void)
Enable heap(s) in memory regions where the startup stacks are located. 
On startup, the pro/app CPUs have a certain memory region they use as stack, so we cannot do allocations in the regions these stack frames are. When FreeRTOS is completely started, they do not use that memory anymore and heap(s) there can be enabled. 

```

#### heap_caps_add_region


```


esp_err_t heap_caps_add_region(intptr_t start, intptr_t end)
Add a region of memory to the collection of heaps at runtime. 
Most memory regions are defined in soc_memory_layout.c for the SoC, and are registered via heap_caps_init(). Some regions can't be used immediately and are later enabled via heap_caps_enable_nonos_stack_heaps().
Call this function to add a region of memory to the heap at some later time.
This function does not consider any of the "reserved" regions or other data in soc_memory_layout, caller needs to consider this themselves.
All memory within the region specified by start & end parameters must be otherwise unused.
The capabilities of the newly registered memory will be determined by the start address, as looked up in the regions specified in soc_memory_layout.c.
Use heap_caps_add_region_with_caps() to register a region with custom capabilities.

Note
Please refer to following example for memory regions allowed for addition to heap based on an existing region (address range for demonstration purpose only):       Existing region: 0x1000 <-> 0x3000
      New region:      0x1000 <-> 0x3000 (Allowed)
      New region:      0x1000 <-> 0x2000 (Allowed)
      New region:      0x0000 <-> 0x1000 (Allowed)
      New region:      0x3000 <-> 0x4000 (Allowed)
      New region:      0x0000 <-> 0x2000 (NOT Allowed)
      New region:      0x0000 <-> 0x4000 (NOT Allowed)
      New region:      0x1000 <-> 0x4000 (NOT Allowed)
      New region:      0x2000 <-> 0x4000 (NOT Allowed)





Parameters

start -- Start address of new region. 
end -- End address of new region.


Returns
ESP_OK on success, ESP_ERR_INVALID_ARG if a parameter is invalid, ESP_ERR_NOT_FOUND if the specified start address doesn't reside in a known region, or any error returned by heap_caps_add_region_with_caps(). 



```

#### heap_caps_add_region_with_caps


```


esp_err_t heap_caps_add_region_with_caps(const uint32_t caps[], intptr_t start, intptr_t end)
Add a region of memory to the collection of heaps at runtime, with custom capabilities. 
Similar to heap_caps_add_region(), only custom memory capabilities are specified by the caller.

Note
Please refer to following example for memory regions allowed for addition to heap based on an existing region (address range for demonstration purpose only):       Existing region: 0x1000 <-> 0x3000
      New region:      0x1000 <-> 0x3000 (Allowed)
      New region:      0x1000 <-> 0x2000 (Allowed)
      New region:      0x0000 <-> 0x1000 (Allowed)
      New region:      0x3000 <-> 0x4000 (Allowed)
      New region:      0x0000 <-> 0x2000 (NOT Allowed)
      New region:      0x0000 <-> 0x4000 (NOT Allowed)
      New region:      0x1000 <-> 0x4000 (NOT Allowed)
      New region:      0x2000 <-> 0x4000 (NOT Allowed)





Parameters

caps -- Ordered array of capability masks for the new region, in order of priority. Must have length SOC_MEMORY_TYPE_NO_PRIOS. Does not need to remain valid after the call returns. 
start -- Start address of new region. 
end -- End address of new region.


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if a parameter is invalid
ESP_ERR_NO_MEM if no memory to register new heap.
ESP_ERR_INVALID_SIZE if the memory region is too small to fit a heap
ESP_FAIL if region overlaps the start and/or end of an existing region 





```




## API Reference - Multi-Heap API


(Note: The multi-heap API is used internally by the heap capabilities allocator. Most ESP-IDF programs never need to call this API directly.)



### Header File


* components/heap/include/multi_heap.h
* This header file can be included with:



> ```
> #include "multi_heap.h"
> 
> ```




### Functions


#### multi_heap_aligned_alloc


```


void *multi_heap_aligned_alloc(multi_heap_handle_t heap, size_t size, size_t alignment)
allocate a chunk of memory with specific alignment 

Parameters

heap -- Handle to a registered heap. 
size -- size in bytes of memory chunk 
alignment -- how the memory must be aligned


Returns
pointer to the memory allocated, NULL on failure 



```

#### multi_heap_malloc


```


void *multi_heap_malloc(multi_heap_handle_t heap, size_t size)
malloc() a buffer in a given heap 
Semantics are the same as standard malloc(), only the returned buffer will be allocated in the specified heap.

Parameters

heap -- Handle to a registered heap. 
size -- Size of desired buffer.


Returns
Pointer to new memory, or NULL if allocation fails. 



```

#### multi_heap_aligned_free


```


void multi_heap_aligned_free(multi_heap_handle_t heap, void *p)
free() a buffer aligned in a given heap. 

Note
This function is deprecated, consider using multi_heap_free() instead 


Parameters

heap -- Handle to a registered heap. 
p -- NULL, or a pointer previously returned from multi_heap_aligned_alloc() for the same heap. 




```

#### multi_heap_free


```


void multi_heap_free(multi_heap_handle_t heap, void *p)
free() a buffer in a given heap. 
Semantics are the same as standard free(), only the argument 'p' must be NULL or have been allocated in the specified heap.

Parameters

heap -- Handle to a registered heap. 
p -- NULL, or a pointer previously returned from multi_heap_malloc() or multi_heap_realloc() for the same heap. 




```

#### multi_heap_realloc


```


void *multi_heap_realloc(multi_heap_handle_t heap, void *p, size_t size)
realloc() a buffer in a given heap. 
Semantics are the same as standard realloc(), only the argument 'p' must be NULL or have been allocated in the specified heap.

Parameters

heap -- Handle to a registered heap. 
p -- NULL, or a pointer previously returned from multi_heap_malloc() or multi_heap_realloc() for the same heap. 
size -- Desired new size for buffer.


Returns
New buffer of 'size' containing contents of 'p', or NULL if reallocation failed. 



```

#### multi_heap_get_allocated_size


```


size_t multi_heap_get_allocated_size(multi_heap_handle_t heap, void *p)
Return the size that a particular pointer was allocated with. 

Parameters

heap -- Handle to a registered heap. 
p -- Pointer, must have been previously returned from multi_heap_malloc() or multi_heap_realloc() for the same heap.


Returns
Size of the memory allocated at this block. May be more than the original size argument, due to padding and minimum block sizes. 



```

#### multi_heap_register


```


multi_heap_handle_t multi_heap_register(void *start, size_t size)
Register a new heap for use. 
This function initialises a heap at the specified address, and returns a handle for future heap operations.
There is no equivalent function for deregistering a heap - if all blocks in the heap are free, you can immediately start using the memory for other purposes.

Parameters

start -- Start address of the memory to use for a new heap. 
size -- Size (in bytes) of the new heap.


Returns
Handle of a new heap ready for use, or NULL if the heap region was too small to be initialised. 



```

#### multi_heap_set_lock


```


void multi_heap_set_lock(multi_heap_handle_t heap, void *lock)
Associate a private lock pointer with a heap. 
The lock argument is supplied to the MULTI_HEAP_LOCK() and MULTI_HEAP_UNLOCK() macros, defined in multi_heap_platform.h.
The lock in question must be recursive.
When the heap is first registered, the associated lock is NULL.

Parameters

heap -- Handle to a registered heap. 
lock -- Optional pointer to a locking structure to associate with this heap. 




```

#### multi_heap_dump


```


void multi_heap_dump(multi_heap_handle_t heap)
Dump heap information to stdout. 
For debugging purposes, this function dumps information about every block in the heap to stdout.

Parameters
heap -- Handle to a registered heap. 



```

#### multi_heap_check


```


bool multi_heap_check(multi_heap_handle_t heap, bool print_errors)
Check heap integrity. 
Walks the heap and checks all heap data structures are valid. If any errors are detected, an error-specific message can be optionally printed to stderr. Print behaviour can be overridden at compile time by defining MULTI_CHECK_FAIL_PRINTF in multi_heap_platform.h.

Note
This function is not thread-safe as it sets a global variable with the value of print_errors.


Parameters

heap -- Handle to a registered heap. 
print_errors -- If true, errors will be printed to stderr. 


Returns
true if heap is valid, false otherwise. 



```

#### multi_heap_free_size


```


size_t multi_heap_free_size(multi_heap_handle_t heap)
Return free heap size. 
Returns the number of bytes available in the heap.
Equivalent to the total_free_bytes member returned by multi_heap_get_heap_info().
Note that the heap may be fragmented, so the actual maximum size for a single malloc() may be lower. To know this size, see the largest_free_block member returned by multi_heap_get_heap_info().

Parameters
heap -- Handle to a registered heap. 

Returns
Number of free bytes. 



```

#### multi_heap_minimum_free_size


```


size_t multi_heap_minimum_free_size(multi_heap_handle_t heap)
Return the lifetime minimum free heap size. 
Equivalent to the minimum_free_bytes member returned by multi_heap_get_info().
Returns the lifetime "low watermark" of possible values returned from multi_free_heap_size(), for the specified heap.

Parameters
heap -- Handle to a registered heap. 

Returns
Number of free bytes. 



```

#### multi_heap_get_info


```


void multi_heap_get_info(multi_heap_handle_t heap, multi_heap_info_t *info)
Return metadata about a given heap. 
Fills a multi_heap_info_t structure with information about the specified heap.

Parameters

heap -- Handle to a registered heap. 
info -- Pointer to a structure to fill with heap metadata. 




```

#### multi_heap_aligned_alloc_offs


```


void *multi_heap_aligned_alloc_offs(multi_heap_handle_t heap, size_t size, size_t alignment, size_t offset)
Perform an aligned allocation from the provided offset. 

Parameters

heap -- The heap in which to perform the allocation 
size -- The size of the allocation 
alignment -- How the memory must be aligned 
offset -- The offset at which the alignment should start 


Returns
void* The ptr to the allocated memory 



```

#### multi_heap_reset_minimum_free_bytes


```


size_t multi_heap_reset_minimum_free_bytes(multi_heap_handle_t heap)
Reset the minimum_free_bytes value (setting it to free_bytes) and return the former value. 

Parameters
heap -- The heap in which the reset is taking place 

Returns
size_t the value of minimum_free_bytes before it is reset 



```

#### multi_heap_restore_minimum_free_bytes


```


void multi_heap_restore_minimum_free_bytes(multi_heap_handle_t heap, const size_t new_minimum_free_bytes_value)
Set the value of minimum_free_bytes to new_minimum_free_bytes_value or keep the current value of minimum_free_bytes if it is smaller than new_minimum_free_bytes_value. 

Parameters

heap -- The heap in which the restore is taking place 
new_minimum_free_bytes_value -- The value to restore the minimum_free_bytes to 




```

#### multi_heap_walk


```


void multi_heap_walk(multi_heap_handle_t heap, multi_heap_walker_cb_t walker_func, void *user_data)
Call the tlsf_walk_pool function of the heap given as parameter with the walker function passed as parameter. 

Parameters

heap -- The heap to traverse 
walker_func -- The walker to trigger on each block of the heap 
user_data -- Opaque pointer to user defined data 




```



### Structures


#### multi_heap_info_t


```


struct multi_heap_info_t
Structure to access heap metadata via multi_heap_get_info. 

Public Members
total_free_bytes

size_t total_free_bytes
Total free bytes in the heap. Equivalent to multi_free_heap_size(). 

total_allocated_bytes

size_t total_allocated_bytes
Total bytes allocated to data in the heap. 

largest_free_block

size_t largest_free_block
Size of the largest free block in the heap. This is the largest malloc-able size. 

minimum_free_bytes

size_t minimum_free_bytes
Lifetime minimum free heap size. Equivalent to multi_minimum_free_heap_size(). 

allocated_blocks

size_t allocated_blocks
Number of (variable size) blocks allocated in the heap. 

free_blocks

size_t free_blocks
Number of (variable size) free blocks in the heap. 

total_blocks

size_t total_blocks
Total number of (variable size) blocks in the heap. 



```



### Type Definitions


#### multi_heap_handle_t


```


typedef struct multi_heap_info *multi_heap_handle_t
Opaque handle to a registered heap. 

```

#### multi_heap_walker_cb_t


```


typedef bool (*multi_heap_walker_cb_t)(void *block_ptr, size_t block_size, int block_used, void *user_data)
Callback called when walking the given heap blocks of memory. 

Param block_ptr
Pointer to the block data 

Param block_size
The size of the block 

Param block_used
Block status. 0: free, 1: allocated 

Param user_data
Opaque pointer to user defined data

Return
True if the walker is expected to continue the heap traversal False if the walker is expected to stop the traversal of the heap 



```