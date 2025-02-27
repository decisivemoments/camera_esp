# Non-Volatile Storage Library




## Introduction


Non-volatile storage (NVS) library is designed to store key-value pairs in flash. This section introduces some concepts used by NVS.



### Underlying Storage


Currently, NVS uses a portion of main flash memory through the esp_partition API. The library uses all the partitions with `data` type and `nvs` subtype. The application can choose to use the partition with the label `nvs` through the `nvs_open()` API function or any other partition by specifying its name using the `nvs_open_from_partition()` API function.


Future versions of this library may have other storage backends to keep data in another flash chip (SPI or I2C), RTC, FRAM, etc.



Note


if an NVS partition is truncated (for example, when the partition table layout is changed), its contents should be erased. ESP-IDF build system provides a `idf.py erase-flash` target to erase all contents of the flash chip.




Note


NVS works best for storing many small values, rather than a few large values of the type 'string' and 'blob'. If you need to store large blobs or strings, consider using the facilities provided by the FAT filesystem on top of the wear levelling library.





### Keys and Values


NVS operates on key-value pairs. Keys are ASCII strings; the maximum key length is currently 15 characters. Values can have one of the following types:


* integer types: `uint8_t`, `int8_t`, `uint16_t`, `int16_t`, `uint32_t`, `int32_t`, `uint64_t`, `int64_t`
* zero-terminated string
* variable length binary data (blob)



Note


String values are currently limited to 4000 bytes. This includes the null terminator. Blob values are limited to 508,000 bytes or 97.6% of the partition size - 4000 bytes, whichever is lower.



Additional types, such as `float` and `double` might be added later.


Keys are required to be unique. Assigning a new value to an existing key replaces the old value and data type with the value and data type specified by a write operation.


A data type check is performed when reading a value. An error is returned if the data type expected by read operation does not match the data type of entry found for the key provided.




### Namespaces


To mitigate potential conflicts in key names between different components, NVS assigns each key-value pair to one of namespaces. Namespace names follow the same rules as key names, i.e., the maximum length is 15 characters. Furthermore, there can be no more than 254 different namespaces in one NVS partition. Namespace name is specified in the `nvs_open()` or `nvs_open_from_partition` call. This call returns an opaque handle, which is used in subsequent calls to the `nvs_get_*`, `nvs_set_*`, and `nvs_commit()` functions. This way, a handle is associated with a namespace, and key names will not collide with same names in other namespaces. Please note that the namespaces with the same name in different NVS partitions are considered as separate namespaces.




### NVS Iterators


Iterators allow to list key-value pairs stored in NVS, based on specified partition name, namespace, and data type.


There are the following functions available:


* `nvs_entry_find()` creates an opaque handle, which is used in subsequent calls to the `nvs_entry_next()` and `nvs_entry_info()` functions.
* `nvs_entry_next()` advances an iterator to the next key-value pair.
* `nvs_entry_info()` returns information about each key-value pair


In general, all iterators obtained via `nvs_entry_find()` have to be released using `nvs_release_iterator()`, which also tolerates `NULL` iterators.


`nvs_entry_find()` and `nvs_entry_next()` set the given iterator to `NULL` or a valid iterator in all cases except a parameter error occurred (i.e., return `ESP_ERR_NVS_NOT_FOUND`). In case of a parameter error, the given iterator will not be modified. Hence, it is best practice to initialize the iterator to `NULL` before calling `nvs_entry_find()` to avoid complicated error checking before releasing the iterator.




### Security, Tampering, and Robustness


NVS is not directly compatible with the ESP32-S3 flash encryption system. However, data can still be stored in encrypted form if NVS encryption is used together with ESP32-S3 flash encryption or with the help of the HMAC peripheral. Please refer to NVS Encryption for more details.


If NVS encryption is not used, it is possible for anyone with physical access to the flash chip to alter, erase, or add key-value pairs. With NVS encryption enabled, it is not possible to alter or add a key-value pair and get recognized as a valid pair without knowing corresponding NVS encryption keys. However, there is no tamper-resistance against the erase operation.


The library does try to recover from conditions when flash memory is in an inconsistent state. In particular, one should be able to power off the device at any point and time and then power it back on. This should not result in loss of data, except for the new key-value pair if it was being written at the moment of powering off. The library should also be able to initialize properly with any random data present in flash memory.





## NVS Encryption


Please refer to the NVS Encryption guide for more details.




## NVS Partition Generator Utility


This utility helps generate NVS partition binary files which can be flashed separately on a dedicated partition via a flashing utility. Key-value pairs to be flashed onto the partition can be provided via a CSV file. For more details, please refer to NVS Partition Generator Utility.


Instead of calling the `nvs_partition_gen.py` tool manually, the creation of the partition binary files can also be done directly from CMake using the function `nvs_create_partition_image`:



```
nvs_create_partition_image(<partition> <csv> [FLASH_IN_PROJECT] [DEPENDS  dep dep dep ...])

```


**Positional Arguments**:







| Parameter | Description |
| --- | --- |
| `partition` | Name of the NVS partition |
| `csv` | Path to CSV file to parse |


**Optional Arguments**:







| Parameter | Description |
| --- | --- |
| `FLASH_IN_PROJECT` | Name of the NVS partition |
| `DEPENDS` | Specify files on which the command depends |


If `FLASH_IN_PROJECT` is not specified, the image will still be generated, but you will have to flash it manually using `idf.py <partition>-flash` (e.g., if your partition name is `nvs`, then use `idf.py nvs-flash`).


`nvs_create_partition_image` must be called from one of the component `CMakeLists.txt` files. Currently, only non-encrypted partitions are supported.




## Application Example


You can find code examples in the storage directory of ESP-IDF examples:


storage/nvs_rw_value



> Demonstrates how to read a single integer value from, and write it to NVS.
> 
> 
> The value checked in this example holds the number of the ESP32-S3 module restarts. The value's function as a counter is only possible due to its storing in NVS.
> 
> 
> The example also shows how to check if a read/write operation was successful, or if a certain value has not been initialized in NVS. The diagnostic procedure is provided in plain text to help you track the program flow and capture any issues on the way.


storage/nvs_rw_blob



> Demonstrates how to read a single integer value and a blob (binary large object), and write them to NVS to preserve this value between ESP32-S3 module restarts.
> 
> 
> 
> > * value - tracks the number of the ESP32-S3 module soft and hard restarts.
> > * blob - contains a table with module run times. The table is read from NVS to dynamically allocated RAM. A new run time is added to the table on each manually triggered soft restart, and then the added run time is written to NVS. Triggering is done by pulling down GPIO0.
> 
> 
> The example also shows how to implement the diagnostic procedure to check if the read/write operation was successful.


storage/nvs_rw_value_cxx



> This example does exactly the same as storage/nvs_rw_value, except that it uses the C++ NVS handle class.




## Internals



### Log of Key-Value Pairs


NVS stores key-value pairs sequentially, with new key-value pairs being added at the end. When a value of any given key has to be updated, a new key-value pair is added at the end of the log and the old key-value pair is marked as erased.




### Pages and Entries


NVS library uses two main entities in its operation: pages and entries. Page is a logical structure which stores a portion of the overall log. Logical page corresponds to one physical sector of flash memory. Pages which are in use have a *sequence number* associated with them. Sequence numbers impose an ordering on pages. Higher sequence numbers correspond to pages which were created later. Each page can be in one of the following states:



Empty/uninitializedFlash storage for the page is empty (all bytes are `0xff`). Page is not used to store any data at this point and does not have a sequence number.



ActiveFlash storage is initialized, page header has been written to flash, page has a valid sequence number. Page has some empty entries and data can be written there. No more than one page can be in this state at any given moment.



FullFlash storage is in a consistent state and is filled with key-value pairs.
Writing new key-value pairs into this page is not possible. It is still possible to mark some key-value pairs as erased.



ErasingNon-erased key-value pairs are being moved into another page so that the current page can be erased. This is a transient state, i.e., page should never stay in this state at the time when any API call returns. In case of a sudden power off, the move-and-erase process will be completed upon the next power-on.



CorruptedPage header contains invalid data, and further parsing of page data was canceled. Any items previously written into this page will not be accessible. The corresponding flash sector will not be erased immediately and will be kept along with sectors in **uninitialized** state for later use. This may be useful for debugging.




Mapping from flash sectors to logical pages does not have any particular order. The library will inspect sequence numbers of pages found in each flash sector and organize pages in a list based on these numbers.



```
+--------+     +--------+     +--------+     +--------+
| Page 1 |     | Page 2 |     | Page 3 |     | Page 4 |
| Full   +---> | Full   +---> | Active |     | Empty  |   <- states
| #11    |     | #12    |     | #14    |     |        |   <- sequence numbers
+---+----+     +----+---+     +----+---+     +---+----+
    |               |              |             |
    |               |              |             |
    |               |              |             |
+---v------+  +-----v----+  +------v---+  +------v---+
| Sector 3 |  | Sector 0 |  | Sector 2 |  | Sector 1 |    <- physical sectors
+----------+  +----------+  +----------+  +----------+

```




### Structure of a Page


For now, we assume that flash sector size is 4096 bytes and that ESP32-S3 flash encryption hardware operates on 32-byte blocks. It is possible to introduce some settings configurable at compile-time (e.g., via menuconfig) to accommodate flash chips with different sector sizes (although it is not clear if other components in the system, e.g., SPI flash driver and SPI flash cache can support these other sizes).


Page consists of three parts: header, entry state bitmap, and entries themselves. To be compatible with ESP32-S3 flash encryption, the entry size is 32 bytes. For integer types, an entry holds one key-value pair. For strings and blobs, an entry holds part of key-value pair (more on that in the entry structure description).


The following diagram illustrates the page structure. Numbers in parentheses indicate the size of each part in bytes.



```
+-----------+--------------+-------------+-------------------------+
| State (4) | Seq. no. (4) | version (1) | Unused (19) | CRC32 (4) |   Header (32)
+-----------+--------------+-------------+-------------------------+
|                Entry state bitmap (32)                           |
+------------------------------------------------------------------+
|                       Entry 0 (32)                               |
+------------------------------------------------------------------+
|                       Entry 1 (32)                               |
+------------------------------------------------------------------+
/                                                                  /
/                                                                  /
+------------------------------------------------------------------+
|                       Entry 125 (32)                             |
+------------------------------------------------------------------+

```


Page header and entry state bitmap are always written to flash unencrypted. Entries are encrypted if flash encryption feature of ESP32-S3 is used.


Page state values are defined in such a way that changing state is possible by writing 0 into some of the bits. Therefore it is not necessary to erase the page to change its state unless that is a change to the *erased* state.


The version field in the header reflects the NVS format version used. For backward compatibility reasons, it is decremented for every version upgrade starting at 0xff (i.e., 0xff for version-1, 0xfe for version-2 and so on).


CRC32 value in the header is calculated over the part which does not include a state value (bytes 4 to 28). The unused part is currently filled with `0xff` bytes.


The following sections describe the structure of entry state bitmap and entry itself.




### Entry and Entry State Bitmap


Each entry can be in one of the following three states represented with two bits in the entry state bitmap. The final four bits in the bitmap (256 - 2 \* 126) are not used.



Empty (2'b11)Nothing is written into the specific entry yet. It is in an uninitialized state (all bytes are `0xff`).



Written (2'b10)A key-value pair (or part of key-value pair which spans multiple entries) has been written into the entry.



Erased (2'b00)A key-value pair in this entry has been discarded. Contents of this entry will not be parsed anymore.






### Structure of Entry


For values of primitive types (currently integers from 1 to 8 bytes long), entry holds one key-value pair. For string and blob types, entry holds part of the whole key-value pair. For strings, in case when a key-value pair spans multiple entries, all entries are stored in the same page. Blobs are allowed to span over multiple pages by dividing them into smaller chunks. For tracking these chunks, an additional fixed length metadata entry is stored called "blob index". Earlier formats of blobs are still supported (can be read and modified). However, once the blobs are modified, they are stored using the new format.



```
+--------+----------+----------+----------------+-----------+---------------+----------+
| NS (1) | Type (1) | Span (1) | ChunkIndex (1) | CRC32 (4) |    Key (16)   | Data (8) |
+--------+----------+----------+----------------+-----------+---------------+----------+

                                         Primitive  +--------------------------------+
                                        +-------->  |     Data (8)                   |
                                        | Types     +--------------------------------+
                   +-> Fixed length --
                   |                    |           +---------+--------------+---------------+-------+
                   |                    +-------->  | Size(4) | ChunkCount(1)| ChunkStart(1) | Rsv(2)|
    Data format ---+                    Blob Index  +---------+--------------+---------------+-------+
                   |
                   |                             +----------+---------+-----------+
                   +->   Variable length   -->   | Size (2) | Rsv (2) | CRC32 (4) |
                        (Strings, Blob Data)     +----------+---------+-----------+

```


Individual fields in entry structure have the following meanings:



NSNamespace index for this entry. For more information on this value, see the section on namespaces implementation.



TypeOne byte indicating the value data type. See the `ItemType` enumeration in nvs_flash/include/nvs_handle.hpp for possible values.



SpanNumber of entries used by this key-value pair. For integer types, this is equal to 1. For strings and blobs, this depends on value length.



ChunkIndexUsed to store the index of a blob-data chunk for blob types. For other types, this should be `0xff`.



CRC32Checksum calculated over all the bytes in this entry, except for the CRC32 field itself.



KeyZero-terminated ASCII string containing a key name. Maximum string length is 15 bytes, excluding a zero terminator.



DataFor integer types, this field contains the value itself. If the value itself is shorter than 8 bytes, it is padded to the right, with unused bytes filled with `0xff`.


For "blob index" entry, these 8 bytes hold the following information about data-chunks:


* Size(Only for blob index.) Size, in bytes, of complete blob data.
* ChunkCount(Only for blob index.) Total number of blob-data chunks into which the blob was divided during storage.
* ChunkStart(Only for blob index.) ChunkIndex of the first blob-data chunk of this blob. Subsequent chunks have chunkIndex incrementally allocated (step of 1).


For string and blob data chunks, these 8 bytes hold additional data about the value, which are described below:


* Size(Only for strings and blobs.) Size, in bytes, of actual data. For strings, this includes zero terminators.
* CRC32(Only for strings and blobs.) Checksum calculated over all bytes of data.




Variable length values (strings and blobs) are written into subsequent entries, 32 bytes per entry. The `Span` field of the first entry indicates how many entries are used.




### Namespaces


As mentioned above, each key-value pair belongs to one of the namespaces. Namespace identifiers (strings) are stored as keys of key-value pairs in namespace with index 0. Values corresponding to these keys are indexes of these namespaces.



```
+-------------------------------------------+
| NS=0 Type=uint8_t Key="wifi" Value=1      |   Entry describing namespace "wifi"
+-------------------------------------------+
| NS=1 Type=uint32_t Key="channel" Value=6  |   Key "channel" in namespace "wifi"
+-------------------------------------------+
| NS=0 Type=uint8_t Key="pwm" Value=2       |   Entry describing namespace "pwm"
+-------------------------------------------+
| NS=2 Type=uint16_t Key="channel" Value=20 |   Key "channel" in namespace "pwm"
+-------------------------------------------+

```




### Item Hash List


To reduce the number of reads from flash memory, each member of the Page class maintains a list of pairs: item index; item hash. This list makes searches much quicker. Instead of iterating over all entries, reading them from flash one at a time, Page::findItem first performs a search for the item hash in the hash list. This gives the item index within the page if such an item exists. Due to a hash collision, it is possible that a different item is found. This is handled by falling back to iteration over items in flash.


Each node in the hash list contains a 24-bit hash and 8-bit item index. Hash is calculated based on item namespace, key name, and ChunkIndex. CRC32 is used for calculation; the result is truncated to 24 bits. To reduce the overhead for storing 32-bit entries in a linked list, the list is implemented as a double-linked list of arrays. Each array holds 29 entries, for the total size of 128 bytes, together with linked list pointers and a 32-bit count field. The minimum amount of extra RAM usage per page is therefore 128 bytes; maximum is 640 bytes.





## API Reference



### Header File


* components/nvs_flash/include/nvs_flash.h
* This header file can be included with:



> ```
> #include "nvs_flash.h"
> 
> ```
* This header file is a part of the API provided by the `nvs_flash` component. To declare that your component depends on `nvs_flash`, add the following to your CMakeLists.txt:



> ```
> REQUIRES nvs_flash
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES nvs_flash
> 
> ```




### Functions


#### nvs_flash_init


```


esp_err_t nvs_flash_init(void)
Initialize the default NVS partition. 
This API initialises the default NVS partition. The default NVS partition is the one that is labeled "nvs" in the partition table.
When "NVS_ENCRYPTION" is enabled in the menuconfig, this API enables the NVS encryption for the default NVS partition as follows
Read security configurations from the first NVS key partition listed in the partition table. (NVS key partition is any "data" type partition which has the subtype value set to "nvs_keys")
If the NVS key partition obtained in the previous step is empty, generate and store new keys in that NVS key partition.
Internally call "nvs_flash_secure_init()" with the security configurations obtained/generated in the previous steps.


Post initialization NVS read/write APIs remain the same irrespective of NVS encryption.

Returns

ESP_OK if storage was successfully initialized.
ESP_ERR_NVS_NO_FREE_PAGES if the NVS storage contains no empty pages (which may happen if NVS partition was truncated)
ESP_ERR_NOT_FOUND if no partition with label "nvs" is found in the partition table
ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
one of the error codes from the underlying flash storage driver
error codes from nvs_flash_read_security_cfg API (when "NVS_ENCRYPTION" is enabled).
error codes from nvs_flash_generate_keys API (when "NVS_ENCRYPTION" is enabled).
error codes from nvs_flash_secure_init_partition API (when "NVS_ENCRYPTION" is enabled) . 





```

#### nvs_flash_init_partition


```


esp_err_t nvs_flash_init_partition(const char *partition_label)
Initialize NVS flash storage for the specified partition. 

Parameters
partition_label -- [in] Label of the partition. Must be no longer than 16 characters.

Returns

ESP_OK if storage was successfully initialized.
ESP_ERR_NVS_NO_FREE_PAGES if the NVS storage contains no empty pages (which may happen if NVS partition was truncated)
ESP_ERR_NOT_FOUND if specified partition is not found in the partition table
ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
one of the error codes from the underlying flash storage driver 





```

#### nvs_flash_init_partition_ptr


```


esp_err_t nvs_flash_init_partition_ptr(const esp_partition_t *partition)
Initialize NVS flash storage for the partition specified by partition pointer. 

Parameters
partition -- [in] pointer to a partition obtained by the ESP partition API.

Returns

ESP_OK if storage was successfully initialized
ESP_ERR_NVS_NO_FREE_PAGES if the NVS storage contains no empty pages (which may happen if NVS partition was truncated)
ESP_ERR_INVALID_ARG in case partition is NULL
ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
one of the error codes from the underlying flash storage driver 





```

#### nvs_flash_deinit


```


esp_err_t nvs_flash_deinit(void)
Deinitialize NVS storage for the default NVS partition. 
Default NVS partition is the partition with "nvs" label in the partition table.

Returns

ESP_OK on success (storage was deinitialized)
ESP_ERR_NVS_NOT_INITIALIZED if the storage was not initialized prior to this call 





```

#### nvs_flash_deinit_partition


```


esp_err_t nvs_flash_deinit_partition(const char *partition_label)
Deinitialize NVS storage for the given NVS partition. 

Parameters
partition_label -- [in] Label of the partition

Returns

ESP_OK on success
ESP_ERR_NVS_NOT_INITIALIZED if the storage for given partition was not initialized prior to this call 





```

#### nvs_flash_erase


```


esp_err_t nvs_flash_erase(void)
Erase the default NVS partition. 
Erases all contents of the default NVS partition (one with label "nvs").

Note
If the partition is initialized, this function first de-initializes it. Afterwards, the partition has to be initialized again to be used.


Returns

ESP_OK on success
ESP_ERR_NOT_FOUND if there is no NVS partition labeled "nvs" in the partition table
different error in case de-initialization fails (shouldn't happen) 





```

#### nvs_flash_erase_partition


```


esp_err_t nvs_flash_erase_partition(const char *part_name)
Erase specified NVS partition. 
Erase all content of a specified NVS partition

Note
If the partition is initialized, this function first de-initializes it. Afterwards, the partition has to be initialized again to be used.


Parameters
part_name -- [in] Name (label) of the partition which should be erased

Returns

ESP_OK on success
ESP_ERR_NOT_FOUND if there is no NVS partition with the specified name in the partition table
different error in case de-initialization fails (shouldn't happen) 





```

#### nvs_flash_erase_partition_ptr


```


esp_err_t nvs_flash_erase_partition_ptr(const esp_partition_t *partition)
Erase custom partition. 
Erase all content of specified custom partition.

Note
If the partition is initialized, this function first de-initializes it. Afterwards, the partition has to be initialized again to be used.


Parameters
partition -- [in] pointer to a partition obtained by the ESP partition API.

Returns

ESP_OK on success
ESP_ERR_NOT_FOUND if there is no partition with the specified parameters in the partition table
ESP_ERR_INVALID_ARG in case partition is NULL
one of the error codes from the underlying flash storage driver 





```

#### nvs_flash_secure_init


```


esp_err_t nvs_flash_secure_init(nvs_sec_cfg_t *cfg)
Initialize the default NVS partition. 
This API initialises the default NVS partition. The default NVS partition is the one that is labeled "nvs" in the partition table.

Parameters
cfg -- [in] Security configuration (keys) to be used for NVS encryption/decryption. If cfg is NULL, no encryption is used.

Returns

ESP_OK if storage has been initialized successfully.
ESP_ERR_NVS_NO_FREE_PAGES if the NVS storage contains no empty pages (which may happen if NVS partition was truncated)
ESP_ERR_NOT_FOUND if no partition with label "nvs" is found in the partition table
ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
one of the error codes from the underlying flash storage driver 





```

#### nvs_flash_secure_init_partition


```


esp_err_t nvs_flash_secure_init_partition(const char *partition_label, nvs_sec_cfg_t *cfg)
Initialize NVS flash storage for the specified partition. 

Parameters

partition_label -- [in] Label of the partition. Note that internally, a reference to passed value is kept and it should be accessible for future operations
cfg -- [in] Security configuration (keys) to be used for NVS encryption/decryption. If cfg is null, no encryption/decryption is used. 


Returns

ESP_OK if storage has been initialized successfully.
ESP_ERR_NVS_NO_FREE_PAGES if the NVS storage contains no empty pages (which may happen if NVS partition was truncated)
ESP_ERR_NOT_FOUND if specified partition is not found in the partition table
ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
one of the error codes from the underlying flash storage driver 





```

#### nvs_flash_generate_keys


```


esp_err_t nvs_flash_generate_keys(const esp_partition_t *partition, nvs_sec_cfg_t *cfg)
Generate and store NVS keys in the provided esp partition. 

Parameters

partition -- [in] Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
cfg -- [out] Pointer to nvs security configuration structure. Pointer must be non-NULL. Generated keys will be populated in this structure.


Returns

ESP_OK, if cfg was read successfully;
ESP_ERR_INVALID_ARG, if partition or cfg is NULL;
or error codes from esp_partition_write/erase APIs. 





```

#### nvs_flash_read_security_cfg


```


esp_err_t nvs_flash_read_security_cfg(const esp_partition_t *partition, nvs_sec_cfg_t *cfg)
Read NVS security configuration from a partition. 

Note
Provided partition is assumed to be marked 'encrypted'.


Parameters

partition -- [in] Pointer to partition structure obtained using esp_partition_find_first or esp_partition_get. Must be non-NULL. 
cfg -- [out] Pointer to nvs security configuration structure. Pointer must be non-NULL.


Returns

ESP_OK, if cfg was read successfully;
ESP_ERR_INVALID_ARG, if partition or cfg is NULL
ESP_ERR_NVS_KEYS_NOT_INITIALIZED, if the partition is not yet written with keys.
ESP_ERR_NVS_CORRUPT_KEY_PART, if the partition containing keys is found to be corrupt
or error codes from esp_partition_read API. 





```

#### nvs_flash_register_security_scheme


```


esp_err_t nvs_flash_register_security_scheme(nvs_sec_scheme_t *scheme_cfg)
Registers the given security scheme for NVS encryption The scheme registered with sec_scheme_id by this API be used as the default security scheme for the "nvs" partition. Users will have to call this API explicitly in their application. 

Parameters
scheme_cfg -- [in] Pointer to the security scheme configuration structure that the user (or the nvs_key_provider) wants to register.

Returns

ESP_OK, if security scheme registration succeeds;
ESP_ERR_INVALID_ARG, if scheme_cfg is NULL;
ESP_FAIL, if security scheme registration fails 





```

#### nvs_flash_get_default_security_scheme


```


nvs_sec_scheme_t *nvs_flash_get_default_security_scheme(void)
Fetch the configuration structure for the default active security scheme for NVS encryption. 

Returns
Pointer to the default active security scheme configuration (NULL if no scheme is registered yet i.e. active) 



```

#### nvs_flash_generate_keys_v2


```


esp_err_t nvs_flash_generate_keys_v2(nvs_sec_scheme_t *scheme_cfg, nvs_sec_cfg_t *cfg)
Generate (and store) the NVS keys using the specified key-protection scheme. 

Parameters

scheme_cfg -- [in] Security scheme specific configuration
cfg -- [out] Security configuration (encryption keys)


Returns

ESP_OK, if cfg was populated successfully with generated encryption keys;
ESP_ERR_INVALID_ARG, if scheme_cfg or cfg is NULL;
ESP_FAIL, if the key generation process fails 





```

#### nvs_flash_read_security_cfg_v2


```


esp_err_t nvs_flash_read_security_cfg_v2(nvs_sec_scheme_t *scheme_cfg, nvs_sec_cfg_t *cfg)
Read NVS security configuration set by the specified security scheme. 

Parameters

scheme_cfg -- [in] Security scheme specific configuration
cfg -- [out] Security configuration (encryption keys)


Returns

ESP_OK, if cfg was read successfully;
ESP_ERR_INVALID_ARG, if scheme_cfg or cfg is NULL;
ESP_FAIL, if the key reading process fails 





```



### Structures


#### nvs_sec_cfg_t


```


struct nvs_sec_cfg_t
Key for encryption and decryption. 

Public Members
eky

uint8_t eky[NVS_KEY_SIZE]
XTS encryption and decryption key 

tky

uint8_t tky[NVS_KEY_SIZE]
XTS tweak key 



```

#### nvs_sec_scheme_t


```


struct nvs_sec_scheme_t
NVS encryption: Security scheme configuration structure. 

Public Members
scheme_id

int scheme_id
Security Scheme ID (E.g. HMAC) 

scheme_data

void *scheme_data
Scheme-specific data (E.g. eFuse block for HMAC-based key generation) 

nvs_flash_key_gen

nvs_flash_generate_keys_t nvs_flash_key_gen
Callback for the nvs_flash_key_gen implementation 

nvs_flash_read_cfg

nvs_flash_read_cfg_t nvs_flash_read_cfg
Callback for the nvs_flash_read_keys implementation 



```



### Macros


#### NVS_KEY_SIZE


```


NVS_KEY_SIZE

```



### Type Definitions


#### nvs_flash_generate_keys_t


```


typedef esp_err_t (*nvs_flash_generate_keys_t)(const void *scheme_data, nvs_sec_cfg_t *cfg)
Callback function prototype for generating the NVS encryption keys. 

```

#### nvs_flash_read_cfg_t


```


typedef esp_err_t (*nvs_flash_read_cfg_t)(const void *scheme_data, nvs_sec_cfg_t *cfg)
Callback function prototype for reading the NVS encryption keys. 

```



### Header File


* components/nvs_flash/include/nvs.h
* This header file can be included with:



> ```
> #include "nvs.h"
> 
> ```
* This header file is a part of the API provided by the `nvs_flash` component. To declare that your component depends on `nvs_flash`, add the following to your CMakeLists.txt:



> ```
> REQUIRES nvs_flash
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES nvs_flash
> 
> ```




### Functions


#### nvs_set_i8


```


esp_err_t nvs_set_i8(nvs_handle_t handle, const char *key, int8_t value)
set int8_t value for given key 
Set value for the key, given its name. Note that the actual storage will not be updated until nvs_commit is called. Regardless whether key-value pair is created or updated, function always requires at least one nvs available entry. See nvs_get_stats . After create type of operation, the number of available entries is decreased by one. After update type of operation, the number of available entries remains the same.

Parameters

handle -- [in] Handle obtained from nvs_open function. Handles that were opened read only cannot be used. 
key -- [in] Key name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
value -- [in] The value to set.


Returns

ESP_OK if value was set successfully
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_ERR_NVS_READ_ONLY if storage handle was opened as read only
ESP_ERR_NVS_INVALID_NAME if key name doesn't satisfy constraints
ESP_ERR_NVS_NOT_ENOUGH_SPACE if there is not enough space in the underlying storage to save the value
ESP_ERR_NVS_REMOVE_FAILED if the value wasn't updated because flash write operation has failed. The value was written however, and update will be finished after re-initialization of nvs, provided that flash operation doesn't fail again. 





```

#### nvs_set_u8


```


esp_err_t nvs_set_u8(nvs_handle_t handle, const char *key, uint8_t value)
set uint8_t value for given key 
This function is the same as nvs_set_i8 except for the data type. 

```

#### nvs_set_i16


```


esp_err_t nvs_set_i16(nvs_handle_t handle, const char *key, int16_t value)
set int16_t value for given key 
This function is the same as nvs_set_i8 except for the data type. 

```

#### nvs_set_u16


```


esp_err_t nvs_set_u16(nvs_handle_t handle, const char *key, uint16_t value)
set uint16_t value for given key 
This function is the same as nvs_set_i8 except for the data type. 

```

#### nvs_set_i32


```


esp_err_t nvs_set_i32(nvs_handle_t handle, const char *key, int32_t value)
set int32_t value for given key 
This function is the same as nvs_set_i8 except for the data type. 

```

#### nvs_set_u32


```


esp_err_t nvs_set_u32(nvs_handle_t handle, const char *key, uint32_t value)
set uint32_t value for given key 
This function is the same as nvs_set_i8 except for the data type. 

```

#### nvs_set_i64


```


esp_err_t nvs_set_i64(nvs_handle_t handle, const char *key, int64_t value)
set int64_t value for given key 
This function is the same as nvs_set_i8 except for the data type. 

```

#### nvs_set_u64


```


esp_err_t nvs_set_u64(nvs_handle_t handle, const char *key, uint64_t value)
set uint64_t value for given key 
This function is the same as nvs_set_i8 except for the data type. 

```

#### nvs_set_str


```


esp_err_t nvs_set_str(nvs_handle_t handle, const char *key, const char *value)
set string for given key 
Sets string value for the key. Function requires whole space for new data to be available as contiguous entries in same nvs page. Operation consumes 1 overhead entry and 1 entry per each 32 characters of new string including zero character to be set. In case of value update for existing key, entries occupied by the previous value and overhead entry are returned to the pool of available entries. Note that storage of long string values can fail due to fragmentation of nvs pages even if available_entries returned by nvs_get_stats suggests enough overall space available. Note that the underlying storage will not be updated until nvs_commit is called.

Parameters

handle -- [in] Handle obtained from nvs_open function. Handles that were opened read only cannot be used. 
key -- [in] Key name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
value -- [in] The value to set. For strings, the maximum length (including null character) is 4000 bytes, if there is one complete page free for writing. This decreases, however, if the free space is fragmented.


Returns

ESP_OK if value was set successfully
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_ERR_NVS_READ_ONLY if storage handle was opened as read only
ESP_ERR_NVS_INVALID_NAME if key name doesn't satisfy constraints
ESP_ERR_NVS_NOT_ENOUGH_SPACE if there is not enough space in the underlying storage to save the value
ESP_ERR_NVS_REMOVE_FAILED if the value wasn't updated because flash write operation has failed. The value was written however, and update will be finished after re-initialization of nvs, provided that flash operation doesn't fail again.
ESP_ERR_NVS_VALUE_TOO_LONG if the string value is too long 





```

#### nvs_get_i8


```


esp_err_t nvs_get_i8(nvs_handle_t handle, const char *key, int8_t *out_value)
get int8_t value for given key 
These functions retrieve value for the key, given its name. If key does not exist, or the requested variable type doesn't match the type which was used when setting a value, an error is returned.
In case of any error, out_value is not modified.
out_value has to be a pointer to an already allocated variable of the given type.
// Example of using nvs_get_i32:
int32_t max_buffer_size = 4096; // default value
esp_err_t err = nvs_get_i32(my_handle, "max_buffer_size", &max_buffer_size);
assert(err == ESP_OK || err == ESP_ERR_NVS_NOT_FOUND);
// if ESP_ERR_NVS_NOT_FOUND was returned, max_buffer_size will still
// have its default value.



Parameters

handle -- [in] Handle obtained from nvs_open function. 
key -- [in] Key name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
out_value -- Pointer to the output value. May be NULL for nvs_get_str and nvs_get_blob, in this case required length will be returned in length argument.


Returns

ESP_OK if the value was retrieved successfully
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_NOT_FOUND if the requested key doesn't exist
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_ERR_NVS_INVALID_NAME if key name doesn't satisfy constraints
ESP_ERR_NVS_INVALID_LENGTH if length is not sufficient to store data 





```

#### nvs_get_u8


```


esp_err_t nvs_get_u8(nvs_handle_t handle, const char *key, uint8_t *out_value)
get uint8_t value for given key 
This function is the same as nvs_get_i8 except for the data type. 

```

#### nvs_get_i16


```


esp_err_t nvs_get_i16(nvs_handle_t handle, const char *key, int16_t *out_value)
get int16_t value for given key 
This function is the same as nvs_get_i8 except for the data type. 

```

#### nvs_get_u16


```


esp_err_t nvs_get_u16(nvs_handle_t handle, const char *key, uint16_t *out_value)
get uint16_t value for given key 
This function is the same as nvs_get_i8 except for the data type. 

```

#### nvs_get_i32


```


esp_err_t nvs_get_i32(nvs_handle_t handle, const char *key, int32_t *out_value)
get int32_t value for given key 
This function is the same as nvs_get_i8 except for the data type. 

```

#### nvs_get_u32


```


esp_err_t nvs_get_u32(nvs_handle_t handle, const char *key, uint32_t *out_value)
get uint32_t value for given key 
This function is the same as nvs_get_i8 except for the data type. 

```

#### nvs_get_i64


```


esp_err_t nvs_get_i64(nvs_handle_t handle, const char *key, int64_t *out_value)
get int64_t value for given key 
This function is the same as nvs_get_i8 except for the data type. 

```

#### nvs_get_u64


```


esp_err_t nvs_get_u64(nvs_handle_t handle, const char *key, uint64_t *out_value)
get uint64_t value for given key 
This function is the same as nvs_get_i8 except for the data type. 

```

#### nvs_get_str


```


esp_err_t nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length)
get string value for given key 
These functions retrieve the data of an entry, given its key. If key does not exist, or the requested variable type doesn't match the type which was used when setting a value, an error is returned.
In case of any error, out_value is not modified.
All functions expect out_value to be a pointer to an already allocated variable of the given type.
nvs_get_str and nvs_get_blob functions support WinAPI-style length queries. To get the size necessary to store the value, call nvs_get_str or nvs_get_blob with zero out_value and non-zero pointer to length. Variable pointed to by length argument will be set to the required length. For nvs_get_str, this length includes the zero terminator. When calling nvs_get_str and nvs_get_blob with non-zero out_value, length has to be non-zero and has to point to the length available in out_value. It is suggested that nvs_get/set_str is used for zero-terminated C strings, and nvs_get/set_blob used for arbitrary data structures.
// Example (without error checking) of using nvs_get_str to get a string into dynamic array:
size_t required_size;
nvs_get_str(my_handle, "server_name", NULL, &required_size);
char* server_name = malloc(required_size);
nvs_get_str(my_handle, "server_name", server_name, &required_size);

// Example (without error checking) of using nvs_get_blob to get a binary data
into a static array:
uint8_t mac_addr[6];
size_t size = sizeof(mac_addr);
nvs_get_blob(my_handle, "dst_mac_addr", mac_addr, &size);



Parameters

handle -- [in] Handle obtained from nvs_open function. 
key -- [in] Key name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
out_value -- [out] Pointer to the output value. May be NULL for nvs_get_str and nvs_get_blob, in this case required length will be returned in length argument. 
length -- [inout] A non-zero pointer to the variable holding the length of out_value. In case out_value a zero, will be set to the length required to hold the value. In case out_value is not zero, will be set to the actual length of the value written. For nvs_get_str this includes zero terminator.


Returns

ESP_OK if the value was retrieved successfully
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_NOT_FOUND if the requested key doesn't exist
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_ERR_NVS_INVALID_NAME if key name doesn't satisfy constraints
ESP_ERR_NVS_INVALID_LENGTH if length is not sufficient to store data 





```

#### nvs_get_blob


```


esp_err_t nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length)
get blob value for given key 
This function behaves the same as nvs_get_str, except for the data type. 

```

#### nvs_open


```


esp_err_t nvs_open(const char *namespace_name, nvs_open_mode_t open_mode, nvs_handle_t *out_handle)
Open non-volatile storage with a given namespace from the default NVS partition. 
Multiple internal ESP-IDF and third party application modules can store their key-value pairs in the NVS module. In order to reduce possible conflicts on key names, each module can use its own namespace. The default NVS partition is the one that is labelled "nvs" in the partition table.

Parameters

namespace_name -- [in] Namespace name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
open_mode -- [in] NVS_READWRITE or NVS_READONLY. If NVS_READONLY, will open a handle for reading only. All write requests will be rejected for this handle. 
out_handle -- [out] If successful (return code is zero), handle will be returned in this argument.


Returns

ESP_OK if storage handle was opened successfully
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_NOT_INITIALIZED if the storage driver is not initialized
ESP_ERR_NVS_PART_NOT_FOUND if the partition with label "nvs" is not found
ESP_ERR_NVS_NOT_FOUND id namespace doesn't exist yet and mode is NVS_READONLY
ESP_ERR_NVS_INVALID_NAME if namespace name doesn't satisfy constraints
ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
ESP_ERR_NVS_NOT_ENOUGH_SPACE if there is no space for a new entry or there are too many different namespaces (maximum allowed different namespaces: 254)
ESP_ERR_NOT_ALLOWED if the NVS partition is read-only and mode is NVS_READWRITE
ESP_ERR_INVALID_ARG if out_handle is equal to NULL
other error codes from the underlying storage driver 





```

#### nvs_open_from_partition


```


esp_err_t nvs_open_from_partition(const char *part_name, const char *namespace_name, nvs_open_mode_t open_mode, nvs_handle_t *out_handle)
Open non-volatile storage with a given namespace from specified partition. 
The behaviour is same as nvs_open() API. However this API can operate on a specified NVS partition instead of default NVS partition. Note that the specified partition must be registered with NVS using nvs_flash_init_partition() API.

Parameters

part_name -- [in] Label (name) of the partition of interest for object read/write/erase 
namespace_name -- [in] Namespace name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
open_mode -- [in] NVS_READWRITE or NVS_READONLY. If NVS_READONLY, will open a handle for reading only. All write requests will be rejected for this handle. 
out_handle -- [out] If successful (return code is zero), handle will be returned in this argument.


Returns

ESP_OK if storage handle was opened successfully
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_NOT_INITIALIZED if the storage driver is not initialized
ESP_ERR_NVS_PART_NOT_FOUND if the partition with specified name is not found
ESP_ERR_NVS_NOT_FOUND id namespace doesn't exist yet and mode is NVS_READONLY
ESP_ERR_NVS_INVALID_NAME if namespace name doesn't satisfy constraints
ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
ESP_ERR_NVS_NOT_ENOUGH_SPACE if there is no space for a new entry or there are too many different namespaces (maximum allowed different namespaces: 254)
ESP_ERR_NOT_ALLOWED if the NVS partition is read-only and mode is NVS_READWRITE
ESP_ERR_INVALID_ARG if out_handle is equal to NULL
other error codes from the underlying storage driver 





```

#### nvs_set_blob


```


esp_err_t nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length)
set variable length binary value for given key 
Sets variable length binary value for the key. Function uses 2 overhead and 1 entry per each 32 bytes of new data from the pool of available entries. See nvs_get_stats . In case of value update for existing key, space occupied by the existing value and 2 overhead entries are returned to the pool of available entries. Note that the underlying storage will not be updated until nvs_commit is called.

Parameters

handle -- [in] Handle obtained from nvs_open function. Handles that were opened read only cannot be used. 
key -- [in] Key name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
value -- [in] The value to set. 
length -- [in] length of binary value to set, in bytes; Maximum length is 508000 bytes or (97.6% of the partition size - 4000) bytes whichever is lower.


Returns

ESP_OK if value was set successfully
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_ERR_NVS_READ_ONLY if storage handle was opened as read only
ESP_ERR_NVS_INVALID_NAME if key name doesn't satisfy constraints
ESP_ERR_NVS_NOT_ENOUGH_SPACE if there is not enough space in the underlying storage to save the value
ESP_ERR_NVS_REMOVE_FAILED if the value wasn't updated because flash write operation has failed. The value was written however, and update will be finished after re-initialization of nvs, provided that flash operation doesn't fail again.
ESP_ERR_NVS_VALUE_TOO_LONG if the value is too long 





```

#### nvs_find_key


```


esp_err_t nvs_find_key(nvs_handle_t handle, const char *key, nvs_type_t *out_type)
Lookup key-value pair with given key name. 
Note that function may indicate both existence of the key as well as the data type of NVS entry if it is found.

Parameters

handle -- [in] Storage handle obtained with nvs_open. 
key -- [in] Key name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty. 
out_type -- [out] Pointer to the output variable populated with data type of NVS entry in case key was found. May be NULL, respective data type is then not provided. 


Returns

ESP_OK if NVS entry for key provided was found
ESP_ERR_NVS_NOT_FOUND if the requested key doesn't exist
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
other error codes from the underlying storage driver 





```

#### nvs_erase_key


```


esp_err_t nvs_erase_key(nvs_handle_t handle, const char *key)
Erase key-value pair with given key name. 
Note that actual storage may not be updated until nvs_commit function is called.

Parameters

handle -- [in] Storage handle obtained with nvs_open. Handles that were opened read only cannot be used.
key -- [in] Key name. Maximum length is (NVS_KEY_NAME_MAX_SIZE-1) characters. Shouldn't be empty.


Returns

ESP_OK if erase operation was successful
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_ERR_NVS_READ_ONLY if handle was opened as read only
ESP_ERR_NVS_NOT_FOUND if the requested key doesn't exist
other error codes from the underlying storage driver 





```

#### nvs_erase_all


```


esp_err_t nvs_erase_all(nvs_handle_t handle)
Erase all key-value pairs in a namespace. 
Note that actual storage may not be updated until nvs_commit function is called.

Parameters
handle -- [in] Storage handle obtained with nvs_open. Handles that were opened read only cannot be used.

Returns

ESP_OK if erase operation was successful
ESP_FAIL if there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
ESP_ERR_NVS_READ_ONLY if handle was opened as read only
other error codes from the underlying storage driver 





```

#### nvs_commit


```


esp_err_t nvs_commit(nvs_handle_t handle)
Write any pending changes to non-volatile storage. 
After setting any values, nvs_commit() must be called to ensure changes are written to non-volatile storage. Individual implementations may write to storage at other times, but this is not guaranteed.

Parameters
handle -- [in] Storage handle obtained with nvs_open. Handles that were opened read only cannot be used.

Returns

ESP_OK if the changes have been written successfully
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL
other error codes from the underlying storage driver 





```

#### nvs_close


```


void nvs_close(nvs_handle_t handle)
Close the storage handle and free any allocated resources. 
This function should be called for each handle opened with nvs_open once the handle is not in use any more. Closing the handle may not automatically write the changes to nonvolatile storage. This has to be done explicitly using nvs_commit function. Once this function is called on a handle, the handle should no longer be used.

Parameters
handle -- [in] Storage handle to close 



```

#### nvs_get_stats


```


esp_err_t nvs_get_stats(const char *part_name, nvs_stats_t *nvs_stats)
Fill structure nvs_stats_t. It provides info about memory used by NVS. 
This function calculates the number of used entries, free entries, available entries, total entries and number of namespaces in partition.
// Example of nvs_get_stats() to get overview of actual statistics of data entries :
nvs_stats_t nvs_stats;
nvs_get_stats(NULL, &nvs_stats);
printf("Count: UsedEntries = (%lu), FreeEntries = (%lu), AvailableEntries = (%lu), AllEntries = (%lu)\n",
       nvs_stats.used_entries, nvs_stats.free_entries, nvs_stats.available_entries, nvs_stats.total_entries);



Parameters

part_name -- [in] Partition name NVS in the partition table. If pass a NULL than will use NVS_DEFAULT_PART_NAME ("nvs").
nvs_stats -- [out] Returns filled structure nvs_states_t. It provides info about used memory the partition.


Returns

ESP_OK if the changes have been written successfully. Return param nvs_stats will be filled.
ESP_ERR_NVS_PART_NOT_FOUND if the partition with label "name" is not found. Return param nvs_stats will be filled 0.
ESP_ERR_NVS_NOT_INITIALIZED if the storage driver is not initialized. Return param nvs_stats will be filled 0.
ESP_ERR_INVALID_ARG if nvs_stats is equal to NULL.
ESP_ERR_INVALID_STATE if there is page with the status of INVALID. Return param nvs_stats will be filled not with correct values because not all pages will be counted. Counting will be interrupted at the first INVALID page. 





```

#### nvs_get_used_entry_count


```


esp_err_t nvs_get_used_entry_count(nvs_handle_t handle, size_t *used_entries)
Calculate all entries in a namespace. 
An entry represents the smallest storage unit in NVS. Strings and blobs may occupy more than one entry. Note that to find out the total number of entries occupied by the namespace, add one to the returned value used_entries (if err is equal to ESP_OK). Because the name space entry takes one entry.
// Example of nvs_get_used_entry_count() to get amount of all key-value pairs in one namespace:
nvs_handle_t handle;
nvs_open("namespace1", NVS_READWRITE, &handle);
...
size_t used_entries;
size_t total_entries_namespace;
if(nvs_get_used_entry_count(handle, &used_entries) == ESP_OK){
    // the total number of entries occupied by the namespace
    total_entries_namespace = used_entries + 1;
}



Parameters

handle -- [in] Handle obtained from nvs_open function.
used_entries -- [out] Returns amount of used entries from a namespace.


Returns

ESP_OK if the changes have been written successfully. Return param used_entries will be filled valid value.
ESP_ERR_NVS_NOT_INITIALIZED if the storage driver is not initialized. Return param used_entries will be filled 0.
ESP_ERR_NVS_INVALID_HANDLE if handle has been closed or is NULL. Return param used_entries will be filled 0.
ESP_ERR_INVALID_ARG if used_entries is equal to NULL.
Other error codes from the underlying storage driver. Return param used_entries will be filled 0. 





```

#### nvs_entry_find


```


esp_err_t nvs_entry_find(const char *part_name, const char *namespace_name, nvs_type_t type, nvs_iterator_t *output_iterator)
Create an iterator to enumerate NVS entries based on one or more parameters. 
// Example of listing all the key-value pairs of any type under specified partition and namespace
 nvs_iterator_t it = NULL;
 esp_err_t res = nvs_entry_find(<nvs_partition_name>, <namespace>, NVS_TYPE_ANY, &it);
 while(res == ESP_OK) {
     nvs_entry_info_t info;
     nvs_entry_info(it, &info); // Can omit error check if parameters are guaranteed to be non-NULL
     printf("key '%s', type '%d' \n", info.key, info.type);
     res = nvs_entry_next(&it);
 }
 nvs_release_iterator(it);



Parameters

part_name -- [in] Partition name
namespace_name -- [in] Set this value if looking for entries with a specific namespace. Pass NULL otherwise.
type -- [in] One of nvs_type_t values.
output_iterator -- [out] Set to a valid iterator to enumerate all the entries found. Set to NULL if no entry for specified criteria was found. If any other error except ESP_ERR_INVALID_ARG occurs, output_iterator is NULL, too. If ESP_ERR_INVALID_ARG occurs, output_iterator is not changed. If a valid iterator is obtained through this function, it has to be released using nvs_release_iterator when not used any more, unless ESP_ERR_INVALID_ARG is returned.


Returns

ESP_OK if no internal error or programming error occurred.
ESP_ERR_NVS_NOT_FOUND if no element of specified criteria has been found.
ESP_ERR_NO_MEM if memory has been exhausted during allocation of internal structures.
ESP_ERR_INVALID_ARG if any of the parameters is NULL. Note: don't release output_iterator in case ESP_ERR_INVALID_ARG has been returned 





```

#### nvs_entry_find_in_handle


```


esp_err_t nvs_entry_find_in_handle(nvs_handle_t handle, nvs_type_t type, nvs_iterator_t *output_iterator)
Create an iterator to enumerate NVS entries based on a handle and type. 
// Example of listing all the key-value pairs of any type under specified handle (which defines a partition and namespace)
 nvs_iterator_t it = NULL;
 esp_err_t res = nvs_entry_find_in_handle(<nvs_handle>, NVS_TYPE_ANY, &it);
 while(res == ESP_OK) {
     nvs_entry_info_t info;
     nvs_entry_info(it, &info); // Can omit error check if parameters are guaranteed to be non-NULL
     printf("key '%s', type '%d' \n", info.key, info.type);
     res = nvs_entry_next(&it);
 }
 nvs_release_iterator(it);



Parameters

handle -- [in] Handle obtained from nvs_open function.
type -- [in] One of nvs_type_t values.
output_iterator -- [out] Set to a valid iterator to enumerate all the entries found. Set to NULL if no entry for specified criteria was found. If any other error except ESP_ERR_INVALID_ARG occurs, output_iterator is NULL, too. If ESP_ERR_INVALID_ARG occurs, output_iterator is not changed. If a valid iterator is obtained through this function, it has to be released using nvs_release_iterator when not used any more, unless ESP_ERR_INVALID_ARG is returned.


Returns

ESP_OK if no internal error or programming error occurred.
ESP_ERR_NVS_NOT_FOUND if no element of specified criteria has been found.
ESP_ERR_NO_MEM if memory has been exhausted during allocation of internal structures.
ESP_ERR_NVS_INVALID_HANDLE if unknown handle was specified.
ESP_ERR_INVALID_ARG if output_iterator parameter is NULL. Note: don't release output_iterator in case ESP_ERR_INVALID_ARG has been returned 





```

#### nvs_entry_next


```


esp_err_t nvs_entry_next(nvs_iterator_t *iterator)
Advances the iterator to next item matching the iterator criteria. 
Note that any copies of the iterator will be invalid after this call.

Parameters
iterator -- [inout] Iterator obtained from nvs_entry_find or nvs_entry_find_in_handle function. Must be non-NULL. If any error except ESP_ERR_INVALID_ARG occurs, iterator is set to NULL. If ESP_ERR_INVALID_ARG occurs, iterator is not changed.

Returns

ESP_OK if no internal error or programming error occurred.
ESP_ERR_NVS_NOT_FOUND if no next element matching the iterator criteria.
ESP_ERR_INVALID_ARG if iterator is NULL.
Possibly other errors in the future for internal programming or flash errors. 





```

#### nvs_entry_info


```


esp_err_t nvs_entry_info(const nvs_iterator_t iterator, nvs_entry_info_t *out_info)
Fills nvs_entry_info_t structure with information about entry pointed to by the iterator. 

Parameters

iterator -- [in] Iterator obtained from nvs_entry_find or nvs_entry_find_in_handle function. Must be non-NULL.
out_info -- [out] Structure to which entry information is copied.


Returns

ESP_OK if all parameters are valid; current iterator data has been written to out_info
ESP_ERR_INVALID_ARG if one of the parameters is NULL. 





```

#### nvs_release_iterator


```


void nvs_release_iterator(nvs_iterator_t iterator)
Release iterator. 

Parameters
iterator -- [in] Release iterator obtained from nvs_entry_find or nvs_entry_find_in_handle or nvs_entry_next function. NULL argument is allowed. 



```



### Structures


#### nvs_entry_info_t


```


struct nvs_entry_info_t
information about entry obtained from nvs_entry_info function 

Public Members
namespace_name

char namespace_name[NVS_NS_NAME_MAX_SIZE]
Namespace to which key-value belong 

key

char key[NVS_KEY_NAME_MAX_SIZE]
Key of stored key-value pair 

type

nvs_type_t type
Type of stored key-value pair 



```

#### nvs_stats_t


```


struct nvs_stats_t

Note
Info about storage space NVS. 


Public Members
used_entries

size_t used_entries
Number of used entries. 

free_entries

size_t free_entries
Number of free entries. It includes also reserved entries. 

available_entries

size_t available_entries
Number of entries available for data storage. 

total_entries

size_t total_entries
Number of all entries. 

namespace_count

size_t namespace_count
Number of namespaces. 



```



### Macros


#### ESP_ERR_NVS_BASE


```


ESP_ERR_NVS_BASE
Starting number of error codes 

```

#### ESP_ERR_NVS_NOT_INITIALIZED


```


ESP_ERR_NVS_NOT_INITIALIZED
The storage driver is not initialized 

```

#### ESP_ERR_NVS_NOT_FOUND


```


ESP_ERR_NVS_NOT_FOUND
A requested entry couldn't be found or namespace doesn’t exist yet and mode is NVS_READONLY 

```

#### ESP_ERR_NVS_TYPE_MISMATCH


```


ESP_ERR_NVS_TYPE_MISMATCH
The type of set or get operation doesn't match the type of value stored in NVS 

```

#### ESP_ERR_NVS_READ_ONLY


```


ESP_ERR_NVS_READ_ONLY
Storage handle was opened as read only 

```

#### ESP_ERR_NVS_NOT_ENOUGH_SPACE


```


ESP_ERR_NVS_NOT_ENOUGH_SPACE
There is not enough space in the underlying storage to save the value 

```

#### ESP_ERR_NVS_INVALID_NAME


```


ESP_ERR_NVS_INVALID_NAME
Namespace name doesn’t satisfy constraints 

```

#### ESP_ERR_NVS_INVALID_HANDLE


```


ESP_ERR_NVS_INVALID_HANDLE
Handle has been closed or is NULL 

```

#### ESP_ERR_NVS_REMOVE_FAILED


```


ESP_ERR_NVS_REMOVE_FAILED
The value wasn’t updated because flash write operation has failed. The value was written however, and update will be finished after re-initialization of nvs, provided that flash operation doesn’t fail again. 

```

#### ESP_ERR_NVS_KEY_TOO_LONG


```


ESP_ERR_NVS_KEY_TOO_LONG
Key name is too long 

```

#### ESP_ERR_NVS_PAGE_FULL


```


ESP_ERR_NVS_PAGE_FULL
Internal error; never returned by nvs API functions 

```

#### ESP_ERR_NVS_INVALID_STATE


```


ESP_ERR_NVS_INVALID_STATE
NVS is in an inconsistent state due to a previous error. Call nvs_flash_init and nvs_open again, then retry. 

```

#### ESP_ERR_NVS_INVALID_LENGTH


```


ESP_ERR_NVS_INVALID_LENGTH
String or blob length is not sufficient to store data 

```

#### ESP_ERR_NVS_NO_FREE_PAGES


```


ESP_ERR_NVS_NO_FREE_PAGES
NVS partition doesn't contain any empty pages. This may happen if NVS partition was truncated. Erase the whole partition and call nvs_flash_init again. 

```

#### ESP_ERR_NVS_VALUE_TOO_LONG


```


ESP_ERR_NVS_VALUE_TOO_LONG
Value doesn't fit into the entry or string or blob length is longer than supported by the implementation 

```

#### ESP_ERR_NVS_PART_NOT_FOUND


```


ESP_ERR_NVS_PART_NOT_FOUND
Partition with specified name is not found in the partition table 

```

#### ESP_ERR_NVS_NEW_VERSION_FOUND


```


ESP_ERR_NVS_NEW_VERSION_FOUND
NVS partition contains data in new format and cannot be recognized by this version of code 

```

#### ESP_ERR_NVS_XTS_ENCR_FAILED


```


ESP_ERR_NVS_XTS_ENCR_FAILED
XTS encryption failed while writing NVS entry 

```

#### ESP_ERR_NVS_XTS_DECR_FAILED


```


ESP_ERR_NVS_XTS_DECR_FAILED
XTS decryption failed while reading NVS entry 

```

#### ESP_ERR_NVS_XTS_CFG_FAILED


```


ESP_ERR_NVS_XTS_CFG_FAILED
XTS configuration setting failed 

```

#### ESP_ERR_NVS_XTS_CFG_NOT_FOUND


```


ESP_ERR_NVS_XTS_CFG_NOT_FOUND
XTS configuration not found 

```

#### ESP_ERR_NVS_ENCR_NOT_SUPPORTED


```


ESP_ERR_NVS_ENCR_NOT_SUPPORTED
NVS encryption is not supported in this version 

```

#### ESP_ERR_NVS_KEYS_NOT_INITIALIZED


```


ESP_ERR_NVS_KEYS_NOT_INITIALIZED
NVS key partition is uninitialized 

```

#### ESP_ERR_NVS_CORRUPT_KEY_PART


```


ESP_ERR_NVS_CORRUPT_KEY_PART
NVS key partition is corrupt 

```

#### ESP_ERR_NVS_WRONG_ENCRYPTION


```


ESP_ERR_NVS_WRONG_ENCRYPTION
NVS partition is marked as encrypted with generic flash encryption. This is forbidden since the NVS encryption works differently. 

```

#### ESP_ERR_NVS_CONTENT_DIFFERS


```


ESP_ERR_NVS_CONTENT_DIFFERS
Internal error; never returned by nvs API functions. NVS key is different in comparison 

```

#### NVS_DEFAULT_PART_NAME


```


NVS_DEFAULT_PART_NAME
Default partition name of the NVS partition in the partition table 

```

#### NVS_PART_NAME_MAX_SIZE


```


NVS_PART_NAME_MAX_SIZE
maximum length of partition name (excluding null terminator) 

```

#### NVS_KEY_NAME_MAX_SIZE


```


NVS_KEY_NAME_MAX_SIZE
Maximum length of NVS key name (including null terminator) 

```

#### NVS_NS_NAME_MAX_SIZE


```


NVS_NS_NAME_MAX_SIZE
Maximum length of NVS namespace name (including null terminator) 

```

#### NVS_GUARD_SYSVIEW_MACRO_EXPANSION_PUSH


```


NVS_GUARD_SYSVIEW_MACRO_EXPANSION_PUSH()

```

#### NVS_GUARD_SYSVIEW_MACRO_EXPANSION_POP


```


NVS_GUARD_SYSVIEW_MACRO_EXPANSION_POP()

```



### Type Definitions


#### nvs_handle_t


```


typedef uint32_t nvs_handle_t
Opaque pointer type representing non-volatile storage handle 

```

#### nvs_handle


```


typedef nvs_handle_t nvs_handle

```

#### nvs_open_mode


```


typedef nvs_open_mode_t nvs_open_mode

```

#### nvs_iterator_t


```


typedef struct nvs_opaque_iterator_t *nvs_iterator_t
Opaque pointer type representing iterator to nvs entries 

```



### Enumerations


#### nvs_open_mode_t


```


enum nvs_open_mode_t
Mode of opening the non-volatile storage. 
Values:
NVS_READONLY

enumerator NVS_READONLY
Read only 

NVS_READWRITE

enumerator NVS_READWRITE
Read and write 


```

#### nvs_type_t


```


enum nvs_type_t
Types of variables. 
Values:
NVS_TYPE_U8

enumerator NVS_TYPE_U8
Type uint8_t 

NVS_TYPE_I8

enumerator NVS_TYPE_I8
Type int8_t 

NVS_TYPE_U16

enumerator NVS_TYPE_U16
Type uint16_t 

NVS_TYPE_I16

enumerator NVS_TYPE_I16
Type int16_t 

NVS_TYPE_U32

enumerator NVS_TYPE_U32
Type uint32_t 

NVS_TYPE_I32

enumerator NVS_TYPE_I32
Type int32_t 

NVS_TYPE_U64

enumerator NVS_TYPE_U64
Type uint64_t 

NVS_TYPE_I64

enumerator NVS_TYPE_I64
Type int64_t 

NVS_TYPE_STR

enumerator NVS_TYPE_STR
Type string 

NVS_TYPE_BLOB

enumerator NVS_TYPE_BLOB
Type blob 

NVS_TYPE_ANY

enumerator NVS_TYPE_ANY
Must be last 


```