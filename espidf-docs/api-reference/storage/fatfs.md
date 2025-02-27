# FAT Filesystem Support



ESP-IDF uses the FatFs library to work with FAT filesystems. FatFs resides in the `fatfs` component. Although the library can be used directly, many of its features can be accessed via VFS using the C standard library and POSIX API functions.


Additionally, FatFs has been modified to support the runtime pluggable disk I/O layer. This allows mapping of FatFs drives to physical disks at runtime.



## Using FatFs with VFS


The header file fatfs/vfs/esp_vfs_fat.h defines the functions for connecting FatFs and VFS.


The function `esp_vfs_fat_register()` allocates a `FATFS` structure and registers a given path prefix in VFS. Subsequent operations on files starting with this prefix are forwarded to FatFs APIs.


The function `esp_vfs_fat_unregister_path()` deletes the registration with VFS, and frees the `FATFS` structure.


Most applications use the following workflow when working with `esp_vfs_fat_` functions:


1. Call `esp_vfs_fat_register()` to specify:
	* Path prefix where to mount the filesystem (e.g., `"/sdcard"`, `"/spiflash"`)
	* FatFs drive number
	* A variable which receives the pointer to the `FATFS` structure
2. Call `ff_diskio_register()` to register the disk I/O driver for the drive number used in Step 1.
3. To mount the filesystem using the same drive number which was passed to `esp_vfs_fat_register()`, call the FatFs function `f_mount()`. If the filesystem is not present on the target logical drive, `f_mount()` will fail with the `FR_NO_FILESYSTEM` error. In such case, call `f_mkfs()` to create a fresh FatFS structure on the drive first, and then call `f_mount()` again. Note that SD cards need to be partitioned with `f_fdisk()` prior to previously described steps. For more information, see FatFs documentation.
4. Call the C standard library and POSIX API functions to perform such actions on files as open, read, write, erase, copy, etc. Use paths starting with the path prefix passed to `esp_vfs_register()` (for example, `"/sdcard/hello.txt"`). The filesystem uses 8.3 filenames format (SFN) by default. If you need to use long filenames (LFN), enable the CONFIG_FATFS_LONG_FILENAMES option. Please refer to FatFs filenames for more details.
5. Optionally, call the FatFs library functions directly. In this case, use paths without a VFS prefix, for example, `"/hello.txt"`.
6. Close all open files.
7. Call the FatFs function `f_mount()` for the same drive number with NULL `FATFS*` argument to unmount the filesystem.
8. Call the FatFs function `ff_diskio_register()` with NULL `ff_diskio_impl_t*` argument and the same drive number to unregister the disk I/O driver.
9. Call `esp_vfs_fat_unregister_path()` with the path where the file system is mounted to remove FatFs from VFS, and free the `FATFS` structure allocated in Step 1.


The convenience functions `esp_vfs_fat_sdmmc_mount()`, `esp_vfs_fat_sdspi_mount()`, and `esp_vfs_fat_sdcard_unmount()` wrap the steps described above and also handle SD card initialization. These functions are described in the next section.



Note


Because FAT filesystem does not support hardlinks, `link()` copies contents of the file instead. (This only applies to files on FatFs volumes.)





## Using FatFs with VFS and SD Cards


The header file fatfs/vfs/esp_vfs_fat.h defines convenience functions `esp_vfs_fat_sdmmc_mount()`, `esp_vfs_fat_sdspi_mount()`, and `esp_vfs_fat_sdcard_unmount()`. These functions perform Steps 1–3 and 7–9 respectively and handle SD card initialization, but provide only limited error handling. Developers are encouraged to check its source code and incorporate more advanced features into production applications.


The convenience function `esp_vfs_fat_sdmmc_unmount()` unmounts the filesystem and releases the resources acquired by `esp_vfs_fat_sdmmc_mount()`.




## Using FatFs with VFS in Read-Only Mode


The header file fatfs/vfs/esp_vfs_fat.h also defines the convenience functions `esp_vfs_fat_spiflash_mount_ro()` and `esp_vfs_fat_spiflash_unmount_ro()`. These functions perform Steps 1-3 and 7-9 respectively for read-only FAT partitions. These are particularly helpful for data partitions written only once during factory provisioning, which will not be changed by production application throughout the lifetime of the hardware.




## Configuration options


The following configuration options are available for the FatFs component:


* CONFIG_FATFS_USE_FASTSEEK - If enabled, the POSIX `lseek()` function will be performed faster. The fast seek does not work for files in write mode, so to take advantage of fast seek, you should open (or close and then reopen) the file in read-only mode.
* CONFIG_FATFS_IMMEDIATE_FSYNC - If enabled, the FatFs will automatically call `f_sync()` to flush recent file changes after each call of `write()`, `pwrite()`, `link()`, `truncate()` and `ftruncate()` functions. This feature improves file-consistency and size reporting accuracy for the FatFs, at a price on decreased performance due to frequent disk operations.
* CONFIG_FATFS_LINK_LOCK - If enabled, this option guarantees the API thread safety, while disabling this option might be necessary for applications that require fast frequent small file operations (e.g., logging to a file). Note that if this option is disabled, the copying performed by `link()` will be non-atomic. In such case, using `link()` on a large file on the same volume in a different task is not guaranteed to be thread safe.




## FatFS Disk IO Layer


FatFs has been extended with API functions that register the disk I/O driver at runtime.


These APIs provide implementation of disk I/O functions for SD/MMC cards and can be registered for the given FatFs drive number using the function `ff_diskio_register_sdmmc()`.


#### ff_diskio_register


```


void ff_diskio_register(BYTE pdrv, const ff_diskio_impl_t *discio_impl)
Register or unregister diskio driver for given drive number.
When FATFS library calls one of disk_xxx functions for driver number pdrv, corresponding function in discio_impl for given pdrv will be called.

Parameters

pdrv -- drive number 
discio_impl -- pointer to ff_diskio_impl_t structure with diskio functions or NULL to unregister and free previously registered drive 




```

#### ff_diskio_impl_t


```


struct ff_diskio_impl_t
Structure of pointers to disk IO driver functions.
See FatFs documentation for details about these functions 

Public Members
init

DSTATUS (*init)(unsigned char pdrv)
disk initialization function 

status

DSTATUS (*status)(unsigned char pdrv)
disk status check function 

read

DRESULT (*read)(unsigned char pdrv, unsigned char *buff, uint32_t sector, unsigned count)
sector read function 

write

DRESULT (*write)(unsigned char pdrv, const unsigned char *buff, uint32_t sector, unsigned count)
sector write function 

ioctl

DRESULT (*ioctl)(unsigned char pdrv, unsigned char cmd, void *buff)
function to get info about disk and do some misc operations 



```

#### ff_diskio_register_sdmmc


```


void ff_diskio_register_sdmmc(unsigned char pdrv, sdmmc_card_t *card)
Register SD/MMC diskio driver

Parameters

pdrv -- drive number 
card -- pointer to sdmmc_card_t structure describing a card; card should be initialized before calling f_mount. 




```

#### ff_diskio_register_wl_partition


```


esp_err_t ff_diskio_register_wl_partition(unsigned char pdrv, wl_handle_t flash_handle)
Register spi flash partition

Parameters

pdrv -- drive number 
flash_handle -- handle of the wear levelling partition. 




```

#### ff_diskio_register_raw_partition


```


esp_err_t ff_diskio_register_raw_partition(unsigned char pdrv, const esp_partition_t *part_handle)
Register spi flash partition

Parameters

pdrv -- drive number 
part_handle -- pointer to raw flash partition. 




```



## FatFs Partition Generator


We provide a partition generator for FatFs (wl_fatfsgen.py ) which is integrated into the build system and could be easily used in the user project.


The tool is used to create filesystem images on a host and populate it with content of the specified host folder.


The script is based on the partition generator (fatfsgen.py ). Apart from generating partition, it can also initialize wear levelling.


The latest version supports both short and long file names, FAT12 and FAT16. The long file names are limited to 255 characters and can contain multiple periods (`.`) characters within the filename and additional characters `+`, `,`, `;`, `=`, `[` and `]`.


An in-depth description of the FatFs partition generator and analyzer can be found at Generating and parsing FAT partition on host.



### Build System Integration with FatFs Partition Generator


It is possible to invoke FatFs generator directly from the CMake build system by calling `fatfs_create_spiflash_image`:



```
fatfs_create_spiflash_image(<partition> <base_dir> [FLASH_IN_PROJECT])

```


If you prefer generating partition without wear levelling support, you can use `fatfs_create_rawflash_image`:



```
fatfs_create_rawflash_image(<partition> <base_dir> [FLASH_IN_PROJECT])

```


`fatfs_create_spiflash_image` respectively `fatfs_create_rawflash_image` must be called from project's CMakeLists.txt.


If you decide for any reason to use `fatfs_create_rawflash_image` (without wear levelling support), beware that it supports mounting only in read-only mode in the device.


The arguments of the function are as follows:


1. partition - the name of the partition as defined in the partition table (e.g., storage/fatfsgen/partitions_example.csv).
2. base_dir - the directory that will be encoded to FatFs partition and optionally flashed into the device. Beware that you have to specify the suitable size of the partition in the partition table.
3. flag `FLASH_IN_PROJECT` - optionally, users can have the image automatically flashed together with the app binaries, partition tables, etc. on `idf.py flash -p <PORT>` by specifying `FLASH_IN_PROJECT`.
4. flag `PRESERVE_TIME` - optionally, users can force preserving the timestamps from the source folder to the target image. Without preserving the time, every timestamp will be set to the FATFS default initial time (1st January 1980).
5. flag `ONE_FAT` - optionally, users can still choose to generate a FATFS volume with a single FAT (file allocation table) instead of two. This makes the free space in the FATFS volume a bit larger (by `number of sectors used by FAT * sector size`) but also more prone to corruption.


For example:



```
fatfs_create_spiflash_image(my_fatfs_partition my_folder FLASH_IN_PROJECT)

```


If FLASH_IN_PROJECT is not specified, the image will still be generated, but you will have to flash it manually using `esptool.py` or a custom build system target.


For an example, see storage/fatfsgen.





## FatFs Partition Analyzer


(fatfsparse.py ) is a partition analyzing tool for FatFs.


It is a reverse tool of (fatfsgen.py ), i.e., it can generate the folder structure on the host based on the FatFs image.


Usage:



```
./fatfsparse.py [-h] [--wl-layer {detect,enabled,disabled}] [--verbose] fatfs_image.img

```


Parameter --verbose prints detailed information from boot sector of the FatFs image to the terminal before folder structure is generated.




## High-level API Reference



### Header File


* components/fatfs/vfs/esp_vfs_fat.h
* This header file can be included with:



> ```
> #include "esp_vfs_fat.h"
> 
> ```
* This header file is a part of the API provided by the `fatfs` component. To declare that your component depends on `fatfs`, add the following to your CMakeLists.txt:



> ```
> REQUIRES fatfs
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES fatfs
> 
> ```




### Functions


#### esp_vfs_fat_register_cfg


```


esp_err_t esp_vfs_fat_register_cfg(const esp_vfs_fat_conf_t *conf, FATFS **out_fs)
Register FATFS with VFS component. 
This function registers given FAT drive in VFS, at the specified base path. Input arguments are held in esp_vfs_fat_conf_t structure. If only one drive is used, fat_drive argument can be an empty string. Refer to FATFS library documentation on how to specify FAT drive. This function also allocates FATFS structure which should be used for f_mount call.

Note
This function doesn't mount the drive into FATFS, it just connects POSIX and C standard library IO function with FATFS. You need to mount desired drive into FATFS separately.


Parameters

conf -- pointer to esp_vfs_fat_conf_t configuration structure 
out_fs -- [out] pointer to FATFS structure which can be used for FATFS f_mount call is returned via this argument. 


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if esp_vfs_fat_register was already called
ESP_ERR_NO_MEM if not enough memory or too many VFSes already registered 





```

#### esp_vfs_fat_unregister_path


```


esp_err_t esp_vfs_fat_unregister_path(const char *base_path)
Un-register FATFS from VFS. 

Note
FATFS structure returned by esp_vfs_fat_register is destroyed after this call. Make sure to call f_mount function to unmount it before calling esp_vfs_fat_unregister_ctx. Difference between this function and the one above is that this one will release the correct drive, while the one above will release the last registered one


Parameters
base_path -- path prefix where FATFS is registered. This is the same used when esp_vfs_fat_register was called 

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if FATFS is not registered in VFS 





```

#### esp_vfs_fat_sdmmc_mount


```


esp_err_t esp_vfs_fat_sdmmc_mount(const char *base_path, const sdmmc_host_t *host_config, const void *slot_config, const esp_vfs_fat_mount_config_t *mount_config, sdmmc_card_t **out_card)
Convenience function to get FAT filesystem on SD card registered in VFS. 
This is an all-in-one function which does the following:
initializes SDMMC driver or SPI driver with configuration in host_config
initializes SD card with configuration in slot_config
mounts FAT partition on SD card using FATFS library, with configuration in mount_config
registers FATFS library with VFS, with prefix given by base_prefix variable


This function is intended to make example code more compact. For real world applications, developers should implement the logic of probing SD card, locating and mounting partition, and registering FATFS in VFS, with proper error checking and handling of exceptional conditions.

Note
Use this API to mount a card through SDSPI is deprecated. Please call esp_vfs_fat_sdspi_mount() instead for that case.


Parameters

base_path -- path where partition should be registered (e.g. "/sdcard") 
host_config -- Pointer to structure describing SDMMC host. When using SDMMC peripheral, this structure can be initialized using SDMMC_HOST_DEFAULT() macro. When using SPI peripheral, this structure can be initialized using SDSPI_HOST_DEFAULT() macro. 
slot_config -- Pointer to structure with slot configuration. For SDMMC peripheral, pass a pointer to sdmmc_slot_config_t structure initialized using SDMMC_SLOT_CONFIG_DEFAULT. 
mount_config -- pointer to structure with extra parameters for mounting FATFS 
out_card -- [out] if not NULL, pointer to the card information structure will be returned via this argument 


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if esp_vfs_fat_sdmmc_mount was already called
ESP_ERR_NO_MEM if memory can not be allocated
ESP_FAIL if partition can not be mounted
other error codes from SDMMC or SPI drivers, SDMMC protocol, or FATFS drivers 





```

#### esp_vfs_fat_sdspi_mount


```


esp_err_t esp_vfs_fat_sdspi_mount(const char *base_path, const sdmmc_host_t *host_config_input, const sdspi_device_config_t *slot_config, const esp_vfs_fat_mount_config_t *mount_config, sdmmc_card_t **out_card)
Convenience function to get FAT filesystem on SD card registered in VFS. 
This is an all-in-one function which does the following:
initializes an SPI Master device based on the SPI Master driver with configuration in slot_config, and attach it to an initialized SPI bus.
initializes SD card with configuration in host_config_input
mounts FAT partition on SD card using FATFS library, with configuration in mount_config
registers FATFS library with VFS, with prefix given by base_prefix variable


This function is intended to make example code more compact. For real world applications, developers should implement the logic of probing SD card, locating and mounting partition, and registering FATFS in VFS, with proper error checking and handling of exceptional conditions.

Note
This function try to attach the new SD SPI device to the bus specified in host_config. Make sure the SPI bus specified in host_config->slot have been initialized by spi_bus_initialize() before.


Parameters

base_path -- path where partition should be registered (e.g. "/sdcard") 
host_config_input -- Pointer to structure describing SDMMC host. This structure can be initialized using SDSPI_HOST_DEFAULT() macro. 
slot_config -- Pointer to structure with slot configuration. For SPI peripheral, pass a pointer to sdspi_device_config_t structure initialized using SDSPI_DEVICE_CONFIG_DEFAULT(). 
mount_config -- pointer to structure with extra parameters for mounting FATFS 
out_card -- [out] If not NULL, pointer to the card information structure will be returned via this argument. It is suggested to hold this handle and use it to unmount the card later if needed. Otherwise it's not suggested to use more than one card at the same time and unmount one of them in your application. 


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if esp_vfs_fat_sdmmc_mount was already called
ESP_ERR_NO_MEM if memory can not be allocated
ESP_FAIL if partition can not be mounted
other error codes from SDMMC or SPI drivers, SDMMC protocol, or FATFS drivers 





```

#### esp_vfs_fat_sdmmc_unmount


```


esp_err_t esp_vfs_fat_sdmmc_unmount(void)
Unmount FAT filesystem and release resources acquired using esp_vfs_fat_sdmmc_mount. 


Deprecated:
Use esp_vfs_fat_sdcard_unmount() instead.



Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if esp_vfs_fat_sdmmc_mount hasn't been called 





```

#### esp_vfs_fat_sdcard_unmount


```


esp_err_t esp_vfs_fat_sdcard_unmount(const char *base_path, sdmmc_card_t *card)
Unmount an SD card from the FAT filesystem and release resources acquired using esp_vfs_fat_sdmmc_mount() or esp_vfs_fat_sdspi_mount()

Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if the card argument is unregistered
ESP_ERR_INVALID_STATE if esp_vfs_fat_sdmmc_mount hasn't been called 





```

#### esp_vfs_fat_sdcard_format_cfg


```


esp_err_t esp_vfs_fat_sdcard_format_cfg(const char *base_path, sdmmc_card_t *card, esp_vfs_fat_mount_config_t *cfg)
Format FAT filesystem with given configuration. 

Note
This API should be only called when the FAT is already mounted.


Parameters

base_path -- Path where partition should be registered (e.g. "/sdcard") 
card -- Pointer to the card handle, which should be initialised by calling esp_vfs_fat_sdspi_mount first 
cfg -- Pointer to structure with extra parameters for formatting FATFS (only relevant fields are used). If NULL, the previous configuration will be used.


Returns

ESP_OK
ESP_ERR_INVALID_STATE: FAT partition isn't mounted, call esp_vfs_fat_sdmmc_mount or esp_vfs_fat_sdspi_mount first
ESP_ERR_NO_MEM: if memory can not be allocated
ESP_FAIL: fail to format it, or fail to mount back 





```

#### esp_vfs_fat_sdcard_format


```


esp_err_t esp_vfs_fat_sdcard_format(const char *base_path, sdmmc_card_t *card)
Format FAT filesystem. 

Note
This API should be only called when the FAT is already mounted.


Parameters

base_path -- Path where partition should be registered (e.g. "/sdcard") 
card -- Pointer to the card handle, which should be initialised by calling esp_vfs_fat_sdspi_mount first


Returns

ESP_OK
ESP_ERR_INVALID_STATE: FAT partition isn't mounted, call esp_vfs_fat_sdmmc_mount or esp_vfs_fat_sdspi_mount first
ESP_ERR_NO_MEM: if memory can not be allocated
ESP_FAIL: fail to format it, or fail to mount back 





```

#### esp_vfs_fat_spiflash_mount_rw_wl


```


esp_err_t esp_vfs_fat_spiflash_mount_rw_wl(const char *base_path, const char *partition_label, const esp_vfs_fat_mount_config_t *mount_config, wl_handle_t *wl_handle)
Convenience function to initialize FAT filesystem in SPI flash and register it in VFS. 
This is an all-in-one function which does the following:

finds the partition with defined partition_label. Partition label should be configured in the partition table.
initializes flash wear levelling library on top of the given partition
mounts FAT partition using FATFS library on top of flash wear levelling library
registers FATFS library with VFS, with prefix given by base_prefix variable


This function is intended to make example code more compact.

Parameters

base_path -- path where FATFS partition should be mounted (e.g. "/spiflash") 
partition_label -- label of the partition which should be used 
mount_config -- pointer to structure with extra parameters for mounting FATFS 
wl_handle -- [out] wear levelling driver handle 


Returns

ESP_OK on success
ESP_ERR_NOT_FOUND if the partition table does not contain FATFS partition with given label
ESP_ERR_INVALID_STATE if esp_vfs_fat_spiflash_mount_rw_wl was already called
ESP_ERR_NO_MEM if memory can not be allocated
ESP_FAIL if partition can not be mounted
other error codes from wear levelling library, SPI flash driver, or FATFS drivers 





```

#### esp_vfs_fat_spiflash_unmount_rw_wl


```


esp_err_t esp_vfs_fat_spiflash_unmount_rw_wl(const char *base_path, wl_handle_t wl_handle)
Unmount FAT filesystem and release resources acquired using esp_vfs_fat_spiflash_mount_rw_wl. 

Parameters

base_path -- path where partition should be registered (e.g. "/spiflash") 
wl_handle -- wear levelling driver handle returned by esp_vfs_fat_spiflash_mount_rw_wl


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if esp_vfs_fat_spiflash_mount_rw_wl hasn't been called 





```

#### esp_vfs_fat_spiflash_format_cfg_rw_wl


```


esp_err_t esp_vfs_fat_spiflash_format_cfg_rw_wl(const char *base_path, const char *partition_label, esp_vfs_fat_mount_config_t *cfg)
Format FAT filesystem with given configuration. 

Note
This API can be called when the FAT is mounted / not mounted. If this API is called when the FAT isn't mounted (by calling esp_vfs_fat_spiflash_mount_rw_wl), this API will first mount the FAT then format it, then restore back to the original state.


Parameters

base_path -- Path where partition should be registered (e.g. "/spiflash") 
partition_label -- Label of the partition which should be used 
cfg -- Pointer to structure with extra parameters for formatting FATFS (only relevant fields are used). If NULL and mounted the previous configuration will be used. If NULL and unmounted the default configuration will be used.


Returns

ESP_OK
ESP_ERR_NO_MEM: if memory can not be allocated
Other errors from esp_vfs_fat_spiflash_mount_rw_wl 





```

#### esp_vfs_fat_spiflash_format_rw_wl


```


esp_err_t esp_vfs_fat_spiflash_format_rw_wl(const char *base_path, const char *partition_label)
Format FAT filesystem. 

Note
This API can be called when the FAT is mounted / not mounted. If this API is called when the FAT isn't mounted (by calling esp_vfs_fat_spiflash_mount_rw_wl), this API will first mount the FAT then format it, then restore back to the original state.


Parameters

base_path -- Path where partition should be registered (e.g. "/spiflash") 
partition_label -- Label of the partition which should be used


Returns

ESP_OK
ESP_ERR_NO_MEM: if memory can not be allocated
Other errors from esp_vfs_fat_spiflash_mount_rw_wl 





```

#### esp_vfs_fat_spiflash_mount_ro


```


esp_err_t esp_vfs_fat_spiflash_mount_ro(const char *base_path, const char *partition_label, const esp_vfs_fat_mount_config_t *mount_config)
Convenience function to initialize read-only FAT filesystem and register it in VFS. 
This is an all-in-one function which does the following:

finds the partition with defined partition_label. Partition label should be configured in the partition table.
mounts FAT partition using FATFS library
registers FATFS library with VFS, with prefix given by base_prefix variable



Note
Wear levelling is not used when FAT is mounted in read-only mode using this function.


Parameters

base_path -- path where FATFS partition should be mounted (e.g. "/spiflash") 
partition_label -- label of the partition which should be used 
mount_config -- pointer to structure with extra parameters for mounting FATFS 


Returns

ESP_OK on success
ESP_ERR_NOT_FOUND if the partition table does not contain FATFS partition with given label
ESP_ERR_INVALID_STATE if esp_vfs_fat_spiflash_mount_ro was already called for the same partition
ESP_ERR_NO_MEM if memory can not be allocated
ESP_FAIL if partition can not be mounted
other error codes from SPI flash driver, or FATFS drivers 





```

#### esp_vfs_fat_spiflash_unmount_ro


```


esp_err_t esp_vfs_fat_spiflash_unmount_ro(const char *base_path, const char *partition_label)
Unmount FAT filesystem and release resources acquired using esp_vfs_fat_spiflash_mount_ro. 

Parameters

base_path -- path where partition should be registered (e.g. "/spiflash") 
partition_label -- label of partition to be unmounted


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if esp_vfs_fat_spiflash_mount_ro hasn't been called 





```

#### esp_vfs_fat_info


```


esp_err_t esp_vfs_fat_info(const char *base_path, uint64_t *out_total_bytes, uint64_t *out_free_bytes)
Get information for FATFS partition. 

Parameters

base_path -- Base path of the partition examined (e.g. "/spiflash") 
out_total_bytes -- [out] Size of the file system 
out_free_bytes -- [out] Free bytes available in the file system 


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if partition not found
ESP_FAIL if another FRESULT error (saved in errno) 





```

#### esp_vfs_fat_create_contiguous_file


```


esp_err_t esp_vfs_fat_create_contiguous_file(const char *base_path, const char *full_path, uint64_t size, bool alloc_now)
Create a file with contiguous space at given path. 

Note
The file cannot exist before calling this function (or the file size has to be 0) For more information see documentation for f_expand from FATFS library


Parameters

base_path -- Base path of the partition examined (e.g. "/spiflash") 
full_path -- Full path of the file (e.g. "/spiflash/ABC.TXT") 
size -- File size expanded to, number of bytes in size to prepare or allocate for the file 
alloc_now -- True == allocate space now, false == prepare to allocate &#8212; see f_expand from FATFS 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if invalid arguments (e.g. any of arguments are NULL or size lower or equal to 0)
ESP_ERR_INVALID_STATE if partition not found
ESP_FAIL if another FRESULT error (saved in errno) 





```

#### esp_vfs_fat_test_contiguous_file


```


esp_err_t esp_vfs_fat_test_contiguous_file(const char *base_path, const char *full_path, bool *is_contiguous)
Test if a file is contiguous in the FAT filesystem. 

Parameters

base_path -- Base path of the partition examined (e.g. "/spiflash") 
full_path -- Full path of the file (e.g. "/spiflash/ABC.TXT") 
is_contiguous -- [out] True == allocate space now, false == prepare to allocate &#8212; see f_expand from FATFS 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if invalid arguments (e.g. any of arguments are NULL)
ESP_ERR_INVALID_STATE if partition not found
ESP_FAIL if another FRESULT error (saved in errno) 





```



### Structures


#### esp_vfs_fat_conf_t


```


struct esp_vfs_fat_conf_t
Configuration structure for esp_vfs_fat_register. 

Public Members
base_path

const char *base_path
Path prefix where FATFS should be registered, 

fat_drive

const char *fat_drive
FATFS drive specification; if only one drive is used, can be an empty string. 

max_files

size_t max_files
Maximum number of files which can be open at the same time. 



```

#### esp_vfs_fat_mount_config_t


```


struct esp_vfs_fat_mount_config_t
Configuration arguments for esp_vfs_fat_sdmmc_mount and esp_vfs_fat_spiflash_mount_rw_wl functions. 

Public Members
format_if_mount_failed

bool format_if_mount_failed
If FAT partition can not be mounted, and this parameter is true, create partition table and format the filesystem. 

max_files

int max_files
Max number of open files. 

allocation_unit_size

size_t allocation_unit_size
If format_if_mount_failed is set, and mount fails, format the card with given allocation unit size. Must be a power of 2, between sector size and 128 * sector size. For SD cards, sector size is always 512 bytes. For wear_levelling, sector size is determined by CONFIG_WL_SECTOR_SIZE option.
Using larger allocation unit size will result in higher read/write performance and higher overhead when storing small files.
Setting this field to 0 will result in allocation unit set to the sector size. 

disk_status_check_enable

bool disk_status_check_enable
Enables real ff_disk_status function implementation for SD cards (ff_sdmmc_status). Possibly slows down IO performance.
Try to enable if you need to handle situations when SD cards are not unmounted properly before physical removal or you are experiencing issues with SD cards.
Doesn't do anything for other memory storage media. 

use_one_fat

bool use_one_fat
Use 1 FAT (File Allocation Tables) instead of 2. This decreases reliability, but makes more space available (usually only one sector). Note that this option has effect only when the filesystem is formatted. When mounting an already-formatted partition, the actual number of FATs may be different. 



```



### Macros


#### VFS_FAT_MOUNT_DEFAULT_CONFIG


```


VFS_FAT_MOUNT_DEFAULT_CONFIG()

```



### Type Definitions


#### esp_vfs_fat_sdmmc_mount_config_t


```


typedef esp_vfs_fat_mount_config_t esp_vfs_fat_sdmmc_mount_config_t

```