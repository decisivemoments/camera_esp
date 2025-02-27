# I2C Interfaced LCD


1. Create I2C bus. Please refer to I2C API doc for more details.



> ```
> i2c_master_bus_handle_t i2c_bus = NULL;
> i2c_master_bus_config_t bus_config = {
>     .clk_source = I2C_CLK_SRC_DEFAULT,
>     .glitch_ignore_cnt = 7,
>     .i2c_port = I2C_BUS_PORT,
>     .sda_io_num = EXAMPLE_PIN_NUM_SDA,
>     .scl_io_num = EXAMPLE_PIN_NUM_SCL,
>     .flags.enable_internal_pullup = true,
> };
> ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));
> 
> ```
2. Allocate an LCD IO device handle from the I2C bus. In this step, you need to provide the following information:



> * `esp_lcd_panel_io_i2c_config_t::dev_addr` sets the I2C device address of the LCD controller chip. The LCD driver uses this address to communicate with the LCD controller chip.
> 	* `esp_lcd_panel_io_i2c_config_t::scl_speed_hz` sets the I2C clock frequency in Hz. The value should not exceed the range recommended in the LCD spec.
> 	* `esp_lcd_panel_io_i2c_config_t::lcd_cmd_bits` and `esp_lcd_panel_io_i2c_config_t::lcd_param_bits` set the bit width of the command and parameter that recognized by the LCD controller chip. This is chip specific, you should refer to your LCD spec in advance.
> 
> ```
> esp_lcd_panel_io_handle_t io_handle = NULL;
> esp_lcd_panel_io_i2c_config_t io_config = {
>     .dev_addr = EXAMPLE_I2C_HW_ADDR,
>     .scl_speed_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
>     .control_phase_bytes = 1, // refer to LCD spec
>     .dc_bit_offset = 6,       // refer to LCD spec
>     .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
>     .lcd_param_bits = EXAMPLE_LCD_CMD_BITS,
> };
> ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));
> 
> ```
3. Install the LCD controller driver. The LCD controller driver is responsible for sending the commands and parameters to the LCD controller chip. In this step, you need to specify the I2C IO device handle that allocated in the last step, and some panel specific configurations:



> * `esp_lcd_panel_dev_config_t::reset_gpio_num` sets the LCD's hardware reset GPIO number. If the LCD does not have a hardware reset pin, set this to `-1`.
> 	* `esp_lcd_panel_dev_config_t::bits_per_pixel` sets the bit width of the pixel color data. The LCD driver uses this value to calculate the number of bytes to send to the LCD controller chip.
> 
> ```
> esp_lcd_panel_handle_t panel_handle = NULL;
> esp_lcd_panel_dev_config_t panel_config = {
>     .bits_per_pixel = 1,
>     .reset_gpio_num = EXAMPLE_PIN_NUM_RST,
> };
> ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
> 
> ```




# API Reference



## Header File


* components/esp_lcd/include/esp_lcd_io_i2c.h
* This header file can be included with:



> ```
> #include "esp_lcd_io_i2c.h"
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




## Functions


#### esp_lcd_new_panel_io_i2c_v1


```


esp_err_t esp_lcd_new_panel_io_i2c_v1(uint32_t bus, const esp_lcd_panel_io_i2c_config_t *io_config, esp_lcd_panel_io_handle_t *ret_io)
Create LCD panel IO handle, for I2C interface in legacy implementation. 

Note
Please don't call this function in your project directly. Please call esp_lcd_new_panel_to_i2c instead.


Parameters

bus -- [in] I2C bus handle, (in uint32_t) 
io_config -- [in] IO configuration, for I2C interface 
ret_io -- [out] Returned IO handle


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_NO_MEM if out of memory
ESP_OK on success 





```

#### esp_lcd_new_panel_io_i2c_v2


```


esp_err_t esp_lcd_new_panel_io_i2c_v2(i2c_master_bus_handle_t bus, const esp_lcd_panel_io_i2c_config_t *io_config, esp_lcd_panel_io_handle_t *ret_io)
Create LCD panel IO handle, for I2C interface in new implementation. 

Note
Please don't call this function in your project directly. Please call esp_lcd_new_panel_to_i2c instead.


Parameters

bus -- [in] I2C bus handle, (in i2c_master_dev_handle_t) 
io_config -- [in] IO configuration, for I2C interface 
ret_io -- [out] Returned IO handle


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_NO_MEM if out of memory
ESP_OK on success 





```



## Structures


#### esp_lcd_panel_io_i2c_config_t


```


struct esp_lcd_panel_io_i2c_config_t
Panel IO configuration structure, for I2C interface. 

Public Members
dev_addr

uint32_t dev_addr
I2C device address 

on_color_trans_done

esp_lcd_panel_io_color_trans_done_cb_t on_color_trans_done
Callback invoked when color data transfer has finished 

user_ctx

void *user_ctx
User private data, passed directly to on_color_trans_done's user_ctx 

control_phase_bytes

size_t control_phase_bytes
I2C LCD panel will encode control information (e.g. D/C selection) into control phase, in several bytes 

dc_bit_offset

unsigned int dc_bit_offset
Offset of the D/C selection bit in control phase 

lcd_cmd_bits

int lcd_cmd_bits
Bit-width of LCD command 

lcd_param_bits

int lcd_param_bits
Bit-width of LCD parameter 

dc_low_on_data

unsigned int dc_low_on_data
If this flag is enabled, DC line = 0 means transfer data, DC line = 1 means transfer command; vice versa 

disable_control_phase

unsigned int disable_control_phase
If this flag is enabled, the control phase isn't used 

flags

struct esp_lcd_panel_io_i2c_config_t::[anonymous] flags
Extra flags to fine-tune the I2C device 

scl_speed_hz

uint32_t scl_speed_hz
I2C LCD SCL frequency (hz) 



```



## Macros


#### esp_lcd_new_panel_io_i2c


```


esp_lcd_new_panel_io_i2c(bus, io_config, ret_io)
Create LCD panel IO handle. 

Parameters

bus -- [in] I2C bus handle 
io_config -- [in] IO configuration, for I2C interface 
ret_io -- [out] Returned IO handle 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_NO_MEM if out of memory
ESP_OK on success 





```



## Type Definitions


#### esp_lcd_i2c_bus_handle_t


```


typedef uint32_t esp_lcd_i2c_bus_handle_t
Type of LCD I2C bus handle 

```