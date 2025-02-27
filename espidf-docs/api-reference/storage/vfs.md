# Virtual Filesystem Component




## Overview


Virtual filesystem (VFS) component provides a unified interface for drivers which can perform operations on file-like objects. These can be real filesystems (FAT, SPIFFS, etc.) or device drivers which provide a file-like interface.


This component allows C library functions, such as fopen and fprintf, to work with FS drivers. At a high level, each FS driver is associated with some path prefix. When one of C library functions needs to open a file, the VFS component searches for the FS driver associated with the file path and forwards the call to that driver. VFS also forwards read, write, and other calls for the given file to the same FS driver.


For example, one can register a FAT filesystem driver with the `/fat` prefix and call `fopen("/fat/file.txt", "w")`. Then the VFS component calls the function `open` of the FAT driver and pass the argument `/file.txt` to it together with appropriate mode flags. All subsequent calls to C library functions for the returned `FILE*` stream will also be forwarded to the FAT driver.




## FS Registration


To register an FS driver, an application needs to define an instance of the `esp_vfs_t` structure and populate it with function pointers to FS APIs:



```
esp_vfs_t myfs = {
    .flags = ESP_VFS_FLAG_DEFAULT,
    .write = &myfs_write,
    .open = &myfs_open,
    .fstat = &myfs_fstat,
    .close = &myfs_close,
    .read = &myfs_read,
};

ESP_ERROR_CHECK(esp_vfs_register("/data", &myfs, NULL));

```


Depending on the way how the FS driver declares its API functions, either `read`, `write`, etc., or `read_p`, `write_p`, etc., should be used.


Case 1: API functions are declared without an extra context pointer (the FS driver is a singleton):



```
ssize_t myfs_write(int fd, const void * data, size_t size);

// In definition of esp_vfs_t:
    .flags = ESP_VFS_FLAG_DEFAULT,
    .write = &myfs_write,
// ... other members initialized

// When registering FS, context pointer (third argument) is NULL:
ESP_ERROR_CHECK(esp_vfs_register("/data", &myfs, NULL));

```


Case 2: API functions are declared with an extra context pointer (the FS driver supports multiple instances):



```
ssize_t myfs_write(myfs_t* fs, int fd, const void * data, size_t size);

// In definition of esp_vfs_t:
    .flags = ESP_VFS_FLAG_CONTEXT_PTR,
    .write_p = &myfs_write,
// ... other members initialized

// When registering FS, pass the FS context pointer into the third argument
// (hypothetical myfs_mount function is used for illustrative purposes)
myfs_t* myfs_inst1 = myfs_mount(partition1->offset, partition1->size);
ESP_ERROR_CHECK(esp_vfs_register("/data1", &myfs, myfs_inst1));

// Can register another instance:
myfs_t* myfs_inst2 = myfs_mount(partition2->offset, partition2->size);
ESP_ERROR_CHECK(esp_vfs_register("/data2", &myfs, myfs_inst2));

```



### Synchronous Input/Output Multiplexing


Synchronous input/output multiplexing by `select()` is supported in the VFS component. The implementation works in the following way.


1. `select()` is called with file descriptors which could belong to various VFS drivers.
2. The file descriptors are divided into groups each belonging to one VFS driver.
3. The file descriptors belonging to non-socket VFS drivers are handed over to the given VFS drivers by `start_select()`, described later on this page. This function represents the driver-specific implementation of `select()` for the given driver. This should be a non-blocking call which means the function should immediately return after setting up the environment for checking events related to the given file descriptors.
4. The file descriptors belonging to the socket VFS driver are handed over to the socket driver by `socket_select()` described later on this page. This is a blocking call which means that it will return only if there is an event related to socket file descriptors or a non-socket driver signals `socket_select()` to exit.
5. Results are collected from each VFS driver and all drivers are stopped by de-initialization of the environment for checking events.
6. The `select()` call ends and returns the appropriate results.



#### Non-Socket VFS Drivers


If you want to use `select()` with a file descriptor belonging to a non-socket VFS driver, then you need to register the driver with functions `start_select()` and `end_select()` similarly to the following example:



```
// In definition of esp_vfs_t:
    .start_select = &uart_start_select,
    .end_select = &uart_end_select,
// ... other members initialized

```


`start_select()` is called for setting up the environment for detection of read/write/error conditions on file descriptors belonging to the given VFS driver.


`end_select()` is called to stop/deinitialize/free the environment which was setup by `start_select()`.



Note


`end_select()` might be called without a previous `start_select()` call in some rare circumstances. `end_select()` should fail gracefully if this is the case (i.e., should not crash but return an error instead).



Please refer to the reference implementation for the UART peripheral in esp_driver_uart/src/uart_vfs.c and most particularly to the functions `uart_vfs_dev_register()`, `uart_start_select()`, and `uart_end_select()` for more information.


Please check the following examples that demonstrate the use of `select()` with VFS file descriptors:


* peripherals/uart/uart_select
* system/select




#### Socket VFS Drivers


A socket VFS driver is using its own internal implementation of `select()` and non-socket VFS drivers notify it upon read/write/error conditions.


A socket VFS driver needs to be registered with the following functions defined:



```
// In definition of esp_vfs_t:
    .socket_select = &lwip_select,
    .get_socket_select_semaphore = &lwip_get_socket_select_semaphore,
    .stop_socket_select = &lwip_stop_socket_select,
    .stop_socket_select_isr = &lwip_stop_socket_select_isr,
// ... other members initialized

```


`socket_select()` is the internal implementation of `select()` for the socket driver. It works only with file descriptors belonging to the socket VFS.


`get_socket_select_semaphore()` returns the signalization object (semaphore) which is used in non-socket drivers to stop the waiting in `socket_select()`.


`stop_socket_select()` call is used to stop the waiting in `socket_select()` by passing the object returned by `get_socket_select_semaphore()`.


`stop_socket_select_isr()` has the same functionality as `stop_socket_select()` but it can be used from ISR.


Please see lwip/port/esp32xx/vfs_lwip.c for a reference socket driver implementation using LWIP.



Note


If you use `select()` for socket file descriptors only then you can disable the CONFIG_VFS_SUPPORT_SELECT option to reduce the code size and improve performance.
You should not change the socket driver during an active `select()` call or you might experience some undefined behavior.







## Paths


Each registered FS has a path prefix associated with it. This prefix can be considered as a "mount point" of this partition.


In case when mount points are nested, the mount point with the longest matching path prefix is used when opening the file. For instance, suppose that the following filesystems are registered in VFS:


* FS 1 on /data
* FS 2 on /data/static


Then:


* FS 1 will be used when opening a file called `/data/log.txt`
* FS 2 will be used when opening a file called `/data/static/index.html`
* Even if `/index.html"` does not exist in FS 2, FS 1 will **not** be searched for `/static/index.html`.


As a general rule, mount point names must start with the path separator (`/`) and must contain at least one character after path separator. However, an empty mount point name is also supported and might be used in cases when an application needs to provide a "fallback" filesystem or to override VFS functionality altogether. Such filesystem will be used if no prefix matches the path given.


VFS does not handle dots (`.`) in path names in any special way. VFS does not treat `..` as a reference to the parent directory. In the above example, using a path `/data/static/../log.txt` will not result in a call to FS 1 to open `/log.txt`. Specific FS drivers (such as FATFS) might handle dots in file names differently.


When opening files, the FS driver receives only relative paths to files. For example:


1. The `myfs` driver is registered with `/data` as a path prefix.
2. The application calls `fopen("/data/config.json", ...)`.
3. The VFS component calls `myfs_open("/config.json", ...)`.
4. The `myfs` driver opens the `/config.json` file.


VFS does not impose any limit on total file path length, but it does limit the FS path prefix to `ESP_VFS_PATH_MAX` characters. Individual FS drivers may have their own filename length limitations.




## File Descriptors


File descriptors are small positive integers from `0` to `FD_SETSIZE - 1`, where `FD_SETSIZE` is defined in `sys/select.h`. The largest file descriptors (configured by `CONFIG_LWIP_MAX_SOCKETS`) are reserved for sockets. The VFS component contains a lookup-table called `s_fd_table` for mapping global file descriptors to VFS driver indexes registered in the `s_vfs` array.




## Standard IO Streams (`stdin`, `stdout`, `stderr`)


If the menuconfig option `UART for console output` is not set to `None`, then `stdin`, `stdout`, and `stderr` are configured to read from, and write to, a UART. It is possible to use UART0 or UART1 for standard IO. By default, UART0 is used with 115200 baud rate; TX pin is GPIO1; RX pin is GPIO3. These parameters can be changed in menuconfig.


Writing to `stdout` or `stderr` sends characters to the UART transmit FIFO. Reading from `stdin` retrieves characters from the UART receive FIFO.


By default, VFS uses simple functions for reading from and writing to UART. Writes busy-wait until all data is put into UART FIFO, and reads are non-blocking, returning only the data present in the FIFO. Due to this non-blocking read behavior, higher level C library calls, such as `fscanf("%d\n", &var);`, might not have desired results.


Applications which use the UART driver can instruct VFS to use the driver's interrupt driven, blocking read and write functions instead. This can be done using a call to the `uart_vfs_dev_use_driver()` function. It is also possible to revert to the basic non-blocking functions using a call to `uart_vfs_dev_use_nonblocking()`.


VFS also provides an optional newline conversion feature for input and output. Internally, most applications send and receive lines terminated by the LF (''n'') character. Different terminal programs may require different line termination, such as CR or CRLF. Applications can configure this separately for input and output either via menuconfig, or by calls to the functions `uart_vfs_dev_port_set_rx_line_endings()` and `uart_vfs_dev_port_set_tx_line_endings()`.



### Standard Streams and FreeRTOS Tasks


`FILE` objects for `stdin`, `stdout`, and `stderr` are shared between all FreeRTOS tasks, but the pointers to these objects are stored in per-task `struct _reent`.


The following code is transferred to `fprintf(__getreent()->_stderr, "42\n");` by the preprocessor:



```
fprintf(stderr, "42\n");

```


The `__getreent()` function returns a per-task pointer to `struct _reent` in newlib libc. This structure is allocated on the TCB of each task. When a task is initialized, `_stdin`, `_stdout`, and `_stderr` members of `struct _reent` are set to the values of `_stdin`, `_stdout`, and `_stderr` of `_GLOBAL_REENT` (i.e., the structure which is used before FreeRTOS is started).


Such a design has the following consequences:


* It is possible to set `stdin`, `stdout`, and `stderr` for any given task without affecting other tasks, e.g., by doing `stdin = fopen("/dev/uart/1", "r")`.
* Closing default `stdin`, `stdout`, or `stderr` using `fclose` closes the `FILE` stream object, which will affect all other tasks.
* To change the default `stdin`, `stdout`, `stderr` streams for new tasks, modify `_GLOBAL_REENT->_stdin` (`_stdout`, `_stderr`) before creating the task.





## `eventfd()`


`eventfd()` call is a powerful tool to notify a `select()` based loop of custom events. The `eventfd()` implementation in ESP-IDF is generally the same as described in man(2) eventfd except for:


* `esp_vfs_eventfd_register()` has to be called before calling `eventfd()`
* Options `EFD_CLOEXEC`, `EFD_NONBLOCK` and `EFD_SEMAPHORE` are not supported in flags.
* Option `EFD_SUPPORT_ISR` has been added in flags. This flag is required to read and write the eventfd in an interrupt handler.


Note that creating an eventfd with `EFD_SUPPORT_ISR` will cause interrupts to be temporarily disabled when reading, writing the file and during the beginning and the ending of the `select()` when this file is set.




## API Reference



### Header File


* components/vfs/include/esp_vfs.h
* This header file can be included with:



> ```
> #include "esp_vfs.h"
> 
> ```
* This header file is a part of the API provided by the `vfs` component. To declare that your component depends on `vfs`, add the following to your CMakeLists.txt:



> ```
> REQUIRES vfs
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES vfs
> 
> ```




### Functions


#### esp_vfs_write


```


ssize_t esp_vfs_write(struct _reent *r, int fd, const void *data, size_t size)
These functions are to be used in newlib syscall table. They will be called by newlib when it needs to use any of the syscalls. 

```

#### esp_vfs_lseek


```


off_t esp_vfs_lseek(struct _reent *r, int fd, off_t size, int mode)

```

#### esp_vfs_read


```


ssize_t esp_vfs_read(struct _reent *r, int fd, void *dst, size_t size)

```

#### esp_vfs_open


```


int esp_vfs_open(struct _reent *r, const char *path, int flags, int mode)

```

#### esp_vfs_close


```


int esp_vfs_close(struct _reent *r, int fd)

```

#### esp_vfs_fstat


```


int esp_vfs_fstat(struct _reent *r, int fd, struct stat *st)

```

#### esp_vfs_stat


```


int esp_vfs_stat(struct _reent *r, const char *path, struct stat *st)

```

#### esp_vfs_link


```


int esp_vfs_link(struct _reent *r, const char *n1, const char *n2)

```

#### esp_vfs_unlink


```


int esp_vfs_unlink(struct _reent *r, const char *path)

```

#### esp_vfs_rename


```


int esp_vfs_rename(struct _reent *r, const char *src, const char *dst)

```

#### esp_vfs_utime


```


int esp_vfs_utime(const char *path, const struct utimbuf *times)

```

#### esp_vfs_register


```


esp_err_t esp_vfs_register(const char *base_path, const esp_vfs_t *vfs, void *ctx)
Register a virtual filesystem for given path prefix.

Parameters

base_path -- file path prefix associated with the filesystem. Must be a zero-terminated C string, may be empty. If not empty, must be up to ESP_VFS_PATH_MAX characters long, and at least 2 characters long. Name must start with a "/" and must not end with "/". For example, "/data" or "/dev/spi" are valid. These VFSes would then be called to handle file paths such as "/data/myfile.txt" or "/dev/spi/0". In the special case of an empty base_path, a "fallback" VFS is registered. Such VFS will handle paths which are not matched by any other registered VFS. 
vfs -- Pointer to esp_vfs_t, a structure which maps syscalls to the filesystem driver functions. VFS component doesn't assume ownership of this pointer. 
ctx -- If vfs->flags has ESP_VFS_FLAG_CONTEXT_PTR set, a pointer which should be passed to VFS functions. Otherwise, NULL.


Returns
ESP_OK if successful, ESP_ERR_NO_MEM if too many VFSes are registered. 



```

#### esp_vfs_register_fd_range


```


esp_err_t esp_vfs_register_fd_range(const esp_vfs_t *vfs, void *ctx, int min_fd, int max_fd)
Special case function for registering a VFS that uses a method other than open() to open new file descriptors from the interval <min_fd; max_fd).
This is a special-purpose function intended for registering LWIP sockets to VFS.

Parameters

vfs -- Pointer to esp_vfs_t. Meaning is the same as for esp_vfs_register(). 
ctx -- Pointer to context structure. Meaning is the same as for esp_vfs_register(). 
min_fd -- The smallest file descriptor this VFS will use. 
max_fd -- Upper boundary for file descriptors this VFS will use (the biggest file descriptor plus one).


Returns
ESP_OK if successful, ESP_ERR_NO_MEM if too many VFSes are registered, ESP_ERR_INVALID_ARG if the file descriptor boundaries are incorrect. 



```

#### esp_vfs_register_with_id


```


esp_err_t esp_vfs_register_with_id(const esp_vfs_t *vfs, void *ctx, esp_vfs_id_t *vfs_id)
Special case function for registering a VFS that uses a method other than open() to open new file descriptors. In comparison with esp_vfs_register_fd_range, this function doesn't pre-registers an interval of file descriptors. File descriptors can be registered later, by using esp_vfs_register_fd.

Parameters

vfs -- Pointer to esp_vfs_t. Meaning is the same as for esp_vfs_register(). 
ctx -- Pointer to context structure. Meaning is the same as for esp_vfs_register(). 
vfs_id -- Here will be written the VFS ID which can be passed to esp_vfs_register_fd for registering file descriptors.


Returns
ESP_OK if successful, ESP_ERR_NO_MEM if too many VFSes are registered, ESP_ERR_INVALID_ARG if the file descriptor boundaries are incorrect. 



```

#### esp_vfs_unregister


```


esp_err_t esp_vfs_unregister(const char *base_path)
Unregister a virtual filesystem for given path prefix

Parameters
base_path -- file prefix previously used in esp_vfs_register call 

Returns
ESP_OK if successful, ESP_ERR_INVALID_STATE if VFS for given prefix hasn't been registered 



```

#### esp_vfs_unregister_with_id


```


esp_err_t esp_vfs_unregister_with_id(esp_vfs_id_t vfs_id)
Unregister a virtual filesystem with the given index

Parameters
vfs_id -- The VFS ID returned by esp_vfs_register_with_id 

Returns
ESP_OK if successful, ESP_ERR_INVALID_STATE if VFS for the given index hasn't been registered 



```

#### esp_vfs_register_fd


```


esp_err_t esp_vfs_register_fd(esp_vfs_id_t vfs_id, int *fd)
Special function for registering another file descriptor for a VFS registered by esp_vfs_register_with_id. This function should only be used to register permanent file descriptors (socket fd) that are not removed after being closed.

Parameters

vfs_id -- VFS identificator returned by esp_vfs_register_with_id. 
fd -- The registered file descriptor will be written to this address.


Returns
ESP_OK if the registration is successful, ESP_ERR_NO_MEM if too many file descriptors are registered, ESP_ERR_INVALID_ARG if the arguments are incorrect. 



```

#### esp_vfs_register_fd_with_local_fd


```


esp_err_t esp_vfs_register_fd_with_local_fd(esp_vfs_id_t vfs_id, int local_fd, bool permanent, int *fd)
Special function for registering another file descriptor with given local_fd for a VFS registered by esp_vfs_register_with_id.

Parameters

vfs_id -- VFS identificator returned by esp_vfs_register_with_id. 
local_fd -- The fd in the local vfs. Passing -1 will set the local fd as the (*fd) value. 
permanent -- Whether the fd should be treated as permannet (not removed after close()) 
fd -- The registered file descriptor will be written to this address.


Returns
ESP_OK if the registration is successful, ESP_ERR_NO_MEM if too many file descriptors are registered, ESP_ERR_INVALID_ARG if the arguments are incorrect. 



```

#### esp_vfs_unregister_fd


```


esp_err_t esp_vfs_unregister_fd(esp_vfs_id_t vfs_id, int fd)
Special function for unregistering a file descriptor belonging to a VFS registered by esp_vfs_register_with_id.

Parameters

vfs_id -- VFS identificator returned by esp_vfs_register_with_id. 
fd -- File descriptor which should be unregistered.


Returns
ESP_OK if the registration is successful, ESP_ERR_INVALID_ARG if the arguments are incorrect. 



```

#### esp_vfs_select


```


int esp_vfs_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
Synchronous I/O multiplexing which implements the functionality of POSIX select() for VFS. 

Parameters

nfds -- Specifies the range of descriptors which should be checked. The first nfds descriptors will be checked in each set. 
readfds -- If not NULL, then points to a descriptor set that on input specifies which descriptors should be checked for being ready to read, and on output indicates which descriptors are ready to read. 
writefds -- If not NULL, then points to a descriptor set that on input specifies which descriptors should be checked for being ready to write, and on output indicates which descriptors are ready to write. 
errorfds -- If not NULL, then points to a descriptor set that on input specifies which descriptors should be checked for error conditions, and on output indicates which descriptors have error conditions. 
timeout -- If not NULL, then points to timeval structure which specifies the time period after which the functions should time-out and return. If it is NULL, then the function will not time-out. Note that the timeout period is rounded up to the system tick and incremented by one.


Returns
The number of descriptors set in the descriptor sets, or -1 when an error (specified by errno) have occurred. 



```

#### esp_vfs_select_triggered


```


void esp_vfs_select_triggered(esp_vfs_select_sem_t sem)
Notification from a VFS driver about a read/write/error condition. 
This function is called when the VFS driver detects a read/write/error condition as it was requested by the previous call to start_select.

Parameters
sem -- semaphore structure which was passed to the driver by the start_select call 



```

#### esp_vfs_select_triggered_isr


```


void esp_vfs_select_triggered_isr(esp_vfs_select_sem_t sem, BaseType_t *woken)
Notification from a VFS driver about a read/write/error condition (ISR version) 
This function is called when the VFS driver detects a read/write/error condition as it was requested by the previous call to start_select.

Parameters

sem -- semaphore structure which was passed to the driver by the start_select call 
woken -- is set to pdTRUE if the function wakes up a task with higher priority 




```

#### esp_vfs_pread


```


ssize_t esp_vfs_pread(int fd, void *dst, size_t size, off_t offset)
Implements the VFS layer of POSIX pread() 

Parameters

fd -- File descriptor used for read 
dst -- Pointer to the buffer where the output will be written 
size -- Number of bytes to be read 
offset -- Starting offset of the read


Returns
A positive return value indicates the number of bytes read. -1 is return on failure and errno is set accordingly. 



```

#### esp_vfs_pwrite


```


ssize_t esp_vfs_pwrite(int fd, const void *src, size_t size, off_t offset)
Implements the VFS layer of POSIX pwrite() 

Parameters

fd -- File descriptor used for write 
src -- Pointer to the buffer from where the output will be read 
size -- Number of bytes to write 
offset -- Starting offset of the write


Returns
A positive return value indicates the number of bytes written. -1 is return on failure and errno is set accordingly. 



```

#### esp_vfs_dump_fds


```


void esp_vfs_dump_fds(FILE *fp)
Dump the existing VFS FDs data to FILE* fp. 
Dump the FDs in the format:         <VFS Path Prefix>-<FD seen by App>-<FD seen by driver>

   where:
    VFS Path Prefix   : file prefix used in the esp_vfs_register call
    FD seen by App    : file descriptor returned by the vfs to the application for the path prefix
    FD seen by driver : file descriptor used by the driver for the same file prefix.




Parameters
fp -- File descriptor where data will be dumped 



```



### Structures


#### esp_vfs_select_sem_t


```


struct esp_vfs_select_sem_t
VFS semaphore type for select() 

Public Members
is_sem_local

bool is_sem_local
type of "sem" is SemaphoreHandle_t when true, defined by socket driver otherwise 

sem

void *sem
semaphore instance 



```

#### esp_vfs_t


```


struct esp_vfs_t
VFS definition structure. 
This structure should be filled with pointers to corresponding FS driver functions.
VFS component will translate all FDs so that the filesystem implementation sees them starting at zero. The caller sees a global FD which is prefixed with an pre-filesystem-implementation.
Some FS implementations expect some state (e.g. pointer to some structure) to be passed in as a first argument. For these implementations, populate the members of this structure which have _p suffix, set flags member to ESP_VFS_FLAG_CONTEXT_PTR and provide the context pointer to esp_vfs_register function. If the implementation doesn't use this extra argument, populate the members without _p suffix and set flags member to ESP_VFS_FLAG_DEFAULT.
If the FS driver doesn't provide some of the functions, set corresponding members to NULL. 

Public Members
flags

int flags
ESP_VFS_FLAG_CONTEXT_PTR and/or ESP_VFS_FLAG_READONLY_FS or ESP_VFS_FLAG_DEFAULT 

write_p

ssize_t (*write_p)(void *p, int fd, const void *data, size_t size)
Write with context pointer 

write

ssize_t (*write)(int fd, const void *data, size_t size)
Write without context pointer 

lseek_p

off_t (*lseek_p)(void *p, int fd, off_t size, int mode)
Seek with context pointer 

lseek

off_t (*lseek)(int fd, off_t size, int mode)
Seek without context pointer 

read_p

ssize_t (*read_p)(void *ctx, int fd, void *dst, size_t size)
Read with context pointer 

read

ssize_t (*read)(int fd, void *dst, size_t size)
Read without context pointer 

pread_p

ssize_t (*pread_p)(void *ctx, int fd, void *dst, size_t size, off_t offset)
pread with context pointer 

pread

ssize_t (*pread)(int fd, void *dst, size_t size, off_t offset)
pread without context pointer 

pwrite_p

ssize_t (*pwrite_p)(void *ctx, int fd, const void *src, size_t size, off_t offset)
pwrite with context pointer 

pwrite

ssize_t (*pwrite)(int fd, const void *src, size_t size, off_t offset)
pwrite without context pointer 

open_p

int (*open_p)(void *ctx, const char *path, int flags, int mode)
open with context pointer 

open

int (*open)(const char *path, int flags, int mode)
open without context pointer 

close_p

int (*close_p)(void *ctx, int fd)
close with context pointer 

close

int (*close)(int fd)
close without context pointer 

fstat_p

int (*fstat_p)(void *ctx, int fd, struct stat *st)
fstat with context pointer 

fstat

int (*fstat)(int fd, struct stat *st)
fstat without context pointer 

stat_p

int (*stat_p)(void *ctx, const char *path, struct stat *st)
stat with context pointer 

stat

int (*stat)(const char *path, struct stat *st)
stat without context pointer 

link_p

int (*link_p)(void *ctx, const char *n1, const char *n2)
link with context pointer 

link

int (*link)(const char *n1, const char *n2)
link without context pointer 

unlink_p

int (*unlink_p)(void *ctx, const char *path)
unlink with context pointer 

unlink

int (*unlink)(const char *path)
unlink without context pointer 

rename_p

int (*rename_p)(void *ctx, const char *src, const char *dst)
rename with context pointer 

rename

int (*rename)(const char *src, const char *dst)
rename without context pointer 

opendir_p

DIR *(*opendir_p)(void *ctx, const char *name)
opendir with context pointer 

opendir

DIR *(*opendir)(const char *name)
opendir without context pointer 

readdir_p

struct dirent *(*readdir_p)(void *ctx, DIR *pdir)
readdir with context pointer 

readdir

struct dirent *(*readdir)(DIR *pdir)
readdir without context pointer 

readdir_r_p

int (*readdir_r_p)(void *ctx, DIR *pdir, struct dirent *entry, struct dirent **out_dirent)
readdir_r with context pointer 

readdir_r

int (*readdir_r)(DIR *pdir, struct dirent *entry, struct dirent **out_dirent)
readdir_r without context pointer 

telldir_p

long (*telldir_p)(void *ctx, DIR *pdir)
telldir with context pointer 

telldir

long (*telldir)(DIR *pdir)
telldir without context pointer 

seekdir_p

void (*seekdir_p)(void *ctx, DIR *pdir, long offset)
seekdir with context pointer 

seekdir

void (*seekdir)(DIR *pdir, long offset)
seekdir without context pointer 

closedir_p

int (*closedir_p)(void *ctx, DIR *pdir)
closedir with context pointer 

closedir

int (*closedir)(DIR *pdir)
closedir without context pointer 

mkdir_p

int (*mkdir_p)(void *ctx, const char *name, mode_t mode)
mkdir with context pointer 

mkdir

int (*mkdir)(const char *name, mode_t mode)
mkdir without context pointer 

rmdir_p

int (*rmdir_p)(void *ctx, const char *name)
rmdir with context pointer 

rmdir

int (*rmdir)(const char *name)
rmdir without context pointer 

fcntl_p

int (*fcntl_p)(void *ctx, int fd, int cmd, int arg)
fcntl with context pointer 

fcntl

int (*fcntl)(int fd, int cmd, int arg)
fcntl without context pointer 

ioctl_p

int (*ioctl_p)(void *ctx, int fd, int cmd, va_list args)
ioctl with context pointer 

ioctl

int (*ioctl)(int fd, int cmd, va_list args)
ioctl without context pointer 

fsync_p

int (*fsync_p)(void *ctx, int fd)
fsync with context pointer 

fsync

int (*fsync)(int fd)
fsync without context pointer 

access_p

int (*access_p)(void *ctx, const char *path, int amode)
access with context pointer 

access

int (*access)(const char *path, int amode)
access without context pointer 

truncate_p

int (*truncate_p)(void *ctx, const char *path, off_t length)
truncate with context pointer 

truncate

int (*truncate)(const char *path, off_t length)
truncate without context pointer 

ftruncate_p

int (*ftruncate_p)(void *ctx, int fd, off_t length)
ftruncate with context pointer 

ftruncate

int (*ftruncate)(int fd, off_t length)
ftruncate without context pointer 

utime_p

int (*utime_p)(void *ctx, const char *path, const struct utimbuf *times)
utime with context pointer 

utime

int (*utime)(const char *path, const struct utimbuf *times)
utime without context pointer 

tcsetattr_p

int (*tcsetattr_p)(void *ctx, int fd, int optional_actions, const struct termios *p)
tcsetattr with context pointer 

tcsetattr

int (*tcsetattr)(int fd, int optional_actions, const struct termios *p)
tcsetattr without context pointer 

tcgetattr_p

int (*tcgetattr_p)(void *ctx, int fd, struct termios *p)
tcgetattr with context pointer 

tcgetattr

int (*tcgetattr)(int fd, struct termios *p)
tcgetattr without context pointer 

tcdrain_p

int (*tcdrain_p)(void *ctx, int fd)
tcdrain with context pointer 

tcdrain

int (*tcdrain)(int fd)
tcdrain without context pointer 

tcflush_p

int (*tcflush_p)(void *ctx, int fd, int select)
tcflush with context pointer 

tcflush

int (*tcflush)(int fd, int select)
tcflush without context pointer 

tcflow_p

int (*tcflow_p)(void *ctx, int fd, int action)
tcflow with context pointer 

tcflow

int (*tcflow)(int fd, int action)
tcflow without context pointer 

tcgetsid_p

pid_t (*tcgetsid_p)(void *ctx, int fd)
tcgetsid with context pointer 

tcgetsid

pid_t (*tcgetsid)(int fd)
tcgetsid without context pointer 

tcsendbreak_p

int (*tcsendbreak_p)(void *ctx, int fd, int duration)
tcsendbreak with context pointer 

tcsendbreak

int (*tcsendbreak)(int fd, int duration)
tcsendbreak without context pointer 

start_select

esp_err_t (*start_select)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, esp_vfs_select_sem_t sem, void **end_select_args)
start_select is called for setting up synchronous I/O multiplexing of the desired file descriptors in the given VFS 

socket_select

int (*socket_select)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
socket select function for socket FDs with the functionality of POSIX select(); this should be set only for the socket VFS 

stop_socket_select

void (*stop_socket_select)(void *sem)
called by VFS to interrupt the socket_select call when select is activated from a non-socket VFS driver; set only for the socket driver 

stop_socket_select_isr

void (*stop_socket_select_isr)(void *sem, BaseType_t *woken)
stop_socket_select which can be called from ISR; set only for the socket driver 

get_socket_select_semaphore

void *(*get_socket_select_semaphore)(void)
end_select is called to stop the I/O multiplexing and deinitialize the environment created by start_select for the given VFS 

end_select

esp_err_t (*end_select)(void *end_select_args)
get_socket_select_semaphore returns semaphore allocated in the socket driver; set only for the socket driver 



```



### Macros


#### MAX_FDS


```


MAX_FDS
Maximum number of (global) file descriptors. 

```

#### ESP_VFS_PATH_MAX


```


ESP_VFS_PATH_MAX
Maximum length of path prefix (not including zero terminator) 

```

#### ESP_VFS_FLAG_DEFAULT


```


ESP_VFS_FLAG_DEFAULT
Default value of flags member in esp_vfs_t structure. 

```

#### ESP_VFS_FLAG_CONTEXT_PTR


```


ESP_VFS_FLAG_CONTEXT_PTR
Flag which indicates that FS needs extra context pointer in syscalls. 

```

#### ESP_VFS_FLAG_READONLY_FS


```


ESP_VFS_FLAG_READONLY_FS
Flag which indicates that FS is located on read-only partition. 

```



### Type Definitions


#### esp_vfs_id_t


```


typedef int esp_vfs_id_t

```



### Header File


* components/vfs/include/esp_vfs_dev.h
* This header file can be included with:



> ```
> #include "esp_vfs_dev.h"
> 
> ```
* This header file is a part of the API provided by the `vfs` component. To declare that your component depends on `vfs`, add the following to your CMakeLists.txt:



> ```
> REQUIRES vfs
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES vfs
> 
> ```




### Functions


#### esp_vfs_dev_uart_register


```


void esp_vfs_dev_uart_register(void)

```

#### esp_vfs_dev_uart_use_nonblocking


```


void esp_vfs_dev_uart_use_nonblocking(int uart_num)

```

#### esp_vfs_dev_uart_use_driver


```


void esp_vfs_dev_uart_use_driver(int uart_num)

```

#### esp_vfs_dev_uart_port_set_rx_line_endings


```


int esp_vfs_dev_uart_port_set_rx_line_endings(int uart_num, esp_line_endings_t mode)

```

#### esp_vfs_dev_uart_port_set_tx_line_endings


```


int esp_vfs_dev_uart_port_set_tx_line_endings(int uart_num, esp_line_endings_t mode)

```

#### esp_vfs_dev_uart_set_rx_line_endings


```


void esp_vfs_dev_uart_set_rx_line_endings(esp_line_endings_t mode)
Set the line endings expected to be received on UART. 
This specifies the conversion between line endings received on UART and newlines ('
', LF) passed into stdin:


ESP_LINE_ENDINGS_CRLF: convert CRLF to LF
ESP_LINE_ENDINGS_CR: convert CR to LF
ESP_LINE_ENDINGS_LF: no modification



Note
this function is not thread safe w.r.t. reading from UART


Parameters
mode -- line endings expected on UART 



```

#### esp_vfs_dev_uart_set_tx_line_endings


```


void esp_vfs_dev_uart_set_tx_line_endings(esp_line_endings_t mode)
Set the line endings to sent to UART. 
This specifies the conversion between newlines ('
', LF) on stdout and line endings sent over UART:


ESP_LINE_ENDINGS_CRLF: convert LF to CRLF
ESP_LINE_ENDINGS_CR: convert LF to CR
ESP_LINE_ENDINGS_LF: no modification



Note
this function is not thread safe w.r.t. writing to UART


Parameters
mode -- line endings to send to UART 



```

#### esp_vfs_usb_serial_jtag_use_driver


```


void esp_vfs_usb_serial_jtag_use_driver(void)
set VFS to use USB-SERIAL-JTAG driver for reading and writing 

Note
application must configure USB-SERIAL-JTAG driver before calling these functions With these functions, read and write are blocking and interrupt-driven. 


```

#### esp_vfs_usb_serial_jtag_use_nonblocking


```


void esp_vfs_usb_serial_jtag_use_nonblocking(void)
set VFS to use simple functions for reading and writing UART Read is non-blocking, write is busy waiting until TX FIFO has enough space. These functions are used by default. 

```



### Header File


* components/esp_driver_uart/include/driver/uart_vfs.h
* This header file can be included with:



> ```
> #include "driver/uart_vfs.h"
> 
> ```
* This header file is a part of the API provided by the `esp_driver_uart` component. To declare that your component depends on `esp_driver_uart`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_driver_uart
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_driver_uart
> 
> ```




### Functions


#### uart_vfs_dev_register


```


void uart_vfs_dev_register(void)
Add /dev/uart virtual filesystem driver. 
This function is called from startup code to enable serial output 

```

#### uart_vfs_dev_port_set_rx_line_endings


```


int uart_vfs_dev_port_set_rx_line_endings(int uart_num, esp_line_endings_t mode)
Set the line endings expected to be received on specified UART. 
This specifies the conversion between line endings received on UART and newlines ('
', LF) passed into stdin:


ESP_LINE_ENDINGS_CRLF: convert CRLF to LF
ESP_LINE_ENDINGS_CR: convert CR to LF
ESP_LINE_ENDINGS_LF: no modification



Note
this function is not thread safe w.r.t. reading from UART


Parameters

uart_num -- the UART number 
mode -- line endings to send to UART


Returns
0 if succeeded, or -1 when an error (specified by errno) have occurred. 



```

#### uart_vfs_dev_port_set_tx_line_endings


```


int uart_vfs_dev_port_set_tx_line_endings(int uart_num, esp_line_endings_t mode)
Set the line endings to sent to specified UART. 
This specifies the conversion between newlines ('
', LF) on stdout and line endings sent over UART:


ESP_LINE_ENDINGS_CRLF: convert LF to CRLF
ESP_LINE_ENDINGS_CR: convert LF to CR
ESP_LINE_ENDINGS_LF: no modification



Note
this function is not thread safe w.r.t. writing to UART


Parameters

uart_num -- the UART number 
mode -- line endings to send to UART


Returns
0 if succeeded, or -1 when an error (specified by errno) have occurred. 



```

#### uart_vfs_dev_use_nonblocking


```


void uart_vfs_dev_use_nonblocking(int uart_num)
set VFS to use simple functions for reading and writing UART 
Read is non-blocking, write is busy waiting until TX FIFO has enough space. These functions are used by default.

Parameters
uart_num -- UART peripheral number 



```

#### uart_vfs_dev_use_driver


```


void uart_vfs_dev_use_driver(int uart_num)
set VFS to use UART driver for reading and writing 

Note
Application must configure UART driver before calling these functions With these functions, read and write are blocking and interrupt-driven.


Parameters
uart_num -- UART peripheral number 



```



### Header File


* components/vfs/include/esp_vfs_eventfd.h
* This header file can be included with:



> ```
> #include "esp_vfs_eventfd.h"
> 
> ```
* This header file is a part of the API provided by the `vfs` component. To declare that your component depends on `vfs`, add the following to your CMakeLists.txt:



> ```
> REQUIRES vfs
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES vfs
> 
> ```




### Functions


#### esp_vfs_eventfd_register


```


esp_err_t esp_vfs_eventfd_register(const esp_vfs_eventfd_config_t *config)
Registers the event vfs. 

Returns
ESP_OK if successful, ESP_ERR_NO_MEM if too many VFSes are registered. 



```

#### esp_vfs_eventfd_unregister


```


esp_err_t esp_vfs_eventfd_unregister(void)
Unregisters the event vfs. 

Returns
ESP_OK if successful, ESP_ERR_INVALID_STATE if VFS for given prefix hasn't been registered 



```

#### eventfd


```


int eventfd(unsigned int initval, int flags)

```



### Structures


#### esp_vfs_eventfd_config_t


```


struct esp_vfs_eventfd_config_t
Eventfd vfs initialization settings. 

Public Members
max_fds

size_t max_fds
The maximum number of eventfds supported 



```



### Macros


#### EFD_SUPPORT_ISR


```


EFD_SUPPORT_ISR

```

#### ESP_VFS_EVENTD_CONFIG_DEFAULT


```


ESP_VFS_EVENTD_CONFIG_DEFAULT()

```