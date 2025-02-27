# Bluetooth® Overview



This document provides an architecture overview of the Bluetooth stack in ESP-IDF and some quick links to related documents and application examples.


ESP32-S3 supports Bluetooth 5.0 (LE) and is certified for Bluetooth LE 5.0.


The Bluetooth stack in ESP-IDF is a layered architecture that enables Bluetooth functionality on ESP32-S3 chip series. The table below shows its architecture.



![ESP32-S3 Bluetooth Stack Architecture](../_images/bluetooth-architecture.png)



ESP32-S3 Bluetooth Stack Architecture






The table below shows whether the Bluetooth modules are supported in a specific chip series.




| Chip Series | Controller | ESP-Bluedroid | ESP-NimBLE | ESP-BLE-MESH | BluFi |
| --- | --- | --- | --- | --- | --- |
| ESP32 | Y | Y | Y | Y | Y |
| ESP32-S2 | – | – | – | – | – |
| ESP32-S3 | Y | Y | Y | Y | Y |
| ESP32-C2 | Y | Y | Y | – | Y |
| ESP32-C3 | Y | Y | Y | Y | Y |
| ESP32-C6 | Y | Y | Y | Y | Y |
| ESP32-H2 | Y | Y | Y | Y | – |


The following sections briefly describe each layer and provide quick links to the related documents and application examples.



## ESP Bluetooth Controller


At the bottom layer is ESP Bluetooth Controller, which encompasses various modules such as PHY, Baseband, Link Controller, Link Manager, Device Manager, and HCI. It handles hardware interface management and link management. It provides functions in the form of libraries and is accessible through APIs. This layer directly interacts with the hardware and low-level Bluetooth protocols.


* API reference
* Application examples




## Hosts


There are two hosts, ESP-Bluedroid and ESP-NimBLE. The major difference between them is as follows:


* Although both support Bluetooth LE, ESP-NimBLE requires less heap and flash size.



### ESP-Bluedroid


ESP-Bluedroid is a modified version of the native Android Bluetooth stack, Bluedroid. It consists of two layers: the Bluetooth Upper Layer (BTU) and the Bluetooth Transport Controller layer (BTC). The BTU layer is responsible for processing bottom layer Bluetooth protocols such as L2CAP, GATT/ATT, SMP, GAP, and other profiles. The BTU layer provides an interface prefixed with "bta". The BTC layer is mainly responsible for providing a supported interface, prefixed with "esp", to the application layer, processing GATT-based profiles and handling miscellaneous tasks. All the APIs are located in the ESP_API layer. Developers should use the Bluetooth APIs prefixed with "esp".


ESP-Bluedroid for ESP32-S3 supports Bluetooth LE only. Classic Bluetooth is not supported.


* API references


	+ Bluetooth® Common
	+ Bluetooth LE
* Application examples




### ESP-NimBLE


ESP-NimBLE is a host stack built on top of the NimBLE host stack developed by Apache Mynewt. The NimBLE host stack is ported for ESP32-S3 chip series and FreeRTOS. The porting layer is kept clean by maintaining all the existing APIs of NimBLE along with a single ESP-NimBLE API for initialization, making it simpler for the application developers.


ESP-NimBLE supports Bluetooth LE only. Classic Bluetooth is not supported.


* Apache Mynewt NimBLE User Guide
* API references


	+ NimBLE API references
	+ ESP-NimBLE API references for initialization
* Application examples





## Profiles


Above the host stacks are the profile implementations by Espressif and some common profiles. Depending on your configuration, these profiles can run on ESP-Bluedroid or ESP-NimBLE.



### ESP-BLE-MESH


Built on top of Zephyr Bluetooth Mesh stack, the ESP-BLE-MESH implementation supports device provisioning and node control. It also supports such node features as Proxy, Relay, Low power and Friend.


* ESP-BLE-MESH documentation: feature list, get started, architecture, description of application examples, frequently asked questions, etc.
* Application examples




### BluFi


The BluFi for ESP32-S3 is a Wi-Fi network configuration function via Bluetooth channel. It provides a secure protocol to pass Wi-Fi configuration and credentials to ESP32-S3. Using this information, ESP32-S3 can then connect to an AP or establish a softAP.


* BluFi documentation
* Application examples





## Applications


At the uppermost layer are applications. You can build your own applications on top of the ESP-Bluedroid and ESP-NimBLE stacks, leveraging the provided APIs and profiles to create Bluetooth-enabled applications tailored to specific use cases.