# Partitions API




## Overview


The `esp_partition` component has higher-level API functions which work with partitions defined in the Partition Tables. These APIs are based on lower level API provided by SPI Flash API.




## Partition Table API


ESP-IDF projects use a partition table to maintain information about various regions of SPI flash memory (bootloader, various application binaries, data, filesystems). More information can be found in Partition Tables.


This component provides API functions to enumerate partitions found in the partition table and perform operations on them. These functions are declared in `esp_partition.h`:


* `esp_partition_find()` checks a partition table for entries with specific type, returns an opaque iterator.
* `esp_partition_get()` returns a structure describing the partition for a given iterator.
* `esp_partition_next()` shifts the iterator to the next found partition.
* `esp_partition_iterator_release()` releases iterator returned by `esp_partition_find()`.
* `esp_partition_find_first()` is a convenience function which returns the structure describing the first partition found by `esp_partition_find()`.
* `esp_partition_read()`, `esp_partition_write()`, `esp_partition_erase_range()` are equivalent to `esp_flash_read()`, `esp_flash_write()`, `esp_flash_erase_region()`, but operate within partition boundaries.




## See Also


* Partition Tables
* Over The Air Updates (OTA) provides high-level API for updating applications stored in flash.
* Non-Volatile Storage Library provides a structured API for storing small pieces of data in SPI flash.




## API Reference - Partition Table



### Header File


* components/esp_partition/include/esp_partition.h
* This header file can be included with:



> ```
> #include "esp_partition.h"
> 
> ```
* This header file is a part of the API provided by the `esp_partition` component. To declare that your component depends on `esp_partition`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_partition
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_partition
> 
> ```




### Functions


#### esp_partition_find


```


esp_partition_iterator_t esp_partition_find(esp_partition_type_t type, esp_partition_subtype_t subtype, const char *label)
Find partition based on one or more parameters. 

Parameters

type -- Partition type, one of esp_partition_type_t values or an 8-bit unsigned integer. To find all partitions, no matter the type, use ESP_PARTITION_TYPE_ANY, and set subtype argument to ESP_PARTITION_SUBTYPE_ANY. 
subtype -- Partition subtype, one of esp_partition_subtype_t values or an 8-bit unsigned integer. To find all partitions of given type, use ESP_PARTITION_SUBTYPE_ANY. 
label -- (optional) Partition label. Set this value if looking for partition with a specific name. Pass NULL otherwise.


Returns
iterator which can be used to enumerate all the partitions found, or NULL if no partitions were found. Iterator obtained through this function has to be released using esp_partition_iterator_release when not used any more. 



```

#### esp_partition_find_first


```


const esp_partition_t *esp_partition_find_first(esp_partition_type_t type, esp_partition_subtype_t subtype, const char *label)
Find first partition based on one or more parameters. 

Parameters

type -- Partition type, one of esp_partition_type_t values or an 8-bit unsigned integer. To find all partitions, no matter the type, use ESP_PARTITION_TYPE_ANY, and set subtype argument to ESP_PARTITION_SUBTYPE_ANY. 
subtype -- Partition subtype, one of esp_partition_subtype_t values or an 8-bit unsigned integer To find all partitions of given type, use ESP_PARTITION_SUBTYPE_ANY. 
label -- (optional) Partition label. Set this value if looking for partition with a specific name. Pass NULL otherwise.


Returns
pointer to esp_partition_t structure, or NULL if no partition is found. This pointer is valid for the lifetime of the application. 



```

#### esp_partition_get


```


const esp_partition_t *esp_partition_get(esp_partition_iterator_t iterator)
Get esp_partition_t structure for given partition. 

Parameters
iterator -- Iterator obtained using esp_partition_find. Must be non-NULL.

Returns
pointer to esp_partition_t structure. This pointer is valid for the lifetime of the application. 



```

#### esp_partition_next


```


esp_partition_iterator_t esp_partition_next(esp_partition_iterator_t iterator)
Move partition iterator to the next partition found. 
Any copies of the iterator will be invalid after this call.

Parameters
iterator -- Iterator obtained using esp_partition_find. Must be non-NULL.

Returns
NULL if no partition was found, valid esp_partition_iterator_t otherwise. 



```

#### esp_partition_iterator_release


```


void esp_partition_iterator_release(esp_partition_iterator_t iterator)
Release partition iterator. 

Parameters
iterator -- Iterator obtained using esp_partition_find. The iterator is allowed to be NULL, so it is not necessary to check its value before calling this function. 



```

#### esp_partition_verify


```


const esp_partition_t *esp_partition_verify(const esp_partition_t *partition)
Verify partition data. 
Given a pointer to partition data, verify this partition exists in the partition table (all fields match.)
This function is also useful to take partition data which may be in a RAM buffer and convert it to a pointer to the permanent partition data stored in flash.
Pointers returned from this function can be compared directly to the address of any pointer returned from esp_partition_get(), as a test for equality.

Parameters
partition -- Pointer to partition data to verify. Must be non-NULL. All fields of this structure must match the partition table entry in flash for this function to return a successful match.

Returns

If partition not found, returns NULL.
If found, returns a pointer to the esp_partition_t structure in flash. This pointer is always valid for the lifetime of the application. 





```

#### esp_partition_read


```


esp_err_t esp_partition_read(const esp_partition_t *partition, size_t src_offset, void *dst, size_t size)
Read data from the partition. 
Partitions marked with an encryption flag will automatically be be read and decrypted via a cache mapping.

Parameters

partition -- Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
dst -- Pointer to the buffer where data should be stored. Pointer must be non-NULL and buffer must be at least 'size' bytes long. 
src_offset -- Address of the data to be read, relative to the beginning of the partition. 
size -- Size of data to be read, in bytes.


Returns
ESP_OK, if data was read successfully; ESP_ERR_INVALID_ARG, if src_offset exceeds partition size; ESP_ERR_INVALID_SIZE, if read would go out of bounds of the partition; or one of error codes from lower-level flash driver. 



```

#### esp_partition_write


```


esp_err_t esp_partition_write(const esp_partition_t *partition, size_t dst_offset, const void *src, size_t size)
Write data to the partition. 
Before writing data to flash, corresponding region of flash needs to be erased. This can be done using esp_partition_erase_range function.
Partitions marked with an encryption flag will automatically be written via the esp_flash_write_encrypted() function. If writing to an encrypted partition, all write offsets and lengths must be multiples of 16 bytes. See the esp_flash_write_encrypted() function for more details. Unencrypted partitions do not have this restriction.

Note
Prior to writing to flash memory, make sure it has been erased with esp_partition_erase_range call.


Parameters

partition -- Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
dst_offset -- Address where the data should be written, relative to the beginning of the partition. 
src -- Pointer to the source buffer. Pointer must be non-NULL and buffer must be at least 'size' bytes long. 
size -- Size of data to be written, in bytes.


Returns
ESP_OK, if data was written successfully; ESP_ERR_INVALID_ARG, if dst_offset exceeds partition size; ESP_ERR_INVALID_SIZE, if write would go out of bounds of the partition; ESP_ERR_NOT_ALLOWED, if partition is read-only; or one of error codes from lower-level flash driver. 



```

#### esp_partition_read_raw


```


esp_err_t esp_partition_read_raw(const esp_partition_t *partition, size_t src_offset, void *dst, size_t size)
Read data from the partition without any transformation/decryption. 

Note
This function is essentially the same as esp_partition_read() above. It just never decrypts data but returns it as is.


Parameters

partition -- Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
dst -- Pointer to the buffer where data should be stored. Pointer must be non-NULL and buffer must be at least 'size' bytes long. 
src_offset -- Address of the data to be read, relative to the beginning of the partition. 
size -- Size of data to be read, in bytes.


Returns
ESP_OK, if data was read successfully; ESP_ERR_INVALID_ARG, if src_offset exceeds partition size; ESP_ERR_INVALID_SIZE, if read would go out of bounds of the partition; or one of error codes from lower-level flash driver. 



```

#### esp_partition_write_raw


```


esp_err_t esp_partition_write_raw(const esp_partition_t *partition, size_t dst_offset, const void *src, size_t size)
Write data to the partition without any transformation/encryption. 

Before writing data to flash, corresponding region of flash needs to be erased. This can be done using esp_partition_erase_range function.

Note
This function is essentially the same as esp_partition_write() above. It just never encrypts data but writes it as is.


Note
Prior to writing to flash memory, make sure it has been erased with esp_partition_erase_range call.


Parameters

partition -- Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
dst_offset -- Address where the data should be written, relative to the beginning of the partition. 
src -- Pointer to the source buffer. Pointer must be non-NULL and buffer must be at least 'size' bytes long. 
size -- Size of data to be written, in bytes.


Returns
ESP_OK, if data was written successfully; ESP_ERR_INVALID_ARG, if dst_offset exceeds partition size; ESP_ERR_INVALID_SIZE, if write would go out of bounds of the partition; ESP_ERR_NOT_ALLOWED, if partition is read-only; or one of the error codes from lower-level flash driver. 



```

#### esp_partition_erase_range


```


esp_err_t esp_partition_erase_range(const esp_partition_t *partition, size_t offset, size_t size)
Erase part of the partition. 

Parameters

partition -- Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
offset -- Offset from the beginning of partition where erase operation should start. Must be aligned to partition->erase_size. 
size -- Size of the range which should be erased, in bytes. Must be divisible by partition->erase_size.


Returns
ESP_OK, if the range was erased successfully; ESP_ERR_INVALID_ARG, if iterator or dst are NULL; ESP_ERR_INVALID_SIZE, if erase would go out of bounds of the partition; ESP_ERR_NOT_ALLOWED, if partition is read-only; or one of error codes from lower-level flash driver. 



```

#### esp_partition_mmap


```


esp_err_t esp_partition_mmap(const esp_partition_t *partition, size_t offset, size_t size, esp_partition_mmap_memory_t memory, const void **out_ptr, esp_partition_mmap_handle_t *out_handle)
Configure MMU to map partition into data memory. 
Unlike spi_flash_mmap function, which requires a 64kB aligned base address, this function doesn't impose such a requirement. If offset results in a flash address which is not aligned to 64kB boundary, address will be rounded to the lower 64kB boundary, so that mapped region includes requested range. Pointer returned via out_ptr argument will be adjusted to point to the requested offset (not necessarily to the beginning of mmap-ed region).
To release mapped memory, pass handle returned via out_handle argument to esp_partition_munmap function.

Parameters

partition -- Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
offset -- Offset from the beginning of partition where mapping should start. 
size -- Size of the area to be mapped. 
memory -- Memory space where the region should be mapped 
out_ptr -- Output, pointer to the mapped memory region 
out_handle -- Output, handle which should be used for esp_partition_munmap call


Returns
ESP_OK, if successful 



```

#### esp_partition_munmap


```


void esp_partition_munmap(esp_partition_mmap_handle_t handle)
Release region previously obtained using esp_partition_mmap. 

Note
Calling this function will not necessarily unmap memory region. Region will only be unmapped when there are no other handles which reference this region. In case of partially overlapping regions it is possible that memory will be unmapped partially.


Parameters
handle -- Handle obtained from spi_flash_mmap 



```

#### esp_partition_get_sha256


```


esp_err_t esp_partition_get_sha256(const esp_partition_t *partition, uint8_t *sha_256)
Get SHA-256 digest for required partition. 
For apps with SHA-256 appended to the app image, the result is the appended SHA-256 value for the app image content. The hash is verified before returning, if app content is invalid then the function returns ESP_ERR_IMAGE_INVALID. For apps without SHA-256 appended to the image, the result is the SHA-256 of all bytes in the app image. For other partition types, the result is the SHA-256 of the entire partition.

Parameters

partition -- [in] Pointer to info for partition containing app or data. (fields: address, size and type, are required to be filled). 
sha_256 -- [out] Returned SHA-256 digest for a given partition.


Returns

ESP_OK: In case of successful operation.
ESP_ERR_INVALID_ARG: The size was 0 or the sha_256 was NULL.
ESP_ERR_NO_MEM: Cannot allocate memory for sha256 operation.
ESP_ERR_IMAGE_INVALID: App partition doesn't contain a valid app image.
ESP_FAIL: An allocation error occurred. 





```

#### esp_partition_check_identity


```


bool esp_partition_check_identity(const esp_partition_t *partition_1, const esp_partition_t *partition_2)
Check for the identity of two partitions by SHA-256 digest. 

Parameters

partition_1 -- [in] Pointer to info for partition 1 containing app or data. (fields: address, size and type, are required to be filled). 
partition_2 -- [in] Pointer to info for partition 2 containing app or data. (fields: address, size and type, are required to be filled).


Returns

True: In case of the two firmware is equal.
False: Otherwise 





```

#### esp_partition_register_external


```


esp_err_t esp_partition_register_external(esp_flash_t *flash_chip, size_t offset, size_t size, const char *label, esp_partition_type_t type, esp_partition_subtype_t subtype, const esp_partition_t **out_partition)
Register a partition on an external flash chip. 
This API allows designating certain areas of external flash chips (identified by the esp_flash_t structure) as partitions. This allows using them with components which access SPI flash through the esp_partition API.

Parameters

flash_chip -- Pointer to the structure identifying the flash chip 
offset -- Address in bytes, where the partition starts 
size -- Size of the partition in bytes 
label -- Partition name 
type -- One of the partition types (ESP_PARTITION_TYPE_*), or an integer. Note that applications can not be booted from external flash chips, so using ESP_PARTITION_TYPE_APP is not supported. 
subtype -- One of the partition subtypes (ESP_PARTITION_SUBTYPE_*), or an integer. 
out_partition -- [out] Output, if non-NULL, receives the pointer to the resulting esp_partition_t structure 


Returns

ESP_OK on success
ESP_ERR_NO_MEM if memory allocation has failed
ESP_ERR_INVALID_ARG if the new partition overlaps another partition on the same flash chip
ESP_ERR_INVALID_SIZE if the partition doesn't fit into the flash chip size 





```

#### esp_partition_deregister_external


```


esp_err_t esp_partition_deregister_external(const esp_partition_t *partition)
Deregister the partition previously registered using esp_partition_register_external. 

Parameters
partition -- pointer to the partition structure obtained from esp_partition_register_external, 

Returns

ESP_OK on success
ESP_ERR_NOT_FOUND if the partition pointer is not found
ESP_ERR_INVALID_ARG if the partition comes from the partition table
ESP_ERR_INVALID_ARG if the partition was not registered using esp_partition_register_external function. 





```

#### esp_partition_unload_all


```


void esp_partition_unload_all(void)
Unload partitions and free space allocated by them. 

```



### Structures


#### esp_partition_t


```


struct esp_partition_t
partition information structure 
This is not the format in flash, that format is esp_partition_info_t.
However, this is the format used by this API. 

Public Members
flash_chip

esp_flash_t *flash_chip
SPI flash chip on which the partition resides 

type

esp_partition_type_t type
partition type (app/data) 

subtype

esp_partition_subtype_t subtype
partition subtype 

address

uint32_t address
starting address of the partition in flash 

size

uint32_t size
size of the partition, in bytes 

erase_size

uint32_t erase_size
size the erase operation should be aligned to 

label

char label[17]
partition label, zero-terminated ASCII string 

encrypted

bool encrypted
flag is set to true if partition is encrypted 

readonly

bool readonly
flag is set to true if partition is read-only 



```



### Macros


#### ESP_PARTITION_SUBTYPE_OTA


```


ESP_PARTITION_SUBTYPE_OTA(i)
Convenience macro to get esp_partition_subtype_t value for the i-th OTA partition. 

```



### Type Definitions


#### esp_partition_mmap_handle_t


```


typedef uint32_t esp_partition_mmap_handle_t
Opaque handle for memory region obtained from esp_partition_mmap. 

```

#### esp_partition_iterator_t


```


typedef struct esp_partition_iterator_opaque_ *esp_partition_iterator_t
Opaque partition iterator type. 

```



### Enumerations


#### esp_partition_mmap_memory_t


```


enum esp_partition_mmap_memory_t
Enumeration which specifies memory space requested in an mmap call. 
Values:
ESP_PARTITION_MMAP_DATA

enumerator ESP_PARTITION_MMAP_DATA
map to data memory (Vaddr0), allows byte-aligned access, 4 MB total 

ESP_PARTITION_MMAP_INST

enumerator ESP_PARTITION_MMAP_INST
map to instruction memory (Vaddr1-3), allows only 4-byte-aligned access, 11 MB total 


```

#### esp_partition_type_t


```


enum esp_partition_type_t
Partition type. 

Note
Partition types with integer value 0x00-0x3F are reserved for partition types defined by ESP-IDF. Any other integer value 0x40-0xFE can be used by individual applications, without restriction. 

Values:
ESP_PARTITION_TYPE_APP

enumerator ESP_PARTITION_TYPE_APP
Application partition type. 

ESP_PARTITION_TYPE_DATA

enumerator ESP_PARTITION_TYPE_DATA
Data partition type. 

ESP_PARTITION_TYPE_ANY

enumerator ESP_PARTITION_TYPE_ANY
Used to search for partitions with any type. 


```

#### esp_partition_subtype_t


```


enum esp_partition_subtype_t
Partition subtype. 

Application-defined partition types (0x40-0xFE) can set any numeric subtype value. 

Note
These ESP-IDF-defined partition subtypes apply to partitions of type ESP_PARTITION_TYPE_APP and ESP_PARTITION_TYPE_DATA.

Values:
ESP_PARTITION_SUBTYPE_APP_FACTORY

enumerator ESP_PARTITION_SUBTYPE_APP_FACTORY
Factory application partition. 

ESP_PARTITION_SUBTYPE_APP_OTA_MIN

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_MIN
Base for OTA partition subtypes. 

ESP_PARTITION_SUBTYPE_APP_OTA_0

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_0
OTA partition 0. 

ESP_PARTITION_SUBTYPE_APP_OTA_1

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_1
OTA partition 1. 

ESP_PARTITION_SUBTYPE_APP_OTA_2

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_2
OTA partition 2. 

ESP_PARTITION_SUBTYPE_APP_OTA_3

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_3
OTA partition 3. 

ESP_PARTITION_SUBTYPE_APP_OTA_4

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_4
OTA partition 4. 

ESP_PARTITION_SUBTYPE_APP_OTA_5

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_5
OTA partition 5. 

ESP_PARTITION_SUBTYPE_APP_OTA_6

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_6
OTA partition 6. 

ESP_PARTITION_SUBTYPE_APP_OTA_7

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_7
OTA partition 7. 

ESP_PARTITION_SUBTYPE_APP_OTA_8

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_8
OTA partition 8. 

ESP_PARTITION_SUBTYPE_APP_OTA_9

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_9
OTA partition 9. 

ESP_PARTITION_SUBTYPE_APP_OTA_10

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_10
OTA partition 10. 

ESP_PARTITION_SUBTYPE_APP_OTA_11

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_11
OTA partition 11. 

ESP_PARTITION_SUBTYPE_APP_OTA_12

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_12
OTA partition 12. 

ESP_PARTITION_SUBTYPE_APP_OTA_13

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_13
OTA partition 13. 

ESP_PARTITION_SUBTYPE_APP_OTA_14

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_14
OTA partition 14. 

ESP_PARTITION_SUBTYPE_APP_OTA_15

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_15
OTA partition 15. 

ESP_PARTITION_SUBTYPE_APP_OTA_MAX

enumerator ESP_PARTITION_SUBTYPE_APP_OTA_MAX
Max subtype of OTA partition. 

ESP_PARTITION_SUBTYPE_APP_TEST

enumerator ESP_PARTITION_SUBTYPE_APP_TEST
Test application partition. 

ESP_PARTITION_SUBTYPE_DATA_OTA

enumerator ESP_PARTITION_SUBTYPE_DATA_OTA
OTA selection partition. 

ESP_PARTITION_SUBTYPE_DATA_PHY

enumerator ESP_PARTITION_SUBTYPE_DATA_PHY
PHY init data partition. 

ESP_PARTITION_SUBTYPE_DATA_NVS

enumerator ESP_PARTITION_SUBTYPE_DATA_NVS
NVS partition. 

ESP_PARTITION_SUBTYPE_DATA_COREDUMP

enumerator ESP_PARTITION_SUBTYPE_DATA_COREDUMP
COREDUMP partition. 

ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS

enumerator ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS
Partition for NVS keys. 

ESP_PARTITION_SUBTYPE_DATA_EFUSE_EM

enumerator ESP_PARTITION_SUBTYPE_DATA_EFUSE_EM
Partition for emulate eFuse bits. 

ESP_PARTITION_SUBTYPE_DATA_UNDEFINED

enumerator ESP_PARTITION_SUBTYPE_DATA_UNDEFINED
Undefined (or unspecified) data partition. 

ESP_PARTITION_SUBTYPE_DATA_ESPHTTPD

enumerator ESP_PARTITION_SUBTYPE_DATA_ESPHTTPD
ESPHTTPD partition. 

ESP_PARTITION_SUBTYPE_DATA_FAT

enumerator ESP_PARTITION_SUBTYPE_DATA_FAT
FAT partition. 

ESP_PARTITION_SUBTYPE_DATA_SPIFFS

enumerator ESP_PARTITION_SUBTYPE_DATA_SPIFFS
SPIFFS partition. 

ESP_PARTITION_SUBTYPE_DATA_LITTLEFS

enumerator ESP_PARTITION_SUBTYPE_DATA_LITTLEFS
LITTLEFS partition. 

ESP_PARTITION_SUBTYPE_ANY

enumerator ESP_PARTITION_SUBTYPE_ANY
Used to search for partitions with any subtype. 


```