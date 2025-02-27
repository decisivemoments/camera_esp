# SPI Flash API




## Overview


The spi_flash component contains API functions related to reading, writing, erasing, and memory mapping for data in the external flash.


For higher-level API functions which work with partitions defined in the partition table, see Partitions API



Note


`esp_partition_*` APIs are recommended to be used instead of the lower level `esp_flash_*` API functions when accessing the main SPI flash chip, since they conduct bounds checking and are guaranteed to calculate correct offsets in flash based on the information in the partition table. `esp_flash_*` functions can still be used directly when accessing an external (secondary) SPI flash chip.



Different from the API before ESP-IDF v4.0, the functionality of `esp_flash_*` APIs is not limited to the "main" SPI flash chip (the same SPI flash chip from which program runs). With different chip pointers, you can access external flash chips connected to not only SPI0/1 but also other SPI buses like SPI2.



Note


Instead of going through the cache connected to the SPI0 peripheral, most `esp_flash_*` APIs go through other SPI peripherals like SPI1, SPI2, etc. This makes them able to access not only the main flash, but also external (secondary) flash.


However, due to the limitations of the cache, operations through the cache are limited to the main flash. The address range limitation for these operations is also on the cache side. The cache is not able to access external flash chips or address range above its capabilities. These cache operations include: mmap, encrypted read/write, executing code or access to variables in the flash.




Note


Flash APIs after ESP-IDF v4.0 are no longer **atomic**. If a write operation occurs during another on-going read operation, and the flash addresses of both operations overlap, the data returned from the read operation may contain both old data and new data (that was updated written by the write operation).




Note


Encrypted flash operations are only supported with the main flash chip (and not with other flash chips, that is on SPI1 with different CS, or on other SPI buses). Reading through cache is only supported on the main flash, which is determined by the HW.





## Support for Features of Flash Chips



### Quad/Dual Mode Chips


Features of different flashes are implemented in different ways and thus need special support. The fast/slow read and Dual mode (DOUT/DIO) of almost all flashes with 24-bit address are supported, because they do not need any vendor-specific commands.


Quad mode (QIO/QOUT) is supported on the following chip types:


1. ISSI
2. GD
3. MXIC
4. FM
5. Winbond
6. XMC
7. BOYA



Note


Only when one flash series listed above is supported by ESP32-S3, this flash series is supported by the chip driver by default. You can use `Component config` > `SPI Flash driver` > `Auto-detect flash chips` in menuconfig to enable/disable a flash series.





### Optional Features




There are some features that are not supported by all flash chips, or not supported by all Espressif chips. These features include:


* OPI flash - means that flash supports octal mode.


* 32-bit address flash - usually means that the flash has higher capacity (equal to or larger than 16 MB) that needs longer addresses.


* High performance mode (HPM) - means that flash works under high frequency which is higher than 80 MHz.


* Flash unique ID - means that flash supports its unique 64-bit ID.


* Suspend & Resume - means that flash can accept suspend/resume command during its writing/erasing. The ESP32-S3 may keep the cache on when the flash is being written/erased and suspend it to read its contents randomly.


If you want to use these features, please ensure both ESP32-S3 and ALL flash chips in your product support these features. For more details, refer to Optional Features for Flash.


You may also customise your own flash chip driver. See Overriding Default Chip Drivers for more details.







## Initializing a Flash Device


To use the `esp_flash_*` APIs, you need to initialise a flash chip on a certain SPI bus, as shown below:


1. Call `spi_bus_initialize()` to properly initialize an SPI bus. This function initializes the resources (I/O, DMA, interrupts) shared among devices attached to this bus.
2. Call `spi_bus_add_flash_device()` to attach the flash device to the bus. This function allocates memory and fills the members for the `esp_flash_t` structure. The CS I/O is also initialized here.
3. Call `esp_flash_init()` to actually communicate with the chip. This also detects the chip type, and influence the following operations.



Note


Multiple flash chips can be attached to the same bus now.





## SPI Flash Access API


This is the set of API functions for working with data in flash:


* `esp_flash_read()` reads data from flash to RAM
* `esp_flash_write()` writes data from RAM to flash
* `esp_flash_erase_region()` erases specific region of flash
* `esp_flash_erase_chip()` erases the whole flash
* `esp_flash_get_chip_size()` returns flash chip size, in bytes, as configured in menuconfig


Generally, try to avoid using the raw SPI flash functions to the "main" SPI flash chip in favour of partition-specific functions.




## SPI Flash Size


The SPI flash size is configured by writing a field in the software bootloader image header, flashed at offset 0x1000.


By default, the SPI flash size is detected by `esptool.py` when this bootloader is written to flash, and the header is updated with the correct size. Alternatively, it is possible to generate a fixed flash size by setting CONFIG_ESPTOOLPY_FLASHSIZE in the project configuration.


If it is necessary to override the configured flash size at runtime, it is possible to set the `chip_size` member of the `g_rom_flashchip` structure. This size is used by `esp_flash_*` functions (in both software & ROM) to check the bounds.




## Concurrency Constraints for Flash on SPI1





Attention


The SPI0/1 bus is shared between the instruction & data cache (for firmware execution) and the SPI1 peripheral (controlled by the drivers including this SPI flash driver). Hence, calling SPI Flash API on SPI1 bus (including the main flash) causes significant influence to the whole system. See Concurrency Constraints for Flash on SPI1 for more details.





## SPI Flash Encryption


It is possible to encrypt the contents of SPI flash and have it transparently decrypted by hardware.


Refer to the Flash Encryption documentation for more details.




## Memory Mapping API


ESP32-S3 features memory hardware which allows regions of flash memory to be mapped into instruction and data address spaces. This mapping works only for read operations. It is not possible to modify contents of flash memory by writing to a mapped memory region.


Mapping happens in 64 KB pages. Memory mapping hardware can map flash into the data address space and the instruction address space. See the technical reference manual for more details and limitations about memory mapping hardware.


Note that some pages are used to map the application itself into memory, so the actual number of available pages may be less than the capability of the hardware.


Reading data from flash using a memory mapped region is the only way to decrypt contents of flash when flash encryption is enabled. Decryption is performed at the hardware level.


Memory mapping API are declared in `spi_flash_mmap.h` and `esp_partition.h`:


* `spi_flash_mmap()` maps a region of physical flash addresses into instruction space or data space of the CPU.
* `spi_flash_munmap()` unmaps previously mapped region.
* `esp_partition_mmap()` maps part of a partition into the instruction space or data space of the CPU.


Differences between `spi_flash_mmap()` and `esp_partition_mmap()` are as follows:


* `spi_flash_mmap()` must be given a 64 KB aligned physical address.
* `esp_partition_mmap()` may be given any arbitrary offset within the partition. It adjusts the returned pointer to mapped memory as necessary.


Note that since memory mapping happens in pages, it may be possible to read data outside of the partition provided to `esp_partition_mmap`, regardless of the partition boundary.



Note


mmap is supported by cache, so it can only be used on main flash.





## SPI Flash Implementation


The `esp_flash_t` structure holds chip data as well as three important parts of this API:


1. The host driver, which provides the hardware support to access the chip;
2. The chip driver, which provides compatibility service to different chips;
3. The OS functions, provide support of some OS functions (e.g., lock, delay) in different stages (1st/2nd boot, or the app).



### Host Driver


The host driver relies on an interface (`spi_flash_host_driver_t`) defined in the `spi_flash_types.h` (in the `hal/include/hal` folder). This interface provides some common functions to communicate with the chip.


In other files of the SPI HAL, some of these functions are implemented with existing ESP32-S3 memory-spi functionalities. However, due to the speed limitations of ESP32-S3, the HAL layer cannot provide high-speed implementations to some reading commands (so the support for it was dropped). The files (`memspi_host_driver.h` and `.c`) implement the high-speed version of these commands with the `common_command` function provided in the HAL, and wrap these functions as `spi_flash_host_driver_t` for upper layer to use.


You can also implement your own host driver, even with the GPIO. As long as all the functions in the `spi_flash_host_driver_t` are implemented, the esp_flash API can access the flash regardless of the low-level hardware.




### Chip Driver


The chip driver, defined in `spi_flash_chip_driver.h`, wraps basic functions provided by the host driver for the API layer to use.


Some operations need some commands to be sent first, or read some status afterwards. Some chips need different commands or values, or need special communication ways.


There is a type of chip called `generic chip` which stands for common chips. Other special chip drivers can be developed on the base of the generic chip.


The chip driver relies on the host driver.




### OS Functions




Currently the OS function layer provides entries of a lock and delay.


The lock (see SPI Bus Lock) is used to resolve the conflicts among the access of devices on the same SPI bus, and the SPI Flash chip access. E.g.


1. On SPI1 bus, the cache (used to fetch the data (code) in the Flash and PSRAM) should be disabled when the flash chip on the SPI0/1 is being accessed.
2. On the other buses, the flash driver needs to disable the ISR registered by SPI Master driver, to avoid conflicts.
3. Some devices of SPI Master driver may require to use the bus monopolized during a period (especially when the device does not have a CS wire, or the wire is controlled by software like SDSPI driver).


The delay is used by some long operations which requires the master to wait or polling periodically.


The top API wraps these the chip driver and OS functions into an entire component, and also provides some argument checking.


OS functions can also help to avoid a watchdog timeout when erasing large flash areas. During this time, the CPU is occupied with the flash erasing task. This stops other tasks from being executed. Among these tasks is the idle task to feed the watchdog timer (WDT). If the configuration option CONFIG_ESP_TASK_WDT_PANIC is selected and the flash operation time is longer than the watchdog timeout period, the system will reboot.


It is pretty hard to totally eliminate this risk, because the erasing time varies with different flash chips, making it hard to be compatible in flash drivers. Therefore, users need to pay attention to it. Please use the following guidelines:


1. It is recommended to enable the CONFIG_SPI_FLASH_YIELD_DURING_ERASE option to allow the scheduler to re-schedule during erasing flash memory. Besides, following parameters can also be used.


* Increase CONFIG_SPI_FLASH_ERASE_YIELD_TICKS or decrease CONFIG_SPI_FLASH_ERASE_YIELD_DURATION_MS in menuconfig.
* You can also increase CONFIG_ESP_TASK_WDT_TIMEOUT_S in menuconfig for a larger watchdog timeout period. However, with larger watchdog timeout period, previously detected timeouts may no longer be detected.


2. Please be aware of the consequences of enabling the CONFIG_ESP_TASK_WDT_PANIC option when doing long-running SPI flash operations which triggers the panic handler when it times out. However, this option can also help dealing with unexpected exceptions in your application. Please decide whether this is needed to be enabled according to actual condition.
3. During your development, please carefully review the actual flash operation according to the specific requirements and time limits on erasing flash memory of your projects. Always allow reasonable redundancy based on your specific product requirements when configuring the flash erasing timeout threshold, thus improving the reliability of your product.





## Implementation Details


In order to perform some flash operations, it is necessary to make sure that both CPUs are not running any code from flash for the duration of the flash operation:


* In a single-core setup, the SDK needs to disable interrupts or scheduler before performing the flash operation.
* In a dual-core setup, the SDK needs to make sure that both CPUs are not running any code from flash.


When SPI flash API is called on CPU A (can be PRO or APP), start the `spi_flash_op_block_func` function on CPU B using the `esp_ipc_call` API. This API wakes up a high priority task on CPU B and tells it to execute a given function, in this case, `spi_flash_op_block_func`. This function disables cache on CPU B and signals that the cache is disabled by setting the `s_flash_op_can_start` flag. Then the task on CPU A disables cache as well and proceeds to execute flash operation.


While a flash operation is running, interrupts can still run on CPUs A and B. It is assumed that all interrupt code is placed into RAM. Once the interrupt allocation API is added, a flag should be added to request the interrupt to be disabled for the duration of a flash operations.


Once the flash operation is complete, the function on CPU A sets another flag, `s_flash_op_complete`, to let the task on CPU B know that it can re-enable cache and release the CPU. Then the function on CPU A re-enables the cache on CPU A as well and returns control to the calling code.


Additionally, all API functions are protected with a mutex (`s_flash_op_mutex`).


In a single core environment (CONFIG_FREERTOS_UNICORE enabled), you need to disable both caches, so that no inter-CPU communication can take place.




## Related Documents


* Optional Features for Flash
* Concurrency Constraints for Flash on SPI1
* SPI Flash API ESP-IDF Version vs Chip-ROM Version









## API Reference - SPI Flash



### Header File


* components/spi_flash/include/esp_flash_spi_init.h
* This header file can be included with:



> ```
> #include "esp_flash_spi_init.h"
> 
> ```
* This header file is a part of the API provided by the `spi_flash` component. To declare that your component depends on `spi_flash`, add the following to your CMakeLists.txt:



> ```
> REQUIRES spi_flash
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES spi_flash
> 
> ```




### Functions


#### spi_bus_add_flash_device


```


esp_err_t spi_bus_add_flash_device(esp_flash_t **out_chip, const esp_flash_spi_device_config_t *config)
Add a SPI Flash device onto the SPI bus.
The bus should be already initialized by spi_bus_initialization.

Parameters

out_chip -- Pointer to hold the initialized chip. 
config -- Configuration of the chips to initialize.


Returns

ESP_ERR_INVALID_ARG: out_chip is NULL, or some field in the config is invalid.
ESP_ERR_NO_MEM: failed to allocate memory for the chip structures.
ESP_OK: success. 





```

#### spi_bus_remove_flash_device


```


esp_err_t spi_bus_remove_flash_device(esp_flash_t *chip)
Remove a SPI Flash device from the SPI bus.

Parameters
chip -- The flash device to remove.

Returns

ESP_ERR_INVALID_ARG: The chip is invalid.
ESP_OK: success. 





```



### Structures


#### esp_flash_spi_device_config_t


```


struct esp_flash_spi_device_config_t
Configurations for the SPI Flash to init. 

Public Members
host_id

spi_host_device_t host_id
Bus to use. 

cs_io_num

int cs_io_num
GPIO pin to output the CS signal. 

io_mode

esp_flash_io_mode_t io_mode
IO mode to read from the Flash. 

speed

enum esp_flash_speed_s speed
Speed of the Flash clock. Replaced by freq_mhz. 

input_delay_ns

int input_delay_ns
Input delay of the data pins, in ns. Set to 0 if unknown. 

cs_id

int cs_id
CS line ID, ignored when not host_id is not SPI1_HOST, or CONFIG_SPI_FLASH_SHARE_SPI1_BUS is enabled. In this case, the CS line used is automatically assigned by the SPI bus lock. 

freq_mhz

int freq_mhz
The frequency of flash chip(MHZ) 



```



### Header File


* components/spi_flash/include/esp_flash.h
* This header file can be included with:



> ```
> #include "esp_flash.h"
> 
> ```
* This header file is a part of the API provided by the `spi_flash` component. To declare that your component depends on `spi_flash`, add the following to your CMakeLists.txt:



> ```
> REQUIRES spi_flash
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES spi_flash
> 
> ```




### Functions


#### esp_flash_init


```


esp_err_t esp_flash_init(esp_flash_t *chip)
Initialise SPI flash chip interface. 
This function must be called before any other API functions are called for this chip.

Note
Only the host and read_mode fields of the chip structure must be initialised before this function is called. Other fields may be auto-detected if left set to zero or NULL.


Note
If the chip->drv pointer is NULL, chip chip_drv will be auto-detected based on its manufacturer & product IDs. See esp_flash_registered_flash_drivers pointer for details of this process.


Parameters
chip -- Pointer to SPI flash chip to use. If NULL, esp_flash_default_chip is substituted. 

Returns
ESP_OK on success, or a flash error code if initialisation fails. 



```

#### esp_flash_chip_driver_initialized


```


bool esp_flash_chip_driver_initialized(const esp_flash_t *chip)
Check if appropriate chip driver is set.

Parameters
chip -- Pointer to SPI flash chip to use. If NULL, esp_flash_default_chip is substituted.

Returns
true if set, otherwise false. 



```

#### esp_flash_read_id


```


esp_err_t esp_flash_read_id(esp_flash_t *chip, uint32_t *out_id)
Read flash ID via the common "RDID" SPI flash command. 

ID is a 24-bit value. Lower 16 bits of 'id' are the chip ID, upper 8 bits are the manufacturer ID.

Parameters

chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init() 
out_id -- [out] Pointer to receive ID value.


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_get_size


```


esp_err_t esp_flash_get_size(esp_flash_t *chip, uint32_t *out_size)
Detect flash size based on flash ID. 

Note
1. Most flash chips use a common format for flash ID, where the lower 4 bits specify the size as a power of 2. If the manufacturer doesn't follow this convention, the size may be incorrectly detected.
The out_size returned only stands for The out_size stands for the size in the binary image header. If you want to get the real size of the chip, please call esp_flash_get_physical_size instead.




Parameters

chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init() 
out_size -- [out] Detected size in bytes, standing for the size in the binary image header.


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_get_physical_size


```


esp_err_t esp_flash_get_physical_size(esp_flash_t *chip, uint32_t *flash_size)
Detect flash size based on flash ID. 

Note
Most flash chips use a common format for flash ID, where the lower 4 bits specify the size as a power of 2. If the manufacturer doesn't follow this convention, the size may be incorrectly detected.


Parameters

chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init() 
flash_size -- [out] Detected size in bytes.


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_read_unique_chip_id


```


esp_err_t esp_flash_read_unique_chip_id(esp_flash_t *chip, uint64_t *out_id)
Read flash unique ID via the common "RDUID" SPI flash command. 

ID is a 64-bit value.

Note
This is an optional feature, which is not supported on all flash chips. READ PROGRAMMING GUIDE FIRST!


Parameters

chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init(). 
out_id -- [out] Pointer to receive unique ID value.


Returns

ESP_OK on success, or a flash error code if operation failed.
ESP_ERR_NOT_SUPPORTED if the chip doesn't support read id. 





```

#### esp_flash_erase_chip


```


esp_err_t esp_flash_erase_chip(esp_flash_t *chip)
Erase flash chip contents. 

Parameters
chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init()

Returns

ESP_OK on success,
ESP_ERR_NOT_SUPPORTED if the chip is not able to perform the operation. This is indicated by WREN = 1 after the command is sent.
ESP_ERR_NOT_ALLOWED if a read-only partition is present.
Other flash error code if operation failed. 





```

#### esp_flash_erase_region


```


esp_err_t esp_flash_erase_region(esp_flash_t *chip, uint32_t start, uint32_t len)
Erase a region of the flash chip. 

Sector size is specifyed in chip->drv->sector_size field (typically 4096 bytes.) ESP_ERR_INVALID_ARG will be returned if the start & length are not a multiple of this size.
Erase is performed using block (multi-sector) erases where possible (block size is specified in chip->drv->block_erase_size field, typically 65536 bytes). Remaining sectors are erased using individual sector erase commands.

Parameters

chip -- Pointer to identify flash chip. If NULL, esp_flash_default_chip is substituted. Must have been successfully initialised via esp_flash_init() 
start -- Address to start erasing flash. Must be sector aligned. 
len -- Length of region to erase. Must also be sector aligned.


Returns

ESP_OK on success,
ESP_ERR_NOT_SUPPORTED if the chip is not able to perform the operation. This is indicated by WREN = 1 after the command is sent.
ESP_ERR_NOT_ALLOWED if the address range (start &#8212; start + len) overlaps with a read-only partition address space
Other flash error code if operation failed. 





```

#### esp_flash_get_chip_write_protect


```


esp_err_t esp_flash_get_chip_write_protect(esp_flash_t *chip, bool *write_protected)
Read if the entire chip is write protected. 

Note
A correct result for this flag depends on the SPI flash chip model and chip_drv in use (via the 'chip->drv' field).


Parameters

chip -- Pointer to identify flash chip. If NULL, esp_flash_default_chip is substituted. Must have been successfully initialised via esp_flash_init() 
write_protected -- [out] Pointer to boolean, set to the value of the write protect flag.


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_set_chip_write_protect


```


esp_err_t esp_flash_set_chip_write_protect(esp_flash_t *chip, bool write_protect)
Set write protection for the SPI flash chip. 

Some SPI flash chips may require a power cycle before write protect status can be cleared. Otherwise, write protection can be removed via a follow-up call to this function.

Note
Correct behaviour of this function depends on the SPI flash chip model and chip_drv in use (via the 'chip->drv' field).


Parameters

chip -- Pointer to identify flash chip. If NULL, esp_flash_default_chip is substituted. Must have been successfully initialised via esp_flash_init() 
write_protect -- Boolean value for the write protect flag


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_get_protectable_regions


```


esp_err_t esp_flash_get_protectable_regions(const esp_flash_t *chip, const esp_flash_region_t **out_regions, uint32_t *out_num_regions)
Read the list of individually protectable regions of this SPI flash chip. 

Note
Correct behaviour of this function depends on the SPI flash chip model and chip_drv in use (via the 'chip->drv' field).


Parameters

chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init() 
out_regions -- [out] Pointer to receive a pointer to the array of protectable regions of the chip. 
out_num_regions -- [out] Pointer to an integer receiving the count of protectable regions in the array returned in 'regions'.


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_get_protected_region


```


esp_err_t esp_flash_get_protected_region(esp_flash_t *chip, const esp_flash_region_t *region, bool *out_protected)
Detect if a region of the SPI flash chip is protected. 

Note
It is possible for this result to be false and write operations to still fail, if protection is enabled for the entire chip.


Note
Correct behaviour of this function depends on the SPI flash chip model and chip_drv in use (via the 'chip->drv' field).


Parameters

chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init() 
region -- Pointer to a struct describing a protected region. This must match one of the regions returned from esp_flash_get_protectable_regions(...). 
out_protected -- [out] Pointer to a flag which is set based on the protected status for this region.


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_set_protected_region


```


esp_err_t esp_flash_set_protected_region(esp_flash_t *chip, const esp_flash_region_t *region, bool protect)
Update the protected status for a region of the SPI flash chip. 

Note
It is possible for the region protection flag to be cleared and write operations to still fail, if protection is enabled for the entire chip.


Note
Correct behaviour of this function depends on the SPI flash chip model and chip_drv in use (via the 'chip->drv' field).


Parameters

chip -- Pointer to identify flash chip. Must have been successfully initialised via esp_flash_init() 
region -- Pointer to a struct describing a protected region. This must match one of the regions returned from esp_flash_get_protectable_regions(...). 
protect -- Write protection flag to set.


Returns
ESP_OK on success, or a flash error code if operation failed. 



```

#### esp_flash_read


```


esp_err_t esp_flash_read(esp_flash_t *chip, void *buffer, uint32_t address, uint32_t length)
Read data from the SPI flash chip. 

There are no alignment constraints on buffer, address or length.

Note
If on-chip flash encryption is used, this function returns raw (ie encrypted) data. Use the flash cache to transparently decrypt data.


Parameters

chip -- Pointer to identify flash chip. If NULL, esp_flash_default_chip is substituted. Must have been successfully initialised via esp_flash_init() 
buffer -- Pointer to a buffer where the data will be read. To get better performance, this should be in the DRAM and word aligned. 
address -- Address on flash to read from. Must be less than chip->size field. 
length -- Length (in bytes) of data to read.


Returns

ESP_OK: success
ESP_ERR_NO_MEM: Buffer is in external PSRAM which cannot be concurrently accessed, and a temporary internal buffer could not be allocated.
or a flash error code if operation failed. 





```

#### esp_flash_write


```


esp_err_t esp_flash_write(esp_flash_t *chip, const void *buffer, uint32_t address, uint32_t length)
Write data to the SPI flash chip. 

There are no alignment constraints on buffer, address or length.

Parameters

chip -- Pointer to identify flash chip. If NULL, esp_flash_default_chip is substituted. Must have been successfully initialised via esp_flash_init() 
address -- Address on flash to write to. Must be previously erased (SPI NOR flash can only write bits 1->0). 
buffer -- Pointer to a buffer with the data to write. To get better performance, this should be in the DRAM and word aligned. 
length -- Length (in bytes) of data to write.


Returns

ESP_OK on success
ESP_FAIL, bad write, this will be detected only when CONFIG_SPI_FLASH_VERIFY_WRITE is enabled
ESP_ERR_NOT_SUPPORTED if the chip is not able to perform the operation. This is indicated by WREN = 1 after the command is sent.
ESP_ERR_NOT_ALLOWED if the address range (address &#8212; address + length) overlaps with a read-only partition address space
Other flash error code if operation failed. 





```

#### esp_flash_write_encrypted


```


esp_err_t esp_flash_write_encrypted(esp_flash_t *chip, uint32_t address, const void *buffer, uint32_t length)
Encrypted and write data to the SPI flash chip using on-chip hardware flash encryption. 

Note
Both address & length must be 16 byte aligned, as this is the encryption block size


Parameters

chip -- Pointer to identify flash chip. Must be NULL (the main flash chip). For other chips, encrypted write is not supported. 
address -- Address on flash to write to. 16 byte aligned. Must be previously erased (SPI NOR flash can only write bits 1->0). 
buffer -- Pointer to a buffer with the data to write. 
length -- Length (in bytes) of data to write. 16 byte aligned.


Returns

ESP_OK: on success
ESP_FAIL: bad write, this will be detected only when CONFIG_SPI_FLASH_VERIFY_WRITE is enabled
ESP_ERR_NOT_SUPPORTED: encrypted write not supported for this chip.
ESP_ERR_INVALID_ARG: Either the address, buffer or length is invalid.
ESP_ERR_NOT_ALLOWED if the address range (address &#8212; address + length) overlaps with a read-only partition address space 





```

#### esp_flash_read_encrypted


```


esp_err_t esp_flash_read_encrypted(esp_flash_t *chip, uint32_t address, void *out_buffer, uint32_t length)
Read and decrypt data from the SPI flash chip using on-chip hardware flash encryption. 

Parameters

chip -- Pointer to identify flash chip. Must be NULL (the main flash chip). For other chips, encrypted read is not supported. 
address -- Address on flash to read from. 
out_buffer -- Pointer to a buffer for the data to read to. 
length -- Length (in bytes) of data to read.


Returns

ESP_OK: on success
ESP_ERR_NOT_SUPPORTED: encrypted read not supported for this chip. 





```

#### esp_flash_is_quad_mode


```


static inline bool esp_flash_is_quad_mode(const esp_flash_t *chip)
Returns true if chip is configured for Quad I/O or Quad Fast Read. 

Parameters
chip -- Pointer to SPI flash chip to use. If NULL, esp_flash_default_chip is substituted.

Returns
true if flash works in quad mode, otherwise false 



```



### Structures


#### esp_flash_region_t


```


struct esp_flash_region_t
Structure for describing a region of flash. 

Public Members
offset

uint32_t offset
Start address of this region. 

size

uint32_t size
Size of the region. 



```

#### esp_flash_os_functions_t


```


struct esp_flash_os_functions_t
OS-level integration hooks for accessing flash chips inside a running OS. 
It's in the public header because some instances should be allocated statically in the startup code. May be updated according to hardware version and new flash chip feature requirements, shouldn't be treated as public API.
For advanced developers, you may replace some of them with your implementations at your own risk. 

Public Members
start

esp_err_t (*start)(void *arg)
Called before commencing any flash operation. Does not need to be recursive (ie is called at most once for each call to 'end'). 

end

esp_err_t (*end)(void *arg)
Called after completing any flash operation. 

region_protected

esp_err_t (*region_protected)(void *arg, size_t start_addr, size_t size)
Called before any erase/write operations to check whether the region is limited by the OS 

delay_us

esp_err_t (*delay_us)(void *arg, uint32_t us)
Delay for at least 'us' microseconds. Called in between 'start' and 'end'. 

get_temp_buffer

void *(*get_temp_buffer)(void *arg, size_t reqest_size, size_t *out_size)
Called for get temp buffer when buffer from application cannot be directly read into/write from. 

release_temp_buffer

void (*release_temp_buffer)(void *arg, void *temp_buf)
Called for release temp buffer. 

check_yield

esp_err_t (*check_yield)(void *arg, uint32_t chip_status, uint32_t *out_request)
Yield to other tasks. Called during erase operations. 

Return
ESP_OK means yield needs to be called (got an event to handle), while ESP_ERR_TIMEOUT means skip yield. 



yield

esp_err_t (*yield)(void *arg, uint32_t *out_status)
Yield to other tasks. Called during erase operations. 

get_system_time

int64_t (*get_system_time)(void *arg)
Called for get system time. 

set_flash_op_status

void (*set_flash_op_status)(uint32_t op_status)
Call to set flash operation status 



```

#### esp_flash_t


```


struct esp_flash_t
Structure to describe a SPI flash chip connected to the system. 
Structure must be initialized before use (passed to esp_flash_init()). It's in the public header because some instances should be allocated statically in the startup code. May be updated according to hardware version and new flash chip feature requirements, shouldn't be treated as public API.
For advanced developers, you may replace some of them with your implementations at your own risk. 

Public Members
host

spi_flash_host_inst_t *host
Pointer to hardware-specific "host_driver" structure. Must be initialized before used. 

chip_drv

const spi_flash_chip_t *chip_drv
Pointer to chip-model-specific "adapter" structure. If NULL, will be detected during initialisation. 

os_func

const esp_flash_os_functions_t *os_func
Pointer to os-specific hook structure. Call esp_flash_init_os_functions() to setup this field, after the host is properly initialized. 

os_func_data

void *os_func_data
Pointer to argument for os-specific hooks. Left NULL and will be initialized with os_func. 

read_mode

esp_flash_io_mode_t read_mode
Configured SPI flash read mode. Set before esp_flash_init is called. 

size

uint32_t size
Size of SPI flash in bytes. If 0, size will be detected during initialisation. Note: this stands for the size in the binary image header. If you want to get the flash physical size, please call esp_flash_get_physical_size. 

chip_id

uint32_t chip_id
Detected chip id. 

busy

uint32_t busy
This flag is used to verify chip's status. 

hpm_dummy_ena

uint32_t hpm_dummy_ena
This flag is used to verify whether flash works under HPM status. 

reserved_flags

uint32_t reserved_flags
reserved. 



```



### Macros


#### SPI_FLASH_YIELD_REQ_YIELD


```


SPI_FLASH_YIELD_REQ_YIELD

```

#### SPI_FLASH_YIELD_REQ_SUSPEND


```


SPI_FLASH_YIELD_REQ_SUSPEND

```

#### SPI_FLASH_YIELD_STA_RESUME


```


SPI_FLASH_YIELD_STA_RESUME

```

#### SPI_FLASH_OS_IS_ERASING_STATUS_FLAG


```


SPI_FLASH_OS_IS_ERASING_STATUS_FLAG

```



### Type Definitions


#### spi_flash_chip_t


```


typedef struct spi_flash_chip_t spi_flash_chip_t

```



### Header File


* components/spi_flash/include/spi_flash_mmap.h
* This header file can be included with:



> ```
> #include "spi_flash_mmap.h"
> 
> ```
* This header file is a part of the API provided by the `spi_flash` component. To declare that your component depends on `spi_flash`, add the following to your CMakeLists.txt:



> ```
> REQUIRES spi_flash
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES spi_flash
> 
> ```




### Functions


#### spi_flash_mmap


```


esp_err_t spi_flash_mmap(size_t src_addr, size_t size, spi_flash_mmap_memory_t memory, const void **out_ptr, spi_flash_mmap_handle_t *out_handle)
Map region of flash memory into data or instruction address space. 
This function allocates sufficient number of 64kB MMU pages and configures them to map the requested region of flash memory into the address space. It may reuse MMU pages which already provide the required mapping.
As with any allocator, if mmap/munmap are heavily used then the address space may become fragmented. To troubleshoot issues with page allocation, use spi_flash_mmap_dump() function.

Parameters

src_addr -- Physical address in flash where requested region starts. This address must be aligned to 64kB boundary (SPI_FLASH_MMU_PAGE_SIZE) 
size -- Size of region to be mapped. This size will be rounded up to a 64kB boundary 
memory -- Address space where the region should be mapped (data or instruction) 
out_ptr -- [out] Output, pointer to the mapped memory region 
out_handle -- [out] Output, handle which should be used for spi_flash_munmap call


Returns
ESP_OK on success, ESP_ERR_NO_MEM if pages can not be allocated 



```

#### spi_flash_mmap_pages


```


esp_err_t spi_flash_mmap_pages(const int *pages, size_t page_count, spi_flash_mmap_memory_t memory, const void **out_ptr, spi_flash_mmap_handle_t *out_handle)
Map sequences of pages of flash memory into data or instruction address space. 
This function allocates sufficient number of 64kB MMU pages and configures them to map the indicated pages of flash memory contiguously into address space. In this respect, it works in a similar way as spi_flash_mmap() but it allows mapping a (maybe non-contiguous) set of pages into a contiguous region of memory.

Parameters

pages -- An array of numbers indicating the 64kB pages in flash to be mapped contiguously into memory. These indicate the indexes of the 64kB pages, not the byte-size addresses as used in other functions. Array must be located in internal memory. 
page_count -- Number of entries in the pages array 
memory -- Address space where the region should be mapped (instruction or data) 
out_ptr -- [out] Output, pointer to the mapped memory region 
out_handle -- [out] Output, handle which should be used for spi_flash_munmap call


Returns

ESP_OK on success
ESP_ERR_NO_MEM if pages can not be allocated
ESP_ERR_INVALID_ARG if pagecount is zero or pages array is not in internal memory 





```

#### spi_flash_munmap


```


void spi_flash_munmap(spi_flash_mmap_handle_t handle)
Release region previously obtained using spi_flash_mmap. 

Note
Calling this function will not necessarily unmap memory region. Region will only be unmapped when there are no other handles which reference this region. In case of partially overlapping regions it is possible that memory will be unmapped partially.


Parameters
handle -- Handle obtained from spi_flash_mmap 



```

#### spi_flash_mmap_dump


```


void spi_flash_mmap_dump(void)
Display information about mapped regions. 
This function lists handles obtained using spi_flash_mmap, along with range of pages allocated to each handle. It also lists all non-zero entries of MMU table and corresponding reference counts. 

```

#### spi_flash_mmap_get_free_pages


```


uint32_t spi_flash_mmap_get_free_pages(spi_flash_mmap_memory_t memory)
get free pages number which can be mmap 
This function will return number of free pages available in mmu table. This could be useful before calling actual spi_flash_mmap (maps flash range to DCache or ICache memory) to check if there is sufficient space available for mapping.

Parameters
memory -- memory type of MMU table free page

Returns
number of free pages which can be mmaped 



```

#### spi_flash_cache2phys


```


size_t spi_flash_cache2phys(const void *cached)
Given a memory address where flash is mapped, return the corresponding physical flash offset. 
Cache address does not have have been assigned via spi_flash_mmap(), any address in memory mapped flash space can be looked up.

Parameters
cached -- Pointer to flashed cached memory.

Returns

SPI_FLASH_CACHE2PHYS_FAIL If cache address is outside flash cache region, or the address is not mapped.
Otherwise, returns physical offset in flash 





```

#### spi_flash_phys2cache


```


const void *spi_flash_phys2cache(size_t phys_offs, spi_flash_mmap_memory_t memory)
Given a physical offset in flash, return the address where it is mapped in the memory space. 
Physical address does not have to have been assigned via spi_flash_mmap(), any address in flash can be looked up.

Note
Only the first matching cache address is returned. If MMU flash cache table is configured so multiple entries point to the same physical address, there may be more than one cache address corresponding to that physical address. It is also possible for a single physical address to be mapped to both the IROM and DROM regions.


Note
This function doesn't impose any alignment constraints, but if memory argument is SPI_FLASH_MMAP_INST and phys_offs is not 4-byte aligned, then reading from the returned pointer will result in a crash.


Parameters

phys_offs -- Physical offset in flash memory to look up. 
memory -- Address space type to look up a flash cache address mapping for (instruction or data)


Returns

NULL if the physical address is invalid or not mapped to flash cache of the specified memory type.
Cached memory address (in IROM or DROM space) corresponding to phys_offs. 





```



### Macros


#### ESP_ERR_FLASH_OP_FAIL


```


ESP_ERR_FLASH_OP_FAIL
This file contains spi_flash_mmap_xx APIs, mainly for doing memory mapping to an SPI0-connected external Flash, as well as some helper functions to convert between virtual and physical address 

```

#### ESP_ERR_FLASH_OP_TIMEOUT


```


ESP_ERR_FLASH_OP_TIMEOUT

```

#### SPI_FLASH_SEC_SIZE


```


SPI_FLASH_SEC_SIZE
SPI Flash sector size 

```

#### SPI_FLASH_MMU_PAGE_SIZE


```


SPI_FLASH_MMU_PAGE_SIZE
Flash cache MMU mapping page size 

```

#### SPI_FLASH_CACHE2PHYS_FAIL


```


SPI_FLASH_CACHE2PHYS_FAIL

```



### Type Definitions


#### spi_flash_mmap_handle_t


```


typedef uint32_t spi_flash_mmap_handle_t
Opaque handle for memory region obtained from spi_flash_mmap. 

```



### Enumerations


#### spi_flash_mmap_memory_t


```


enum spi_flash_mmap_memory_t
Enumeration which specifies memory space requested in an mmap call. 
Values:
SPI_FLASH_MMAP_DATA

enumerator SPI_FLASH_MMAP_DATA
map to data memory, allows byte-aligned access 

SPI_FLASH_MMAP_INST

enumerator SPI_FLASH_MMAP_INST
map to instruction memory, allows only 4-byte-aligned access 


```



### Header File


* components/hal/include/hal/spi_flash_types.h
* This header file can be included with:



> ```
> #include "hal/spi_flash_types.h"
> 
> ```




### Structures


#### spi_flash_trans_t


```


struct spi_flash_trans_t
Definition of a common transaction. Also holds the return value. 

Public Members
reserved

uint8_t reserved
Reserved, must be 0. 

mosi_len

uint8_t mosi_len
Output data length, in bytes. 

miso_len

uint8_t miso_len
Input data length, in bytes. 

address_bitlen

uint8_t address_bitlen
Length of address in bits, set to 0 if command does not need an address. 

address

uint32_t address
Address to perform operation on. 

mosi_data

const uint8_t *mosi_data
Output data to salve. 

miso_data

uint8_t *miso_data
[out] Input data from slave, little endian 

flags

uint32_t flags
Flags for this transaction. Set to 0 for now. 

command

uint16_t command
Command to send. 

dummy_bitlen

uint8_t dummy_bitlen
Basic dummy bits to use. 

io_mode

uint32_t io_mode
Flash working mode when SPI_FLASH_IGNORE_BASEIO is specified. 



```

#### spi_flash_sus_cmd_conf


```


struct spi_flash_sus_cmd_conf
Configuration structure for the flash chip suspend feature. 

Public Members
sus_mask

uint32_t sus_mask
SUS/SUS1/SUS2 bit in flash register. 

cmd_rdsr

uint32_t cmd_rdsr
Read flash status register(2) command. 

sus_cmd

uint32_t sus_cmd
Flash suspend command. 

res_cmd

uint32_t res_cmd
Flash resume command. 

reserved

uint32_t reserved
Reserved, set to 0. 



```

#### spi_flash_encryption_t


```


struct spi_flash_encryption_t
Structure for flash encryption operations. 

Public Members
flash_encryption_enable

void (*flash_encryption_enable)(void)
Enable the flash encryption. 

flash_encryption_disable

void (*flash_encryption_disable)(void)
Disable the flash encryption. 

flash_encryption_data_prepare

void (*flash_encryption_data_prepare)(uint32_t address, const uint32_t *buffer, uint32_t size)
Prepare flash encryption before operation.

Note
address and buffer must be 8-word aligned. 


Param address
The destination address in flash for the write operation. 

Param buffer
Data for programming 

Param size
Size to program.



flash_encryption_done

void (*flash_encryption_done)(void)
flash data encryption operation is done. 

flash_encryption_destroy

void (*flash_encryption_destroy)(void)
Destroy encrypted result 

flash_encryption_check

bool (*flash_encryption_check)(uint32_t address, uint32_t length)
Check if is qualified to encrypt the buffer

Param address
the address of written flash partition. 

Param length
Buffer size. 





```

#### spi_flash_host_inst_t


```


struct spi_flash_host_inst_t
SPI Flash Host driver instance 

Public Members
driver

const struct spi_flash_host_driver_s *driver
Pointer to the implementation function table. 



```

#### spi_flash_host_driver_s


```


struct spi_flash_host_driver_s
Host driver configuration and context structure. 

Public Members
dev_config

esp_err_t (*dev_config)(spi_flash_host_inst_t *host)
Configure the device-related register before transactions. This saves some time to re-configure those registers when we send continuously 

common_command

esp_err_t (*common_command)(spi_flash_host_inst_t *host, spi_flash_trans_t *t)
Send an user-defined spi transaction to the device. 

read_id

esp_err_t (*read_id)(spi_flash_host_inst_t *host, uint32_t *id)
Read flash ID. 

erase_chip

void (*erase_chip)(spi_flash_host_inst_t *host)
Erase whole flash chip. 

erase_sector

void (*erase_sector)(spi_flash_host_inst_t *host, uint32_t start_address)
Erase a specific sector by its start address. 

erase_block

void (*erase_block)(spi_flash_host_inst_t *host, uint32_t start_address)
Erase a specific block by its start address. 

read_status

esp_err_t (*read_status)(spi_flash_host_inst_t *host, uint8_t *out_sr)
Read the status of the flash chip. 

set_write_protect

esp_err_t (*set_write_protect)(spi_flash_host_inst_t *host, bool wp)
Disable write protection. 

program_page

void (*program_page)(spi_flash_host_inst_t *host, const void *buffer, uint32_t address, uint32_t length)
Program a page of the flash. Check max_write_bytes for the maximum allowed writing length. 

supports_direct_write

bool (*supports_direct_write)(spi_flash_host_inst_t *host, const void *p)
Check whether the SPI host supports direct write. 
When cache is disabled, SPI1 doesn't support directly write when buffer isn't internal. 

write_data_slicer

int (*write_data_slicer)(spi_flash_host_inst_t *host, uint32_t address, uint32_t len, uint32_t *align_addr, uint32_t page_size)
Slicer for write data. The program_page should be called iteratively with the return value of this function.

Param address
Beginning flash address to write 

Param len
Length request to write 

Param align_addr
Output of the aligned address to write to 

Param page_size
Physical page size of the flash chip 

Return
Length that can be actually written in one program_page call 



read

esp_err_t (*read)(spi_flash_host_inst_t *host, void *buffer, uint32_t address, uint32_t read_len)
Read data from the flash. Check max_read_bytes for the maximum allowed reading length. 

supports_direct_read

bool (*supports_direct_read)(spi_flash_host_inst_t *host, const void *p)
Check whether the SPI host supports direct read. 
When cache is disabled, SPI1 doesn't support directly read when the given buffer isn't internal. 

read_data_slicer

int (*read_data_slicer)(spi_flash_host_inst_t *host, uint32_t address, uint32_t len, uint32_t *align_addr, uint32_t page_size)
Slicer for read data. The read should be called iteratively with the return value of this function.

Param address
Beginning flash address to read 

Param len
Length request to read 

Param align_addr
Output of the aligned address to read 

Param page_size
Physical page size of the flash chip 

Return
Length that can be actually read in one read call 



host_status

uint32_t (*host_status)(spi_flash_host_inst_t *host)
Check the host status, 0:busy, 1:idle, 2:suspended. 

configure_host_io_mode

esp_err_t (*configure_host_io_mode)(spi_flash_host_inst_t *host, uint32_t command, uint32_t addr_bitlen, int dummy_bitlen_base, esp_flash_io_mode_t io_mode)
Configure the host to work at different read mode. Responsible to compensate the timing and set IO mode. 

poll_cmd_done

void (*poll_cmd_done)(spi_flash_host_inst_t *host)
Internal use, poll the HW until the last operation is done. 

flush_cache

esp_err_t (*flush_cache)(spi_flash_host_inst_t *host, uint32_t addr, uint32_t size)
For some host (SPI1), they are shared with a cache. When the data is modified, the cache needs to be flushed. Left NULL if not supported. 

check_suspend

void (*check_suspend)(spi_flash_host_inst_t *host)
Suspend check erase/program operation, reserved for ESP32-C3 and ESP32-S3 spi flash ROM IMPL. 

resume

void (*resume)(spi_flash_host_inst_t *host)
Resume flash from suspend manually 

suspend

void (*suspend)(spi_flash_host_inst_t *host)
Set flash in suspend status manually 

sus_setup

esp_err_t (*sus_setup)(spi_flash_host_inst_t *host, const spi_flash_sus_cmd_conf *sus_conf)
Suspend feature setup for setting cmd and status register mask. 



```



### Macros


#### SPI_FLASH_TRANS_FLAG_CMD16


```


SPI_FLASH_TRANS_FLAG_CMD16
Send command of 16 bits. 

```

#### SPI_FLASH_TRANS_FLAG_IGNORE_BASEIO


```


SPI_FLASH_TRANS_FLAG_IGNORE_BASEIO
Not applying the basic io mode configuration for this transaction. 

```

#### SPI_FLASH_TRANS_FLAG_BYTE_SWAP


```


SPI_FLASH_TRANS_FLAG_BYTE_SWAP
Used for DTR mode, to swap the bytes of a pair of rising/falling edge. 

```

#### SPI_FLASH_TRANS_FLAG_PE_CMD


```


SPI_FLASH_TRANS_FLAG_PE_CMD
Indicates that this transaction is to erase/program flash chip. 

```

#### SPI_FLASH_CONFIG_CONF_BITS


```


SPI_FLASH_CONFIG_CONF_BITS
OR the io_mode with this mask, to enable the dummy output feature or replace the first several dummy bits into address to meet the requirements of conf bits. (Used in DIO/QIO/OIO mode) 

```

#### SPI_FLASH_OPI_FLAG


```


SPI_FLASH_OPI_FLAG
A flag for flash work in opi mode, the io mode below are opi, above are SPI/QSPI mode. DO NOT use this value in any API. 

```

#### SPI_FLASH_READ_MODE_MIN


```


SPI_FLASH_READ_MODE_MIN
Slowest io mode supported by ESP32, currently SlowRd. 

```



### Type Definitions


#### esp_flash_speed_t


```


typedef enum esp_flash_speed_s esp_flash_speed_t
SPI flash clock speed values, always refer to them by the enum rather than the actual value (more speed may be appended into the list). 
A strategy to select the maximum allowed speed is to enumerate from the ESP_FLSH_SPEED_MAX-1 or highest frequency supported by your flash, and decrease the speed until the probing success. 

```

#### spi_flash_host_driver_t


```


typedef struct spi_flash_host_driver_s spi_flash_host_driver_t

```



### Enumerations


#### esp_flash_speed_s


```


enum esp_flash_speed_s
SPI flash clock speed values, always refer to them by the enum rather than the actual value (more speed may be appended into the list). 
A strategy to select the maximum allowed speed is to enumerate from the ESP_FLSH_SPEED_MAX-1 or highest frequency supported by your flash, and decrease the speed until the probing success. 
Values:
ESP_FLASH_5MHZ

enumerator ESP_FLASH_5MHZ
The flash runs under 5MHz. 

ESP_FLASH_10MHZ

enumerator ESP_FLASH_10MHZ
The flash runs under 10MHz. 

ESP_FLASH_20MHZ

enumerator ESP_FLASH_20MHZ
The flash runs under 20MHz. 

ESP_FLASH_26MHZ

enumerator ESP_FLASH_26MHZ
The flash runs under 26MHz. 

ESP_FLASH_40MHZ

enumerator ESP_FLASH_40MHZ
The flash runs under 40MHz. 

ESP_FLASH_80MHZ

enumerator ESP_FLASH_80MHZ
The flash runs under 80MHz. 

ESP_FLASH_120MHZ

enumerator ESP_FLASH_120MHZ
The flash runs under 120MHz, 120MHZ can only be used by main flash after timing tuning in system. Do not use this directely in any API. 

ESP_FLASH_SPEED_MAX

enumerator ESP_FLASH_SPEED_MAX
The maximum frequency supported by the host is ESP_FLASH_SPEED_MAX-1. 


```

#### esp_flash_io_mode_t


```


enum esp_flash_io_mode_t
Mode used for reading from SPI flash. 
Values:
SPI_FLASH_SLOWRD

enumerator SPI_FLASH_SLOWRD
Data read using single I/O, some limits on speed. 

SPI_FLASH_FASTRD

enumerator SPI_FLASH_FASTRD
Data read using single I/O, no limit on speed. 

SPI_FLASH_DOUT

enumerator SPI_FLASH_DOUT
Data read using dual I/O. 

SPI_FLASH_DIO

enumerator SPI_FLASH_DIO
Both address & data transferred using dual I/O. 

SPI_FLASH_QOUT

enumerator SPI_FLASH_QOUT
Data read using quad I/O. 

SPI_FLASH_QIO

enumerator SPI_FLASH_QIO
Both address & data transferred using quad I/O. 

SPI_FLASH_OPI_STR

enumerator SPI_FLASH_OPI_STR
Only support on OPI flash, flash read and write under STR mode. 

SPI_FLASH_OPI_DTR

enumerator SPI_FLASH_OPI_DTR
Only support on OPI flash, flash read and write under DTR mode. 

SPI_FLASH_READ_MODE_MAX

enumerator SPI_FLASH_READ_MODE_MAX
The fastest io mode supported by the host is ESP_FLASH_READ_MODE_MAX-1. 


```



### Header File


* components/hal/include/hal/esp_flash_err.h
* This header file can be included with:



> ```
> #include "hal/esp_flash_err.h"
> 
> ```




### Macros


#### ESP_ERR_FLASH_NOT_INITIALISED


```


ESP_ERR_FLASH_NOT_INITIALISED
esp_flash_chip_t structure not correctly initialised by esp_flash_init(). 

```

#### ESP_ERR_FLASH_UNSUPPORTED_HOST


```


ESP_ERR_FLASH_UNSUPPORTED_HOST
Requested operation isn't supported via this host SPI bus (chip->spi field). 

```

#### ESP_ERR_FLASH_UNSUPPORTED_CHIP


```


ESP_ERR_FLASH_UNSUPPORTED_CHIP
Requested operation isn't supported by this model of SPI flash chip. 

```

#### ESP_ERR_FLASH_PROTECTED


```


ESP_ERR_FLASH_PROTECTED
Write operation failed due to chip's write protection being enabled. 

```



### Enumerations


#### [anonymous]


```


enum [anonymous]
Values:
ESP_ERR_FLASH_SIZE_NOT_MATCH

enumerator ESP_ERR_FLASH_SIZE_NOT_MATCH
The chip doesn't have enough space for the current partition table. 

ESP_ERR_FLASH_NO_RESPONSE

enumerator ESP_ERR_FLASH_NO_RESPONSE
Chip did not respond to the command, or timed out. 


```



### Header File


* components/spi_flash/include/esp_spi_flash_counters.h
* This header file can be included with:



> ```
> #include "esp_spi_flash_counters.h"
> 
> ```
* This header file is a part of the API provided by the `spi_flash` component. To declare that your component depends on `spi_flash`, add the following to your CMakeLists.txt:



> ```
> REQUIRES spi_flash
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES spi_flash
> 
> ```




### Functions


#### esp_flash_reset_counters


```


void esp_flash_reset_counters(void)
Reset SPI flash operation counters. 

```

#### spi_flash_reset_counters


```


void spi_flash_reset_counters(void)

```

#### esp_flash_dump_counters


```


void esp_flash_dump_counters(FILE *stream)
Print SPI flash operation counters. 

```

#### spi_flash_dump_counters


```


void spi_flash_dump_counters(void)

```

#### esp_flash_get_counters


```


const esp_flash_counters_t *esp_flash_get_counters(void)
Return current SPI flash operation counters. 

Returns
pointer to the esp_flash_counters_t structure holding values of the operation counters 



```

#### spi_flash_get_counters


```


const spi_flash_counters_t *spi_flash_get_counters(void)

```



### Structures


#### esp_flash_counter_t


```


struct esp_flash_counter_t
Structure holding statistics for one type of operation 

Public Members
count

uint32_t count
number of times operation was executed 

time

uint32_t time
total time taken, in microseconds 

bytes

uint32_t bytes
total number of bytes 



```

#### esp_flash_counters_t


```


struct esp_flash_counters_t
Structure for counters of flash actions 

Public Members
read

esp_flash_counter_t read
counters for read action, like esp_flash_read

write

esp_flash_counter_t write
counters for write action, like esp_flash_write

erase

esp_flash_counter_t erase
counters for erase action, like esp_flash_erase



```



### Type Definitions


#### spi_flash_counter_t


```


typedef esp_flash_counter_t spi_flash_counter_t

```

#### spi_flash_counters_t


```


typedef esp_flash_counters_t spi_flash_counters_t

```




## API Reference - Flash Encrypt



### Header File


* components/bootloader_support/include/esp_flash_encrypt.h
* This header file can be included with:



> ```
> #include "esp_flash_encrypt.h"
> 
> ```
* This header file is a part of the API provided by the `bootloader_support` component. To declare that your component depends on `bootloader_support`, add the following to your CMakeLists.txt:



> ```
> REQUIRES bootloader_support
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES bootloader_support
> 
> ```




### Functions


#### esp_flash_encryption_enabled


```


bool esp_flash_encryption_enabled(void)
Is flash encryption currently enabled in hardware? 
Flash encryption is enabled if the FLASH_CRYPT_CNT efuse has an odd number of bits set.

Returns
true if flash encryption is enabled. 



```

#### esp_flash_encrypt_check_and_update


```


esp_err_t esp_flash_encrypt_check_and_update(void)

```

#### esp_flash_encrypt_state


```


bool esp_flash_encrypt_state(void)
Returns the Flash Encryption state and prints it. 

Returns
True - Flash Encryption is enabled False - Flash Encryption is not enabled 



```

#### esp_flash_encrypt_initialized_once


```


bool esp_flash_encrypt_initialized_once(void)
Checks if the first initialization was done. 
If the first initialization was done then FLASH_CRYPT_CNT != 0

Returns
true - the first initialization was done false - the first initialization was NOT done 



```

#### esp_flash_encrypt_init


```


esp_err_t esp_flash_encrypt_init(void)
The first initialization of Flash Encryption key and related eFuses. 

Returns
ESP_OK if all operations succeeded 



```

#### esp_flash_encrypt_contents


```


esp_err_t esp_flash_encrypt_contents(void)
Encrypts flash content. 

Returns
ESP_OK if all operations succeeded 



```

#### esp_flash_encrypt_enable


```


esp_err_t esp_flash_encrypt_enable(void)
Activates Flash encryption on the chip. 
It burns FLASH_CRYPT_CNT eFuse based on the CONFIG_SECURE_FLASH_ENCRYPTION_MODE_RELEASE option.

Returns
ESP_OK if all operations succeeded 



```

#### esp_flash_encrypt_is_write_protected


```


bool esp_flash_encrypt_is_write_protected(bool print_error)
Returns True if the write protection of FLASH_CRYPT_CNT is set. 

Parameters
print_error -- Print error if it is write protected

Returns
true - if FLASH_CRYPT_CNT is write protected 



```

#### esp_flash_encrypt_region


```


esp_err_t esp_flash_encrypt_region(uint32_t src_addr, size_t data_length)
Encrypt-in-place a block of flash sectors. 

Note
This function resets RTC_WDT between operations with sectors. 


Parameters

src_addr -- Source offset in flash. Should be multiple of 4096 bytes. 
data_length -- Length of data to encrypt in bytes. Will be rounded up to next multiple of 4096 bytes.


Returns
ESP_OK if all operations succeeded, ESP_ERR_FLASH_OP_FAIL if SPI flash fails, ESP_ERR_FLASH_OP_TIMEOUT if flash times out. 



```

#### esp_flash_write_protect_crypt_cnt


```


void esp_flash_write_protect_crypt_cnt(void)
Write protect FLASH_CRYPT_CNT. 
Intended to be called as a part of boot process if flash encryption is enabled but secure boot is not used. This should protect against serial re-flashing of an unauthorised code in absence of secure boot.

Note
On ESP32 V3 only, write protecting FLASH_CRYPT_CNT will also prevent disabling UART Download Mode. If both are wanted, call esp_efuse_disable_rom_download_mode() before calling this function. 


```

#### esp_get_flash_encryption_mode


```


esp_flash_enc_mode_t esp_get_flash_encryption_mode(void)
Return the flash encryption mode. 
The API is called during boot process but can also be called by application to check the current flash encryption mode of ESP32

Returns




```

#### esp_flash_encryption_init_checks


```


void esp_flash_encryption_init_checks(void)
Check the flash encryption mode during startup. 

Verifies the flash encryption config during startup:

Correct any insecure flash encryption settings if hardware Secure Boot is enabled.
Log warnings if the efuse config doesn't match the project config in any way 



Note
This function is called automatically during app startup, it doesn't need to be called from the app.


```

#### esp_flash_encryption_cfg_verify_release_mode


```


bool esp_flash_encryption_cfg_verify_release_mode(void)
Returns the verification status for all physical security features of flash encryption in release mode. 
If the device has flash encryption feature configured in the release mode, then it is highly recommended to call this API in the application startup code. This API verifies the sanity of the eFuse configuration against the release (production) mode of the flash encryption feature.

Returns

True - all eFuses are configured correctly
False - not all eFuses are configured correctly. 





```

#### esp_flash_encryption_set_release_mode


```


void esp_flash_encryption_set_release_mode(void)
Switches Flash Encryption from "Development" to "Release". 
If already in "Release" mode, the function will do nothing. If flash encryption efuse is not enabled yet then abort. It burns:
"disable encrypt in dl mode"
set FLASH_CRYPT_CNT efuse to max 



```



### Enumerations


#### esp_flash_enc_mode_t


```


enum esp_flash_enc_mode_t
Values:
ESP_FLASH_ENC_MODE_DISABLED

enumerator ESP_FLASH_ENC_MODE_DISABLED

ESP_FLASH_ENC_MODE_DEVELOPMENT

enumerator ESP_FLASH_ENC_MODE_DEVELOPMENT

ESP_FLASH_ENC_MODE_RELEASE

enumerator ESP_FLASH_ENC_MODE_RELEASE


```