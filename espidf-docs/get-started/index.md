# Get Started



This document is intended to help you set up the software development environment for the hardware based on the ESP32-S3 chip by Espressif. After that, a simple example will show you how to use ESP-IDF (Espressif IoT Development Framework) for menu configuration, then for building and flashing firmware onto an ESP32-S3 board.



Note


This is documentation for stable version v5.3.1 of ESP-IDF. Other ESP-IDF Versions are also available.




## Introduction


ESP32-S3 is a system on a chip that integrates the following features:


* Wi-Fi (2.4 GHz band)
* Bluetooth Low Energy
* Dual high performance Xtensa® 32-bit LX7 CPU cores
* Ultra Low Power co-processor running either RISC-V or FSM core
* Multiple peripherals
* Built-in security hardware
* USB OTG interface
* USB Serial/JTAG Controller


Powered by 40 nm technology, ESP32-S3 provides a robust, highly integrated platform, which helps meet the continuous demands for efficient power usage, compact design, security, high performance, and reliability.


Espressif provides basic hardware and software resources to help application developers realize their ideas using the ESP32-S3 series hardware. The software development framework by Espressif is intended for development of Internet-of-Things (IoT) applications with Wi-Fi, Bluetooth, power management and several other system features.




## What You Need



### Hardware


* An **ESP32-S3** board.
* **USB cable** - USB A / micro USB B.
* **Computer** running Windows, Linux, or macOS.



Note


Currently, some of the development boards are using USB Type C connectors. Be sure you have the correct cable to connect your board!



If you have one of ESP32-S3 official development boards listed below, you can click on the link to learn more about the hardware.



* ESP32-S3-DevKitC-1
* ESP32-S3-DevKitM-1





### Software


To start using ESP-IDF on **ESP32-S3**, install the following software:



> * **Toolchain** to compile code for ESP32-S3
> * **Build tools** - CMake and Ninja to build a full **Application** for ESP32-S3
> * **ESP-IDF** that essentially contains API (software libraries and source code) for ESP32-S3 and scripts to operate the **Toolchain**



!Development of applications for ESP32-S3




## Installation


To install all the required software, we offer some different ways to facilitate this task. Choose from one of the available options.



### IDE



Note


We highly recommend installing the ESP-IDF through your favorite IDE.



* Eclipse Plugin
* VSCode Extension




### Manual Installation


For the manual procedure, please select according to your operating system.



* Windows Installer
* Linux and macOS






## Build Your First Project


If you already have the ESP-IDF installed and are not using an IDE, you can build your first project from the command line following the Start a Project on Windows or Start a Project on Linux and macOS.




## Uninstall ESP-IDF


If you want to remove ESP-IDF, please follow Uninstall ESP-IDF.