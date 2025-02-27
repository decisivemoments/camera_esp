# Bluetooth® Device APIs



## Overview


Bluetooth device reference APIs.




## API Reference



### Header File


* components/bt/host/bluedroid/api/include/api/esp_bt_device.h
* This header file can be included with:



> ```
> #include "esp_bt_device.h"
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


#### esp_bt_dev_register_callback


```


esp_err_t esp_bt_dev_register_callback(esp_bt_dev_cb_t callback)
register callback function. This function should be called after esp_bluedroid_enable() completes successfully 

Returns

ESP_OK : Succeed
ESP_FAIL: others 





```

#### esp_bt_dev_get_address


```


const uint8_t *esp_bt_dev_get_address(void)
Get bluetooth device address. Must use after "esp_bluedroid_enable". 

Returns
bluetooth device address (six bytes), or NULL if bluetooth stack is not enabled 



```

#### esp_bt_dev_set_device_name


```


esp_err_t esp_bt_dev_set_device_name(const char *name)
Set bluetooth device name. This function should be called after esp_bluedroid_enable() completes successfully. 
A BR/EDR/LE device type shall have a single Bluetooth device name which shall be identical irrespective of the physical channel used to perform the name discovery procedure.

Parameters
name -- [in] : device name to be set

Returns

ESP_OK : Succeed
ESP_ERR_INVALID_ARG : if name is NULL pointer or empty, or string length out of limit
ESP_ERR_INVALID_STATE : if bluetooth stack is not yet enabled
ESP_FAIL : others 





```

#### esp_bt_dev_get_device_name


```


esp_err_t esp_bt_dev_get_device_name(void)
Get bluetooth device name. This function should be called after esp_bluedroid_enable() completes successfully. 
A BR/EDR/LE device type shall have a single Bluetooth device name which shall be identical irrespective of the physical channel used to perform the name discovery procedure.

Returns

ESP_OK : Succeed
ESP_ERR_INVALID_STATE : if bluetooth stack is not yet enabled
ESP_FAIL : others 





```

#### esp_bt_dev_coex_status_config


```


esp_err_t esp_bt_dev_coex_status_config(esp_bt_dev_coex_type_t type, esp_bt_dev_coex_op_t op, uint8_t status)
Config bluetooth device coexis status. This function should be called after esp_bluedroid_enable() completes successfully. 

Parameters

type -- [in] : coexist type to operate on 
op -- [in] : clear or set coexist status 
status -- [in] : coexist status to be configured


Returns

ESP_OK : Succeed
ESP_ERR_INVALID_ARG : if name is NULL pointer or empty, or string length out of limit
ESP_ERR_INVALID_STATE : if bluetooth stack is not yet enabled
ESP_FAIL : others 





```

#### esp_bt_config_file_path_update


```


esp_err_t esp_bt_config_file_path_update(const char *file_path)
This function is used to update the path name of bluetooth bond keys saved in the NVS module and need to be called before esp_bluedroid_init(). 

Parameters
file_path -- [in] the name of config file path, the length of file_path should be less than NVS_NS_NAME_MAX_SIZE

Returns

ESP_OK: success
other: failed 





```



### Unions


#### esp_bt_dev_cb_param_t


```


union esp_bt_dev_cb_param_t

#include <esp_bt_device.h>
BT device callback parameters. 

Public Members
name_res

struct esp_bt_dev_cb_param_t::name_res_param name_res
discovery result parameter struct 


name_res_param

struct name_res_param

#include <esp_bt_device.h>
ESP_BT_DEV_NAME_RES_EVT. 

Public Members
status

esp_bt_status_t status
Status of getting device name 

name

char *name
Name of Bluetooth device 




```



### Macros


#### ESP_BT_DEV_COEX_BLE_ST_MESH_CONFIG


```


ESP_BT_DEV_COEX_BLE_ST_MESH_CONFIG

```

#### ESP_BT_DEV_COEX_BLE_ST_MESH_TRAFFIC


```


ESP_BT_DEV_COEX_BLE_ST_MESH_TRAFFIC

```

#### ESP_BT_DEV_COEX_BLE_ST_MESH_STANDBY


```


ESP_BT_DEV_COEX_BLE_ST_MESH_STANDBY

```

#### ESP_BT_DEV_COEX_BT_ST_A2DP_STREAMING


```


ESP_BT_DEV_COEX_BT_ST_A2DP_STREAMING

```

#### ESP_BT_DEV_COEX_BT_ST_A2DP_PAUSED


```


ESP_BT_DEV_COEX_BT_ST_A2DP_PAUSED

```

#### ESP_BT_DEV_COEX_OP_CLEAR


```


ESP_BT_DEV_COEX_OP_CLEAR

```

#### ESP_BT_DEV_COEX_OP_SET


```


ESP_BT_DEV_COEX_OP_SET

```



### Type Definitions


#### esp_bt_dev_coex_op_t


```


typedef uint8_t esp_bt_dev_coex_op_t

```

#### esp_bt_dev_cb_t


```


typedef void (*esp_bt_dev_cb_t)(esp_bt_dev_cb_event_t event, esp_bt_dev_cb_param_t *param)
bluetooth device callback function type 

Param event
: Event type

Param param
: Pointer to callback parameter 



```



### Enumerations


#### esp_bt_dev_coex_type_t


```


enum esp_bt_dev_coex_type_t
Bluetooth device coex type. 
Values:
ESP_BT_DEV_COEX_TYPE_BLE

enumerator ESP_BT_DEV_COEX_TYPE_BLE

ESP_BT_DEV_COEX_TYPE_BT

enumerator ESP_BT_DEV_COEX_TYPE_BT


```

#### esp_bt_dev_cb_event_t


```


enum esp_bt_dev_cb_event_t
BT device callback events. 
Values:
ESP_BT_DEV_NAME_RES_EVT

enumerator ESP_BT_DEV_NAME_RES_EVT
Device name result event 

ESP_BT_DEV_EVT_MAX

enumerator ESP_BT_DEV_EVT_MAX


```