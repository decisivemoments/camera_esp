# ESP-NETIF Custom I/O Driver



This section outlines implementing a new I/O driver with ESP-NETIF connection capabilities.


By convention, the I/O driver has to register itself as an ESP-NETIF driver, and thus holds a dependency on ESP-NETIF component and is responsible for providing data path functions, post-attach callback and in most cases, also default event handlers to define network interface actions based on driver's lifecycle transitions.



## Packet Input/Output


According to the diagram shown in the ESP-NETIF Architecture part, the following three API functions for the packet data path must be defined for connecting with ESP-NETIF:


* `esp_netif_transmit()`
* `esp_netif_free_rx_buffer()`
* `esp_netif_receive()`


The first two functions for transmitting and freeing the rx buffer are provided as callbacks, i.e., they get called from ESP-NETIF (and its underlying TCP/IP stack) and I/O driver provides their implementation.


The receiving function on the other hand gets called from the I/O driver, so that the driver's code simply calls `esp_netif_receive()` on a new data received event.




## Post Attach Callback


A final part of the network interface initialization consists of attaching the ESP-NETIF instance to the I/O driver, by means of calling the following API:



```
esp_err_t esp_netif_attach(esp_netif_t *esp_netif, esp_netif_iodriver_handle driver_handle);

```


It is assumed that the `esp_netif_iodriver_handle` is a pointer to driver's object, a struct derived from `struct esp_netif_driver_base_s`, so that the first member of I/O driver structure must be this base structure with pointers to:


* post-attach function callback
* related ESP-NETIF instance


As a result, the I/O driver has to create an instance of the struct per below:



```
typedef struct my_netif_driver_s {
        esp_netif_driver_base_t base;           /*!< base structure reserved as esp-netif driver */
        driver_impl             *h;             /*!< handle of driver implementation */
    } my_netif_driver_t;

```


with actual values of `my_netif_driver_t::base.post_attach` and the actual drivers handle `my_netif_driver_t::h`.


So when the `esp_netif_attach()` gets called from the initialization code, the post-attach callback from I/O driver's code gets executed to mutually register callbacks between ESP-NETIF and I/O driver instances. Typically the driver is started as well in the post-attach callback. An example of a simple post-attach callback is outlined below:



```
static esp_err_t my_post_attach_start(esp_netif_t * esp_netif, void * args)
{
    my_netif_driver_t *driver = args;
    const esp_netif_driver_ifconfig_t driver_ifconfig = {
            .driver_free_rx_buffer = my_free_rx_buf,
            .transmit = my_transmit,
            .handle = driver->driver_impl
    };
    driver->base.netif = esp_netif;
    ESP_ERROR_CHECK(esp_netif_set_driver_config(esp_netif, &driver_ifconfig));
    my_driver_start(driver->driver_impl);
    return ESP_OK;
}

```




## Default Handlers


I/O drivers also typically provide default definitions of lifecycle behavior of related network interfaces based on state transitions of I/O drivers. For example *driver start* `->` *network start*, etc.


An example of such a default handler is provided below:



```
esp_err_t my_driver_netif_set_default_handlers(my_netif_driver_t *driver, esp_netif_t * esp_netif)
{
    driver_set_event_handler(driver->driver_impl, esp_netif_action_start, MY_DRV_EVENT_START, esp_netif);
    driver_set_event_handler(driver->driver_impl, esp_netif_action_stop, MY_DRV_EVENT_STOP, esp_netif);
    return ESP_OK;
}

```



### Network Stack Connection


The packet data path functions for transmitting and freeing the rx buffer (defined in the I/O driver) are called from the ESP-NETIF, specifically from its TCP/IP stack connecting layer.


Note that ESP-IDF provides several network stack configurations for the most common network interfaces, such as for the Wi-Fi station or Ethernet. These configurations are defined in esp_netif/include/esp_netif_defaults.h and should be sufficient for most network drivers. In rare cases, expert users might want to define custom lwIP based interface layers; it is possible, but an explicit dependency to lwIP needs to be set.


The following API reference outlines these network stack interaction with the ESP-NETIF:





## Header File


* components/esp_netif/include/esp_netif_net_stack.h
* This header file can be included with:



> ```
> #include "esp_netif_net_stack.h"
> 
> ```
* This header file is a part of the API provided by the `esp_netif` component. To declare that your component depends on `esp_netif`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_netif
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_netif
> 
> ```




## Functions


#### esp_netif_get_handle_from_netif_impl


```


esp_netif_t *esp_netif_get_handle_from_netif_impl(void *dev)
Returns esp-netif handle. 

Parameters
dev -- [in] opaque ptr to network interface of specific TCP/IP stack

Returns
handle to related esp-netif instance 



```

#### esp_netif_get_netif_impl


```


void *esp_netif_get_netif_impl(esp_netif_t *esp_netif)
Returns network stack specific implementation handle. 

Parameters
esp_netif -- [in] Handle to esp-netif instance

Returns
handle to related network stack netif handle 



```

#### esp_netif_set_link_speed


```


esp_err_t esp_netif_set_link_speed(esp_netif_t *esp_netif, uint32_t speed)
Set link-speed for the specified network interface. 

Parameters

esp_netif -- [in] Handle to esp-netif instance 
speed -- [in] Link speed in bit/s 


Returns
ESP_OK on success 



```

#### esp_netif_transmit


```


esp_err_t esp_netif_transmit(esp_netif_t *esp_netif, void *data, size_t len)
Outputs packets from the TCP/IP stack to the media to be transmitted. 
This function gets called from network stack to output packets to IO driver.

Parameters

esp_netif -- [in] Handle to esp-netif instance 
data -- [in] Data to be transmitted 
len -- [in] Length of the data frame


Returns
ESP_OK on success, an error passed from the I/O driver otherwise 



```

#### esp_netif_transmit_wrap


```


esp_err_t esp_netif_transmit_wrap(esp_netif_t *esp_netif, void *data, size_t len, void *netstack_buf)
Outputs packets from the TCP/IP stack to the media to be transmitted. 
This function gets called from network stack to output packets to IO driver.

Parameters

esp_netif -- [in] Handle to esp-netif instance 
data -- [in] Data to be transmitted 
len -- [in] Length of the data frame 
netstack_buf -- [in] net stack buffer


Returns
ESP_OK on success, an error passed from the I/O driver otherwise 



```

#### esp_netif_free_rx_buffer


```


void esp_netif_free_rx_buffer(void *esp_netif, void *buffer)
Free the rx buffer allocated by the media driver. 
This function gets called from network stack when the rx buffer to be freed in IO driver context, i.e. to deallocate a buffer owned by io driver (when data packets were passed to higher levels to avoid copying)

Parameters

esp_netif -- [in] Handle to esp-netif instance 
buffer -- [in] Rx buffer pointer 




```