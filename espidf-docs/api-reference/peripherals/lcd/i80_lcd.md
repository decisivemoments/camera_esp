# I80 Interfaced LCD


1. Create I80 bus by `esp_lcd_new_i80_bus()`. You need to set up the following parameters for an Intel 8080 parallel bus:



> * `esp_lcd_i80_bus_config_t::clk_src` sets the clock source of the I80 bus. Note, the default clock source may be different between ESP targets.
> 	* `esp_lcd_i80_bus_config_t::wr_gpio_num` sets the GPIO number of the pixel clock (also referred as `WR` in some LCD spec)
> 	* `esp_lcd_i80_bus_config_t::dc_gpio_num` sets the GPIO number of the data/command select pin (also referred as `RS` in some LCD spec)
> 	* `esp_lcd_i80_bus_config_t::bus_width` sets the bit width of the data bus (only support `8` or `16`)
> 	* `esp_lcd_i80_bus_config_t::data_gpio_nums` is the array of the GPIO number of the data bus. The number of GPIOs should be equal to the `esp_lcd_i80_bus_config_t::bus_width` value.
> 	* `esp_lcd_i80_bus_config_t::max_transfer_bytes` sets the maximum number of bytes that can be transferred in one transaction.
> 
> ```
> esp_lcd_i80_bus_handle_t i80_bus = NULL;
> esp_lcd_i80_bus_config_t bus_config = {
>     .clk_src = LCD_CLK_SRC_DEFAULT,
>     .dc_gpio_num = EXAMPLE_PIN_NUM_DC,
>     .wr_gpio_num = EXAMPLE_PIN_NUM_PCLK,
>     .data_gpio_nums = {
>         EXAMPLE_PIN_NUM_DATA0,
>         EXAMPLE_PIN_NUM_DATA1,
>         EXAMPLE_PIN_NUM_DATA2,
>         EXAMPLE_PIN_NUM_DATA3,
>         EXAMPLE_PIN_NUM_DATA4,
>         EXAMPLE_PIN_NUM_DATA5,
>         EXAMPLE_PIN_NUM_DATA6,
>         EXAMPLE_PIN_NUM_DATA7,
>     },
>     .bus_width = 8,
>     .max_transfer_bytes = EXAMPLE_LCD_H_RES * 100 * sizeof(uint16_t), // transfer 100 lines of pixels (assume pixel is RGB565) at most in one transaction
>     .dma_burst_size = EXAMPLE_DMA_BURST_SIZE,
> };
> ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));
> 
> ```
2. Allocate an LCD IO device handle from the I80 bus. In this step, you need to provide the following information:



> * `esp_lcd_panel_io_i80_config_t::cs_gpio_num` sets the GPIO number of the chip select pin.
> 	* `esp_lcd_panel_io_i80_config_t::pclk_hz` sets the pixel clock frequency in Hz. Higher pixel clock frequency results in higher refresh rate, but may cause flickering if the DMA bandwidth is not sufficient or the LCD controller chip does not support high pixel clock frequency.
> 	* `esp_lcd_panel_io_i80_config_t::lcd_cmd_bits` and `esp_lcd_panel_io_i80_config_t::lcd_param_bits` set the bit width of the command and parameter that recognized by the LCD controller chip. This is chip specific, you should refer to your LCD spec in advance.
> 	* `esp_lcd_panel_io_i80_config_t::trans_queue_depth` sets the maximum number of transactions that can be queued in the LCD IO device. A bigger value means more transactions can be queued up, but it also consumes more memory.
> 
> ```
> esp_lcd_panel_io_handle_t io_handle = NULL;
> esp_lcd_panel_io_i80_config_t io_config = {
>     .cs_gpio_num = EXAMPLE_PIN_NUM_CS,
>     .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
>     .trans_queue_depth = 10,
>     .dc_levels = {
>         .dc_idle_level = 0,
>         .dc_cmd_level = 0,
>         .dc_dummy_level = 0,
>         .dc_data_level = 1,
>     },
>     .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
>     .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
> };
> ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));
> 
> ```
3. Install the LCD controller driver. The LCD controller driver is responsible for sending the commands and parameters to the LCD controller chip. In this step, you need to specify the I80 IO device handle that allocated in the last step, and some panel specific configurations:



> * `esp_lcd_panel_dev_config_t::bits_per_pixel` sets the bit width of the pixel color data. The LCD driver uses this value to calculate the number of bytes to send to the LCD controller chip.
> 	* `esp_lcd_panel_dev_config_t::reset_gpio_num` sets the GPIO number of the reset pin. If the LCD controller chip does not have a reset pin, you can set this value to `-1`.
> 	* `esp_lcd_panel_dev_config_t::rgb_ele_order` sets the color order the pixel color data.
> 
> ```
> esp_lcd_panel_dev_config_t panel_config = {
>     .reset_gpio_num = EXAMPLE_PIN_NUM_RST,
>     .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
>     .bits_per_pixel = 16,
> };
> ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
> 
> ```




# API Reference



## Header File


* components/esp_lcd/include/esp_lcd_io_i80.h
* This header file can be included with:



> ```
> #include "esp_lcd_io_i80.h"
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


#### esp_lcd_new_i80_bus


```


esp_err_t esp_lcd_new_i80_bus(const esp_lcd_i80_bus_config_t *bus_config, esp_lcd_i80_bus_handle_t *ret_bus)
Create Intel 8080 bus handle. 

Parameters

bus_config -- [in] Bus configuration 
ret_bus -- [out] Returned bus handle 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_NO_MEM if out of memory
ESP_ERR_NOT_FOUND if no free bus is available
ESP_OK on success 





```

#### esp_lcd_del_i80_bus


```


esp_err_t esp_lcd_del_i80_bus(esp_lcd_i80_bus_handle_t bus)
Destroy Intel 8080 bus handle. 

Parameters
bus -- [in] Intel 8080 bus handle, created by esp_lcd_new_i80_bus()

Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_INVALID_STATE if there still be some device attached to the bus
ESP_OK on success 





```

#### esp_lcd_new_panel_io_i80


```


esp_err_t esp_lcd_new_panel_io_i80(esp_lcd_i80_bus_handle_t bus, const esp_lcd_panel_io_i80_config_t *io_config, esp_lcd_panel_io_handle_t *ret_io)
Create LCD panel IO, for Intel 8080 interface. 

Parameters

bus -- [in] Intel 8080 bus handle, created by esp_lcd_new_i80_bus()
io_config -- [in] IO configuration, for i80 interface 
ret_io -- [out] Returned panel IO handle 


Returns

ESP_ERR_INVALID_ARG if parameter is invalid
ESP_ERR_NOT_SUPPORTED if some configuration can't be satisfied, e.g. pixel clock out of the range
ESP_ERR_NO_MEM if out of memory
ESP_OK on success 





```

#### esp_lcd_i80_alloc_draw_buffer


```


void *esp_lcd_i80_alloc_draw_buffer(esp_lcd_panel_io_handle_t io, size_t size, uint32_t caps)
Allocate a draw buffer that can be used by I80 interfaced LCD panel. 

Note
This function differs from the normal 'heap_caps_*' functions in that it can also automatically handle the alignment required by DMA burst, cache line size, etc.


Parameters

io -- [in] Panel IO handle, created by esp_lcd_new_panel_io_i80()
size -- [in] Size of memory to be allocated 
caps -- [in] Bitwise OR of MALLOC_CAP_* flags indicating the type of memory desired for the allocation 


Returns
Pointer to a new buffer of size 'size' with capabilities 'caps', or NULL if allocation failed 



```



## Structures


#### esp_lcd_i80_bus_config_t


```


struct esp_lcd_i80_bus_config_t
LCD Intel 8080 bus configuration structure. 

Public Members
dc_gpio_num

int dc_gpio_num
GPIO used for D/C line 

wr_gpio_num

int wr_gpio_num
GPIO used for WR line 

clk_src

lcd_clock_source_t clk_src
Clock source for the I80 LCD peripheral 

data_gpio_nums

int data_gpio_nums[ESP_LCD_I80_BUS_WIDTH_MAX]
GPIOs used for data lines 

bus_width

size_t bus_width
Number of data lines, 8 or 16 

max_transfer_bytes

size_t max_transfer_bytes
Maximum transfer size, this determines the length of internal DMA link 

psram_trans_align

size_t psram_trans_align
DMA transfer alignment for data allocated from PSRAM 

dma_burst_size

size_t dma_burst_size
DMA burst size, in bytes 

sram_trans_align

size_t sram_trans_align
DMA transfer alignment for data allocated from SRAM 



```

#### esp_lcd_panel_io_i80_config_t


```


struct esp_lcd_panel_io_i80_config_t
Panel IO configuration structure, for intel 8080 interface. 

Public Members
cs_gpio_num

int cs_gpio_num
GPIO used for CS line, set to -1 will declaim exclusively use of I80 bus 

pclk_hz

uint32_t pclk_hz
Frequency of pixel clock 

trans_queue_depth

size_t trans_queue_depth
Transaction queue size, larger queue, higher throughput 

on_color_trans_done

esp_lcd_panel_io_color_trans_done_cb_t on_color_trans_done
Callback invoked when color data was transferred done 

user_ctx

void *user_ctx
User private data, passed directly to on_color_trans_done's user_ctx 

lcd_cmd_bits

int lcd_cmd_bits
Bit-width of LCD command 

lcd_param_bits

int lcd_param_bits
Bit-width of LCD parameter 

dc_idle_level

unsigned int dc_idle_level
Level of DC line in IDLE phase 

dc_cmd_level

unsigned int dc_cmd_level
Level of DC line in CMD phase 

dc_dummy_level

unsigned int dc_dummy_level
Level of DC line in DUMMY phase 

dc_data_level

unsigned int dc_data_level
Level of DC line in DATA phase 

dc_levels

struct esp_lcd_panel_io_i80_config_t::[anonymous] dc_levels
Each i80 device might have its own D/C control logic 

cs_active_high

unsigned int cs_active_high
If set, a high level of CS line will select the device, otherwise, CS line is low level active 

reverse_color_bits

unsigned int reverse_color_bits
Reverse the data bits, D[N:0] -> D[0:N] 

swap_color_bytes

unsigned int swap_color_bytes
Swap adjacent two color bytes 

pclk_active_neg

unsigned int pclk_active_neg
The display will write data lines when there's a falling edge on WR signal (a.k.a the PCLK) 

pclk_idle_low

unsigned int pclk_idle_low
The WR signal (a.k.a the PCLK) stays at low level in IDLE phase 

flags

struct esp_lcd_panel_io_i80_config_t::[anonymous] flags
Panel IO config flags 



```



## Macros


#### ESP_LCD_I80_BUS_WIDTH_MAX


```


ESP_LCD_I80_BUS_WIDTH_MAX
Maximum width of I80 bus 

```



## Type Definitions


#### esp_lcd_i80_bus_handle_t


```


typedef struct esp_lcd_i80_bus_t *esp_lcd_i80_bus_handle_t
Type of LCD intel 8080 bus handle 

```