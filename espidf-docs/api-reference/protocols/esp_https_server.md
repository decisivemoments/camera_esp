# HTTPS Server




## Overview


This component is built on top of HTTP Server. The HTTPS server takes advantage of hook registration functions in the regular HTTP server to provide callback function for SSL session.


All documentation for HTTP Server applies also to a server you create this way.




## Used APIs


The following APIs of HTTP Server should not be used with HTTPS Server, as they are used internally to handle secure sessions and to maintain internal state:


* "send", "receive" and "pending" callback registration functions - secure socket handling


	+ `httpd_sess_set_send_override()`
	+ `httpd_sess_set_recv_override()`
	+ `httpd_sess_set_pending_override()`
* "transport context" - both global and session


	+ `httpd_sess_get_transport_ctx()` - returns SSL used for the session
	+ `httpd_sess_set_transport_ctx()`
	+ `httpd_get_global_transport_ctx()` - returns the shared SSL context
	+ `httpd_config::global_transport_ctx`
	+ `httpd_config::global_transport_ctx_free_fn`
	+ `httpd_config::open_fn` - used to set up secure sockets


Everything else can be used without limitations.




## Usage


Please see the example protocols/https_server to learn how to set up a secure server.


Basically, all you need is to generate a certificate, embed it into the firmware, and pass the init struct into the start function after the certificate address and lengths are correctly configured in the init struct.


The server can be started with or without SSL by changing a flag in the init struct - `httpd_ssl_config::transport_mode`. This could be used, e.g., for testing or in trusted environments where you prefer speed over security.




## Performance


The initial session setup can take about two seconds, or more with slower clock speed or more verbose logging. Subsequent requests through the open secure socket are much faster (down to under 100 ms).




## Event Handling


ESP HTTPS Server has various events for which a handler can be triggered by the Event Loop Library when the particular event occurs. The handler has to be registered using `esp_event_handler_register()`. This helps in event handling for ESP HTTPS Server.


`esp_https_server_event_id_t` has all the events which can happen for ESP HTTPS server.


Expected data type for different ESP HTTPS server events in event loop:



> * HTTPS_SERVER_EVENT_ERROR : `esp_https_server_last_error_t`
> * HTTPS_SERVER_EVENT_START : `NULL`
> * HTTPS_SERVER_EVENT_ON_CONNECTED : `NULL`
> * HTTPS_SERVER_EVENT_ON_DATA : `int`
> * HTTPS_SERVER_EVENT_SENT_DATA : `NULL`
> * HTTPS_SERVER_EVENT_DISCONNECTED : `NULL`
> * HTTPS_SERVER_EVENT_STOP : `NULL`




## API Reference



### Header File


* components/esp_https_server/include/esp_https_server.h
* This header file can be included with:



> ```
> #include "esp_https_server.h"
> 
> ```
* This header file is a part of the API provided by the `esp_https_server` component. To declare that your component depends on `esp_https_server`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_https_server
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_https_server
> 
> ```




### Functions


#### httpd_ssl_start


```


esp_err_t httpd_ssl_start(httpd_handle_t *handle, httpd_ssl_config_t *config)
Create a SSL capable HTTP server (secure mode may be disabled in config)

Parameters

config -- [inout] - server config, must not be const. Does not have to stay valid after calling this function. 
handle -- [out] - storage for the server handle, must be a valid pointer 


Returns
success 



```

#### httpd_ssl_stop


```


esp_err_t httpd_ssl_stop(httpd_handle_t handle)
Stop the server. Blocks until the server is shut down.

Parameters
handle -- [in] 

Returns

ESP_OK: Server stopped successfully
ESP_ERR_INVALID_ARG: Invalid argument
ESP_FAIL: Failure to shut down server 





```



### Structures


#### esp_https_server_user_cb_arg


```


struct esp_https_server_user_cb_arg
Callback data struct, contains the ESP-TLS connection handle and the connection state at which the callback is executed. 

Public Members
user_cb_state

httpd_ssl_user_cb_state_t user_cb_state
State of user callback 

tls

esp_tls_t *tls
ESP-TLS connection handle 



```

#### httpd_ssl_config


```


struct httpd_ssl_config
HTTPS server config struct
Please use HTTPD_SSL_CONFIG_DEFAULT() to initialize it. 

Public Members
httpd

httpd_config_t httpd
Underlying HTTPD server config
Parameters like task stack size and priority can be adjusted here. 

servercert

const uint8_t *servercert
Server certificate 

servercert_len

size_t servercert_len
Server certificate byte length 

cacert_pem

const uint8_t *cacert_pem
CA certificate ((CA used to sign clients, or client cert itself) 

cacert_len

size_t cacert_len
CA certificate byte length 

prvtkey_pem

const uint8_t *prvtkey_pem
Private key 

prvtkey_len

size_t prvtkey_len
Private key byte length 

use_ecdsa_peripheral

bool use_ecdsa_peripheral
Use ECDSA peripheral to use private key 

ecdsa_key_efuse_blk

uint8_t ecdsa_key_efuse_blk
The efuse block where ECDSA key is stored 

transport_mode

httpd_ssl_transport_mode_t transport_mode
Transport Mode (default secure) 

port_secure

uint16_t port_secure
Port used when transport mode is secure (default 443) 

port_insecure

uint16_t port_insecure
Port used when transport mode is insecure (default 80) 

session_tickets

bool session_tickets
Enable tls session tickets 

use_secure_element

bool use_secure_element
Enable secure element for server session 

user_cb

esp_https_server_user_cb *user_cb
User callback for esp_https_server 

ssl_userdata

void *ssl_userdata
User data to add to the ssl context 

cert_select_cb

esp_tls_handshake_callback cert_select_cb
Certificate selection callback to use. The callback is only applicable when CONFIG_ESP_TLS_SERVER_CERT_SELECT_HOOK is enabled in menuconfig 

alpn_protos

const char **alpn_protos
Application protocols the server supports in order of prefernece. Used for negotiating during the TLS handshake, first one the client supports is selected. The data structure must live as long as the https server itself 



```



### Macros


#### HTTPD_SSL_CONFIG_DEFAULT


```


HTTPD_SSL_CONFIG_DEFAULT()
Default config struct init Notes:
port is set when starting the server, according to 'transport_mode'
one socket uses ~ 40kB RAM with SSL, we reduce the default socket count to 4
SSL sockets are usually long-lived, closing LRU prevents pool exhaustion DOS
Stack size may need adjustments depending on the user application 



```



### Type Definitions


#### esp_https_server_user_cb_arg_t


```


typedef struct esp_https_server_user_cb_arg esp_https_server_user_cb_arg_t
Callback data struct, contains the ESP-TLS connection handle and the connection state at which the callback is executed. 

```

#### esp_https_server_last_error_t


```


typedef esp_tls_last_error_t esp_https_server_last_error_t

```

#### esp_https_server_user_cb


```


typedef void esp_https_server_user_cb(esp_https_server_user_cb_arg_t *user_cb)
Callback function prototype Can be used to get connection or client information (SSL context) E.g. Client certificate, Socket FD, Connection state, etc. 

Param user_cb
Callback data struct 



```

#### httpd_ssl_config_t


```


typedef struct httpd_ssl_config httpd_ssl_config_t

```



### Enumerations


#### esp_https_server_event_id_t


```


enum esp_https_server_event_id_t
Values:
HTTPS_SERVER_EVENT_ERROR

enumerator HTTPS_SERVER_EVENT_ERROR
This event occurs when there are any errors during execution 

HTTPS_SERVER_EVENT_START

enumerator HTTPS_SERVER_EVENT_START
This event occurs when HTTPS Server is started 

HTTPS_SERVER_EVENT_ON_CONNECTED

enumerator HTTPS_SERVER_EVENT_ON_CONNECTED
Once the HTTPS Server has been connected to the client 

HTTPS_SERVER_EVENT_ON_DATA

enumerator HTTPS_SERVER_EVENT_ON_DATA
Occurs when receiving data from the client 

HTTPS_SERVER_EVENT_SENT_DATA

enumerator HTTPS_SERVER_EVENT_SENT_DATA
Occurs when an ESP HTTPS server sends data to the client 

HTTPS_SERVER_EVENT_DISCONNECTED

enumerator HTTPS_SERVER_EVENT_DISCONNECTED
The connection has been disconnected 

HTTPS_SERVER_EVENT_STOP

enumerator HTTPS_SERVER_EVENT_STOP
This event occurs when HTTPS Server is stopped 


```

#### httpd_ssl_transport_mode_t


```


enum httpd_ssl_transport_mode_t
Values:
HTTPD_SSL_TRANSPORT_SECURE

enumerator HTTPD_SSL_TRANSPORT_SECURE

HTTPD_SSL_TRANSPORT_INSECURE

enumerator HTTPD_SSL_TRANSPORT_INSECURE


```

#### httpd_ssl_user_cb_state_t


```


enum httpd_ssl_user_cb_state_t
Indicates the state at which the user callback is executed, i.e at session creation or session close. 
Values:
HTTPD_SSL_USER_CB_SESS_CREATE

enumerator HTTPD_SSL_USER_CB_SESS_CREATE

HTTPD_SSL_USER_CB_SESS_CLOSE

enumerator HTTPD_SSL_USER_CB_SESS_CLOSE


```