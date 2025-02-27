# Bluetooth® Main API



## API Reference



### Header File


* components/bt/host/bluedroid/api/include/api/esp_bt_main.h
* This header file can be included with:



> ```
> #include "esp_bt_main.h"
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


#### esp_bluedroid_get_status


```


esp_bluedroid_status_t esp_bluedroid_get_status(void)
Get bluetooth stack status. 

Returns
Bluetooth stack status 



```

#### esp_bluedroid_enable


```


esp_err_t esp_bluedroid_enable(void)
Enable bluetooth, must after esp_bluedroid_init()/esp_bluedroid_init_with_cfg(). 

Returns

ESP_OK : Succeed
Other : Failed 





```

#### esp_bluedroid_disable


```


esp_err_t esp_bluedroid_disable(void)
Disable Bluetooth, must be called prior to esp_bluedroid_deinit(). 

Note
Before calling this API, ensure that all activities related to the application, such as connections, scans, etc., are properly closed.


Returns

ESP_OK : Succeed
Other : Failed 





```

#### esp_bluedroid_init


```


esp_err_t esp_bluedroid_init(void)
Init and alloc the resource for bluetooth, must be prior to every bluetooth stuff. 

Returns

ESP_OK : Succeed
Other : Failed 





```

#### esp_bluedroid_init_with_cfg


```


esp_err_t esp_bluedroid_init_with_cfg(esp_bluedroid_config_t *cfg)
Init and alloc the resource for bluetooth, must be prior to every bluetooth stuff. 

Parameters
cfg -- Initial configuration of ESP Bluedroid stack.

Returns

ESP_OK : Succeed
Other : Failed 





```

#### esp_bluedroid_deinit


```


esp_err_t esp_bluedroid_deinit(void)
Deinit and free the resource for bluetooth, must be after every bluetooth stuff. 

Returns

ESP_OK : Succeed
Other : Failed 





```



### Structures


#### esp_bluedroid_config_t


```


struct esp_bluedroid_config_t
Bluetooth stack configuration. 

Public Members
ssp_en

bool ssp_en
Whether SSP(secure simple pairing) or legacy pairing is used for Classic Bluetooth 



```



### Macros


#### BT_BLUEDROID_INIT_CONFIG_DEFAULT


```


BT_BLUEDROID_INIT_CONFIG_DEFAULT()

```



### Enumerations


#### esp_bluedroid_status_t


```


enum esp_bluedroid_status_t
Bluetooth stack status type, to indicate whether the bluetooth stack is ready. 
Values:
ESP_BLUEDROID_STATUS_UNINITIALIZED

enumerator ESP_BLUEDROID_STATUS_UNINITIALIZED
Bluetooth not initialized 

ESP_BLUEDROID_STATUS_INITIALIZED

enumerator ESP_BLUEDROID_STATUS_INITIALIZED
Bluetooth initialized but not enabled 

ESP_BLUEDROID_STATUS_ENABLED

enumerator ESP_BLUEDROID_STATUS_ENABLED
Bluetooth initialized and enabled 


```