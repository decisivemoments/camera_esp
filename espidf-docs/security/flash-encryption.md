# Flash Encryption



This is a quick start guide to ESP32-S3's flash encryption feature. Using application code as an example, it demonstrates how to test and verify flash encryption operations during development and production.



## Introduction


Flash encryption is intended for encrypting the contents of the ESP32-S3's off-chip flash memory. Once this feature is enabled, firmware is flashed as plaintext, and then the data is encrypted in place on the first boot. As a result, physical readout of flash will not be sufficient to recover most flash contents.



Important


For production use, flash encryption should be enabled in the "Release" mode only.




Important


Enabling flash encryption limits the options for further updates of ESP32-S3. Before using this feature, read the document and make sure to understand the implications.





## Encrypted Partitions


With flash encryption enabled, the following types of data are encrypted by default:


* Second Stage Bootloader (Firmware Bootloader)
* Partition Table
* NVS Key Partition
* Otadata
* All `app` type partitions


Other types of data can be encrypted conditionally:


* Any partition marked with the `encrypted` flag in the partition table. For details, see Encrypted Partition Flag.
* Secure Boot bootloader digest if Secure Boot is enabled (see below).




## Relevant eFuses


The flash encryption operation is controlled by various eFuses available on ESP32-S3. The list of eFuses and their descriptions is given in the table below. The names in eFuse column are also used by espefuse.py tool. For usage in the eFuse API, modify the name by adding `ESP_EFUSE_`, for example: esp_efuse_read_field_bit(ESP_EFUSE_DISABLE_DL_ENCRYPT).




eFuses Used in Flash Encryption





| **eFuse** | **Description** | **Bit Depth** |
| --- | --- | --- |
| `BLOCK_KEYN` | AES key storage. N is between 0 and 5. | One 256 bit key block for XTS_AES_128, Two 256 bit key blocks for XTS_AES_256 (512 bit total) |
| `KEY_PURPOSE_N` | Controls the purpose of eFuse block `BLOCK_KEYN`, where N is between 0 and 5. Possible values: `2` for `XTS_AES_256_KEY_1` , `3` for `XTS_AES_256_KEY_2`, and `4` for `XTS_AES_128_KEY`. Final AES key is derived based on the value of one or two of these purpose eFuses. For a detailed description of the possible combinations, see *ESP32-S3 Technical Reference Manual* > *External Memory Encryption and Decryption (XTS_AES)* [PDF]. | 4 |
| `DIS_DOWNLOAD_MANUAL_ENCRYPT` | If set, disables flash encryption when in download bootmodes. | 1 |
| `SPI_BOOT_CRYPT_CNT` | Enables encryption and decryption, when an SPI boot mode is set. Feature is enabled if 1 or 3 bits are set in the eFuse, disabled otherwise. | 3 |



Note


* R/W access control is available for all the eFuse bits listed in the table above.
* The default value of these bits is 0 after manufacturing.



Read and write access to eFuse bits is controlled by appropriate fields in the registers `WR_DIS` and `RD_DIS`. For more information on ESP32-S3 eFuses, see eFuse manager. To change protection bits of eFuse field using espefuse.py, use these two commands: read_protect_efuse and write_protect_efuse. Example `espefuse.py write_protect_efuse DISABLE_DL_ENCRYPT`.




## Flash Encryption Process


Assuming that the eFuse values are in their default states and the firmware bootloader is compiled to support flash encryption, the flash encryption process executes as shown below:


1. On the first power-on reset, all data in flash is un-encrypted (plaintext). The ROM bootloader loads the firmware bootloader.
2. Firmware bootloader reads the `SPI_BOOT_CRYPT_CNT` eFuse value (`0b000`). Since the value is `0` (even number of bits set), it configures and enables the flash encryption block. For more information on the flash encryption block, see *ESP32-S3 Technical Reference Manual* > *eFuse Controller (eFuse)* > *Auto Encryption Block* [PDF].
3. Firmware bootloader first checks if a valid key is already present in the eFuse (e.g., burned using espefuse tool) then the process of key generation is skipped and the same key is used for flash encryption process. Otherwise, Firmware bootloader uses RNG (random) module to generate an 256 bit or 512 bit key, depending on the value of Size of generated XTS-AES key, and then writes it into respectively one or two BLOCK_KEYN eFuses. The software also updates the `KEY_PURPOSE_N` for the blocks where the keys were stored. The key cannot be accessed via software as the write and read protection bits for one or two BLOCK_KEYN eFuses are set. `KEY_PURPOSE_N` field is write-protected as well. The flash encryption operations happen entirely by hardware, and the key cannot be accessed via software.
4. Flash encryption block encrypts the flash contents - the firmware bootloader, applications and partitions marked as `encrypted`. Encrypting in-place can take time, up to a minute for large partitions.
5. Firmware bootloader sets the first available bit in `SPI_BOOT_CRYPT_CNT` (0b001) to mark the flash contents as encrypted. Odd number of bits is set.
6. For Development Mode, the firmware bootloader allows the UART bootloader to re-flash encrypted binaries. Also, the `SPI_BOOT_CRYPT_CNT` eFuse bits are NOT write-protected. In addition, the firmware bootloader by default sets the following eFuse bits:



> * `DIS_DOWNLOAD_ICACHE`
> * `DIS_DOWNLOAD_DCACHE`
> * `HARD_DIS_JTAG`
> * `DIS_LEGACY_SPI_BOOT`


7. For Release Mode, the firmware bootloader sets all the eFuse bits set under development mode as well as `DIS_DOWNLOAD_MANUAL_ENCRYPT`. It also write-protects the `SPI_BOOT_CRYPT_CNT` eFuse bits. To modify this behavior, see Enabling UART Bootloader Encryption/Decryption.
8. The device is then rebooted to start executing the encrypted image. The firmware bootloader calls the flash decryption block to decrypt the flash contents and then loads the decrypted contents into IRAM.


During the development stage, there is a frequent need to program different plaintext flash images and test the flash encryption process. This requires that Firmware Download mode is able to load new plaintext images as many times as it might be needed. However, during manufacturing or production stages, Firmware Download mode should not be allowed to access flash contents for security reasons.


Hence, two different flash encryption configurations were created: for development and for production. For details on these configurations, see Section Flash Encryption Configuration.




## Flash Encryption Configuration


The following flash encryption modes are available:


* Development Mode - recommended for use only during development. In this mode, it is still possible to flash new plaintext firmware to the device, and the bootloader will transparently encrypt this firmware using the key stored in hardware. This allows, indirectly, to read out the plaintext of the firmware in flash.
* Release Mode - recommended for manufacturing and production. In this mode, flashing plaintext firmware to the device without knowing the encryption key is no longer possible.


This section provides information on the mentioned flash encryption modes and step by step instructions on how to use them.



### Development Mode


During development, you can encrypt flash using either an ESP32-S3 generated key or external host-generated key.



#### Using ESP32-S3 Generated Key


Development mode allows you to download multiple plaintext images using Firmware Download mode.


To test flash encryption process, take the following steps:


1. Ensure that you have an ESP32-S3 device with default flash encryption eFuse settings as shown in Relevant eFuses.



> See how to check ESP32-S3 Flash Encryption Status.


2. In Project Configuration Menu, do the following:



> * Enable flash encryption on boot.
> * Select encryption mode (**Development mode** by default).
> * Select UART ROM download mode (**enabled** by default).
> * Set Size of generated XTS-AES key.
> * Select the appropriate bootloader log verbosity.
> * Save the configuration and exit.


Enabling flash encryption will increase the size of bootloader, which might require updating partition table offset. See Bootloader Size.


3. Run the command given below to build and flash the complete images.



> ```
> idf.py flash monitor
> 
> ```
> 
> 
> 
> Note
> 
> 
> This command does not include any user files which should be written to the partitions on the flash memory. Please write them manually before running this command otherwise the files should be encrypted separately before writing.
> 
> 
> 
> This command will write to flash memory unencrypted images: the firmware bootloader, the partition table and applications. Once the flashing is complete, ESP32-S3 will reset. On the next boot, the firmware bootloader encrypts: the firmware bootloader, application partitions and partitions marked as `encrypted` then resets. Encrypting in-place can take time, up to a minute for large partitions. After that, the application is decrypted at runtime and executed.


A sample output of the first ESP32-S3 boot after enabling flash encryption is given below:



```
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd0270,len:0x2598
load:0x403b6000,len:0x878
load:0x403ba000,len:0x3dd4
entry 0x403b61c0
I (27) boot: ESP-IDF v4.4-dev-2003-g72fdecc1b7-dirty 2nd stage bootloader
I (28) boot: compile time 14:15:37
I (28) boot: chip revision: 0
I (32) boot.esp32s3: SPI Speed      : 80MHz
I (36) boot.esp32s3: SPI Mode       : DIO
I (41) boot.esp32s3: SPI Flash Size : 2MB
I (46) boot: Enabling RNG early entropy source...
I (58) boot: Partition Table:
I (62) boot: ## Label            Usage          Type ST Offset   Length
I (69) boot:  0 nvs              WiFi data        01 02 0000a000 00006000
I (76) boot:  1 storage          Unknown data     01 ff 00010000 00001000
I (84) boot:  2 factory          factory app      00 00 00020000 00100000
I (91) boot:  3 nvs_key          NVS keys         01 04 00120000 00001000
I (99) boot: End of partition table
I (103) esp_image: segment 0: paddr=00020020 vaddr=3c020020 size=08118h ( 33048) map
I (117) esp_image: segment 1: paddr=00028140 vaddr=3fc8fa30 size=023f4h (  9204) load
I (122) esp_image: segment 2: paddr=0002a53c vaddr=40374000 size=05adch ( 23260) load
I (134) esp_image: segment 3: paddr=00030020 vaddr=42000020 size=1a710h (108304) map
I (156) esp_image: segment 4: paddr=0004a738 vaddr=40379adc size=05f48h ( 24392) load
I (162) esp_image: segment 5: paddr=00050688 vaddr=600fe000 size=00010h (    16) load
I (167) boot: Loaded app from partition at offset 0x20000
I (168) boot: Checking flash encryption...
I (173) efuse: Batch mode of writing fields is enabled
I (179) flash_encrypt: Generating new flash encryption key...
I (188) efuse: Writing EFUSE_BLK_KEY0 with purpose 4
W (194) flash_encrypt: Not disabling UART bootloader encryption
I (197) flash_encrypt: Disable UART bootloader cache...
I (203) flash_encrypt: Disable JTAG...
I (212) efuse: Batch mode. Prepared fields are committed
I (214) esp_image: segment 0: paddr=00000020 vaddr=3fcd0270 size=02598h (  9624)
I (223) esp_image: segment 1: paddr=000025c0 vaddr=403b6000 size=00878h (  2168)
I (230) esp_image: segment 2: paddr=00002e40 vaddr=403ba000 size=03dd4h ( 15828)
I (534) flash_encrypt: bootloader encrypted successfully
I (578) flash_encrypt: partition table encrypted and loaded successfully
I (578) flash_encrypt: Encrypting partition 1 at offset 0x10000 (length 0x1000)...
I (628) flash_encrypt: Done encrypting
I (629) esp_image: segment 0: paddr=00020020 vaddr=3c020020 size=08118h ( 33048) map
I (636) esp_image: segment 1: paddr=00028140 vaddr=3fc8fa30 size=023f4h (  9204)
I (640) esp_image: segment 2: paddr=0002a53c vaddr=40374000 size=05adch ( 23260)
I (651) esp_image: segment 3: paddr=00030020 vaddr=42000020 size=1a710h (108304) map
I (675) esp_image: segment 4: paddr=0004a738 vaddr=40379adc size=05f48h ( 24392)
I (679) esp_image: segment 5: paddr=00050688 vaddr=600fe000 size=00010h (    16)
I (680) flash_encrypt: Encrypting partition 2 at offset 0x20000 (length 0x100000)...
I (11571) flash_encrypt: Done encrypting
I (11571) flash_encrypt: Encrypting partition 3 at offset 0x120000 (length 0x1000)...
I (11617) flash_encrypt: Done encrypting
I (11618) flash_encrypt: Flash encryption completed
I (11623) boot: Resetting with flash encryption enabled...

```


A sample output of subsequent ESP32-S3 boots just mentions that flash encryption is already enabled:



```
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x3 (RTC_SW_SYS_RST),boot:0x8 (SPI_FAST_FLASH_BOOT)
Saved PC:0x403bb1d6
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd0270,len:0x2598
load:0x403b6000,len:0x878
load:0x403ba000,len:0x3dd4
entry 0x403b61c0
I (35) boot: ESP-IDF v4.4-dev-2003-g72fdecc1b7-dirty 2nd stage bootloader
I (35) boot: compile time 14:15:37
I (35) boot: chip revision: 0
I (39) boot.esp32s3: SPI Speed      : 80MHz
I (44) boot.esp32s3: SPI Mode       : DIO
I (48) boot.esp32s3: SPI Flash Size : 2MB
I (53) boot: Enabling RNG early entropy source...
I (65) boot: Partition Table:
I (69) boot: ## Label            Usage          Type ST Offset   Length
I (76) boot:  0 nvs              WiFi data        01 02 0000a000 00006000
I (84) boot:  1 storage          Unknown data     01 ff 00010000 00001000
I (91) boot:  2 factory          factory app      00 00 00020000 00100000
I (99) boot:  3 nvs_key          NVS keys         01 04 00120000 00001000
I (106) boot: End of partition table
I (110) esp_image: segment 0: paddr=00020020 vaddr=3c020020 size=08118h ( 33048) map
I (126) esp_image: segment 1: paddr=00028140 vaddr=3fc8fa30 size=023f4h (  9204) load
I (129) esp_image: segment 2: paddr=0002a53c vaddr=40374000 size=05adch ( 23260) load
I (141) esp_image: segment 3: paddr=00030020 vaddr=42000020 size=1a710h (108304) map
I (166) esp_image: segment 4: paddr=0004a738 vaddr=40379adc size=05f48h ( 24392) load
I (172) esp_image: segment 5: paddr=00050688 vaddr=600fe000 size=00010h (    16) load
I (177) boot: Loaded app from partition at offset 0x20000
I (178) boot: Checking flash encryption...
I (183) flash_encrypt: flash encryption is enabled (1 plaintext flashes left)
I (190) boot: Disabling RNG early entropy source...
I (214) cpu_start: Pro cpu up.
I (214) cpu_start: Starting app cpu, entry point is 0x40374fa8
0x40374fa8: call_start_cpu1 at /home/marius/esp-idf_3/components/esp_system/port/cpu_start.c:160

I (0) cpu_start: App cpu up.
I (228) cpu_start: Pro cpu start user code
I (228) cpu_start: cpu freq: 160000000
I (228) cpu_start: Application information:
I (231) cpu_start: Project name:     flash_encryption
I (237) cpu_start: App version:      v4.4-dev-2003-g72fdecc1b7-dirty
I (244) cpu_start: Compile time:     Jul 12 2021 14:15:34
I (250) cpu_start: ELF file SHA256:  a7e6343c6a1c2215...
I (256) cpu_start: ESP-IDF:          v4.4-dev-2003-g72fdecc1b7-dirty
I (263) heap_init: Initializing. RAM available for dynamic allocation:
I (270) heap_init: At 3FC92810 len 0004D7F0 (309 KiB): D/IRAM
I (277) heap_init: At 3FCE0000 len 0000EE34 (59 KiB): STACK/DRAM
I (283) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (290) spi_flash: detected chip: generic
I (294) spi_flash: flash io: dio
W (298) spi_flash: Detected size(8192k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (311) flash_encrypt: Flash encryption mode is DEVELOPMENT (not secure)
I (318) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.

Example to check Flash Encryption status
This is esp32s3 chip with 2 CPU core(s), WiFi/BLE, silicon revision 0, 2MB external flash
FLASH_CRYPT_CNT eFuse value is 1
Flash encryption feature is enabled in DEVELOPMENT mode

```


At this stage, if you need to update and re-flash binaries, see Re-flashing Updated Partitions.




#### Using Host Generated Key


It is possible to pre-generate a flash encryption key on the host computer and burn it into the eFuse. This allows you to pre-encrypt data on the host and flash already encrypted data without needing a plaintext flash update. This feature can be used in both Development Mode and Release Mode. Without a pre-generated key, data is flashed in plaintext and then ESP32-S3 encrypts the data in-place.



Note


This option is not recommended for production, unless a separate key is generated for each individual device.



To use a host generated key, take the following steps:


1. Ensure that you have an ESP32-S3 device with default flash encryption eFuse settings as shown in Relevant eFuses.



> See how to check ESP32-S3 Flash Encryption Status.


2. Generate a random key by running:



> If Size of generated XTS-AES key is AES-128 (256-bit key):
> 
> 
> 
> ```
> espsecure.py generate_flash_encryption_key my_flash_encryption_key.bin
> 
> ```
> 
> 
> else if Size of generated XTS-AES key is AES-256 (512-bit key):
> 
> 
> 
> ```
> espsecure.py generate_flash_encryption_key --keylen 512 my_flash_encryption_key.bin
> 
> ```


3. **Before the first encrypted boot**, burn the key into your device's eFuse using the command below. This action can be done **only once**.



> ```
> espefuse.py --port PORT burn_key BLOCK my_flash_encryption_key.bin KEYPURPOSE
> 
> ```
> 
> 
> where `BLOCK` is a free keyblock between `BLOCK_KEY0` and `BLOCK_KEY5`. And `KEYPURPOSE` is either `AES_256_KEY_1`, `XTS_AES_256_KEY_2`, `XTS_AES_128_KEY`. See ESP32-S3 Technical Reference Manual for a description of the key purposes.
> 
> 
> For AES-128 (256-bit key) - `XTS_AES_128_KEY`:
> 
> 
> 
> ```
> espefuse.py --port PORT burn_key BLOCK my_flash_encryption_key.bin XTS_AES_128_KEY
> 
> ```
> 
> 
> For AES-256 (512-bit key) - `XTS_AES_256_KEY_1` and `XTS_AES_256_KEY_2`. `espefuse.py` supports burning both these two key purposes together with a 512 bit key to two separate key blocks via the virtual key purpose `XTS_AES_256_KEY`. When this is used `espefuse.py` will burn the first 256 bit of the key to the specified `BLOCK` and burn the corresponding block key purpose to `XTS_AES_256_KEY_1`. The last 256 bit of the key will be burned to the first free key block after `BLOCK` and the corresponding block key purpose to `XTS_AES_256_KEY_2`
> 
> 
> 
> ```
> espefuse.py  --port PORT  burn_key BLOCK my_flash_encryption_key.bin XTS_AES_256_KEY
> 
> ```
> 
> 
> If you wish to specify exactly which two blocks are used then it is possible to divide key into two 256 bit keys, and manually burn each half with `XTS_AES_256_KEY_1` and `XTS_AES_256_KEY_2` as key purposes:
> 
> 
> 
> ```
> split -b 32 my_flash_encryption_key.bin my_flash_encryption_key.bin.
> espefuse.py  --port PORT  burn_key BLOCK my_flash_encryption_key.bin.aa XTS_AES_256_KEY_1
> espefuse.py  --port PORT  burn_key BLOCK+1 my_flash_encryption_key.bin.ab XTS_AES_256_KEY_2
> 
> ```
> 
> 
> If the key is not burned and the device is started after enabling flash encryption, the ESP32-S3 will generate a random key that software cannot access or modify.


4. In Project Configuration Menu, do the following:



> * Enable flash encryption on boot
> 	* Select encryption mode (**Development mode** by default)
> 	* Select the appropriate bootloader log verbosity
> 	* Save the configuration and exit.


Enabling flash encryption will increase the size of bootloader, which might require updating partition table offset. See Bootloader Size.


5. Run the command given below to build and flash the complete images.



> ```
> idf.py flash monitor
> 
> ```
> 
> 
> 
> Note
> 
> 
> This command does not include any user files which should be written to the partitions on the flash memory. Please write them manually before running this command otherwise the files should be encrypted separately before writing.
> 
> 
> 
> This command will write to flash memory unencrypted images: the firmware bootloader, the partition table and applications. Once the flashing is complete, ESP32-S3 will reset. On the next boot, the firmware bootloader encrypts: the firmware bootloader, application partitions and partitions marked as `encrypted` then resets. Encrypting in-place can take time, up to a minute for large partitions. After that, the application is decrypted at runtime and executed.


If using Development Mode, then the easiest way to update and re-flash binaries is Re-flashing Updated Partitions.


If using Release Mode, then it is possible to pre-encrypt the binaries on the host and then flash them as ciphertext. See Manually Encrypting Files.




#### Re-flashing Updated Partitions


If you update your application code (done in plaintext) and want to re-flash it, you will need to encrypt it before flashing. To encrypt the application and flash it in one step, run:



```
idf.py encrypted-app-flash monitor

```


If all partitions needs to be updated in encrypted format, run:



```
idf.py encrypted-flash monitor

```





### Release Mode


In Release mode, UART bootloader cannot perform flash encryption operations. New plaintext images can ONLY be downloaded using the over-the-air (OTA) scheme which will encrypt the plaintext image before writing to flash.


To use this mode, take the following steps:


1. Ensure that you have an ESP32-S3 device with default flash encryption eFuse settings as shown in Relevant eFuses.



> See how to check ESP32-S3 Flash Encryption Status.


2. In Project Configuration Menu, do the following:



> * Enable flash encryption on boot
> * Select Release mode (Note that once Release mode is selected, the `EFUSE_DIS_DOWNLOAD_MANUAL_ENCRYPT` eFuse bit will be burned to disable flash encryption hardware in ROM Download Mode.)
> * Select UART ROM download mode (Permanently switch to Secure mode (recommended)). This is the default option, and is recommended. It is also possible to change this configuration setting to permanently disable UART ROM download mode, if this mode is not needed.
> * Select the appropriate bootloader log verbosity
> * Save the configuration and exit.


Enabling flash encryption will increase the size of bootloader, which might require updating partition table offset. See Bootloader Size.


3. Run the command given below to build and flash the complete images.



> ```
> idf.py flash monitor
> 
> ```
> 
> 
> 
> Note
> 
> 
> This command does not include any user files which should be written to the partitions on the flash memory. Please write them manually before running this command otherwise the files should be encrypted separately before writing.
> 
> 
> 
> This command will write to flash memory unencrypted images: the firmware bootloader, the partition table and applications. Once the flashing is complete, ESP32-S3 will reset. On the next boot, the firmware bootloader encrypts: the firmware bootloader, application partitions and partitions marked as `encrypted` then resets. Encrypting in-place can take time, up to a minute for large partitions. After that, the application is decrypted at runtime and executed.


Once the flash encryption is enabled in Release mode, the bootloader will write-protect the `SPI_BOOT_CRYPT_CNT` eFuse.


For subsequent plaintext field updates, use OTA scheme.



Note


If you have pre-generated the flash encryption key and stored a copy, and the UART download mode is not permanently disabled via CONFIG_SECURE_UART_ROM_DL_MODE , then it is possible to update the flash locally by pre-encrypting the files and then flashing the ciphertext. See Manually Encrypting Files.





### Best Practices


When using Flash Encryption in production:


* Do not reuse the same flash encryption key between multiple devices. This means that an attacker who copies encrypted data from one device cannot transfer it to a second device.
* The UART ROM Download Mode should be disabled entirely if it is not needed, or permanently set to "Secure Download Mode" otherwise. Secure Download Mode permanently limits the available commands to updating SPI config, changing baud rate, basic flash write, and returning a summary of the currently enabled security features with the get_security_info command. The default behaviour is to set Secure Download Mode on first boot in Release mode. To disable Download Mode entirely, select CONFIG_SECURE_UART_ROM_DL_MODE to "Permanently disable ROM Download Mode (recommended)" or call `esp_efuse_disable_rom_download_mode()` at runtime.
* Enable Secure Boot as an extra layer of protection, and to prevent an attacker from selectively corrupting any part of the flash before boot.








## Enable Flash Encryption Externally


In the process mentioned above, flash encryption related eFuses which ultimately enable flash encryption are programmed through the firmware bootloader. Alternatively, all the eFuses can be programmed with the help of `espefuse` tool. Please refer Enable Flash Encryption Externally for more details.




## Possible Failures


Once flash encryption is enabled, the `SPI_BOOT_CRYPT_CNT` eFuse value will have an odd number of bits set. It means that all the partitions marked with the encryption flag are expected to contain encrypted ciphertext. Below are the three typical failure cases if the ESP32-S3 is erroneously loaded with plaintext data:


1. If the bootloader partition is re-flashed with a **plaintext firmware bootloader image**, the ROM bootloader will fail to load the firmware bootloader resulting in the following failure:



```
rst:0x3 (SW_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
invalid header: 0xb414f76b
invalid header: 0xb414f76b
invalid header: 0xb414f76b
invalid header: 0xb414f76b
invalid header: 0xb414f76b
invalid header: 0xb414f76b
invalid header: 0xb414f76b

```



Note


The value of invalid header will be different for every application.




Note


This error also appears if the flash contents are erased or corrupted.



2. If the firmware bootloader is encrypted, but the partition table is re-flashed with a **plaintext partition table image**, the bootloader will fail to read the partition table resulting in the following failure:



> ```
> rst:0x3 (SW_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
> configsip: 0, SPIWP:0xee
> clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
> mode:DIO, clock div:2
> load:0x3fff0018,len:4
> load:0x3fff001c,len:10464
> ho 0 tail 12 room 4
> load:0x40078000,len:19168
> load:0x40080400,len:6664
> entry 0x40080764
> I (60) boot: ESP-IDF v4.0-dev-763-g2c55fae6c-dirty 2nd stage bootloader
> I (60) boot: compile time 19:15:54
> I (62) boot: Enabling RNG early entropy source...
> I (67) boot: SPI Speed      : 40MHz
> I (72) boot: SPI Mode       : DIO
> I (76) boot: SPI Flash Size : 4MB
> E (80) flash_parts: partition 0 invalid magic number 0x94f6
> E (86) boot: Failed to verify partition table
> E (91) boot: load partition table error!
> 
> ```


3. If the bootloader and partition table are encrypted, but the application is re-flashed with a **plaintext application image**, the bootloader will fail to load the application resulting in the following failure:



> ```
> rst:0x3 (SW_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
> configsip: 0, SPIWP:0xee
> clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
> mode:DIO, clock div:2
> load:0x3fff0018,len:4
> load:0x3fff001c,len:8452
> load:0x40078000,len:13616
> load:0x40080400,len:6664
> entry 0x40080764
> I (56) boot: ESP-IDF v4.0-dev-850-gc4447462d-dirty 2nd stage bootloader
> I (56) boot: compile time 15:37:14
> I (58) boot: Enabling RNG early entropy source...
> I (64) boot: SPI Speed      : 40MHz
> I (68) boot: SPI Mode       : DIO
> I (72) boot: SPI Flash Size : 4MB
> I (76) boot: Partition Table:
> I (79) boot: ## Label            Usage          Type ST Offset   Length
> I (87) boot:  0 nvs              WiFi data        01 02 0000a000 00006000
> I (94) boot:  1 phy_init         RF data          01 01 00010000 00001000
> I (102) boot:  2 factory          factory app      00 00 00020000 00100000
> I (109) boot: End of partition table
> E (113) esp_image: image at 0x20000 has invalid magic byte
> W (120) esp_image: image at 0x20000 has invalid SPI mode 108
> W (126) esp_image: image at 0x20000 has invalid SPI size 11
> E (132) boot: Factory app partition is not bootable
> E (138) boot: No bootable app partitions in the partition table
> 
> ```




## ESP32-S3 Flash Encryption Status


1. Ensure that you have an ESP32-S3 device with default flash encryption eFuse settings as shown in Relevant eFuses.


To check if flash encryption on your ESP32-S3 device is enabled, do one of the following:


* flash the application example security/flash_encryption onto your device. This application prints the `SPI_BOOT_CRYPT_CNT` eFuse value and if flash encryption is enabled or disabled.
* Find the serial port name under which your ESP32-S3 device is connected, replace `PORT` with your port name in the following command, and run it:



```
espefuse.py -p PORT summary

```




## Reading and Writing Data in Encrypted Flash


ESP32-S3 application code can check if flash encryption is currently enabled by calling `esp_flash_encryption_enabled()`. Also, a device can identify the flash encryption mode by calling `esp_get_flash_encryption_mode()`.


Once flash encryption is enabled, be more careful with accessing flash contents from code.



### Scope of Flash Encryption


Whenever the `SPI_BOOT_CRYPT_CNT` eFuse is set to a value with an odd number of bits, all flash content accessed via the MMU's flash cache is transparently decrypted. It includes:


* Executable application code in flash (IROM).
* All read-only data stored in flash (DROM).
* Any data accessed via `spi_flash_mmap()`.
* The firmware bootloader image when it is read by the ROM bootloader.



Important


The MMU flash cache unconditionally decrypts all existing data. Data which is stored unencrypted in flash memory will also be "transparently decrypted" via the flash cache and will appear to software as random garbage.





### Reading from Encrypted Flash


To read data without using a flash cache MMU mapping, you can use the partition read function `esp_partition_read()`. This function will only decrypt data when it is read from an encrypted partition. Data read from unencrypted partitions will not be decrypted. In this way, software can access encrypted and non-encrypted flash in the same way.


You can also use the following SPI flash API functions:


* `esp_flash_read()` to read raw (encrypted) data which will not be decrypted
* `esp_flash_read_encrypted()` to read and decrypt data


Data stored using the Non-Volatile Storage (NVS) API is always stored and read decrypted from the perspective of flash encryption. It is up to the library to provide encryption feature if required. Refer to NVS Encryption for more details.




### Writing to Encrypted Flash


It is recommended to use the partition write function `esp_partition_write()`. This function will only encrypt data when it is written to an encrypted partition. Data written to unencrypted partitions will not be encrypted. In this way, software can access encrypted and non-encrypted flash in the same way.


You can also pre-encrypt and write data using the function `esp_flash_write_encrypted()`


Also, the following ROM function exist but not supported in esp-idf applications:


* `esp_rom_spiflash_write_encrypted` pre-encrypts and writes data to flash
* `SPIWrite` writes unencrypted data to flash


Since data is encrypted in blocks, the minimum write size for encrypted data is 16 bytes and the alignment is also 16 bytes.





## Updating Encrypted Flash



### OTA Updates


OTA updates to encrypted partitions will automatically write encrypted data if the function `esp_partition_write()` is used.


Before building the application image for OTA updating of an already encrypted device, enable the option Enable flash encryption on boot in project configuration menu.


For general information about ESP-IDF OTA updates, please refer to OTA




### Updating Encrypted Flash via Serial


Flashing an encrypted device via serial bootloader requires that the serial bootloader download interface has not been permanently disabled via eFuse.


In Development Mode, the recommended method is Re-flashing Updated Partitions.


In Release Mode, if a copy of the same key stored in eFuse is available on the host then it is possible to pre-encrypt files on the host and then flash them. See Manually Encrypting Files.





## Disabling Flash Encryption


If flash encryption was enabled accidentally, flashing of plaintext data will soft-brick the ESP32-S3. The device will reboot continuously, printing the error `flash read err, 1000` or `invalid header: 0xXXXXXX`.


For flash encryption in Development mode, encryption can be disabled by burning the `SPI_BOOT_CRYPT_CNT` eFuse. It can only be done one time per chip by taking the following steps:


1. In Project Configuration Menu, disable Enable flash encryption on boot, then save and exit.
2. Open project configuration menu again and **double-check** that you have disabled this option! If this option is left enabled, the bootloader will immediately re-enable encryption when it boots.
3. With flash encryption disabled, build and flash the new bootloader and application by running `idf.py flash`.
4. Use `espefuse.py` (in `components/esptool_py/esptool`) to disable the `SPI_BOOT_CRYPT_CNT` by running:



> ```
> espefuse.py burn_efuse SPI_BOOT_CRYPT_CNT
> 
> ```


Reset the ESP32-S3. Flash encryption will be disabled, and the bootloader will boot as usual.




## Key Points About Flash Encryption


* Flash memory contents is encrypted using XTS-AES-128 or XTS-AES-256. The flash encryption key is 256 bits and 512 bits respectively and stored in one or two `BLOCK_KEYN` eFuses internal to the chip and, by default, is protected from software access.
* Flash access is transparent via the flash cache mapping feature of ESP32-S3 - any flash regions which are mapped to the address space will be transparently decrypted when read.


Some data partitions might need to remain unencrypted for ease of access or might require the use of flash-friendly update algorithms which are ineffective if the data is encrypted. NVS partitions for non-volatile storage cannot be encrypted since the NVS library is not directly compatible with flash encryption. For details, refer to NVS Encryption.
* If flash encryption might be used in future, the programmer must keep it in mind and take certain precautions when writing code that uses encrypted flash.
* If secure boot is enabled, re-flashing the bootloader of an encrypted device requires a "Re-flashable" secure boot digest (see Flash Encryption and Secure Boot).





Enabling flash encryption will increase the size of bootloader, which might require updating partition table offset. See Bootloader Size.



> Important
> 
> 
> Do not interrupt power to the ESP32-S3 while the first boot encryption pass is running. If power is interrupted, the flash contents will be corrupted and will require flashing with unencrypted data again. In this case, re-flashing will not count towards the flashing limit.




## Limitations of Flash Encryption


Flash encryption protects firmware against unauthorised readout and modification. It is important to understand the limitations of the flash encryption feature:


* Flash encryption is only as strong as the key. For this reason, we recommend keys are generated on the device during first boot (default behaviour). If generating keys off-device, ensure proper procedure is followed and do not share the same key between all production devices.
* Not all data is stored encrypted. If storing data on flash, check if the method you are using (library, API, etc.) supports flash encryption.
* Flash encryption does not prevent an attacker from understanding the high-level layout of the flash. This is because the same AES key is used for every pair of adjacent 16 byte AES blocks. When these adjacent 16 byte blocks contain identical content (such as empty or padding areas), these blocks will encrypt to produce matching pairs of encrypted blocks. This may allow an attacker to make high-level comparisons between encrypted devices (i.e., to tell if two devices are probably running the same firmware version).
* Flash encryption alone may not prevent an attacker from modifying the firmware of the device. To prevent unauthorised firmware from running on the device, use flash encryption in combination with Secure Boot.







## Flash Encryption and Secure Boot


It is recommended to use flash encryption in combination with Secure Boot. However, if Secure Boot is enabled, additional restrictions apply to device re-flashing:


* OTA Updates are not restricted, provided that the new app is signed correctly with the Secure Boot signing key.




## Advanced Features


The following section covers advanced features of flash encryption.



### Encrypted Partition Flag


Some partitions are encrypted by default. Other partitions can be marked in the partition table description as requiring encryption by adding the flag `encrypted` to the partitions' flag field. As a result, data in these marked partitions will be treated as encrypted in the same manner as an app partition.



```
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x6000
phy_init, data, phy,     0xf000,  0x1000
factory,  app,  factory, 0x10000, 1M
secret_data, 0x40, 0x01, 0x20000, 256K, encrypted

```


For details on partition table description, see partition table.


Further information about encryption of partitions:


* Default partition tables do not include any encrypted data partitions.
* With flash encryption enabled, the `app` partition is always treated as encrypted and does not require marking.
* If flash encryption is not enabled, the flag "encrypted" has no effect.
* You can also consider protecting `phy_init` data from physical access, readout, or modification, by marking the optional `phy` partition with the flag `encrypted`.
* The `nvs` partition cannot be encrypted, because the NVS library is not directly compatible with flash encryption.




### Enabling UART Bootloader Encryption/Decryption


On the first boot, the flash encryption process burns by default the following eFuses:


* `DIS_DOWNLOAD_MANUAL_ENCRYPT` which disables flash encryption operation when running in UART bootloader boot mode.
* `DIS_DOWNLOAD_ICACHE` and `DIS_DOWNLOAD_DCACHE` which disables the entire MMU flash cache when running in UART bootloader mode.
* `HARD_DIS_JTAG` and `DIS_USB_JTAG` which disables JTAG.
* `DIS_DIRECT_BOOT` (old name `DIS_LEGACY_SPI_BOOT`) which disables direct boot mode





However, before the first boot you can choose to keep any of these features enabled by burning only selected eFuses and write-protect the rest of eFuses with unset value 0. For example:



```
espefuse.py --port PORT burn_efuse DIS_DOWNLOAD_MANUAL_ENCRYPT
espefuse.py --port PORT write_protect_efuse DIS_DOWNLOAD_MANUAL_ENCRYPT

```



Note


Set all appropriate bits before write-protecting!


Write protection of all the three eFuses is controlled by one bit. It means that write-protecting one eFuse bit will inevitably write-protect all unset eFuse bits.



Write protecting these eFuses to keep them unset is not currently very useful, as `esptool.py` does not support reading encrypted flash.




### JTAG Debugging


By default, when Flash Encryption is enabled (in either Development or Release mode) then JTAG debugging is disabled via eFuse. The bootloader does this on first boot, at the same time it enables flash encryption.


See JTAG with Flash Encryption or Secure Boot for more information about using JTAG Debugging with Flash Encryption.




### Manually Encrypting Files


Manually encrypting or decrypting files requires the flash encryption key to be pre-burned in eFuse (see Using Host Generated Key) and a copy to be kept on the host. If the flash encryption is configured in Development Mode then it is not necessary to keep a copy of the key or follow these steps, the simpler Re-flashing Updated Partitions steps can be used.


The key file should be a single raw binary file (example: `key.bin`).


For example, these are the steps to encrypt the file `build/my-app.bin` to flash at offset 0x10000. Run espsecure.py as follows:



```
espsecure.py encrypt_flash_data --aes_xts --keyfile /path/to/key.bin --address 0x10000 --output my-app-ciphertext.bin build/my-app.bin

```


The file `my-app-ciphertext.bin` can then be flashed to offset 0x10000 using `esptool.py`. To see all of the command line options recommended for `esptool.py`, see the output printed when `idf.py build` succeeds.



Note


If the flashed ciphertext file is not recognized by the ESP32-S3 when it boots, check that the keys match and that the command line arguments match exactly, including the correct offset.



The command `espsecure.py decrypt_flash_data` can be used with the same options (and different input/output files), to decrypt ciphertext flash contents or a previously encrypted file.





## External RAM


When Flash Encryption is enabled any data read from and written to external SPI RAM through the cache will also be encrypted/decrypted. This happens the same way and with the same key as for Flash Encryption. If Flash Encryption is enabled then encryption for external SPI RAM is also always enabled, it is not possible to separately control this functionality.




## Technical Details


The following sections provide some reference information about the operation of flash encryption.



### Flash Encryption Algorithm


* ESP32-S3 use the XTS-AES block cipher mode with 256 bit or 512 bit key size for flash encryption.
* XTS-AES is a block cipher mode specifically designed for disc encryption and addresses the weaknesses other potential modes (e.g., AES-CTR) have for this use case. A detailed description of the XTS-AES algorithm can be found in IEEE Std 1619-2007.
* The flash encryption key is stored in one or two `BLOCK_KEYN` eFuses and, by default, is protected from further writes or software readout.
* To see the full flash encryption algorithm implemented in Python, refer to the _flash_encryption_operation() function in the `espsecure.py` source code.