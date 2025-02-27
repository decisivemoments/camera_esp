# Wi-Fi




## Introduction


The Wi-Fi libraries provide support for configuring and monitoring the ESP32-S3 Wi-Fi networking functionality. This includes configuration for:


* Station mode (aka STA mode or Wi-Fi client mode). ESP32-S3 connects to an access point.
* AP mode (aka Soft-AP mode or Access Point mode). Stations connect to the ESP32-S3.
* Station/AP-coexistence mode (ESP32-S3 is concurrently an access point and a station connected to another access point).
* Various security modes for the above (WPA, WPA2, WPA3, etc.)
* Scanning for access points (active & passive scanning).
* Promiscuous mode for monitoring of IEEE802.11 Wi-Fi packets.




## Application Examples


Several application examples demonstrating the functionality of Wi-Fi library are provided in wifi directory of ESP-IDF repository. Please check the README  for more details.




## API Reference



### Header File


* components/esp_wifi/include/esp_wifi.h
* This header file can be included with:



> ```
> #include "esp_wifi.h"
> 
> ```
* This header file is a part of the API provided by the `esp_wifi` component. To declare that your component depends on `esp_wifi`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_wifi
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_wifi
> 
> ```




### Functions


#### esp_wifi_init


```


esp_err_t esp_wifi_init(const wifi_init_config_t *config)
Initialize WiFi Allocate resource for WiFi driver, such as WiFi control structure, RX/TX buffer, WiFi NVS structure etc. This WiFi also starts WiFi task. 

Attention1. This API must be called before all other WiFi API can be called 

Attention2. Always use WIFI_INIT_CONFIG_DEFAULT macro to initialize the configuration to default values, this can guarantee all the fields get correct value when more fields are added into wifi_init_config_t in future release. If you want to set your own initial values, overwrite the default values which are set by WIFI_INIT_CONFIG_DEFAULT. Please be notified that the field 'magic' of wifi_init_config_t should always be WIFI_INIT_CONFIG_MAGIC!




Parameters
config -- pointer to WiFi initialized configuration structure; can point to a temporary variable.

Returns

ESP_OK: succeed
ESP_ERR_NO_MEM: out of memory
others: refer to error code esp_err.h





```

#### esp_wifi_deinit


```


esp_err_t esp_wifi_deinit(void)
Deinit WiFi Free all resource allocated in esp_wifi_init and stop WiFi task. 

Attention1. This API should be called if you want to remove WiFi driver from the system




Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_set_mode


```


esp_err_t esp_wifi_set_mode(wifi_mode_t mode)
Set the WiFi operating mode. 
       Set the WiFi operating mode as station, soft-AP, station+soft-AP or NAN.
       The default mode is station mode.




Parameters
mode -- WiFi operating mode

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument
others: refer to error code in esp_err.h





```

#### esp_wifi_get_mode


```


esp_err_t esp_wifi_get_mode(wifi_mode_t *mode)
Get current operating mode of WiFi. 

Parameters
mode -- [out] store current WiFi mode

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_start


```


esp_err_t esp_wifi_start(void)
Start WiFi according to current configuration If mode is WIFI_MODE_STA, it creates station control block and starts station If mode is WIFI_MODE_AP, it creates soft-AP control block and starts soft-AP If mode is WIFI_MODE_APSTA, it creates soft-AP and station control block and starts soft-AP and station If mode is WIFI_MODE_NAN, it creates NAN control block and starts NAN. 

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: It doesn't normally happen, the function called inside the API was passed invalid argument, user should check if the wifi related config is correct
ESP_ERR_NO_MEM: out of memory
ESP_ERR_WIFI_CONN: WiFi internal error, station or soft-AP control block wrong
ESP_FAIL: other WiFi internal errors 





```

#### esp_wifi_stop


```


esp_err_t esp_wifi_stop(void)
Stop WiFi If mode is WIFI_MODE_STA, it stops station and frees station control block If mode is WIFI_MODE_AP, it stops soft-AP and frees soft-AP control block If mode is WIFI_MODE_APSTA, it stops station/soft-AP and frees station/soft-AP control block If mode is WIFI_MODE_NAN, it stops NAN and frees NAN control block. 

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_restore


```


esp_err_t esp_wifi_restore(void)
Restore WiFi stack persistent settings to default values. 
This function will reset settings made using the following APIs:
esp_wifi_set_bandwidth,
esp_wifi_set_protocol,
esp_wifi_set_config related
esp_wifi_set_mode



Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_connect


```


esp_err_t esp_wifi_connect(void)
Connect WiFi station to the AP. 

Attention1. This API only impact WIFI_MODE_STA or WIFI_MODE_APSTA mode 

Attention2. If station interface is connected to an AP, call esp_wifi_disconnect to disconnect. 

Attention3. The scanning triggered by esp_wifi_scan_start() will not be effective until connection between device and the AP is established. If device is scanning and connecting at the same time, it will abort scanning and return a warning message and error number ESP_ERR_WIFI_STATE. 

Attention4. This API attempts to connect to an Access Point (AP) only once. To enable reconnection in case of a connection failure, please use the 'failure_retry_cnt' feature in the 'wifi_sta_config_t'. Users are suggested to implement reconnection logic in their application for scenarios where the specified AP does not exist, or reconnection is desired after the device has received a disconnect event.




Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_WIFI_MODE: WiFi mode error
ESP_ERR_WIFI_CONN: WiFi internal error, station or soft-AP control block wrong
ESP_ERR_WIFI_SSID: SSID of AP which station connects is invalid 





```

#### esp_wifi_disconnect


```


esp_err_t esp_wifi_disconnect(void)
Disconnect WiFi station from the AP. 

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi was not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi was not started by esp_wifi_start
ESP_FAIL: other WiFi internal errors 





```

#### esp_wifi_clear_fast_connect


```


esp_err_t esp_wifi_clear_fast_connect(void)
Currently this API is just an stub API. 

Returns

ESP_OK: succeed
others: fail 





```

#### esp_wifi_deauth_sta


```


esp_err_t esp_wifi_deauth_sta(uint16_t aid)
deauthenticate all stations or associated id equals to aid 

Parameters
aid -- when aid is 0, deauthenticate all stations, otherwise deauthenticate station whose associated id is aid

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi was not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_WIFI_MODE: WiFi mode is wrong 





```

#### esp_wifi_scan_start


```


esp_err_t esp_wifi_scan_start(const wifi_scan_config_t *config, bool block)
Scan all available APs. 

AttentionIf this API is called, the found APs are stored in WiFi driver dynamic allocated memory. And then can be freed in esp_wifi_scan_get_ap_records(), esp_wifi_scan_get_ap_record() or esp_wifi_clear_ap_list(), so call any one to free the memory once the scan is done. 

AttentionThe values of maximum active scan time and passive scan time per channel are limited to 1500 milliseconds. Values above 1500ms may cause station to disconnect from AP and are not recommended.




Parameters

config -- configuration settings for scanning, if set to NULL default settings will be used of which default values are show_hidden:false, scan_type:active, scan_time.active.min:0, scan_time.active.max:120 milliseconds, scan_time.passive:360 milliseconds home_chan_dwell_time:30ms
block -- if block is true, this API will block the caller until the scan is done, otherwise it will return immediately


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi was not started by esp_wifi_start
ESP_ERR_WIFI_TIMEOUT: blocking scan is timeout
ESP_ERR_WIFI_STATE: wifi still connecting when invoke esp_wifi_scan_start
others: refer to error code in esp_err.h





```

#### esp_wifi_set_scan_parameters


```


esp_err_t esp_wifi_set_scan_parameters(const wifi_scan_default_params_t *config)
Set default parameters used for scanning by station. 

AttentionThe values set using this API are also used for scans used while connecting.

AttentionThe values of maximum active scan time and passive scan time per channel are limited to 1500 milliseconds.

AttentionThe home_chan_dwell_time needs to be a minimum of 30ms and a maximum of 150ms.

AttentionSet any of the parameters to 0 to indicate using the default parameters - scan_time.active.min : 0ms, scan_time.active.max : 120ms home_chan_dwell_time : 30ms scan_time.passive : 360ms

AttentionDefault values can be retrieved using the macro WIFI_SCAN_PARAMS_DEFAULT_CONFIG()

AttentionSet the config parameter to NULL to reset previously set scan parameters to their default values.




Parameters
config -- default configuration settings for all scans by stations

Returns

ESP_OK: succeed
ESP_FAIL: failed as station mode has not been started yet
ESP_ERR_INVALID_ARG: values provided do not satisfy the requirements
ESP_ERR_NOT_SUPPORTED: This API is not supported in AP mode yet
ESP_ERR_INVALID_STATE: a scan/connect is in progress right now, cannot change scan parameters
others: refer to error code in esp_err.h





```

#### esp_wifi_get_scan_parameters


```


esp_err_t esp_wifi_get_scan_parameters(wifi_scan_default_params_t *config)
Get default parameters used for scanning by station. 

Parameters
config -- structure variable within which scan default params will be stored

Returns

ESP_OK: succeed
ESP_ERR_INVALID_ARG: passed parameter does not point to a valid memory
others: refer to error code in esp_err.h





```

#### esp_wifi_scan_stop


```


esp_err_t esp_wifi_scan_stop(void)
Stop the scan in process. 

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start 





```

#### esp_wifi_scan_get_ap_num


```


esp_err_t esp_wifi_scan_get_ap_num(uint16_t *number)
Get number of APs found in last scan. 

AttentionThis API can only be called when the scan is completed, otherwise it may get wrong value.




Parameters
number -- [out] store number of APs found in last scan

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_scan_get_ap_records


```


esp_err_t esp_wifi_scan_get_ap_records(uint16_t *number, wifi_ap_record_t *ap_records)
Get AP list found in last scan. 

AttentionThis API will free all memory occupied by scanned AP list.




Parameters

number -- [inout] As input param, it stores max AP number ap_records can hold. As output param, it receives the actual AP number this API returns. 
ap_records -- wifi_ap_record_t array to hold the found APs


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_NO_MEM: out of memory 





```

#### esp_wifi_scan_get_ap_record


```


esp_err_t esp_wifi_scan_get_ap_record(wifi_ap_record_t *ap_record)
Get one AP record from the scanned AP list. 

AttentionDifferent from esp_wifi_scan_get_ap_records(), this API only gets one AP record from the scanned AP list each time. This API will free the memory of one AP record, if the user doesn't get all records in the scannned AP list, then needs to call esp_wifi_clear_ap_list() to free the remaining memory.




Parameters
ap_record -- [out] pointer to one AP record

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument
ESP_FAIL: scan APs is NULL, means all AP records fetched or no AP found 





```

#### esp_wifi_clear_ap_list


```


esp_err_t esp_wifi_clear_ap_list(void)
Clear AP list found in last scan. 

AttentionThis API will free all memory occupied by scanned AP list. When the obtained AP list fails, AP records must be cleared,otherwise it may cause memory leakage.




Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_WIFI_MODE: WiFi mode is wrong
ESP_ERR_INVALID_ARG: It doesn't normally happen, the function called inside the API was passed invalid argument, user should check if the wifi related config is correct 





```

#### esp_wifi_sta_get_ap_info


```


esp_err_t esp_wifi_sta_get_ap_info(wifi_ap_record_t *ap_info)
Get information of AP to which the device is associated with. 

AttentionWhen the obtained country information is empty, it means that the AP does not carry country information




Parameters
ap_info -- the wifi_ap_record_t to hold AP information sta can get the connected ap's phy mode info through the struct member phy_11b，phy_11g，phy_11n，phy_lr in the wifi_ap_record_t struct. For example, phy_11b = 1 imply that ap support 802.11b mode

Returns

ESP_OK: succeed
ESP_ERR_WIFI_CONN: The station interface don't initialized
ESP_ERR_WIFI_NOT_CONNECT: The station is in disconnect status 





```

#### esp_wifi_set_ps


```


esp_err_t esp_wifi_set_ps(wifi_ps_type_t type)
Set current WiFi power save type. 

AttentionDefault power save type is WIFI_PS_MIN_MODEM.




Parameters
type -- power save type

Returns
ESP_OK: succeed 



```

#### esp_wifi_get_ps


```


esp_err_t esp_wifi_get_ps(wifi_ps_type_t *type)
Get current WiFi power save type. 

AttentionDefault power save type is WIFI_PS_MIN_MODEM.




Parameters
type -- [out] store current power save type

Returns
ESP_OK: succeed 



```

#### esp_wifi_set_protocol


```


esp_err_t esp_wifi_set_protocol(wifi_interface_t ifx, uint8_t protocol_bitmap)
Set protocol type of specified interface The default protocol is (WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N). if CONFIG_SOC_WIFI_HE_SUPPORT and band is 2.4G, the default protocol is (WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_11AX). if CONFIG_SOC_WIFI_HE_SUPPORT and band is 5G, the default protocol is (WIFI_PROTOCOL_11A|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_11AC|WIFI_PROTOCOL_11AX). 

Attention2.4G: Support 802.11b or 802.11bg or 802.11bgn or 802.11bgnax or LR mode 5G: Support 802.11a or 802.11an or 802.11anac or 802.11anacax




Parameters

ifx -- interfaces 
protocol_bitmap -- WiFi protocol bitmap


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_IF: invalid interface
others: refer to error codes in esp_err.h





```

#### esp_wifi_get_protocol


```


esp_err_t esp_wifi_get_protocol(wifi_interface_t ifx, uint8_t *protocol_bitmap)
Get the current protocol bitmap of the specified interface. 

Parameters

ifx -- interface 
protocol_bitmap -- [out] store current WiFi protocol bitmap of interface ifx


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_IF: invalid interface
ESP_ERR_INVALID_ARG: invalid argument
others: refer to error codes in esp_err.h





```

#### esp_wifi_set_bandwidth


```


esp_err_t esp_wifi_set_bandwidth(wifi_interface_t ifx, wifi_bandwidth_t bw)
Set the bandwidth of specified interface. 

Attention1. API return false if try to configure an interface that is not enabled 

Attention2. WIFI_BW_HT40 is supported only when the interface support 11N




Parameters

ifx -- interface to be configured 
bw -- bandwidth


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_IF: invalid interface
ESP_ERR_INVALID_ARG: invalid argument
others: refer to error codes in esp_err.h





```

#### esp_wifi_get_bandwidth


```


esp_err_t esp_wifi_get_bandwidth(wifi_interface_t ifx, wifi_bandwidth_t *bw)
Get the bandwidth of specified interface. 

Attention1. API return false if try to get a interface that is not enable




Parameters

ifx -- interface to be configured 
bw -- [out] store bandwidth of interface ifx


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_IF: invalid interface
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_channel


```


esp_err_t esp_wifi_set_channel(uint8_t primary, wifi_second_chan_t second)
Set primary/secondary channel of device. 

Attention1. This API should be called after esp_wifi_start() and before esp_wifi_stop() 

Attention2. When device is in STA mode, this API should not be called when STA is scanning or connecting to an external AP 

Attention3. When device is in softAP mode, this API should not be called when softAP has connected to external STAs 

Attention4. When device is in STA+softAP mode, this API should not be called when in the scenarios described above 

Attention5. The channel info set by this API will not be stored in NVS. So If you want to remember the channel used before wifi stop, you need to call this API again after wifi start, or you can call esp_wifi_set_config() to store the channel info in NVS.




Parameters

primary -- for HT20, primary is the channel number, for HT40, primary is the primary channel 
second -- for HT20, second is ignored, for HT40, second is the second channel


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_IF: invalid interface
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start 





```

#### esp_wifi_get_channel


```


esp_err_t esp_wifi_get_channel(uint8_t *primary, wifi_second_chan_t *second)
Get the primary/secondary channel of device. 

Attention1. API return false if try to get a interface that is not enable




Parameters

primary -- store current primary channel 
second -- [out] store current second channel


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_country


```


esp_err_t esp_wifi_set_country(const wifi_country_t *country)
configure country info 

Attention1. It is discouraged to call this API since this doesn't validate the per-country rules, it's up to the user to fill in all fields according to local regulations. Please use esp_wifi_set_country_code instead. 

Attention2. The default country is "01" (world safe mode) {.cc="01", .schan=1, .nchan=11, .policy=WIFI_COUNTRY_POLICY_AUTO}. 

Attention3. The third octet of country code string is one of the following: ' ', 'O', 'I', 'X', otherwise it is considered as ' '. 

Attention4. When the country policy is WIFI_COUNTRY_POLICY_AUTO, the country info of the AP to which the station is connected is used. E.g. if the configured country info is {.cc="US", .schan=1, .nchan=11} and the country info of the AP to which the station is connected is {.cc="JP", .schan=1, .nchan=14} then the country info that will be used is {.cc="JP", .schan=1, .nchan=14}. If the station disconnected from the AP the country info is set back to the country info of the station automatically, {.cc="US", .schan=1, .nchan=11} in the example. 

Attention5. When the country policy is WIFI_COUNTRY_POLICY_MANUAL, then the configured country info is used always. 

Attention6. When the country info is changed because of configuration or because the station connects to a different external AP, the country IE in probe response/beacon of the soft-AP is also changed. 

Attention7. The country configuration is stored into flash. 

Attention8. When this API is called, the PHY init data will switch to the PHY init data type corresponding to the country info.




Parameters
country -- the configured country info

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_get_country


```


esp_err_t esp_wifi_get_country(wifi_country_t *country)
get the current country info 

Parameters
country -- country info

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_mac


```


esp_err_t esp_wifi_set_mac(wifi_interface_t ifx, const uint8_t mac[6])
Set MAC address of WiFi station, soft-AP or NAN interface. 

Attention1. This API can only be called when the interface is disabled 

Attention2. Above mentioned interfaces have different MAC addresses, do not set them to be the same. 

Attention3. The bit 0 of the first byte of MAC address can not be 1. For example, the MAC address can set to be "1a:XX:XX:XX:XX:XX", but can not be "15:XX:XX:XX:XX:XX".




Parameters

ifx -- interface 
mac -- the MAC address


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_WIFI_IF: invalid interface
ESP_ERR_WIFI_MAC: invalid mac address
ESP_ERR_WIFI_MODE: WiFi mode is wrong
others: refer to error codes in esp_err.h





```

#### esp_wifi_get_mac


```


esp_err_t esp_wifi_get_mac(wifi_interface_t ifx, uint8_t mac[6])
Get mac of specified interface. 

Parameters

ifx -- interface 
mac -- [out] store mac of the interface ifx


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_WIFI_IF: invalid interface 





```

#### esp_wifi_set_promiscuous_rx_cb


```


esp_err_t esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t cb)
Register the RX callback function in the promiscuous mode. 
Each time a packet is received, the registered callback function will be called.

Parameters
cb -- callback

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_set_promiscuous


```


esp_err_t esp_wifi_set_promiscuous(bool en)
Enable the promiscuous mode. 

Parameters
en -- false - disable, true - enable

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_get_promiscuous


```


esp_err_t esp_wifi_get_promiscuous(bool *en)
Get the promiscuous mode. 

Parameters
en -- [out] store the current status of promiscuous mode

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_promiscuous_filter


```


esp_err_t esp_wifi_set_promiscuous_filter(const wifi_promiscuous_filter_t *filter)
Enable the promiscuous mode packet type filter. 

Note
The default filter is to filter all packets except WIFI_PKT_MISC


Parameters
filter -- the packet type filtered in promiscuous mode.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_get_promiscuous_filter


```


esp_err_t esp_wifi_get_promiscuous_filter(wifi_promiscuous_filter_t *filter)
Get the promiscuous filter. 

Parameters
filter -- [out] store the current status of promiscuous filter

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_promiscuous_ctrl_filter


```


esp_err_t esp_wifi_set_promiscuous_ctrl_filter(const wifi_promiscuous_filter_t *filter)
Enable subtype filter of the control packet in promiscuous mode. 

Note
The default filter is to filter none control packet.


Parameters
filter -- the subtype of the control packet filtered in promiscuous mode.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_get_promiscuous_ctrl_filter


```


esp_err_t esp_wifi_get_promiscuous_ctrl_filter(wifi_promiscuous_filter_t *filter)
Get the subtype filter of the control packet in promiscuous mode. 

Parameters
filter -- [out] store the current status of subtype filter of the control packet in promiscuous mode

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_config


```


esp_err_t esp_wifi_set_config(wifi_interface_t interface, wifi_config_t *conf)
Set the configuration of the STA, AP or NAN. 

Attention1. This API can be called only when specified interface is enabled, otherwise, API fail 

Attention2. For station configuration, bssid_set needs to be 0; and it needs to be 1 only when users need to check the MAC address of the AP. 

Attention3. ESP devices are limited to only one channel, so when in the soft-AP+station mode, the soft-AP will adjust its channel automatically to be the same as the channel of the station. 

Attention4. The configuration will be stored in NVS for station and soft-AP




Parameters

interface -- interface 
conf -- station, soft-AP or NAN configuration


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_WIFI_IF: invalid interface
ESP_ERR_WIFI_MODE: invalid mode
ESP_ERR_WIFI_PASSWORD: invalid password
ESP_ERR_WIFI_NVS: WiFi internal NVS error
others: refer to the error code in esp_err.h





```

#### esp_wifi_get_config


```


esp_err_t esp_wifi_get_config(wifi_interface_t interface, wifi_config_t *conf)
Get configuration of specified interface. 

Parameters

interface -- interface 
conf -- [out] station or soft-AP configuration


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_WIFI_IF: invalid interface 





```

#### esp_wifi_ap_get_sta_list


```


esp_err_t esp_wifi_ap_get_sta_list(wifi_sta_list_t *sta)
Get STAs associated with soft-AP. 

AttentionSSC only API




Parameters
sta -- [out] station list ap can get the connected sta's phy mode info through the struct member phy_11b，phy_11g，phy_11n，phy_lr in the wifi_sta_info_t struct. For example, phy_11b = 1 imply that sta support 802.11b mode

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_WIFI_MODE: WiFi mode is wrong
ESP_ERR_WIFI_CONN: WiFi internal error, the station/soft-AP control block is invalid 





```

#### esp_wifi_ap_get_sta_aid


```


esp_err_t esp_wifi_ap_get_sta_aid(const uint8_t mac[6], uint16_t *aid)
Get AID of STA connected with soft-AP. 

Parameters

mac -- STA's mac address 
aid -- [out] Store the AID corresponding to STA mac


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument
ESP_ERR_NOT_FOUND: Requested resource not found
ESP_ERR_WIFI_MODE: WiFi mode is wrong
ESP_ERR_WIFI_CONN: WiFi internal error, the station/soft-AP control block is invalid 





```

#### esp_wifi_set_storage


```


esp_err_t esp_wifi_set_storage(wifi_storage_t storage)
Set the WiFi API configuration storage type. 

Attention1. The default value is WIFI_STORAGE_FLASH




Parameters
storage -- : storage type

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_vendor_ie


```


esp_err_t esp_wifi_set_vendor_ie(bool enable, wifi_vendor_ie_type_t type, wifi_vendor_ie_id_t idx, const void *vnd_ie)
Set 802.11 Vendor-Specific Information Element. 

Parameters

enable -- If true, specified IE is enabled. If false, specified IE is removed. 
type -- Information Element type. Determines the frame type to associate with the IE. 
idx -- Index to set or clear. Each IE type can be associated with up to two elements (indices 0 & 1). 
vnd_ie -- Pointer to vendor specific element data. First 6 bytes should be a header with fields matching vendor_ie_data_t. If enable is false, this argument is ignored and can be NULL. Data does not need to remain valid after the function returns.


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init()
ESP_ERR_INVALID_ARG: Invalid argument, including if first byte of vnd_ie is not WIFI_VENDOR_IE_ELEMENT_ID (0xDD) or second byte is an invalid length.
ESP_ERR_NO_MEM: Out of memory 





```

#### esp_wifi_set_vendor_ie_cb


```


esp_err_t esp_wifi_set_vendor_ie_cb(esp_vendor_ie_cb_t cb, void *ctx)
Register Vendor-Specific Information Element monitoring callback. 

Parameters

cb -- Callback function 
ctx -- Context argument, passed to callback function.


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_set_max_tx_power


```


esp_err_t esp_wifi_set_max_tx_power(int8_t power)
Set maximum transmitting power after WiFi start. 

Attention1. Maximum power before wifi startup is limited by PHY init data bin. 

Attention2. The value set by this API will be mapped to the max_tx_power of the structure wifi_country_t variable. 

Attention3. Mapping Table {Power, max_tx_power} = {{8, 2}, {20, 5}, {28, 7}, {34, 8}, {44, 11}, {52, 13}, {56, 14}, {60, 15}, {66, 16}, {72, 18}, {80, 20}}. 

Attention4. Param power unit is 0.25dBm, range is [8, 84] corresponding to 2dBm - 20dBm. 

Attention5. Relationship between set value and actual value. As follows: {set value range, actual value} = {{[8, 19],8}, {[20, 27],20}, {[28, 33],28}, {[34, 43],34}, {[44, 51],44}, {[52, 55],52}, {[56, 59],56}, {[60, 65],60}, {[66, 71],66}, {[72, 79],72}, {[80, 84],80}}.




Parameters
power -- Maximum WiFi transmitting power.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument, e.g. parameter is out of range 





```

#### esp_wifi_get_max_tx_power


```


esp_err_t esp_wifi_get_max_tx_power(int8_t *power)
Get maximum transmitting power after WiFi start. 

Parameters
power -- Maximum WiFi transmitting power, unit is 0.25dBm.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_set_event_mask


```


esp_err_t esp_wifi_set_event_mask(uint32_t mask)
Set mask to enable or disable some WiFi events. 

Attention1. Mask can be created by logical OR of various WIFI_EVENT_MASK_ constants. Events which have corresponding bit set in the mask will not be delivered to the system event handler. 

Attention2. Default WiFi event mask is WIFI_EVENT_MASK_AP_PROBEREQRECVED. 

Attention3. There may be lots of stations sending probe request data around. Don't unmask this event unless you need to receive probe request data.




Parameters
mask -- WiFi event mask.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_get_event_mask


```


esp_err_t esp_wifi_get_event_mask(uint32_t *mask)
Get mask of WiFi events. 

Parameters
mask -- WiFi event mask.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_80211_tx


```


esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq)
Send raw ieee80211 data. 

AttentionCurrently only support for sending beacon/probe request/probe response/action and non-QoS data frame




Parameters

ifx -- interface if the Wi-Fi mode is Station, the ifx should be WIFI_IF_STA. If the Wi-Fi mode is SoftAP, the ifx should be WIFI_IF_AP. If the Wi-Fi mode is Station+SoftAP, the ifx should be WIFI_IF_STA or WIFI_IF_AP. If the ifx is wrong, the API returns ESP_ERR_WIFI_IF. 
buffer -- raw ieee80211 buffer 
len -- the length of raw buffer, the len must be <= 1500 Bytes and >= 24 Bytes 
en_sys_seq -- indicate whether use the internal sequence number. If en_sys_seq is false, the sequence in raw buffer is unchanged, otherwise it will be overwritten by WiFi driver with the system sequence number. Generally, if esp_wifi_80211_tx is called before the Wi-Fi connection has been set up, both en_sys_seq==true and en_sys_seq==false are fine. However, if the API is called after the Wi-Fi connection has been set up, en_sys_seq must be true, otherwise ESP_ERR_INVALID_ARG is returned.


Returns

ESP_OK: success
ESP_ERR_WIFI_IF: Invalid interface
ESP_ERR_INVALID_ARG: Invalid parameter
ESP_ERR_WIFI_NO_MEM: out of memory 





```

#### esp_wifi_set_csi_rx_cb


```


esp_err_t esp_wifi_set_csi_rx_cb(wifi_csi_cb_t cb, void *ctx)
Register the RX callback function of CSI data. 
   Each time a CSI data is received, the callback function will be called.




Parameters

cb -- callback 
ctx -- context argument, passed to callback function


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init 





```

#### esp_wifi_set_csi_config


```


esp_err_t esp_wifi_set_csi_config(const wifi_csi_config_t *config)
Set CSI data configuration. 

return
ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start or promiscuous mode is not enabled
ESP_ERR_INVALID_ARG: invalid argument 



Parameters
config -- configuration



```

#### esp_wifi_get_csi_config


```


esp_err_t esp_wifi_get_csi_config(wifi_csi_config_t *config)
Get CSI data configuration. 

return
ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start or promiscuous mode is not enabled
ESP_ERR_INVALID_ARG: invalid argument 



Parameters
config -- configuration



```

#### esp_wifi_set_csi


```


esp_err_t esp_wifi_set_csi(bool en)
Enable or disable CSI. 

return
ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start or promiscuous mode is not enabled
ESP_ERR_INVALID_ARG: invalid argument 



Parameters
en -- true - enable, false - disable



```

#### esp_wifi_set_ant_gpio


```


esp_err_t esp_wifi_set_ant_gpio(const wifi_ant_gpio_config_t *config)
Set antenna GPIO configuration. 

Parameters
config -- Antenna GPIO configuration.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: Invalid argument, e.g. parameter is NULL, invalid GPIO number etc 





```

#### esp_wifi_get_ant_gpio


```


esp_err_t esp_wifi_get_ant_gpio(wifi_ant_gpio_config_t *config)
Get current antenna GPIO configuration. 

Parameters
config -- Antenna GPIO configuration.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument, e.g. parameter is NULL 





```

#### esp_wifi_set_ant


```


esp_err_t esp_wifi_set_ant(const wifi_ant_config_t *config)
Set antenna configuration. 

Parameters
config -- Antenna configuration.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: Invalid argument, e.g. parameter is NULL, invalid antenna mode or invalid GPIO number 





```

#### esp_wifi_get_ant


```


esp_err_t esp_wifi_get_ant(wifi_ant_config_t *config)
Get current antenna configuration. 

Parameters
config -- Antenna configuration.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument, e.g. parameter is NULL 





```

#### esp_wifi_get_tsf_time


```


int64_t esp_wifi_get_tsf_time(wifi_interface_t interface)
Get the TSF time In Station mode or SoftAP+Station mode if station is not connected or station doesn't receive at least one beacon after connected, will return 0. 

AttentionEnabling power save may cause the return value inaccurate, except WiFi modem sleep




Parameters
interface -- The interface whose tsf_time is to be retrieved.

Returns
0 or the TSF time 



```

#### esp_wifi_set_inactive_time


```


esp_err_t esp_wifi_set_inactive_time(wifi_interface_t ifx, uint16_t sec)
Set the inactive time of the STA or AP. 

Attention1. For Station, If the station does not receive a beacon frame from the connected SoftAP during the inactive time, disconnect from SoftAP. Default 6s. 

Attention2. For SoftAP, If the softAP doesn't receive any data from the connected STA during inactive time, the softAP will force deauth the STA. Default is 300s. 

Attention3. The inactive time configuration is not stored into flash




Parameters

ifx -- interface to be configured. 
sec -- Inactive time. Unit seconds.


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument, For Station, if sec is less than 3. For SoftAP, if sec is less than 10. 





```

#### esp_wifi_get_inactive_time


```


esp_err_t esp_wifi_get_inactive_time(wifi_interface_t ifx, uint16_t *sec)
Get inactive time of specified interface. 

Parameters

ifx -- Interface to be configured. 
sec -- Inactive time. Unit seconds.


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_statis_dump


```


esp_err_t esp_wifi_statis_dump(uint32_t modules)
Dump WiFi statistics. 

Parameters
modules -- statistic modules to be dumped

Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_set_rssi_threshold


```


esp_err_t esp_wifi_set_rssi_threshold(int32_t rssi)
Set RSSI threshold, if average rssi gets lower than threshold, WiFi task will post event WIFI_EVENT_STA_BSS_RSSI_LOW. 

AttentionIf the user wants to receive another WIFI_EVENT_STA_BSS_RSSI_LOW event after receiving one, this API needs to be called again with an updated/same RSSI threshold.




Parameters
rssi -- threshold value in dbm between -100 to 10 Note that in some rare cases where signal strength is very strong, rssi values can be slightly positive.

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_ftm_initiate_session


```


esp_err_t esp_wifi_ftm_initiate_session(wifi_ftm_initiator_cfg_t *cfg)
Start an FTM Initiator session by sending FTM request If successful, event WIFI_EVENT_FTM_REPORT is generated with the result of the FTM procedure. 

Attention1. Use this API only in Station mode. 

Attention2. If FTM is initiated on a different channel than Station is connected in or internal SoftAP is started in, FTM defaults to a single burst in ASAP mode.




Parameters
cfg -- FTM Initiator session configuration

Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_ftm_end_session


```


esp_err_t esp_wifi_ftm_end_session(void)
End the ongoing FTM Initiator session. 

AttentionThis API works only on FTM Initiator




Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_ftm_resp_set_offset


```


esp_err_t esp_wifi_ftm_resp_set_offset(int16_t offset_cm)
Set offset in cm for FTM Responder. An equivalent offset is calculated in picoseconds and added in TOD of FTM Measurement frame (T1). 

AttentionUse this API only in AP mode before performing FTM as responder




Parameters
offset_cm -- T1 Offset to be added in centimeters

Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_ftm_get_report


```


esp_err_t esp_wifi_ftm_get_report(wifi_ftm_report_entry_t *report, uint8_t num_entries)
Get FTM measurements report copied into a user provided buffer. 

Attention1. To get the FTM report, user first needs to allocate a buffer of size (sizeof(wifi_ftm_report_entry_t) * num_entries) where the API will fill up to num_entries valid FTM measurements in the buffer. Total number of entries can be found in the event WIFI_EVENT_FTM_REPORT as ftm_report_num_entries 

Attention2. The internal FTM report is freed upon use of this API which means the API can only be used once after every FTM session initiated 

Attention3. Passing the buffer as NULL merely frees the FTM report




Parameters

report -- Pointer to the buffer for receiving the FTM report 
num_entries -- Number of FTM report entries to be filled in the report


Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_config_11b_rate


```


esp_err_t esp_wifi_config_11b_rate(wifi_interface_t ifx, bool disable)
Enable or disable 11b rate of specified interface. 

Attention1. This API should be called after esp_wifi_init() and before esp_wifi_start(). 

Attention2. Only when really need to disable 11b rate call this API otherwise don't call this.




Parameters

ifx -- Interface to be configured. 
disable -- true means disable 11b rate while false means enable 11b rate.


Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_connectionless_module_set_wake_interval


```


esp_err_t esp_wifi_connectionless_module_set_wake_interval(uint16_t wake_interval)
Set wake interval for connectionless modules to wake up periodically. 

Attention1. Only one wake interval for all connectionless modules. 

Attention2. This configuration could work at connected status. When ESP_WIFI_STA_DISCONNECTED_PM_ENABLE is enabled, this configuration could work at disconnected status. 

Attention3. Event WIFI_EVENT_CONNECTIONLESS_MODULE_WAKE_INTERVAL_START would be posted each time wake interval starts. 

Attention4. Recommend to configure interval in multiples of hundred. (e.g. 100ms) 

Attention5. Recommend to configure interval to ESP_WIFI_CONNECTIONLESS_INTERVAL_DEFAULT_MODE to get stable performance at coexistence mode.




Parameters
wake_interval -- Milliseconds after would the chip wake up, from 1 to 65535. 



```

#### esp_wifi_force_wakeup_acquire


```


esp_err_t esp_wifi_force_wakeup_acquire(void)
Request extra reference of Wi-Fi radio. Wi-Fi keep active state(RF opened) to be able to receive packets. 

AttentionPlease pair the use of esp_wifi_force_wakeup_acquire with esp_wifi_force_wakeup_release.




Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start 





```

#### esp_wifi_force_wakeup_release


```


esp_err_t esp_wifi_force_wakeup_release(void)
Release extra reference of Wi-Fi radio. Wi-Fi go to sleep state(RF closed) if no more use of radio. 

AttentionPlease pair the use of esp_wifi_force_wakeup_acquire with esp_wifi_force_wakeup_release.




Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start 





```

#### esp_wifi_set_country_code


```


esp_err_t esp_wifi_set_country_code(const char *country, bool ieee80211d_enabled)
configure country 

Attention1. When ieee80211d_enabled, the country info of the AP to which the station is connected is used. E.g. if the configured country is US and the country info of the AP to which the station is connected is JP then the country info that will be used is JP. If the station disconnected from the AP the country info is set back to the country info of the station automatically, US in the example. 

Attention2. When ieee80211d_enabled is disabled, then the configured country info is used always. 

Attention3. When the country info is changed because of configuration or because the station connects to a different external AP, the country IE in probe response/beacon of the soft-AP is also changed. 

Attention4. The country configuration is stored into flash. 

Attention5. When this API is called, the PHY init data will switch to the PHY init data type corresponding to the country info. 

Attention6. Supported country codes are "01"(world safe mode) "AT","AU","BE","BG","BR", "CA","CH","CN","CY","CZ","DE","DK","EE","ES","FI","FR","GB","GR","HK","HR","HU", "IE","IN","IS","IT","JP","KR","LI","LT","LU","LV","MT","MX","NL","NO","NZ","PL","PT", "RO","SE","SI","SK","TW","US"

Attention7. When country code "01" (world safe mode) is set, SoftAP mode won't contain country IE. 

Attention8. The default country is "01" (world safe mode) and ieee80211d_enabled is TRUE. 

Attention9. The third octet of country code string is one of the following: ' ', 'O', 'I', 'X', otherwise it is considered as ' '.




Parameters

country -- the configured country ISO code 
ieee80211d_enabled -- 802.11d is enabled or not


Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_get_country_code


```


esp_err_t esp_wifi_get_country_code(char *country)
get the current country code 

Parameters
country -- country code

Returns

ESP_OK: succeed
ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
ESP_ERR_INVALID_ARG: invalid argument 





```

#### esp_wifi_config_80211_tx_rate


```


esp_err_t esp_wifi_config_80211_tx_rate(wifi_interface_t ifx, wifi_phy_rate_t rate)
Config 80211 tx rate of specified interface. 

Attention1. This API should be called after esp_wifi_init() and before esp_wifi_start().




Parameters

ifx -- Interface to be configured. 
rate -- Phy rate to be configured.


Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_disable_pmf_config


```


esp_err_t esp_wifi_disable_pmf_config(wifi_interface_t ifx)
Disable PMF configuration for specified interface. 

AttentionThis API should be called after esp_wifi_set_config() and before esp_wifi_start().




Parameters
ifx -- Interface to be configured.

Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_sta_get_aid


```


esp_err_t esp_wifi_sta_get_aid(uint16_t *aid)
Get the Association id assigned to STA by AP. 

Attentionaid = 0 if station is not connected to AP.




Parameters
aid -- [out] store the aid

Returns

ESP_OK: succeed 





```

#### esp_wifi_sta_get_negotiated_phymode


```


esp_err_t esp_wifi_sta_get_negotiated_phymode(wifi_phy_mode_t *phymode)
Get the negotiated phymode after connection. 

Parameters
phymode -- [out] store the negotiated phymode.

Returns

ESP_OK: succeed 





```

#### esp_wifi_set_dynamic_cs


```


esp_err_t esp_wifi_set_dynamic_cs(bool enabled)
Config dynamic carrier sense. 

AttentionThis API should be called after esp_wifi_start().




Parameters
enabled -- Dynamic carrier sense is enabled or not.

Returns

ESP_OK: succeed
others: failed 





```

#### esp_wifi_sta_get_rssi


```


esp_err_t esp_wifi_sta_get_rssi(int *rssi)
Get the rssi information of AP to which the device is associated with. 

Attention1. This API should be called after station connected to AP. 

Attention2. Use this API only in WIFI_MODE_STA or WIFI_MODE_APSTA mode.




Parameters
rssi -- store the rssi info received from last beacon.

Returns

ESP_OK: succeed
ESP_ERR_INVALID_ARG: invalid argument
ESP_FAIL: failed 





```



### Structures


#### wifi_init_config_t


```


struct wifi_init_config_t
WiFi stack configuration parameters passed to esp_wifi_init call. 

Public Members
osi_funcs

wifi_osi_funcs_t *osi_funcs
WiFi OS functions 

wpa_crypto_funcs

wpa_crypto_funcs_t wpa_crypto_funcs
WiFi station crypto functions when connect 

static_rx_buf_num

int static_rx_buf_num
WiFi static RX buffer number 

dynamic_rx_buf_num

int dynamic_rx_buf_num
WiFi dynamic RX buffer number 

tx_buf_type

int tx_buf_type
WiFi TX buffer type 

static_tx_buf_num

int static_tx_buf_num
WiFi static TX buffer number 

dynamic_tx_buf_num

int dynamic_tx_buf_num
WiFi dynamic TX buffer number 

rx_mgmt_buf_type

int rx_mgmt_buf_type
WiFi RX MGMT buffer type 

rx_mgmt_buf_num

int rx_mgmt_buf_num
WiFi RX MGMT buffer number 

cache_tx_buf_num

int cache_tx_buf_num
WiFi TX cache buffer number 

csi_enable

int csi_enable
WiFi channel state information enable flag 

ampdu_rx_enable

int ampdu_rx_enable
WiFi AMPDU RX feature enable flag 

ampdu_tx_enable

int ampdu_tx_enable
WiFi AMPDU TX feature enable flag 

amsdu_tx_enable

int amsdu_tx_enable
WiFi AMSDU TX feature enable flag 

nvs_enable

int nvs_enable
WiFi NVS flash enable flag 

nano_enable

int nano_enable
Nano option for printf/scan family enable flag 

rx_ba_win

int rx_ba_win
WiFi Block Ack RX window size 

wifi_task_core_id

int wifi_task_core_id
WiFi Task Core ID 

beacon_max_len

int beacon_max_len
WiFi softAP maximum length of the beacon 

mgmt_sbuf_num

int mgmt_sbuf_num
WiFi management short buffer number, the minimum value is 6, the maximum value is 32 

feature_caps

uint64_t feature_caps
Enables additional WiFi features and capabilities 

sta_disconnected_pm

bool sta_disconnected_pm
WiFi Power Management for station at disconnected status 

espnow_max_encrypt_num

int espnow_max_encrypt_num
Maximum encrypt number of peers supported by espnow 

tx_hetb_queue_num

int tx_hetb_queue_num
WiFi TX HE TB QUEUE number for STA HE TB PPDU transmission 

dump_hesigb_enable

bool dump_hesigb_enable
enable dump sigb field 

magic

int magic
WiFi init magic number, it should be the last field 



```



### Macros


#### ESP_ERR_WIFI_NOT_INIT


```


ESP_ERR_WIFI_NOT_INIT
WiFi driver was not installed by esp_wifi_init 

```

#### ESP_ERR_WIFI_NOT_STARTED


```


ESP_ERR_WIFI_NOT_STARTED
WiFi driver was not started by esp_wifi_start 

```

#### ESP_ERR_WIFI_NOT_STOPPED


```


ESP_ERR_WIFI_NOT_STOPPED
WiFi driver was not stopped by esp_wifi_stop 

```

#### ESP_ERR_WIFI_IF


```


ESP_ERR_WIFI_IF
WiFi interface error 

```

#### ESP_ERR_WIFI_MODE


```


ESP_ERR_WIFI_MODE
WiFi mode error 

```

#### ESP_ERR_WIFI_STATE


```


ESP_ERR_WIFI_STATE
WiFi internal state error 

```

#### ESP_ERR_WIFI_CONN


```


ESP_ERR_WIFI_CONN
WiFi internal control block of station or soft-AP error 

```

#### ESP_ERR_WIFI_NVS


```


ESP_ERR_WIFI_NVS
WiFi internal NVS module error 

```

#### ESP_ERR_WIFI_MAC


```


ESP_ERR_WIFI_MAC
MAC address is invalid 

```

#### ESP_ERR_WIFI_SSID


```


ESP_ERR_WIFI_SSID
SSID is invalid 

```

#### ESP_ERR_WIFI_PASSWORD


```


ESP_ERR_WIFI_PASSWORD
Password is invalid 

```

#### ESP_ERR_WIFI_TIMEOUT


```


ESP_ERR_WIFI_TIMEOUT
Timeout error 

```

#### ESP_ERR_WIFI_WAKE_FAIL


```


ESP_ERR_WIFI_WAKE_FAIL
WiFi is in sleep state(RF closed) and wakeup fail 

```

#### ESP_ERR_WIFI_WOULD_BLOCK


```


ESP_ERR_WIFI_WOULD_BLOCK
The caller would block 

```

#### ESP_ERR_WIFI_NOT_CONNECT


```


ESP_ERR_WIFI_NOT_CONNECT
Station still in disconnect status 

```

#### ESP_ERR_WIFI_POST


```


ESP_ERR_WIFI_POST
Failed to post the event to WiFi task 

```

#### ESP_ERR_WIFI_INIT_STATE


```


ESP_ERR_WIFI_INIT_STATE
Invalid WiFi state when init/deinit is called 

```

#### ESP_ERR_WIFI_STOP_STATE


```


ESP_ERR_WIFI_STOP_STATE
Returned when WiFi is stopping 

```

#### ESP_ERR_WIFI_NOT_ASSOC


```


ESP_ERR_WIFI_NOT_ASSOC
The WiFi connection is not associated 

```

#### ESP_ERR_WIFI_TX_DISALLOW


```


ESP_ERR_WIFI_TX_DISALLOW
The WiFi TX is disallowed 

```

#### ESP_ERR_WIFI_TWT_FULL


```


ESP_ERR_WIFI_TWT_FULL
no available flow id 

```

#### ESP_ERR_WIFI_TWT_SETUP_TIMEOUT


```


ESP_ERR_WIFI_TWT_SETUP_TIMEOUT
Timeout of receiving twt setup response frame, timeout times can be set during twt setup 

```

#### ESP_ERR_WIFI_TWT_SETUP_TXFAIL


```


ESP_ERR_WIFI_TWT_SETUP_TXFAIL
TWT setup frame tx failed 

```

#### ESP_ERR_WIFI_TWT_SETUP_REJECT


```


ESP_ERR_WIFI_TWT_SETUP_REJECT
The twt setup request was rejected by the AP 

```

#### ESP_ERR_WIFI_DISCARD


```


ESP_ERR_WIFI_DISCARD
Discard frame 

```

#### ESP_ERR_WIFI_ROC_IN_PROGRESS


```


ESP_ERR_WIFI_ROC_IN_PROGRESS
ROC op is in progress 

```

#### WIFI_STATIC_TX_BUFFER_NUM


```


WIFI_STATIC_TX_BUFFER_NUM

```

#### WIFI_CACHE_TX_BUFFER_NUM


```


WIFI_CACHE_TX_BUFFER_NUM

```

#### WIFI_DYNAMIC_TX_BUFFER_NUM


```


WIFI_DYNAMIC_TX_BUFFER_NUM

```

#### WIFI_RX_MGMT_BUF_NUM_DEF


```


WIFI_RX_MGMT_BUF_NUM_DEF

```

#### WIFI_CSI_ENABLED


```


WIFI_CSI_ENABLED

```

#### WIFI_AMPDU_RX_ENABLED


```


WIFI_AMPDU_RX_ENABLED

```

#### WIFI_AMPDU_TX_ENABLED


```


WIFI_AMPDU_TX_ENABLED

```

#### WIFI_AMSDU_TX_ENABLED


```


WIFI_AMSDU_TX_ENABLED

```

#### WIFI_NVS_ENABLED


```


WIFI_NVS_ENABLED

```

#### WIFI_NANO_FORMAT_ENABLED


```


WIFI_NANO_FORMAT_ENABLED

```

#### WIFI_INIT_CONFIG_MAGIC


```


WIFI_INIT_CONFIG_MAGIC

```

#### WIFI_DEFAULT_RX_BA_WIN


```


WIFI_DEFAULT_RX_BA_WIN

```

#### WIFI_TASK_CORE_ID


```


WIFI_TASK_CORE_ID

```

#### WIFI_SOFTAP_BEACON_MAX_LEN


```


WIFI_SOFTAP_BEACON_MAX_LEN

```

#### WIFI_MGMT_SBUF_NUM


```


WIFI_MGMT_SBUF_NUM

```

#### WIFI_STA_DISCONNECTED_PM_ENABLED


```


WIFI_STA_DISCONNECTED_PM_ENABLED

```

#### WIFI_ENABLE_WPA3_SAE


```


WIFI_ENABLE_WPA3_SAE

```

#### WIFI_ENABLE_SPIRAM


```


WIFI_ENABLE_SPIRAM

```

#### WIFI_FTM_INITIATOR


```


WIFI_FTM_INITIATOR

```

#### WIFI_FTM_RESPONDER


```


WIFI_FTM_RESPONDER

```

#### WIFI_ENABLE_GCMP


```


WIFI_ENABLE_GCMP

```

#### WIFI_ENABLE_GMAC


```


WIFI_ENABLE_GMAC

```

#### WIFI_ENABLE_11R


```


WIFI_ENABLE_11R

```

#### WIFI_ENABLE_ENTERPRISE


```


WIFI_ENABLE_ENTERPRISE

```

#### WIFI_DUMP_HESIGB_ENABLED


```


WIFI_DUMP_HESIGB_ENABLED

```

#### WIFI_TX_HETB_QUEUE_NUM


```


WIFI_TX_HETB_QUEUE_NUM

```

#### CONFIG_FEATURE_WPA3_SAE_BIT


```


CONFIG_FEATURE_WPA3_SAE_BIT

```

#### CONFIG_FEATURE_CACHE_TX_BUF_BIT


```


CONFIG_FEATURE_CACHE_TX_BUF_BIT

```

#### CONFIG_FEATURE_FTM_INITIATOR_BIT


```


CONFIG_FEATURE_FTM_INITIATOR_BIT

```

#### CONFIG_FEATURE_FTM_RESPONDER_BIT


```


CONFIG_FEATURE_FTM_RESPONDER_BIT

```

#### CONFIG_FEATURE_GCMP_BIT


```


CONFIG_FEATURE_GCMP_BIT

```

#### CONFIG_FEATURE_GMAC_BIT


```


CONFIG_FEATURE_GMAC_BIT

```

#### CONFIG_FEATURE_11R_BIT


```


CONFIG_FEATURE_11R_BIT

```

#### CONFIG_FEATURE_WIFI_ENT_BIT


```


CONFIG_FEATURE_WIFI_ENT_BIT

```

#### WIFI_FEATURE_CAPS


```


WIFI_FEATURE_CAPS

```

#### WIFI_INIT_CONFIG_DEFAULT


```


WIFI_INIT_CONFIG_DEFAULT()

```

#### ESP_WIFI_CONNECTIONLESS_INTERVAL_DEFAULT_MODE


```


ESP_WIFI_CONNECTIONLESS_INTERVAL_DEFAULT_MODE

```



### Type Definitions


#### wifi_osi_funcs_t


```


typedef struct wifi_osi_funcs_t wifi_osi_funcs_t

```

#### wifi_promiscuous_cb_t


```


typedef void (*wifi_promiscuous_cb_t)(void *buf, wifi_promiscuous_pkt_type_t type)
The RX callback function in the promiscuous mode. Each time a packet is received, the callback function will be called. 

Param buf
Data received. Type of data in buffer (wifi_promiscuous_pkt_t or wifi_pkt_rx_ctrl_t) indicated by 'type' parameter. 

Param type
promiscuous packet type. 



```

#### wifi_sta_list_t


```


typedef struct wifi_sta_list_t wifi_sta_list_t
Forward declare wifi_sta_list_t. The definition depends on the target device that implements esp_wifi. 

```

#### esp_vendor_ie_cb_t


```


typedef void (*esp_vendor_ie_cb_t)(void *ctx, wifi_vendor_ie_type_t type, const uint8_t sa[6], const vendor_ie_data_t *vnd_ie, int rssi)
Function signature for received Vendor-Specific Information Element callback. 

Param ctx
Context argument, as passed to esp_wifi_set_vendor_ie_cb() when registering callback. 

Param type
Information element type, based on frame type received. 

Param sa
Source 802.11 address. 

Param vnd_ie
Pointer to the vendor specific element data received. 

Param rssi
Received signal strength indication. 



```

#### wifi_csi_cb_t


```


typedef void (*wifi_csi_cb_t)(void *ctx, wifi_csi_info_t *data)
The RX callback function of Channel State Information(CSI) data. 
   Each time a CSI data is received, the callback function will be called.




Param ctx
context argument, passed to esp_wifi_set_csi_rx_cb() when registering callback function. 

Param data
CSI data received. The memory that it points to will be deallocated after callback function returns. 



```



### Header File


* components/esp_wifi/include/esp_wifi_types.h
* This header file can be included with:



> ```
> #include "esp_wifi_types.h"
> 
> ```
* This header file is a part of the API provided by the `esp_wifi` component. To declare that your component depends on `esp_wifi`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_wifi
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_wifi
> 
> ```




### Type Definitions


#### wifi_csi_config_t


```


typedef struct wifi_csi_config_t wifi_csi_config_t

```

#### wifi_pkt_rx_ctrl_t


```


typedef struct wifi_pkt_rx_ctrl_t wifi_pkt_rx_ctrl_t

```



### Header File


* components/wpa_supplicant/esp_supplicant/include/esp_eap_client.h
* This header file can be included with:



> ```
> #include "esp_eap_client.h"
> 
> ```
* This header file is a part of the API provided by the `wpa_supplicant` component. To declare that your component depends on `wpa_supplicant`, add the following to your CMakeLists.txt:



> ```
> REQUIRES wpa_supplicant
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES wpa_supplicant
> 
> ```




### Functions


#### esp_wifi_sta_enterprise_enable


```


esp_err_t esp_wifi_sta_enterprise_enable(void)
Enable EAP authentication(WiFi Enterprise) for the station mode. 
This function enables Extensible Authentication Protocol (EAP) authentication for the Wi-Fi station mode. When EAP authentication is enabled, the ESP device will attempt to authenticate with the configured EAP credentials when connecting to a secure Wi-Fi network.

Note
Before calling this function, ensure that the Wi-Fi configuration and EAP credentials (such as username and password) have been properly set using the appropriate configuration APIs.


Returns

ESP_OK: EAP authentication enabled successfully.
ESP_ERR_NO_MEM: Failed to enable EAP authentication due to memory allocation failure. 





```

#### esp_wifi_sta_enterprise_disable


```


esp_err_t esp_wifi_sta_enterprise_disable(void)
Disable EAP authentication(WiFi Enterprise) for the station mode. 
This function disables Extensible Authentication Protocol (EAP) authentication for the Wi-Fi station mode. When EAP authentication is disabled, the ESP device will not attempt to authenticate using EAP credentials when connecting to a secure Wi-Fi network.

Note
Disabling EAP authentication may cause the device to connect to the Wi-Fi network using other available authentication methods, if configured using esp_wifi_set_config().


Returns

ESP_OK: EAP authentication disabled successfully.
ESP_ERR_INVALID_STATE: EAP client is in an invalid state for disabling. 





```

#### esp_eap_client_set_identity


```


esp_err_t esp_eap_client_set_identity(const unsigned char *identity, int len)
Set identity for PEAP/TTLS authentication method. 
This function sets the identity to be used during PEAP/TTLS authentication.

Parameters

identity -- [in] Pointer to the identity data. 
len -- [in] Length of the identity data (limited to 1~127 bytes).


Returns

ESP_OK: The identity was set successfully.
ESP_ERR_INVALID_ARG: Invalid argument (len <= 0 or len >= 128).
ESP_ERR_NO_MEM: Memory allocation failure. 





```

#### esp_eap_client_clear_identity


```


void esp_eap_client_clear_identity(void)
Clear the previously set identity for PEAP/TTLS authentication. 
This function clears the identity that was previously set for the EAP client. After calling this function, the EAP client will no longer use the previously configured identity during the authentication process. 

```

#### esp_eap_client_set_username


```


esp_err_t esp_eap_client_set_username(const unsigned char *username, int len)
Set username for PEAP/TTLS authentication method. 
This function sets the username to be used during PEAP/TTLS authentication.

Parameters

username -- [in] Pointer to the username data. 
len -- [in] Length of the username data (limited to 1~127 bytes).


Returns

ESP_OK: The username was set successfully.
ESP_ERR_INVALID_ARG: Failed due to an invalid argument (len <= 0 or len >= 128).
ESP_ERR_NO_MEM: Failed due to memory allocation failure. 





```

#### esp_eap_client_clear_username


```


void esp_eap_client_clear_username(void)
Clear username for PEAP/TTLS method. 
This function clears the previously set username for the EAP client. 

```

#### esp_eap_client_set_password


```


esp_err_t esp_eap_client_set_password(const unsigned char *password, int len)
Set password for PEAP/TTLS authentication method. 
This function sets the password to be used during PEAP/TTLS authentication.

Parameters

password -- [in] Pointer to the password data. 
len -- [in] Length of the password data (len > 0).


Returns

ESP_OK: The password was set successfully.
ESP_ERR_INVALID_ARG: Failed due to an invalid argument (len <= 0).
ESP_ERR_NO_MEM: Failed due to memory allocation failure. 





```

#### esp_eap_client_clear_password


```


void esp_eap_client_clear_password(void)
Clear password for PEAP/TTLS method. 
This function clears the previously set password for the EAP client. 

```

#### esp_eap_client_set_new_password


```


esp_err_t esp_eap_client_set_new_password(const unsigned char *new_password, int len)
Set a new password for MSCHAPv2 authentication method. 
This function sets the new password to be used during MSCHAPv2 authentication. The new password is used to substitute the old password when an eap-mschapv2 failure request message with error code ERROR_PASSWD_EXPIRED is received.

Parameters

new_password -- [in] Pointer to the new password data. 
len -- [in] Length of the new password data.


Returns

ESP_OK: The new password was set successfully.
ESP_ERR_INVALID_ARG: Failed due to an invalid argument (len <= 0).
ESP_ERR_NO_MEM: Failed due to memory allocation failure. 





```

#### esp_eap_client_clear_new_password


```


void esp_eap_client_clear_new_password(void)
Clear new password for MSCHAPv2 method. 
This function clears the previously set new password for the EAP client. 

```

#### esp_eap_client_set_ca_cert


```


esp_err_t esp_eap_client_set_ca_cert(const unsigned char *ca_cert, int ca_cert_len)
Set CA certificate for EAP authentication. 
This function sets the Certificate Authority (CA) certificate to be used during EAP authentication. The CA certificate is passed to the EAP client module through a global pointer.

Parameters

ca_cert -- [in] Pointer to the CA certificate data. 
ca_cert_len -- [in] Length of the CA certificate data.


Returns

ESP_OK: The CA certificate was set successfully. 





```

#### esp_eap_client_clear_ca_cert


```


void esp_eap_client_clear_ca_cert(void)
Clear the previously set Certificate Authority (CA) certificate for EAP authentication. 
This function clears the CA certificate that was previously set for the EAP client. After calling this function, the EAP client will no longer use the previously configured CA certificate during the authentication process. 

```

#### esp_eap_client_set_certificate_and_key


```


esp_err_t esp_eap_client_set_certificate_and_key(const unsigned char *client_cert, int client_cert_len, const unsigned char *private_key, int private_key_len, const unsigned char *private_key_password, int private_key_passwd_len)
Set client certificate and private key for EAP authentication. 
This function sets the client certificate and private key to be used during authentication. Optionally, a private key password can be provided for encrypted private keys.

Attention1. The client certificate, private key, and private key password are provided as pointers to the respective data arrays. 

Attention2. The client_cert, private_key, and private_key_password should be zero-terminated.




Parameters

client_cert -- [in] Pointer to the client certificate data. 
client_cert_len -- [in] Length of the client certificate data. 
private_key -- [in] Pointer to the private key data. 
private_key_len -- [in] Length of the private key data (limited to 1~4096 bytes). 
private_key_password -- [in] Pointer to the private key password data (optional). 
private_key_passwd_len -- [in] Length of the private key password data (can be 0 for no password).


Returns

ESP_OK: The certificate, private key, and password (if provided) were set successfully. 





```

#### esp_eap_client_clear_certificate_and_key


```


void esp_eap_client_clear_certificate_and_key(void)
Clear the previously set client certificate and private key for EAP authentication. 
This function clears the client certificate and private key that were previously set for the EAP client. After calling this function, the EAP client will no longer use the previously configured certificate and private key during the authentication process. 

```

#### esp_eap_client_set_disable_time_check


```


esp_err_t esp_eap_client_set_disable_time_check(bool disable)
Set EAP client certificates time check (disable or not). 
This function enables or disables the time check for EAP client certificates. When disabled, the certificates' expiration time will not be checked during the authentication process.

Parameters
disable -- [in] True to disable EAP client certificates time check, false to enable it.

Returns

ESP_OK: The EAP client certificates time check setting was updated successfully. 





```

#### esp_eap_client_get_disable_time_check


```


esp_err_t esp_eap_client_get_disable_time_check(bool *disable)
Get EAP client certificates time check status. 
This function retrieves the current status of the EAP client certificates time check.

Parameters
disable -- [out] Pointer to a boolean variable to store the disable status.

Returns

ESP_OK: The status of EAP client certificates time check was retrieved successfully. 





```

#### esp_eap_client_set_ttls_phase2_method


```


esp_err_t esp_eap_client_set_ttls_phase2_method(esp_eap_ttls_phase2_types type)
Set EAP-TTLS phase 2 method. 
This function sets the phase 2 method to be used during EAP-TTLS authentication.

Parameters
type -- [in] The type of phase 2 method to be used (e.g., EAP, MSCHAPv2, MSCHAP, PAP, CHAP).

Returns

ESP_OK: The EAP-TTLS phase 2 method was set successfully. 





```

#### esp_eap_client_set_suiteb_192bit_certification


```


esp_err_t esp_eap_client_set_suiteb_192bit_certification(bool enable)
Enable or disable Suite-B 192-bit certification checks. 
This function enables or disables the 192-bit Suite-B certification checks during EAP-TLS authentication. Suite-B is a set of cryptographic algorithms which generally are considered more secure.

Parameters
enable -- [in] True to enable 192-bit Suite-B certification checks, false to disable it.

Returns

ESP_OK: The 192-bit Suite-B certification checks were set successfully. 





```

#### esp_eap_client_set_pac_file


```


esp_err_t esp_eap_client_set_pac_file(const unsigned char *pac_file, int pac_file_len)
Set the PAC (Protected Access Credential) file for EAP-FAST authentication. 
EAP-FAST requires a PAC file that contains the client's credentials.

Attention1. For files read from the file system, length has to be decremented by 1 byte. 

Attention2. Disabling the ESP_WIFI_MBEDTLS_TLS_CLIENT config is required to use EAP-FAST.




Parameters

pac_file -- [in] Pointer to the PAC file buffer. 
pac_file_len -- [in] Length of the PAC file buffer.


Returns

ESP_OK: The PAC file for EAP-FAST authentication was set successfully. 





```

#### esp_eap_client_set_fast_params


```


esp_err_t esp_eap_client_set_fast_params(esp_eap_fast_config config)
Set the parameters for EAP-FAST Phase 1 authentication. 
EAP-FAST supports Fast Provisioning, where clients can be authenticated faster using precomputed keys (PAC). This function allows configuring parameters for Fast Provisioning.

Attention1. Disabling the ESP_WIFI_MBEDTLS_TLS_CLIENT config is required to use EAP-FAST.




Parameters
config -- [in] Configuration structure with Fast Provisioning parameters.

Returns

ESP_OK: The parameters for EAP-FAST Phase 1 authentication were set successfully. 





```

#### esp_eap_client_use_default_cert_bundle


```


esp_err_t esp_eap_client_use_default_cert_bundle(bool use_default_bundle)
Use the default certificate bundle for EAP authentication. 
By default, the EAP client uses a built-in certificate bundle for server verification. Enabling this option allows the use of the default certificate bundle.

Parameters
use_default_bundle -- [in] True to use the default certificate bundle, false to use a custom bundle.

Returns

ESP_OK: The option to use the default certificate bundle was set successfully. 





```



### Structures


#### esp_eap_fast_config


```


struct esp_eap_fast_config
Configuration settings for EAP-FAST (Extensible Authentication Protocol - Flexible Authentication via Secure Tunneling). 
This structure defines the configuration options that can be used to customize the behavior of the EAP-FAST authentication protocol, specifically for Fast Provisioning and PAC (Protected Access Credential) handling. 

Public Members
fast_provisioning

int fast_provisioning
Enable or disable Fast Provisioning in EAP-FAST (0 = disabled, 1 = enabled) 

fast_max_pac_list_len

int fast_max_pac_list_len
Maximum length of the PAC (Protected Access Credential) list 

fast_pac_format_binary

bool fast_pac_format_binary
Set to true for binary format PAC, false for ASCII format PAC 



```



### Enumerations


#### esp_eap_ttls_phase2_types


```


enum esp_eap_ttls_phase2_types
Enumeration of phase 2 authentication types for EAP-TTLS. 
This enumeration defines the supported phase 2 authentication methods that can be used in the EAP-TTLS (Extensible Authentication Protocol - Tunneled Transport Layer Security) protocol for the second authentication phase. 
Values:
ESP_EAP_TTLS_PHASE2_EAP

enumerator ESP_EAP_TTLS_PHASE2_EAP
EAP (Extensible Authentication Protocol) 

ESP_EAP_TTLS_PHASE2_MSCHAPV2

enumerator ESP_EAP_TTLS_PHASE2_MSCHAPV2
MS-CHAPv2 (Microsoft Challenge Handshake Authentication Protocol - Version 2) 

ESP_EAP_TTLS_PHASE2_MSCHAP

enumerator ESP_EAP_TTLS_PHASE2_MSCHAP
MS-CHAP (Microsoft Challenge Handshake Authentication Protocol) 

ESP_EAP_TTLS_PHASE2_PAP

enumerator ESP_EAP_TTLS_PHASE2_PAP
PAP (Password Authentication Protocol) 

ESP_EAP_TTLS_PHASE2_CHAP

enumerator ESP_EAP_TTLS_PHASE2_CHAP
CHAP (Challenge Handshake Authentication Protocol) 


```



### Header File


* components/wpa_supplicant/esp_supplicant/include/esp_wps.h
* This header file can be included with:



> ```
> #include "esp_wps.h"
> 
> ```
* This header file is a part of the API provided by the `wpa_supplicant` component. To declare that your component depends on `wpa_supplicant`, add the following to your CMakeLists.txt:



> ```
> REQUIRES wpa_supplicant
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES wpa_supplicant
> 
> ```




### Functions


#### esp_wifi_wps_enable


```


esp_err_t esp_wifi_wps_enable(const esp_wps_config_t *config)
Enable Wi-Fi WPS function. 

Parameters
config -- : WPS config to be used in connection

Returns

ESP_OK : succeed
ESP_ERR_WIFI_WPS_TYPE : wps type is invalid
ESP_ERR_WIFI_WPS_MODE : wifi is not in station mode or sniffer mode is on
ESP_FAIL : wps initialization fails 





```

#### esp_wifi_wps_disable


```


esp_err_t esp_wifi_wps_disable(void)
Disable Wi-Fi WPS function and release resource it taken. 

Returns

ESP_OK : succeed
ESP_ERR_WIFI_WPS_MODE : wifi is not in station mode or sniffer mode is on 





```

#### esp_wifi_wps_start


```


esp_err_t esp_wifi_wps_start(int timeout_ms)
Start WPS session. 

AttentionWPS can only be used when station is enabled. WPS needs to be enabled first for using this API.




Parameters
timeout_ms -- : deprecated: This argument's value will have not effect in functionality of API. The argument will be removed in future. The app should start WPS and register for WIFI events to get the status. WPS status is updated through WPS events. See wifi_event_t enum for more info.

Returns

ESP_OK : succeed
ESP_ERR_WIFI_WPS_TYPE : wps type is invalid
ESP_ERR_WIFI_WPS_MODE : wifi is not in station mode or sniffer mode is on
ESP_ERR_WIFI_WPS_SM : wps state machine is not initialized
ESP_FAIL : wps initialization fails 





```

#### esp_wifi_ap_wps_enable


```


esp_err_t esp_wifi_ap_wps_enable(const esp_wps_config_t *config)
Enable Wi-Fi AP WPS function. 

AttentionWPS can only be used when softAP is enabled.




Parameters
config -- wps configuration to be used.

Returns

ESP_OK : succeed
ESP_ERR_WIFI_WPS_TYPE : wps type is invalid
ESP_ERR_WIFI_WPS_MODE : wifi is not in station mode or sniffer mode is on
ESP_FAIL : wps initialization fails 





```

#### esp_wifi_ap_wps_disable


```


esp_err_t esp_wifi_ap_wps_disable(void)
Disable Wi-Fi SoftAP WPS function and release resource it taken. 

Returns

ESP_OK : succeed
ESP_ERR_WIFI_WPS_MODE : wifi is not in station mode or sniffer mode is on 





```

#### esp_wifi_ap_wps_start


```


esp_err_t esp_wifi_ap_wps_start(const unsigned char *pin)
WPS starts to work. 

AttentionWPS can only be used when softAP is enabled.




Parameters
pin -- : Pin to be used in case of WPS mode is pin. If Pin is not provided, device will use the pin generated/provided during esp_wifi_ap_wps_enable() and reported in WIFI_EVENT_AP_WPS_RG_PIN

Returns

ESP_OK : succeed
ESP_ERR_WIFI_WPS_TYPE : wps type is invalid
ESP_ERR_WIFI_WPS_MODE : wifi is not in station mode or sniffer mode is on
ESP_ERR_WIFI_WPS_SM : wps state machine is not initialized
ESP_FAIL : wps initialization fails 





```



### Structures


#### wps_factory_information_t


```


struct wps_factory_information_t
Structure representing WPS factory information for ESP device. 
This structure holds various strings representing factory information for a device, such as the manufacturer, model number, model name, and device name. Each string is a null-terminated character array. If any of the strings are empty, the default values are used. 

Public Members
manufacturer

char manufacturer[WPS_MAX_MANUFACTURER_LEN]
Manufacturer of the device. If empty, the default manufacturer is used. 

model_number

char model_number[WPS_MAX_MODEL_NUMBER_LEN]
Model number of the device. If empty, the default model number is used. 

model_name

char model_name[WPS_MAX_MODEL_NAME_LEN]
Model name of the device. If empty, the default model name is used. 

device_name

char device_name[WPS_MAX_DEVICE_NAME_LEN]
Device name. If empty, the default device name is used. 



```

#### esp_wps_config_t


```


struct esp_wps_config_t
Structure representing configuration settings for WPS (Wi-Fi Protected Setup). 
This structure encapsulates various configuration settings for WPS, including the WPS type (PBC or PIN), factory information that will be shown in the WPS Information Element (IE), and a PIN if the WPS type is set to PIN. 

Public Members
wps_type

wps_type_t wps_type
The type of WPS to be used (PBC or PIN). 

factory_info

wps_factory_information_t factory_info
Factory information to be shown in the WPS Information Element (IE). Vendor can choose to display their own information. 

pin

char pin[PIN_LEN]
WPS PIN (Personal Identification Number) used when wps_type is set to WPS_TYPE_PIN. 



```



### Macros


#### ESP_ERR_WIFI_REGISTRAR


```


ESP_ERR_WIFI_REGISTRAR
WPS registrar is not supported 

```

#### ESP_ERR_WIFI_WPS_TYPE


```


ESP_ERR_WIFI_WPS_TYPE
WPS type error 

```

#### ESP_ERR_WIFI_WPS_SM


```


ESP_ERR_WIFI_WPS_SM
WPS state machine is not initialized 

```

#### WPS_MAX_MANUFACTURER_LEN


```


WPS_MAX_MANUFACTURER_LEN
Maximum length of the manufacturer name in WPS information 

```

#### WPS_MAX_MODEL_NUMBER_LEN


```


WPS_MAX_MODEL_NUMBER_LEN
Maximum length of the model number in WPS information 

```

#### WPS_MAX_MODEL_NAME_LEN


```


WPS_MAX_MODEL_NAME_LEN
Maximum length of the model name in WPS information 

```

#### WPS_MAX_DEVICE_NAME_LEN


```


WPS_MAX_DEVICE_NAME_LEN
Maximum length of the device name in WPS information 

```

#### PIN_LEN


```


PIN_LEN
The length of the WPS PIN (Personal Identification Number). 

```

#### WPS_CONFIG_INIT_DEFAULT


```


WPS_CONFIG_INIT_DEFAULT(type)
Initialize a default WPS configuration structure with specified WPS type. 
This macro initializes a esp_wps_config_t structure with default values for the specified WPS type. It sets the WPS type, factory information (including default manufacturer, model number, model name, and device name), and a default PIN value if applicable.

Parameters

type -- The WPS type to be used (PBC or PIN). 


Returns
An initialized esp_wps_config_t structure with the specified WPS type and default values. 



```



### Type Definitions


#### wps_type_t


```


typedef enum wps_type wps_type_t
Enumeration of WPS (Wi-Fi Protected Setup) types. 

```



### Enumerations


#### wps_type


```


enum wps_type
Enumeration of WPS (Wi-Fi Protected Setup) types. 
Values:
WPS_TYPE_DISABLE

enumerator WPS_TYPE_DISABLE
WPS is disabled 

WPS_TYPE_PBC

enumerator WPS_TYPE_PBC
WPS Push Button Configuration method 

WPS_TYPE_PIN

enumerator WPS_TYPE_PIN
WPS PIN (Personal Identification Number) method 

WPS_TYPE_MAX

enumerator WPS_TYPE_MAX
Maximum value for WPS type enumeration 


```



### Header File


* components/wpa_supplicant/esp_supplicant/include/esp_rrm.h
* This header file can be included with:



> ```
> #include "esp_rrm.h"
> 
> ```
* This header file is a part of the API provided by the `wpa_supplicant` component. To declare that your component depends on `wpa_supplicant`, add the following to your CMakeLists.txt:



> ```
> REQUIRES wpa_supplicant
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES wpa_supplicant
> 
> ```




### Functions


#### esp_rrm_send_neighbor_rep_request


```


int esp_rrm_send_neighbor_rep_request(neighbor_rep_request_cb cb, void *cb_ctx)
Send Radio measurement neighbor report request to connected AP. 


Deprecated:
This function is deprecated and will be removed in the future. Please use 'esp_rrm_send_neighbor_report_request' 



Parameters

cb -- callback function for neighbor report 
cb_ctx -- callback context


Returns

0: success
-1: AP does not support RRM
-2: station not connected to AP 





```

#### esp_rrm_send_neighbor_report_request


```


int esp_rrm_send_neighbor_report_request(void)
Send Radio measurement neighbor report request to connected AP. 

Returns

0: success
-1: AP does not support RRM
-2: station not connected to AP 





```

#### esp_rrm_is_rrm_supported_connection


```


bool esp_rrm_is_rrm_supported_connection(void)
Check RRM capability of connected AP. 

Returns

true: AP supports RRM
false: AP does not support RRM or station not connected to AP 





```



### Type Definitions


#### neighbor_rep_request_cb


```


typedef void (*neighbor_rep_request_cb)(void *ctx, const uint8_t *report, size_t report_len)
Callback function type to get neighbor report. 

Param ctx
neighbor report context 

Param report
neighbor report 

Param report_len
neighbor report length

Return

void 





```



### Header File


* components/wpa_supplicant/esp_supplicant/include/esp_wnm.h
* This header file can be included with:



> ```
> #include "esp_wnm.h"
> 
> ```
* This header file is a part of the API provided by the `wpa_supplicant` component. To declare that your component depends on `wpa_supplicant`, add the following to your CMakeLists.txt:



> ```
> REQUIRES wpa_supplicant
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES wpa_supplicant
> 
> ```




### Functions


#### esp_wnm_send_bss_transition_mgmt_query


```


int esp_wnm_send_bss_transition_mgmt_query(enum btm_query_reason query_reason, const char *btm_candidates, int cand_list)
Send bss transition query to connected AP. 

Parameters

query_reason -- reason for sending query 
btm_candidates -- btm candidates list if available 
cand_list -- whether candidate list to be included from scan results available in supplicant's cache.


Returns

0: success
-1: AP does not support BTM
-2: station not connected to AP 





```

#### esp_wnm_is_btm_supported_connection


```


bool esp_wnm_is_btm_supported_connection(void)
Check bss trasition capability of connected AP. 

Returns

true: AP supports BTM
false: AP does not support BTM or station not connected to AP 





```



### Enumerations


#### btm_query_reason


```


enum btm_query_reason
enum btm_query_reason: Reason code for sending btm query 
Values:
REASON_UNSPECIFIED

enumerator REASON_UNSPECIFIED

REASON_FRAME_LOSS

enumerator REASON_FRAME_LOSS

REASON_DELAY

enumerator REASON_DELAY

REASON_BANDWIDTH

enumerator REASON_BANDWIDTH

REASON_LOAD_BALANCE

enumerator REASON_LOAD_BALANCE

REASON_RSSI

enumerator REASON_RSSI

REASON_RETRANSMISSIONS

enumerator REASON_RETRANSMISSIONS

REASON_INTERFERENCE

enumerator REASON_INTERFERENCE

REASON_GRAY_ZONE

enumerator REASON_GRAY_ZONE

REASON_PREMIUM_AP

enumerator REASON_PREMIUM_AP


```



### Header File


* components/wpa_supplicant/esp_supplicant/include/esp_mbo.h
* This header file can be included with:



> ```
> #include "esp_mbo.h"
> 
> ```
* This header file is a part of the API provided by the `wpa_supplicant` component. To declare that your component depends on `wpa_supplicant`, add the following to your CMakeLists.txt:



> ```
> REQUIRES wpa_supplicant
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES wpa_supplicant
> 
> ```




### Functions


#### esp_mbo_update_non_pref_chan


```


int esp_mbo_update_non_pref_chan(struct non_pref_chan_s *non_pref_chan)
Update channel preference for MBO IE. 

Parameters
non_pref_chan -- Non preference channel list

Returns

0: success else failure 





```



### Structures


#### non_pref_chan


```


struct non_pref_chan
Structure representing a non-preferred channel in a wireless network. 
This structure encapsulates information about a non-preferred channel including the reason for its non-preference, the operating class, channel number, and preference level. 

Public Members
reason

enum non_pref_chan_reason reason
Reason for the channel being non-preferred 

oper_class

uint8_t oper_class
Operating class of the channel 

chan

uint8_t chan
Channel number 

preference

uint8_t preference
Preference level of the channel 



```

#### non_pref_chan_s


```


struct non_pref_chan_s
Structure representing a list of non-preferred channels in a wireless network. 
This structure encapsulates information about a list of non-preferred channels including the number of non-preferred channels and an array of structures representing individual non-preferred channels. 

Public Members
non_pref_chan_num

size_t non_pref_chan_num
Number of non-preferred channels in the list 

chan

struct non_pref_chan chan[]
Array of structures representing individual non-preferred channels 



```



### Enumerations


#### non_pref_chan_reason


```


enum non_pref_chan_reason
Enumeration of reasons for a channel being non-preferred in a wireless network. 
This enumeration defines various reasons why a specific channel might be considered non-preferred in a wireless network configuration. 
Values:
NON_PREF_CHAN_REASON_UNSPECIFIED

enumerator NON_PREF_CHAN_REASON_UNSPECIFIED
Unspecified reason for non-preference 

NON_PREF_CHAN_REASON_RSSI

enumerator NON_PREF_CHAN_REASON_RSSI
Non-preferred due to low RSSI (Received Signal Strength Indication) 

NON_PREF_CHAN_REASON_EXT_INTERFERENCE

enumerator NON_PREF_CHAN_REASON_EXT_INTERFERENCE
Non-preferred due to external interference 

NON_PREF_CHAN_REASON_INT_INTERFERENCE

enumerator NON_PREF_CHAN_REASON_INT_INTERFERENCE
Non-preferred due to internal interference 


```