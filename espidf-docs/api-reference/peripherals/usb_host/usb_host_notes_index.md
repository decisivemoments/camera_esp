# USB Host Maintainers Notes (Introduction)



This document contains information regarding the implementation details of the USB Host stack. This document is intended for the maintainers and third-party contributors of the USB Host stack. Users of the USB Host stack should refer to USB Host instead.



Warning


The implementations details of the USB Host stack is categorized as private API. Thus, all layers (other than the USB Host Library) do not adhere to ESP-IDF's versioning scheme (i.e., breaking changes are permitted).




!Diagram of Host Stack Layers

This document is split into the following sections:



* USB Host Maintainers Notes (Design Guidelines)
* USB Host Maintainers Notes (Architecture)
* USB Host Maintainers Notes (DWC_OTG Controller)
* USB Host Driver (USBH)



Todo:


* USB Host Maintainers Notes (HAL & LL)
* USB Host Maintainers Notes (HCD)
* USB Host Maintainers Notes (Hub)
* USB Host Maintainers Notes (USB Host Library)



## Introduction


The ESP-IDF USB Host Stack allows the ESP32-S3 to operate as a USB Host. Operating as a USB Host allows the ESP32-S3 to communicate with a wide range of USB devices. However, most USB Host Stack implementations do not run on embedded hardware (i.e., runs on PCs and smartphones), thus have comparatively more resources (i.e., memory and CPU speed).


The implementation of the ESP-IDF USB Host Stack (henceforth referred to as the Host Stack) takes into account the embedded nature of the ESP32-S3 which is reflected in various aspects of the Host Stack's design.



### Features & Limitations


**The Host Stack currently supports the following notable features:**


* Supports FS (Full Speed) and LS (Low Speed) devices
* Supports all transfer types (Control, Bulk, Isochronous, and Interrupt)
* Automatically enumerates connected devices
* Allows multiple class drivers (i.e., Clients of the USB Host Library) to run simultaneously and share the same device (i.e., composite devices)


**The Host Stack currently has the following notable limitations:**


* No HS (High Speed) support
* No Hub support (currently only supports a single device)