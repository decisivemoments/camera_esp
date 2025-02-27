# GATT Client API



## Application Example


Check bluetooth/bluedroid/ble folder in ESP-IDF examples, which contains the following demos and their tutorials:


* This is a GATT client demo and its tutorial. This demo can scan for devices, connect to the GATT server and discover its services.



> + bluetooth/bluedroid/ble/gatt_client
> 	+ GATT Client Example Walkthrough
* This is a multiple connection demo and its tutorial. This demo can connect to multiple GATT server devices and discover their services.



> + bluetooth/bluedroid/ble/gattc_multi_connect
> 	+ GATT Client Multi-connection Example Walkthrough
* This is a demo similar to Bluetooth® Low Energy (Bluetooth LE) SPP. This demo, which acts as a GATT client, can receive data from UART and then send the data to the peer device automatically.



> + bluetooth/bluedroid/ble/ble_spp_client




## API Reference



### Header File


* components/bt/host/bluedroid/api/include/api/esp_gattc_api.h
* This header file can be included with:



> ```
> #include "esp_gattc_api.h"
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


#### esp_ble_gattc_register_callback


```


esp_err_t esp_ble_gattc_register_callback(esp_gattc_cb_t callback)
This function is called to register application callbacks with GATTC module. 

Parameters
callback -- [in] : pointer to the application callback function.

Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_callback


```


esp_gattc_cb_t esp_ble_gattc_get_callback(void)
This function is called to get the current application callbacks with BTA GATTC module. 

Returns

esp_gattC_cb_t : current callback 





```

#### esp_ble_gattc_app_register


```


esp_err_t esp_ble_gattc_app_register(uint16_t app_id)
This function is called to register application callbacks with GATTC module. 

Parameters
app_id -- [in] : Application Identify (UUID), for different application

Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_app_unregister


```


esp_err_t esp_ble_gattc_app_unregister(esp_gatt_if_t gattc_if)
This function is called to unregister an application from the GATTC module. 

Note
Before calling this API, ensure that all activities related to the application, such as connections, scans, ADV, are properly closed.


Parameters
gattc_if -- [in] Gatt client access interface.

Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_open


```


esp_err_t esp_ble_gattc_open(esp_gatt_if_t gattc_if, esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type, bool is_direct)
Open a direct connection or add a background auto connection. 

Parameters

gattc_if -- [in] Gatt client access interface. 
remote_bda -- [in] remote device bluetooth device address. 
remote_addr_type -- [in] remote device bluetooth device the address type. 
is_direct -- [in] direct connection or background auto connection(by now, background auto connection is not supported).


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_aux_open


```


esp_err_t esp_ble_gattc_aux_open(esp_gatt_if_t gattc_if, esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type, bool is_direct)

```

#### esp_ble_gattc_close


```


esp_err_t esp_ble_gattc_close(esp_gatt_if_t gattc_if, uint16_t conn_id)
Close the virtual connection to the GATT server. gattc may have multiple virtual GATT server connections when multiple app_id registered, this API only close one virtual GATT server connection. if there exist other virtual GATT server connections, it does not disconnect the physical connection. if you want to disconnect the physical connection directly, you can use esp_ble_gap_disconnect(esp_bd_addr_t remote_device). 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID to be closed.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_send_mtu_req


```


esp_err_t esp_ble_gattc_send_mtu_req(esp_gatt_if_t gattc_if, uint16_t conn_id)
Configure the MTU size in the GATT channel. This can be done only once per connection. Before using, use esp_ble_gatt_set_local_mtu() to configure the local MTU size. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_search_service


```


esp_err_t esp_ble_gattc_search_service(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_bt_uuid_t *filter_uuid)
This function is called to get service from local cache. This function report service search result by a callback event, and followed by a service search complete event. Note: 128-bit base UUID will automatically be converted to a 16-bit UUID in the search results. Other types of UUID remain unchanged. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID. 
filter_uuid -- [in] a UUID of the service application is interested in. If Null, discover for all services.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_service


```


esp_gatt_status_t esp_ble_gattc_get_service(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_bt_uuid_t *svc_uuid, esp_gattc_service_elem_t *result, uint16_t *count, uint16_t offset)
Find all the service with the given service uuid in the gattc cache, if the svc_uuid is NULL, find all the service. Note: It just get service from local cache, won't get from remote devices. If want to get it from remote device, need to used the esp_ble_gattc_cache_refresh, then call esp_ble_gattc_get_service again. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
svc_uuid -- [in] the pointer to the service uuid. 
result -- [out] The pointer to the service which has been found in the gattc cache. 
count -- [inout] input the number of service want to find, it will output the number of service has been found in the gattc cache with the given service uuid. 
offset -- [in] Offset of the service position to get.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_all_char


```


esp_gatt_status_t esp_ble_gattc_get_all_char(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_gattc_char_elem_t *result, uint16_t *count, uint16_t offset)
Find all the characteristic with the given service in the gattc cache Note: It just get characteristic from local cache, won't get from remote devices. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
start_handle -- [in] the attribute start handle. 
end_handle -- [in] the attribute end handle 
result -- [out] The pointer to the characteristic in the service. 
count -- [inout] input the number of characteristic want to find, it will output the number of characteristic has been found in the gattc cache with the given service. 
offset -- [in] Offset of the characteristic position to get.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_all_descr


```


esp_gatt_status_t esp_ble_gattc_get_all_descr(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t char_handle, esp_gattc_descr_elem_t *result, uint16_t *count, uint16_t offset)
Find all the descriptor with the given characteristic in the gattc cache Note: It just get descriptor from local cache, won't get from remote devices. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
char_handle -- [in] the given characteristic handle 
result -- [out] The pointer to the descriptor in the characteristic. 
count -- [inout] input the number of descriptor want to find, it will output the number of descriptor has been found in the gattc cache with the given characteristic. 
offset -- [in] Offset of the descriptor position to get.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_char_by_uuid


```


esp_gatt_status_t esp_ble_gattc_get_char_by_uuid(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_bt_uuid_t char_uuid, esp_gattc_char_elem_t *result, uint16_t *count)
Find the characteristic with the given characteristic uuid in the gattc cache Note: It just get characteristic from local cache, won't get from remote devices. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
start_handle -- [in] the attribute start handle 
end_handle -- [in] the attribute end handle 
char_uuid -- [in] the characteristic uuid 
result -- [out] The pointer to the characteristic in the service. 
count -- [inout] input the number of characteristic want to find, it will output the number of characteristic has been found in the gattc cache with the given service.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_descr_by_uuid


```


esp_gatt_status_t esp_ble_gattc_get_descr_by_uuid(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_bt_uuid_t char_uuid, esp_bt_uuid_t descr_uuid, esp_gattc_descr_elem_t *result, uint16_t *count)
Find the descriptor with the given characteristic uuid in the gattc cache Note: It just get descriptor from local cache, won't get from remote devices. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
start_handle -- [in] the attribute start handle 
end_handle -- [in] the attribute end handle 
char_uuid -- [in] the characteristic uuid. 
descr_uuid -- [in] the descriptor uuid. 
result -- [out] The pointer to the descriptor in the given characteristic. 
count -- [inout] input the number of descriptor want to find, it will output the number of descriptor has been found in the gattc cache with the given characteristic.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_descr_by_char_handle


```


esp_gatt_status_t esp_ble_gattc_get_descr_by_char_handle(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t char_handle, esp_bt_uuid_t descr_uuid, esp_gattc_descr_elem_t *result, uint16_t *count)
Find the descriptor with the given characteristic handle in the gattc cache Note: It just get descriptor from local cache, won't get from remote devices. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
char_handle -- [in] the characteristic handle. 
descr_uuid -- [in] the descriptor uuid. 
result -- [out] The pointer to the descriptor in the given characteristic. 
count -- [inout] input the number of descriptor want to find, it will output the number of descriptor has been found in the gattc cache with the given characteristic.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_include_service


```


esp_gatt_status_t esp_ble_gattc_get_include_service(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_bt_uuid_t *incl_uuid, esp_gattc_incl_svc_elem_t *result, uint16_t *count)
Find the include service with the given service handle in the gattc cache Note: It just get include service from local cache, won't get from remote devices. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
start_handle -- [in] the attribute start handle 
end_handle -- [in] the attribute end handle 
incl_uuid -- [in] the include service uuid 
result -- [out] The pointer to the include service in the given service. 
count -- [inout] input the number of include service want to find, it will output the number of include service has been found in the gattc cache with the given service.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_attr_count


```


esp_gatt_status_t esp_ble_gattc_get_attr_count(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_gatt_db_attr_type_t type, uint16_t start_handle, uint16_t end_handle, uint16_t char_handle, uint16_t *count)
Find the attribute count with the given service or characteristic in the gattc cache. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] connection ID which identify the server. 
type -- [in] the attribute type. 
start_handle -- [in] the attribute start handle, if the type is ESP_GATT_DB_DESCRIPTOR, this parameter should be ignore 
end_handle -- [in] the attribute end handle, if the type is ESP_GATT_DB_DESCRIPTOR, this parameter should be ignore 
char_handle -- [in] the characteristic handle, this parameter valid when the type is ESP_GATT_DB_DESCRIPTOR. If the type isn't ESP_GATT_DB_DESCRIPTOR, this parameter should be ignore. 
count -- [out] output the number of attribute has been found in the gattc cache with the given attribute type.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_get_db


```


esp_gatt_status_t esp_ble_gattc_get_db(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_gattc_db_elem_t *db, uint16_t *count)
This function is called to get the GATT database. Note: It just get attribute data base from local cache, won't get from remote devices. 

Parameters

gattc_if -- [in] Gatt client access interface. 
start_handle -- [in] the attribute start handle 
end_handle -- [in] the attribute end handle 
conn_id -- [in] connection ID which identify the server. 
db -- [in] output parameter which will contain the GATT database copy. Caller is responsible for freeing it. 
count -- [in] number of elements in database.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_read_char


```


esp_err_t esp_ble_gattc_read_char(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, esp_gatt_auth_req_t auth_req)
This function is called to read a service's characteristics of the given characteristic handle. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
handle -- [in] : characteristic handle to read. 
auth_req -- [in] : authenticate request type


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_read_by_type


```


esp_err_t esp_ble_gattc_read_by_type(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_bt_uuid_t *uuid, esp_gatt_auth_req_t auth_req)
This function is called to read a service's characteristics of the given characteristic UUID. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
start_handle -- [in] : the attribute start handle. 
end_handle -- [in] : the attribute end handle 
uuid -- [in] : The UUID of attribute which will be read. 
auth_req -- [in] : authenticate request type


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_read_multiple


```


esp_err_t esp_ble_gattc_read_multiple(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_gattc_multi_t *read_multi, esp_gatt_auth_req_t auth_req)
This function is called to read multiple characteristic or characteristic descriptors. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
read_multi -- [in] : pointer to the read multiple parameter. 
auth_req -- [in] : authenticate request type


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_read_multiple_variable


```


esp_err_t esp_ble_gattc_read_multiple_variable(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_gattc_multi_t *read_multi, esp_gatt_auth_req_t auth_req)
This function is called to read multiple variable length characteristic or characteristic descriptors. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
read_multi -- [in] : pointer to the read multiple parameter. 
auth_req -- [in] : authenticate request type


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_read_char_descr


```


esp_err_t esp_ble_gattc_read_char_descr(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, esp_gatt_auth_req_t auth_req)
This function is called to read a characteristics descriptor. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
handle -- [in] : descriptor handle to read. 
auth_req -- [in] : authenticate request type


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_write_char


```


esp_err_t esp_ble_gattc_write_char(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req)
This function is called to write characteristic value. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
handle -- [in] : characteristic handle to write. 
value_len -- [in] length of the value to be written. 
value -- [in] : the value to be written. 
write_type -- [in] : the type of attribute write operation. 
auth_req -- [in] : authentication request.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_write_char_descr


```


esp_err_t esp_ble_gattc_write_char_descr(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req)
This function is called to write characteristic descriptor value. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID 
handle -- [in] : descriptor handle to write. 
value_len -- [in] length of the value to be written. 
value -- [in] : the value to be written. 
write_type -- [in] : the type of attribute write operation. 
auth_req -- [in] : authentication request.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_prepare_write


```


esp_err_t esp_ble_gattc_prepare_write(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t offset, uint16_t value_len, uint8_t *value, esp_gatt_auth_req_t auth_req)
This function is called to prepare write a characteristic value. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
handle -- [in] : characteristic handle to prepare write. 
offset -- [in] : offset of the write value. 
value_len -- [in] length of the value to be written. 
value -- [in] : the value to be written. 
auth_req -- [in] : authentication request.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_prepare_write_char_descr


```


esp_err_t esp_ble_gattc_prepare_write_char_descr(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t offset, uint16_t value_len, uint8_t *value, esp_gatt_auth_req_t auth_req)
This function is called to prepare write a characteristic descriptor value. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
handle -- [in] : characteristic descriptor handle to prepare write. 
offset -- [in] : offset of the write value. 
value_len -- [in] length of the value to be written. 
value -- [in] : the value to be written. 
auth_req -- [in] : authentication request.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_execute_write


```


esp_err_t esp_ble_gattc_execute_write(esp_gatt_if_t gattc_if, uint16_t conn_id, bool is_execute)
This function is called to execute write a prepare write sequence. 

Parameters

gattc_if -- [in] Gatt client access interface. 
conn_id -- [in] : connection ID. 
is_execute -- [in] : execute or cancel.


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_register_for_notify


```


esp_err_t esp_ble_gattc_register_for_notify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, uint16_t handle)
This function is called to register for notification of a service. 

Parameters

gattc_if -- [in] Gatt client access interface. 
server_bda -- [in] : target GATT server. 
handle -- [in] : GATT characteristic handle.


Returns

ESP_OK: registration succeeds
other: failed 





```

#### esp_ble_gattc_unregister_for_notify


```


esp_err_t esp_ble_gattc_unregister_for_notify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, uint16_t handle)
This function is called to de-register for notification of a service. 

Parameters

gattc_if -- [in] Gatt client access interface. 
server_bda -- [in] : target GATT server. 
handle -- [in] : GATT characteristic handle.


Returns

ESP_OK: unregister succeeds
other: failed 





```

#### esp_ble_gattc_cache_refresh


```


esp_err_t esp_ble_gattc_cache_refresh(esp_bd_addr_t remote_bda)
Refresh the server cache store in the gattc stack of the remote device. If the device is connected, this API will restart the discovery of service information of the remote device. 

Parameters
remote_bda -- [in] remote device BD address.

Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_cache_assoc


```


esp_err_t esp_ble_gattc_cache_assoc(esp_gatt_if_t gattc_if, esp_bd_addr_t src_addr, esp_bd_addr_t assoc_addr, bool is_assoc)
Add or delete the associated address with the source address. Note: The role of this API is mainly when the client side has stored a server-side database, when it needs to connect another device, but the device's attribute database is the same as the server database stored on the client-side, calling this API can use the database that the device has stored used as the peer server database to reduce the attribute database search and discovery process and speed up the connection time. The associated address mains that device want to used the database has stored in the local cache. The source address mains that device want to share the database to the associated address device. 

Parameters

gattc_if -- [in] Gatt client access interface. 
src_addr -- [in] the source address which provide the attribute table. 
assoc_addr -- [in] the associated device address which went to share the attribute table with the source address. 
is_assoc -- [in] true add the associated device address, false remove the associated device address. 


Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_cache_get_addr_list


```


esp_err_t esp_ble_gattc_cache_get_addr_list(esp_gatt_if_t gattc_if)
Get the address list which has store the attribute table in the gattc cache. There will callback ESP_GATTC_GET_ADDR_LIST_EVT event when get address list complete. 

Parameters
gattc_if -- [in] Gatt client access interface. 

Returns

ESP_OK: success
other: failed 





```

#### esp_ble_gattc_cache_clean


```


esp_err_t esp_ble_gattc_cache_clean(esp_bd_addr_t remote_bda)
Clean the service cache of this device in the gattc stack,. 

Parameters
remote_bda -- [in] remote device BD address.

Returns

ESP_OK: success
other: failed 





```



### Unions


#### esp_ble_gattc_cb_param_t


```


union esp_ble_gattc_cb_param_t

#include <esp_gattc_api.h>
Gatt client callback parameters union. 

Public Members
reg

struct esp_ble_gattc_cb_param_t::gattc_reg_evt_param reg
Gatt client callback param of ESP_GATTC_REG_EVT 

open

struct esp_ble_gattc_cb_param_t::gattc_open_evt_param open
Gatt client callback param of ESP_GATTC_OPEN_EVT 

close

struct esp_ble_gattc_cb_param_t::gattc_close_evt_param close
Gatt client callback param of ESP_GATTC_CLOSE_EVT 

cfg_mtu

struct esp_ble_gattc_cb_param_t::gattc_cfg_mtu_evt_param cfg_mtu
Gatt client callback param of ESP_GATTC_CFG_MTU_EVT 

search_cmpl

struct esp_ble_gattc_cb_param_t::gattc_search_cmpl_evt_param search_cmpl
Gatt client callback param of ESP_GATTC_SEARCH_CMPL_EVT 

search_res

struct esp_ble_gattc_cb_param_t::gattc_search_res_evt_param search_res
Gatt client callback param of ESP_GATTC_SEARCH_RES_EVT 

read

struct esp_ble_gattc_cb_param_t::gattc_read_char_evt_param read
Gatt client callback param of ESP_GATTC_READ_CHAR_EVT 

write

struct esp_ble_gattc_cb_param_t::gattc_write_evt_param write
Gatt client callback param of ESP_GATTC_WRITE_DESCR_EVT 

exec_cmpl

struct esp_ble_gattc_cb_param_t::gattc_exec_cmpl_evt_param exec_cmpl
Gatt client callback param of ESP_GATTC_EXEC_EVT 

notify

struct esp_ble_gattc_cb_param_t::gattc_notify_evt_param notify
Gatt client callback param of ESP_GATTC_NOTIFY_EVT 

srvc_chg

struct esp_ble_gattc_cb_param_t::gattc_srvc_chg_evt_param srvc_chg
Gatt client callback param of ESP_GATTC_SRVC_CHG_EVT 

congest

struct esp_ble_gattc_cb_param_t::gattc_congest_evt_param congest
Gatt client callback param of ESP_GATTC_CONGEST_EVT 

reg_for_notify

struct esp_ble_gattc_cb_param_t::gattc_reg_for_notify_evt_param reg_for_notify
Gatt client callback param of ESP_GATTC_REG_FOR_NOTIFY_EVT 

unreg_for_notify

struct esp_ble_gattc_cb_param_t::gattc_unreg_for_notify_evt_param unreg_for_notify
Gatt client callback param of ESP_GATTC_UNREG_FOR_NOTIFY_EVT 

connect

struct esp_ble_gattc_cb_param_t::gattc_connect_evt_param connect
Gatt client callback param of ESP_GATTC_CONNECT_EVT 

disconnect

struct esp_ble_gattc_cb_param_t::gattc_disconnect_evt_param disconnect
Gatt client callback param of ESP_GATTC_DISCONNECT_EVT 

set_assoc_cmp

struct esp_ble_gattc_cb_param_t::gattc_set_assoc_addr_cmp_evt_param set_assoc_cmp
Gatt client callback param of ESP_GATTC_SET_ASSOC_EVT 

get_addr_list

struct esp_ble_gattc_cb_param_t::gattc_get_addr_list_evt_param get_addr_list
Gatt client callback param of ESP_GATTC_GET_ADDR_LIST_EVT 

queue_full

struct esp_ble_gattc_cb_param_t::gattc_queue_full_evt_param queue_full
Gatt client callback param of ESP_GATTC_QUEUE_FULL_EVT 

dis_srvc_cmpl

struct esp_ble_gattc_cb_param_t::gattc_dis_srvc_cmpl_evt_param dis_srvc_cmpl
Gatt client callback param of ESP_GATTC_DIS_SRVC_CMPL_EVT 


gattc_cfg_mtu_evt_param

struct gattc_cfg_mtu_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_CFG_MTU_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

mtu

uint16_t mtu
MTU size 



gattc_close_evt_param

struct gattc_close_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_CLOSE_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

remote_bda

esp_bd_addr_t remote_bda
Remote bluetooth device address 

reason

esp_gatt_conn_reason_t reason
The reason of gatt connection close 



gattc_congest_evt_param

struct gattc_congest_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_CONGEST_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

congested

bool congested
Congested or not 



gattc_connect_evt_param

struct gattc_connect_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_CONNECT_EVT. 

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
current connection parameters 

ble_addr_type

esp_ble_addr_type_t ble_addr_type
Remote BLE device address type 

conn_handle

uint16_t conn_handle
HCI connection handle 



gattc_dis_srvc_cmpl_evt_param

struct gattc_dis_srvc_cmpl_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_DIS_SRVC_CMPL_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 



gattc_disconnect_evt_param

struct gattc_disconnect_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_DISCONNECT_EVT. 

Public Members
reason

esp_gatt_conn_reason_t reason
disconnection reason 

conn_id

uint16_t conn_id
Connection id 

remote_bda

esp_bd_addr_t remote_bda
Remote bluetooth device address 



gattc_exec_cmpl_evt_param

struct gattc_exec_cmpl_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_EXEC_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 



gattc_get_addr_list_evt_param

struct gattc_get_addr_list_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_GET_ADDR_LIST_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

num_addr

uint8_t num_addr
The number of address in the gattc cache address list 

addr_list

esp_bd_addr_t *addr_list
The pointer to the address list which has been get from the gattc cache 



gattc_notify_evt_param

struct gattc_notify_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_NOTIFY_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

remote_bda

esp_bd_addr_t remote_bda
Remote bluetooth device address 

handle

uint16_t handle
The Characteristic or descriptor handle 

value_len

uint16_t value_len
Notify attribute value 

value

uint8_t *value
Notify attribute value 

is_notify

bool is_notify
True means notify, false means indicate 



gattc_open_evt_param

struct gattc_open_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_OPEN_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

remote_bda

esp_bd_addr_t remote_bda
Remote bluetooth device address 

mtu

uint16_t mtu
MTU size 



gattc_queue_full_evt_param

struct gattc_queue_full_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_QUEUE_FULL_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

is_full

bool is_full
The gattc command queue is full or not 



gattc_read_char_evt_param

struct gattc_read_char_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_READ_CHAR_EVT, ESP_GATTC_READ_DESCR_EVT, ESP_GATTC_READ_MULTIPLE_EVT, ESP_GATTC_READ_MULTI_VAR_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

handle

uint16_t handle
Characteristic handle 

value

uint8_t *value
Characteristic value 

value_len

uint16_t value_len
Characteristic value length 



gattc_reg_evt_param

struct gattc_reg_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_REG_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

app_id

uint16_t app_id
Application id which input in register API 



gattc_reg_for_notify_evt_param

struct gattc_reg_for_notify_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_REG_FOR_NOTIFY_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

handle

uint16_t handle
The characteristic or descriptor handle 



gattc_search_cmpl_evt_param

struct gattc_search_cmpl_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_SEARCH_CMPL_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

searched_service_source

esp_service_source_t searched_service_source
The source of the service information 



gattc_search_res_evt_param

struct gattc_search_res_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_SEARCH_RES_EVT. 

Public Members
conn_id

uint16_t conn_id
Connection id 

start_handle

uint16_t start_handle
Service start handle 

end_handle

uint16_t end_handle
Service end handle 

srvc_id

esp_gatt_id_t srvc_id
Service id, include service uuid and other information 

is_primary

bool is_primary
True if this is the primary service 



gattc_set_assoc_addr_cmp_evt_param

struct gattc_set_assoc_addr_cmp_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_SET_ASSOC_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 



gattc_srvc_chg_evt_param

struct gattc_srvc_chg_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_SRVC_CHG_EVT. 

Public Members
remote_bda

esp_bd_addr_t remote_bda
Remote bluetooth device address 



gattc_unreg_for_notify_evt_param

struct gattc_unreg_for_notify_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_UNREG_FOR_NOTIFY_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

handle

uint16_t handle
The characteristic or descriptor handle 



gattc_write_evt_param

struct gattc_write_evt_param

#include <esp_gattc_api.h>
ESP_GATTC_WRITE_CHAR_EVT, ESP_GATTC_PREP_WRITE_EVT, ESP_GATTC_WRITE_DESCR_EVT. 

Public Members
status

esp_gatt_status_t status
Operation status 

conn_id

uint16_t conn_id
Connection id 

handle

uint16_t handle
The Characteristic or descriptor handle 

offset

uint16_t offset
The prepare write offset, this value is valid only when prepare write 




```



### Type Definitions


#### esp_gattc_cb_t


```


typedef void (*esp_gattc_cb_t)(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
GATT Client callback function type. 

Param event
: Event type 

Param gattc_if
: GATT client access interface, normally different gattc_if correspond to different profile 

Param param
: Point to callback parameter, currently is union type 



```



### Enumerations


#### esp_gattc_cb_event_t


```


enum esp_gattc_cb_event_t
GATT Client callback function events. 
Values:
ESP_GATTC_REG_EVT

enumerator ESP_GATTC_REG_EVT
When GATT client is registered, the event comes 

ESP_GATTC_UNREG_EVT

enumerator ESP_GATTC_UNREG_EVT
When GATT client is unregistered, the event comes 

ESP_GATTC_OPEN_EVT

enumerator ESP_GATTC_OPEN_EVT
When GATT virtual connection is set up, the event comes 

ESP_GATTC_READ_CHAR_EVT

enumerator ESP_GATTC_READ_CHAR_EVT
When GATT characteristic is read, the event comes 

ESP_GATTC_WRITE_CHAR_EVT

enumerator ESP_GATTC_WRITE_CHAR_EVT
When GATT characteristic write operation completes, the event comes 

ESP_GATTC_CLOSE_EVT

enumerator ESP_GATTC_CLOSE_EVT
When GATT virtual connection is closed, the event comes 

ESP_GATTC_SEARCH_CMPL_EVT

enumerator ESP_GATTC_SEARCH_CMPL_EVT
When GATT service discovery is completed, the event comes 

ESP_GATTC_SEARCH_RES_EVT

enumerator ESP_GATTC_SEARCH_RES_EVT
When GATT service discovery result is got, the event comes 

ESP_GATTC_READ_DESCR_EVT

enumerator ESP_GATTC_READ_DESCR_EVT
When GATT characteristic descriptor read completes, the event comes 

ESP_GATTC_WRITE_DESCR_EVT

enumerator ESP_GATTC_WRITE_DESCR_EVT
When GATT characteristic descriptor write completes, the event comes 

ESP_GATTC_NOTIFY_EVT

enumerator ESP_GATTC_NOTIFY_EVT
When GATT notification or indication arrives, the event comes 

ESP_GATTC_PREP_WRITE_EVT

enumerator ESP_GATTC_PREP_WRITE_EVT
When GATT prepare-write operation completes, the event comes 

ESP_GATTC_EXEC_EVT

enumerator ESP_GATTC_EXEC_EVT
When write execution completes, the event comes 

ESP_GATTC_ACL_EVT

enumerator ESP_GATTC_ACL_EVT
When ACL connection is up, the event comes 

ESP_GATTC_CANCEL_OPEN_EVT

enumerator ESP_GATTC_CANCEL_OPEN_EVT
When GATT client ongoing connection is cancelled, the event comes 

ESP_GATTC_SRVC_CHG_EVT

enumerator ESP_GATTC_SRVC_CHG_EVT
When "service changed" occurs, the event comes 

ESP_GATTC_ENC_CMPL_CB_EVT

enumerator ESP_GATTC_ENC_CMPL_CB_EVT
When encryption procedure completes, the event comes 

ESP_GATTC_CFG_MTU_EVT

enumerator ESP_GATTC_CFG_MTU_EVT
When configuration of MTU completes, the event comes 

ESP_GATTC_ADV_DATA_EVT

enumerator ESP_GATTC_ADV_DATA_EVT
When advertising of data, the event comes 

ESP_GATTC_MULT_ADV_ENB_EVT

enumerator ESP_GATTC_MULT_ADV_ENB_EVT
When multi-advertising is enabled, the event comes 

ESP_GATTC_MULT_ADV_UPD_EVT

enumerator ESP_GATTC_MULT_ADV_UPD_EVT
When multi-advertising parameters are updated, the event comes 

ESP_GATTC_MULT_ADV_DATA_EVT

enumerator ESP_GATTC_MULT_ADV_DATA_EVT
When multi-advertising data arrives, the event comes 

ESP_GATTC_MULT_ADV_DIS_EVT

enumerator ESP_GATTC_MULT_ADV_DIS_EVT
When multi-advertising is disabled, the event comes 

ESP_GATTC_CONGEST_EVT

enumerator ESP_GATTC_CONGEST_EVT
When GATT connection congestion comes, the event comes 

ESP_GATTC_BTH_SCAN_ENB_EVT

enumerator ESP_GATTC_BTH_SCAN_ENB_EVT
When batch scan is enabled, the event comes 

ESP_GATTC_BTH_SCAN_CFG_EVT

enumerator ESP_GATTC_BTH_SCAN_CFG_EVT
When batch scan storage is configured, the event comes 

ESP_GATTC_BTH_SCAN_RD_EVT

enumerator ESP_GATTC_BTH_SCAN_RD_EVT
When Batch scan read event is reported, the event comes 

ESP_GATTC_BTH_SCAN_THR_EVT

enumerator ESP_GATTC_BTH_SCAN_THR_EVT
When Batch scan threshold is set, the event comes 

ESP_GATTC_BTH_SCAN_PARAM_EVT

enumerator ESP_GATTC_BTH_SCAN_PARAM_EVT
When Batch scan parameters are set, the event comes 

ESP_GATTC_BTH_SCAN_DIS_EVT

enumerator ESP_GATTC_BTH_SCAN_DIS_EVT
When Batch scan is disabled, the event comes 

ESP_GATTC_SCAN_FLT_CFG_EVT

enumerator ESP_GATTC_SCAN_FLT_CFG_EVT
When Scan filter configuration completes, the event comes 

ESP_GATTC_SCAN_FLT_PARAM_EVT

enumerator ESP_GATTC_SCAN_FLT_PARAM_EVT
When Scan filter parameters are set, the event comes 

ESP_GATTC_SCAN_FLT_STATUS_EVT

enumerator ESP_GATTC_SCAN_FLT_STATUS_EVT
When Scan filter status is reported, the event comes 

ESP_GATTC_ADV_VSC_EVT

enumerator ESP_GATTC_ADV_VSC_EVT
When advertising vendor spec content event is reported, the event comes 

ESP_GATTC_REG_FOR_NOTIFY_EVT

enumerator ESP_GATTC_REG_FOR_NOTIFY_EVT
When register for notification of a service completes, the event comes 

ESP_GATTC_UNREG_FOR_NOTIFY_EVT

enumerator ESP_GATTC_UNREG_FOR_NOTIFY_EVT
When unregister for notification of a service completes, the event comes 

ESP_GATTC_CONNECT_EVT

enumerator ESP_GATTC_CONNECT_EVT
When the ble physical connection is set up, the event comes 

ESP_GATTC_DISCONNECT_EVT

enumerator ESP_GATTC_DISCONNECT_EVT
When the ble physical connection disconnected, the event comes 

ESP_GATTC_READ_MULTIPLE_EVT

enumerator ESP_GATTC_READ_MULTIPLE_EVT
When the ble characteristic or descriptor multiple complete, the event comes 

ESP_GATTC_QUEUE_FULL_EVT

enumerator ESP_GATTC_QUEUE_FULL_EVT
When the gattc command queue full, the event comes 

ESP_GATTC_SET_ASSOC_EVT

enumerator ESP_GATTC_SET_ASSOC_EVT
When the ble gattc set the associated address complete, the event comes 

ESP_GATTC_GET_ADDR_LIST_EVT

enumerator ESP_GATTC_GET_ADDR_LIST_EVT
When the ble get gattc address list in cache finish, the event comes 

ESP_GATTC_DIS_SRVC_CMPL_EVT

enumerator ESP_GATTC_DIS_SRVC_CMPL_EVT
When the ble discover service complete, the event comes 

ESP_GATTC_READ_MULTI_VAR_EVT

enumerator ESP_GATTC_READ_MULTI_VAR_EVT
When read multiple variable characteristic complete, the event comes 


```