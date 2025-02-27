# Thread




## Introduction


Thread is an IP-based mesh networking protocol. It is based on the 802.15.4 physical and MAC layer.




## Application Examples


The openthread directory of ESP-IDF examples contains the following applications:


* The OpenThread interactive shell openthread/ot_cli
* The Thread Border Router openthread/ot_br
* The Thread Radio Co-Processor openthread/ot_rcp




## API Reference


For manipulating the Thread network, the OpenThread API shall be used. The OpenThread API docs can be found at the OpenThread API docs.


ESP-IDF provides extra APIs for launching and managing the OpenThread stack, binding to network interfaces and border routing features.



### Header File


* components/openthread/include/esp_openthread.h
* This header file can be included with:



> ```
> #include "esp_openthread.h"
> 
> ```
* This header file is a part of the API provided by the `openthread` component. To declare that your component depends on `openthread`, add the following to your CMakeLists.txt:



> ```
> REQUIRES openthread
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES openthread
> 
> ```




### Functions


#### esp_openthread_init


```


esp_err_t esp_openthread_init(const esp_openthread_platform_config_t *init_config)
Initializes the full OpenThread stack. 

Note
The OpenThread instance will also be initialized in this function.


Parameters
init_config -- [in] The initialization configuration.

Returns

ESP_OK on success
ESP_ERR_NO_MEM if allocation has failed
ESP_ERR_INVALID_ARG if radio or host connection mode not supported
ESP_ERR_INVALID_STATE if already initialized 





```

#### esp_openthread_auto_start


```


esp_err_t esp_openthread_auto_start(otOperationalDatasetTlvs *datasetTlvs)
Starts the Thread protocol operation and attaches to a Thread network. 

Parameters
datasetTlvs -- [in] The operational dataset (TLV encoded), if it's NULL, the function will generate the dataset based on the configurations from kconfig.

Returns

ESP_OK on success
ESP_FAIL on failures 





```

#### esp_openthread_launch_mainloop


```


esp_err_t esp_openthread_launch_mainloop(void)
Launches the OpenThread main loop. 

Note
This function will not return unless error happens when running the OpenThread stack.


Returns

ESP_OK on success
ESP_ERR_NO_MEM if allocation has failed
ESP_FAIL on other failures 





```

#### esp_openthread_deinit


```


esp_err_t esp_openthread_deinit(void)
This function performs OpenThread stack and platform driver deinitialization. 

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if not initialized 





```

#### esp_openthread_get_instance


```


otInstance *esp_openthread_get_instance(void)
This function acquires the underlying OpenThread instance. 

Note
This function can be called on other tasks without lock.


Returns
The OpenThread instance pointer 



```



### Header File


* components/openthread/include/esp_openthread_types.h
* This header file can be included with:



> ```
> #include "esp_openthread_types.h"
> 
> ```
* This header file is a part of the API provided by the `openthread` component. To declare that your component depends on `openthread`, add the following to your CMakeLists.txt:



> ```
> REQUIRES openthread
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES openthread
> 
> ```




### Structures


#### esp_openthread_role_changed_event_t


```


struct esp_openthread_role_changed_event_t
OpenThread role changed event data. 

Public Members
previous_role

otDeviceRole previous_role
Previous Thread role 

current_role

otDeviceRole current_role
Current Thread role 



```

#### esp_openthread_mainloop_context_t


```


struct esp_openthread_mainloop_context_t
This structure represents a context for a select() based mainloop. 

Public Members
read_fds

fd_set read_fds
The read file descriptors 

write_fds

fd_set write_fds
The write file descriptors 

error_fds

fd_set error_fds
The error file descriptors 

max_fd

int max_fd
The max file descriptor 

timeout

struct timeval timeout
The timeout 



```

#### esp_openthread_uart_config_t


```


struct esp_openthread_uart_config_t
The uart port config for OpenThread. 

Public Members
port

uart_port_t port
UART port number 

uart_config

uart_config_t uart_config
UART configuration, see uart_config_t docs 

rx_pin

gpio_num_t rx_pin
UART RX pin 

tx_pin

gpio_num_t tx_pin
UART TX pin 



```

#### esp_openthread_spi_host_config_t


```


struct esp_openthread_spi_host_config_t
The spi port config for OpenThread. 

Public Members
host_device

spi_host_device_t host_device
SPI host device 

dma_channel

spi_dma_chan_t dma_channel
DMA channel 

spi_interface

spi_bus_config_t spi_interface
SPI bus 

spi_device

spi_device_interface_config_t spi_device
SPI peripheral device 

intr_pin

gpio_num_t intr_pin
SPI interrupt pin 



```

#### esp_openthread_spi_slave_config_t


```


struct esp_openthread_spi_slave_config_t
The spi slave config for OpenThread. 

Public Members
host_device

spi_host_device_t host_device
SPI host device 

bus_config

spi_bus_config_t bus_config
SPI bus config 

slave_config

spi_slave_interface_config_t slave_config
SPI slave config 

intr_pin

gpio_num_t intr_pin
SPI interrupt pin 



```

#### esp_openthread_radio_config_t


```


struct esp_openthread_radio_config_t
The OpenThread radio configuration. 

Public Members
radio_mode

esp_openthread_radio_mode_t radio_mode
The radio mode 

radio_uart_config

esp_openthread_uart_config_t radio_uart_config
The uart configuration to RCP 

radio_spi_config

esp_openthread_spi_host_config_t radio_spi_config
The spi configuration to RCP 



```

#### esp_openthread_host_connection_config_t


```


struct esp_openthread_host_connection_config_t
The OpenThread host connection configuration. 

Public Members
host_connection_mode

esp_openthread_host_connection_mode_t host_connection_mode
The host connection mode 

host_uart_config

esp_openthread_uart_config_t host_uart_config
The uart configuration to host 

host_usb_config

usb_serial_jtag_driver_config_t host_usb_config
The usb configuration to host 

spi_slave_config

esp_openthread_spi_slave_config_t spi_slave_config
The spi configuration to host 



```

#### esp_openthread_port_config_t


```


struct esp_openthread_port_config_t
The OpenThread port specific configuration. 

Public Members
storage_partition_name

const char *storage_partition_name
The partition for storing OpenThread dataset 

netif_queue_size

uint8_t netif_queue_size
The packet queue size for the network interface 

task_queue_size

uint8_t task_queue_size
The task queue size 



```

#### esp_openthread_platform_config_t


```


struct esp_openthread_platform_config_t
The OpenThread platform configuration. 

Public Members
radio_config

esp_openthread_radio_config_t radio_config
The radio configuration 

host_config

esp_openthread_host_connection_config_t host_config
The host connection configuration 

port_config

esp_openthread_port_config_t port_config
The port configuration 



```



### Type Definitions


#### esp_openthread_rcp_failure_handler


```


typedef void (*esp_openthread_rcp_failure_handler)(void)

```



### Enumerations


#### esp_openthread_event_t


```


enum esp_openthread_event_t
OpenThread event declarations. 
Values:
OPENTHREAD_EVENT_START

enumerator OPENTHREAD_EVENT_START
OpenThread stack start 

OPENTHREAD_EVENT_STOP

enumerator OPENTHREAD_EVENT_STOP
OpenThread stack stop 

OPENTHREAD_EVENT_DETACHED

enumerator OPENTHREAD_EVENT_DETACHED
OpenThread detached 

OPENTHREAD_EVENT_ATTACHED

enumerator OPENTHREAD_EVENT_ATTACHED
OpenThread attached 

OPENTHREAD_EVENT_ROLE_CHANGED

enumerator OPENTHREAD_EVENT_ROLE_CHANGED
OpenThread role changed 

OPENTHREAD_EVENT_IF_UP

enumerator OPENTHREAD_EVENT_IF_UP
OpenThread network interface up 

OPENTHREAD_EVENT_IF_DOWN

enumerator OPENTHREAD_EVENT_IF_DOWN
OpenThread network interface down 

OPENTHREAD_EVENT_GOT_IP6

enumerator OPENTHREAD_EVENT_GOT_IP6
OpenThread stack added IPv6 address 

OPENTHREAD_EVENT_LOST_IP6

enumerator OPENTHREAD_EVENT_LOST_IP6
OpenThread stack removed IPv6 address 

OPENTHREAD_EVENT_MULTICAST_GROUP_JOIN

enumerator OPENTHREAD_EVENT_MULTICAST_GROUP_JOIN
OpenThread stack joined IPv6 multicast group 

OPENTHREAD_EVENT_MULTICAST_GROUP_LEAVE

enumerator OPENTHREAD_EVENT_MULTICAST_GROUP_LEAVE
OpenThread stack left IPv6 multicast group 

OPENTHREAD_EVENT_TREL_ADD_IP6

enumerator OPENTHREAD_EVENT_TREL_ADD_IP6
OpenThread stack added TREL IPv6 address 

OPENTHREAD_EVENT_TREL_REMOVE_IP6

enumerator OPENTHREAD_EVENT_TREL_REMOVE_IP6
OpenThread stack removed TREL IPv6 address 

OPENTHREAD_EVENT_TREL_MULTICAST_GROUP_JOIN

enumerator OPENTHREAD_EVENT_TREL_MULTICAST_GROUP_JOIN
OpenThread stack joined TREL IPv6 multicast group 

OPENTHREAD_EVENT_SET_DNS_SERVER

enumerator OPENTHREAD_EVENT_SET_DNS_SERVER
OpenThread stack set DNS server > 

OPENTHREAD_EVENT_PUBLISH_MESHCOP_E

enumerator OPENTHREAD_EVENT_PUBLISH_MESHCOP_E
OpenThread stack start to publish meshcop-e service > 

OPENTHREAD_EVENT_REMOVE_MESHCOP_E

enumerator OPENTHREAD_EVENT_REMOVE_MESHCOP_E
OpenThread stack start to remove meshcop-e service > 


```

#### esp_openthread_radio_mode_t


```


enum esp_openthread_radio_mode_t
The radio mode of OpenThread. 
Values:
RADIO_MODE_NATIVE

enumerator RADIO_MODE_NATIVE
Use the native 15.4 radio 

RADIO_MODE_UART_RCP

enumerator RADIO_MODE_UART_RCP
UART connection to a 15.4 capable radio co-processor (RCP) 

RADIO_MODE_SPI_RCP

enumerator RADIO_MODE_SPI_RCP
SPI connection to a 15.4 capable radio co-processor (RCP) 

RADIO_MODE_MAX

enumerator RADIO_MODE_MAX
Using for parameter check 


```

#### esp_openthread_host_connection_mode_t


```


enum esp_openthread_host_connection_mode_t
How OpenThread connects to the host. 
Values:
HOST_CONNECTION_MODE_NONE

enumerator HOST_CONNECTION_MODE_NONE
Disable host connection 

HOST_CONNECTION_MODE_CLI_UART

enumerator HOST_CONNECTION_MODE_CLI_UART
CLI UART connection to the host 

HOST_CONNECTION_MODE_CLI_USB

enumerator HOST_CONNECTION_MODE_CLI_USB
CLI USB connection to the host 

HOST_CONNECTION_MODE_RCP_UART

enumerator HOST_CONNECTION_MODE_RCP_UART
RCP UART connection to the host 

HOST_CONNECTION_MODE_RCP_SPI

enumerator HOST_CONNECTION_MODE_RCP_SPI
RCP SPI connection to the host 

HOST_CONNECTION_MODE_MAX

enumerator HOST_CONNECTION_MODE_MAX
Using for parameter check 


```



### Header File


* components/openthread/include/esp_openthread_lock.h
* This header file can be included with:



> ```
> #include "esp_openthread_lock.h"
> 
> ```
* This header file is a part of the API provided by the `openthread` component. To declare that your component depends on `openthread`, add the following to your CMakeLists.txt:



> ```
> REQUIRES openthread
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES openthread
> 
> ```




### Functions


#### esp_openthread_lock_init


```


esp_err_t esp_openthread_lock_init(void)
This function initializes the OpenThread API lock. 

Returns

ESP_OK on success
ESP_ERR_NO_MEM if allocation has failed
ESP_ERR_INVALID_STATE if already initialized 





```

#### esp_openthread_lock_deinit


```


void esp_openthread_lock_deinit(void)
This function deinitializes the OpenThread API lock. 

```

#### esp_openthread_lock_acquire


```


bool esp_openthread_lock_acquire(TickType_t block_ticks)
This function acquires the OpenThread API lock. 

Note
Every Openthread APIs that takes an otInstance argument MUST be protected with this API lock except that the call site is in Openthread callbacks.


Parameters
block_ticks -- [in] The maximum number of RTOS ticks to wait for the lock.

Returns

True on lock acquired
False on failing to acquire the lock with the timeout. 





```

#### esp_openthread_lock_release


```


void esp_openthread_lock_release(void)
This function releases the OpenThread API lock. 

```

#### esp_openthread_task_switching_lock_acquire


```


bool esp_openthread_task_switching_lock_acquire(TickType_t block_ticks)
This function acquires the OpenThread API task switching lock. 

Note
In OpenThread API context, it waits for some actions to be done in other tasks (like lwip), after task switching, it needs to call OpenThread API again. Normally it's not allowed, since the previous OpenThread API lock is not released yet. This task_switching lock allows the OpenThread API can be called in this case.


Note
Please use esp_openthread_lock_acquire() for normal cases.


Parameters
block_ticks -- [in] The maximum number of RTOS ticks to wait for the lock.

Returns

True on lock acquired
False on failing to acquire the lock with the timeout. 





```

#### esp_openthread_task_switching_lock_release


```


void esp_openthread_task_switching_lock_release(void)
This function releases the OpenThread API task switching lock. 

Note
This API must be called after esp_openthread_task_switching_lock_acquire or esp_openthread_lock_acquire and will cause a crash if the current task is not the task switching lock holder. This error could be caused by calling OpenThread APIs without locking OpenThread stack. 


```



### Header File


* components/openthread/include/esp_openthread_netif_glue.h
* This header file can be included with:



> ```
> #include "esp_openthread_netif_glue.h"
> 
> ```
* This header file is a part of the API provided by the `openthread` component. To declare that your component depends on `openthread`, add the following to your CMakeLists.txt:



> ```
> REQUIRES openthread
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES openthread
> 
> ```




### Functions


#### esp_openthread_netif_glue_init


```


void *esp_openthread_netif_glue_init(const esp_openthread_platform_config_t *config)
This function initializes the OpenThread network interface glue. 

Parameters
config -- [in] The platform configuration.

Returns

glue pointer on success
NULL on failure 





```

#### esp_openthread_netif_glue_deinit


```


void esp_openthread_netif_glue_deinit(void)
This function deinitializes the OpenThread network interface glue. 

```

#### esp_openthread_get_netif


```


esp_netif_t *esp_openthread_get_netif(void)
This function acquires the OpenThread netif. 

Returns
The OpenThread netif or NULL if not initialzied. 



```

#### esp_openthread_register_meshcop_e_handler


```


void esp_openthread_register_meshcop_e_handler(esp_event_handler_t handler, bool for_publish)
This function register a handler for meshcop-e service publish event and remove event. 

Parameters

handler -- [in] The handler. 
for_publish -- [in] The usage of handler, true for publish event and false for remove event. 




```



### Macros


#### ESP_NETIF_INHERENT_DEFAULT_OPENTHREAD


```


ESP_NETIF_INHERENT_DEFAULT_OPENTHREAD()
Default configuration reference of OpenThread esp-netif. 

```

#### ESP_NETIF_DEFAULT_OPENTHREAD


```


ESP_NETIF_DEFAULT_OPENTHREAD()

```



### Header File


* components/openthread/include/esp_openthread_border_router.h
* This header file can be included with:



> ```
> #include "esp_openthread_border_router.h"
> 
> ```
* This header file is a part of the API provided by the `openthread` component. To declare that your component depends on `openthread`, add the following to your CMakeLists.txt:



> ```
> REQUIRES openthread
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES openthread
> 
> ```




### Functions


#### esp_openthread_set_backbone_netif


```


void esp_openthread_set_backbone_netif(esp_netif_t *backbone_netif)
Sets the backbone interface used for border routing. 

Note
This function must be called before esp_openthread_init


Parameters
backbone_netif -- [in] The backbone network interface (WiFi or ethernet) 



```

#### esp_openthread_border_router_init


```


esp_err_t esp_openthread_border_router_init(void)
Initializes the border router features of OpenThread. 

Note
Calling this function will make the device behave as an OpenThread border router. Kconfig option CONFIG_OPENTHREAD_BORDER_ROUTER is required.


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if feature not supported
ESP_ERR_INVALID_STATE if already initialized
ESP_FIAL on other failures 





```

#### esp_openthread_border_router_deinit


```


esp_err_t esp_openthread_border_router_deinit(void)
Deinitializes the border router features of OpenThread. 

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if not initialized
ESP_FIAL on other failures 





```

#### esp_openthread_get_backbone_netif


```


esp_netif_t *esp_openthread_get_backbone_netif(void)
Gets the backbone interface of OpenThread border router. 

Returns
The backbone interface or NULL if border router not initialized. 



```

#### esp_openthread_register_rcp_failure_handler


```


void esp_openthread_register_rcp_failure_handler(esp_openthread_rcp_failure_handler handler)
Registers the callback for RCP failure. 

```

#### esp_openthread_rcp_deinit


```


esp_err_t esp_openthread_rcp_deinit(void)
Deinitializes the connection to RCP. 

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if fail to deinitialize RCP 





```

#### esp_openthread_rcp_init


```


esp_err_t esp_openthread_rcp_init(void)
Initializes the connection to RCP. 

Returns

ESP_OK on success
ESP_FAIL if fail to initialize RCP 





```

#### esp_openthread_set_meshcop_instance_name


```


esp_err_t esp_openthread_set_meshcop_instance_name(const char *instance_name)
Sets the meshcop(e) instance name. 

Note
This function can only be called before esp_openthread_border_router_init. If instance_name is NULL, then the service will use the hostname as instance name.


Parameters
instance_name -- [in] The instance name, can be NULL.

Returns

ESP_OK on success
ESP_FAIL if fail to initialize RCP 





```