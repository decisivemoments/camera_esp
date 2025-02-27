# Internal and Unstable APIs



This section is listing some APIs that are internal or likely to be changed or removed in the next releases of ESP-IDF.



## API Reference



### Header File


* components/esp_rom/include/esp_rom_sys.h
* This header file can be included with:



> ```
> #include "esp_rom_sys.h"
> 
> ```




### Functions


#### esp_rom_software_reset_system


```


void esp_rom_software_reset_system(void)
Software Reset digital core include RTC. 
It is not recommended to use this function in esp-idf, use esp_restart() instead. 

```

#### esp_rom_software_reset_cpu


```


void esp_rom_software_reset_cpu(int cpu_no)
Software Reset cpu core. 
It is not recommended to use this function in esp-idf, use esp_restart() instead.

Parameters
cpu_no -- : The CPU to reset, 0 for PRO CPU, 1 for APP CPU. 



```

#### esp_rom_printf


```


int esp_rom_printf(const char *fmt, ...)
Print formated string to console device. 

Note
float and long long data are not supported!


Parameters

fmt -- Format string 
... -- Additional arguments, depending on the format string 


Returns
int: Total number of characters written on success; A negative number on failure. 



```

#### esp_rom_delay_us


```


void esp_rom_delay_us(uint32_t us)
Pauses execution for us microseconds. 

Parameters
us -- Number of microseconds to pause 



```

#### esp_rom_install_channel_putc


```


void esp_rom_install_channel_putc(int channel, void (*putc)(char c))
esp_rom_printf can print message to different channels simultaneously. This function can help install the low level putc function for esp_rom_printf. 

Parameters

channel -- Channel number (startting from 1) 
putc -- Function pointer to the putc implementation. Set NULL can disconnect esp_rom_printf with putc. 




```

#### esp_rom_install_uart_printf


```


void esp_rom_install_uart_printf(void)
Install UART1 as the default console channel, equivalent to esp_rom_install_channel_putc(1, esp_rom_output_putc)

```

#### esp_rom_get_reset_reason


```


soc_reset_reason_t esp_rom_get_reset_reason(int cpu_no)
Get reset reason of CPU. 

Parameters
cpu_no -- CPU number 

Returns
Reset reason code (see in soc/reset_reasons.h) 



```

#### esp_rom_route_intr_matrix


```


void esp_rom_route_intr_matrix(int cpu_core, uint32_t periph_intr_id, uint32_t cpu_intr_num)
Route peripheral interrupt sources to CPU's interrupt port by matrix. 
Usually there're 4 steps to use an interrupt:
Route peripheral interrupt source to CPU. e.g. esp_rom_route_intr_matrix(0, ETS_WIFI_MAC_INTR_SOURCE, ETS_WMAC_INUM)
Set interrupt handler for CPU
Enable CPU interrupt
Enable peripheral interrupt



Parameters

cpu_core -- The CPU number, which the peripheral interrupt will inform to 
periph_intr_id -- The peripheral interrupt source number 
cpu_intr_num -- The CPU (external) interrupt number. On targets that use CLIC as their interrupt controller, this number represents the external interrupt number. For example, passing cpu_intr_num = i to this function would in fact bind peripheral source to CPU interrupt CLIC_EXT_INTR_NUM_OFFSET + i. 




```

#### esp_rom_get_cpu_ticks_per_us


```


uint32_t esp_rom_get_cpu_ticks_per_us(void)
Get the real CPU ticks per us. 

Returns
CPU ticks per us 



```

#### esp_rom_set_cpu_ticks_per_us


```


void esp_rom_set_cpu_ticks_per_us(uint32_t ticks_per_us)
Set the real CPU tick rate. 

Note
Call this function when CPU frequency is changed, otherwise the esp_rom_delay_us can be inaccurate.


Parameters
ticks_per_us -- CPU ticks per us 



```