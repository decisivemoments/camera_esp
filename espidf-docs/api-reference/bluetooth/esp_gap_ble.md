# GAP API




## Application Example


Check the bluetooth/bluedroid/ble folder in ESP-IDF examples, which contains the following demos and their tutorials:


* The following shows an SMP security client demo with its tutorial. This demo initiates its security parameters and acts as a GATT client, which can send a security request to the peer device and then complete the encryption procedure.



> + bluetooth/bluedroid/ble/gatt_security_client
> 	+ GATT Security Client Example Walkthrough
* The following shows an SMP security server demo with its tutorial. This demo initiates its security parameters and acts as a GATT server, which can send a pair request to the peer device and then complete the encryption procedure.



> + bluetooth/bluedroid/ble/gatt_security_server
> 	+ GATT Security Server Example Walkthrough




## API Reference



### Header File


* components/bt/host/bluedroid/api/include/api/esp_gap_ble_api.h
* This header file can be included with:



> ```
> #include "esp_gap_ble_api.h"
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


#### esp_ble_gap_register_callback


```


esp_err_t esp_ble_gap_register_callback(esp_gap_ble_cb_t callback)
This function is called to occur gap event, such as scan result. 

Parameters
callback -- [in] callback function

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_get_callback


```


esp_gap_ble_cb_t esp_ble_gap_get_callback(void)
This function is called to get the current gap callback. 

Returns

esp_gap_ble_cb_t : callback function 





```

#### esp_ble_gap_config_adv_data


```


esp_err_t esp_ble_gap_config_adv_data(esp_ble_adv_data_t *adv_data)
This function is called to override the BTA default ADV parameters. 

Parameters
adv_data -- [in] Pointer to User defined ADV data structure. This memory space can not be freed until callback of config_adv_data is received.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_scan_params


```


esp_err_t esp_ble_gap_set_scan_params(esp_ble_scan_params_t *scan_params)
This function is called to set scan parameters. 

Parameters
scan_params -- [in] Pointer to User defined scan_params data structure. This memory space can not be freed until callback of set_scan_params

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_start_scanning


```


esp_err_t esp_ble_gap_start_scanning(uint32_t duration)
This procedure keep the device scanning the peer device which advertising on the air. 

Parameters
duration -- [in] Keeping the scanning time, the unit is second.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_stop_scanning


```


esp_err_t esp_ble_gap_stop_scanning(void)
This function call to stop the device scanning the peer device which advertising on the air. 

Returns

ESP_OK : success
other : failed 







```

#### esp_ble_gap_start_advertising


```


esp_err_t esp_ble_gap_start_advertising(esp_ble_adv_params_t *adv_params)
This function is called to start advertising. 

Parameters
adv_params -- [in] pointer to User defined adv_params data structure.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_stop_advertising


```


esp_err_t esp_ble_gap_stop_advertising(void)
This function is called to stop advertising. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_update_conn_params


```


esp_err_t esp_ble_gap_update_conn_params(esp_ble_conn_update_params_t *params)
Update connection parameters, can only be used when connection is up. 

Parameters
params -- [in] - connection update parameters

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_pkt_data_len


```


esp_err_t esp_ble_gap_set_pkt_data_len(esp_bd_addr_t remote_device, uint16_t tx_data_length)
This function is to set maximum LE data packet size. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_rand_addr


```


esp_err_t esp_ble_gap_set_rand_addr(esp_bd_addr_t rand_addr)
This function allows configuring either a Non-Resolvable Private Address or a Static Random Address. 

Parameters
rand_addr -- [in] The address to be configured. Refer to the table below for possible address subtypes:           | address [47:46] | Address Type                | Corresponding API                      |
          |-----------------|-----------------------------|----------------------------------------|
          |      0b00       | Non-Resolvable Private      | esp_ble_gap_addr_create_nrpa           |
          |                 | Address (NRPA)              |                                        |
          |-----------------|-----------------------------|----------------------------------------|
          |      0b11       | Static Random Address       | esp_ble_gap_addr_create_static         |
          |-----------------|-----------------------------|----------------------------------------|




Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_addr_create_static


```


esp_err_t esp_ble_gap_addr_create_static(esp_bd_addr_t rand_addr)
Create a static device address. 

Parameters
rand_addr -- [out] Pointer to the buffer where the static device address will be stored. 

Returns
- ESP_OK : Success
Other : Failed 





```

#### esp_ble_gap_addr_create_nrpa


```


esp_err_t esp_ble_gap_addr_create_nrpa(esp_bd_addr_t rand_addr)
Create a non-resolvable private address (NRPA) 

Parameters
rand_addr -- [out] Pointer to the buffer where the NRPA will be stored. 

Returns
- ESP_OK : Success
Other : Failed 





```

#### esp_ble_gap_set_resolvable_private_address_timeout


```


esp_err_t esp_ble_gap_set_resolvable_private_address_timeout(uint16_t rpa_timeout)
This function sets the length of time the Controller uses a Resolvable Private Address before generating and starting to use a new resolvable private address. 

Note
Note: This function is currently not supported on the ESP32 but will be enabled in a future update.


Parameters
rpa_timeout -- [in] The timeout duration in seconds for how long a Resolvable Private Address is used before a new one is generated. The value must be within the range specified by the Bluetooth specification (0x0001 to 0x0E10), which corresponds to a time range of 1 second to 1 hour. The default value is 0x0384 (900 seconds or 15 minutes). 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_add_device_to_resolving_list


```


esp_err_t esp_ble_gap_add_device_to_resolving_list(esp_bd_addr_t peer_addr, uint8_t addr_type, uint8_t *peer_irk)
This function adds a device to the resolving list used to generate and resolve Resolvable Private Addresses in the Controller. 

Note
Note: This function shall not be used when address resolution is enabled in the Controller and:
Advertising (other than periodic advertising) is enabled,
Scanning is enabled, or
an HCI_LE_Create_Connection, HCI_LE_Extended_Create_Connection, or HCI_LE_Periodic_Advertising_Create_Sync command is pending. This command may be used at any time when address resolution is disabled in the Controller. The added device shall be set to Network Privacy mode.




Parameters

peer_addr -- [in] The peer identity address of the device to be added to the resolving list. 
addr_type -- [in] The address type of the peer identity address (BLE_ADDR_TYPE_PUBLIC or BLE_ADDR_TYPE_RANDOM). 
peer_irk -- [in] The Identity Resolving Key (IRK) of the device. 


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_clear_rand_addr


```


esp_err_t esp_ble_gap_clear_rand_addr(void)
This function clears the random address for the application. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_config_local_privacy


```


esp_err_t esp_ble_gap_config_local_privacy(bool privacy_enable)
Enable/disable privacy (including address resolution) on the local device. 

Parameters
privacy_enable -- [in] - enable/disable privacy on remote device.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_config_local_icon


```


esp_err_t esp_ble_gap_config_local_icon(uint16_t icon)
set local gap appearance icon 

Parameters
icon -- [in] - External appearance value, these values are defined by the Bluetooth SIG, please refer to https://www.bluetooth.com/specifications/assigned-numbers/

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_update_whitelist


```


esp_err_t esp_ble_gap_update_whitelist(bool add_remove, esp_bd_addr_t remote_bda, esp_ble_wl_addr_type_t wl_addr_type)
Add or remove device from white list. 

Parameters

add_remove -- [in] the value is true if added the ble device to the white list, and false remove to the white list. 
remote_bda -- [in] the remote device address add/remove from the white list. 
wl_addr_type -- [in] whitelist address type 


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_clear_whitelist


```


esp_err_t esp_ble_gap_clear_whitelist(void)
Clear all white list. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_get_whitelist_size


```


esp_err_t esp_ble_gap_get_whitelist_size(uint16_t *length)
Get the whitelist size in the controller. 

Parameters
length -- [out] the white list length. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_prefer_conn_params


```


esp_err_t esp_ble_gap_set_prefer_conn_params(esp_bd_addr_t bd_addr, uint16_t min_conn_int, uint16_t max_conn_int, uint16_t slave_latency, uint16_t supervision_tout)
This function is called to set the preferred connection parameters when default connection parameter is not desired before connecting. This API can only be used in the master role. 

Parameters

bd_addr -- [in] BD address of the peripheral 
min_conn_int -- [in] minimum preferred connection interval 
max_conn_int -- [in] maximum preferred connection interval 
slave_latency -- [in] preferred slave latency 
supervision_tout -- [in] preferred supervision timeout


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_device_name


```


esp_err_t esp_ble_gap_set_device_name(const char *name)
Set device name to the local device Note: This API don't affect the advertising data. 

Parameters
name -- [in] - device name.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_get_device_name


```


esp_err_t esp_ble_gap_get_device_name(void)
Get device name of the local device. 

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_get_local_used_addr


```


esp_err_t esp_ble_gap_get_local_used_addr(esp_bd_addr_t local_used_addr, uint8_t *addr_type)
This function is called to get local used address and address type. uint8_t *esp_bt_dev_get_address(void) get the public address. 

Parameters

local_used_addr -- [in] - current local used ble address (six bytes) 
addr_type -- [in] - ble address type


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_resolve_adv_data


```


uint8_t *esp_ble_resolve_adv_data(uint8_t *adv_data, uint8_t type, uint8_t *length)
This function is called to get ADV data for a specific type. 

Parameters

adv_data -- [in] - pointer of ADV data which to be resolved 
type -- [in] - finding ADV data type 
length -- [out] - return the length of ADV data not including type


Returns
pointer of ADV data 



```

#### esp_ble_gap_config_adv_data_raw


```


esp_err_t esp_ble_gap_config_adv_data_raw(uint8_t *raw_data, uint32_t raw_data_len)
This function is called to set raw advertising data. User need to fill ADV data by self. 

Parameters

raw_data -- [in] : raw advertising data with the format: [Length 1][Data Type 1][Data 1][Length 2][Data Type 2][Data 2] ... 
raw_data_len -- [in] : raw advertising data length , less than 31 bytes


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_config_scan_rsp_data_raw


```


esp_err_t esp_ble_gap_config_scan_rsp_data_raw(uint8_t *raw_data, uint32_t raw_data_len)
This function is called to set raw scan response data. User need to fill scan response data by self. 

Parameters

raw_data -- [in] : raw scan response data 
raw_data_len -- [in] : raw scan response data length , less than 31 bytes


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_read_rssi


```


esp_err_t esp_ble_gap_read_rssi(esp_bd_addr_t remote_addr)
This function is called to read the RSSI of remote device. The address of link policy results are returned in the gap callback function with ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT event. 

Parameters
remote_addr -- [in] : The remote connection device address.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_add_duplicate_scan_exceptional_device


```


esp_err_t esp_ble_gap_add_duplicate_scan_exceptional_device(esp_ble_duplicate_exceptional_info_type_t type, esp_duplicate_info_t device_info)
This function is called to add a device info into the duplicate scan exceptional list. 

Parameters

type -- [in] device info type, it is defined in esp_ble_duplicate_exceptional_info_type_t when type is MESH_BEACON_TYPE, MESH_PROV_SRV_ADV or MESH_PROXY_SRV_ADV , device_info is invalid. 
device_info -- [in] the device information. 


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_remove_duplicate_scan_exceptional_device


```


esp_err_t esp_ble_gap_remove_duplicate_scan_exceptional_device(esp_ble_duplicate_exceptional_info_type_t type, esp_duplicate_info_t device_info)
This function is called to remove a device info from the duplicate scan exceptional list. 

Parameters

type -- [in] device info type, it is defined in esp_ble_duplicate_exceptional_info_type_t when type is MESH_BEACON_TYPE, MESH_PROV_SRV_ADV or MESH_PROXY_SRV_ADV , device_info is invalid. 
device_info -- [in] the device information. 


Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_clean_duplicate_scan_exceptional_list


```


esp_err_t esp_ble_gap_clean_duplicate_scan_exceptional_list(esp_duplicate_scan_exceptional_list_type_t list_type)
This function is called to clean the duplicate scan exceptional list. This API will delete all device information in the duplicate scan exceptional list. 

Parameters
list_type -- [in] duplicate scan exceptional list type, the value can be one or more of esp_duplicate_scan_exceptional_list_type_t.

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_security_param


```


esp_err_t esp_ble_gap_set_security_param(esp_ble_sm_param_t param_type, void *value, uint8_t len)
Set a GAP security parameter value. Overrides the default value. 
               Secure connection is highly recommended to avoid some major
               vulnerabilities like 'Impersonation in the Pin Pairing Protocol'
               (CVE-2020-26555) and 'Authentication of the LE Legacy Pairing
               Protocol'.

               To accept only `secure connection mode`, it is necessary do as following:

               1. Set bit `ESP_LE_AUTH_REQ_SC_ONLY` (`param_type` is
               `ESP_BLE_SM_AUTHEN_REQ_MODE`), bit `ESP_LE_AUTH_BOND` and bit
               `ESP_LE_AUTH_REQ_MITM` is optional as required.

               2. Set to `ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_ENABLE` (`param_type` is
               `ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH`).




Parameters

param_type -- [in] : the type of the param which to be set 
value -- [in] : the param value 
len -- [in] : the length of the param value


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_security_rsp


```


esp_err_t esp_ble_gap_security_rsp(esp_bd_addr_t bd_addr, bool accept)
Grant security request access. 

Parameters

bd_addr -- [in] : BD address of the peer 
accept -- [in] : accept the security request or not


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_set_encryption


```


esp_err_t esp_ble_set_encryption(esp_bd_addr_t bd_addr, esp_ble_sec_act_t sec_act)
Set a gap parameter value. Use this function to change the default GAP parameter values. 

Parameters

bd_addr -- [in] : the address of the peer device need to encryption 
sec_act -- [in] : This is the security action to indicate what kind of BLE security level is required for the BLE link if the BLE is supported


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_passkey_reply


```


esp_err_t esp_ble_passkey_reply(esp_bd_addr_t bd_addr, bool accept, uint32_t passkey)
Reply the key value to the peer device in the legacy connection stage. 

Parameters

bd_addr -- [in] : BD address of the peer 
accept -- [in] : passkey entry successful or declined. 
passkey -- [in] : passkey value, must be a 6 digit number, can be lead by 0.


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_confirm_reply


```


esp_err_t esp_ble_confirm_reply(esp_bd_addr_t bd_addr, bool accept)
Reply the confirm value to the peer device in the secure connection stage. 

Parameters

bd_addr -- [in] : BD address of the peer device 
accept -- [in] : numbers to compare are the same or different.


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_remove_bond_device


```


esp_err_t esp_ble_remove_bond_device(esp_bd_addr_t bd_addr)
Removes a device from the security database list of peer device. It manages unpairing event while connected. 

Parameters
bd_addr -- [in] : BD address of the peer device

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_get_bond_device_num


```


int esp_ble_get_bond_device_num(void)
Get the device number from the security database list of peer device. It will return the device bonded number immediately. 

Returns
- >= 0 : bonded devices number.
ESP_FAIL : failed 





```

#### esp_ble_get_bond_device_list


```


esp_err_t esp_ble_get_bond_device_list(int *dev_num, esp_ble_bond_dev_t *dev_list)
Get the device from the security database list of peer device. It will return the device bonded information immediately. 

Parameters

dev_num -- [inout] Indicate the dev_list array(buffer) size as input. If dev_num is large enough, it means the actual number as output. Suggest that dev_num value equal to esp_ble_get_bond_device_num().
dev_list -- [out] an array(buffer) of esp_ble_bond_dev_t type. Use for storing the bonded devices address. The dev_list should be allocated by who call this API. 


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_oob_req_reply


```


esp_err_t esp_ble_oob_req_reply(esp_bd_addr_t bd_addr, uint8_t *TK, uint8_t len)
This function is called to provide the OOB data for SMP in response to ESP_GAP_BLE_OOB_REQ_EVT. 

Parameters

bd_addr -- [in] BD address of the peer device. 
TK -- [in] Temporary Key value, the TK value shall be a 128-bit random number 
len -- [in] length of temporary key, should always be 128-bit


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_sc_oob_req_reply


```


esp_err_t esp_ble_sc_oob_req_reply(esp_bd_addr_t bd_addr, uint8_t p_c[16], uint8_t p_r[16])
This function is called to provide the OOB data for SMP in response to ESP_GAP_BLE_SC_OOB_REQ_EVT. 

Parameters

bd_addr -- [in] BD address of the peer device. 
p_c -- [in] Confirmation value, it shall be a 128-bit random number 
p_r -- [in] Randomizer value, it should be a 128-bit random number


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_create_sc_oob_data


```


esp_err_t esp_ble_create_sc_oob_data(void)
This function is called to create the OOB data for SMP when secure connection. 

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_disconnect


```


esp_err_t esp_ble_gap_disconnect(esp_bd_addr_t remote_device)
This function is to disconnect the physical connection of the peer device gattc may have multiple virtual GATT server connections when multiple app_id registered. esp_ble_gattc_close (esp_gatt_if_t gattc_if, uint16_t conn_id) only close one virtual GATT server connection. if there exist other virtual GATT server connections, it does not disconnect the physical connection. esp_ble_gap_disconnect(esp_bd_addr_t remote_device) disconnect the physical connection directly. 

Parameters
remote_device -- [in] : BD address of the peer device

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_get_current_conn_params


```


esp_err_t esp_ble_get_current_conn_params(esp_bd_addr_t bd_addr, esp_gap_conn_params_t *conn_params)
This function is called to read the connection parameters information of the device. 

Parameters

bd_addr -- [in] BD address of the peer device. 
conn_params -- [out] the connection parameters information


Returns
- ESP_OK : success
other : failed 





```

#### esp_gap_ble_set_channels


```


esp_err_t esp_gap_ble_set_channels(esp_gap_ble_channels channels)
BLE set channels. 

Parameters
channels -- [in] : The n th such field (in the range 0 to 36) contains the value for the link layer channel index n. 0 means channel n is bad. 1 means channel n is unknown. The most significant bits are reserved and shall be set to 0. At least one channel shall be marked as unknown.

Returns
- ESP_OK : success
ESP_ERR_INVALID_STATE: if bluetooth stack is not yet enabled
other : failed 





```

#### esp_gap_ble_set_authorization


```


esp_err_t esp_gap_ble_set_authorization(esp_bd_addr_t bd_addr, bool authorize)
This function is called to authorized a link after Authentication(MITM protection) 

Parameters

bd_addr -- [in] BD address of the peer device. 
authorize -- [out] Authorized the link or not.


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_read_phy


```


esp_err_t esp_ble_gap_read_phy(esp_bd_addr_t bd_addr)
This function is used to read the current transmitter PHY and receiver PHY on the connection identified by remote address. 

Parameters
bd_addr -- [in] : BD address of the peer device

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_preferred_default_phy


```


esp_err_t esp_ble_gap_set_preferred_default_phy(esp_ble_gap_phy_mask_t tx_phy_mask, esp_ble_gap_phy_mask_t rx_phy_mask)
This function is used to allows the Host to specify its preferred values for the transmitter PHY and receiver PHY to be used for all subsequent connections over the LE transport. 

Parameters

tx_phy_mask -- [in] : indicates the transmitter PHYs that the Host prefers the Controller to use 
rx_phy_mask -- [in] : indicates the receiver PHYs that the Host prefers the Controller to use


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_preferred_phy


```


esp_err_t esp_ble_gap_set_preferred_phy(esp_bd_addr_t bd_addr, esp_ble_gap_all_phys_t all_phys_mask, esp_ble_gap_phy_mask_t tx_phy_mask, esp_ble_gap_phy_mask_t rx_phy_mask, esp_ble_gap_prefer_phy_options_t phy_options)
This function is used to set the PHY preferences for the connection identified by the remote address. The Controller might not be able to make the change (e.g. because the peer does not support the requested PHY) or may decide that the current PHY is preferable. 

Parameters

bd_addr -- [in] : remote address 
all_phys_mask -- [in] : a bit field that allows the Host to specify 
tx_phy_mask -- [in] : a bit field that indicates the transmitter PHYs that the Host prefers the Controller to use 
rx_phy_mask -- [in] : a bit field that indicates the receiver PHYs that the Host prefers the Controller to use 
phy_options -- [in] : a bit field that allows the Host to specify options for PHYs


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_ext_adv_set_rand_addr


```


esp_err_t esp_ble_gap_ext_adv_set_rand_addr(uint8_t instance, esp_bd_addr_t rand_addr)
This function is used by the Host to set the random device address specified by the Random_Address parameter. 

Parameters

instance -- [in] : Used to identify an advertising set 
rand_addr -- [in] : Random Device Address


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_ext_adv_set_params


```


esp_err_t esp_ble_gap_ext_adv_set_params(uint8_t instance, const esp_ble_gap_ext_adv_params_t *params)
This function is used by the Host to set the advertising parameters. 

Parameters

instance -- [in] : identifies the advertising set whose parameters are being configured. 
params -- [in] : advertising parameters


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_config_ext_adv_data_raw


```


esp_err_t esp_ble_gap_config_ext_adv_data_raw(uint8_t instance, uint16_t length, const uint8_t *data)
This function is used to set the data used in advertising PDUs that have a data field. 

Parameters

instance -- [in] : identifies the advertising set whose data are being configured 
length -- [in] : data length 
data -- [in] : data information


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_config_ext_scan_rsp_data_raw


```


esp_err_t esp_ble_gap_config_ext_scan_rsp_data_raw(uint8_t instance, uint16_t length, const uint8_t *scan_rsp_data)
This function is used to provide scan response data used in scanning response PDUs. 

Parameters

instance -- [in] : identifies the advertising set whose response data are being configured. 
length -- [in] : responsedata length 
scan_rsp_data -- [in] : response data information


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_ext_adv_start


```


esp_err_t esp_ble_gap_ext_adv_start(uint8_t num_adv, const esp_ble_gap_ext_adv_t *ext_adv)
This function is used to request the Controller to enable one or more advertising sets using the advertising sets identified by the instance parameter. 

Parameters

num_adv -- [in] : Number of advertising sets to enable or disable 
ext_adv -- [in] : adv parameters


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_ext_adv_stop


```


esp_err_t esp_ble_gap_ext_adv_stop(uint8_t num_adv, const uint8_t *ext_adv_inst)
This function is used to request the Controller to disable one or more advertising sets using the advertising sets identified by the instance parameter. 

Parameters

num_adv -- [in] : Number of advertising sets to enable or disable 
ext_adv_inst -- [in] : ext adv instance


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_ext_adv_set_remove


```


esp_err_t esp_ble_gap_ext_adv_set_remove(uint8_t instance)
This function is used to remove an advertising set from the Controller. 

Parameters
instance -- [in] : Used to identify an advertising set

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_ext_adv_set_clear


```


esp_err_t esp_ble_gap_ext_adv_set_clear(void)
This function is used to remove all existing advertising sets from the Controller. 

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_set_params


```


esp_err_t esp_ble_gap_periodic_adv_set_params(uint8_t instance, const esp_ble_gap_periodic_adv_params_t *params)
This function is used by the Host to set the parameters for periodic advertising. 

Parameters

instance -- [in] : identifies the advertising set whose periodic advertising parameters are being configured. 
params -- [in] : periodic adv parameters


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_config_periodic_adv_data_raw


```


esp_err_t esp_ble_gap_config_periodic_adv_data_raw(uint8_t instance, uint16_t length, const uint8_t *data)
This function is used to set the data used in periodic advertising PDUs. 

Parameters

instance -- [in] : identifies the advertising set whose periodic advertising parameters are being configured. 
length -- [in] : the length of periodic data 
data -- [in] : periodic data information


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_start


```


esp_err_t esp_ble_gap_periodic_adv_start(uint8_t instance)
This function is used to request the Controller to enable the periodic advertising for the advertising set specified. 

Parameters
instance -- [in] : Used to identify an advertising set

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_stop


```


esp_err_t esp_ble_gap_periodic_adv_stop(uint8_t instance)
This function is used to request the Controller to disable the periodic advertising for the advertising set specified. 

Parameters
instance -- [in] : Used to identify an advertising set

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_ext_scan_params


```


esp_err_t esp_ble_gap_set_ext_scan_params(const esp_ble_ext_scan_params_t *params)
This function is used to set the extended scan parameters to be used on the advertising channels. 

Parameters
params -- [in] : scan parameters

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_start_ext_scan


```


esp_err_t esp_ble_gap_start_ext_scan(uint32_t duration, uint16_t period)
This function is used to enable scanning. 

Parameters

duration -- [in] Scan duration time, where Time = N * 10 ms. Range: 0x0001 to 0xFFFF. 
period -- [in] Time interval from when the Controller started its last Scan Duration until it begins the subsequent Scan Duration. Time = N * 1.28 sec. Range: 0x0001 to 0xFFFF.


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_stop_ext_scan


```


esp_err_t esp_ble_gap_stop_ext_scan(void)
This function is used to disable scanning. 

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_create_sync


```


esp_err_t esp_ble_gap_periodic_adv_create_sync(const esp_ble_gap_periodic_adv_sync_params_t *params)
This function is used to synchronize with periodic advertising from an advertiser and begin receiving periodic advertising packets. 

Parameters
params -- [in] : sync parameters

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_sync_cancel


```


esp_err_t esp_ble_gap_periodic_adv_sync_cancel(void)
This function is used to cancel the LE_Periodic_Advertising_Create_Sync command while it is pending. 

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_sync_terminate


```


esp_err_t esp_ble_gap_periodic_adv_sync_terminate(uint16_t sync_handle)
This function is used to stop reception of the periodic advertising identified by the Sync Handle parameter. 

Parameters
sync_handle -- [in] : identify the periodic advertiser

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_add_dev_to_list


```


esp_err_t esp_ble_gap_periodic_adv_add_dev_to_list(esp_ble_addr_type_t addr_type, esp_bd_addr_t addr, uint8_t sid)
This function is used to add a single device to the Periodic Advertiser list stored in the Controller. 

Parameters

addr_type -- [in] : address type 
addr -- [in] : Device Address 
sid -- [in] : Advertising SID subfield in the ADI field used to identify the Periodic Advertising


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_remove_dev_from_list


```


esp_err_t esp_ble_gap_periodic_adv_remove_dev_from_list(esp_ble_addr_type_t addr_type, esp_bd_addr_t addr, uint8_t sid)
This function is used to remove one device from the list of Periodic Advertisers stored in the Controller. Removals from the Periodic Advertisers List take effect immediately. 

Parameters

addr_type -- [in] : address type 
addr -- [in] : Device Address 
sid -- [in] : Advertising SID subfield in the ADI field used to identify the Periodic Advertising


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_clear_dev


```


esp_err_t esp_ble_gap_periodic_adv_clear_dev(void)
This function is used to remove all devices from the list of Periodic Advertisers in the Controller. 

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_prefer_ext_connect_params_set


```


esp_err_t esp_ble_gap_prefer_ext_connect_params_set(esp_bd_addr_t addr, esp_ble_gap_phy_mask_t phy_mask, const esp_ble_gap_conn_params_t *phy_1m_conn_params, const esp_ble_gap_conn_params_t *phy_2m_conn_params, const esp_ble_gap_conn_params_t *phy_coded_conn_params)
This function is used to set aux connection parameters. 

Parameters

addr -- [in] : device address 
phy_mask -- [in] : indicates the PHY(s) on which the advertising packets should be received on the primary advertising channel and the PHYs for which connection parameters have been specified. 
phy_1m_conn_params -- [in] : Scan connectable advertisements on the LE 1M PHY. Connection parameters for the LE 1M PHY are provided. 
phy_2m_conn_params -- [in] : Connection parameters for the LE 2M PHY are provided. 
phy_coded_conn_params -- [in] : Scan connectable advertisements on the LE Coded PHY. Connection parameters for the LE Coded PHY are provided.


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_recv_enable


```


esp_err_t esp_ble_gap_periodic_adv_recv_enable(uint16_t sync_handle, uint8_t enable)
This function is used to set periodic advertising receive enable. 

Parameters

sync_handle -- [in] : Handle of periodic advertising sync 
enable -- [in] : Determines whether reporting and duplicate filtering are enabled or disabled


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_sync_trans


```


esp_err_t esp_ble_gap_periodic_adv_sync_trans(esp_bd_addr_t addr, uint16_t service_data, uint16_t sync_handle)
This function is used to transfer periodic advertising sync. 

Parameters

addr -- [in] : Peer device address 
service_data -- [in] : Service data used by Host 
sync_handle -- [in] : Handle of periodic advertising sync


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_periodic_adv_set_info_trans


```


esp_err_t esp_ble_gap_periodic_adv_set_info_trans(esp_bd_addr_t addr, uint16_t service_data, uint8_t adv_handle)
This function is used to transfer periodic advertising set info. 

Parameters

addr -- [in] : Peer device address 
service_data -- [in] : Service data used by Host 
adv_handle -- [in] : Handle of advertising set


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_periodic_adv_sync_trans_params


```


esp_err_t esp_ble_gap_set_periodic_adv_sync_trans_params(esp_bd_addr_t addr, const esp_ble_gap_past_params_t *params)
This function is used to set periodic advertising sync transfer params. 

Parameters

addr -- [in] : Peer device address 
params -- [in] : Params of periodic advertising sync transfer


Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_dtm_tx_start


```


esp_err_t esp_ble_dtm_tx_start(const esp_ble_dtm_tx_t *tx_params)
This function is used to start a test where the DUT generates reference packets at a fixed interval. 

Parameters
tx_params -- [in] : DTM Transmitter parameters

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_dtm_rx_start


```


esp_err_t esp_ble_dtm_rx_start(const esp_ble_dtm_rx_t *rx_params)
This function is used to start a test where the DUT receives test reference packets at a fixed interval. 

Parameters
rx_params -- [in] : DTM Receiver parameters

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_dtm_enh_tx_start


```


esp_err_t esp_ble_dtm_enh_tx_start(const esp_ble_dtm_enh_tx_t *tx_params)
This function is used to start a test where the DUT generates reference packets at a fixed interval. 

Parameters
tx_params -- [in] : DTM Transmitter parameters

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_dtm_enh_rx_start


```


esp_err_t esp_ble_dtm_enh_rx_start(const esp_ble_dtm_enh_rx_t *rx_params)
This function is used to start a test where the DUT receives test reference packets at a fixed interval. 

Parameters
rx_params -- [in] : DTM Receiver parameters

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_dtm_stop


```


esp_err_t esp_ble_dtm_stop(void)
This function is used to stop any test which is in progress. 

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_clear_advertising


```


esp_err_t esp_ble_gap_clear_advertising(void)
This function is used to clear legacy advertising. 

Returns
- ESP_OK : success
other : failed 





```

#### esp_ble_gap_vendor_command_send


```


esp_err_t esp_ble_gap_vendor_command_send(esp_ble_vendor_cmd_params_t *vendor_cmd_param)
This function is called to send vendor hci command. 

Parameters
vendor_cmd_param -- [in] vendor hci command parameters

Returns

ESP_OK : success
other : failed 





```

#### esp_ble_gap_set_privacy_mode


```


esp_err_t esp_ble_gap_set_privacy_mode(esp_ble_addr_type_t addr_type, esp_bd_addr_t addr, esp_ble_privacy_mode_t mode)
This function set the privacy mode of the device in resolving list. 

Note
This feature is not supported on ESP32.


Parameters

addr_type -- [in] The address type of the peer identity address (BLE_ADDR_TYPE_PUBLIC or BLE_ADDR_TYPE_RANDOM). 
addr -- [in] The peer identity address of the device. 
mode -- [in] The privacy mode of the device.


Returns

ESP_OK : success
other : failed 





```



### Unions


#### esp_ble_key_value_t


```


union esp_ble_key_value_t

#include <esp_gap_ble_api.h>
union type of the security key value 

Public Members
penc_key

esp_ble_penc_keys_t penc_key
received peer encryption key 

pcsrk_key

esp_ble_pcsrk_keys_t pcsrk_key
received peer device SRK 

pid_key

esp_ble_pid_keys_t pid_key
peer device ID key 

lenc_key

esp_ble_lenc_keys_t lenc_key
local encryption reproduction keys LTK = = d1(ER,DIV,0) 

lcsrk_key

esp_ble_lcsrk_keys lcsrk_key
local device CSRK = d1(ER,DIV,1) 



```

#### esp_ble_sec_t


```


union esp_ble_sec_t

#include <esp_gap_ble_api.h>
union associated with ble security 

Public Members
key_notif

esp_ble_sec_key_notif_t key_notif
passkey notification 

ble_req

esp_ble_sec_req_t ble_req
BLE SMP related request 

ble_key

esp_ble_key_t ble_key
BLE SMP keys used when pairing 

ble_id_keys

esp_ble_local_id_keys_t ble_id_keys
BLE IR event 

oob_data

esp_ble_local_oob_data_t oob_data
BLE SMP secure connection OOB data 

auth_cmpl

esp_ble_auth_cmpl_t auth_cmpl
Authentication complete indication. 



```

#### esp_ble_gap_cb_param_t


```


union esp_ble_gap_cb_param_t

#include <esp_gap_ble_api.h>
Gap callback parameters union. 

Public Members
get_dev_name_cmpl

struct esp_ble_gap_cb_param_t::ble_get_dev_name_cmpl_evt_param get_dev_name_cmpl
Event parameter of ESP_GAP_BLE_GET_DEV_NAME_COMPLETE_EVT 

adv_data_cmpl

struct esp_ble_gap_cb_param_t::ble_adv_data_cmpl_evt_param adv_data_cmpl
Event parameter of ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT 

scan_rsp_data_cmpl

struct esp_ble_gap_cb_param_t::ble_scan_rsp_data_cmpl_evt_param scan_rsp_data_cmpl
Event parameter of ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT 

scan_param_cmpl

struct esp_ble_gap_cb_param_t::ble_scan_param_cmpl_evt_param scan_param_cmpl
Event parameter of ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT 

scan_rst

struct esp_ble_gap_cb_param_t::ble_scan_result_evt_param scan_rst
Event parameter of ESP_GAP_BLE_SCAN_RESULT_EVT 

adv_data_raw_cmpl

struct esp_ble_gap_cb_param_t::ble_adv_data_raw_cmpl_evt_param adv_data_raw_cmpl
Event parameter of ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT 

scan_rsp_data_raw_cmpl

struct esp_ble_gap_cb_param_t::ble_scan_rsp_data_raw_cmpl_evt_param scan_rsp_data_raw_cmpl
Event parameter of ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT 

adv_start_cmpl

struct esp_ble_gap_cb_param_t::ble_adv_start_cmpl_evt_param adv_start_cmpl
Event parameter of ESP_GAP_BLE_ADV_START_COMPLETE_EVT 

scan_start_cmpl

struct esp_ble_gap_cb_param_t::ble_scan_start_cmpl_evt_param scan_start_cmpl
Event parameter of ESP_GAP_BLE_SCAN_START_COMPLETE_EVT 

ble_security

esp_ble_sec_t ble_security
ble gap security union type 

scan_stop_cmpl

struct esp_ble_gap_cb_param_t::ble_scan_stop_cmpl_evt_param scan_stop_cmpl
Event parameter of ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT 

adv_stop_cmpl

struct esp_ble_gap_cb_param_t::ble_adv_stop_cmpl_evt_param adv_stop_cmpl
Event parameter of ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT 

adv_clear_cmpl

struct esp_ble_gap_cb_param_t::ble_adv_clear_cmpl_evt_param adv_clear_cmpl
Event parameter of ESP_GAP_BLE_ADV_CLEAR_COMPLETE_EVT 

set_rand_addr_cmpl

struct esp_ble_gap_cb_param_t::ble_set_rand_cmpl_evt_param set_rand_addr_cmpl
Event parameter of ESP_GAP_BLE_SET_STATIC_RAND_ADDR_EVT 

update_conn_params

struct esp_ble_gap_cb_param_t::ble_update_conn_params_evt_param update_conn_params
Event parameter of ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT 

pkt_data_length_cmpl

struct esp_ble_gap_cb_param_t::ble_pkt_data_length_cmpl_evt_param pkt_data_length_cmpl
Event parameter of ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT 

local_privacy_cmpl

struct esp_ble_gap_cb_param_t::ble_local_privacy_cmpl_evt_param local_privacy_cmpl
Event parameter of ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT 

set_rpa_timeout_cmpl

struct esp_ble_gap_cb_param_t::ble_rpa_timeout_cmpl_evt_param set_rpa_timeout_cmpl
Event parameter of ESP_GAP_BLE_SET_RPA_TIMEOUT_COMPLETE_EVT 

add_dev_to_resolving_list_cmpl

struct esp_ble_gap_cb_param_t::ble_add_dev_to_resolving_list_cmpl_evt_param add_dev_to_resolving_list_cmpl
Event parameter of ESP_GAP_BLE_ADD_DEV_TO_RESOLVING_LIST_COMPLETE_EVT 

remove_bond_dev_cmpl

struct esp_ble_gap_cb_param_t::ble_remove_bond_dev_cmpl_evt_param remove_bond_dev_cmpl
Event parameter of ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT 

clear_bond_dev_cmpl

struct esp_ble_gap_cb_param_t::ble_clear_bond_dev_cmpl_evt_param clear_bond_dev_cmpl
Event parameter of ESP_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT 

get_bond_dev_cmpl

struct esp_ble_gap_cb_param_t::ble_get_bond_dev_cmpl_evt_param get_bond_dev_cmpl
Event parameter of ESP_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT 

read_rssi_cmpl

struct esp_ble_gap_cb_param_t::ble_read_rssi_cmpl_evt_param read_rssi_cmpl
Event parameter of ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT 

update_whitelist_cmpl

struct esp_ble_gap_cb_param_t::ble_update_whitelist_cmpl_evt_param update_whitelist_cmpl
Event parameter of ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT 

update_duplicate_exceptional_list_cmpl

struct esp_ble_gap_cb_param_t::ble_update_duplicate_exceptional_list_cmpl_evt_param update_duplicate_exceptional_list_cmpl
Event parameter of ESP_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT 

ble_set_channels

struct esp_ble_gap_cb_param_t::ble_set_channels_evt_param ble_set_channels
Event parameter of ESP_GAP_BLE_SET_CHANNELS_EVT 

read_phy

struct esp_ble_gap_cb_param_t::ble_read_phy_cmpl_evt_param read_phy
Event parameter of ESP_GAP_BLE_READ_PHY_COMPLETE_EVT 

set_perf_def_phy

struct esp_ble_gap_cb_param_t::ble_set_perf_def_phy_cmpl_evt_param set_perf_def_phy
Event parameter of ESP_GAP_BLE_SET_PREFERRED_DEFAULT_PHY_COMPLETE_EVT 

set_perf_phy

struct esp_ble_gap_cb_param_t::ble_set_perf_phy_cmpl_evt_param set_perf_phy
Event parameter of ESP_GAP_BLE_SET_PREFERRED_PHY_COMPLETE_EVT 

ext_adv_set_rand_addr

struct esp_ble_gap_cb_param_t::ble_ext_adv_set_rand_addr_cmpl_evt_param ext_adv_set_rand_addr
Event parameter of ESP_GAP_BLE_EXT_ADV_SET_RAND_ADDR_COMPLETE_EVT 

ext_adv_set_params

struct esp_ble_gap_cb_param_t::ble_ext_adv_set_params_cmpl_evt_param ext_adv_set_params
Event parameter of ESP_GAP_BLE_EXT_ADV_SET_PARAMS_COMPLETE_EVT 

ext_adv_data_set

struct esp_ble_gap_cb_param_t::ble_ext_adv_data_set_cmpl_evt_param ext_adv_data_set
Event parameter of ESP_GAP_BLE_EXT_ADV_DATA_SET_COMPLETE_EVT 

scan_rsp_set

struct esp_ble_gap_cb_param_t::ble_ext_adv_scan_rsp_set_cmpl_evt_param scan_rsp_set
Event parameter of ESP_GAP_BLE_EXT_SCAN_RSP_DATA_SET_COMPLETE_EVT 

ext_adv_start

struct esp_ble_gap_cb_param_t::ble_ext_adv_start_cmpl_evt_param ext_adv_start
Event parameter of ESP_GAP_BLE_EXT_ADV_START_COMPLETE_EVT 

ext_adv_stop

struct esp_ble_gap_cb_param_t::ble_ext_adv_stop_cmpl_evt_param ext_adv_stop
Event parameter of ESP_GAP_BLE_EXT_ADV_STOP_COMPLETE_EVT 

ext_adv_remove

struct esp_ble_gap_cb_param_t::ble_ext_adv_set_remove_cmpl_evt_param ext_adv_remove
Event parameter of ESP_GAP_BLE_EXT_ADV_SET_REMOVE_COMPLETE_EVT 

ext_adv_clear

struct esp_ble_gap_cb_param_t::ble_ext_adv_set_clear_cmpl_evt_param ext_adv_clear
Event parameter of ESP_GAP_BLE_EXT_ADV_SET_CLEAR_COMPLETE_EVT 

peroid_adv_set_params

struct esp_ble_gap_cb_param_t::ble_periodic_adv_set_params_cmpl_param peroid_adv_set_params
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SET_PARAMS_COMPLETE_EVT 

period_adv_data_set

struct esp_ble_gap_cb_param_t::ble_periodic_adv_data_set_cmpl_param period_adv_data_set
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_DATA_SET_COMPLETE_EVT 

period_adv_start

struct esp_ble_gap_cb_param_t::ble_periodic_adv_start_cmpl_param period_adv_start
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_START_COMPLETE_EVT 

period_adv_stop

struct esp_ble_gap_cb_param_t::ble_periodic_adv_stop_cmpl_param period_adv_stop
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_STOP_COMPLETE_EVT 

period_adv_create_sync

struct esp_ble_gap_cb_param_t::ble_period_adv_create_sync_cmpl_param period_adv_create_sync
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_CREATE_SYNC_COMPLETE_EVT 

period_adv_sync_cancel

struct esp_ble_gap_cb_param_t::ble_period_adv_sync_cancel_cmpl_param period_adv_sync_cancel
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SYNC_CANCEL_COMPLETE_EVT 

period_adv_sync_term

struct esp_ble_gap_cb_param_t::ble_period_adv_sync_terminate_cmpl_param period_adv_sync_term
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SYNC_TERMINATE_COMPLETE_EVT 

period_adv_add_dev

struct esp_ble_gap_cb_param_t::ble_period_adv_add_dev_cmpl_param period_adv_add_dev
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_ADD_DEV_COMPLETE_EVT 

period_adv_remove_dev

struct esp_ble_gap_cb_param_t::ble_period_adv_remove_dev_cmpl_param period_adv_remove_dev
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_REMOVE_DEV_COMPLETE_EVT 

period_adv_clear_dev

struct esp_ble_gap_cb_param_t::ble_period_adv_clear_dev_cmpl_param period_adv_clear_dev
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_CLEAR_DEV_COMPLETE_EVT 

set_ext_scan_params

struct esp_ble_gap_cb_param_t::ble_set_ext_scan_params_cmpl_param set_ext_scan_params
Event parameter of ESP_GAP_BLE_SET_EXT_SCAN_PARAMS_COMPLETE_EVT 

ext_scan_start

struct esp_ble_gap_cb_param_t::ble_ext_scan_start_cmpl_param ext_scan_start
Event parameter of ESP_GAP_BLE_EXT_SCAN_START_COMPLETE_EVT 

ext_scan_stop

struct esp_ble_gap_cb_param_t::ble_ext_scan_stop_cmpl_param ext_scan_stop
Event parameter of ESP_GAP_BLE_EXT_SCAN_STOP_COMPLETE_EVT 

ext_conn_params_set

struct esp_ble_gap_cb_param_t::ble_ext_conn_params_set_cmpl_param ext_conn_params_set
Event parameter of ESP_GAP_BLE_PREFER_EXT_CONN_PARAMS_SET_COMPLETE_EVT 

adv_terminate

struct esp_ble_gap_cb_param_t::ble_adv_terminate_param adv_terminate
Event parameter of ESP_GAP_BLE_ADV_TERMINATED_EVT 

scan_req_received

struct esp_ble_gap_cb_param_t::ble_scan_req_received_param scan_req_received
Event parameter of ESP_GAP_BLE_SCAN_REQ_RECEIVED_EVT 

channel_sel_alg

struct esp_ble_gap_cb_param_t::ble_channel_sel_alg_param channel_sel_alg
Event parameter of ESP_GAP_BLE_CHANNEL_SELECT_ALGORITHM_EVT 

periodic_adv_sync_lost

struct esp_ble_gap_cb_param_t::ble_periodic_adv_sync_lost_param periodic_adv_sync_lost
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SYNC_LOST_EVT 

periodic_adv_sync_estab

struct esp_ble_gap_cb_param_t::ble_periodic_adv_sync_estab_param periodic_adv_sync_estab
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SYNC_ESTAB_EVT 

phy_update

struct esp_ble_gap_cb_param_t::ble_phy_update_cmpl_param phy_update
Event parameter of ESP_GAP_BLE_PHY_UPDATE_COMPLETE_EVT 

ext_adv_report

struct esp_ble_gap_cb_param_t::ble_ext_adv_report_param ext_adv_report
Event parameter of ESP_GAP_BLE_EXT_ADV_REPORT_EVT 

period_adv_report

struct esp_ble_gap_cb_param_t::ble_periodic_adv_report_param period_adv_report
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_REPORT_EVT 

period_adv_recv_enable

struct esp_ble_gap_cb_param_t::ble_periodic_adv_recv_enable_cmpl_param period_adv_recv_enable
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_RECV_ENABLE_COMPLETE_EVT 

period_adv_sync_trans

struct esp_ble_gap_cb_param_t::ble_periodic_adv_sync_trans_cmpl_param period_adv_sync_trans
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_COMPLETE_EVT 

period_adv_set_info_trans

struct esp_ble_gap_cb_param_t::ble_periodic_adv_set_info_trans_cmpl_param period_adv_set_info_trans
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SET_INFO_TRANS_COMPLETE_EVT 

set_past_params

struct esp_ble_gap_cb_param_t::ble_set_past_params_cmpl_param set_past_params
Event parameter of ESP_GAP_BLE_SET_PAST_PARAMS_COMPLETE_EVT 

past_received

struct esp_ble_gap_cb_param_t::ble_periodic_adv_sync_trans_recv_param past_received
Event parameter of ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_RECV_EVT 

dtm_state_update

struct esp_ble_gap_cb_param_t::ble_dtm_state_update_evt_param dtm_state_update
Event parameter of ESP_GAP_BLE_DTM_TEST_UPDATE_EVT 

vendor_cmd_cmpl

struct esp_ble_gap_cb_param_t::vendor_cmd_cmpl_evt_param vendor_cmd_cmpl
Event parameter of ESP_GAP_BLE_VENDOR_CMD_COMPLETE_EVT 

set_privacy_mode_cmpl

struct esp_ble_gap_cb_param_t::ble_set_privacy_mode_cmpl_evt_param set_privacy_mode_cmpl
Event parameter of ESP_GAP_BLE_SET_PRIVACY_MODE_COMPLETE_EVT 


ble_add_dev_to_resolving_list_cmpl_evt_param

struct ble_add_dev_to_resolving_list_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_ADD_DEV_TO_RESOLVING_LIST_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicates the success status of adding a device to the resolving list 



ble_adv_clear_cmpl_evt_param

struct ble_adv_clear_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_ADV_CLEAR_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate adv clear operation success status 



ble_adv_data_cmpl_evt_param

struct ble_adv_data_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set advertising data operation success status 



ble_adv_data_raw_cmpl_evt_param

struct ble_adv_data_raw_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set raw advertising data operation success status 



ble_adv_start_cmpl_evt_param

struct ble_adv_start_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_ADV_START_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate advertising start operation success status 



ble_adv_stop_cmpl_evt_param

struct ble_adv_stop_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate adv stop operation success status 



ble_adv_terminate_param

struct ble_adv_terminate_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_ADV_TERMINATED_EVT. 

Public Members
status

uint8_t status
Indicate adv terminate status 

adv_instance

uint8_t adv_instance
extend advertising handle 

conn_idx

uint16_t conn_idx
connection index 

completed_event

uint8_t completed_event
the number of completed extend advertising events 



ble_channel_sel_alg_param

struct ble_channel_sel_alg_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_CHANNEL_SELECT_ALGORITHM_EVT. 

Public Members
conn_handle

uint16_t conn_handle
connection handle 

channel_sel_alg

uint8_t channel_sel_alg
channel selection algorithm 



ble_clear_bond_dev_cmpl_evt_param

struct ble_clear_bond_dev_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the clear bond device operation success status 



ble_dtm_state_update_evt_param

struct ble_dtm_state_update_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_DTM_TEST_UPDATE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate DTM operation success status 

update_evt

esp_ble_dtm_update_evt_t update_evt
DTM state change event, 0x00: DTM TX start, 0x01: DTM RX start, 0x02:DTM end 

num_of_pkt

uint16_t num_of_pkt
number of packets received, only valid if update_evt is DTM_TEST_STOP_EVT and shall be reported as 0 for a transmitter 



ble_ext_adv_data_set_cmpl_evt_param

struct ble_ext_adv_data_set_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_DATA_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend advertising data set status 

instance

uint8_t instance
extend advertising handle 



ble_ext_adv_report_param

struct ble_ext_adv_report_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_REPORT_EVT. 

Public Members
params

esp_ble_gap_ext_adv_report_t params
extend advertising report parameters 



ble_ext_adv_scan_rsp_set_cmpl_evt_param

struct ble_ext_adv_scan_rsp_set_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_SCAN_RSP_DATA_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend advertising scan response data set status 

instance

uint8_t instance
extend advertising handle 



ble_ext_adv_set_clear_cmpl_evt_param

struct ble_ext_adv_set_clear_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_SET_CLEAR_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate advertising stop operation success status 

instance

uint8_t instance
extend advertising handle 



ble_ext_adv_set_params_cmpl_evt_param

struct ble_ext_adv_set_params_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_SET_PARAMS_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend advertising parameters set status 

instance

uint8_t instance
extend advertising handle 



ble_ext_adv_set_rand_addr_cmpl_evt_param

struct ble_ext_adv_set_rand_addr_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_SET_RAND_ADDR_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend advertising random address set status 

instance

uint8_t instance
extend advertising handle 



ble_ext_adv_set_remove_cmpl_evt_param

struct ble_ext_adv_set_remove_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_SET_REMOVE_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate advertising stop operation success status 

instance

uint8_t instance
extend advertising handle 



ble_ext_adv_start_cmpl_evt_param

struct ble_ext_adv_start_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_START_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate advertising start operation success status 

instance_num

uint8_t instance_num
extend advertising handle numble 

instance

uint8_t instance[EXT_ADV_NUM_SETS_MAX]
extend advertising handle list 



ble_ext_adv_stop_cmpl_evt_param

struct ble_ext_adv_stop_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_ADV_STOP_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate advertising stop operation success status 

instance_num

uint8_t instance_num
extend advertising handle numble 

instance

uint8_t instance[EXT_ADV_NUM_SETS_MAX]
extend advertising handle list 



ble_ext_conn_params_set_cmpl_param

struct ble_ext_conn_params_set_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PREFER_EXT_CONN_PARAMS_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend connection parameters set status 



ble_ext_scan_start_cmpl_param

struct ble_ext_scan_start_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_SCAN_START_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend advertising start status 



ble_ext_scan_stop_cmpl_param

struct ble_ext_scan_stop_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_EXT_SCAN_STOP_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend advertising stop status 



ble_get_bond_dev_cmpl_evt_param

struct ble_get_bond_dev_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the get bond device operation success status 

dev_num

uint8_t dev_num
Indicate the get number device in the bond list 

bond_dev

esp_ble_bond_dev_t *bond_dev
the pointer to the bond device Structure 



ble_get_dev_name_cmpl_evt_param

struct ble_get_dev_name_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_GET_DEV_NAME_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the get device name success status 

name

char *name
Name of bluetooth device 



ble_local_privacy_cmpl_evt_param

struct ble_local_privacy_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set local privacy operation success status 



ble_period_adv_add_dev_cmpl_param

struct ble_period_adv_add_dev_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_ADD_DEV_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising device list add status 



ble_period_adv_clear_dev_cmpl_param

struct ble_period_adv_clear_dev_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_CLEAR_DEV_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising device list clean status 



ble_period_adv_create_sync_cmpl_param

struct ble_period_adv_create_sync_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_CREATE_SYNC_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising create sync status 



ble_period_adv_remove_dev_cmpl_param

struct ble_period_adv_remove_dev_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_REMOVE_DEV_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising device list remove status 



ble_period_adv_sync_cancel_cmpl_param

struct ble_period_adv_sync_cancel_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SYNC_CANCEL_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising sync cancel status 



ble_period_adv_sync_terminate_cmpl_param

struct ble_period_adv_sync_terminate_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SYNC_TERMINATE_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising sync terminate status 



ble_periodic_adv_data_set_cmpl_param

struct ble_periodic_adv_data_set_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_DATA_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising data set status 

instance

uint8_t instance
extend advertising handle 



ble_periodic_adv_recv_enable_cmpl_param

struct ble_periodic_adv_recv_enable_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_RECV_ENABLE_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Set periodic advertising receive enable status 



ble_periodic_adv_report_param

struct ble_periodic_adv_report_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_REPORT_EVT. 

Public Members
params

esp_ble_gap_periodic_adv_report_t params
periodic advertising report parameters 



ble_periodic_adv_set_info_trans_cmpl_param

struct ble_periodic_adv_set_info_trans_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SET_INFO_TRANS_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Periodic advertising set info transfer status 

bda

esp_bd_addr_t bda
The remote device address 



ble_periodic_adv_set_params_cmpl_param

struct ble_periodic_adv_set_params_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SET_PARAMS_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertisingparameters set status 

instance

uint8_t instance
extend advertising handle 



ble_periodic_adv_start_cmpl_param

struct ble_periodic_adv_start_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_START_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising start status 

instance

uint8_t instance
extend advertising handle 



ble_periodic_adv_stop_cmpl_param

struct ble_periodic_adv_stop_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_STOP_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate periodic advertising stop status 

instance

uint8_t instance
extend advertising handle 



ble_periodic_adv_sync_estab_param

struct ble_periodic_adv_sync_estab_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SYNC_ESTAB_EVT. 

Public Members
status

uint8_t status
periodic advertising sync status 

sync_handle

uint16_t sync_handle
periodic advertising sync handle 

sid

uint8_t sid
periodic advertising sid 

adv_addr_type

esp_ble_addr_type_t adv_addr_type
periodic advertising address type 

adv_addr

esp_bd_addr_t adv_addr
periodic advertising address 

adv_phy

esp_ble_gap_phy_t adv_phy
periodic advertising phy type 

period_adv_interval

uint16_t period_adv_interval
periodic advertising interval 

adv_clk_accuracy

uint8_t adv_clk_accuracy
periodic advertising clock accuracy 



ble_periodic_adv_sync_lost_param

struct ble_periodic_adv_sync_lost_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SYNC_LOST_EVT. 

Public Members
sync_handle

uint16_t sync_handle
sync handle 



ble_periodic_adv_sync_trans_cmpl_param

struct ble_periodic_adv_sync_trans_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Periodic advertising sync transfer status 

bda

esp_bd_addr_t bda
The remote device address 



ble_periodic_adv_sync_trans_recv_param

struct ble_periodic_adv_sync_trans_recv_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_RECV_EVT. 

Public Members
status

esp_bt_status_t status
Periodic advertising sync transfer received status 

bda

esp_bd_addr_t bda
The remote device address 

service_data

uint16_t service_data
The value provided by the peer device 

sync_handle

uint16_t sync_handle
Periodic advertising sync handle 

adv_sid

uint8_t adv_sid
Periodic advertising set id 

adv_addr_type

uint8_t adv_addr_type
Periodic advertiser address type 

adv_addr

esp_bd_addr_t adv_addr
Periodic advertiser address 

adv_phy

esp_ble_gap_phy_t adv_phy
Periodic advertising PHY 

adv_interval

uint16_t adv_interval
Periodic advertising interval 

adv_clk_accuracy

uint8_t adv_clk_accuracy
Periodic advertising clock accuracy 



ble_phy_update_cmpl_param

struct ble_phy_update_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_PHY_UPDATE_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
phy update status 

bda

esp_bd_addr_t bda
address 

tx_phy

esp_ble_gap_phy_t tx_phy
tx phy type 

rx_phy

esp_ble_gap_phy_t rx_phy
rx phy type 



ble_pkt_data_length_cmpl_evt_param

struct ble_pkt_data_length_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set pkt data length operation success status 

params

esp_ble_pkt_data_length_params_t params
pkt data length value 



ble_read_phy_cmpl_evt_param

struct ble_read_phy_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_READ_PHY_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
read phy complete status 

bda

esp_bd_addr_t bda
read phy address 

tx_phy

esp_ble_gap_phy_t tx_phy
tx phy type 

rx_phy

esp_ble_gap_phy_t rx_phy
rx phy type 



ble_read_rssi_cmpl_evt_param

struct ble_read_rssi_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the read adv tx power operation success status 

rssi

int8_t rssi
The ble remote device rssi value, the range is from -127 to 20, the unit is dbm, if the RSSI cannot be read, the RSSI metric shall be set to 127. 

remote_addr

esp_bd_addr_t remote_addr
The remote device address 



ble_remove_bond_dev_cmpl_evt_param

struct ble_remove_bond_dev_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the remove bond device operation success status 

bd_addr

esp_bd_addr_t bd_addr
The device address which has been remove from the bond list 



ble_rpa_timeout_cmpl_evt_param

struct ble_rpa_timeout_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_RPA_TIMEOUT_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set RPA timeout operation success status 



ble_scan_param_cmpl_evt_param

struct ble_scan_param_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set scan param operation success status 



ble_scan_req_received_param

struct ble_scan_req_received_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SCAN_REQ_RECEIVED_EVT. 

Public Members
adv_instance

uint8_t adv_instance
extend advertising handle 

scan_addr_type

esp_ble_addr_type_t scan_addr_type
scanner address type 

scan_addr

esp_bd_addr_t scan_addr
scanner address 



ble_scan_result_evt_param

struct ble_scan_result_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SCAN_RESULT_EVT. 

Public Members
search_evt

esp_gap_search_evt_t search_evt
Search event type 

bda

esp_bd_addr_t bda
Bluetooth device address which has been searched 

dev_type

esp_bt_dev_type_t dev_type
Device type 

ble_addr_type

esp_ble_addr_type_t ble_addr_type
Ble device address type 

ble_evt_type

esp_ble_evt_type_t ble_evt_type
Ble scan result event type 

rssi

int rssi
Searched device's RSSI 

ble_adv

uint8_t ble_adv[ESP_BLE_ADV_DATA_LEN_MAX + ESP_BLE_SCAN_RSP_DATA_LEN_MAX]
Received EIR 

flag

int flag
Advertising data flag bit 

num_resps

int num_resps
Scan result number 

adv_data_len

uint8_t adv_data_len
Adv data length 

scan_rsp_len

uint8_t scan_rsp_len
Scan response length 

num_dis

uint32_t num_dis
The number of discard packets 



ble_scan_rsp_data_cmpl_evt_param

struct ble_scan_rsp_data_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set scan response data operation success status 



ble_scan_rsp_data_raw_cmpl_evt_param

struct ble_scan_rsp_data_raw_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the set raw advertising data operation success status 



ble_scan_start_cmpl_evt_param

struct ble_scan_start_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SCAN_START_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate scan start operation success status 



ble_scan_stop_cmpl_evt_param

struct ble_scan_stop_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate scan stop operation success status 



ble_set_channels_evt_param

struct ble_set_channels_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_CHANNELS_EVT. 

Public Members
stat

esp_bt_status_t stat
BLE set channel status 



ble_set_ext_scan_params_cmpl_param

struct ble_set_ext_scan_params_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_EXT_SCAN_PARAMS_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate extend advertising parameters set status 



ble_set_past_params_cmpl_param

struct ble_set_past_params_cmpl_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_PAST_PARAMS_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Set periodic advertising sync transfer params status 

bda

esp_bd_addr_t bda
The remote device address 



ble_set_perf_def_phy_cmpl_evt_param

struct ble_set_perf_def_phy_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_PREFERRED_DEFAULT_PHY_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate perf default phy set status 



ble_set_perf_phy_cmpl_evt_param

struct ble_set_perf_phy_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_PREFERRED_PHY_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate perf phy set status 



ble_set_privacy_mode_cmpl_evt_param

struct ble_set_privacy_mode_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_PRIVACY_MODE_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate privacy mode set operation success status 



ble_set_rand_cmpl_evt_param

struct ble_set_rand_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_SET_STATIC_RAND_ADDR_EVT. 

Public Members
status

esp_bt_status_t status
Indicate set static rand address operation success status 



ble_update_conn_params_evt_param

struct ble_update_conn_params_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT. 

Public Members
status

esp_bt_status_t status
Indicate update connection parameters success status 

bda

esp_bd_addr_t bda
Bluetooth device address 

min_int

uint16_t min_int
Min connection interval 

max_int

uint16_t max_int
Max connection interval 

latency

uint16_t latency
Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3 

conn_int

uint16_t conn_int
Current connection interval 

timeout

uint16_t timeout
Supervision timeout for the LE Link. Range: 0x000A to 0x0C80. Mandatory Range: 0x000A to 0x0C80 Time = N * 10 msec 



ble_update_duplicate_exceptional_list_cmpl_evt_param

struct ble_update_duplicate_exceptional_list_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate update duplicate scan exceptional list operation success status 

subcode

uint8_t subcode
Define in esp_bt_duplicate_exceptional_subcode_type_t 

length

uint16_t length
The length of device_info 

device_info

esp_duplicate_info_t device_info
device information, when subcode is ESP_BLE_DUPLICATE_EXCEPTIONAL_LIST_CLEAN, the value is invalid 



ble_update_whitelist_cmpl_evt_param

struct ble_update_whitelist_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT. 

Public Members
status

esp_bt_status_t status
Indicate the add or remove whitelist operation success status 

wl_operation

esp_ble_wl_operation_t wl_operation
The value is ESP_BLE_WHITELIST_ADD if add address to whitelist operation success, ESP_BLE_WHITELIST_REMOVE if remove address from the whitelist operation success 



vendor_cmd_cmpl_evt_param

struct vendor_cmd_cmpl_evt_param

#include <esp_gap_ble_api.h>
ESP_GAP_BLE_VENDOR_CMD_COMPLETE_EVT. 

Public Members
opcode

uint16_t opcode
vendor hci command opcode 

param_len

uint16_t param_len
The length of parameter buffer 

p_param_buf

uint8_t *p_param_buf
The point of parameter buffer 




```



### Structures


#### esp_ble_vendor_cmd_params_t


```


struct esp_ble_vendor_cmd_params_t
Vendor HCI command parameters. 

Public Members
opcode

uint16_t opcode
vendor hci command opcode 

param_len

uint8_t param_len
the length of parameter 

p_param_buf

uint8_t *p_param_buf
the point of parameter buffer 



```

#### esp_ble_dtm_tx_t


```


struct esp_ble_dtm_tx_t
DTM TX parameters. 

Public Members
tx_channel

uint8_t tx_channel
channel for sending test data, tx_channel = (Frequency -2402)/2, tx_channel range:0x00-0x27, Frequency range: 2402 MHz to 2480 MHz 

len_of_data

uint8_t len_of_data
length in bytes of payload data in each packet 

pkt_payload

esp_ble_dtm_pkt_payload_t pkt_payload
packet payload type. value range: 0x00-0x07 



```

#### esp_ble_dtm_rx_t


```


struct esp_ble_dtm_rx_t
DTM RX parameters. 

Public Members
rx_channel

uint8_t rx_channel
channel for test data reception, rx_channel = (Frequency -2402)/2, tx_channel range:0x00-0x27, Frequency range: 2402 MHz to 2480 MHz 



```

#### esp_ble_adv_params_t


```


struct esp_ble_adv_params_t
Advertising parameters. 

Public Members
adv_int_min

uint16_t adv_int_min
Minimum advertising interval for undirected and low duty cycle directed advertising. Range: 0x0020 to 0x4000 Default: N = 0x0800 (1.28 second) Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec 

adv_int_max

uint16_t adv_int_max
Maximum advertising interval for undirected and low duty cycle directed advertising. Range: 0x0020 to 0x4000 Default: N = 0x0800 (1.28 second) Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec Advertising max interval 

adv_type

esp_ble_adv_type_t adv_type
Advertising type 

own_addr_type

esp_ble_addr_type_t own_addr_type
Owner bluetooth device address type 

peer_addr

esp_bd_addr_t peer_addr
Peer device bluetooth device address 

peer_addr_type

esp_ble_addr_type_t peer_addr_type
Peer device bluetooth device address type, only support public address type and random address type 

channel_map

esp_ble_adv_channel_t channel_map
Advertising channel map 

adv_filter_policy

esp_ble_adv_filter_t adv_filter_policy
Advertising filter policy 



```

#### esp_ble_adv_data_t


```


struct esp_ble_adv_data_t
Advertising data content, according to "Supplement to the Bluetooth Core Specification". 

Public Members
set_scan_rsp

bool set_scan_rsp
Set this advertising data as scan response or not 

include_name

bool include_name
Advertising data include device name or not 

include_txpower

bool include_txpower
Advertising data include TX power 

min_interval

int min_interval
Advertising data show slave preferred connection min interval. The connection interval in the following manner: connIntervalmin = Conn_Interval_Min * 1.25 ms Conn_Interval_Min range: 0x0006 to 0x0C80 Value of 0xFFFF indicates no specific minimum. Values not defined above are reserved for future use. 

max_interval

int max_interval
Advertising data show slave preferred connection max interval. The connection interval in the following manner: connIntervalmax = Conn_Interval_Max * 1.25 ms Conn_Interval_Max range: 0x0006 to 0x0C80 Conn_Interval_Max shall be equal to or greater than the Conn_Interval_Min. Value of 0xFFFF indicates no specific maximum. Values not defined above are reserved for future use. 

appearance

int appearance
External appearance of device 

manufacturer_len

uint16_t manufacturer_len
Manufacturer data length 

p_manufacturer_data

uint8_t *p_manufacturer_data
Manufacturer data point 

service_data_len

uint16_t service_data_len
Service data length 

p_service_data

uint8_t *p_service_data
Service data point 

service_uuid_len

uint16_t service_uuid_len
Service uuid length 

p_service_uuid

uint8_t *p_service_uuid
Service uuid array point 

flag

uint8_t flag
Advertising flag of discovery mode, see BLE_ADV_DATA_FLAG detail 



```

#### esp_ble_scan_params_t


```


struct esp_ble_scan_params_t
Ble scan parameters. 

Public Members
scan_type

esp_ble_scan_type_t scan_type
Scan type 

own_addr_type

esp_ble_addr_type_t own_addr_type
Owner address type 

scan_filter_policy

esp_ble_scan_filter_t scan_filter_policy
Scan filter policy 

scan_interval

uint16_t scan_interval
Scan interval. This is defined as the time interval from when the Controller started its last LE scan until it begins the subsequent LE scan. Range: 0x0004 to 0x4000 Default: 0x0010 (10 ms) Time = N * 0.625 msec Time Range: 2.5 msec to 10.24 seconds 

scan_window

uint16_t scan_window
Scan window. The duration of the LE scan. LE_Scan_Window shall be less than or equal to LE_Scan_Interval Range: 0x0004 to 0x4000 Default: 0x0010 (10 ms) Time = N * 0.625 msec Time Range: 2.5 msec to 10240 msec 

scan_duplicate

esp_ble_scan_duplicate_t scan_duplicate
The Scan_Duplicates parameter controls whether the Link Layer should filter out duplicate advertising reports (BLE_SCAN_DUPLICATE_ENABLE) to the Host, or if the Link Layer should generate advertising reports for each packet received 



```

#### esp_gap_conn_params_t


```


struct esp_gap_conn_params_t
connection parameters information 

Public Members
interval

uint16_t interval
connection interval 

latency

uint16_t latency
Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3 

timeout

uint16_t timeout
Supervision timeout for the LE Link. Range: 0x000A to 0x0C80. Mandatory Range: 0x000A to 0x0C80 Time = N * 10 msec Time Range: 100 msec to 32 seconds 



```

#### esp_ble_conn_update_params_t


```


struct esp_ble_conn_update_params_t
Connection update parameters. 

Public Members
bda

esp_bd_addr_t bda
Bluetooth device address 

min_int

uint16_t min_int
Min connection interval 

max_int

uint16_t max_int
Max connection interval 

latency

uint16_t latency
Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3 

timeout

uint16_t timeout
Supervision timeout for the LE Link. Range: 0x000A to 0x0C80. Mandatory Range: 0x000A to 0x0C80 Time = N * 10 msec Time Range: 100 msec to 32 seconds 



```

#### esp_ble_pkt_data_length_params_t


```


struct esp_ble_pkt_data_length_params_t
BLE pkt date length keys. 

Public Members
rx_len

uint16_t rx_len
pkt rx data length value 

tx_len

uint16_t tx_len
pkt tx data length value 



```

#### esp_ble_penc_keys_t


```


struct esp_ble_penc_keys_t
BLE encryption keys. 

Public Members
ltk

esp_bt_octet16_t ltk
The long term key 

rand

esp_bt_octet8_t rand
The random number 

ediv

uint16_t ediv
The ediv value 

sec_level

uint8_t sec_level
The security level of the security link 

key_size

uint8_t key_size
The key size(7~16) of the security link 



```

#### esp_ble_pcsrk_keys_t


```


struct esp_ble_pcsrk_keys_t
BLE CSRK keys. 

Public Members
counter

uint32_t counter
The counter 

csrk

esp_bt_octet16_t csrk
The csrk key 

sec_level

uint8_t sec_level
The security level 



```

#### esp_ble_pid_keys_t


```


struct esp_ble_pid_keys_t
BLE pid keys. 

Public Members
irk

esp_bt_octet16_t irk
The irk value 

addr_type

esp_ble_addr_type_t addr_type
The address type 

static_addr

esp_bd_addr_t static_addr
The static address 



```

#### esp_ble_lenc_keys_t


```


struct esp_ble_lenc_keys_t
BLE Encryption reproduction keys. 

Public Members
ltk

esp_bt_octet16_t ltk
The long term key 

div

uint16_t div
The div value 

key_size

uint8_t key_size
The key size of the security link 

sec_level

uint8_t sec_level
The security level of the security link 



```

#### esp_ble_lcsrk_keys


```


struct esp_ble_lcsrk_keys
BLE SRK keys. 

Public Members
counter

uint32_t counter
The counter value 

div

uint16_t div
The div value 

sec_level

uint8_t sec_level
The security level of the security link 

csrk

esp_bt_octet16_t csrk
The csrk key value 



```

#### esp_ble_sec_key_notif_t


```


struct esp_ble_sec_key_notif_t
Structure associated with ESP_KEY_NOTIF_EVT. 

Public Members
bd_addr

esp_bd_addr_t bd_addr
peer address 

passkey

uint32_t passkey
the numeric value for comparison. If just_works, do not show this number to UI 



```

#### esp_ble_sec_req_t


```


struct esp_ble_sec_req_t
Structure of the security request. 

Public Members
bd_addr

esp_bd_addr_t bd_addr
peer address 



```

#### esp_ble_bond_key_info_t


```


struct esp_ble_bond_key_info_t
struct type of the bond key information value 

Public Members
key_mask

esp_ble_key_mask_t key_mask
the key mask to indicate witch key is present 

penc_key

esp_ble_penc_keys_t penc_key
received peer encryption key 

pcsrk_key

esp_ble_pcsrk_keys_t pcsrk_key
received peer device SRK 

pid_key

esp_ble_pid_keys_t pid_key
peer device ID key 



```

#### esp_ble_bond_dev_t


```


struct esp_ble_bond_dev_t
struct type of the bond device value 

Public Members
bd_addr

esp_bd_addr_t bd_addr
peer address 

bond_key

esp_ble_bond_key_info_t bond_key
the bond key information 



```

#### esp_ble_key_t


```


struct esp_ble_key_t
union type of the security key value 

Public Members
bd_addr

esp_bd_addr_t bd_addr
peer address 

key_type

esp_ble_key_type_t key_type
key type of the security link 

p_key_value

esp_ble_key_value_t p_key_value
the pointer to the key value 



```

#### esp_ble_local_id_keys_t


```


struct esp_ble_local_id_keys_t
structure type of the ble local id keys value 

Public Members
ir

esp_bt_octet16_t ir
the 16 bits of the ir value 

irk

esp_bt_octet16_t irk
the 16 bits of the ir key value 

dhk

esp_bt_octet16_t dhk
the 16 bits of the dh key value 



```

#### esp_ble_local_oob_data_t


```


struct esp_ble_local_oob_data_t
structure type of the ble local oob data value 

Public Members
oob_c

esp_bt_octet16_t oob_c
the 128 bits of confirmation value 

oob_r

esp_bt_octet16_t oob_r
the 128 bits of randomizer value 



```

#### esp_ble_auth_cmpl_t


```


struct esp_ble_auth_cmpl_t
Structure associated with ESP_AUTH_CMPL_EVT. 

Public Members
bd_addr

esp_bd_addr_t bd_addr
BD address of peer device 

key_present

bool key_present
True if the link key value is valid; false otherwise 

key

esp_link_key key
Link key associated with peer device 

key_type

uint8_t key_type
The type of link key 

success

bool success
True if authentication succeeded; false otherwise 

fail_reason

esp_ble_auth_fail_rsn_t fail_reason
The HCI reason/error code for failure when success is false 

addr_type

esp_ble_addr_type_t addr_type
Peer device address type 

dev_type

esp_bt_dev_type_t dev_type
Device type 

auth_mode

esp_ble_auth_req_t auth_mode
Authentication mode 



```

#### esp_ble_gap_ext_adv_params_t


```


struct esp_ble_gap_ext_adv_params_t
ext adv parameters 

Public Members
type

esp_ble_ext_adv_type_mask_t type
ext adv type 

interval_min

uint32_t interval_min
ext adv minimum interval 

interval_max

uint32_t interval_max
ext adv maximum interval 

channel_map

esp_ble_adv_channel_t channel_map
ext adv channel map 

own_addr_type

esp_ble_addr_type_t own_addr_type
ext adv own address type 

peer_addr_type

esp_ble_addr_type_t peer_addr_type
ext adv peer address type 

peer_addr

esp_bd_addr_t peer_addr
ext adv peer address 

filter_policy

esp_ble_adv_filter_t filter_policy
ext adv filter policy 

tx_power

int8_t tx_power
ext adv tx power 

primary_phy

esp_ble_gap_pri_phy_t primary_phy
ext adv primary phy 

max_skip

uint8_t max_skip
ext adv maximum skip 

secondary_phy

esp_ble_gap_phy_t secondary_phy
ext adv secondary phy 

sid

uint8_t sid
ext adv sid 

scan_req_notif

bool scan_req_notif
ext adv scan request event notify 



```

#### esp_ble_ext_scan_cfg_t


```


struct esp_ble_ext_scan_cfg_t
ext scan config 

Public Members
scan_type

esp_ble_scan_type_t scan_type
ext scan type 

scan_interval

uint16_t scan_interval
ext scan interval 

scan_window

uint16_t scan_window
ext scan window 



```

#### esp_ble_ext_scan_params_t


```


struct esp_ble_ext_scan_params_t
ext scan parameters 

Public Members
own_addr_type

esp_ble_addr_type_t own_addr_type
ext scan own address type 

filter_policy

esp_ble_scan_filter_t filter_policy
ext scan filter policy 

scan_duplicate

esp_ble_scan_duplicate_t scan_duplicate
ext scan duplicate scan 

cfg_mask

esp_ble_ext_scan_cfg_mask_t cfg_mask
ext scan config mask 

uncoded_cfg

esp_ble_ext_scan_cfg_t uncoded_cfg
ext scan uncoded config parameters 

coded_cfg

esp_ble_ext_scan_cfg_t coded_cfg
ext scan coded config parameters 



```

#### esp_ble_gap_conn_params_t


```


struct esp_ble_gap_conn_params_t
create extend connection parameters 

Public Members
scan_interval

uint16_t scan_interval
init scan interval 

scan_window

uint16_t scan_window
init scan window 

interval_min

uint16_t interval_min
minimum interval 

interval_max

uint16_t interval_max
maximum interval 

latency

uint16_t latency
ext scan type 

supervision_timeout

uint16_t supervision_timeout
connection supervision timeout 

min_ce_len

uint16_t min_ce_len
minimum ce length 

max_ce_len

uint16_t max_ce_len
maximum ce length 



```

#### esp_ble_gap_ext_adv_t


```


struct esp_ble_gap_ext_adv_t
extend adv enable parameters 

Public Members
instance

uint8_t instance
advertising handle 

duration

int duration
advertising duration 

max_events

int max_events
maximum number of extended advertising events 



```

#### esp_ble_gap_periodic_adv_params_t


```


struct esp_ble_gap_periodic_adv_params_t
periodic adv parameters 

Public Members
interval_min

uint16_t interval_min
periodic advertising minimum interval 

interval_max

uint16_t interval_max
periodic advertising maximum interval 

properties

uint8_t properties
periodic advertising properties 



```

#### esp_ble_gap_periodic_adv_sync_params_t


```


struct esp_ble_gap_periodic_adv_sync_params_t
periodic adv sync parameters 

Public Members
filter_policy

esp_ble_gap_sync_t filter_policy
Configures the filter policy for periodic advertising sync: 0: Use Advertising SID, Advertiser Address Type, and Advertiser Address parameters to determine the advertiser to listen to. 1: Use the Periodic Advertiser List to determine the advertiser to listen to. 

sid

uint8_t sid
SID of the periodic advertising 

addr_type

esp_ble_addr_type_t addr_type
Address type of the periodic advertising 

addr

esp_bd_addr_t addr
Address of the periodic advertising 

skip

uint16_t skip
Maximum number of periodic advertising events that can be skipped 

sync_timeout

uint16_t sync_timeout
Synchronization timeout 



```

#### esp_ble_gap_ext_adv_report_t


```


struct esp_ble_gap_ext_adv_report_t
extend adv report parameters 

Public Members
event_type

esp_ble_gap_adv_type_t event_type
extend advertising type 

addr_type

uint8_t addr_type
extend advertising address type 

addr

esp_bd_addr_t addr
extend advertising address 

primary_phy

esp_ble_gap_pri_phy_t primary_phy
extend advertising primary phy 

secondly_phy

esp_ble_gap_phy_t secondly_phy
extend advertising secondary phy 

sid

uint8_t sid
extend advertising sid 

tx_power

uint8_t tx_power
extend advertising tx power 

rssi

int8_t rssi
extend advertising rssi 

per_adv_interval

uint16_t per_adv_interval
periodic advertising interval 

dir_addr_type

uint8_t dir_addr_type
direct address type 

dir_addr

esp_bd_addr_t dir_addr
direct address 

data_status

esp_ble_gap_ext_adv_data_status_t data_status
data type 

adv_data_len

uint8_t adv_data_len
extend advertising data length 

adv_data

uint8_t adv_data[251]
extend advertising data 



```

#### esp_ble_gap_periodic_adv_report_t


```


struct esp_ble_gap_periodic_adv_report_t
periodic adv report parameters 

Public Members
sync_handle

uint16_t sync_handle
periodic advertising train handle 

tx_power

uint8_t tx_power
periodic advertising tx power 

rssi

int8_t rssi
periodic advertising rssi 

data_status

esp_ble_gap_ext_adv_data_status_t data_status
periodic advertising data type 

data_length

uint8_t data_length
periodic advertising data length 

data

uint8_t data[251]
periodic advertising data 



```

#### esp_ble_gap_periodic_adv_sync_estab_t


```


struct esp_ble_gap_periodic_adv_sync_estab_t
perodic adv sync establish parameters 

Public Members
status

uint8_t status
periodic advertising sync status 

sync_handle

uint16_t sync_handle
periodic advertising train handle 

sid

uint8_t sid
periodic advertising sid 

addr_type

esp_ble_addr_type_t addr_type
periodic advertising address type 

adv_addr

esp_bd_addr_t adv_addr
periodic advertising address 

adv_phy

esp_ble_gap_phy_t adv_phy
periodic advertising adv phy type 

period_adv_interval

uint16_t period_adv_interval
periodic advertising interval 

adv_clk_accuracy

uint8_t adv_clk_accuracy
periodic advertising clock accuracy 



```

#### esp_ble_dtm_enh_tx_t


```


struct esp_ble_dtm_enh_tx_t
DTM TX parameters. 

Public Members
tx_channel

uint8_t tx_channel
channel for sending test data, tx_channel = (Frequency -2402)/2, tx_channel range:0x00-0x27, Frequency range: 2402 MHz to 2480 MHz 

len_of_data

uint8_t len_of_data
length in bytes of payload data in each packet 

pkt_payload

esp_ble_dtm_pkt_payload_t pkt_payload
packet payload type. value range: 0x00-0x07 

phy

esp_ble_gap_phy_t phy
the phy type used by the transmitter, coded phy with S=2:0x04 



```

#### esp_ble_dtm_enh_rx_t


```


struct esp_ble_dtm_enh_rx_t
DTM RX parameters. 

Public Members
rx_channel

uint8_t rx_channel
channel for test data reception, rx_channel = (Frequency -2402)/2, tx_channel range:0x00-0x27, Frequency range: 2402 MHz to 2480 MHz 

phy

esp_ble_gap_phy_t phy
the phy type used by the receiver, 1M phy: 0x01, 2M phy:0x02, coded phy:0x03 

modulation_idx

uint8_t modulation_idx
modulation index, 0x00:standard modulation index, 0x01:stable modulation index 



```

#### esp_ble_gap_past_params_t


```


struct esp_ble_gap_past_params_t
periodic adv sync transfer parameters 

Public Members
mode

esp_ble_gap_past_mode_t mode
periodic advertising sync transfer mode 

skip

uint16_t skip
the number of periodic advertising packets that can be skipped 

sync_timeout

uint16_t sync_timeout
synchronization timeout for the periodic advertising train 

cte_type

uint8_t cte_type
periodic advertising sync transfer CET type 



```



### Macros


#### ESP_BLE_ADV_FLAG_LIMIT_DISC


```


ESP_BLE_ADV_FLAG_LIMIT_DISC
BLE_ADV_DATA_FLAG data flag bit definition used for advertising data flag. 

```

#### ESP_BLE_ADV_FLAG_GEN_DISC


```


ESP_BLE_ADV_FLAG_GEN_DISC

```

#### ESP_BLE_ADV_FLAG_BREDR_NOT_SPT


```


ESP_BLE_ADV_FLAG_BREDR_NOT_SPT

```

#### ESP_BLE_ADV_FLAG_DMT_CONTROLLER_SPT


```


ESP_BLE_ADV_FLAG_DMT_CONTROLLER_SPT

```

#### ESP_BLE_ADV_FLAG_DMT_HOST_SPT


```


ESP_BLE_ADV_FLAG_DMT_HOST_SPT

```

#### ESP_BLE_ADV_FLAG_NON_LIMIT_DISC


```


ESP_BLE_ADV_FLAG_NON_LIMIT_DISC

```

#### ESP_LE_KEY_NONE


```


ESP_LE_KEY_NONE
relate to BTM_LE_KEY_xxx in stack/btm_api.h 
No encryption key 

```

#### ESP_LE_KEY_PENC


```


ESP_LE_KEY_PENC
encryption key, encryption information of peer device 

```

#### ESP_LE_KEY_PID


```


ESP_LE_KEY_PID
identity key of the peer device 

```

#### ESP_LE_KEY_PCSRK


```


ESP_LE_KEY_PCSRK
peer SRK 

```

#### ESP_LE_KEY_PLK


```


ESP_LE_KEY_PLK
Link key 

```

#### ESP_LE_KEY_LLK


```


ESP_LE_KEY_LLK
peer link key 

```

#### ESP_LE_KEY_LENC


```


ESP_LE_KEY_LENC
master role security information:div 

```

#### ESP_LE_KEY_LID


```


ESP_LE_KEY_LID
master device ID key 

```

#### ESP_LE_KEY_LCSRK


```


ESP_LE_KEY_LCSRK
local CSRK has been deliver to peer 

```

#### ESP_LE_AUTH_NO_BOND


```


ESP_LE_AUTH_NO_BOND
relate to BTM_LE_AUTH_xxx in stack/btm_api.h 
0 no bondingv 

```

#### ESP_LE_AUTH_BOND


```


ESP_LE_AUTH_BOND
1 << 0 device in the bonding with peer 

```

#### ESP_LE_AUTH_REQ_MITM


```


ESP_LE_AUTH_REQ_MITM
1 << 2 man in the middle attack 

```

#### ESP_LE_AUTH_REQ_BOND_MITM


```


ESP_LE_AUTH_REQ_BOND_MITM
0101 banding with man in the middle attack 

```

#### ESP_LE_AUTH_REQ_SC_ONLY


```


ESP_LE_AUTH_REQ_SC_ONLY
1 << 3 secure connection 

```

#### ESP_LE_AUTH_REQ_SC_BOND


```


ESP_LE_AUTH_REQ_SC_BOND
1001 secure connection with band 

```

#### ESP_LE_AUTH_REQ_SC_MITM


```


ESP_LE_AUTH_REQ_SC_MITM
1100 secure conn with MITM 

```

#### ESP_LE_AUTH_REQ_SC_MITM_BOND


```


ESP_LE_AUTH_REQ_SC_MITM_BOND
1101 SC with MITM and Bonding 

```

#### ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE


```


ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE
authentication disable 

```

#### ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_ENABLE


```


ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_ENABLE
authentication enable 

```

#### ESP_BLE_OOB_DISABLE


```


ESP_BLE_OOB_DISABLE
disable the out of bond 

```

#### ESP_BLE_OOB_ENABLE


```


ESP_BLE_OOB_ENABLE
enable the out of bond 

```

#### ESP_IO_CAP_OUT


```


ESP_IO_CAP_OUT
relate to BTM_IO_CAP_xxx in stack/btm_api.h 
DisplayOnly 

```

#### ESP_IO_CAP_IO


```


ESP_IO_CAP_IO
DisplayYesNo 

```

#### ESP_IO_CAP_IN


```


ESP_IO_CAP_IN
KeyboardOnly 

```

#### ESP_IO_CAP_NONE


```


ESP_IO_CAP_NONE
NoInputNoOutput 

```

#### ESP_IO_CAP_KBDISP


```


ESP_IO_CAP_KBDISP
Keyboard display 

```

#### ESP_BLE_APPEARANCE_UNKNOWN


```


ESP_BLE_APPEARANCE_UNKNOWN
relate to BTM_BLE_APPEARANCE_UNKNOWN in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_PHONE


```


ESP_BLE_APPEARANCE_GENERIC_PHONE
relate to BTM_BLE_APPEARANCE_GENERIC_PHONE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_COMPUTER


```


ESP_BLE_APPEARANCE_GENERIC_COMPUTER
relate to BTM_BLE_APPEARANCE_GENERIC_COMPUTER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_WATCH


```


ESP_BLE_APPEARANCE_GENERIC_WATCH
relate to BTM_BLE_APPEARANCE_GENERIC_WATCH in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_SPORTS_WATCH


```


ESP_BLE_APPEARANCE_SPORTS_WATCH
relate to BTM_BLE_APPEARANCE_SPORTS_WATCH in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_CLOCK


```


ESP_BLE_APPEARANCE_GENERIC_CLOCK
relate to BTM_BLE_APPEARANCE_GENERIC_CLOCK in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_DISPLAY


```


ESP_BLE_APPEARANCE_GENERIC_DISPLAY
relate to BTM_BLE_APPEARANCE_GENERIC_DISPLAY in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_REMOTE


```


ESP_BLE_APPEARANCE_GENERIC_REMOTE
relate to BTM_BLE_APPEARANCE_GENERIC_REMOTE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_EYEGLASSES


```


ESP_BLE_APPEARANCE_GENERIC_EYEGLASSES
relate to BTM_BLE_APPEARANCE_GENERIC_EYEGLASSES in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_TAG


```


ESP_BLE_APPEARANCE_GENERIC_TAG
relate to BTM_BLE_APPEARANCE_GENERIC_TAG in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_KEYRING


```


ESP_BLE_APPEARANCE_GENERIC_KEYRING
relate to BTM_BLE_APPEARANCE_GENERIC_KEYRING in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER


```


ESP_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER
relate to BTM_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER


```


ESP_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER
relate to BTM_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_THERMOMETER


```


ESP_BLE_APPEARANCE_GENERIC_THERMOMETER
relate to BTM_BLE_APPEARANCE_GENERIC_THERMOMETER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_THERMOMETER_EAR


```


ESP_BLE_APPEARANCE_THERMOMETER_EAR
relate to BTM_BLE_APPEARANCE_THERMOMETER_EAR in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_HEART_RATE


```


ESP_BLE_APPEARANCE_GENERIC_HEART_RATE
relate to BTM_BLE_APPEARANCE_GENERIC_HEART_RATE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HEART_RATE_BELT


```


ESP_BLE_APPEARANCE_HEART_RATE_BELT
relate to BTM_BLE_APPEARANCE_HEART_RATE_BELT in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE


```


ESP_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE
relate to BTM_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_BLOOD_PRESSURE_ARM


```


ESP_BLE_APPEARANCE_BLOOD_PRESSURE_ARM
relate to BTM_BLE_APPEARANCE_BLOOD_PRESSURE_ARM in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST


```


ESP_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST
relate to BTM_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_HID


```


ESP_BLE_APPEARANCE_GENERIC_HID
relate to BTM_BLE_APPEARANCE_GENERIC_HID in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_KEYBOARD


```


ESP_BLE_APPEARANCE_HID_KEYBOARD
relate to BTM_BLE_APPEARANCE_HID_KEYBOARD in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_MOUSE


```


ESP_BLE_APPEARANCE_HID_MOUSE
relate to BTM_BLE_APPEARANCE_HID_MOUSE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_JOYSTICK


```


ESP_BLE_APPEARANCE_HID_JOYSTICK
relate to BTM_BLE_APPEARANCE_HID_JOYSTICK in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_GAMEPAD


```


ESP_BLE_APPEARANCE_HID_GAMEPAD
relate to BTM_BLE_APPEARANCE_HID_GAMEPAD in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_DIGITIZER_TABLET


```


ESP_BLE_APPEARANCE_HID_DIGITIZER_TABLET
relate to BTM_BLE_APPEARANCE_HID_DIGITIZER_TABLET in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_CARD_READER


```


ESP_BLE_APPEARANCE_HID_CARD_READER
relate to BTM_BLE_APPEARANCE_HID_CARD_READER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_DIGITAL_PEN


```


ESP_BLE_APPEARANCE_HID_DIGITAL_PEN
relate to BTM_BLE_APPEARANCE_HID_DIGITAL_PEN in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_HID_BARCODE_SCANNER


```


ESP_BLE_APPEARANCE_HID_BARCODE_SCANNER
relate to BTM_BLE_APPEARANCE_HID_BARCODE_SCANNER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_GLUCOSE


```


ESP_BLE_APPEARANCE_GENERIC_GLUCOSE
relate to BTM_BLE_APPEARANCE_GENERIC_GLUCOSE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_WALKING


```


ESP_BLE_APPEARANCE_GENERIC_WALKING
relate to BTM_BLE_APPEARANCE_GENERIC_WALKING in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_WALKING_IN_SHOE


```


ESP_BLE_APPEARANCE_WALKING_IN_SHOE
relate to BTM_BLE_APPEARANCE_WALKING_IN_SHOE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_WALKING_ON_SHOE


```


ESP_BLE_APPEARANCE_WALKING_ON_SHOE
relate to BTM_BLE_APPEARANCE_WALKING_ON_SHOE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_WALKING_ON_HIP


```


ESP_BLE_APPEARANCE_WALKING_ON_HIP
relate to BTM_BLE_APPEARANCE_WALKING_ON_HIP in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_CYCLING


```


ESP_BLE_APPEARANCE_GENERIC_CYCLING
relate to BTM_BLE_APPEARANCE_GENERIC_CYCLING in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_CYCLING_COMPUTER


```


ESP_BLE_APPEARANCE_CYCLING_COMPUTER
relate to BTM_BLE_APPEARANCE_CYCLING_COMPUTER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_CYCLING_SPEED


```


ESP_BLE_APPEARANCE_CYCLING_SPEED
relate to BTM_BLE_APPEARANCE_CYCLING_SPEED in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_CYCLING_CADENCE


```


ESP_BLE_APPEARANCE_CYCLING_CADENCE
relate to BTM_BLE_APPEARANCE_CYCLING_CADENCE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_CYCLING_POWER


```


ESP_BLE_APPEARANCE_CYCLING_POWER
relate to BTM_BLE_APPEARANCE_CYCLING_POWER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_CYCLING_SPEED_CADENCE


```


ESP_BLE_APPEARANCE_CYCLING_SPEED_CADENCE
relate to BTM_BLE_APPEARANCE_CYCLING_SPEED_CADENCE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_STANDALONE_SPEAKER


```


ESP_BLE_APPEARANCE_STANDALONE_SPEAKER
relate to BTM_BLE_APPEARANCE_STANDALONE_SPEAKER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_PULSE_OXIMETER


```


ESP_BLE_APPEARANCE_GENERIC_PULSE_OXIMETER
relate to BTM_BLE_APPEARANCE_GENERIC_PULSE_OXIMETER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP


```


ESP_BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP
relate to BTM_BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_PULSE_OXIMETER_WRIST


```


ESP_BLE_APPEARANCE_PULSE_OXIMETER_WRIST
relate to BTM_BLE_APPEARANCE_PULSE_OXIMETER_WRIST in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_WEIGHT


```


ESP_BLE_APPEARANCE_GENERIC_WEIGHT
relate to BTM_BLE_APPEARANCE_GENERIC_WEIGHT in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE


```


ESP_BLE_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE
relate to BTM_BLE_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_POWERED_WHEELCHAIR


```


ESP_BLE_APPEARANCE_POWERED_WHEELCHAIR
relate to BTM_BLE_APPEARANCE_POWERED_WHEELCHAIR in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_MOBILITY_SCOOTER


```


ESP_BLE_APPEARANCE_MOBILITY_SCOOTER
relate to BTM_BLE_APPEARANCE_MOBILITY_SCOOTER in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_CONTINUOUS_GLUCOSE_MONITOR


```


ESP_BLE_APPEARANCE_GENERIC_CONTINUOUS_GLUCOSE_MONITOR
relate to BTM_BLE_APPEARANCE_GENERIC_CONTINUOUS_GLUCOSE_MONITOR in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_INSULIN_PUMP


```


ESP_BLE_APPEARANCE_GENERIC_INSULIN_PUMP
relate to BTM_BLE_APPEARANCE_GENERIC_INSULIN_PUMP in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_INSULIN_PUMP_DURABLE_PUMP


```


ESP_BLE_APPEARANCE_INSULIN_PUMP_DURABLE_PUMP
relate to BTM_BLE_APPEARANCE_INSULIN_PUMP_DURABLE_PUMP in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_INSULIN_PUMP_PATCH_PUMP


```


ESP_BLE_APPEARANCE_INSULIN_PUMP_PATCH_PUMP
relate to BTM_BLE_APPEARANCE_INSULIN_PUMP_PATCH_PUMP in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_INSULIN_PEN


```


ESP_BLE_APPEARANCE_INSULIN_PEN
relate to BTM_BLE_APPEARANCE_INSULIN_PEN in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_MEDICATION_DELIVERY


```


ESP_BLE_APPEARANCE_GENERIC_MEDICATION_DELIVERY
relate to BTM_BLE_APPEARANCE_GENERIC_MEDICATION_DELIVERY in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS


```


ESP_BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS
relate to BTM_BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION


```


ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION
relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV


```


ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV
relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD


```


ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD
relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD in stack/btm_ble_api.h 

```

#### ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV


```


ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV
relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV in stack/btm_ble_api.h 

```

#### BLE_DTM_PKT_PAYLOAD_0x00


```


BLE_DTM_PKT_PAYLOAD_0x00
PRBS9 sequence ‘11111111100000111101...’ (in transmission order) as described in [Vol 6] Part F, Section 4.1.5 

```

#### BLE_DTM_PKT_PAYLOAD_0x01


```


BLE_DTM_PKT_PAYLOAD_0x01
Repeated ‘11110000’ (in transmission order) sequence as described in [Vol 6] Part F, Section 4.1.5 

```

#### BLE_DTM_PKT_PAYLOAD_0x02


```


BLE_DTM_PKT_PAYLOAD_0x02
Repeated ‘10101010’ (in transmission order) sequence as described in [Vol 6] Part F, Section 4.1.5 

```

#### BLE_DTM_PKT_PAYLOAD_0x03


```


BLE_DTM_PKT_PAYLOAD_0x03
PRBS15 sequence as described in [Vol 6] Part F, Section 4.1.5 

```

#### BLE_DTM_PKT_PAYLOAD_0x04


```


BLE_DTM_PKT_PAYLOAD_0x04
Repeated ‘11111111’ (in transmission order) sequence 

```

#### BLE_DTM_PKT_PAYLOAD_0x05


```


BLE_DTM_PKT_PAYLOAD_0x05
Repeated ‘00000000’ (in transmission order) sequence 

```

#### BLE_DTM_PKT_PAYLOAD_0x06


```


BLE_DTM_PKT_PAYLOAD_0x06
Repeated ‘00001111’ (in transmission order) sequence 

```

#### BLE_DTM_PKT_PAYLOAD_0x07


```


BLE_DTM_PKT_PAYLOAD_0x07
Repeated ‘01010101’ (in transmission order) sequence 

```

#### BLE_DTM_PKT_PAYLOAD_MAX


```


BLE_DTM_PKT_PAYLOAD_MAX
0x08 ~ 0xFF, Reserved for future use 

```

#### ESP_GAP_BLE_CHANNELS_LEN


```


ESP_GAP_BLE_CHANNELS_LEN
channel length 

```

#### ESP_GAP_BLE_ADD_WHITELIST_COMPLETE_EVT


```


ESP_GAP_BLE_ADD_WHITELIST_COMPLETE_EVT
This is the old name, just for backwards compatibility. 

```

#### ESP_BLE_ADV_DATA_LEN_MAX


```


ESP_BLE_ADV_DATA_LEN_MAX
Advertising data maximum length. 

```

#### ESP_BLE_SCAN_RSP_DATA_LEN_MAX


```


ESP_BLE_SCAN_RSP_DATA_LEN_MAX
Scan response data maximum length. 

```

#### VENDOR_HCI_CMD_MASK


```


VENDOR_HCI_CMD_MASK

```

#### BLE_BIT


```


BLE_BIT(n)

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_NONCONN_NONSCANNABLE_UNDIRECTED


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_NONCONN_NONSCANNABLE_UNDIRECTED
Non-Connectable and Non-Scannable Undirected advertising 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_CONNECTABLE


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_CONNECTABLE
Connectable advertising 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_SCANNABLE


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_SCANNABLE
Scannable advertising 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_DIRECTED


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_DIRECTED
Directed advertising 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_HD_DIRECTED


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_HD_DIRECTED
High Duty Cycle Directed Connectable advertising (<= 3.75 ms Advertising Interval) 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY
Use legacy advertising PDUs 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_ANON_ADV


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_ANON_ADV
Omit advertiser's address from all PDUs ("anonymous advertising") 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_INCLUDE_TX_PWR


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_INCLUDE_TX_PWR
Include TxPower in the extended header of the advertising PDU 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_MASK


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_MASK
Reserved for future use If extended advertising PDU types are being used (bit 4 = 0) then: The advertisement shall not be both connectable and scannable. High duty cycle directed connectable advertising (<= 3.75 ms advertising interval) shall not be used (bit 3 = 0) ADV_IND 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_IND


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_IND
ADV_DIRECT_IND (low duty cycle) 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_LD_DIR


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_LD_DIR
ADV_DIRECT_IND (high duty cycle) 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_HD_DIR


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_HD_DIR
ADV_SCAN_IND 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_SCAN


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_SCAN
ADV_NONCONN_IND 

```

#### ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_NONCONN


```


ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_NONCONN

```

#### ESP_BLE_GAP_PHY_1M


```


ESP_BLE_GAP_PHY_1M
Secondery Advertisement PHY is LE1M 

```

#### ESP_BLE_GAP_PHY_2M


```


ESP_BLE_GAP_PHY_2M
Secondery Advertisement PHY is LE2M 

```

#### ESP_BLE_GAP_PHY_CODED


```


ESP_BLE_GAP_PHY_CODED
Secondery Advertisement PHY is LE Coded 

```

#### ESP_BLE_GAP_NO_PREFER_TRANSMIT_PHY


```


ESP_BLE_GAP_NO_PREFER_TRANSMIT_PHY
No Prefer TX PHY supported by controller 

```

#### ESP_BLE_GAP_NO_PREFER_RECEIVE_PHY


```


ESP_BLE_GAP_NO_PREFER_RECEIVE_PHY
No Prefer RX PHY supported by controller 

```

#### ESP_BLE_GAP_PRI_PHY_1M


```


ESP_BLE_GAP_PRI_PHY_1M
Primary phy only support 1M and LE coded phy. 
Primary Phy is LE1M 

```

#### ESP_BLE_GAP_PRI_PHY_CODED


```


ESP_BLE_GAP_PRI_PHY_CODED
Primary Phy is LE CODED 

```

#### ESP_BLE_GAP_PHY_1M_PREF_MASK


```


ESP_BLE_GAP_PHY_1M_PREF_MASK
The Host prefers use the LE1M transmitter or receiver PHY 

```

#### ESP_BLE_GAP_PHY_2M_PREF_MASK


```


ESP_BLE_GAP_PHY_2M_PREF_MASK
The Host prefers use the LE2M transmitter or receiver PHY 

```

#### ESP_BLE_GAP_PHY_CODED_PREF_MASK


```


ESP_BLE_GAP_PHY_CODED_PREF_MASK
The Host prefers use the LE CODED transmitter or receiver PHY 

```

#### ESP_BLE_GAP_PHY_OPTIONS_NO_PREF


```


ESP_BLE_GAP_PHY_OPTIONS_NO_PREF
The Host has no preferred coding when transmitting on the LE Coded PHY 

```

#### ESP_BLE_GAP_PHY_OPTIONS_PREF_S2_CODING


```


ESP_BLE_GAP_PHY_OPTIONS_PREF_S2_CODING
The Host prefers that S=2 coding be used when transmitting on the LE Coded PHY 

```

#### ESP_BLE_GAP_PHY_OPTIONS_PREF_S8_CODING


```


ESP_BLE_GAP_PHY_OPTIONS_PREF_S8_CODING
The Host prefers that S=8 coding be used when transmitting on the LE Coded PHY 

```

#### ESP_BLE_GAP_EXT_SCAN_CFG_UNCODE_MASK


```


ESP_BLE_GAP_EXT_SCAN_CFG_UNCODE_MASK
Scan Advertisements on the LE1M PHY 

```

#### ESP_BLE_GAP_EXT_SCAN_CFG_CODE_MASK


```


ESP_BLE_GAP_EXT_SCAN_CFG_CODE_MASK
Scan advertisements on the LE coded PHY 

```

#### ESP_BLE_GAP_EXT_ADV_DATA_COMPLETE


```


ESP_BLE_GAP_EXT_ADV_DATA_COMPLETE
Advertising data. 
extended advertising data compete 

```

#### ESP_BLE_GAP_EXT_ADV_DATA_INCOMPLETE


```


ESP_BLE_GAP_EXT_ADV_DATA_INCOMPLETE
extended advertising data incomplete 

```

#### ESP_BLE_GAP_EXT_ADV_DATA_TRUNCATED


```


ESP_BLE_GAP_EXT_ADV_DATA_TRUNCATED
extended advertising data truncated mode 

```

#### ESP_BLE_GAP_SYNC_POLICY_BY_ADV_INFO


```


ESP_BLE_GAP_SYNC_POLICY_BY_ADV_INFO
Advertising SYNC policy. 
sync policy by advertising info 

```

#### ESP_BLE_GAP_SYNC_POLICY_BY_PERIODIC_LIST


```


ESP_BLE_GAP_SYNC_POLICY_BY_PERIODIC_LIST
periodic advertising sync policy 

```

#### ESP_BLE_ADV_REPORT_EXT_ADV_IND


```


ESP_BLE_ADV_REPORT_EXT_ADV_IND
Advertising report. 
advertising report with extended advertising indication type 

```

#### ESP_BLE_ADV_REPORT_EXT_SCAN_IND


```


ESP_BLE_ADV_REPORT_EXT_SCAN_IND
advertising report with extended scan indication type 

```

#### ESP_BLE_ADV_REPORT_EXT_DIRECT_ADV


```


ESP_BLE_ADV_REPORT_EXT_DIRECT_ADV
advertising report with extended direct advertising indication type 

```

#### ESP_BLE_ADV_REPORT_EXT_SCAN_RSP


```


ESP_BLE_ADV_REPORT_EXT_SCAN_RSP
advertising report with extended scan response indication type Bluetooth 5.0, Vol 2, Part E, 7.7.65.13 

```

#### ESP_BLE_LEGACY_ADV_TYPE_IND


```


ESP_BLE_LEGACY_ADV_TYPE_IND
advertising report with legacy advertising indication type 

```

#### ESP_BLE_LEGACY_ADV_TYPE_DIRECT_IND


```


ESP_BLE_LEGACY_ADV_TYPE_DIRECT_IND
advertising report with legacy direct indication type 

```

#### ESP_BLE_LEGACY_ADV_TYPE_SCAN_IND


```


ESP_BLE_LEGACY_ADV_TYPE_SCAN_IND
advertising report with legacy scan indication type 

```

#### ESP_BLE_LEGACY_ADV_TYPE_NONCON_IND


```


ESP_BLE_LEGACY_ADV_TYPE_NONCON_IND
advertising report with legacy non connectable indication type 

```

#### ESP_BLE_LEGACY_ADV_TYPE_SCAN_RSP_TO_ADV_IND


```


ESP_BLE_LEGACY_ADV_TYPE_SCAN_RSP_TO_ADV_IND
advertising report with legacy scan response indication type 

```

#### ESP_BLE_LEGACY_ADV_TYPE_SCAN_RSP_TO_ADV_SCAN_IND


```


ESP_BLE_LEGACY_ADV_TYPE_SCAN_RSP_TO_ADV_SCAN_IND
advertising report with legacy advertising with scan response indication type 

```

#### EXT_ADV_TX_PWR_NO_PREFERENCE


```


EXT_ADV_TX_PWR_NO_PREFERENCE
Extend advertising tx power, range: [-127, +126] dBm. 
host has no preference for tx power 

```

#### EXT_ADV_NUM_SETS_MAX


```


EXT_ADV_NUM_SETS_MAX
max number of advertising sets to enable or disable 
max evt instance num 

```

#### ESP_BLE_GAP_PAST_MODE_NO_SYNC_EVT


```


ESP_BLE_GAP_PAST_MODE_NO_SYNC_EVT
Periodic advertising sync trans mode. 
No attempt is made to sync and no periodic adv sync transfer received event 

```

#### ESP_BLE_GAP_PAST_MODE_NO_REPORT_EVT


```


ESP_BLE_GAP_PAST_MODE_NO_REPORT_EVT
An periodic adv sync transfer received event and no periodic adv report events 

```

#### ESP_BLE_GAP_PAST_MODE_DUP_FILTER_DISABLED


```


ESP_BLE_GAP_PAST_MODE_DUP_FILTER_DISABLED
Periodic adv report events will be enabled with duplicate filtering disabled 

```

#### ESP_BLE_GAP_PAST_MODE_DUP_FILTER_ENABLED


```


ESP_BLE_GAP_PAST_MODE_DUP_FILTER_ENABLED
Periodic adv report events will be enabled with duplicate filtering enabled 

```



### Type Definitions


#### esp_ble_key_type_t


```


typedef uint8_t esp_ble_key_type_t

```

#### esp_ble_auth_req_t


```


typedef uint8_t esp_ble_auth_req_t
combination of the above bit pattern 

```

#### esp_ble_io_cap_t


```


typedef uint8_t esp_ble_io_cap_t
combination of the io capability 

```

#### esp_ble_dtm_pkt_payload_t


```


typedef uint8_t esp_ble_dtm_pkt_payload_t

```

#### esp_gap_ble_channels


```


typedef uint8_t esp_gap_ble_channels[ESP_GAP_BLE_CHANNELS_LEN]

```

#### esp_duplicate_info_t


```


typedef uint8_t esp_duplicate_info_t[ESP_BD_ADDR_LEN]

```

#### esp_ble_ext_adv_type_mask_t


```


typedef uint16_t esp_ble_ext_adv_type_mask_t

```

#### esp_ble_gap_phy_t


```


typedef uint8_t esp_ble_gap_phy_t

```

#### esp_ble_gap_all_phys_t


```


typedef uint8_t esp_ble_gap_all_phys_t

```

#### esp_ble_gap_pri_phy_t


```


typedef uint8_t esp_ble_gap_pri_phy_t

```

#### esp_ble_gap_phy_mask_t


```


typedef uint8_t esp_ble_gap_phy_mask_t

```

#### esp_ble_gap_prefer_phy_options_t


```


typedef uint16_t esp_ble_gap_prefer_phy_options_t

```

#### esp_ble_ext_scan_cfg_mask_t


```


typedef uint8_t esp_ble_ext_scan_cfg_mask_t

```

#### esp_ble_gap_ext_adv_data_status_t


```


typedef uint8_t esp_ble_gap_ext_adv_data_status_t

```

#### esp_ble_gap_sync_t


```


typedef uint8_t esp_ble_gap_sync_t

```

#### esp_ble_gap_adv_type_t


```


typedef uint8_t esp_ble_gap_adv_type_t

```

#### esp_ble_gap_past_mode_t


```


typedef uint8_t esp_ble_gap_past_mode_t

```

#### esp_gap_ble_cb_t


```


typedef void (*esp_gap_ble_cb_t)(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
GAP callback function type. 

Param event
: Event type 

Param param
: Point to callback parameter, currently is union type 



```



### Enumerations


#### esp_gap_ble_cb_event_t


```


enum esp_gap_ble_cb_event_t
GAP BLE callback event type. 
Values:
ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT
When advertising data set complete, the event comes 

ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT
When scan response data set complete, the event comes 

ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT
When scan parameters set complete, the event comes 

ESP_GAP_BLE_SCAN_RESULT_EVT

enumerator ESP_GAP_BLE_SCAN_RESULT_EVT
When one scan result ready, the event comes each time 

ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT
When raw advertising data set complete, the event comes 

ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT
When raw scan response data set complete, the event comes 

ESP_GAP_BLE_ADV_START_COMPLETE_EVT

enumerator ESP_GAP_BLE_ADV_START_COMPLETE_EVT
When start advertising complete, the event comes 

ESP_GAP_BLE_SCAN_START_COMPLETE_EVT

enumerator ESP_GAP_BLE_SCAN_START_COMPLETE_EVT
When start scan complete, the event comes 

ESP_GAP_BLE_AUTH_CMPL_EVT

enumerator ESP_GAP_BLE_AUTH_CMPL_EVT
Authentication complete indication. 

ESP_GAP_BLE_KEY_EVT

enumerator ESP_GAP_BLE_KEY_EVT
BLE key event for peer device keys 

ESP_GAP_BLE_SEC_REQ_EVT

enumerator ESP_GAP_BLE_SEC_REQ_EVT
BLE security request 

ESP_GAP_BLE_PASSKEY_NOTIF_EVT

enumerator ESP_GAP_BLE_PASSKEY_NOTIF_EVT
passkey notification event 

ESP_GAP_BLE_PASSKEY_REQ_EVT

enumerator ESP_GAP_BLE_PASSKEY_REQ_EVT
passkey request event 

ESP_GAP_BLE_OOB_REQ_EVT

enumerator ESP_GAP_BLE_OOB_REQ_EVT
OOB request event 

ESP_GAP_BLE_LOCAL_IR_EVT

enumerator ESP_GAP_BLE_LOCAL_IR_EVT
BLE local IR (identity Root 128-bit random static value used to generate Long Term Key) event 

ESP_GAP_BLE_LOCAL_ER_EVT

enumerator ESP_GAP_BLE_LOCAL_ER_EVT
BLE local ER (Encryption Root value used to generate identity resolving key) event 

ESP_GAP_BLE_NC_REQ_EVT

enumerator ESP_GAP_BLE_NC_REQ_EVT
Numeric Comparison request event 

ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT

enumerator ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT
When stop adv complete, the event comes 

ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT

enumerator ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT
When stop scan complete, the event comes 

ESP_GAP_BLE_SET_STATIC_RAND_ADDR_EVT

enumerator ESP_GAP_BLE_SET_STATIC_RAND_ADDR_EVT
When set the static rand address complete, the event comes 

ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT

enumerator ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT
When update connection parameters complete, the event comes 

ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT
When set pkt length complete, the event comes 

ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT
When Enable/disable privacy on the local device complete, the event comes 

ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT

enumerator ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT
When remove the bond device complete, the event comes 

ESP_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT

enumerator ESP_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT
When clear the bond device clear complete, the event comes 

ESP_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT

enumerator ESP_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT
When get the bond device list complete, the event comes 

ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT

enumerator ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT
When read the rssi complete, the event comes 

ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT

enumerator ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT
When add or remove whitelist complete, the event comes 

ESP_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT

enumerator ESP_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT
When update duplicate exceptional list complete, the event comes 

ESP_GAP_BLE_SET_CHANNELS_EVT

enumerator ESP_GAP_BLE_SET_CHANNELS_EVT
When setting BLE channels complete, the event comes 

ESP_GAP_BLE_READ_PHY_COMPLETE_EVT

enumerator ESP_GAP_BLE_READ_PHY_COMPLETE_EVT
when reading phy complete, this event comes 

ESP_GAP_BLE_SET_PREFERRED_DEFAULT_PHY_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_PREFERRED_DEFAULT_PHY_COMPLETE_EVT
when preferred default phy complete, this event comes 

ESP_GAP_BLE_SET_PREFERRED_PHY_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_PREFERRED_PHY_COMPLETE_EVT
when preferred phy complete , this event comes 

ESP_GAP_BLE_EXT_ADV_SET_RAND_ADDR_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_ADV_SET_RAND_ADDR_COMPLETE_EVT
when extended set random address complete, the event comes 

ESP_GAP_BLE_EXT_ADV_SET_PARAMS_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_ADV_SET_PARAMS_COMPLETE_EVT
when extended advertising parameter complete, the event comes 

ESP_GAP_BLE_EXT_ADV_DATA_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_ADV_DATA_SET_COMPLETE_EVT
when extended advertising data complete, the event comes 

ESP_GAP_BLE_EXT_SCAN_RSP_DATA_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_SCAN_RSP_DATA_SET_COMPLETE_EVT
when extended scan response data complete, the event comes 

ESP_GAP_BLE_EXT_ADV_START_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_ADV_START_COMPLETE_EVT
when extended advertising start complete, the event comes 

ESP_GAP_BLE_EXT_ADV_STOP_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_ADV_STOP_COMPLETE_EVT
when extended advertising stop complete, the event comes 

ESP_GAP_BLE_EXT_ADV_SET_REMOVE_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_ADV_SET_REMOVE_COMPLETE_EVT
when extended advertising set remove complete, the event comes 

ESP_GAP_BLE_EXT_ADV_SET_CLEAR_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_ADV_SET_CLEAR_COMPLETE_EVT
when extended advertising set clear complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SET_PARAMS_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SET_PARAMS_COMPLETE_EVT
when periodic advertising parameter complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_DATA_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_DATA_SET_COMPLETE_EVT
when periodic advertising data complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_START_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_START_COMPLETE_EVT
when periodic advertising start complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_STOP_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_STOP_COMPLETE_EVT
when periodic advertising stop complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_CREATE_SYNC_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_CREATE_SYNC_COMPLETE_EVT
when periodic advertising create sync complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SYNC_CANCEL_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SYNC_CANCEL_COMPLETE_EVT
when extended advertising sync cancel complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SYNC_TERMINATE_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SYNC_TERMINATE_COMPLETE_EVT
when extended advertising sync terminate complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_ADD_DEV_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_ADD_DEV_COMPLETE_EVT
when extended advertising add device complete , the event comes 

ESP_GAP_BLE_PERIODIC_ADV_REMOVE_DEV_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_REMOVE_DEV_COMPLETE_EVT
when extended advertising remove device complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_CLEAR_DEV_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_CLEAR_DEV_COMPLETE_EVT
when extended advertising clear device, the event comes 

ESP_GAP_BLE_SET_EXT_SCAN_PARAMS_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_EXT_SCAN_PARAMS_COMPLETE_EVT
when extended scan parameter complete, the event comes 

ESP_GAP_BLE_EXT_SCAN_START_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_SCAN_START_COMPLETE_EVT
when extended scan start complete, the event comes 

ESP_GAP_BLE_EXT_SCAN_STOP_COMPLETE_EVT

enumerator ESP_GAP_BLE_EXT_SCAN_STOP_COMPLETE_EVT
when extended scan stop complete, the event comes 

ESP_GAP_BLE_PREFER_EXT_CONN_PARAMS_SET_COMPLETE_EVT

enumerator ESP_GAP_BLE_PREFER_EXT_CONN_PARAMS_SET_COMPLETE_EVT
when extended prefer connection parameter set complete, the event comes 

ESP_GAP_BLE_PHY_UPDATE_COMPLETE_EVT

enumerator ESP_GAP_BLE_PHY_UPDATE_COMPLETE_EVT
when ble phy update complete, the event comes 

ESP_GAP_BLE_EXT_ADV_REPORT_EVT

enumerator ESP_GAP_BLE_EXT_ADV_REPORT_EVT
when extended advertising report complete, the event comes 

ESP_GAP_BLE_SCAN_TIMEOUT_EVT

enumerator ESP_GAP_BLE_SCAN_TIMEOUT_EVT
when scan timeout complete, the event comes 

ESP_GAP_BLE_ADV_TERMINATED_EVT

enumerator ESP_GAP_BLE_ADV_TERMINATED_EVT
when advertising terminate data complete, the event comes 

ESP_GAP_BLE_SCAN_REQ_RECEIVED_EVT

enumerator ESP_GAP_BLE_SCAN_REQ_RECEIVED_EVT
when scan req received complete, the event comes 

ESP_GAP_BLE_CHANNEL_SELECT_ALGORITHM_EVT

enumerator ESP_GAP_BLE_CHANNEL_SELECT_ALGORITHM_EVT
when channel select algorithm complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_REPORT_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_REPORT_EVT
when periodic report advertising complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SYNC_LOST_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SYNC_LOST_EVT
when periodic advertising sync lost complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SYNC_ESTAB_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SYNC_ESTAB_EVT
when periodic advertising sync establish complete, the event comes 

ESP_GAP_BLE_SC_OOB_REQ_EVT

enumerator ESP_GAP_BLE_SC_OOB_REQ_EVT
Secure Connection OOB request event 

ESP_GAP_BLE_SC_CR_LOC_OOB_EVT

enumerator ESP_GAP_BLE_SC_CR_LOC_OOB_EVT
Secure Connection create OOB data complete event 

ESP_GAP_BLE_GET_DEV_NAME_COMPLETE_EVT

enumerator ESP_GAP_BLE_GET_DEV_NAME_COMPLETE_EVT
When getting BT device name complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_RECV_ENABLE_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_RECV_ENABLE_COMPLETE_EVT
when set periodic advertising receive enable complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_COMPLETE_EVT
when periodic advertising sync transfer complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SET_INFO_TRANS_COMPLETE_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SET_INFO_TRANS_COMPLETE_EVT
when periodic advertising set info transfer complete, the event comes 

ESP_GAP_BLE_SET_PAST_PARAMS_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_PAST_PARAMS_COMPLETE_EVT
when set periodic advertising sync transfer params complete, the event comes 

ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_RECV_EVT

enumerator ESP_GAP_BLE_PERIODIC_ADV_SYNC_TRANS_RECV_EVT
when periodic advertising sync transfer received, the event comes 

ESP_GAP_BLE_DTM_TEST_UPDATE_EVT

enumerator ESP_GAP_BLE_DTM_TEST_UPDATE_EVT
when direct test mode state changes, the event comes 

ESP_GAP_BLE_ADV_CLEAR_COMPLETE_EVT

enumerator ESP_GAP_BLE_ADV_CLEAR_COMPLETE_EVT
When clear advertising complete, the event comes 

ESP_GAP_BLE_SET_RPA_TIMEOUT_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_RPA_TIMEOUT_COMPLETE_EVT
When set the Resolvable Private Address (RPA) timeout completes, the event comes 

ESP_GAP_BLE_ADD_DEV_TO_RESOLVING_LIST_COMPLETE_EVT

enumerator ESP_GAP_BLE_ADD_DEV_TO_RESOLVING_LIST_COMPLETE_EVT
when add a device to the resolving list completes, the event comes 

ESP_GAP_BLE_VENDOR_CMD_COMPLETE_EVT

enumerator ESP_GAP_BLE_VENDOR_CMD_COMPLETE_EVT
When vendor hci command complete, the event comes 

ESP_GAP_BLE_SET_PRIVACY_MODE_COMPLETE_EVT

enumerator ESP_GAP_BLE_SET_PRIVACY_MODE_COMPLETE_EVT
When set privacy mode complete, the event comes 

ESP_GAP_BLE_EVT_MAX

enumerator ESP_GAP_BLE_EVT_MAX
when maximum advertising event complete, the event comes 


```

#### esp_ble_adv_data_type


```


enum esp_ble_adv_data_type
The type of advertising data(not adv_type) 
Values:
ESP_BLE_AD_TYPE_FLAG

enumerator ESP_BLE_AD_TYPE_FLAG

ESP_BLE_AD_TYPE_16SRV_PART

enumerator ESP_BLE_AD_TYPE_16SRV_PART

ESP_BLE_AD_TYPE_16SRV_CMPL

enumerator ESP_BLE_AD_TYPE_16SRV_CMPL

ESP_BLE_AD_TYPE_32SRV_PART

enumerator ESP_BLE_AD_TYPE_32SRV_PART

ESP_BLE_AD_TYPE_32SRV_CMPL

enumerator ESP_BLE_AD_TYPE_32SRV_CMPL

ESP_BLE_AD_TYPE_128SRV_PART

enumerator ESP_BLE_AD_TYPE_128SRV_PART

ESP_BLE_AD_TYPE_128SRV_CMPL

enumerator ESP_BLE_AD_TYPE_128SRV_CMPL

ESP_BLE_AD_TYPE_NAME_SHORT

enumerator ESP_BLE_AD_TYPE_NAME_SHORT

ESP_BLE_AD_TYPE_NAME_CMPL

enumerator ESP_BLE_AD_TYPE_NAME_CMPL

ESP_BLE_AD_TYPE_TX_PWR

enumerator ESP_BLE_AD_TYPE_TX_PWR

ESP_BLE_AD_TYPE_DEV_CLASS

enumerator ESP_BLE_AD_TYPE_DEV_CLASS

ESP_BLE_AD_TYPE_SM_TK

enumerator ESP_BLE_AD_TYPE_SM_TK

ESP_BLE_AD_TYPE_SM_OOB_FLAG

enumerator ESP_BLE_AD_TYPE_SM_OOB_FLAG

ESP_BLE_AD_TYPE_INT_RANGE

enumerator ESP_BLE_AD_TYPE_INT_RANGE

ESP_BLE_AD_TYPE_SOL_SRV_UUID

enumerator ESP_BLE_AD_TYPE_SOL_SRV_UUID

ESP_BLE_AD_TYPE_128SOL_SRV_UUID

enumerator ESP_BLE_AD_TYPE_128SOL_SRV_UUID

ESP_BLE_AD_TYPE_SERVICE_DATA

enumerator ESP_BLE_AD_TYPE_SERVICE_DATA

ESP_BLE_AD_TYPE_PUBLIC_TARGET

enumerator ESP_BLE_AD_TYPE_PUBLIC_TARGET

ESP_BLE_AD_TYPE_RANDOM_TARGET

enumerator ESP_BLE_AD_TYPE_RANDOM_TARGET

ESP_BLE_AD_TYPE_APPEARANCE

enumerator ESP_BLE_AD_TYPE_APPEARANCE

ESP_BLE_AD_TYPE_ADV_INT

enumerator ESP_BLE_AD_TYPE_ADV_INT

ESP_BLE_AD_TYPE_LE_DEV_ADDR

enumerator ESP_BLE_AD_TYPE_LE_DEV_ADDR

ESP_BLE_AD_TYPE_LE_ROLE

enumerator ESP_BLE_AD_TYPE_LE_ROLE

ESP_BLE_AD_TYPE_SPAIR_C256

enumerator ESP_BLE_AD_TYPE_SPAIR_C256

ESP_BLE_AD_TYPE_SPAIR_R256

enumerator ESP_BLE_AD_TYPE_SPAIR_R256

ESP_BLE_AD_TYPE_32SOL_SRV_UUID

enumerator ESP_BLE_AD_TYPE_32SOL_SRV_UUID

ESP_BLE_AD_TYPE_32SERVICE_DATA

enumerator ESP_BLE_AD_TYPE_32SERVICE_DATA

ESP_BLE_AD_TYPE_128SERVICE_DATA

enumerator ESP_BLE_AD_TYPE_128SERVICE_DATA

ESP_BLE_AD_TYPE_LE_SECURE_CONFIRM

enumerator ESP_BLE_AD_TYPE_LE_SECURE_CONFIRM

ESP_BLE_AD_TYPE_LE_SECURE_RANDOM

enumerator ESP_BLE_AD_TYPE_LE_SECURE_RANDOM

ESP_BLE_AD_TYPE_URI

enumerator ESP_BLE_AD_TYPE_URI

ESP_BLE_AD_TYPE_INDOOR_POSITION

enumerator ESP_BLE_AD_TYPE_INDOOR_POSITION

ESP_BLE_AD_TYPE_TRANS_DISC_DATA

enumerator ESP_BLE_AD_TYPE_TRANS_DISC_DATA

ESP_BLE_AD_TYPE_LE_SUPPORT_FEATURE

enumerator ESP_BLE_AD_TYPE_LE_SUPPORT_FEATURE

ESP_BLE_AD_TYPE_CHAN_MAP_UPDATE

enumerator ESP_BLE_AD_TYPE_CHAN_MAP_UPDATE

ESP_BLE_AD_MANUFACTURER_SPECIFIC_TYPE

enumerator ESP_BLE_AD_MANUFACTURER_SPECIFIC_TYPE


```

#### esp_ble_adv_type_t


```


enum esp_ble_adv_type_t
Advertising mode. 
Values:
ADV_TYPE_IND

enumerator ADV_TYPE_IND

ADV_TYPE_DIRECT_IND_HIGH

enumerator ADV_TYPE_DIRECT_IND_HIGH

ADV_TYPE_SCAN_IND

enumerator ADV_TYPE_SCAN_IND

ADV_TYPE_NONCONN_IND

enumerator ADV_TYPE_NONCONN_IND

ADV_TYPE_DIRECT_IND_LOW

enumerator ADV_TYPE_DIRECT_IND_LOW


```

#### esp_ble_adv_channel_t


```


enum esp_ble_adv_channel_t
Advertising channel mask. 
Values:
ADV_CHNL_37

enumerator ADV_CHNL_37

ADV_CHNL_38

enumerator ADV_CHNL_38

ADV_CHNL_39

enumerator ADV_CHNL_39

ADV_CHNL_ALL

enumerator ADV_CHNL_ALL


```

#### esp_ble_adv_filter_t


```


enum esp_ble_adv_filter_t
Values:
ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY

enumerator ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
Allow both scan and connection requests from anyone. 

ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY

enumerator ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY
Allow both scan req from White List devices only and connection req from anyone. 

ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST

enumerator ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST
Allow both scan req from anyone and connection req from White List devices only. 

ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST

enumerator ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST
Allow scan and connection requests from White List devices only. 


```

#### esp_ble_sec_act_t


```


enum esp_ble_sec_act_t
Values:
ESP_BLE_SEC_ENCRYPT

enumerator ESP_BLE_SEC_ENCRYPT
relate to BTA_DM_BLE_SEC_ENCRYPT in bta/bta_api.h. If the device has already bonded, the stack will used Long Term Key (LTK) to encrypt with the remote device directly. Else if the device hasn't bonded, the stack will used the default authentication request used the esp_ble_gap_set_security_param function set by the user. 

ESP_BLE_SEC_ENCRYPT_NO_MITM

enumerator ESP_BLE_SEC_ENCRYPT_NO_MITM
relate to BTA_DM_BLE_SEC_ENCRYPT_NO_MITM in bta/bta_api.h. If the device has been already bonded, the stack will check the LTK (Long Term Key) Whether the authentication request has been met, and if met, use the LTK to encrypt with the remote device directly, else re-pair with the remote device. Else if the device hasn't been bonded, the stack will use NO MITM authentication request in the current link instead of using the authreq in the esp_ble_gap_set_security_param function set by the user. 

ESP_BLE_SEC_ENCRYPT_MITM

enumerator ESP_BLE_SEC_ENCRYPT_MITM
relate to BTA_DM_BLE_SEC_ENCRYPT_MITM in bta/bta_api.h. If the device has been already bonded, the stack will check the LTK (Long Term Key) whether the authentication request has been met, and if met, use the LTK to encrypt with the remote device directly, else re-pair with the remote device. Else if the device hasn't been bonded, the stack will use MITM authentication request in the current link instead of using the authreq in the esp_ble_gap_set_security_param function set by the user. 


```

#### esp_ble_sm_param_t


```


enum esp_ble_sm_param_t
Values:
ESP_BLE_SM_PASSKEY

enumerator ESP_BLE_SM_PASSKEY
Authentication requirements of local device 

ESP_BLE_SM_AUTHEN_REQ_MODE

enumerator ESP_BLE_SM_AUTHEN_REQ_MODE
The IO capability of local device 

ESP_BLE_SM_IOCAP_MODE

enumerator ESP_BLE_SM_IOCAP_MODE
Initiator Key Distribution/Generation 

ESP_BLE_SM_SET_INIT_KEY

enumerator ESP_BLE_SM_SET_INIT_KEY
Responder Key Distribution/Generation 

ESP_BLE_SM_SET_RSP_KEY

enumerator ESP_BLE_SM_SET_RSP_KEY
Maximum Encryption key size to support 

ESP_BLE_SM_MAX_KEY_SIZE

enumerator ESP_BLE_SM_MAX_KEY_SIZE
Minimum Encryption key size requirement from Peer 

ESP_BLE_SM_MIN_KEY_SIZE

enumerator ESP_BLE_SM_MIN_KEY_SIZE
Set static Passkey 

ESP_BLE_SM_SET_STATIC_PASSKEY

enumerator ESP_BLE_SM_SET_STATIC_PASSKEY
Reset static Passkey 

ESP_BLE_SM_CLEAR_STATIC_PASSKEY

enumerator ESP_BLE_SM_CLEAR_STATIC_PASSKEY
Accept only specified SMP Authentication requirement 

ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH

enumerator ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH
Enable/Disable OOB support 

ESP_BLE_SM_OOB_SUPPORT

enumerator ESP_BLE_SM_OOB_SUPPORT
Appl encryption key size 

ESP_BLE_APP_ENC_KEY_SIZE

enumerator ESP_BLE_APP_ENC_KEY_SIZE
authentication max param 

ESP_BLE_SM_MAX_PARAM

enumerator ESP_BLE_SM_MAX_PARAM


```

#### esp_ble_dtm_update_evt_t


```


enum esp_ble_dtm_update_evt_t
Values:
DTM_TX_START_EVT

enumerator DTM_TX_START_EVT
DTM TX start event. 

DTM_RX_START_EVT

enumerator DTM_RX_START_EVT
DTM RX start event. 

DTM_TEST_STOP_EVT

enumerator DTM_TEST_STOP_EVT
DTM test end event. 


```

#### esp_ble_scan_type_t


```


enum esp_ble_scan_type_t
Ble scan type. 
Values:
BLE_SCAN_TYPE_PASSIVE

enumerator BLE_SCAN_TYPE_PASSIVE
Passive scan 

BLE_SCAN_TYPE_ACTIVE

enumerator BLE_SCAN_TYPE_ACTIVE
Active scan 


```

#### esp_ble_scan_filter_t


```


enum esp_ble_scan_filter_t
Ble scan filter type. 
Values:
BLE_SCAN_FILTER_ALLOW_ALL

enumerator BLE_SCAN_FILTER_ALLOW_ALL
Accept all :
advertisement packets except directed advertising packets not addressed to this device (default). 



BLE_SCAN_FILTER_ALLOW_ONLY_WLST

enumerator BLE_SCAN_FILTER_ALLOW_ONLY_WLST
Accept only :
advertisement packets from devices where the advertiser’s address is in the White list.
Directed advertising packets which are not addressed for this device shall be ignored. 



BLE_SCAN_FILTER_ALLOW_UND_RPA_DIR

enumerator BLE_SCAN_FILTER_ALLOW_UND_RPA_DIR
Accept all :
undirected advertisement packets, and
directed advertising packets where the initiator address is a resolvable private address, and
directed advertising packets addressed to this device. 



BLE_SCAN_FILTER_ALLOW_WLIST_RPA_DIR

enumerator BLE_SCAN_FILTER_ALLOW_WLIST_RPA_DIR
Accept all :
advertisement packets from devices where the advertiser’s address is in the White list, and
directed advertising packets where the initiator address is a resolvable private address, and
directed advertising packets addressed to this device. 




```

#### esp_ble_scan_duplicate_t


```


enum esp_ble_scan_duplicate_t
Ble scan duplicate type. 
Values:
BLE_SCAN_DUPLICATE_DISABLE

enumerator BLE_SCAN_DUPLICATE_DISABLE
the Link Layer should generate advertising reports to the host for each packet received 

BLE_SCAN_DUPLICATE_ENABLE

enumerator BLE_SCAN_DUPLICATE_ENABLE
the Link Layer should filter out duplicate advertising reports to the Host 

BLE_SCAN_DUPLICATE_ENABLE_RESET

enumerator BLE_SCAN_DUPLICATE_ENABLE_RESET
Duplicate filtering enabled, reset for each scan period, only supported in BLE 5.0. 

BLE_SCAN_DUPLICATE_MAX

enumerator BLE_SCAN_DUPLICATE_MAX
Reserved for future use. 


```

#### esp_ble_auth_fail_rsn_t


```


enum esp_ble_auth_fail_rsn_t
Definition of the authentication failed reason. 
Values:
ESP_AUTH_SMP_PASSKEY_FAIL

enumerator ESP_AUTH_SMP_PASSKEY_FAIL
The user input of passkey failed 

ESP_AUTH_SMP_OOB_FAIL

enumerator ESP_AUTH_SMP_OOB_FAIL
The OOB data is not available 

ESP_AUTH_SMP_PAIR_AUTH_FAIL

enumerator ESP_AUTH_SMP_PAIR_AUTH_FAIL
The authentication requirements cannot be met 

ESP_AUTH_SMP_CONFIRM_VALUE_FAIL

enumerator ESP_AUTH_SMP_CONFIRM_VALUE_FAIL
The confirm value does not match the calculated comparison value 

ESP_AUTH_SMP_PAIR_NOT_SUPPORT

enumerator ESP_AUTH_SMP_PAIR_NOT_SUPPORT
Pairing is not supported by the device 

ESP_AUTH_SMP_ENC_KEY_SIZE

enumerator ESP_AUTH_SMP_ENC_KEY_SIZE
The resultant encryption key size is not long enough 

ESP_AUTH_SMP_INVALID_CMD

enumerator ESP_AUTH_SMP_INVALID_CMD
The SMP command received is not supported by this device 

ESP_AUTH_SMP_UNKNOWN_ERR

enumerator ESP_AUTH_SMP_UNKNOWN_ERR
Pairing failed due to an unspecified reason 

ESP_AUTH_SMP_REPEATED_ATTEMPT

enumerator ESP_AUTH_SMP_REPEATED_ATTEMPT
Pairing or authentication procedure is disallowed 

ESP_AUTH_SMP_INVALID_PARAMETERS

enumerator ESP_AUTH_SMP_INVALID_PARAMETERS
The command length is invalid or that a parameter is outside the specified range 

ESP_AUTH_SMP_DHKEY_CHK_FAIL

enumerator ESP_AUTH_SMP_DHKEY_CHK_FAIL
The DHKey Check value received doesn’t match the one calculated by the local device 

ESP_AUTH_SMP_NUM_COMP_FAIL

enumerator ESP_AUTH_SMP_NUM_COMP_FAIL
The confirm values in the numeric comparison protocol do not match 

ESP_AUTH_SMP_BR_PARING_IN_PROGR

enumerator ESP_AUTH_SMP_BR_PARING_IN_PROGR
Pairing Request sent over the BR/EDR transport is in progress 

ESP_AUTH_SMP_XTRANS_DERIVE_NOT_ALLOW

enumerator ESP_AUTH_SMP_XTRANS_DERIVE_NOT_ALLOW
The BR/EDR Link Key or BLE LTK cannot be used to derive 

ESP_AUTH_SMP_INTERNAL_ERR

enumerator ESP_AUTH_SMP_INTERNAL_ERR
Internal error in pairing procedure 

ESP_AUTH_SMP_UNKNOWN_IO

enumerator ESP_AUTH_SMP_UNKNOWN_IO
Unknown IO capability, unable to decide association model 

ESP_AUTH_SMP_INIT_FAIL

enumerator ESP_AUTH_SMP_INIT_FAIL
SMP pairing initiation failed 

ESP_AUTH_SMP_CONFIRM_FAIL

enumerator ESP_AUTH_SMP_CONFIRM_FAIL
The confirm value does not match 

ESP_AUTH_SMP_BUSY

enumerator ESP_AUTH_SMP_BUSY
Pending security request on going 

ESP_AUTH_SMP_ENC_FAIL

enumerator ESP_AUTH_SMP_ENC_FAIL
The Controller failed to start encryption 

ESP_AUTH_SMP_STARTED

enumerator ESP_AUTH_SMP_STARTED
SMP pairing process started 

ESP_AUTH_SMP_RSP_TIMEOUT

enumerator ESP_AUTH_SMP_RSP_TIMEOUT
Security Manager timeout due to no SMP command being received 

ESP_AUTH_SMP_DIV_NOT_AVAIL

enumerator ESP_AUTH_SMP_DIV_NOT_AVAIL
Encrypted Diversifier value not available 

ESP_AUTH_SMP_UNSPEC_ERR

enumerator ESP_AUTH_SMP_UNSPEC_ERR
Unspecified failed reason 

ESP_AUTH_SMP_CONN_TOUT

enumerator ESP_AUTH_SMP_CONN_TOUT
Pairing process failed due to connection timeout 


```

#### esp_gap_search_evt_t


```


enum esp_gap_search_evt_t
Sub Event of ESP_GAP_BLE_SCAN_RESULT_EVT. 
Values:
ESP_GAP_SEARCH_INQ_RES_EVT

enumerator ESP_GAP_SEARCH_INQ_RES_EVT
Inquiry result for a peer device. 

ESP_GAP_SEARCH_INQ_CMPL_EVT

enumerator ESP_GAP_SEARCH_INQ_CMPL_EVT
Inquiry complete. 

ESP_GAP_SEARCH_DISC_RES_EVT

enumerator ESP_GAP_SEARCH_DISC_RES_EVT
Discovery result for a peer device. 

ESP_GAP_SEARCH_DISC_BLE_RES_EVT

enumerator ESP_GAP_SEARCH_DISC_BLE_RES_EVT
Discovery result for BLE GATT based service on a peer device. 

ESP_GAP_SEARCH_DISC_CMPL_EVT

enumerator ESP_GAP_SEARCH_DISC_CMPL_EVT
Discovery complete. 

ESP_GAP_SEARCH_DI_DISC_CMPL_EVT

enumerator ESP_GAP_SEARCH_DI_DISC_CMPL_EVT
Discovery complete. 

ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT

enumerator ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT
Search cancelled 

ESP_GAP_SEARCH_INQ_DISCARD_NUM_EVT

enumerator ESP_GAP_SEARCH_INQ_DISCARD_NUM_EVT
The number of pkt discarded by flow control 


```

#### esp_ble_evt_type_t


```


enum esp_ble_evt_type_t
Ble scan result event type, to indicate the result is scan response or advertising data or other. 
Values:
ESP_BLE_EVT_CONN_ADV

enumerator ESP_BLE_EVT_CONN_ADV
Connectable undirected advertising (ADV_IND) 

ESP_BLE_EVT_CONN_DIR_ADV

enumerator ESP_BLE_EVT_CONN_DIR_ADV
Connectable directed advertising (ADV_DIRECT_IND) 

ESP_BLE_EVT_DISC_ADV

enumerator ESP_BLE_EVT_DISC_ADV
Scannable undirected advertising (ADV_SCAN_IND) 

ESP_BLE_EVT_NON_CONN_ADV

enumerator ESP_BLE_EVT_NON_CONN_ADV
Non connectable undirected advertising (ADV_NONCONN_IND) 

ESP_BLE_EVT_SCAN_RSP

enumerator ESP_BLE_EVT_SCAN_RSP
Scan Response (SCAN_RSP) 


```

#### esp_ble_wl_operation_t


```


enum esp_ble_wl_operation_t
Values:
ESP_BLE_WHITELIST_REMOVE

enumerator ESP_BLE_WHITELIST_REMOVE
remove mac from whitelist 

ESP_BLE_WHITELIST_ADD

enumerator ESP_BLE_WHITELIST_ADD
add address to whitelist 

ESP_BLE_WHITELIST_CLEAR

enumerator ESP_BLE_WHITELIST_CLEAR
clear all device in whitelist 


```

#### esp_bt_duplicate_exceptional_subcode_type_t


```


enum esp_bt_duplicate_exceptional_subcode_type_t
Values:
ESP_BLE_DUPLICATE_EXCEPTIONAL_LIST_ADD

enumerator ESP_BLE_DUPLICATE_EXCEPTIONAL_LIST_ADD
Add device info into duplicate scan exceptional list 

ESP_BLE_DUPLICATE_EXCEPTIONAL_LIST_REMOVE

enumerator ESP_BLE_DUPLICATE_EXCEPTIONAL_LIST_REMOVE
Remove device info from duplicate scan exceptional list 

ESP_BLE_DUPLICATE_EXCEPTIONAL_LIST_CLEAN

enumerator ESP_BLE_DUPLICATE_EXCEPTIONAL_LIST_CLEAN
Clean duplicate scan exceptional list 


```

#### esp_ble_duplicate_exceptional_info_type_t


```


enum esp_ble_duplicate_exceptional_info_type_t
Values:
ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_ADV_ADDR

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_ADV_ADDR
BLE advertising address , device info will be added into ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ADDR_LIST 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_LINK_ID

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_LINK_ID
BLE mesh link ID, it is for BLE mesh, device info will be added into ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_LINK_ID_LIST 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_BEACON_TYPE

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_BEACON_TYPE
BLE mesh beacon AD type, the format is | Len | 0x2B | Beacon Type | Beacon Data | 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROV_SRV_ADV

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROV_SRV_ADV
BLE mesh provisioning service uuid, the format is | 0x02 | 0x01 | flags | 0x03 | 0x03 | 0x1827 | .... |` 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROXY_SRV_ADV

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROXY_SRV_ADV
BLE mesh adv with proxy service uuid, the format is | 0x02 | 0x01 | flags | 0x03 | 0x03 | 0x1828 | .... |` 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROXY_SOLIC_ADV

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROXY_SOLIC_ADV
BLE mesh adv with proxy service uuid, the format is | 0x02 | 0x01 | flags | 0x03 | 0x03 | 0x1859 | .... |` 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_URI_ADV

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_URI_ADV
BLE mesh URI adv, the format is ...| Len | 0x24 | data |... 


```

#### esp_duplicate_scan_exceptional_list_type_t


```


enum esp_duplicate_scan_exceptional_list_type_t
Values:
ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ADDR_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ADDR_LIST
duplicate scan exceptional addr list 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_LINK_ID_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_LINK_ID_LIST
duplicate scan exceptional mesh link ID list 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_BEACON_TYPE_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_BEACON_TYPE_LIST
duplicate scan exceptional mesh beacon type list 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROV_SRV_ADV_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROV_SRV_ADV_LIST
duplicate scan exceptional mesh adv with provisioning service uuid 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROXY_SRV_ADV_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROXY_SRV_ADV_LIST
duplicate scan exceptional mesh adv with proxy service uuid 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROXY_SOLIC_ADV_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROXY_SOLIC_ADV_LIST
duplicate scan exceptional mesh adv with proxy solicitation PDU uuid 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_URI_ADV_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_URI_ADV_LIST
duplicate scan exceptional URI list 

ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ALL_LIST

enumerator ESP_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ALL_LIST
duplicate scan exceptional all list 


```

#### esp_ble_privacy_mode_t


```


enum esp_ble_privacy_mode_t
Values:
ESP_BLE_NETWORK_PRIVACY_MODE

enumerator ESP_BLE_NETWORK_PRIVACY_MODE
Network Privacy Mode for peer device (default) 

ESP_BLE_DEVICE_PRIVACY_MODE

enumerator ESP_BLE_DEVICE_PRIVACY_MODE
Device Privacy Mode for peer device 


```