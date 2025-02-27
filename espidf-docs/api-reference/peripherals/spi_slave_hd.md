# SPI Slave Half Duplex




## Introduction


The Half Duplex (HD) Mode is a special mode provided by ESP SPI Slave peripheral. Under this mode, the hardware provides more services than the Full Duplex (FD) Mode (the mode for general-purpose SPI transactions, see SPI Slave Driver). These services reduce the CPU load and the response time of SPI Slave. However, it is important to note that the communication format is determined by the hardware and is always in a half-duplex configuration, allowing only one-way data transfer at any given time. Hence, the mode is named Half Duplex Mode due to this characteristic.


When conducting an SPI transaction, transactions can be classified into several types based on the **command** phase of the transaction. Each transaction may consist of the following phases: command, address, dummy, and data. The command phase is mandatory, while the other phases may be determined by the command field. During the command, address, and dummy phases, the bus is always controlled by the master (usually the host), while the direction of the data phase depends on the command. The data phase can be either an input phase, where the master writes data to the slave (e.g., the host sends data to the slave), or an output phase, where the master reads data from the slave (e.g., the host receives data from the slave).



### Protocol


About the details of how master should communicate with the SPI Slave, see ESP SPI Slave HD (Half Duplex) Mode Protocol.


Through these different transactions, the slave provides these services to the master:


* A DMA channel for the master to write a great amount of data to the slave.
* A DMA channel for the master to read a great amount of data from the slave.
* Several general purpose registers, shared between the master and the slave.
* Several general purpose interrupts, for the master to interrupt the SW of the slave.





## Terminology


* Transaction
* Channel
* Sending
* Receiving
* Data Descriptor




## Driver Feature


* Transaction read/write by master in segments
* Queues for data to send and received




## Driver Usage



### Slave Initialization


Call `spi_slave_hd_init()` to initialize the SPI bus as well as the peripheral and the driver. The SPI Slave exclusively uses the SPI peripheral, pins of the bus before it is deinitialized, which means other devices are unable to use the above resources during initialization. Thus, to ensure SPI resources are correctly occupied and the connections work properly, most configurations of the slave should be done as soon as the slave is initialized.


The `spi_bus_config_t` specifies how the bus should be initialized, while `spi_slave_hd_slot_config_t` specifies how the SPI Slave driver should work.




### Deinitialization (Optional)


Call `spi_slave_hd_deinit()` to uninstall the driver. The resources, including the pins, SPI peripheral, internal memory used by the driver, and interrupt sources, are released by the `deinit()` function.




### Send/Receive Data by DMA Channels


To send data to the master through the sending DMA channel, the application should properly wrap the data in an `spi_slave_hd_data_t` descriptor structure before calling `spi_slave_hd_queue_trans()` with the data descriptor and the channel argument of `SPI_SLAVE_CHAN_TX`. The pointers to descriptors are stored in the queue, and the data is sent to the master in the same order they are enqueued using `spi_slave_hd_queue_trans()`, upon receiving the master's `Rd_DMA` command.


The application should check the result of data sending by calling `spi_slave_hd_get_trans_res()` with the channel set as `SPI_SLAVE_CHAN_TX`. This function blocks until the transaction with the command `Rd_DMA` from the master successfully completes (or timeout). The `out_trans` argument of the function outputs the pointer of the data descriptor which is just finished, providing information about the sending.


Receiving data from the master through the receiving DMA channel is quite similar. The application calls `spi_slave_hd_queue_trans()` with proper data descriptor and the channel argument of `SPI_SLAVE_CHAN_RX`. And the application calls the `spi_slave_hd_get_trans_res()` later to get the descriptor to the receiving buffer before it handles the data in the receiving buffer.



Note


This driver itself does not have an internal buffer for the data to send or just received. The application should provide data buffer for driver via data descriptors to send to the master, or to receive data from the master.


The application has to properly keep the data descriptor as well as the buffer it points, after the descriptor is successfully sent into the driver internal queue by `spi_slave_hd_queue_trans()`, and before returned by `spi_slave_hd_get_trans_res()`. During this period, the hardware as well as the driver may read or write to the buffer and the descriptor when required at any time.



Please note that, when using this driver for data transfer, the buffer does not have to be fully sent or filled before it is terminated. For example, in the segment transaction mode, the master has to send `CMD7` to terminate a `Wr_DMA` transaction or send `CMD8` to terminate an `Rd_DMA` transaction (in segments), no matter whether the send (receive) buffer is used up (full) or not.




### Using Data Descriptor with Customized User Arguments


Sometimes you may have initiator (sending data descriptor) and closure (handling returned descriptors) functions in different places. When you get the returned data descriptor in the closure, you may need some extra information when handling the finished data descriptor. For example, you may want to know which round it is for the returned descriptor when you send the same piece of data several times.


Set the `arg` member in the data descriptor to a variable indicating the transaction by force casting, or point it to a structure that wraps all the information you may need when handling the sending/receiving data. Then you can get what you need in your closure.




### Using Callbacks



Note


These callbacks are called in the ISR, so the required operations need to be processed quickly and returned as soon as possible to ensure that the system is functioning properly. You may need to be very careful to write the code in the ISR.


Since the interrupt handling is executed concurrently with the application, long delays or blocking may cause the system to respond slower or lead to unpredictable behavior. Therefore, when writing callback functions, avoid using operations that may cause delays or blocking, e.g., waiting, sleeping, resource locking, etc.



The `spi_slave_hd_callback_config_t` member in the `spi_slave_hd_slot_config_t` configuration structure passed when initializing the SPI Slave HD driver, allows you to have callbacks for each event you may concern.


The corresponding interrupt for each callback that is not **NULL** is enabled, so that the callbacks can be called immediately when the events happen. You do not need to provide callbacks for the unconcerned events.


The `arg` member in the configuration structure can help you pass some context to the callback or indicate the specific SPI Slave instance when using the same callbacks for multiple SPI Slave peripherals. You can set the arg member to a variable that indicates the SPI Slave instance by performing a forced type casting or point it to a context structure. All the callbacks are called with this `arg` argument you set when the callbacks are initialized.


There are two other arguments: the `event` and the `awoken`.



> * The `event` passes the information of the current event to the callback. The `spi_slave_hd_event_t` type contains the information of the event, for example, event type, the data descriptor just finished (The data argument is very useful in this case!).
> * The `awoken` argument serves as an output parameter. It informs the ISR that tasks have been awakened after the callback function, and the ISR should call portYIELD_FROM_ISR() to schedule these tasks. Simply pass the `awoken` argument to all FreeRTOS APIs that may unblock tasks, and the value of `awoken` will be returned to the ISR.




### Writing/Reading Shared Registers


Call `spi_slave_hd_write_buffer()` to write the shared buffer, and `spi_slave_hd_read_buffer()` to read the shared buffer.



Note


On ESP32-S3, the shared registers are read/written in words by the application but read/written in bytes by the master. There is no guarantee four continuous bytes read from the master are from the same word written by the slave's application. It is also possible that if the slave reads a word while the master is writing bytes of the word, the slave may get one word with half of them just written by the master, and the other half has not been written into.


The master can confirm that the word is not in transition by reading the word twice and comparing the values.


For the slave, it is more difficult to ensure the word is not in transition because the process of master writing four bytes can be very long (32 SPI clocks). You can put some CRC in the last (largest address) byte of a word so that when the byte is written, the word is sure to be all written.


Due to the conflicts that may be among read/write from SW (worse if there are multi-cores) and master, it is suggested that a word is only used in one direction (only written by the master or only written by the slave).





### Receiving General Purpose Interrupts from the Master


When the master sends `CMD8`, `CMD9` or `CMDA`, the slave corresponding is triggered. Currently the `CMD8` is permanently used to indicate the termination of `Rd_DMA` segments. To receive general-purpose interrupts, register callbacks for `CMD9` and `CMDA` when the slave is initialized, see Using Callbacks.





## Application Example


The code example for Device/Host communication can be found in the peripherals/spi_slave_hd directory of ESP-IDF examples.




## API Reference



### Header File


* components/esp_driver_spi/include/driver/spi_slave_hd.h
* This header file can be included with:



> ```
> #include "driver/spi_slave_hd.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_spi` component. To declare that your component depends on `esp_driver_spi`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_spi
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_spi
> 
> ```




### Functions


#### spi_slave_hd_init


```


esp_err_t spi_slave_hd_init(spi_host_device_t host_id, const spi_bus_config_t *bus_config, const spi_slave_hd_slot_config_t *config)
Initialize the SPI Slave HD driver. 

Parameters

host_id -- The host to use 
bus_config -- Bus configuration for the bus used 
config -- Configuration for the SPI Slave HD driver 


Returns

ESP_OK: on success
ESP_ERR_INVALID_ARG: invalid argument given
ESP_ERR_INVALID_STATE: function called in invalid state, may be some resources are already in use
ESP_ERR_NOT_FOUND if there is no available DMA channel
ESP_ERR_NO_MEM: memory allocation failed
or other return value from esp_intr_alloc





```

#### spi_slave_hd_deinit


```


esp_err_t spi_slave_hd_deinit(spi_host_device_t host_id)
Deinitialize the SPI Slave HD driver. 

Parameters
host_id -- The host to deinitialize the driver 

Returns

ESP_OK: on success
ESP_ERR_INVALID_ARG: if the host_id is not correct 





```

#### spi_slave_hd_queue_trans


```


esp_err_t spi_slave_hd_queue_trans(spi_host_device_t host_id, spi_slave_chan_t chan, spi_slave_hd_data_t *trans, TickType_t timeout)
Queue transactions (segment mode) 

Parameters

host_id -- Host to queue the transaction 
chan -- SPI_SLAVE_CHAN_TX or SPI_SLAVE_CHAN_RX 
trans -- Transaction descriptors 
timeout -- Timeout before the data is queued 


Returns

ESP_OK: on success
ESP_ERR_INVALID_ARG: The input argument is invalid. Can be the following reason:
The buffer given is not DMA capable
The length of data is invalid (not larger than 0, or exceed the max transfer length)
The transaction direction is invalid


ESP_ERR_TIMEOUT: Cannot queue the data before timeout. Master is still processing previous transaction.
ESP_ERR_INVALID_STATE: Function called in invalid state. This API should be called under segment mode. 





```

#### spi_slave_hd_get_trans_res


```


esp_err_t spi_slave_hd_get_trans_res(spi_host_device_t host_id, spi_slave_chan_t chan, spi_slave_hd_data_t **out_trans, TickType_t timeout)
Get the result of a data transaction (segment mode) 

Note
This API should be called successfully the same times as the spi_slave_hd_queue_trans.


Parameters

host_id -- Host to queue the transaction 
chan -- Channel to get the result, SPI_SLAVE_CHAN_TX or SPI_SLAVE_CHAN_RX 
out_trans -- [out] Pointer to the transaction descriptor (spi_slave_hd_data_t) passed to the driver before. Hardware has finished this transaction. Member trans_len indicates the actual number of bytes of received data, it's meaningless for TX. 
timeout -- Timeout before the result is got 


Returns

ESP_OK: on success
ESP_ERR_INVALID_ARG: Function is not valid
ESP_ERR_TIMEOUT: There's no transaction done before timeout
ESP_ERR_INVALID_STATE: Function called in invalid state. This API should be called under segment mode. 





```

#### spi_slave_hd_read_buffer


```


void spi_slave_hd_read_buffer(spi_host_device_t host_id, int addr, uint8_t *out_data, size_t len)
Read the shared registers. 

Parameters

host_id -- Host to read the shared registers 
addr -- Address of register to read, 0 to SOC_SPI_MAXIMUM_BUFFER_SIZE-1
out_data -- [out] Output buffer to store the read data 
len -- Length to read, not larger than SOC_SPI_MAXIMUM_BUFFER_SIZE-addr




```

#### spi_slave_hd_write_buffer


```


void spi_slave_hd_write_buffer(spi_host_device_t host_id, int addr, uint8_t *data, size_t len)
Write the shared registers. 

Parameters

host_id -- Host to write the shared registers 
addr -- Address of register to write, 0 to SOC_SPI_MAXIMUM_BUFFER_SIZE-1
data -- Buffer holding the data to write 
len -- Length to write, SOC_SPI_MAXIMUM_BUFFER_SIZE-addr




```

#### spi_slave_hd_append_trans


```


esp_err_t spi_slave_hd_append_trans(spi_host_device_t host_id, spi_slave_chan_t chan, spi_slave_hd_data_t *trans, TickType_t timeout)
Load transactions (append mode) 

Note
In this mode, user transaction descriptors will be appended to the DMA and the DMA will keep processing the data without stopping


Parameters

host_id -- Host to load transactions 
chan -- SPI_SLAVE_CHAN_TX or SPI_SLAVE_CHAN_RX 
trans -- Transaction descriptor 
timeout -- Timeout before the transaction is loaded 


Returns

ESP_OK: on success
ESP_ERR_INVALID_ARG: The input argument is invalid. Can be the following reason:
The buffer given is not DMA capable
The length of data is invalid (not larger than 0, or exceed the max transfer length)
The transaction direction is invalid


ESP_ERR_TIMEOUT: Master is still processing previous transaction. There is no available transaction for slave to load
ESP_ERR_INVALID_STATE: Function called in invalid state. This API should be called under append mode. 





```

#### spi_slave_hd_get_append_trans_res


```


esp_err_t spi_slave_hd_get_append_trans_res(spi_host_device_t host_id, spi_slave_chan_t chan, spi_slave_hd_data_t **out_trans, TickType_t timeout)
Get the result of a data transaction (append mode) 

Note
This API should be called the same times as the spi_slave_hd_append_trans


Parameters

host_id -- Host to load the transaction 
chan -- SPI_SLAVE_CHAN_TX or SPI_SLAVE_CHAN_RX 
out_trans -- [out] Pointer to the transaction descriptor (spi_slave_hd_data_t) passed to the driver before. Hardware has finished this transaction. Member trans_len indicates the actual number of bytes of received data, it's meaningless for TX. 
timeout -- Timeout before the result is got 


Returns

ESP_OK: on success
ESP_ERR_INVALID_ARG: Function is not valid
ESP_ERR_TIMEOUT: There's no transaction done before timeout
ESP_ERR_INVALID_STATE: Function called in invalid state. This API should be called under append mode. 





```



### Structures


#### spi_slave_hd_data_t


```


struct spi_slave_hd_data_t
Descriptor of data to send/receive. 

Public Members
data

uint8_t *data
Buffer to send, must be DMA capable. 

len

size_t len
Len of data to send/receive. For receiving the buffer length should be multiples of 4 bytes, otherwise the extra part will be truncated. 

trans_len

size_t trans_len
For RX direction, it indicates the data actually received. For TX direction, it is meaningless. 

flags

uint32_t flags
Bitwise OR of SPI_SLAVE_HD_TRANS_* flags. 

arg

void *arg
Extra argument indicating this data. 



```

#### spi_slave_hd_event_t


```


struct spi_slave_hd_event_t
Information of SPI Slave HD event. 

Public Members
event

spi_event_t event
Event type. 

trans

spi_slave_hd_data_t *trans
Corresponding transaction for SPI_EV_SEND and SPI_EV_RECV events. 



```

#### spi_slave_hd_callback_config_t


```


struct spi_slave_hd_callback_config_t
Callback configuration structure for SPI Slave HD. 

Public Members
cb_buffer_tx

slave_cb_t cb_buffer_tx
Callback when master reads from shared buffer. 

cb_buffer_rx

slave_cb_t cb_buffer_rx
Callback when master writes to shared buffer. 

cb_send_dma_ready

slave_cb_t cb_send_dma_ready
Callback when TX data buffer is loaded to the hardware (DMA) 

cb_sent

slave_cb_t cb_sent
Callback when data are sent. 

cb_recv_dma_ready

slave_cb_t cb_recv_dma_ready
Callback when RX data buffer is loaded to the hardware (DMA) 

cb_recv

slave_cb_t cb_recv
Callback when data are received. 

cb_cmd9

slave_cb_t cb_cmd9
Callback when CMD9 received. 

cb_cmdA

slave_cb_t cb_cmdA
Callback when CMDA received. 

arg

void *arg
Argument indicating this SPI Slave HD peripheral instance. 



```

#### spi_slave_hd_slot_config_t


```


struct spi_slave_hd_slot_config_t
Configuration structure for the SPI Slave HD driver. 

Public Members
mode

uint8_t mode
SPI mode, representing a pair of (CPOL, CPHA) configuration:
0: (0, 0)
1: (0, 1)
2: (1, 0)
3: (1, 1) 



spics_io_num

uint32_t spics_io_num
CS GPIO pin for this device. 

flags

uint32_t flags
Bitwise OR of SPI_SLAVE_HD_* flags. 

command_bits

uint32_t command_bits
command field bits, multiples of 8 and at least 8. 

address_bits

uint32_t address_bits
address field bits, multiples of 8 and at least 8. 

dummy_bits

uint32_t dummy_bits
dummy field bits, multiples of 8 and at least 8. 

queue_size

uint32_t queue_size
Transaction queue size. This sets how many transactions can be 'in the air' (queued using spi_slave_hd_queue_trans but not yet finished using spi_slave_hd_get_trans_result) at the same time. 

dma_chan

spi_dma_chan_t dma_chan
DMA channel to used. 

cb_config

spi_slave_hd_callback_config_t cb_config
Callback configuration. 



```



### Macros


#### SPI_SLAVE_HD_TRANS_DMA_BUFFER_ALIGN_AUTO


```


SPI_SLAVE_HD_TRANS_DMA_BUFFER_ALIGN_AUTO
Automatically re-malloc dma buffer if user buffer doesn't meet hardware alignment or dma_capable, this process may lose some memory and performance. 

```

#### SPI_SLAVE_HD_TXBIT_LSBFIRST


```


SPI_SLAVE_HD_TXBIT_LSBFIRST
Transmit command/address/data LSB first instead of the default MSB first. 

```

#### SPI_SLAVE_HD_RXBIT_LSBFIRST


```


SPI_SLAVE_HD_RXBIT_LSBFIRST
Receive data LSB first instead of the default MSB first. 

```

#### SPI_SLAVE_HD_BIT_LSBFIRST


```


SPI_SLAVE_HD_BIT_LSBFIRST
Transmit and receive LSB first. 

```

#### SPI_SLAVE_HD_APPEND_MODE


```


SPI_SLAVE_HD_APPEND_MODE
Adopt DMA append mode for transactions. In this mode, users can load(append) DMA descriptors without stopping the DMA. 

```



### Type Definitions


#### slave_cb_t


```


typedef bool (*slave_cb_t)(void *arg, spi_slave_hd_event_t *event, BaseType_t *awoken)
Callback for SPI Slave HD. 

```



### Enumerations


#### spi_slave_chan_t


```


enum spi_slave_chan_t
Channel of SPI Slave HD to do data transaction. 
Values:
SPI_SLAVE_CHAN_TX

enumerator SPI_SLAVE_CHAN_TX
The output channel (RDDMA) 

SPI_SLAVE_CHAN_RX

enumerator SPI_SLAVE_CHAN_RX
The input channel (WRDMA) 


```