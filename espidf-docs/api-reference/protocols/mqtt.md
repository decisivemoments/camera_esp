# ESP-MQTT




## Overview


ESP-MQTT is an implementation of MQTT protocol client, which is a lightweight publish/subscribe messaging protocol. Now ESP-MQTT supports MQTT v5.0.




## Features



> * Support MQTT over TCP, SSL with Mbed TLS, MQTT over WebSocket, and MQTT over WebSocket Secure
> * Easy to setup with URI
> * Multiple instances (multiple clients in one application)
> * Support subscribing, publishing, authentication, last will messages, keep alive pings, and all 3 Quality of Service (QoS) levels (it should be a fully functional client)




## Application Examples



> * protocols/mqtt/tcp: MQTT over TCP, default port 1883
> * protocols/mqtt/ssl: MQTT over TLS, default port 8883
> * protocols/mqtt/ssl_ds: MQTT over TLS using digital signature peripheral for authentication, default port 8883
> * protocols/mqtt/ssl_mutual_auth: MQTT over TLS using certificates for authentication, default port 8883
> * protocols/mqtt/ssl_psk: MQTT over TLS using pre-shared keys for authentication, default port 8883
> * protocols/mqtt/ws: MQTT over WebSocket, default port 80
> * protocols/mqtt/wss: MQTT over WebSocket Secure, default port 443
> * protocols/mqtt5: Uses ESP-MQTT library to connect to broker with MQTT v5.0




## MQTT Message Retransmission


A new MQTT message is created by calling `esp_mqtt_client_publish` or its non blocking counterpart `esp_mqtt_client_enqueue`.


Messages with QoS 0 is sent only once. QoS 1 and 2 have different behaviors since the protocol requires extra steps to complete the process.


The ESP-MQTT library opts to always retransmit unacknowledged QoS 1 and 2 publish messages to avoid losses in faulty connections, even though the MQTT specification requires the re-transmission only on reconnect with Clean Session flag been set to 0 (set `disable_clean_session` to true for this behavior).


QoS 1 and 2 messages that may need retransmission are always enqueued, but first transmission try occurs immediately if `esp_mqtt_client_publish` is used. A transmission retry for unacknowledged messages will occur after `message_retransmit_timeout`. After CONFIG_MQTT_OUTBOX_EXPIRED_TIMEOUT_MS messages will expire and be deleted. If CONFIG_MQTT_REPORT_DELETED_MESSAGES is set, an event will be sent to notify the user.




## Configuration


The configuration is made by setting fields in `esp_mqtt_client_config_t` struct. The configuration struct has the following sub structs to configure different aspects of the client operation.



> * `esp_mqtt_client_config_t::broker_t` - Allow to set address and security verification.
> * `esp_mqtt_client_config_t::credentials_t` - Client credentials for authentication.
> * `esp_mqtt_client_config_t::session_t` - Configuration for MQTT session aspects.
> * `esp_mqtt_client_config_t::network_t` - Networking related configuration.
> * `esp_mqtt_client_config_t::task_t` - Allow to configure FreeRTOS task.
> * `esp_mqtt_client_config_t::buffer_t` - Buffer size for input and output.


In the following sections, the most common aspects are detailed.



### Broker



#### Address


Broker address can be set by usage of `address` struct. The configuration can be made by usage of `uri` field or the combination of `hostname`, `transport` and `port`. Optionally, `path` could be set, this field is useful in WebSocket connections.


The `uri` field is used in the format `scheme://hostname:port/path`.


* Currently support `mqtt`, `mqtts`, `ws`, `wss` schemes
* MQTT over TCP samples:


	+ `mqtt://mqtt.eclipseprojects.io`: MQTT over TCP, default port 1883
	+ `mqtt://mqtt.eclipseprojects.io:1884`: MQTT over TCP, port 1884
	+ `mqtt://username:password@mqtt.eclipseprojects.io:1884`: MQTT over TCP,
	port 1884, with username and password
* MQTT over SSL samples:


	+ `mqtts://mqtt.eclipseprojects.io`: MQTT over SSL, port 8883
	+ `mqtts://mqtt.eclipseprojects.io:8884`: MQTT over SSL, port 8884
* MQTT over WebSocket samples:


	+ `ws://mqtt.eclipseprojects.io:80/mqtt`
* MQTT over WebSocket Secure samples:


	+ `wss://mqtt.eclipseprojects.io:443/mqtt`
* Minimal configurations:



```
const esp_mqtt_client_config_t mqtt_cfg = {
    .broker.address.uri = "mqtt://mqtt.eclipseprojects.io",
};
esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
esp_mqtt_client_start(client);

```



Note


By default MQTT client uses event loop library to post related MQTT events (connected, subscribed, published, etc.).





#### Verification


For secure connections with TLS used, and to guarantee Broker's identity, the `verification` struct must be set.
The broker certificate may be set in PEM or DER format. To select DER, the equivalent `certificate_len` field must be set. Otherwise, a null-terminated string in PEM format should be provided to `certificate` field.


* Get certificate from server, example: `mqtt.eclipseprojects.io`
```
openssl s_client -showcerts -connect mqtt.eclipseprojects.io:8883 < /dev/null \
2> /dev/null | openssl x509 -outform PEM > mqtt_eclipse_org.pem

```
* Check the sample application: protocols/mqtt/ssl
* Configuration:



```
const esp_mqtt_client_config_t mqtt_cfg = {
    .broker = {
      .address.uri = "mqtts://mqtt.eclipseprojects.io:8883",
      .verification.certificate = (const char *)mqtt_eclipse_org_pem_start,
    },
};

```


For details about other fields, please check the API Reference and TLS Server Verification.





### Client Credentials


All client related credentials are under the `credentials` field.



> * `username`: pointer to the username used for connecting to the broker, can also be set by URI
> * `client_id`: pointer to the client ID, defaults to `ESP32_%CHIPID%` where `%CHIPID%` are the last 3 bytes of MAC address in hex format



#### Authentication


It is possible to set authentication parameters through the `authentication` field. The client supports the following authentication methods:



> * `password`: use a password by setting
> * `certificate` and `key`: mutual authentication with TLS, and both can be provided in PEM or DER format
> * `use_secure_element`: use secure element (ATECC608A) interfaced to ESP32
> * `ds_data`: use Digital Signature Peripheral available in some Espressif devices





### Session


For MQTT session-related configurations, `session` fields should be used.



#### Last Will and Testament


MQTT allows for a last will and testament (LWT) message to notify other clients when a client ungracefully disconnects. This is configured by the following fields in the `last_will` struct.



> * `topic`: pointer to the LWT message topic
> * `msg`: pointer to the LWT message
> * `msg_len`: length of the LWT message, required if `msg` is not null-terminated
> * `qos`: quality of service for the LWT message
> * `retain`: specifies the retain flag of the LWT message





### Change Settings in Project Configuration Menu


The settings for MQTT can be found using `idf.py menuconfig`, under `Component config` > `ESP-MQTT Configuration`.


The following settings are available:


* CONFIG_MQTT_PROTOCOL_311: enable 3.1.1 version of MQTT protocol
* CONFIG_MQTT_TRANSPORT_SSL and CONFIG_MQTT_TRANSPORT_WEBSOCKET: enable specific MQTT transport layer, such as SSL, WEBSOCKET, and WEBSOCKET_SECURE
* CONFIG_MQTT_CUSTOM_OUTBOX: disable default implementation of mqtt_outbox, so a specific implementation can be supplied





## Events


The following events may be posted by the MQTT client:


* `MQTT_EVENT_BEFORE_CONNECT`: The client is initialized and about to start connecting to the broker.
* `MQTT_EVENT_CONNECTED`: The client has successfully established a connection to the broker. The client is now ready to send and receive data.
* `MQTT_EVENT_DISCONNECTED`: The client has aborted the connection due to being unable to read or write data, e.g., because the server is unavailable.
* `MQTT_EVENT_SUBSCRIBED`: The broker has acknowledged the client's subscribe request. The event data contains the message ID of the subscribe message.
* `MQTT_EVENT_UNSUBSCRIBED`: The broker has acknowledged the client's unsubscribe request. The event data contains the message ID of the unsubscribe message.
* `MQTT_EVENT_PUBLISHED`: The broker has acknowledged the client's publish message. This is only posted for QoS level 1 and 2, as level 0 does not use acknowledgements. The event data contains the message ID of the publish message.
* `MQTT_EVENT_DATA`: The client has received a publish message. The event data contains: message ID, name of the topic it was published to, received data and its length. For data that exceeds the internal buffer, multiple `MQTT_EVENT_DATA` events are posted and `current_data_offset` and `total_data_len` from event data updated to keep track of the fragmented message.
* `MQTT_EVENT_ERROR`: The client has encountered an error. The field `error_handle` in the event data contains `error_type` that can be used to identify the error. The type of error determines which parts of the `error_handle` struct is filled.




## API Reference



### Header File


* components/mqtt/esp-mqtt/include/mqtt_client.h
* This header file can be included with:



> ```
> #include "mqtt_client.h"
> 
> ```
* This header file is a part of the API provided by the `mqtt` component. To declare that your component depends on `mqtt`, add the following to your CMakeLists.txt:



> ```
> REQUIRES mqtt
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES mqtt
> 
> ```




### Functions


#### esp_mqtt_client_init


```


esp_mqtt_client_handle_t esp_mqtt_client_init(const esp_mqtt_client_config_t *config)
Creates MQTT client handle based on the configuration. 

Parameters
config -- MQTT configuration structure

Returns
mqtt_client_handle if successfully created, NULL on error 



```

#### esp_mqtt_client_set_uri


```


esp_err_t esp_mqtt_client_set_uri(esp_mqtt_client_handle_t client, const char *uri)
Sets MQTT connection URI. This API is usually used to overrides the URI configured in esp_mqtt_client_init. 

Parameters

client -- MQTT client handle 
uri -- 


Returns
ESP_FAIL if URI parse error, ESP_OK on success 



```

#### esp_mqtt_client_start


```


esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t client)
Starts MQTT client with already created client handle. 

Parameters
client -- MQTT client handle

Returns
ESP_OK on success ESP_ERR_INVALID_ARG on wrong initialization ESP_FAIL on other error 



```

#### esp_mqtt_client_reconnect


```


esp_err_t esp_mqtt_client_reconnect(esp_mqtt_client_handle_t client)
This api is typically used to force reconnection upon a specific event. 

Parameters
client -- MQTT client handle

Returns
ESP_OK on success ESP_ERR_INVALID_ARG on wrong initialization ESP_FAIL if client is in invalid state 



```

#### esp_mqtt_client_disconnect


```


esp_err_t esp_mqtt_client_disconnect(esp_mqtt_client_handle_t client)
This api is typically used to force disconnection from the broker. 

Parameters
client -- MQTT client handle

Returns
ESP_OK on success ESP_ERR_INVALID_ARG on wrong initialization 



```

#### esp_mqtt_client_stop


```


esp_err_t esp_mqtt_client_stop(esp_mqtt_client_handle_t client)
Stops MQTT client tasks. 

Notes:
Cannot be called from the MQTT event handler



Parameters
client -- MQTT client handle

Returns
ESP_OK on success ESP_ERR_INVALID_ARG on wrong initialization ESP_FAIL if client is in invalid state 



```

#### esp_mqtt_client_subscribe_single


```


int esp_mqtt_client_subscribe_single(esp_mqtt_client_handle_t client, const char *topic, int qos)
Subscribe the client to defined topic with defined qos. 
Notes:
Client must be connected to send subscribe message
This API is could be executed from a user task or from a MQTT event callback i.e. internal MQTT task (API is protected by internal mutex, so it might block if a longer data receive operation is in progress.
esp_mqtt_client_subscribe could be used to call this function.



Parameters

client -- MQTT client handle 
topic -- topic filter to subscribe 
qos -- Max qos level of the subscription


Returns
message_id of the subscribe message on success -1 on failure -2 in case of full outbox. 



```

#### esp_mqtt_client_subscribe_multiple


```


int esp_mqtt_client_subscribe_multiple(esp_mqtt_client_handle_t client, const esp_mqtt_topic_t *topic_list, int size)
Subscribe the client to a list of defined topics with defined qos. 
Notes:
Client must be connected to send subscribe message
This API is could be executed from a user task or from a MQTT event callback i.e. internal MQTT task (API is protected by internal mutex, so it might block if a longer data receive operation is in progress.
esp_mqtt_client_subscribe could be used to call this function.



Parameters

client -- MQTT client handle 
topic_list -- List of topics to subscribe 
size -- size of topic_list


Returns
message_id of the subscribe message on success -1 on failure -2 in case of full outbox. 



```

#### esp_mqtt_client_unsubscribe


```


int esp_mqtt_client_unsubscribe(esp_mqtt_client_handle_t client, const char *topic)
Unsubscribe the client from defined topic. 
Notes:
Client must be connected to send unsubscribe message
It is thread safe, please refer to esp_mqtt_client_subscribe_single for details



Parameters

client -- MQTT client handle 
topic -- 


Returns
message_id of the subscribe message on success -1 on failure 



```

#### esp_mqtt_client_publish


```


int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain)
Client to send a publish message to the broker. 
Notes:
This API might block for several seconds, either due to network timeout (10s) or if publishing payloads longer than internal buffer (due to message fragmentation)
Client doesn't have to be connected for this API to work, enqueueing the messages with qos>1 (returning -1 for all the qos=0 messages if disconnected). If MQTT_SKIP_PUBLISH_IF_DISCONNECTED is enabled, this API will not attempt to publish when the client is not connected and will always return -1.
It is thread safe, please refer to esp_mqtt_client_subscribe for details



Parameters

client -- MQTT client handle 
topic -- topic string 
data -- payload string (set to NULL, sending empty payload message) 
len -- data length, if set to 0, length is calculated from payload string 
qos -- QoS of publish message 
retain -- retain flag


Returns
message_id of the publish message (for QoS 0 message_id will always be zero) on success. -1 on failure, -2 in case of full outbox. 



```

#### esp_mqtt_client_enqueue


```


int esp_mqtt_client_enqueue(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain, bool store)
Enqueue a message to the outbox, to be sent later. Typically used for messages with qos>0, but could be also used for qos=0 messages if store=true. 
This API generates and stores the publish message into the internal outbox and the actual sending to the network is performed in the mqtt-task context (in contrast to the esp_mqtt_client_publish() which sends the publish message immediately in the user task's context). Thus, it could be used as a non blocking version of esp_mqtt_client_publish().

Parameters

client -- MQTT client handle 
topic -- topic string 
data -- payload string (set to NULL, sending empty payload message) 
len -- data length, if set to 0, length is calculated from payload string 
qos -- QoS of publish message 
retain -- retain flag 
store -- if true, all messages are enqueued; otherwise only QoS 1 and QoS 2 are enqueued


Returns
message_id if queued successfully, -1 on failure, -2 in case of full outbox. 



```

#### esp_mqtt_client_destroy


```


esp_err_t esp_mqtt_client_destroy(esp_mqtt_client_handle_t client)
Destroys the client handle. 
Notes:
Cannot be called from the MQTT event handler



Parameters
client -- MQTT client handle

Returns
ESP_OK ESP_ERR_INVALID_ARG on wrong initialization 



```

#### esp_mqtt_set_config


```


esp_err_t esp_mqtt_set_config(esp_mqtt_client_handle_t client, const esp_mqtt_client_config_t *config)
Set configuration structure, typically used when updating the config (i.e. on "before_connect" event. 
Notes:
When calling this function make sure to have all the intendend configurations set, otherwise default values are set. 



Parameters

client -- MQTT client handle
config -- MQTT configuration structure


Returns
ESP_ERR_NO_MEM if failed to allocate ESP_ERR_INVALID_ARG if conflicts on transport configuration. ESP_OK on success 



```

#### esp_mqtt_client_register_event


```


esp_err_t esp_mqtt_client_register_event(esp_mqtt_client_handle_t client, esp_mqtt_event_id_t event, esp_event_handler_t event_handler, void *event_handler_arg)
Registers MQTT event. 

Parameters

client -- MQTT client handle 
event -- event type 
event_handler -- handler callback 
event_handler_arg -- handlers context


Returns
ESP_ERR_NO_MEM if failed to allocate ESP_ERR_INVALID_ARG on wrong initialization ESP_OK on success 



```

#### esp_mqtt_client_unregister_event


```


esp_err_t esp_mqtt_client_unregister_event(esp_mqtt_client_handle_t client, esp_mqtt_event_id_t event, esp_event_handler_t event_handler)
Unregisters mqtt event. 

Parameters

client -- mqtt client handle 
event -- event ID 
event_handler -- handler to unregister


Returns
ESP_ERR_NO_MEM if failed to allocate ESP_ERR_INVALID_ARG on invalid event ID ESP_OK on success 



```

#### esp_mqtt_client_get_outbox_size


```


int esp_mqtt_client_get_outbox_size(esp_mqtt_client_handle_t client)
Get outbox size. 

Parameters
client -- MQTT client handle 

Returns
outbox size 0 on wrong initialization 



```

#### esp_mqtt_dispatch_custom_event


```


esp_err_t esp_mqtt_dispatch_custom_event(esp_mqtt_client_handle_t client, esp_mqtt_event_t *event)
Dispatch user event to the mqtt internal event loop. 

Parameters

client -- MQTT client handle 
event -- MQTT event handle structure 


Returns
ESP_OK on success ESP_ERR_TIMEOUT if the event couldn't be queued (ref also CONFIG_MQTT_EVENT_QUEUE_SIZE) 



```



### Structures


#### esp_mqtt_error_codes


```


struct esp_mqtt_error_codes
MQTT error code structure to be passed as a contextual information into ERROR event 
Important: This structure extends esp_tls_last_error error structure and is backward compatible with it (so might be down-casted and treated as esp_tls_last_error error, but recommended to update applications if used this way previously)
Use this structure directly checking error_type first and then appropriate error code depending on the source of the error:
| error_type | related member variables | note | | MQTT_ERROR_TYPE_TCP_TRANSPORT | esp_tls_last_esp_err, esp_tls_stack_err, esp_tls_cert_verify_flags, sock_errno | Error reported from tcp_transport/esp-tls | | MQTT_ERROR_TYPE_CONNECTION_REFUSED | connect_return_code | Internal error reported from MQTT broker on connection | 

Public Members
esp_tls_last_esp_err

esp_err_t esp_tls_last_esp_err
last esp_err code reported from esp-tls component 

esp_tls_stack_err

int esp_tls_stack_err
tls specific error code reported from underlying tls stack 

esp_tls_cert_verify_flags

int esp_tls_cert_verify_flags
tls flags reported from underlying tls stack during certificate verification 

error_type

esp_mqtt_error_type_t error_type
error type referring to the source of the error 

connect_return_code

esp_mqtt_connect_return_code_t connect_return_code
connection refused error code reported from MQTT* broker on connection 

esp_transport_sock_errno

int esp_transport_sock_errno
errno from the underlying socket 



```

#### esp_mqtt_event_t


```


struct esp_mqtt_event_t
MQTT event configuration structure 

Public Members
event_id

esp_mqtt_event_id_t event_id
MQTT event type 

client

esp_mqtt_client_handle_t client
MQTT client handle for this event 

data

char *data
Data associated with this event 

data_len

int data_len
Length of the data for this event 

total_data_len

int total_data_len
Total length of the data (longer data are supplied with multiple events) 

current_data_offset

int current_data_offset
Actual offset for the data associated with this event 

topic

char *topic
Topic associated with this event 

topic_len

int topic_len
Length of the topic for this event associated with this event 

msg_id

int msg_id
MQTT messaged id of message 

session_present

int session_present
MQTT session_present flag for connection event 

error_handle

esp_mqtt_error_codes_t *error_handle
esp-mqtt error handle including esp-tls errors as well as internal MQTT errors 

retain

bool retain
Retained flag of the message associated with this event 

qos

int qos
QoS of the messages associated with this event 

dup

bool dup
dup flag of the message associated with this event 

protocol_ver

esp_mqtt_protocol_ver_t protocol_ver
MQTT protocol version used for connection, defaults to value from menuconfig 



```

#### esp_mqtt_client_config_t


```


struct esp_mqtt_client_config_t
MQTT client configuration structure

Default values can be set via menuconfig
All certificates and key data could be passed in PEM or DER format. PEM format must have a terminating NULL character and the related len field set to 0. DER format requires a related len field set to the correct length. 



Public Members
broker

struct esp_mqtt_client_config_t::broker_t broker
Broker address and security verification 

credentials

struct esp_mqtt_client_config_t::credentials_t credentials
User credentials for broker 

session

struct esp_mqtt_client_config_t::session_t session
MQTT session configuration. 

network

struct esp_mqtt_client_config_t::network_t network
Network configuration 

task

struct esp_mqtt_client_config_t::task_t task
FreeRTOS task configuration. 

buffer

struct esp_mqtt_client_config_t::buffer_t buffer
Buffer size configuration. 

outbox

struct esp_mqtt_client_config_t::outbox_config_t outbox
Outbox configuration. 


broker_t

struct broker_t
Broker related configuration 

Public Members
address

struct esp_mqtt_client_config_t::broker_t::address_t address
Broker address configuration 

verification

struct esp_mqtt_client_config_t::broker_t::verification_t verification
Security verification of the broker 


address_t

struct address_t
Broker address

uri have precedence over other fields
If uri isn't set at least hostname, transport and port should. 



Public Members
uri

const char *uri
Complete MQTT broker URI 

hostname

const char *hostname
Hostname, to set ipv4 pass it as string) 

transport

esp_mqtt_transport_t transport
Selects transport 

path

const char *path
Path in the URI 

port

uint32_t port
MQTT server port 



verification_t

struct verification_t
Broker identity verification
If fields are not set broker's identity isn't verified. it's recommended to set the options in this struct for security reasons. 

Public Members
use_global_ca_store

bool use_global_ca_store
Use a global ca_store, look esp-tls documentation for details. 

crt_bundle_attach

esp_err_t (*crt_bundle_attach)(void *conf)
Pointer to ESP x509 Certificate Bundle attach function for the usage of certificate bundles. Client only attach the bundle, the clean up must be done by the user. 

certificate

const char *certificate
Certificate data, default is NULL. It's not copied nor freed by the client, user needs to clean up. 

certificate_len

size_t certificate_len
Length of the buffer pointed to by certificate. 

psk_hint_key

const struct psk_key_hint *psk_hint_key
Pointer to PSK struct defined in esp_tls.h to enable PSK authentication (as alternative to certificate verification). PSK is enabled only if there are no other ways to verify broker. It's not copied nor freed by the client, user needs to clean up. 

skip_cert_common_name_check

bool skip_cert_common_name_check
Skip any validation of server certificate CN field, this reduces the security of TLS and makes the MQTT client susceptible to MITM attacks 

alpn_protos

const char **alpn_protos
NULL-terminated list of supported application protocols to be used for ALPN. 

common_name

const char *common_name
Pointer to the string containing server certificate common name. If non-NULL, server certificate CN must match this name, If NULL, server certificate CN must match hostname. This is ignored if skip_cert_common_name_check=true. It's not copied nor freed by the client, user needs to clean up. 




buffer_t

struct buffer_t
Client buffer size configuration
Client have two buffers for input and output respectivelly. 

Public Members
size

int size
size of MQTT send/receive buffer 

out_size

int out_size
size of MQTT output buffer. If not defined, defaults to the size defined by buffer_size



credentials_t

struct credentials_t
Client related credentials for authentication. 

Public Members
username

const char *username
MQTT username 

client_id

const char *client_id
Set MQTT client identifier. Ignored if set_null_client_id == true If NULL set the default client id. Default client id is ESP32_CHIPID% where CHIPID% are last 3 bytes of MAC address in hex format 

set_null_client_id

bool set_null_client_id
Selects a NULL client id 

authentication

struct esp_mqtt_client_config_t::credentials_t::authentication_t authentication
Client authentication 


authentication_t

struct authentication_t
Client authentication
Fields related to client authentication by broker
For mutual authentication using TLS, user could select certificate and key, secure element or digital signature peripheral if available. 

Public Members
password

const char *password
MQTT password 

certificate

const char *certificate
Certificate for ssl mutual authentication, not required if mutual authentication is not needed. Must be provided with key. It's not copied nor freed by the client, user needs to clean up. 

certificate_len

size_t certificate_len
Length of the buffer pointed to by certificate. 

key

const char *key
Private key for SSL mutual authentication, not required if mutual authentication is not needed. If it is not NULL, also certificate has to be provided. It's not copied nor freed by the client, user needs to clean up. 

key_len

size_t key_len
Length of the buffer pointed to by key. 

key_password

const char *key_password
Client key decryption password, not PEM nor DER, if provided key_password_len must be correctly set. 

key_password_len

int key_password_len
Length of the password pointed to by key_password

use_secure_element

bool use_secure_element
Enable secure element, available in ESP32-ROOM-32SE, for SSL connection 

ds_data

void *ds_data
Carrier of handle for digital signature parameters, digital signature peripheral is available in some Espressif devices. It's not copied nor freed by the client, user needs to clean up. 




network_t

struct network_t
Network related configuration 

Public Members
reconnect_timeout_ms

int reconnect_timeout_ms
Reconnect to the broker after this value in miliseconds if auto reconnect is not disabled (defaults to 10s) 

timeout_ms

int timeout_ms
Abort network operation if it is not completed after this value, in milliseconds (defaults to 10s). 

refresh_connection_after_ms

int refresh_connection_after_ms
Refresh connection after this value (in milliseconds) 

disable_auto_reconnect

bool disable_auto_reconnect
Client will reconnect to server (when errors/disconnect). Set disable_auto_reconnect=true to disable 

transport

esp_transport_handle_t transport
Custom transport handle to use. Warning: The transport should be valid during the client lifetime and is destroyed when esp_mqtt_client_destroy is called. 

if_name

struct ifreq *if_name
The name of interface for data to go through. Use the default interface without setting 



outbox_config_t

struct outbox_config_t
Client outbox configuration options. 

Public Members
limit

uint64_t limit
Size limit for the outbox in bytes. 



session_t

struct session_t
MQTT Session related configuration 

Public Members
last_will

struct esp_mqtt_client_config_t::session_t::last_will_t last_will
Last will configuration 

disable_clean_session

bool disable_clean_session
MQTT clean session, default clean_session is true 

keepalive

int keepalive
MQTT keepalive, default is 120 seconds When configuring this value, keep in mind that the client attempts to communicate with the broker at half the interval that is actually set. This conservative approach allows for more attempts before the broker's timeout occurs 

disable_keepalive

bool disable_keepalive
Set disable_keepalive=true to turn off keep-alive mechanism, keepalive is active by default. Note: setting the config value keepalive to 0 doesn't disable keepalive feature, but uses a default keepalive period 

protocol_ver

esp_mqtt_protocol_ver_t protocol_ver
MQTT protocol version used for connection. 

message_retransmit_timeout

int message_retransmit_timeout
timeout for retransmitting of failed packet 


last_will_t

struct last_will_t
Last Will and Testament message configuration. 

Public Members
topic

const char *topic
LWT (Last Will and Testament) message topic 

msg

const char *msg
LWT message, may be NULL terminated 

msg_len

int msg_len
LWT message length, if msg isn't NULL terminated must have the correct length 

qos

int qos
LWT message QoS 

retain

int retain
LWT retained message flag 




task_t

struct task_t
Client task configuration 

Public Members
priority

int priority
MQTT task priority 

stack_size

int stack_size
MQTT task stack size 




```

#### topic_t


```


struct topic_t
Topic definition struct 

Public Members
filter

const char *filter
Topic filter to subscribe 

qos

int qos
Max QoS level of the subscription 



```



### Macros


#### MQTT_ERROR_TYPE_ESP_TLS


```


MQTT_ERROR_TYPE_ESP_TLS
MQTT_ERROR_TYPE_TCP_TRANSPORT error type hold all sorts of transport layer errors, including ESP-TLS error, but in the past only the errors from MQTT_ERROR_TYPE_ESP_TLS layer were reported, so the ESP-TLS error type is re-defined here for backward compatibility 

```

#### esp_mqtt_client_subscribe


```


esp_mqtt_client_subscribe(client_handle, topic_type, qos_or_size)
Convenience macro to select subscribe function to use. 
Notes:
Usage of esp_mqtt_client_subscribe_single is the same as previous esp_mqtt_client_subscribe, refer to it for details.



Parameters

client_handle -- MQTT client handle 
topic_type -- Needs to be char* for single subscription or esp_mqtt_topic_t for multiple topics 
qos_or_size -- It's either a qos when subscribing to a single topic or the size of the subscription array when subscribing to multiple topics.


Returns
message_id of the subscribe message on success -1 on failure -2 in case of full outbox. 



```



### Type Definitions


#### esp_mqtt_client_handle_t


```


typedef struct esp_mqtt_client *esp_mqtt_client_handle_t

```

#### esp_mqtt_event_id_t


```


typedef enum esp_mqtt_event_id_t esp_mqtt_event_id_t
MQTT event types. 
User event handler receives context data in esp_mqtt_event_t structure with
client - MQTT client handle
various other data depending on event type 



```

#### esp_mqtt_connect_return_code_t


```


typedef enum esp_mqtt_connect_return_code_t esp_mqtt_connect_return_code_t
MQTT connection error codes propagated via ERROR event 

```

#### esp_mqtt_error_type_t


```


typedef enum esp_mqtt_error_type_t esp_mqtt_error_type_t
MQTT connection error codes propagated via ERROR event 

```

#### esp_mqtt_transport_t


```


typedef enum esp_mqtt_transport_t esp_mqtt_transport_t

```

#### esp_mqtt_protocol_ver_t


```


typedef enum esp_mqtt_protocol_ver_t esp_mqtt_protocol_ver_t
MQTT protocol version used for connection 

```

#### esp_mqtt_error_codes_t


```


typedef struct esp_mqtt_error_codes esp_mqtt_error_codes_t
MQTT error code structure to be passed as a contextual information into ERROR event 
Important: This structure extends esp_tls_last_error error structure and is backward compatible with it (so might be down-casted and treated as esp_tls_last_error error, but recommended to update applications if used this way previously)
Use this structure directly checking error_type first and then appropriate error code depending on the source of the error:
| error_type | related member variables | note | | MQTT_ERROR_TYPE_TCP_TRANSPORT | esp_tls_last_esp_err, esp_tls_stack_err, esp_tls_cert_verify_flags, sock_errno | Error reported from tcp_transport/esp-tls | | MQTT_ERROR_TYPE_CONNECTION_REFUSED | connect_return_code | Internal error reported from MQTT broker on connection | 

```

#### esp_mqtt_event_t


```


typedef struct esp_mqtt_event_t esp_mqtt_event_t
MQTT event configuration structure 

```

#### esp_mqtt_event_handle_t


```


typedef esp_mqtt_event_t *esp_mqtt_event_handle_t

```

#### esp_mqtt_client_config_t


```


typedef struct esp_mqtt_client_config_t esp_mqtt_client_config_t
MQTT client configuration structure

Default values can be set via menuconfig
All certificates and key data could be passed in PEM or DER format. PEM format must have a terminating NULL character and the related len field set to 0. DER format requires a related len field set to the correct length. 



```

#### esp_mqtt_topic_t


```


typedef struct topic_t esp_mqtt_topic_t
Topic definition struct 

```



### Enumerations


#### esp_mqtt_event_id_t


```


enum esp_mqtt_event_id_t
MQTT event types. 
User event handler receives context data in esp_mqtt_event_t structure with
client - MQTT client handle
various other data depending on event type 


Values:
MQTT_EVENT_ANY

enumerator MQTT_EVENT_ANY

MQTT_EVENT_ERROR

enumerator MQTT_EVENT_ERROR
on error event, additional context: connection return code, error handle from esp_tls (if supported) 

MQTT_EVENT_CONNECTED

enumerator MQTT_EVENT_CONNECTED
connected event, additional context: session_present flag 

MQTT_EVENT_DISCONNECTED

enumerator MQTT_EVENT_DISCONNECTED
disconnected event 

MQTT_EVENT_SUBSCRIBED

enumerator MQTT_EVENT_SUBSCRIBED
subscribed event, additional context:
msg_id message id
error_handle error_type in case subscribing failed
data pointer to broker response, check for errors.
data_len length of the data for this event 



MQTT_EVENT_UNSUBSCRIBED

enumerator MQTT_EVENT_UNSUBSCRIBED
unsubscribed event, additional context: msg_id 

MQTT_EVENT_PUBLISHED

enumerator MQTT_EVENT_PUBLISHED
published event, additional context: msg_id 

MQTT_EVENT_DATA

enumerator MQTT_EVENT_DATA
data event, additional context:
msg_id message id
topic pointer to the received topic
topic_len length of the topic
data pointer to the received data
data_len length of the data for this event
current_data_offset offset of the current data for this event
total_data_len total length of the data received
retain retain flag of the message
qos QoS level of the message
dup dup flag of the message Note: Multiple MQTT_EVENT_DATA could be fired for one message, if it is longer than internal buffer. In that case only first event contains topic pointer and length, other contain data only with current data length and current data offset updating. 



MQTT_EVENT_BEFORE_CONNECT

enumerator MQTT_EVENT_BEFORE_CONNECT
The event occurs before connecting 

MQTT_EVENT_DELETED

enumerator MQTT_EVENT_DELETED
Notification on delete of one message from the internal outbox, if the message couldn't have been sent and acknowledged before expiring defined in OUTBOX_EXPIRED_TIMEOUT_MS. (events are not posted upon deletion of successfully acknowledged messages)
This event id is posted only if MQTT_REPORT_DELETED_MESSAGES==1
Additional context: msg_id (id of the deleted message). 



MQTT_USER_EVENT

enumerator MQTT_USER_EVENT
Custom event used to queue tasks into mqtt event handler All fields from the esp_mqtt_event_t type could be used to pass an additional context data to the handler. 


```

#### esp_mqtt_connect_return_code_t


```


enum esp_mqtt_connect_return_code_t
MQTT connection error codes propagated via ERROR event 
Values:
MQTT_CONNECTION_ACCEPTED

enumerator MQTT_CONNECTION_ACCEPTED
Connection accepted 

MQTT_CONNECTION_REFUSE_PROTOCOL

enumerator MQTT_CONNECTION_REFUSE_PROTOCOL
MQTT connection refused reason: Wrong protocol 

MQTT_CONNECTION_REFUSE_ID_REJECTED

enumerator MQTT_CONNECTION_REFUSE_ID_REJECTED
MQTT connection refused reason: ID rejected 

MQTT_CONNECTION_REFUSE_SERVER_UNAVAILABLE

enumerator MQTT_CONNECTION_REFUSE_SERVER_UNAVAILABLE
MQTT connection refused reason: Server unavailable 

MQTT_CONNECTION_REFUSE_BAD_USERNAME

enumerator MQTT_CONNECTION_REFUSE_BAD_USERNAME
MQTT connection refused reason: Wrong user 

MQTT_CONNECTION_REFUSE_NOT_AUTHORIZED

enumerator MQTT_CONNECTION_REFUSE_NOT_AUTHORIZED
MQTT connection refused reason: Wrong username or password 


```

#### esp_mqtt_error_type_t


```


enum esp_mqtt_error_type_t
MQTT connection error codes propagated via ERROR event 
Values:
MQTT_ERROR_TYPE_NONE

enumerator MQTT_ERROR_TYPE_NONE

MQTT_ERROR_TYPE_TCP_TRANSPORT

enumerator MQTT_ERROR_TYPE_TCP_TRANSPORT

MQTT_ERROR_TYPE_CONNECTION_REFUSED

enumerator MQTT_ERROR_TYPE_CONNECTION_REFUSED

MQTT_ERROR_TYPE_SUBSCRIBE_FAILED

enumerator MQTT_ERROR_TYPE_SUBSCRIBE_FAILED


```

#### esp_mqtt_transport_t


```


enum esp_mqtt_transport_t
Values:
MQTT_TRANSPORT_UNKNOWN

enumerator MQTT_TRANSPORT_UNKNOWN

MQTT_TRANSPORT_OVER_TCP

enumerator MQTT_TRANSPORT_OVER_TCP
MQTT over TCP, using scheme: MQTT

MQTT_TRANSPORT_OVER_SSL

enumerator MQTT_TRANSPORT_OVER_SSL
MQTT over SSL, using scheme: MQTTS

MQTT_TRANSPORT_OVER_WS

enumerator MQTT_TRANSPORT_OVER_WS
MQTT over Websocket, using scheme:: ws

MQTT_TRANSPORT_OVER_WSS

enumerator MQTT_TRANSPORT_OVER_WSS
MQTT over Websocket Secure, using scheme: wss


```

#### esp_mqtt_protocol_ver_t


```


enum esp_mqtt_protocol_ver_t
MQTT protocol version used for connection 
Values:
MQTT_PROTOCOL_UNDEFINED

enumerator MQTT_PROTOCOL_UNDEFINED

MQTT_PROTOCOL_V_3_1

enumerator MQTT_PROTOCOL_V_3_1

MQTT_PROTOCOL_V_3_1_1

enumerator MQTT_PROTOCOL_V_3_1_1

MQTT_PROTOCOL_V_5

enumerator MQTT_PROTOCOL_V_5


```