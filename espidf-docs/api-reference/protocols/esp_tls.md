# ESP-TLS




## Overview


The ESP-TLS component provides a simplified API interface for accessing the commonly used TLS functions. It supports common scenarios like CA certification validation, SNI, ALPN negotiation, and non-blocking connection among others. All the configurations can be specified in the `esp_tls_cfg_t` data structure. Once done, TLS communication can be conducted using the following APIs:



> * `esp_tls_init()`: for initializing the TLS connection handle.
> * `esp_tls_conn_new_sync()`: for opening a new blocking TLS connection.
> * `esp_tls_conn_new_async()`: for opening a new non-blocking TLS connection.
> * `esp_tls_conn_read()`: for reading from the connection.
> * `esp_tls_conn_write()`: for writing into the connection.
> * `esp_tls_conn_destroy()`: for freeing up the connection.


Any application layer protocol like HTTP1, HTTP2, etc can be executed on top of this layer.




## Application Example


Simple HTTPS example that uses ESP-TLS to establish a secure socket connection: protocols/https_request.




## Tree Structure for ESP-TLS Component



> ```
> ├── esp_tls.c
> ├── esp_tls.h
> ├── esp_tls_mbedtls.c
> ├── esp_tls_wolfssl.c
> └── private_include
>     ├── esp_tls_mbedtls.h
>     └── esp_tls_wolfssl.h
> 
> ```


The ESP-TLS component has a file esp-tls/esp_tls.h which contains the public API headers for the component. Internally, the ESP-TLS component operates using either MbedTLS or WolfSSL, which are SSL/TLS libraries. APIs specific to MbedTLS are present in esp-tls/private_include/esp_tls_mbedtls.h and APIs specific to WolfSSL are present in esp-tls/private_include/esp_tls_wolfssl.h.




## TLS Server Verification


ESP-TLS provides multiple options for TLS server verification on the client side. The ESP-TLS client can verify the server by validating the peer's server certificate or with the help of pre-shared keys. The user should select only one of the following options in the `esp_tls_cfg_t` structure for TLS server verification. If no option is selected, the client will return a fatal error by default during the TLS connection setup.



> * **cacert_buf** and **cacert_bytes**: The CA certificate can be provided in a buffer to the `esp_tls_cfg_t` structure. The ESP-TLS uses the CA certificate present in the buffer to verify the server. The following variables in the `esp_tls_cfg_t` structure must be set.
> 
> 
> 
> > + `cacert_buf` - pointer to the buffer which contains the CA certification.
> > 	+ `cacert_bytes` - the size of the CA certificate in bytes.
> * **use_global_ca_store**: The `global_ca_store` can be initialized and set at once. Then it can be used to verify the server for all the ESP-TLS connections which have set `use_global_ca_store = true` in their respective `esp_tls_cfg_t` structure. See the API Reference section below for information regarding different APIs used for initializing and setting up the `global_ca_store`.
> * **crt_bundle_attach**: The ESP x509 Certificate Bundle API provides an easy way to include a bundle of custom x509 root certificates for TLS server verification. More details can be found at ESP x509 Certificate Bundle.
> * **psk_hint_key**: To use pre-shared keys for server verification, CONFIG_ESP_TLS_PSK_VERIFICATION should be enabled in the ESP-TLS menuconfig. Then the pointer to the PSK hint and key should be provided to the `esp_tls_cfg_t` structure. The ESP-TLS will use the PSK for server verification only when no other option regarding server verification is selected.
> * **skip server verification**: This is an insecure option provided in the ESP-TLS for testing purposes. The option can be set by enabling CONFIG_ESP_TLS_INSECURE and CONFIG_ESP_TLS_SKIP_SERVER_CERT_VERIFY in the ESP-TLS menuconfig. When this option is enabled the ESP-TLS will skip server verification by default when no other options for server verification are selected in the `esp_tls_cfg_t` structure.
> 
> 
> 
> Warning
> 
> 
> Enabling this option comes with a potential risk of establishing a TLS connection with a server that has a fake identity, provided that the server certificate is not provided either through API or other mechanisms like ca_store etc.




## ESP-TLS Server Cert Selection Hook


The ESP-TLS component provides an option to set the server certification selection hook when using the MbedTLS stack. This provides an ability to configure and use a certificate selection callback during server handshake. The callback helps to select a certificate to present to the client based on the TLS extensions supplied in the client hello message, such as ALPN and SNI. To enable this feature, please enable CONFIG_ESP_TLS_SERVER_CERT_SELECT_HOOK in the ESP-TLS menuconfig.


The certificate selection callback can be configured in the `esp_tls_cfg_t` structure as follows:



```
int cert_selection_callback(mbedtls_ssl_context *ssl)
{
    /* Code that the callback should execute */
    return 0;
}

esp_tls_cfg_t cfg = {
    cert_select_cb = cert_section_callback,
};

```




## Underlying SSL/TLS Library Options


The ESP-TLS component offers the option to use MbedTLS or WolfSSL as its underlying SSL/TLS library. By default, only MbedTLS is available and used, WolfSSL SSL/TLS library is also available publicly at https://github.com/espressif/esp-wolfssl. The repository provides the WolfSSL component in binary format, and it also provides a few examples that are useful for understanding the API. Please refer to the repository `README.md` for information on licensing and other options. Please see the below section for instructions on how to use WolfSSL in your project.



Note


As the library options are internal to ESP-TLS, switching the libraries will not change ESP-TLS specific code for a project.





## How to Use WolfSSL with ESP-IDF


There are two ways to use WolfSSL in your project:


1. Directly add WolfSSL as a component in your project with the following three commands:



```
(First, change the directory (cd) to your project directory)
mkdir components
cd components
git clone --recursive https://github.com/espressif/esp-wolfssl.git

```
2. Add WolfSSL as an extra component in your project.


* Download WolfSSL with:



```
git clone --recursive https://github.com/espressif/esp-wolfssl.git

```
* Include ESP-WolfSSL in ESP-IDF with setting `EXTRA_COMPONENT_DIRS` in `CMakeLists.txt` of your project as done in wolfssl/examples. For reference see Optional Project Variables in build-system..


After the above steps, you will have the option to choose WolfSSL as the underlying SSL/TLS library in the configuration menu of your project as follows:



```
idf.py menuconfig > ESP-TLS > SSL/TLS Library > Mbedtls/Wolfssl

```




## Comparison Between MbedTLS and WolfSSL


The following table shows a typical comparison between WolfSSL and MbedTLS when the protocols/https_request example (which includes server authentication) is running with both SSL/TLS libraries and with all respective configurations set to default. For MbedTLS, the IN_CONTENT length and OUT_CONTENT length are set to 16384 bytes and 4096 bytes respectively.








| Property | WolfSSL | MbedTLS |
| --- | --- | --- |
| Total Heap Consumed | ~ 19 KB | ~ 37 KB |
| Task Stack Used | ~ 2.2 KB | ~ 3.6 KB |
| Bin size | ~ 858 KB | ~ 736 KB |



Note


These values can vary based on configuration options and version of respective libraries.





## Digital Signature with ESP-TLS


ESP-TLS provides support for using the Digital Signature (DS) with ESP32-S3. Use of the DS for TLS is supported only when ESP-TLS is used with MbedTLS (default stack) as its underlying SSL/TLS stack. For more details on Digital Signature, please refer to the Digital Signature (DS). The technical details of Digital Signature such as how to calculate private key parameters can be found in **ESP32-S3 Technical Reference Manual** > **Digital Signature (DS)** [PDF]. The DS peripheral must be configured before it can be used to perform Digital Signature, see Configure the DS Peripheral for a TLS Connection.


The DS peripheral must be initialized with the required encrypted private key parameters, which are obtained when the DS peripheral is configured. ESP-TLS internally initializes the DS peripheral when provided with the required DS context, i.e., DS parameters. Please see the below code snippet for passing the DS context to the ESP-TLS context. The DS context passed to the ESP-TLS context should not be freed till the TLS connection is deleted.



```
#include "esp_tls.h"
esp_ds_data_ctx_t *ds_ctx;
/* initialize ds_ctx with encrypted private key parameters, which can be read from the nvs or provided through the application code */
esp_tls_cfg_t cfg = {
    .clientcert_buf = /* the client certification */,
    .clientcert_bytes = /* length of the client certification */,
    /* other configurations options */
    .ds_data = (void *)ds_ctx,
};

```



Note


When using Digital Signature for the TLS connection, along with the other required params, only the client certification (clientcert_buf) and the DS params (ds_data) are required and the client key (clientkey_buf) can be set to NULL.



* An example of mutual authentication with the DS peripheral can be found at ssl mutual auth which internally uses (ESP-TLS) for the TLS connection.




## TLS Ciphersuites


ESP-TLS provides the ability to set a ciphersuites list in client mode. The TLS ciphersuites list informs the server about the supported ciphersuites for the specific TLS connection regardless of the TLS stack configuration. If the server supports any ciphersuite from this list, then the TLS connection will succeed; otherwise, it will fail.


You can set `ciphersuites_list` in the `esp_tls_cfg_t` structure during client connection as follows:



```
/* ciphersuites_list must end with 0 and must be available in the memory scope active during the entire TLS connection */
static const int ciphersuites_list[] = {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384, MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384, 0};
esp_tls_cfg_t cfg = {
    .ciphersuites_list = ciphersuites_list,
};

```


ESP-TLS will not check the validity of `ciphersuites_list` that was set, you should call `esp_tls_get_ciphersuites_list()` to get ciphersuites list supported in the TLS stack and cross-check it against the supplied list.



Note


This feature is supported only in the MbedTLS stack.





## TLS Protocol Version


ESP-TLS provides the ability to set the TLS protocol version for the respective TLS connection. Once the version is specified, it should be exclusively used to establish the TLS connection. This provides an ability to route different TLS connections to different protocol versions like TLS 1.2 and TLS 1.3 at runtime.



Note


At the moment, the feature is supported only when ESP-TLS is used with MbedTLS as its underlying SSL/TLS stack.



To set TLS protocol version with ESP-TLS, set `esp_tls_cfg_t::tls_version` to the required protocol version from `esp_tls_proto_ver_t`. If the protocol version field is not set, then the default policy is to allow TLS connection based on the server requirement.


The ESP-TLS connection can be configured to use the specified protocol version as follows:



> ```
> #include "esp_tls.h"
> esp_tls_cfg_t cfg = {
>     .tls_version = ESP_TLS_VER_TLS_1_2,
> };
> 
> ```




## API Reference



### Header File


* components/esp-tls/esp_tls.h
* This header file can be included with:



> ```
> #include "esp_tls.h"
> 
> ```
* This header file is a part of the API provided by the `esp-tls` component. To declare that your component depends on `esp-tls`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp-tls
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp-tls
> 
> ```




### Functions


#### esp_tls_cfg_server_session_tickets_init


```


esp_err_t esp_tls_cfg_server_session_tickets_init(esp_tls_cfg_server_t *cfg)
Initialize the server side TLS session ticket context. 
This function initializes the server side tls session ticket context which holds all necessary data structures to enable tls session tickets according to RFC5077. Use esp_tls_cfg_server_session_tickets_free to free the data.

Parameters
cfg -- [in] server configuration as esp_tls_cfg_server_t 

Returns
ESP_OK if setup succeeded ESP_ERR_INVALID_ARG if context is already initialized ESP_ERR_NO_MEM if memory allocation failed ESP_ERR_NOT_SUPPORTED if session tickets are not available due to build configuration ESP_FAIL if setup failed 



```

#### esp_tls_cfg_server_session_tickets_free


```


void esp_tls_cfg_server_session_tickets_free(esp_tls_cfg_server_t *cfg)
Free the server side TLS session ticket context. 

Parameters
cfg -- server configuration as esp_tls_cfg_server_t 



```

#### esp_tls_init


```


esp_tls_t *esp_tls_init(void)
Create TLS connection. 
This function allocates and initializes esp-tls structure handle.

Returns
tls Pointer to esp-tls as esp-tls handle if successfully initialized, NULL if allocation error 



```

#### esp_tls_conn_http_new


```


esp_tls_t *esp_tls_conn_http_new(const char *url, const esp_tls_cfg_t *cfg)
Create a new blocking TLS/SSL connection with a given "HTTP" url. 
Note: This API is present for backward compatibility reasons. Alternative function with the same functionality is esp_tls_conn_http_new_sync (and its asynchronous version esp_tls_conn_http_new_async)

Parameters

url -- [in] url of host. 
cfg -- [in] TLS configuration as esp_tls_cfg_t. If you wish to open non-TLS connection, keep this NULL. For TLS connection, a pass pointer to 'esp_tls_cfg_t'. At a minimum, this structure should be zero-initialized. 


Returns
pointer to esp_tls_t, or NULL if connection couldn't be opened. 



```

#### esp_tls_conn_new_sync


```


int esp_tls_conn_new_sync(const char *hostname, int hostlen, int port, const esp_tls_cfg_t *cfg, esp_tls_t *tls)
Create a new blocking TLS/SSL connection. 
This function establishes a TLS/SSL connection with the specified host in blocking manner.

Parameters

hostname -- [in] Hostname of the host. 
hostlen -- [in] Length of hostname. 
port -- [in] Port number of the host. 
cfg -- [in] TLS configuration as esp_tls_cfg_t. If you wish to open non-TLS connection, keep this NULL. For TLS connection, a pass pointer to esp_tls_cfg_t. At a minimum, this structure should be zero-initialized. 
tls -- [in] Pointer to esp-tls as esp-tls handle.


Returns

-1 If connection establishment fails.
1 If connection establishment is successful.
0 If connection state is in progress. 





```

#### esp_tls_conn_http_new_sync


```


int esp_tls_conn_http_new_sync(const char *url, const esp_tls_cfg_t *cfg, esp_tls_t *tls)
Create a new blocking TLS/SSL connection with a given "HTTP" url. 
The behaviour is same as esp_tls_conn_new_sync() API. However this API accepts host's url.

Parameters

url -- [in] url of host. 
cfg -- [in] TLS configuration as esp_tls_cfg_t. If you wish to open non-TLS connection, keep this NULL. For TLS connection, a pass pointer to 'esp_tls_cfg_t'. At a minimum, this structure should be zero-initialized. 
tls -- [in] Pointer to esp-tls as esp-tls handle.


Returns

-1 If connection establishment fails.
1 If connection establishment is successful.
0 If connection state is in progress. 





```

#### esp_tls_conn_new_async


```


int esp_tls_conn_new_async(const char *hostname, int hostlen, int port, const esp_tls_cfg_t *cfg, esp_tls_t *tls)
Create a new non-blocking TLS/SSL connection. 
This function initiates a non-blocking TLS/SSL connection with the specified host, but due to its non-blocking nature, it doesn't wait for the connection to get established.

Parameters

hostname -- [in] Hostname of the host. 
hostlen -- [in] Length of hostname. 
port -- [in] Port number of the host. 
cfg -- [in] TLS configuration as esp_tls_cfg_t. non_block member of this structure should be set to be true. 
tls -- [in] pointer to esp-tls as esp-tls handle.


Returns

-1 If connection establishment fails.
0 If connection establishment is in progress.
1 If connection establishment is successful. 





```

#### esp_tls_conn_http_new_async


```


int esp_tls_conn_http_new_async(const char *url, const esp_tls_cfg_t *cfg, esp_tls_t *tls)
Create a new non-blocking TLS/SSL connection with a given "HTTP" url. 
The behaviour is same as esp_tls_conn_new_async() API. However this API accepts host's url.

Parameters

url -- [in] url of host. 
cfg -- [in] TLS configuration as esp_tls_cfg_t. 
tls -- [in] pointer to esp-tls as esp-tls handle.


Returns

-1 If connection establishment fails.
0 If connection establishment is in progress.
1 If connection establishment is successful. 





```

#### esp_tls_conn_write


```


ssize_t esp_tls_conn_write(esp_tls_t *tls, const void *data, size_t datalen)
Write from buffer 'data' into specified tls connection. 

Parameters

tls -- [in] pointer to esp-tls as esp-tls handle. 
data -- [in] Buffer from which data will be written. 
datalen -- [in] Length of data buffer.


Returns

>=0 if write operation was successful, the return value is the number of bytes actually written to the TLS/SSL connection.
<0 if write operation was not successful, because either an error occured or an action must be taken by the calling process.
ESP_TLS_ERR_SSL_WANT_READ/ ESP_TLS_ERR_SSL_WANT_WRITE. if the handshake is incomplete and waiting for data to be available for reading. In this case this functions needs to be called again when the underlying transport is ready for operation. 





```

#### esp_tls_conn_read


```


ssize_t esp_tls_conn_read(esp_tls_t *tls, void *data, size_t datalen)
Read from specified tls connection into the buffer 'data'. 

Parameters

tls -- [in] pointer to esp-tls as esp-tls handle. 
data -- [in] Buffer to hold read data. 
datalen -- [in] Length of data buffer.


Returns

>0 if read operation was successful, the return value is the number of bytes actually read from the TLS/SSL connection.
0 if read operation was not successful. The underlying connection was closed.
<0 if read operation was not successful, because either an error occured or an action must be taken by the calling process. 





```

#### esp_tls_conn_destroy


```


int esp_tls_conn_destroy(esp_tls_t *tls)
Close the TLS/SSL connection and free any allocated resources. 
This function should be called to close each tls connection opened with esp_tls_conn_new_sync() (or esp_tls_conn_http_new_sync()) and esp_tls_conn_new_async() (or esp_tls_conn_http_new_async()) APIs.

Parameters
tls -- [in] pointer to esp-tls as esp-tls handle.

Returns
- 0 on success
-1 if socket error or an invalid argument 





```

#### esp_tls_get_bytes_avail


```


ssize_t esp_tls_get_bytes_avail(esp_tls_t *tls)
Return the number of application data bytes remaining to be read from the current record. 
This API is a wrapper over mbedtls's mbedtls_ssl_get_bytes_avail() API.

Parameters
tls -- [in] pointer to esp-tls as esp-tls handle.

Returns

-1 in case of invalid arg
bytes available in the application data record read buffer 





```

#### esp_tls_get_conn_sockfd


```


esp_err_t esp_tls_get_conn_sockfd(esp_tls_t *tls, int *sockfd)
Returns the connection socket file descriptor from esp_tls session. 

Parameters

tls -- [in] handle to esp_tls context
sockfd -- [out] int pointer to sockfd value.


Returns
- ESP_OK on success and value of sockfd will be updated with socket file descriptor for connection
ESP_ERR_INVALID_ARG if (tls == NULL || sockfd == NULL) 





```

#### esp_tls_set_conn_sockfd


```


esp_err_t esp_tls_set_conn_sockfd(esp_tls_t *tls, int sockfd)
Sets the connection socket file descriptor for the esp_tls session. 

Parameters

tls -- [in] handle to esp_tls context
sockfd -- [in] sockfd value to set.


Returns
- ESP_OK on success and value of sockfd for the tls connection shall updated withthe provided value
ESP_ERR_INVALID_ARG if (tls == NULL || sockfd < 0) 





```

#### esp_tls_get_conn_state


```


esp_err_t esp_tls_get_conn_state(esp_tls_t *tls, esp_tls_conn_state_t *conn_state)
Gets the connection state for the esp_tls session. 

Parameters

tls -- [in] handle to esp_tls context
conn_state -- [out] pointer to the connection state value.


Returns
- ESP_OK on success and value of sockfd for the tls connection shall updated withthe provided value
ESP_ERR_INVALID_ARG (Invalid arguments) 





```

#### esp_tls_set_conn_state


```


esp_err_t esp_tls_set_conn_state(esp_tls_t *tls, esp_tls_conn_state_t conn_state)
Sets the connection state for the esp_tls session. 

Parameters

tls -- [in] handle to esp_tls context
conn_state -- [in] connection state value to set.


Returns
- ESP_OK on success and value of sockfd for the tls connection shall updated withthe provided value
ESP_ERR_INVALID_ARG (Invalid arguments) 





```

#### esp_tls_get_ssl_context


```


void *esp_tls_get_ssl_context(esp_tls_t *tls)
Returns the ssl context. 

Parameters
tls -- [in] handle to esp_tls context

Returns
- ssl_ctx pointer to ssl context of underlying TLS layer on success
NULL in case of error 





```

#### esp_tls_init_global_ca_store


```


esp_err_t esp_tls_init_global_ca_store(void)
Create a global CA store, initially empty. 
This function should be called if the application wants to use the same CA store for multiple connections. This function initialises the global CA store which can be then set by calling esp_tls_set_global_ca_store(). To be effective, this function must be called before any call to esp_tls_set_global_ca_store().

Returns

ESP_OK if creating global CA store was successful.
ESP_ERR_NO_MEM if an error occured when allocating the mbedTLS resources. 





```

#### esp_tls_set_global_ca_store


```


esp_err_t esp_tls_set_global_ca_store(const unsigned char *cacert_pem_buf, const unsigned int cacert_pem_bytes)
Set the global CA store with the buffer provided in pem format. 
This function should be called if the application wants to set the global CA store for multiple connections i.e. to add the certificates in the provided buffer to the certificate chain. This function implicitly calls esp_tls_init_global_ca_store() if it has not already been called. The application must call this function before calling esp_tls_conn_new().

Parameters

cacert_pem_buf -- [in] Buffer which has certificates in pem format. This buffer is used for creating a global CA store, which can be used by other tls connections. 
cacert_pem_bytes -- [in] Length of the buffer.


Returns

ESP_OK if adding certificates was successful.
Other if an error occured or an action must be taken by the calling process. 





```

#### esp_tls_free_global_ca_store


```


void esp_tls_free_global_ca_store(void)
Free the global CA store currently being used. 
The memory being used by the global CA store to store all the parsed certificates is freed up. The application can call this API if it no longer needs the global CA store. 

```

#### esp_tls_get_and_clear_last_error


```


esp_err_t esp_tls_get_and_clear_last_error(esp_tls_error_handle_t h, int *esp_tls_code, int *esp_tls_flags)
Returns last error in esp_tls with detailed mbedtls related error codes. The error information is cleared internally upon return. 

Parameters

h -- [in] esp-tls error handle. 
esp_tls_code -- [out] last error code returned from mbedtls api (set to zero if none) This pointer could be NULL if caller does not care about esp_tls_code 
esp_tls_flags -- [out] last certification verification flags (set to zero if none) This pointer could be NULL if caller does not care about esp_tls_code


Returns

ESP_ERR_INVALID_STATE if invalid parameters
ESP_OK (0) if no error occurred
specific error code (based on ESP_ERR_ESP_TLS_BASE) otherwise 





```

#### esp_tls_get_and_clear_error_type


```


esp_err_t esp_tls_get_and_clear_error_type(esp_tls_error_handle_t h, esp_tls_error_type_t err_type, int *error_code)
Returns the last error captured in esp_tls of a specific type The error information is cleared internally upon return. 

Parameters

h -- [in] esp-tls error handle. 
err_type -- [in] specific error type 
error_code -- [out] last error code returned from mbedtls api (set to zero if none) This pointer could be NULL if caller does not care about esp_tls_code 


Returns

ESP_ERR_INVALID_STATE if invalid parameters
ESP_OK if a valid error returned and was cleared 





```

#### esp_tls_get_error_handle


```


esp_err_t esp_tls_get_error_handle(esp_tls_t *tls, esp_tls_error_handle_t *error_handle)
Returns the ESP-TLS error_handle. 

Parameters

tls -- [in] handle to esp_tls context
error_handle -- [out] pointer to the error handle.


Returns

ESP_OK on success and error_handle will be updated with the ESP-TLS error handle.
ESP_ERR_INVALID_ARG if (tls == NULL || error_handle == NULL) 





```

#### esp_tls_get_global_ca_store


```


mbedtls_x509_crt *esp_tls_get_global_ca_store(void)
Get the pointer to the global CA store currently being used. 
The application must first call esp_tls_set_global_ca_store(). Then the same CA store could be used by the application for APIs other than esp_tls.

Note
Modifying the pointer might cause a failure in verifying the certificates.


Returns

Pointer to the global CA store currently being used if successful.
NULL if there is no global CA store set. 





```

#### esp_tls_get_ciphersuites_list


```


const int *esp_tls_get_ciphersuites_list(void)
Get supported TLS ciphersuites list. 
See https://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml#tls-parameters-4 for the list of ciphersuites

Returns
Pointer to a zero-terminated array of IANA identifiers of TLS ciphersuites. 



```

#### esp_tls_server_session_create


```


int esp_tls_server_session_create(esp_tls_cfg_server_t *cfg, int sockfd, esp_tls_t *tls)
Create TLS/SSL server session. 
This function creates a TLS/SSL server context for already accepted client connection and performs TLS/SSL handshake with the client

Parameters

cfg -- [in] Pointer to esp_tls_cfg_server_t 
sockfd -- [in] FD of accepted connection 
tls -- [out] Pointer to allocated esp_tls_t


Returns

0 if successful
<0 in case of error 





```

#### esp_tls_server_session_delete


```


void esp_tls_server_session_delete(esp_tls_t *tls)
Close the server side TLS/SSL connection and free any allocated resources. 
This function should be called to close each tls connection opened with esp_tls_server_session_create()

Parameters
tls -- [in] pointer to esp_tls_t 



```

#### esp_tls_plain_tcp_connect


```


esp_err_t esp_tls_plain_tcp_connect(const char *host, int hostlen, int port, const esp_tls_cfg_t *cfg, esp_tls_error_handle_t error_handle, int *sockfd)
Creates a plain TCP connection, returning a valid socket fd on success or an error handle. 

Parameters

host -- [in] Hostname of the host. 
hostlen -- [in] Length of hostname. 
port -- [in] Port number of the host. 
cfg -- [in] ESP-TLS configuration as esp_tls_cfg_t. 
error_handle -- [out] ESP-TLS error handle holding potential errors occurred during connection 
sockfd -- [out] Socket descriptor if successfully connected on TCP layer 


Returns
ESP_OK on success ESP_ERR_INVALID_ARG if invalid output parameters ESP-TLS based error codes on failure 



```



### Structures


#### psk_key_hint


```


struct psk_key_hint
ESP-TLS preshared key and hint structure. 

Public Members
key

const uint8_t *key
key in PSK authentication mode in binary format 

key_size

const size_t key_size
length of the key 

hint

const char *hint
hint in PSK authentication mode in string format 



```

#### tls_keep_alive_cfg


```


struct tls_keep_alive_cfg
esp-tls client session ticket ctx 
Keep alive parameters structure 

Public Members
keep_alive_enable

bool keep_alive_enable
Enable keep-alive timeout 

keep_alive_idle

int keep_alive_idle
Keep-alive idle time (second) 

keep_alive_interval

int keep_alive_interval
Keep-alive interval time (second) 

keep_alive_count

int keep_alive_count
Keep-alive packet retry send count 



```

#### esp_tls_cfg


```


struct esp_tls_cfg
ESP-TLS configuration parameters. 

Note
Note about format of certificates:
This structure includes certificates of a Certificate Authority, of client or server as well as private keys, which may be of PEM or DER format. In case of PEM format, the buffer must be NULL terminated (with NULL character included in certificate size).
Certificate Authority's certificate may be a chain of certificates in case of PEM format, but could be only one certificate in case of DER format
Variables names of certificates and private key buffers and sizes are defined as unions providing backward compatibility for legacy *_pem_buf and *_pem_bytes names which suggested only PEM format was supported. It is encouraged to use generic names such as cacert_buf and cacert_bytes. 




Public Members
alpn_protos

const char **alpn_protos
Application protocols required for HTTP2. If HTTP2/ALPN support is required, a list of protocols that should be negotiated. The format is length followed by protocol name. For the most common cases the following is ok: const char **alpn_protos = { "h2", NULL };
where 'h2' is the protocol name 



cacert_buf

const unsigned char *cacert_buf
Certificate Authority's certificate in a buffer. Format may be PEM or DER, depending on mbedtls-support This buffer should be NULL terminated in case of PEM 

cacert_pem_buf

const unsigned char *cacert_pem_buf
CA certificate buffer legacy name 

cacert_bytes

unsigned int cacert_bytes
Size of Certificate Authority certificate pointed to by cacert_buf (including NULL-terminator in case of PEM format) 

cacert_pem_bytes

unsigned int cacert_pem_bytes
Size of Certificate Authority certificate legacy name 

clientcert_buf

const unsigned char *clientcert_buf
Client certificate in a buffer Format may be PEM or DER, depending on mbedtls-support This buffer should be NULL terminated in case of PEM 

clientcert_pem_buf

const unsigned char *clientcert_pem_buf
Client certificate legacy name 

clientcert_bytes

unsigned int clientcert_bytes
Size of client certificate pointed to by clientcert_pem_buf (including NULL-terminator in case of PEM format) 

clientcert_pem_bytes

unsigned int clientcert_pem_bytes
Size of client certificate legacy name 

clientkey_buf

const unsigned char *clientkey_buf
Client key in a buffer Format may be PEM or DER, depending on mbedtls-support This buffer should be NULL terminated in case of PEM 

clientkey_pem_buf

const unsigned char *clientkey_pem_buf
Client key legacy name 

clientkey_bytes

unsigned int clientkey_bytes
Size of client key pointed to by clientkey_pem_buf (including NULL-terminator in case of PEM format) 

clientkey_pem_bytes

unsigned int clientkey_pem_bytes
Size of client key legacy name 

clientkey_password

const unsigned char *clientkey_password
Client key decryption password string 

clientkey_password_len

unsigned int clientkey_password_len
String length of the password pointed to by clientkey_password 

use_ecdsa_peripheral

bool use_ecdsa_peripheral
Use the ECDSA peripheral for the private key operations 

ecdsa_key_efuse_blk

uint8_t ecdsa_key_efuse_blk
The efuse block where the ECDSA key is stored 

non_block

bool non_block
Configure non-blocking mode. If set to true the underneath socket will be configured in non blocking mode after tls session is established 

use_secure_element

bool use_secure_element
Enable this option to use secure element or atecc608a chip 

timeout_ms

int timeout_ms
Network timeout in milliseconds. Note: If this value is not set, by default the timeout is set to 10 seconds. If you wish that the session should wait indefinitely then please use a larger value e.g., INT32_MAX 

use_global_ca_store

bool use_global_ca_store
Use a global ca_store for all the connections in which this bool is set. 

common_name

const char *common_name
If non-NULL, server certificate CN must match this name. If NULL, server certificate CN must match hostname. 

skip_common_name

bool skip_common_name
Skip any validation of server certificate CN field 

keep_alive_cfg

tls_keep_alive_cfg_t *keep_alive_cfg
Enable TCP keep-alive timeout for SSL connection 

psk_hint_key

const psk_hint_key_t *psk_hint_key
Pointer to PSK hint and key. if not NULL (and certificates are NULL) then PSK authentication is enabled with configured setup. Important note: the pointer must be valid for connection 

crt_bundle_attach

esp_err_t (*crt_bundle_attach)(void *conf)
Function pointer to esp_crt_bundle_attach. Enables the use of certification bundle for server verification, must be enabled in menuconfig 

ds_data

void *ds_data
Pointer for digital signature peripheral context 

is_plain_tcp

bool is_plain_tcp
Use non-TLS connection: When set to true, the esp-tls uses plain TCP transport rather then TLS/SSL connection. Note, that it is possible to connect using a plain tcp transport directly with esp_tls_plain_tcp_connect() API 

if_name

struct ifreq *if_name
The name of interface for data to go through. Use the default interface without setting 

addr_family

esp_tls_addr_family_t addr_family
The address family to use when connecting to a host. 

ciphersuites_list

const int *ciphersuites_list
Pointer to a zero-terminated array of IANA identifiers of TLS ciphersuites. Please check the list validity by esp_tls_get_ciphersuites_list() API 

tls_version

esp_tls_proto_ver_t tls_version
TLS protocol version of the connection, e.g., TLS 1.2, TLS 1.3 (default - no preference) 



```

#### esp_tls_cfg_server


```


struct esp_tls_cfg_server
ESP-TLS Server configuration parameters. 

Public Members
alpn_protos

const char **alpn_protos
Application protocols required for HTTP2. If HTTP2/ALPN support is required, a list of protocols that should be negotiated. The format is length followed by protocol name. For the most common cases the following is ok: const char **alpn_protos = { "h2", NULL };
where 'h2' is the protocol name 



cacert_buf

const unsigned char *cacert_buf
Client CA certificate in a buffer. This buffer should be NULL terminated 

cacert_pem_buf

const unsigned char *cacert_pem_buf
Client CA certificate legacy name 

cacert_bytes

unsigned int cacert_bytes
Size of client CA certificate pointed to by cacert_pem_buf 

cacert_pem_bytes

unsigned int cacert_pem_bytes
Size of client CA certificate legacy name 

servercert_buf

const unsigned char *servercert_buf
Server certificate in a buffer This buffer should be NULL terminated 

servercert_pem_buf

const unsigned char *servercert_pem_buf
Server certificate legacy name 

servercert_bytes

unsigned int servercert_bytes
Size of server certificate pointed to by servercert_pem_buf 

servercert_pem_bytes

unsigned int servercert_pem_bytes
Size of server certificate legacy name 

serverkey_buf

const unsigned char *serverkey_buf
Server key in a buffer This buffer should be NULL terminated 

serverkey_pem_buf

const unsigned char *serverkey_pem_buf
Server key legacy name 

serverkey_bytes

unsigned int serverkey_bytes
Size of server key pointed to by serverkey_pem_buf 

serverkey_pem_bytes

unsigned int serverkey_pem_bytes
Size of server key legacy name 

serverkey_password

const unsigned char *serverkey_password
Server key decryption password string 

serverkey_password_len

unsigned int serverkey_password_len
String length of the password pointed to by serverkey_password 

use_ecdsa_peripheral

bool use_ecdsa_peripheral
Use ECDSA peripheral to use private key 

ecdsa_key_efuse_blk

uint8_t ecdsa_key_efuse_blk
The efuse block where ECDSA key is stored 

use_secure_element

bool use_secure_element
Enable this option to use secure element or atecc608a chip 

userdata

void *userdata
User data to be added to the ssl context. Can be retrieved by callbacks 



```



### Type Definitions


#### esp_tls_conn_state_t


```


typedef enum esp_tls_conn_state esp_tls_conn_state_t
ESP-TLS Connection State. 

```

#### esp_tls_role_t


```


typedef enum esp_tls_role esp_tls_role_t

```

#### psk_hint_key_t


```


typedef struct psk_key_hint psk_hint_key_t
ESP-TLS preshared key and hint structure. 

```

#### tls_keep_alive_cfg_t


```


typedef struct tls_keep_alive_cfg tls_keep_alive_cfg_t
esp-tls client session ticket ctx 
Keep alive parameters structure 

```

#### esp_tls_addr_family_t


```


typedef enum esp_tls_addr_family esp_tls_addr_family_t

```

#### esp_tls_cfg_t


```


typedef struct esp_tls_cfg esp_tls_cfg_t
ESP-TLS configuration parameters. 

Note
Note about format of certificates:
This structure includes certificates of a Certificate Authority, of client or server as well as private keys, which may be of PEM or DER format. In case of PEM format, the buffer must be NULL terminated (with NULL character included in certificate size).
Certificate Authority's certificate may be a chain of certificates in case of PEM format, but could be only one certificate in case of DER format
Variables names of certificates and private key buffers and sizes are defined as unions providing backward compatibility for legacy *_pem_buf and *_pem_bytes names which suggested only PEM format was supported. It is encouraged to use generic names such as cacert_buf and cacert_bytes. 




```

#### esp_tls_handshake_callback


```


typedef void *esp_tls_handshake_callback

```

#### esp_tls_cfg_server_t


```


typedef struct esp_tls_cfg_server esp_tls_cfg_server_t
ESP-TLS Server configuration parameters. 

```

#### esp_tls_t


```


typedef struct esp_tls esp_tls_t

```



### Enumerations


#### esp_tls_conn_state


```


enum esp_tls_conn_state
ESP-TLS Connection State. 
Values:
ESP_TLS_INIT

enumerator ESP_TLS_INIT

ESP_TLS_CONNECTING

enumerator ESP_TLS_CONNECTING

ESP_TLS_HANDSHAKE

enumerator ESP_TLS_HANDSHAKE

ESP_TLS_FAIL

enumerator ESP_TLS_FAIL

ESP_TLS_DONE

enumerator ESP_TLS_DONE


```

#### esp_tls_role


```


enum esp_tls_role
Values:
ESP_TLS_CLIENT

enumerator ESP_TLS_CLIENT

ESP_TLS_SERVER

enumerator ESP_TLS_SERVER


```

#### esp_tls_addr_family


```


enum esp_tls_addr_family
Values:
ESP_TLS_AF_UNSPEC

enumerator ESP_TLS_AF_UNSPEC
Unspecified address family. 

ESP_TLS_AF_INET

enumerator ESP_TLS_AF_INET
IPv4 address family. 

ESP_TLS_AF_INET6

enumerator ESP_TLS_AF_INET6
IPv6 address family. 


```

#### esp_tls_proto_ver_t


```


enum esp_tls_proto_ver_t
Values:
ESP_TLS_VER_ANY

enumerator ESP_TLS_VER_ANY

ESP_TLS_VER_TLS_1_2

enumerator ESP_TLS_VER_TLS_1_2

ESP_TLS_VER_TLS_1_3

enumerator ESP_TLS_VER_TLS_1_3

ESP_TLS_VER_TLS_MAX

enumerator ESP_TLS_VER_TLS_MAX


```



### Header File


* components/esp-tls/esp_tls_errors.h
* This header file can be included with:



> ```
> #include "esp_tls_errors.h"
> 
> ```
* This header file is a part of the API provided by the `esp-tls` component. To declare that your component depends on `esp-tls`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp-tls
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp-tls
> 
> ```




### Structures


#### esp_tls_last_error


```


struct esp_tls_last_error
Error structure containing relevant errors in case tls error occurred. 

Public Members
last_error

esp_err_t last_error
error code (based on ESP_ERR_ESP_TLS_BASE) of the last occurred error 

esp_tls_error_code

int esp_tls_error_code
esp_tls error code from last esp_tls failed api 

esp_tls_flags

int esp_tls_flags
last certification verification flags 



```



### Macros


#### ESP_ERR_ESP_TLS_BASE


```


ESP_ERR_ESP_TLS_BASE
Starting number of ESP-TLS error codes 

```

#### ESP_ERR_ESP_TLS_CANNOT_RESOLVE_HOSTNAME


```


ESP_ERR_ESP_TLS_CANNOT_RESOLVE_HOSTNAME
Error if hostname couldn't be resolved upon tls connection 

```

#### ESP_ERR_ESP_TLS_CANNOT_CREATE_SOCKET


```


ESP_ERR_ESP_TLS_CANNOT_CREATE_SOCKET
Failed to create socket 

```

#### ESP_ERR_ESP_TLS_UNSUPPORTED_PROTOCOL_FAMILY


```


ESP_ERR_ESP_TLS_UNSUPPORTED_PROTOCOL_FAMILY
Unsupported protocol family 

```

#### ESP_ERR_ESP_TLS_FAILED_CONNECT_TO_HOST


```


ESP_ERR_ESP_TLS_FAILED_CONNECT_TO_HOST
Failed to connect to host 

```

#### ESP_ERR_ESP_TLS_SOCKET_SETOPT_FAILED


```


ESP_ERR_ESP_TLS_SOCKET_SETOPT_FAILED
failed to set/get socket option 

```

#### ESP_ERR_ESP_TLS_CONNECTION_TIMEOUT


```


ESP_ERR_ESP_TLS_CONNECTION_TIMEOUT
new connection in esp_tls_low_level_conn connection timeouted 

```

#### ESP_ERR_ESP_TLS_SE_FAILED


```


ESP_ERR_ESP_TLS_SE_FAILED

```

#### ESP_ERR_ESP_TLS_TCP_CLOSED_FIN


```


ESP_ERR_ESP_TLS_TCP_CLOSED_FIN

```

#### ESP_ERR_MBEDTLS_CERT_PARTLY_OK


```


ESP_ERR_MBEDTLS_CERT_PARTLY_OK
mbedtls parse certificates was partly successful 

```

#### ESP_ERR_MBEDTLS_CTR_DRBG_SEED_FAILED


```


ESP_ERR_MBEDTLS_CTR_DRBG_SEED_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED


```


ESP_ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED


```


ESP_ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED


```


ESP_ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_X509_CRT_PARSE_FAILED


```


ESP_ERR_MBEDTLS_X509_CRT_PARSE_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED


```


ESP_ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_SSL_SETUP_FAILED


```


ESP_ERR_MBEDTLS_SSL_SETUP_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_SSL_WRITE_FAILED


```


ESP_ERR_MBEDTLS_SSL_WRITE_FAILED
mbedtls api returned error 

```

#### ESP_ERR_MBEDTLS_PK_PARSE_KEY_FAILED


```


ESP_ERR_MBEDTLS_PK_PARSE_KEY_FAILED
mbedtls api returned failed 

```

#### ESP_ERR_MBEDTLS_SSL_HANDSHAKE_FAILED


```


ESP_ERR_MBEDTLS_SSL_HANDSHAKE_FAILED
mbedtls api returned failed 

```

#### ESP_ERR_MBEDTLS_SSL_CONF_PSK_FAILED


```


ESP_ERR_MBEDTLS_SSL_CONF_PSK_FAILED
mbedtls api returned failed 

```

#### ESP_ERR_MBEDTLS_SSL_TICKET_SETUP_FAILED


```


ESP_ERR_MBEDTLS_SSL_TICKET_SETUP_FAILED
mbedtls api returned failed 

```

#### ESP_ERR_WOLFSSL_SSL_SET_HOSTNAME_FAILED


```


ESP_ERR_WOLFSSL_SSL_SET_HOSTNAME_FAILED
wolfSSL api returned error 

```

#### ESP_ERR_WOLFSSL_SSL_CONF_ALPN_PROTOCOLS_FAILED


```


ESP_ERR_WOLFSSL_SSL_CONF_ALPN_PROTOCOLS_FAILED
wolfSSL api returned error 

```

#### ESP_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED


```


ESP_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED
wolfSSL api returned error 

```

#### ESP_ERR_WOLFSSL_KEY_VERIFY_SETUP_FAILED


```


ESP_ERR_WOLFSSL_KEY_VERIFY_SETUP_FAILED
wolfSSL api returned error 

```

#### ESP_ERR_WOLFSSL_SSL_HANDSHAKE_FAILED


```


ESP_ERR_WOLFSSL_SSL_HANDSHAKE_FAILED
wolfSSL api returned failed 

```

#### ESP_ERR_WOLFSSL_CTX_SETUP_FAILED


```


ESP_ERR_WOLFSSL_CTX_SETUP_FAILED
wolfSSL api returned failed 

```

#### ESP_ERR_WOLFSSL_SSL_SETUP_FAILED


```


ESP_ERR_WOLFSSL_SSL_SETUP_FAILED
wolfSSL api returned failed 

```

#### ESP_ERR_WOLFSSL_SSL_WRITE_FAILED


```


ESP_ERR_WOLFSSL_SSL_WRITE_FAILED
wolfSSL api returned failed 

```

#### ESP_TLS_ERR_SSL_WANT_READ


```


ESP_TLS_ERR_SSL_WANT_READ
Definition of errors reported from IO API (potentially non-blocking) in case of error:
esp_tls_conn_read()
esp_tls_conn_write() 



```

#### ESP_TLS_ERR_SSL_WANT_WRITE


```


ESP_TLS_ERR_SSL_WANT_WRITE

```

#### ESP_TLS_ERR_SSL_TIMEOUT


```


ESP_TLS_ERR_SSL_TIMEOUT

```



### Type Definitions


#### esp_tls_error_handle_t


```


typedef struct esp_tls_last_error *esp_tls_error_handle_t

```

#### esp_tls_last_error_t


```


typedef struct esp_tls_last_error esp_tls_last_error_t
Error structure containing relevant errors in case tls error occurred. 

```



### Enumerations


#### esp_tls_error_type_t


```


enum esp_tls_error_type_t
Definition of different types/sources of error codes reported from different components 
Values:
ESP_TLS_ERR_TYPE_UNKNOWN

enumerator ESP_TLS_ERR_TYPE_UNKNOWN

ESP_TLS_ERR_TYPE_SYSTEM

enumerator ESP_TLS_ERR_TYPE_SYSTEM
System error &#8212; errno 

ESP_TLS_ERR_TYPE_MBEDTLS

enumerator ESP_TLS_ERR_TYPE_MBEDTLS
Error code from mbedTLS library 

ESP_TLS_ERR_TYPE_MBEDTLS_CERT_FLAGS

enumerator ESP_TLS_ERR_TYPE_MBEDTLS_CERT_FLAGS
Certificate flags defined in mbedTLS 

ESP_TLS_ERR_TYPE_ESP

enumerator ESP_TLS_ERR_TYPE_ESP
ESP-IDF error type &#8212; esp_err_t 

ESP_TLS_ERR_TYPE_WOLFSSL

enumerator ESP_TLS_ERR_TYPE_WOLFSSL
Error code from wolfSSL library 

ESP_TLS_ERR_TYPE_WOLFSSL_CERT_FLAGS

enumerator ESP_TLS_ERR_TYPE_WOLFSSL_CERT_FLAGS
Certificate flags defined in wolfSSL 

ESP_TLS_ERR_TYPE_MAX

enumerator ESP_TLS_ERR_TYPE_MAX
Last err type &#8212; invalid entry 


```