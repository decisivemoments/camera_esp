# Inter-IC Sound (I2S)




## Introduction


I2S (Inter-IC Sound) is a synchronous serial communication protocol usually used for transmitting audio data between two digital audio devices.


ESP32-S3 contains two I2S peripheral(s). These peripherals can be configured to input and output sample data via the I2S driver.


An I2S bus that communicates in standard or TDM mode consists of the following lines:


* **MCLK:** Master clock line. It is an optional signal depending on the slave side, mainly used for offering a reference clock to the I2S slave device.
* **BCLK:** Bit clock line. The bit clock for data line.
* **WS:** Word (Slot) select line. It is usually used to identify the vocal tract except PDM mode.
* **DIN/DOUT:** Serial data input/output line. Data will loopback internally if DIN and DOUT are set to a same GPIO.


An I2S bus that communicates in PDM mode consists of the following lines:


* **CLK:** PDM clock line.
* **DIN/DOUT:** Serial data input/output line.


Each I2S controller has the following features that can be configured by the I2S driver:


* Operation as system master or slave
* Capable of acting as transmitter or receiver
* DMA controller that allows stream sampling of data without requiring the CPU to copy each data sample


Each controller has separate RX and TX channels. That means they are able to work under different clocks and slot configurations with separate GPIO pins. Note that although the internal MCLKs of TX channel and RX channel are separate on a controller, the output MCLK signal can only be attached to one channel. If independent MCLK output is required for each channel, they must be allocated on different I2S controllers.




## I2S File Structure



!I2S file structure



I2S File Structure






**Public headers that need to be included in the I2S application are as follows:**


* `i2s.h`: The header file that provides legacy I2S APIs (for apps using legacy driver).
* `i2s_std.h`: The header file that provides standard communication mode specific APIs (for apps using new driver with standard mode).
* `i2s_pdm.h`: The header file that provides PDM communication mode specific APIs (for apps using new driver with PDM mode).
* `i2s_tdm.h`: The header file that provides TDM communication mode specific APIs (for apps using new driver with TDM mode).



Note


The legacy driver cannot coexist with the new driver. Include `i2s.h` to use the legacy driver, or include the other three headers to use the new driver. The legacy driver might be removed in future.



**Public headers that have been included in the headers above are as follows:**


* `i2s_types_legacy.h`: The header file that provides legacy public types that are only used in the legacy driver.
* `i2s_types.h`: The header file that provides public types.
* `i2s_common.h`: The header file that provides common APIs for all communication modes.




## I2S Clock



### Clock Source


* `i2s_clock_src_t::I2S_CLK_SRC_DEFAULT`: Default PLL clock.


* `i2s_clock_src_t::I2S_CLK_SRC_PLL_160M`: 160 MHz PLL clock.




### Clock Terminology


* **Sample rate**: The number of sampled data in one second per slot.
* **SCLK**: Source clock frequency. It is the frequency of the clock source.
* **MCLK**: Master clock frequency. BCLK is generated from this clock. The MCLK signal usually serves as a reference clock and is mostly needed to synchronize BCLK and WS between I2S master and slave roles.
* **BCLK**: Bit clock frequency. Every tick of this clock stands for one data bit on data pin. The slot bit width configured in `i2s_std_slot_config_t::slot_bit_width` is equal to the number of BCLK ticks, which means there will be 8/16/24/32 BCLK ticks in one slot.
* **LRCK** / **WS**: Left/right clock or word select clock. For non-PDM mode, its frequency is equal to the sample rate.



Note


Normally, MCLK should be the multiple of `sample rate` and BCLK at the same time. The field `i2s_std_clk_config_t::mclk_multiple` indicates the multiple of MCLK to the `sample rate`. In most cases, `I2S_MCLK_MULTIPLE_256` should be enough. However, if `slot_bit_width` is set to `I2S_SLOT_BIT_WIDTH_24BIT`, to keep MCLK a multiple to the BCLK, `i2s_std_clk_config_t::mclk_multiple` should be set to multiples that are divisible by 3 such as `I2S_MCLK_MULTIPLE_384`. Otherwise, WS will be inaccurate.






## I2S Communication Mode



### Overview of All Modes












| Target | Standard | PDM TX | PDM RX | TDM | ADC/DAC | LCD/Camera |
| --- | --- | --- | --- | --- | --- | --- |
| ESP32 | I2S 0/1 | I2S 0 | I2S 0 | none | I2S 0 | I2S 0 |
| ESP32-S2 | I2S 0 | none | none | none | none | I2S 0 |
| ESP32-C3 | I2S 0 | I2S 0 | none | I2S 0 | none | none |
| ESP32-C6 | I2S 0 | I2S 0 | none | I2S 0 | none | none |
| ESP32-S3 | I2S 0/1 | I2S 0 | I2S 0 | I2S 0/1 | none | none |
| ESP32-H2 | I2S 0 | I2S 0 | none | I2S 0 | none | none |
| ESP32-P4 | I2S 0~2 | I2S 0 | I2S 0 | I2S 0~2 | none | none |




### Standard Mode


In standard mode, there are always two sound channels, i.e., the left and right channels, which are called "slots". These slots support 8/16/24/32-bit width sample data. The communication format for the slots mainly includes the following:


* **Philips Format**: Data signal has one-bit shift comparing to the WS signal, and the duty of WS signal is 50%.





* **MSB Format**: Basically the same as Philips format, but without data shift.





* **PCM Short Format**: Data has one-bit shift and meanwhile the WS signal becomes a pulse lasting for one BCLK cycle.







### PDM Mode (TX)


PDM (Pulse-density Modulation) mode for the TX channel can convert PCM data into PDM format which always has left and right slots. PDM TX is only supported on I2S0 and it only supports 16-bit width sample data. It needs at least a CLK pin for clock signal and a DOUT pin for data signal (i.e., the WS and SD signal in the following figure; the BCK signal is an internal bit sampling clock, which is not needed between PDM devices). This mode allows users to configure the up-sampling parameters `i2s_pdm_tx_clk_config_t::up_sample_fp` and `i2s_pdm_tx_clk_config_t::up_sample_fs`. The up-sampling rate can be calculated by `up_sample_rate = i2s_pdm_tx_clk_config_t::up_sample_fp / i2s_pdm_tx_clk_config_t::up_sample_fs`. There are two up-sampling modes in PDM TX:


* **Fixed Clock Frequency**: In this mode, the up-sampling rate changes according to the sample rate. Setting `fp = 960` and `fs = sample_rate / 100`, then the clock frequency (Fpdm) on CLK pin will be fixed to `128 * 48 KHz = 6.144 MHz`. Note that this frequency is not equal to the sample rate (Fpcm).
* **Fixed Up-sampling Rate**: In this mode, the up-sampling rate is fixed to 2. Setting `fp = 960` and `fs = 480`, then the clock frequency (Fpdm) on CLK pin will be `128 * sample_rate`.







### PDM Mode (RX)


PDM (Pulse-density Modulation) mode for RX channel can receive PDM-format data and convert the data into PCM format. PDM RX is only supported on I2S0, and it only supports 16-bit width sample data. PDM RX needs at least a CLK pin for clock signal and a DIN pin for data signal. This mode allows users to configure the down-sampling parameter `i2s_pdm_rx_clk_config_t::dn_sample_mode`. There are two down-sampling modes in PDM RX:


* `i2s_pdm_dsr_t::I2S_PDM_DSR_8S`: In this mode, the clock frequency (Fpdm) on the WS pin is `sample_rate (Fpcm) * 64`.
* `i2s_pdm_dsr_t::I2S_PDM_DSR_16S`: In this mode, the clock frequency (Fpdm) on the WS pin is `sample_rate (Fpcm) * 128`.




### TDM Mode


TDM (Time Division Multiplexing) mode supports up to 16 slots. These slots can be enabled by `i2s_tdm_slot_config_t::slot_mask`.


But due to the hardware limitation, only up to 4 slots are supported while the slot is set to 32 bit-width, and 8 slots for 16 bit-width, 16 slots for 8 bit-width. The slot communication format of TDM is almost the same as the standard mode, yet with some small differences.


* **Philips Format**: Data signal has one-bit shift comparing to the WS signal. And no matter how many slots are contained in one frame, the duty of WS signal always keeps 50%.





* **MSB Format**: Basically the same as the Philips format, but without data shift.





* **PCM Short Format**: Data has one-bit shift and the WS signal becomes a pulse lasting one BCLK cycle for every frame.





* **PCM Long Format**: Data has one-bit shift and the WS signal lasts one-slot bit width for every frame. For example, the duty of WS will be 25% if there are four slots enabled, and 20% if there are five slots.








## Functional Overview


The I2S driver offers the following services:



### Resource Management


There are three levels of resources in the I2S driver:


* `platform level`: Resources of all I2S controllers in the current target.
* `controller level`: Resources in one I2S controller.
* `channel level`: Resources of TX or RX channel in one I2S controller.


The public APIs are all channel-level APIs. The channel handle `i2s_chan_handle_t` can help users to manage the resources under a specific channel without considering the other two levels. The other two upper levels' resources are private and are managed by the driver automatically. Users can call `i2s_new_channel()` to allocate a channel handle and call `i2s_del_channel()` to delete it.




### Power Management


When the power management is enabled (i.e., CONFIG_PM_ENABLE is on), the system will adjust or stop the source clock of I2S before entering Light-sleep, thus potentially changing the I2S signals and leading to transmitting or receiving invalid data.


The I2S driver can prevent the system from changing or stopping the source clock by acquiring a power management lock. When the source clock is generated from APB, the lock type will be set to `esp_pm_lock_type_t::ESP_PM_APB_FREQ_MAX` and when the source clock is APLL (if supported), it will be set to `esp_pm_lock_type_t::ESP_PM_NO_LIGHT_SLEEP`. Whenever the user is reading or writing via I2S (i.e., calling `i2s_channel_read()` or `i2s_channel_write()`), the driver guarantees that the power management lock is acquired. Likewise, the driver releases the lock after the reading or writing finishes.




### Finite State Machine


There are three states for an I2S channel, namely, `registered`, `ready`, and `running`. Their relationship is shown in the following diagram:



!I2S Finite State Machine



I2S Finite State Machine






The `<mode>` in the diagram can be replaced by corresponding I2S communication modes, e.g., `std` for standard two-slot mode. For more information about communication modes, please refer to the I2S Communication Mode section.




### Data Transport


The data transport of the I2S peripheral, including sending and receiving, is realized by DMA. Before transporting data, please call `i2s_channel_enable()` to enable the specific channel. When the sent or received data reaches the size of one DMA buffer, the `I2S_OUT_EOF` or `I2S_IN_SUC_EOF` interrupt will be triggered. Note that the DMA buffer size is not equal to `i2s_chan_config_t::dma_frame_num`. One frame here refers to all the sampled data in one WS circle. Therefore, `dma_buffer_size = dma_frame_num * slot_num * slot_bit_width / 8`. For the data transmitting, users can input the data by calling `i2s_channel_write()`. This function helps users to copy the data from the source buffer to the DMA TX buffer and wait for the transmission to finish. Then it will repeat until the sent bytes reach the given size. For the data receiving, the function `i2s_channel_read()` waits to receive the message queue which contains the DMA buffer address. It helps users copy the data from the DMA RX buffer to the destination buffer.


Both `i2s_channel_write()` and `i2s_channel_read()` are blocking functions. They keeps waiting until the whole source buffer is sent or the whole destination buffer is loaded, unless they exceed the max blocking time, where the error code `ESP_ERR_TIMEOUT` returns. To send or receive data asynchronously, callbacks can be registered by `i2s_channel_register_event_callback()`. Users are able to access the DMA buffer directly in the callback function instead of transmitting or receiving by the two blocking functions. However, please be aware that it is an interrupt callback, so do not add complex logic, run floating operation, or call non-reentrant functions in the callback.




### Configuration


Users can initialize a channel by calling corresponding functions (i.e., `i2s_channel_init_std_mode()`, `i2s_channel_init_pdm_rx_mode()`, `i2s_channel_init_pdm_tx_mode()`, or `i2s_channel_init_tdm_mode()`) to a specific mode. If the configurations need to be updated after initialization, users have to first call `i2s_channel_disable()` to ensure that the channel has stopped, and then call corresponding `reconfig` functions, like `i2s_channel_reconfig_std_slot()`, `i2s_channel_reconfig_std_clock()`, and `i2s_channel_reconfig_std_gpio()`.




### IRAM Safe


By default, the I2S interrupt will be deferred when the cache is disabled for reasons like writing/erasing flash. Thus the EOF interrupt will not get executed in time.


To avoid such case in real-time applications, you can enable the Kconfig option CONFIG_I2S_ISR_IRAM_SAFE that:


1. Keeps the interrupt being serviced even when the cache is disabled.
2. Places driver object into DRAM (in case it is linked to PSRAM by accident).


This allows the interrupt to run while the cache is disabled, but comes at the cost of increased IRAM consumption.




### Thread Safety


All the public I2S APIs are guaranteed to be thread safe by the driver, which means users can call them from different RTOS tasks without protection by extra locks. Notice that the I2S driver uses mutex lock to ensure the thread safety, thus these APIs are not allowed to be used in ISR.




### Kconfig Options


* CONFIG_I2S_ISR_IRAM_SAFE controls whether the default ISR handler can work when the cache is disabled. See IRAM Safe for more information.
* CONFIG_I2S_SUPPRESS_DEPRECATE_WARN controls whether to suppress the compiling warning message while using the legacy I2S driver.
* CONFIG_I2S_ENABLE_DEBUG_LOG is used to enable the debug log output. Enable this option increases the firmware binary size.





## Application Example


The examples of the I2S driver can be found in the directory peripherals/i2s. Here are some simple usages of each mode:



### Standard TX/RX Usage


Different slot communication formats can be generated by the following helper macros for standard mode. As described above, there are three formats in standard mode, and their helper macros are:


* `I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG`
* `I2S_STD_PCM_SLOT_DEFAULT_CONFIG`
* `I2S_STD_MSB_SLOT_DEFAULT_CONFIG`


The clock config helper macro is:


* `I2S_STD_CLK_DEFAULT_CONFIG`


Please refer to Standard Mode for information about STD API. And for more details, please refer to esp_driver_i2s/include/driver/i2s_std.h.



#### STD TX Mode


Take 16-bit data width for example. When the data in a `uint16_t` writing buffer are:














| data 0 | data 1 | data 2 | data 3 | data 4 | data 5 | data 6 | data 7 | ... |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005 | 0x0006 | 0x0007 | 0x0008 | ... |


Here is the table of the real data on the line with different `i2s_std_slot_config_t::slot_mode` and `i2s_std_slot_config_t::slot_mask`.
















| data bit width | slot mode | slot mask | WS low | WS high | WS low | WS high | WS low | WS high | WS low | WS high |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 16 bit | mono | left | 0x0001 | 0x0000 | 0x0002 | 0x0000 | 0x0003 | 0x0000 | 0x0004 | 0x0000 |
| right | 0x0000 | 0x0001 | 0x0000 | 0x0002 | 0x0000 | 0x0003 | 0x0000 | 0x0004 |
| both | 0x0001 | 0x0001 | 0x0002 | 0x0002 | 0x0003 | 0x0003 | 0x0004 | 0x0004 |
| stereo | left | 0x0001 | 0x0000 | 0x0003 | 0x0000 | 0x0005 | 0x0000 | 0x0007 | 0x0000 |
| right | 0x0000 | 0x0002 | 0x0000 | 0x0004 | 0x0000 | 0x0006 | 0x0000 | 0x0008 |
| both | 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005 | 0x0006 | 0x0007 | 0x0008 |



Note


Similar for 8-bit and 32-bit data widths, the type of the buffer is better to be `uint8_t` and `uint32_t`. But specially, when the data width is 24-bit, the data buffer should be aligned with 3-byte (i.e., every 3 bytes stands for a 24-bit data in one slot). Additionally, `i2s_chan_config_t::dma_frame_num`, `i2s_std_clk_config_t::mclk_multiple`, and the writing buffer size should be the multiple of `3`, otherwise the data on the line or the sample rate will be incorrect.




```
#include "driver/i2s_std.h"
#include "driver/gpio.h"

i2s_chan_handle_t tx_handle;
/* Get the default channel configuration by the helper macro.
 * This helper macro is defined in `i2s_common.h` and shared by all the I2S communication modes.
 * It can help to specify the I2S role and port ID */
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
/* Allocate a new TX channel and get the handle of this channel */
i2s_new_channel(&chan_cfg, &tx_handle, NULL);

/* Setting the configurations, the slot configuration and clock configuration can be generated by the macros
 * These two helper macros are defined in `i2s_std.h` which can only be used in STD mode.
 * They can help to specify the slot and clock configurations for initialization or updating */
i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(48000),
    .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = GPIO_NUM_4,
        .ws = GPIO_NUM_5,
        .dout = GPIO_NUM_18,
        .din = I2S_GPIO_UNUSED,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};
/* Initialize the channel */
i2s_channel_init_std_mode(tx_handle, &std_cfg);

/* Before writing data, start the TX channel first */
i2s_channel_enable(tx_handle);
i2s_channel_write(tx_handle, src_buf, bytes_to_write, bytes_written, ticks_to_wait);

/* If the configurations of slot or clock need to be updated,
 * stop the channel first and then update it */
// i2s_channel_disable(tx_handle);
// std_cfg.slot_cfg.slot_mode = I2S_SLOT_MODE_MONO; // Default is stereo
// i2s_channel_reconfig_std_slot(tx_handle, &std_cfg.slot_cfg);
// std_cfg.clk_cfg.sample_rate_hz = 96000;
// i2s_channel_reconfig_std_clock(tx_handle, &std_cfg.clk_cfg);

/* Have to stop the channel before deleting it */
i2s_channel_disable(tx_handle);
/* If the handle is not needed any more, delete it to release the channel resources */
i2s_del_channel(tx_handle);

```




#### STD RX Mode


Taking 16-bit data width for example, when the data on the line are:














| WS low | WS high | WS low | WS high | WS low | WS high | WS low | WS high | ... |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005 | 0x0006 | 0x0007 | 0x0008 | ... |


Here is the table of the data received in the buffer with different `i2s_std_slot_config_t::slot_mode` and `i2s_std_slot_config_t::slot_mask`.
















| data bit width | slot mode | slot mask | data 0 | data 1 | data 2 | data 3 | data 4 | data 5 | data 6 | data 7 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 16 bit | mono | left | 0x0001 | 0x0003 | 0x0005 | 0x0007 | 0x0009 | 0x000b | 0x000d | 0x000f |
| right | 0x0002 | 0x0004 | 0x0006 | 0x0008 | 0x000a | 0x000c | 0x000e | 0x0010 |
| stereo | any | 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005 | 0x0006 | 0x0007 | 0x0008 |



Note


8-bit, 24-bit, and 32-bit are similar as 16-bit, the data bit-width in the receiving buffer is equal to the data bit-width on the line. Additionally, when using 24-bit data width, `i2s_chan_config_t::dma_frame_num`, `i2s_std_clk_config_t::mclk_multiple`, and the receiving buffer size should be the multiple of `3`, otherwise the data on the line or the sample rate will be incorrect.




```
#include "driver/i2s_std.h"
#include "driver/gpio.h"

i2s_chan_handle_t rx_handle;
/* Get the default channel configuration by helper macro.
 * This helper macro is defined in `i2s_common.h` and shared by all the I2S communication modes.
 * It can help to specify the I2S role and port ID */
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
/* Allocate a new RX channel and get the handle of this channel */
i2s_new_channel(&chan_cfg, NULL, &rx_handle);

/* Setting the configurations, the slot configuration and clock configuration can be generated by the macros
 * These two helper macros are defined in `i2s_std.h` which can only be used in STD mode.
 * They can help to specify the slot and clock configurations for initialization or updating */
i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(48000),
    .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = GPIO_NUM_4,
        .ws = GPIO_NUM_5,
        .dout = I2S_GPIO_UNUSED,
        .din = GPIO_NUM_19,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};
/* Initialize the channel */
i2s_channel_init_std_mode(rx_handle, &std_cfg);

/* Before reading data, start the RX channel first */
i2s_channel_enable(rx_handle);
i2s_channel_read(rx_handle, desc_buf, bytes_to_read, bytes_read, ticks_to_wait);

/* Have to stop the channel before deleting it */
i2s_channel_disable(rx_handle);
/* If the handle is not needed any more, delete it to release the channel resources */
i2s_del_channel(rx_handle);

```





### PDM TX Usage


For PDM mode in TX channel, the slot configuration helper macro is:


* `I2S_PDM_TX_SLOT_DEFAULT_CONFIG`


The clock configuration helper macro is:


* `I2S_PDM_TX_CLK_DEFAULT_CONFIG`


Please refer to PDM Mode for information about PDM TX API. And for more details, please refer to esp_driver_i2s/include/driver/i2s_pdm.h.


The PDM data width is fixed to 16-bit. When the data in an `int16_t` writing buffer is:














| data 0 | data 1 | data 2 | data 3 | data 4 | data 5 | data 6 | data 7 | ... |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005 | 0x0006 | 0x0007 | 0x0008 | ... |


Here is the table of the real data on the line with different `i2s_pdm_tx_slot_config_t::slot_mode` and `i2s_pdm_tx_slot_config_t::line_mode` (The PDM format on the line is transferred to PCM format for easier comprehension).
















| line mode | slot mode | line | left | right | left | right | left | right | left | right |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| one-line Codec | mono | dout | 0x0001 | 0x0000 | 0x0002 | 0x0000 | 0x0003 | 0x0000 | 0x0004 | 0x0000 |
| stereo | dout | 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005 | 0x0006 | 0x0007 | 0x0008 |
| one-line DAC | mono | dout | 0x0001 | 0x0001 | 0x0002 | 0x0002 | 0x0003 | 0x0003 | 0x0004 | 0x0004 |
| two-line DAC | mono | dout | 0x0002 | 0x0002 | 0x0004 | 0x0004 | 0x0006 | 0x0006 | 0x0008 | 0x0008 |
| dout2 | 0x0000 | 0x0000 | 0x0000 | 0x0000 | 0x0000 | 0x0000 | 0x0000 | 0x0000 |
| stereo | dout | 0x0002 | 0x0002 | 0x0004 | 0x0004 | 0x0006 | 0x0006 | 0x0008 | 0x0008 |
| dout2 | 0x0001 | 0x0001 | 0x0003 | 0x0003 | 0x0005 | 0x0005 | 0x0007 | 0x0007 |



Note


There are three line modes for PDM TX mode, i.e., `I2S_PDM_TX_ONE_LINE_CODEC`, `I2S_PDM_TX_ONE_LINE_DAC`, and `I2S_PDM_TX_TWO_LINE_DAC`. One-line codec is for the PDM codecs that require clock signal. The PDM codec can differentiate the left and right slots by the clock level. The other two modes are used to drive power amplifiers directly with a low-pass filter. They do not need the clock signal, so there are two lines to differentiate the left and right slots. Additionally, for the mono mode of one-line codec, users can force change the slot to the right by setting the clock invert flag in GPIO configuration.




```
#include "driver/i2s_pdm.h"
#include "driver/gpio.h"

/* Allocate an I2S TX channel */
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
i2s_new_channel(&chan_cfg, &tx_handle, NULL);

/* Init the channel into PDM TX mode */
i2s_pdm_tx_config_t pdm_tx_cfg = {
    .clk_cfg = I2S_PDM_TX_CLK_DEFAULT_CONFIG(36000),
    .slot_cfg = I2S_PDM_TX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
    .gpio_cfg = {
        .clk = GPIO_NUM_5,
        .dout = GPIO_NUM_18,
        .invert_flags = {
            .clk_inv = false,
        },
    },
};
i2s_channel_init_pdm_tx_mode(tx_handle, &pdm_tx_cfg);

...

```




### PDM RX Usage


For PDM mode in RX channel, the slot configuration helper macro is:


* `I2S_PDM_RX_SLOT_DEFAULT_CONFIG`


The clock configuration helper macro is:


* `I2S_PDM_RX_CLK_DEFAULT_CONFIG`


Please refer to PDM Mode for information about PDM RX API. And for more details, please refer to esp_driver_i2s/include/driver/i2s_pdm.h.


The PDM data width is fixed to 16-bit. When the data on the line (The PDM format on the line is transferred to PCM format for easier comprehension) is:














| left | right | left | right | left | right | left | right | ... |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005 | 0x0006 | 0x0007 | 0x0008 | ... |


Here is the table of the data received in a `int16_t` buffer with different `i2s_pdm_rx_slot_config_t::slot_mode` and `i2s_pdm_rx_slot_config_t::slot_mask`.















| slot mode | slot mask | data 0 | data 1 | data 2 | data 3 | data 4 | data 5 | data 6 | data 7 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| mono | left | 0x0001 | 0x0003 | 0x0005 | 0x0007 | 0x0009 | 0x000b | 0x000d | 0x000f |
| right | 0x0002 | 0x0004 | 0x0006 | 0x0008 | 0x000a | 0x000c | 0x000e | 0x0010 |
| stereo | both | 0x0002 | 0x0001 | 0x0004 | 0x0003 | 0x0006 | 0x0005 | 0x0008 | 0x0007 |



Note


The right slot is received first in stereo mode. To switch the left and right slots in the buffer, please set the `i2s_pdm_rx_gpio_config_t::invert_flags::clk_inv` to force invert the clock signal.


Specially, ESP32-S3 supports up to 4 data lines in PDM RX mode, where each data line can be connected to two PDM MICs (left and right slots). This means that the PDM RX on ESP32-S3 can support up to 8 PDM MICs. To enable multiple data lines, set the bits in `i2s_pdm_rx_gpio_config_t::slot_mask` to enable corresponding slots first, and then set the data GPIOs in `i2s_pdm_rx_gpio_config_t`.




```
#include "driver/i2s_pdm.h"
#include "driver/gpio.h"

i2s_chan_handle_t rx_handle;

/* Allocate an I2S RX channel */
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
i2s_new_channel(&chan_cfg, NULL, &rx_handle);

/* Init the channel into PDM RX mode */
i2s_pdm_rx_config_t pdm_rx_cfg = {
    .clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(36000),
    .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
    .gpio_cfg = {
        .clk = GPIO_NUM_5,
        .din = GPIO_NUM_19,
        .invert_flags = {
            .clk_inv = false,
        },
    },
};
i2s_channel_init_pdm_rx_mode(rx_handle, &pdm_rx_cfg);

...

```




### TDM TX/RX Usage


Different slot communication formats can be generated by the following helper macros for TDM mode. As described above, there are four formats in TDM mode, and their helper macros are:


* `I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG`
* `I2S_TDM_MSB_SLOT_DEFAULT_CONFIG`
* `I2S_TDM_PCM_SHORT_SLOT_DEFAULT_CONFIG`
* `I2S_TDM_PCM_LONG_SLOT_DEFAULT_CONFIG`


The clock config helper macro is:


* `I2S_TDM_CLK_DEFAULT_CONFIG`


Please refer to TDM Mode for information about TDM API. And for more details, please refer to esp_driver_i2s/include/driver/i2s_tdm.h.



Note


Due to hardware limitation, when setting the clock configuration for a slave role, please be aware that `i2s_tdm_clk_config_t::bclk_div` should not be smaller than 8. Increasing this field can reduce the lagging of the data sent from the slave. In the high sample rate case, the data might lag behind for more than one BCLK which leads to data malposition. Users may gradually increase `i2s_tdm_clk_config_t::bclk_div` to correct it.


As `i2s_tdm_clk_config_t::bclk_div` is the division of MCLK to BCLK, increasing it also increases the MCLK frequency. Therefore, the clock calculation may fail if MCLK is too high to divide from the source clock. This means that a larger value for `i2s_tdm_clk_config_t::bclk_div` is not necessarily better.




#### TDM TX Mode



```
#include "driver/i2s_tdm.h"
#include "driver/gpio.h"

/* Allocate an I2S TX channel */
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
i2s_new_channel(&chan_cfg, &tx_handle, NULL);

/* Init the channel into TDM mode */
i2s_tdm_config_t tdm_cfg = {
    .clk_cfg = I2S_TDM_CLK_DEFAULT_CONFIG(44100),
    .slot_cfg = I2S_TDM_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO,
                I2S_TDM_SLOT0 | I2S_TDM_SLOT1 | I2S_TDM_SLOT2 | I2S_TDM_SLOT3),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = GPIO_NUM_4,
        .ws = GPIO_NUM_5,
        .dout = GPIO_NUM_18,
        .din = I2S_GPIO_UNUSED,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};
i2s_channel_init_tdm_mode(tx_handle, &tdm_cfg);

...

```




#### TDM RX Mode



```
#include "driver/i2s_tdm.h"
#include "driver/gpio.h"

/* Set the channel mode to TDM */
i2s_chan_config_t chan_cfg = I2S_CHANNEL_CONFIG(I2S_ROLE_MASTER, I2S_COMM_MODE_TDM, &i2s_pin);
i2s_new_channel(&chan_cfg, NULL, &rx_handle);

/* Init the channel into TDM mode */
i2s_tdm_config_t tdm_cfg = {
    .clk_cfg = I2S_TDM_CLK_DEFAULT_CONFIG(44100),
    .slot_cfg = I2S_TDM_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO,
                I2S_TDM_SLOT0 | I2S_TDM_SLOT1 | I2S_TDM_SLOT2 | I2S_TDM_SLOT3),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = GPIO_NUM_4,
        .ws = GPIO_NUM_5,
        .dout = I2S_GPIO_UNUSED,
        .din = GPIO_NUM_18,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};
i2s_channel_init_tdm_mode(rx_handle, &tdm_cfg);
...

```





### Full-duplex


Full-duplex mode registers TX and RX channel in an I2S port at the same time, and the channels share the BCLK and WS signals. Currently, STD and TDM communication modes supports full-duplex mode in the following way, but PDM full-duplex is not supported because due to different PDM TX and RX clocks.


Note that one handle can only stand for one channel. Therefore, it is still necessary to configure the slot and clock for both TX and RX channels one by one.


Here is an example of how to allocate a pair of full-duplex channels:



```
#include "driver/i2s_std.h"
#include "driver/gpio.h"

i2s_chan_handle_t tx_handle;
i2s_chan_handle_t rx_handle;

/* Allocate a pair of I2S channel */
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
/* Allocate for TX and RX channel at the same time, then they will work in full-duplex mode */
i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle);

/* Set the configurations for BOTH TWO channels, since TX and RX channel have to be same in full-duplex mode */
i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(32000),
    .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = GPIO_NUM_4,
        .ws = GPIO_NUM_5,
        .dout = GPIO_NUM_18,
        .din = GPIO_NUM_19,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};
i2s_channel_init_std_mode(tx_handle, &std_cfg);
i2s_channel_init_std_mode(rx_handle, &std_cfg);

i2s_channel_enable(tx_handle);
i2s_channel_enable(rx_handle);

...

```




### Simplex Mode


To allocate a channel in simplex mode, `i2s_new_channel()` should be called for each channel. The clock and GPIO pins of TX/RX channel on ESP32-S3 are independent, so they can be configured with different modes and clocks, and are able to coexist on the same I2S port in simplex mode. PDM duplex can be realized by registering PDM TX simplex and PDM RX simplex on the same I2S port. But in this way, PDM TX/RX might work with different clocks, so take care when configuring the GPIO pins and clocks.


The following example offers a use case for the simplex mode, but note that although the internal MCLK signals for TX and RX channel are separate, the output MCLK can only be bound to one of them if they are from the same controller. If MCLK has been initialized by both channels, it will be bound to the channel that initializes later.



```
#include "driver/i2s_std.h"
#include "driver/gpio.h"

i2s_chan_handle_t tx_handle;
i2s_chan_handle_t rx_handle;
i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
i2s_new_channel(&chan_cfg, &tx_handle, NULL);
i2s_std_config_t std_tx_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(48000),
    .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
        .mclk = GPIO_NUM_0,
        .bclk = GPIO_NUM_4,
        .ws = GPIO_NUM_5,
        .dout = GPIO_NUM_18,
        .din = I2S_GPIO_UNUSED,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};
/* Initialize the channel */
i2s_channel_init_std_mode(tx_handle, &std_tx_cfg);
i2s_channel_enable(tx_handle);

/* RX channel will be registered on another I2S, if no other available I2S unit found
 * it will return ESP_ERR_NOT_FOUND */
i2s_new_channel(&chan_cfg, NULL, &rx_handle); // Both RX and TX channel will be registered on I2S0, but they can work with different configurations.
i2s_std_config_t std_rx_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000),
    .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = GPIO_NUM_6,
        .ws = GPIO_NUM_7,
        .dout = I2S_GPIO_UNUSED,
        .din = GPIO_NUM_19,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};
i2s_channel_init_std_mode(rx_handle, &std_rx_cfg);
i2s_channel_enable(rx_handle);

```





## Application Notes



### How to Prevent Data Lost


For applications that need a high frequency sample rate, the massive data throughput may cause data lost. Users can receive data lost event by registering the ISR callback function to receive the event queue:



> ```
> static IRAM_ATTR bool i2s_rx_queue_overflow_callback(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
> {
>     // handle RX queue overflow event ...
>     return false;
> }
> 
> i2s_event_callbacks_t cbs = {
>     .on_recv = NULL,
>     .on_recv_q_ovf = i2s_rx_queue_overflow_callback,
>     .on_sent = NULL,
>     .on_send_q_ovf = NULL,
> };
> TEST_ESP_OK(i2s_channel_register_event_callback(rx_handle, &cbs, NULL));
> 
> ```


Please follow these steps to prevent data lost:


1. Determine the interrupt interval. Generally, when data lost happens, the bigger the interval, the better, which helps to reduce the interrupt times. This means `dma_frame_num` should be as big as possible while the DMA buffer size is below the maximum value of 4092. The relationships are:



```
interrupt_interval(unit: sec) = dma_frame_num / sample_rate
dma_buffer_size = dma_frame_num * slot_num * data_bit_width / 8 <= 4092

```
2. Determine `dma_desc_num`. `dma_desc_num` is decided by the maximum time of `i2s_channel_read` polling cycle. All the received data is supposed to be stored between two `i2s_channel_read`. This cycle can be measured by a timer or an outputting GPIO signal. The relationship is:



```
dma_desc_num > polling_cycle / interrupt_interval

```
3. Determine the receiving buffer size. The receiving buffer offered by users in `i2s_channel_read` should be able to take all the data in all DMA buffers, which means that it should be larger than the total size of all the DMA buffers:



```
recv_buffer_size > dma_desc_num * dma_buffer_size

```


For example, if there is an I2S application, and the known values are:



```
sample_rate = 144000 Hz
data_bit_width = 32 bits
slot_num = 2
polling_cycle = 10 ms

```


Then the parameters `dma_frame_num`, `dma_desc_num`, and `recv_buf_size` can be calculated as follows:



```
dma_frame_num * slot_num * data_bit_width / 8 = dma_buffer_size <= 4092
dma_frame_num <= 511
interrupt_interval = dma_frame_num / sample_rate = 511 / 144000 = 0.003549 s = 3.549 ms
dma_desc_num > polling_cycle / interrupt_interval = cell(10 / 3.549) = cell(2.818) = 3
recv_buffer_size > dma_desc_num * dma_buffer_size = 3 * 4092 = 12276 bytes

```





## API Reference



### Standard Mode




### Header File


* components/esp_driver_i2s/include/driver/i2s_std.h
* This header file can be included with:



> ```
> #include "driver/i2s_std.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_i2s` component. To declare that your component depends on `esp_driver_i2s`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_i2s
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_i2s
> 
> ```




### Functions


#### i2s_channel_init_std_mode


```


esp_err_t i2s_channel_init_std_mode(i2s_chan_handle_t handle, const i2s_std_config_t *std_cfg)
Initialize I2S channel to standard mode. 

Note
Only allowed to be called when the channel state is REGISTERED, (i.e., channel has been allocated, but not initialized) and the state will be updated to READY if initialization success, otherwise the state will return to REGISTERED.


Parameters

handle -- [in] I2S channel handler 
std_cfg -- [in] Configurations for standard mode, including clock, slot and GPIO The clock configuration can be generated by the helper macro I2S_STD_CLK_DEFAULT_CONFIG The slot configuration can be generated by the helper macro I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG, I2S_STD_PCM_SLOT_DEFAULT_CONFIG or I2S_STD_MSB_SLOT_DEFAULT_CONFIG


Returns

ESP_OK Initialize successfully
ESP_ERR_NO_MEM No memory for storing the channel information
ESP_ERR_INVALID_ARG NULL pointer or invalid configuration
ESP_ERR_INVALID_STATE This channel is not registered 





```

#### i2s_channel_reconfig_std_clock


```


esp_err_t i2s_channel_reconfig_std_clock(i2s_chan_handle_t handle, const i2s_std_clk_config_t *clk_cfg)
Reconfigure the I2S clock for standard mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to standard mode, i.e., i2s_channel_init_std_mode has been called before reconfiguring


Parameters

handle -- [in] I2S channel handler 
clk_cfg -- [in] Standard mode clock configuration, can be generated by I2S_STD_CLK_DEFAULT_CONFIG


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not standard mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_std_slot


```


esp_err_t i2s_channel_reconfig_std_slot(i2s_chan_handle_t handle, const i2s_std_slot_config_t *slot_cfg)
Reconfigure the I2S slot for standard mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to standard mode, i.e., i2s_channel_init_std_mode has been called before reconfiguring


Parameters

handle -- [in] I2S channel handler 
slot_cfg -- [in] Standard mode slot configuration, can be generated by I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG, I2S_STD_PCM_SLOT_DEFAULT_CONFIG and I2S_STD_MSB_SLOT_DEFAULT_CONFIG. 


Returns

ESP_OK Set clock successfully
ESP_ERR_NO_MEM No memory for DMA buffer
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not standard mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_std_gpio


```


esp_err_t i2s_channel_reconfig_std_gpio(i2s_chan_handle_t handle, const i2s_std_gpio_config_t *gpio_cfg)
Reconfigure the I2S GPIO for standard mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to standard mode, i.e., i2s_channel_init_std_mode has been called before reconfiguring


Parameters

handle -- [in] I2S channel handler 
gpio_cfg -- [in] Standard mode GPIO configuration, specified by user 


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not standard mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```



### Structures


#### i2s_std_slot_config_t


```


struct i2s_std_slot_config_t
I2S slot configuration for standard mode. 

Public Members
data_bit_width

i2s_data_bit_width_t data_bit_width
I2S sample data bit width (valid data bits per sample) 

slot_bit_width

i2s_slot_bit_width_t slot_bit_width
I2S slot bit width (total bits per slot) 

slot_mode

i2s_slot_mode_t slot_mode
Set mono or stereo mode with I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO In TX direction, mono means the written buffer contains only one slot data and stereo means the written buffer contains both left and right data 

slot_mask

i2s_std_slot_mask_t slot_mask
Select the left, right or both slot 

ws_width

uint32_t ws_width
WS signal width (i.e. the number of BCLK ticks that WS signal is high) 

ws_pol

bool ws_pol
WS signal polarity, set true to enable high lever first 

bit_shift

bool bit_shift
Set to enable bit shift in Philips mode 

left_align

bool left_align
Set to enable left alignment 

big_endian

bool big_endian
Set to enable big endian 

bit_order_lsb

bool bit_order_lsb
Set to enable lsb first 



```

#### i2s_std_clk_config_t


```


struct i2s_std_clk_config_t
I2S clock configuration for standard mode. 

Public Members
sample_rate_hz

uint32_t sample_rate_hz
I2S sample rate 

clk_src

i2s_clock_src_t clk_src
Choose clock source, see soc_periph_i2s_clk_src_t for the supported clock sources. selected I2S_CLK_SRC_EXTERNAL (if supports) to enable the external source clock input via MCLK pin, 

ext_clk_freq_hz

uint32_t ext_clk_freq_hz
External clock source frequency in Hz, only take effect when clk_src = I2S_CLK_SRC_EXTERNAL, otherwise this field will be ignored, Please make sure the frequency input is equal or greater than BCLK, i.e. sample_rate_hz * slot_bits * 2

mclk_multiple

i2s_mclk_multiple_t mclk_multiple
The multiple of MCLK to the sample rate Default is 256 in the helper macro, it can satisfy most of cases, but please set this field a multiple of 3 (like 384) when using 24-bit data width, otherwise the sample rate might be inaccurate 



```

#### i2s_std_gpio_config_t


```


struct i2s_std_gpio_config_t
I2S standard mode GPIO pins configuration. 

Public Members
mclk

gpio_num_t mclk
MCK pin, output by default, input if the clock source is selected to I2S_CLK_SRC_EXTERNAL

bclk

gpio_num_t bclk
BCK pin, input in slave role, output in master role 

ws

gpio_num_t ws
WS pin, input in slave role, output in master role 

dout

gpio_num_t dout
DATA pin, output 

din

gpio_num_t din
DATA pin, input 

mclk_inv

uint32_t mclk_inv
Set 1 to invert the MCLK input/output 

bclk_inv

uint32_t bclk_inv
Set 1 to invert the BCLK input/output 

ws_inv

uint32_t ws_inv
Set 1 to invert the WS input/output 

invert_flags

struct i2s_std_gpio_config_t::[anonymous] invert_flags
GPIO pin invert flags 



```

#### i2s_std_config_t


```


struct i2s_std_config_t
I2S standard mode major configuration that including clock/slot/GPIO configuration. 

Public Members
clk_cfg

i2s_std_clk_config_t clk_cfg
Standard mode clock configuration, can be generated by macro I2S_STD_CLK_DEFAULT_CONFIG 

slot_cfg

i2s_std_slot_config_t slot_cfg
Standard mode slot configuration, can be generated by macros I2S_STD_[mode]_SLOT_DEFAULT_CONFIG, [mode] can be replaced with PHILIPS/MSB/PCM 

gpio_cfg

i2s_std_gpio_config_t gpio_cfg
Standard mode GPIO configuration, specified by user 



```



### Macros


#### I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG


```


I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo)
Philips format in 2 slots. 
This file is specified for I2S standard communication mode Features:
Philips/MSB/PCM are supported in standard mode
Fixed to 2 slots 



Parameters

bits_per_sample -- I2S data bit width 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 




```

#### I2S_STD_PCM_SLOT_DEFAULT_CONFIG


```


I2S_STD_PCM_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo)
PCM(short) format in 2 slots. 

Note
PCM(long) is same as Philips in 2 slots 


Parameters

bits_per_sample -- I2S data bit width 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 




```

#### I2S_STD_MSB_SLOT_DEFAULT_CONFIG


```


I2S_STD_MSB_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo)
MSB format in 2 slots. 

Parameters

bits_per_sample -- I2S data bit width 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 




```

#### I2S_STD_CLK_DEFAULT_CONFIG


```


I2S_STD_CLK_DEFAULT_CONFIG(rate)
I2S default standard clock configuration. 

Note
Please set the mclk_multiple to I2S_MCLK_MULTIPLE_384 while using 24 bits data width Otherwise the sample rate might be imprecise since the BCLK division is not a integer 


Parameters

rate -- sample rate 




```



### PDM Mode




### Header File


* components/esp_driver_i2s/include/driver/i2s_pdm.h
* This header file can be included with:



> ```
> #include "driver/i2s_pdm.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_i2s` component. To declare that your component depends on `esp_driver_i2s`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_i2s
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_i2s
> 
> ```




### Functions


#### i2s_channel_init_pdm_rx_mode


```


esp_err_t i2s_channel_init_pdm_rx_mode(i2s_chan_handle_t handle, const i2s_pdm_rx_config_t *pdm_rx_cfg)
Initialize I2S channel to PDM RX mode. 

Note
Only allowed to be called when the channel state is REGISTERED, (i.e., channel has been allocated, but not initialized) and the state will be updated to READY if initialization success, otherwise the state will return to REGISTERED.


Parameters

handle -- [in] I2S RX channel handler 
pdm_rx_cfg -- [in] Configurations for PDM RX mode, including clock, slot and GPIO The clock configuration can be generated by the helper macro I2S_PDM_RX_CLK_DEFAULT_CONFIG The slot configuration can be generated by the helper macro I2S_PDM_RX_SLOT_DEFAULT_CONFIG


Returns

ESP_OK Initialize successfully
ESP_ERR_NO_MEM No memory for storing the channel information
ESP_ERR_INVALID_ARG NULL pointer or invalid configuration
ESP_ERR_INVALID_STATE This channel is not registered 





```

#### i2s_channel_reconfig_pdm_rx_clock


```


esp_err_t i2s_channel_reconfig_pdm_rx_clock(i2s_chan_handle_t handle, const i2s_pdm_rx_clk_config_t *clk_cfg)
Reconfigure the I2S clock for PDM RX mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to PDM RX mode, i.e., i2s_channel_init_pdm_rx_mode has been called before reconfiguring


Parameters

handle -- [in] I2S RX channel handler 
clk_cfg -- [in] PDM RX mode clock configuration, can be generated by I2S_PDM_RX_CLK_DEFAULT_CONFIG


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not PDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_pdm_rx_slot


```


esp_err_t i2s_channel_reconfig_pdm_rx_slot(i2s_chan_handle_t handle, const i2s_pdm_rx_slot_config_t *slot_cfg)
Reconfigure the I2S slot for PDM RX mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to PDM RX mode, i.e., i2s_channel_init_pdm_rx_mode has been called before reconfiguring


Parameters

handle -- [in] I2S RX channel handler 
slot_cfg -- [in] PDM RX mode slot configuration, can be generated by I2S_PDM_RX_SLOT_DEFAULT_CONFIG


Returns

ESP_OK Set clock successfully
ESP_ERR_NO_MEM No memory for DMA buffer
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not PDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_pdm_rx_gpio


```


esp_err_t i2s_channel_reconfig_pdm_rx_gpio(i2s_chan_handle_t handle, const i2s_pdm_rx_gpio_config_t *gpio_cfg)
Reconfigure the I2S GPIO for PDM RX mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to PDM RX mode, i.e., i2s_channel_init_pdm_rx_mode has been called before reconfiguring


Parameters

handle -- [in] I2S RX channel handler 
gpio_cfg -- [in] PDM RX mode GPIO configuration, specified by user 


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not PDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_init_pdm_tx_mode


```


esp_err_t i2s_channel_init_pdm_tx_mode(i2s_chan_handle_t handle, const i2s_pdm_tx_config_t *pdm_tx_cfg)
Initialize I2S channel to PDM TX mode. 

Note
Only allowed to be called when the channel state is REGISTERED, (i.e., channel has been allocated, but not initialized) and the state will be updated to READY if initialization success, otherwise the state will return to REGISTERED.


Parameters

handle -- [in] I2S TX channel handler 
pdm_tx_cfg -- [in] Configurations for PDM TX mode, including clock, slot and GPIO The clock configuration can be generated by the helper macro I2S_PDM_TX_CLK_DEFAULT_CONFIG The slot configuration can be generated by the helper macro I2S_PDM_TX_SLOT_DEFAULT_CONFIG


Returns

ESP_OK Initialize successfully
ESP_ERR_NO_MEM No memory for storing the channel information
ESP_ERR_INVALID_ARG NULL pointer or invalid configuration
ESP_ERR_INVALID_STATE This channel is not registered 





```

#### i2s_channel_reconfig_pdm_tx_clock


```


esp_err_t i2s_channel_reconfig_pdm_tx_clock(i2s_chan_handle_t handle, const i2s_pdm_tx_clk_config_t *clk_cfg)
Reconfigure the I2S clock for PDM TX mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to PDM TX mode, i.e., i2s_channel_init_pdm_tx_mode has been called before reconfiguring


Parameters

handle -- [in] I2S TX channel handler 
clk_cfg -- [in] PDM TX mode clock configuration, can be generated by I2S_PDM_TX_CLK_DEFAULT_CONFIG


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not PDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_pdm_tx_slot


```


esp_err_t i2s_channel_reconfig_pdm_tx_slot(i2s_chan_handle_t handle, const i2s_pdm_tx_slot_config_t *slot_cfg)
Reconfigure the I2S slot for PDM TX mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to PDM TX mode, i.e., i2s_channel_init_pdm_tx_mode has been called before reconfiguring


Parameters

handle -- [in] I2S TX channel handler 
slot_cfg -- [in] PDM TX mode slot configuration, can be generated by I2S_PDM_TX_SLOT_DEFAULT_CONFIG


Returns

ESP_OK Set clock successfully
ESP_ERR_NO_MEM No memory for DMA buffer
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not PDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_pdm_tx_gpio


```


esp_err_t i2s_channel_reconfig_pdm_tx_gpio(i2s_chan_handle_t handle, const i2s_pdm_tx_gpio_config_t *gpio_cfg)
Reconfigure the I2S GPIO for PDM TX mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to PDM TX mode, i.e., i2s_channel_init_pdm_tx_mode has been called before reconfiguring


Parameters

handle -- [in] I2S TX channel handler 
gpio_cfg -- [in] PDM TX mode GPIO configuration, specified by user 


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not PDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```



### Structures


#### i2s_pdm_rx_slot_config_t


```


struct i2s_pdm_rx_slot_config_t
I2S slot configuration for PDM RX mode. 

Public Members
data_bit_width

i2s_data_bit_width_t data_bit_width
I2S sample data bit width (valid data bits per sample), only support 16 bits for PDM mode 

slot_bit_width

i2s_slot_bit_width_t slot_bit_width
I2S slot bit width (total bits per slot) , only support 16 bits for PDM mode 

slot_mode

i2s_slot_mode_t slot_mode
Set mono or stereo mode with I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 

slot_mask

i2s_pdm_slot_mask_t slot_mask
Choose the slots to activate 



```

#### i2s_pdm_rx_clk_config_t


```


struct i2s_pdm_rx_clk_config_t
I2S clock configuration for PDM RX mode. 

Public Members
sample_rate_hz

uint32_t sample_rate_hz
I2S sample rate 

clk_src

i2s_clock_src_t clk_src
Choose clock source 

mclk_multiple

i2s_mclk_multiple_t mclk_multiple
The multiple of MCLK to the sample rate 

dn_sample_mode

i2s_pdm_dsr_t dn_sample_mode
Down-sampling rate mode 

bclk_div

uint32_t bclk_div
The division from MCLK to BCLK. The typical and minimum value is I2S_PDM_RX_BCLK_DIV_MIN. It will be set to I2S_PDM_RX_BCLK_DIV_MIN by default if it is smaller than I2S_PDM_RX_BCLK_DIV_MIN 



```

#### i2s_pdm_rx_gpio_config_t


```


struct i2s_pdm_rx_gpio_config_t
I2S PDM TX mode GPIO pins configuration. 

Public Members
clk

gpio_num_t clk
PDM clk pin, output 

din

gpio_num_t din
DATA pin 0, input 

dins

gpio_num_t dins[(4)]
DATA pins, input, only take effect when corresponding I2S_PDM_RX_LINEx_SLOT_xxx is enabled in i2s_pdm_rx_slot_config_t::slot_mask

clk_inv

uint32_t clk_inv
Set 1 to invert the clk output 

invert_flags

struct i2s_pdm_rx_gpio_config_t::[anonymous] invert_flags
GPIO pin invert flags 



```

#### i2s_pdm_rx_config_t


```


struct i2s_pdm_rx_config_t
I2S PDM RX mode major configuration that including clock/slot/GPIO configuration. 

Public Members
clk_cfg

i2s_pdm_rx_clk_config_t clk_cfg
PDM RX clock configurations, can be generated by macro I2S_PDM_RX_CLK_DEFAULT_CONFIG 

slot_cfg

i2s_pdm_rx_slot_config_t slot_cfg
PDM RX slot configurations, can be generated by macro I2S_PDM_RX_SLOT_DEFAULT_CONFIG 

gpio_cfg

i2s_pdm_rx_gpio_config_t gpio_cfg
PDM RX slot configurations, specified by user 



```

#### i2s_pdm_tx_slot_config_t


```


struct i2s_pdm_tx_slot_config_t
I2S slot configuration for PDM TX mode. 

Public Members
data_bit_width

i2s_data_bit_width_t data_bit_width
I2S sample data bit width (valid data bits per sample), only support 16 bits for PDM mode 

slot_bit_width

i2s_slot_bit_width_t slot_bit_width
I2S slot bit width (total bits per slot), only support 16 bits for PDM mode 

slot_mode

i2s_slot_mode_t slot_mode
Set mono or stereo mode with I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO For PDM TX mode, mono means the data buffer only contains one slot data, Stereo means the data buffer contains two slots data 

sd_prescale

uint32_t sd_prescale
Sigma-delta filter prescale 

sd_scale

i2s_pdm_sig_scale_t sd_scale
Sigma-delta filter scaling value 

hp_scale

i2s_pdm_sig_scale_t hp_scale
High pass filter scaling value 

lp_scale

i2s_pdm_sig_scale_t lp_scale
Low pass filter scaling value 

sinc_scale

i2s_pdm_sig_scale_t sinc_scale
Sinc filter scaling value 

line_mode

i2s_pdm_tx_line_mode_t line_mode
PDM TX line mode, one-line codec, one-line dac, two-line dac mode can be selected 

hp_en

bool hp_en
High pass filter enable 

hp_cut_off_freq_hz

float hp_cut_off_freq_hz
High pass filter cut-off frequency, range 23.3Hz ~ 185Hz, see cut-off frequency sheet above 

sd_dither

uint32_t sd_dither
Sigma-delta filter dither 

sd_dither2

uint32_t sd_dither2
Sigma-delta filter dither2 



```

#### i2s_pdm_tx_clk_config_t


```


struct i2s_pdm_tx_clk_config_t
I2S clock configuration for PDM TX mode. 

Public Members
sample_rate_hz

uint32_t sample_rate_hz
I2S sample rate, not suggest to exceed 48000 Hz, otherwise more glitches and noise may appear 

clk_src

i2s_clock_src_t clk_src
Choose clock source 

mclk_multiple

i2s_mclk_multiple_t mclk_multiple
The multiple of MCLK to the sample rate 

up_sample_fp

uint32_t up_sample_fp
Up-sampling param fp 

up_sample_fs

uint32_t up_sample_fs
Up-sampling param fs, not allowed to be greater than 480 

bclk_div

uint32_t bclk_div
The division from MCLK to BCLK. The minimum value is I2S_PDM_TX_BCLK_DIV_MIN. It will be set to I2S_PDM_TX_BCLK_DIV_MIN by default if it is smaller than I2S_PDM_TX_BCLK_DIV_MIN 



```

#### i2s_pdm_tx_gpio_config_t


```


struct i2s_pdm_tx_gpio_config_t
I2S PDM TX mode GPIO pins configuration. 

Public Members
clk

gpio_num_t clk
PDM clk pin, output 

dout

gpio_num_t dout
DATA pin, output 

dout2

gpio_num_t dout2
The second data pin for the DAC dual-line mode, only take effect when the line mode is I2S_PDM_TX_TWO_LINE_DAC

clk_inv

uint32_t clk_inv
Set 1 to invert the clk output 

invert_flags

struct i2s_pdm_tx_gpio_config_t::[anonymous] invert_flags
GPIO pin invert flags 



```

#### i2s_pdm_tx_config_t


```


struct i2s_pdm_tx_config_t
I2S PDM TX mode major configuration that including clock/slot/GPIO configuration. 

Public Members
clk_cfg

i2s_pdm_tx_clk_config_t clk_cfg
PDM TX clock configurations, can be generated by macro I2S_PDM_TX_CLK_DEFAULT_CONFIG 

slot_cfg

i2s_pdm_tx_slot_config_t slot_cfg
PDM TX slot configurations, can be generated by macro I2S_PDM_TX_SLOT_DEFAULT_CONFIG 

gpio_cfg

i2s_pdm_tx_gpio_config_t gpio_cfg
PDM TX GPIO configurations, specified by user 



```



### Macros


#### I2S_PDM_RX_SLOT_DEFAULT_CONFIG


```


I2S_PDM_RX_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo)
PDM format in 2 slots(RX) 
This file is specified for I2S PDM communication mode Features:
Only support PDM TX/RX mode
Fixed to 2 slots
Data bit width only support 16 bits 



Parameters

bits_per_sample -- I2S data bit width, only support 16 bits for PDM mode 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 




```

#### I2S_PDM_RX_CLK_DEFAULT_CONFIG


```


I2S_PDM_RX_CLK_DEFAULT_CONFIG(rate)
I2S default PDM RX clock configuration. 

Parameters

rate -- sample rate 




```

#### I2S_PDM_TX_SLOT_DEFAULT_CONFIG


```


I2S_PDM_TX_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo)
PDM style in 2 slots(TX) for codec line mode. 

Parameters

bits_per_sample -- I2S data bit width, only support 16 bits for PDM mode 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 




```

#### I2S_PDM_TX_SLOT_DAC_DEFAULT_CONFIG


```


I2S_PDM_TX_SLOT_DAC_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo)
PDM style in 1 slots(TX) for DAC line mode. 

Note
The noise might be different with different configurations, this macro provides a set of configurations that have relatively high SNR (Signal Noise Ratio), you can also adjust them to fit your case. 


Parameters

bits_per_sample -- I2S data bit width, only support 16 bits for PDM mode 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 




```

#### I2S_PDM_TX_CLK_DEFAULT_CONFIG


```


I2S_PDM_TX_CLK_DEFAULT_CONFIG(rate)
I2S default PDM TX clock configuration for codec line mode. 

Note
TX PDM can only be set to the following two up-sampling rate configurations: 1: fp = 960, fs = sample_rate_hz / 100, in this case, Fpdm = 128*48000 2: fp = 960, fs = 480, in this case, Fpdm = 128*Fpcm = 128*sample_rate_hz If the PDM receiver do not care the PDM serial clock, it's recommended set Fpdm = 128*48000. Otherwise, the second configuration should be adopted. 


Parameters

rate -- sample rate (not suggest to exceed 48000 Hz, otherwise more glitches and noise may appear) 




```

#### I2S_PDM_TX_CLK_DAC_DEFAULT_CONFIG


```


I2S_PDM_TX_CLK_DAC_DEFAULT_CONFIG(rate)
I2S default PDM TX clock configuration for DAC line mode. 

Note
TX PDM can only be set to the following two up-sampling rate configurations: 1: fp = 960, fs = sample_rate_hz / 100, in this case, Fpdm = 128*48000 2: fp = 960, fs = 480, in this case, Fpdm = 128*Fpcm = 128*sample_rate_hz If the PDM receiver do not care the PDM serial clock, it's recommended set Fpdm = 128*48000. Otherwise, the second configuration should be adopted. 


Note
The noise might be different with different configurations, this macro provides a set of configurations that have relatively high SNR (Signal Noise Ratio), you can also adjust them to fit your case. 


Parameters

rate -- sample rate (not suggest to exceed 48000 Hz, otherwise more glitches and noise may appear) 




```



### TDM Mode




### Header File


* components/esp_driver_i2s/include/driver/i2s_tdm.h
* This header file can be included with:



> ```
> #include "driver/i2s_tdm.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_i2s` component. To declare that your component depends on `esp_driver_i2s`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_i2s
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_i2s
> 
> ```




### Functions


#### i2s_channel_init_tdm_mode


```


esp_err_t i2s_channel_init_tdm_mode(i2s_chan_handle_t handle, const i2s_tdm_config_t *tdm_cfg)
Initialize I2S channel to TDM mode. 

Note
Only allowed to be called when the channel state is REGISTERED, (i.e., channel has been allocated, but not initialized) and the state will be updated to READY if initialization success, otherwise the state will return to REGISTERED.


Parameters

handle -- [in] I2S channel handler 
tdm_cfg -- [in] Configurations for TDM mode, including clock, slot and GPIO The clock configuration can be generated by the helper macro I2S_TDM_CLK_DEFAULT_CONFIG The slot configuration can be generated by the helper macro I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG, I2S_TDM_PCM_SHORT_SLOT_DEFAULT_CONFIG, I2S_TDM_PCM_LONG_SLOT_DEFAULT_CONFIG or I2S_TDM_MSB_SLOT_DEFAULT_CONFIG


Returns

ESP_OK Initialize successfully
ESP_ERR_NO_MEM No memory for storing the channel information
ESP_ERR_INVALID_ARG NULL pointer or invalid configuration
ESP_ERR_INVALID_STATE This channel is not registered 





```

#### i2s_channel_reconfig_tdm_clock


```


esp_err_t i2s_channel_reconfig_tdm_clock(i2s_chan_handle_t handle, const i2s_tdm_clk_config_t *clk_cfg)
Reconfigure the I2S clock for TDM mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to TDM mode, i.e., i2s_channel_init_tdm_mode has been called before reconfiguring


Parameters

handle -- [in] I2S channel handler 
clk_cfg -- [in] Standard mode clock configuration, can be generated by I2S_TDM_CLK_DEFAULT_CONFIG


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not TDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_tdm_slot


```


esp_err_t i2s_channel_reconfig_tdm_slot(i2s_chan_handle_t handle, const i2s_tdm_slot_config_t *slot_cfg)
Reconfigure the I2S slot for TDM mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to TDM mode, i.e., i2s_channel_init_tdm_mode has been called before reconfiguring


Parameters

handle -- [in] I2S channel handler 
slot_cfg -- [in] Standard mode slot configuration, can be generated by I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG, I2S_TDM_PCM_SHORT_SLOT_DEFAULT_CONFIG, I2S_TDM_PCM_LONG_SLOT_DEFAULT_CONFIG or I2S_TDM_MSB_SLOT_DEFAULT_CONFIG. 


Returns

ESP_OK Set clock successfully
ESP_ERR_NO_MEM No memory for DMA buffer
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not TDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```

#### i2s_channel_reconfig_tdm_gpio


```


esp_err_t i2s_channel_reconfig_tdm_gpio(i2s_chan_handle_t handle, const i2s_tdm_gpio_config_t *gpio_cfg)
Reconfigure the I2S GPIO for TDM mode. 

Note
Only allowed to be called when the channel state is READY, i.e., channel has been initialized, but not started this function won't change the state. i2s_channel_disable should be called before calling this function if I2S has started. 


Note
The input channel handle has to be initialized to TDM mode, i.e., i2s_channel_init_tdm_mode has been called before reconfiguring


Parameters

handle -- [in] I2S channel handler 
gpio_cfg -- [in] Standard mode GPIO configuration, specified by user 


Returns

ESP_OK Set clock successfully
ESP_ERR_INVALID_ARG NULL pointer, invalid configuration or not TDM mode
ESP_ERR_INVALID_STATE This channel is not initialized or not stopped 





```



### Structures


#### i2s_tdm_slot_config_t


```


struct i2s_tdm_slot_config_t
I2S slot configuration for TDM mode. 

Public Members
data_bit_width

i2s_data_bit_width_t data_bit_width
I2S sample data bit width (valid data bits per sample) 

slot_bit_width

i2s_slot_bit_width_t slot_bit_width
I2S slot bit width (total bits per slot) 

slot_mode

i2s_slot_mode_t slot_mode
Set mono or stereo mode with I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 

slot_mask

i2s_tdm_slot_mask_t slot_mask
Slot mask. Activating slots by setting 1 to corresponding bits. When the activated slots is not consecutive, those data in inactivated slots will be ignored 

ws_width

uint32_t ws_width
WS signal width (i.e. the number of BCLK ticks that WS signal is high) 

ws_pol

bool ws_pol
WS signal polarity, set true to enable high lever first 

bit_shift

bool bit_shift
Set true to enable bit shift in Philips mode 

left_align

bool left_align
Set true to enable left alignment 

big_endian

bool big_endian
Set true to enable big endian 

bit_order_lsb

bool bit_order_lsb
Set true to enable lsb first 

skip_mask

bool skip_mask
Set true to enable skip mask. If it is enabled, only the data of the enabled channels will be sent, otherwise all data stored in DMA TX buffer will be sent 

total_slot

uint32_t total_slot
I2S total number of slots. If it is smaller than the biggest activated channel number, it will be set to this number automatically. 



```

#### i2s_tdm_clk_config_t


```


struct i2s_tdm_clk_config_t
I2S clock configuration for TDM mode. 

Public Members
sample_rate_hz

uint32_t sample_rate_hz
I2S sample rate 

clk_src

i2s_clock_src_t clk_src
Choose clock source, see soc_periph_i2s_clk_src_t for the supported clock sources. selected I2S_CLK_SRC_EXTERNAL (if supports) to enable the external source clock inputted via MCLK pin, please make sure the frequency inputted is equal or greater than sample_rate_hz * mclk_multiple

ext_clk_freq_hz

uint32_t ext_clk_freq_hz
External clock source frequency in Hz, only take effect when clk_src = I2S_CLK_SRC_EXTERNAL, otherwise this field will be ignored Please make sure the frequency inputted is equal or greater than BCLK, i.e. sample_rate_hz * slot_bits * slot_num

mclk_multiple

i2s_mclk_multiple_t mclk_multiple
The multiple of MCLK to the sample rate, only take effect for master role 

bclk_div

uint32_t bclk_div
The division from MCLK to BCLK, only take effect for slave role, it shouldn't be smaller than 8. Increase this field when data sent by slave lag behind 



```

#### i2s_tdm_gpio_config_t


```


struct i2s_tdm_gpio_config_t
I2S TDM mode GPIO pins configuration. 

Public Members
mclk

gpio_num_t mclk
MCK pin, output by default, input if the clock source is selected to I2S_CLK_SRC_EXTERNAL

bclk

gpio_num_t bclk
BCK pin, input in slave role, output in master role 

ws

gpio_num_t ws
WS pin, input in slave role, output in master role 

dout

gpio_num_t dout
DATA pin, output 

din

gpio_num_t din
DATA pin, input 

mclk_inv

uint32_t mclk_inv
Set 1 to invert the MCLK input/output 

bclk_inv

uint32_t bclk_inv
Set 1 to invert the BCLK input/output 

ws_inv

uint32_t ws_inv
Set 1 to invert the WS input/output 

invert_flags

struct i2s_tdm_gpio_config_t::[anonymous] invert_flags
GPIO pin invert flags 



```

#### i2s_tdm_config_t


```


struct i2s_tdm_config_t
I2S TDM mode major configuration that including clock/slot/GPIO configuration. 

Public Members
clk_cfg

i2s_tdm_clk_config_t clk_cfg
TDM mode clock configuration, can be generated by macro I2S_TDM_CLK_DEFAULT_CONFIG 

slot_cfg

i2s_tdm_slot_config_t slot_cfg
TDM mode slot configuration, can be generated by macros I2S_TDM_[mode]_SLOT_DEFAULT_CONFIG, [mode] can be replaced with PHILIPS/MSB/PCM_SHORT/PCM_LONG 

gpio_cfg

i2s_tdm_gpio_config_t gpio_cfg
TDM mode GPIO configuration, specified by user 



```



### Macros


#### I2S_TDM_AUTO_SLOT_NUM


```


I2S_TDM_AUTO_SLOT_NUM
This file is specified for I2S TDM communication mode Features:
More than 2 slots 



```

#### I2S_TDM_AUTO_WS_WIDTH


```


I2S_TDM_AUTO_WS_WIDTH

```

#### I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG


```


I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo, mask)
Philips format in active slot that enabled by mask. 

Parameters

bits_per_sample -- I2S data bit width 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 
mask -- active slot mask 




```

#### I2S_TDM_MSB_SLOT_DEFAULT_CONFIG


```


I2S_TDM_MSB_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo, mask)
MSB format in active slot enabled that by mask. 

Parameters

bits_per_sample -- I2S data bit width 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 
mask -- active slot mask 




```

#### I2S_TDM_PCM_SHORT_SLOT_DEFAULT_CONFIG


```


I2S_TDM_PCM_SHORT_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo, mask)
PCM(short) format in active slot that enabled by mask. 

Parameters

bits_per_sample -- I2S data bit width 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 
mask -- active slot mask 




```

#### I2S_TDM_PCM_LONG_SLOT_DEFAULT_CONFIG


```


I2S_TDM_PCM_LONG_SLOT_DEFAULT_CONFIG(bits_per_sample, mono_or_stereo, mask)
PCM(long) format in active slot that enabled by mask. 

Parameters

bits_per_sample -- I2S data bit width 
mono_or_stereo -- I2S_SLOT_MODE_MONO or I2S_SLOT_MODE_STEREO 
mask -- active slot mask 




```

#### I2S_TDM_CLK_DEFAULT_CONFIG


```


I2S_TDM_CLK_DEFAULT_CONFIG(rate)
I2S default TDM clock configuration. 

Note
Please set the mclk_multiple to I2S_MCLK_MULTIPLE_384 while the data width in slot configuration is set to 24 bits Otherwise the sample rate might be imprecise since the BCLK division is not a integer 


Parameters

rate -- sample rate 




```



### I2S Driver




### Header File


* components/esp_driver_i2s/include/driver/i2s_common.h
* This header file can be included with:



> ```
> #include "driver/i2s_common.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_i2s` component. To declare that your component depends on `esp_driver_i2s`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_i2s
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_i2s
> 
> ```




### Functions


#### i2s_new_channel


```


esp_err_t i2s_new_channel(const i2s_chan_config_t *chan_cfg, i2s_chan_handle_t *ret_tx_handle, i2s_chan_handle_t *ret_rx_handle)
Allocate new I2S channel(s) 

Note
The new created I2S channel handle will be REGISTERED state after it is allocated successfully. 


Note
When the port id in channel configuration is I2S_NUM_AUTO, driver will allocate I2S port automatically on one of the I2S controller, otherwise driver will try to allocate the new channel on the selected port. 


Note
If both tx_handle and rx_handle are not NULL, it means this I2S controller will work at full-duplex mode, the RX and TX channels will be allocated on a same I2S port in this case. Note that some configurations of TX/RX channel are shared on ESP32 and ESP32S2, so please make sure they are working at same condition and under same status(start/stop). Currently, full-duplex mode can't guarantee TX/RX channels write/read synchronously, they can only share the clock signals for now. 


Note
If tx_handle OR rx_handle is NULL, it means this I2S controller will work at simplex mode. For ESP32 and ESP32S2, the whole I2S controller (i.e. both RX and TX channel) will be occupied, even if only one of RX or TX channel is registered. For the other targets, another channel on this controller will still available.


Parameters

chan_cfg -- [in] I2S controller channel configurations 
ret_tx_handle -- [out] I2S channel handler used for managing the sending channel(optional) 
ret_rx_handle -- [out] I2S channel handler used for managing the receiving channel(optional) 


Returns

ESP_OK Allocate new channel(s) success
ESP_ERR_NOT_SUPPORTED The communication mode is not supported on the current chip
ESP_ERR_INVALID_ARG NULL pointer or illegal parameter in i2s_chan_config_t
ESP_ERR_NOT_FOUND No available I2S channel found 





```

#### i2s_del_channel


```


esp_err_t i2s_del_channel(i2s_chan_handle_t handle)
Delete the I2S channel. 

Note
Only allowed to be called when the I2S channel is at REGISTERED or READY state (i.e., it should stop before deleting it). 


Note
Resource will be free automatically if all channels in one port are deleted


Parameters
handle -- [in] I2S channel handler
ESP_OK Delete successfully
ESP_ERR_INVALID_ARG NULL pointer 





```

#### i2s_channel_get_info


```


esp_err_t i2s_channel_get_info(i2s_chan_handle_t handle, i2s_chan_info_t *chan_info)
Get I2S channel information. 

Parameters

handle -- [in] I2S channel handler 
chan_info -- [out] I2S channel basic information 


Returns

ESP_OK Get I2S channel information success
ESP_ERR_NOT_FOUND The input handle doesn't match any registered I2S channels, it may not an I2S channel handle or not available any more
ESP_ERR_INVALID_ARG The input handle or chan_info pointer is NULL 





```

#### i2s_channel_enable


```


esp_err_t i2s_channel_enable(i2s_chan_handle_t handle)
Enable the I2S channel. 

Note
Only allowed to be called when the channel state is READY, (i.e., channel has been initialized, but not started) the channel will enter RUNNING state once it is enabled successfully. 


Note
Enable the channel can start the I2S communication on hardware. It will start outputting BCLK and WS signal. For MCLK signal, it will start to output when initialization is finished


Parameters
handle -- [in] I2S channel handler
ESP_OK Start successfully
ESP_ERR_INVALID_ARG NULL pointer
ESP_ERR_INVALID_STATE This channel has not initialized or already started 





```

#### i2s_channel_disable


```


esp_err_t i2s_channel_disable(i2s_chan_handle_t handle)
Disable the I2S channel. 

Note
Only allowed to be called when the channel state is RUNNING, (i.e., channel has been started) the channel will enter READY state once it is disabled successfully. 


Note
Disable the channel can stop the I2S communication on hardware. It will stop BCLK and WS signal but not MCLK signal


Parameters
handle -- [in] I2S channel handler 

Returns

ESP_OK Stop successfully
ESP_ERR_INVALID_ARG NULL pointer
ESP_ERR_INVALID_STATE This channel has not stated 





```

#### i2s_channel_preload_data


```


esp_err_t i2s_channel_preload_data(i2s_chan_handle_t tx_handle, const void *src, size_t size, size_t *bytes_loaded)
Preload the data into TX DMA buffer. 

Note
Only allowed to be called when the channel state is READY, (i.e., channel has been initialized, but not started) 


Note
As the initial DMA buffer has no data inside, it will transmit the empty buffer after enabled the channel, this function is used to preload the data into the DMA buffer, so that the valid data can be transmitted immediately after the channel is enabled. 


Note
This function can be called multiple times before enabling the channel, the buffer that loaded later will be concatenated behind the former loaded buffer. But when all the DMA buffers have been loaded, no more data can be preload then, please check the bytes_loaded parameter to see how many bytes are loaded successfully, when the bytes_loaded is smaller than the size, it means the DMA buffers are full.


Parameters

tx_handle -- [in] I2S TX channel handler 
src -- [in] The pointer of the source buffer to be loaded 
size -- [in] The source buffer size 
bytes_loaded -- [out] The bytes that successfully been loaded into the TX DMA buffer 


Returns

ESP_OK Load data successful
ESP_ERR_INVALID_ARG NULL pointer or not TX direction
ESP_ERR_INVALID_STATE This channel has not stated 





```

#### i2s_channel_write


```


esp_err_t i2s_channel_write(i2s_chan_handle_t handle, const void *src, size_t size, size_t *bytes_written, uint32_t timeout_ms)
I2S write data. 

Note
Only allowed to be called when the channel state is RUNNING, (i.e., TX channel has been started and is not writing now) but the RUNNING only stands for the software state, it doesn't mean there is no the signal transporting on line.


Parameters

handle -- [in] I2S channel handler 
src -- [in] The pointer of sent data buffer 
size -- [in] Max data buffer length 
bytes_written -- [out] Byte number that actually be sent, can be NULL if not needed 
timeout_ms -- [in] Max block time 


Returns

ESP_OK Write successfully
ESP_ERR_INVALID_ARG NULL pointer or this handle is not TX handle
ESP_ERR_TIMEOUT Writing timeout, no writing event received from ISR within ticks_to_wait
ESP_ERR_INVALID_STATE I2S is not ready to write 





```

#### i2s_channel_read


```


esp_err_t i2s_channel_read(i2s_chan_handle_t handle, void *dest, size_t size, size_t *bytes_read, uint32_t timeout_ms)
I2S read data. 

Note
Only allowed to be called when the channel state is RUNNING but the RUNNING only stands for the software state, it doesn't mean there is no the signal transporting on line.


Parameters

handle -- [in] I2S channel handler 
dest -- [in] The pointer of receiving data buffer 
size -- [in] Max data buffer length 
bytes_read -- [out] Byte number that actually be read, can be NULL if not needed 
timeout_ms -- [in] Max block time 


Returns

ESP_OK Read successfully
ESP_ERR_INVALID_ARG NULL pointer or this handle is not RX handle
ESP_ERR_TIMEOUT Reading timeout, no reading event received from ISR within ticks_to_wait
ESP_ERR_INVALID_STATE I2S is not ready to read 





```

#### i2s_channel_register_event_callback


```


esp_err_t i2s_channel_register_event_callback(i2s_chan_handle_t handle, const i2s_event_callbacks_t *callbacks, void *user_data)
Set event callbacks for I2S channel. 

Note
Only allowed to be called when the channel state is REGISTERED / READY, (i.e., before channel starts) 


Note
User can deregister a previously registered callback by calling this function and setting the callback member in the callbacks structure to NULL. 


Note
When CONFIG_I2S_ISR_IRAM_SAFE is enabled, the callback itself and functions called by it should be placed in IRAM. The variables used in the function should be in the SRAM as well. The user_data should also reside in SRAM or internal RAM as well.


Parameters

handle -- [in] I2S channel handler 
callbacks -- [in] Group of callback functions 
user_data -- [in] User data, which will be passed to callback functions directly 


Returns

ESP_OK Set event callbacks successfully
ESP_ERR_INVALID_ARG Set event callbacks failed because of invalid argument
ESP_ERR_INVALID_STATE Set event callbacks failed because the current channel state is not REGISTERED or READY 





```



### Structures


#### i2s_event_callbacks_t


```


struct i2s_event_callbacks_t
Group of I2S callbacks. 

Note
The callbacks are all running under ISR environment 


Note
When CONFIG_I2S_ISR_IRAM_SAFE is enabled, the callback itself and functions called by it should be placed in IRAM. The variables used in the function should be in the SRAM as well. 


Public Members
on_recv

i2s_isr_callback_t on_recv
Callback of data received event, only for RX channel The event data includes DMA buffer address and size that just finished receiving data 

on_recv_q_ovf

i2s_isr_callback_t on_recv_q_ovf
Callback of receiving queue overflowed event, only for RX channel The event data includes buffer size that has been overwritten 

on_sent

i2s_isr_callback_t on_sent
Callback of data sent event, only for TX channel The event data includes DMA buffer address and size that just finished sending data 

on_send_q_ovf

i2s_isr_callback_t on_send_q_ovf
Callback of sending queue overflowed event, only for TX channel The event data includes buffer size that has been overwritten 



```

#### i2s_chan_config_t


```


struct i2s_chan_config_t
I2S controller channel configuration. 

Public Members
id

i2s_port_t id
I2S port id 

role

i2s_role_t role
I2S role, I2S_ROLE_MASTER or I2S_ROLE_SLAVE 

dma_desc_num

uint32_t dma_desc_num
I2S DMA buffer number, it is also the number of DMA descriptor 

dma_frame_num

uint32_t dma_frame_num
I2S frame number in one DMA buffer. One frame means one-time sample data in all slots, it should be the multiple of 3 when the data bit width is 24. 

auto_clear

bool auto_clear
Alias of auto_clear_after_cb

auto_clear_after_cb

bool auto_clear_after_cb
Set to auto clear DMA TX buffer after on_sent callback, I2S will always send zero automatically if no data to send. So that user can assign the data to the DMA buffers directly in the callback, and the data won't be cleared after quit the callback. 

auto_clear_before_cb

bool auto_clear_before_cb
Set to auto clear DMA TX buffer before on_sent callback, I2S will always send zero automatically if no data to send So that user can access data in the callback that just finished to send. 

intr_priority

int intr_priority
I2S interrupt priority, range [0, 7], if set to 0, the driver will try to allocate an interrupt with a relative low priority (1,2,3) 



```

#### i2s_chan_info_t


```


struct i2s_chan_info_t
I2S channel information. 

Public Members
id

i2s_port_t id
I2S port id 

role

i2s_role_t role
I2S role, I2S_ROLE_MASTER or I2S_ROLE_SLAVE 

dir

i2s_dir_t dir
I2S channel direction 

mode

i2s_comm_mode_t mode
I2S channel communication mode 

pair_chan

i2s_chan_handle_t pair_chan
I2S pair channel handle in duplex mode, always NULL in simplex mode 

total_dma_buf_size

uint32_t total_dma_buf_size
Total size of all the allocated DMA buffers
0 if the channel has not been initialized
non-zero if the channel has been initialized 





```



### Macros


#### I2S_CHANNEL_DEFAULT_CONFIG


```


I2S_CHANNEL_DEFAULT_CONFIG(i2s_num, i2s_role)
get default I2S property 

```

#### I2S_GPIO_UNUSED


```


I2S_GPIO_UNUSED
Used in i2s_gpio_config_t for signals which are not used 

```



### I2S Types




### Header File


* components/esp_driver_i2s/include/driver/i2s_types.h
* This header file can be included with:



> ```
> #include "driver/i2s_types.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_i2s` component. To declare that your component depends on `esp_driver_i2s`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_i2s
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_i2s
> 
> ```




### Structures


#### i2s_event_data_t


```


struct i2s_event_data_t
Event structure used in I2S event queue. 

Public Members
data

void *data
(Deprecated) The secondary pointer of DMA buffer that just finished sending or receiving for on_recv and on_sent callback NULL for on_recv_q_ovf and on_send_q_ovf callback 

dma_buf

void *dma_buf
The first level pointer of DMA buffer that just finished sending or receiving for on_recv and on_sent callback NULL for on_recv_q_ovf and on_send_q_ovf callback 

size

size_t size
The buffer size of DMA buffer when success to send or receive, also the buffer size that dropped when queue overflow. It is related to the dma_frame_num and data_bit_width, typically it is fixed when data_bit_width is not changed. 



```



### Type Definitions


#### i2s_chan_handle_t


```


typedef struct i2s_channel_obj_t *i2s_chan_handle_t
I2S channel object handle, the control unit of the I2S driver 

```

#### i2s_isr_callback_t


```


typedef bool (*i2s_isr_callback_t)(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
I2S event callback. 

Param handle
[in] I2S channel handle, created from i2s_new_channel()

Param event
[in] I2S event data 

Param user_ctx
[in] User registered context, passed from i2s_channel_register_event_callback()

Return
Whether a high priority task has been waken up by this callback function 



```



### Enumerations


#### i2s_port_t


```


enum i2s_port_t
I2S controller port number, the max port number is (SOC_I2S_NUM -1). 
Values:
I2S_NUM_0

enumerator I2S_NUM_0
I2S controller port 0 

I2S_NUM_1

enumerator I2S_NUM_1
I2S controller port 1 

I2S_NUM_AUTO

enumerator I2S_NUM_AUTO
Select whichever port is available 


```

#### i2s_comm_mode_t


```


enum i2s_comm_mode_t
I2S controller communication mode. 
Values:
I2S_COMM_MODE_STD

enumerator I2S_COMM_MODE_STD
I2S controller using standard communication mode, support Philips/MSB/PCM format 

I2S_COMM_MODE_PDM

enumerator I2S_COMM_MODE_PDM
I2S controller using PDM communication mode, support PDM output or input 

I2S_COMM_MODE_TDM

enumerator I2S_COMM_MODE_TDM
I2S controller using TDM communication mode, support up to 16 slots per frame 

I2S_COMM_MODE_NONE

enumerator I2S_COMM_MODE_NONE
Unspecified I2S controller mode 


```

#### i2s_mclk_multiple_t


```


enum i2s_mclk_multiple_t
The multiple of MCLK to sample rate. 

Note
MCLK is the minimum resolution of the I2S clock. Increasing mclk multiple can reduce the clock jitter of BCLK and WS, which is also useful for the codec that don't require MCLK but have strict requirement to BCLK. For the 24-bit slot width, please choose a multiple that can be divided by 3 (i.e. 24-bit compatible). 

Values:
I2S_MCLK_MULTIPLE_128

enumerator I2S_MCLK_MULTIPLE_128
MCLK = sample_rate * 128 

I2S_MCLK_MULTIPLE_192

enumerator I2S_MCLK_MULTIPLE_192
MCLK = sample_rate * 192 (24-bit compatible) 

I2S_MCLK_MULTIPLE_256

enumerator I2S_MCLK_MULTIPLE_256
MCLK = sample_rate * 256 

I2S_MCLK_MULTIPLE_384

enumerator I2S_MCLK_MULTIPLE_384
MCLK = sample_rate * 384 (24-bit compatible) 

I2S_MCLK_MULTIPLE_512

enumerator I2S_MCLK_MULTIPLE_512
MCLK = sample_rate * 512 

I2S_MCLK_MULTIPLE_576

enumerator I2S_MCLK_MULTIPLE_576
MCLK = sample_rate * 576 (24-bit compatible) 

I2S_MCLK_MULTIPLE_768

enumerator I2S_MCLK_MULTIPLE_768
MCLK = sample_rate * 768 (24-bit compatible) 

I2S_MCLK_MULTIPLE_1024

enumerator I2S_MCLK_MULTIPLE_1024
MCLK = sample_rate * 1024 

I2S_MCLK_MULTIPLE_1152

enumerator I2S_MCLK_MULTIPLE_1152
MCLK = sample_rate * 1152 (24-bit compatible) 


```



### Header File


* components/hal/include/hal/i2s_types.h
* This header file can be included with:



> ```
> #include "hal/i2s_types.h"
> 
> ```




### Type Definitions


#### i2s_clock_src_t


```


typedef soc_periph_i2s_clk_src_t i2s_clock_src_t
I2S clock source 

```



### Enumerations


#### i2s_slot_mode_t


```


enum i2s_slot_mode_t
I2S channel slot mode. 
Values:
I2S_SLOT_MODE_MONO

enumerator I2S_SLOT_MODE_MONO
I2S channel slot format mono, transmit same data in all slots for tx mode, only receive the data in the first slots for rx mode. 

I2S_SLOT_MODE_STEREO

enumerator I2S_SLOT_MODE_STEREO
I2S channel slot format stereo, transmit different data in different slots for tx mode, receive the data in all slots for rx mode. 


```

#### i2s_dir_t


```


enum i2s_dir_t
I2S channel direction. 
Values:
I2S_DIR_RX

enumerator I2S_DIR_RX
I2S channel direction RX 

I2S_DIR_TX

enumerator I2S_DIR_TX
I2S channel direction TX 


```

#### i2s_role_t


```


enum i2s_role_t
I2S controller role. 
Values:
I2S_ROLE_MASTER

enumerator I2S_ROLE_MASTER
I2S controller master role, bclk and ws signal will be set to output 

I2S_ROLE_SLAVE

enumerator I2S_ROLE_SLAVE
I2S controller slave role, bclk and ws signal will be set to input 


```

#### i2s_data_bit_width_t


```


enum i2s_data_bit_width_t
Available data bit width in one slot. 
Values:
I2S_DATA_BIT_WIDTH_8BIT

enumerator I2S_DATA_BIT_WIDTH_8BIT
I2S channel data bit-width: 8 

I2S_DATA_BIT_WIDTH_16BIT

enumerator I2S_DATA_BIT_WIDTH_16BIT
I2S channel data bit-width: 16 

I2S_DATA_BIT_WIDTH_24BIT

enumerator I2S_DATA_BIT_WIDTH_24BIT
I2S channel data bit-width: 24 

I2S_DATA_BIT_WIDTH_32BIT

enumerator I2S_DATA_BIT_WIDTH_32BIT
I2S channel data bit-width: 32 


```

#### i2s_slot_bit_width_t


```


enum i2s_slot_bit_width_t
Total slot bit width in one slot. 
Values:
I2S_SLOT_BIT_WIDTH_AUTO

enumerator I2S_SLOT_BIT_WIDTH_AUTO
I2S channel slot bit-width equals to data bit-width 

I2S_SLOT_BIT_WIDTH_8BIT

enumerator I2S_SLOT_BIT_WIDTH_8BIT
I2S channel slot bit-width: 8 

I2S_SLOT_BIT_WIDTH_16BIT

enumerator I2S_SLOT_BIT_WIDTH_16BIT
I2S channel slot bit-width: 16 

I2S_SLOT_BIT_WIDTH_24BIT

enumerator I2S_SLOT_BIT_WIDTH_24BIT
I2S channel slot bit-width: 24 

I2S_SLOT_BIT_WIDTH_32BIT

enumerator I2S_SLOT_BIT_WIDTH_32BIT
I2S channel slot bit-width: 32 


```

#### i2s_pcm_compress_t


```


enum i2s_pcm_compress_t
A/U-law decompress or compress configuration. 
Values:
I2S_PCM_DISABLE

enumerator I2S_PCM_DISABLE
Disable A/U law decompress or compress 

I2S_PCM_A_DECOMPRESS

enumerator I2S_PCM_A_DECOMPRESS
A-law decompress 

I2S_PCM_A_COMPRESS

enumerator I2S_PCM_A_COMPRESS
A-law compress 

I2S_PCM_U_DECOMPRESS

enumerator I2S_PCM_U_DECOMPRESS
U-law decompress 

I2S_PCM_U_COMPRESS

enumerator I2S_PCM_U_COMPRESS
U-law compress 


```

#### i2s_pdm_dsr_t


```


enum i2s_pdm_dsr_t
I2S PDM RX down-sampling mode. 
Values:
I2S_PDM_DSR_8S

enumerator I2S_PDM_DSR_8S
downsampling number is 8 for PDM RX mode 

I2S_PDM_DSR_16S

enumerator I2S_PDM_DSR_16S
downsampling number is 16 for PDM RX mode 

I2S_PDM_DSR_MAX

enumerator I2S_PDM_DSR_MAX


```

#### i2s_pdm_sig_scale_t


```


enum i2s_pdm_sig_scale_t
pdm tx singnal scaling mode 
Values:
I2S_PDM_SIG_SCALING_DIV_2

enumerator I2S_PDM_SIG_SCALING_DIV_2
I2S TX PDM signal scaling: /2 

I2S_PDM_SIG_SCALING_MUL_1

enumerator I2S_PDM_SIG_SCALING_MUL_1
I2S TX PDM signal scaling: x1 

I2S_PDM_SIG_SCALING_MUL_2

enumerator I2S_PDM_SIG_SCALING_MUL_2
I2S TX PDM signal scaling: x2 

I2S_PDM_SIG_SCALING_MUL_4

enumerator I2S_PDM_SIG_SCALING_MUL_4
I2S TX PDM signal scaling: x4 


```

#### i2s_pdm_tx_line_mode_t


```


enum i2s_pdm_tx_line_mode_t
PDM TX line mode. 

Note
For the standard codec mode, PDM pins are connect to a codec which requires both clock signal and data signal For the DAC output mode, PDM data signal can be connected to a power amplifier directly with a low-pass filter, normally, DAC output mode doesn't need the clock signal. 

Values:
I2S_PDM_TX_ONE_LINE_CODEC

enumerator I2S_PDM_TX_ONE_LINE_CODEC
Standard PDM format output, left and right slot data on a single line 

I2S_PDM_TX_ONE_LINE_DAC

enumerator I2S_PDM_TX_ONE_LINE_DAC
PDM DAC format output, left or right slot data on a single line 

I2S_PDM_TX_TWO_LINE_DAC

enumerator I2S_PDM_TX_TWO_LINE_DAC
PDM DAC format output, left and right slot data on separated lines 


```

#### i2s_std_slot_mask_t


```


enum i2s_std_slot_mask_t
I2S slot select in standard mode. 

Note
It has different meanings in tx/rx/mono/stereo mode, and it may have differen behaviors on different targets For the details, please refer to the I2S API reference 

Values:
I2S_STD_SLOT_LEFT

enumerator I2S_STD_SLOT_LEFT
I2S transmits or receives left slot 

I2S_STD_SLOT_RIGHT

enumerator I2S_STD_SLOT_RIGHT
I2S transmits or receives right slot 

I2S_STD_SLOT_BOTH

enumerator I2S_STD_SLOT_BOTH
I2S transmits or receives both left and right slot 


```

#### i2s_pdm_slot_mask_t


```


enum i2s_pdm_slot_mask_t
I2S slot select in PDM mode. 
Values:
I2S_PDM_SLOT_RIGHT

enumerator I2S_PDM_SLOT_RIGHT
I2S PDM only transmits or receives the PDM device whose 'select' pin is pulled up 

I2S_PDM_SLOT_LEFT

enumerator I2S_PDM_SLOT_LEFT
I2S PDM only transmits or receives the PDM device whose 'select' pin is pulled down 

I2S_PDM_SLOT_BOTH

enumerator I2S_PDM_SLOT_BOTH
I2S PDM transmits or receives both two slots 

I2S_PDM_RX_LINE0_SLOT_RIGHT

enumerator I2S_PDM_RX_LINE0_SLOT_RIGHT
I2S PDM receives the right slot on line 0 

I2S_PDM_RX_LINE0_SLOT_LEFT

enumerator I2S_PDM_RX_LINE0_SLOT_LEFT
I2S PDM receives the left slot on line 0 

I2S_PDM_RX_LINE1_SLOT_RIGHT

enumerator I2S_PDM_RX_LINE1_SLOT_RIGHT
I2S PDM receives the right slot on line 1 

I2S_PDM_RX_LINE1_SLOT_LEFT

enumerator I2S_PDM_RX_LINE1_SLOT_LEFT
I2S PDM receives the left slot on line 1 

I2S_PDM_RX_LINE2_SLOT_RIGHT

enumerator I2S_PDM_RX_LINE2_SLOT_RIGHT
I2S PDM receives the right slot on line 2 

I2S_PDM_RX_LINE2_SLOT_LEFT

enumerator I2S_PDM_RX_LINE2_SLOT_LEFT
I2S PDM receives the left slot on line 2 

I2S_PDM_RX_LINE3_SLOT_RIGHT

enumerator I2S_PDM_RX_LINE3_SLOT_RIGHT
I2S PDM receives the right slot on line 3 

I2S_PDM_RX_LINE3_SLOT_LEFT

enumerator I2S_PDM_RX_LINE3_SLOT_LEFT
I2S PDM receives the left slot on line 3 

I2S_PDM_LINE_SLOT_ALL

enumerator I2S_PDM_LINE_SLOT_ALL
I2S PDM receives all slots 


```

#### i2s_tdm_slot_mask_t


```


enum i2s_tdm_slot_mask_t
tdm slot number 

Note
Multiple slots in TDM mode. For TX module, only the active slot send the audio data, the inactive slot send a constant or will be skipped if 'skip_msk' is set. For RX module, only receive the audio data in active slots, the data in inactive slots will be ignored. the bit map of active slot can not exceed (0x1<<total_slot_num). e.g: slot_mask = (I2S_TDM_SLOT0 | I2S_TDM_SLOT3), here the active slot number is 2 and total_slot is not supposed to be smaller than 4. 

Values:
I2S_TDM_SLOT0

enumerator I2S_TDM_SLOT0
I2S slot 0 enabled 

I2S_TDM_SLOT1

enumerator I2S_TDM_SLOT1
I2S slot 1 enabled 

I2S_TDM_SLOT2

enumerator I2S_TDM_SLOT2
I2S slot 2 enabled 

I2S_TDM_SLOT3

enumerator I2S_TDM_SLOT3
I2S slot 3 enabled 

I2S_TDM_SLOT4

enumerator I2S_TDM_SLOT4
I2S slot 4 enabled 

I2S_TDM_SLOT5

enumerator I2S_TDM_SLOT5
I2S slot 5 enabled 

I2S_TDM_SLOT6

enumerator I2S_TDM_SLOT6
I2S slot 6 enabled 

I2S_TDM_SLOT7

enumerator I2S_TDM_SLOT7
I2S slot 7 enabled 

I2S_TDM_SLOT8

enumerator I2S_TDM_SLOT8
I2S slot 8 enabled 

I2S_TDM_SLOT9

enumerator I2S_TDM_SLOT9
I2S slot 9 enabled 

I2S_TDM_SLOT10

enumerator I2S_TDM_SLOT10
I2S slot 10 enabled 

I2S_TDM_SLOT11

enumerator I2S_TDM_SLOT11
I2S slot 11 enabled 

I2S_TDM_SLOT12

enumerator I2S_TDM_SLOT12
I2S slot 12 enabled 

I2S_TDM_SLOT13

enumerator I2S_TDM_SLOT13
I2S slot 13 enabled 

I2S_TDM_SLOT14

enumerator I2S_TDM_SLOT14
I2S slot 14 enabled 

I2S_TDM_SLOT15

enumerator I2S_TDM_SLOT15
I2S slot 15 enabled 


```