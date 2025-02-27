# Bootloader Image Format



The bootloader image consists of the same structures as the application image, see Application Image Structures. The only difference is in the Bootloader Description structure.


To get information about the bootloader image, please run the following command:



```
esptool.py --chip esp32s3 image_info build/bootloader/bootloader.bin --version 2

```


The resultant output will resemble the following:



```
File size: 26576 (bytes)

ESP32 image header
==================
Image version: 1
Entry point: 0x40080658
Segments: 4
Flash size: 2MB
Flash freq: 40m
Flash mode: DIO

ESP32 extended image header
===========================
WP pin: 0xee
Flash pins drive settings: clk_drv: 0x0, q_drv: 0x0, d_drv: 0x0, cs0_drv: 0x0, hd_drv: 0x0, wp_drv: 0x0
Chip ID: 0
Minimal chip revision: v0.0, (legacy min_rev = 0)
Maximal chip revision: v3.99

Segments information
====================
Segment   Length   Load addr   File offs  Memory types
-------  -------  ----------  ----------  ------------
    1  0x01bb0  0x3fff0030  0x00000018  BYTE_ACCESSIBLE, DRAM, DIRAM_DRAM
    2  0x03c90  0x40078000  0x00001bd0  CACHE_APP
    3  0x00004  0x40080400  0x00005868  IRAM
    4  0x00f2c  0x40080404  0x00005874  IRAM

ESP32 image footer
==================
Checksum: 0x65 (valid)
Validation hash: 6f31a7f8512f26f6bce7c3b270f93bf6cf1ee4602c322998ca8ce27433527e92 (valid)

Bootloader information
======================
Bootloader version: 1
ESP-IDF: v5.1-dev-4304-gcb51a3b-dirty
Compile time: Mar 30 2023 19:14:17

```



## Bootloader Description


The `DRAM0` segment of the bootloader binary starts with the `esp_bootloader_desc_t` structure which carries specific fields describing the bootloader. This structure is located at a fixed offset = sizeof(`esp_image_header_t`) + sizeof(`esp_image_segment_header_t`).



> * `magic_byte`: the magic byte for the esp_bootloader_desc structure
> * `reserved`: reserved for the future IDF use
> * `version`: bootloader version, see CONFIG_BOOTLOADER_PROJECT_VER
> * `idf_ver`: ESP-IDF version. 1
> * `date` and `time`: compile date and time
> * `reserved2`: reserved for the future IDF use



1
The maximum length is 32 characters, including null-termination character.




To get the `esp_bootloader_desc_t` structure from the running bootloader, use `esp_bootloader_get_description()`.


To get the `esp_bootloader_desc_t` structure from a running application, use `esp_ota_get_bootloader_description()`.




## API Reference



### Header File


* components/esp_bootloader_format/include/esp_bootloader_desc.h
* This header file can be included with:



> ```
> #include "esp_bootloader_desc.h"
> 
> ```
* This header file is a part of the API provided by the `esp_bootloader_format` component. To declare that your component depends on `esp_bootloader_format`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_bootloader_format
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_bootloader_format
> 
> ```




### Functions


#### esp_bootloader_get_description


```


const esp_bootloader_desc_t *esp_bootloader_get_description(void)
Return esp_bootloader_desc structure. 
Intended for use by the bootloader. 

Returns
Pointer to esp_bootloader_desc structure. 



```



### Structures


#### esp_bootloader_desc_t


```


struct esp_bootloader_desc_t
Bootloader description structure. 

Public Members
magic_byte

uint8_t magic_byte
Magic byte ESP_BOOTLOADER_DESC_MAGIC_BYTE 

reserved

uint8_t reserved[3]
reserved for IDF 

version

uint32_t version
Bootloader version 

idf_ver

char idf_ver[32]
Version IDF 

date_time

char date_time[24]
Compile date and time 

reserved2

uint8_t reserved2[16]
reserved for IDF 



```



### Macros


#### ESP_BOOTLOADER_DESC_MAGIC_BYTE


```


ESP_BOOTLOADER_DESC_MAGIC_BYTE
The magic byte for the esp_bootloader_desc structure that is in DRAM. 

```