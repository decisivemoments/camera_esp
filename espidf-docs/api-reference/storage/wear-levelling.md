# Wear Levelling API




## Overview


Most of flash memory and especially SPI flash that is used in ESP32-S3 has a sector-based organization and also has a limited number of erase/modification cycles per memory sector. The wear levelling component helps to distribute wear and tear among sectors more evenly without requiring any attention from the user.


The wear levelling component provides API functions related to reading, writing, erasing, and memory mapping of data in external SPI flash through the partition component. The component also has higher-level API functions which work with the FAT filesystem defined in FAT filesystem.


The wear levelling component, together with the FAT FS component, uses FAT FS sectors of 4096 bytes, which is a standard size for flash memory. With this size, the component shows the best performance but needs additional memory in RAM.


To save internal memory, the component has two additional modes which both use sectors of 512 bytes:


* **Performance mode.** Erase sector operation data is stored in RAM, the sector is erased, and then data is copied back to flash memory. However, if a device is powered off for any reason, all 4096 bytes of data is lost.
* **Safety mode.** The data is first saved to flash memory, and after the sector is erased, the data is saved back. If a device is powered off, the data can be recovered as soon as the device boots up.


The default settings are as follows:


* Sector size is 512 bytes
* Performance mode


You can change the settings through the configuration menu.


The wear levelling component does not cache data in RAM. The write and erase functions modify flash directly, and flash contents are consistent when the function returns.




## Wear Levelling access API functions


This is the set of API functions for working with data in flash:


* `wl_mount` - initializes the wear levelling module and mounts the specified partition
* `wl_unmount` - unmounts the partition and deinitializes the wear levelling module
* `wl_erase_range` - erases a range of addresses in flash
* `wl_write` - writes data to a partition
* `wl_read` - reads data from a partition
* `wl_size` - returns the size of available memory in bytes
* `wl_sector_size` - returns the size of one sector


As a rule, try to avoid using raw wear levelling functions and use filesystem-specific functions instead.




## Memory Size


The memory size is calculated in the wear levelling module based on partition parameters. The module uses some sectors of flash for internal data.




## See Also


* FAT Filesystem Support
* Partition Tables




## Application Example


An example that combines the wear levelling driver with the FATFS library is provided in the storage/wear_levelling directory. This example initializes the wear levelling driver, mounts FatFs partition, as well as writes and reads data from it using POSIX and C library APIs. See storage/wear_levelling/README.md for more information.




## High-level API Reference



### Header Files


* fatfs/vfs/esp_vfs_fat.h


High-level wear levelling functions `esp_vfs_fat_spiflash_mount_rw_wl()`, `esp_vfs_fat_spiflash_unmount_rw_wl()` and struct `esp_vfs_fat_mount_config_t` are described in FAT Filesystem Support.





## Mid-level API Reference



### Header File


* components/wear_levelling/include/wear_levelling.h
* This header file can be included with:



> ```
> #include "wear_levelling.h"
> 
> ```
* This header file is a part of the API provided by the `wear_levelling` component. To declare that your component depends on `wear_levelling`, add the following to your CMakeLists.txt:



> ```
> REQUIRES wear_levelling
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES wear_levelling
> 
> ```




### Functions


#### wl_mount


```


esp_err_t wl_mount(const esp_partition_t *partition, wl_handle_t *out_handle)
Mount WL for defined partition. 

Parameters

partition -- that will be used for access 
out_handle -- handle of the WL instance


Returns

ESP_OK, if the WL allocation is successful;
ESP_ERR_INVALID_ARG, if the arguments for WL configuration are not valid;
ESP_ERR_NO_MEM, if the WL allocation fails because of insufficient memory; 





```

#### wl_unmount


```


esp_err_t wl_unmount(wl_handle_t handle)
Unmount WL for defined partition. 

Parameters
handle -- WL partition handle

Returns

ESP_OK, if the operation is successful;
or one of error codes from lower-level flash driver. 





```

#### wl_erase_range


```


esp_err_t wl_erase_range(wl_handle_t handle, size_t start_addr, size_t size)
Erase part of the WL storage. 

Parameters

handle -- WL handle that are related to the partition 
start_addr -- Address from where erase operation should start. Must be aligned to the result of function wl_sector_size(...). 
size -- Size of the range which should be erased, in bytes. Must be divisible by the result of function wl_sector_size(...)..


Returns

ESP_OK, if the given range was erased successfully;
ESP_ERR_INVALID_ARG, if iterator or dst are NULL;
ESP_ERR_INVALID_SIZE, if erase would go out of bounds of the partition;
or one of error codes from lower-level flash driver. 





```

#### wl_write


```


esp_err_t wl_write(wl_handle_t handle, size_t dest_addr, const void *src, size_t size)
Write data to the WL storage. 
Before writing data to flash, corresponding region of flash needs to be erased. This can be done using wl_erase_range function.

Note
Prior to writing to WL storage, make sure it has been erased with wl_erase_range call.


Parameters

handle -- WL handle corresponding to the WL partition 
dest_addr -- Address where the data should be written, relative to the beginning of the partition. 
src -- Pointer to the source buffer. Pointer must be non-NULL and buffer must be at least 'size' bytes long. 
size -- Size of data to be written, in bytes.


Returns

ESP_OK, if data was written successfully;
ESP_ERR_INVALID_ARG, if dst_offset exceeds partition size;
ESP_ERR_INVALID_SIZE, if write would go out of bounds of the partition;
or one of error codes from lower-level flash driver. 





```

#### wl_read


```


esp_err_t wl_read(wl_handle_t handle, size_t src_addr, void *dest, size_t size)
Read data from the WL storage. 

Parameters

handle -- WL module instance that was initialized before 
dest -- Pointer to the buffer where data should be stored. The Pointer must be non-NULL and the buffer must be at least 'size' bytes long. 
src_addr -- Address of the data to be read, relative to the beginning of the partition. 
size -- Size of data to be read, in bytes.


Returns

ESP_OK, if data was read successfully;
ESP_ERR_INVALID_ARG, if src_offset exceeds partition size;
ESP_ERR_INVALID_SIZE, if read would go out of bounds of the partition;
or one of error codes from lower-level flash driver. 





```

#### wl_size


```


size_t wl_size(wl_handle_t handle)
Get the actual flash size in use for the WL storage partition. 

Parameters
handle -- WL module handle that was initialized before 

Returns
usable size, in bytes 



```

#### wl_sector_size


```


size_t wl_sector_size(wl_handle_t handle)
Get sector size of the WL instance. 

Parameters
handle -- WL module handle that was initialized before 

Returns
sector size, in bytes 



```



### Macros


#### WL_INVALID_HANDLE


```


WL_INVALID_HANDLE

```



### Type Definitions


#### wl_handle_t


```


typedef int32_t wl_handle_t
wear levelling handle 

```