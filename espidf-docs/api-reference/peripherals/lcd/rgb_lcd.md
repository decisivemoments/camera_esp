# RGB Interfaced LCD


RGB LCD panel is allocated in one step: `esp_lcd_new_rgb_panel()`, with various configurations specified by `esp_lcd_rgb_panel_config_t`.



> * `esp_lcd_rgb_panel_config_t::clk_src` selects the clock source for the RGB LCD controller. The available clock sources are listed in `lcd_clock_source_t`.
> * `esp_lcd_rgb_panel_config_t::data_width` set number of data lines used by the RGB interface. Currently, the supported value can be 8 or 16.
> * `esp_lcd_rgb_panel_config_t::bits_per_pixel` set the number of bits per pixel. This is different from `esp_lcd_rgb_panel_config_t::data_width`. By default, if you set this field to 0, the driver will automatically adjust the bpp to the `esp_lcd_rgb_panel_config_t::data_width`. But in some cases, these two value must be different. For example, a Serial RGB interface LCD only needs `8` data lines, but the color width can reach to `RGB888`, i.e., the `esp_lcd_rgb_panel_config_t::bits_per_pixel` should be set to `24`.
> * `esp_lcd_rgb_panel_config_t::hsync_gpio_num`, `esp_lcd_rgb_panel_config_t::vsync_gpio_num`, `esp_lcd_rgb_panel_config_t::de_gpio_num`, `esp_lcd_rgb_panel_config_t::pclk_gpio_num`, `esp_lcd_rgb_panel_config_t::disp_gpio_num` and `esp_lcd_rgb_panel_config_t::data_gpio_nums` are the GPIO pins used by the RGB LCD controller. If some of them are not used, please set it to -1.
> * `esp_lcd_rgb_panel_config_t::dma_burst_size` set the DMA transfer burst size, the value must be a power of 2.
> * `esp_lcd_rgb_panel_config_t::bounce_buffer_size_px` set the size of bounce buffer. This is only necessary for a so-called "bounce buffer" mode. Please refer to Bounce Buffer with Single PSRAM Frame Buffer for more information.
> * `esp_lcd_rgb_panel_config_t::timings` sets the LCD panel specific timing parameters. All required parameters are listed in the `esp_lcd_rgb_timing_t`, including the LCD resolution and blanking porches. Please fill them according to the datasheet of your LCD.
> * `esp_lcd_rgb_panel_config_t::fb_in_psram` sets whether to allocate the frame buffer from PSRAM or not. Please refer to Single Frame Buffer in PSRAM for more information.
> * `esp_lcd_rgb_panel_config_t::num_fbs` sets the number of frame buffers allocated by the driver. For backward compatibility, `0` means to allocate `one` frame buffer. Please use `esp_lcd_rgb_panel_config_t::no_fb` if you do not want to allocate any frame buffer.
> * `esp_lcd_rgb_panel_config_t::no_fb` if sets, no frame buffer will be allocated. This is also called the Bounce Buffer Only mode.



## RGB LCD Frame Buffer Operation Modes


Most of the time, the RGB LCD driver should maintain at least one screen sized frame buffer. According to the number and location of the frame buffer, the driver provides several different buffer modes.



### Single Frame Buffer in Internal Memory


This is the default and simplest and you do not have to specify flags or bounce buffer options. A frame buffer is allocated from the internal memory. The frame data is read out by DMA to the LCD verbatim. It needs no CPU intervention to function, but it has the downside that it uses up a fair bit of the limited amount of internal memory.



> ```
> esp_lcd_panel_handle_t panel_handle = NULL;
> esp_lcd_rgb_panel_config_t panel_config = {
>     .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
>     .clk_src = LCD_CLK_SRC_DEFAULT,
>     .disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN,
>     .pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK,
>     .vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC,
>     .hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC,
>     .de_gpio_num = EXAMPLE_PIN_NUM_DE,
>     .data_gpio_nums = {
>         EXAMPLE_PIN_NUM_DATA0,
>         EXAMPLE_PIN_NUM_DATA1,
>         EXAMPLE_PIN_NUM_DATA2,
>         // other GPIOs
>         // The number of GPIOs here should be the same to the value of `data_width` above
>         ...
>     },
>     // The timing parameters should refer to your LCD spec
>     .timings = {
>         .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
>         .h_res = EXAMPLE_LCD_H_RES,
>         .v_res = EXAMPLE_LCD_V_RES,
>         .hsync_back_porch = 40,
>         .hsync_front_porch = 20,
>         .hsync_pulse_width = 1,
>         .vsync_back_porch = 8,
>         .vsync_front_porch = 4,
>         .vsync_pulse_width = 1,
>     },
> };
> ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
> 
> ```




### Single Frame Buffer in PSRAM


If you have PSRAM and want to store the frame buffer there rather than in the limited internal memory, the LCD peripheral will use EDMA to fetch frame data directly from the PSRAM, bypassing the internal cache. You can enable this feature by setting the `esp_lcd_rgb_panel_config_t::fb_in_psram` to `true`. The downside of this is that when both the CPU as well as EDMA need access to the PSRAM, the bandwidth will be **shared** between them, that is, EDMA gets half and the CPUs get the other half. If there are other peripherals using EDMA as well, with a high enough pixel clock this can lead to starvation of the LCD peripheral, leading to display corruption. However, if the pixel clock is low enough for this not to be an issue, this is a solution that uses almost no CPU intervention.



> The PSRAM shares the same SPI bus with the main Flash (the one stores your firmware binary). At one time, there only be one consumer of the SPI bus. When you also use the main flash to serve your file system (e.g., SPIFFS), the bandwidth of the underlying SPI bus will also be shared, leading to display corruption. You can use `esp_lcd_rgb_panel_set_pclk()` to update the pixel clock frequency to a lower value.
> 
> 
> 
> ```
> esp_lcd_panel_handle_t panel_handle = NULL;
> esp_lcd_rgb_panel_config_t panel_config = {
>     .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
>     .clk_src = LCD_CLK_SRC_DEFAULT,
>     .disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN,
>     .pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK,
>     .vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC,
>     .hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC,
>     .de_gpio_num = EXAMPLE_PIN_NUM_DE,
>     .data_gpio_nums = {
>         EXAMPLE_PIN_NUM_DATA0,
>         EXAMPLE_PIN_NUM_DATA1,
>         EXAMPLE_PIN_NUM_DATA2,
>         // other GPIOs
>         // The number of GPIOs here should be the same to the value of `data_width` above
>         ...
>     },
>     // The timing parameters should refer to your LCD spec
>     .timings = {
>         .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
>         .h_res = EXAMPLE_LCD_H_RES,
>         .v_res = EXAMPLE_LCD_V_RES,
>         .hsync_back_porch = 40,
>         .hsync_front_porch = 20,
>         .hsync_pulse_width = 1,
>         .vsync_back_porch = 8,
>         .vsync_front_porch = 4,
>         .vsync_pulse_width = 1,
>     },
>     .flags.fb_in_psram = true, // allocate frame buffer from PSRAM
> };
> ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
> 
> ```




### Double Frame Buffer in PSRAM


To avoid tearing effect, using two screen sized frame buffers is the easiest approach. In this mode, the frame buffer can only be allocated from PSRAM, because of the limited internal memory. The frame buffer that the CPU write to and the frame buffer that the EDMA read from are guaranteed to be different and independent. The EDMA will only switch between the two frame buffers when the previous write operation is finished and the current frame has been sent to the LCD. The downside of this mode is that, you have to maintain the synchronization between the two frame buffers.



> ```
> esp_lcd_panel_handle_t panel_handle = NULL;
> esp_lcd_rgb_panel_config_t panel_config = {
>     .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
>     .num_fbs = 2,     // allocate double frame buffer
>     .clk_src = LCD_CLK_SRC_DEFAULT,
>     .disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN,
>     .pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK,
>     .vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC,
>     .hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC,
>     .de_gpio_num = EXAMPLE_PIN_NUM_DE,
>     .data_gpio_nums = {
>         EXAMPLE_PIN_NUM_DATA0,
>         EXAMPLE_PIN_NUM_DATA1,
>         EXAMPLE_PIN_NUM_DATA2,
>         // other GPIOs
>         // The number of GPIOs here should be the same to the value of `data_width` above
>         ...
>     },
>     // The timing parameters should refer to your LCD spec
>     .timings = {
>         .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
>         .h_res = EXAMPLE_LCD_H_RES,
>         .v_res = EXAMPLE_LCD_V_RES,
>         .hsync_back_porch = 40,
>         .hsync_front_porch = 20,
>         .hsync_pulse_width = 1,
>         .vsync_back_porch = 8,
>         .vsync_front_porch = 4,
>         .vsync_pulse_width = 1,
>     },
>     .flags.fb_in_psram = true, // allocate frame buffer from PSRAM
> };
> ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
> 
> ```




### Bounce Buffer with Single PSRAM Frame Buffer


This mode allocates two so-called `bounce buffers` from the internal memory, and a main frame buffer that is still in PSRAM. This mode is selected by setting the `esp_lcd_rgb_panel_config_t::fb_in_psram` flag and additionally specifying a non-zero `esp_lcd_rgb_panel_config_t::bounce_buffer_size_px` value. The bounce buffers only need to be large enough to hold a few lines of display data, which is significantly less than the main frame buffer. The LCD peripheral uses DMA to read data from one of the bounce buffers, and meanwhile an interrupt routine uses the CPU DCache to copy data from the main PSRAM frame buffer into the other bounce buffer. Once the LCD peripheral has finished reading the bounce buffer, the two buffers change place and the CPU can fill the others. The advantage of this mode is that, you can achieve higher pixel clock frequency. As the bounce buffers are larger than the FIFOs in the EDMA path, this method is also more robust against short bandwidth spikes. The downside is a major increase in CPU use and the LCD **CAN NOT** work if we disable the cache of the external memory, via e.g., OTA or NVS write to the main flash.



> Note
> 
> 
> It is highly recommended to turn on the "PSRAM XIP (Execute In Place)" feature in this mode by enabling the Kconfig options: CONFIG_SPIRAM_FETCH_INSTRUCTIONS and CONFIG_SPIRAM_RODATA, which allows the CPU to fetch instructions and readonly data from the PSRAM instead of the main flash. What is more, the external memory cache will not be disabled even if you attempt to write to the main flash through SPI1. This makes it possible to display an OTA progress bar for your application.
> 
> 
> 
> 
> Note
> 
> 
> This mode still has another problem which is also caused by insufficient PSRAM bandwidth. e.g., when your draw buffers are allocated from PSRAM, and their contents are copied into the internal frame buffer on CPU core 1. On CPU core 0, there is another memory copy happening in the DMA EOF ISR. In this situation, both CPUs are accessing the PSRAM by cache and sharing the bandwidth of the PSRAM. This increases the memory copy time that spent in the DMA EOF ISR significantly. The driver can not switch the bounce buffer in time, thus leading to a shift on the LCD screen. Although the driver can detect such a condition and perform a restart in the LCD's VSYNC interrupt handler, you still can see a flickering on the screen.
> 
> 
> 
> 
> ```
> esp_lcd_panel_handle_t panel_handle = NULL;
> esp_lcd_rgb_panel_config_t panel_config = {
>     .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
>     .clk_src = LCD_CLK_SRC_DEFAULT,
>     .bounce_buffer_size_px = 10 * EXAMPLE_LCD_H_RES, // allocate 10 lines data as bounce buffer from internal memory
>     .disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN,
>     .pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK,
>     .vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC,
>     .hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC,
>     .de_gpio_num = EXAMPLE_PIN_NUM_DE,
>     .data_gpio_nums = {
>         EXAMPLE_PIN_NUM_DATA0,
>         EXAMPLE_PIN_NUM_DATA1,
>         EXAMPLE_PIN_NUM_DATA2,
>         // other GPIOs
>         // The number of GPIOs here should be the same to the value of `data_width` above
>         ...
>     },
>     // The timing parameters should refer to your LCD spec
>     .timings = {
>         .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
>         .h_res = EXAMPLE_LCD_H_RES,
>         .v_res = EXAMPLE_LCD_V_RES,
>         .hsync_back_porch = 40,
>         .hsync_front_porch = 20,
>         .hsync_pulse_width = 1,
>         .vsync_back_porch = 8,
>         .vsync_front_porch = 4,
>         .vsync_pulse_width = 1,
>     },
>     .flags.fb_in_psram = true, // allocate frame buffer from PSRAM
> };
> ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
> 
> ```


Note that this mode also allows for a `esp_lcd_rgb_panel_config_t::bb_invalidate_cache` flag to be set. Enabling this frees up the cache lines after they are used to read out the frame buffer data from PSRAM, but it may lead to slight corruption if the other core writes data to the frame buffer at the exact time the cache lines are freed up. (Technically, a write to the frame buffer can be ignored if it falls between the cache writeback and the cache invalidate calls.)




### Bounce Buffer Only


This mode is similar to the Bounce Buffer with Single PSRAM Frame Buffer, but there is no PSRAM frame buffer initialized by the LCD driver. Instead, the user supplies a callback function that is responsible for filling the bounce buffers. As this driver does not care where the written pixels come from, this allows for the callback doing e.g., on-the-fly conversion from a smaller, 8-bit-per-pixel PSRAM frame buffer to an 16-bit LCD, or even procedurally-generated frame-buffer-less graphics. This option is selected by setting the `esp_lcd_rgb_panel_config_t::no_fb` flag and supplying a `esp_lcd_rgb_panel_config_t::bounce_buffer_size_px` value. And then register the `esp_lcd_rgb_panel_event_callbacks_t::on_bounce_empty` callback by calling `esp_lcd_rgb_panel_register_event_callbacks()`.



> Note
> 
> 
> It should never happen in a well-designed embedded application, but it can in theory be possible that the DMA cannot deliver data as fast as the LCD consumes it. In the ESP32-S3 hardware, this leads to the LCD simply outputting dummy bytes while DMA waits for data. If we were to run DMA in a stream fashion, this would mean a de-sync between the LCD address the DMA reads the data for and the LCD address the LCD peripheral thinks it outputs data for, leading to a **permanently** shifted image.
> In order to stop this from happening, you can either enable the CONFIG_LCD_RGB_RESTART_IN_VSYNC option, so the driver can restart the DMA in the VBlank interrupt automatically or call `esp_lcd_rgb_panel_restart()` to restart the DMA manually. Note `esp_lcd_rgb_panel_restart()` does not restart the DMA immediately, the DMA is still restarted in the next VSYNC event.





## API Reference



### Header File


* components/esp_lcd/rgb/include/esp_lcd_panel_rgb.h
* This header file can be included with:



> ```
> #include "esp_lcd_panel_rgb.h"
> 
> ```
* This header file is a part of the API provided by the `esp_lcd` component. To declare that your component depends on `esp_lcd`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_lcd
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_lcd
> 
> ```




### Functions


#### esp_lcd_new_rgb_panel


```


esp_err_t esp_lcd_new_rgb_panel(const esp_lcd_rgb_panel_config_t *rgb_panel_config, esp_lcd_panel_handle_t *ret_panel)
Create RGB LCD panel. 

Parameters

rgb_panel_config -- [in] RGB panel configuration 
ret_panel -- [out] Returned LCD panel handle 


Returns

ESP_ERR_INVALID_ARG: Create RGB LCD panel failed because of invalid argument
ESP_ERR_NO_MEM: Create RGB LCD panel failed because of out of memory
ESP_ERR_NOT_FOUND: Create RGB LCD panel failed because some mandatory hardware resources are not found
ESP_OK: Create RGB LCD panel successfully 





```

#### esp_lcd_rgb_panel_register_event_callbacks


```


esp_err_t esp_lcd_rgb_panel_register_event_callbacks(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_callbacks_t *callbacks, void *user_ctx)
Register LCD RGB panel event callbacks. 

Parameters

panel -- [in] LCD panel handle, returned from esp_lcd_new_rgb_panel
callbacks -- [in] Group of callback functions 
user_ctx -- [in] User data, which will be passed to the callback functions directly 


Returns

ESP_OK: Set event callbacks successfully
ESP_ERR_INVALID_ARG: Set event callbacks failed because of invalid argument
ESP_FAIL: Set event callbacks failed because of other error 





```

#### esp_lcd_rgb_panel_set_pclk


```


esp_err_t esp_lcd_rgb_panel_set_pclk(esp_lcd_panel_handle_t panel, uint32_t freq_hz)
Set frequency of PCLK for RGB LCD panel. 

Note
The PCLK frequency is set in the esp_lcd_rgb_timing_t and gets configured during LCD panel initialization. Usually you don't need to call this function to set the PCLK again, but in some cases, you might want to change the PCLK frequency. e.g. slow down the PCLK frequency to reduce power consumption or to reduce the memory throughput during OTA. 


Note
This function doesn't cause the hardware to update the PCLK immediately but to record the new frequency and set a flag internally. Only in the next VSYNC event handler, will the driver attempt to update the PCLK frequency.


Parameters

panel -- [in] LCD panel handle, returned from esp_lcd_new_rgb_panel
freq_hz -- [in] Frequency of pixel clock, in Hz 


Returns

ESP_ERR_INVALID_ARG: Set PCLK frequency failed because of invalid argument
ESP_OK: Set PCLK frequency successfully 





```

#### esp_lcd_rgb_panel_restart


```


esp_err_t esp_lcd_rgb_panel_restart(esp_lcd_panel_handle_t panel)
Restart the LCD transmission. 

Note
This function can be useful when the LCD controller is out of sync with the DMA because of insufficient bandwidth. To save the screen from a permanent shift, you can call this function to restart the LCD DMA. 


Note
This function doesn't restart the DMA immediately but to set a flag internally. Only in the next VSYNC event handler, will the driver attempt to do the restart job. 


Note
If CONFIG_LCD_RGB_RESTART_IN_VSYNC is enabled, you don't need to call this function manually, because the restart job will be done automatically in the VSYNC event handler.


Parameters
panel -- [in] panel LCD panel handle, returned from esp_lcd_new_rgb_panel

Returns

ESP_ERR_INVALID_ARG: Restart the LCD failed because of invalid argument
ESP_ERR_INVALID_STATE: Restart the LCD failed because the LCD diver is working in refresh-on-demand mode
ESP_OK: Restart the LCD successfully 





```

#### esp_lcd_rgb_panel_get_frame_buffer


```


esp_err_t esp_lcd_rgb_panel_get_frame_buffer(esp_lcd_panel_handle_t panel, uint32_t fb_num, void **fb0, ...)
Get the address of the frame buffer(s) that allocated by the driver. 

Parameters

panel -- [in] LCD panel handle, returned from esp_lcd_new_rgb_panel
fb_num -- [in] Number of frame buffer(s) to get. This value must be the same as the number of the following parameters. 
fb0 -- [out] Returned address of the frame buffer 0 
... -- [out] List of other frame buffer addresses 


Returns

ESP_ERR_INVALID_ARG: Get frame buffer address failed because of invalid argument
ESP_OK: Get frame buffer address successfully 





```

#### esp_lcd_rgb_panel_refresh


```


esp_err_t esp_lcd_rgb_panel_refresh(esp_lcd_panel_handle_t panel)
Manually trigger once transmission of the frame buffer to the LCD panel. 

Note
This function should only be called when the RGB panel is working under the refresh_on_demand mode.


Parameters
panel -- [in] LCD panel handle, returned from esp_lcd_new_rgb_panel

Returns

ESP_ERR_INVALID_ARG: Start a refresh failed because of invalid argument
ESP_ERR_INVALID_STATE: Start a refresh failed because the LCD panel is not created with the refresh_on_demand flag enabled.
ESP_OK: Start a refresh successfully 





```

#### esp_lcd_rgb_panel_set_yuv_conversion


```


esp_err_t esp_lcd_rgb_panel_set_yuv_conversion(esp_lcd_panel_handle_t panel, const esp_lcd_yuv_conv_config_t *config)
Configure how to convert the color format between RGB and YUV. 

Note
Pass in config as NULL will disable the RGB-YUV converter. 


Note
The hardware converter can only parse a "packed" storage format, while "planar" and "semi-planar" format is not supported.


Parameters

panel -- [in] LCD panel handle, returned from esp_lcd_new_rgb_panel
config -- [in] Configuration of RGB-YUV conversion 


Returns

ESP_ERR_INVALID_ARG: Configure RGB-YUV conversion failed because of invalid argument
ESP_ERR_NOT_SUPPORTED: Configure RGB-YUV conversion failed because the conversion mode is not supported by the hardware
ESP_OK: Configure RGB-YUV conversion successfully 





```



### Structures


#### esp_lcd_rgb_timing_t


```


struct esp_lcd_rgb_timing_t
LCD RGB timing structure. 
*                                                 Total Width
*                             <--------------------------------------------------->
*                       HSYNC width HBP             Active Width                HFP
*                             <---><--><--------------------------------------><--->
*                         ____    ____|_______________________________________|____|
*                             |___|   |                                       |    |
*                                     |                                       |    |
*                         __|         |                                       |    |
*            /|\    /|\  |            |                                       |    |
*             | VSYNC|   |            |                                       |    |
*             |Width\|/  |__          |                                       |    |
*             |     /|\     |         |                                       |    |
*             |  VBP |      |         |                                       |    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Height   |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Height|      |         |/ / / / / / Active Display Area / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |                                                      |
*             |  VFP |      |                                                      |
*            \|/    \|/_____|______________________________________________________|
*



Public Members
pclk_hz

uint32_t pclk_hz
Frequency of pixel clock 

h_res

uint32_t h_res
Horizontal resolution, i.e. the number of pixels in a line 

v_res

uint32_t v_res
Vertical resolution, i.e. the number of lines in the frame 

hsync_pulse_width

uint32_t hsync_pulse_width
Horizontal sync width, unit: PCLK period 

hsync_back_porch

uint32_t hsync_back_porch
Horizontal back porch, number of PCLK between hsync and start of line active data 

hsync_front_porch

uint32_t hsync_front_porch
Horizontal front porch, number of PCLK between the end of active data and the next hsync 

vsync_pulse_width

uint32_t vsync_pulse_width
Vertical sync width, unit: number of lines 

vsync_back_porch

uint32_t vsync_back_porch
Vertical back porch, number of invalid lines between vsync and start of frame 

vsync_front_porch

uint32_t vsync_front_porch
Vertical front porch, number of invalid lines between the end of frame and the next vsync 

hsync_idle_low

uint32_t hsync_idle_low
The hsync signal is low in IDLE state 

vsync_idle_low

uint32_t vsync_idle_low
The vsync signal is low in IDLE state 

de_idle_high

uint32_t de_idle_high
The de signal is high in IDLE state 

pclk_active_neg

uint32_t pclk_active_neg
Whether the display data is clocked out on the falling edge of PCLK 

pclk_idle_high

uint32_t pclk_idle_high
The PCLK stays at high level in IDLE phase 

flags

struct esp_lcd_rgb_timing_t::[anonymous] flags
LCD RGB timing flags 



```

#### esp_lcd_rgb_panel_event_data_t


```


struct esp_lcd_rgb_panel_event_data_t
Type of RGB LCD panel event data. 

```

#### esp_lcd_rgb_panel_event_callbacks_t


```


struct esp_lcd_rgb_panel_event_callbacks_t
Group of supported RGB LCD panel callbacks. 

Note
The callbacks are all running under ISR environment 


Note
When CONFIG_LCD_RGB_ISR_IRAM_SAFE is enabled, the callback itself and functions called by it should be placed in IRAM. 


Public Members
on_vsync

esp_lcd_rgb_panel_vsync_cb_t on_vsync
VSYNC event callback 

on_bounce_empty

esp_lcd_rgb_panel_bounce_buf_fill_cb_t on_bounce_empty
Bounce buffer empty callback. 

on_bounce_frame_finish

esp_lcd_rgb_panel_bounce_buf_finish_cb_t on_bounce_frame_finish
Bounce buffer finish callback. 



```

#### esp_lcd_rgb_panel_config_t


```


struct esp_lcd_rgb_panel_config_t
LCD RGB panel configuration structure. 

Public Members
clk_src

lcd_clock_source_t clk_src
Clock source for the RGB LCD peripheral 

timings

esp_lcd_rgb_timing_t timings
RGB timing parameters, including the screen resolution 

data_width

size_t data_width
Number of data lines 

bits_per_pixel

size_t bits_per_pixel
Frame buffer color depth, in bpp, specially, if set to zero, it will default to data_width. When using a Serial RGB interface, this value could be different from data_width

num_fbs

size_t num_fbs
Number of screen-sized frame buffers that allocated by the driver. By default (set to either 0 or 1) only one frame buffer will be used. Maximum number of buffers are 3 

bounce_buffer_size_px

size_t bounce_buffer_size_px
If it's non-zero, the driver allocates two DRAM bounce buffers for DMA use. DMA fetching from DRAM bounce buffer is much faster than PSRAM frame buffer. 

sram_trans_align

size_t sram_trans_align
Alignment of buffers (frame buffer or bounce buffer) that allocated in SRAM 

psram_trans_align

size_t psram_trans_align
Alignment of buffers (frame buffer) that allocated in PSRAM 

dma_burst_size

size_t dma_burst_size
DMA burst size, in bytes 

hsync_gpio_num

int hsync_gpio_num
GPIO used for HSYNC signal 

vsync_gpio_num

int vsync_gpio_num
GPIO used for VSYNC signal 

de_gpio_num

int de_gpio_num
GPIO used for DE signal, set to -1 if it's not used 

pclk_gpio_num

int pclk_gpio_num
GPIO used for PCLK signal, set to -1 if it's not used 

disp_gpio_num

int disp_gpio_num
GPIO used for display control signal, set to -1 if it's not used 

data_gpio_nums

int data_gpio_nums[(16)]
GPIOs used for data lines 

disp_active_low

uint32_t disp_active_low
If this flag is enabled, a low level of display control signal can turn the screen on; vice versa 

refresh_on_demand

uint32_t refresh_on_demand
If this flag is enabled, the host only refresh the frame buffer when esp_lcd_panel_draw_bitmap is called. This is useful when the LCD screen has a GRAM and can refresh the LCD by itself. 

fb_in_psram

uint32_t fb_in_psram
If this flag is enabled, the frame buffer will be allocated from PSRAM, preferentially 

double_fb

uint32_t double_fb
If this flag is enabled, the driver will allocate two screen sized frame buffer, same as num_fbs=2 

no_fb

uint32_t no_fb
If this flag is enabled, the driver won't allocate frame buffer. Instead, user should fill in the bounce buffer manually in the on_bounce_empty callback 

bb_invalidate_cache

uint32_t bb_invalidate_cache
If this flag is enabled, in bounce back mode we'll do a cache invalidate on the read data, freeing the cache. Can be dangerous if data is written from other core(s). 

flags

struct esp_lcd_rgb_panel_config_t::[anonymous] flags
LCD RGB panel configuration flags 



```

#### esp_lcd_color_conv_profile_t


```


struct esp_lcd_color_conv_profile_t
LCD color conversion profile. 

Public Members
color_space

lcd_color_space_t color_space
Color space of the image 

color_range

lcd_color_range_t color_range
Color range of the image 

yuv_sample

lcd_yuv_sample_t yuv_sample
YUV sample format of the image 



```

#### esp_lcd_yuv_conv_config_t


```


struct esp_lcd_yuv_conv_config_t
Configuration of YUG-RGB conversion. 

Public Members
std

lcd_yuv_conv_std_t std
YUV conversion standard: BT601, BT709 

src

esp_lcd_color_conv_profile_t src
Color conversion profile of the input image 

dst

esp_lcd_color_conv_profile_t dst
Color conversion profile of the output image 



```



### Type Definitions


#### esp_lcd_rgb_panel_vsync_cb_t


```


typedef bool (*esp_lcd_rgb_panel_vsync_cb_t)(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)
RGB LCD VSYNC event callback prototype. 

Param panel
[in] LCD panel handle, returned from esp_lcd_new_rgb_panel

Param edata
[in] Panel event data, fed by driver 

Param user_ctx
[in] User data, passed from esp_lcd_rgb_panel_register_event_callbacks()

Return
Whether a high priority task has been waken up by this function 



```

#### esp_lcd_rgb_panel_bounce_buf_fill_cb_t


```


typedef bool (*esp_lcd_rgb_panel_bounce_buf_fill_cb_t)(esp_lcd_panel_handle_t panel, void *bounce_buf, int pos_px, int len_bytes, void *user_ctx)
Prototype for function to re-fill a bounce buffer, rather than copying from the frame buffer. 

Param panel
[in] LCD panel handle, returned from esp_lcd_new_rgb_panel

Param bounce_buf
[in] Bounce buffer to write data into 

Param pos_px
[in] How many pixels already were sent to the display in this frame, in other words, at what pixel the routine should start putting data into bounce_buf 

Param len_bytes
[in] Length, in bytes, of the bounce buffer. Routine should fill this length fully. 

Param user_ctx
[in] Opaque pointer that was passed from esp_lcd_rgb_panel_register_event_callbacks()

Return
Whether a high priority task has been waken up by this function 



```

#### esp_lcd_rgb_panel_bounce_buf_finish_cb_t


```


typedef bool (*esp_lcd_rgb_panel_bounce_buf_finish_cb_t)(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)
Prototype for the function to be called when the bounce buffer finish copying the entire frame. 

Param panel
[in] LCD panel handle, returned from esp_lcd_new_rgb_panel

Param edata
[in] Panel event data, fed by driver 

Param user_ctx
[in] User data, passed from esp_lcd_rgb_panel_register_event_callbacks()

Return
Whether a high priority task has been waken up by this function 



```