# ESP Local Control




## Overview


ESP Local Control (**esp_local_ctrl**) component in ESP-IDF provides capability to control an ESP device over HTTPS or Bluetooth® Low Energy. It provides access to application defined **properties** that are available for reading/writing via a set of configurable handlers.


Initialization of the **esp_local_ctrl** service over Bluetooth Low Energy transport is performed as follows:



> ```
> esp_local_ctrl_config_t config = {
>     .transport = ESP_LOCAL_CTRL_TRANSPORT_BLE,
>     .transport_config = {
>         .ble = & (protocomm_ble_config_t) {
>             .device_name  = SERVICE_NAME,
>             .service_uuid = {
>                 /* LSB <---------------------------------------
>                 * ---------------------------------------> MSB */
>                 0x21, 0xd5, 0x3b, 0x8d, 0xbd, 0x75, 0x68, 0x8a,
>                 0xb4, 0x42, 0xeb, 0x31, 0x4a, 0x1e, 0x98, 0x3d
>             }
>         }
>     },
>     .proto_sec = {
>         .version = PROTOCOM_SEC0,
>         .custom_handle = NULL,
>         .sec_params = NULL,
>     },
>     .handlers = {
>         /* User defined handler functions */
>         .get_prop_values = get_property_values,
>         .set_prop_values = set_property_values,
>         .usr_ctx         = NULL,
>         .usr_ctx_free_fn = NULL
>     },
>     /* Maximum number of properties that may be set */
>     .max_properties = 10
> };
> 
> /* Start esp_local_ctrl service */
> ESP_ERROR_CHECK(esp_local_ctrl_start(&config));
> 
> ```


Initialization of the **esp_local_ctrl** service over HTTPS transport is performed as follows:



> ```
> /* Set the configuration */
> httpd_ssl_config_t https_conf = HTTPD_SSL_CONFIG_DEFAULT();
> 
> /* Load server certificate */
> extern const unsigned char servercert_start[] asm("_binary_servercert_pem_start");
> extern const unsigned char servercert_end[]   asm("_binary_servercert_pem_end");
> https_conf.servercert = servercert_start;
> https_conf.servercert_len = servercert_end - servercert_start;
> 
> /* Load server private key */
> extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
> extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
> https_conf.prvtkey_pem = prvtkey_pem_start;
> https_conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;
> 
> esp_local_ctrl_config_t config = {
>     .transport = ESP_LOCAL_CTRL_TRANSPORT_HTTPD,
>     .transport_config = {
>         .httpd = &https_conf
>     },
>     .proto_sec = {
>         .version = PROTOCOM_SEC0,
>         .custom_handle = NULL,
>         .sec_params = NULL,
>     },
>     .handlers = {
>         /* User defined handler functions */
>         .get_prop_values = get_property_values,
>         .set_prop_values = set_property_values,
>         .usr_ctx         = NULL,
>         .usr_ctx_free_fn = NULL
>     },
>     /* Maximum number of properties that may be set */
>     .max_properties = 10
> };
> 
> /* Start esp_local_ctrl service */
> ESP_ERROR_CHECK(esp_local_ctrl_start(&config));
> 
> ```


You may set security for transport in ESP local control using following options:


1. `PROTOCOM_SEC2`: specifies that SRP6a-based key exchange and end-to-end encryption based on AES-GCM are used. This is the most preferred option as it adds a robust security with Augmented PAKE protocol, i.e., SRP6a.
2. `PROTOCOM_SEC1`: specifies that Curve25519-based key exchange and end-to-end encryption based on AES-CTR are used.
3. `PROTOCOM_SEC0`: specifies that data will be exchanged as a plain text (no security).
4. `PROTOCOM_SEC_CUSTOM`: you can define your own security requirement. Please note that you will also have to provide `custom_handle` of type `protocomm_security_t *` in this context.



Note


The respective security schemes need to be enabled through the project configuration menu. Please refer to the Enabling protocom security version section in Protocol Communication for more details.





## Creating a Property


Now that we know how to start the **esp_local_ctrl** service, let's add a property to it. Each property must have a unique ``name`` (string), a `type` (e.g., enum), `flags`` (bit fields) and `size``.


The `size` is to be kept 0, if we want our property value to be of variable length (e.g., if it is a string or bytestream). For data types with fixed-length property value, like int, float, etc., setting the `size` field to the right value helps **esp_local_ctrl** to perform internal checks on arguments received with write requests.


The interpretation of `type` and `flags` fields is totally upto the application, hence they may be used as enumerations, bitfields, or even simple integers. One way is to use `type` values to classify properties, while `flags` to specify characteristics of a property.


Here is an example property which is to function as a timestamp. It is assumed that the application defines `TYPE_TIMESTAMP` and `READONLY`, which are used for setting the `type` and `flags` fields here.



> ```
> /* Create a timestamp property */
> esp_local_ctrl_prop_t timestamp = {
>     .name        = "timestamp",
>     .type        = TYPE_TIMESTAMP,
>     .size        = sizeof(int32_t),
>     .flags       = READONLY,
>     .ctx         = func_get_time,
>     .ctx_free_fn = NULL
> };
> 
> /* Now register the property */
> esp_local_ctrl_add_property(&timestamp);
> 
> ```


Also notice that there is a ctx field, which is set to point to some custom func_get_time(). This can be used inside the property get/set handlers to retrieve timestamp.


Here is an example of `get_prop_values()` handler, which is used for retrieving the timestamp.



> ```
> static esp_err_t get_property_values(size_t props_count,
>                                      const esp_local_ctrl_prop_t *props,
>                                      esp_local_ctrl_prop_val_t *prop_values,
>                                      void *usr_ctx)
> {
>     for (uint32_t i = 0; i < props_count; i++) {
>         ESP_LOGI(TAG, "Reading %s", props[i].name);
>         if (props[i].type == TYPE_TIMESTAMP) {
>             /* Obtain the timer function from ctx */
>             int32_t (*func_get_time)(void) = props[i].ctx;
> 
>             /* Use static variable for saving the value. This is essential because the value has to be valid even after this function returns. Alternative is to use dynamic allocation and set the free_fn field */
>             static int32_t ts = func_get_time();
>             prop_values[i].data = &ts;
>         }
>     }
>     return ESP_OK;
> }
> 
> ```


Here is an example of `set_prop_values()` handler. Notice how we restrict from writing to read-only properties.



> ```
> static esp_err_t set_property_values(size_t props_count,
>                                      const esp_local_ctrl_prop_t *props,
>                                      const esp_local_ctrl_prop_val_t *prop_values,
>                                      void *usr_ctx)
> {
>     for (uint32_t i = 0; i < props_count; i++) {
>         if (props[i].flags & READONLY) {
>             ESP_LOGE(TAG, "Cannot write to read-only property %s", props[i].name);
>             return ESP_ERR_INVALID_ARG;
>         } else {
>             ESP_LOGI(TAG, "Setting %s", props[i].name);
> 
>             /* For keeping it simple, lets only log the incoming data */
>             ESP_LOG_BUFFER_HEX_LEVEL(TAG, prop_values[i].data,
>                                      prop_values[i].size, ESP_LOG_INFO);
>         }
>     }
>     return ESP_OK;
> }
> 
> ```


For complete example see protocols/esp_local_ctrl.




## Client Side Implementation


The client side implementation establishes a protocomm session with the device first, over the supported mode of transport, and then send and receive protobuf messages understood by the **esp_local_ctrl** service. The service translates these messages into requests and then call the appropriate handlers (set/get). Then, the generated response for each handler is again packed into a protobuf message and transmitted back to the client.


See below the various protobuf messages understood by the **esp_local_ctrl** service:


1. `get_prop_count` : This should simply return the total number of properties supported by the service.
2. `get_prop_values` : This accepts an array of indices and should return the information (name, type, flags) and values of the properties corresponding to those indices.
3. `set_prop_values` : This accepts an array of indices and an array of new values, which are used for setting the values of the properties corresponding to the indices.


Note that indices may or may not be the same for a property, across multiple sessions. Therefore, the client must only use the names of the properties to uniquely identify them. So, every time a new session is established, the client should first call `get_prop_count` and then `get_prop_values`, hence form an index-to-name mapping for all properties. Now when calling `set_prop_values` for a set of properties, it must first convert the names to indexes, using the created mapping. As emphasized earlier, the client must refresh the index-to-name mapping every time a new session is established with the same device.


The various protocomm endpoints provided by **esp_local_ctrl** are listed below:




Endpoints provided by ESP Local Control





| Endpoint Name (Bluetooth Low Energy + GATT Server) | URI (HTTPS Server + mDNS) | Description |
| --- | --- | --- |
| esp_local_ctrl/version | https://<mdns-hostname>.local/esp_local_ctrl/version | Endpoint used for retrieving version string |
| esp_local_ctrl/control | https://<mdns-hostname>.local/esp_local_ctrl/control | Endpoint used for sending or receiving control messages |




## API Reference



### Header File


* components/esp_local_ctrl/include/esp_local_ctrl.h
* This header file can be included with:



> ```
> #include "esp_local_ctrl.h"
> 
> ```
* This header file is a part of the API provided by the `esp_local_ctrl` component. To declare that your component depends on `esp_local_ctrl`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_local_ctrl
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_local_ctrl
> 
> ```




### Functions


#### esp_local_ctrl_get_transport_ble


```


const esp_local_ctrl_transport_t *esp_local_ctrl_get_transport_ble(void)
Function for obtaining BLE transport mode. 

```

#### esp_local_ctrl_get_transport_httpd


```


const esp_local_ctrl_transport_t *esp_local_ctrl_get_transport_httpd(void)
Function for obtaining HTTPD transport mode. 

```

#### esp_local_ctrl_start


```


esp_err_t esp_local_ctrl_start(const esp_local_ctrl_config_t *config)
Start local control service. 

Parameters
config -- [in] Pointer to configuration structure

Returns

ESP_OK : Success
ESP_FAIL : Failure 





```

#### esp_local_ctrl_stop


```


esp_err_t esp_local_ctrl_stop(void)
Stop local control service. 

```

#### esp_local_ctrl_add_property


```


esp_err_t esp_local_ctrl_add_property(const esp_local_ctrl_prop_t *prop)
Add a new property. 
This adds a new property and allocates internal resources for it. The total number of properties that could be added is limited by configuration option max_properties

Parameters
prop -- [in] Property description structure

Returns

ESP_OK : Success
ESP_FAIL : Failure 





```

#### esp_local_ctrl_remove_property


```


esp_err_t esp_local_ctrl_remove_property(const char *name)
Remove a property. 
This finds a property by name, and releases the internal resources which are associated with it.

Parameters
name -- [in] Name of the property to remove

Returns

ESP_OK : Success
ESP_ERR_NOT_FOUND : Failure 





```

#### esp_local_ctrl_get_property


```


const esp_local_ctrl_prop_t *esp_local_ctrl_get_property(const char *name)
Get property description structure by name. 
This API may be used to get a property's context structure esp_local_ctrl_prop_t when its name is known

Parameters
name -- [in] Name of the property to find

Returns

Pointer to property
NULL if not found 





```

#### esp_local_ctrl_set_handler


```


esp_err_t esp_local_ctrl_set_handler(const char *ep_name, protocomm_req_handler_t handler, void *user_ctx)
Register protocomm handler for a custom endpoint. 
This API can be called by the application to register a protocomm handler for an endpoint after the local control service has started.

Note
In case of BLE transport the names and uuids of all custom endpoints must be provided beforehand as a part of the protocomm_ble_config_t structure set in esp_local_ctrl_config_t, and passed to esp_local_ctrl_start().


Parameters

ep_name -- [in] Name of the endpoint 
handler -- [in] Endpoint handler function 
user_ctx -- [in] User data


Returns

ESP_OK : Success
ESP_FAIL : Failure 





```



### Unions


#### esp_local_ctrl_transport_config_t


```


union esp_local_ctrl_transport_config_t

#include <esp_local_ctrl.h>
Transport mode (BLE / HTTPD) configuration. 

Public Members
ble

esp_local_ctrl_transport_config_ble_t *ble
This is same as protocomm_ble_config_t. See protocomm_ble.h for available configuration parameters. 

httpd

esp_local_ctrl_transport_config_httpd_t *httpd
This is same as httpd_ssl_config_t. See esp_https_server.h for available configuration parameters. 



```



### Structures


#### esp_local_ctrl_prop


```


struct esp_local_ctrl_prop
Property description data structure, which is to be populated and passed to the esp_local_ctrl_add_property() function. 
Once a property is added, its structure is available for read-only access inside get_prop_values() and set_prop_values() handlers. 

Public Members
name

char *name
Unique name of property 

type

uint32_t type
Type of property. This may be set to application defined enums 

size

size_t size
Size of the property value, which:
if zero, the property can have values of variable size
if non-zero, the property can have values of fixed size only, therefore, checks are performed internally by esp_local_ctrl when setting the value of such a property 



flags

uint32_t flags
Flags set for this property. This could be a bit field. A flag may indicate property behavior, e.g. read-only / constant 

ctx

void *ctx
Pointer to some context data relevant for this property. This will be available for use inside the get_prop_values and set_prop_values handlers as a part of this property structure. When set, this is valid throughout the lifetime of a property, till either the property is removed or the esp_local_ctrl service is stopped. 

ctx_free_fn

void (*ctx_free_fn)(void *ctx)
Function used by esp_local_ctrl to internally free the property context when esp_local_ctrl_remove_property() or esp_local_ctrl_stop() is called. 



```

#### esp_local_ctrl_prop_val


```


struct esp_local_ctrl_prop_val
Property value data structure. This gets passed to the get_prop_values() and set_prop_values() handlers for the purpose of retrieving or setting the present value of a property. 

Public Members
data

void *data
Pointer to memory holding property value 

size

size_t size
Size of property value 

free_fn

void (*free_fn)(void *data)
This may be set by the application in get_prop_values() handler to tell esp_local_ctrl to call this function on the data pointer above, for freeing its resources after sending the get_prop_values response. 



```

#### esp_local_ctrl_handlers


```


struct esp_local_ctrl_handlers
Handlers for receiving and responding to local control commands for getting and setting properties. 

Public Members
get_prop_values

esp_err_t (*get_prop_values)(size_t props_count, const esp_local_ctrl_prop_t props[], esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx)
Handler function to be implemented for retrieving current values of properties. 

Note
If any of the properties have fixed sizes, the size field of corresponding element in prop_values need to be set


Param props_count
[in] Total elements in the props array 

Param props
[in] Array of properties, the current values for which have been requested by the client 

Param prop_values
[out] Array of empty property values, the elements of which need to be populated with the current values of those properties specified by props argument 

Param usr_ctx
[in] This provides value of the usr_ctx field of esp_local_ctrl_handlers_t structure

Return
Returning different error codes will convey the corresponding protocol level errors to the client :
ESP_OK : Success
ESP_ERR_INVALID_ARG : InvalidArgument
ESP_ERR_INVALID_STATE : InvalidProto
All other error codes : InternalError 





set_prop_values

esp_err_t (*set_prop_values)(size_t props_count, const esp_local_ctrl_prop_t props[], const esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx)
Handler function to be implemented for changing values of properties. 

Note
If any of the properties have variable sizes, the size field of the corresponding element in prop_values must be checked explicitly before making any assumptions on the size.


Param props_count
[in] Total elements in the props array 

Param props
[in] Array of properties, the values for which the client requests to change 

Param prop_values
[in] Array of property values, the elements of which need to be used for updating those properties specified by props argument 

Param usr_ctx
[in] This provides value of the usr_ctx field of esp_local_ctrl_handlers_t structure

Return
Returning different error codes will convey the corresponding protocol level errors to the client :
ESP_OK : Success
ESP_ERR_INVALID_ARG : InvalidArgument
ESP_ERR_INVALID_STATE : InvalidProto
All other error codes : InternalError 





usr_ctx

void *usr_ctx
Context pointer to be passed to above handler functions upon invocation. This is different from the property level context, as this is valid throughout the lifetime of the esp_local_ctrl service, and freed only when the service is stopped. 

usr_ctx_free_fn

void (*usr_ctx_free_fn)(void *usr_ctx)
Pointer to function which will be internally invoked on usr_ctx for freeing the context resources when esp_local_ctrl_stop() is called. 



```

#### esp_local_ctrl_proto_sec_cfg


```


struct esp_local_ctrl_proto_sec_cfg
Protocom security configs 

Public Members
version

esp_local_ctrl_proto_sec_t version
This sets protocom security version, sec0/sec1 or custom If custom, user must provide handle via proto_sec_custom_handle below 

custom_handle

void *custom_handle
Custom security handle if security is set custom via proto_sec above This handle must follow protocomm_security_t signature 

pop

const void *pop
Proof of possession to be used for local control. Could be NULL. 

sec_params

const void *sec_params
Pointer to security params (NULL if not needed). This is not needed for protocomm security 0 This pointer should hold the struct of type esp_local_ctrl_security1_params_t for protocomm security 1 and esp_local_ctrl_security2_params_t for protocomm security 2 respectively. Could be NULL. 



```

#### esp_local_ctrl_config


```


struct esp_local_ctrl_config
Configuration structure to pass to esp_local_ctrl_start()

Public Members
transport

const esp_local_ctrl_transport_t *transport
Transport layer over which service will be provided 

transport_config

esp_local_ctrl_transport_config_t transport_config
Transport layer over which service will be provided 

proto_sec

esp_local_ctrl_proto_sec_cfg_t proto_sec
Security version and POP 

handlers

esp_local_ctrl_handlers_t handlers
Register handlers for responding to get/set requests on properties 

max_properties

size_t max_properties
This limits the number of properties that are available at a time 



```



### Macros


#### ESP_LOCAL_CTRL_TRANSPORT_BLE


```


ESP_LOCAL_CTRL_TRANSPORT_BLE

```

#### ESP_LOCAL_CTRL_TRANSPORT_HTTPD


```


ESP_LOCAL_CTRL_TRANSPORT_HTTPD

```



### Type Definitions


#### esp_local_ctrl_prop_t


```


typedef struct esp_local_ctrl_prop esp_local_ctrl_prop_t
Property description data structure, which is to be populated and passed to the esp_local_ctrl_add_property() function. 
Once a property is added, its structure is available for read-only access inside get_prop_values() and set_prop_values() handlers. 

```

#### esp_local_ctrl_prop_val_t


```


typedef struct esp_local_ctrl_prop_val esp_local_ctrl_prop_val_t
Property value data structure. This gets passed to the get_prop_values() and set_prop_values() handlers for the purpose of retrieving or setting the present value of a property. 

```

#### esp_local_ctrl_handlers_t


```


typedef struct esp_local_ctrl_handlers esp_local_ctrl_handlers_t
Handlers for receiving and responding to local control commands for getting and setting properties. 

```

#### esp_local_ctrl_transport_t


```


typedef struct esp_local_ctrl_transport esp_local_ctrl_transport_t
Transport mode (BLE / HTTPD) over which the service will be provided. 
This is forward declaration of a private structure, implemented internally by esp_local_ctrl. 

```

#### esp_local_ctrl_transport_config_ble_t


```


typedef struct protocomm_ble_config esp_local_ctrl_transport_config_ble_t
Configuration for transport mode BLE. 
This is a forward declaration for protocomm_ble_config_t. To use this, application must set CONFIG_BT_ENABLED and include protocomm_ble.h. 

```

#### esp_local_ctrl_transport_config_httpd_t


```


typedef struct httpd_config esp_local_ctrl_transport_config_httpd_t
Configuration for transport mode HTTPD. 
This is a forward declaration for httpd_ssl_config_t (for HTTPS) or httpd_config_t (for HTTP) 

```

#### esp_local_ctrl_proto_sec_t


```


typedef enum esp_local_ctrl_proto_sec esp_local_ctrl_proto_sec_t
Security types for esp_local_control. 

```

#### esp_local_ctrl_security1_params_t


```


typedef protocomm_security1_params_t esp_local_ctrl_security1_params_t

```

#### esp_local_ctrl_security2_params_t


```


typedef protocomm_security2_params_t esp_local_ctrl_security2_params_t

```

#### esp_local_ctrl_proto_sec_cfg_t


```


typedef struct esp_local_ctrl_proto_sec_cfg esp_local_ctrl_proto_sec_cfg_t
Protocom security configs 

```

#### esp_local_ctrl_config_t


```


typedef struct esp_local_ctrl_config esp_local_ctrl_config_t
Configuration structure to pass to esp_local_ctrl_start()

```



### Enumerations


#### esp_local_ctrl_proto_sec


```


enum esp_local_ctrl_proto_sec
Security types for esp_local_control. 
Values:
PROTOCOM_SEC0

enumerator PROTOCOM_SEC0

PROTOCOM_SEC1

enumerator PROTOCOM_SEC1

PROTOCOM_SEC2

enumerator PROTOCOM_SEC2

PROTOCOM_SEC_CUSTOM

enumerator PROTOCOM_SEC_CUSTOM


```