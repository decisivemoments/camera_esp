# Protocol Communication




## Overview


The Protocol Communication (protocomm) component manages secure sessions and provides the framework for multiple transports. The application can also use the protocomm layer directly to have application-specific extensions for the provisioning or non-provisioning use cases.


Following features are available for provisioning:



> * Communication security at the application level
> 
> 
> 
> > + `protocomm_security0` (no security)
> > 	+ `protocomm_security1` (Curve25519 key exchange + AES-CTR encryption/decryption)
> > 	+ `protocomm_security2` (SRP6a-based key exchange + AES-GCM encryption/decryption)
> * Proof-of-possession (support with protocomm_security1 only)
> * Salt and Verifier (support with protocomm_security2 only)


Protocomm internally uses protobuf (protocol buffers) for secure session establishment. Users can choose to implement their own security (even without using protobuf). Protocomm can also be used without any security layer.


Protocomm provides the framework for various transports:


* Bluetooth LE
* Wi-Fi (SoftAP + HTTPD)
* Console, in which case the handler invocation is automatically taken care of on the device side. See Transport Examples below for code snippets.





Note that for protocomm_security1 and protocomm_security2, the client still needs to establish sessions by performing the two-way handshake.


See Unified Provisioning for more details about the secure handshake logic.




## Enabling Protocomm Security Version


The protocomm component provides a project configuration menu to enable/disable support of respective security versions. The respective configuration options are as follows:



> * Support `protocomm_security0`, with no security: CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_0, this option is enabled by default.
> * Support `protocomm_security1` with Curve25519 key exchange + AES-CTR encryption/decryption: CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_1, this option is enabled by default.
> * Support `protocomm_security2` with SRP6a-based key exchange + AES-GCM encryption/decryption: CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_2.



Note


Enabling multiple security versions at once offers the ability to control them dynamically but also increases the firmware size.





## SoftAP + HTTP Transport Example with Security 2


For sample usage, see wifi_provisioning/src/scheme_softap.c.



```
/* The endpoint handler to be registered with protocomm. This simply echoes back the received data. */
esp_err_t echo_req_handler (uint32_t session_id,
                            const uint8_t *inbuf, ssize_t inlen,
                            uint8_t **outbuf, ssize_t *outlen,
                            void *priv_data)
{
    /* Session ID may be used for persistence. */
    printf("Session ID : %d", session_id);

    /* Echo back the received data. */
    *outlen = inlen;            /* Output the data length updated. */
    *outbuf = malloc(inlen);    /* This is to be deallocated outside. */
    memcpy(*outbuf, inbuf, inlen);

    /* Private data that was passed at the time of endpoint creation. */
    uint32_t *priv = (uint32_t *) priv_data;
    if (priv) {
        printf("Private data : %d", *priv);
    }

    return ESP_OK;
}

static const char sec2_salt[] = {0xf7, 0x5f, 0xe2, 0xbe, 0xba, 0x7c, 0x81, 0xcd};
static const char sec2_verifier[] = {0xbf, 0x86, 0xce, 0x63, 0x8a, 0xbb, 0x7e, 0x2f, 0x38, 0xa8, 0x19, 0x1b, 0x35,
    0xc9, 0xe3, 0xbe, 0xc3, 0x2b, 0x45, 0xee, 0x10, 0x74, 0x22, 0x1a, 0x95, 0xbe, 0x62, 0xf7, 0x0c, 0x65, 0x83, 0x50,
    0x08, 0xef, 0xaf, 0xa5, 0x94, 0x4b, 0xcb, 0xe1, 0xce, 0x59, 0x2a, 0xe8, 0x7b, 0x27, 0xc8, 0x72, 0x26, 0x71, 0xde,
    0xb2, 0xf2, 0x80, 0x02, 0xdd, 0x11, 0xf0, 0x38, 0x0e, 0x95, 0x25, 0x00, 0xcf, 0xb3, 0x3f, 0xf0, 0x73, 0x2a, 0x25,
    0x03, 0xe8, 0x51, 0x72, 0xef, 0x6d, 0x3e, 0x14, 0xb9, 0x2e, 0x9f, 0x2a, 0x90, 0x9e, 0x26, 0xb6, 0x3e, 0xc7, 0xe4,
    0x9f, 0xe3, 0x20, 0xce, 0x28, 0x7c, 0xbf, 0x89, 0x50, 0xc9, 0xb6, 0xec, 0xdd, 0x81, 0x18, 0xf1, 0x1a, 0xd9, 0x7a,
    0x21, 0x99, 0xf1, 0xee, 0x71, 0x2f, 0xcc, 0x93, 0x16, 0x34, 0x0c, 0x79, 0x46, 0x23, 0xe4, 0x32, 0xec, 0x2d, 0x9e,
    0x18, 0xa6, 0xb9, 0xbb, 0x0a, 0xcf, 0xc4, 0xa8, 0x32, 0xc0, 0x1c, 0x32, 0xa3, 0x97, 0x66, 0xf8, 0x30, 0xb2, 0xda,
    0xf9, 0x8d, 0xc3, 0x72, 0x72, 0x5f, 0xe5, 0xee, 0xc3, 0x5c, 0x24, 0xc8, 0xdd, 0x54, 0x49, 0xfc, 0x12, 0x91, 0x81,
    0x9c, 0xc3, 0xac, 0x64, 0x5e, 0xd6, 0x41, 0x88, 0x2f, 0x23, 0x66, 0xc8, 0xac, 0xb0, 0x35, 0x0b, 0xf6, 0x9c, 0x88,
    0x6f, 0xac, 0xe1, 0xf4, 0xca, 0xc9, 0x07, 0x04, 0x11, 0xda, 0x90, 0x42, 0xa9, 0xf1, 0x97, 0x3d, 0x94, 0x65, 0xe4,
    0xfb, 0x52, 0x22, 0x3b, 0x7a, 0x7b, 0x9e, 0xe9, 0xee, 0x1c, 0x44, 0xd0, 0x73, 0x72, 0x2a, 0xca, 0x85, 0x19, 0x4a,
    0x60, 0xce, 0x0a, 0xc8, 0x7d, 0x57, 0xa4, 0xf8, 0x77, 0x22, 0xc1, 0xa5, 0xfa, 0xfb, 0x7b, 0x91, 0x3b, 0xfe, 0x87,
    0x5f, 0xfe, 0x05, 0xd2, 0xd6, 0xd3, 0x74, 0xe5, 0x2e, 0x68, 0x79, 0x34, 0x70, 0x40, 0x12, 0xa8, 0xe1, 0xb4, 0x6c,
    0xaa, 0x46, 0x73, 0xcd, 0x8d, 0x17, 0x72, 0x67, 0x32, 0x42, 0xdc, 0x10, 0xd3, 0x71, 0x7e, 0x8b, 0x00, 0x46, 0x9b,
    0x0a, 0xe9, 0xb4, 0x0f, 0xeb, 0x70, 0x52, 0xdd, 0x0a, 0x1c, 0x7e, 0x2e, 0xb0, 0x61, 0xa6, 0xe1, 0xa3, 0x34, 0x4b,
    0x2a, 0x3c, 0xc4, 0x5d, 0x42, 0x05, 0x58, 0x25, 0xd3, 0xca, 0x96, 0x5c, 0xb9, 0x52, 0xf9, 0xe9, 0x80, 0x75, 0x3d,
    0xc8, 0x9f, 0xc7, 0xb2, 0xaa, 0x95, 0x2e, 0x76, 0xb3, 0xe1, 0x48, 0xc1, 0x0a, 0xa1, 0x0a, 0xe8, 0xaf, 0x41, 0x28,
    0xd2, 0x16, 0xe1, 0xa6, 0xd0, 0x73, 0x51, 0x73, 0x79, 0x98, 0xd9, 0xb9, 0x00, 0x50, 0xa2, 0x4d, 0x99, 0x18, 0x90,
    0x70, 0x27, 0xe7, 0x8d, 0x56, 0x45, 0x34, 0x1f, 0xb9, 0x30, 0xda, 0xec, 0x4a, 0x08, 0x27, 0x9f, 0xfa, 0x59, 0x2e,
    0x36, 0x77, 0x00, 0xe2, 0xb6, 0xeb, 0xd1, 0x56, 0x50, 0x8e};

/* The example function for launching a protocomm instance over HTTP. */
protocomm_t *start_pc()
{
    protocomm_t *pc = protocomm_new();


    /* Config for protocomm_httpd_start(). */
    protocomm_httpd_config_t pc_config = {
        .data = {
        .config = PROTOCOMM_HTTPD_DEFAULT_CONFIG()
        }
    };

    /* Start the protocomm server on top of HTTP. */
    protocomm_httpd_start(pc, &pc_config);

    /* Create Security2 params object from salt and verifier. It must be valid throughout the scope of protocomm endpoint. This does not need to be static, i.e., could be dynamically allocated and freed at the time of endpoint removal. */
    const static protocomm_security2_params_t sec2_params = {
        .salt = (const uint8_t *) salt,
        .salt_len = sizeof(salt),
        .verifier = (const uint8_t *) verifier,
        .verifier_len = sizeof(verifier),
    };

    /* Set security for communication at the application level. Just like for request handlers, setting security creates an endpoint and registers the handler provided by protocomm_security1. One can similarly use protocomm_security0. Only one type of security can be set for a protocomm instance at a time. */
    protocomm_set_security(pc, "security_endpoint", &protocomm_security2, &sec2_params);

    /* Private data passed to the endpoint must be valid throughout the scope of protocomm endpoint. This need not be static, i.e., could be dynamically allocated and freed at the time of endpoint removal. */
    static uint32_t priv_data = 1234;

    /* Add a new endpoint for the protocomm instance, identified by a unique name, and register a handler function along with the private data to be passed at the time of handler execution. Multiple endpoints can be added as long as they are identified by unique names. */
    protocomm_add_endpoint(pc, "echo_req_endpoint",
                           echo_req_handler, (void *) &priv_data);
    return pc;
}

/* The example function for stopping a protocomm instance. */
void stop_pc(protocomm_t *pc)
{
    /* Remove the endpoint identified by its unique name. */
    protocomm_remove_endpoint(pc, "echo_req_endpoint");

    /* Remove the security endpoint identified by its name. */
    protocomm_unset_security(pc, "security_endpoint");

    /* Stop the HTTP server. */
    protocomm_httpd_stop(pc);

    /* Delete, namely deallocate the protocomm instance. */
    protocomm_delete(pc);
}

```




## SoftAP + HTTP Transport Example with Security 1


For sample usage, see wifi_provisioning/src/scheme_softap.c.



```
/* The endpoint handler to be registered with protocomm. This simply echoes back the received data. */
esp_err_t echo_req_handler (uint32_t session_id,
                            const uint8_t *inbuf, ssize_t inlen,
                            uint8_t **outbuf, ssize_t *outlen,
                            void *priv_data)
{
    /* Session ID may be used for persistence. */
    printf("Session ID : %d", session_id);

    /* Echo back the received data. */
    *outlen = inlen;            /* Output the data length updated. */
    *outbuf = malloc(inlen);    /* This is to be deallocated outside. */
    memcpy(*outbuf, inbuf, inlen);

    /* Private data that was passed at the time of endpoint creation. */
    uint32_t *priv = (uint32_t *) priv_data;
    if (priv) {
        printf("Private data : %d", *priv);
    }

    return ESP_OK;
}

/* The example function for launching a protocomm instance over HTTP. */
protocomm_t *start_pc(const char *pop_string)
{
    protocomm_t *pc = protocomm_new();


    /* Config for protocomm_httpd_start(). */
    protocomm_httpd_config_t pc_config = {
        .data = {
        .config = PROTOCOMM_HTTPD_DEFAULT_CONFIG()
        }
    };

    /* Start the protocomm server on top of HTTP. */
    protocomm_httpd_start(pc, &pc_config);

    /* Create security1 params object from pop_string. It must be valid throughout the scope of protocomm endpoint. This need not be static, i.e., could be dynamically allocated and freed at the time of endpoint removal. */
    const static protocomm_security1_params_t sec1_params = {
        .data = (const uint8_t *) strdup(pop_string),
        .len = strlen(pop_string)
    };

    /* Set security for communication at the application level. Just like for request handlers, setting security creates an endpoint and registers the handler provided by protocomm_security1. One can similarly use protocomm_security0. Only one type of security can be set for a protocomm instance at a time. */
    protocomm_set_security(pc, "security_endpoint", &protocomm_security1, &sec1_params);

    /* Private data passed to the endpoint must be valid throughout the scope of protocomm endpoint. This need not be static, i.e., could be dynamically allocated and freed at the time of endpoint removal. */
    static uint32_t priv_data = 1234;

    /* Add a new endpoint for the protocomm instance identified by a unique name, and register a handler function along with the private data to be passed at the time of handler execution. Multiple endpoints can be added as long as they are identified by unique names. */
    protocomm_add_endpoint(pc, "echo_req_endpoint",
                           echo_req_handler, (void *) &priv_data);
    return pc;
}

/* The example function for stopping a protocomm instance. */
void stop_pc(protocomm_t *pc)
{
    /* Remove the endpoint identified by its unique name. */
    protocomm_remove_endpoint(pc, "echo_req_endpoint");

    /* Remove the security endpoint identified by its name. */
    protocomm_unset_security(pc, "security_endpoint");

    /* Stop the HTTP server. */
    protocomm_httpd_stop(pc);

    /* Delete, namely deallocate the protocomm instance. */
    protocomm_delete(pc);
}

```




## Bluetooth LE Transport Example with Security 0


For sample usage, see wifi_provisioning/src/scheme_ble.c.



```
/* The example function for launching a secure protocomm instance over Bluetooth LE. */
protocomm_t *start_pc()
{
    protocomm_t *pc = protocomm_new();

    /* Endpoint UUIDs */
    protocomm_ble_name_uuid_t nu_lookup_table[] = {
        {"security_endpoint", 0xFF51},
        {"echo_req_endpoint", 0xFF52}
    };

    /* Config for protocomm_ble_start(). */
    protocomm_ble_config_t config = {
        .service_uuid = {
            /* LSB <---------------------------------------
            * ---------------------------------------> MSB */
            0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80,
            0x00, 0x10, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
        },
        .nu_lookup_count = sizeof(nu_lookup_table)/sizeof(nu_lookup_table[0]),
        .nu_lookup = nu_lookup_table
    };

    /* Start protocomm layer on top of Bluetooth LE. */
    protocomm_ble_start(pc, &config);

    /* For protocomm_security0, Proof of Possession is not used, and can be kept NULL. */
    protocomm_set_security(pc, "security_endpoint", &protocomm_security0, NULL);
    protocomm_add_endpoint(pc, "echo_req_endpoint", echo_req_handler, NULL);
    return pc;
}

/* The example function for stopping a protocomm instance. */
void stop_pc(protocomm_t *pc)
{
    protocomm_remove_endpoint(pc, "echo_req_endpoint");
    protocomm_unset_security(pc, "security_endpoint");

    /* Stop the Bluetooth LE protocomm service. */
    protocomm_ble_stop(pc);

    protocomm_delete(pc);
}

```




## API Reference



### Header File


* components/protocomm/include/common/protocomm.h
* This header file can be included with:



> ```
> #include "protocomm.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Functions


#### protocomm_new


```


protocomm_t *protocomm_new(void)
Create a new protocomm instance. 
This API will return a new dynamically allocated protocomm instance with all elements of the protocomm_t structure initialized to NULL.

Returns

protocomm_t* : On success
NULL : No memory for allocating new instance 





```

#### protocomm_delete


```


void protocomm_delete(protocomm_t *pc)
Delete a protocomm instance. 
This API will deallocate a protocomm instance that was created using protocomm_new().

Parameters
pc -- [in] Pointer to the protocomm instance to be deleted 



```

#### protocomm_add_endpoint


```


esp_err_t protocomm_add_endpoint(protocomm_t *pc, const char *ep_name, protocomm_req_handler_t h, void *priv_data)
Add endpoint request handler for a protocomm instance. 
This API will bind an endpoint handler function to the specified endpoint name, along with any private data that needs to be pass to the handler at the time of call.

Note

An endpoint must be bound to a valid protocomm instance, created using protocomm_new().
This function internally calls the registered add_endpoint() function of the selected transport which is a member of the protocomm_t instance structure.




Parameters

pc -- [in] Pointer to the protocomm instance 
ep_name -- [in] Endpoint identifier(name) string 
h -- [in] Endpoint handler function 
priv_data -- [in] Pointer to private data to be passed as a parameter to the handler function on call. Pass NULL if not needed.


Returns

ESP_OK : Success
ESP_FAIL : Error adding endpoint / Endpoint with this name already exists
ESP_ERR_NO_MEM : Error allocating endpoint resource
ESP_ERR_INVALID_ARG : Null instance/name/handler arguments 





```

#### protocomm_remove_endpoint


```


esp_err_t protocomm_remove_endpoint(protocomm_t *pc, const char *ep_name)
Remove endpoint request handler for a protocomm instance. 
This API will remove a registered endpoint handler identified by an endpoint name.

Note

This function internally calls the registered remove_endpoint() function which is a member of the protocomm_t instance structure.




Parameters

pc -- [in] Pointer to the protocomm instance 
ep_name -- [in] Endpoint identifier(name) string


Returns

ESP_OK : Success
ESP_ERR_NOT_FOUND : Endpoint with specified name doesn't exist
ESP_ERR_INVALID_ARG : Null instance/name arguments 





```

#### protocomm_open_session


```


esp_err_t protocomm_open_session(protocomm_t *pc, uint32_t session_id)
Allocates internal resources for new transport session. 

Note

An endpoint must be bound to a valid protocomm instance, created using protocomm_new().




Parameters

pc -- [in] Pointer to the protocomm instance 
session_id -- [in] Unique ID for a communication session


Returns

ESP_OK : Request handled successfully
ESP_ERR_NO_MEM : Error allocating internal resource
ESP_ERR_INVALID_ARG : Null instance/name arguments 





```

#### protocomm_close_session


```


esp_err_t protocomm_close_session(protocomm_t *pc, uint32_t session_id)
Frees internal resources used by a transport session. 

Note

An endpoint must be bound to a valid protocomm instance, created using protocomm_new().




Parameters

pc -- [in] Pointer to the protocomm instance 
session_id -- [in] Unique ID for a communication session


Returns

ESP_OK : Request handled successfully
ESP_ERR_INVALID_ARG : Null instance/name arguments 





```

#### protocomm_req_handle


```


esp_err_t protocomm_req_handle(protocomm_t *pc, const char *ep_name, uint32_t session_id, const uint8_t *inbuf, ssize_t inlen, uint8_t **outbuf, ssize_t *outlen)
Calls the registered handler of an endpoint session for processing incoming data and generating the response. 

Note

An endpoint must be bound to a valid protocomm instance, created using protocomm_new().
Resulting output buffer must be deallocated by the caller.




Parameters

pc -- [in] Pointer to the protocomm instance 
ep_name -- [in] Endpoint identifier(name) string 
session_id -- [in] Unique ID for a communication session 
inbuf -- [in] Input buffer contains input request data which is to be processed by the registered handler 
inlen -- [in] Length of the input buffer 
outbuf -- [out] Pointer to internally allocated output buffer, where the resulting response data output from the registered handler is to be stored 
outlen -- [out] Buffer length of the allocated output buffer


Returns

ESP_OK : Request handled successfully
ESP_FAIL : Internal error in execution of registered handler
ESP_ERR_NO_MEM : Error allocating internal resource
ESP_ERR_NOT_FOUND : Endpoint with specified name doesn't exist
ESP_ERR_INVALID_ARG : Null instance/name arguments 





```

#### protocomm_set_security


```


esp_err_t protocomm_set_security(protocomm_t *pc, const char *ep_name, const protocomm_security_t *sec, const void *sec_params)
Add endpoint security for a protocomm instance. 
This API will bind a security session establisher to the specified endpoint name, along with any proof of possession that may be required for authenticating a session client.

Note

An endpoint must be bound to a valid protocomm instance, created using protocomm_new().
The choice of security can be any protocomm_security_t instance. Choices protocomm_security0 and protocomm_security1 and protocomm_security2 are readily available.




Parameters

pc -- [in] Pointer to the protocomm instance 
ep_name -- [in] Endpoint identifier(name) string 
sec -- [in] Pointer to endpoint security instance 
sec_params -- [in] Pointer to security params (NULL if not needed) The pointer should contain the security params struct of appropriate security version. For protocomm security version 1 and 2 sec_params should contain pointer to struct of type protocomm_security1_params_t and protocmm_security2_params_t respectively. The contents of this pointer must be valid till the security session has been running and is not closed. 


Returns

ESP_OK : Success
ESP_FAIL : Error adding endpoint / Endpoint with this name already exists
ESP_ERR_INVALID_STATE : Security endpoint already set
ESP_ERR_NO_MEM : Error allocating endpoint resource
ESP_ERR_INVALID_ARG : Null instance/name/handler arguments 





```

#### protocomm_unset_security


```


esp_err_t protocomm_unset_security(protocomm_t *pc, const char *ep_name)
Remove endpoint security for a protocomm instance. 
This API will remove a registered security endpoint identified by an endpoint name.

Parameters

pc -- [in] Pointer to the protocomm instance 
ep_name -- [in] Endpoint identifier(name) string


Returns

ESP_OK : Success
ESP_ERR_NOT_FOUND : Endpoint with specified name doesn't exist
ESP_ERR_INVALID_ARG : Null instance/name arguments 





```

#### protocomm_set_version


```


esp_err_t protocomm_set_version(protocomm_t *pc, const char *ep_name, const char *version)
Set endpoint for version verification. 
This API can be used for setting an application specific protocol version which can be verified by clients through the endpoint.

Note

An endpoint must be bound to a valid protocomm instance, created using protocomm_new().




Parameters

pc -- [in] Pointer to the protocomm instance 
ep_name -- [in] Endpoint identifier(name) string 
version -- [in] Version identifier(name) string


Returns

ESP_OK : Success
ESP_FAIL : Error adding endpoint / Endpoint with this name already exists
ESP_ERR_INVALID_STATE : Version endpoint already set
ESP_ERR_NO_MEM : Error allocating endpoint resource
ESP_ERR_INVALID_ARG : Null instance/name/handler arguments 





```

#### protocomm_unset_version


```


esp_err_t protocomm_unset_version(protocomm_t *pc, const char *ep_name)
Remove version verification endpoint from a protocomm instance. 
This API will remove a registered version endpoint identified by an endpoint name.

Parameters

pc -- [in] Pointer to the protocomm instance 
ep_name -- [in] Endpoint identifier(name) string


Returns

ESP_OK : Success
ESP_ERR_NOT_FOUND : Endpoint with specified name doesn't exist
ESP_ERR_INVALID_ARG : Null instance/name arguments 





```



### Type Definitions


#### protocomm_req_handler_t


```


typedef esp_err_t (*protocomm_req_handler_t)(uint32_t session_id, const uint8_t *inbuf, ssize_t inlen, uint8_t **outbuf, ssize_t *outlen, void *priv_data)
Function prototype for protocomm endpoint handler. 

```

#### protocomm_t


```


typedef struct protocomm protocomm_t
This structure corresponds to a unique instance of protocomm returned when the API protocomm_new() is called. The remaining Protocomm APIs require this object as the first parameter. 

Note
Structure of the protocomm object is kept private 


```



### Header File


* components/protocomm/include/security/protocomm_security.h
* This header file can be included with:



> ```
> #include "protocomm_security.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Structures


#### protocomm_security1_params


```


struct protocomm_security1_params
Protocomm Security 1 parameters: Proof Of Possession. 

Public Members
data

const uint8_t *data
Pointer to buffer containing the proof of possession data 

len

uint16_t len
Length (in bytes) of the proof of possession data 



```

#### protocomm_security2_params


```


struct protocomm_security2_params
Protocomm Security 2 parameters: Salt and Verifier. 

Public Members
salt

const char *salt
Pointer to the buffer containing the salt 

salt_len

uint16_t salt_len
Length (in bytes) of the salt 

verifier

const char *verifier
Pointer to the buffer containing the verifier 

verifier_len

uint16_t verifier_len
Length (in bytes) of the verifier 



```

#### protocomm_security


```


struct protocomm_security
Protocomm security object structure. 
The member functions are used for implementing secure protocomm sessions.

Note
This structure should not have any dynamic members to allow re-entrancy 


Public Members
ver

int ver
Unique version number of security implementation 

init

esp_err_t (*init)(protocomm_security_handle_t *handle)
Function for initializing/allocating security infrastructure 

cleanup

esp_err_t (*cleanup)(protocomm_security_handle_t handle)
Function for deallocating security infrastructure 

new_transport_session

esp_err_t (*new_transport_session)(protocomm_security_handle_t handle, uint32_t session_id)
Starts new secure transport session with specified ID 

close_transport_session

esp_err_t (*close_transport_session)(protocomm_security_handle_t handle, uint32_t session_id)
Closes a secure transport session with specified ID 

security_req_handler

esp_err_t (*security_req_handler)(protocomm_security_handle_t handle, const void *sec_params, uint32_t session_id, const uint8_t *inbuf, ssize_t inlen, uint8_t **outbuf, ssize_t *outlen, void *priv_data)
Handler function for authenticating connection request and establishing secure session 

encrypt

esp_err_t (*encrypt)(protocomm_security_handle_t handle, uint32_t session_id, const uint8_t *inbuf, ssize_t inlen, uint8_t **outbuf, ssize_t *outlen)
Function which implements the encryption algorithm 

decrypt

esp_err_t (*decrypt)(protocomm_security_handle_t handle, uint32_t session_id, const uint8_t *inbuf, ssize_t inlen, uint8_t **outbuf, ssize_t *outlen)
Function which implements the decryption algorithm 



```



### Type Definitions


#### protocomm_security1_params_t


```


typedef struct protocomm_security1_params protocomm_security1_params_t
Protocomm Security 1 parameters: Proof Of Possession. 

```

#### protocomm_security_pop_t


```


typedef protocomm_security1_params_t protocomm_security_pop_t

```

#### protocomm_security2_params_t


```


typedef struct protocomm_security2_params protocomm_security2_params_t
Protocomm Security 2 parameters: Salt and Verifier. 

```

#### protocomm_security_handle_t


```


typedef void *protocomm_security_handle_t

```

#### protocomm_security_t


```


typedef struct protocomm_security protocomm_security_t
Protocomm security object structure. 
The member functions are used for implementing secure protocomm sessions.

Note
This structure should not have any dynamic members to allow re-entrancy 


```



### Enumerations


#### protocomm_security_session_event_t


```


enum protocomm_security_session_event_t
Events generated by the protocomm security layer. 
These events are generated while establishing secured session. 
Values:
PROTOCOMM_SECURITY_SESSION_SETUP_OK

enumerator PROTOCOMM_SECURITY_SESSION_SETUP_OK
Secured session established successfully 

PROTOCOMM_SECURITY_SESSION_INVALID_SECURITY_PARAMS

enumerator PROTOCOMM_SECURITY_SESSION_INVALID_SECURITY_PARAMS
Received invalid (NULL) security parameters (username / client public-key) 

PROTOCOMM_SECURITY_SESSION_CREDENTIALS_MISMATCH

enumerator PROTOCOMM_SECURITY_SESSION_CREDENTIALS_MISMATCH
Received incorrect credentials (username / PoP) 


```



### Header File


* components/protocomm/include/security/protocomm_security0.h
* This header file can be included with:



> ```
> #include "protocomm_security0.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Header File


* components/protocomm/include/security/protocomm_security1.h
* This header file can be included with:



> ```
> #include "protocomm_security1.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Header File


* components/protocomm/include/security/protocomm_security2.h
* This header file can be included with:



> ```
> #include "protocomm_security2.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Header File


* components/protocomm/include/crypto/srp6a/esp_srp.h
* This header file can be included with:



> ```
> #include "esp_srp.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Functions


#### esp_srp_init


```


esp_srp_handle_t *esp_srp_init(esp_ng_type_t ng)
Initialize srp context for given NG type. 

Note
the handle gets freed with esp_srp_free


Parameters
ng -- NG type given by esp_ng_type_t

Returns
esp_srp_handle_t* srp handle



```

#### esp_srp_free


```


void esp_srp_free(esp_srp_handle_t *hd)
free esp_srp_context 

Parameters
hd -- handle to be free 



```

#### esp_srp_srv_pubkey


```


esp_err_t esp_srp_srv_pubkey(esp_srp_handle_t *hd, const char *username, int username_len, const char *pass, int pass_len, int salt_len, char **bytes_B, int *len_B, char **bytes_salt)
Returns B (pub key) and salt. [Step2.b]. 

Note
*bytes_B MUST NOT BE FREED BY THE CALLER 


Note
*bytes_salt MUST NOT BE FREE BY THE CALLER 


Parameters

hd -- esp_srp handle 
username -- Username not expected NULL terminated 
username_len -- Username length 
pass -- Password not expected to be NULL terminated 
pass_len -- Pasword length 
salt_len -- Salt length 
bytes_B -- Public Key returned 
len_B -- Length of the public key 
bytes_salt -- Salt bytes generated 


Returns
esp_err_t ESP_OK on success, appropriate error otherwise



```

#### esp_srp_gen_salt_verifier


```


esp_err_t esp_srp_gen_salt_verifier(const char *username, int username_len, const char *pass, int pass_len, char **bytes_salt, int salt_len, char **verifier, int *verifier_len)
Generate salt-verifier pair, given username, password and salt length. 

Note
if API has returned ESP_OK, salt and verifier generated need to be freed by caller 


Note
Usually, username and password are not saved on the device. Rather salt and verifier are generated outside the device and are embedded. this covenience API can be used to generate salt and verifier on the fly for development use case. OR for devices which intentionally want to generate different password each time and can send it to the client securely. e.g., a device has a display and it shows the pin 


Parameters

username -- [in] username 
username_len -- [in] length of the username 
pass -- [in] password 
pass_len -- [in] length of the password 
bytes_salt -- [out] generated salt on successful generation, or NULL 
salt_len -- [in] salt length 
verifier -- [out] generated verifier on successful generation, or NULL 
verifier_len -- [out] length of the generated verifier 


Returns
esp_err_t ESP_OK on success, appropriate error otherwise



```

#### esp_srp_set_salt_verifier


```


esp_err_t esp_srp_set_salt_verifier(esp_srp_handle_t *hd, const char *salt, int salt_len, const char *verifier, int verifier_len)
Set the Salt and Verifier pre-generated for a given password. This should be used only if the actual password is not available. The public key can then be generated using esp_srp_srv_pubkey_from_salt_verifier() and not esp_srp_srv_pubkey()

Parameters

hd -- esp_srp_handle 
salt -- pre-generated salt bytes 
salt_len -- length of the salt bytes 
verifier -- pre-generated verifier 
verifier_len -- length of the verifier bytes 


Returns
esp_err_t ESP_OK on success, appropriate error otherwise 



```

#### esp_srp_srv_pubkey_from_salt_verifier


```


esp_err_t esp_srp_srv_pubkey_from_salt_verifier(esp_srp_handle_t *hd, char **bytes_B, int *len_B)
Returns B (pub key)[Step2.b] when the salt and verifier are set using esp_srp_set_salt_verifier()

Note
*bytes_B MUST NOT BE FREED BY THE CALLER 


Parameters

hd -- esp_srp handle 
bytes_B -- Key returned to the called 
len_B -- Length of the key returned 


Returns
esp_err_t ESP_OK on success, appropriate error otherwise



```

#### esp_srp_get_session_key


```


esp_err_t esp_srp_get_session_key(esp_srp_handle_t *hd, char *bytes_A, int len_A, char **bytes_key, uint16_t *len_key)
Get session key in *bytes_key given by len in *len_key. [Step2.c]. 
This calculated session key is used for further communication given the proofs are exchanged/authenticated with esp_srp_exchange_proofs

Note
*bytes_key MUST NOT BE FREED BY THE CALLER 


Parameters

hd -- esp_srp handle 
bytes_A -- Private Key 
len_A -- Private Key length 
bytes_key -- Key returned to the caller 
len_key -- length of the key in *bytes_key 


Returns
esp_err_t ESP_OK on success, appropriate error otherwise



```

#### esp_srp_exchange_proofs


```


esp_err_t esp_srp_exchange_proofs(esp_srp_handle_t *hd, char *username, uint16_t username_len, char *bytes_user_proof, char *bytes_host_proof)
Complete the authentication. If this step fails, the session_key exchanged should not be used. 
This is the final authentication step in SRP algorithm [Step4.1, Step4.b, Step4.c]

Parameters

hd -- esp_srp handle 
username -- Username not expected NULL terminated 
username_len -- Username length 
bytes_user_proof -- param in 
bytes_host_proof -- parameter out (should be SHA512_DIGEST_LENGTH) bytes in size 


Returns
esp_err_t ESP_OK if user's proof is ok and subsequently bytes_host_proof is populated with our own proof. 



```



### Type Definitions


#### esp_srp_handle_t


```


typedef struct esp_srp_handle esp_srp_handle_t
esp_srp handle as the result of esp_srp_init
The handle is returned by esp_srp_init on successful init. It is then passed for subsequent API calls as an argument. esp_srp_free can be used to clean up the handle. After esp_srp_free the handle becomes invalid. 

```



### Enumerations


#### esp_ng_type_t


```


enum esp_ng_type_t
Large prime+generator to be used for the algorithm. 
Values:
ESP_NG_3072

enumerator ESP_NG_3072


```



### Header File


* components/protocomm/include/transports/protocomm_httpd.h
* This header file can be included with:



> ```
> #include "protocomm_httpd.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Functions


#### protocomm_httpd_start


```


esp_err_t protocomm_httpd_start(protocomm_t *pc, const protocomm_httpd_config_t *config)
Start HTTPD protocomm transport. 
This API internally creates a framework to allow endpoint registration and security configuration for the protocomm.

Note
This is a singleton. ie. Protocomm can have multiple instances, but only one instance can be bound to an HTTP transport layer.


Parameters

pc -- [in] Protocomm instance pointer obtained from protocomm_new() 
config -- [in] Pointer to config structure for initializing HTTP server


Returns

ESP_OK : Success
ESP_ERR_INVALID_ARG : Null arguments
ESP_ERR_NOT_SUPPORTED : Transport layer bound to another protocomm instance
ESP_ERR_INVALID_STATE : Transport layer already bound to this protocomm instance
ESP_ERR_NO_MEM : Memory allocation for server resource failed
ESP_ERR_HTTPD_* : HTTP server error on start 





```

#### protocomm_httpd_stop


```


esp_err_t protocomm_httpd_stop(protocomm_t *pc)
Stop HTTPD protocomm transport. 
This API cleans up the HTTPD transport protocomm and frees all the handlers registered with the protocomm.

Parameters
pc -- [in] Same protocomm instance that was passed to protocomm_httpd_start()

Returns

ESP_OK : Success
ESP_ERR_INVALID_ARG : Null / incorrect protocomm instance pointer 





```



### Unions


#### protocomm_httpd_config_data_t


```


union protocomm_httpd_config_data_t

#include <protocomm_httpd.h>
Protocomm HTTPD Configuration Data 

Public Members
handle

void *handle
HTTP Server Handle, if ext_handle_provided is set to true 

config

protocomm_http_server_config_t config
HTTP Server Configuration, if a server is not already active 



```



### Structures


#### protocomm_http_server_config_t


```


struct protocomm_http_server_config_t
Config parameters for protocomm HTTP server. 

Public Members
port

uint16_t port
Port on which the HTTP server will listen 

stack_size

size_t stack_size
Stack size of server task, adjusted depending upon stack usage of endpoint handler 

task_priority

unsigned task_priority
Priority of server task 



```

#### protocomm_httpd_config_t


```


struct protocomm_httpd_config_t
Config parameters for protocomm HTTP server. 

Public Members
ext_handle_provided

bool ext_handle_provided
Flag to indicate of an external HTTP Server Handle has been provided. In such as case, protocomm will use the same HTTP Server and not start a new one internally. 

data

protocomm_httpd_config_data_t data
Protocomm HTTPD Configuration Data 



```



### Macros


#### PROTOCOMM_HTTPD_DEFAULT_CONFIG


```


PROTOCOMM_HTTPD_DEFAULT_CONFIG()

```



### Header File


* components/protocomm/include/transports/protocomm_ble.h
* This header file can be included with:



> ```
> #include "protocomm_ble.h"
> 
> ```
* This header file is a part of the API provided by the `protocomm` component. To declare that your component depends on `protocomm`, add the following to your CMakeLists.txt:



> ```
> REQUIRES protocomm
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES protocomm
> 
> ```




### Functions


#### protocomm_ble_start


```


esp_err_t protocomm_ble_start(protocomm_t *pc, const protocomm_ble_config_t *config)
Start Bluetooth Low Energy based transport layer for provisioning. 
Initialize and start required BLE service for provisioning. This includes the initialization for characteristics/service for BLE.

Parameters

pc -- [in] Protocomm instance pointer obtained from protocomm_new() 
config -- [in] Pointer to config structure for initializing BLE


Returns

ESP_OK : Success
ESP_FAIL : Simple BLE start error
ESP_ERR_NO_MEM : Error allocating memory for internal resources
ESP_ERR_INVALID_STATE : Error in ble config
ESP_ERR_INVALID_ARG : Null arguments 





```

#### protocomm_ble_stop


```


esp_err_t protocomm_ble_stop(protocomm_t *pc)
Stop Bluetooth Low Energy based transport layer for provisioning. 
Stops service/task responsible for BLE based interactions for provisioning

Note
You might want to optionally reclaim memory from Bluetooth. Refer to the documentation of esp_bt_mem_release in that case.


Parameters
pc -- [in] Same protocomm instance that was passed to protocomm_ble_start()

Returns

ESP_OK : Success
ESP_FAIL : Simple BLE stop error
ESP_ERR_INVALID_ARG : Null / incorrect protocomm instance 





```



### Structures


#### name_uuid


```


struct name_uuid
This structure maps handler required by protocomm layer to UUIDs which are used to uniquely identify BLE characteristics from a smartphone or a similar client device. 

Public Members
name

const char *name
Name of the handler, which is passed to protocomm layer 

uuid

uint16_t uuid
UUID to be assigned to the BLE characteristic which is mapped to the handler 



```

#### protocomm_ble_event_t


```


struct protocomm_ble_event_t
Structure for BLE events in Protocomm. 

Public Members
evt_type

uint16_t evt_type
This field indicates the type of BLE event that occurred. 

conn_handle

uint16_t conn_handle
The handle of the relevant connection. 

conn_status

uint16_t conn_status
The status of the connection attempt; o 0: the connection was successfully established. o BLE host error code: the connection attempt failed for the specified reason. 

disconnect_reason

uint16_t disconnect_reason
Return code indicating the reason for the disconnect. 



```

#### protocomm_ble_config


```


struct protocomm_ble_config
Config parameters for protocomm BLE service. 

Public Members
device_name

char device_name[MAX_BLE_DEVNAME_LEN + 1]
BLE device name being broadcast at the time of provisioning 

service_uuid

uint8_t service_uuid[BLE_UUID128_VAL_LENGTH]
128 bit UUID of the provisioning service 

manufacturer_data

uint8_t *manufacturer_data
BLE device manufacturer data pointer in advertisement 

manufacturer_data_len

ssize_t manufacturer_data_len
BLE device manufacturer data length in advertisement 

nu_lookup_count

ssize_t nu_lookup_count
Number of entries in the Name-UUID lookup table 

nu_lookup

protocomm_ble_name_uuid_t *nu_lookup
Pointer to the Name-UUID lookup table 

ble_bonding

unsigned ble_bonding
BLE bonding 

ble_sm_sc

unsigned ble_sm_sc
BLE security flag 

ble_link_encryption

unsigned ble_link_encryption
BLE security flag 

ble_addr

uint8_t *ble_addr
BLE address 



```



### Macros


#### MAX_BLE_DEVNAME_LEN


```


MAX_BLE_DEVNAME_LEN
BLE device name cannot be larger than this value 31 bytes (max scan response size) - 1 byte (length) - 1 byte (type) = 29 bytes 

```

#### BLE_UUID128_VAL_LENGTH


```


BLE_UUID128_VAL_LENGTH

```

#### MAX_BLE_MANUFACTURER_DATA_LEN


```


MAX_BLE_MANUFACTURER_DATA_LEN
Theoretically, the limit for max manufacturer length remains same as BLE device name i.e. 31 bytes (max scan response size) - 1 byte (length) - 1 byte (type) = 29 bytes However, manufacturer data goes along with BLE device name in scan response. So, it is important to understand the actual length should be smaller than (29 - (BLE device name length) - 2). 

```

#### BLE_ADDR_LEN


```


BLE_ADDR_LEN

```



### Type Definitions


#### protocomm_ble_name_uuid_t


```


typedef struct name_uuid protocomm_ble_name_uuid_t
This structure maps handler required by protocomm layer to UUIDs which are used to uniquely identify BLE characteristics from a smartphone or a similar client device. 

```

#### protocomm_ble_config_t


```


typedef struct protocomm_ble_config protocomm_ble_config_t
Config parameters for protocomm BLE service. 

```



### Enumerations


#### protocomm_transport_ble_event_t


```


enum protocomm_transport_ble_event_t
Events generated by BLE transport. 
These events are generated when the BLE transport is paired and disconnected. 
Values:
PROTOCOMM_TRANSPORT_BLE_CONNECTED

enumerator PROTOCOMM_TRANSPORT_BLE_CONNECTED

PROTOCOMM_TRANSPORT_BLE_DISCONNECTED

enumerator PROTOCOMM_TRANSPORT_BLE_DISCONNECTED


```