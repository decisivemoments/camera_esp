# HTTP Server




## Overview


The HTTP Server component provides an ability for running a lightweight web server on ESP32-S3. Following are detailed steps to use the API exposed by HTTP Server:



> * `httpd_start()`: Creates an instance of HTTP server, allocate memory/resources for it depending upon the specified configuration and outputs a handle to the server instance. The server has both, a listening socket (TCP) for HTTP traffic, and a control socket (UDP) for control signals, which are selected in a round robin fashion in the server task loop. The task priority and stack size are configurable during server instance creation by passing `httpd_config_t` structure to `httpd_start()`. TCP traffic is parsed as HTTP requests and, depending on the requested URI, user registered handlers are invoked which are supposed to send back HTTP response packets.
> * `httpd_stop()`: This stops the server with the provided handle and frees up any associated memory/resources. This is a blocking function that first signals a halt to the server task and then waits for the task to terminate. While stopping, the task closes all open connections, removes registered URI handlers and resets all session context data to empty.
> * `httpd_register_uri_handler()`: A URI handler is registered by passing object of type `httpd_uri_t` structure which has members including `uri` name, `method` type (eg. `HTTPD_GET/HTTPD_POST/HTTPD_PUT` etc.), function pointer of type `esp_err_t *handler (httpd_req_t *req)` and `user_ctx` pointer to user context data.




## Application Example



> ```
> /* Our URI handler function to be called during GET /uri request */
> esp_err_t get_handler(httpd_req_t *req)
> {
>     /* Send a simple response */
>     const char resp[] = "URI GET Response";
>     httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
>     return ESP_OK;
> }
> 
> /* Our URI handler function to be called during POST /uri request */
> esp_err_t post_handler(httpd_req_t *req)
> {
>     /* Destination buffer for content of HTTP POST request.
>      * httpd_req_recv() accepts char* only, but content could
>      * as well be any binary data (needs type casting).
>      * In case of string data, null termination will be absent, and
>      * content length would give length of string */
>     char content[100];
> 
>     /* Truncate if content length larger than the buffer */
>     size_t recv_size = MIN(req->content_len, sizeof(content));
> 
>     int ret = httpd_req_recv(req, content, recv_size);
>     if (ret <= 0) {  /* 0 return value indicates connection closed */
>         /* Check if timeout occurred */
>         if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
>             /* In case of timeout one can choose to retry calling
>              * httpd_req_recv(), but to keep it simple, here we
>              * respond with an HTTP 408 (Request Timeout) error */
>             httpd_resp_send_408(req);
>         }
>         /* In case of error, returning ESP_FAIL will
>          * ensure that the underlying socket is closed */
>         return ESP_FAIL;
>     }
> 
>     /* Send a simple response */
>     const char resp[] = "URI POST Response";
>     httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
>     return ESP_OK;
> }
> 
> /* URI handler structure for GET /uri */
> httpd_uri_t uri_get = {
>     .uri      = "/uri",
>     .method   = HTTP_GET,
>     .handler  = get_handler,
>     .user_ctx = NULL
> };
> 
> /* URI handler structure for POST /uri */
> httpd_uri_t uri_post = {
>     .uri      = "/uri",
>     .method   = HTTP_POST,
>     .handler  = post_handler,
>     .user_ctx = NULL
> };
> 
> /* Function for starting the webserver */
> httpd_handle_t start_webserver(void)
> {
>     /* Generate default configuration */
>     httpd_config_t config = HTTPD_DEFAULT_CONFIG();
> 
>     /* Empty handle to esp_http_server */
>     httpd_handle_t server = NULL;
> 
>     /* Start the httpd server */
>     if (httpd_start(&server, &config) == ESP_OK) {
>         /* Register URI handlers */
>         httpd_register_uri_handler(server, &uri_get);
>         httpd_register_uri_handler(server, &uri_post);
>     }
>     /* If server failed to start, handle will be NULL */
>     return server;
> }
> 
> /* Function for stopping the webserver */
> void stop_webserver(httpd_handle_t server)
> {
>     if (server) {
>         /* Stop the httpd server */
>         httpd_stop(server);
>     }
> }
> 
> ```



### Simple HTTP Server Example


Check HTTP server example under protocols/http_server/simple where handling of arbitrary content lengths, reading request headers and URL query parameters, and setting response headers is demonstrated.





## Persistent Connections


HTTP server features persistent connections, allowing for the reuse of the same connection (session) for several transfers, all the while maintaining context specific data for the session. Context data may be allocated dynamically by the handler in which case a custom function may need to be specified for freeing this data when the connection/session is closed.



### Persistent Connections Example



```
/* Custom function to free context */
void free_ctx_func(void *ctx)
{
    /* Could be something other than free */
    free(ctx);
}

esp_err_t adder_post_handler(httpd_req_t *req)
{
    /* Create session's context if not already available */
    if (! req->sess_ctx) {
        req->sess_ctx = malloc(sizeof(ANY_DATA_TYPE));  /*!< Pointer to context data */
        req->free_ctx = free_ctx_func;                  /*!< Function to free context data */
    }

    /* Access context data */
    ANY_DATA_TYPE *ctx_data = (ANY_DATA_TYPE *)req->sess_ctx;

    /* Respond */
    ...............
    ...............
    ...............

    return ESP_OK;
}

```


Check the example under protocols/http_server/persistent_sockets.





## Websocket Server


The HTTP server component provides websocket support. The websocket feature can be enabled in menuconfig using the CONFIG_HTTPD_WS_SUPPORT option. Please refer to the protocols/http_server/ws_echo_server example which demonstrates usage of the websocket feature.




## Event Handling


ESP HTTP server has various events for which a handler can be triggered by the Event Loop library when the particular event occurs. The handler has to be registered using `esp_event_handler_register()`. This helps in event handling for ESP HTTP server.


`esp_http_server_event_id_t` has all the events which can happen for ESP HTTP server.


Expected data type for different ESP HTTP server events in event loop:



> * HTTP_SERVER_EVENT_ERROR : `httpd_err_code_t`
> * HTTP_SERVER_EVENT_START : `NULL`
> * HTTP_SERVER_EVENT_ON_CONNECTED : `int`
> * HTTP_SERVER_EVENT_ON_HEADER : `int`
> * HTTP_SERVER_EVENT_HEADERS_SENT : `int`
> * HTTP_SERVER_EVENT_ON_DATA : `esp_http_server_event_data`
> * HTTP_SERVER_EVENT_SENT_DATA : `esp_http_server_event_data`
> * HTTP_SERVER_EVENT_DISCONNECTED : `int`
> * HTTP_SERVER_EVENT_STOP : `NULL`




## API Reference



### Header File


* components/esp_http_server/include/esp_http_server.h
* This header file can be included with:



> ```
> #include "esp_http_server.h"
> 
> ```
* This header file is a part of the API provided by the `esp_http_server` component. To declare that your component depends on `esp_http_server`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_http_server
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_http_server
> 
> ```




### Functions


#### httpd_start


```


esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config)
Starts the web server. 
Create an instance of HTTP server and allocate memory/resources for it depending upon the specified configuration.
Example usage: //Function for starting the webserver
httpd_handle_t start_webserver(void)
{
     // Generate default configuration
     httpd_config_t config = HTTPD_DEFAULT_CONFIG();

     // Empty handle to http_server
     httpd_handle_t server = NULL;

     // Start the httpd server
     if (httpd_start(&server, &config) == ESP_OK) {
         // Register URI handlers
         httpd_register_uri_handler(server, &uri_get);
         httpd_register_uri_handler(server, &uri_post);
     }
     // If server failed to start, handle will be NULL
     return server;
}




Parameters

config -- [in] Configuration for new instance of the server 
handle -- [out] Handle to newly created instance of the server. NULL on error 


Returns

ESP_OK : Instance created successfully
ESP_ERR_INVALID_ARG : Null argument(s)
ESP_ERR_HTTPD_ALLOC_MEM : Failed to allocate memory for instance
ESP_ERR_HTTPD_TASK : Failed to launch server task 





```

#### httpd_stop


```


esp_err_t httpd_stop(httpd_handle_t handle)
Stops the web server. 
Deallocates memory/resources used by an HTTP server instance and deletes it. Once deleted the handle can no longer be used for accessing the instance.
Example usage: // Function for stopping the webserver
void stop_webserver(httpd_handle_t server)
{
     // Ensure handle is non NULL
     if (server != NULL) {
         // Stop the httpd server
         httpd_stop(server);
     }
}




Parameters
handle -- [in] Handle to server returned by httpd_start 

Returns

ESP_OK : Server stopped successfully
ESP_ERR_INVALID_ARG : Handle argument is Null 





```

#### httpd_register_uri_handler


```


esp_err_t httpd_register_uri_handler(httpd_handle_t handle, const httpd_uri_t *uri_handler)
Registers a URI handler. 

Example usage: esp_err_t my_uri_handler(httpd_req_t* req)
{
    // Recv , Process and Send
    ....
    ....
    ....

    // Fail condition
    if (....) {
        // Return fail to close session //
        return ESP_FAIL;
    }

    // On success
    return ESP_OK;
}

// URI handler structure
httpd_uri_t my_uri {
    .uri      = "/my_uri/path/xyz",
    .method   = HTTPD_GET,
    .handler  = my_uri_handler,
    .user_ctx = NULL
};

// Register handler
if (httpd_register_uri_handler(server_handle, &my_uri) != ESP_OK) {
   // If failed to register handler
   ....
}




Note
URI handlers can be registered in real time as long as the server handle is valid.


Parameters

handle -- [in] handle to HTTPD server instance 
uri_handler -- [in] pointer to handler that needs to be registered


Returns

ESP_OK : On successfully registering the handler
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_HANDLERS_FULL : If no slots left for new handler
ESP_ERR_HTTPD_HANDLER_EXISTS : If handler with same URI and method is already registered 





```

#### httpd_unregister_uri_handler


```


esp_err_t httpd_unregister_uri_handler(httpd_handle_t handle, const char *uri, httpd_method_t method)
Unregister a URI handler. 

Parameters

handle -- [in] handle to HTTPD server instance 
uri -- [in] URI string 
method -- [in] HTTP method


Returns

ESP_OK : On successfully deregistering the handler
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_NOT_FOUND : Handler with specified URI and method not found 





```

#### httpd_unregister_uri


```


esp_err_t httpd_unregister_uri(httpd_handle_t handle, const char *uri)
Unregister all URI handlers with the specified uri string. 

Parameters

handle -- [in] handle to HTTPD server instance 
uri -- [in] uri string specifying all handlers that need to be deregisterd


Returns

ESP_OK : On successfully deregistering all such handlers
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_NOT_FOUND : No handler registered with specified uri string 





```

#### httpd_sess_set_recv_override


```


esp_err_t httpd_sess_set_recv_override(httpd_handle_t hd, int sockfd, httpd_recv_func_t recv_func)
Override web server's receive function (by session FD) 
This function overrides the web server's receive function. This same function is used to read HTTP request packets.

Note
This API is supposed to be called either from the context of
an http session APIs where sockfd is a valid parameter
a URI handler where sockfd is obtained using httpd_req_to_sockfd()




Parameters

hd -- [in] HTTPD instance handle 
sockfd -- [in] Session socket FD 
recv_func -- [in] The receive function to be set for this session


Returns

ESP_OK : On successfully registering override
ESP_ERR_INVALID_ARG : Null arguments 





```

#### httpd_sess_set_send_override


```


esp_err_t httpd_sess_set_send_override(httpd_handle_t hd, int sockfd, httpd_send_func_t send_func)
Override web server's send function (by session FD) 
This function overrides the web server's send function. This same function is used to send out any response to any HTTP request.

Note
This API is supposed to be called either from the context of
an http session APIs where sockfd is a valid parameter
a URI handler where sockfd is obtained using httpd_req_to_sockfd()




Parameters

hd -- [in] HTTPD instance handle 
sockfd -- [in] Session socket FD 
send_func -- [in] The send function to be set for this session


Returns

ESP_OK : On successfully registering override
ESP_ERR_INVALID_ARG : Null arguments 





```

#### httpd_sess_set_pending_override


```


esp_err_t httpd_sess_set_pending_override(httpd_handle_t hd, int sockfd, httpd_pending_func_t pending_func)
Override web server's pending function (by session FD) 
This function overrides the web server's pending function. This function is used to test for pending bytes in a socket.

Note
This API is supposed to be called either from the context of
an http session APIs where sockfd is a valid parameter
a URI handler where sockfd is obtained using httpd_req_to_sockfd()




Parameters

hd -- [in] HTTPD instance handle 
sockfd -- [in] Session socket FD 
pending_func -- [in] The receive function to be set for this session


Returns

ESP_OK : On successfully registering override
ESP_ERR_INVALID_ARG : Null arguments 





```

#### httpd_req_async_handler_begin


```


esp_err_t httpd_req_async_handler_begin(httpd_req_t *r, httpd_req_t **out)
Start an asynchronous request. This function can be called in a request handler to get a request copy that can be used on a async thread. 

Note

This function is necessary in order to handle multiple requests simultaneously. See examples/async_requests for example usage.
You must call httpd_req_async_handler_complete() when you are done with the request.




Parameters

r -- [in] The request to create an async copy of 
out -- [out] A newly allocated request which can be used on an async thread


Returns

ESP_OK : async request object created 





```

#### httpd_req_async_handler_complete


```


esp_err_t httpd_req_async_handler_complete(httpd_req_t *r)
Mark an asynchronous request as completed. This will. 

free the request memory
relinquish ownership of the underlying socket, so it can be reused.
allow the http server to close our socket if needed (lru_purge_enable)



Note
If async requests are not marked completed, eventually the server will no longer accept incoming connections. The server will log a "httpd_accept_conn: error in accept (23)" message if this happens.


Parameters
r -- [in] The request to mark async work as completed

Returns

ESP_OK : async request was marked completed 





```

#### httpd_req_to_sockfd


```


int httpd_req_to_sockfd(httpd_req_t *r)
Get the Socket Descriptor from the HTTP request. 
This API will return the socket descriptor of the session for which URI handler was executed on reception of HTTP request. This is useful when user wants to call functions that require session socket fd, from within a URI handler, ie. : httpd_sess_get_ctx(), httpd_sess_trigger_close(), httpd_sess_update_lru_counter().

Note
This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.


Parameters
r -- [in] The request whose socket descriptor should be found

Returns

Socket descriptor : The socket descriptor for this request
-1 : Invalid/NULL request pointer 





```

#### httpd_req_recv


```


int httpd_req_recv(httpd_req_t *r, char *buf, size_t buf_len)
API to read content data from the HTTP request. 
This API will read HTTP content data from the HTTP request into provided buffer. Use content_len provided in httpd_req_t structure to know the length of data to be fetched. If content_len is too large for the buffer then user may have to make multiple calls to this function, each time fetching 'buf_len' number of bytes, while the pointer to content data is incremented internally by the same number.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
If an error is returned, the URI handler must further return an error. This will ensure that the erroneous socket is closed and cleaned up by the web server.
Presently Chunked Encoding is not supported




Parameters

r -- [in] The request being responded to 
buf -- [in] Pointer to a buffer that the data will be read into 
buf_len -- [in] Length of the buffer


Returns

Bytes : Number of bytes read into the buffer successfully
0 : Buffer length parameter is zero / connection closed by peer
HTTPD_SOCK_ERR_INVALID : Invalid arguments
HTTPD_SOCK_ERR_TIMEOUT : Timeout/interrupted while calling socket recv()
HTTPD_SOCK_ERR_FAIL : Unrecoverable error while calling socket recv() 





```

#### httpd_req_get_hdr_value_len


```


size_t httpd_req_get_hdr_value_len(httpd_req_t *r, const char *field)
Search for a field in request headers and return the string length of it's value. 

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once httpd_resp_send() API is called all request headers are purged, so request headers need be copied into separate buffers if they are required later.




Parameters

r -- [in] The request being responded to 
field -- [in] The header field to be searched in the request


Returns

Length : If field is found in the request URL
Zero : Field not found / Invalid request / Null arguments 





```

#### httpd_req_get_hdr_value_str


```


esp_err_t httpd_req_get_hdr_value_str(httpd_req_t *r, const char *field, char *val, size_t val_size)
Get the value string of a field from the request headers. 

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once httpd_resp_send() API is called all request headers are purged, so request headers need be copied into separate buffers if they are required later.
If output size is greater than input, then the value is truncated, accompanied by truncation error as return value.
Use httpd_req_get_hdr_value_len() to know the right buffer length




Parameters

r -- [in] The request being responded to 
field -- [in] The field to be searched in the header 
val -- [out] Pointer to the buffer into which the value will be copied if the field is found 
val_size -- [in] Size of the user buffer "val"


Returns

ESP_OK : Field found in the request header and value string copied
ESP_ERR_NOT_FOUND : Key not found
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_INVALID_REQ : Invalid HTTP request pointer
ESP_ERR_HTTPD_RESULT_TRUNC : Value string truncated 





```

#### httpd_req_get_url_query_len


```


size_t httpd_req_get_url_query_len(httpd_req_t *r)
Get Query string length from the request URL. 

Note
This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid


Parameters
r -- [in] The request being responded to

Returns

Length : Query is found in the request URL
Zero : Query not found / Null arguments / Invalid request 





```

#### httpd_req_get_url_query_str


```


esp_err_t httpd_req_get_url_query_str(httpd_req_t *r, char *buf, size_t buf_len)
Get Query string from the request URL. 

Note

Presently, the user can fetch the full URL query string, but decoding will have to be performed by the user. Request headers can be read using httpd_req_get_hdr_value_str() to know the 'Content-Type' (eg. Content-Type: application/x-www-form-urlencoded) and then the appropriate decoding algorithm needs to be applied.
This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid
If output size is greater than input, then the value is truncated, accompanied by truncation error as return value
Prior to calling this function, one can use httpd_req_get_url_query_len() to know the query string length beforehand and hence allocate the buffer of right size (usually query string length + 1 for null termination) for storing the query string




Parameters

r -- [in] The request being responded to 
buf -- [out] Pointer to the buffer into which the query string will be copied (if found) 
buf_len -- [in] Length of output buffer


Returns

ESP_OK : Query is found in the request URL and copied to buffer
ESP_ERR_NOT_FOUND : Query not found
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_INVALID_REQ : Invalid HTTP request pointer
ESP_ERR_HTTPD_RESULT_TRUNC : Query string truncated 





```

#### httpd_query_key_value


```


esp_err_t httpd_query_key_value(const char *qry, const char *key, char *val, size_t val_size)
Helper function to get a URL query tag from a query string of the type param1=val1&param2=val2. 

Note

The components of URL query string (keys and values) are not URLdecoded. The user must check for 'Content-Type' field in the request headers and then depending upon the specified encoding (URLencoded or otherwise) apply the appropriate decoding algorithm.
If actual value size is greater than val_size, then the value is truncated, accompanied by truncation error as return value.




Parameters

qry -- [in] Pointer to query string 
key -- [in] The key to be searched in the query string 
val -- [out] Pointer to the buffer into which the value will be copied if the key is found 
val_size -- [in] Size of the user buffer "val"


Returns

ESP_OK : Key is found in the URL query string and copied to buffer
ESP_ERR_NOT_FOUND : Key not found
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESULT_TRUNC : Value string truncated 





```

#### httpd_req_get_cookie_val


```


esp_err_t httpd_req_get_cookie_val(httpd_req_t *req, const char *cookie_name, char *val, size_t *val_size)
Get the value string of a cookie value from the "Cookie" request headers by cookie name. 

Parameters

req -- [in] Pointer to the HTTP request 
cookie_name -- [in] The cookie name to be searched in the request 
val -- [out] Pointer to the buffer into which the value of cookie will be copied if the cookie is found 
val_size -- [inout] Pointer to size of the user buffer "val". This variable will contain cookie length if ESP_OK is returned and required buffer length incase ESP_ERR_HTTPD_RESULT_TRUNC is returned.


Returns

ESP_OK : Key is found in the cookie string and copied to buffer
ESP_ERR_NOT_FOUND : Key not found
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESULT_TRUNC : Value string truncated
ESP_ERR_NO_MEM : Memory allocation failure 





```

#### httpd_uri_match_wildcard


```


bool httpd_uri_match_wildcard(const char *uri_template, const char *uri_to_match, size_t match_upto)
Test if a URI matches the given wildcard template. 
Template may end with "?" to make the previous character optional (typically a slash), "*" for a wildcard match, and "?*" to make the previous character optional, and if present, allow anything to follow.
Example:
* matches everything
/foo/? matches /foo and /foo/
/foo/* (sans the backslash) matches /foo/ and /foo/bar, but not /foo or /fo
/foo/?* or /foo/*? (sans the backslash) matches /foo/, /foo/bar, and also /foo, but not /foox or /fo


The special characters "?" and "*" anywhere else in the template will be taken literally.

Parameters

uri_template -- [in] URI template (pattern) 
uri_to_match -- [in] URI to be matched 
match_upto -- [in] how many characters of the URI buffer to test (there may be trailing query string etc.)


Returns
true if a match was found 



```

#### httpd_resp_send


```


esp_err_t httpd_resp_send(httpd_req_t *r, const char *buf, ssize_t buf_len)
API to send a complete HTTP response. 
This API will send the data as an HTTP response to the request. This assumes that you have the entire response ready in a single buffer. If you wish to send response in incremental chunks use httpd_resp_send_chunk() instead.
If no status code and content-type were set, by default this will send 200 OK status code and content type as text/html. You may call the following functions before this API to configure the response headers : httpd_resp_set_status() - for setting the HTTP status string, httpd_resp_set_type() - for setting the Content Type, httpd_resp_set_hdr() - for appending any additional field value entries in the response header

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once this API is called, the request has been responded to.
No additional data can then be sent for the request.
Once this API is called, all request headers are purged, so request headers need be copied into separate buffers if they are required later.




Parameters

r -- [in] The request being responded to 
buf -- [in] Buffer from where the content is to be fetched 
buf_len -- [in] Length of the buffer, HTTPD_RESP_USE_STRLEN to use strlen()


Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null request pointer
ESP_ERR_HTTPD_RESP_HDR : Essential headers are too large for internal buffer
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request 





```

#### httpd_resp_send_chunk


```


esp_err_t httpd_resp_send_chunk(httpd_req_t *r, const char *buf, ssize_t buf_len)
API to send one HTTP chunk. 
This API will send the data as an HTTP response to the request. This API will use chunked-encoding and send the response in the form of chunks. If you have the entire response contained in a single buffer, please use httpd_resp_send() instead.
If no status code and content-type were set, by default this will send 200 OK status code and content type as text/html. You may call the following functions before this API to configure the response headers httpd_resp_set_status() - for setting the HTTP status string, httpd_resp_set_type() - for setting the Content Type, httpd_resp_set_hdr() - for appending any additional field value entries in the response header

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
When you are finished sending all your chunks, you must call this function with buf_len as 0.
Once this API is called, all request headers are purged, so request headers need be copied into separate buffers if they are required later.




Parameters

r -- [in] The request being responded to 
buf -- [in] Pointer to a buffer that stores the data 
buf_len -- [in] Length of the buffer, HTTPD_RESP_USE_STRLEN to use strlen()


Returns

ESP_OK : On successfully sending the response packet chunk
ESP_ERR_INVALID_ARG : Null request pointer
ESP_ERR_HTTPD_RESP_HDR : Essential headers are too large for internal buffer
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_sendstr


```


static inline esp_err_t httpd_resp_sendstr(httpd_req_t *r, const char *str)
API to send a complete string as HTTP response. 
This API simply calls http_resp_send with buffer length set to string length assuming the buffer contains a null terminated string

Parameters

r -- [in] The request being responded to 
str -- [in] String to be sent as response body


Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null request pointer
ESP_ERR_HTTPD_RESP_HDR : Essential headers are too large for internal buffer
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request 





```

#### httpd_resp_sendstr_chunk


```


static inline esp_err_t httpd_resp_sendstr_chunk(httpd_req_t *r, const char *str)
API to send a string as an HTTP response chunk. 
This API simply calls http_resp_send_chunk with buffer length set to string length assuming the buffer contains a null terminated string

Parameters

r -- [in] The request being responded to 
str -- [in] String to be sent as response body (NULL to finish response packet)


Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null request pointer
ESP_ERR_HTTPD_RESP_HDR : Essential headers are too large for internal buffer
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request 





```

#### httpd_resp_set_status


```


esp_err_t httpd_resp_set_status(httpd_req_t *r, const char *status)
API to set the HTTP status code. 
This API sets the status of the HTTP response to the value specified. By default, the '200 OK' response is sent as the response.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
This API only sets the status to this value. The status isn't sent out until any of the send APIs is executed.
Make sure that the lifetime of the status string is valid till send function is called.




Parameters

r -- [in] The request being responded to 
status -- [in] The HTTP status code of this response


Returns

ESP_OK : On success
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_set_type


```


esp_err_t httpd_resp_set_type(httpd_req_t *r, const char *type)
API to set the HTTP content type. 
This API sets the 'Content Type' field of the response. The default content type is 'text/html'.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
This API only sets the content type to this value. The type isn't sent out until any of the send APIs is executed.
Make sure that the lifetime of the type string is valid till send function is called.




Parameters

r -- [in] The request being responded to 
type -- [in] The Content Type of the response


Returns

ESP_OK : On success
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_set_hdr


```


esp_err_t httpd_resp_set_hdr(httpd_req_t *r, const char *field, const char *value)
API to append any additional headers. 
This API sets any additional header fields that need to be sent in the response.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
The header isn't sent out until any of the send APIs is executed.
The maximum allowed number of additional headers is limited to value of max_resp_headers in config structure.
Make sure that the lifetime of the field value strings are valid till send function is called.




Parameters

r -- [in] The request being responded to 
field -- [in] The field name of the HTTP header 
value -- [in] The value of this HTTP header


Returns

ESP_OK : On successfully appending new header
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESP_HDR : Total additional headers exceed max allowed
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_send_err


```


esp_err_t httpd_resp_send_err(httpd_req_t *req, httpd_err_code_t error, const char *msg)
For sending out error code in response to HTTP request. 

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once this API is called, all request headers are purged, so request headers need be copied into separate buffers if they are required later.
If you wish to send additional data in the body of the response, please use the lower-level functions directly.




Parameters

req -- [in] Pointer to the HTTP request for which the response needs to be sent 
error -- [in] Error type to send 
msg -- [in] Error message string (pass NULL for default message)


Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_send_custom_err


```


esp_err_t httpd_resp_send_custom_err(httpd_req_t *req, const char *status, const char *msg)
For sending out custom error code in response to HTTP request. 

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once this API is called, all request headers are purged, so request headers need be copied into separate buffers if they are required later.
If you wish to send additional data in the body of the response, please use the lower-level functions directly.




Parameters

req -- [in] Pointer to the HTTP request for which the response needs to be sent 
status -- [in] Error status to send 
msg -- [in] Error message string


Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_send_404


```


static inline esp_err_t httpd_resp_send_404(httpd_req_t *r)
Helper function for HTTP 404. 
Send HTTP 404 message. If you wish to send additional data in the body of the response, please use the lower-level functions directly.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once this API is called, all request headers are purged, so request headers need be copied into separate buffers if they are required later.




Parameters
r -- [in] The request being responded to

Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_send_408


```


static inline esp_err_t httpd_resp_send_408(httpd_req_t *r)
Helper function for HTTP 408. 
Send HTTP 408 message. If you wish to send additional data in the body of the response, please use the lower-level functions directly.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once this API is called, all request headers are purged, so request headers need be copied into separate buffers if they are required later.




Parameters
r -- [in] The request being responded to

Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_resp_send_500


```


static inline esp_err_t httpd_resp_send_500(httpd_req_t *r)
Helper function for HTTP 500. 
Send HTTP 500 message. If you wish to send additional data in the body of the response, please use the lower-level functions directly.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Once this API is called, all request headers are purged, so request headers need be copied into separate buffers if they are required later.




Parameters
r -- [in] The request being responded to

Returns

ESP_OK : On successfully sending the response packet
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_HTTPD_RESP_SEND : Error in raw send
ESP_ERR_HTTPD_INVALID_REQ : Invalid request pointer 





```

#### httpd_send


```


int httpd_send(httpd_req_t *r, const char *buf, size_t buf_len)
Raw HTTP send. 
Call this API if you wish to construct your custom response packet. When using this, all essential header, eg. HTTP version, Status Code, Content Type and Length, Encoding, etc. will have to be constructed manually, and HTTP delimeters (CRLF) will need to be placed correctly for separating sub-sections of the HTTP response packet.
If the send override function is set, this API will end up calling that function eventually to send data out.

Note

This API is supposed to be called only from the context of a URI handler where httpd_req_t* request pointer is valid.
Unless the response has the correct HTTP structure (which the user must now ensure) it is not guaranteed that it will be recognized by the client. For most cases, you wouldn't have to call this API, but you would rather use either of : httpd_resp_send(), httpd_resp_send_chunk()




Parameters

r -- [in] The request being responded to 
buf -- [in] Buffer from where the fully constructed packet is to be read 
buf_len -- [in] Length of the buffer


Returns

Bytes : Number of bytes that were sent successfully
HTTPD_SOCK_ERR_INVALID : Invalid arguments
HTTPD_SOCK_ERR_TIMEOUT : Timeout/interrupted while calling socket send()
HTTPD_SOCK_ERR_FAIL : Unrecoverable error while calling socket send() 





```

#### httpd_socket_send


```


int httpd_socket_send(httpd_handle_t hd, int sockfd, const char *buf, size_t buf_len, int flags)
A low level API to send data on a given socket

This internally calls the default send function, or the function registered by httpd_sess_set_send_override().

Note
This API is not recommended to be used in any request handler. Use this only for advanced use cases, wherein some asynchronous data is to be sent over a socket.


Parameters

hd -- [in] server instance 
sockfd -- [in] session socket file descriptor 
buf -- [in] buffer with bytes to send 
buf_len -- [in] data size 
flags -- [in] flags for the send() function 


Returns

Bytes : The number of bytes sent successfully
HTTPD_SOCK_ERR_INVALID : Invalid arguments
HTTPD_SOCK_ERR_TIMEOUT : Timeout/interrupted while calling socket send()
HTTPD_SOCK_ERR_FAIL : Unrecoverable error while calling socket send() 





```

#### httpd_socket_recv


```


int httpd_socket_recv(httpd_handle_t hd, int sockfd, char *buf, size_t buf_len, int flags)
A low level API to receive data from a given socket

This internally calls the default recv function, or the function registered by httpd_sess_set_recv_override().

Note
This API is not recommended to be used in any request handler. Use this only for advanced use cases, wherein some asynchronous communication is required.


Parameters

hd -- [in] server instance 
sockfd -- [in] session socket file descriptor 
buf -- [in] buffer with bytes to send 
buf_len -- [in] data size 
flags -- [in] flags for the send() function 


Returns

Bytes : The number of bytes received successfully
0 : Buffer length parameter is zero / connection closed by peer
HTTPD_SOCK_ERR_INVALID : Invalid arguments
HTTPD_SOCK_ERR_TIMEOUT : Timeout/interrupted while calling socket recv()
HTTPD_SOCK_ERR_FAIL : Unrecoverable error while calling socket recv() 





```

#### httpd_register_err_handler


```


esp_err_t httpd_register_err_handler(httpd_handle_t handle, httpd_err_code_t error, httpd_err_handler_func_t handler_fn)
Function for registering HTTP error handlers. 
This function maps a handler function to any supported error code given by httpd_err_code_t. See prototype httpd_err_handler_func_t above for details.

Parameters

handle -- [in] HTTP server handle 
error -- [in] Error type 
handler_fn -- [in] User implemented handler function (Pass NULL to unset any previously set handler)


Returns

ESP_OK : handler registered successfully
ESP_ERR_INVALID_ARG : invalid error code or server handle 





```

#### httpd_queue_work


```


esp_err_t httpd_queue_work(httpd_handle_t handle, httpd_work_fn_t work, void *arg)
Queue execution of a function in HTTPD's context. 
This API queues a work function for asynchronous execution

Note
Some protocols require that the web server generate some asynchronous data and send it to the persistently opened connection. This facility is for use by such protocols.


Parameters

handle -- [in] Handle to server returned by httpd_start 
work -- [in] Pointer to the function to be executed in the HTTPD's context 
arg -- [in] Pointer to the arguments that should be passed to this function


Returns

ESP_OK : On successfully queueing the work
ESP_FAIL : Failure in ctrl socket
ESP_ERR_INVALID_ARG : Null arguments 





```

#### httpd_sess_get_ctx


```


void *httpd_sess_get_ctx(httpd_handle_t handle, int sockfd)
Get session context from socket descriptor. 
Typically if a session context is created, it is available to URI handlers through the httpd_req_t structure. But, there are cases where the web server's send/receive functions may require the context (for example, for accessing keying information etc). Since the send/receive function only have the socket descriptor at their disposal, this API provides them with a way to retrieve the session context.

Parameters

handle -- [in] Handle to server returned by httpd_start 
sockfd -- [in] The socket descriptor for which the context should be extracted.


Returns

void* : Pointer to the context associated with this session
NULL : Empty context / Invalid handle / Invalid socket fd 





```

#### httpd_sess_set_ctx


```


void httpd_sess_set_ctx(httpd_handle_t handle, int sockfd, void *ctx, httpd_free_ctx_fn_t free_fn)
Set session context by socket descriptor. 

Parameters

handle -- [in] Handle to server returned by httpd_start 
sockfd -- [in] The socket descriptor for which the context should be extracted. 
ctx -- [in] Context object to assign to the session 
free_fn -- [in] Function that should be called to free the context 




```

#### httpd_sess_get_transport_ctx


```


void *httpd_sess_get_transport_ctx(httpd_handle_t handle, int sockfd)
Get session 'transport' context by socket descriptor. 

This context is used by the send/receive functions, for example to manage SSL context.
See also
httpd_sess_get_ctx()



Parameters

handle -- [in] Handle to server returned by httpd_start 
sockfd -- [in] The socket descriptor for which the context should be extracted. 


Returns

void* : Pointer to the transport context associated with this session
NULL : Empty context / Invalid handle / Invalid socket fd 





```

#### httpd_sess_set_transport_ctx


```


void httpd_sess_set_transport_ctx(httpd_handle_t handle, int sockfd, void *ctx, httpd_free_ctx_fn_t free_fn)
Set session 'transport' context by socket descriptor. 

See also
httpd_sess_set_ctx()



Parameters

handle -- [in] Handle to server returned by httpd_start 
sockfd -- [in] The socket descriptor for which the context should be extracted. 
ctx -- [in] Transport context object to assign to the session 
free_fn -- [in] Function that should be called to free the transport context 




```

#### httpd_get_global_user_ctx


```


void *httpd_get_global_user_ctx(httpd_handle_t handle)
Get HTTPD global user context (it was set in the server config struct) 

Parameters
handle -- [in] Handle to server returned by httpd_start 

Returns
global user context 



```

#### httpd_get_global_transport_ctx


```


void *httpd_get_global_transport_ctx(httpd_handle_t handle)
Get HTTPD global transport context (it was set in the server config struct) 

Parameters
handle -- [in] Handle to server returned by httpd_start 

Returns
global transport context 



```

#### httpd_sess_trigger_close


```


esp_err_t httpd_sess_trigger_close(httpd_handle_t handle, int sockfd)
Trigger an httpd session close externally. 

Note
Calling this API is only required in special circumstances wherein some application requires to close an httpd client session asynchronously.


Parameters

handle -- [in] Handle to server returned by httpd_start 
sockfd -- [in] The socket descriptor of the session to be closed


Returns

ESP_OK : On successfully initiating closure
ESP_FAIL : Failure to queue work
ESP_ERR_NOT_FOUND : Socket fd not found
ESP_ERR_INVALID_ARG : Null arguments 





```

#### httpd_sess_update_lru_counter


```


esp_err_t httpd_sess_update_lru_counter(httpd_handle_t handle, int sockfd)
Update LRU counter for a given socket. 
LRU Counters are internally associated with each session to monitor how recently a session exchanged traffic. When LRU purge is enabled, if a client is requesting for connection but maximum number of sockets/sessions is reached, then the session having the earliest LRU counter is closed automatically.
Updating the LRU counter manually prevents the socket from being purged due to the Least Recently Used (LRU) logic, even though it might not have received traffic for some time. This is useful when all open sockets/session are frequently exchanging traffic but the user specifically wants one of the sessions to be kept open, irrespective of when it last exchanged a packet.

Note
Calling this API is only necessary if the LRU Purge Enable option is enabled.


Parameters

handle -- [in] Handle to server returned by httpd_start 
sockfd -- [in] The socket descriptor of the session for which LRU counter is to be updated


Returns

ESP_OK : Socket found and LRU counter updated
ESP_ERR_NOT_FOUND : Socket not found
ESP_ERR_INVALID_ARG : Null arguments 





```

#### httpd_get_client_list


```


esp_err_t httpd_get_client_list(httpd_handle_t handle, size_t *fds, int *client_fds)
Returns list of current socket descriptors of active sessions. 

Note
Size of provided array has to be equal or greater then maximum number of opened sockets, configured upon initialization with max_open_sockets field in httpd_config_t structure.


Parameters

handle -- [in] Handle to server returned by httpd_start 
fds -- [inout] In: Size of provided client_fds array Out: Number of valid client fds returned in client_fds, 
client_fds -- [out] Array of client fds


Returns

ESP_OK : Successfully retrieved session list
ESP_ERR_INVALID_ARG : Wrong arguments or list is longer than provided array 





```



### Structures


#### esp_http_server_event_data


```


struct esp_http_server_event_data
Argument structure for HTTP_SERVER_EVENT_ON_DATA and HTTP_SERVER_EVENT_SENT_DATA event 

Public Members
fd

int fd
Session socket file descriptor 

data_len

int data_len
Data length 



```

#### httpd_config


```


struct httpd_config
HTTP Server Configuration Structure. 

Note
Use HTTPD_DEFAULT_CONFIG() to initialize the configuration to a default value and then modify only those fields that are specifically determined by the use case. 


Public Members
task_priority

unsigned task_priority
Priority of FreeRTOS task which runs the server 

stack_size

size_t stack_size
The maximum stack size allowed for the server task 

core_id

BaseType_t core_id
The core the HTTP server task will run on 

task_caps

uint32_t task_caps
The memory capabilities to use when allocating the HTTP server task's stack 

server_port

uint16_t server_port
TCP Port number for receiving and transmitting HTTP traffic 

ctrl_port

uint16_t ctrl_port
UDP Port number for asynchronously exchanging control signals between various components of the server 

max_open_sockets

uint16_t max_open_sockets
Max number of sockets/clients connected at any time (3 sockets are reserved for internal working of the HTTP server) 

max_uri_handlers

uint16_t max_uri_handlers
Maximum allowed uri handlers 

max_resp_headers

uint16_t max_resp_headers
Maximum allowed additional headers in HTTP response 

backlog_conn

uint16_t backlog_conn
Number of backlog connections 

lru_purge_enable

bool lru_purge_enable
Purge "Least Recently Used" connection 

recv_wait_timeout

uint16_t recv_wait_timeout
Timeout for recv function (in seconds) 

send_wait_timeout

uint16_t send_wait_timeout
Timeout for send function (in seconds) 

global_user_ctx

void *global_user_ctx
Global user context.
This field can be used to store arbitrary user data within the server context. The value can be retrieved using the server handle, available e.g. in the httpd_req_t struct.
When shutting down, the server frees up the user context by calling free() on the global_user_ctx field. If you wish to use a custom function for freeing the global user context, please specify that here. 

global_user_ctx_free_fn

httpd_free_ctx_fn_t global_user_ctx_free_fn
Free function for global user context 

global_transport_ctx

void *global_transport_ctx
Global transport context.
Similar to global_user_ctx, but used for session encoding or encryption (e.g. to hold the SSL context). It will be freed using free(), unless global_transport_ctx_free_fn is specified. 

global_transport_ctx_free_fn

httpd_free_ctx_fn_t global_transport_ctx_free_fn
Free function for global transport context 

enable_so_linger

bool enable_so_linger
bool to enable/disable linger 

linger_timeout

int linger_timeout
linger timeout (in seconds) 

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

open_fn

httpd_open_func_t open_fn
Custom session opening callback.
Called on a new session socket just after accept(), but before reading any data.
This is an opportunity to set up e.g. SSL encryption using global_transport_ctx and the send/recv/pending session overrides.
If a context needs to be maintained between these functions, store it in the session using httpd_sess_set_transport_ctx() and retrieve it later with httpd_sess_get_transport_ctx()
Returning a value other than ESP_OK will immediately close the new socket. 

close_fn

httpd_close_func_t close_fn
Custom session closing callback.
Called when a session is deleted, before freeing user and transport contexts and before closing the socket. This is a place for custom de-init code common to all sockets.
The server will only close the socket if no custom session closing callback is set. If a custom callback is used, close(sockfd) should be called in here for most cases.
Set the user or transport context to NULL if it was freed here, so the server does not try to free it again.
This function is run for all terminated sessions, including sessions where the socket was closed by the network stack - that is, the file descriptor may not be valid anymore. 

uri_match_fn

httpd_uri_match_func_t uri_match_fn
URI matcher function.
Called when searching for a matching URI: 1) whose request handler is to be executed right after an HTTP request is successfully parsed 2) in order to prevent duplication while registering a new URI handler using httpd_register_uri_handler()
Available options are: 1) NULL : Internally do basic matching using strncmp() 2) httpd_uri_match_wildcard() : URI wildcard matcher
Users can implement their own matching functions (See description of the httpd_uri_match_func_t function prototype) 



```

#### httpd_req


```


struct httpd_req
HTTP Request Data Structure. 

Public Members
handle

httpd_handle_t handle
Handle to server instance 

method

int method
The type of HTTP request, -1 if unsupported method, HTTP_ANY for wildcard method to support every method 

uri

const char uri[HTTPD_MAX_URI_LEN + 1]
The URI of this request (1 byte extra for null termination) 

content_len

size_t content_len
Length of the request body 

aux

void *aux
Internally used members 

user_ctx

void *user_ctx
User context pointer passed during URI registration. 

sess_ctx

void *sess_ctx
Session Context Pointer
A session context. Contexts are maintained across 'sessions' for a given open TCP connection. One session could have multiple request responses. The web server will ensure that the context persists across all these request and responses.
By default, this is NULL. URI Handlers can set this to any meaningful value.
If the underlying socket gets closed, and this pointer is non-NULL, the web server will free up the context by calling free(), unless free_ctx function is set. 

free_ctx

httpd_free_ctx_fn_t free_ctx
Pointer to free context hook
Function to free session context
If the web server's socket closes, it frees up the session context by calling free() on the sess_ctx member. If you wish to use a custom function for freeing the session context, please specify that here. 

ignore_sess_ctx_changes

bool ignore_sess_ctx_changes
Flag indicating if Session Context changes should be ignored
By default, if you change the sess_ctx in some URI handler, the http server will internally free the earlier context (if non NULL), after the URI handler returns. If you want to manage the allocation/reallocation/freeing of sess_ctx yourself, set this flag to true, so that the server will not perform any checks on it. The context will be cleared by the server (by calling free_ctx or free()) only if the socket gets closed. 



```

#### httpd_uri


```


struct httpd_uri
Structure for URI handler. 

Public Members
uri

const char *uri
The URI to handle 

method

httpd_method_t method
Method supported by the URI, HTTP_ANY for wildcard method to support all methods 

handler

esp_err_t (*handler)(httpd_req_t *r)
Handler to call for supported request method. This must return ESP_OK, or else the underlying socket will be closed. 

user_ctx

void *user_ctx
Pointer to user context data which will be available to handler 



```



### Macros


#### HTTP_ANY


```


HTTP_ANY

```

#### HTTPD_MAX_REQ_HDR_LEN


```


HTTPD_MAX_REQ_HDR_LEN

```

#### HTTPD_MAX_URI_LEN


```


HTTPD_MAX_URI_LEN

```

#### HTTPD_SOCK_ERR_FAIL


```


HTTPD_SOCK_ERR_FAIL

```

#### HTTPD_SOCK_ERR_INVALID


```


HTTPD_SOCK_ERR_INVALID

```

#### HTTPD_SOCK_ERR_TIMEOUT


```


HTTPD_SOCK_ERR_TIMEOUT

```

#### HTTPD_200


```


HTTPD_200
HTTP Response 200 

```

#### HTTPD_204


```


HTTPD_204
HTTP Response 204 

```

#### HTTPD_207


```


HTTPD_207
HTTP Response 207 

```

#### HTTPD_400


```


HTTPD_400
HTTP Response 400 

```

#### HTTPD_404


```


HTTPD_404
HTTP Response 404 

```

#### HTTPD_408


```


HTTPD_408
HTTP Response 408 

```

#### HTTPD_500


```


HTTPD_500
HTTP Response 500 

```

#### HTTPD_TYPE_JSON


```


HTTPD_TYPE_JSON
HTTP Content type JSON 

```

#### HTTPD_TYPE_TEXT


```


HTTPD_TYPE_TEXT
HTTP Content type text/HTML 

```

#### HTTPD_TYPE_OCTET


```


HTTPD_TYPE_OCTET
HTTP Content type octext-stream 

```

#### ESP_HTTPD_DEF_CTRL_PORT


```


ESP_HTTPD_DEF_CTRL_PORT
HTTP Server control socket port 

```

#### HTTPD_DEFAULT_CONFIG


```


HTTPD_DEFAULT_CONFIG()

```

#### ESP_ERR_HTTPD_BASE


```


ESP_ERR_HTTPD_BASE
Starting number of HTTPD error codes 

```

#### ESP_ERR_HTTPD_HANDLERS_FULL


```


ESP_ERR_HTTPD_HANDLERS_FULL
All slots for registering URI handlers have been consumed 

```

#### ESP_ERR_HTTPD_HANDLER_EXISTS


```


ESP_ERR_HTTPD_HANDLER_EXISTS
URI handler with same method and target URI already registered 

```

#### ESP_ERR_HTTPD_INVALID_REQ


```


ESP_ERR_HTTPD_INVALID_REQ
Invalid request pointer 

```

#### ESP_ERR_HTTPD_RESULT_TRUNC


```


ESP_ERR_HTTPD_RESULT_TRUNC
Result string truncated 

```

#### ESP_ERR_HTTPD_RESP_HDR


```


ESP_ERR_HTTPD_RESP_HDR
Response header field larger than supported 

```

#### ESP_ERR_HTTPD_RESP_SEND


```


ESP_ERR_HTTPD_RESP_SEND
Error occured while sending response packet 

```

#### ESP_ERR_HTTPD_ALLOC_MEM


```


ESP_ERR_HTTPD_ALLOC_MEM
Failed to dynamically allocate memory for resource 

```

#### ESP_ERR_HTTPD_TASK


```


ESP_ERR_HTTPD_TASK
Failed to launch server task/thread 

```

#### HTTPD_RESP_USE_STRLEN


```


HTTPD_RESP_USE_STRLEN

```



### Type Definitions


#### httpd_handle_t


```


typedef void *httpd_handle_t
HTTP Server Instance Handle. 
Every instance of the server will have a unique handle. 

```

#### httpd_method_t


```


typedef enum http_method httpd_method_t
HTTP Method Type wrapper over "enum http_method" available in "http_parser" library. 

```

#### httpd_free_ctx_fn_t


```


typedef void (*httpd_free_ctx_fn_t)(void *ctx)
Prototype for freeing context data (if any) 

Param ctx
[in] object to free 



```

#### httpd_open_func_t


```


typedef esp_err_t (*httpd_open_func_t)(httpd_handle_t hd, int sockfd)
Function prototype for opening a session. 
Called immediately after the socket was opened to set up the send/recv functions and other parameters of the socket.

Param hd
[in] server instance 

Param sockfd
[in] session socket file descriptor 

Return

ESP_OK : On success
Any value other than ESP_OK will signal the server to close the socket immediately 





```

#### httpd_close_func_t


```


typedef void (*httpd_close_func_t)(httpd_handle_t hd, int sockfd)
Function prototype for closing a session. 

Note
It's possible that the socket descriptor is invalid at this point, the function is called for all terminated sessions. Ensure proper handling of return codes.


Param hd
[in] server instance 

Param sockfd
[in] session socket file descriptor 



```

#### httpd_uri_match_func_t


```


typedef bool (*httpd_uri_match_func_t)(const char *reference_uri, const char *uri_to_match, size_t match_upto)
Function prototype for URI matching. 

Param reference_uri
[in] URI/template with respect to which the other URI is matched 

Param uri_to_match
[in] URI/template being matched to the reference URI/template 

Param match_upto
[in] For specifying the actual length of uri_to_match up to which the matching algorithm is to be applied (The maximum value is strlen(uri_to_match), independent of the length of reference_uri) 

Return
true on match 



```

#### httpd_config_t


```


typedef struct httpd_config httpd_config_t
HTTP Server Configuration Structure. 

Note
Use HTTPD_DEFAULT_CONFIG() to initialize the configuration to a default value and then modify only those fields that are specifically determined by the use case. 


```

#### httpd_req_t


```


typedef struct httpd_req httpd_req_t
HTTP Request Data Structure. 

```

#### httpd_uri_t


```


typedef struct httpd_uri httpd_uri_t
Structure for URI handler. 

```

#### httpd_send_func_t


```


typedef int (*httpd_send_func_t)(httpd_handle_t hd, int sockfd, const char *buf, size_t buf_len, int flags)
Prototype for HTTPDs low-level send function. 

Note
User specified send function must handle errors internally, depending upon the set value of errno, and return specific HTTPD_SOCK_ERR_ codes, which will eventually be conveyed as return value of httpd_send() function


Param hd
[in] server instance 

Param sockfd
[in] session socket file descriptor 

Param buf
[in] buffer with bytes to send 

Param buf_len
[in] data size 

Param flags
[in] flags for the send() function 

Return

Bytes : The number of bytes sent successfully
HTTPD_SOCK_ERR_INVALID : Invalid arguments
HTTPD_SOCK_ERR_TIMEOUT : Timeout/interrupted while calling socket send()
HTTPD_SOCK_ERR_FAIL : Unrecoverable error while calling socket send() 





```

#### httpd_recv_func_t


```


typedef int (*httpd_recv_func_t)(httpd_handle_t hd, int sockfd, char *buf, size_t buf_len, int flags)
Prototype for HTTPDs low-level recv function. 

Note
User specified recv function must handle errors internally, depending upon the set value of errno, and return specific HTTPD_SOCK_ERR_ codes, which will eventually be conveyed as return value of httpd_req_recv() function


Param hd
[in] server instance 

Param sockfd
[in] session socket file descriptor 

Param buf
[in] buffer with bytes to send 

Param buf_len
[in] data size 

Param flags
[in] flags for the send() function 

Return

Bytes : The number of bytes received successfully
0 : Buffer length parameter is zero / connection closed by peer
HTTPD_SOCK_ERR_INVALID : Invalid arguments
HTTPD_SOCK_ERR_TIMEOUT : Timeout/interrupted while calling socket recv()
HTTPD_SOCK_ERR_FAIL : Unrecoverable error while calling socket recv() 





```

#### httpd_pending_func_t


```


typedef int (*httpd_pending_func_t)(httpd_handle_t hd, int sockfd)
Prototype for HTTPDs low-level "get pending bytes" function. 

Note
User specified pending function must handle errors internally, depending upon the set value of errno, and return specific HTTPD_SOCK_ERR_ codes, which will be handled accordingly in the server task.


Param hd
[in] server instance 

Param sockfd
[in] session socket file descriptor 

Return

Bytes : The number of bytes waiting to be received
HTTPD_SOCK_ERR_INVALID : Invalid arguments
HTTPD_SOCK_ERR_TIMEOUT : Timeout/interrupted while calling socket pending()
HTTPD_SOCK_ERR_FAIL : Unrecoverable error while calling socket pending() 





```

#### httpd_err_handler_func_t


```


typedef esp_err_t (*httpd_err_handler_func_t)(httpd_req_t *req, httpd_err_code_t error)
Function prototype for HTTP error handling. 
This function is executed upon HTTP errors generated during internal processing of an HTTP request. This is used to override the default behavior on error, which is to send HTTP error response and close the underlying socket.

Note

If implemented, the server will not automatically send out HTTP error response codes, therefore, httpd_resp_send_err() must be invoked inside this function if user wishes to generate HTTP error responses.
When invoked, the validity of uri, method, content_len and user_ctx fields of the httpd_req_t parameter is not guaranteed as the HTTP request may be partially received/parsed.
The function must return ESP_OK if underlying socket needs to be kept open. Any other value will ensure that the socket is closed. The return value is ignored when error is of type HTTPD_500_INTERNAL_SERVER_ERROR and the socket closed anyway.




Param req
[in] HTTP request for which the error needs to be handled 

Param error
[in] Error type

Return

ESP_OK : error handled successful
ESP_FAIL : failure indicates that the underlying socket needs to be closed 





```

#### httpd_work_fn_t


```


typedef void (*httpd_work_fn_t)(void *arg)
Prototype of the HTTPD work function Please refer to httpd_queue_work() for more details. 

Param arg
[in] The arguments for this work function 



```



### Enumerations


#### httpd_err_code_t


```


enum httpd_err_code_t
Error codes sent as HTTP response in case of errors encountered during processing of an HTTP request. 
Values:
HTTPD_500_INTERNAL_SERVER_ERROR

enumerator HTTPD_500_INTERNAL_SERVER_ERROR

HTTPD_501_METHOD_NOT_IMPLEMENTED

enumerator HTTPD_501_METHOD_NOT_IMPLEMENTED

HTTPD_505_VERSION_NOT_SUPPORTED

enumerator HTTPD_505_VERSION_NOT_SUPPORTED

HTTPD_400_BAD_REQUEST

enumerator HTTPD_400_BAD_REQUEST

HTTPD_401_UNAUTHORIZED

enumerator HTTPD_401_UNAUTHORIZED

HTTPD_403_FORBIDDEN

enumerator HTTPD_403_FORBIDDEN

HTTPD_404_NOT_FOUND

enumerator HTTPD_404_NOT_FOUND

HTTPD_405_METHOD_NOT_ALLOWED

enumerator HTTPD_405_METHOD_NOT_ALLOWED

HTTPD_408_REQ_TIMEOUT

enumerator HTTPD_408_REQ_TIMEOUT

HTTPD_411_LENGTH_REQUIRED

enumerator HTTPD_411_LENGTH_REQUIRED

HTTPD_414_URI_TOO_LONG

enumerator HTTPD_414_URI_TOO_LONG

HTTPD_431_REQ_HDR_FIELDS_TOO_LARGE

enumerator HTTPD_431_REQ_HDR_FIELDS_TOO_LARGE

HTTPD_ERR_CODE_MAX

enumerator HTTPD_ERR_CODE_MAX


```

#### esp_http_server_event_id_t


```


enum esp_http_server_event_id_t
HTTP Server events id. 
Values:
HTTP_SERVER_EVENT_ERROR

enumerator HTTP_SERVER_EVENT_ERROR
This event occurs when there are any errors during execution 

HTTP_SERVER_EVENT_START

enumerator HTTP_SERVER_EVENT_START
This event occurs when HTTP Server is started 

HTTP_SERVER_EVENT_ON_CONNECTED

enumerator HTTP_SERVER_EVENT_ON_CONNECTED
Once the HTTP Server has been connected to the client, no data exchange has been performed 

HTTP_SERVER_EVENT_ON_HEADER

enumerator HTTP_SERVER_EVENT_ON_HEADER
Occurs when receiving each header sent from the client 

HTTP_SERVER_EVENT_HEADERS_SENT

enumerator HTTP_SERVER_EVENT_HEADERS_SENT
After sending all the headers to the client 

HTTP_SERVER_EVENT_ON_DATA

enumerator HTTP_SERVER_EVENT_ON_DATA
Occurs when receiving data from the client 

HTTP_SERVER_EVENT_SENT_DATA

enumerator HTTP_SERVER_EVENT_SENT_DATA
Occurs when an ESP HTTP server session is finished 

HTTP_SERVER_EVENT_DISCONNECTED

enumerator HTTP_SERVER_EVENT_DISCONNECTED
The connection has been disconnected 

HTTP_SERVER_EVENT_STOP

enumerator HTTP_SERVER_EVENT_STOP
This event occurs when HTTP Server is stopped 


```