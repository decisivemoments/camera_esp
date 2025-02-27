# Removed or Deprecated Components




## Components Moved to ESP-IDF Component Registry


Following components are removed from ESP-IDF and moved to ESP-IDF Component Registry:


* libsodium
* cbor
* jsmn
* esp_modem
* nghttp
* mdns
* esp_websocket_client
* asio
* freemodbus
* sh2lib
* expat
* coap
* esp-cryptoauthlib
* qrcode
* tjpgd
* esp_serial_slave_link
* tinyusb



Note


Please note that http parser functionality which was previously part of `nghttp` component is now part of http_parser  component.



These components can be installed using `idf.py add-dependency` command.


For example, to install libsodium component with the exact version X.Y, run `idf.py add-dependency libsodium==X.Y`.


To install libsodium component with the latest version compatible to X.Y according to semver rules, run `idf.py add-dependency libsodium~X.Y`.


To find out which versions of each component are available, open https://components.espressif.com, search for the component by its name and check the versions listed on the component page.




## Deprecated Components


The following components are removed since they were deprecated in ESP-IDF v4.x:


* `tcpip_adapter`. Please use the ESP-NETIF component instead; you can follow the TCP/IP Adapter.



Note


OpenSSL-API component is no longer supported. It is not available in the IDF Component Registry, either. Please use ESP-TLS or mbedtls API directly.




Note


`esp_adc_cal` component is no longer supported. New adc calibration driver is in `esp_adc` component. Legacy adc calibration driver has been moved into `esp_adc` component. To use legacy `esp_adc_cal` driver APIs, you should add `esp_adc` component to the list of component requirements in CMakeLists.txt. Also check Peripherals Migration Guide for more details.



The targets components are no longer necessary after refactoring and have been removed:



> * `esp32`
> * `esp32s2`
> * `esp32s3`
> * `esp32c2`
> * `esp32c3`
> * `esp32h2`