# ESP Serial Slave Link




## Overview


Espressif provides several chips that can work as slaves. These slave devices rely on some common buses, and have their own communication protocols over those buses. The `esp_serial_slave_link` component is designed for the master to communicate with ESP slave devices through those protocols over the bus drivers.


After an `esp_serial_slave_link` device is initialized properly, the application can use it to communicate with the ESP slave devices conveniently.



Note


The ESP-IDF component `esp_serial_slave_link` has been moved from ESP-IDF since version v5.0 to a separate repository:


* ESSL component on GitHub


To add ESSL component in your project, please run `idf.py add-dependency espressif/esp_serial_slave_link`.





## Espressif Device Protocols


For more details about Espressif device protocols, see the following documents.



* ESP SPI Slave HD (Half Duplex) Mode Protocol





## Terminology


* ESSL: Abbreviation for ESP Serial Slave Link, the component described by this document.
* Master: The device running the `esp_serial_slave_link` component.
* ESSL device: a virtual device on the master associated with an ESP slave device. The device context has the knowledge of the slave protocol above the bus, relying on some bus drivers to communicate with the slave.
* ESSL device handle: a handle to ESSL device context containing the configuration, status and data required by the ESSL component. The context stores the driver configurations, communication state, data shared by master and slave, etc.



> + The context should be initialized before it is used, and get deinitialized if not used any more. The master application operates on the ESSL device through this handle.
* ESP slave: the slave device connected to the bus, which ESSL component is designed to communicate with.
* Bus: The bus over which the master and the slave communicate with each other.
* Slave protocol: The special communication protocol specified by Espressif HW/SW over the bus.
* TX buffer num: a counter, which is on the slave and can be read by the master, indicates the accumulated buffer numbers that the slave has loaded to the hardware to receive data from the master.
* RX data size: a counter, which is on the slave and can be read by the master, indicates the accumulated data size that the slave has loaded to the hardware to send to the master.




## Services Provided by ESP Slave


There are some common services provided by the Espressif slaves:


1. Tohost Interrupts: The slave can inform the master about certain events by the interrupt line. (optional)
2. Frhost Interrupts: The master can inform the slave about certain events.
3. TX FIFO (master to slave): The slave can receive data from the master in units of receiving buffers.


The slave updates the TX buffer num to inform the master how much data it can receive, and the master then read the TX buffer num, and take off the used buffer number to know how many buffers are remaining.
4. RX FIFO (slave to master): The slave can send data in stream to the master. The SDIO slave can also indicate it has new data to send to master by the interrupt line.


The slave updates the RX data size to inform the master how much data it has prepared to send, and then the master read the data size, and take off the data length it has already received to know how many data is remaining.
5. Shared registers: The master can read some part of the registers on the slave, and also write these registers to let the slave read.


The services provided by the slave depends on the slave's model. See SPI Slave Capabilities of Espressif Chips for more details.




## Initialization of ESP Serial Slave Link



### ESP SDIO Slave


The ESP SDIO slave link (ESSL SDIO) devices relies on the SDMMC component. It includes the usage of communicating with ESP SDIO Slave device via the SDMMC Host or SDSPI Host feature. The ESSL device should be initialized as below:


1. Initialize a SDMMC card (see :doc:` Document of SDMMC driver </api-reference/storage/sdmmc>`) structure.
2. Call `sdmmc_card_init()` to initialize the card.
3. Initialize the ESSL device with `essl_sdio_config_t`. The `card` member should be the `sdmmc_card_t` got in step 2, and the `recv_buffer_size` member should be filled correctly according to pre-negotiated value.
4. Call `essl_init()` to do initialization of the SDIO part.
5. Call `essl_wait_for_ready()` to wait for the slave to be ready.




### ESP SPI Slave



Note


If you are communicating with the ESP SDIO Slave device through SPI interface, you should use the SDIO interface instead.



Has not been supported yet.





## APIs


After the initialization process above is performed, you can call the APIs below to make use of the services provided by the slave:



### Tohost Interrupts (Optional)


1. Call `essl_get_intr_ena()` to know which events trigger the interrupts to the master.
2. Call `essl_set_intr_ena()` to set the events that trigger interrupts to the master.
3. Call `essl_wait_int()` to wait until interrupt from the slave, or timeout.
4. When interrupt is triggered, call `essl_get_intr()` to know which events are active, and call `essl_clear_intr()` to clear them.




### Frhost Interrupts


1. Call `essl_send_slave_intr()` to trigger general purpose interrupt of the slave.




### TX FIFO


1. Call `essl_get_tx_buffer_num()` to know how many buffers the slave has prepared to receive data from the master. This is optional. The master will poll `tx_buffer_num` when it tries to send packets to the slave, until the slave has enough buffer or timeout.
2. Call `essl_send_packet()` to send data to the slave.




### RX FIFO


1. Call `essl_get_rx_data_size()` to know how many data the slave has prepared to send to the master. This is optional. When the master tries to receive data from the slave, it updates the `rx_data_size` for once, if the current `rx_data_size` is shorter than the buffer size the master prepared to receive. And it may poll the `rx_data_size` if the `rx_data_size` keeps 0, until timeout.
2. Call `essl_get_packet()` to receive data from the slave.




### Reset Counters (Optional)


Call `essl_reset_cnt()` to reset the internal counter if you find the slave has reset its counter.





## Application Example


The example below shows how ESP32-S3 SDIO host and slave communicate with each other. The host uses the ESSL SDIO:


peripherals/sdio


Please refer to the specific example README.md for details.




## API Reference



### Header File


* components/driver/test_apps/components/esp_serial_slave_link/include/esp_serial_slave_link/essl.h




### Functions


#### essl_init


```


esp_err_t essl_init(essl_handle_t handle, uint32_t wait_ms)
Initialize the slave. 

Parameters

handle -- Handle of an ESSL device. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9. 


Returns

ESP_OK: If success
ESP_ERR_NOT_SUPPORTED: Current device does not support this function.
Other value returned from lower layer init. 





```

#### essl_wait_for_ready


```


esp_err_t essl_wait_for_ready(essl_handle_t handle, uint32_t wait_ms)
Wait for interrupt of an ESSL slave device.

Parameters

handle -- Handle of an ESSL device. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: If success
ESP_ERR_NOT_SUPPORTED: Current device does not support this function.
One of the error codes from SDMMC host controller 





```

#### essl_get_tx_buffer_num


```


esp_err_t essl_get_tx_buffer_num(essl_handle_t handle, uint32_t *out_tx_num, uint32_t wait_ms)
Get buffer num for the host to send data to the slave. The buffers are size of buffer_size.

Parameters

handle -- Handle of a ESSL device. 
out_tx_num -- Output of buffer num that host can send data to ESSL slave. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: Success
ESP_ERR_NOT_SUPPORTED: This API is not supported in this mode
One of the error codes from SDMMC/SPI host controller 





```

#### essl_get_rx_data_size


```


esp_err_t essl_get_rx_data_size(essl_handle_t handle, uint32_t *out_rx_size, uint32_t wait_ms)
Get the size, in bytes, of the data that the ESSL slave is ready to send

Parameters

handle -- Handle of an ESSL device. 
out_rx_size -- Output of data size to read from slave, in bytes 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: Success
ESP_ERR_NOT_SUPPORTED: This API is not supported in this mode
One of the error codes from SDMMC/SPI host controller 





```

#### essl_reset_cnt


```


esp_err_t essl_reset_cnt(essl_handle_t handle)
Reset the counters of this component. Usually you don't need to do this unless you know the slave is reset.

Parameters
handle -- Handle of an ESSL device.

Returns

ESP_OK: Success
ESP_ERR_NOT_SUPPORTED: This API is not supported in this mode
ESP_ERR_INVALID_ARG: Invalid argument, handle is not init. 





```

#### essl_send_packet


```


esp_err_t essl_send_packet(essl_handle_t handle, const void *start, size_t length, uint32_t wait_ms)
Send a packet to the ESSL Slave. The Slave receives the packet into buffers whose size is buffer_size (configured during initialization).

Parameters

handle -- Handle of an ESSL device. 
start -- Start address of the packet to send 
length -- Length of data to send, if the packet is over-size, the it will be divided into blocks and hold into different buffers automatically. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK Success
ESP_ERR_INVALID_ARG: Invalid argument, handle is not init or other argument is not valid.
ESP_ERR_TIMEOUT: No buffer to use, or error ftrom SDMMC host controller.
ESP_ERR_NOT_FOUND: Slave is not ready for receiving.
ESP_ERR_NOT_SUPPORTED: This API is not supported in this mode
One of the error codes from SDMMC/SPI host controller. 





```

#### essl_get_packet


```


esp_err_t essl_get_packet(essl_handle_t handle, void *out_data, size_t size, size_t *out_length, uint32_t wait_ms)
Get a packet from ESSL slave.

Parameters

handle -- Handle of an ESSL device. 
out_data -- [out] Data output address 
size -- The size of the output buffer, if the buffer is smaller than the size of data to receive from slave, the driver returns ESP_ERR_NOT_FINISHED
out_length -- [out] Output of length the data actually received from slave. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK Success: All the data has been read from the slave.
ESP_ERR_INVALID_ARG: Invalid argument, The handle is not initialized or the other arguments are invalid.
ESP_ERR_NOT_FINISHED: Read was successful, but there is still data remaining.
ESP_ERR_NOT_FOUND: Slave is not ready to send data.
ESP_ERR_NOT_SUPPORTED: This API is not supported in this mode
One of the error codes from SDMMC/SPI host controller. 





```

#### essl_write_reg


```


esp_err_t essl_write_reg(essl_handle_t handle, uint8_t addr, uint8_t value, uint8_t *value_o, uint32_t wait_ms)
Write general purpose R/W registers (8-bit) of ESSL slave.

Note
sdio 28-31 are reserved, the lower API helps to skip.


Parameters

handle -- Handle of an ESSL device. 
addr -- Address of register to write. For SDIO, valid address: 0-59. For SPI, see essl_spi.h
value -- Value to write to the register. 
value_o -- Output of the returned written value. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK Success
One of the error codes from SDMMC/SPI host controller 





```

#### essl_read_reg


```


esp_err_t essl_read_reg(essl_handle_t handle, uint8_t add, uint8_t *value_o, uint32_t wait_ms)
Read general purpose R/W registers (8-bit) of ESSL slave.

Parameters

handle -- Handle of a essl device. 
add -- Address of register to read. For SDIO, Valid address: 0-27, 32-63 (28-31 reserved, return interrupt bits on read). For SPI, see essl_spi.h
value_o -- Output value read from the register. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK Success
One of the error codes from SDMMC/SPI host controller 





```

#### essl_wait_int


```


esp_err_t essl_wait_int(essl_handle_t handle, uint32_t wait_ms)
wait for an interrupt of the slave

Parameters

handle -- Handle of an ESSL device. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: If interrupt is triggered.
ESP_ERR_NOT_SUPPORTED: Current device does not support this function.
ESP_ERR_TIMEOUT: No interrupts before timeout. 





```

#### essl_clear_intr


```


esp_err_t essl_clear_intr(essl_handle_t handle, uint32_t intr_mask, uint32_t wait_ms)
Clear interrupt bits of ESSL slave. All the bits set in the mask will be cleared, while other bits will stay the same.

Parameters

handle -- Handle of an ESSL device. 
intr_mask -- Mask of interrupt bits to clear. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: Success
ESP_ERR_NOT_SUPPORTED: Current device does not support this function.
One of the error codes from SDMMC host controller 





```

#### essl_get_intr


```


esp_err_t essl_get_intr(essl_handle_t handle, uint32_t *intr_raw, uint32_t *intr_st, uint32_t wait_ms)
Get interrupt bits of ESSL slave.

Parameters

handle -- Handle of an ESSL device. 
intr_raw -- Output of the raw interrupt bits. Set to NULL if only masked bits are read. 
intr_st -- Output of the masked interrupt bits. set to NULL if only raw bits are read. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: Success
ESP_INVALID_ARG: If both intr_raw and intr_st are NULL.
ESP_ERR_NOT_SUPPORTED: Current device does not support this function.
One of the error codes from SDMMC host controller 





```

#### essl_set_intr_ena


```


esp_err_t essl_set_intr_ena(essl_handle_t handle, uint32_t ena_mask, uint32_t wait_ms)
Set interrupt enable bits of ESSL slave. The slave only sends interrupt on the line when there is a bit both the raw status and the enable are set.

Parameters

handle -- Handle of an ESSL device. 
ena_mask -- Mask of the interrupt bits to enable. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: Success
ESP_ERR_NOT_SUPPORTED: Current device does not support this function.
One of the error codes from SDMMC host controller 





```

#### essl_get_intr_ena


```


esp_err_t essl_get_intr_ena(essl_handle_t handle, uint32_t *ena_mask_o, uint32_t wait_ms)
Get interrupt enable bits of ESSL slave.

Parameters

handle -- Handle of an ESSL device. 
ena_mask_o -- Output of interrupt bit enable mask. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK Success
One of the error codes from SDMMC host controller 





```

#### essl_send_slave_intr


```


esp_err_t essl_send_slave_intr(essl_handle_t handle, uint32_t intr_mask, uint32_t wait_ms)
Send interrupts to slave. Each bit of the interrupt will be triggered.

Parameters

handle -- Handle of an ESSL device. 
intr_mask -- Mask of interrupt bits to send to slave. 
wait_ms -- Millisecond to wait before timeout, will not wait at all if set to 0-9.


Returns

ESP_OK: Success
ESP_ERR_NOT_SUPPORTED: Current device does not support this function.
One of the error codes from SDMMC host controller 





```



### Type Definitions


#### essl_handle_t


```


typedef struct essl_dev_t *essl_handle_t
Handle of an ESSL device. 

```



### Header File


* components/driver/test_apps/components/esp_serial_slave_link/include/esp_serial_slave_link/essl_sdio.h




### Functions


#### essl_sdio_init_dev


```


esp_err_t essl_sdio_init_dev(essl_handle_t *out_handle, const essl_sdio_config_t *config)
Initialize the ESSL SDIO device and get its handle. 

Parameters

out_handle -- Output of the handle. 
config -- Configuration for the ESSL SDIO device. 


Returns

ESP_OK: on success
ESP_ERR_NO_MEM: memory exhausted. 





```

#### essl_sdio_deinit_dev


```


esp_err_t essl_sdio_deinit_dev(essl_handle_t handle)
Deinitialize and free the space used by the ESSL SDIO device. 

Parameters
handle -- Handle of the ESSL SDIO device to deinit. 

Returns

ESP_OK: on success
ESP_ERR_INVALID_ARG: wrong handle passed 





```



### Structures


#### essl_sdio_config_t


```


struct essl_sdio_config_t
Configuration for the ESSL SDIO device. 

Public Members
card

sdmmc_card_t *card
The initialized sdmmc card pointer of the slave. 

recv_buffer_size

int recv_buffer_size
The pre-negotiated recv buffer size used by both the host and the slave. 



```



### Header File


* components/driver/test_apps/components/esp_serial_slave_link/include/esp_serial_slave_link/essl_spi.h




### Functions


#### essl_spi_init_dev


```


esp_err_t essl_spi_init_dev(essl_handle_t *out_handle, const essl_spi_config_t *init_config)
Initialize the ESSL SPI device function list and get its handle. 

Parameters

out_handle -- [out] Output of the handle 
init_config -- Configuration for the ESSL SPI device 


Returns

ESP_OK: On success
ESP_ERR_NO_MEM: Memory exhausted
ESP_ERR_INVALID_STATE: SPI driver is not initialized
ESP_ERR_INVALID_ARG: Wrong register ID 





```

#### essl_spi_deinit_dev


```


esp_err_t essl_spi_deinit_dev(essl_handle_t handle)
Deinitialize the ESSL SPI device and free the memory used by the device. 

Parameters
handle -- Handle of the ESSL SPI device 

Returns

ESP_OK: On success
ESP_ERR_INVALID_STATE: ESSL SPI is not in use 





```

#### essl_spi_read_reg


```


esp_err_t essl_spi_read_reg(void *arg, uint8_t addr, uint8_t *out_value, uint32_t wait_ms)
Read from the shared registers. 

Note
The registers for Master/Slave synchronization are reserved. Do not use them. (see rx_sync_reg in essl_spi_config_t)


Parameters

arg -- Context of the component. (Member arg from essl_handle_t) 
addr -- Address of the shared registers. (Valid: 0 ~ SOC_SPI_MAXIMUM_BUFFER_SIZE, registers for M/S sync are reserved, see note1). 
out_value -- [out] Read buffer for the shared registers. 
wait_ms -- Time to wait before timeout (reserved for future use, user should set this to 0). 


Returns

ESP_OK: success
ESP_ERR_INVALID_STATE: ESSL SPI has not been initialized.
ESP_ERR_INVALID_ARG: The address argument is not valid. See note 1.
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_get_packet


```


esp_err_t essl_spi_get_packet(void *arg, void *out_data, size_t size, uint32_t wait_ms)
Get a packet from Slave. 

Parameters

arg -- Context of the component. (Member arg from essl_handle_t) 
out_data -- [out] Output data address 
size -- The size of the output data. 
wait_ms -- Time to wait before timeout (reserved for future use, user should set this to 0). 


Returns

ESP_OK: On Success
ESP_ERR_INVALID_STATE: ESSL SPI has not been initialized.
ESP_ERR_INVALID_ARG: The output data address is neither DMA capable nor 4 byte-aligned
ESP_ERR_INVALID_SIZE: Master requires size bytes of data but Slave did not load enough bytes. 





```

#### essl_spi_write_reg


```


esp_err_t essl_spi_write_reg(void *arg, uint8_t addr, uint8_t value, uint8_t *out_value, uint32_t wait_ms)
Write to the shared registers. 

Note
The registers for Master/Slave synchronization are reserved. Do not use them. (see tx_sync_reg in essl_spi_config_t) 


Note
Feature of checking the actual written value (out_value) is not supported.


Parameters

arg -- Context of the component. (Member arg from essl_handle_t) 
addr -- Address of the shared registers. (Valid: 0 ~ SOC_SPI_MAXIMUM_BUFFER_SIZE, registers for M/S sync are reserved, see note1) 
value -- Buffer for data to send, should be align to 4. 
out_value -- [out] Not supported, should be set to NULL. 
wait_ms -- Time to wait before timeout (reserved for future use, user should set this to 0). 


Returns

ESP_OK: success
ESP_ERR_INVALID_STATE: ESSL SPI has not been initialized.
ESP_ERR_INVALID_ARG: The address argument is not valid. See note 1.
ESP_ERR_NOT_SUPPORTED: Should set out_value to NULL. See note 2.
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_send_packet


```


esp_err_t essl_spi_send_packet(void *arg, const void *data, size_t size, uint32_t wait_ms)
Send a packet to Slave. 

Parameters

arg -- Context of the component. (Member arg from essl_handle_t) 
data -- Address of the data to send 
size -- Size of the data to send. 
wait_ms -- Time to wait before timeout (reserved for future use, user should set this to 0). 


Returns

ESP_OK: On success
ESP_ERR_INVALID_STATE: ESSL SPI has not been initialized.
ESP_ERR_INVALID_ARG: The data address is not DMA capable
ESP_ERR_INVALID_SIZE: Master will send size bytes of data but Slave did not load enough RX buffer 





```

#### essl_spi_reset_cnt


```


void essl_spi_reset_cnt(void *arg)
Reset the counter in Master context. 

Note
Shall only be called if the slave has reset its counter. Else, Slave and Master would be desynchronized


Parameters
arg -- Context of the component. (Member arg from essl_handle_t) 



```

#### essl_spi_rdbuf


```


esp_err_t essl_spi_rdbuf(spi_device_handle_t spi, uint8_t *out_data, int addr, int len, uint32_t flags)
Read the shared buffer from the slave in ISR way. 

Note
The slave's HW doesn't guarantee the data in one SPI transaction is consistent. It sends data in unit of byte. In other words, if the slave SW attempts to update the shared register when a rdbuf SPI transaction is in-flight, the data got by the master will be the combination of bytes of different writes of slave SW.


Note
out_data should be prepared in words and in the DRAM. The buffer may be written in words by the DMA. When a byte is written, the remaining bytes in the same word will also be overwritten, even the len is shorter than a word.


Parameters

spi -- SPI device handle representing the slave 
out_data -- [out] Buffer for read data, strongly suggested to be in the DRAM and aligned to 4 
addr -- Address of the slave shared buffer 
len -- Length to read 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: on success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_rdbuf_polling


```


esp_err_t essl_spi_rdbuf_polling(spi_device_handle_t spi, uint8_t *out_data, int addr, int len, uint32_t flags)
Read the shared buffer from the slave in polling way. 

Note
out_data should be prepared in words and in the DRAM. The buffer may be written in words by the DMA. When a byte is written, the remaining bytes in the same word will also be overwritten, even the len is shorter than a word.


Parameters

spi -- SPI device handle representing the slave 
out_data -- [out] Buffer for read data, strongly suggested to be in the DRAM and aligned to 4 
addr -- Address of the slave shared buffer 
len -- Length to read 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: on success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_wrbuf


```


esp_err_t essl_spi_wrbuf(spi_device_handle_t spi, const uint8_t *data, int addr, int len, uint32_t flags)
Write the shared buffer of the slave in ISR way. 

Note
out_data should be prepared in words and in the DRAM. The buffer may be written in words by the DMA. When a byte is written, the remaining bytes in the same word will also be overwritten, even the len is shorter than a word.


Parameters

spi -- SPI device handle representing the slave 
data -- Buffer for data to send, strongly suggested to be in the DRAM 
addr -- Address of the slave shared buffer, 
len -- Length to write 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_wrbuf_polling


```


esp_err_t essl_spi_wrbuf_polling(spi_device_handle_t spi, const uint8_t *data, int addr, int len, uint32_t flags)
Write the shared buffer of the slave in polling way. 

Note
out_data should be prepared in words and in the DRAM. The buffer may be written in words by the DMA. When a byte is written, the remaining bytes in the same word will also be overwritten, even the len is shorter than a word.


Parameters

spi -- SPI device handle representing the slave 
data -- Buffer for data to send, strongly suggested to be in the DRAM 
addr -- Address of the slave shared buffer, 
len -- Length to write 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_polling_transmit. 





```

#### essl_spi_rddma


```


esp_err_t essl_spi_rddma(spi_device_handle_t spi, uint8_t *out_data, int len, int seg_len, uint32_t flags)
Receive long buffer in segments from the slave through its DMA. 

Note
This function combines several :cpp:func:essl_spi_rddma_seg and one :cpp:func:essl_spi_rddma_done at the end. Used when the slave is working in segment mode.


Parameters

spi -- SPI device handle representing the slave 
out_data -- [out] Buffer to hold the received data, strongly suggested to be in the DRAM and aligned to 4 
len -- Total length of data to receive. 
seg_len -- Length of each segment, which is not larger than the maximum transaction length allowed for the spi device. Suggested to be multiples of 4. When set < 0, means send all data in one segment (the rddma_done will still be sent.) 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_rddma_seg


```


esp_err_t essl_spi_rddma_seg(spi_device_handle_t spi, uint8_t *out_data, int seg_len, uint32_t flags)
Read one data segment from the slave through its DMA. 

Note
To read long buffer, call :cpp:func:essl_spi_rddma instead.


Parameters

spi -- SPI device handle representing the slave 
out_data -- [out] Buffer to hold the received data. strongly suggested to be in the DRAM and aligned to 4 
seg_len -- Length of this segment 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_rddma_done


```


esp_err_t essl_spi_rddma_done(spi_device_handle_t spi, uint32_t flags)
Send the rddma_done command to the slave. Upon receiving this command, the slave will stop sending the current buffer even there are data unsent, and maybe prepare the next buffer to send. 

Note
This is required only when the slave is working in segment mode.


Parameters

spi -- SPI device handle representing the slave 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_wrdma


```


esp_err_t essl_spi_wrdma(spi_device_handle_t spi, const uint8_t *data, int len, int seg_len, uint32_t flags)
Send long buffer in segments to the slave through its DMA. 

Note
This function combines several :cpp:func:essl_spi_wrdma_seg and one :cpp:func:essl_spi_wrdma_done at the end. Used when the slave is working in segment mode.


Parameters

spi -- SPI device handle representing the slave 
data -- Buffer for data to send, strongly suggested to be in the DRAM 
len -- Total length of data to send. 
seg_len -- Length of each segment, which is not larger than the maximum transaction length allowed for the spi device. Suggested to be multiples of 4. When set < 0, means send all data in one segment (the wrdma_done will still be sent.) 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_wrdma_seg


```


esp_err_t essl_spi_wrdma_seg(spi_device_handle_t spi, const uint8_t *data, int seg_len, uint32_t flags)
Send one data segment to the slave through its DMA. 

Note
To send long buffer, call :cpp:func:essl_spi_wrdma instead.


Parameters

spi -- SPI device handle representing the slave 
data -- Buffer for data to send, strongly suggested to be in the DRAM 
seg_len -- Length of this segment 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_transmit. 





```

#### essl_spi_wrdma_done


```


esp_err_t essl_spi_wrdma_done(spi_device_handle_t spi, uint32_t flags)
Send the wrdma_done command to the slave. Upon receiving this command, the slave will stop receiving, process the received data, and maybe prepare the next buffer to receive. 

Note
This is required only when the slave is working in segment mode.


Parameters

spi -- SPI device handle representing the slave 
flags -- SPI_TRANS_* flags to control the transaction mode of the transaction to send. 


Returns

ESP_OK: success
or other return value from :cpp:func:spi_device_transmit. 





```



### Structures


#### essl_spi_config_t


```


struct essl_spi_config_t
Configuration of ESSL SPI device. 

Public Members
spi

spi_device_handle_t *spi
Pointer to SPI device handle. 

tx_buf_size

uint32_t tx_buf_size
The pre-negotiated Master TX buffer size used by both the host and the slave. 

tx_sync_reg

uint8_t tx_sync_reg
The pre-negotiated register ID for Master-TX-SLAVE-RX synchronization. 1 word (4 Bytes) will be reserved for the synchronization. 

rx_sync_reg

uint8_t rx_sync_reg
The pre-negotiated register ID for Master-RX-Slave-TX synchronization. 1 word (4 Bytes) will be reserved for the synchronization. 



```