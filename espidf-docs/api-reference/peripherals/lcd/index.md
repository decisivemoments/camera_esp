# LCD



## Introduction


ESP chips can generate various kinds of timings needed by common LCDs on the market, like SPI LCD, I2C LCD, Parallel LCD (Intel 8080), RGB/SRGB LCD, MIPI DSI LCD and etc. The `esp_lcd` component offers an abstracted driver framework to support them in a unified way.


An LCD typically consists of two main planes:


* **Control Plane**: This plane allows us to read and write to the internal registers of the LCD device controller. Host typically uses this plane for tasks such as initializing the LCD power supply and performing gamma calibration.
* **Data Plane**: The data plane is responsible for transmitting pixel data to the LCD device.




## Functional Overview


In the context of `esp_lcd`, both the data plane and the control plane are represented by the `esp_lcd_panel_handle_t` type.


On some LCDs, these two planes may be combined into a single plane. In this configuration, pixel data is transmitted through the control plane, achieving functionality similar to that of the data plane. This merging is common in SPI LCDs and I2C LCDs.
Additionally, there are LCDs that do not require a separate control plane. For instance, certain RGB LCDs automatically execute necessary initialization procedures after power-up. Host devices only need to continuously refresh pixel data through the data plane. However, it's essential to note that not all RGB LCDs eliminate the control plane entirely. Some LCD devices can simultaneously support multiple interfaces, requiring the host to send specific commands via the control plane (such as those based on the SPI interface) to enable the RGB mode.


This document will discuss how to create the control plane and data plane, as mentioned earlier, based on different types of LCDs.



* SPI Interfaced LCD
* API Reference
* I2C Interfaced LCD
* API Reference
* I80 Interfaced LCD
* API Reference
* RGB Interfaced LCD




Note


ESP-IDF provides only a limited number of LCD device controller drivers out of the box (e.g., ST7789), more drivers are available in the Espressif Component Registry.





## LCD Control Panel Operations


* `esp_lcd_panel_reset()` can reset the LCD control panel.
* `esp_lcd_panel_init()` performs a basic initialization of the control panel. To perform more manufacture specific initialization, please refer to Steps to Add Manufacture Specific Initialization.
* By combining using `esp_lcd_panel_swap_xy()` and `esp_lcd_panel_mirror()`, you can achieve the functionality of rotating or mirroring the LCD screen.
* `esp_lcd_panel_disp_on_off()` can turn on or off the LCD screen by cutting down the output path from the frame buffer to the LCD screen. Please note, this is not controlling the LCD backlight. Backlight control is not covered by the `esp_lcd` driver.
* `esp_lcd_panel_disp_sleep()` can reduce the power consumption of the LCD screen by entering the sleep mode. The internal frame buffer is still retained.




## LCD Data Panel Operations


* `esp_lcd_panel_reset()` can reset the LCD data panel.
* `esp_lcd_panel_init()` performs a basic initialization of the data panel.
* `esp_lcd_panel_draw_bitmap()` is the function which does the magic to flush the user draw buffer to the LCD screen, where the target draw window is configurable. Please note, this function expects the draw buffer is a 1-D array and there's no stride in between each lines.




## Steps to Add Manufacture Specific Initialization


The LCD controller drivers (e.g., st7789) in ESP-IDF only provide basic initialization in the `esp_lcd_panel_init()`, leaving the vast majority of settings to the default values. Some LCD modules needs to set a bunch of manufacture specific configurations before it can display normally. These configurations usually include gamma, power voltage and so on. If you want to add manufacture specific initialization, please follow the steps below:



```
esp_lcd_panel_reset(panel_handle);
esp_lcd_panel_init(panel_handle);
// set extra configurations e.g., gamma control
// with the underlying IO handle
// please consult your manufacture for special commands and corresponding values
esp_lcd_panel_io_tx_param(io_handle, GAMMA_CMD, (uint8_t[]) {
       GAMMA_ARRAY
    }, N);
// turn on the display
esp_lcd_panel_disp_on_off(panel_handle, true);

```




## Application Example


* Software JPEG decoding and display - peripherals/lcd/tjpgd
* Universal SPI LCD example with SPI touch - peripherals/lcd/spi_lcd_touch
* i80 controller based LCD and LVGL animation UI - peripherals/lcd/i80_controller
* RGB panel example with scatter chart UI - peripherals/lcd/rgb_panel
* I2C interfaced OLED display scrolling text - peripherals/lcd/i2c_oled







## API Reference



### Header File


* components/hal/include/hal/lcd_types.h
* This header file can be included with:



> ```
> #include "hal/lcd_types.h"
> 
> ```




### Type Definitions


#### lcd_clock_source_t


```


typedef soc_periph_lcd_clk_src_t lcd_clock_source_t
LCD clock source. 

```



### Enumerations


#### lcd_rgb_data_endian_t


```


enum lcd_rgb_data_endian_t
RGB data endian. 
Values:
LCD_RGB_DATA_ENDIAN_BIG

enumerator LCD_RGB_DATA_ENDIAN_BIG
RGB data endian: MSB first 

LCD_RGB_DATA_ENDIAN_LITTLE

enumerator LCD_RGB_DATA_ENDIAN_LITTLE
RGB data endian: LSB first 


```

#### lcd_color_space_t


```


enum lcd_color_space_t
LCD color space. 
Values:
LCD_COLOR_SPACE_RGB

enumerator LCD_COLOR_SPACE_RGB
Color space: RGB 

LCD_COLOR_SPACE_YUV

enumerator LCD_COLOR_SPACE_YUV
Color space: YUV 


```

#### lcd_color_rgb_pixel_format_t


```


enum lcd_color_rgb_pixel_format_t
LCD color pixel format in RGB color space. 
Values:
LCD_COLOR_PIXEL_FORMAT_RGB565

enumerator LCD_COLOR_PIXEL_FORMAT_RGB565
16 bits, 5 bits per R/B value, 6 bits for G value 

LCD_COLOR_PIXEL_FORMAT_RGB666

enumerator LCD_COLOR_PIXEL_FORMAT_RGB666
18 bits, 6 bits per R/G/B value 

LCD_COLOR_PIXEL_FORMAT_RGB888

enumerator LCD_COLOR_PIXEL_FORMAT_RGB888
24 bits, 8 bits per R/G/B value 


```

#### lcd_color_range_t


```


enum lcd_color_range_t
LCD color range. 
Values:
LCD_COLOR_RANGE_LIMIT

enumerator LCD_COLOR_RANGE_LIMIT
Limited color range 

LCD_COLOR_RANGE_FULL

enumerator LCD_COLOR_RANGE_FULL
Full color range 


```

#### lcd_yuv_sample_t


```


enum lcd_yuv_sample_t
YUV sampling method. 
Values:
LCD_YUV_SAMPLE_422

enumerator LCD_YUV_SAMPLE_422
YUV 4:2:2 sampling 

LCD_YUV_SAMPLE_420

enumerator LCD_YUV_SAMPLE_420
YUV 4:2:0 sampling 

LCD_YUV_SAMPLE_411

enumerator LCD_YUV_SAMPLE_411
YUV 4:1:1 sampling 


```

#### lcd_yuv_conv_std_t


```


enum lcd_yuv_conv_std_t
The standard used for conversion between RGB and YUV. 
Values:
LCD_YUV_CONV_STD_BT601

enumerator LCD_YUV_CONV_STD_BT601
YUV<->RGB conversion standard: BT.601 

LCD_YUV_CONV_STD_BT709

enumerator LCD_YUV_CONV_STD_BT709
YUV<->RGB conversion standard: BT.709 


```



### Header File


* components/esp_lcd/include/esp_lcd_types.h
* This header file can be included with:



> ```
> #include "esp_lcd_types.h"
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




### Structures


#### esp_lcd_video_timing_t


```


struct esp_lcd_video_timing_t
Timing parameters for the video data transmission. 

Public Members
h_size

uint32_t h_size
Horizontal resolution, i.e. the number of pixels in a line 

v_size

uint32_t v_size
Vertical resolution, i.e. the number of lines in the frame 

hsync_pulse_width

uint32_t hsync_pulse_width
Horizontal sync width, in pixel clock 

hsync_back_porch

uint32_t hsync_back_porch
Horizontal back porch, number of pixel clock between hsync and start of line active data 

hsync_front_porch

uint32_t hsync_front_porch
Horizontal front porch, number of pixel clock between the end of active data and the next hsync 

vsync_pulse_width

uint32_t vsync_pulse_width
Vertical sync width, in number of lines 

vsync_back_porch

uint32_t vsync_back_porch
Vertical back porch, number of invalid lines between vsync and start of frame 

vsync_front_porch

uint32_t vsync_front_porch
Vertical front porch, number of invalid lines between the end of frame and the next vsync 



```

#### esp_lcd_panel_io_event_data_t


```


struct esp_lcd_panel_io_event_data_t
Type of LCD panel IO event data. 

```

#### esp_lcd_panel_io_callbacks_t


```


struct esp_lcd_panel_io_callbacks_t
Type of LCD panel IO callbacks. 

Public Members
on_color_trans_done

esp_lcd_panel_io_color_trans_done_cb_t on_color_trans_done
Callback invoked when color data transfer has finished 



```



### Type Definitions


#### esp_lcd_panel_io_handle_t


```


typedef struct esp_lcd_panel_io_t *esp_lcd_panel_io_handle_t
Type of LCD panel IO handle 

```

#### esp_lcd_panel_handle_t


```


typedef struct esp_lcd_panel_t *esp_lcd_panel_handle_t
Type of LCD panel handle 

```

#### esp_lcd_panel_io_color_trans_done_cb_t


```


typedef bool (*esp_lcd_panel_io_color_trans_done_cb_t)(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
Declare the prototype of the function that will be invoked when panel IO finishes transferring color data. 

Param panel_io
[in] LCD panel IO handle, which is created by factory API like esp_lcd_new_panel_io_spi()

Param edata
[in] Panel IO event data, fed by driver 

Param user_ctx
[in] User data, passed from esp_lcd_panel_io_xxx_config_t

Return
Whether a high priority task has been waken up by this function 



```



### Enumerations


#### lcd_rgb_element_order_t


```


enum lcd_rgb_element_order_t
RGB element order. 
Values:
LCD_RGB_ELEMENT_ORDER_RGB

enumerator LCD_RGB_ELEMENT_ORDER_RGB
RGB element order: RGB 

LCD_RGB_ELEMENT_ORDER_BGR

enumerator LCD_RGB_ELEMENT_ORDER_BGR
RGB element order: BGR 


```



### Header File


* components/esp_lcd/include/esp_lcd_panel_io.h
* This header file can be included with:



> ```
> #include "esp_lcd_panel_io.h"
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


#### esp_lcd_panel_io_rx_param


```


esp_err_t esp_lcd_panel_io_rx_param(esp_lcd_panel_io_handle_t io, int lcd_cmd, void *param, size_t param_size)
Transmit LCD command and receive corresponding parameters. 

Note
Commands sent by this function are short, so they are sent using polling transactions. The function does not return before the command transfer is completed. If any queued transactions sent by esp_lcd_panel_io_tx_color() are still pending when this function is called, this function will wait until they are finished and the queue is empty before sending the command(s).


Parameters

io -- [in] LCD panel IO handle, which is created by other factory API like esp_lcd_new_panel_io_spi()
lcd_cmd -- [in] The specific LCD command, set to -1 if no command needed 
param -- [out] Buffer for the command data 
param_size -- [in] Size of param buffer 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_NOT_SUPPORTED if read is not supported by transport
ESP_OK on success 





```

#### esp_lcd_panel_io_tx_param


```


esp_err_t esp_lcd_panel_io_tx_param(esp_lcd_panel_io_handle_t io, int lcd_cmd, const void *param, size_t param_size)
Transmit LCD command and corresponding parameters. 

Note
Commands sent by this function are short, so they are sent using polling transactions. The function does not return before the command transfer is completed. If any queued transactions sent by esp_lcd_panel_io_tx_color() are still pending when this function is called, this function will wait until they are finished and the queue is empty before sending the command(s).


Parameters

io -- [in] LCD panel IO handle, which is created by other factory API like esp_lcd_new_panel_io_spi()
lcd_cmd -- [in] The specific LCD command, set to -1 if no command needed 
param -- [in] Buffer that holds the command specific parameters, set to NULL if no parameter is needed for the command 
param_size -- [in] Size of param in memory, in bytes, set to zero if no parameter is needed for the command 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_OK on success 





```

#### esp_lcd_panel_io_tx_color


```


esp_err_t esp_lcd_panel_io_tx_color(esp_lcd_panel_io_handle_t io, int lcd_cmd, const void *color, size_t color_size)
Transmit LCD RGB data. 

Note
This function will package the command and RGB data into a transaction, and push into a queue. The real transmission is performed in the background (DMA+interrupt). The caller should take care of the lifecycle of the color buffer. Recycling of color buffer should be done in the callback on_color_trans_done().


Parameters

io -- [in] LCD panel IO handle, which is created by factory API like esp_lcd_new_panel_io_spi()
lcd_cmd -- [in] The specific LCD command, set to -1 if no command needed 
color -- [in] Buffer that holds the RGB color data 
color_size -- [in] Size of color in memory, in bytes 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_OK on success 





```

#### esp_lcd_panel_io_del


```


esp_err_t esp_lcd_panel_io_del(esp_lcd_panel_io_handle_t io)
Destroy LCD panel IO handle (deinitialize panel and free all corresponding resource) 

Parameters
io -- [in] LCD panel IO handle, which is created by factory API like esp_lcd_new_panel_io_spi()

Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_OK on success 





```

#### esp_lcd_panel_io_register_event_callbacks


```


esp_err_t esp_lcd_panel_io_register_event_callbacks(esp_lcd_panel_io_handle_t io, const esp_lcd_panel_io_callbacks_t *cbs, void *user_ctx)
Register LCD panel IO callbacks. 

Parameters

io -- [in] LCD panel IO handle, which is created by factory API like esp_lcd_new_panel_io_spi()
cbs -- [in] structure with all LCD panel IO callbacks 
user_ctx -- [in] User private data, passed directly to callback's user_ctx 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_OK on success 





```



### Header File


* components/esp_lcd/include/esp_lcd_panel_ops.h
* This header file can be included with:



> ```
> #include "esp_lcd_panel_ops.h"
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


#### esp_lcd_panel_reset


```


esp_err_t esp_lcd_panel_reset(esp_lcd_panel_handle_t panel)
Reset LCD panel. 

Note
Panel reset must be called before attempting to initialize the panel using esp_lcd_panel_init().


Parameters
panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()

Returns

ESP_OK on success 





```

#### esp_lcd_panel_init


```


esp_err_t esp_lcd_panel_init(esp_lcd_panel_handle_t panel)
Initialize LCD panel. 

Note
Before calling this function, make sure the LCD panel has finished the reset stage by esp_lcd_panel_reset().


Parameters
panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()

Returns

ESP_OK on success 





```

#### esp_lcd_panel_del


```


esp_err_t esp_lcd_panel_del(esp_lcd_panel_handle_t panel)
Deinitialize the LCD panel. 

Parameters
panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()

Returns

ESP_OK on success 





```

#### esp_lcd_panel_draw_bitmap


```


esp_err_t esp_lcd_panel_draw_bitmap(esp_lcd_panel_handle_t panel, int x_start, int y_start, int x_end, int y_end, const void *color_data)
Draw bitmap on LCD panel. 

Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
x_start -- [in] Start pixel index in the target frame buffer, on x-axis (x_start is included) 
y_start -- [in] Start pixel index in the target frame buffer, on y-axis (y_start is included) 
x_end -- [in] End pixel index in the target frame buffer, on x-axis (x_end is not included) 
y_end -- [in] End pixel index in the target frame buffer, on y-axis (y_end is not included) 
color_data -- [in] RGB color data that will be dumped to the specific window range 


Returns

ESP_OK on success 





```

#### esp_lcd_panel_mirror


```


esp_err_t esp_lcd_panel_mirror(esp_lcd_panel_handle_t panel, bool mirror_x, bool mirror_y)
Mirror the LCD panel on specific axis. 

Note
Combined with esp_lcd_panel_swap_xy(), one can realize screen rotation


Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
mirror_x -- [in] Whether the panel will be mirrored about the x axis 
mirror_y -- [in] Whether the panel will be mirrored about the y axis 


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if this function is not supported by the panel 





```

#### esp_lcd_panel_swap_xy


```


esp_err_t esp_lcd_panel_swap_xy(esp_lcd_panel_handle_t panel, bool swap_axes)
Swap/Exchange x and y axis. 

Note
Combined with esp_lcd_panel_mirror(), one can realize screen rotation


Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
swap_axes -- [in] Whether to swap the x and y axis 


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if this function is not supported by the panel 





```

#### esp_lcd_panel_set_gap


```


esp_err_t esp_lcd_panel_set_gap(esp_lcd_panel_handle_t panel, int x_gap, int y_gap)
Set extra gap in x and y axis. 
The gap is the space (in pixels) between the left/top sides of the LCD panel and the first row/column respectively of the actual contents displayed.

Note
Setting a gap is useful when positioning or centering a frame that is smaller than the LCD.


Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
x_gap -- [in] Extra gap on x axis, in pixels 
y_gap -- [in] Extra gap on y axis, in pixels 


Returns

ESP_OK on success 





```

#### esp_lcd_panel_invert_color


```


esp_err_t esp_lcd_panel_invert_color(esp_lcd_panel_handle_t panel, bool invert_color_data)
Invert the color (bit-wise invert the color data line) 

Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
invert_color_data -- [in] Whether to invert the color data 


Returns

ESP_OK on success 





```

#### esp_lcd_panel_disp_on_off


```


esp_err_t esp_lcd_panel_disp_on_off(esp_lcd_panel_handle_t panel, bool on_off)
Turn on or off the display. 

Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
on_off -- [in] True to turns on display, False to turns off display 


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if this function is not supported by the panel 





```

#### esp_lcd_panel_disp_off


```


esp_err_t esp_lcd_panel_disp_off(esp_lcd_panel_handle_t panel, bool off)
Turn off the display. 

Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
off -- [in] Whether to turn off the screen 


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if this function is not supported by the panel 





```

#### esp_lcd_panel_disp_sleep


```


esp_err_t esp_lcd_panel_disp_sleep(esp_lcd_panel_handle_t panel, bool sleep)
Enter or exit sleep mode. 

Parameters

panel -- [in] LCD panel handle, which is created by other factory API like esp_lcd_new_panel_st7789()
sleep -- [in] True to enter sleep mode, False to wake up 


Returns

ESP_OK on success
ESP_ERR_NOT_SUPPORTED if this function is not supported by the panel 





```



### Header File


* components/esp_lcd/include/esp_lcd_panel_vendor.h
* This header file can be included with:



> ```
> #include "esp_lcd_panel_vendor.h"
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