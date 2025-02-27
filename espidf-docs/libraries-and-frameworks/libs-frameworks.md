# Espressif's Frameworks



Here you will find a collection of the official Espressif libraries and frameworks.



## Espressif Audio Development Framework


The ESP-ADF is a comprehensive framework for audio applications including:


* CODEC's HAL
* Music players and recorders
* Audio processing
* Bluetooth speakers
* Internet radios
* Hands-free devices
* Speech decognition


This framework is available on GitHub: ESP-ADF.




## ESP-CSI


ESP-CSI is an experimental implementation that uses the Wi-Fi Channel State Information to detect the presence of a human body.


See the ESP-CSI project for more information.




## Espressif DSP Library


The library provides algorithms optimized specifically for digital signal processing applications. This library supports:


* Matrix multiplication
* Dot product
* FFT (Fast Fourier Transform)
* IIR (Infinite Impulse Response)
* FIR (Finite Impulse Response)
* Vector math operations


This library is available on Github: ESP-DSP library.




## ESP-WIFI-MESH Development Framework


This framework is based on the ESP-WIFI-MESH protocol with the following features:


* Fast network configuration
* Stable upgrade
* Efficient debugging
* LAN control
* Various application demos


This framework is available on Github: ESP-MDF.




## ESP-WHO


The ESP-WHO is a face detection and recognition framework using the ESP32 and camera.


This framework is available on Github: ESP-WHO.




## ESP RainMaker


ESP RainMaker is a complete solution for accelerated AIoT development. Using ESP RainMaker, you can create AIoT devices from the firmware to the integration with voice-assistant, phone apps and cloud backend.


This project is available on Github: ESP RainMaker on GitHub.




## ESP-IoT-Solution


ESP-IoT-Solution contains commonly used device drivers and code frameworks when developing IoT systems. The device drivers and code frameworks within the ESP-IoT-Solution are organized as separate components, allowing them to be easily integrated into an ESP-IDF project.


ESP-IoT-Solution includes:


* Device drivers for sensors, display, audio, GUI, input, actuators, etc.
* Framework and documentation for low power, security, storage, etc.
* Guide for Espressif open source solutions from practical application point.


This solution is available on Github: ESP-IoT-Solution on GitHub.




## ESP-Protocols


The ESP-Protocols repository contains a collection of protocol components for ESP-IDF. The code within ESP-Protocols is organized into separate components, allowing them to be easily integrated into an ESP-IDF project. Additionally, each component is available in IDF Component Registry.


ESP-Protocols components:


* esp_modem enables connectivity with GSM/LTE modems using AT commands or PPP protocol. See the esp_modem documentation.
* mdns (mDNS) is a multicast UDP service that is used to provide local network service and host discovery. See the mdns documentation.
* esp_websocket_client is a managed component for ESP-IDF that contains implementation of WebSocket protocol client for ESP32. See the esp_websocket_client documentation. For detailes of WebSocket protocol client, see WebSocket_protocol_client.
* asio is a cross-platform C++ library, see https://think-async.com/Asio/. It provides a consistent asynchronous model using a modern C++ approach. See the asio documentation.




## ESP-BSP


The ESP-BSP repository contains Board Support Packages (BSPs) for various Espressif's and third-party development boards. BSPs help to quickly get started with a supported board. Usually they contain pinout definition and helper functions that will initialize peripherals for the specific board. Additionally, the BSPs contain drivers for external chips populated on the development board, such as sensors, displays, audio codecs, etc.




## ESP-IDF-CXX


ESP-IDF-CXX contains C++ wrappers for part of ESP-IDF. The focuses are on ease of use, safety, automatic resource management. They also move error checking from runtime to compile time to prevent running failure. There are C++ classes for ESP-Timer, I2C, SPI, GPIO and other peripherals or features of ESP-IDF. ESP-IDF-CXX is available as a component from the component registry. Please check the project's README.md for more information.