# BluFi API



## Overview


BluFi is a profile based GATT to config ESP32 Wi-Fi to connect/disconnect AP or setup a softap and etc.


Use should concern these things:


1. The event sent from profile. Then you need to do something as the event indicate.
2. Security reference. You can write your own Security functions such as symmetrical encryption/decryption and checksum functions. Even you can define the "Key Exchange/Negotiation" procedure.




## Application Example


Check bluetooth folder in ESP-IDF examples, which contains the following application:


* This is the BluFi demo. This demo can set ESP32's Wi-Fi to softap/station/softap&station mode and config Wi-Fi connections - bluetooth/blufi




## API Reference



### Header File


* components/bt/common/api/include/api/esp_blufi_api.h
* This header file can be included with:



> ```
> #include "esp_blufi_api.h"
> 
> ```
* This header file is a part of the API provided by the `bt` component. To declare that your component depends on `bt`, add the following to your CMakeLists.txt:



> ```
> REQUIRES bt
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES bt
> 
> ```




### Functions


#### esp_blufi_register_callbacks


```


esp_err_t esp_blufi_register_callbacks(esp_blufi_callbacks_t *callbacks)
This function is called to receive blufi callback event. 

Parameters
callbacks -- [in] callback functions

Returns
ESP_OK - success, other - failed 



```

#### esp_blufi_profile_init


```


esp_err_t esp_blufi_profile_init(void)
This function is called to initialize blufi_profile. 

Returns
ESP_OK - success, other - failed 



```

#### esp_blufi_profile_deinit


```


esp_err_t esp_blufi_profile_deinit(void)
This function is called to de-initialize blufi_profile. 

Returns
ESP_OK - success, other - failed 



```

#### esp_blufi_send_wifi_conn_report


```


esp_err_t esp_blufi_send_wifi_conn_report(wifi_mode_t opmode, esp_blufi_sta_conn_state_t sta_conn_state, uint8_t softap_conn_num, esp_blufi_extra_info_t *extra_info)
This function is called to send wifi connection report. 

Parameters

opmode -- : wifi opmode 
sta_conn_state -- : station is already in connection or not 
softap_conn_num -- : softap connection number 
extra_info -- : extra information, such as sta_ssid, softap_ssid and etc.


Returns
ESP_OK - success, other - failed 



```

#### esp_blufi_send_wifi_list


```


esp_err_t esp_blufi_send_wifi_list(uint16_t apCount, esp_blufi_ap_record_t *list)
This function is called to send wifi list. 

Parameters

apCount -- : wifi list count 
list -- : wifi list


Returns
ESP_OK - success, other - failed 



```

#### esp_blufi_get_version


```


uint16_t esp_blufi_get_version(void)
Get BLUFI profile version. 

Returns
Most 8bit significant is Great version, Least 8bit is Sub version 



```

#### esp_blufi_send_error_info


```


esp_err_t esp_blufi_send_error_info(esp_blufi_error_state_t state)
This function is called to send blufi error information. 

Parameters
state -- : error state

Returns
ESP_OK - success, other - failed 



```

#### esp_blufi_send_custom_data


```


esp_err_t esp_blufi_send_custom_data(uint8_t *data, uint32_t data_len)
This function is called to custom data. 

Parameters

data -- : custom data value 
data_len -- : the length of custom data


Returns
ESP_OK - success, other - failed 



```



### Unions


#### esp_blufi_cb_param_t


```


union esp_blufi_cb_param_t

#include <esp_blufi_api.h>
BLUFI callback parameters union. 

Public Members
init_finish

struct esp_blufi_cb_param_t::blufi_init_finish_evt_param init_finish
Blufi callback param of ESP_BLUFI_EVENT_INIT_FINISH 

deinit_finish

struct esp_blufi_cb_param_t::blufi_deinit_finish_evt_param deinit_finish
Blufi callback param of ESP_BLUFI_EVENT_DEINIT_FINISH 

wifi_mode

struct esp_blufi_cb_param_t::blufi_set_wifi_mode_evt_param wifi_mode
Blufi callback param of ESP_BLUFI_EVENT_INIT_FINISH 

connect

struct esp_blufi_cb_param_t::blufi_connect_evt_param connect
Blufi callback param of ESP_BLUFI_EVENT_CONNECT 

disconnect

struct esp_blufi_cb_param_t::blufi_disconnect_evt_param disconnect
Blufi callback param of ESP_BLUFI_EVENT_DISCONNECT 

sta_bssid

struct esp_blufi_cb_param_t::blufi_recv_sta_bssid_evt_param sta_bssid
Blufi callback param of ESP_BLUFI_EVENT_RECV_STA_BSSID 

sta_ssid

struct esp_blufi_cb_param_t::blufi_recv_sta_ssid_evt_param sta_ssid
Blufi callback param of ESP_BLUFI_EVENT_RECV_STA_SSID 

sta_passwd

struct esp_blufi_cb_param_t::blufi_recv_sta_passwd_evt_param sta_passwd
Blufi callback param of ESP_BLUFI_EVENT_RECV_STA_PASSWD 

softap_ssid

struct esp_blufi_cb_param_t::blufi_recv_softap_ssid_evt_param softap_ssid
Blufi callback param of ESP_BLUFI_EVENT_RECV_SOFTAP_SSID 

softap_passwd

struct esp_blufi_cb_param_t::blufi_recv_softap_passwd_evt_param softap_passwd
Blufi callback param of ESP_BLUFI_EVENT_RECV_SOFTAP_PASSWD 

softap_max_conn_num

struct esp_blufi_cb_param_t::blufi_recv_softap_max_conn_num_evt_param softap_max_conn_num
Blufi callback param of ESP_BLUFI_EVENT_RECV_SOFTAP_MAX_CONN_NUM 

softap_auth_mode

struct esp_blufi_cb_param_t::blufi_recv_softap_auth_mode_evt_param softap_auth_mode
Blufi callback param of ESP_BLUFI_EVENT_RECV_SOFTAP_AUTH_MODE 

softap_channel

struct esp_blufi_cb_param_t::blufi_recv_softap_channel_evt_param softap_channel
Blufi callback param of ESP_BLUFI_EVENT_RECV_SOFTAP_CHANNEL 

username

struct esp_blufi_cb_param_t::blufi_recv_username_evt_param username
Blufi callback param of ESP_BLUFI_EVENT_RECV_USERNAME 

ca

struct esp_blufi_cb_param_t::blufi_recv_ca_evt_param ca
Blufi callback param of ESP_BLUFI_EVENT_RECV_CA_CERT 

client_cert

struct esp_blufi_cb_param_t::blufi_recv_client_cert_evt_param client_cert
Blufi callback param of ESP_BLUFI_EVENT_RECV_CLIENT_CERT 

server_cert

struct esp_blufi_cb_param_t::blufi_recv_server_cert_evt_param server_cert
Blufi callback param of ESP_BLUFI_EVENT_RECV_SERVER_CERT 

client_pkey

struct esp_blufi_cb_param_t::blufi_recv_client_pkey_evt_param client_pkey
Blufi callback param of ESP_BLUFI_EVENT_RECV_CLIENT_PRIV_KEY 

server_pkey

struct esp_blufi_cb_param_t::blufi_recv_server_pkey_evt_param server_pkey
Blufi callback param of ESP_BLUFI_EVENT_RECV_SERVER_PRIV_KEY 

report_error

struct esp_blufi_cb_param_t::blufi_get_error_evt_param report_error
Blufi callback param of ESP_BLUFI_EVENT_REPORT_ERROR 

custom_data

struct esp_blufi_cb_param_t::blufi_recv_custom_data_evt_param custom_data
Blufi callback param of ESP_BLUFI_EVENT_RECV_CUSTOM_DATA 


blufi_connect_evt_param

struct blufi_connect_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_CONNECT. 

Public Members
remote_bda

esp_blufi_bd_addr_t remote_bda
Blufi Remote bluetooth device address 

server_if

uint8_t server_if
server interface 

conn_id

uint16_t conn_id
Connection id 



blufi_deinit_finish_evt_param

struct blufi_deinit_finish_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_DEINIT_FINISH. 

Public Members
state

esp_blufi_deinit_state_t state
De-initial status 



blufi_disconnect_evt_param

struct blufi_disconnect_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_DISCONNECT. 

Public Members
remote_bda

esp_blufi_bd_addr_t remote_bda
Blufi Remote bluetooth device address 



blufi_get_error_evt_param

struct blufi_get_error_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_REPORT_ERROR. 

Public Members
state

esp_blufi_error_state_t state
Blufi error state 



blufi_init_finish_evt_param

struct blufi_init_finish_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_INIT_FINISH. 

Public Members
state

esp_blufi_init_state_t state
Initial status 



blufi_recv_ca_evt_param

struct blufi_recv_ca_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_CA_CERT. 

Public Members
cert

uint8_t *cert
CA certificate point 

cert_len

int cert_len
CA certificate length 



blufi_recv_client_cert_evt_param

struct blufi_recv_client_cert_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_CLIENT_CERT 

Public Members
cert

uint8_t *cert
Client certificate point 

cert_len

int cert_len
Client certificate length 



blufi_recv_client_pkey_evt_param

struct blufi_recv_client_pkey_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_CLIENT_PRIV_KEY 

Public Members
pkey

uint8_t *pkey
Client Private Key point, if Client certificate not contain Key 

pkey_len

int pkey_len
Client Private key length 



blufi_recv_custom_data_evt_param

struct blufi_recv_custom_data_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_CUSTOM_DATA. 

Public Members
data

uint8_t *data
Custom data 

data_len

uint32_t data_len
Custom data Length 



blufi_recv_server_cert_evt_param

struct blufi_recv_server_cert_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_SERVER_CERT 

Public Members
cert

uint8_t *cert
Client certificate point 

cert_len

int cert_len
Client certificate length 



blufi_recv_server_pkey_evt_param

struct blufi_recv_server_pkey_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_SERVER_PRIV_KEY 

Public Members
pkey

uint8_t *pkey
Client Private Key point, if Client certificate not contain Key 

pkey_len

int pkey_len
Client Private key length 



blufi_recv_softap_auth_mode_evt_param

struct blufi_recv_softap_auth_mode_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_SOFTAP_AUTH_MODE. 

Public Members
auth_mode

wifi_auth_mode_t auth_mode
Authentication mode 



blufi_recv_softap_channel_evt_param

struct blufi_recv_softap_channel_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_SOFTAP_CHANNEL. 

Public Members
channel

uint8_t channel
Authentication mode 



blufi_recv_softap_max_conn_num_evt_param

struct blufi_recv_softap_max_conn_num_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_SOFTAP_MAX_CONN_NUM. 

Public Members
max_conn_num

int max_conn_num
SSID 



blufi_recv_softap_passwd_evt_param

struct blufi_recv_softap_passwd_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_SOFTAP_PASSWD. 

Public Members
passwd

uint8_t *passwd
Password 

passwd_len

int passwd_len
Password Length 



blufi_recv_softap_ssid_evt_param

struct blufi_recv_softap_ssid_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_SOFTAP_SSID. 

Public Members
ssid

uint8_t *ssid
SSID 

ssid_len

int ssid_len
SSID length 



blufi_recv_sta_bssid_evt_param

struct blufi_recv_sta_bssid_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_STA_BSSID. 

Public Members
bssid

uint8_t bssid[6]
BSSID 



blufi_recv_sta_passwd_evt_param

struct blufi_recv_sta_passwd_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_STA_PASSWD. 

Public Members
passwd

uint8_t *passwd
Password 

passwd_len

int passwd_len
Password Length 



blufi_recv_sta_ssid_evt_param

struct blufi_recv_sta_ssid_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_STA_SSID. 

Public Members
ssid

uint8_t *ssid
SSID 

ssid_len

int ssid_len
SSID length 



blufi_recv_username_evt_param

struct blufi_recv_username_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_RECV_USERNAME. 

Public Members
name

uint8_t *name
Username point 

name_len

int name_len
Username length 



blufi_set_wifi_mode_evt_param

struct blufi_set_wifi_mode_evt_param

#include <esp_blufi_api.h>
ESP_BLUFI_EVENT_SET_WIFI_MODE. 

Public Members
op_mode

wifi_mode_t op_mode
Wifi operation mode 




```



### Structures


#### esp_blufi_extra_info_t


```


struct esp_blufi_extra_info_t
BLUFI extra information structure. 

Public Members
sta_bssid

uint8_t sta_bssid[6]
BSSID of station interface 

sta_bssid_set

bool sta_bssid_set
is BSSID of station interface set 

sta_ssid

uint8_t *sta_ssid
SSID of station interface 

sta_ssid_len

int sta_ssid_len
length of SSID of station interface 

sta_passwd

uint8_t *sta_passwd
password of station interface 

sta_passwd_len

int sta_passwd_len
length of password of station interface 

softap_ssid

uint8_t *softap_ssid
SSID of softap interface 

softap_ssid_len

int softap_ssid_len
length of SSID of softap interface 

softap_passwd

uint8_t *softap_passwd
password of station interface 

softap_passwd_len

int softap_passwd_len
length of password of station interface 

softap_authmode

uint8_t softap_authmode
authentication mode of softap interface 

softap_authmode_set

bool softap_authmode_set
is authentication mode of softap interface set 

softap_max_conn_num

uint8_t softap_max_conn_num
max connection number of softap interface 

softap_max_conn_num_set

bool softap_max_conn_num_set
is max connection number of softap interface set 

softap_channel

uint8_t softap_channel
channel of softap interface 

softap_channel_set

bool softap_channel_set
is channel of softap interface set 

sta_max_conn_retry

uint8_t sta_max_conn_retry
max retry of sta establish connection 

sta_max_conn_retry_set

bool sta_max_conn_retry_set
is max retry of sta establish connection set 

sta_conn_end_reason

uint8_t sta_conn_end_reason
reason of sta connection end 

sta_conn_end_reason_set

bool sta_conn_end_reason_set
is reason of sta connection end set 

sta_conn_rssi

int8_t sta_conn_rssi
rssi of sta connection 

sta_conn_rssi_set

bool sta_conn_rssi_set
is rssi of sta connection set 



```

#### esp_blufi_ap_record_t


```


struct esp_blufi_ap_record_t
Description of an WiFi AP. 

Public Members
ssid

uint8_t ssid[33]
SSID of AP 

rssi

int8_t rssi
signal strength of AP 



```

#### esp_blufi_callbacks_t


```


struct esp_blufi_callbacks_t
BLUFI callback functions type. 

Public Members
event_cb

esp_blufi_event_cb_t event_cb
BLUFI event callback 

negotiate_data_handler

esp_blufi_negotiate_data_handler_t negotiate_data_handler
BLUFI negotiate data function for negotiate share key 

encrypt_func

esp_blufi_encrypt_func_t encrypt_func
BLUFI encrypt data function with share key generated by negotiate_data_handler 

decrypt_func

esp_blufi_decrypt_func_t decrypt_func
BLUFI decrypt data function with share key generated by negotiate_data_handler 

checksum_func

esp_blufi_checksum_func_t checksum_func
BLUFI check sum function (FCS) 



```



### Macros


#### ESP_BLUFI_BD_ADDR_LEN


```


ESP_BLUFI_BD_ADDR_LEN
Bluetooth address length. 

```



### Type Definitions


#### esp_blufi_bd_addr_t


```


typedef uint8_t esp_blufi_bd_addr_t[ESP_BLUFI_BD_ADDR_LEN]
Bluetooth device address. 

```

#### esp_blufi_event_cb_t


```


typedef void (*esp_blufi_event_cb_t)(esp_blufi_cb_event_t event, esp_blufi_cb_param_t *param)
BLUFI event callback function type. 

Param event
: Event type 

Param param
: Point to callback parameter, currently is union type 



```

#### esp_blufi_negotiate_data_handler_t


```


typedef void (*esp_blufi_negotiate_data_handler_t)(uint8_t *data, int len, uint8_t **output_data, int *output_len, bool *need_free)
BLUFI negotiate data handler. 

Param data
: data from phone 

Param len
: length of data from phone 

Param output_data
: data want to send to phone 

Param output_len
: length of data want to send to phone 

Param need_free
: output reporting if memory needs to be freed or not * 



```

#### esp_blufi_encrypt_func_t


```


typedef int (*esp_blufi_encrypt_func_t)(uint8_t iv8, uint8_t *crypt_data, int crypt_len)
BLUFI encrypt the data after negotiate a share key. 

Param iv8
: initial vector(8bit), normally, blufi core will input packet sequence number 

Param crypt_data
: plain text and encrypted data, the encrypt function must support autochthonous encrypt 

Param crypt_len
: length of plain text 

Return
Nonnegative number is encrypted length, if error, return negative number; 



```

#### esp_blufi_decrypt_func_t


```


typedef int (*esp_blufi_decrypt_func_t)(uint8_t iv8, uint8_t *crypt_data, int crypt_len)
BLUFI decrypt the data after negotiate a share key. 

Param iv8
: initial vector(8bit), normally, blufi core will input packet sequence number 

Param crypt_data
: encrypted data and plain text, the encrypt function must support autochthonous decrypt 

Param crypt_len
: length of encrypted text 

Return
Nonnegative number is decrypted length, if error, return negative number; 



```

#### esp_blufi_checksum_func_t


```


typedef uint16_t (*esp_blufi_checksum_func_t)(uint8_t iv8, uint8_t *data, int len)
BLUFI checksum. 

Param iv8
: initial vector(8bit), normally, blufi core will input packet sequence number 

Param data
: data need to checksum 

Param len
: length of data 



```



### Enumerations


#### esp_blufi_cb_event_t


```


enum esp_blufi_cb_event_t
Values:
ESP_BLUFI_EVENT_INIT_FINISH

enumerator ESP_BLUFI_EVENT_INIT_FINISH

ESP_BLUFI_EVENT_DEINIT_FINISH

enumerator ESP_BLUFI_EVENT_DEINIT_FINISH

ESP_BLUFI_EVENT_SET_WIFI_OPMODE

enumerator ESP_BLUFI_EVENT_SET_WIFI_OPMODE

ESP_BLUFI_EVENT_BLE_CONNECT

enumerator ESP_BLUFI_EVENT_BLE_CONNECT

ESP_BLUFI_EVENT_BLE_DISCONNECT

enumerator ESP_BLUFI_EVENT_BLE_DISCONNECT

ESP_BLUFI_EVENT_REQ_CONNECT_TO_AP

enumerator ESP_BLUFI_EVENT_REQ_CONNECT_TO_AP

ESP_BLUFI_EVENT_REQ_DISCONNECT_FROM_AP

enumerator ESP_BLUFI_EVENT_REQ_DISCONNECT_FROM_AP

ESP_BLUFI_EVENT_GET_WIFI_STATUS

enumerator ESP_BLUFI_EVENT_GET_WIFI_STATUS

ESP_BLUFI_EVENT_DEAUTHENTICATE_STA

enumerator ESP_BLUFI_EVENT_DEAUTHENTICATE_STA

ESP_BLUFI_EVENT_RECV_STA_BSSID

enumerator ESP_BLUFI_EVENT_RECV_STA_BSSID

ESP_BLUFI_EVENT_RECV_STA_SSID

enumerator ESP_BLUFI_EVENT_RECV_STA_SSID

ESP_BLUFI_EVENT_RECV_STA_PASSWD

enumerator ESP_BLUFI_EVENT_RECV_STA_PASSWD

ESP_BLUFI_EVENT_RECV_SOFTAP_SSID

enumerator ESP_BLUFI_EVENT_RECV_SOFTAP_SSID

ESP_BLUFI_EVENT_RECV_SOFTAP_PASSWD

enumerator ESP_BLUFI_EVENT_RECV_SOFTAP_PASSWD

ESP_BLUFI_EVENT_RECV_SOFTAP_MAX_CONN_NUM

enumerator ESP_BLUFI_EVENT_RECV_SOFTAP_MAX_CONN_NUM

ESP_BLUFI_EVENT_RECV_SOFTAP_AUTH_MODE

enumerator ESP_BLUFI_EVENT_RECV_SOFTAP_AUTH_MODE

ESP_BLUFI_EVENT_RECV_SOFTAP_CHANNEL

enumerator ESP_BLUFI_EVENT_RECV_SOFTAP_CHANNEL

ESP_BLUFI_EVENT_RECV_USERNAME

enumerator ESP_BLUFI_EVENT_RECV_USERNAME

ESP_BLUFI_EVENT_RECV_CA_CERT

enumerator ESP_BLUFI_EVENT_RECV_CA_CERT

ESP_BLUFI_EVENT_RECV_CLIENT_CERT

enumerator ESP_BLUFI_EVENT_RECV_CLIENT_CERT

ESP_BLUFI_EVENT_RECV_SERVER_CERT

enumerator ESP_BLUFI_EVENT_RECV_SERVER_CERT

ESP_BLUFI_EVENT_RECV_CLIENT_PRIV_KEY

enumerator ESP_BLUFI_EVENT_RECV_CLIENT_PRIV_KEY

ESP_BLUFI_EVENT_RECV_SERVER_PRIV_KEY

enumerator ESP_BLUFI_EVENT_RECV_SERVER_PRIV_KEY

ESP_BLUFI_EVENT_RECV_SLAVE_DISCONNECT_BLE

enumerator ESP_BLUFI_EVENT_RECV_SLAVE_DISCONNECT_BLE

ESP_BLUFI_EVENT_GET_WIFI_LIST

enumerator ESP_BLUFI_EVENT_GET_WIFI_LIST

ESP_BLUFI_EVENT_REPORT_ERROR

enumerator ESP_BLUFI_EVENT_REPORT_ERROR

ESP_BLUFI_EVENT_RECV_CUSTOM_DATA

enumerator ESP_BLUFI_EVENT_RECV_CUSTOM_DATA


```

#### esp_blufi_sta_conn_state_t


```


enum esp_blufi_sta_conn_state_t
BLUFI config status. 
Values:
ESP_BLUFI_STA_CONN_SUCCESS

enumerator ESP_BLUFI_STA_CONN_SUCCESS

ESP_BLUFI_STA_CONN_FAIL

enumerator ESP_BLUFI_STA_CONN_FAIL

ESP_BLUFI_STA_CONNECTING

enumerator ESP_BLUFI_STA_CONNECTING

ESP_BLUFI_STA_NO_IP

enumerator ESP_BLUFI_STA_NO_IP


```

#### esp_blufi_init_state_t


```


enum esp_blufi_init_state_t
BLUFI init status. 
Values:
ESP_BLUFI_INIT_OK

enumerator ESP_BLUFI_INIT_OK

ESP_BLUFI_INIT_FAILED

enumerator ESP_BLUFI_INIT_FAILED


```

#### esp_blufi_deinit_state_t


```


enum esp_blufi_deinit_state_t
BLUFI deinit status. 
Values:
ESP_BLUFI_DEINIT_OK

enumerator ESP_BLUFI_DEINIT_OK

ESP_BLUFI_DEINIT_FAILED

enumerator ESP_BLUFI_DEINIT_FAILED


```

#### esp_blufi_error_state_t


```


enum esp_blufi_error_state_t
Values:
ESP_BLUFI_SEQUENCE_ERROR

enumerator ESP_BLUFI_SEQUENCE_ERROR

ESP_BLUFI_CHECKSUM_ERROR

enumerator ESP_BLUFI_CHECKSUM_ERROR

ESP_BLUFI_DECRYPT_ERROR

enumerator ESP_BLUFI_DECRYPT_ERROR

ESP_BLUFI_ENCRYPT_ERROR

enumerator ESP_BLUFI_ENCRYPT_ERROR

ESP_BLUFI_INIT_SECURITY_ERROR

enumerator ESP_BLUFI_INIT_SECURITY_ERROR

ESP_BLUFI_DH_MALLOC_ERROR

enumerator ESP_BLUFI_DH_MALLOC_ERROR

ESP_BLUFI_DH_PARAM_ERROR

enumerator ESP_BLUFI_DH_PARAM_ERROR

ESP_BLUFI_READ_PARAM_ERROR

enumerator ESP_BLUFI_READ_PARAM_ERROR

ESP_BLUFI_MAKE_PUBLIC_ERROR

enumerator ESP_BLUFI_MAKE_PUBLIC_ERROR

ESP_BLUFI_DATA_FORMAT_ERROR

enumerator ESP_BLUFI_DATA_FORMAT_ERROR

ESP_BLUFI_CALC_MD5_ERROR

enumerator ESP_BLUFI_CALC_MD5_ERROR

ESP_BLUFI_WIFI_SCAN_FAIL

enumerator ESP_BLUFI_WIFI_SCAN_FAIL

ESP_BLUFI_MSG_STATE_ERROR

enumerator ESP_BLUFI_MSG_STATE_ERROR


```