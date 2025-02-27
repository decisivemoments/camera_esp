# ULP RISC-V Coprocessor Programming



The ULP RISC-V coprocessor is a variant of the ULP present in ESP32-S3. Similar to ULP FSM, the ULP RISC-V coprocessor can perform tasks such as sensor readings while the main CPU stays in low power modes. The main difference between ULP FSM and ULP RISC-V is that the latter can be programmed in C using standard GNU tools. The ULP RISC-V coprocessor can access the RTC_SLOW_MEM memory region, and registers in `RTC_CNTL`, `RTC_IO`, and `SARADC` peripherals. The RISC-V processor is a 32-bit fixed point machine. Its instruction set is based on RV32IMC which includes hardware multiplication and division, and compressed code.



## Installing the ULP RISC-V Toolchain


The ULP RISC-V coprocessor code is written in C (assembly is also possible) and compiled using the RISC-V toolchain based on GCC.


If you have already set up ESP-IDF with CMake build system according to the Getting Started Guide, then the toolchain should already be installed.



Note


In earlier versions of ESP-IDF, RISC-V toolchain had a different prefix: `riscv-none-embed-gcc`.





## Compiling the ULP RISC-V Code


To compile the ULP RISC-V code as part of the component, the following steps must be taken:


1. The ULP RISC-V code, written in C or assembly (must use the `.S` extension), must be placed in a separate directory inside the component directory, for instance, `ulp/`.



Note


When registering the component (via `idf_component_register`), this directory should not be added to the `SRC_DIRS` argument as it is currently done for the ULP FSM. See the step below for how to properly add ULP source files.



2. Call `ulp_embed_binary` from the component CMakeLists.txt after registration. For example:



```
...
idf_component_register()

set(ulp_app_name ulp_${COMPONENT_NAME})
set(ulp_sources "ulp/ulp_c_source_file.c" "ulp/ulp_assembly_source_file.S")
set(ulp_exp_dep_srcs "ulp_c_source_file.c")

ulp_embed_binary(${ulp_app_name} "${ulp_sources}" "${ulp_exp_dep_srcs}")

```



> The first argument to `ulp_embed_binary` specifies the ULP binary name. The name specified here will also be used by other generated artifacts such as the ELF file, map file, header file, and linker export file. The second argument specifies the ULP source files. Finally, the third argument specifies the list of component source files which include the header file to be generated. This list is needed to build the dependencies correctly and ensure that the generated header file will be created before any of these files are compiled. See the section below for the concept of generated header files for ULP applications.


3. Build the application as usual (e.g., `idf.py app`).


Inside, the build system will take the following steps to build ULP program:


	1. **Run each source file through the C compiler and assembler.** This step generates the object files (`.obj.c` or `.obj.S` depending of source file processed) in the component build directory.
	2. **Run the linker script template through the C preprocessor.** The template is located in `components/ulp/ld` directory.
	3. **Link the object files into an output ELF file** (`ulp_app_name.elf`). The Map file (`ulp_app_name.map`) generated at this stage may be useful for debugging purposes.
	4. **Dump the contents of the ELF file into a binary** (`ulp_app_name.bin`) which can then be embedded into the application.
	5. **Generate a list of global symbols** (`ulp_app_name.sym`) in the ELF file using `riscv32-esp-elf-nm`.
	6. **Create an LD export script and a header file** (`ulp_app_name.ld` and `ulp_app_name.h`) containing the symbols from `ulp_app_name.sym`. This is done using the `esp32ulp_mapgen.py` utility.
	7. **Add the generated binary to the list of binary files** to be embedded into the application.




## Accessing the ULP RISC-V Program Variables


Global symbols defined in the ULP RISC-V program may be used inside the main program.


For example, the ULP RISC-V program may define a variable `measurement_count` which will define the number of ADC measurements the program needs to make before waking up the chip from deep sleep.



```
volatile int measurement_count;

int some_function()
{
    //read the measurement count for later use.
    int temp = measurement_count;

    ...do something.
}

```


The main program can access the global ULP RISC-V program variables as the build system makes this possible by generating the `${ULP_APP_NAME}.h` and `${ULP_APP_NAME}.ld` files which define the global symbols present in the ULP RISC-V program. Each global symbol defined in the ULP RISC-V program is included in these files and are prefixed with `ulp_`.


The header file contains the declaration of the symbol:



```
extern uint32_t ulp_measurement_count;

```


Note that all symbols (variables, arrays, functions) are declared as `uint32_t`. For functions and arrays, take the address of the symbol and cast it to the appropriate type.


The generated linker script file defines the locations of symbols in RTC_SLOW_MEM:



```
PROVIDE ( ulp_measurement_count = 0x50000060 );

```


To access the ULP RISC-V program variables from the main program, the generated header file should be included using an `include` statement. This will allow the ULP RISC-V program variables to be accessed as regular variables.



```
#include "ulp_app_name.h"

void init_ulp_vars() {
    ulp_measurement_count = 64;
}

```



### Mutual Exclusion


If mutual exclusion is needed when accessing a variable shared between the main program and ULP, then this can be achieved by using the ULP RISC-V lock API:



> * `ulp_riscv_lock_acquire()`
> * `ulp_riscv_lock_release()`


The ULP does not have any hardware instructions to facilitate mutual exclusion, so the lock API achieves this through a software algorithm (Peterson's algorithm).


The locks are intended to only be called from a single thread in the main program, and will not provide mutual exclusion if used simultaneously from multiple threads.





## Starting the ULP RISC-V Program


To run a ULP RISC-V program, the main application needs to load the ULP program into RTC memory using the `ulp_riscv_load_binary()` function, and then start it using the `ulp_riscv_run()` function.


Note that the `CONFIG_ULP_COPROC_ENABLED` and `CONFIG_ULP_COPROC_TYPE_RISCV` options must be enabled in menuconfig to work with ULP RISC-V. To reserve memory for the ULP, the `RTC slow memory reserved for coprocessor` option must be set to a value big enough to store ULP RISC-V code and data. If the application components contain multiple ULP programs, then the size of the RTC memory must be sufficient to hold the largest one.


Each ULP RISC-V program is embedded into the ESP-IDF application as a binary blob. The application can reference this blob and load it in the following way (suppose ULP_APP_NAME was defined to `ulp_app_name`):



```
extern const uint8_t bin_start[] asm("_binary_ulp_app_name_bin_start");
extern const uint8_t bin_end[]   asm("_binary_ulp_app_name_bin_end");

void start_ulp_program() {
    ESP_ERROR_CHECK( ulp_riscv_load_binary( bin_start,
        (bin_end - bin_start)) );
}

```


Once the program is loaded into RTC memory, the application can start it by calling the `ulp_riscv_run()` function:



```
ESP_ERROR_CHECK( ulp_riscv_run() );

```




## ULP RISC-V Program Flow


The ULP RISC-V coprocessor is started by a timer. The timer is started once `ulp_riscv_run()` is called. The timer counts the number of RTC_SLOW_CLK ticks (by default, produced by an internal 136 kHz RC oscillator). The number of ticks is set using `RTC_CNTL_ULP_CP_TIMER_1_REG` register. When starting the ULP, `RTC_CNTL_ULP_CP_TIMER_1_REG` will be used to set the number of timer ticks.


The application can set ULP timer period values (RTC_CNTL_ULP_CP_TIMER_1_REG) using the `ulp_set_wakeup_period()` function.


Once the timer counts the number of ticks set in the `RTC_CNTL_ULP_CP_TIMER_1_REG` register, the ULP RISC-V coprocessor will power up and start running the program from the entry point set in the call to `ulp_riscv_run()`.


The program runs until the field `RTC_CNTL_COCPU_DONE` in register `RTC_CNTL_COCPU_CTRL_REG` gets written or when a trap occurs due to illegal processor state. Once the program halts, the ULP RISC-V coprocessor will power down, and the timer will be started again.


To disable the timer (effectively preventing the ULP program from running again), please clear the `RTC_CNTL_ULP_CP_SLP_TIMER_EN` bit in the `RTC_CNTL_ULP_CP_TIMER_REG` register. This can be done both from the ULP code and from the main program.




## ULP RISC-V Peripheral Support


To enhance the capabilities of the ULP RISC-V coprocessor, it has access to peripherals which operate in the low-power (RTC) domain. The ULP RISC-V coprocessor can interact with these peripherals when the main CPU is in sleep mode, and can wake up the main CPU once a wakeup condition is reached. The following peripherals are supported.



### RTC I2C


The RTC I2C controller provides I2C master functionality in the RTC domain. The ULP RISC-V coprocessor can read from or write to I2C slave devices using this controller. To use the RTC I2C peripheral, call the `ulp_riscv_i2c_master_init()` from your application running on the main core before initializing the ULP RISC-V core and going to sleep.


Once the RTC I2C controller is initialized, the I2C slave device address must be programmed via the `ulp_riscv_i2c_master_set_slave_addr()` API before any read or write operation is performed.



Note


The RTC I2C peripheral always expects a slave sub-register address to be programmed via the `ulp_riscv_i2c_master_set_slave_reg_addr()` API. If it is not, the I2C peripheral uses the `SENS_SAR_I2C_CTRL_REG[18:11]` as the sub-register address for the subsequent read or write operations. This could make the RTC I2C peripheral incompatible with certain I2C devices or sensors which do not need any sub-register to be programmed.




Note


There is no hardware atomicity protection in accessing the RTC I2C peripheral between the main CPU and the ULP RISC-V core. Therefore, care must be taken that both cores are not accessing the peripheral simultaneously.



In case your RTC I2C based ULP RISC-V program is not working as expected, the following sanity checks can help in debugging the issue:



> * Incorrect SDA/SCL pin selection: The SDA pin can only be set up as GPIO1 or GPIO3 and SCL pin can only be set up as GPIO0 or GPIO2. Make sure that the pin configuration is correct.
> * Incorrect I2C timing parameters: The RTC I2C bus timing configuration is limited by the I2C standard bus specification. Any timing parameters which violate the standard I2C bus specifications would result in an error. For details on the timing parameters, please read the standard I2C bus specifications.
> * If the I2C slave device or sensor does not require a sub-register address to be programmed, it may not be compatible with the RTC I2C peripheral. Please refer the notes above.
> * If the RTC driver reports a `Write Failed!` or `Read Failed!` error log when running on the main CPU, then make sure:
> 
> 
> 
> > + The I2C slave device or sensor works correctly with the standard I2C master on Espressif SoCs. This would rule out any problems with the I2C slave device itself.
> > 	+ If the RTC I2C interrupt status log reports a `TIMEOUT` error or `ACK` error, it could typically mean that the I2C device did not respond to a `START` condition sent out by the RTC I2C controller. This could happen if the I2C slave device is not connected properly to the controller pins or if the I2C slave device is in a bad state. Make sure that the I2C slave device is in a good state and connected properly before continuing.
> > 	+ If the RTC I2C interrupt log does not report any error status, it could mean that the driver is not fast enough in receiving data from the I2C slave device. This could happen as the RTC I2C controller does not have a TX/RX FIFO to store multiple bytes of data but rather, it depends on single byte transmissions using an interrupt status polling mechanism. This could be mitigated to some extent by making sure that the SCL clock of the peripheral is running as fast as possible. This can be tweaked by configuring the SCL low period and SCL high period values in the initialization config parameters for the peripheral.


* Other methods of debugging problems would be to ensure that the RTC I2C controller is operational **only** on the main CPU **without** any ULP RISC-V code interfering and **without** any sleep mode being activated. This is the basic configuration under which the RTC I2C peripheral must work. This way you can rule out any potential issues due to the ULP or sleep modes.





## ULP RISC-V Interrupt Handling


The ULP RISC-V core supports interrupt handling from certain internal and external events. By design, the ULP RISC-V core can handle interrupts from the following sources:




ULP RISC-V interrupt sources





| Interrupt Source | Type | IRQ |
| --- | --- | --- |
| Internal Timer Interrupt | Internal | 0 |
| EBREAK or ECALL or Illegal Instruction | Internal | 1 |
| Unaligned Memory Access | Internal | 2 |
| RTC Peripheral Sources | External | 31 |


Interrupt handling is enabled via special 32-bit registers Q0-Q3 and custom R-type instructions. For more information, see *ESP32-S3 Technical Reference Manual* > *ULP Coprocessor* > *ULP-RISC-V* > *ULP-RISC-V Interrupts* [PDF].


All interrupts are enabled globally during start-up. When an interrupt occurs, the processor jumps to the IRQ vector. The IRQ vector performs the task of saving the register context and then calling the global interrupt dispatcher. The ULP RISC-V driver implements a *weak* interrupt dispatcher `_ulp_riscv_interrupt_handler()` which serves as the central point for handling all interrupts. This global dispatcher calls respective interrupt handlers which have been allocated via the `ulp_riscv_intr_alloc()`.


Interrupt handling on the ULP RISC-V is not fully featured yet. At present, interrupt handling for internal interrupt sources is not supported. Support is provided for two RTC peripheral sources, viz., software-triggered interrupts and RTC IO-triggered interrupts. ULP RISC-V does not support nested interrupts. If users need custom interrupt handling then they may override the default global interrupt dispatcher by defining their own `_ulp_riscv_interrupt_handler()`.




## Debugging Your ULP RISC-V Program


When programming the ULP RISC-V, it can sometimes be challenging to figure out why the program is not behaving as expected. Due to the simplicity of the core, many of the standard methods of debugging, e.g., JTAG or `printf`, are simply not available.


Keeping this in mind, here are some ways that may help you debug your ULP RISC-V program:



> * Share program state through shared variables: as described in Accessing the ULP RISC-V Program Variables, both the main CPU and the ULP core can easily access global variables in RTC memory. Writing state information to such a variable from the ULP and reading it from the main CPU can help you discern what is happening on the ULP core. The downside of this approach is that it requires the main CPU to be awake, which will not always be the case. Keeping the main CPU awake might even, in some cases, mask problems, as some issues may only occur when certain power domains are powered down.
> * Use the bit-banged UART driver to print: the ULP RISC-V component comes with a low-speed bit-banged UART TX driver that can be used for printing information independently of the main CPU state. See system/ulp/ulp_riscv/uart_print for an example of how to use this driver.
> * Trap signal: the ULP RISC-V has a hardware trap that will trigger under certain conditions, e.g., illegal instruction. This will cause the main CPU to be woken up with the wake-up cause `ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG`.




## Application Examples


* ULP RISC-V Coprocessor polls GPIO while main CPU is in deep sleep: system/ulp/ulp_riscv/gpio.
* ULP RISC-V Coprocessor uses bit-banged UART driver to print: system/ulp/ulp_riscv/uart_print.
* ULP RISC-V Coprocessor reads external temperature sensor while main CPU is in deep sleep: system/ulp/ulp_riscv/ds18b20_onewire.
* ULP RISC-V Coprocessor reads external I2C temperature and humidity sensor (BMP180) while the main CPU is in Deep-sleep and wakes up the main CPU once a threshold is met: system/ulp/ulp_riscv/i2c.
* ULP RISC-V Coprocessor handles software interrupt and RTC IO interrupt: system/ulp/ulp_riscv/interrupts.




## API Reference



### Header File


* components/ulp/ulp_riscv/include/ulp_riscv.h
* This header file can be included with:



> ```
> #include "ulp_riscv.h"
> 
> ```
* This header file is a part of the API provided by the `ulp` component. To declare that your component depends on `ulp`, add the following to your CMakeLists.txt:



> ```
> REQUIRES ulp
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES ulp
> 
> ```




### Functions


#### ulp_riscv_isr_register


```


esp_err_t ulp_riscv_isr_register(intr_handler_t fn, void *arg, uint32_t mask)
Register ULP signal ISR. 

Note
The ISR routine will only be active if the main CPU is not in deepsleep


Parameters

fn -- ISR callback function 
arg -- ISR callback function arguments 
mask -- Bit mask to enable the required ULP RISC-V interrupt signals 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if callback function is NULL or if the interrupt bits are invalid
ESP_ERR_NO_MEM if heap memory cannot be allocated for the interrupt
other errors returned by esp_intr_alloc 





```

#### ulp_riscv_isr_deregister


```


esp_err_t ulp_riscv_isr_deregister(intr_handler_t fn, void *arg, uint32_t mask)
Deregister ULP signal ISR. 

Parameters

fn -- ISR callback function 
arg -- ISR callback function arguments 
mask -- Bit mask to enable the required ULP RISC-V interrupt signals 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if callback function is NULL or if the interrupt bits are invalid
ESP_ERR_INVALID_STATE if a handler matching both callback function and its arguments isn't registered 





```

#### ulp_riscv_config_and_run


```


esp_err_t ulp_riscv_config_and_run(ulp_riscv_cfg_t *cfg)
Configure the ULP and run the program loaded into RTC memory. 

Parameters
cfg -- pointer to the config struct 

Returns
ESP_OK on success 



```

#### ulp_riscv_run


```


esp_err_t ulp_riscv_run(void)
Configure the ULP with default settings and run the program loaded into RTC memory. 

Returns
ESP_OK on success 



```

#### ulp_riscv_load_binary


```


esp_err_t ulp_riscv_load_binary(const uint8_t *program_binary, size_t program_size_bytes)
Load ULP-RISC-V program binary into RTC memory. 
Different than ULP FSM, the binary program has no special format, it is the ELF file generated by RISC-V toolchain converted to binary format using objcopy.
Linker script in components/ulp/ld/ulp_riscv.ld produces ELF files which correspond to this format. This linker script produces binaries with load_addr == 0.

Parameters

program_binary -- pointer to program binary 
program_size_bytes -- size of the program binary 


Returns

ESP_OK on success
ESP_ERR_INVALID_SIZE if program_size_bytes is more than 8KiB 





```

#### ulp_riscv_timer_stop


```


void ulp_riscv_timer_stop(void)
Stop the ULP timer. 

Note
This will stop the ULP from waking up if halted, but will not abort any program currently executing on the ULP. 


```

#### ulp_riscv_timer_resume


```


void ulp_riscv_timer_resume(void)
Resumes the ULP timer. 

Note
This will resume an already configured timer, but does no other configuration 


```

#### ulp_riscv_halt


```


void ulp_riscv_halt(void)
Halts the program currently running on the ULP-RISC-V. 

Note
Program will restart at the next ULP timer trigger if timer is still running. If you want to stop the ULP from waking up then call ulp_riscv_timer_stop() first. 


```

#### ulp_riscv_reset


```


void ulp_riscv_reset(void)
Resets the ULP-RISC-V core from the main CPU. 

Note
This will reset the ULP core from the main CPU. It is intended to be used when the ULP is in a bad state and cannot run as intended due to a corrupt firmware or any other reason. The main core can reset the ULP core with this API and then re-initilialize the ULP. 


```



### Structures


#### ulp_riscv_cfg_t


```


struct ulp_riscv_cfg_t
ULP riscv init parameters. 

Public Members
wakeup_source

ulp_riscv_wakeup_source_t wakeup_source
ULP wakeup source 



```



### Macros


#### ULP_RISCV_DEFAULT_CONFIG


```


ULP_RISCV_DEFAULT_CONFIG()

```

#### ULP_RISCV_SW_INT


```


ULP_RISCV_SW_INT

```

#### ULP_RISCV_TRAP_INT


```


ULP_RISCV_TRAP_INT

```



### Enumerations


#### ulp_riscv_wakeup_source_t


```


enum ulp_riscv_wakeup_source_t
Values:
ULP_RISCV_WAKEUP_SOURCE_TIMER

enumerator ULP_RISCV_WAKEUP_SOURCE_TIMER

ULP_RISCV_WAKEUP_SOURCE_GPIO

enumerator ULP_RISCV_WAKEUP_SOURCE_GPIO


```



### Header File


* components/ulp/ulp_riscv/shared/include/ulp_riscv_lock_shared.h
* This header file can be included with:



> ```
> #include "ulp_riscv_lock_shared.h"
> 
> ```
* This header file is a part of the API provided by the `ulp` component. To declare that your component depends on `ulp`, add the following to your CMakeLists.txt:



> ```
> REQUIRES ulp
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES ulp
> 
> ```




### Structures


#### ulp_riscv_lock_t


```


struct ulp_riscv_lock_t
Structure representing a lock shared between ULP and main CPU. 

Public Members
critical_section_flag_ulp

bool critical_section_flag_ulp
ULP wants to enter the critical sections 

critical_section_flag_main_cpu

bool critical_section_flag_main_cpu
Main CPU wants to enter the critical sections 

turn

ulp_riscv_lock_turn_t turn
Which CPU is allowed to enter the critical section 



```



### Enumerations


#### ulp_riscv_lock_turn_t


```


enum ulp_riscv_lock_turn_t
Enum representing which processor is allowed to enter the critical section. 
Values:
ULP_RISCV_LOCK_TURN_ULP

enumerator ULP_RISCV_LOCK_TURN_ULP
ULP's turn to enter the critical section 

ULP_RISCV_LOCK_TURN_MAIN_CPU

enumerator ULP_RISCV_LOCK_TURN_MAIN_CPU
Main CPU's turn to enter the critical section 


```



### Header File


* components/ulp/ulp_riscv/include/ulp_riscv_lock.h
* This header file can be included with:



> ```
> #include "ulp_riscv_lock.h"
> 
> ```
* This header file is a part of the API provided by the `ulp` component. To declare that your component depends on `ulp`, add the following to your CMakeLists.txt:



> ```
> REQUIRES ulp
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES ulp
> 
> ```




### Functions


#### ulp_riscv_lock_acquire


```


void ulp_riscv_lock_acquire(ulp_riscv_lock_t *lock)
Locks are based on the Peterson's algorithm, https://en.wikipedia.org/wiki/Peterson%27s_algorithm. 
Acquire the lock, preventing the ULP from taking until released. Spins until lock is acquired.

Note
The lock is only designed for being used by a single thread on the main CPU, it is not safe to try to acquire it from multiple threads.


Parameters
lock -- Pointer to lock struct, shared with ULP 



```

#### ulp_riscv_lock_release


```


void ulp_riscv_lock_release(ulp_riscv_lock_t *lock)
Release the lock. 

Parameters
lock -- Pointer to lock struct, shared with ULP 



```



### Header File


* components/ulp/ulp_riscv/include/ulp_riscv_i2c.h
* This header file can be included with:



> ```
> #include "ulp_riscv_i2c.h"
> 
> ```
* This header file is a part of the API provided by the `ulp` component. To declare that your component depends on `ulp`, add the following to your CMakeLists.txt:



> ```
> REQUIRES ulp
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES ulp
> 
> ```




### Functions


#### ulp_riscv_i2c_master_set_slave_addr


```


void ulp_riscv_i2c_master_set_slave_addr(uint8_t slave_addr)
Set the I2C slave device address. 

Parameters
slave_addr -- I2C slave address (7 bit) 



```

#### ulp_riscv_i2c_master_set_slave_reg_addr


```


void ulp_riscv_i2c_master_set_slave_reg_addr(uint8_t slave_reg_addr)
Set the I2C slave device sub register address. 

Note
The RTC I2C peripheral always expects a slave sub register address to be programmed. If it is not, the I2C peripheral uses the SENS_SAR_I2C_CTRL_REG[18:11] as the sub register address for the subsequent read or write operation.


Parameters
slave_reg_addr -- I2C slave sub register address 



```

#### ulp_riscv_i2c_master_read_from_device


```


void ulp_riscv_i2c_master_read_from_device(uint8_t *data_rd, size_t size)
Read from I2C slave device. 

Note
The I2C slave device address must be configured at least once before invoking this API.


Parameters

data_rd -- Buffer to hold data to be read 
size -- Size of data to be read in bytes 




```

#### ulp_riscv_i2c_master_write_to_device


```


void ulp_riscv_i2c_master_write_to_device(uint8_t *data_wr, size_t size)
Write to I2C slave device. 

Note
The I2C slave device address must be configured at least once before invoking this API.


Parameters

data_wr -- Buffer which holds the data to be written 
size -- Size of data to be written in bytes 




```

#### ulp_riscv_i2c_master_init


```


esp_err_t ulp_riscv_i2c_master_init(const ulp_riscv_i2c_cfg_t *cfg)
Initialize and configure the RTC I2C for use by ULP RISC-V Currently RTC I2C can only be used in master mode. 

Parameters
cfg -- Configuration parameters 

Returns
esp_err_t ESP_OK when successful 



```



### Structures


#### ulp_riscv_i2c_pin_cfg_t


```


struct ulp_riscv_i2c_pin_cfg_t
ULP RISC-V RTC I2C pin config. 

Public Members
sda_io_num

uint32_t sda_io_num
GPIO pin for SDA signal. Only GPIO#1 or GPIO#3 can be used as the SDA pin. 

scl_io_num

uint32_t scl_io_num
GPIO pin for SCL signal. Only GPIO#0 or GPIO#2 can be used as the SCL pin. 

sda_pullup_en

bool sda_pullup_en
SDA line enable internal pullup. Can be configured if external pullup is not used. 

scl_pullup_en

bool scl_pullup_en
SCL line enable internal pullup. Can be configured if external pullup is not used. 



```

#### ulp_riscv_i2c_timing_cfg_t


```


struct ulp_riscv_i2c_timing_cfg_t
ULP RISC-V RTC I2C timing config. 

Public Members
scl_low_period

float scl_low_period
SCL low period in micro seconds 

scl_high_period

float scl_high_period
SCL high period in micro seconds 

sda_duty_period

float sda_duty_period
Period between the SDA switch and the falling edge of SCL in micro seconds 

scl_start_period

float scl_start_period
Waiting time after the START condition in micro seconds 

scl_stop_period

float scl_stop_period
Waiting time before the END condition in micro seconds 

i2c_trans_timeout

float i2c_trans_timeout
I2C transaction timeout in micro seconds 



```

#### ulp_riscv_i2c_cfg_t


```


struct ulp_riscv_i2c_cfg_t
ULP RISC-V RTC I2C init parameters. 

Public Members
i2c_pin_cfg

ulp_riscv_i2c_pin_cfg_t i2c_pin_cfg
RTC I2C pin configuration 

i2c_timing_cfg

ulp_riscv_i2c_timing_cfg_t i2c_timing_cfg
RTC I2C timing configuration 



```



### Macros


#### ULP_RISCV_I2C_DEFAULT_GPIO_CONFIG


```


ULP_RISCV_I2C_DEFAULT_GPIO_CONFIG()

```

#### ULP_RISCV_I2C_FAST_MODE_CONFIG


```


ULP_RISCV_I2C_FAST_MODE_CONFIG()

```

#### ULP_RISCV_I2C_STANDARD_MODE_CONFIG


```


ULP_RISCV_I2C_STANDARD_MODE_CONFIG()

```

#### ULP_RISCV_I2C_DEFAULT_CONFIG


```


ULP_RISCV_I2C_DEFAULT_CONFIG()

```