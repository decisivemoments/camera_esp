# Storage API



This section contains reference of the high-level storage APIs. They are based on low-level drivers such as SPI flash, SD/MMC.


* Partitions API allow block based access to SPI flash according to the Partition Tables.
* Non-Volatile Storage library (NVS) implements a fault-tolerant wear-levelled key-value storage in SPI NOR flash.
* Virtual File System (VFS) library provides an interface for registration of file system drivers. SPIFFS, FAT and various other file system libraries are based on the VFS.
* SPIFFS is a wear-levelled file system optimized for SPI NOR flash, well suited for small partition sizes and low throughput
* FAT is a standard file system which can be used in SPI flash or on SD/MMC cards
* Wear Levelling library implements a flash translation layer (FTL) suitable for SPI NOR flash. It is used as a container for FAT partitions in flash.



Note


It is suggested to use high-level APIs (`esp_partition` or file system) instead of low-level driver APIs to access the SPI NOR flash.


Due to the restriction of NOR flash and ESP hardware, accessing the main flash will affect the performance of the whole system. See SPI Flash API to learn more about the limitations.




* FAT Filesystem Support
* Manufacturing Utility
* Non-Volatile Storage Library
* NVS Encryption
* NVS Partition Generator Utility
* NVS Partition Parser Utility
* SD/SDIO/MMC Driver
* Partitions API
* SPIFFS Filesystem
* Virtual Filesystem Component
* Wear Levelling API



Code examples for this API section are provided in the storage directory of ESP-IDF examples.