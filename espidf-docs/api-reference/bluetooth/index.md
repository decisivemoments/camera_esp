# Bluetooth® API




* Bluetooth® Common
	+ Bluetooth Define
	+ Bluetooth Main
	+ Bluetooth Device
* Bluetooth® Low Energy (Bluetooth LE)
	+ Bluetooth Low Energy GAP
	+ Bluetooth Low Energy GATT Define
	+ Bluetooth Low Energy GATT Server
	+ Bluetooth Low Energy GATT Client
	+ Bluetooth Low Energy BluFi
* Controller && VHCI
	+ Application Example
	+ API Reference
* ESP-BLE-MESH
	+ Application Examples and Demos
	+ API Reference
	+ ESP-BLE-MESH Definitions
	+ ESP-BLE-MESH Core API Reference
	+ ESP-BLE-MESH Models API Reference
	+ ESP-BLE-MESH (v1.1) Core API Reference
* NimBLE-based Host APIs
	+ Overview
	+ Architecture
	+ Threading Model
	+ Programming Sequence
	+ API Reference



ESP-IDF currently supports two host stacks. The Bluedroid based stack (default) supports classic Bluetooth as well as Bluetooth Low Energy (Bluetooth LE). On the other hand, Apache NimBLE based stack is Bluetooth Low Energy only. For users to make a choice:


* For usecases involving classic Bluetooth as well as Bluetooth Low Energy, Bluedroid should be used.
* For Bluetooth Low Energy-only usecases, using NimBLE is recommended. It is less demanding in terms of code footprint and runtime memory, making it suitable for such scenarios.


Code examples for this API section are provided in the bluetooth/bluedroid directory of ESP-IDF examples.


The following examples contain detailed walkthroughs:


* GATT Client Example Walkthrough
* GATT Server Service Table Example Walkthrough
* GATT Server Example Walkthrough
* GATT Security Client Example Walkthrough
* GATT Security Server Example Walkthrough
* GATT Client Multi-connection Example Walkthrough