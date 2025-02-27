# Wi-Fi Easy ConnectTM (DPP)



Wi-Fi Easy ConnectTM, also known as Device Provisioning Protocol (DPP) or Easy Connect, is a provisioning protocol certified by Wi-Fi Alliance. It is a secure and standardized provisioning protocol for configuration of Wi-Fi Devices. With Easy Connect, adding a new device to a network is as simple as scanning a QR Code. This reduces complexity and enhances user experience while onboarding devices without UI like Smart Home and IoT products. Unlike old protocols like Wi-Fi Protected Setup (WPS), Wi-Fi Easy Connect in corporates strong encryption through public key cryptography to ensure networks remain secure as new devices are added.


Easy Connect brings many benefits in the user experience:



> * Simple and intuitive to use; no lengthy instructions to follow for new device setup
> * No need to remember and enter passwords into the device being provisioned
> * Works with electronic or printed QR codes, or human-readable strings
> * Supports both WPA2 and WPA3 networks


Please refer to Wi-Fi Alliance's official page on Easy Connect for more information.


ESP32-S3 supports Enrollee mode of Easy Connect with QR Code as the provisioning method. A display is required to display this QR Code. Users can scan this QR Code using their capable device and provision the ESP32-S3 to their Wi-Fi network. The provisioning device needs to be connected to the AP which need not support Wi-Fi Easy ConnectTM.


Easy Connect is still an evolving protocol. Of known platforms that support the QR Code method are some Android smartphones with Android 10 or higher. To use Easy Connect, no additional App needs to be installed on the supported smartphone.



## Application Example


Example on how to provision ESP32-S3 using a supported smartphone: wifi/wifi_easy_connect/dpp-enrollee.




## API Reference



### Header File


* components/wpa_supplicant/esp_supplicant/include/esp_dpp.h
* This header file can be included with:



> ```
> #include "esp_dpp.h"
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


#### esp_supp_dpp_init


```


esp_err_t esp_supp_dpp_init(esp_supp_dpp_event_cb_t evt_cb)
Initialize DPP Supplicant. 
   Starts DPP Supplicant and initializes related Data Structures.


 
return
ESP_OK: Success
ESP_FAIL: Failure 



Parameters
evt_cb -- Callback function to receive DPP related events



```

#### esp_supp_dpp_deinit


```


esp_err_t esp_supp_dpp_deinit(void)
De-initalize DPP Supplicant. 
   Frees memory from DPP Supplicant Data Structures.




Returns

ESP_OK: Success 





```

#### esp_supp_dpp_bootstrap_gen


```


esp_err_t esp_supp_dpp_bootstrap_gen(const char *chan_list, esp_supp_dpp_bootstrap_t type, const char *key, const char *info)
Generates Bootstrap Information as an Enrollee. 
   Generates Out Of Band Bootstrap information as an Enrollee which can be
   used by a DPP Configurator to provision the Enrollee.




Parameters

chan_list -- List of channels device will be available on for listening 
type -- Bootstrap method type, only QR Code method is supported for now. 
key -- (Optional) 32 byte Raw Private Key for generating a Bootstrapping Public Key 
info -- (Optional) Ancilliary Device Information like Serial Number


Returns

ESP_OK: Success
ESP_FAIL: Failure 





```

#### esp_supp_dpp_start_listen


```


esp_err_t esp_supp_dpp_start_listen(void)
Start listening on Channels provided during esp_supp_dpp_bootstrap_gen. 
   Listens on every Channel from Channel List for a pre-defined wait time.




Returns

ESP_OK: Success
ESP_FAIL: Generic Failure
ESP_ERR_INVALID_STATE: ROC attempted before WiFi is started
ESP_ERR_NO_MEM: Memory allocation failed while posting ROC request 





```

#### esp_supp_dpp_stop_listen


```


esp_err_t esp_supp_dpp_stop_listen(void)
Stop listening on Channels. 
   Stops listening on Channels and cancels ongoing listen operation.




Returns

ESP_OK: Success
ESP_FAIL: Failure 





```



### Macros


#### ESP_DPP_AUTH_TIMEOUT_SECS


```


ESP_DPP_AUTH_TIMEOUT_SECS

```

#### ESP_ERR_DPP_FAILURE


```


ESP_ERR_DPP_FAILURE
Generic failure during DPP Operation 

```

#### ESP_ERR_DPP_TX_FAILURE


```


ESP_ERR_DPP_TX_FAILURE
DPP Frame Tx failed OR not Acked 

```

#### ESP_ERR_DPP_INVALID_ATTR


```


ESP_ERR_DPP_INVALID_ATTR
Encountered invalid DPP Attribute 

```

#### ESP_ERR_DPP_AUTH_TIMEOUT


```


ESP_ERR_DPP_AUTH_TIMEOUT
DPP Auth response was not recieved in time 

```



### Type Definitions


#### esp_supp_dpp_bootstrap_t


```


typedef enum dpp_bootstrap_type esp_supp_dpp_bootstrap_t
Types of Bootstrap Methods for DPP. 

```

#### esp_supp_dpp_event_cb_t


```


typedef void (*esp_supp_dpp_event_cb_t)(esp_supp_dpp_event_t evt, void *data)
Callback function for receiving DPP Events from Supplicant. 
   Callback function will be called with DPP related information.




Param evt
DPP event ID 

Param data
Event data payload 



```



### Enumerations


#### dpp_bootstrap_type


```


enum dpp_bootstrap_type
Types of Bootstrap Methods for DPP. 
Values:
DPP_BOOTSTRAP_QR_CODE

enumerator DPP_BOOTSTRAP_QR_CODE
QR Code Method 

DPP_BOOTSTRAP_PKEX

enumerator DPP_BOOTSTRAP_PKEX
Proof of Knowledge Method 

DPP_BOOTSTRAP_NFC_URI

enumerator DPP_BOOTSTRAP_NFC_URI
NFC URI record Method 


```

#### esp_supp_dpp_event_t


```


enum esp_supp_dpp_event_t
Types of Callback Events received from DPP Supplicant. 
Values:
ESP_SUPP_DPP_URI_READY

enumerator ESP_SUPP_DPP_URI_READY
URI is ready through Bootstrapping 

ESP_SUPP_DPP_CFG_RECVD

enumerator ESP_SUPP_DPP_CFG_RECVD
Config received via DPP Authentication 

ESP_SUPP_DPP_PDR_RECVD

enumerator ESP_SUPP_DPP_PDR_RECVD
Peer Discovery Response is received 

ESP_SUPP_DPP_FAIL

enumerator ESP_SUPP_DPP_FAIL
DPP Authentication failure 


```