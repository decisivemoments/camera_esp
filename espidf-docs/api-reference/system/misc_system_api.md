# Miscellaneous System APIs




## Software Reset


To perform software reset of the chip, the `esp_restart()` function is provided. When the function is called, execution of the program stops, both CPUs are reset, the application is loaded by the bootloader and starts execution again.


Additionally, the `esp_register_shutdown_handler()` function can register a routine that will be automatically called before a restart (that is triggered by `esp_restart()`) occurs. This is similar to the functionality of `atexit` POSIX function.




## Reset Reason


ESP-IDF applications can be started or restarted due to a variety of reasons. To get the last reset reason, call `esp_reset_reason()` function. See description of `esp_reset_reason_t` for the list of possible reset reasons.




## Heap Memory


Two heap-memory-related functions are provided:


* `esp_get_free_heap_size()` returns the current size of free heap memory.
* `esp_get_minimum_free_heap_size()` returns the minimum size of free heap memory that has ever been available (i.e., the smallest size of free heap memory in the application's lifetime).


Note that ESP-IDF supports multiple heaps with different capabilities. The functions mentioned in this section return the size of heap memory that can be allocated using the `malloc` family of functions. For further information about heap memory, see Heap Memory Allocation.




## MAC Address


These APIs allow querying and customizing MAC addresses for different supported network interfaces (e.g., Wi-Fi, Bluetooth, Ethernet).


To fetch the MAC address for a specific network interface (e.g., Wi-Fi, Bluetooth, Ethernet), call the function `esp_read_mac()`.


In ESP-IDF, the MAC addresses for the various network interfaces are calculated from a single **base MAC address**. By default, the Espressif base MAC address is used. This base MAC address is pre-programmed into the ESP32-S3 eFuse in the factory during production.








| Interface | MAC Address (4 universally administered, default) | MAC Address (2 universally administered) |
| --- | --- | --- |
| Wi-Fi Station | base_mac | base_mac |
| Wi-Fi SoftAP | base_mac, +1 to the last octet | Local MAC (derived from Wi-Fi Station MAC) |
| Bluetooth | base_mac, +2 to the last octet | base_mac, +1 to the last octet |
| Ethernet | base_mac, +3 to the last octet | Local MAC (derived from Bluetooth MAC) |



Note


The configuration configures the number of universally administered MAC addresses that are provided by Espressif.




Note


Although ESP32-S3 has no integrated Ethernet MAC, it is still possible to calculate an Ethernet MAC address. However, this MAC address can only be used with an external ethernet interface such as an SPI-Ethernet device. See Ethernet.




### Custom Interface MAC


Sometimes you may need to define custom MAC addresses that are not generated from the base MAC address. To set a custom interface MAC address, use the `esp_iface_mac_addr_set()` function. This function allows you to overwrite the MAC addresses of interfaces set (or not yet set) by the base MAC address. Once a MAC address has been set for a particular interface, it will not be affected when the base MAC address is changed.




### Custom Base MAC


The default base MAC is pre-programmed by Espressif in eFuse BLK1. To set a custom base MAC instead, call the function `esp_iface_mac_addr_set()` with the `ESP_MAC_BASE` argument (or `esp_base_mac_addr_set()`) before initializing any network interfaces or calling the `esp_read_mac()` function. The custom MAC address can be stored in any supported storage device (e.g., flash, NVS).


The custom base MAC addresses should be allocated such that derived MAC addresses will not overlap. Based on the table above, users can configure the option CONFIG_ESP32S3_UNIVERSAL_MAC_ADDRESSES to set the number of valid universal MAC addresses that can be derived from the custom base MAC.



Note


It is also possible to call the function `esp_netif_set_mac()` to set the specific MAC used by a network interface after network initialization. But it is recommended to use the base MAC approach documented here to avoid the possibility of the original MAC address briefly appearing on the network before being changed.




#### Custom MAC Address in eFuse


When reading custom MAC addresses from eFuse, ESP-IDF provides a helper function `esp_efuse_mac_get_custom()`. Users can also use `esp_read_mac()` with the `ESP_MAC_EFUSE_CUSTOM` argument. This loads the MAC address from eFuse BLK3. The `esp_efuse_mac_get_custom()` function assumes that the custom base MAC address is stored in the following format:








| Field | # of bits | Range of bits |
| --- | --- | --- |
| MAC address | 48 | 200:248 |



Note


The eFuse BLK3 uses RS-coding during burning, which means that all eFuse fields in this block must be burnt at the same time.



Once custom eFuse MAC address has been obtained (using `esp_efuse_mac_get_custom()` or `esp_read_mac()`), you need to set it as the base MAC address. There are two ways to do it:


1. Use an old API: call `esp_base_mac_addr_set()`.
2. Use a new API: call `esp_iface_mac_addr_set()` with the `ESP_MAC_BASE` argument.





### Local Versus Universal MAC Addresses


ESP32-S3 comes pre-programmed with enough valid Espressif universally administered MAC addresses for all internal interfaces. The table above shows how to calculate and derive the MAC address for a specific interface according to the base MAC address.


When using a custom MAC address scheme, it is possible that not all interfaces can be assigned with a universally administered MAC address. In these cases, a locally administered MAC address is assigned. Note that these addresses are intended for use on a single local network only.


See this article) for the definition of locally and universally administered MAC addresses.


Function `esp_derive_local_mac()` is called internally to derive a local MAC address from a universal MAC address. The process is as follows:


1. The U/L bit (bit value 0x2) is set in the first octet of the universal MAC address, creating a local MAC address.
2. If this bit is already set in the supplied universal MAC address (i.e., the supplied "universal" MAC address was in fact already a local MAC address), then the first octet of the local MAC address is XORed with 0x4.





## Chip Version


`esp_chip_info()` function fills `esp_chip_info_t` structure with information about the chip. This includes the chip revision, number of CPU cores, and a bit mask of features enabled in the chip.




## SDK Version


`esp_get_idf_version()` returns a string describing the ESP-IDF version which is used to compile the application. This is the same value as the one available through `IDF_VER` variable of the build system. The version string generally has the format of `git describe` output.


To get the version at build time, additional version macros are provided. They can be used to enable or disable parts of the program depending on the ESP-IDF version.


* `ESP_IDF_VERSION_MAJOR`, `ESP_IDF_VERSION_MINOR`, `ESP_IDF_VERSION_PATCH` are defined to integers representing major, minor, and patch version.
* `ESP_IDF_VERSION_VAL` and `ESP_IDF_VERSION` can be used when implementing version checks:



```
#include "esp_idf_version.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
    // enable functionality present in ESP-IDF v4.0
#endif

```




## App Version


The application version is stored in `esp_app_desc_t` structure. It is located in DROM sector and has a fixed offset from the beginning of the binary file. The structure is located after `esp_image_header_t` and `esp_image_segment_header_t` structures. The type of the field version is string and it has a maximum length of 32 chars.


To set the version in your project manually, you need to set the `PROJECT_VER` variable in the `CMakeLists.txt` of your project. In application `CMakeLists.txt`, put `set(PROJECT_VER "0.1.0.1")` before including `project.cmake`.


If the CONFIG_APP_PROJECT_VER_FROM_CONFIG option is set, the value of CONFIG_APP_PROJECT_VER will be used. Otherwise, if the `PROJECT_VER` variable is not set in the project, it will be retrieved either from the `$(PROJECT_PATH)/version.txt` file (if present) or using git command `git describe`. If neither is available, `PROJECT_VER` will be set to "1". Application can make use of this by calling `esp_app_get_description()` or `esp_ota_get_partition_description()` functions.




## API Reference



### Header File


* components/esp_system/include/esp_system.h
* This header file can be included with:



> ```
> #include "esp_system.h"
> 
> ```




### Functions


#### esp_register_shutdown_handler


```


esp_err_t esp_register_shutdown_handler(shutdown_handler_t handle)
Register shutdown handler. 
This function allows you to register a handler that gets invoked before the application is restarted using esp_restart function. 

Parameters
handle -- function to execute on restart 

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if the handler has already been registered
ESP_ERR_NO_MEM if no more shutdown handler slots are available 





```

#### esp_unregister_shutdown_handler


```


esp_err_t esp_unregister_shutdown_handler(shutdown_handler_t handle)
Unregister shutdown handler. 
This function allows you to unregister a handler which was previously registered using esp_register_shutdown_handler function.
ESP_OK on success
ESP_ERR_INVALID_STATE if the given handler hasn't been registered before 



```

#### esp_restart


```


void esp_restart(void)
Restart PRO and APP CPUs. 
This function can be called both from PRO and APP CPUs. After successful restart, CPU reset reason will be SW_CPU_RESET. Peripherals (except for Wi-Fi, BT, UART0, SPI1, and legacy timers) are not reset. This function does not return. 

```

#### esp_reset_reason


```


esp_reset_reason_t esp_reset_reason(void)
Get reason of last reset. 

Returns
See description of esp_reset_reason_t for explanation of each value. 



```

#### esp_get_free_heap_size


```


uint32_t esp_get_free_heap_size(void)
Get the size of available heap. 

Note
Note that the returned value may be larger than the maximum contiguous block which can be allocated.


Returns
Available heap size, in bytes. 



```

#### esp_get_free_internal_heap_size


```


uint32_t esp_get_free_internal_heap_size(void)
Get the size of available internal heap. 

Note
Note that the returned value may be larger than the maximum contiguous block which can be allocated.


Returns
Available internal heap size, in bytes. 



```

#### esp_get_minimum_free_heap_size


```


uint32_t esp_get_minimum_free_heap_size(void)
Get the minimum heap that has ever been available. 

Returns
Minimum free heap ever available 



```

#### esp_system_abort


```


void esp_system_abort(const char *details)
Trigger a software abort. 

Parameters
details -- Details that will be displayed during panic handling. 



```



### Type Definitions


#### shutdown_handler_t


```


typedef void (*shutdown_handler_t)(void)
Shutdown handler type 

```



### Enumerations


#### esp_reset_reason_t


```


enum esp_reset_reason_t
Reset reasons. 
Values:
ESP_RST_UNKNOWN

enumerator ESP_RST_UNKNOWN
Reset reason can not be determined. 

ESP_RST_POWERON

enumerator ESP_RST_POWERON
Reset due to power-on event. 

ESP_RST_EXT

enumerator ESP_RST_EXT
Reset by external pin (not applicable for ESP32) 

ESP_RST_SW

enumerator ESP_RST_SW
Software reset via esp_restart. 

ESP_RST_PANIC

enumerator ESP_RST_PANIC
Software reset due to exception/panic. 

ESP_RST_INT_WDT

enumerator ESP_RST_INT_WDT
Reset (software or hardware) due to interrupt watchdog. 

ESP_RST_TASK_WDT

enumerator ESP_RST_TASK_WDT
Reset due to task watchdog. 

ESP_RST_WDT

enumerator ESP_RST_WDT
Reset due to other watchdogs. 

ESP_RST_DEEPSLEEP

enumerator ESP_RST_DEEPSLEEP
Reset after exiting deep sleep mode. 

ESP_RST_BROWNOUT

enumerator ESP_RST_BROWNOUT
Brownout reset (software or hardware) 

ESP_RST_SDIO

enumerator ESP_RST_SDIO
Reset over SDIO. 

ESP_RST_USB

enumerator ESP_RST_USB
Reset by USB peripheral. 

ESP_RST_JTAG

enumerator ESP_RST_JTAG
Reset by JTAG. 

ESP_RST_EFUSE

enumerator ESP_RST_EFUSE
Reset due to efuse error. 

ESP_RST_PWR_GLITCH

enumerator ESP_RST_PWR_GLITCH
Reset due to power glitch detected. 

ESP_RST_CPU_LOCKUP

enumerator ESP_RST_CPU_LOCKUP
Reset due to CPU lock up. 


```



### Header File


* components/esp_common/include/esp_idf_version.h
* This header file can be included with:



> ```
> #include "esp_idf_version.h"
> 
> ```




### Functions


#### esp_get_idf_version


```


const char *esp_get_idf_version(void)
Return full IDF version string, same as 'git describe' output.

Note
If you are printing the ESP-IDF version in a log file or other information, this function provides more information than using the numerical version macros. For example, numerical version macros don't differentiate between development, pre-release and release versions, but the output of this function does.


Returns
constant string from IDF_VER 



```



### Macros


#### ESP_IDF_VERSION_MAJOR


```


ESP_IDF_VERSION_MAJOR
Major version number (X.x.x) 

```

#### ESP_IDF_VERSION_MINOR


```


ESP_IDF_VERSION_MINOR
Minor version number (x.X.x) 

```

#### ESP_IDF_VERSION_PATCH


```


ESP_IDF_VERSION_PATCH
Patch version number (x.x.X) 

```

#### ESP_IDF_VERSION_VAL


```


ESP_IDF_VERSION_VAL(major, minor, patch)
Macro to convert IDF version number into an integer
To be used in comparisons, such as ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0) 

```

#### ESP_IDF_VERSION


```


ESP_IDF_VERSION
Current IDF version, as an integer
To be used in comparisons, such as ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0) 

```



### Header File


* components/esp_hw_support/include/esp_mac.h
* This header file can be included with:



> ```
> #include "esp_mac.h"
> 
> ```




### Functions


#### esp_base_mac_addr_set


```


esp_err_t esp_base_mac_addr_set(const uint8_t *mac)
Set base MAC address with the MAC address which is stored in BLK3 of EFUSE or external storage e.g. flash and EEPROM. 
Base MAC address is used to generate the MAC addresses used by network interfaces.
If using a custom base MAC address, call this API before initializing any network interfaces. Refer to the ESP-IDF Programming Guide for details about how the Base MAC is used.

Note
Base MAC must be a unicast MAC (least significant bit of first byte must be zero).


Note
If not using a valid OUI, set the "locally administered" bit (bit value 0x02 in the first byte) to avoid collisions.


Parameters
mac -- base MAC address, length: 6 bytes. length: 6 bytes for MAC-48

Returns
ESP_OK on success ESP_ERR_INVALID_ARG If mac is NULL or is not a unicast MAC 



```

#### esp_base_mac_addr_get


```


esp_err_t esp_base_mac_addr_get(uint8_t *mac)
Return base MAC address which is set using esp_base_mac_addr_set. 

Note
If no custom Base MAC has been set, this returns the pre-programmed Espressif base MAC address.


Parameters
mac -- base MAC address, length: 6 bytes. length: 6 bytes for MAC-48

Returns
ESP_OK on success ESP_ERR_INVALID_ARG mac is NULL ESP_ERR_INVALID_MAC base MAC address has not been set 



```

#### esp_efuse_mac_get_custom


```


esp_err_t esp_efuse_mac_get_custom(uint8_t *mac)
Return base MAC address which was previously written to BLK3 of EFUSE. 
Base MAC address is used to generate the MAC addresses used by the networking interfaces. This API returns the custom base MAC address which was previously written to EFUSE BLK3 in a specified format.
Writing this EFUSE allows setting of a different (non-Espressif) base MAC address. It is also possible to store a custom base MAC address elsewhere, see esp_base_mac_addr_set() for details.

Note
This function is currently only supported on ESP32.


Parameters
mac -- base MAC address, length: 6 bytes/8 bytes. length: 6 bytes for MAC-48 8 bytes for EUI-64(used for IEEE 802.15.4, if CONFIG_SOC_IEEE802154_SUPPORTED=y)

Returns
ESP_OK on success ESP_ERR_INVALID_ARG mac is NULL ESP_ERR_INVALID_MAC CUSTOM_MAC address has not been set, all zeros (for esp32-xx) ESP_ERR_INVALID_VERSION An invalid MAC version field was read from BLK3 of EFUSE (for esp32) ESP_ERR_INVALID_CRC An invalid MAC CRC was read from BLK3 of EFUSE (for esp32) 



```

#### esp_efuse_mac_get_default


```


esp_err_t esp_efuse_mac_get_default(uint8_t *mac)
Return base MAC address which is factory-programmed by Espressif in EFUSE. 

Parameters
mac -- base MAC address, length: 6 bytes/8 bytes. length: 6 bytes for MAC-48 8 bytes for EUI-64(used for IEEE 802.15.4, if CONFIG_SOC_IEEE802154_SUPPORTED=y)

Returns
ESP_OK on success ESP_ERR_INVALID_ARG mac is NULL 



```

#### esp_read_mac


```


esp_err_t esp_read_mac(uint8_t *mac, esp_mac_type_t type)
Read base MAC address and set MAC address of the interface. 
This function first get base MAC address using esp_base_mac_addr_get(). Then calculates the MAC address of the specific interface requested, refer to ESP-IDF Programming Guide for the algorithm.
The MAC address set by the esp_iface_mac_addr_set() function will not depend on the base MAC address.

Parameters

mac -- base MAC address, length: 6 bytes/8 bytes. length: 6 bytes for MAC-48 8 bytes for EUI-64(used for IEEE 802.15.4, if CONFIG_SOC_IEEE802154_SUPPORTED=y) 
type -- Type of MAC address to return


Returns
ESP_OK on success 



```

#### esp_derive_local_mac


```


esp_err_t esp_derive_local_mac(uint8_t *local_mac, const uint8_t *universal_mac)
Derive local MAC address from universal MAC address. 
This function copies a universal MAC address and then sets the "locally
administered" bit (bit 0x2) in the first octet, creating a locally administered MAC address.

If the universal MAC address argument is already a locally administered MAC address, then the first octet is XORed with 0x4 in order to create a different locally administered MAC address.

Parameters

local_mac -- base MAC address, length: 6 bytes. length: 6 bytes for MAC-48 
universal_mac -- Source universal MAC address, length: 6 bytes.


Returns
ESP_OK on success 



```

#### esp_iface_mac_addr_set


```


esp_err_t esp_iface_mac_addr_set(const uint8_t *mac, esp_mac_type_t type)
Set custom MAC address of the interface. This function allows you to overwrite the MAC addresses of the interfaces set by the base MAC address. 

Parameters

mac -- MAC address, length: 6 bytes/8 bytes. length: 6 bytes for MAC-48 8 bytes for EUI-64(used for ESP_MAC_IEEE802154 type, if CONFIG_SOC_IEEE802154_SUPPORTED=y) 
type -- Type of MAC address


Returns
ESP_OK on success 



```

#### esp_mac_addr_len_get


```


size_t esp_mac_addr_len_get(esp_mac_type_t type)
Return the size of the MAC type in bytes. 
If CONFIG_SOC_IEEE802154_SUPPORTED is set then for these types:
ESP_MAC_IEEE802154 is 8 bytes.
ESP_MAC_BASE, ESP_MAC_EFUSE_FACTORY and ESP_MAC_EFUSE_CUSTOM the MAC size is 6 bytes.
ESP_MAC_EFUSE_EXT is 2 bytes. If CONFIG_SOC_IEEE802154_SUPPORTED is not set then for all types it returns 6 bytes.



Parameters
type -- Type of MAC address

Returns
0 MAC type not found (not supported) 6 bytes for MAC-48. 8 bytes for EUI-64. 



```



### Macros


#### MAC2STR


```


MAC2STR(a)

```

#### MACSTR


```


MACSTR

```



### Enumerations


#### esp_mac_type_t


```


enum esp_mac_type_t
Values:
ESP_MAC_WIFI_STA

enumerator ESP_MAC_WIFI_STA
MAC for WiFi Station (6 bytes) 

ESP_MAC_WIFI_SOFTAP

enumerator ESP_MAC_WIFI_SOFTAP
MAC for WiFi Soft-AP (6 bytes) 

ESP_MAC_BT

enumerator ESP_MAC_BT
MAC for Bluetooth (6 bytes) 

ESP_MAC_ETH

enumerator ESP_MAC_ETH
MAC for Ethernet (6 bytes) 

ESP_MAC_IEEE802154

enumerator ESP_MAC_IEEE802154
if CONFIG_SOC_IEEE802154_SUPPORTED=y, MAC for IEEE802154 (8 bytes) 

ESP_MAC_BASE

enumerator ESP_MAC_BASE
Base MAC for that used for other MAC types (6 bytes) 

ESP_MAC_EFUSE_FACTORY

enumerator ESP_MAC_EFUSE_FACTORY
MAC_FACTORY eFuse which was burned by Espressif in production (6 bytes) 

ESP_MAC_EFUSE_CUSTOM

enumerator ESP_MAC_EFUSE_CUSTOM
MAC_CUSTOM eFuse which was can be burned by customer (6 bytes) 

ESP_MAC_EFUSE_EXT

enumerator ESP_MAC_EFUSE_EXT
if CONFIG_SOC_IEEE802154_SUPPORTED=y, MAC_EXT eFuse which is used as an extender for IEEE802154 MAC (2 bytes) 


```



### Header File


* components/esp_hw_support/include/esp_chip_info.h
* This header file can be included with:



> ```
> #include "esp_chip_info.h"
> 
> ```




### Functions


#### esp_chip_info


```


void esp_chip_info(esp_chip_info_t *out_info)
Fill an esp_chip_info_t structure with information about the chip. 

Parameters
out_info -- [out] structure to be filled 



```



### Structures


#### esp_chip_info_t


```


struct esp_chip_info_t
The structure represents information about the chip. 

Public Members
model

esp_chip_model_t model
chip model, one of esp_chip_model_t 

features

uint32_t features
bit mask of CHIP_FEATURE_x feature flags 

revision

uint16_t revision
chip revision number (in format MXX; where M - wafer major version, XX - wafer minor version) 

cores

uint8_t cores
number of CPU cores 



```



### Macros


#### CHIP_FEATURE_EMB_FLASH


```


CHIP_FEATURE_EMB_FLASH
Chip has embedded flash memory. 

```

#### CHIP_FEATURE_WIFI_BGN


```


CHIP_FEATURE_WIFI_BGN
Chip has 2.4GHz WiFi. 

```

#### CHIP_FEATURE_BLE


```


CHIP_FEATURE_BLE
Chip has Bluetooth LE. 

```

#### CHIP_FEATURE_BT


```


CHIP_FEATURE_BT
Chip has Bluetooth Classic. 

```

#### CHIP_FEATURE_IEEE802154


```


CHIP_FEATURE_IEEE802154
Chip has IEEE 802.15.4. 

```

#### CHIP_FEATURE_EMB_PSRAM


```


CHIP_FEATURE_EMB_PSRAM
Chip has embedded psram. 

```



### Enumerations


#### esp_chip_model_t


```


enum esp_chip_model_t
Chip models. 
Values:
CHIP_ESP32

enumerator CHIP_ESP32
ESP32. 

CHIP_ESP32S2

enumerator CHIP_ESP32S2
ESP32-S2. 

CHIP_ESP32S3

enumerator CHIP_ESP32S3
ESP32-S3. 

CHIP_ESP32C3

enumerator CHIP_ESP32C3
ESP32-C3. 

CHIP_ESP32C2

enumerator CHIP_ESP32C2
ESP32-C2. 

CHIP_ESP32C6

enumerator CHIP_ESP32C6
ESP32-C6. 

CHIP_ESP32H2

enumerator CHIP_ESP32H2
ESP32-H2. 

CHIP_ESP32P4

enumerator CHIP_ESP32P4
ESP32-P4. 

CHIP_ESP32C61

enumerator CHIP_ESP32C61
ESP32-C61. 

CHIP_POSIX_LINUX

enumerator CHIP_POSIX_LINUX
The code is running on POSIX/Linux simulator. 


```



### Header File


* components/esp_hw_support/include/esp_cpu.h
* This header file can be included with:



> ```
> #include "esp_cpu.h"
> 
> ```




### Functions


#### esp_cpu_stall


```


void esp_cpu_stall(int core_id)
Stall a CPU core. 

Parameters
core_id -- The core's ID 



```

#### esp_cpu_unstall


```


void esp_cpu_unstall(int core_id)
Resume a previously stalled CPU core. 

Parameters
core_id -- The core's ID 



```

#### esp_cpu_reset


```


void esp_cpu_reset(int core_id)
Reset a CPU core. 

Parameters
core_id -- The core's ID 



```

#### esp_cpu_wait_for_intr


```


void esp_cpu_wait_for_intr(void)
Wait for Interrupt. 
This function causes the current CPU core to execute its Wait For Interrupt (WFI or equivalent) instruction. After executing this function, the CPU core will stop execution until an interrupt occurs. 

```

#### esp_cpu_get_core_id


```


int esp_cpu_get_core_id(void)
Get the current core's ID. 
This function will return the ID of the current CPU (i.e., the CPU that calls this function).

Returns
The current core's ID [0..SOC_CPU_CORES_NUM - 1] 



```

#### esp_cpu_get_sp


```


void *esp_cpu_get_sp(void)
Read the current stack pointer address. 

Returns
Stack pointer address 



```

#### esp_cpu_get_cycle_count


```


esp_cpu_cycle_count_t esp_cpu_get_cycle_count(void)
Get the current CPU core's cycle count. 
Each CPU core maintains an internal counter (i.e., cycle count) that increments every CPU clock cycle.

Returns
Current CPU's cycle count, 0 if not supported. 



```

#### esp_cpu_set_cycle_count


```


void esp_cpu_set_cycle_count(esp_cpu_cycle_count_t cycle_count)
Set the current CPU core's cycle count. 
Set the given value into the internal counter that increments every CPU clock cycle.

Parameters
cycle_count -- CPU cycle count 



```

#### esp_cpu_pc_to_addr


```


void *esp_cpu_pc_to_addr(uint32_t pc)
Convert a program counter (PC) value to address. 
If the architecture does not store the true virtual address in the CPU's PC or return addresses, this function will convert the PC value to a virtual address. Otherwise, the PC is just returned

Parameters
pc -- PC value 

Returns
Virtual address 



```

#### esp_cpu_intr_get_desc


```


void esp_cpu_intr_get_desc(int core_id, int intr_num, esp_cpu_intr_desc_t *intr_desc_ret)
Get a CPU interrupt's descriptor. 
Each CPU interrupt has a descriptor describing the interrupt's capabilities and restrictions. This function gets the descriptor of a particular interrupt on a particular CPU.

Parameters

core_id -- [in] The core's ID 
intr_num -- [in] Interrupt number 
intr_desc_ret -- [out] The interrupt's descriptor 




```

#### esp_cpu_intr_set_ivt_addr


```


void esp_cpu_intr_set_ivt_addr(const void *ivt_addr)
Set the base address of the current CPU's Interrupt Vector Table (IVT) 

Parameters
ivt_addr -- Interrupt Vector Table's base address 



```

#### esp_cpu_intr_has_handler


```


bool esp_cpu_intr_has_handler(int intr_num)
Check if a particular interrupt already has a handler function. 
Check if a particular interrupt on the current CPU already has a handler function assigned.

Note
This function simply checks if the IVT of the current CPU already has a handler assigned. 


Parameters
intr_num -- Interrupt number (from 0 to 31) 

Returns
True if the interrupt has a handler function, false otherwise. 



```

#### esp_cpu_intr_set_handler


```


void esp_cpu_intr_set_handler(int intr_num, esp_cpu_intr_handler_t handler, void *handler_arg)
Set the handler function of a particular interrupt. 
Assign a handler function (i.e., ISR) to a particular interrupt on the current CPU.

Note
This function simply sets the handler function (in the IVT) and does not actually enable the interrupt. 


Parameters

intr_num -- Interrupt number (from 0 to 31) 
handler -- Handler function 
handler_arg -- Argument passed to the handler function 




```

#### esp_cpu_intr_get_handler_arg


```


void *esp_cpu_intr_get_handler_arg(int intr_num)
Get a handler function's argument of. 
Get the argument of a previously assigned handler function on the current CPU.

Parameters
intr_num -- Interrupt number (from 0 to 31) 

Returns
The the argument passed to the handler function 



```

#### esp_cpu_intr_enable


```


void esp_cpu_intr_enable(uint32_t intr_mask)
Enable particular interrupts on the current CPU. 

Parameters
intr_mask -- Bit mask of the interrupts to enable 



```

#### esp_cpu_intr_disable


```


void esp_cpu_intr_disable(uint32_t intr_mask)
Disable particular interrupts on the current CPU. 

Parameters
intr_mask -- Bit mask of the interrupts to disable 



```

#### esp_cpu_intr_get_enabled_mask


```


uint32_t esp_cpu_intr_get_enabled_mask(void)
Get the enabled interrupts on the current CPU. 

Returns
Bit mask of the enabled interrupts 



```

#### esp_cpu_intr_edge_ack


```


void esp_cpu_intr_edge_ack(int intr_num)
Acknowledge an edge interrupt. 

Parameters
intr_num -- Interrupt number (from 0 to 31) 



```

#### esp_cpu_configure_region_protection


```


void esp_cpu_configure_region_protection(void)
Configure the CPU to disable access to invalid memory regions. 

```

#### esp_cpu_set_breakpoint


```


esp_err_t esp_cpu_set_breakpoint(int bp_num, const void *bp_addr)
Set and enable a hardware breakpoint on the current CPU. 

Note
This function is meant to be called by the panic handler to set a breakpoint for an attached debugger during a panic. 


Note
Overwrites previously set breakpoint with same breakpoint number. 


Parameters

bp_num -- Hardware breakpoint number [0..SOC_CPU_BREAKPOINTS_NUM - 1] 
bp_addr -- Address to set a breakpoint on 


Returns
ESP_OK if breakpoint is set. Failure otherwise 



```

#### esp_cpu_clear_breakpoint


```


esp_err_t esp_cpu_clear_breakpoint(int bp_num)
Clear a hardware breakpoint on the current CPU. 

Note
Clears a breakpoint regardless of whether it was previously set 


Parameters
bp_num -- Hardware breakpoint number [0..SOC_CPU_BREAKPOINTS_NUM - 1] 

Returns
ESP_OK if breakpoint is cleared. Failure otherwise 



```

#### esp_cpu_set_watchpoint


```


esp_err_t esp_cpu_set_watchpoint(int wp_num, const void *wp_addr, size_t size, esp_cpu_watchpoint_trigger_t trigger)
Set and enable a hardware watchpoint on the current CPU. 
Set and enable a hardware watchpoint on the current CPU, specifying the memory range and trigger operation. Watchpoints will break/panic the CPU when the CPU accesses (according to the trigger type) on a certain memory range.

Note
Overwrites previously set watchpoint with same watchpoint number. On RISC-V chips, this API uses method0(Exact matching) and method1(NAPOT matching) according to the riscv-debug-spec-0.13 specification for address matching. If the watch region size is 1byte, it uses exact matching (method 0). If the watch region size is larger than 1byte, it uses NAPOT matching (method 1). This mode requires the watching region start address to be aligned to the watching region size.


Parameters

wp_num -- Hardware watchpoint number [0..SOC_CPU_WATCHPOINTS_NUM - 1] 
wp_addr -- Watchpoint's base address, must be naturally aligned to the size of the region 
size -- Size of the region to watch. Must be one of 2^n and in the range of [1 ... SOC_CPU_WATCHPOINT_MAX_REGION_SIZE] 
trigger -- Trigger type 


Returns
ESP_ERR_INVALID_ARG on invalid arg, ESP_OK otherwise 



```

#### esp_cpu_clear_watchpoint


```


esp_err_t esp_cpu_clear_watchpoint(int wp_num)
Clear a hardware watchpoint on the current CPU. 

Note
Clears a watchpoint regardless of whether it was previously set 


Parameters
wp_num -- Hardware watchpoint number [0..SOC_CPU_WATCHPOINTS_NUM - 1] 

Returns
ESP_OK if watchpoint was cleared. Failure otherwise. 



```

#### esp_cpu_dbgr_is_attached


```


bool esp_cpu_dbgr_is_attached(void)
Check if the current CPU has a debugger attached. 

Returns
True if debugger is attached, false otherwise 



```

#### esp_cpu_dbgr_break


```


void esp_cpu_dbgr_break(void)
Trigger a call to the current CPU's attached debugger. 

```

#### esp_cpu_get_call_addr


```


intptr_t esp_cpu_get_call_addr(intptr_t return_address)
Given the return address, calculate the address of the preceding call instruction This is typically used to answer the question "where was the function called from?". 

Parameters
return_address -- The value of the return address register. Typically set to the value of __builtin_return_address(0). 

Returns
Address of the call instruction preceding the return address. 



```

#### esp_cpu_compare_and_set


```


bool esp_cpu_compare_and_set(volatile uint32_t *addr, uint32_t compare_value, uint32_t new_value)
Atomic compare-and-set operation. 

Parameters

addr -- Address of atomic variable 
compare_value -- Value to compare the atomic variable to 
new_value -- New value to set the atomic variable to 


Returns
Whether the atomic variable was set or not 



```



### Structures


#### esp_cpu_intr_desc_t


```


struct esp_cpu_intr_desc_t
CPU interrupt descriptor. 
Each particular CPU interrupt has an associated descriptor describing that particular interrupt's characteristics. Call esp_cpu_intr_get_desc() to get the descriptors of a particular interrupt. 

Public Members
priority

int priority
Priority of the interrupt if it has a fixed priority, (-1) if the priority is configurable. 

type

esp_cpu_intr_type_t type
Whether the interrupt is an edge or level type interrupt, ESP_CPU_INTR_TYPE_NA if the type is configurable. 

flags

uint32_t flags
Flags indicating extra details. 



```



### Macros


#### ESP_CPU_INTR_DESC_FLAG_SPECIAL


```


ESP_CPU_INTR_DESC_FLAG_SPECIAL
Interrupt descriptor flags of esp_cpu_intr_desc_t. 
The interrupt is a special interrupt (e.g., a CPU timer interrupt) 

```

#### ESP_CPU_INTR_DESC_FLAG_RESVD


```


ESP_CPU_INTR_DESC_FLAG_RESVD
The interrupt is reserved for internal use 

```



### Type Definitions


#### esp_cpu_cycle_count_t


```


typedef uint32_t esp_cpu_cycle_count_t
CPU cycle count type. 
This data type represents the CPU's clock cycle count 

```

#### esp_cpu_intr_handler_t


```


typedef void (*esp_cpu_intr_handler_t)(void *arg)
CPU interrupt handler type. 

```



### Enumerations


#### esp_cpu_intr_type_t


```


enum esp_cpu_intr_type_t
CPU interrupt type. 
Values:
ESP_CPU_INTR_TYPE_LEVEL

enumerator ESP_CPU_INTR_TYPE_LEVEL

ESP_CPU_INTR_TYPE_EDGE

enumerator ESP_CPU_INTR_TYPE_EDGE

ESP_CPU_INTR_TYPE_NA

enumerator ESP_CPU_INTR_TYPE_NA


```

#### esp_cpu_watchpoint_trigger_t


```


enum esp_cpu_watchpoint_trigger_t
CPU watchpoint trigger type. 
Values:
ESP_CPU_WATCHPOINT_LOAD

enumerator ESP_CPU_WATCHPOINT_LOAD

ESP_CPU_WATCHPOINT_STORE

enumerator ESP_CPU_WATCHPOINT_STORE

ESP_CPU_WATCHPOINT_ACCESS

enumerator ESP_CPU_WATCHPOINT_ACCESS


```



### Header File


* components/esp_app_format/include/esp_app_desc.h
* This header file can be included with:



> ```
> #include "esp_app_desc.h"
> 
> ```
* This header file is a part of the API provided by the `esp_app_format` component. To declare that your component depends on `esp_app_format`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_app_format
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_app_format
> 
> ```




### Functions


#### esp_app_get_description


```


const esp_app_desc_t *esp_app_get_description(void)
Return esp_app_desc structure. This structure includes app version. 
Return description for running app. 

Returns
Pointer to esp_app_desc structure. 



```

#### esp_app_get_elf_sha256


```


int esp_app_get_elf_sha256(char *dst, size_t size)
Fill the provided buffer with SHA256 of the ELF file, formatted as hexadecimal, null-terminated. If the buffer size is not sufficient to fit the entire SHA256 in hex plus a null terminator, the largest possible number of bytes will be written followed by a null. 

Parameters

dst -- Destination buffer 
size -- Size of the buffer 


Returns
Number of bytes written to dst (including null terminator) 



```

#### esp_app_get_elf_sha256_str


```


char *esp_app_get_elf_sha256_str(void)
Return SHA256 of the ELF file which is already formatted as hexadecimal, null-terminated included. Can be used in panic handler or core dump during when cache is disabled. The length is defined by CONFIG_APP_RETRIEVE_LEN_ELF_SHA option. 

Returns
Hexadecimal SHA256 string 



```



### Structures


#### esp_app_desc_t


```


struct esp_app_desc_t
Description about application. 

Public Members
magic_word

uint32_t magic_word
Magic word ESP_APP_DESC_MAGIC_WORD 

secure_version

uint32_t secure_version
Secure version 

reserv1

uint32_t reserv1[2]
reserv1 

version

char version[32]
Application version 

project_name

char project_name[32]
Project name 

time

char time[16]
Compile time 

date

char date[16]
Compile date 

idf_ver

char idf_ver[32]
Version IDF 

app_elf_sha256

uint8_t app_elf_sha256[32]
sha256 of elf file 

reserv2

uint32_t reserv2[20]
reserv2 



```



### Macros


#### ESP_APP_DESC_MAGIC_WORD


```


ESP_APP_DESC_MAGIC_WORD
The magic word for the esp_app_desc structure that is in DROM. 

```