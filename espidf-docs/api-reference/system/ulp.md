# ULP Coprocessor Programming



The Ultra Low Power (ULP) coprocessor is a simple finite state machine (FSM) which is designed to perform measurements using the ADC, temperature sensor, and external I2C sensors, while the main processors are in Deep-sleep mode. The ULP coprocessor can access the `RTC_SLOW_MEM` memory region, and registers in the `RTC_CNTL`, `RTC_IO`, and `SARADC` peripherals. The ULP coprocessor uses fixed-width 32-bit instructions, 32-bit memory addressing, and has 4 general-purpose 16-bit registers. This coprocessor is referred to as `ULP FSM` in ESP-IDF.


ESP32-S3 provides a second type of ULP coprocessor which is based on a RISC-V instruction set architecture. For details regarding ULP RISC-V refer ULP-RISC-V Coprocessor.



## Installing the Toolchain


The ULP FSM coprocessor code is written in assembly and compiled using the binutils-esp32ulp toolchain.


If you have already set up ESP-IDF with CMake build system according to the Getting Started Guide, then the ULP FSM toolchain will already be installed.




## Programming ULP FSM


The ULP FSM can be programmed using the supported instruction set. Alternatively, the ULP FSM coprocessor can also be programmed using C Macros on the main CPU. Theses two methods are described in the following section:



* Instruction set reference for ESP32-S3 ULP
* Programming using macros (legacy)





## Compiling the ULP Code


To compile the ULP FSM code as part of the component, the following steps must be taken:


1. The ULP FSM code, written in assembly, must be added to one or more files with `.S` extension. These files must be placed into a separate directory inside the component directory, for instance, `ulp/`.



Note


When registering the component (via `idf_component_register`), this directory should not be added to the `SRC_DIRS` argument. The logic behind this is that the ESP-IDF build system will compile files found in `SRC_DIRS` based on their extensions. For `.S` files, `xtensa-esp32s3-elf-as` assembler is used. This is not desirable for ULP FSM assembly files, so the easiest way to achieve the distinction is by placing ULP FSM assembly files into a separate directory. The ULP FSM assembly source files should also **not** be added to `SRCS` for the same reason. See the steps below for how to properly add ULP FSM assembly source files.



2. Call `ulp_embed_binary` from the component CMakeLists.txt after registration. For example:



```
...
idf_component_register()

set(ulp_app_name ulp_${COMPONENT_NAME})
set(ulp_s_sources ulp/ulp_assembly_source_file.S)
set(ulp_exp_dep_srcs "ulp_c_source_file.c")

ulp_embed_binary(${ulp_app_name} "${ulp_s_sources}" "${ulp_exp_dep_srcs}")

```


The first argument to `ulp_embed_binary` specifies the ULP FSM binary name. The name specified here will also be used by other generated artifacts such as the ELF file, map file, header file and linker export file. The second argument specifies the ULP FSM assembly source files. Finally, the third argument specifies the list of component source files which include the header file to be generated. This list is needed to build the dependencies correctly and ensure that the generated header file will be created before any of these files are compiled. See the section below for the concept of generated header files for ULP applications.


3. Build the application as usual (e.g., `idf.py app`).


Inside, the build system will take the following steps to build ULP FSM program:


	1. **Run each assembly file (foo.S) through the C preprocessor.** This step generates the preprocessed assembly files (foo.ulp.S) in the component build directory. This step also generates dependency files (foo.ulp.d).
	2. **Run preprocessed assembly sources through the assembler.** This produces object (foo.ulp.o) and listing (foo.ulp.lst) files. Listing files are generated for debugging purposes and are not used at later stages of the build process.
	3. **Run the linker script template through the C preprocessor.** The template is located in `components/ulp/ld` directory.
	4. **Link the object files into an output ELF file** (`ulp_app_name.elf`). The Map file (`ulp_app_name.map`) generated at this stage may be useful for debugging purposes.
	5. **Dump the contents of the ELF file into a binary** (`ulp_app_name.bin`) which can then be embedded into the application.
	6. **Generate a list of global symbols** (`ulp_app_name.sym`) in the ELF file using `esp32ulp-elf-nm`.
	7. **Create an LD export script and a header file** (`ulp_app_name.ld` and `ulp_app_name.h`) containing the symbols from `ulp_app_name.sym`. This is done using the `esp32ulp_mapgen.py` utility.
	8. **Add the generated binary to the list of binary files** to be embedded into the application.




## Accessing the ULP FSM Program Variables


Global symbols defined in the ULP FSM program may be used inside the main program.


For example, the ULP FSM program may define a variable `measurement_count` which will define the number of ADC measurements the program needs to make before waking up the chip from Deep-sleep:



```
                        .global measurement_count
measurement_count:      .long 0

                        // later, use measurement_count
                        move r3, measurement_count
                        ld r3, r3, 0

```


The main program needs to initialize this variable before the ULP program is started. The build system makes this possible by generating the `${ULP_APP_NAME}.h` and `${ULP_APP_NAME}.ld` files which define the global symbols present in the ULP program. Each global symbol defined in the ULP program is included in these files and are prefixed with `ulp_`.


The header file contains the declaration of the symbol:



```
extern uint32_t ulp_measurement_count;

```


Note that all symbols (variables, arrays, functions) are declared as `uint32_t`. For functions and arrays, take the address of the symbol and cast it to the appropriate type.


The generated linker script file defines the locations of symbols in RTC_SLOW_MEM:



```
PROVIDE ( ulp_measurement_count = 0x50000060 );

```


To access the ULP program variables from the main program, the generated header file should be included using an `include` statement. This will allow the ULP program variables to be accessed as regular variables:



```
#include "ulp_app_name.h"

// later
void init_ulp_vars() {
    ulp_measurement_count = 64;
}

```




## Starting the ULP FSM Program


To run a ULP FSM program, the main application needs to load the ULP program into RTC memory using the `ulp_load_binary()` function, and then start it using the `ulp_run()` function.


Note that the `Enable Ultra Low Power (ULP) Coprocessor` option must be enabled in menuconfig to work with ULP. To select the type of ULP to be used, the `ULP Co-processor type` option must be set. To reserve memory for the ULP, the `RTC slow memory reserved for coprocessor` option must be set to a value big enough to store ULP code and data. If the application components contain multiple ULP programs, then the size of the RTC memory must be sufficient to hold the largest one.


Each ULP program is embedded into the ESP-IDF application as a binary blob. The application can reference this blob and load it in the following way (suppose ULP_APP_NAME was defined to `ulp_app_name`):



```
extern const uint8_t bin_start[] asm("_binary_ulp_app_name_bin_start");
extern const uint8_t bin_end[]   asm("_binary_ulp_app_name_bin_end");

void start_ulp_program() {
    ESP_ERROR_CHECK( ulp_load_binary(
        0 // load address, set to 0 when using default linker scripts
        bin_start,
        (bin_end - bin_start) / sizeof(uint32_t)) );
}

```


Once the program is loaded into RTC memory, the application can start it by passing the address of the entry point to the `ulp_run` function:



```
ESP_ERROR_CHECK( ulp_run(&ulp_entry - RTC_SLOW_MEM) );

```


Declaration of the entry point symbol comes from the generated header file mentioned above, `${ULP_APP_NAME}.h`. In the assembly source of the ULP FSM application, this symbol must be marked as `.global`:



```
        .global entry
entry:
        // code starts here

```




## ESP32-S3 ULP Program Flow


ESP32-S3 ULP coprocessor is started by a timer. The timer is started once `ulp_run()` is called. The timer counts a number of RTC_SLOW_CLK ticks (by default, produced by an internal 90 kHz RC oscillator). The number of ticks is set using `RTC_CNTL_ULP_CP_TIMER_1_REG` register.


The application can set ULP timer period values by `ulp_set_wakeup_period()` function.


Once the timer counts the number of ticks set in the selected `RTC_CNTL_ULP_CP_TIMER_1_REG` register, ULP coprocessor powers up and starts running the program from the entry point set in the call to `ulp_run()`.


The program runs until it encounters a `halt` instruction or an illegal instruction. Once the program halts, ULP coprocessor powers down, and the timer is started again.


To disable the timer (effectively preventing the ULP program from running again), clear the `RTC_CNTL_ULP_CP_SLP_TIMER_EN` bit in the `RTC_CNTL_ULP_CP_TIMER_REG` register. This can be done both from ULP code and from the main program.




## Application Examples


* ULP FSM Coprocessor counts pulses on an IO while main CPU is in Deep-sleep: system/ulp/ulp_fsm/ulp.
* ULP FSM Coprocessor polls ADC in while main CPU is in Deep-sleep: system/ulp/ulp_fsm/ulp_adc.




## API Reference



### Header File


* components/ulp/ulp_fsm/include/ulp_fsm_common.h
* This header file can be included with:



> ```
> #include "ulp_fsm_common.h"
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


#### ulp_isr_register


```


esp_err_t ulp_isr_register(intr_handler_t fn, void *arg)
Register ULP wakeup signal ISR. 

Note
The ISR routine will only be active if the main CPU is not in deepsleep


Parameters

fn -- ISR callback function 
arg -- ISR callback function arguments 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if callback function is NULL
ESP_ERR_NO_MEM if heap memory cannot be allocated for the interrupt 





```

#### ulp_isr_deregister


```


esp_err_t ulp_isr_deregister(intr_handler_t fn, void *arg)
Deregister ULP wakeup signal ISR. 

Parameters

fn -- ISR callback function 
arg -- ISR callback function arguments 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if callback function is NULL
ESP_ERR_INVALID_STATE if a handler matching both callback function and its arguments isn't registered 





```

#### ulp_process_macros_and_load


```


esp_err_t ulp_process_macros_and_load(uint32_t load_addr, const ulp_insn_t *program, size_t *psize)
Resolve all macro references in a program and load it into RTC memory. 

Parameters

load_addr -- address where the program should be loaded, expressed in 32-bit words 
program -- ulp_insn_t array with the program 
psize -- size of the program, expressed in 32-bit words 


Returns

ESP_OK on success
ESP_ERR_NO_MEM if auxiliary temporary structure can not be allocated
one of ESP_ERR_ULP_xxx if program is not valid or can not be loaded 





```

#### ulp_load_binary


```


esp_err_t ulp_load_binary(uint32_t load_addr, const uint8_t *program_binary, size_t program_size)
Load ULP program binary into RTC memory. 
ULP program binary should have the following format (all values little-endian):

MAGIC, (value 0x00706c75, 4 bytes)
TEXT_OFFSET, offset of .text section from binary start (2 bytes)
TEXT_SIZE, size of .text section (2 bytes)
DATA_SIZE, size of .data section (2 bytes)
BSS_SIZE, size of .bss section (2 bytes)
(TEXT_OFFSET - 12) bytes of arbitrary data (will not be loaded into RTC memory)
.text section
.data section


Linker script in components/ulp/ld/esp32.ulp.ld produces ELF files which correspond to this format. This linker script produces binaries with load_addr == 0.

Parameters

load_addr -- address where the program should be loaded, expressed in 32-bit words 
program_binary -- pointer to program binary 
program_size -- size of the program binary 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if load_addr is out of range
ESP_ERR_INVALID_SIZE if program_size doesn't match (TEXT_OFFSET + TEXT_SIZE + DATA_SIZE)
ESP_ERR_NOT_SUPPORTED if the magic number is incorrect 





```

#### ulp_run


```


esp_err_t ulp_run(uint32_t entry_point)
Run the program loaded into RTC memory. 

Parameters
entry_point -- entry point, expressed in 32-bit words 

Returns
ESP_OK on success 



```



### Macros


#### ESP_ERR_ULP_BASE


```


ESP_ERR_ULP_BASE
Offset for ULP-related error codes 

```

#### ESP_ERR_ULP_SIZE_TOO_BIG


```


ESP_ERR_ULP_SIZE_TOO_BIG
Program doesn't fit into RTC memory reserved for the ULP 

```

#### ESP_ERR_ULP_INVALID_LOAD_ADDR


```


ESP_ERR_ULP_INVALID_LOAD_ADDR
Load address is outside of RTC memory reserved for the ULP 

```

#### ESP_ERR_ULP_DUPLICATE_LABEL


```


ESP_ERR_ULP_DUPLICATE_LABEL
More than one label with the same number was defined 

```

#### ESP_ERR_ULP_UNDEFINED_LABEL


```


ESP_ERR_ULP_UNDEFINED_LABEL
Branch instructions references an undefined label 

```

#### ESP_ERR_ULP_BRANCH_OUT_OF_RANGE


```


ESP_ERR_ULP_BRANCH_OUT_OF_RANGE
Branch target is out of range of B instruction (try replacing with BX) 

```



### Type Definitions


#### ulp_insn_t


```


typedef union ulp_insn ulp_insn_t

```



### Header File


* components/ulp/ulp_common/include/ulp_common.h
* This header file can be included with:



> ```
> #include "ulp_common.h"
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


#### ulp_set_wakeup_period


```


esp_err_t ulp_set_wakeup_period(size_t period_index, uint32_t period_us)
Set one of ULP wakeup period values. 
ULP coprocessor starts running the program when the wakeup timer counts up to a given value (called period). There are 5 period values which can be programmed into SENS_ULP_CP_SLEEP_CYCx_REG registers, x = 0..4 for ESP32, and one period value which can be programmed into RTC_CNTL_ULP_CP_TIMER_1_REG register for ESP32-S2/S3. By default, for ESP32, wakeup timer will use the period set into SENS_ULP_CP_SLEEP_CYC0_REG, i.e. period number 0. ULP program code can use SLEEP instruction to select which of the SENS_ULP_CP_SLEEP_CYCx_REG should be used for subsequent wakeups.
However, please note that SLEEP instruction issued (from ULP program) while the system is in deep sleep mode does not have effect, and sleep cycle count 0 is used.
For ESP32-S2/S3 the SLEEP instruction not exist. Instead a WAKE instruction will be used.

Note
The ULP FSM requires two clock cycles to wakeup before being able to run the program. Then additional 16 cycles are reserved after wakeup waiting until the 8M clock is stable. The FSM also requires two more clock cycles to go to sleep after the program execution is halted. The minimum wakeup period that may be set up for the ULP is equal to the total number of cycles spent on the above internal tasks. For a default configuration of the ULP running at 150kHz it makes about 133us. 


Parameters

period_index -- wakeup period setting number (0 - 4) 
period_us -- wakeup period, us 


Returns

ESP_OK on success
ESP_ERR_INVALID_ARG if period_index is out of range 





```

#### ulp_timer_stop


```


void ulp_timer_stop(void)
Stop the ULP timer. 

Note
This will stop the ULP from waking up if halted, but will not abort any program currently executing on the ULP. 


```

#### ulp_timer_resume


```


void ulp_timer_resume(void)
Resume the ULP timer. 

Note
This will resume an already configured timer, but does no other configuration 


```