# USB Host



The document provides information regarding the USB Host Library. This document is split into the following sections:



Sections


* USB Host


	+ Overview
	+ Architecture
	+ Usage
	+ Examples
	+ Host Stack Configuration
	+ API Reference
	+ Maintainers Notes




## Overview


The USB Host Library (hereinafter referred to as the Host Library) is the lowest layer of the USB Host stack that exposes a public facing API. In most cases, applications that require USB Host functionality do not need to interface with the Host Library directly. Instead, most applications use the API provided by a host class driver that is implemented on top of the Host Library.


However, you may want to use the Host Library directly for some of (but not limited to) the following reasons:


* Implementation of a custom host class driver
* Usage of lower level USB Host API



### Features & Limitations


The Host Library has the following features:


* Supports Full Speed (FS) and Low Speed (LS) Devices.
* Supports all four transfer types, i.e., Control, Bulk, Interrupt, and Isochronous.
* Allows multiple class drivers to run simultaneously, i.e., multiple clients of the Host Library.
* A single device can be used by multiple clients simultaneously, e.g., composite devices.
* The Host Library itself and the underlying Host Stack does not internally instantiate any OS tasks. The number of tasks is entirely controlled by how the Host Library interface is used. However, a general rule of thumb regarding the number of tasks is `(the number of host class drivers running + 1)`.


Currently, the Host Library and the underlying Host Stack has the following limitations:


* Only supports a single device, but the Host Library's API is designed for multiple device support.
* Only supports Asynchronous transfers.
* Only supports using the first configuration found. Changing to other configurations is not supported yet.
* Transfer timeouts are not supported yet.





## Architecture



!Diagram of the Key Entities of USB Host Functionality



Diagram of the key entities involved in USB Host functionality






The diagram above shows the key entities that are involved when implementing USB Host functionality. These entities are:


* The **Host Library**
* **Clients** of the Host Library
* **Devices**
* Host Library **Daemon Task**



### Host Library


The Host Library is the lowest public-facing API layer of the ESP-IDF USB Host Stack. Any other ESP-IDF component (such as a class driver or a user component) that needs to communicate with a connected USB device can only do so using the Host Library API either directly or indirectly.


The Host Library's API is split into two sub-sets, namely the **Library API** and **Client API**.


* The Client API handles the communication between a client of the Host Library and one or more USB devices. The Client API should only be called by registered clients of the Host Library.
* The Library API handles all of the Host Library processing that is not specific to a single client, such as device enumeration. Usually, the library API is called by a Host Library Daemon Task.




### Clients


A client of the Host Library is a software component, such as a host class driver or user component, which utilizes the Host Library to establish communication with a USB device. Generally, each client has a one-to-one relation with a task. This implies that all Client API calls pertaining to a specific client must originate from the context of the same task.


By organizing the software components that use the Host Library's into clients, the Host Library can delegate the handling of all events specific to that client to the client's task. In other words, each client task is responsible for all the required processing and event handling associated with the USB communication that the client initiates.




### Daemon Task


Although the Host Library delegates the handling of client events to the clients themselves, there are still Library events – events that are not specific to any particular client – that need to be handled. Library event handling can include things such as:


* Handling USB device connection, enumeration, and disconnection
* Rerouting control transfers to/from clients
* Forwarding events to clients


Therefore, in addition to the client tasks, the Host Library also requires a task, which is usually the Host Library Daemon Task, to handle all of the library events.




### Devices


The Host Library shields clients from the details of device handling, encompassing details such as connection, memory allocation, and enumeration. The clients are provided only with a list of already connected and enumerated devices to choose from. During enumeration, each device is automatically configured to use the first configuration found, namely, the first configuration descriptor returned on a Get Configuration Descriptor request. For most standard devices, the first configuration will have a `bConfigurationValue` of `1`.


It is possible for two or more clients to simultaneously communicate with the same device as long as they are not communicating to the same interface. However, multiple clients can simultaneously communicate with the same device's default endpoint (i.e., EP0), which will result in their control transfers being serialized.


For a client to communicate with a device, the client must:


1. Open the device using the device's address. This lets the Host Library know that the client is using that device.
2. Claim the interface(s) that will be used for communication. This prevents other clients from claiming the same interface(s).
3. Send transfers to the endpoints of claimed interfaces. The client's task is responsible for handling its own processing and events related to USB device communication.





## Usage


The Host Library and the underlying Host Stack will not create any tasks. All tasks, namely the client tasks and the Daemon Task, need to be created by the class drivers or the user. Instead, the Host Library provides two event handler functions that handle all of the required Host Library processing, thus these functions should be called repeatedly from the client tasks and the Daemon Task. Therefore, the implementation of client tasks and the Daemon Task will be largely centered around the invocation of these event handler functions.



### Host Library & Daemon Task



#### Basic Usage


The Host Library API provides `usb_host_lib_handle_events()` to handle library events. This function should be called repeatedly, typically from the Daemon Task. Some notable features regarding `usb_host_lib_handle_events()` are:


* The function can block until a library event needs handling.
* Event flags are returned on each invocation. These event flags are useful for knowing when the Host Library can be uninstalled.


A bare-bones Daemon Task would resemble something like the following code snippet:



```
#include "usb/usb_host.h"

void daemon_task(void *arg)
{
    ...
    bool exit = false;
    while (!exit) {
        uint32_t event_flags;
        usb_host_lib_handle_events(portMAX_DELAY, &event_flags);
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
            ...
        }
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
            ...
        }
        ...
    }
    ...
}

```



Note


See the peripherals/usb/host/usb_host_lib example for full implementation of the Daemon Task.





#### Lifecycle



!Graph of Typical USB Host Library Lifecycle



Graph of Typical USB Host Library Lifecycle






The graph above illustrates the typical lifecycle of the Host Library with multiple clients and devices. Specifically, the example involves:


* two registered clients (Client 1 and Client 2).
* two connected devices (Device 1 and Device 2), where Client 1 communicates with Device 1 and Client 2 communicates with Device 2.


With reference to the graph above, the typical lifecycle involves the following key stages.


1. The Host Library is installed by calling `usb_host_install()`.
	* Installation must be done before any other Host Library API is called.
	* Where `usb_host_install()` is called (e.g., from the Daemon Task or another task) depends on the synchronization logic between the Daemon Task, client tasks, and the rest of the system.
2. Once the Host Library is installed, the clients can be registered by calling `usb_host_client_register()`.
	* This is typically called from the client task, where the client task waits for a signal from the Daemon Task.
	* This can be called elsewhere if necessary as long it is called after `usb_host_install()`.
3. Device 1 connects and is then enumerated.
	* Each registered client (in this case Client 1 and Client 2) is notified of the new device by way of the `USB_HOST_CLIENT_EVENT_NEW_DEV` event.
	* Client 1 opens Device 1 and begins communication with it.
4. Similarly Device 2 connects and is enumerated.
	* Client 1 and 2 are notified of a new device via a `USB_HOST_CLIENT_EVENT_NEW_DEV` event.
	* Client 2 opens Device 2 and begins communication with it.
5. Device 1 suddenly disconnects.
	* Client 1 is notified by way of `USB_HOST_CLIENT_EVENT_DEV_GONE` and begins its cleanup.
	* Client 2 is not notified as it has not opened Device 1.
6. Client 1 completes its cleanup and deregisters by calling `usb_host_client_deregister()`.
	* This is typically called from the client task before the task exits.
	* This can be called elsewhere if necessary as long as Client 1 has already completed its cleanup.
7. Client 2 completes its communication with Device 2. Client 2 then closes Device 2 and deregisters itself.
	* The Daemon Task is notified of the deregistration of all clients by way the `USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS` event flag as Client 2 is the last client to deregister.
	* Device 2 is still allocated (i.e., not freed), as it is still connected albeit not currently opened by any client.
8. The Daemon Task decides to clean up as there are no more clients.
	* The Daemon Task must free Device 2 first by calling `usb_host_device_free_all()`.
	* If `usb_host_device_free_all()` was able to free all devices, the function will return ESP_OK indicating that all devices have been freed.
	* If `usb_host_device_free_all()` was unable to free all devices for reasons like the device is still opened by a client, the function will return ESP_ERR_NOT_FINISHED.
	* The Daemon Task must wait for `usb_host_lib_handle_events()` to return the `USB_HOST_LIB_EVENT_FLAGS_ALL_FREE` event flag in order to know when all devices have been freed.
9. Once the Daemon Task has verified that all clients have deregistered and all devices have been freed, it can now uninstall the Host Library by calling `usb_host_uninstall()`.





### Clients & Class Driver



#### Basic Usage


The Host Library API provides `usb_host_client_handle_events()` to handle a particular client's events. This function should be called repeatedly, typically from the client's task. Some notable features regarding `usb_host_client_handle_events()` are:


* The function can block until a client event needs handling.
* The function's primary purpose is to call the various event handling callbacks when a client event occurs.


The following callbacks are called from within `usb_host_client_handle_events()` thus allowing the client task to be notified of events.


* The client event callback of type `usb_host_client_event_cb_t` delivers client event messages to the client. Client event messages indicate events such as the addition or removal of a device.
* The USB transfer completion callback of type `usb_transfer_cb_t` indicates that a particular USB transfer previously submitted by the client has been completed.



Note


Given that the callbacks are called from within `usb_host_client_handle_events()`, users should avoid blocking from within the callbacks as this will result in `usb_host_client_handle_events()` being blocked as well, thus preventing other pending client events from being handled.



The following code snippet demonstrates a bare-bones host class driver and its client task. The code snippet contains:


* A simple client task function `client_task` that calls `usb_host_client_handle_events()` in a loop.
* Implementations of a client event callback and transfer completion callbacks.
* Implementation of a simple state machine for the class driver. The class driver simply opens a device, sends an OUT transfer to EP1, then closes the device.



```
#include <string.h>
#include "usb/usb_host.h"

#define CLASS_DRIVER_ACTION_OPEN_DEV    0x01
#define CLASS_DRIVER_ACTION_TRANSFER    0x02
#define CLASS_DRIVER_ACTION_CLOSE_DEV   0x03

struct class_driver_control {
    uint32_t actions;
    uint8_t dev_addr;
    usb_host_client_handle_t client_hdl;
    usb_device_handle_t dev_hdl;
};

static void client_event_cb(const usb_host_client_event_msg_t *event_msg, void *arg)
{
    // This function is called from within usb_host_client_handle_events(). Do not block and try to keep it short
    struct class_driver_control *class_driver_obj = (struct class_driver_control *)arg;
    switch (event_msg->event) {
        case USB_HOST_CLIENT_EVENT_NEW_DEV:
            class_driver_obj->actions |= CLASS_DRIVER_ACTION_OPEN_DEV;
            class_driver_obj->dev_addr = event_msg->new_dev.address; //Store the address of the new device
            break;
        case USB_HOST_CLIENT_EVENT_DEV_GONE:
            class_driver_obj->actions |= CLASS_DRIVER_ACTION_CLOSE_DEV;
            break;
        default:
            break;
    }
}

static void transfer_cb(usb_transfer_t *transfer)
{
    // This function is called from within usb_host_client_handle_events(). Do not block and try to keep it short
    struct class_driver_control *class_driver_obj = (struct class_driver_control *)transfer->context;
    printf("Transfer status %d, actual number of bytes transferred %d\n", transfer->status, transfer->actual_num_bytes);
    class_driver_obj->actions |= CLASS_DRIVER_ACTION_CLOSE_DEV;
}

void client_task(void *arg)
{
    ... // Wait until Host Library is installed
    // Initialize class driver objects
    struct class_driver_control class_driver_obj = {0};
    // Register the client
    usb_host_client_config_t client_config = {
        .is_synchronous = false,
        .max_num_event_msg = 5,
        .async = {
            .client_event_callback = client_event_cb,
            .callback_arg = &class_driver_obj,
        }
    };
    usb_host_client_register(&client_config, &class_driver_obj.client_hdl);
    //Allocate a USB transfer
    usb_transfer_t *transfer;
    usb_host_transfer_alloc(1024, 0, &transfer);

    //Event handling loop
    bool exit = false;
    while (!exit) {
        // Call the client event handler function
        usb_host_client_handle_events(class_driver_obj.client_hdl, portMAX_DELAY);
        // Execute pending class driver actions
        if (class_driver_obj.actions & CLASS_DRIVER_ACTION_OPEN_DEV) {
            // Open the device and claim interface 1
            usb_host_device_open(class_driver_obj.client_hdl, class_driver_obj.dev_addr, &class_driver_obj.dev_hdl);
            usb_host_interface_claim(class_driver_obj.client_hdl, class_driver_obj.dev_hdl, 1, 0);
        }
        if (class_driver_obj.actions & CLASS_DRIVER_ACTION_TRANSFER) {
            // Send an OUT transfer to EP1
            memset(transfer->data_buffer, 0xAA, 1024);
            transfer->num_bytes = 1024;
            transfer->device_handle = class_driver_obj.dev_hdl;
            transfer->bEndpointAddress = 0x01;
            transfer->callback = transfer_cb;
            transfer->context = (void *)&class_driver_obj;
            usb_host_transfer_submit(transfer);
        }
        if (class_driver_obj.actions & CLASS_DRIVER_ACTION_CLOSE_DEV) {
            // Release the interface and close the device
            usb_host_interface_release(class_driver_obj.client_hdl, class_driver_obj.dev_hdl, 1);
            usb_host_device_close(class_driver_obj.client_hdl, class_driver_obj.dev_hdl);
            exit = true;
        }
        ... // Handle any other actions required by the class driver
    }

    // Cleanup class driver
    usb_host_transfer_free(transfer);
    usb_host_client_deregister(class_driver_obj.client_hdl);
    ... // Delete the client task. Signal the Daemon Task if necessary.
}

```



Note


An actual host class driver is likely to support many more features, thus will have a much more complex state machine. A host class driver is also likely to need to:


* Be able to open multiple devices
* Parse an opened device's descriptors to identify if the device is of the target class
* Communicate with multiple endpoints of an interface in a particular order
* Claim multiple interfaces of a device
* Handle various errors





#### Lifecycle


The typical life cycle of a client task and class driver will go through the following stages:


1. Wait for some signal regarding the Host Library being installed.
2. Register the client via `usb_host_client_register()` and allocate any other class driver resources, such as allocating transfers using `usb_host_transfer_alloc()`.
3. For each new device that the class driver needs to communicate with:



> 1. Check if the device is already connected via `usb_host_device_addr_list_fill()`.
> 	2. If the device is not already connected, wait for a `USB_HOST_CLIENT_EVENT_NEW_DEV` event from the client event callback.
> 	3. Open the device via `usb_host_device_open()`.
> 	4. Parse the device and configuration descriptors via `usb_host_get_device_descriptor()` and `usb_host_get_active_config_descriptor()` respectively.
> 	5. Claim the necessary interfaces of the device via `usb_host_interface_claim()`.
4. Submit transfers to the device via `usb_host_transfer_submit()` or `usb_host_transfer_submit_control()`.
5. Once an opened device is no longer needed by the class driver, or has disconnected, as indicated by a `USB_HOST_CLIENT_EVENT_DEV_GONE` event:



> 1. Stop any previously submitted transfers to the device's endpoints by calling `usb_host_endpoint_halt()` and `usb_host_endpoint_flush()` on those endpoints.
> 	2. Release all previously claimed interfaces via `usb_host_interface_release()`.
> 	3. Close the device via `usb_host_device_close()`.
6. Deregister the client via `usb_host_client_deregister()` and free any other class driver resources.
7. Delete the client task. Signal the Daemon Task if necessary.






## Examples



### Host Library Examples


The peripherals/usb/host/usb_host_lib demonstrates basic usage of the USB Host Library's API to implement a pseudo-class driver.




### Class Driver Examples


The USB Host Stack provides a number of examples that implement host class drivers using the Host Library's API.



#### CDC-ACM


* A host class driver for the Communication Device Class (Abstract Control Model) is distributed as a managed component via the ESP-IDF Component Registry.
* The peripherals/usb/host/cdc/cdc_acm_host example uses the CDC-ACM host driver component to communicate with CDC-ACM devices.
* The peripherals/usb/host/cdc/cdc_acm_vcp example shows how can you extend the CDC-ACM host driver to interface Virtual COM Port devices.
* The CDC-ACM driver is also used in esp_modem examples, where it is used for communication with cellular modems.




#### MSC


* A host class driver for the Mass Storage Class (Bulk-Only Transport) is deployed to ESP-IDF Component Registry.
* The peripherals/usb/host/msc example demonstrates the usage of the MSC host driver to read and write to a USB flash drive.




#### HID


* A host class driver for the HID (Human interface device) is distributed as a managed component via the ESP-IDF Component Registry.
* The peripherals/usb/host/hid example demonstrates the possibility to receive reports from a USB HID device with several interfaces.




#### UVC


* A host class driver for the USB Video Device Class is distributed as a managed component via the ESP-IDF Component Registry.
* The peripherals/usb/host/uvc example demonstrates the usage of the UVC host driver to receive a video stream from a USB camera and optionally forward that stream over Wi-Fi.






## Host Stack Configuration



### Non-Compliant Device Support


To support USB devices that are non-compliant in various scenarios or exhibit specific behaviors, it is possible to configure the USB Host stack.


As a USB device may be hot-plugged, it is essential to have configurable delays between power switching and device attachment, and when the device's internal power has stabilized.



#### Enumeration Configuration


During the process of enumerating connected USB devices, several delay values ensure the proper functioning of the device.



!USB Root Hub Power-on and Connection Events Timing



USB Root Hub Power-on and Connection Events Timing






The figure above shows all the delay values associated with both turning on port power with a device connected and hot-plugging a device.


* After a port is reset or resumed, the USB system software is expected to provide a "recovery" interval of 10 ms before the device attached to the port is expected to respond to data transfers.
* After the reset/resume recovery interval, if a device receives a `SetAddress()` request, the device must be able to complete processing of the request and be able to successfully complete the Status stage of the request within 50 ms.
* After successful completion of the Status stage, the device is allowed a `SetAddress()` recovery interval of 2 ms.



Note


For more details regarding connection event timings, please refer to USB 2.0 Specification > Chapter 7.1.7.3 *Connect and Disconnect Signaling*.



Configurable parameters of the USB host stack can be configured with multiple options via Menuconfig.


* For debounce delay, refer to CONFIG_USB_HOST_DEBOUNCE_DELAY_MS.
* For reset hold interval, refer to CONFIG_USB_HOST_RESET_HOLD_MS.
* For reset recovery interval, refer to CONFIG_USB_HOST_RESET_RECOVERY_MS.
* For `SetAddress()` recovery interval, refer to CONFIG_USB_HOST_SET_ADDR_RECOVERY_MS.





### Multiple Configuration Support


To support USB devices that have more than one configuration, it is possible to specify the desired configuration number during a device's enumeration process.



#### Enumeration Filter


The enumeration filter is a callback function of type `usb_host_enum_filter_cb_t` called at the beginning of the enumeration process once a device descriptor is read from a newly attached USB device. Consequently, the user is provided with the obtained device descriptor. Through this callback, the user can:


* Select the configuration of the USB device.
* Filter which USB devices should be enumerated.


To use the enumeration filter, users should enable the CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK option using menuconfig. Users can specify the callback by setting `usb_host_config_t::enum_filter_cb` which is then passed to the Host Library when calling `usb_host_install()`.






## API Reference


The API of the USB Host Library is separated into the following header files. However, it is sufficient for applications to only `#include "usb/usb_host.h"` and all USB Host Library headers will also be included.


* usb/include/usb/usb_host.h contains the functions and types of the USB Host Library.
* usb/include/usb/usb_helpers.h contains various helper functions that are related to the USB protocol such as descriptor parsing.
* usb/include/usb/usb_types_stack.h contains types that are used across multiple layers of the USB Host stack.
* usb/include/usb/usb_types_ch9.h contains types and macros related to Chapter 9 of the USB2.0 specification, i.e., descriptors and standard requests.



### Header File


* components/usb/include/usb/usb_host.h
* This header file can be included with:



> ```
> #include "usb/usb_host.h"
> 
> ```
* This header file is a part of the API provided by the `usb` component. To declare that your component depends on `usb`, add the following to your CMakeLists.txt:



> ```
> REQUIRES usb
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES usb
> 
> ```




### Functions


#### usb_host_install


```


esp_err_t usb_host_install(const usb_host_config_t *config)
Install the USB Host Library. 

This function should only once to install the USB Host Library
This function should be called before any other USB Host Library functions are called



Note
If skip_phy_setup is set in the install configuration, the user is responsible for ensuring that the underlying Host Controller is enabled and the USB PHY (internal or external) is already setup before this function is called. 


Parameters
config -- [in] USB Host Library configuration 

Returns
esp_err_t 



```

#### usb_host_uninstall


```


esp_err_t usb_host_uninstall(void)
Uninstall the USB Host Library. 

This function should be called to uninstall the USB Host Library, thereby freeing its resources
All clients must have been deregistered before calling this function
All devices must have been freed by calling usb_host_device_free_all() and receiving the USB_HOST_LIB_EVENT_FLAGS_ALL_FREE event flag



Note
If skip_phy_setup was set when the Host Library was installed, the user is responsible for disabling the underlying Host Controller and USB PHY (internal or external). 


Returns
esp_err_t 



```

#### usb_host_lib_handle_events


```


esp_err_t usb_host_lib_handle_events(TickType_t timeout_ticks, uint32_t *event_flags_ret)
Handle USB Host Library events. 

This function handles all of the USB Host Library's processing and should be called repeatedly in a loop
Check event_flags_ret to see if an flags are set indicating particular USB Host Library events
This function should never be called by multiple threads simultaneously



Note
This function can block 


Parameters

timeout_ticks -- [in] Timeout in ticks to wait for an event to occur 
event_flags_ret -- [out] Event flags that indicate what USB Host Library event occurred. 


Returns
esp_err_t 



```

#### usb_host_lib_unblock


```


esp_err_t usb_host_lib_unblock(void)
Unblock the USB Host Library handler. 

This function simply unblocks the USB Host Library event handling function (usb_host_lib_handle_events())



Returns
esp_err_t 



```

#### usb_host_lib_info


```


esp_err_t usb_host_lib_info(usb_host_lib_info_t *info_ret)
Get current information about the USB Host Library. 

Parameters
info_ret -- [out] USB Host Library Information 

Returns
esp_err_t 



```

#### usb_host_client_register


```


esp_err_t usb_host_client_register(const usb_host_client_config_t *client_config, usb_host_client_handle_t *client_hdl_ret)
Register a client of the USB Host Library. 

This function registers a client of the USB Host Library
Once a client is registered, its processing function usb_host_client_handle_events() should be called repeatedly



Parameters

client_config -- [in] Client configuration 
client_hdl_ret -- [out] Client handle 


Returns
esp_err_t 



```

#### usb_host_client_deregister


```


esp_err_t usb_host_client_deregister(usb_host_client_handle_t client_hdl)
Deregister a USB Host Library client. 

This function deregisters a client of the USB Host Library
The client must have closed all previously opened devices before attempting to deregister



Parameters
client_hdl -- [in] Client handle 

Returns
esp_err_t 



```

#### usb_host_client_handle_events


```


esp_err_t usb_host_client_handle_events(usb_host_client_handle_t client_hdl, TickType_t timeout_ticks)
USB Host Library client processing function. 

This function handles all of a client's processing and should be called repeatedly in a loop
For a particular client, this function should never be called by multiple threads simultaneously



Note
This function can block 


Parameters

client_hdl -- [in] Client handle 
timeout_ticks -- [in] Timeout in ticks to wait for an event to occur 


Returns
esp_err_t 



```

#### usb_host_client_unblock


```


esp_err_t usb_host_client_unblock(usb_host_client_handle_t client_hdl)
Unblock a client. 

This function simply unblocks a client if it is blocked on the usb_host_client_handle_events() function.
This function is useful when need to unblock a client in order to deregister it.



Parameters
client_hdl -- [in] Client handle 

Returns
esp_err_t 



```

#### usb_host_device_open


```


esp_err_t usb_host_device_open(usb_host_client_handle_t client_hdl, uint8_t dev_addr, usb_device_handle_t *dev_hdl_ret)
Open a device. 

This function allows a client to open a device
A client must open a device first before attempting to use it (e.g., sending transfers, device requests etc.)



Parameters

client_hdl -- [in] Client handle 
dev_addr -- [in] Device's address 
dev_hdl_ret -- [out] Device's handle 


Returns
esp_err_t 



```

#### usb_host_device_close


```


esp_err_t usb_host_device_close(usb_host_client_handle_t client_hdl, usb_device_handle_t dev_hdl)
Close a device. 

This function allows a client to close a device
A client must close a device after it has finished using the device (claimed interfaces must also be released)
A client must close all devices it has opened before deregistering



Note
This function can block 


Parameters

client_hdl -- [in] Client handle 
dev_hdl -- [in] Device handle 


Returns
esp_err_t 



```

#### usb_host_device_free_all


```


esp_err_t usb_host_device_free_all(void)
Indicate that all devices can be freed when possible. 

This function marks all devices as waiting to be freed
If a device is not opened by any clients, it will be freed immediately
If a device is opened by at least one client, the device will be free when the last client closes that device.
Wait for the USB_HOST_LIB_EVENT_FLAGS_ALL_FREE flag to be set by usb_host_lib_handle_events() in order to know when all devices have been freed
This function is useful when cleaning up devices before uninstalling the USB Host Library



Returns

ESP_ERR_NOT_FINISHED: There are one or more devices that still need to be freed. Wait for USB_HOST_LIB_EVENT_FLAGS_ALL_FREE event
ESP_OK: All devices already freed (i.e., there were no devices)
Other: Error 





```

#### usb_host_device_addr_list_fill


```


esp_err_t usb_host_device_addr_list_fill(int list_len, uint8_t *dev_addr_list, int *num_dev_ret)
Fill a list of device address. 

This function fills an empty list with the address of connected devices
The Device addresses can then used in usb_host_device_open()
If there are more devices than the list_len, this function will only fill up to list_len number of devices.



Parameters

list_len -- [in] Length of the empty list 
dev_addr_list -- [inout] Empty list to be filled 
num_dev_ret -- [out] Number of devices 


Returns
esp_err_t 



```

#### usb_host_device_info


```


esp_err_t usb_host_device_info(usb_device_handle_t dev_hdl, usb_device_info_t *dev_info)
Get device's information. 

This function gets some basic information of a device
The device must be opened first before attempting to get its information



Note
This function can block 


Parameters

dev_hdl -- [in] Device handle 
dev_info -- [out] Device information 


Returns
esp_err_t 



```

#### usb_host_get_device_descriptor


```


esp_err_t usb_host_get_device_descriptor(usb_device_handle_t dev_hdl, const usb_device_desc_t **device_desc)
Get device's device descriptor. 

A client must call usb_host_device_open() first
No control transfer is sent. The device's descriptor is cached on enumeration
This function simple returns a pointer to the cached descriptor



Note
No control transfer is sent. The device's descriptor is cached on enumeration 


Parameters

dev_hdl -- [in] Device handle 
device_desc -- [out] Device descriptor 


Returns
esp_err_t 



```

#### usb_host_get_active_config_descriptor


```


esp_err_t usb_host_get_active_config_descriptor(usb_device_handle_t dev_hdl, const usb_config_desc_t **config_desc)
Get device's active configuration descriptor. 

A client must call usb_host_device_open() first
No control transfer is sent. The device's active configuration descriptor is cached on enumeration
This function simple returns a pointer to the cached descriptor



Note
This function can block 


Note
No control transfer is sent. A device's active configuration descriptor is cached on enumeration 


Parameters

dev_hdl -- [in] Device handle 
config_desc -- [out] Configuration descriptor 


Returns
esp_err_t 



```

#### usb_host_get_config_desc


```


esp_err_t usb_host_get_config_desc(usb_host_client_handle_t client_hdl, usb_device_handle_t dev_hdl, uint8_t bConfigurationValue, const usb_config_desc_t **config_desc_ret)
Get get device's configuration descriptor. 

The USB Host library only caches a device's active configuration descriptor.
This function reads any configuration descriptor of a particular device (specified by bConfigurationValue).
This function will read the specified configuration descriptor via control transfers, and allocate memory to store that descriptor.
Users can call usb_host_free_config_desc() to free the descriptor's memory afterwards.



Note
This function can block 


Note
A client must call usb_host_device_open() on the device first 


Note
bConfigurationValue starts from index 1 


Parameters

client_hdl -- [in] Client handle - usb_host_client_handle_events() should be called repeatedly in a separate task to handle client events 
dev_hdl -- [in] Device handle 
config_desc_ret -- [out] Returned configuration descriptor 
bConfigurationValue -- [in] Index of device's configuration descriptor to be read 


Returns
esp_err_t 



```

#### usb_host_free_config_desc


```


esp_err_t usb_host_free_config_desc(const usb_config_desc_t *config_desc)
Free a configuration descriptor. 
This function frees a configuration descriptor that was returned by usb_host_get_config_desc()

Parameters
config_desc -- [out] Configuration descriptor 

Returns
esp_err_t 



```

#### usb_host_interface_claim


```


esp_err_t usb_host_interface_claim(usb_host_client_handle_t client_hdl, usb_device_handle_t dev_hdl, uint8_t bInterfaceNumber, uint8_t bAlternateSetting)
Function for a client to claim a device's interface. 

A client must claim a device's interface before attempting to communicate with any of its endpoints
Once an interface is claimed by a client, it cannot be claimed by any other client.



Note
This function can block 


Parameters

client_hdl -- [in] Client handle 
dev_hdl -- [in] Device handle 
bInterfaceNumber -- [in] Interface number 
bAlternateSetting -- [in] Interface alternate setting number 


Returns
esp_err_t 



```

#### usb_host_interface_release


```


esp_err_t usb_host_interface_release(usb_host_client_handle_t client_hdl, usb_device_handle_t dev_hdl, uint8_t bInterfaceNumber)
Function for a client to release a previously claimed interface. 

A client should release a device's interface after it no longer needs to communicate with the interface
A client must release all of its interfaces of a device it has claimed before being able to close the device



Note
This function can block 


Parameters

client_hdl -- [in] Client handle 
dev_hdl -- [in] Device handle 
bInterfaceNumber -- [in] Interface number 


Returns
esp_err_t 



```

#### usb_host_endpoint_halt


```


esp_err_t usb_host_endpoint_halt(usb_device_handle_t dev_hdl, uint8_t bEndpointAddress)
Halt a particular endpoint. 

The device must have been opened by a client
The endpoint must be part of an interface claimed by a client
Once halted, the endpoint must be cleared using usb_host_endpoint_clear() before it can communicate again



Note
This function can block 


Parameters

dev_hdl -- Device handle 
bEndpointAddress -- Endpoint address 


Returns
esp_err_t 



```

#### usb_host_endpoint_flush


```


esp_err_t usb_host_endpoint_flush(usb_device_handle_t dev_hdl, uint8_t bEndpointAddress)
Flush a particular endpoint. 

The device must have been opened by a client
The endpoint must be part of an interface claimed by a client
The endpoint must have been halted (either through a transfer error, or usb_host_endpoint_halt())
Flushing an endpoint will caused an queued up transfers to be canceled



Note
This function can block 


Parameters

dev_hdl -- Device handle 
bEndpointAddress -- Endpoint address 


Returns
esp_err_t 



```

#### usb_host_endpoint_clear


```


esp_err_t usb_host_endpoint_clear(usb_device_handle_t dev_hdl, uint8_t bEndpointAddress)
Clear a halt on a particular endpoint. 

The device must have been opened by a client
The endpoint must be part of an interface claimed by a client
The endpoint must have been halted (either through a transfer error, or usb_host_endpoint_halt())
If the endpoint has any queued up transfers, clearing a halt will resume their execution



Note
This function can block 


Parameters

dev_hdl -- Device handle 
bEndpointAddress -- Endpoint address 


Returns
esp_err_t 



```

#### usb_host_transfer_alloc


```


esp_err_t usb_host_transfer_alloc(size_t data_buffer_size, int num_isoc_packets, usb_transfer_t **transfer)
Allocate a transfer object. 

This function allocates a transfer object
Each transfer object has a fixed sized buffer specified on allocation
A transfer object can be re-used indefinitely
A transfer can be submitted using usb_host_transfer_submit() or usb_host_transfer_submit_control()



Parameters

data_buffer_size -- [in] Size of the transfer's data buffer 
num_isoc_packets -- [in] Number of isochronous packets in transfer (set to 0 for non-isochronous transfers) 
transfer -- [out] Transfer object 


Returns
esp_err_t 



```

#### usb_host_transfer_free


```


esp_err_t usb_host_transfer_free(usb_transfer_t *transfer)
Free a transfer object. 

Free a transfer object previously allocated using usb_host_transfer_alloc()
The transfer must not be in-flight when attempting to free it
If a NULL pointer is passed, this function will simply return ESP_OK



Parameters
transfer -- [in] Transfer object 

Returns
esp_err_t 



```

#### usb_host_transfer_submit


```


esp_err_t usb_host_transfer_submit(usb_transfer_t *transfer)
Submit a non-control transfer. 

Submit a transfer to a particular endpoint. The device and endpoint number is specified inside the transfer
The transfer must be properly initialized before submitting
On completion, the transfer's callback will be called from the client's usb_host_client_handle_events() function.



Parameters
transfer -- [in] Initialized transfer object 

Returns
esp_err_t 



```

#### usb_host_transfer_submit_control


```


esp_err_t usb_host_transfer_submit_control(usb_host_client_handle_t client_hdl, usb_transfer_t *transfer)
Submit a control transfer. 

Submit a control transfer to a particular device. The client must have opened the device first
The transfer must be properly initialized before submitting. The first 8 bytes of the transfer's data buffer should contain the control transfer setup packet
On completion, the transfer's callback will be called from the client's usb_host_client_handle_events() function.



Parameters

client_hdl -- [in] Client handle 
transfer -- [in] Initialized transfer object 


Returns
esp_err_t 



```



### Structures


#### usb_host_client_event_msg_t


```


struct usb_host_client_event_msg_t
Client event message. 
Client event messages are sent to each client of the USB Host Library in order to notify them of various USB Host Library events such as:
Addition of new devices
Removal of existing devices



Note
The event message structure has a union with members corresponding to each particular event. Based on the event type, only the relevant member field should be accessed. 


Public Members
event

usb_host_client_event_t event
Type of event 

address

uint8_t address
New device's address 

new_dev

struct usb_host_client_event_msg_t::[anonymous]::[anonymous] new_dev
New device info 

dev_hdl

usb_device_handle_t dev_hdl
The handle of the device that was gone 

dev_gone

struct usb_host_client_event_msg_t::[anonymous]::[anonymous] dev_gone
Gone device info 



```

#### usb_host_lib_info_t


```


struct usb_host_lib_info_t
Current information about the USB Host Library obtained via usb_host_lib_info() 

Public Members
num_devices

int num_devices
Current number of connected (and enumerated) devices 

num_clients

int num_clients
Current number of registered clients 



```

#### usb_host_config_t


```


struct usb_host_config_t
USB Host Library configuration. 
Configuration structure of the USB Host Library. Provided in the usb_host_install() function 

Public Members
skip_phy_setup

bool skip_phy_setup
If set, the USB Host Library will not configure the USB PHY thus allowing the user to manually configure the USB PHY before calling usb_host_install(). Users should set this if they want to use an external USB PHY. Otherwise, the USB Host Library will automatically configure the internal USB PHY 

intr_flags

int intr_flags
Interrupt flags for the underlying ISR used by the USB Host stack 

enum_filter_cb

usb_host_enum_filter_cb_t enum_filter_cb
Enumeration filter callback. Enable CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK to use this feature. Set to NULL otherwise. 



```

#### usb_host_client_config_t


```


struct usb_host_client_config_t
USB Host Library Client configuration. 
Configuration structure for a USB Host Library client. Provided in usb_host_client_register() 

Public Members
is_synchronous

bool is_synchronous
Whether the client is asynchronous or synchronous or not. Set to false for now. 

max_num_event_msg

int max_num_event_msg
Maximum number of event messages that can be stored (e.g., 3) 

client_event_callback

usb_host_client_event_cb_t client_event_callback
Client's event callback function 

callback_arg

void *callback_arg
Event callback function argument 

async

struct usb_host_client_config_t::[anonymous]::[anonymous] async
Async callback config 



```



### Macros


#### USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS


```


USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS
All clients have been deregistered from the USB Host Library 

```

#### USB_HOST_LIB_EVENT_FLAGS_ALL_FREE


```


USB_HOST_LIB_EVENT_FLAGS_ALL_FREE
The USB Host Library has freed all devices 

```



### Type Definitions


#### usb_host_client_handle_t


```


typedef struct usb_host_client_handle_s *usb_host_client_handle_t
Handle to a USB Host Library asynchronous client. 
An asynchronous client can be registered using usb_host_client_register()

Note
Asynchronous API 


```

#### usb_host_client_event_cb_t


```


typedef void (*usb_host_client_event_cb_t)(const usb_host_client_event_msg_t *event_msg, void *arg)
Client event callback. 

Each client of the USB Host Library must register an event callback to receive event messages from the USB Host Library.
The client event callback is run from the context of the clients usb_host_client_handle_events() function 



```



### Enumerations


#### usb_host_client_event_t


```


enum usb_host_client_event_t
The type event in a client event message. 
Values:
USB_HOST_CLIENT_EVENT_NEW_DEV

enumerator USB_HOST_CLIENT_EVENT_NEW_DEV
A new device has been enumerated and added to the USB Host Library 

USB_HOST_CLIENT_EVENT_DEV_GONE

enumerator USB_HOST_CLIENT_EVENT_DEV_GONE
A device opened by the client is now gone 


```



### Header File


* components/usb/include/usb/usb_helpers.h
* This header file can be included with:



> ```
> #include "usb/usb_helpers.h"
> 
> ```
* This header file is a part of the API provided by the `usb` component. To declare that your component depends on `usb`, add the following to your CMakeLists.txt:



> ```
> REQUIRES usb
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES usb
> 
> ```




### Functions


#### usb_parse_next_descriptor


```


const usb_standard_desc_t *usb_parse_next_descriptor(const usb_standard_desc_t *cur_desc, uint16_t wTotalLength, int *offset)
Get the next descriptor. 
Given a particular descriptor within a full configuration descriptor, get the next descriptor within the configuration descriptor. This is a convenience function that can be used to walk each individual descriptor within a full configuration descriptor.

Parameters

cur_desc -- [in] Current descriptor 
wTotalLength -- [in] Total length of the configuration descriptor 
offset -- [inout] Byte offset relative to the start of the configuration descriptor. On input, it is the offset of the current descriptor. On output, it is the offset of the returned descriptor. 


Returns
usb_standard_desc_t* Next descriptor, NULL if end of configuration descriptor reached 



```

#### usb_parse_next_descriptor_of_type


```


const usb_standard_desc_t *usb_parse_next_descriptor_of_type(const usb_standard_desc_t *cur_desc, uint16_t wTotalLength, uint8_t bDescriptorType, int *offset)
Get the next descriptor of a particular type. 
Given a particular descriptor within a full configuration descriptor, get the next descriptor of a particular type (i.e., using the bDescriptorType value) within the configuration descriptor.

Parameters

cur_desc -- [in] Current descriptor 
wTotalLength -- [in] Total length of the configuration descriptor 
bDescriptorType -- [in] Type of the next descriptor to get 
offset -- [inout] Byte offset relative to the start of the configuration descriptor. On input, it is the offset of the current descriptor. On output, it is the offset of the returned descriptor. 


Returns
usb_standard_desc_t* Next descriptor, NULL if end descriptor is not found or configuration descriptor reached 



```

#### usb_parse_interface_number_of_alternate


```


int usb_parse_interface_number_of_alternate(const usb_config_desc_t *config_desc, uint8_t bInterfaceNumber)
Get the number of alternate settings for a bInterfaceNumber. 
Given a particular configuration descriptor, for a particular bInterfaceNumber, get the number of alternate settings available for that interface (i.e., the max possible value of bAlternateSetting for that bInterfaceNumber).

Parameters

config_desc -- [in] Pointer to the start of a full configuration descriptor 
bInterfaceNumber -- [in] Interface number 


Returns
int The number of alternate settings that the interface has, -1 if bInterfaceNumber not found 



```

#### usb_parse_interface_descriptor


```


const usb_intf_desc_t *usb_parse_interface_descriptor(const usb_config_desc_t *config_desc, uint8_t bInterfaceNumber, uint8_t bAlternateSetting, int *offset)
Get a particular interface descriptor (using bInterfaceNumber and bAlternateSetting) 
Given a full configuration descriptor, get a particular interface descriptor.

Note
To get the number of alternate settings for a particular bInterfaceNumber, call usb_parse_interface_number_of_alternate()


Parameters

config_desc -- [in] Pointer to the start of a full configuration descriptor 
bInterfaceNumber -- [in] Interface number 
bAlternateSetting -- [in] Alternate setting number 
offset -- [out] Byte offset of the interface descriptor relative to the start of the configuration descriptor. Can be NULL. 


Returns
const usb_intf_desc_t* Pointer to interface descriptor, NULL if not found. 



```

#### usb_parse_endpoint_descriptor_by_index


```


const usb_ep_desc_t *usb_parse_endpoint_descriptor_by_index(const usb_intf_desc_t *intf_desc, int index, uint16_t wTotalLength, int *offset)
Get an endpoint descriptor within an interface descriptor. 
Given an interface descriptor, get the Nth endpoint descriptor of the interface. The number of endpoints in an interface is indicated by the bNumEndpoints field of the interface descriptor.

Note
If bNumEndpoints is 0, it means the interface uses the default endpoint only


Parameters

intf_desc -- [in] Pointer to thee start of an interface descriptor 
index -- [in] Endpoint index 
wTotalLength -- [in] Total length of the containing configuration descriptor 
offset -- [inout] Byte offset relative to the start of the configuration descriptor. On input, it is the offset of the interface descriptor. On output, it is the offset of the endpoint descriptor. 


Returns
const usb_ep_desc_t* Pointer to endpoint descriptor, NULL if not found. 



```

#### usb_parse_endpoint_descriptor_by_address


```


const usb_ep_desc_t *usb_parse_endpoint_descriptor_by_address(const usb_config_desc_t *config_desc, uint8_t bInterfaceNumber, uint8_t bAlternateSetting, uint8_t bEndpointAddress, int *offset)
Get an endpoint descriptor based on an endpoint's address. 
Given a configuration descriptor, get an endpoint descriptor based on it's bEndpointAddress, bAlternateSetting, and bInterfaceNumber.

Parameters

config_desc -- [in] Pointer to the start of a full configuration descriptor 
bInterfaceNumber -- [in] Interface number 
bAlternateSetting -- [in] Alternate setting number 
bEndpointAddress -- [in] Endpoint address 
offset -- [out] Byte offset of the endpoint descriptor relative to the start of the configuration descriptor. Can be NULL 


Returns
const usb_ep_desc_t* Pointer to endpoint descriptor, NULL if not found. 



```

#### usb_print_device_descriptor


```


void usb_print_device_descriptor(const usb_device_desc_t *devc_desc)
Print device descriptor. 

Parameters
devc_desc -- Device descriptor 



```

#### usb_print_config_descriptor


```


void usb_print_config_descriptor(const usb_config_desc_t *cfg_desc, print_class_descriptor_cb class_specific_cb)
Print configuration descriptor. 

This function prints the full contents of a configuration descriptor (including interface and endpoint descriptors)
When a non-standard descriptor is encountered, this function will call the class_specific_cb if it is provided



Parameters

cfg_desc -- Configuration descriptor 
class_specific_cb -- Class specific descriptor callback. Can be NULL 




```

#### usb_print_string_descriptor


```


void usb_print_string_descriptor(const usb_str_desc_t *str_desc)
Print a string descriptor. 
This funciton will only print ASCII characters of the UTF-16 encoded string

Parameters
str_desc -- String descriptor 



```

#### usb_round_up_to_mps


```


static inline int usb_round_up_to_mps(int num_bytes, int mps)
Round up to an integer multiple of endpoint's MPS. 
This is a convenience function to round up a size/length to an endpoint's MPS (Maximum packet size). This is useful when calculating transfer or buffer lengths of IN endpoints.
If MPS <= 0, this function will return 0
If num_bytes <= 0, this function will return 0



Parameters

num_bytes -- [in] Number of bytes 
mps -- [in] MPS 


Returns
int Round up integer multiple of MPS 



```



### Type Definitions


#### print_class_descriptor_cb


```


typedef void (*print_class_descriptor_cb)(const usb_standard_desc_t*)
Print class specific descriptor callback. 
Optional callback to be provided to usb_print_config_descriptor() function. The callback is called when when a non-standard descriptor is encountered. The callback should decode the descriptor as print it. 

```



### Header File


* components/usb/include/usb/usb_types_stack.h
* This header file can be included with:



> ```
> #include "usb/usb_types_stack.h"
> 
> ```
* This header file is a part of the API provided by the `usb` component. To declare that your component depends on `usb`, add the following to your CMakeLists.txt:



> ```
> REQUIRES usb
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES usb
> 
> ```




### Structures


#### usb_device_info_t


```


struct usb_device_info_t
Basic information of an enumerated device. 

Public Members
speed

usb_speed_t speed
Device's speed 

dev_addr

uint8_t dev_addr
Device's address 

bMaxPacketSize0

uint8_t bMaxPacketSize0
The maximum packet size of the device's default endpoint 

bConfigurationValue

uint8_t bConfigurationValue
Device's current configuration number 

str_desc_manufacturer

const usb_str_desc_t *str_desc_manufacturer
Pointer to Manufacturer string descriptor (can be NULL) 

str_desc_product

const usb_str_desc_t *str_desc_product
Pointer to Product string descriptor (can be NULL) 

str_desc_serial_num

const usb_str_desc_t *str_desc_serial_num
Pointer to Serial Number string descriptor (can be NULL) 



```

#### usb_isoc_packet_desc_t


```


struct usb_isoc_packet_desc_t
Isochronous packet descriptor. 
If the number of bytes in an Isochronous transfer is larger than the MPS of the endpoint, the transfer is split into multiple packets transmitted at the endpoint's specified interval. An array of Isochronous packet descriptors describes how an Isochronous transfer should be split into multiple packets. 

Public Members
num_bytes

int num_bytes
Number of bytes to transmit/receive in the packet. IN packets should be integer multiple of MPS 

actual_num_bytes

int actual_num_bytes
Actual number of bytes transmitted/received in the packet 

status

usb_transfer_status_t status
Status of the packet 



```

#### usb_transfer_s


```


struct usb_transfer_s
USB transfer structure. 

Public Members
data_buffer

uint8_t *const data_buffer
Pointer to data buffer 

data_buffer_size

const size_t data_buffer_size
Size of the data buffer in bytes 

num_bytes

int num_bytes
Number of bytes to transfer. Control transfers should include the size of the setup packet. Isochronous transfer should be the total transfer size of all packets. For non-control IN transfers, num_bytes should be an integer multiple of MPS. 

actual_num_bytes

int actual_num_bytes
Actual number of bytes transferred 

flags

uint32_t flags
Transfer flags 

device_handle

usb_device_handle_t device_handle
Device handle 

bEndpointAddress

uint8_t bEndpointAddress
Endpoint Address 

status

usb_transfer_status_t status
Status of the transfer 

timeout_ms

uint32_t timeout_ms
Timeout (in milliseconds) of the packet (currently not supported yet) 

callback

usb_transfer_cb_t callback
Transfer callback 

context

void *context
Context variable for transfer to associate transfer with something 

num_isoc_packets

const int num_isoc_packets
Only relevant to Isochronous. Number of service periods (i.e., intervals) to transfer data buffer over. 

isoc_packet_desc

usb_isoc_packet_desc_t isoc_packet_desc[]
Descriptors for each Isochronous packet 



```



### Macros


#### USB_TRANSFER_FLAG_ZERO_PACK


```


USB_TRANSFER_FLAG_ZERO_PACK
Terminate Bulk/Interrupt OUT transfer with a zero length packet. 
OUT transfers normally terminate when the Host has transferred the exact amount of data it needs to the device. However, for bulk and interrupt OUT transfers, if the transfer size just happened to be a multiple of MPS, it will be impossible to know the boundary between two consecutive transfers to the same endpoint.
Therefore, this flag will cause the transfer to automatically add a zero length packet (ZLP) at the end of the transfer if the following conditions are met:
The target endpoint is a Bulk/Interrupt OUT endpoint (Host to device)
The transfer's length (i.e., transfer.num_bytes) is a multiple of the endpoint's MPS


Otherwise, this flag has no effect.
Users should check whether their target device's class requires a ZLP, as not all Bulk/Interrupt OUT endpoints require them. For example:
For MSC Bulk Only Transport class, the Host MUST NEVER send a ZLP. Bulk transfer boundaries are determined by the CBW and CSW instead
For CDC Ethernet, the Host MUST ALWAYS send a ZLP if a segment (i.e., a transfer) is a multiple of MPS (See 3.3.1 Segment Delineation)



Note
See USB2.0 specification 5.7.3 and 5.8.3 for more details 


Note
IN transfers normally terminate when the Host as receive the exact amount of data it needs (must be multiple of MPS) or the endpoint sends a short packet to the Host (For bulk OUT only). Indicates that a bulk OUT transfers should always terminate with a short packet, even if it means adding an extra zero length packet 


```



### Type Definitions


#### usb_device_handle_t


```


typedef struct usb_device_handle_s *usb_device_handle_t
Handle of a USB Device connected to a USB Host. 

```

#### usb_host_enum_filter_cb_t


```


typedef bool (*usb_host_enum_filter_cb_t)(const usb_device_desc_t *dev_desc, uint8_t *bConfigurationValue)
Enumeration filter callback. 
This callback is called at the beginning of the enumeration process for a newly attached device. Through this callback, users are able to:

filter which devices should be enumerated
select the configuration number to use when enumerating the device


The device descriptor is passed to this callback to allow users to filter devices based on Vendor ID, Product ID, and class code.

AttentionThis callback must be non-blocking 

AttentionThis callback must not submit any USB transfers 




Param dev_desc
[in] Device descriptor of the device to enumerate 

Param bConfigurationValue
[out] Configuration number to use when enumerating the device (starts with 1)

Return
bool
true: USB device will be enumerated
false: USB device will not be enumerated 





```

#### usb_transfer_t


```


typedef struct usb_transfer_s usb_transfer_t
USB transfer structure. 
This structure is used to represent a transfer from a software client to an endpoint over the USB bus. Some of the fields are made const on purpose as they are fixed on allocation. Users should call the appropriate USB Host Library function to allocate a USB transfer structure instead of allocating this structure themselves.
The transfer type is inferred from the endpoint this transfer is sent to. Depending on the transfer type, users should note the following:

Bulk: This structure represents a single bulk transfer. If the number of bytes exceeds the endpoint's MPS, the transfer will be split into multiple MPS sized packets followed by a short packet.
Control: This structure represents a single control transfer. This first 8 bytes of the data_buffer must be filled with the setup packet (see usb_setup_packet_t). The num_bytes field should be the total size of the transfer (i.e., size of setup packet + wLength).
Interrupt: Represents an interrupt transfer. If num_bytes exceeds the MPS of the endpoint, the transfer will be split into multiple packets, and each packet is transferred at the endpoint's specified interval.
Isochronous: Represents a stream of bytes that should be transferred to an endpoint at a fixed rate. The transfer is split into packets according to the each isoc_packet_desc. A packet is transferred at each interval of the endpoint. If an entire ISOC URB was transferred without error (skipped packets do not count as errors), the URB's overall status and the status of each packet descriptor will be updated, and the actual_num_bytes reflects the total bytes transferred over all packets. If the ISOC URB encounters an error, the entire URB is considered erroneous so only the overall status will updated.



Note
For Bulk/Control/Interrupt IN transfers, the num_bytes must be a integer multiple of the endpoint's MPS 


Note
This structure should be allocated via usb_host_transfer_alloc() 


Note
Once the transfer has be submitted, users should not modify the structure until the transfer has completed 


```

#### usb_transfer_cb_t


```


typedef void (*usb_transfer_cb_t)(usb_transfer_t *transfer)
USB transfer completion callback. 

```



### Enumerations


#### usb_speed_t


```


enum usb_speed_t
USB Standard Speeds. 
Values:
USB_SPEED_LOW

enumerator USB_SPEED_LOW
USB Low Speed (1.5 Mbit/s) 

USB_SPEED_FULL

enumerator USB_SPEED_FULL
USB Full Speed (12 Mbit/s) 

USB_SPEED_HIGH

enumerator USB_SPEED_HIGH
USB High Speed (480 Mbit/s) 


```

#### usb_transfer_type_t


```


enum usb_transfer_type_t
The type of USB transfer. 

Note
The enum values need to match the bmAttributes field of an EP descriptor 

Values:
USB_TRANSFER_TYPE_CTRL

enumerator USB_TRANSFER_TYPE_CTRL

USB_TRANSFER_TYPE_ISOCHRONOUS

enumerator USB_TRANSFER_TYPE_ISOCHRONOUS

USB_TRANSFER_TYPE_BULK

enumerator USB_TRANSFER_TYPE_BULK

USB_TRANSFER_TYPE_INTR

enumerator USB_TRANSFER_TYPE_INTR


```

#### usb_transfer_status_t


```


enum usb_transfer_status_t
The status of a particular transfer. 
Values:
USB_TRANSFER_STATUS_COMPLETED

enumerator USB_TRANSFER_STATUS_COMPLETED
The transfer was successful (but may be short) 

USB_TRANSFER_STATUS_ERROR

enumerator USB_TRANSFER_STATUS_ERROR
The transfer failed because due to excessive errors (e.g. no response or CRC error) 

USB_TRANSFER_STATUS_TIMED_OUT

enumerator USB_TRANSFER_STATUS_TIMED_OUT
The transfer failed due to a time out 

USB_TRANSFER_STATUS_CANCELED

enumerator USB_TRANSFER_STATUS_CANCELED
The transfer was canceled 

USB_TRANSFER_STATUS_STALL

enumerator USB_TRANSFER_STATUS_STALL
The transfer was stalled 

USB_TRANSFER_STATUS_OVERFLOW

enumerator USB_TRANSFER_STATUS_OVERFLOW
The transfer as more data was sent than was requested 

USB_TRANSFER_STATUS_SKIPPED

enumerator USB_TRANSFER_STATUS_SKIPPED
ISOC packets only. The packet was skipped due to system latency or bus overload 

USB_TRANSFER_STATUS_NO_DEVICE

enumerator USB_TRANSFER_STATUS_NO_DEVICE
The transfer failed because the target device is gone 


```



### Header File


* components/usb/include/usb/usb_types_ch9.h
* This header file can be included with:



> ```
> #include "usb/usb_types_ch9.h"
> 
> ```
* This header file is a part of the API provided by the `usb` component. To declare that your component depends on `usb`, add the following to your CMakeLists.txt:



> ```
> REQUIRES usb
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES usb
> 
> ```




### Unions


#### usb_setup_packet_t


```


union usb_setup_packet_t

#include <usb_types_ch9.h>
Structure representing a USB control transfer setup packet. 
See Table 9-2 of USB2.0 specification for more details 

Public Members
bmRequestType

uint8_t bmRequestType
Characteristics of request 

bRequest

uint8_t bRequest
Specific request 

wValue

uint16_t wValue
Word-sized field that varies according to request 

wIndex

uint16_t wIndex
Word-sized field that varies according to request; typically used to pass an index or offset 

wLength

uint16_t wLength
Number of bytes to transfer if there is a data stage 

USB_DESC_ATTR

struct usb_setup_packet_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_SETUP_PACKET_SIZE]
Descriptor value 



```

#### usb_standard_desc_t


```


union usb_standard_desc_t

#include <usb_types_ch9.h>
USB standard descriptor. 
All USB standard descriptors start with these two bytes. Use this type when traversing over configuration descriptors 

Public Members
bLength

uint8_t bLength
Size of the descriptor in bytes 

bDescriptorType

uint8_t bDescriptorType
Descriptor Type 

USB_DESC_ATTR

struct usb_standard_desc_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_STANDARD_DESC_SIZE]
Descriptor value 



```

#### usb_device_desc_t


```


union usb_device_desc_t

#include <usb_types_ch9.h>
Structure representing a USB device descriptor. 
See Table 9-8 of USB2.0 specification for more details 

Public Members
bLength

uint8_t bLength
Size of the descriptor in bytes 

bDescriptorType

uint8_t bDescriptorType
DEVICE Descriptor Type 

bcdUSB

uint16_t bcdUSB
USB Specification Release Number in Binary-Coded Decimal (i.e., 2.10 is 210H) 

bDeviceClass

uint8_t bDeviceClass
Class code (assigned by the USB-IF) 

bDeviceSubClass

uint8_t bDeviceSubClass
Subclass code (assigned by the USB-IF) 

bDeviceProtocol

uint8_t bDeviceProtocol
Protocol code (assigned by the USB-IF) 

bMaxPacketSize0

uint8_t bMaxPacketSize0
Maximum packet size for endpoint zero (only 8, 16, 32, or 64 are valid) 

idVendor

uint16_t idVendor
Vendor ID (assigned by the USB-IF) 

idProduct

uint16_t idProduct
Product ID (assigned by the manufacturer) 

bcdDevice

uint16_t bcdDevice
Device release number in binary-coded decimal 

iManufacturer

uint8_t iManufacturer
Index of string descriptor describing manufacturer 

iProduct

uint8_t iProduct
Index of string descriptor describing product 

iSerialNumber

uint8_t iSerialNumber
Index of string descriptor describing the device’s serial number 

bNumConfigurations

uint8_t bNumConfigurations
Number of possible configurations 

USB_DESC_ATTR

struct usb_device_desc_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_DEVICE_DESC_SIZE]
Descriptor value 



```

#### usb_config_desc_t


```


union usb_config_desc_t

#include <usb_types_ch9.h>
Structure representing a short USB configuration descriptor. 
See Table 9-10 of USB2.0 specification for more details

Note
The full USB configuration includes all the interface and endpoint descriptors of that configuration. 


Public Members
bLength

uint8_t bLength
Size of the descriptor in bytes 

bDescriptorType

uint8_t bDescriptorType
CONFIGURATION Descriptor Type 

wTotalLength

uint16_t wTotalLength
Total length of data returned for this configuration 

bNumInterfaces

uint8_t bNumInterfaces
Number of interfaces supported by this configuration 

bConfigurationValue

uint8_t bConfigurationValue
Value to use as an argument to the SetConfiguration() request to select this configuration 

iConfiguration

uint8_t iConfiguration
Index of string descriptor describing this configuration 

bmAttributes

uint8_t bmAttributes
Configuration characteristics 

bMaxPower

uint8_t bMaxPower
Maximum power consumption of the USB device from the bus in this specific configuration when the device is fully operational. 

USB_DESC_ATTR

struct usb_config_desc_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_CONFIG_DESC_SIZE]
Descriptor value 



```

#### usb_iad_desc_t


```


union usb_iad_desc_t

#include <usb_types_ch9.h>
Structure representing a USB interface association descriptor. 

Public Members
bLength

uint8_t bLength
Size of the descriptor in bytes 

bDescriptorType

uint8_t bDescriptorType
INTERFACE ASSOCIATION Descriptor Type 

bFirstInterface

uint8_t bFirstInterface
Interface number of the first interface that is associated with this function 

bInterfaceCount

uint8_t bInterfaceCount
Number of contiguous interfaces that are associated with this function 

bFunctionClass

uint8_t bFunctionClass
Class code (assigned by USB-IF) 

bFunctionSubClass

uint8_t bFunctionSubClass
Subclass code (assigned by USB-IF) 

bFunctionProtocol

uint8_t bFunctionProtocol
Protocol code (assigned by USB-IF) 

iFunction

uint8_t iFunction
Index of string descriptor describing this function 

USB_DESC_ATTR

struct usb_iad_desc_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_IAD_DESC_SIZE]
Descriptor value 



```

#### usb_intf_desc_t


```


union usb_intf_desc_t

#include <usb_types_ch9.h>
Structure representing a USB interface descriptor. 
See Table 9-12 of USB2.0 specification for more details 

Public Members
bLength

uint8_t bLength
Size of the descriptor in bytes 

bDescriptorType

uint8_t bDescriptorType
INTERFACE Descriptor Type 

bInterfaceNumber

uint8_t bInterfaceNumber
Number of this interface. 

bAlternateSetting

uint8_t bAlternateSetting
Value used to select this alternate setting for the interface identified in the prior field 

bNumEndpoints

uint8_t bNumEndpoints
Number of endpoints used by this interface (excluding endpoint zero). 

bInterfaceClass

uint8_t bInterfaceClass
Class code (assigned by the USB-IF) 

bInterfaceSubClass

uint8_t bInterfaceSubClass
Subclass code (assigned by the USB-IF) 

bInterfaceProtocol

uint8_t bInterfaceProtocol
Protocol code (assigned by the USB) 

iInterface

uint8_t iInterface
Index of string descriptor describing this interface 

USB_DESC_ATTR

struct usb_intf_desc_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_INTF_DESC_SIZE]
Descriptor value 



```

#### usb_ep_desc_t


```


union usb_ep_desc_t

#include <usb_types_ch9.h>
Structure representing a USB endpoint descriptor. 
See Table 9-13 of USB2.0 specification for more details 

Public Members
bLength

uint8_t bLength
Size of the descriptor in bytes 

bDescriptorType

uint8_t bDescriptorType
ENDPOINT Descriptor Type 

bEndpointAddress

uint8_t bEndpointAddress
The address of the endpoint on the USB device described by this descriptor 

bmAttributes

uint8_t bmAttributes
This field describes the endpoint’s attributes when it is configured using the bConfigurationValue. 

wMaxPacketSize

uint16_t wMaxPacketSize
Maximum packet size this endpoint is capable of sending or receiving when this configuration is selected. 

bInterval

uint8_t bInterval
Interval for polling Isochronous and Interrupt endpoints. Expressed in frames or microframes depending on the device operating speed (1 ms for Low-Speed and Full-Speed or 125 us for USB High-Speed and above). 

USB_DESC_ATTR

struct usb_ep_desc_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_EP_DESC_SIZE]
Descriptor value 



```

#### usb_str_desc_t


```


union usb_str_desc_t

#include <usb_types_ch9.h>
Structure representing a USB string descriptor. 

Public Members
bLength

uint8_t bLength
Size of the descriptor in bytes 

bDescriptorType

uint8_t bDescriptorType
STRING Descriptor Type 

wData

uint16_t wData[]
UTF-16LE encoded 

USB_DESC_ATTR

struct usb_str_desc_t::[anonymous] USB_DESC_ATTR
USB descriptor attributes 

val

uint8_t val[USB_STR_DESC_SIZE]
Descriptor value 



```



### Macros


#### USB_DESC_ATTR


```


USB_DESC_ATTR

```

#### USB_B_DESCRIPTOR_TYPE_DEVICE


```


USB_B_DESCRIPTOR_TYPE_DEVICE
Descriptor types from USB2.0 specification table 9.5. 

```

#### USB_B_DESCRIPTOR_TYPE_CONFIGURATION


```


USB_B_DESCRIPTOR_TYPE_CONFIGURATION

```

#### USB_B_DESCRIPTOR_TYPE_STRING


```


USB_B_DESCRIPTOR_TYPE_STRING

```

#### USB_B_DESCRIPTOR_TYPE_INTERFACE


```


USB_B_DESCRIPTOR_TYPE_INTERFACE

```

#### USB_B_DESCRIPTOR_TYPE_ENDPOINT


```


USB_B_DESCRIPTOR_TYPE_ENDPOINT

```

#### USB_B_DESCRIPTOR_TYPE_DEVICE_QUALIFIER


```


USB_B_DESCRIPTOR_TYPE_DEVICE_QUALIFIER

```

#### USB_B_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION


```


USB_B_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION

```

#### USB_B_DESCRIPTOR_TYPE_INTERFACE_POWER


```


USB_B_DESCRIPTOR_TYPE_INTERFACE_POWER

```

#### USB_B_DESCRIPTOR_TYPE_OTG


```


USB_B_DESCRIPTOR_TYPE_OTG
Descriptor types from USB 2.0 ECN. 

```

#### USB_B_DESCRIPTOR_TYPE_DEBUG


```


USB_B_DESCRIPTOR_TYPE_DEBUG

```

#### USB_B_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION


```


USB_B_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION

```

#### USB_B_DESCRIPTOR_TYPE_SECURITY


```


USB_B_DESCRIPTOR_TYPE_SECURITY
Descriptor types from Wireless USB spec. 

```

#### USB_B_DESCRIPTOR_TYPE_KEY


```


USB_B_DESCRIPTOR_TYPE_KEY

```

#### USB_B_DESCRIPTOR_TYPE_ENCRYPTION_TYPE


```


USB_B_DESCRIPTOR_TYPE_ENCRYPTION_TYPE

```

#### USB_B_DESCRIPTOR_TYPE_BOS


```


USB_B_DESCRIPTOR_TYPE_BOS

```

#### USB_B_DESCRIPTOR_TYPE_DEVICE_CAPABILITY


```


USB_B_DESCRIPTOR_TYPE_DEVICE_CAPABILITY

```

#### USB_B_DESCRIPTOR_TYPE_WIRELESS_ENDPOINT_COMP


```


USB_B_DESCRIPTOR_TYPE_WIRELESS_ENDPOINT_COMP

```

#### USB_B_DESCRIPTOR_TYPE_WIRE_ADAPTER


```


USB_B_DESCRIPTOR_TYPE_WIRE_ADAPTER

```

#### USB_B_DESCRIPTOR_TYPE_RPIPE


```


USB_B_DESCRIPTOR_TYPE_RPIPE

```

#### USB_B_DESCRIPTOR_TYPE_CS_RADIO_CONTROL


```


USB_B_DESCRIPTOR_TYPE_CS_RADIO_CONTROL

```

#### USB_B_DESCRIPTOR_TYPE_PIPE_USAGE


```


USB_B_DESCRIPTOR_TYPE_PIPE_USAGE
Descriptor types from UAS specification. 

```

#### USB_SETUP_PACKET_SIZE


```


USB_SETUP_PACKET_SIZE
Size of a USB control transfer setup packet in bytes. 

```

#### USB_BM_REQUEST_TYPE_DIR_OUT


```


USB_BM_REQUEST_TYPE_DIR_OUT
Bit masks belonging to the bmRequestType field of a setup packet. 

```

#### USB_BM_REQUEST_TYPE_DIR_IN


```


USB_BM_REQUEST_TYPE_DIR_IN

```

#### USB_BM_REQUEST_TYPE_TYPE_STANDARD


```


USB_BM_REQUEST_TYPE_TYPE_STANDARD

```

#### USB_BM_REQUEST_TYPE_TYPE_CLASS


```


USB_BM_REQUEST_TYPE_TYPE_CLASS

```

#### USB_BM_REQUEST_TYPE_TYPE_VENDOR


```


USB_BM_REQUEST_TYPE_TYPE_VENDOR

```

#### USB_BM_REQUEST_TYPE_TYPE_RESERVED


```


USB_BM_REQUEST_TYPE_TYPE_RESERVED

```

#### USB_BM_REQUEST_TYPE_TYPE_MASK


```


USB_BM_REQUEST_TYPE_TYPE_MASK

```

#### USB_BM_REQUEST_TYPE_RECIP_DEVICE


```


USB_BM_REQUEST_TYPE_RECIP_DEVICE

```

#### USB_BM_REQUEST_TYPE_RECIP_INTERFACE


```


USB_BM_REQUEST_TYPE_RECIP_INTERFACE

```

#### USB_BM_REQUEST_TYPE_RECIP_ENDPOINT


```


USB_BM_REQUEST_TYPE_RECIP_ENDPOINT

```

#### USB_BM_REQUEST_TYPE_RECIP_OTHER


```


USB_BM_REQUEST_TYPE_RECIP_OTHER

```

#### USB_BM_REQUEST_TYPE_RECIP_MASK


```


USB_BM_REQUEST_TYPE_RECIP_MASK

```

#### USB_B_REQUEST_GET_STATUS


```


USB_B_REQUEST_GET_STATUS
Bit masks belonging to the bRequest field of a setup packet. 

```

#### USB_B_REQUEST_CLEAR_FEATURE


```


USB_B_REQUEST_CLEAR_FEATURE

```

#### USB_B_REQUEST_SET_FEATURE


```


USB_B_REQUEST_SET_FEATURE

```

#### USB_B_REQUEST_SET_ADDRESS


```


USB_B_REQUEST_SET_ADDRESS

```

#### USB_B_REQUEST_GET_DESCRIPTOR


```


USB_B_REQUEST_GET_DESCRIPTOR

```

#### USB_B_REQUEST_SET_DESCRIPTOR


```


USB_B_REQUEST_SET_DESCRIPTOR

```

#### USB_B_REQUEST_GET_CONFIGURATION


```


USB_B_REQUEST_GET_CONFIGURATION

```

#### USB_B_REQUEST_SET_CONFIGURATION


```


USB_B_REQUEST_SET_CONFIGURATION

```

#### USB_B_REQUEST_GET_INTERFACE


```


USB_B_REQUEST_GET_INTERFACE

```

#### USB_B_REQUEST_SET_INTERFACE


```


USB_B_REQUEST_SET_INTERFACE

```

#### USB_B_REQUEST_SYNCH_FRAME


```


USB_B_REQUEST_SYNCH_FRAME

```

#### USB_W_VALUE_DT_DEVICE


```


USB_W_VALUE_DT_DEVICE
Bit masks belonging to the wValue field of a setup packet. 

```

#### USB_W_VALUE_DT_CONFIG


```


USB_W_VALUE_DT_CONFIG

```

#### USB_W_VALUE_DT_STRING


```


USB_W_VALUE_DT_STRING

```

#### USB_W_VALUE_DT_INTERFACE


```


USB_W_VALUE_DT_INTERFACE

```

#### USB_W_VALUE_DT_ENDPOINT


```


USB_W_VALUE_DT_ENDPOINT

```

#### USB_W_VALUE_DT_DEVICE_QUALIFIER


```


USB_W_VALUE_DT_DEVICE_QUALIFIER

```

#### USB_W_VALUE_DT_OTHER_SPEED_CONFIG


```


USB_W_VALUE_DT_OTHER_SPEED_CONFIG

```

#### USB_W_VALUE_DT_INTERFACE_POWER


```


USB_W_VALUE_DT_INTERFACE_POWER

```

#### USB_SETUP_PACKET_INIT_SET_ADDR


```


USB_SETUP_PACKET_INIT_SET_ADDR(setup_pkt_ptr, addr)
Initializer for a SET_ADDRESS request. 
Sets the address of a connected device 

```

#### USB_SETUP_PACKET_INIT_GET_DEVICE_DESC


```


USB_SETUP_PACKET_INIT_GET_DEVICE_DESC(setup_pkt_ptr)
Initializer for a request to get a device's device descriptor. 

```

#### USB_SETUP_PACKET_INIT_GET_CONFIG


```


USB_SETUP_PACKET_INIT_GET_CONFIG(setup_pkt_ptr)
Initializer for a request to get a device's current configuration number. 

```

#### USB_SETUP_PACKET_INIT_GET_CONFIG_DESC


```


USB_SETUP_PACKET_INIT_GET_CONFIG_DESC(setup_pkt_ptr, desc_index, desc_len)
Initializer for a request to get one of the device's current configuration descriptor. 

desc_index indicates the configuration's index number
Number of bytes of the configuration descriptor to get 



```

#### USB_SETUP_PACKET_INIT_SET_CONFIG


```


USB_SETUP_PACKET_INIT_SET_CONFIG(setup_pkt_ptr, config_num)
Initializer for a request to set a device's current configuration number. 

```

#### USB_SETUP_PACKET_INIT_SET_INTERFACE


```


USB_SETUP_PACKET_INIT_SET_INTERFACE(setup_pkt_ptr, intf_num, alt_setting_num)
Initializer for a request to set an interface's alternate setting. 

```

#### USB_SETUP_PACKET_INIT_GET_STR_DESC


```


USB_SETUP_PACKET_INIT_GET_STR_DESC(setup_pkt_ptr, string_index, lang_id, desc_len)
Initializer for a request to get an string descriptor. 

```

#### USB_STANDARD_DESC_SIZE


```


USB_STANDARD_DESC_SIZE
Size of dummy USB standard descriptor. 

```

#### USB_DEVICE_DESC_SIZE


```


USB_DEVICE_DESC_SIZE
Size of a USB device descriptor in bytes. 

```

#### USB_CLASS_PER_INTERFACE


```


USB_CLASS_PER_INTERFACE
Possible base class values of the bDeviceClass field of a USB device descriptor. 

```

#### USB_CLASS_AUDIO


```


USB_CLASS_AUDIO

```

#### USB_CLASS_COMM


```


USB_CLASS_COMM

```

#### USB_CLASS_HID


```


USB_CLASS_HID

```

#### USB_CLASS_PHYSICAL


```


USB_CLASS_PHYSICAL

```

#### USB_CLASS_STILL_IMAGE


```


USB_CLASS_STILL_IMAGE

```

#### USB_CLASS_PRINTER


```


USB_CLASS_PRINTER

```

#### USB_CLASS_MASS_STORAGE


```


USB_CLASS_MASS_STORAGE

```

#### USB_CLASS_HUB


```


USB_CLASS_HUB

```

#### USB_CLASS_CDC_DATA


```


USB_CLASS_CDC_DATA

```

#### USB_CLASS_CSCID


```


USB_CLASS_CSCID

```

#### USB_CLASS_CONTENT_SEC


```


USB_CLASS_CONTENT_SEC

```

#### USB_CLASS_VIDEO


```


USB_CLASS_VIDEO

```

#### USB_CLASS_WIRELESS_CONTROLLER


```


USB_CLASS_WIRELESS_CONTROLLER

```

#### USB_CLASS_PERSONAL_HEALTHCARE


```


USB_CLASS_PERSONAL_HEALTHCARE

```

#### USB_CLASS_AUDIO_VIDEO


```


USB_CLASS_AUDIO_VIDEO

```

#### USB_CLASS_BILLBOARD


```


USB_CLASS_BILLBOARD

```

#### USB_CLASS_USB_TYPE_C_BRIDGE


```


USB_CLASS_USB_TYPE_C_BRIDGE

```

#### USB_CLASS_MISC


```


USB_CLASS_MISC

```

#### USB_CLASS_APP_SPEC


```


USB_CLASS_APP_SPEC

```

#### USB_CLASS_VENDOR_SPEC


```


USB_CLASS_VENDOR_SPEC

```

#### USB_SUBCLASS_VENDOR_SPEC


```


USB_SUBCLASS_VENDOR_SPEC
Vendor specific subclass code. 

```

#### USB_CONFIG_DESC_SIZE


```


USB_CONFIG_DESC_SIZE
Size of a short USB configuration descriptor in bytes. 

Note
The size of a full USB configuration includes all the interface and endpoint descriptors of that configuration. 


```

#### USB_BM_ATTRIBUTES_ONE


```


USB_BM_ATTRIBUTES_ONE
Bit masks belonging to the bmAttributes field of a configuration descriptor. 
Must be set 

```

#### USB_BM_ATTRIBUTES_SELFPOWER


```


USB_BM_ATTRIBUTES_SELFPOWER
Self powered 

```

#### USB_BM_ATTRIBUTES_WAKEUP


```


USB_BM_ATTRIBUTES_WAKEUP
Can wake-up 

```

#### USB_BM_ATTRIBUTES_BATTERY


```


USB_BM_ATTRIBUTES_BATTERY
Battery powered 

```

#### USB_IAD_DESC_SIZE


```


USB_IAD_DESC_SIZE
Size of a USB interface association descriptor in bytes. 

```

#### USB_INTF_DESC_SIZE


```


USB_INTF_DESC_SIZE
Size of a USB interface descriptor in bytes. 

```

#### USB_EP_DESC_SIZE


```


USB_EP_DESC_SIZE
Size of a USB endpoint descriptor in bytes. 

```

#### USB_B_ENDPOINT_ADDRESS_EP_NUM_MASK


```


USB_B_ENDPOINT_ADDRESS_EP_NUM_MASK
Bit masks belonging to the bEndpointAddress field of an endpoint descriptor. 

```

#### USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK


```


USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK

```

#### USB_W_MAX_PACKET_SIZE_MPS_MASK


```


USB_W_MAX_PACKET_SIZE_MPS_MASK
Bit masks belonging to the wMaxPacketSize field of endpoint descriptor. 

```

#### USB_W_MAX_PACKET_SIZE_MULT_MASK


```


USB_W_MAX_PACKET_SIZE_MULT_MASK

```

#### USB_BM_ATTRIBUTES_XFERTYPE_MASK


```


USB_BM_ATTRIBUTES_XFERTYPE_MASK
Bit masks belonging to the bmAttributes field of an endpoint descriptor. 

```

#### USB_BM_ATTRIBUTES_XFER_CONTROL


```


USB_BM_ATTRIBUTES_XFER_CONTROL

```

#### USB_BM_ATTRIBUTES_XFER_ISOC


```


USB_BM_ATTRIBUTES_XFER_ISOC

```

#### USB_BM_ATTRIBUTES_XFER_BULK


```


USB_BM_ATTRIBUTES_XFER_BULK

```

#### USB_BM_ATTRIBUTES_XFER_INT


```


USB_BM_ATTRIBUTES_XFER_INT

```

#### USB_BM_ATTRIBUTES_SYNCTYPE_MASK


```


USB_BM_ATTRIBUTES_SYNCTYPE_MASK

```

#### USB_BM_ATTRIBUTES_SYNC_NONE


```


USB_BM_ATTRIBUTES_SYNC_NONE

```

#### USB_BM_ATTRIBUTES_SYNC_ASYNC


```


USB_BM_ATTRIBUTES_SYNC_ASYNC

```

#### USB_BM_ATTRIBUTES_SYNC_ADAPTIVE


```


USB_BM_ATTRIBUTES_SYNC_ADAPTIVE

```

#### USB_BM_ATTRIBUTES_SYNC_SYNC


```


USB_BM_ATTRIBUTES_SYNC_SYNC

```

#### USB_BM_ATTRIBUTES_USAGETYPE_MASK


```


USB_BM_ATTRIBUTES_USAGETYPE_MASK

```

#### USB_BM_ATTRIBUTES_USAGE_DATA


```


USB_BM_ATTRIBUTES_USAGE_DATA

```

#### USB_BM_ATTRIBUTES_USAGE_FEEDBACK


```


USB_BM_ATTRIBUTES_USAGE_FEEDBACK

```

#### USB_BM_ATTRIBUTES_USAGE_IMPLICIT_FB


```


USB_BM_ATTRIBUTES_USAGE_IMPLICIT_FB

```

#### USB_EP_DESC_GET_XFERTYPE


```


USB_EP_DESC_GET_XFERTYPE(desc_ptr)
Macro helpers to get information about an endpoint from its descriptor. 

```

#### USB_EP_DESC_GET_EP_NUM


```


USB_EP_DESC_GET_EP_NUM(desc_ptr)

```

#### USB_EP_DESC_GET_EP_DIR


```


USB_EP_DESC_GET_EP_DIR(desc_ptr)

```

#### USB_EP_DESC_GET_MPS


```


USB_EP_DESC_GET_MPS(desc_ptr)

```

#### USB_EP_DESC_GET_MULT


```


USB_EP_DESC_GET_MULT(desc_ptr)

```

#### USB_EP_DESC_GET_SYNCTYPE


```


USB_EP_DESC_GET_SYNCTYPE(desc_ptr)

```

#### USB_EP_DESC_GET_USAGETYPE


```


USB_EP_DESC_GET_USAGETYPE(desc_ptr)

```

#### USB_STR_DESC_SIZE


```


USB_STR_DESC_SIZE
Size of a short USB string descriptor in bytes. 

```



### Enumerations


#### usb_device_state_t


```


enum usb_device_state_t
USB2.0 device states. 
See Table 9-1 of USB2.0 specification for more details

Note
The USB_DEVICE_STATE_NOT_ATTACHED is not part of the USB2.0 specification, but is a catch all state for devices that need to be cleaned up after a sudden disconnection or port error. 

Values:
USB_DEVICE_STATE_NOT_ATTACHED

enumerator USB_DEVICE_STATE_NOT_ATTACHED
The device was previously configured or suspended, but is no longer attached (either suddenly disconnected or a port error) 

USB_DEVICE_STATE_ATTACHED

enumerator USB_DEVICE_STATE_ATTACHED
Device is attached to the USB, but is not powered. 

USB_DEVICE_STATE_POWERED

enumerator USB_DEVICE_STATE_POWERED
Device is attached to the USB and powered, but has not been reset. 

USB_DEVICE_STATE_DEFAULT

enumerator USB_DEVICE_STATE_DEFAULT
Device is attached to the USB and powered and has been reset, but has not been assigned a unique address. Device responds at the default address. 

USB_DEVICE_STATE_ADDRESS

enumerator USB_DEVICE_STATE_ADDRESS
Device is attached to the USB, powered, has been reset, and a unique device address has been assigned. Device is not configured. 

USB_DEVICE_STATE_CONFIGURED

enumerator USB_DEVICE_STATE_CONFIGURED
Device is attached to the USB, powered, has been reset, has a unique address, is configured, and is not suspended. The host may now use the function provided by the device. 

USB_DEVICE_STATE_SUSPENDED

enumerator USB_DEVICE_STATE_SUSPENDED
Device is, at minimum, attached to the USB and is powered and has not seen bus activity for 3 ms. It may also have a unique address and be configured for use. However, because the device is suspended, the host may not use the device’s function. 


```




## Maintainers Notes



Note


For more details regarding the internal implementation details of the USB Host stack, please refer to USB Host Maintainers Notes (Introduction).