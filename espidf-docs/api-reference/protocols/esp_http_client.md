# ESP HTTP Client




## Overview


`esp_http_client` component provides a set of APIs for making HTTP/S requests from ESP-IDF applications. The steps to use these APIs are as follows:



> * `esp_http_client_init()`: Creates an `esp_http_client_handle_t` instance, i.e., an HTTP client handle based on the given `esp_http_client_config_t` configuration. This function must be the first to be called; default values are assumed for the configuration values that are not explicitly defined by the user.
> * `esp_http_client_perform()`: Performs all operations of the `esp_http_client` - opening the connection, exchanging data, and closing the connection (as required), while blocking the current task before its completion. All related events are invoked through the event handler (as specified in `esp_http_client_config_t`).
> * `esp_http_client_cleanup()`: Closes the connection (if any) and frees up all the memory allocated to the HTTP client instance. This must be the last function to be called after the completion of operations.




## Application Example


Simple example that uses ESP HTTP Client to make HTTP/S requests can be found at protocols/esp_http_client.




## Basic HTTP Request


Check out the example functions `http_rest_with_url` and `http_rest_with_hostname_path` in the application example for implementation details.




## Persistent Connections


Persistent connection means that the HTTP client can reuse the same connection for several exchanges. If the server does not request to close the connection with the `Connection: close` header, the connection is not dropped but is instead kept open and used for further requests.


To allow ESP HTTP client to take full advantage of persistent connections, one should make as many requests as possible using the same handle instance. Check out the example functions `http_rest_with_url` and `http_rest_with_hostname_path` in the application example. Here, once the connection is created, multiple requests (`GET`, `POST`, `PUT`, etc.) are made before the connection is closed.




## HTTPS Request


ESP HTTP client supports SSL connections using **mbedTLS**, with the `url` configuration starting with `https` scheme or `transport_type` set to `HTTP_TRANSPORT_OVER_SSL`. HTTPS support can be configured via CONFIG_ESP_HTTP_CLIENT_ENABLE_HTTPS (enabled by default).



Note


While making HTTPS requests, if server verification is needed, an additional root certificate (in PEM format) needs to be provided to the `cert_pem` member in the `esp_http_client_config_t` configuration. Users can also use the `ESP x509 Certificate Bundle` for server verification using the `crt_bundle_attach` member of the `esp_http_client_config_t` configuration.



Check out the example functions `https_with_url` and `https_with_hostname_path` in the application example for implementation details of the above note.




## HTTP Stream


Some applications need to open the connection and control the exchange of data actively (data streaming). In such cases, the application flow is different from regular requests. Example flow is given below:



> * `esp_http_client_init()`: Create a HTTP client handle.
> * `esp_http_client_set_*` or `esp_http_client_delete_*`: Modify the HTTP connection parameters (optional).
> * `esp_http_client_open()`: Open the HTTP connection with `write_len` parameter (content length that needs to be written to server), set `write_len=0` for read-only connection.
> * `esp_http_client_write()`: Write data to server with a maximum length equal to `write_len` of `esp_http_client_open()` function; no need to call this function for `write_len=0`.
> * `esp_http_client_fetch_headers()`: Read the HTTP Server response headers, after sending the request headers and server data (if any). Returns the `content-length` from the server and can be succeeded by `esp_http_client_get_status_code()` for getting the HTTP status of the connection.
> * `esp_http_client_read()`: Read the HTTP stream.
> * `esp_http_client_close()`: Close the connection.
> * `esp_http_client_cleanup()`: Release allocated resources.


Check out the example function `http_perform_as_stream_reader` in the application example for implementation details.




## HTTP Authentication



ESP HTTP client supports both **Basic** and **Digest** Authentication.* Users can provide the username and password in the `url` or the `username` and `password` members of the `esp_http_client_config_t` configuration. For `auth_type = HTTP_AUTH_TYPE_BASIC`, the HTTP client takes only one perform operation to pass the authentication process.
* If `auth_type = HTTP_AUTH_TYPE_NONE`, but the `username` and `password` fields are present in the configuration, the HTTP client takes two perform operations. The client will receive the `401 Unauthorized` header in its first attempt to connect to the server. Based on this information, it decides which authentication method to choose and performs it in the second operation.
* Check out the example functions `http_auth_basic`, `http_auth_basic_redirect` (for Basic authentication) and `http_auth_digest` (for Digest authentication) in the application example for implementation details.
* Currently, Digest authentication supports only MD5 and SHA-256 algorithms.





### Examples of Authentication Configuration



> * Authentication with URI
> 
> 
> 
> > ```
> > esp_http_client_config_t config = {
> >     .url = "http://user:passwd@httpbin.org/basic-auth/user/passwd",
> >     .auth_type = HTTP_AUTH_TYPE_BASIC,
> > };
> > 
> > ```
> * Authentication with username and password entry
> 
> 
> 
> > ```
> > esp_http_client_config_t config = {
> >     .url = "http://httpbin.org/basic-auth/user/passwd",
> >     .username = "user",
> >     .password = "passwd",
> >     .auth_type = HTTP_AUTH_TYPE_BASIC,
> > };
> > 
> > ```





## Event Handling


ESP HTTP Client supports event handling by triggering an event handler corresponding to the event which takes place. `esp_http_client_event_id_t` contains all the events which could occur while performing an HTTP request using the ESP HTTP Client.


To enable event handling, you just need to set a callback function using the `esp_http_client_config_t::event_handler` member.




## ESP HTTP Client Diagnostic Information


Diagnostic information could be helpful to gain insights into a problem. In the case of ESP HTTP Client, the diagnostic information can be collected by registering an event handler with the Event Loop library. This feature has been added by keeping in mind the ESP Insights framework which collects the diagnostic information. However, this feature can also be used without any dependency on the ESP Insights framework for the diagnostic purpose. Event handler can be registered to the event loop using the `esp_event_handler_register()` function.


Expected data types for different HTTP Client events in the event loop are as follows:



> * HTTP_EVENT_ERROR : `esp_http_client_handle_t`
> * HTTP_EVENT_ON_CONNECTED : `esp_http_client_handle_t`
> * HTTP_EVENT_HEADERS_SENT : `esp_http_client_handle_t`
> * HTTP_EVENT_ON_HEADER : `esp_http_client_handle_t`
> * HTTP_EVENT_ON_DATA : `esp_http_client_on_data_t`
> * HTTP_EVENT_ON_FINISH : `esp_http_client_handle_t`
> * HTTP_EVENT_DISCONNECTED : `esp_http_client_handle_t`
> * HTTP_EVENT_REDIRECT : `esp_http_client_redirect_event_data_t`


The `esp_http_client_handle_t` received along with the event data will be valid until `HTTP_EVENT_DISCONNECTED` is not received. This handle has been sent primarily to differentiate between different client connections and must not be used for any other purpose, as it may change based on client connection state.




## TLS Protocol Version


TLS protocol version to be used for the underlying TLS connection can be set in `esp_http_client_config_t`. Please refer to the **TLS Protocol Version** section in the ESP-TLS for more details.


The TLS protocol version for the HTTP client can be configured as follows:



> ```
> #include "esp_http_client.h"
> esp_http_client_config_t config = {
>     .tls_version = ESP_HTTP_CLIENT_TLS_VER_TLS_1_2,
> };
> 
> ```




## API Reference



### Header File


* components/esp_http_client/include/esp_http_client.h
* This header file can be included with:



> ```
> #include "esp_http_client.h"
> 
> ```
* This header file is a part of the API provided by the `esp_http_client` component. To declare that your component depends on `esp_http_client`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_http_client
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_http_client
> 
> ```




### Functions


#### esp_http_client_init


```


esp_http_client_handle_t esp_http_client_init(const esp_http_client_config_t *config)
Start a HTTP session This function must be the first function to call, and it returns a esp_http_client_handle_t that you must use as input to other functions in the interface. This call MUST have a corresponding call to esp_http_client_cleanup when the operation is complete. 

Parameters
config -- [in] The configurations, see http_client_config_t

Returns

esp_http_client_handle_t
NULL if any errors 





```

#### esp_http_client_perform


```


esp_err_t esp_http_client_perform(esp_http_client_handle_t client)
Invoke this function after esp_http_client_init and all the options calls are made, and will perform the transfer as described in the options. It must be called with the same esp_http_client_handle_t as input as the esp_http_client_init call returned. esp_http_client_perform performs the entire request in either blocking or non-blocking manner. By default, the API performs request in a blocking manner and returns when done, or if it failed, and in non-blocking manner, it returns if EAGAIN/EWOULDBLOCK or EINPROGRESS is encountered, or if it failed. And in case of non-blocking request, the user may call this API multiple times unless request & response is complete or there is a failure. To enable non-blocking esp_http_client_perform(), is_async member of esp_http_client_config_t must be set while making a call to esp_http_client_init() API. You can do any amount of calls to esp_http_client_perform while using the same esp_http_client_handle_t. The underlying connection may be kept open if the server allows it. If you intend to transfer more than one file, you are even encouraged to do so. esp_http_client will then attempt to reuse the same connection for the following transfers, thus making the operations faster, less CPU intense and using less network resources. Just note that you will have to use esp_http_client_set_** between the invokes to set options for the following esp_http_client_perform. 

Note
You must never call this function simultaneously from two places using the same client handle. Let the function return first before invoking it another time. If you want parallel transfers, you must use several esp_http_client_handle_t. This function include esp_http_client_open -> esp_http_client_write -> esp_http_client_fetch_headers -> esp_http_client_read (and option) esp_http_client_close.


Parameters
client -- The esp_http_client handle

Returns

ESP_OK on successful
ESP_FAIL on error 





```

#### esp_http_client_cancel_request


```


esp_err_t esp_http_client_cancel_request(esp_http_client_handle_t client)
Cancel an ongoing HTTP request. This API closes the current socket and opens a new socket with the same esp_http_client context. 

Parameters
client -- The esp_http_client handle 

Returns

ESP_OK on successful
ESP_FAIL on error
ESP_ERR_INVALID_ARG
ESP_ERR_INVALID_STATE 





```

#### esp_http_client_set_url


```


esp_err_t esp_http_client_set_url(esp_http_client_handle_t client, const char *url)
Set URL for client, when performing this behavior, the options in the URL will replace the old ones. 

Parameters

client -- [in] The esp_http_client handle 
url -- [in] The url


Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_set_post_field


```


esp_err_t esp_http_client_set_post_field(esp_http_client_handle_t client, const char *data, int len)
Set post data, this function must be called before esp_http_client_perform. Note: The data parameter passed to this function is a pointer and this function will not copy the data. 

Parameters

client -- [in] The esp_http_client handle 
data -- [in] post data pointer 
len -- [in] post length


Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_get_post_field


```


int esp_http_client_get_post_field(esp_http_client_handle_t client, char **data)
Get current post field information. 

Parameters

client -- [in] The client 
data -- [out] Point to post data pointer


Returns
Size of post data 



```

#### esp_http_client_set_header


```


esp_err_t esp_http_client_set_header(esp_http_client_handle_t client, const char *key, const char *value)
Set http request header, this function must be called after esp_http_client_init and before any perform function. 

Parameters

client -- [in] The esp_http_client handle 
key -- [in] The header key 
value -- [in] The header value


Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_get_header


```


esp_err_t esp_http_client_get_header(esp_http_client_handle_t client, const char *key, char **value)
Get http request header. The value parameter will be set to NULL if there is no header which is same as the key specified, otherwise the address of header value will be assigned to value parameter. This function must be called after esp_http_client_init. 

Parameters

client -- [in] The esp_http_client handle 
key -- [in] The header key 
value -- [out] The header value


Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_get_username


```


esp_err_t esp_http_client_get_username(esp_http_client_handle_t client, char **value)
Get http request username. The address of username buffer will be assigned to value parameter. This function must be called after esp_http_client_init. 

Parameters

client -- [in] The esp_http_client handle 
value -- [out] The username value


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_set_username


```


esp_err_t esp_http_client_set_username(esp_http_client_handle_t client, const char *username)
Set http request username. The value of username parameter will be assigned to username buffer. If the username parameter is NULL then username buffer will be freed. 

Parameters

client -- [in] The esp_http_client handle 
username -- [in] The username value


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_get_password


```


esp_err_t esp_http_client_get_password(esp_http_client_handle_t client, char **value)
Get http request password. The address of password buffer will be assigned to value parameter. This function must be called after esp_http_client_init. 

Parameters

client -- [in] The esp_http_client handle 
value -- [out] The password value


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_set_password


```


esp_err_t esp_http_client_set_password(esp_http_client_handle_t client, const char *password)
Set http request password. The value of password parameter will be assigned to password buffer. If the password parameter is NULL then password buffer will be freed. 

Parameters

client -- [in] The esp_http_client handle 
password -- [in] The password value


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_set_authtype


```


esp_err_t esp_http_client_set_authtype(esp_http_client_handle_t client, esp_http_client_auth_type_t auth_type)
Set http request auth_type. 

Parameters

client -- [in] The esp_http_client handle 
auth_type -- [in] The esp_http_client auth type


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_get_user_data


```


esp_err_t esp_http_client_get_user_data(esp_http_client_handle_t client, void **data)
Get http request user_data. The value stored from the esp_http_client_config_t will be written to the address passed into data. 

Parameters

client -- [in] The esp_http_client handle 
data -- [out] A pointer to the pointer that will be set to user_data.


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_set_user_data


```


esp_err_t esp_http_client_set_user_data(esp_http_client_handle_t client, void *data)
Set http request user_data. The value passed in +data+ will be available during event callbacks. No memory management will be performed on the user's behalf. 

Parameters

client -- [in] The esp_http_client handle 
data -- [in] The pointer to the user data


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_get_errno


```


int esp_http_client_get_errno(esp_http_client_handle_t client)
Get HTTP client session errno. 

Parameters
client -- [in] The esp_http_client handle

Returns

(-1) if invalid argument
errno 





```

#### esp_http_client_set_method


```


esp_err_t esp_http_client_set_method(esp_http_client_handle_t client, esp_http_client_method_t method)
Set http request method. 

Parameters

client -- [in] The esp_http_client handle 
method -- [in] The method


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_set_timeout_ms


```


esp_err_t esp_http_client_set_timeout_ms(esp_http_client_handle_t client, int timeout_ms)
Set http request timeout. 

Parameters

client -- [in] The esp_http_client handle 
timeout_ms -- [in] The timeout value


Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_delete_header


```


esp_err_t esp_http_client_delete_header(esp_http_client_handle_t client, const char *key)
Delete http request header. 

Parameters

client -- [in] The esp_http_client handle 
key -- [in] The key


Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_open


```


esp_err_t esp_http_client_open(esp_http_client_handle_t client, int write_len)
This function will be open the connection, write all header strings and return. 

Parameters

client -- [in] The esp_http_client handle 
write_len -- [in] HTTP Content length need to write to the server


Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_write


```


int esp_http_client_write(esp_http_client_handle_t client, const char *buffer, int len)
This function will write data to the HTTP connection previously opened by esp_http_client_open() 

Parameters

client -- [in] The esp_http_client handle 
buffer -- The buffer 
len -- [in] This value must not be larger than the write_len parameter provided to esp_http_client_open()


Returns

(-1) if any errors
Length of data written 





```

#### esp_http_client_fetch_headers


```


int64_t esp_http_client_fetch_headers(esp_http_client_handle_t client)
This function need to call after esp_http_client_open, it will read from http stream, process all receive headers. 

Parameters
client -- [in] The esp_http_client handle

Returns

(0) if stream doesn't contain content-length header, or chunked encoding (checked by esp_http_client_is_chunked response)
(-1: ESP_FAIL) if any errors
(-ESP_ERR_HTTP_EAGAIN = -0x7007) if call is timed-out before any data was ready
Download data length defined by content-length header 





```

#### esp_http_client_is_chunked_response


```


bool esp_http_client_is_chunked_response(esp_http_client_handle_t client)
Check response data is chunked. 

Parameters
client -- [in] The esp_http_client handle

Returns
true or false 



```

#### esp_http_client_read


```


int esp_http_client_read(esp_http_client_handle_t client, char *buffer, int len)
Read data from http stream. 

Note
(-ESP_ERR_HTTP_EAGAIN = -0x7007) is returned when call is timed-out before any data was ready 


Parameters

client -- [in] The esp_http_client handle 
buffer -- The buffer 
len -- [in] The length


Returns

(-1) if any errors
Length of data was read





```

#### esp_http_client_get_status_code


```


int esp_http_client_get_status_code(esp_http_client_handle_t client)
Get http response status code, the valid value if this function invoke after esp_http_client_perform

Parameters
client -- [in] The esp_http_client handle

Returns
Status code 



```

#### esp_http_client_get_content_length


```


int64_t esp_http_client_get_content_length(esp_http_client_handle_t client)
Get http response content length (from header Content-Length) the valid value if this function invoke after esp_http_client_perform

Parameters
client -- [in] The esp_http_client handle

Returns

(-1) Chunked transfer
Content-Length value as bytes 





```

#### esp_http_client_close


```


esp_err_t esp_http_client_close(esp_http_client_handle_t client)
Close http connection, still kept all http request resources. 

Parameters
client -- [in] The esp_http_client handle

Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_cleanup


```


esp_err_t esp_http_client_cleanup(esp_http_client_handle_t client)
This function must be the last function to call for an session. It is the opposite of the esp_http_client_init function and must be called with the same handle as input that a esp_http_client_init call returned. This might close all connections this handle has used and possibly has kept open until now. Don't call this function if you intend to transfer more files, re-using handles is a key to good performance with esp_http_client. 

Parameters
client -- [in] The esp_http_client handle

Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_get_transport_type


```


esp_http_client_transport_t esp_http_client_get_transport_type(esp_http_client_handle_t client)
Get transport type. 

Parameters
client -- [in] The esp_http_client handle

Returns

HTTP_TRANSPORT_UNKNOWN
HTTP_TRANSPORT_OVER_TCP
HTTP_TRANSPORT_OVER_SSL 





```

#### esp_http_client_set_redirection


```


esp_err_t esp_http_client_set_redirection(esp_http_client_handle_t client)
Set redirection URL. When received the 30x code from the server, the client stores the redirect URL provided by the server. This function will set the current URL to redirect to enable client to execute the redirection request. When disable_auto_redirect is set, the client will not call this function but the event HTTP_EVENT_REDIRECT will be dispatched giving the user control over the redirection event. 

Parameters
client -- [in] The esp_http_client handle

Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_reset_redirect_counter


```


esp_err_t esp_http_client_reset_redirect_counter(esp_http_client_handle_t client)
Reset the redirection counter. This is useful to reset redirect counter in cases where the same handle is used for multiple requests. 

Parameters
client -- [in] The esp_http_client handle

Returns

ESP_OK
ESP_ERR_INVALID_ARG 





```

#### esp_http_client_set_auth_data


```


esp_err_t esp_http_client_set_auth_data(esp_http_client_handle_t client, const char *auth_data, int len)
On receiving a custom authentication header, this API can be invoked to set the authentication information from the header. This API can be called from the event handler. 

Parameters

client -- [in] The esp_http_client handle 
auth_data -- [in] The authentication data received in the header 
len -- [in] length of auth_data.


Returns

ESP_ERR_INVALID_ARG
ESP_OK 





```

#### esp_http_client_add_auth


```


void esp_http_client_add_auth(esp_http_client_handle_t client)
On receiving HTTP Status code 401, this API can be invoked to add authorization information. 

Note
There is a possibility of receiving body message with redirection status codes, thus make sure to flush off body data after calling this API.


Parameters
client -- [in] The esp_http_client handle 



```

#### esp_http_client_is_complete_data_received


```


bool esp_http_client_is_complete_data_received(esp_http_client_handle_t client)
Checks if entire data in the response has been read without any error. 

Parameters
client -- [in] The esp_http_client handle

Returns

true
false 





```

#### esp_http_client_read_response


```


int esp_http_client_read_response(esp_http_client_handle_t client, char *buffer, int len)
Helper API to read larger data chunks This is a helper API which internally calls esp_http_client_read multiple times till the end of data is reached or till the buffer gets full. 

Parameters

client -- [in] The esp_http_client handle 
buffer -- The buffer 
len -- [in] The buffer length


Returns

Length of data was read 





```

#### esp_http_client_flush_response


```


esp_err_t esp_http_client_flush_response(esp_http_client_handle_t client, int *len)
Process all remaining response data This uses an internal buffer to repeatedly receive, parse, and discard response data until complete data is processed. As no additional user-supplied buffer is required, this may be preferable to esp_http_client_read_response in situations where the content of the response may be ignored. 

Parameters

client -- [in] The esp_http_client handle 
len -- Length of data discarded


Returns

ESP_OK If successful, len will have discarded length
ESP_FAIL If failed to read response
ESP_ERR_INVALID_ARG If the client is NULL 





```

#### esp_http_client_get_url


```


esp_err_t esp_http_client_get_url(esp_http_client_handle_t client, char *url, const int len)
Get URL from client. 

Parameters

client -- [in] The esp_http_client handle 
url -- [inout] The buffer to store URL 
len -- [in] The buffer length


Returns

ESP_OK
ESP_FAIL 





```

#### esp_http_client_get_chunk_length


```


esp_err_t esp_http_client_get_chunk_length(esp_http_client_handle_t client, int *len)
Get Chunk-Length from client. 

Parameters

client -- [in] The esp_http_client handle 
len -- [out] Variable to store length


Returns

ESP_OK If successful, len will have length of current chunk
ESP_FAIL If the server is not a chunked server
ESP_ERR_INVALID_ARG If the client or len are NULL 





```



### Structures


#### esp_http_client_event


```


struct esp_http_client_event
HTTP Client events data. 

Public Members
event_id

esp_http_client_event_id_t event_id
event_id, to know the cause of the event 

client

esp_http_client_handle_t client
esp_http_client_handle_t context 

data

void *data
data of the event 

data_len

int data_len
data length of data 

user_data

void *user_data
user_data context, from esp_http_client_config_t user_data 

header_key

char *header_key
For HTTP_EVENT_ON_HEADER event_id, it's store current http header key 

header_value

char *header_value
For HTTP_EVENT_ON_HEADER event_id, it's store current http header value 



```

#### esp_http_client_on_data


```


struct esp_http_client_on_data
Argument structure for HTTP_EVENT_ON_DATA event. 

Public Members
client

esp_http_client_handle_t client
Client handle 

data_process

int64_t data_process
Total data processed 



```

#### esp_http_client_redirect_event_data


```


struct esp_http_client_redirect_event_data
Argument structure for HTTP_EVENT_REDIRECT event. 

Public Members
client

esp_http_client_handle_t client
Client handle 

status_code

int status_code
Status Code 



```

#### esp_http_client_config_t


```


struct esp_http_client_config_t
HTTP configuration. 

Public Members
url

const char *url
HTTP URL, the information on the URL is most important, it overrides the other fields below, if any 

host

const char *host
Domain or IP as string 

port

int port
Port to connect, default depend on esp_http_client_transport_t (80 or 443) 

username

const char *username
Using for Http authentication 

password

const char *password
Using for Http authentication 

auth_type

esp_http_client_auth_type_t auth_type
Http authentication type, see esp_http_client_auth_type_t

path

const char *path
HTTP Path, if not set, default is /

query

const char *query
HTTP query 

cert_pem

const char *cert_pem
SSL server certification, PEM format as string, if the client requires to verify server 

cert_len

size_t cert_len
Length of the buffer pointed to by cert_pem. May be 0 for null-terminated pem 

client_cert_pem

const char *client_cert_pem
SSL client certification, PEM format as string, if the server requires to verify client 

client_cert_len

size_t client_cert_len
Length of the buffer pointed to by client_cert_pem. May be 0 for null-terminated pem 

client_key_pem

const char *client_key_pem
SSL client key, PEM format as string, if the server requires to verify client 

client_key_len

size_t client_key_len
Length of the buffer pointed to by client_key_pem. May be 0 for null-terminated pem 

client_key_password

const char *client_key_password
Client key decryption password string 

client_key_password_len

size_t client_key_password_len
String length of the password pointed to by client_key_password 

tls_version

esp_http_client_proto_ver_t tls_version
TLS protocol version of the connection, e.g., TLS 1.2, TLS 1.3 (default - no preference) 

user_agent

const char *user_agent
The User Agent string to send with HTTP requests 

method

esp_http_client_method_t method
HTTP Method 

timeout_ms

int timeout_ms
Network timeout in milliseconds 

disable_auto_redirect

bool disable_auto_redirect
Disable HTTP automatic redirects 

max_redirection_count

int max_redirection_count
Max number of redirections on receiving HTTP redirect status code, using default value if zero 

max_authorization_retries

int max_authorization_retries
Max connection retries on receiving HTTP unauthorized status code, using default value if zero. Disables authorization retry if -1 

event_handler

http_event_handle_cb event_handler
HTTP Event Handle 

transport_type

esp_http_client_transport_t transport_type
HTTP transport type, see esp_http_client_transport_t

buffer_size

int buffer_size
HTTP receive buffer size 

buffer_size_tx

int buffer_size_tx
HTTP transmit buffer size 

user_data

void *user_data
HTTP user_data context 

is_async

bool is_async
Set asynchronous mode, only supported with HTTPS for now 

use_global_ca_store

bool use_global_ca_store
Use a global ca_store for all the connections in which this bool is set. 

skip_cert_common_name_check

bool skip_cert_common_name_check
Skip any validation of server certificate CN field 

common_name

const char *common_name
Pointer to the string containing server certificate common name. If non-NULL, server certificate CN must match this name, If NULL, server certificate CN must match hostname. 

crt_bundle_attach

esp_err_t (*crt_bundle_attach)(void *conf)
Function pointer to esp_crt_bundle_attach. Enables the use of certification bundle for server verification, must be enabled in menuconfig 

keep_alive_enable

bool keep_alive_enable
Enable keep-alive timeout 

keep_alive_idle

int keep_alive_idle
Keep-alive idle time. Default is 5 (second) 

keep_alive_interval

int keep_alive_interval
Keep-alive interval time. Default is 5 (second) 

keep_alive_count

int keep_alive_count
Keep-alive packet retry send count. Default is 3 counts 

if_name

struct ifreq *if_name
The name of interface for data to go through. Use the default interface without setting 

ds_data

void *ds_data
Pointer for digital signature peripheral context, see ESP-TLS Documentation for more details 



```



### Macros


#### DEFAULT_HTTP_BUF_SIZE


```


DEFAULT_HTTP_BUF_SIZE

```

#### ESP_ERR_HTTP_BASE


```


ESP_ERR_HTTP_BASE
Starting number of HTTP error codes 

```

#### ESP_ERR_HTTP_MAX_REDIRECT


```


ESP_ERR_HTTP_MAX_REDIRECT
The error exceeds the number of HTTP redirects 

```

#### ESP_ERR_HTTP_CONNECT


```


ESP_ERR_HTTP_CONNECT
Error open the HTTP connection 

```

#### ESP_ERR_HTTP_WRITE_DATA


```


ESP_ERR_HTTP_WRITE_DATA
Error write HTTP data 

```

#### ESP_ERR_HTTP_FETCH_HEADER


```


ESP_ERR_HTTP_FETCH_HEADER
Error read HTTP header from server 

```

#### ESP_ERR_HTTP_INVALID_TRANSPORT


```


ESP_ERR_HTTP_INVALID_TRANSPORT
There are no transport support for the input scheme 

```

#### ESP_ERR_HTTP_CONNECTING


```


ESP_ERR_HTTP_CONNECTING
HTTP connection hasn't been established yet 

```

#### ESP_ERR_HTTP_EAGAIN


```


ESP_ERR_HTTP_EAGAIN
Mapping of errno EAGAIN to esp_err_t 

```

#### ESP_ERR_HTTP_CONNECTION_CLOSED


```


ESP_ERR_HTTP_CONNECTION_CLOSED
Read FIN from peer and the connection closed 

```



### Type Definitions


#### esp_http_client_handle_t


```


typedef struct esp_http_client *esp_http_client_handle_t

```

#### esp_http_client_event_handle_t


```


typedef struct esp_http_client_event *esp_http_client_event_handle_t

```

#### esp_http_client_event_t


```


typedef struct esp_http_client_event esp_http_client_event_t
HTTP Client events data. 

```

#### esp_http_client_on_data_t


```


typedef struct esp_http_client_on_data esp_http_client_on_data_t
Argument structure for HTTP_EVENT_ON_DATA event. 

```

#### esp_http_client_redirect_event_data_t


```


typedef struct esp_http_client_redirect_event_data esp_http_client_redirect_event_data_t
Argument structure for HTTP_EVENT_REDIRECT event. 

```

#### http_event_handle_cb


```


typedef esp_err_t (*http_event_handle_cb)(esp_http_client_event_t *evt)

```



### Enumerations


#### esp_http_client_event_id_t


```


enum esp_http_client_event_id_t
HTTP Client events id. 
Values:
HTTP_EVENT_ERROR

enumerator HTTP_EVENT_ERROR
This event occurs when there are any errors during execution 

HTTP_EVENT_ON_CONNECTED

enumerator HTTP_EVENT_ON_CONNECTED
Once the HTTP has been connected to the server, no data exchange has been performed 

HTTP_EVENT_HEADERS_SENT

enumerator HTTP_EVENT_HEADERS_SENT
After sending all the headers to the server 

HTTP_EVENT_HEADER_SENT

enumerator HTTP_EVENT_HEADER_SENT
This header has been kept for backward compatibility and will be deprecated in future versions esp-idf 

HTTP_EVENT_ON_HEADER

enumerator HTTP_EVENT_ON_HEADER
Occurs when receiving each header sent from the server 

HTTP_EVENT_ON_DATA

enumerator HTTP_EVENT_ON_DATA
Occurs when receiving data from the server, possibly multiple portions of the packet 

HTTP_EVENT_ON_FINISH

enumerator HTTP_EVENT_ON_FINISH
Occurs when finish a HTTP session 

HTTP_EVENT_DISCONNECTED

enumerator HTTP_EVENT_DISCONNECTED
The connection has been disconnected 

HTTP_EVENT_REDIRECT

enumerator HTTP_EVENT_REDIRECT
Intercepting HTTP redirects to handle them manually 


```

#### esp_http_client_transport_t


```


enum esp_http_client_transport_t
HTTP Client transport. 
Values:
HTTP_TRANSPORT_UNKNOWN

enumerator HTTP_TRANSPORT_UNKNOWN
Unknown 

HTTP_TRANSPORT_OVER_TCP

enumerator HTTP_TRANSPORT_OVER_TCP
Transport over tcp 

HTTP_TRANSPORT_OVER_SSL

enumerator HTTP_TRANSPORT_OVER_SSL
Transport over ssl 


```

#### esp_http_client_proto_ver_t


```


enum esp_http_client_proto_ver_t
Values:
ESP_HTTP_CLIENT_TLS_VER_ANY

enumerator ESP_HTTP_CLIENT_TLS_VER_ANY

ESP_HTTP_CLIENT_TLS_VER_TLS_1_2

enumerator ESP_HTTP_CLIENT_TLS_VER_TLS_1_2

ESP_HTTP_CLIENT_TLS_VER_TLS_1_3

enumerator ESP_HTTP_CLIENT_TLS_VER_TLS_1_3

ESP_HTTP_CLIENT_TLS_VER_MAX

enumerator ESP_HTTP_CLIENT_TLS_VER_MAX


```

#### esp_http_client_method_t


```


enum esp_http_client_method_t
HTTP method. 
Values:
HTTP_METHOD_GET

enumerator HTTP_METHOD_GET
HTTP GET Method 

HTTP_METHOD_POST

enumerator HTTP_METHOD_POST
HTTP POST Method 

HTTP_METHOD_PUT

enumerator HTTP_METHOD_PUT
HTTP PUT Method 

HTTP_METHOD_PATCH

enumerator HTTP_METHOD_PATCH
HTTP PATCH Method 

HTTP_METHOD_DELETE

enumerator HTTP_METHOD_DELETE
HTTP DELETE Method 

HTTP_METHOD_HEAD

enumerator HTTP_METHOD_HEAD
HTTP HEAD Method 

HTTP_METHOD_NOTIFY

enumerator HTTP_METHOD_NOTIFY
HTTP NOTIFY Method 

HTTP_METHOD_SUBSCRIBE

enumerator HTTP_METHOD_SUBSCRIBE
HTTP SUBSCRIBE Method 

HTTP_METHOD_UNSUBSCRIBE

enumerator HTTP_METHOD_UNSUBSCRIBE
HTTP UNSUBSCRIBE Method 

HTTP_METHOD_OPTIONS

enumerator HTTP_METHOD_OPTIONS
HTTP OPTIONS Method 

HTTP_METHOD_COPY

enumerator HTTP_METHOD_COPY
HTTP COPY Method 

HTTP_METHOD_MOVE

enumerator HTTP_METHOD_MOVE
HTTP MOVE Method 

HTTP_METHOD_LOCK

enumerator HTTP_METHOD_LOCK
HTTP LOCK Method 

HTTP_METHOD_UNLOCK

enumerator HTTP_METHOD_UNLOCK
HTTP UNLOCK Method 

HTTP_METHOD_PROPFIND

enumerator HTTP_METHOD_PROPFIND
HTTP PROPFIND Method 

HTTP_METHOD_PROPPATCH

enumerator HTTP_METHOD_PROPPATCH
HTTP PROPPATCH Method 

HTTP_METHOD_MKCOL

enumerator HTTP_METHOD_MKCOL
HTTP MKCOL Method 

HTTP_METHOD_MAX

enumerator HTTP_METHOD_MAX


```

#### esp_http_client_auth_type_t


```


enum esp_http_client_auth_type_t
HTTP Authentication type. 
Values:
HTTP_AUTH_TYPE_NONE

enumerator HTTP_AUTH_TYPE_NONE
No authention 

HTTP_AUTH_TYPE_BASIC

enumerator HTTP_AUTH_TYPE_BASIC
HTTP Basic authentication 

HTTP_AUTH_TYPE_DIGEST

enumerator HTTP_AUTH_TYPE_DIGEST
HTTP Digest authentication 


```

#### HttpStatus_Code


```


enum HttpStatus_Code
Enum for the HTTP status codes. 
Values:
HttpStatus_Ok

enumerator HttpStatus_Ok

HttpStatus_MultipleChoices

enumerator HttpStatus_MultipleChoices

HttpStatus_MovedPermanently

enumerator HttpStatus_MovedPermanently

HttpStatus_Found

enumerator HttpStatus_Found

HttpStatus_SeeOther

enumerator HttpStatus_SeeOther

HttpStatus_TemporaryRedirect

enumerator HttpStatus_TemporaryRedirect

HttpStatus_PermanentRedirect

enumerator HttpStatus_PermanentRedirect

HttpStatus_BadRequest

enumerator HttpStatus_BadRequest

HttpStatus_Unauthorized

enumerator HttpStatus_Unauthorized

HttpStatus_Forbidden

enumerator HttpStatus_Forbidden

HttpStatus_NotFound

enumerator HttpStatus_NotFound

HttpStatus_InternalError

enumerator HttpStatus_InternalError


```