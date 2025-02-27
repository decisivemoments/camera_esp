# SPI Interfaced LCD


1. Create an SPI bus. Please refer to SPI Master API doc for more details.



> ```
> spi_bus_config_t buscfg = {
>     .sclk_io_num = EXAMPLE_PIN_NUM_SCLK,
>     .mosi_io_num = EXAMPLE_PIN_NUM_MOSI,
>     .miso_io_num = EXAMPLE_PIN_NUM_MISO,
>     .quadwp_io_num = -1, // Quad SPI LCD driver is not yet supported
>     .quadhd_io_num = -1, // Quad SPI LCD driver is not yet supported
>     .max_transfer_sz = EXAMPLE_LCD_H_RES * 80 * sizeof(uint16_t), // transfer 80 lines of pixels (assume pixel is RGB565) at most in one SPI transaction
> };
> ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO)); // Enable the DMA feature
> 
> ```
2. Allocate an LCD IO device handle from the SPI bus. In this step, you need to provide the following information:



> * `esp_lcd_panel_io_spi_config_t::dc_gpio_num`: Sets the gpio number for the DC signal line (some LCD calls this `RS` line). The LCD driver uses this GPIO to switch between sending command and sending data.
> 	* `esp_lcd_panel_io_spi_config_t::cs_gpio_num`: Sets the gpio number for the CS signal line. The LCD driver uses this GPIO to select the LCD chip. If the SPI bus only has one device attached (i.e., this LCD), you can set the gpio number to `-1` to occupy the bus exclusively.
> 	* `esp_lcd_panel_io_spi_config_t::pclk_hz` sets the frequency of the pixel clock, in Hz. The value should not exceed the range recommended in the LCD spec.
> 	* `esp_lcd_panel_io_spi_config_t::spi_mode` sets the SPI mode. The LCD driver uses this mode to communicate with the LCD. For the meaning of the SPI mode, please refer to the SPI Master API doc.
> 	* `esp_lcd_panel_io_spi_config_t::lcd_cmd_bits` and `esp_lcd_panel_io_spi_config_t::lcd_param_bits` set the bit width of the command and parameter that recognized by the LCD controller chip. This is chip specific, you should refer to your LCD spec in advance.
> 	* `esp_lcd_panel_io_spi_config_t::trans_queue_depth` sets the depth of the SPI transaction queue. A bigger value means more transactions can be queued up, but it also consumes more memory.
> 
> ```
> esp_lcd_panel_io_handle_t io_handle = NULL;
> esp_lcd_panel_io_spi_config_t io_config = {
>     .dc_gpio_num = EXAMPLE_PIN_NUM_LCD_DC,
>     .cs_gpio_num = EXAMPLE_PIN_NUM_LCD_CS,
>     .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
>     .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
>     .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
>     .spi_mode = 0,
>     .trans_queue_depth = 10,
> };
> // Attach the LCD to the SPI bus
> ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));
> 
> ```
3. Install the LCD controller driver. The LCD controller driver is responsible for sending the commands and parameters to the LCD controller chip. In this step, you need to specify the SPI IO device handle that allocated in the last step, and some panel specific configurations:



> * `esp_lcd_panel_dev_config_t::reset_gpio_num` sets the LCD's hardware reset GPIO number. If the LCD does not have a hardware reset pin, set this to `-1`.
> 	* `esp_lcd_panel_dev_config_t::rgb_ele_order` sets the R-G-B element order of each color data.
> 	* `esp_lcd_panel_dev_config_t::bits_per_pixel` sets the bit width of the pixel color data. The LCD driver uses this value to calculate the number of bytes to send to the LCD controller chip.
> 	* `esp_lcd_panel_dev_config_t::data_endian` specifies the data endian to be transmitted to the screen. No need to specify for color data within 1 byte, like RGB232. For drivers that do not support specifying data endian, this field would be ignored.
> 
> ```
> esp_lcd_panel_handle_t panel_handle = NULL;
> esp_lcd_panel_dev_config_t panel_config = {
>     .reset_gpio_num = EXAMPLE_PIN_NUM_RST,
>     .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
>     .bits_per_pixel = 16,
> };
> // Create LCD panel handle for ST7789, with the SPI IO device handle
> ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
> 
> ```




# API Reference



## Header File


* components/esp_lcd/include/esp_lcd_io_spi.h
* This header file can be included with:



> ```
> #include "esp_lcd_io_spi.h"
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


#### esp_lcd_new_panel_io_spi


```


esp_err_t esp_lcd_new_panel_io_spi(esp_lcd_spi_bus_handle_t bus, const esp_lcd_panel_io_spi_config_t *io_config, esp_lcd_panel_io_handle_t *ret_io)
Create LCD panel IO handle, for SPI interface. 

Parameters

bus -- [in] SPI bus handle 
io_config -- [in] IO configuration, for SPI interface 
ret_io -- [out] Returned IO handle 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_NO_MEM if out of memory
ESP_OK on success 





```



## Structures


#### esp_lcd_panel_io_spi_config_t


```


struct esp_lcd_panel_io_spi_config_t
Panel IO configuration structure, for SPI interface. 

Public Members
cs_gpio_num

int cs_gpio_num
GPIO used for CS line 

dc_gpio_num

int dc_gpio_num
GPIO used to select the D/C line, set this to -1 if the D/C line is not used 

spi_mode

int spi_mode
Traditional SPI mode (0~3) 

pclk_hz

unsigned int pclk_hz
Frequency of pixel clock 

trans_queue_depth

size_t trans_queue_depth
Size of internal transaction queue 

on_color_trans_done

esp_lcd_panel_io_color_trans_done_cb_t on_color_trans_done
Callback invoked when color data transfer has finished 

user_ctx

void *user_ctx
User private data, passed directly to on_color_trans_done's user_ctx 

lcd_cmd_bits

int lcd_cmd_bits
Bit-width of LCD command 

lcd_param_bits

int lcd_param_bits
Bit-width of LCD parameter 

dc_high_on_cmd

unsigned int dc_high_on_cmd
If enabled, DC level = 1 indicates command transfer 

dc_low_on_data

unsigned int dc_low_on_data
If enabled, DC level = 0 indicates color data transfer 

dc_low_on_param

unsigned int dc_low_on_param
If enabled, DC level = 0 indicates parameter transfer 

octal_mode

unsigned int octal_mode
transmit with octal mode (8 data lines), this mode is used to simulate Intel 8080 timing 

quad_mode

unsigned int quad_mode
transmit with quad mode (4 data lines), this mode is useful when transmitting LCD parameters (Only use one line for command) 

sio_mode

unsigned int sio_mode
Read and write through a single data line (MOSI) 

lsb_first

unsigned int lsb_first
transmit LSB bit first 

cs_high_active

unsigned int cs_high_active
CS line is high active 

flags

struct esp_lcd_panel_io_spi_config_t::[anonymous] flags
Extra flags to fine-tune the SPI device 



```



## Type Definitions


#### esp_lcd_spi_bus_handle_t


```


typedef int esp_lcd_spi_bus_handle_t
Type of LCD SPI bus handle 

```