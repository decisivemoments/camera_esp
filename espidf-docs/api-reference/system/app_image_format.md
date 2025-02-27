# App Image Format




## Application Image Structures


An application image consists of the following:


1. The `esp_image_header_t` structure describes the mode of SPI flash and the count of memory segments.
2. The `esp_image_segment_header_t` structure describes each segment, its length, and its location in ESP32-S3's memory, followed by the data with a length of `data_len`. The data offset for each segment in the image is calculated in the following way:



> * offset for 0 Segment = sizeof(`esp_image_header_t`) + sizeof(`esp_image_segment_header_t`)
> 	* offset for 1 Segment = offset for 0 Segment + length of 0 Segment + sizeof(`esp_image_segment_header_t`)
> 	* offset for 2 Segment = offset for 1 Segment + length of 1 Segment + sizeof(`esp_image_segment_header_t`)
> 	* ...


The count of each segment is defined in the `segment_count` field that is stored in `esp_image_header_t`. The count cannot be more than `ESP_IMAGE_MAX_SEGMENTS`.


To get the list of your image segments, please run the following command:



```
esptool.py --chip esp32s3 image_info build/app.bin

```



```
esptool.py v2.3.1
Image version: 1
Entry point: 40080ea4
13 segments

Segment 1: len 0x13ce0 load 0x3f400020 file_offs 0x00000018 SOC_DROM
Segment 2: len 0x00000 load 0x3ff80000 file_offs 0x00013d00 SOC_RTC_DRAM
Segment 3: len 0x00000 load 0x3ff80000 file_offs 0x00013d08 SOC_RTC_DRAM
Segment 4: len 0x028e0 load 0x3ffb0000 file_offs 0x00013d10 DRAM
Segment 5: len 0x00000 load 0x3ffb28e0 file_offs 0x000165f8 DRAM
Segment 6: len 0x00400 load 0x40080000 file_offs 0x00016600 SOC_IRAM
Segment 7: len 0x09600 load 0x40080400 file_offs 0x00016a08 SOC_IRAM
Segment 8: len 0x62e4c load 0x400d0018 file_offs 0x00020010 SOC_IROM
Segment 9: len 0x06cec load 0x40089a00 file_offs 0x00082e64 SOC_IROM
Segment 10: len 0x00000 load 0x400c0000 file_offs 0x00089b58 SOC_RTC_IRAM
Segment 11: len 0x00004 load 0x50000000 file_offs 0x00089b60 SOC_RTC_DATA
Segment 12: len 0x00000 load 0x50000004 file_offs 0x00089b6c SOC_RTC_DATA
Segment 13: len 0x00000 load 0x50000004 file_offs 0x00089b74 SOC_RTC_DATA
Checksum: e8 (valid)
Validation Hash: 407089ca0eae2bbf83b4120979d3354b1c938a49cb7a0c997f240474ef2ec76b (valid)

```


You can also see the information on segments in the ESP-IDF logs while your application is booting:



```
I (443) esp_image: segment 0: paddr=0x00020020 vaddr=0x3f400020 size=0x13ce0 ( 81120) map
I (489) esp_image: segment 1: paddr=0x00033d08 vaddr=0x3ff80000 size=0x00000 ( 0) load
I (530) esp_image: segment 2: paddr=0x00033d10 vaddr=0x3ff80000 size=0x00000 ( 0) load
I (571) esp_image: segment 3: paddr=0x00033d18 vaddr=0x3ffb0000 size=0x028e0 ( 10464) load
I (612) esp_image: segment 4: paddr=0x00036600 vaddr=0x3ffb28e0 size=0x00000 ( 0) load
I (654) esp_image: segment 5: paddr=0x00036608 vaddr=0x40080000 size=0x00400 ( 1024) load
I (695) esp_image: segment 6: paddr=0x00036a10 vaddr=0x40080400 size=0x09600 ( 38400) load
I (737) esp_image: segment 7: paddr=0x00040018 vaddr=0x400d0018 size=0x62e4c (405068) map
I (847) esp_image: segment 8: paddr=0x000a2e6c vaddr=0x40089a00 size=0x06cec ( 27884) load
I (888) esp_image: segment 9: paddr=0x000a9b60 vaddr=0x400c0000 size=0x00000 ( 0) load
I (929) esp_image: segment 10: paddr=0x000a9b68 vaddr=0x50000000 size=0x00004 ( 4) load
I (971) esp_image: segment 11: paddr=0x000a9b74 vaddr=0x50000004 size=0x00000 ( 0) load
I (1012) esp_image: segment 12: paddr=0x000a9b7c vaddr=0x50000004 size=0x00000 ( 0) load

```


For more details on the type of memory segments and their address ranges, see **ESP32-S3 Technical Reference Manual** > **System and Memory** > **Internal Memory** [PDF].


3. The image has a single checksum byte after the last segment. This byte is written on a sixteen byte padded boundary, so the application image might need padding.
4. If the `hash_appended` field from `esp_image_header_t` is set then a SHA256 checksum will be appended. The value of the SHA256 hash is calculated on the range from the first byte and up to this field. The length of this field is 32 bytes.
5. If the option CONFIG_SECURE_SIGNED_APPS_SCHEME is set to ECDSA then the application image will have an additional 68 bytes for an ECDSA signature, which includes:



> * version word (4 bytes)
> 	* signature data (64 bytes)
6. If the option CONFIG_SECURE_SIGNED_APPS_SCHEME is set to RSA or ECDSA (V2) then the application image will have an additional signature sector of 4 KB in size. For more details on the format of this signature sector, please refer to Signature Block Format.




## Application Description


The `DROM` segment of the application binary starts with the `esp_app_desc_t` structure which carries specific fields describing the application:


* `magic_word`: the magic word for the `esp_app_desc_t` structure
* `secure_version`: see Anti-rollback
* `version`: see App version 1
* `project_name`: filled from `PROJECT_NAME` 1
* `time` and `date`: compile time and date
* `idf_ver`: version of ESP-IDF 1
* `app_elf_sha256`: contains SHA256 hash for the application ELF file



1(1,2,3)
The maximum length is 32 characters, including null-termination character. For example, if the length of `PROJECT_NAME` exceeds 31 characters, the excess characters will be disregarded.




This structure is useful for identification of images uploaded via Over-the-Air (OTA) updates because it has a fixed offset = sizeof(`esp_image_header_t`) + sizeof(`esp_image_segment_header_t`). As soon as a device receives the first fragment containing this structure, it has all the information to determine whether the update should be continued with or not.


To obtain the `esp_app_desc_t` structure for the currently running application, use `esp_app_get_description()`.


To obtain the `esp_app_desc_t` structure for another OTA partition, use `esp_ota_get_partition_description()`.




## Adding a Custom Structure to an Application


Users also have the opportunity to have similar structure with a fixed offset relative to the beginning of the image.


The following pattern can be used to add a custom structure to your image:



```
const __attribute__((section(".rodata_custom_desc"))) esp_custom_app_desc_t custom_app_desc = { ... }

```


Offset for custom structure is sizeof(`esp_image_header_t`) + sizeof(`esp_image_segment_header_t`) + sizeof(`esp_app_desc_t`).


To guarantee that the custom structure is located in the image even if it is not used, you need to add `target_link_libraries(${COMPONENT_TARGET} "-u custom_app_desc")` into `CMakeLists.txt`.




## API Reference



### Header File


* components/bootloader_support/include/esp_app_format.h
* This header file can be included with:



> ```
> #include "esp_app_format.h"
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




### Structures


#### esp_image_header_t


```


struct esp_image_header_t
Main header of binary image. 

Public Members
magic

uint8_t magic
Magic word ESP_IMAGE_HEADER_MAGIC 

segment_count

uint8_t segment_count
Count of memory segments 

spi_mode

uint8_t spi_mode
flash read mode (esp_image_spi_mode_t as uint8_t) 

spi_speed

uint8_t spi_speed
flash frequency (esp_image_spi_freq_t as uint8_t) 

spi_size

uint8_t spi_size
flash chip size (esp_image_flash_size_t as uint8_t) 

entry_addr

uint32_t entry_addr
Entry address 

wp_pin

uint8_t wp_pin
WP pin when SPI pins set via efuse (read by ROM bootloader, the IDF bootloader uses software to configure the WP pin and sets this field to 0xEE=disabled) 

spi_pin_drv

uint8_t spi_pin_drv[3]
Drive settings for the SPI flash pins (read by ROM bootloader) 

chip_id

esp_chip_id_t chip_id
Chip identification number 

min_chip_rev

uint8_t min_chip_rev
Minimal chip revision supported by image After the Major and Minor revision eFuses were introduced into the chips, this field is no longer used. But for compatibility reasons, we keep this field and the data in it. Use min_chip_rev_full instead. The software interprets this as a Major version for most of the chips and as a Minor version for the ESP32-C3. 

min_chip_rev_full

uint16_t min_chip_rev_full
Minimal chip revision supported by image, in format: major * 100 + minor 

max_chip_rev_full

uint16_t max_chip_rev_full
Maximal chip revision supported by image, in format: major * 100 + minor 

reserved

uint8_t reserved[4]
Reserved bytes in additional header space, currently unused 

hash_appended

uint8_t hash_appended
If 1, a SHA256 digest "simple hash" (of the entire image) is appended after the checksum. Included in image length. This digest is separate to secure boot and only used for detecting corruption. For secure boot signed images, the signature is appended after this (and the simple hash is included in the signed data). 



```

#### esp_image_segment_header_t


```


struct esp_image_segment_header_t
Header of binary image segment. 

Public Members
load_addr

uint32_t load_addr
Address of segment 

data_len

uint32_t data_len
Length of data 



```



### Macros


#### ESP_IMAGE_HEADER_MAGIC


```


ESP_IMAGE_HEADER_MAGIC
The magic word for the esp_image_header_t structure. 

```

#### ESP_IMAGE_MAX_SEGMENTS


```


ESP_IMAGE_MAX_SEGMENTS
Max count of segments in the image. 

```



### Enumerations


#### esp_chip_id_t


```


enum esp_chip_id_t
ESP chip ID. 
Values:
ESP_CHIP_ID_ESP32

enumerator ESP_CHIP_ID_ESP32
chip ID: ESP32 

ESP_CHIP_ID_ESP32S2

enumerator ESP_CHIP_ID_ESP32S2
chip ID: ESP32-S2 

ESP_CHIP_ID_ESP32C3

enumerator ESP_CHIP_ID_ESP32C3
chip ID: ESP32-C3 

ESP_CHIP_ID_ESP32S3

enumerator ESP_CHIP_ID_ESP32S3
chip ID: ESP32-S3 

ESP_CHIP_ID_ESP32C2

enumerator ESP_CHIP_ID_ESP32C2
chip ID: ESP32-C2 

ESP_CHIP_ID_ESP32C6

enumerator ESP_CHIP_ID_ESP32C6
chip ID: ESP32-C6 

ESP_CHIP_ID_ESP32H2

enumerator ESP_CHIP_ID_ESP32H2
chip ID: ESP32-H2 

ESP_CHIP_ID_ESP32P4

enumerator ESP_CHIP_ID_ESP32P4
chip ID: ESP32-P4 

ESP_CHIP_ID_INVALID

enumerator ESP_CHIP_ID_INVALID
Invalid chip ID (we defined it to make sure the esp_chip_id_t is 2 bytes size) 


```

#### esp_image_spi_mode_t


```


enum esp_image_spi_mode_t
SPI flash mode, used in esp_image_header_t. 
Values:
ESP_IMAGE_SPI_MODE_QIO

enumerator ESP_IMAGE_SPI_MODE_QIO
SPI mode QIO 

ESP_IMAGE_SPI_MODE_QOUT

enumerator ESP_IMAGE_SPI_MODE_QOUT
SPI mode QOUT 

ESP_IMAGE_SPI_MODE_DIO

enumerator ESP_IMAGE_SPI_MODE_DIO
SPI mode DIO 

ESP_IMAGE_SPI_MODE_DOUT

enumerator ESP_IMAGE_SPI_MODE_DOUT
SPI mode DOUT 

ESP_IMAGE_SPI_MODE_FAST_READ

enumerator ESP_IMAGE_SPI_MODE_FAST_READ
SPI mode FAST_READ 

ESP_IMAGE_SPI_MODE_SLOW_READ

enumerator ESP_IMAGE_SPI_MODE_SLOW_READ
SPI mode SLOW_READ 


```

#### esp_image_spi_freq_t


```


enum esp_image_spi_freq_t
SPI flash clock division factor. 
Values:
ESP_IMAGE_SPI_SPEED_DIV_2

enumerator ESP_IMAGE_SPI_SPEED_DIV_2
The SPI flash clock frequency is divided by 2 of the clock source 

ESP_IMAGE_SPI_SPEED_DIV_3

enumerator ESP_IMAGE_SPI_SPEED_DIV_3
The SPI flash clock frequency is divided by 3 of the clock source 

ESP_IMAGE_SPI_SPEED_DIV_4

enumerator ESP_IMAGE_SPI_SPEED_DIV_4
The SPI flash clock frequency is divided by 4 of the clock source 

ESP_IMAGE_SPI_SPEED_DIV_1

enumerator ESP_IMAGE_SPI_SPEED_DIV_1
The SPI flash clock frequency equals to the clock source 


```

#### esp_image_flash_size_t


```


enum esp_image_flash_size_t
Supported SPI flash sizes. 
Values:
ESP_IMAGE_FLASH_SIZE_1MB

enumerator ESP_IMAGE_FLASH_SIZE_1MB
SPI flash size 1 MB 

ESP_IMAGE_FLASH_SIZE_2MB

enumerator ESP_IMAGE_FLASH_SIZE_2MB
SPI flash size 2 MB 

ESP_IMAGE_FLASH_SIZE_4MB

enumerator ESP_IMAGE_FLASH_SIZE_4MB
SPI flash size 4 MB 

ESP_IMAGE_FLASH_SIZE_8MB

enumerator ESP_IMAGE_FLASH_SIZE_8MB
SPI flash size 8 MB 

ESP_IMAGE_FLASH_SIZE_16MB

enumerator ESP_IMAGE_FLASH_SIZE_16MB
SPI flash size 16 MB 

ESP_IMAGE_FLASH_SIZE_32MB

enumerator ESP_IMAGE_FLASH_SIZE_32MB
SPI flash size 32 MB 

ESP_IMAGE_FLASH_SIZE_64MB

enumerator ESP_IMAGE_FLASH_SIZE_64MB
SPI flash size 64 MB 

ESP_IMAGE_FLASH_SIZE_128MB

enumerator ESP_IMAGE_FLASH_SIZE_128MB
SPI flash size 128 MB 

ESP_IMAGE_FLASH_SIZE_MAX

enumerator ESP_IMAGE_FLASH_SIZE_MAX
SPI flash size MAX 


```