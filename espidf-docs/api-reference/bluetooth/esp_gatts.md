# GATT Server API




## Application Examples


Check bluetooth/bluedroid/ble folder in ESP-IDF examples, which contains the following demos and their tutorials:


* This is a GATT server demo and its tutorial. This demo creates a GATT service with an attribute table, which releases the user from the operation of adding attributes one by one. This is the recommended method of adding attributes (officially recommended).



> + bluetooth/bluedroid/ble/gatt_server_service_table
> 	+ GATT Server Service Table Example Walkthrough
* This is a GATT server demo and its tutorial. This demo creates a GATT service by adding attributes one by one as defined by Bluedroid. The recommended method of adding attributes is presented in the example below.



> + bluetooth/bluedroid/ble/gatt_server
> 	+ GATT Server Example Walkthrough
* This is a demo similar to Bluetooth® Low Energy (Bluetooth LE) SPP. In this demo, GATT server can receive data from UART and then send the data to the peer device automatically.



> + bluetooth/bluedroid/ble/ble_spp_server




## API Reference



### Header File


* components/bt/host/bluedroid/api/include/api/esp_gatts_api.h
* This header file can be included with:



> ```
> #include "esp_gatts_api.h"
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


#### esp_ble_gatts_register_callback


```


esp_err_t esp_ble_gatts_register_callback(esp_gatts_cb_t callback)
This function is called to register application callbacks with BTA GATTS module. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_get_callback


```


esp_gatts_cb_t esp_ble_gatts_get_callback(void)
This function is called to get the current application callbacks with BTA GATTS module. 

Returns

esp_gatts_cb_t : current callback 





```

#### esp_ble_gatts_app_register


```


esp_err_t esp_ble_gatts_app_register(uint16_t app_id)
This function is called to register application identifier. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_app_unregister


```


esp_err_t esp_ble_gatts_app_unregister(esp_gatt_if_t gatts_if)
unregister with GATT Server. 

Parameters
gatts_if -- [in] GATT server access interface 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_create_service


```


esp_err_t esp_ble_gatts_create_service(esp_gatt_if_t gatts_if, esp_gatt_srvc_id_t *service_id, uint16_t num_handle)
Create a service. When service creation is done, a callback event ESP_GATTS_CREATE_EVT is called to report status and service ID to the profile. The service ID obtained in the callback function needs to be used when adding included service and characteristics/descriptors into the service. 

Parameters

gatts_if -- [in] GATT server access interface 
service_id -- [in] service ID. 
num_handle -- [in] number of handle requested for this service.


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_create_attr_tab


```


esp_err_t esp_ble_gatts_create_attr_tab(const esp_gatts_attr_db_t *gatts_attr_db, esp_gatt_if_t gatts_if, uint16_t max_nb_attr, uint8_t srvc_inst_id)
Create a service attribute tab. 

Parameters

gatts_attr_db -- [in] the pointer to the service attr tab 
gatts_if -- [in] GATT server access interface 
max_nb_attr -- [in] the number of attribute to be added to the service database. 
srvc_inst_id -- [in] the instance id of the service


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_add_included_service


```


esp_err_t esp_ble_gatts_add_included_service(uint16_t service_handle, uint16_t included_service_handle)
This function is called to add an included service. This function have to be called between 'esp_ble_gatts_create_service' and 'esp_ble_gatts_add_char'. After included service is included, a callback event ESP_GATTS_ADD_INCL_SRVC_EVT is reported the included service ID. 

Parameters

service_handle -- [in] service handle to which this included service is to be added. 
included_service_handle -- [in] the service ID to be included.


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_add_char


```


esp_err_t esp_ble_gatts_add_char(uint16_t service_handle, esp_bt_uuid_t *char_uuid, esp_gatt_perm_t perm, esp_gatt_char_prop_t property, esp_attr_value_t *char_val, esp_attr_control_t *control)
This function is called to add a characteristic into a service. 

Parameters

service_handle -- [in] service handle to which this included service is to be added. 
char_uuid -- [in] : Characteristic UUID. 
perm -- [in] : Characteristic value declaration attribute permission. 
property -- [in] : Characteristic Properties 
char_val -- [in] : Characteristic value 
control -- [in] : attribute response control byte


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_add_char_descr


```


esp_err_t esp_ble_gatts_add_char_descr(uint16_t service_handle, esp_bt_uuid_t *descr_uuid, esp_gatt_perm_t perm, esp_attr_value_t *char_descr_val, esp_attr_control_t *control)
This function is called to add characteristic descriptor. When it's done, a callback event ESP_GATTS_ADD_DESCR_EVT is called to report the status and an ID number for this descriptor. 

Parameters

service_handle -- [in] service handle to which this characteristic descriptor is to be added. 
perm -- [in] descriptor access permission. 
descr_uuid -- [in] descriptor UUID. 
char_descr_val -- [in] : Characteristic descriptor value 
control -- [in] : attribute response control byte 


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_delete_service


```


esp_err_t esp_ble_gatts_delete_service(uint16_t service_handle)
This function is called to delete a service. When this is done, a callback event ESP_GATTS_DELETE_EVT is report with the status. 

Parameters
service_handle -- [in] service_handle to be deleted.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_start_service


```


esp_err_t esp_ble_gatts_start_service(uint16_t service_handle)
This function is called to start a service. 

Parameters
service_handle -- [in] the service handle to be started.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_stop_service


```


esp_err_t esp_ble_gatts_stop_service(uint16_t service_handle)
This function is called to stop a service. 

Parameters
service_handle -- [in] - service to be topped.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_send_indicate


```


esp_err_t esp_ble_gatts_send_indicate(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle, uint16_t value_len, uint8_t *value, bool need_confirm)
Send indicate or notify to GATT client. Set param need_confirm as false will send notification, otherwise indication. Note: the size of indicate or notify data need less than MTU size,see "esp_ble_gattc_send_mtu_req". 

Parameters

gatts_if -- [in] GATT server access interface 
conn_id -- [in] - connection id to indicate. 
attr_handle -- [in] - attribute handle to indicate. 
value_len -- [in] - indicate value length. 
value -- [in] value to indicate. 
need_confirm -- [in] - Whether a confirmation is required. false sends a GATT notification, true sends a GATT indication.


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_send_response


```


esp_err_t esp_ble_gatts_send_response(esp_gatt_if_t gatts_if, uint16_t conn_id, uint32_t trans_id, esp_gatt_status_t status, esp_gatt_rsp_t *rsp)
This function is called to send a response to a request. 

Parameters

gatts_if -- [in] GATT server access interface 
conn_id -- [in] - connection identifier. 
trans_id -- [in] - transfer id 
status -- [in] - response status 
rsp -- [in] - response data.


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_set_attr_value


```


esp_err_t esp_ble_gatts_set_attr_value(uint16_t attr_handle, uint16_t length, const uint8_t *value)
This function is called to set the attribute value by the application. 

Parameters

attr_handle -- [in] the attribute handle which to be set 
length -- [in] the value length 
value -- [in] the pointer to the attribute value


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_get_attr_value


```


esp_gatt_status_t esp_ble_gatts_get_attr_value(uint16_t attr_handle, uint16_t *length, const uint8_t **value)
Retrieve attribute value. 

Parameters

attr_handle -- [in] Attribute handle. 
length -- [out] pointer to the attribute value length 
value -- [out] Pointer to attribute value payload, the value cannot be modified by user


Returns

ESP_GATT_OK : success
other : failed 





```

#### esp_ble_gatts_open


```


esp_err_t esp_ble_gatts_open(esp_gatt_if_t gatts_if, esp_bd_addr_t remote_bda, bool is_direct)
Open a direct open connection or add a background auto connection. 

Parameters

gatts_if -- [in] GATT server access interface 
remote_bda -- [in] remote device bluetooth device address. 
is_direct -- [in] direct connection or background auto connection


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_close


```


esp_err_t esp_ble_gatts_close(esp_gatt_if_t gatts_if, uint16_t conn_id)
Close a connection a remote device. 

Parameters

gatts_if -- [in] GATT server access interface 
conn_id -- [in] connection ID to be closed.


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_send_service_change_indication


```


esp_err_t esp_ble_gatts_send_service_change_indication(esp_gatt_if_t gatts_if, esp_bd_addr_t remote_bda)
Send service change indication. 

Parameters

gatts_if -- [in] GATT server access interface 
remote_bda -- [in] remote device bluetooth device address. If remote_bda is NULL then it will send service change indication to all the connected devices and if not then to a specific device


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gatts_show_local_database


```


esp_err_t esp_ble_gatts_show_local_database(void)
Print local database (GATT service table) 

Returns

ESP_OK : success
other : failed 





```



### Unions


#### esp_ble_gatts_cb_param_t


```


union esp_ble_gatts_cb_param_t

#include <esp_gatts_api.h>
Gatt server callback parameters union. 

Public Members
reg

struct esp_ble_gatts_cb_param_t::gatts_reg_evt_param reg
Gatt server callback param of ESP_GATTS_REG_EVT 

read

struct esp_ble_gatts_cb_param_t::gatts_read_evt_param read
Gatt server callback param of ESP_GATTS_READ_EVT 

write

struct esp_ble_gatts_cb_param_t::gatts_write_evt_param write
Gatt server callback param of ESP_GATTS_WRITE_EVT 

exec_write

struct esp_ble_gatts_cb_param_t::gatts_exec_write_evt_param exec_write
Gatt server callback param of ESP_GATTS_EXEC_WRITE_EVT 

mtu

struct esp_ble_gatts_cb_param_t::gatts_mtu_evt_param mtu
Gatt server callback param of ESP_GATTS_MTU_EVT 

conf

struct esp_ble_gatts_cb_param_t::gatts_conf_evt_param conf
Gatt server callback param of ESP_GATTS_CONF_EVT (confirm) 

create

struct esp_ble_gatts_cb_param_t::gatts_create_evt_param create
Gatt server callback param of ESP_GATTS_CREATE_EVT 

add_incl_srvc

struct esp_ble_gatts_cb_param_t::gatts_add_incl_srvc_evt_param add_incl_srvc
Gatt server callback param of ESP_GATTS_ADD_INCL_SRVC_EVT 

add_char

struct esp_ble_gatts_cb_param_t::gatts_add_char_evt_param add_char
Gatt server callback param of ESP_GATTS_ADD_CHAR_EVT 

add_char_descr

struct esp_ble_gatts_cb_param_t::gatts_add_char_descr_evt_param add_char_descr
Gatt server callback param of ESP_GATTS_ADD_CHAR_DESCR_EVT 

del

struct esp_ble_gatts_cb_param_t::gatts_delete_evt_param del
Gatt server callback param of ESP_GATTS_DELETE_EVT 

start

struct esp_ble_gatts_cb_param_t::gatts_start_evt_param start
Gatt server callback param of ESP_GATTS_START_EVT 

stop

struct esp_ble_gatts_cb_param_t::gatts_stop_evt_param stop
Gatt server callback param of ESP_GATTS_STOP_EVT 

connect

struct esp_ble_gatts_cb_param_t::gatts_connect_evt_param connect
Gatt server callback param of ESP_GATTS_CONNECT_EVT 

disconnect

struct esp_ble_gatts_cb_param_t::gatts_disconnect_evt_param disconnect
Gatt server callback param of ESP_GATTS_DISCONNECT_EVT 

open

struct esp_ble_gatts_cb_param_t::gatts_open_evt_param open
Gatt server callback param of ESP_GATTS_OPEN_EVT 

cancel_open

struct esp_ble_gatts_cb_param_t::gatts_cancel_open_evt_param cancel_open
Gatt server callback param of ESP_GATTS_CANCEL_OPEN_EVT 

close

struct esp_ble_gatts_cb_param_t::gatts_close_evt_param close
Gatt server callback param of ESP_GATTS_CLOSE_EVT 

congest

struct esp_ble_gatts_cb_param_t::gatts_congest_evt_param congest
Gatt server callback param of ESP_GATTS_CONGEST_EVT 

rsp

struct esp_ble_gatts_cb_param_t::gatts_rsp_evt_param rsp
Gatt server callback param of ESP_GATTS_RESPONSE_EVT 

add_attr_tab

struct esp_ble_gatts_cb_param_t::gatts_add_attr_tab_evt_param add_attr_tab
Gatt server callback param of ESP_GATTS_CREAT_ATTR_TAB_EVT 

set_attr_val

struct esp_ble_gatts_cb_param_t::gatts_set_attr_val_evt_param set_attr_val
Gatt server callback param of ESP_GATTS_SET_ATTR_VAL_EVT 

service_change

struct esp_ble_gatts_cb_param_t::gatts_send_service_change_evt_param service_change
Gatt server callback param of ESP_GATTS_SEND_SERVICE_CHANGE_EVT 


gatts_add_attr_tab_evt_param

struct gatts_add_attr_tab_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_CREAT_ATTR_TAB_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

svc_uuid

esp_bt_uuid_t svc_uuid
Service uuid type 

svc_inst_id

uint8_t svc_inst_id
Service id 

num_handle

uint16_t num_handle
The number of the attribute handle to be added to the gatts database 

handles

uint16_t *handles
The number to the handles 



gatts_add_char_descr_evt_param

struct gatts_add_char_descr_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_ADD_CHAR_DESCR_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

attr_handle

uint16_t attr_handle
Descriptor attribute handle 

service_handle

uint16_t service_handle
Service attribute handle 

descr_uuid

esp_bt_uuid_t descr_uuid
Characteristic descriptor uuid 



gatts_add_char_evt_param

struct gatts_add_char_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_ADD_CHAR_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

attr_handle

uint16_t attr_handle
Characteristic attribute handle 

service_handle

uint16_t service_handle
Service attribute handle 

char_uuid

esp_bt_uuid_t char_uuid
Characteristic uuid 



gatts_add_incl_srvc_evt_param

struct gatts_add_incl_srvc_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_ADD_INCL_SRVC_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

attr_handle

uint16_t attr_handle
Included service attribute handle 

service_handle

uint16_t service_handle
Service attribute handle 



gatts_cancel_open_evt_param

struct gatts_cancel_open_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_CANCEL_OPEN_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 



gatts_close_evt_param

struct gatts_close_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_CLOSE_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 



gatts_conf_evt_param

struct gatts_conf_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_CONF_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

handle

uint16_t handle
attribute handle 

len

uint16_t len
The indication or notification value length, len is valid when send notification or indication failed 

value

uint8_t *value
The indication or notification value , value is valid when send notification or indication failed 



gatts_congest_evt_param

struct gatts_congest_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_LISTEN_EVT. 
ESP_GATTS_CONGEST_EVT 

Public Members
conn_id

uint16_t conn_id
Connection id 

congested

bool congested
Congested or not 



gatts_connect_evt_param

struct gatts_connect_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_CONNECT_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

link_role

uint8_t link_role
Link role : master role = 0 ; slave role = 1 

remote_bda

esp_bd_addr_t remote_bda
Remote bluetooth device address 

conn_params

esp_gatt_conn_params_t conn_params
current Connection parameters 

ble_addr_type

esp_ble_addr_type_t ble_addr_type
Remote BLE device address type 

conn_handle

uint16_t conn_handle
HCI connection handle 



gatts_create_evt_param

struct gatts_create_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_UNREG_EVT. 
ESP_GATTS_CREATE_EVT 

Public Members
status

esp_gatt_status_t status
Operation status 

service_handle

uint16_t service_handle
Service attribute handle 

service_id

esp_gatt_srvc_id_t service_id
Service id, include service uuid and other information 



gatts_delete_evt_param

struct gatts_delete_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_DELETE_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

service_handle

uint16_t service_handle
Service attribute handle 



gatts_disconnect_evt_param

struct gatts_disconnect_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_DISCONNECT_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

remote_bda

esp_bd_addr_t remote_bda
Remote bluetooth device address 

reason

esp_gatt_conn_reason_t reason
Indicate the reason of disconnection 



gatts_exec_write_evt_param

struct gatts_exec_write_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_EXEC_WRITE_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

trans_id

uint32_t trans_id
Transfer id 

bda

esp_bd_addr_t bda
The bluetooth device address which been written 

exec_write_flag

uint8_t exec_write_flag
Execute write flag 



gatts_mtu_evt_param

struct gatts_mtu_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_MTU_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

mtu

uint16_t mtu
MTU size 



gatts_open_evt_param

struct gatts_open_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_OPEN_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 



gatts_read_evt_param

struct gatts_read_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_READ_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

trans_id

uint32_t trans_id
Transfer id 

bda

esp_bd_addr_t bda
The bluetooth device address which been read 

handle

uint16_t handle
The attribute handle 

offset

uint16_t offset
Offset of the value, if the value is too long 

is_long

bool is_long
The value is too long or not 

need_rsp

bool need_rsp
The read operation need to do response 



gatts_reg_evt_param

struct gatts_reg_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_REG_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

app_id

uint16_t app_id
Application id which input in register API 



gatts_rsp_evt_param

struct gatts_rsp_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_RESPONSE_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

handle

uint16_t handle
Attribute handle which send response 



gatts_send_service_change_evt_param

struct gatts_send_service_change_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_SEND_SERVICE_CHANGE_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 



gatts_set_attr_val_evt_param

struct gatts_set_attr_val_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_SET_ATTR_VAL_EVT. 

Public Members
srvc_handle

uint16_t srvc_handle
The service handle 

attr_handle

uint16_t attr_handle
The attribute handle 

status

esp_gatt_status_t status
Operation status 



gatts_start_evt_param

struct gatts_start_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_START_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

service_handle

uint16_t service_handle
Service attribute handle 



gatts_stop_evt_param

struct gatts_stop_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_STOP_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

service_handle

uint16_t service_handle
Service attribute handle 



gatts_write_evt_param

struct gatts_write_evt_param

#include <esp_gatts_api.h>
ESP_GATTS_WRITE_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

trans_id

uint32_t trans_id
Transfer id 

bda

esp_bd_addr_t bda
The bluetooth device address which been written 

handle

uint16_t handle
The attribute handle 

offset

uint16_t offset
Offset of the value, if the value is too long 

need_rsp

bool need_rsp
The write operation need to do response 

is_prep

bool is_prep
This write operation is prepare write 

len

uint16_t len
The write attribute value length 

value

uint8_t *value
The write attribute value 




```



### Macros


#### ESP_GATT_PREP_WRITE_CANCEL


```


ESP_GATT_PREP_WRITE_CANCEL
Prepare write flag to indicate cancel prepare write 

```

#### ESP_GATT_PREP_WRITE_EXEC


```


ESP_GATT_PREP_WRITE_EXEC
Prepare write flag to indicate execute prepare write 

```



### Type Definitions


#### esp_gatts_cb_t


```


typedef void (*esp_gatts_cb_t)(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
GATT Server callback function type. 

Param event
: Event type 

Param gatts_if
: GATT server access interface, normally different gatts_if correspond to different profile 

Param param
: Point to callback parameter, currently is union type 



```



### Enumerations


#### esp_gatts_cb_event_t


```


enum esp_gatts_cb_event_t
GATT Server callback function events. 
Values:
ESP_GATTS_REG_EVT

enumerator ESP_GATTS_REG_EVT
When register application id, the event comes 

ESP_GATTS_READ_EVT

enumerator ESP_GATTS_READ_EVT
When gatt client request read operation, the event comes 

ESP_GATTS_WRITE_EVT

enumerator ESP_GATTS_WRITE_EVT
When gatt client request write operation, the event comes 

ESP_GATTS_EXEC_WRITE_EVT

enumerator ESP_GATTS_EXEC_WRITE_EVT
When gatt client request execute write, the event comes 

ESP_GATTS_MTU_EVT

enumerator ESP_GATTS_MTU_EVT
When set mtu complete, the event comes 

ESP_GATTS_CONF_EVT

enumerator ESP_GATTS_CONF_EVT
When receive confirm, the event comes 

ESP_GATTS_UNREG_EVT

enumerator ESP_GATTS_UNREG_EVT
When unregister application id, the event comes 

ESP_GATTS_CREATE_EVT

enumerator ESP_GATTS_CREATE_EVT
When create service complete, the event comes 

ESP_GATTS_ADD_INCL_SRVC_EVT

enumerator ESP_GATTS_ADD_INCL_SRVC_EVT
When add included service complete, the event comes 

ESP_GATTS_ADD_CHAR_EVT

enumerator ESP_GATTS_ADD_CHAR_EVT
When add characteristic complete, the event comes 

ESP_GATTS_ADD_CHAR_DESCR_EVT

enumerator ESP_GATTS_ADD_CHAR_DESCR_EVT
When add descriptor complete, the event comes 

ESP_GATTS_DELETE_EVT

enumerator ESP_GATTS_DELETE_EVT
When delete service complete, the event comes 

ESP_GATTS_START_EVT

enumerator ESP_GATTS_START_EVT
When start service complete, the event comes 

ESP_GATTS_STOP_EVT

enumerator ESP_GATTS_STOP_EVT
When stop service complete, the event comes 

ESP_GATTS_CONNECT_EVT

enumerator ESP_GATTS_CONNECT_EVT
When gatt client connect, the event comes 

ESP_GATTS_DISCONNECT_EVT

enumerator ESP_GATTS_DISCONNECT_EVT
When gatt client disconnect, the event comes 

ESP_GATTS_OPEN_EVT

enumerator ESP_GATTS_OPEN_EVT
When connect to peer, the event comes 

ESP_GATTS_CANCEL_OPEN_EVT

enumerator ESP_GATTS_CANCEL_OPEN_EVT
When disconnect from peer, the event comes 

ESP_GATTS_CLOSE_EVT

enumerator ESP_GATTS_CLOSE_EVT
When gatt server close, the event comes 

ESP_GATTS_LISTEN_EVT

enumerator ESP_GATTS_LISTEN_EVT
When gatt listen to be connected the event comes 

ESP_GATTS_CONGEST_EVT

enumerator ESP_GATTS_CONGEST_EVT
When congest happen, the event comes 

ESP_GATTS_RESPONSE_EVT

enumerator ESP_GATTS_RESPONSE_EVT
When gatt send response complete, the event comes 

ESP_GATTS_CREAT_ATTR_TAB_EVT

enumerator ESP_GATTS_CREAT_ATTR_TAB_EVT
When gatt create table complete, the event comes 

ESP_GATTS_SET_ATTR_VAL_EVT

enumerator ESP_GATTS_SET_ATTR_VAL_EVT
When gatt set attr value complete, the event comes 

ESP_GATTS_SEND_SERVICE_CHANGE_EVT

enumerator ESP_GATTS_SEND_SERVICE_CHANGE_EVT
When gatt send service change indication complete, the event comes 


```