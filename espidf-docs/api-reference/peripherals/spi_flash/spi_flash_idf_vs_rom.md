# SPI Flash API ESP-IDF Version vs Chip-ROM Version




There is a set of SPI Flash drivers in Chip-ROM which you can use by enabling CONFIG_SPI_FLASH_ROM_IMPL. Most of the ESP-IDF SPI Flash driver code are in internal RAM, therefore enabling this option frees some internal RAM usage. Note if you enable this option, this means some SPI Flash driver features and bugfixes that are done in ESP-IDF might not be included in the Chip-ROM version.



## Feature Supported by ESP-IDF but Not in Chip-ROM


* Octal Flash chip support. See OPI Flash Support for details.
* 32-bit-address support for GD25Q256. Note this feature is an optional feature, please do read 32-bit Address Support of QSPI Flash Chips for details.
* TH Flash chip support.
* Kconfig option CONFIG_SPI_FLASH_CHECK_ERASE_TIMEOUT_DISABLED.
* CONFIG_SPI_FLASH_VERIFY_WRITE, enabling this option helps you detect bad writing.
* CONFIG_SPI_FLASH_LOG_FAILED_WRITE, enabling this option prints the bad writing.
* CONFIG_SPI_FLASH_WARN_SETTING_ZERO_TO_ONE, enabling this option checks if you are writing zero to one.
* CONFIG_SPI_FLASH_DANGEROUS_WRITE, enabling this option checks for flash programming to certain protected regions like bootloader, partition table or application itself.
* CONFIG_SPI_FLASH_ENABLE_COUNTERS, enabling this option to collect performance data for ESP-IDF SPI Flash driver APIs.
* CONFIG_SPI_FLASH_AUTO_SUSPEND, enabling this option to automatically suspend / resume a long Flash operation when short Flash operation happens. Note this feature is an optional feature, please do read Auto Suspend & Resume for more limitations.







## Bugfixes Introduced in ESP-IDF but Not in Chip-ROM


* Detected Flash physical size correctly, for larger than 256MBit Flash chips. (Commit ID: b4964279d44f73cce7cfd5cf684567fbdfd6fd9e)
* Fixed issue that 4-line Flash encryption cannot work normally when 8-line PSRAM enabled. (Commit ID: 683d92bc884e0f2a7eebea40a551cf05f0c28256)
* Fixed issue that only 128KB virtual address ranges can be mapped to instructions on Flash.
* Fixed issue that only 16MB virtual address ranges can be mapped to read-only data on Flash.