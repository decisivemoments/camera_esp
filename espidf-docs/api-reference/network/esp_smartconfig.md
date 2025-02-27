# SmartConfig



The SmartConfigTM is a provisioning technology developed by TI to connect a new Wi-Fi device to a Wi-Fi network. It uses a mobile application to broadcast the network credentials from a smartphone, or a tablet, to an un-provisioned Wi-Fi device.


The advantage of this technology is that the device does not need to directly know SSID or password of an Access Point (AP). This information is provided using the smartphone. This is particularly important to headless device and systems, due to their lack of a user interface.


If you are looking for other options to provision your ESP32-S3 devices, check Provisioning API.



## Application Example


Connect ESP32-S3 to the target AP using SmartConfig: wifi/smart_config.




## API Reference



### Header File


* components/esp_wifi/include/esp_smartconfig.h
* This header file can be included with:



> ```
> #include "esp_smartconfig.h"
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


#### esp_smartconfig_get_version


```


const char *esp_smartconfig_get_version(void)
Get the version of SmartConfig. 

Returns

SmartConfig version const char. 





```

#### esp_smartconfig_start


```


esp_err_t esp_smartconfig_start(const smartconfig_start_config_t *config)
Start SmartConfig, config ESP device to connect AP. You need to broadcast information by phone APP. Device sniffer special packets from the air that containing SSID and password of target AP. 

Attention1. This API can be called in station or softAP-station mode. 

Attention2. Can not call esp_smartconfig_start twice before it finish, please call esp_smartconfig_stop first.




Parameters
config -- pointer to smartconfig start configure structure

Returns

ESP_OK: succeed
others: fail 





```

#### esp_smartconfig_stop


```


esp_err_t esp_smartconfig_stop(void)
Stop SmartConfig, free the buffer taken by esp_smartconfig_start. 

AttentionWhether connect to AP succeed or not, this API should be called to free memory taken by smartconfig_start.




Returns

ESP_OK: succeed
others: fail 





```

#### esp_esptouch_set_timeout


```


esp_err_t esp_esptouch_set_timeout(uint8_t time_s)
Set timeout of SmartConfig process. 

AttentionTiming starts from SC_STATUS_FIND_CHANNEL status. SmartConfig will restart if timeout.




Parameters
time_s -- range 15s~255s, offset:45s.

Returns

ESP_OK: succeed
others: fail 





```

#### esp_smartconfig_set_type


```


esp_err_t esp_smartconfig_set_type(smartconfig_type_t type)
Set protocol type of SmartConfig. 

AttentionIf users need to set the SmartConfig type, please set it before calling esp_smartconfig_start.




Parameters
type -- Choose from the smartconfig_type_t.

Returns

ESP_OK: succeed
others: fail 





```

#### esp_smartconfig_fast_mode


```


esp_err_t esp_smartconfig_fast_mode(bool enable)
Set mode of SmartConfig. default normal mode. 

Attention1. Please call it before API esp_smartconfig_start. 

Attention2. Fast mode have corresponding APP(phone). 

Attention3. Two mode is compatible.




Parameters
enable -- false-disable(default); true-enable;

Returns

ESP_OK: succeed
others: fail 





```

#### esp_smartconfig_get_rvd_data


```


esp_err_t esp_smartconfig_get_rvd_data(uint8_t *rvd_data, uint8_t len)
Get reserved data of ESPTouch v2. 

Parameters

rvd_data -- reserved data 
len -- length of reserved data


Returns

ESP_OK: succeed
others: fail 





```



### Structures


#### smartconfig_event_got_ssid_pswd_t


```


struct smartconfig_event_got_ssid_pswd_t
Argument structure for SC_EVENT_GOT_SSID_PSWD event 

Public Members
ssid

uint8_t ssid[32]
SSID of the AP. Null terminated string. 

password

uint8_t password[64]
Password of the AP. Null terminated string. 

bssid_set

bool bssid_set
whether set MAC address of target AP or not. 

bssid

uint8_t bssid[6]
MAC address of target AP. 

type

smartconfig_type_t type
Type of smartconfig(ESPTouch or AirKiss). 

token

uint8_t token
Token from cellphone which is used to send ACK to cellphone. 

cellphone_ip

uint8_t cellphone_ip[4]
IP address of cellphone. 



```

#### smartconfig_start_config_t


```


struct smartconfig_start_config_t
Configure structure for esp_smartconfig_start 

Public Members
enable_log

bool enable_log
Enable smartconfig logs. 

esp_touch_v2_enable_crypt

bool esp_touch_v2_enable_crypt
Enable ESPTouch v2 crypt. 

esp_touch_v2_key

char *esp_touch_v2_key
ESPTouch v2 crypt key, len should be 16. 



```



### Macros


#### SMARTCONFIG_START_CONFIG_DEFAULT


```


SMARTCONFIG_START_CONFIG_DEFAULT()

```



### Enumerations


#### smartconfig_type_t


```


enum smartconfig_type_t
Values:
SC_TYPE_ESPTOUCH

enumerator SC_TYPE_ESPTOUCH
protocol: ESPTouch 

SC_TYPE_AIRKISS

enumerator SC_TYPE_AIRKISS
protocol: AirKiss 

SC_TYPE_ESPTOUCH_AIRKISS

enumerator SC_TYPE_ESPTOUCH_AIRKISS
protocol: ESPTouch and AirKiss 

SC_TYPE_ESPTOUCH_V2

enumerator SC_TYPE_ESPTOUCH_V2
protocol: ESPTouch v2 


```

#### smartconfig_event_t


```


enum smartconfig_event_t
Smartconfig event declarations 
Values:
SC_EVENT_SCAN_DONE

enumerator SC_EVENT_SCAN_DONE
Station smartconfig has finished to scan for APs 

SC_EVENT_FOUND_CHANNEL

enumerator SC_EVENT_FOUND_CHANNEL
Station smartconfig has found the channel of the target AP 

SC_EVENT_GOT_SSID_PSWD

enumerator SC_EVENT_GOT_SSID_PSWD
Station smartconfig got the SSID and password 

SC_EVENT_SEND_ACK_DONE

enumerator SC_EVENT_SEND_ACK_DONE
Station smartconfig has sent ACK to cellphone 


```