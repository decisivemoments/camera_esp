# NimBLE-based Host APIs



## Overview


Apache MyNewt NimBLE is a highly configurable and Bluetooth® SIG qualifiable Bluetooth Low Energy (Bluetooth LE) stack providing both host and controller functionalities. ESP-IDF supports NimBLE host stack which is specifically ported for ESP32 platform and FreeRTOS. The underlying controller is still the same (as in case of Bluedroid) providing VHCI interface. Refer to NimBLE user guide for a complete list of features and additional information on NimBLE stack. Most features of NimBLE including Bluetooth Low Energy Mesh are supported by ESP-IDF. The porting layer is kept cleaner by maintaining all the existing APIs of NimBLE along with a single ESP-NimBLE API for initialization, making it simpler for the application developers.




## Architecture


Currently, NimBLE host and controller support different transports such as UART and RAM between them. However, RAM transport cannot be used as is in case of ESP as ESP controller supports VHCI interface and buffering schemes used by NimBLE host is incompatible with that used by ESP controller. Therefore, a new transport between NimBLE host and ESP controller has been added. This is depicted in the figure below. This layer is responsible for maintaining pool of transport buffers and formatting buffers exchanges between host and controller as per the requirements.



![ESP NimBLE Stack.](../../../_images/esp32_nimble_stack.png)



ESP NimBLE Stack








## Threading Model


The NimBLE host can run inside the application thread or can have its own independent thread. This flexibility is inherently provided by NimBLE design. By default, a thread is spawned by the porting function `nimble_port_freertos_init`. This behavior can be changed by overriding the same function. For Bluetooth Low Energy Mesh, additional thread (advertising thread) is used which keeps on feeding advertisement events to the main thread.




## Programming Sequence


To begin with, make sure that the NimBLE stack is enabled from menuconfig choose NimBLE for the Bluetooth host.



Typical programming sequence with NimBLE stack consists of the following steps:* Initialize NVS flash using `nvs_flash_init()` API. This is because ESP controller uses NVS during initialization.
* Initialize the host and controller stack using `nimble_port_init`.
* Initialize the required NimBLE host configuration parameters and callbacks
* Perform application specific tasks/initialization
* Run the thread for host stack using `nimble_port_freertos_init`




This documentation does not cover NimBLE APIs. Refer to NimBLE tutorial for more details on the programming sequence/NimBLE APIs for different scenarios.




## API Reference



### Header File


* components/bt/host/nimble/esp-hci/include/esp_nimble_hci.h
* This header file can be included with:



> ```
> #include "esp_nimble_hci.h"
> 
> ```
* This header file is a part of the API provided by the `bt` component. To declare that your component depends on `bt`, add the following to your CMakeLists.txt:



> ```
> REQUIRES bt
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES bt
> 
> ```




### Functions


#### esp_nimble_hci_init


```


esp_err_t esp_nimble_hci_init(void)
Initialize VHCI transport layer between NimBLE Host and ESP Bluetooth controller. 
This function initializes the transport buffers to be exchanged between NimBLE host and ESP controller. It also registers required host callbacks with the controller.

Returns

ESP_OK if the initialization is successful
Appropriate error code from esp_err_t in case of an error 





```

#### esp_nimble_hci_deinit


```


esp_err_t esp_nimble_hci_deinit(void)
Deinitialize VHCI transport layer between NimBLE Host and ESP Bluetooth controller. 

Note
This function should be called after the NimBLE host is deinitialized.


Returns

ESP_OK if the deinitialization is successful
Appropriate error codes from esp_err_t in case of an error 





```



### Macros


#### BLE_HCI_UART_H4_NONE


```


BLE_HCI_UART_H4_NONE

```

#### BLE_HCI_UART_H4_CMD


```


BLE_HCI_UART_H4_CMD

```

#### BLE_HCI_UART_H4_ACL


```


BLE_HCI_UART_H4_ACL

```

#### BLE_HCI_UART_H4_SCO


```


BLE_HCI_UART_H4_SCO

```

#### BLE_HCI_UART_H4_EVT


```


BLE_HCI_UART_H4_EVT

```