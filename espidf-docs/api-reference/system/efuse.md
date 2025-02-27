# eFuse Manager



## Introduction


The eFuse Manager library is designed to structure access to eFuse bits and make using these easy. This library operates eFuse bits by a structure name which is assigned in eFuse table. This sections introduces some concepts used by eFuse Manager.




## Hardware Description


The ESP32-S3 has a number of eFuses which can store system and user parameters. Each eFuse is a one-bit field which can be programmed to 1 after which it cannot be reverted back to 0.
Some of system parameters are using these eFuse bits directly by hardware modules and have special place (for example EFUSE_BLK0).


For more details, see **ESP32-S3 Technical Reference Manual** > **eFuse Controller (eFuse)** [PDF]. Some eFuse bits are available for user applications.


ESP32-S3 has 11 eFuse blocks each of the size of 256 bits (not all bits are available):


* EFUSE_BLK0 is used entirely for system purposes;
* EFUSE_BLK1 is used entirely for system purposes;
* EFUSE_BLK2 is used entirely for system purposes;
* EFUSE_BLK3 (also named EFUSE_BLK_USER_DATA) can be used for user purposes;
* EFUSE_BLK4 (also named EFUSE_BLK_KEY0) can be used as key (for secure_boot or flash_encryption) or for user purposes;
* EFUSE_BLK5 (also named EFUSE_BLK_KEY1) can be used as key (for secure_boot or flash_encryption) or for user purposes;
* EFUSE_BLK6 (also named EFUSE_BLK_KEY2) can be used as key (for secure_boot or flash_encryption) or for user purposes;
* EFUSE_BLK7 (also named EFUSE_BLK_KEY3) can be used as key (for secure_boot or flash_encryption) or for user purposes;
* EFUSE_BLK8 (also named EFUSE_BLK_KEY4) can be used as key (for secure_boot or flash_encryption) or for user purposes;
* EFUSE_BLK9 (also named EFUSE_BLK_KEY5) can be used for any purpose except for flash encryption (due to a HW bug);
* EFUSE_BLK10 (also named EFUSE_BLK_SYS_DATA_PART2) is reserved for system purposes.





Each block is divided into 8 32-bits registers.




## eFuse Manager Component


The component has API functions for reading and writing fields. Access to the fields is carried out through the structures that describe the location of the eFuse bits in the blocks. The component provides the ability to form fields of any length and from any number of individual bits. The description of the fields is made in a CSV file in a table form. To generate from a tabular form (CSV file) in the C-source uses the tool `efuse_table_gen.py`. The tool checks the CSV file for uniqueness of field names and bit intersection, in case of using a custom file from the user's project directory, the utility checks with the common CSV file.


CSV files:


* common (esp_efuse_table.csv) - contains eFuse fields which are used inside the ESP-IDF. C-source generation should be done manually when changing this file (run command `idf.py efuse-common-table`). Note that changes in this file can lead to incorrect operation.
* custom - (optional and can be enabled by CONFIG_EFUSE_CUSTOM_TABLE) contains eFuse fields that are used by the user in their application. C-source generation should be done manually when changing this file and running `idf.py efuse-custom-table`.




## Description CSV File


The CSV file contains a description of the eFuse fields. In the simple case, one field has one line of description.
Table header:



```
# field_name,  efuse_block(EFUSE_BLK0..EFUSE_BLK10), bit_start(0..255),    bit_count(1..256),        comment

```


Individual params in CSV file the following meanings:


field_name



> Name of field. The prefix ESP_EFUSE_ is added to the name, and this field name is available in the code. This name is used to access the fields. The name must be unique for all fields. If the line has an empty name, then this line is combined with the previous field. This allows you to set an arbitrary order of bits in the field, and expand the field as well (see `MAC_FACTORY` field in the common table). The field_name supports structured format using . to show that the field belongs to another field (see `WR_DIS` and `RD_DIS` in the common table).


efuse_block



> Block number. It determines where the eFuse bits are placed for this field. Available EFUSE_BLK0..EFUSE_BLK10.


bit_start



> Start bit number (0..255). The bit_start field can be omitted. In this case, it is set to bit_start + bit_count from the previous record, if it has the same efuse_block. Otherwise (if efuse_block is different, or this is the first entry), an error will be generated.


bit_count



> The number of bits to use in this field (1..-). This parameter cannot be omitted. This field also may be `MAX_BLK_LEN` in this case, the field length has the maximum block length.


comment



> This param is using for comment field, it also move to C-header file. The comment field can be omitted.


If a non-sequential bit order is required to describe a field, then the field description in the following lines should be continued without specifying a name, indicating that it belongs to one field. For example two fields `MAC_FACTORY` and `MAC_FACTORY_CRC`:



```
# Factory MAC address #
#######################
MAC_FACTORY,            EFUSE_BLK0,    72,    8,    Factory MAC addr [0]
,                       EFUSE_BLK0,    64,    8,    Factory MAC addr [1]
,                       EFUSE_BLK0,    56,    8,    Factory MAC addr [2]
,                       EFUSE_BLK0,    48,    8,    Factory MAC addr [3]
,                       EFUSE_BLK0,    40,    8,    Factory MAC addr [4]
,                       EFUSE_BLK0,    32,    8,    Factory MAC addr [5]
MAC_FACTORY_CRC,        EFUSE_BLK0,    80,    8,    CRC8 for factory MAC address

```


This field is available in code as `ESP_EFUSE_MAC_FACTORY` and `ESP_EFUSE_MAC_FACTORY_CRC`.




## Structured eFuse Fields



```
WR_DIS,                           EFUSE_BLK0,   0,    32,     Write protection
WR_DIS.RD_DIS,                    EFUSE_BLK0,   0,    1,      Write protection for RD_DIS
WR_DIS.FIELD_1,                   EFUSE_BLK0,   1,    1,      Write protection for FIELD_1
WR_DIS.FIELD_2,                   EFUSE_BLK0,   2,    4,      Write protection for FIELD_2 (includes B1 and B2)
WR_DIS.FIELD_2.B1,                EFUSE_BLK0,   2,    2,      Write protection for FIELD_2.B1
WR_DIS.FIELD_2.B2,                EFUSE_BLK0,   4,    2,      Write protection for FIELD_2.B2
WR_DIS.FIELD_3,                   EFUSE_BLK0,   5,    1,      Write protection for FIELD_3
WR_DIS.FIELD_3.ALIAS,             EFUSE_BLK0,   5,    1,      Write protection for FIELD_3 (just a alias for WR_DIS.FIELD_3)
WR_DIS.FIELD_4,                   EFUSE_BLK0,   7,    1,      Write protection for FIELD_4

```


The structured eFuse field looks like `WR_DIS.RD_DIS` where the dot points that this field belongs to the parent field - `WR_DIS` and cannot be out of the parent's range.


It is possible to use some levels of structured fields as WR_DIS.FIELD_2.B1 and B2. These fields should not be crossed each other and should be in the range of two fields: `WR_DIS` and `WR_DIS.FIELD_2`.


It is possible to create aliases for fields with the same range, see `WR_DIS.FIELD_3` and `WR_DIS.FIELD_3.ALIAS`.


The ESP-IDF names for structured eFuse fields should be unique. The `efuse_table_gen` tool generates the final names where the dot is replaced by `_`. The names for using in ESP-IDF are ESP_EFUSE_WR_DIS, ESP_EFUSE_WR_DIS_RD_DIS, ESP_EFUSE_WR_DIS_FIELD_2_B1, etc.


The `efuse_table_gen` tool checks that the fields do not overlap each other and must be within the range of a field if there is a violation, then throws the following error:



```
Field at USER_DATA, EFUSE_BLK3, 0, 256  intersected with  SERIAL_NUMBER, EFUSE_BLK3, 0, 32

```


Solution: Describe `SERIAL_NUMBER` to be included in `USER_DATA`. (`USER_DATA.SERIAL_NUMBER`).



```
Field at FIELD, EFUSE_BLK3, 0, 50  out of range  FIELD.MAJOR_NUMBER, EFUSE_BLK3, 60, 32

```


Solution: Change `bit_start` for `FIELD.MAJOR_NUMBER` from 60 to 0, so `MAJOR_NUMBER` is in the `FIELD` range.




## `efuse_table_gen.py` Tool


The tool is designed to generate C-source files from CSV file and validate fields. First of all, the check is carried out on the uniqueness of the names and overlaps of the field bits. If an additional custom file is used, it will be checked with the existing common file (esp_efuse_table.csv). In case of errors, a message will be displayed and the string that caused the error. C-source files contain structures of type esp_efuse_desc_t.


To generate a common files, use the following command `idf.py efuse-common-table` or:



```
cd $IDF_PATH/components/efuse/
./efuse_table_gen.py --idf_target esp32s3 esp32s3/esp_efuse_table.csv

```


After generation in the folder $IDF_PATH/components/efuse/esp32s3 create:


* esp_efuse_table.c file.
* In include folder esp_efuse_table.c file.


To generate a custom files, use the following command `idf.py efuse-custom-table` or:



```
cd $IDF_PATH/components/efuse/
./efuse_table_gen.py --idf_target esp32s3 esp32s3/esp_efuse_table.csv PROJECT_PATH/main/esp_efuse_custom_table.csv

```


After generation in the folder PROJECT_PATH/main create:


* esp_efuse_custom_table.c file.
* In include folder esp_efuse_custom_table.c file.


To use the generated fields, you need to include two files:



```
#include "esp_efuse.h"
#include "esp_efuse_table.h" // or "esp_efuse_custom_table.h"

```




## Supported Coding Scheme


Coding schemes are used to protect against data corruption. ESP32-S3 supports two coding schemes:


* `None`. EFUSE_BLK0 is stored with four backups, meaning each bit is stored four times. This backup scheme is automatically applied by the hardware and is not visible to software. EFUSE_BLK0 can be written many times.
* `RS`. EFUSE_BLK1 - EFUSE_BLK10 use Reed-Solomon coding scheme that supports up to 5 bytes of automatic error correction. Software encodes the 32-byte EFUSE_BLKx using RS (44, 32) to generate a 12-byte check code, and then burn the EFUSE_BLKx and the check code into eFuse at the same time. The eFuse Controller automatically decodes the RS encoding and applies error correction when reading back the eFuse block. Because the RS check codes are generated across the entire 256-bit eFuse block, each block can only be written to one time.


To write some fields into one block, or different blocks in one time, you need to use `the batch writing mode`. Firstly set this mode through `esp_efuse_batch_write_begin()` function then write some fields as usual using the `esp_efuse_write_...` functions. At the end to burn them, call the `esp_efuse_batch_write_commit()` function. It burns prepared data to the eFuse blocks and disables the `batch recording mode`.



Note


If there is already pre-written data in the eFuse block using the `Reed-Solomon` encoding scheme, then it is not possible to write anything extra (even if the required bits are empty) without breaking the previous encoding data. This encoding data will be overwritten with new encoding data and completely destroyed (however, the payload eFuses are not damaged). It can be related to: CUSTOM_MAC, SPI_PAD_CONFIG_HD, SPI_PAD_CONFIG_CS, etc. Please contact Espressif to order the required pre-burnt eFuses.


FOR TESTING ONLY (NOT RECOMMENDED): You can ignore or suppress errors that violate encoding scheme data in order to burn the necessary bits in the eFuse block.





## eFuse API


Access to the fields is via a pointer to the description structure. API functions have some basic operation:


* `esp_efuse_read_field_blob()` - returns an array of read eFuse bits.
* `esp_efuse_read_field_cnt()` - returns the number of bits programmed as "1".
* `esp_efuse_write_field_blob()` - writes an array.
* `esp_efuse_write_field_cnt()` - writes a required count of bits as "1".
* `esp_efuse_get_field_size()` - returns the number of bits by the field name.
* `esp_efuse_read_reg()` - returns value of eFuse register.
* `esp_efuse_write_reg()` - writes value to eFuse register.
* `esp_efuse_get_coding_scheme()` - returns eFuse coding scheme for blocks.
* `esp_efuse_read_block()` - reads key to eFuse block starting at the offset and the required size.
* `esp_efuse_write_block()` - writes key to eFuse block starting at the offset and the required size.
* `esp_efuse_batch_write_begin()` - set the batch mode of writing fields.
* `esp_efuse_batch_write_commit()` - writes all prepared data for batch writing mode and reset the batch writing mode.
* `esp_efuse_batch_write_cancel()` - reset the batch writing mode and prepared data.
* `esp_efuse_get_key_dis_read()` - Returns a read protection for the key block.
* `esp_efuse_set_key_dis_read()` - Sets a read protection for the key block.
* `esp_efuse_get_key_dis_write()` - Returns a write protection for the key block.
* `esp_efuse_set_key_dis_write()` - Sets a write protection for the key block.
* `esp_efuse_get_key_purpose()` - Returns the current purpose set for an eFuse key block.
* `esp_efuse_write_key()` - Programs a block of key data to an eFuse block
* `esp_efuse_write_keys()` - Programs keys to unused eFuse blocks
* `esp_efuse_find_purpose()` - Finds a key block with the particular purpose set.
* `esp_efuse_get_keypurpose_dis_write()` - Returns a write protection of the key purpose field for an eFuse key block (for esp32 always true).
* `esp_efuse_key_block_unused()` - Returns true if the key block is unused, false otherwise.
* `esp_efuse_destroy_block()` - Destroys the data in this eFuse block. There are two things to do (1) if write protection is not set, then the remaining unset bits are burned, (2) set read protection for this block if it is not locked.


For frequently used fields, special functions are made, like this `esp_efuse_get_pkg_ver()`.




## eFuse API for Keys


EFUSE_BLK_KEY0 - EFUSE_BLK_KEY5 are intended to keep up to 6 keys with a length of 256-bits. Each key has an `ESP_EFUSE_KEY_PURPOSE_x` field which defines the purpose of these keys. The purpose field is described in `esp_efuse_purpose_t`.


The purposes like `ESP_EFUSE_KEY_PURPOSE_XTS_AES_...` are used for flash encryption.


The purposes like `ESP_EFUSE_KEY_PURPOSE_SECURE_BOOT_DIGEST...` are used for secure boot.


There are some eFuse APIs useful to work with states of keys.


* `esp_efuse_get_purpose_field()` - Returns a pointer to a key purpose for an eFuse key block.
* `esp_efuse_get_key()` - Returns a pointer to a key block.
* `esp_efuse_set_key_purpose()` - Sets a key purpose for an eFuse key block.
* `esp_efuse_set_keypurpose_dis_write()` - Sets a write protection of the key purpose field for an eFuse key block.
* `esp_efuse_find_unused_key_block()` - Search for an unused key block and return the first one found.
* `esp_efuse_count_unused_key_blocks()` - Returns the number of unused eFuse key blocks in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX


* `esp_efuse_get_digest_revoke()` - Returns the status of the Secure Boot public key digest revocation bit.
* `esp_efuse_set_digest_revoke()` - Sets the Secure Boot public key digest revocation bit.
* `esp_efuse_get_write_protect_of_digest_revoke()` - Returns a write protection of the Secure Boot public key digest revocation bit.
* `esp_efuse_set_write_protect_of_digest_revoke()` - Sets a write protection of the Secure Boot public key digest revocation bit.




## How to Add a New Field


1. Find a free bits for field. Show esp_efuse_table.csv file or run `idf.py show-efuse-table` or the next command:



```
$ ./efuse_table_gen.py esp32s3/esp_efuse_table.csv --info

Max number of bits in BLK 256
Sorted efuse table:
#       field_name                              efuse_block     bit_start       bit_count
1       WR_DIS                                  EFUSE_BLK0         0               32
2       WR_DIS.RD_DIS                           EFUSE_BLK0         0               1
3       WR_DIS.DIS_ICACHE                       EFUSE_BLK0         2               1
4       WR_DIS.DIS_DCACHE                       EFUSE_BLK0         2               1
5       WR_DIS.DIS_DOWNLOAD_ICACHE              EFUSE_BLK0         2               1
6       WR_DIS.DIS_DOWNLOAD_DCACHE              EFUSE_BLK0         2               1
7       WR_DIS.DIS_FORCE_DOWNLOAD               EFUSE_BLK0         2               1
8       WR_DIS.DIS_USB_OTG                      EFUSE_BLK0         2               1
9       WR_DIS.DIS_TWAI                         EFUSE_BLK0         2               1
10      WR_DIS.DIS_APP_CPU                      EFUSE_BLK0         2               1
11      WR_DIS.DIS_PAD_JTAG                     EFUSE_BLK0         2               1
12      WR_DIS.DIS_DOWNLOAD_MANUAL_ENCRYPT      EFUSE_BLK0         2               1
13      WR_DIS.DIS_USB_JTAG                     EFUSE_BLK0         2               1
14      WR_DIS.DIS_USB_SERIAL_JTAG              EFUSE_BLK0         2               1
15      WR_DIS.STRAP_JTAG_SEL                   EFUSE_BLK0         2               1
16      WR_DIS.USB_PHY_SEL                      EFUSE_BLK0         2               1
17      WR_DIS.VDD_SPI_XPD                      EFUSE_BLK0         3               1
18      WR_DIS.VDD_SPI_TIEH                     EFUSE_BLK0         3               1
19      WR_DIS.VDD_SPI_FORCE                    EFUSE_BLK0         3               1
20      WR_DIS.WDT_DELAY_SEL                    EFUSE_BLK0         3               1
21      WR_DIS.SPI_BOOT_CRYPT_CNT               EFUSE_BLK0         4               1
22      WR_DIS.SECURE_BOOT_KEY_REVOKE0          EFUSE_BLK0         5               1
23      WR_DIS.SECURE_BOOT_KEY_REVOKE1          EFUSE_BLK0         6               1
24      WR_DIS.SECURE_BOOT_KEY_REVOKE2          EFUSE_BLK0         7               1
25      WR_DIS.KEY_PURPOSE_0                    EFUSE_BLK0         8               1
26      WR_DIS.KEY_PURPOSE_1                    EFUSE_BLK0         9               1
27      WR_DIS.KEY_PURPOSE_2                    EFUSE_BLK0         10              1
28      WR_DIS.KEY_PURPOSE_3                    EFUSE_BLK0         11              1
29      WR_DIS.KEY_PURPOSE_4                    EFUSE_BLK0         12              1
30      WR_DIS.KEY_PURPOSE_5                    EFUSE_BLK0         13              1
31      WR_DIS.SECURE_BOOT_EN                   EFUSE_BLK0         15              1
32      WR_DIS.SECURE_BOOT_AGGRESSIVE_REVOKE    EFUSE_BLK0         16              1
33      WR_DIS.FLASH_TPUW                       EFUSE_BLK0         18              1
34      WR_DIS.DIS_DOWNLOAD_MODE                EFUSE_BLK0         18              1
35      WR_DIS.DIS_DIRECT_BOOT                  EFUSE_BLK0         18              1
36      WR_DIS.DIS_USB_SERIAL_JTAG_ROM_PRINT    EFUSE_BLK0         18              1
37      WR_DIS.FLASH_ECC_MODE                   EFUSE_BLK0         18              1
38      WR_DIS.DIS_USB_SERIAL_JTAG_DOWNLOAD_MODE EFUSE_BLK0        18              1
39      WR_DIS.ENABLE_SECURITY_DOWNLOAD         EFUSE_BLK0         18              1
40      WR_DIS.UART_PRINT_CONTROL               EFUSE_BLK0         18              1
41      WR_DIS.PIN_POWER_SELECTION              EFUSE_BLK0         18              1
42      WR_DIS.FLASH_TYPE                       EFUSE_BLK0         18              1
43      WR_DIS.FLASH_PAGE_SIZE                  EFUSE_BLK0         18              1
44      WR_DIS.FLASH_ECC_EN                     EFUSE_BLK0         18              1
45      WR_DIS.FORCE_SEND_RESUME                EFUSE_BLK0         18              1
46      WR_DIS.SECURE_VERSION                   EFUSE_BLK0         18              1
47      WR_DIS.DIS_USB_OTG_DOWNLOAD_MODE        EFUSE_BLK0         19              1
48      WR_DIS.DISABLE_WAFER_VERSION_MAJOR      EFUSE_BLK0         19              1
49      WR_DIS.DISABLE_BLK_VERSION_MAJOR        EFUSE_BLK0         19              1
50      WR_DIS.BLK1                             EFUSE_BLK0         20              1
51      WR_DIS.MAC                              EFUSE_BLK0         20              1
52      WR_DIS.SPI_PAD_CONFIG_CLK               EFUSE_BLK0         20              1
53      WR_DIS.SPI_PAD_CONFIG_Q                 EFUSE_BLK0         20              1
54      WR_DIS.SPI_PAD_CONFIG_D                 EFUSE_BLK0         20              1
55      WR_DIS.SPI_PAD_CONFIG_CS                EFUSE_BLK0         20              1
56      WR_DIS.SPI_PAD_CONFIG_HD                EFUSE_BLK0         20              1
57      WR_DIS.SPI_PAD_CONFIG_WP                EFUSE_BLK0         20              1
58      WR_DIS.SPI_PAD_CONFIG_DQS               EFUSE_BLK0         20              1
59      WR_DIS.SPI_PAD_CONFIG_D4                EFUSE_BLK0         20              1
60      WR_DIS.SPI_PAD_CONFIG_D5                EFUSE_BLK0         20              1
61      WR_DIS.SPI_PAD_CONFIG_D6                EFUSE_BLK0         20              1
62      WR_DIS.SPI_PAD_CONFIG_D7                EFUSE_BLK0         20              1
63      WR_DIS.WAFER_VERSION_MINOR_LO           EFUSE_BLK0         20              1
64      WR_DIS.PKG_VERSION                      EFUSE_BLK0         20              1
65      WR_DIS.BLK_VERSION_MINOR                EFUSE_BLK0         20              1
66      WR_DIS.K_RTC_LDO                        EFUSE_BLK0         20              1
67      WR_DIS.K_DIG_LDO                        EFUSE_BLK0         20              1
68      WR_DIS.V_RTC_DBIAS20                    EFUSE_BLK0         20              1
69      WR_DIS.V_DIG_DBIAS20                    EFUSE_BLK0         20              1
70      WR_DIS.DIG_DBIAS_HVT                    EFUSE_BLK0         20              1
71      WR_DIS.WAFER_VERSION_MINOR_HI           EFUSE_BLK0         20              1
72      WR_DIS.WAFER_VERSION_MAJOR              EFUSE_BLK0         20              1
73      WR_DIS.ADC2_CAL_VOL_ATTEN3              EFUSE_BLK0         20              1
74      WR_DIS.SYS_DATA_PART1                   EFUSE_BLK0         21              1
75      WR_DIS.OPTIONAL_UNIQUE_ID               EFUSE_BLK0         21              1
76      WR_DIS.BLK_VERSION_MAJOR                EFUSE_BLK0         21              1
77      WR_DIS.TEMP_CALIB                       EFUSE_BLK0         21              1
78      WR_DIS.OCODE                            EFUSE_BLK0         21              1
79      WR_DIS.ADC1_INIT_CODE_ATTEN0            EFUSE_BLK0         21              1
80      WR_DIS.ADC1_INIT_CODE_ATTEN1            EFUSE_BLK0         21              1
81      WR_DIS.ADC1_INIT_CODE_ATTEN2            EFUSE_BLK0         21              1
82      WR_DIS.ADC1_INIT_CODE_ATTEN3            EFUSE_BLK0         21              1
83      WR_DIS.ADC2_INIT_CODE_ATTEN0            EFUSE_BLK0         21              1
84      WR_DIS.ADC2_INIT_CODE_ATTEN1            EFUSE_BLK0         21              1
85      WR_DIS.ADC2_INIT_CODE_ATTEN2            EFUSE_BLK0         21              1
86      WR_DIS.ADC2_INIT_CODE_ATTEN3            EFUSE_BLK0         21              1
87      WR_DIS.ADC1_CAL_VOL_ATTEN0              EFUSE_BLK0         21              1
88      WR_DIS.ADC1_CAL_VOL_ATTEN1              EFUSE_BLK0         21              1
89      WR_DIS.ADC1_CAL_VOL_ATTEN2              EFUSE_BLK0         21              1
90      WR_DIS.ADC1_CAL_VOL_ATTEN3              EFUSE_BLK0         21              1
91      WR_DIS.ADC2_CAL_VOL_ATTEN0              EFUSE_BLK0         21              1
92      WR_DIS.ADC2_CAL_VOL_ATTEN1              EFUSE_BLK0         21              1
93      WR_DIS.ADC2_CAL_VOL_ATTEN2              EFUSE_BLK0         21              1
94      WR_DIS.BLOCK_USR_DATA                   EFUSE_BLK0         22              1
95      WR_DIS.CUSTOM_MAC                       EFUSE_BLK0         22              1
96      WR_DIS.BLOCK_KEY0                       EFUSE_BLK0         23              1
97      WR_DIS.BLOCK_KEY1                       EFUSE_BLK0         24              1
98      WR_DIS.BLOCK_KEY2                       EFUSE_BLK0         25              1
99      WR_DIS.BLOCK_KEY3                       EFUSE_BLK0         26              1
100     WR_DIS.BLOCK_KEY4                       EFUSE_BLK0         27              1
101     WR_DIS.BLOCK_KEY5                       EFUSE_BLK0         28              1
102     WR_DIS.BLOCK_SYS_DATA2                  EFUSE_BLK0         29              1
103     WR_DIS.USB_EXCHG_PINS                   EFUSE_BLK0         30              1
104     WR_DIS.USB_EXT_PHY_ENABLE               EFUSE_BLK0         30              1
105     WR_DIS.SOFT_DIS_JTAG                    EFUSE_BLK0         31              1
106     RD_DIS                                  EFUSE_BLK0         32              7
107     RD_DIS.BLOCK_KEY0                       EFUSE_BLK0         32              1
108     RD_DIS.BLOCK_KEY1                       EFUSE_BLK0         33              1
109     RD_DIS.BLOCK_KEY2                       EFUSE_BLK0         34              1
110     RD_DIS.BLOCK_KEY3                       EFUSE_BLK0         35              1
111     RD_DIS.BLOCK_KEY4                       EFUSE_BLK0         36              1
112     RD_DIS.BLOCK_KEY5                       EFUSE_BLK0         37              1
113     RD_DIS.BLOCK_SYS_DATA2                  EFUSE_BLK0         38              1
114     DIS_ICACHE                              EFUSE_BLK0         40              1
115     DIS_DCACHE                              EFUSE_BLK0         41              1
116     DIS_DOWNLOAD_ICACHE                     EFUSE_BLK0         42              1
117     DIS_DOWNLOAD_DCACHE                     EFUSE_BLK0         43              1
118     DIS_FORCE_DOWNLOAD                      EFUSE_BLK0         44              1
119     DIS_USB_OTG                             EFUSE_BLK0         45              1
120     DIS_TWAI                                EFUSE_BLK0         46              1
121     DIS_APP_CPU                             EFUSE_BLK0         47              1
122     SOFT_DIS_JTAG                           EFUSE_BLK0         48              3
123     DIS_PAD_JTAG                            EFUSE_BLK0         51              1
124     DIS_DOWNLOAD_MANUAL_ENCRYPT             EFUSE_BLK0         52              1
125     USB_EXCHG_PINS                          EFUSE_BLK0         57              1
126     USB_EXT_PHY_ENABLE                      EFUSE_BLK0         58              1
127     VDD_SPI_XPD                             EFUSE_BLK0         68              1
128     VDD_SPI_TIEH                            EFUSE_BLK0         69              1
129     VDD_SPI_FORCE                           EFUSE_BLK0         70              1
130     WDT_DELAY_SEL                           EFUSE_BLK0         80              2
131     SPI_BOOT_CRYPT_CNT                      EFUSE_BLK0         82              3
132     SECURE_BOOT_KEY_REVOKE0                 EFUSE_BLK0         85              1
133     SECURE_BOOT_KEY_REVOKE1                 EFUSE_BLK0         86              1
134     SECURE_BOOT_KEY_REVOKE2                 EFUSE_BLK0         87              1
135     KEY_PURPOSE_0                           EFUSE_BLK0         88              4
136     KEY_PURPOSE_1                           EFUSE_BLK0         92              4
137     KEY_PURPOSE_2                           EFUSE_BLK0         96              4
138     KEY_PURPOSE_3                           EFUSE_BLK0        100              4
139     KEY_PURPOSE_4                           EFUSE_BLK0        104              4
140     KEY_PURPOSE_5                           EFUSE_BLK0        108              4
141     SECURE_BOOT_EN                          EFUSE_BLK0        116              1
142     SECURE_BOOT_AGGRESSIVE_REVOKE           EFUSE_BLK0        117              1
143     DIS_USB_JTAG                            EFUSE_BLK0        118              1
144     DIS_USB_SERIAL_JTAG                     EFUSE_BLK0        119              1
145     STRAP_JTAG_SEL                          EFUSE_BLK0        120              1
146     USB_PHY_SEL                             EFUSE_BLK0        121              1
147     FLASH_TPUW                              EFUSE_BLK0        124              4
148     DIS_DOWNLOAD_MODE                       EFUSE_BLK0        128              1
149     DIS_DIRECT_BOOT                         EFUSE_BLK0        129              1
150     DIS_USB_SERIAL_JTAG_ROM_PRINT           EFUSE_BLK0        130              1
151     FLASH_ECC_MODE                          EFUSE_BLK0        131              1
152     DIS_USB_SERIAL_JTAG_DOWNLOAD_MODE       EFUSE_BLK0        132              1
153     ENABLE_SECURITY_DOWNLOAD                EFUSE_BLK0        133              1
154     UART_PRINT_CONTROL                      EFUSE_BLK0        134              2
155     PIN_POWER_SELECTION                     EFUSE_BLK0        136              1
156     FLASH_TYPE                              EFUSE_BLK0        137              1
157     FLASH_PAGE_SIZE                         EFUSE_BLK0        138              2
158     FLASH_ECC_EN                            EFUSE_BLK0        140              1
159     FORCE_SEND_RESUME                       EFUSE_BLK0        141              1
160     SECURE_VERSION                          EFUSE_BLK0        142              16
161     DIS_USB_OTG_DOWNLOAD_MODE               EFUSE_BLK0        159              1
162     DISABLE_WAFER_VERSION_MAJOR             EFUSE_BLK0        160              1
163     DISABLE_BLK_VERSION_MAJOR               EFUSE_BLK0        161              1
164     MAC                                     EFUSE_BLK1         0               8
165     MAC                                     EFUSE_BLK1         8               8
166     MAC                                     EFUSE_BLK1         16              8
167     MAC                                     EFUSE_BLK1         24              8
168     MAC                                     EFUSE_BLK1         32              8
169     MAC                                     EFUSE_BLK1         40              8
170     SPI_PAD_CONFIG_CLK                      EFUSE_BLK1         48              6
171     SPI_PAD_CONFIG_Q                        EFUSE_BLK1         54              6
172     SPI_PAD_CONFIG_D                        EFUSE_BLK1         60              6
173     SPI_PAD_CONFIG_CS                       EFUSE_BLK1         66              6
174     SPI_PAD_CONFIG_HD                       EFUSE_BLK1         72              6
175     SPI_PAD_CONFIG_WP                       EFUSE_BLK1         78              6
176     SPI_PAD_CONFIG_DQS                      EFUSE_BLK1         84              6
177     SPI_PAD_CONFIG_D4                       EFUSE_BLK1         90              6
178     SPI_PAD_CONFIG_D5                       EFUSE_BLK1         96              6
179     SPI_PAD_CONFIG_D6                       EFUSE_BLK1        102              6
180     SPI_PAD_CONFIG_D7                       EFUSE_BLK1        108              6
181     WAFER_VERSION_MINOR_LO                  EFUSE_BLK1        114              3
182     PKG_VERSION                             EFUSE_BLK1        117              3
183     BLK_VERSION_MINOR                       EFUSE_BLK1        120              3
184     K_RTC_LDO                               EFUSE_BLK1        141              7
185     K_DIG_LDO                               EFUSE_BLK1        148              7
186     V_RTC_DBIAS20                           EFUSE_BLK1        155              8
187     V_DIG_DBIAS20                           EFUSE_BLK1        163              8
188     DIG_DBIAS_HVT                           EFUSE_BLK1        171              5
189     WAFER_VERSION_MINOR_HI                  EFUSE_BLK1        183              1
190     WAFER_VERSION_MAJOR                     EFUSE_BLK1        184              2
191     ADC2_CAL_VOL_ATTEN3                     EFUSE_BLK1        186              6
192     SYS_DATA_PART2                          EFUSE_BLK10        0              256
193     OPTIONAL_UNIQUE_ID                      EFUSE_BLK2         0              128
194     BLK_VERSION_MAJOR                       EFUSE_BLK2        128              2
195     TEMP_CALIB                              EFUSE_BLK2        132              9
196     OCODE                                   EFUSE_BLK2        141              8
197     ADC1_INIT_CODE_ATTEN0                   EFUSE_BLK2        149              8
198     ADC1_INIT_CODE_ATTEN1                   EFUSE_BLK2        157              6
199     ADC1_INIT_CODE_ATTEN2                   EFUSE_BLK2        163              6
200     ADC1_INIT_CODE_ATTEN3                   EFUSE_BLK2        169              6
201     ADC2_INIT_CODE_ATTEN0                   EFUSE_BLK2        175              8
202     ADC2_INIT_CODE_ATTEN1                   EFUSE_BLK2        183              6
203     ADC2_INIT_CODE_ATTEN2                   EFUSE_BLK2        189              6
204     ADC2_INIT_CODE_ATTEN3                   EFUSE_BLK2        195              6
205     ADC1_CAL_VOL_ATTEN0                     EFUSE_BLK2        201              8
206     ADC1_CAL_VOL_ATTEN1                     EFUSE_BLK2        209              8
207     ADC1_CAL_VOL_ATTEN2                     EFUSE_BLK2        217              8
208     ADC1_CAL_VOL_ATTEN3                     EFUSE_BLK2        225              8
209     ADC2_CAL_VOL_ATTEN0                     EFUSE_BLK2        233              8
210     ADC2_CAL_VOL_ATTEN1                     EFUSE_BLK2        241              7
211     ADC2_CAL_VOL_ATTEN2                     EFUSE_BLK2        248              7
212     USER_DATA                               EFUSE_BLK3         0              256
213     USER_DATA.MAC_CUSTOM                    EFUSE_BLK3        200              48
214     KEY0                                    EFUSE_BLK4         0              256
215     KEY1                                    EFUSE_BLK5         0              256
216     KEY2                                    EFUSE_BLK6         0              256
217     KEY3                                    EFUSE_BLK7         0              256
218     KEY4                                    EFUSE_BLK8         0              256
219     KEY5                                    EFUSE_BLK9         0              256

Used bits in efuse table:
EFUSE_BLK0
[0 31] [0 0] [2 2] ... [22 30] [30 38] [32 38] [40 52] [57 58] [68 70] [80 111] [116 121] [124 157] [159 161]
EFUSE_BLK1
[0 122] [141 175] [183 191]
EFUSE_BLK10
[0 255]
EFUSE_BLK2
[0 129] [132 254]
EFUSE_BLK3
[0 255] [200 247]
EFUSE_BLK4
[0 255]
EFUSE_BLK5
[0 255]
EFUSE_BLK6
[0 255]
EFUSE_BLK7
[0 255]
EFUSE_BLK8
[0 255]
EFUSE_BLK9
[0 255]
Note: Not printed ranges are free for using. (bits in EFUSE_BLK0 are reserved for Espressif)

```


The number of bits not included in square brackets is free (some bits are reserved for Espressif). All fields are checked for overlapping.


To add fields to an existing field, use the Structured efuse fields technique. For example, adding the fields: SERIAL_NUMBER, MODEL_NUMBER and HARDWARE REV to an existing `USER_DATA` field. Use `.` (dot) to show an attachment in a field.



```
USER_DATA.SERIAL_NUMBER,                  EFUSE_BLK3,    0,  32,
USER_DATA.MODEL_NUMBER,                   EFUSE_BLK3,    32, 10,
USER_DATA.HARDWARE_REV,                   EFUSE_BLK3,    42, 10,

```


2. Fill a line for field: field_name, efuse_block, bit_start, bit_count, comment.
3. Run a `show_efuse_table` command to check eFuse table. To generate source files run `efuse_common_table` or `efuse_custom_table` command.


You may get errors such as `intersects with` or `out of range`. Please see how to solve them in the Structured efuse fields article.




## Bit Order


The eFuses bit order is little endian (see the example below), it means that eFuse bits are read and written from LSB to MSB:



```
$ espefuse.py dump

USER_DATA      (BLOCK3          ) [3 ] read_regs: 03020100 07060504 0B0A0908 0F0E0D0C 13121111 17161514 1B1A1918 1F1E1D1C
BLOCK4         (BLOCK4          ) [4 ] read_regs: 03020100 07060504 0B0A0908 0F0E0D0C 13121111 17161514 1B1A1918 1F1E1D1C

where is the register representation:

EFUSE_RD_USR_DATA0_REG = 0x03020100
EFUSE_RD_USR_DATA1_REG = 0x07060504
EFUSE_RD_USR_DATA2_REG = 0x0B0A0908
EFUSE_RD_USR_DATA3_REG = 0x0F0E0D0C
EFUSE_RD_USR_DATA4_REG = 0x13121111
EFUSE_RD_USR_DATA5_REG = 0x17161514
EFUSE_RD_USR_DATA6_REG = 0x1B1A1918
EFUSE_RD_USR_DATA7_REG = 0x1F1E1D1C

where is the byte representation:

byte[0] = 0x00, byte[1] = 0x01, ... byte[3] = 0x03, byte[4] = 0x04, ..., byte[31] = 0x1F

```


For example, csv file describes the `USER_DATA` field, which occupies all 256 bits (a whole block).



```
USER_DATA,          EFUSE_BLK3,    0,  256,     User data
USER_DATA.FIELD1,   EFUSE_BLK3,    16,  16,     Field1

ID,                 EFUSE_BLK4,    8,  3,      ID bit[0..2]
,                   EFUSE_BLK4,    16, 2,      ID bit[3..4]
,                   EFUSE_BLK4,    32, 3,      ID bit[5..7]

```


Thus, reading the eFuse `USER_DATA` block written as above gives the following results:



```
uint8_t buf[32] = { 0 };
esp_efuse_read_field_blob(ESP_EFUSE_USER_DATA, &buf, sizeof(buf) * 8);
// buf[0] = 0x00, buf[1] = 0x01, ... buf[31] = 0x1F

uint32_t field1 = 0;
size_t field1_size = ESP_EFUSE_USER_DATA[0]->bit_count; // can be used for this case because it only consists of one entry
esp_efuse_read_field_blob(ESP_EFUSE_USER_DATA, &field1, field1_size);
// field1 = 0x0302

uint32_t field1_1 = 0;
esp_efuse_read_field_blob(ESP_EFUSE_USER_DATA, &field1_1, 2); // reads only first 2 bits
// field1 = 0x0002

uint8_t id = 0;
size_t id_size = esp_efuse_get_field_size(ESP_EFUSE_ID); // returns 6
// size_t id_size = ESP_EFUSE_USER_DATA[0]->bit_count; // cannot be used because it consists of 3 entries. It returns 3 not 6.
esp_efuse_read_field_blob(ESP_EFUSE_ID, &id, id_size);
// id = 0x91
// b'100 10  001
//   [3] [2] [3]

uint8_t id_1 = 0;
esp_efuse_read_field_blob(ESP_EFUSE_ID, &id_1, 3);
// id = 0x01
// b'001

```




## Get eFuses During Build


There is a way to get the state of eFuses at the build stage of the project. There are two cmake functions for this:


* `espefuse_get_json_summary()` - It calls the `espefuse.py summary --format json` command and returns a json string (it is not stored in a file).
* `espefuse_get_efuse()` - It finds a given eFuse name in the json string and returns its property.


The json string has the following properties:



```
{
    "MAC": {
        "bit_len": 48,
        "block": 0,
        "category": "identity",
        "description": "Factory MAC Address",
        "efuse_type": "bytes:6",
        "name": "MAC",
        "pos": 0,
        "readable": true,
        "value": "94:b9:7e:5a:6e:58 (CRC 0xe2 OK)",
        "word": 1,
        "writeable": true
    },
}

```


These functions can be used from a top-level project `CMakeLists.txt` (get-started/hello_world/CMakeLists.txt):



```
# ...
project(hello_world)

espefuse_get_json_summary(efuse_json)
espefuse_get_efuse(ret_data ${efuse_json} "MAC" "value")
message("MAC:" ${ret_data})

```


The format of the `value` property is the same as shown in `espefuse.py summary`.



```
MAC:94:b9:7e:5a:6e:58 (CRC 0xe2 OK)

```


There is an example test system/efuse/CMakeLists.txt which adds a custom target `efuse-summary`. This allows you to run the `idf.py efuse-summary` command to read the required eFuses (specified in the `efuse_names` list) at any time, not just at project build time.




## Debug eFuse & Unit Tests



### Virtual eFuses


The Kconfig option CONFIG_EFUSE_VIRTUAL virtualizes eFuse values inside the eFuse Manager, so writes are emulated and no eFuse values are permanently changed. This can be useful for debugging app and unit tests.
During startup, the eFuses are copied to RAM. All eFuse operations (read and write) are performed with RAM instead of the real eFuse registers.


In addition to the CONFIG_EFUSE_VIRTUAL option there is CONFIG_EFUSE_VIRTUAL_KEEP_IN_FLASH option that adds a feature to keep eFuses in flash memory. To use this mode the partition_table should have the efuse partition. partition.csv: `"efuse_em, data, efuse,   ,   0x2000,"`.
During startup, the eFuses are copied from flash or, in case if flash is empty, from real eFuse to RAM and then update flash. This option allows keeping eFuses after reboots (possible to test secure_boot and flash_encryption features with this option).



#### Flash Encryption Testing


Flash Encryption (FE) is a hardware feature that requires the physical burning of eFuses: key and FLASH_CRYPT_CNT. If FE is not actually enabled then enabling the CONFIG_EFUSE_VIRTUAL_KEEP_IN_FLASH option just gives testing possibilities and does not encrypt anything in the flash, even though the logs say encryption happens. The `bootloader_flash_write()` is adapted for this purpose. But if FE is already enabled on the chip and you run an application or bootloader created with the CONFIG_EFUSE_VIRTUAL_KEEP_IN_FLASH option then the flash encryption/decryption operations will work properly (data are encrypted as it is written into an encrypted flash partition and decrypted when they are read from an encrypted partition).





### `espefuse.py`


esptool includes a useful tool for reading/writing ESP32-S3 eFuse bits - espefuse.py.



```
espefuse.py -p PORT summary

espefuse.py v4.6-dev
Connecting....
Detecting chip type... ESP32-S3

=== Run "summary" command ===
EFUSE_NAME (Block) Description  = [Meaningful Value] [Readable/Writeable] (Hex Value)
----------------------------------------------------------------------------------------
Calibration fuses:
K_RTC_LDO (BLOCK1)                                 BLOCK1 K_RTC_LDO                                   = 12 R/W (0b0000011)
K_DIG_LDO (BLOCK1)                                 BLOCK1 K_DIG_LDO                                   = -28 R/W (0b1000111)
V_RTC_DBIAS20 (BLOCK1)                             BLOCK1 voltage of rtc dbias20                      = 20 R/W (0x05)
V_DIG_DBIAS20 (BLOCK1)                             BLOCK1 voltage of digital dbias20                  = -44 R/W (0x8b)
DIG_DBIAS_HVT (BLOCK1)                             BLOCK1 digital dbias when hvt                      = -36 R/W (0b11001)
ADC2_CAL_VOL_ATTEN3 (BLOCK1)                       ADC2 calibration voltage at atten3                 = -24 R/W (0b100110)
TEMP_CALIB (BLOCK2)                                Temperature calibration data                       = -10.9 R/W (0b101101101)
OCODE (BLOCK2)                                     ADC OCode                                          = 88 R/W (0x58)
ADC1_INIT_CODE_ATTEN0 (BLOCK2)                     ADC1 init code at atten0                           = 432 R/W (0x6c)
ADC1_INIT_CODE_ATTEN1 (BLOCK2)                     ADC1 init code at atten1                           = -16 R/W (0b100100)
ADC1_INIT_CODE_ATTEN2 (BLOCK2)                     ADC1 init code at atten2                           = 88 R/W (0b010110)
ADC1_INIT_CODE_ATTEN3 (BLOCK2)                     ADC1 init code at atten3                           = 0 R/W (0b100000)
ADC2_INIT_CODE_ATTEN0 (BLOCK2)                     ADC2 init code at atten0                           = -72 R/W (0x92)
ADC2_INIT_CODE_ATTEN1 (BLOCK2)                     ADC2 init code at atten1                           = -16 R/W (0b100100)
ADC2_INIT_CODE_ATTEN2 (BLOCK2)                     ADC2 init code at atten2                           = 48 R/W (0b001100)
ADC2_INIT_CODE_ATTEN3 (BLOCK2)                     ADC2 init code at atten3                           = 112 R/W (0b011100)
ADC1_CAL_VOL_ATTEN0 (BLOCK2)                       ADC1 calibration voltage at atten0                 = 412 R/W (0x67)
ADC1_CAL_VOL_ATTEN1 (BLOCK2)                       ADC1 calibration voltage at atten1                 = 392 R/W (0x62)
ADC1_CAL_VOL_ATTEN2 (BLOCK2)                       ADC1 calibration voltage at atten2                 = 356 R/W (0x59)
ADC1_CAL_VOL_ATTEN3 (BLOCK2)                       ADC1 calibration voltage at atten3                 = 412 R/W (0x67)
ADC2_CAL_VOL_ATTEN0 (BLOCK2)                       ADC2 calibration voltage at atten0                 = -116 R/W (0x9d)
ADC2_CAL_VOL_ATTEN1 (BLOCK2)                       ADC2 calibration voltage at atten1                 = -72 R/W (0b1010010)
ADC2_CAL_VOL_ATTEN2 (BLOCK2)                       ADC2 calibration voltage at atten2                 = -64 R/W (0b1010000)

Config fuses:
WR_DIS (BLOCK0)                                    Disable programming of individual eFuses           = 0 R/W (0x00000000)
RD_DIS (BLOCK0)                                    Disable reading from BlOCK4-10                     = 0 R/W (0b0000000)
DIS_ICACHE (BLOCK0)                                Set this bit to disable Icache                     = False R/W (0b0)
DIS_DCACHE (BLOCK0)                                Set this bit to disable Dcache                     = False R/W (0b0)
DIS_TWAI (BLOCK0)                                  Set this bit to disable CAN function               = False R/W (0b0)
DIS_APP_CPU (BLOCK0)                               Disable app cpu                                    = False R/W (0b0)
DIS_DIRECT_BOOT (BLOCK0)                           Disable direct boot mode                           = False R/W (0b0)
UART_PRINT_CONTROL (BLOCK0)                        Set the default UART boot message output mode      = Enable R/W (0b00)
PIN_POWER_SELECTION (BLOCK0)                       Set default power supply for GPIO33-GPIO37; set wh = VDD3P3_CPU R/W (0b0)
                                                   en SPI flash is initialized
BLOCK_USR_DATA (BLOCK3)                            User data
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W
BLOCK_SYS_DATA2 (BLOCK10)                          System data part 2 (reserved)
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W

Flash fuses:
FLASH_TPUW (BLOCK0)                                Configures flash waiting time after power-up; in u = 0 R/W (0x0)
                                                   nit of ms. If the value is less than 15; the waiti
                                                   ng time is the configurable value.  Otherwise; the
                                                    waiting time is twice the configurable value
FLASH_ECC_MODE (BLOCK0)                            Flash ECC mode in ROM                              = 16to18 byte R/W (0b0)
FLASH_TYPE (BLOCK0)                                SPI flash type                                     = 4 data lines R/W (0b0)
FLASH_PAGE_SIZE (BLOCK0)                           Set Flash page size                                = 0 R/W (0b00)
FLASH_ECC_EN (BLOCK0)                              Set 1 to enable ECC for flash boot                 = False R/W (0b0)
FORCE_SEND_RESUME (BLOCK0)                         Set this bit to force ROM code to send a resume co = False R/W (0b0)
                                                   mmand during SPI boot

Identity fuses:
DISABLE_WAFER_VERSION_MAJOR (BLOCK0)               Disables check of wafer version major              = False R/W (0b0)
DISABLE_BLK_VERSION_MAJOR (BLOCK0)                 Disables check of blk version major                = False R/W (0b0)
WAFER_VERSION_MINOR_LO (BLOCK1)                    WAFER_VERSION_MINOR least significant bits         = 3 R/W (0b011)
PKG_VERSION (BLOCK1)                               Package version                                    = 0 R/W (0b000)
BLK_VERSION_MINOR (BLOCK1)                         BLK_VERSION_MINOR                                  = 3 R/W (0b011)
WAFER_VERSION_MINOR_HI (BLOCK1)                    WAFER_VERSION_MINOR most significant bit           = False R/W (0b0)
WAFER_VERSION_MAJOR (BLOCK1)                       WAFER_VERSION_MAJOR                                = 0 R/W (0b00)
OPTIONAL_UNIQUE_ID (BLOCK2)                        Optional unique 128-bit ID
   = cb 3a c9 b8 88 2b c3 bc 5e f4 00 60 ac 25 be 4b R/W
BLK_VERSION_MAJOR (BLOCK2)                         BLK_VERSION_MAJOR of BLOCK2                        = ADC calib V1 R/W (0b01)
WAFER_VERSION_MINOR (BLOCK0)                       calc WAFER VERSION MINOR = WAFER_VERSION_MINOR_HI  = 3 R/W (0x3)
                                                   << 3 + WAFER_VERSION_MINOR_LO (read only)

Jtag fuses:
SOFT_DIS_JTAG (BLOCK0)                             Set these bits to disable JTAG in the soft way (od = 0 R/W (0b000)
                                                   d number 1 means disable ). JTAG can be enabled in
                                                    HMAC module
DIS_PAD_JTAG (BLOCK0)                              Set this bit to disable JTAG in the hard way. JTAG = False R/W (0b0)
                                                    is disabled permanently
STRAP_JTAG_SEL (BLOCK0)                            Set this bit to enable selection between usb_to_jt = False R/W (0b0)
                                                   ag and pad_to_jtag through strapping gpio10 when b
                                                   oth reg_dis_usb_jtag and reg_dis_pad_jtag are equa
                                                   l to 0

Mac fuses:
MAC (BLOCK1)                                       MAC address
   = ec:da:3b:41:f2:70 (OK) R/W
CUSTOM_MAC (BLOCK3)                                Custom MAC
   = 00:00:00:00:00:00 (OK) R/W

Security fuses:
DIS_DOWNLOAD_ICACHE (BLOCK0)                       Set this bit to disable Icache in download mode (b = False R/W (0b0)
                                                   oot_mode[3:0] is 0; 1; 2; 3; 6; 7)
DIS_DOWNLOAD_DCACHE (BLOCK0)                       Set this bit to disable Dcache in download mode (  = False R/W (0b0)
                                                   boot_mode[3:0] is 0; 1; 2; 3; 6; 7)
DIS_FORCE_DOWNLOAD (BLOCK0)                        Set this bit to disable the function that forces c = False R/W (0b0)
                                                   hip into download mode
DIS_DOWNLOAD_MANUAL_ENCRYPT (BLOCK0)               Set this bit to disable flash encryption when in d = False R/W (0b0)
                                                   ownload boot modes
SPI_BOOT_CRYPT_CNT (BLOCK0)                        Enables flash encryption when 1 or 3 bits are set  = Disable R/W (0b000)
                                                   and disabled otherwise
SECURE_BOOT_KEY_REVOKE0 (BLOCK0)                   Revoke 1st secure boot key                         = False R/W (0b0)
SECURE_BOOT_KEY_REVOKE1 (BLOCK0)                   Revoke 2nd secure boot key                         = False R/W (0b0)
SECURE_BOOT_KEY_REVOKE2 (BLOCK0)                   Revoke 3rd secure boot key                         = False R/W (0b0)
KEY_PURPOSE_0 (BLOCK0)                             Purpose of Key0                                    = USER R/W (0x0)
KEY_PURPOSE_1 (BLOCK0)                             Purpose of Key1                                    = USER R/W (0x0)
KEY_PURPOSE_2 (BLOCK0)                             Purpose of Key2                                    = USER R/W (0x0)
KEY_PURPOSE_3 (BLOCK0)                             Purpose of Key3                                    = USER R/W (0x0)
KEY_PURPOSE_4 (BLOCK0)                             Purpose of Key4                                    = USER R/W (0x0)
KEY_PURPOSE_5 (BLOCK0)                             Purpose of Key5                                    = USER R/W (0x0)
SECURE_BOOT_EN (BLOCK0)                            Set this bit to enable secure boot                 = False R/W (0b0)
SECURE_BOOT_AGGRESSIVE_REVOKE (BLOCK0)             Set this bit to enable revoking aggressive secure  = False R/W (0b0)
                                                   boot
DIS_DOWNLOAD_MODE (BLOCK0)                         Set this bit to disable download mode (boot_mode[3 = False R/W (0b0)
                                                   :0] = 0; 1; 2; 3; 6; 7)
ENABLE_SECURITY_DOWNLOAD (BLOCK0)                  Set this bit to enable secure UART download mode   = False R/W (0b0)
SECURE_VERSION (BLOCK0)                            Secure version (used by ESP-IDF anti-rollback feat = 0 R/W (0x0000)
                                                   ure)
BLOCK_KEY0 (BLOCK4)
  Purpose: USER
               Key0 or user data
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W
BLOCK_KEY1 (BLOCK5)
  Purpose: USER
               Key1 or user data
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W
BLOCK_KEY2 (BLOCK6)
  Purpose: USER
               Key2 or user data
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W
BLOCK_KEY3 (BLOCK7)
  Purpose: USER
               Key3 or user data
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W
BLOCK_KEY4 (BLOCK8)
  Purpose: USER
               Key4 or user data
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W
BLOCK_KEY5 (BLOCK9)
  Purpose: USER
               Key5 or user data
   = 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 R/W

Spi Pad fuses:
SPI_PAD_CONFIG_CLK (BLOCK1)                        SPI_PAD_configure CLK                              = 0 R/W (0b000000)
SPI_PAD_CONFIG_Q (BLOCK1)                          SPI_PAD_configure Q(D1)                            = 0 R/W (0b000000)
SPI_PAD_CONFIG_D (BLOCK1)                          SPI_PAD_configure D(D0)                            = 0 R/W (0b000000)
SPI_PAD_CONFIG_CS (BLOCK1)                         SPI_PAD_configure CS                               = 0 R/W (0b000000)
SPI_PAD_CONFIG_HD (BLOCK1)                         SPI_PAD_configure HD(D3)                           = 0 R/W (0b000000)
SPI_PAD_CONFIG_WP (BLOCK1)                         SPI_PAD_configure WP(D2)                           = 0 R/W (0b000000)
SPI_PAD_CONFIG_DQS (BLOCK1)                        SPI_PAD_configure DQS                              = 0 R/W (0b000000)
SPI_PAD_CONFIG_D4 (BLOCK1)                         SPI_PAD_configure D4                               = 0 R/W (0b000000)
SPI_PAD_CONFIG_D5 (BLOCK1)                         SPI_PAD_configure D5                               = 0 R/W (0b000000)
SPI_PAD_CONFIG_D6 (BLOCK1)                         SPI_PAD_configure D6                               = 0 R/W (0b000000)
SPI_PAD_CONFIG_D7 (BLOCK1)                         SPI_PAD_configure D7                               = 0 R/W (0b000000)

Usb fuses:
DIS_USB_OTG (BLOCK0)                               Set this bit to disable USB function               = False R/W (0b0)
USB_EXCHG_PINS (BLOCK0)                            Set this bit to exchange USB D+ and D- pins        = False R/W (0b0)
USB_EXT_PHY_ENABLE (BLOCK0)                        Set this bit to enable external PHY                = False R/W (0b0)
DIS_USB_JTAG (BLOCK0)                              Set this bit to disable function of usb switch to  = False R/W (0b0)
                                                   jtag in module of usb device
DIS_USB_SERIAL_JTAG (BLOCK0)                       Set this bit to disable usb device                 = False R/W (0b0)
USB_PHY_SEL (BLOCK0)                               This bit is used to switch internal PHY and extern
   = internal PHY is assigned to USB Device while external PHY is assigned to USB OTG R/W (0b0)
                                                   al PHY for USB OTG and USB Device
DIS_USB_SERIAL_JTAG_ROM_PRINT (BLOCK0)             USB printing                                       = Enable R/W (0b0)
DIS_USB_SERIAL_JTAG_DOWNLOAD_MODE (BLOCK0)         Set this bit to disable UART download mode through = False R/W (0b0)
                                                    USB
DIS_USB_OTG_DOWNLOAD_MODE (BLOCK0)                 Set this bit to disable download through USB-OTG   = False R/W (0b0)

Vdd fuses:
VDD_SPI_XPD (BLOCK0)                               SPI regulator power up signal                      = False R/W (0b0)
VDD_SPI_TIEH (BLOCK0)                              If VDD_SPI_FORCE is 1; determines VDD_SPI voltage
   = VDD_SPI connects to 1.8 V LDO R/W (0b0)
VDD_SPI_FORCE (BLOCK0)                             Set this bit and force to use the configuration of = False R/W (0b0)
                                                    eFuse to configure VDD_SPI

Wdt fuses:
WDT_DELAY_SEL (BLOCK0)                             RTC watchdog timeout threshold; in unit of slow cl = 40000 R/W (0b00)
                                                   ock cycle

Flash voltage (VDD_SPI) determined by GPIO45 on reset (GPIO45=High: VDD_SPI pin is powered from internal 1.8V LDO
GPIO45=Low or NC: VDD_SPI pin is powered directly from VDD3P3_RTC_IO via resistor Rspi. Typically this voltage is 3.3 V).

```


To get a dump for all eFuse registers.



```
espefuse.py -p PORT dump

espefuse.py v4.6-dev
Connecting....
Detecting chip type... ESP32-S3
BLOCK0          (                ) [0 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000
MAC_SPI_8M_0    (BLOCK1          ) [1 ] read_regs: 3b41f270 0000ecda 00000000 030c0000 2c707800 9800cc58
BLOCK_SYS_DATA  (BLOCK2          ) [2 ] read_regs: b8c93acb bcc32b88 6000f45e 4bbe25ac 8d8b16d1 924940b4 b2c4cee1 50a53ace
BLOCK_USR_DATA  (BLOCK3          ) [3 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
BLOCK_KEY0      (BLOCK4          ) [4 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
BLOCK_KEY1      (BLOCK5          ) [5 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
BLOCK_KEY2      (BLOCK6          ) [6 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
BLOCK_KEY3      (BLOCK7          ) [7 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
BLOCK_KEY4      (BLOCK8          ) [8 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
BLOCK_KEY5      (BLOCK9          ) [9 ] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
BLOCK_SYS_DATA2 (BLOCK10         ) [10] read_regs: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000

BLOCK0          (                ) [0 ] err__regs: 00000000 00000000 00000000 00000000 00000000 00000000
EFUSE_RD_RS_ERR0_REG        0x00000000
EFUSE_RD_RS_ERR1_REG        0x00000000

=== Run "dump" command ===

```




### Header File


* components/efuse/esp32s3/include/esp_efuse_chip.h
* This header file can be included with:



> ```
> #include "esp_efuse_chip.h"
> 
> ```
* This header file is a part of the API provided by the `efuse` component. To declare that your component depends on `efuse`, add the following to your CMakeLists.txt:



> ```
> REQUIRES efuse
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES efuse
> 
> ```




### Enumerations


#### esp_efuse_block_t


```


enum esp_efuse_block_t
Type of eFuse blocks ESP32S3. 
Values:
EFUSE_BLK0

enumerator EFUSE_BLK0
Number of eFuse BLOCK0. REPEAT_DATA 

EFUSE_BLK1

enumerator EFUSE_BLK1
Number of eFuse BLOCK1. MAC_SPI_8M_SYS 

EFUSE_BLK2

enumerator EFUSE_BLK2
Number of eFuse BLOCK2. SYS_DATA_PART1 

EFUSE_BLK_SYS_DATA_PART1

enumerator EFUSE_BLK_SYS_DATA_PART1
Number of eFuse BLOCK2. SYS_DATA_PART1 

EFUSE_BLK3

enumerator EFUSE_BLK3
Number of eFuse BLOCK3. USER_DATA 

EFUSE_BLK_USER_DATA

enumerator EFUSE_BLK_USER_DATA
Number of eFuse BLOCK3. USER_DATA 

EFUSE_BLK4

enumerator EFUSE_BLK4
Number of eFuse BLOCK4. KEY0 

EFUSE_BLK_KEY0

enumerator EFUSE_BLK_KEY0
Number of eFuse BLOCK4. KEY0 

EFUSE_BLK5

enumerator EFUSE_BLK5
Number of eFuse BLOCK5. KEY1 

EFUSE_BLK_KEY1

enumerator EFUSE_BLK_KEY1
Number of eFuse BLOCK5. KEY1 

EFUSE_BLK6

enumerator EFUSE_BLK6
Number of eFuse BLOCK6. KEY2 

EFUSE_BLK_KEY2

enumerator EFUSE_BLK_KEY2
Number of eFuse BLOCK6. KEY2 

EFUSE_BLK7

enumerator EFUSE_BLK7
Number of eFuse BLOCK7. KEY3 

EFUSE_BLK_KEY3

enumerator EFUSE_BLK_KEY3
Number of eFuse BLOCK7. KEY3 

EFUSE_BLK8

enumerator EFUSE_BLK8
Number of eFuse BLOCK8. KEY4 

EFUSE_BLK_KEY4

enumerator EFUSE_BLK_KEY4
Number of eFuse BLOCK8. KEY4 

EFUSE_BLK9

enumerator EFUSE_BLK9
Number of eFuse BLOCK9. KEY5 

EFUSE_BLK_KEY5

enumerator EFUSE_BLK_KEY5
Number of eFuse BLOCK9. KEY5 

EFUSE_BLK_KEY_MAX

enumerator EFUSE_BLK_KEY_MAX

EFUSE_BLK10

enumerator EFUSE_BLK10
Number of eFuse BLOCK10. SYS_DATA_PART2 

EFUSE_BLK_SYS_DATA_PART2

enumerator EFUSE_BLK_SYS_DATA_PART2
Number of eFuse BLOCK10. SYS_DATA_PART2 

EFUSE_BLK_MAX

enumerator EFUSE_BLK_MAX


```

#### esp_efuse_coding_scheme_t


```


enum esp_efuse_coding_scheme_t
Type of coding scheme. 
Values:
EFUSE_CODING_SCHEME_NONE

enumerator EFUSE_CODING_SCHEME_NONE
None 

EFUSE_CODING_SCHEME_RS

enumerator EFUSE_CODING_SCHEME_RS
Reed-Solomon coding 


```

#### esp_efuse_purpose_t


```


enum esp_efuse_purpose_t
Type of key purpose. 
Values:
ESP_EFUSE_KEY_PURPOSE_USER

enumerator ESP_EFUSE_KEY_PURPOSE_USER
User purposes (software-only use) 

ESP_EFUSE_KEY_PURPOSE_RESERVED

enumerator ESP_EFUSE_KEY_PURPOSE_RESERVED
Reserved 

ESP_EFUSE_KEY_PURPOSE_XTS_AES_256_KEY_1

enumerator ESP_EFUSE_KEY_PURPOSE_XTS_AES_256_KEY_1
XTS_AES_256_KEY_1 (flash/PSRAM encryption) 

ESP_EFUSE_KEY_PURPOSE_XTS_AES_256_KEY_2

enumerator ESP_EFUSE_KEY_PURPOSE_XTS_AES_256_KEY_2
XTS_AES_256_KEY_2 (flash/PSRAM encryption) 

ESP_EFUSE_KEY_PURPOSE_XTS_AES_128_KEY

enumerator ESP_EFUSE_KEY_PURPOSE_XTS_AES_128_KEY
XTS_AES_128_KEY (flash/PSRAM encryption) 

ESP_EFUSE_KEY_PURPOSE_HMAC_DOWN_ALL

enumerator ESP_EFUSE_KEY_PURPOSE_HMAC_DOWN_ALL
HMAC Downstream mode 

ESP_EFUSE_KEY_PURPOSE_HMAC_DOWN_JTAG

enumerator ESP_EFUSE_KEY_PURPOSE_HMAC_DOWN_JTAG
JTAG soft enable key (uses HMAC Downstream mode) 

ESP_EFUSE_KEY_PURPOSE_HMAC_DOWN_DIGITAL_SIGNATURE

enumerator ESP_EFUSE_KEY_PURPOSE_HMAC_DOWN_DIGITAL_SIGNATURE
Digital Signature peripheral key (uses HMAC Downstream mode) 

ESP_EFUSE_KEY_PURPOSE_HMAC_UP

enumerator ESP_EFUSE_KEY_PURPOSE_HMAC_UP
HMAC Upstream mode 

ESP_EFUSE_KEY_PURPOSE_SECURE_BOOT_DIGEST0

enumerator ESP_EFUSE_KEY_PURPOSE_SECURE_BOOT_DIGEST0
SECURE_BOOT_DIGEST0 (Secure Boot key digest) 

ESP_EFUSE_KEY_PURPOSE_SECURE_BOOT_DIGEST1

enumerator ESP_EFUSE_KEY_PURPOSE_SECURE_BOOT_DIGEST1
SECURE_BOOT_DIGEST1 (Secure Boot key digest) 

ESP_EFUSE_KEY_PURPOSE_SECURE_BOOT_DIGEST2

enumerator ESP_EFUSE_KEY_PURPOSE_SECURE_BOOT_DIGEST2
SECURE_BOOT_DIGEST2 (Secure Boot key digest) 

ESP_EFUSE_KEY_PURPOSE_MAX

enumerator ESP_EFUSE_KEY_PURPOSE_MAX
MAX PURPOSE 


```



### Header File


* components/efuse/include/esp_efuse.h
* This header file can be included with:



> ```
> #include "esp_efuse.h"
> 
> ```
* This header file is a part of the API provided by the `efuse` component. To declare that your component depends on `efuse`, add the following to your CMakeLists.txt:



> ```
> REQUIRES efuse
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES efuse
> 
> ```




### Functions


#### esp_efuse_read_field_blob


```


esp_err_t esp_efuse_read_field_blob(const esp_efuse_desc_t *field[], void *dst, size_t dst_size_bits)
Reads bits from EFUSE field and writes it into an array. 
The number of read bits will be limited to the minimum value from the description of the bits in "field" structure or "dst_size_bits" required size. Use "esp_efuse_get_field_size()" function to determine the length of the field.

Note
Please note that reading in the batch mode does not show uncommitted changes.


Parameters

field -- [in] A pointer to the structure describing the fields of efuse. 
dst -- [out] A pointer to array that will contain the result of reading. 
dst_size_bits -- [in] The number of bits required to read. If the requested number of bits is greater than the field, the number will be limited to the field size.


Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments. 





```

#### esp_efuse_read_field_bit


```


bool esp_efuse_read_field_bit(const esp_efuse_desc_t *field[])
Read a single bit eFuse field as a boolean value. 

Note
The value must exist and must be a single bit wide. If there is any possibility of an error in the provided arguments, call esp_efuse_read_field_blob() and check the returned value instead.


Note
If assertions are enabled and the parameter is invalid, execution will abort 


Note
Please note that reading in the batch mode does not show uncommitted changes.


Parameters
field -- [in] A pointer to the structure describing the fields of efuse. 

Returns

true: The field parameter is valid and the bit is set.
false: The bit is not set, or the parameter is invalid and assertions are disabled. 





```

#### esp_efuse_read_field_cnt


```


esp_err_t esp_efuse_read_field_cnt(const esp_efuse_desc_t *field[], size_t *out_cnt)
Reads bits from EFUSE field and returns number of bits programmed as "1". 
If the bits are set not sequentially, they will still be counted. 

Note
Please note that reading in the batch mode does not show uncommitted changes.


Parameters

field -- [in] A pointer to the structure describing the fields of efuse. 
out_cnt -- [out] A pointer that will contain the number of programmed as "1" bits.


Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments. 





```

#### esp_efuse_write_field_blob


```


esp_err_t esp_efuse_write_field_blob(const esp_efuse_desc_t *field[], const void *src, size_t src_size_bits)
Writes array to EFUSE field. 
The number of write bits will be limited to the minimum value from the description of the bits in "field" structure or "src_size_bits" required size. Use "esp_efuse_get_field_size()" function to determine the length of the field. After the function is completed, the writing registers are cleared. 

Parameters

field -- [in] A pointer to the structure describing the fields of efuse. 
src -- [in] A pointer to array that contains the data for writing. 
src_size_bits -- [in] The number of bits required to write.


Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_write_field_cnt


```


esp_err_t esp_efuse_write_field_cnt(const esp_efuse_desc_t *field[], size_t cnt)
Writes a required count of bits as "1" to EFUSE field. 
If there are no free bits in the field to set the required number of bits to "1", ESP_ERR_EFUSE_CNT_IS_FULL error is returned, the field will not be partially recorded. After the function is completed, the writing registers are cleared. 

Parameters

field -- [in] A pointer to the structure describing the fields of efuse. 
cnt -- [in] Required number of programmed as "1" bits.


Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_CNT_IS_FULL: Not all requested cnt bits is set. 





```

#### esp_efuse_write_field_bit


```


esp_err_t esp_efuse_write_field_bit(const esp_efuse_desc_t *field[])
Write a single bit eFuse field to 1. 
For use with eFuse fields that are a single bit. This function will write the bit to value 1 if it is not already set, or does nothing if the bit is already set.
This is equivalent to calling esp_efuse_write_field_cnt() with the cnt parameter equal to 1, except that it will return ESP_OK if the field is already set to 1.

Parameters
field -- [in] Pointer to the structure describing the efuse field.

Returns

ESP_OK: The operation was successfully completed, or the bit was already set to value 1.
ESP_ERR_INVALID_ARG: Error in the passed arugments, including if the efuse field is not 1 bit wide. 





```

#### esp_efuse_set_write_protect


```


esp_err_t esp_efuse_set_write_protect(esp_efuse_block_t blk)
Sets a write protection for the whole block. 
After that, it is impossible to write to this block. The write protection does not apply to block 0. 

Parameters
blk -- [in] Block number of eFuse. (EFUSE_BLK1, EFUSE_BLK2 and EFUSE_BLK3)

Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_CNT_IS_FULL: Not all requested cnt bits is set.
ESP_ERR_NOT_SUPPORTED: The block does not support this command. 





```

#### esp_efuse_set_read_protect


```


esp_err_t esp_efuse_set_read_protect(esp_efuse_block_t blk)
Sets a read protection for the whole block. 
After that, it is impossible to read from this block. The read protection does not apply to block 0. 

Parameters
blk -- [in] Block number of eFuse. (EFUSE_BLK1, EFUSE_BLK2 and EFUSE_BLK3)

Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_CNT_IS_FULL: Not all requested cnt bits is set.
ESP_ERR_NOT_SUPPORTED: The block does not support this command. 





```

#### esp_efuse_get_field_size


```


int esp_efuse_get_field_size(const esp_efuse_desc_t *field[])
Returns the number of bits used by field. 

Parameters
field -- [in] A pointer to the structure describing the fields of efuse.

Returns
Returns the number of bits used by field. 



```

#### esp_efuse_read_reg


```


uint32_t esp_efuse_read_reg(esp_efuse_block_t blk, unsigned int num_reg)
Returns value of efuse register. 
This is a thread-safe implementation. Example: EFUSE_BLK2_RDATA3_REG where (blk=2, num_reg=3) 

Note
Please note that reading in the batch mode does not show uncommitted changes.


Parameters

blk -- [in] Block number of eFuse. 
num_reg -- [in] The register number in the block.


Returns
Value of register 



```

#### esp_efuse_write_reg


```


esp_err_t esp_efuse_write_reg(esp_efuse_block_t blk, unsigned int num_reg, uint32_t val)
Write value to efuse register. 
Apply a coding scheme if necessary. This is a thread-safe implementation. Example: EFUSE_BLK3_WDATA0_REG where (blk=3, num_reg=0) 

Parameters

blk -- [in] Block number of eFuse. 
num_reg -- [in] The register number in the block. 
val -- [in] Value to write.


Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden. 





```

#### esp_efuse_get_coding_scheme


```


esp_efuse_coding_scheme_t esp_efuse_get_coding_scheme(esp_efuse_block_t blk)
Return efuse coding scheme for blocks. 

Note
The coding scheme is applicable only to 1, 2 and 3 blocks. For 0 block, the coding scheme is always NONE.


Parameters
blk -- [in] Block number of eFuse. 

Returns
Return efuse coding scheme for blocks 



```

#### esp_efuse_read_block


```


esp_err_t esp_efuse_read_block(esp_efuse_block_t blk, void *dst_key, size_t offset_in_bits, size_t size_bits)
Read key to efuse block starting at the offset and the required size. 

Note
Please note that reading in the batch mode does not show uncommitted changes.


Parameters

blk -- [in] Block number of eFuse. 
dst_key -- [in] A pointer to array that will contain the result of reading. 
offset_in_bits -- [in] Start bit in block. 
size_bits -- [in] The number of bits required to read.


Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_write_block


```


esp_err_t esp_efuse_write_block(esp_efuse_block_t blk, const void *src_key, size_t offset_in_bits, size_t size_bits)
Write key to efuse block starting at the offset and the required size. 

Parameters

blk -- [in] Block number of eFuse. 
src_key -- [in] A pointer to array that contains the key for writing. 
offset_in_bits -- [in] Start bit in block. 
size_bits -- [in] The number of bits required to write.


Returns

ESP_OK: The operation was successfully completed.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_CODING: Error range of data does not match the coding scheme.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits 





```

#### esp_efuse_get_pkg_ver


```


uint32_t esp_efuse_get_pkg_ver(void)
Returns chip package from efuse. 

Returns
chip package 



```

#### esp_efuse_reset


```


void esp_efuse_reset(void)
Reset efuse write registers. 
Efuse write registers are written to zero, to negate any changes that have been staged here.

Note
This function is not threadsafe, if calling code updates efuse values from multiple tasks then this is caller's responsibility to serialise. 


```

#### esp_efuse_disable_rom_download_mode


```


esp_err_t esp_efuse_disable_rom_download_mode(void)
Disable ROM Download Mode via eFuse. 
Permanently disables the ROM Download Mode feature. Once disabled, if the SoC is booted with strapping pins set for ROM Download Mode then an error is printed instead.

Note
Not all SoCs support this option. An error will be returned if called on an ESP32 with a silicon revision lower than 3, as these revisions do not support this option.


Note
If ROM Download Mode is already disabled, this function does nothing and returns success.


Returns

ESP_OK If the eFuse was successfully burned, or had already been burned.
ESP_ERR_NOT_SUPPORTED (ESP32 only) This SoC is not capable of disabling UART download mode
ESP_ERR_INVALID_STATE (ESP32 only) This eFuse is write protected and cannot be written 





```

#### esp_efuse_set_rom_log_scheme


```


esp_err_t esp_efuse_set_rom_log_scheme(esp_efuse_rom_log_scheme_t log_scheme)
Set boot ROM log scheme via eFuse. 

Note
By default, the boot ROM will always print to console. This API can be called to set the log scheme only once per chip, once the value is changed from the default it can't be changed again.


Parameters
log_scheme -- Supported ROM log scheme 

Returns

ESP_OK If the eFuse was successfully burned, or had already been burned.
ESP_ERR_NOT_SUPPORTED (ESP32 only) This SoC is not capable of setting ROM log scheme
ESP_ERR_INVALID_STATE This eFuse is write protected or has been burned already 





```

#### esp_efuse_enable_rom_secure_download_mode


```


esp_err_t esp_efuse_enable_rom_secure_download_mode(void)
Switch ROM Download Mode to Secure Download mode via eFuse. 
Permanently enables Secure Download mode. This mode limits the use of ROM Download Mode functions to simple flash read, write and erase operations, plus a command to return a summary of currently enabled security features.

Note
If Secure Download mode is already enabled, this function does nothing and returns success.


Note
Disabling the ROM Download Mode also disables Secure Download Mode.


Returns

ESP_OK If the eFuse was successfully burned, or had already been burned.
ESP_ERR_INVALID_STATE ROM Download Mode has been disabled via eFuse, so Secure Download mode is unavailable. 





```

#### esp_efuse_read_secure_version


```


uint32_t esp_efuse_read_secure_version(void)
Return secure_version from efuse field. 

Returns
Secure version from efuse field 



```

#### esp_efuse_check_secure_version


```


bool esp_efuse_check_secure_version(uint32_t secure_version)
Check secure_version from app and secure_version and from efuse field. 

Parameters
secure_version -- Secure version from app. 

Returns

True: If version of app is equal or more then secure_version from efuse. 





```

#### esp_efuse_update_secure_version


```


esp_err_t esp_efuse_update_secure_version(uint32_t secure_version)
Write efuse field by secure_version value. 
Update the secure_version value is available if the coding scheme is None. Note: Do not use this function in your applications. This function is called as part of the other API.

Parameters
secure_version -- [in] Secure version from app. 

Returns

ESP_OK: Successful.
ESP_FAIL: secure version of app cannot be set to efuse field.
ESP_ERR_NOT_SUPPORTED: Anti rollback is not supported with the 3/4 and Repeat coding scheme. 





```

#### esp_efuse_batch_write_begin


```


esp_err_t esp_efuse_batch_write_begin(void)
Set the batch mode of writing fields. 
This mode allows you to write the fields in the batch mode when need to burn several efuses at one time. To enable batch mode call begin() then perform as usually the necessary operations read and write and at the end call commit() to actually burn all written efuses. The batch mode can be used nested. The commit will be done by the last commit() function. The number of begin() functions should be equal to the number of commit() functions.

Note: If batch mode is enabled by the first task, at this time the second task cannot write/read efuses. The second task will wait for the first task to complete the batch operation.
// Example of using the batch writing mode.

// set the batch writing mode
esp_efuse_batch_write_begin();

// use any writing functions as usual
esp_efuse_write_field_blob(ESP_EFUSE_...);
esp_efuse_write_field_cnt(ESP_EFUSE_...);
esp_efuse_set_write_protect(EFUSE_BLKx);
esp_efuse_write_reg(EFUSE_BLKx, ...);
esp_efuse_write_block(EFUSE_BLKx, ...);
esp_efuse_write(ESP_EFUSE_1, 3);  // ESP_EFUSE_1 == 1, here we write a new value = 3. The changes will be burn by the commit() function.
esp_efuse_read_...(ESP_EFUSE_1);  // this function returns ESP_EFUSE_1 == 1 because uncommitted changes are not readable, it will be available only after commit.
...

// esp_efuse_batch_write APIs can be called recursively.
esp_efuse_batch_write_begin();
esp_efuse_set_write_protect(EFUSE_BLKx);
esp_efuse_batch_write_commit(); // the burn will be skipped here, it will be done in the last commit().

...

// Write all of these fields to the efuse registers
esp_efuse_batch_write_commit();
esp_efuse_read_...(ESP_EFUSE_1);  // this function returns ESP_EFUSE_1 == 3.



Note
Please note that reading in the batch mode does not show uncommitted changes.


Returns

ESP_OK: Successful. 





```

#### esp_efuse_batch_write_cancel


```


esp_err_t esp_efuse_batch_write_cancel(void)
Reset the batch mode of writing fields. 
It will reset the batch writing mode and any written changes.

Returns

ESP_OK: Successful.
ESP_ERR_INVALID_STATE: Tha batch mode was not set. 





```

#### esp_efuse_batch_write_commit


```


esp_err_t esp_efuse_batch_write_commit(void)
Writes all prepared data for the batch mode. 
Must be called to ensure changes are written to the efuse registers. After this the batch writing mode will be reset.

Returns

ESP_OK: Successful.
ESP_ERR_INVALID_STATE: The deferred writing mode was not set. 





```

#### esp_efuse_block_is_empty


```


bool esp_efuse_block_is_empty(esp_efuse_block_t block)
Checks that the given block is empty. 

Returns

True: The block is empty.
False: The block is not empty or was an error. 





```

#### esp_efuse_get_key_dis_read


```


bool esp_efuse_get_key_dis_read(esp_efuse_block_t block)
Returns a read protection for the key block. 

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns
True: The key block is read protected False: The key block is readable. 



```

#### esp_efuse_set_key_dis_read


```


esp_err_t esp_efuse_set_key_dis_read(esp_efuse_block_t block)
Sets a read protection for the key block. 

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_get_key_dis_write


```


bool esp_efuse_get_key_dis_write(esp_efuse_block_t block)
Returns a write protection for the key block. 

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns
True: The key block is write protected False: The key block is writeable. 



```

#### esp_efuse_set_key_dis_write


```


esp_err_t esp_efuse_set_key_dis_write(esp_efuse_block_t block)
Sets a write protection for the key block. 

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_key_block_unused


```


bool esp_efuse_key_block_unused(esp_efuse_block_t block)
Returns true if the key block is unused, false otherwise. 
An unused key block is all zero content, not read or write protected, and has purpose 0 (ESP_EFUSE_KEY_PURPOSE_USER)

Parameters
block -- key block to check.

Returns

True if key block is unused,
False if key block is used or the specified block index is not a key block. 





```

#### esp_efuse_find_purpose


```


bool esp_efuse_find_purpose(esp_efuse_purpose_t purpose, esp_efuse_block_t *block)
Find a key block with the particular purpose set. 

Parameters

purpose -- [in] Purpose to search for. 
block -- [out] Pointer in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX which will be set to the key block if found. Can be NULL, if only need to test the key block exists.


Returns

True: If found,
False: If not found (value at block pointer is unchanged). 





```

#### esp_efuse_get_keypurpose_dis_write


```


bool esp_efuse_get_keypurpose_dis_write(esp_efuse_block_t block)
Returns a write protection of the key purpose field for an efuse key block. 

Note
For ESP32: no keypurpose, it returns always True.


Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns
True: The key purpose is write protected. False: The key purpose is writeable. 



```

#### esp_efuse_get_key_purpose


```


esp_efuse_purpose_t esp_efuse_get_key_purpose(esp_efuse_block_t block)
Returns the current purpose set for an efuse key block. 

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns

Value: If Successful, it returns the value of the purpose related to the given key block.
ESP_EFUSE_KEY_PURPOSE_MAX: Otherwise. 





```

#### esp_efuse_get_purpose_field


```


const esp_efuse_desc_t **esp_efuse_get_purpose_field(esp_efuse_block_t block)
Returns a pointer to a key purpose for an efuse key block. 

To get the value of this field use esp_efuse_read_field_blob() or esp_efuse_get_key_purpose().

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns
Pointer: If Successful returns a pointer to the corresponding efuse field otherwise NULL. 



```

#### esp_efuse_get_key


```


const esp_efuse_desc_t **esp_efuse_get_key(esp_efuse_block_t block)
Returns a pointer to a key block. 

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns
Pointer: If Successful returns a pointer to the corresponding efuse field otherwise NULL. 



```

#### esp_efuse_set_key_purpose


```


esp_err_t esp_efuse_set_key_purpose(esp_efuse_block_t block, esp_efuse_purpose_t purpose)
Sets a key purpose for an efuse key block. 

Parameters

block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX 
purpose -- [in] Key purpose.


Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_set_keypurpose_dis_write


```


esp_err_t esp_efuse_set_keypurpose_dis_write(esp_efuse_block_t block)
Sets a write protection of the key purpose field for an efuse key block. 

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_find_unused_key_block


```


esp_efuse_block_t esp_efuse_find_unused_key_block(void)
Search for an unused key block and return the first one found. 
See esp_efuse_key_block_unused for a description of an unused key block.

Returns
First unused key block, or EFUSE_BLK_KEY_MAX if no unused key block is found. 



```

#### esp_efuse_count_unused_key_blocks


```


unsigned esp_efuse_count_unused_key_blocks(void)
Return the number of unused efuse key blocks in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX. 

```

#### esp_efuse_get_digest_revoke


```


bool esp_efuse_get_digest_revoke(unsigned num_digest)
Returns the status of the Secure Boot public key digest revocation bit. 

Parameters
num_digest -- [in] The number of digest in range 0..2

Returns

True: If key digest is revoked,
False; If key digest is not revoked. 





```

#### esp_efuse_set_digest_revoke


```


esp_err_t esp_efuse_set_digest_revoke(unsigned num_digest)
Sets the Secure Boot public key digest revocation bit. 

Parameters
num_digest -- [in] The number of digest in range 0..2

Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_get_write_protect_of_digest_revoke


```


bool esp_efuse_get_write_protect_of_digest_revoke(unsigned num_digest)
Returns a write protection of the Secure Boot public key digest revocation bit. 

Parameters
num_digest -- [in] The number of digest in range 0..2

Returns
True: The revocation bit is write protected. False: The revocation bit is writeable. 



```

#### esp_efuse_set_write_protect_of_digest_revoke


```


esp_err_t esp_efuse_set_write_protect_of_digest_revoke(unsigned num_digest)
Sets a write protection of the Secure Boot public key digest revocation bit. 

Parameters
num_digest -- [in] The number of digest in range 0..2

Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_write_key


```


esp_err_t esp_efuse_write_key(esp_efuse_block_t block, esp_efuse_purpose_t purpose, const void *key, size_t key_size_bytes)
Program a block of key data to an efuse block. 
The burn of a key, protection bits, and a purpose happens in batch mode.

Note
This API also enables the read protection efuse bit for certain key blocks like XTS-AES, HMAC, ECDSA etc. This ensures that the key is only accessible to hardware peripheral.


Note
For SoC's with capability SOC_EFUSE_ECDSA_USE_HARDWARE_K (e.g., ESP32-H2), this API writes an additional efuse bit for ECDSA key purpose to enforce hardware TRNG generated k mode in the peripheral.


Parameters

block -- [in] Block to read purpose for. Must be in range EFUSE_BLK_KEY0 to EFUSE_BLK_KEY_MAX. Key block must be unused (esp_efuse_key_block_unused). 
purpose -- [in] Purpose to set for this key. Purpose must be already unset. 
key -- [in] Pointer to data to write. 
key_size_bytes -- [in] Bytes length of data to write.


Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_INVALID_STATE: Error in efuses state, unused block not found.
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_efuse_write_keys


```


esp_err_t esp_efuse_write_keys(const esp_efuse_purpose_t purposes[], uint8_t keys[][32], unsigned number_of_keys)
Program keys to unused efuse blocks. 
The burn of keys, protection bits, and purposes happens in batch mode.

Note
This API also enables the read protection efuse bit for certain key blocks like XTS-AES, HMAC, ECDSA etc. This ensures that the key is only accessible to hardware peripheral.


Note
For SoC's with capability SOC_EFUSE_ECDSA_USE_HARDWARE_K (e.g., ESP32-H2), this API writes an additional efuse bit for ECDSA key purpose to enforce hardware TRNG generated k mode in the peripheral.


Parameters

purposes -- [in] Array of purposes (purpose[number_of_keys]). 
keys -- [in] Array of keys (uint8_t keys[number_of_keys][32]). Each key is 32 bytes long. 
number_of_keys -- [in] The number of keys to write (up to 6 keys).


Returns

ESP_OK: Successful.
ESP_ERR_INVALID_ARG: Error in the passed arguments.
ESP_ERR_INVALID_STATE: Error in efuses state, unused block not found.
ESP_ERR_NOT_ENOUGH_UNUSED_KEY_BLOCKS: Error not enough unused key blocks available
ESP_ERR_EFUSE_REPEATED_PROG: Error repeated programming of programmed bits is strictly forbidden.
ESP_ERR_CODING: Error range of data does not match the coding scheme. 





```

#### esp_secure_boot_read_key_digests


```


esp_err_t esp_secure_boot_read_key_digests(esp_secure_boot_key_digests_t *trusted_key_digests)
Read key digests from efuse. Any revoked/missing digests will be marked as NULL. 

Parameters
trusted_key_digests -- [out] Trusted keys digests, stored in this parameter after successfully completing this function. The number of digests depends on the SOC's capabilities.

Returns

ESP_OK: Successful.
ESP_FAIL: If trusted_keys is NULL or there is no valid digest. 





```

#### esp_efuse_check_errors


```


esp_err_t esp_efuse_check_errors(void)
Checks eFuse errors in BLOCK0. 

It does a BLOCK0 check if eFuse EFUSE_ERR_RST_ENABLE is set. If BLOCK0 has an error, it prints the error and returns ESP_FAIL, which should be treated as esp_restart.

Note
Refers to ESP32-C3 only.


Returns

ESP_OK: No errors in BLOCK0.
ESP_FAIL: Error in BLOCK0 requiring reboot. 





```

#### esp_efuse_destroy_block


```


esp_err_t esp_efuse_destroy_block(esp_efuse_block_t block)
Destroys the data in the given efuse block, if possible. 
Data destruction occurs through the following steps: 1) Destroy data in the block:
If write protection is inactive for the block, then unset bits are burned.
If write protection is active, the block remains unaltered. 2) Set read protection for the block if possible (check write-protection for RD_DIS). In this case, data becomes inaccessible, and the software reads it as all zeros. If write protection is enabled and read protection can not be set, data in the block remains readable (returns an error).


Do not use the batch mode with this function as it does the burning itself!

Parameters
block -- [in] A key block in the range EFUSE_BLK_KEY0..EFUSE_BLK_KEY_MAX

Returns

ESP_OK: Successful.
ESP_FAIL: Data remained readable because the block is write-protected and read protection can not be set. 





```



### Structures


#### esp_efuse_desc_t


```


struct esp_efuse_desc_t
Type definition for an eFuse field. 

Public Members
efuse_block

esp_efuse_block_t efuse_block
Block of eFuse 

bit_start

uint8_t bit_start
Start bit [0..255] 

bit_count

uint16_t bit_count
Length of bit field [1..-] 



```

#### esp_secure_boot_key_digests_t


```


struct esp_secure_boot_key_digests_t
Pointers to the trusted key digests. 
The number of digests depends on the SOC's capabilities. 

Public Members
key_digests

const void *key_digests[3]
Pointers to the key digests 



```



### Macros


#### ESP_ERR_EFUSE


```


ESP_ERR_EFUSE
Base error code for efuse api. 

```

#### ESP_OK_EFUSE_CNT


```


ESP_OK_EFUSE_CNT
OK the required number of bits is set. 

```

#### ESP_ERR_EFUSE_CNT_IS_FULL


```


ESP_ERR_EFUSE_CNT_IS_FULL
Error field is full. 

```

#### ESP_ERR_EFUSE_REPEATED_PROG


```


ESP_ERR_EFUSE_REPEATED_PROG
Error repeated programming of programmed bits is strictly forbidden. 

```

#### ESP_ERR_CODING


```


ESP_ERR_CODING
Error while a encoding operation. 

```

#### ESP_ERR_NOT_ENOUGH_UNUSED_KEY_BLOCKS


```


ESP_ERR_NOT_ENOUGH_UNUSED_KEY_BLOCKS
Error not enough unused key blocks available 

```

#### ESP_ERR_DAMAGED_READING


```


ESP_ERR_DAMAGED_READING
Error. Burn or reset was done during a reading operation leads to damage read data. This error is internal to the efuse component and not returned by any public API. 

```



### Enumerations


#### esp_efuse_rom_log_scheme_t


```


enum esp_efuse_rom_log_scheme_t
Type definition for ROM log scheme. 
Values:
ESP_EFUSE_ROM_LOG_ALWAYS_ON

enumerator ESP_EFUSE_ROM_LOG_ALWAYS_ON
Always enable ROM logging 

ESP_EFUSE_ROM_LOG_ON_GPIO_LOW

enumerator ESP_EFUSE_ROM_LOG_ON_GPIO_LOW
ROM logging is enabled when specific GPIO level is low during start up 

ESP_EFUSE_ROM_LOG_ON_GPIO_HIGH

enumerator ESP_EFUSE_ROM_LOG_ON_GPIO_HIGH
ROM logging is enabled when specific GPIO level is high during start up 

ESP_EFUSE_ROM_LOG_ALWAYS_OFF

enumerator ESP_EFUSE_ROM_LOG_ALWAYS_OFF
Disable ROM logging permanently 


```