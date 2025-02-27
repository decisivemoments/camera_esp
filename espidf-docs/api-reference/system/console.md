# Console



ESP-IDF provides `console` component, which includes building blocks needed to develop an interactive console over serial port. This component includes the following features:


* Line editing, provided by linenoise library. This includes handling of backspace and arrow keys, scrolling through command history, command auto-completion, and argument hints.
* Splitting of command line into arguments.
* Argument parsing, provided by argtable3 library. This library includes APIs used for parsing GNU style command line arguments.
* Functions for registration and dispatching of commands.
* Functions to establish a basic REPL (Read-Evaluate-Print-Loop) environment.



Note


These features can be used together or independently. For example, it is possible to use line editing and command registration features, but use `getopt` or custom code for argument parsing, instead of argtable3. Likewise, it is possible to use simpler means of command input (such as `fgets`) together with the rest of the means for command splitting and argument parsing.




Note


When using a console application on a chip that supports a hardware USB serial interface, we suggest to disable the secondary serial console output. The secondary output will be output-only and consequently does not make sense in an interactive application.




## Line Editing


Line editing feature lets users compose commands by typing them, erasing symbols using the `backspace` key, navigating within the command using the left/right keys, navigating to previously typed commands using the up/down keys, and performing autocompletion using the `tab` key.



Note


This feature relies on ANSI escape sequence support in the terminal application. As such, serial monitors which display raw UART data can not be used together with the line editing library. If you see `6n` or similar escape sequence when running [system/console example instead of a command prompt (e.g., `esp>` ), it means that the serial monitor does not support escape sequences. Programs which are known to work are GNU screen, minicom, and esp-idf-monitor (which can be invoked using `idf.py monitor` from project directory).



Here is an overview of functions provided by linenoise library.



### Configuration


Linenoise library does not need explicit initialization. However, some configuration defaults may need to be changed before invoking the main line editing function.


* `linenoiseClearScreen()`


Clear terminal screen using an escape sequence and position the cursor at the top left corner.
* `linenoiseSetMultiLine()`


Switch between single line and multi line editing modes. In single line mode, if the length of the command exceeds the width of the terminal, the command text is scrolled within the line to show the end of the text. In this case the beginning of the text is hidden. Single line mode needs less data to be sent to refresh screen on each key press, so exhibits less glitching compared to the multi line mode. On the flip side, editing commands and copying command text from terminal in single line mode is harder. Default is single line mode.
* `linenoiseAllowEmpty()`


Set whether linenoise library returns a zero-length string (if `true`) or `NULL` (if `false`) for empty lines. By default, zero-length strings are returned.
* `linenoiseSetMaxLineLen()`


Set maximum length of the line for linenoise library. Default length is 4096 bytes. The default value can be updated to optimize RAM memory usage.




### Main Loop


* `linenoise()`


In most cases, console applications have some form of read/eval loop. `linenoise()` is the single function which handles user's key presses and returns the completed line once the `enter` key is pressed. As such, it handles the `read` part of the loop.
* `linenoiseFree()`


This function must be called to release the command line buffer obtained from `linenoise()` function.




### Hints and Completions


* `linenoiseSetCompletionCallback()`


When the user presses the `tab` key, linenoise library invokes the completion callback. The callback should inspect the contents of the command typed so far and provide a list of possible completions using calls to `linenoiseAddCompletion()` function. `linenoiseSetCompletionCallback()` function should be called to register this completion callback, if completion feature is desired.


`console` component provides a ready made function to provide completions for registered commands, `esp_console_get_completion()` (see below).
* `linenoiseAddCompletion()`


Function to be called by completion callback to inform the library about possible completions of the currently typed command.
* `linenoiseSetHintsCallback()`


Whenever user input changes, linenoise invokes the hints callback. This callback can inspect the command line typed so far, and provide a string with hints (which can include list of command arguments, for example). The library then displays the hint text on the same line where editing happens, possibly with a different color.
* `linenoiseSetFreeHintsCallback()`


If the hint string returned by hints callback is dynamically allocated or needs to be otherwise recycled, the function which performs such cleanup should be registered via `linenoiseSetFreeHintsCallback()`.




### History


* `linenoiseHistorySetMaxLen()`


This function sets the number of most recently typed commands to be kept in memory. Users can navigate the history using the up/down arrows keys.
* `linenoiseHistoryAdd()`


Linenoise does not automatically add commands to history. Instead, applications need to call this function to add command strings to the history.
* `linenoiseHistorySave()`


Function saves command history from RAM to a text file, for example on an SD card or on a filesystem in flash memory.
* `linenoiseHistoryLoad()`


Counterpart to `linenoiseHistorySave()`, loads history from a file.
* `linenoiseHistoryFree()`


Releases memory used to store command history. Call this function when done working with linenoise library.





## Splitting of Command Line into Arguments


`console` component provides `esp_console_split_argv()` function to split command line string into arguments. The function returns the number of arguments found (`argc`) and fills an array of pointers which can be passed as `argv` argument to any function which accepts arguments in `argc, argv` format.


The command line is split into arguments according to the following rules:


* Arguments are separated by spaces
* If spaces within arguments are required, they can be escaped using `\` (backslash) character.
* Other escape sequences which are recognized are `\\` (which produces literal backslash) and `\"`, which produces a double quote.
* Arguments can be quoted using double quotes. Quotes may appear only in the beginning and at the end of the argument. Quotes within the argument must be escaped as mentioned above. Quotes surrounding the argument are stripped by `esp_console_split_argv` function.


Examples:


* `abc def 1 20 .3` > [ `abc`, `def`, `1`, `20`, `.3` ]
* `abc "123 456" def` > [ `abc`, `123 456`, `def` ]
* ``a\ b\\c\"` > [ `a b\c"` ]




## Argument Parsing


For argument parsing, `console` component includes argtable3 library. Please see tutorial for an introduction to argtable3. Github repository also includes examples.




## Command Registration and Dispatching


`console` component includes utility functions which handle registration of commands, matching commands typed by the user to registered ones, and calling these commands with the arguments given on the command line.


Application first initializes command registration module using a call to `esp_console_init()`, and calls `esp_console_cmd_register()` function to register command handlers.


For each command, application provides the following information (in the form of `esp_console_cmd_t` structure):


* Command name (string without spaces)
* Help text explaining what the command does
* Optional hint text listing the arguments of the command. If application uses Argtable3 for argument parsing, hint text can be generated automatically by providing a pointer to argtable argument definitions structure instead.
* Command handler function (without context), or
* Command handler function (with context). If this function is given, an additional call to `esp_console_cmd_set_context()` must follow *before* the command may be called to initialize the context.



Note


You can either use a command handler function which takes a context or a command handler function which does not take a context, not both. If you use the command handler function which takes a context, you MUST call `esp_console_cmd_set_context()` to initialize its context, otherwise the function may access the uninitialized context.



A few other functions are provided by the command registration module:


* `esp_console_run()`


This function takes the command line string, splits it into argc/argv argument list using `esp_console_split_argv()`, looks up the command in the list of registered components, and if it is found, executes its handler.
* `esp_console_register_help_command()`


Adds `help` command to the list of registered commands. This command prints the list of all the registered commands, along with their arguments and help texts.
* `esp_console_get_completion()`


Callback function to be used with `linenoiseSetCompletionCallback()` from linenoise library. Provides completions to linenoise based on the list of registered commands.
* `esp_console_get_hint()`


Callback function to be used with `linenoiseSetHintsCallback()` from linenoise library. Provides argument hints for registered commands to linenoise.




## Initialize Console REPL Environment


To establish a basic REPL environment, `console` component provides several useful APIs, combining those functions described above.


In a typical application, you only need to call `esp_console_new_repl_uart()` to initialize the REPL environment based on UART device, including driver install, basic console configuration, spawning a thread to do REPL task and register several useful commands (e.g., help).


After that, you can register your own commands with `esp_console_cmd_register()`. The REPL environment keeps in init state until you call `esp_console_start_repl()`.


Likewise, if your REPL environment is based on USB_SERIAL_JTAG device, you only need to call `esp_console_new_repl_usb_serial_jtag()` at first step. Then call other functions as usual.




## Application Example


Example application illustrating usage of the `console` component is available in system/console directory. This example shows how to initialize UART and VFS functions, set up linenoise library, read and handle commands from UART, and store command history in Flash. See README.md in the example directory for more details.


Besides that, ESP-IDF contains several useful examples which are based on the `console` component and can be treated as "tools" when developing applications. For example, peripherals/i2c/i2c_tools, wifi/iperf.




## API Reference



### Header File


* components/console/esp_console.h
* This header file can be included with:



> ```
> #include "esp_console.h"
> 
> ```
* This header file is a part of the API provided by the `console` component. To declare that your component depends on `console`, add the following to your CMakeLists.txt:



> ```
> REQUIRES console
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES console
> 
> ```




### Functions


#### esp_console_init


```


esp_err_t esp_console_init(const esp_console_config_t *config)
initialize console module 

Note
Call this once before using other console module features 


Parameters
config -- console configuration 

Returns

ESP_OK on success
ESP_ERR_NO_MEM if out of memory
ESP_ERR_INVALID_STATE if already initialized
ESP_ERR_INVALID_ARG if the configuration is invalid 





```

#### esp_console_deinit


```


esp_err_t esp_console_deinit(void)
de-initialize console module 

Note
Call this once when done using console module functions 


Returns

ESP_OK on success
ESP_ERR_INVALID_STATE if not initialized yet 





```

#### esp_console_cmd_register


```


esp_err_t esp_console_cmd_register(const esp_console_cmd_t *cmd)
Register console command. 

Note
If the member func_w_context of cmd is set instead of func, then the member context is passed to the function pointed to by func_w_context.


Parameters
cmd -- pointer to the command description; can point to a temporary value

Returns

ESP_OK on success
ESP_ERR_NO_MEM if out of memory
ESP_ERR_INVALID_ARG if command description includes invalid arguments
ESP_ERR_INVALID_ARG if both func and func_w_context members of cmd are non-NULL
ESP_ERR_INVALID_ARG if both func and func_w_context members of cmd are NULL 





```

#### esp_console_run


```


esp_err_t esp_console_run(const char *cmdline, int *cmd_ret)
Run command line. 

Parameters

cmdline -- command line (command name followed by a number of arguments) 
cmd_ret -- [out] return code from the command (set if command was run) 


Returns

ESP_OK, if command was run
ESP_ERR_INVALID_ARG, if the command line is empty, or only contained whitespace
ESP_ERR_NOT_FOUND, if command with given name wasn't registered
ESP_ERR_INVALID_STATE, if esp_console_init wasn't called 





```

#### esp_console_split_argv


```


size_t esp_console_split_argv(char *line, char **argv, size_t argv_size)
Split command line into arguments in place. 
* - This function finds whitespace-separated arguments in the given input line.
*
*     'abc def 1 20 .3' -> [ 'abc', 'def', '1', '20', '.3' ]
*
* - Argument which include spaces may be surrounded with quotes. In this case
*   spaces are preserved and quotes are stripped.
*
*     'abc "123 456" def' -> [ 'abc', '123 456', 'def' ]
*
* - Escape sequences may be used to produce backslash, double quote, and space:
*
*     'a\ b\\c\"' -> [ 'a b\c"' ]
*




Note
Pointers to at most argv_size - 1 arguments are returned in argv array. The pointer after the last one (i.e. argv[argc]) is set to NULL.


Parameters

line -- pointer to buffer to parse; it is modified in place 
argv -- array where the pointers to arguments are written 
argv_size -- number of elements in argv_array (max. number of arguments) 


Returns
number of arguments found (argc) 



```

#### esp_console_get_completion


```


void esp_console_get_completion(const char *buf, linenoiseCompletions *lc)
Callback which provides command completion for linenoise library. 
When using linenoise for line editing, command completion support can be enabled like this:
linenoiseSetCompletionCallback(&esp_console_get_completion);

Parameters

buf -- the string typed by the user 
lc -- linenoiseCompletions to be filled in 




```

#### esp_console_get_hint


```


const char *esp_console_get_hint(const char *buf, int *color, int *bold)
Callback which provides command hints for linenoise library. 
When using linenoise for line editing, hints support can be enabled as follows:
linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);
The extra cast is needed because linenoiseHintsCallback is defined as returning a char* instead of const char*.

Parameters

buf -- line typed by the user 
color -- [out] ANSI color code to be used when displaying the hint 
bold -- [out] set to 1 if hint has to be displayed in bold 


Returns
string containing the hint text. This string is persistent and should not be freed (i.e. linenoiseSetFreeHintsCallback should not be used). 



```

#### esp_console_register_help_command


```


esp_err_t esp_console_register_help_command(void)
Register a 'help' command. 
Default 'help' command prints the list of registered commands along with hints and help strings if no additional argument is given. If an additional argument is given, the help command will look for a command with the same name and only print the hints and help strings of that command.

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE, if esp_console_init wasn't called 





```

#### esp_console_new_repl_uart


```


esp_err_t esp_console_new_repl_uart(const esp_console_dev_uart_config_t *dev_config, const esp_console_repl_config_t *repl_config, esp_console_repl_t **ret_repl)
Establish a console REPL environment over UART driver. 

AttentionThis function is meant to be used in the examples to make the code more compact. Applications which use console functionality should be based on the underlying linenoise and esp_console functions.




Note
This is an all-in-one function to establish the environment needed for REPL, includes:
Install the UART driver on the console UART (8n1, 115200, REF_TICK clock source)
Configures the stdin/stdout to go through the UART driver
Initializes linenoise
Spawn new thread to run REPL in the background




Parameters

dev_config -- [in] UART device configuration 
repl_config -- [in] REPL configuration 
ret_repl -- [out] return REPL handle after initialization succeed, return NULL otherwise


Returns

ESP_OK on success
ESP_FAIL Parameter error 





```

#### esp_console_new_repl_usb_cdc


```


esp_err_t esp_console_new_repl_usb_cdc(const esp_console_dev_usb_cdc_config_t *dev_config, const esp_console_repl_config_t *repl_config, esp_console_repl_t **ret_repl)
Establish a console REPL environment over USB CDC. 

AttentionThis function is meant to be used in the examples to make the code more compact. Applications which use console functionality should be based on the underlying linenoise and esp_console functions.




Note
This is a all-in-one function to establish the environment needed for REPL, includes:
Initializes linenoise
Spawn new thread to run REPL in the background




Parameters

dev_config -- [in] USB CDC configuration 
repl_config -- [in] REPL configuration 
ret_repl -- [out] return REPL handle after initialization succeed, return NULL otherwise


Returns

ESP_OK on success
ESP_FAIL Parameter error 





```

#### esp_console_new_repl_usb_serial_jtag


```


esp_err_t esp_console_new_repl_usb_serial_jtag(const esp_console_dev_usb_serial_jtag_config_t *dev_config, const esp_console_repl_config_t *repl_config, esp_console_repl_t **ret_repl)
Establish a console REPL (Read-eval-print loop) environment over USB-SERIAL-JTAG. 

AttentionThis function is meant to be used in the examples to make the code more compact. Applications which use console functionality should be based on the underlying linenoise and esp_console functions.




Note
This is an all-in-one function to establish the environment needed for REPL, includes:
Initializes linenoise
Spawn new thread to run REPL in the background




Parameters

dev_config -- [in] USB-SERIAL-JTAG configuration 
repl_config -- [in] REPL configuration 
ret_repl -- [out] return REPL handle after initialization succeed, return NULL otherwise


Returns

ESP_OK on success
ESP_FAIL Parameter error 





```

#### esp_console_start_repl


```


esp_err_t esp_console_start_repl(esp_console_repl_t *repl)
Start REPL environment. 

Note
Once the REPL gets started, it won't be stopped until the user calls repl->del(repl) to destroy the REPL environment. 


Parameters
repl -- [in] REPL handle returned from esp_console_new_repl_xxx 

Returns

ESP_OK on success
ESP_ERR_INVALID_STATE, if repl has started already 





```



### Structures


#### esp_console_config_t


```


struct esp_console_config_t
Parameters for console initialization. 

Public Members
max_cmdline_length

size_t max_cmdline_length
length of command line buffer, in bytes 

max_cmdline_args

size_t max_cmdline_args
maximum number of command line arguments to parse 

heap_alloc_caps

uint32_t heap_alloc_caps
where to (e.g. MALLOC_CAP_SPIRAM) allocate heap objects such as cmds used by esp_console 

hint_color

int hint_color
ASCII color code of hint text. 

hint_bold

int hint_bold
Set to 1 to print hint text in bold. 



```

#### esp_console_repl_config_t


```


struct esp_console_repl_config_t
Parameters for console REPL (Read Eval Print Loop) 

Public Members
max_history_len

uint32_t max_history_len
maximum length for the history 

history_save_path

const char *history_save_path
file path used to save history commands, set to NULL won't save to file system 

task_stack_size

uint32_t task_stack_size
repl task stack size 

task_priority

uint32_t task_priority
repl task priority 

task_core_id

BaseType_t task_core_id
repl task affinity, i.e. which core the task is pinned to 

prompt

const char *prompt
prompt (NULL represents default: "esp> ") 

max_cmdline_length

size_t max_cmdline_length
maximum length of a command line. If 0, default value will be used 



```

#### esp_console_dev_uart_config_t


```


struct esp_console_dev_uart_config_t
Parameters for console device: UART. 

Public Members
channel

int channel
UART channel number (count from zero) 

baud_rate

int baud_rate
Comunication baud rate. 

tx_gpio_num

int tx_gpio_num
GPIO number for TX path, -1 means using default one. 

rx_gpio_num

int rx_gpio_num
GPIO number for RX path, -1 means using default one. 



```

#### esp_console_dev_usb_cdc_config_t


```


struct esp_console_dev_usb_cdc_config_t
Parameters for console device: USB CDC. 

Note
It's an empty structure for now, reserved for future 


```

#### esp_console_dev_usb_serial_jtag_config_t


```


struct esp_console_dev_usb_serial_jtag_config_t
Parameters for console device: USB-SERIAL-JTAG. 

Note
It's an empty structure for now, reserved for future 


```

#### esp_console_cmd_t


```


struct esp_console_cmd_t
Console command description. 

Public Members
command

const char *command
Command name. Must not be NULL, must not contain spaces. The pointer must be valid until the call to esp_console_deinit. 

help

const char *help
Help text for the command, shown by help command. If set, the pointer must be valid until the call to esp_console_deinit. If not set, the command will not be listed in 'help' output. 

hint

const char *hint
Hint text, usually lists possible arguments. If set to NULL, and 'argtable' field is non-NULL, hint will be generated automatically 

func

esp_console_cmd_func_t func
Pointer to a function which implements the command. 

Note
: Setting both func and func_w_context is not allowed. 


argtable

void *argtable
Array or structure of pointers to arg_xxx structures, may be NULL. Used to generate hint text if 'hint' is set to NULL. Array/structure which this field points to must end with an arg_end. Only used for the duration of esp_console_cmd_register call. 

func_w_context

esp_console_cmd_func_with_context_t func_w_context
Pointer to a context aware function which implements the command. 

Note
: Setting both func and func_w_context is not allowed. 


context

void *context
Context pointer to user-defined per-command context data. This is used if context aware function func_w_context is set. 



```

#### esp_console_repl_s


```


struct esp_console_repl_s
Console REPL base structure. 

Public Members
del

esp_err_t (*del)(esp_console_repl_t *repl)
Delete console REPL environment. 

Param repl
[in] REPL handle returned from esp_console_new_repl_xxx 

Return

ESP_OK on success
ESP_FAIL on errors 







```



### Macros


#### ESP_CONSOLE_CONFIG_DEFAULT


```


ESP_CONSOLE_CONFIG_DEFAULT()
Default console configuration value. 

```

#### ESP_CONSOLE_REPL_CONFIG_DEFAULT


```


ESP_CONSOLE_REPL_CONFIG_DEFAULT()
Default console repl configuration value. 

```

#### ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT


```


ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT()

```

#### ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT


```


ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT()

```

#### ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT


```


ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT()

```



### Type Definitions


#### linenoiseCompletions


```


typedef struct linenoiseCompletions linenoiseCompletions

```

#### esp_console_cmd_func_t


```


typedef int (*esp_console_cmd_func_t)(int argc, char **argv)
Console command main function. 

Param argc
number of arguments 

Param argv
array with argc entries, each pointing to a zero-terminated string argument 

Return
console command return code, 0 indicates "success" 



```

#### esp_console_cmd_func_with_context_t


```


typedef int (*esp_console_cmd_func_with_context_t)(void *context, int argc, char **argv)
Console command main function, with context. 

Param context
a user context given at invocation 

Param argc
number of arguments 

Param argv
array with argc entries, each pointing to a zero-terminated string argument 

Return
console command return code, 0 indicates "success" 



```

#### esp_console_repl_t


```


typedef struct esp_console_repl_s esp_console_repl_t
Type defined for console REPL. 

```