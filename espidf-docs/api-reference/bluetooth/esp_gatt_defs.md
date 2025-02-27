# GATT Defines



## API Reference



### Header File


* components/bt/host/bluedroid/api/include/api/esp_gatt_defs.h
* This header file can be included with:



> ```
> #include "esp_gatt_defs.h"
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




### Unions


#### esp_gatt_rsp_t


```


union esp_gatt_rsp_t

#include <esp_gatt_defs.h>
Represents the response type for a GATT remote read request. 

Public Members
attr_value

esp_gatt_value_t attr_value
The GATT attribute value, including its data, handle, and metadata. 

handle

uint16_t handle
Only the handle of the GATT attribute, when that's the only required information. 



```



### Structures


#### esp_gatt_id_t


```


struct esp_gatt_id_t
Represents a GATT identifier. 

Public Members
uuid

esp_bt_uuid_t uuid
The UUID component of the GATT ID. 

inst_id

uint8_t inst_id
The instance ID component of the GATT ID, providing further differentiation of the GATT ID. 



```

#### esp_gatt_srvc_id_t


```


struct esp_gatt_srvc_id_t
Represents a GATT service identifier. 

Public Members
id

esp_gatt_id_t id
Encapsulates the UUID and instance ID of the GATT service. 

is_primary

bool is_primary
Indicates if the service is primary. A value of true means it is a primary service, false indicates a secondary service. 



```

#### esp_attr_desc_t


```


struct esp_attr_desc_t
Defines an attribute's description. 
This structure is used to describe an attribute in the GATT database. It includes details such as the UUID of the attribute, its permissions, and its value. 

Public Members
uuid_length

uint16_t uuid_length
Length of the UUID in bytes. 

uuid_p

uint8_t *uuid_p
Pointer to the UUID value. 

perm

uint16_t perm
Attribute permissions, defined by esp_gatt_perm_t. 

max_length

uint16_t max_length
Maximum length of the attribute's value. 

length

uint16_t length
Current length of the attribute's value. 

value

uint8_t *value
Pointer to the attribute's value array. 



```

#### esp_attr_control_t


```


struct esp_attr_control_t
Defines the auto response setting for attribute operations. 
This structure is used to control whether the GATT stack or the application will handle responses to Read/Write operations. 

Public Members
auto_rsp

uint8_t auto_rsp
Controls who handles the response to Read/Write operations. 

If set to ESP_GATT_RSP_BY_APP, the application is responsible for generating the response.
If set to ESP_GATT_AUTO_RSP, the GATT stack will automatically generate the response. 





```

#### esp_gatts_attr_db_t


```


struct esp_gatts_attr_db_t
attribute type added to the GATT server database 

Public Members
attr_control

esp_attr_control_t attr_control
The attribute control type 

att_desc

esp_attr_desc_t att_desc
The attribute type 



```

#### esp_attr_value_t


```


struct esp_attr_value_t
set the attribute value type 

Public Members
attr_max_len

uint16_t attr_max_len
attribute max value length 

attr_len

uint16_t attr_len
attribute current value length 

attr_value

uint8_t *attr_value
the pointer to attribute value 



```

#### esp_gatts_incl_svc_desc_t


```


struct esp_gatts_incl_svc_desc_t
Gatt include service entry element. 

Public Members
start_hdl

uint16_t start_hdl
Gatt start handle value of included service 

end_hdl

uint16_t end_hdl
Gatt end handle value of included service 

uuid

uint16_t uuid
Gatt attribute value UUID of included service 



```

#### esp_gatts_incl128_svc_desc_t


```


struct esp_gatts_incl128_svc_desc_t
Gatt include 128 bit service entry element. 

Public Members
start_hdl

uint16_t start_hdl
Gatt start handle value of included 128 bit service 

end_hdl

uint16_t end_hdl
Gatt end handle value of included 128 bit service 



```

#### esp_gatt_value_t


```


struct esp_gatt_value_t
Represents a GATT attribute's value. 

Public Members
value

uint8_t value[ESP_GATT_MAX_ATTR_LEN]
Array holding the value of the GATT attribute. 

handle

uint16_t handle
Unique identifier (handle) of the GATT attribute. 

offset

uint16_t offset
Offset within the attribute's value, for partial updates. 

len

uint16_t len
Current length of the data in the value array. 

auth_req

uint8_t auth_req
Authentication requirements for accessing this attribute. 



```

#### esp_gatt_conn_params_t


```


struct esp_gatt_conn_params_t
Connection parameters for GATT. 

Public Members
interval

uint16_t interval
Connection interval. 

latency

uint16_t latency
Slave latency for the connection in number of connection events. 

timeout

uint16_t timeout
Supervision timeout for the LE Link. 



```

#### esp_gattc_multi_t


```


struct esp_gattc_multi_t
Represents multiple attributes for reading. 

Public Members
num_attr

uint8_t num_attr
Number of attributes. 

handles

uint16_t handles[ESP_GATT_MAX_READ_MULTI_HANDLES]
List of attribute handles. 



```

#### esp_gattc_db_elem_t


```


struct esp_gattc_db_elem_t
GATT database attribute element. 

Public Members
type

esp_gatt_db_attr_type_t type
Attribute type. 

attribute_handle

uint16_t attribute_handle
Attribute handle. 

start_handle

uint16_t start_handle
Service start handle. 

end_handle

uint16_t end_handle
Service end handle. 

properties

esp_gatt_char_prop_t properties
Characteristic properties. 

uuid

esp_bt_uuid_t uuid
Attribute UUID. 



```

#### esp_gattc_service_elem_t


```


struct esp_gattc_service_elem_t
Represents a GATT service element. 

Public Members
is_primary

bool is_primary
Indicates if the service is primary. 

start_handle

uint16_t start_handle
Service start handle. 

end_handle

uint16_t end_handle
Service end handle. 

uuid

esp_bt_uuid_t uuid
Service UUID. 



```

#### esp_gattc_char_elem_t


```


struct esp_gattc_char_elem_t
Represents a GATT characteristic element. 

Public Members
char_handle

uint16_t char_handle
Characteristic handle. 

properties

esp_gatt_char_prop_t properties
Characteristic properties. 

uuid

esp_bt_uuid_t uuid
Characteristic UUID. 



```

#### esp_gattc_descr_elem_t


```


struct esp_gattc_descr_elem_t
Represents a GATT descriptor element. 

Public Members
handle

uint16_t handle
Descriptor handle. 

uuid

esp_bt_uuid_t uuid
Descriptor UUID. 



```

#### esp_gattc_incl_svc_elem_t


```


struct esp_gattc_incl_svc_elem_t
Represents an included GATT service element. 

Public Members
handle

uint16_t handle
Current attribute handle of the included service. 

incl_srvc_s_handle

uint16_t incl_srvc_s_handle
Start handle of the included service. 

incl_srvc_e_handle

uint16_t incl_srvc_e_handle
End handle of the included service. 

uuid

esp_bt_uuid_t uuid
Included service UUID. 



```



### Macros


#### ESP_GATT_ILLEGAL_UUID


```


ESP_GATT_ILLEGAL_UUID
GATT INVALID UUID. 

```

#### ESP_GATT_ILLEGAL_HANDLE


```


ESP_GATT_ILLEGAL_HANDLE
GATT INVALID HANDLE. 

```

#### ESP_GATT_ATTR_HANDLE_MAX


```


ESP_GATT_ATTR_HANDLE_MAX
GATT attribute max handle. 

```

#### ESP_GATT_MAX_READ_MULTI_HANDLES


```


ESP_GATT_MAX_READ_MULTI_HANDLES
Maximum number of attributes to read in one request. 

```

#### ESP_GATT_UUID_IMMEDIATE_ALERT_SVC


```


ESP_GATT_UUID_IMMEDIATE_ALERT_SVC
Immediate Alert Service UUID. 

```

#### ESP_GATT_UUID_LINK_LOSS_SVC


```


ESP_GATT_UUID_LINK_LOSS_SVC
Link Loss Service UUID. 

```

#### ESP_GATT_UUID_TX_POWER_SVC


```


ESP_GATT_UUID_TX_POWER_SVC
TX Power Service UUID. 

```

#### ESP_GATT_UUID_CURRENT_TIME_SVC


```


ESP_GATT_UUID_CURRENT_TIME_SVC
Current Time Service UUID. 

```

#### ESP_GATT_UUID_REF_TIME_UPDATE_SVC


```


ESP_GATT_UUID_REF_TIME_UPDATE_SVC
Reference Time Update Service UUID. 

```

#### ESP_GATT_UUID_NEXT_DST_CHANGE_SVC


```


ESP_GATT_UUID_NEXT_DST_CHANGE_SVC
Next DST Change Service UUID. 

```

#### ESP_GATT_UUID_GLUCOSE_SVC


```


ESP_GATT_UUID_GLUCOSE_SVC
Glucose Service UUID. 

```

#### ESP_GATT_UUID_HEALTH_THERMOM_SVC


```


ESP_GATT_UUID_HEALTH_THERMOM_SVC
Health Thermometer Service UUID. 

```

#### ESP_GATT_UUID_DEVICE_INFO_SVC


```


ESP_GATT_UUID_DEVICE_INFO_SVC
Device Information Service UUID. 

```

#### ESP_GATT_UUID_HEART_RATE_SVC


```


ESP_GATT_UUID_HEART_RATE_SVC
Heart Rate Service UUID. 

```

#### ESP_GATT_UUID_PHONE_ALERT_STATUS_SVC


```


ESP_GATT_UUID_PHONE_ALERT_STATUS_SVC
Phone Alert Status Service UUID. 

```

#### ESP_GATT_UUID_BATTERY_SERVICE_SVC


```


ESP_GATT_UUID_BATTERY_SERVICE_SVC
Battery Service UUID. 

```

#### ESP_GATT_UUID_BLOOD_PRESSURE_SVC


```


ESP_GATT_UUID_BLOOD_PRESSURE_SVC
Blood Pressure Service UUID. 

```

#### ESP_GATT_UUID_ALERT_NTF_SVC


```


ESP_GATT_UUID_ALERT_NTF_SVC
Alert Notification Service UUID. 

```

#### ESP_GATT_UUID_HID_SVC


```


ESP_GATT_UUID_HID_SVC
HID Service UUID. 

```

#### ESP_GATT_UUID_SCAN_PARAMETERS_SVC


```


ESP_GATT_UUID_SCAN_PARAMETERS_SVC
Scan Parameters Service UUID. 

```

#### ESP_GATT_UUID_RUNNING_SPEED_CADENCE_SVC


```


ESP_GATT_UUID_RUNNING_SPEED_CADENCE_SVC
Running Speed and Cadence Service UUID. 

```

#### ESP_GATT_UUID_Automation_IO_SVC


```


ESP_GATT_UUID_Automation_IO_SVC
Automation IO Service UUID. 

```

#### ESP_GATT_UUID_CYCLING_SPEED_CADENCE_SVC


```


ESP_GATT_UUID_CYCLING_SPEED_CADENCE_SVC
Cycling Speed and Cadence Service UUID. 

```

#### ESP_GATT_UUID_CYCLING_POWER_SVC


```


ESP_GATT_UUID_CYCLING_POWER_SVC
Cycling Power Service UUID. 

```

#### ESP_GATT_UUID_LOCATION_AND_NAVIGATION_SVC


```


ESP_GATT_UUID_LOCATION_AND_NAVIGATION_SVC
Location and Navigation Service UUID. 

```

#### ESP_GATT_UUID_ENVIRONMENTAL_SENSING_SVC


```


ESP_GATT_UUID_ENVIRONMENTAL_SENSING_SVC
Environmental Sensing Service UUID. 

```

#### ESP_GATT_UUID_BODY_COMPOSITION


```


ESP_GATT_UUID_BODY_COMPOSITION
Body Composition Service UUID. 

```

#### ESP_GATT_UUID_USER_DATA_SVC


```


ESP_GATT_UUID_USER_DATA_SVC
User Data Service UUID. 

```

#### ESP_GATT_UUID_WEIGHT_SCALE_SVC


```


ESP_GATT_UUID_WEIGHT_SCALE_SVC
Weight Scale Service UUID. 

```

#### ESP_GATT_UUID_BOND_MANAGEMENT_SVC


```


ESP_GATT_UUID_BOND_MANAGEMENT_SVC
Bond Management Service UUID. 

```

#### ESP_GATT_UUID_CONT_GLUCOSE_MONITOR_SVC


```


ESP_GATT_UUID_CONT_GLUCOSE_MONITOR_SVC
Continuous Glucose Monitoring Service UUID. 

```

#### ESP_GATT_UUID_PRI_SERVICE


```


ESP_GATT_UUID_PRI_SERVICE
Primary Service UUID. 

```

#### ESP_GATT_UUID_SEC_SERVICE


```


ESP_GATT_UUID_SEC_SERVICE
Secondary Service UUID. 

```

#### ESP_GATT_UUID_INCLUDE_SERVICE


```


ESP_GATT_UUID_INCLUDE_SERVICE
Include Service UUID. 

```

#### ESP_GATT_UUID_CHAR_DECLARE


```


ESP_GATT_UUID_CHAR_DECLARE
Characteristic Declaration UUID. 

```

#### ESP_GATT_UUID_CHAR_EXT_PROP


```


ESP_GATT_UUID_CHAR_EXT_PROP
Characteristic Extended Properties UUID. 

```

#### ESP_GATT_UUID_CHAR_DESCRIPTION


```


ESP_GATT_UUID_CHAR_DESCRIPTION
Characteristic User Description UUID. 

```

#### ESP_GATT_UUID_CHAR_CLIENT_CONFIG


```


ESP_GATT_UUID_CHAR_CLIENT_CONFIG
Client Characteristic Configuration UUID. 

```

#### ESP_GATT_UUID_CHAR_SRVR_CONFIG


```


ESP_GATT_UUID_CHAR_SRVR_CONFIG
Server Characteristic Configuration UUID. 

```

#### ESP_GATT_UUID_CHAR_PRESENT_FORMAT


```


ESP_GATT_UUID_CHAR_PRESENT_FORMAT
Characteristic Presentation Format UUID. 

```

#### ESP_GATT_UUID_CHAR_AGG_FORMAT


```


ESP_GATT_UUID_CHAR_AGG_FORMAT
Characteristic Aggregate Format UUID. 

```

#### ESP_GATT_UUID_CHAR_VALID_RANGE


```


ESP_GATT_UUID_CHAR_VALID_RANGE
Characteristic Valid Range UUID. 

```

#### ESP_GATT_UUID_EXT_RPT_REF_DESCR


```


ESP_GATT_UUID_EXT_RPT_REF_DESCR
External Report Reference Descriptor UUID. 

```

#### ESP_GATT_UUID_RPT_REF_DESCR


```


ESP_GATT_UUID_RPT_REF_DESCR
Report Reference Descriptor UUID. 

```

#### ESP_GATT_UUID_NUM_DIGITALS_DESCR


```


ESP_GATT_UUID_NUM_DIGITALS_DESCR
Number of Digitals Descriptor UUID. 

```

#### ESP_GATT_UUID_VALUE_TRIGGER_DESCR


```


ESP_GATT_UUID_VALUE_TRIGGER_DESCR
Value Trigger Setting Descriptor UUID. 

```

#### ESP_GATT_UUID_ENV_SENSING_CONFIG_DESCR


```


ESP_GATT_UUID_ENV_SENSING_CONFIG_DESCR
Environmental Sensing Configuration Descriptor UUID. 

```

#### ESP_GATT_UUID_ENV_SENSING_MEASUREMENT_DESCR


```


ESP_GATT_UUID_ENV_SENSING_MEASUREMENT_DESCR
Environmental Sensing Measurement Descriptor UUID. 

```

#### ESP_GATT_UUID_ENV_SENSING_TRIGGER_DESCR


```


ESP_GATT_UUID_ENV_SENSING_TRIGGER_DESCR
Environmental Sensing Trigger Setting Descriptor UUID. 

```

#### ESP_GATT_UUID_TIME_TRIGGER_DESCR


```


ESP_GATT_UUID_TIME_TRIGGER_DESCR
Time Trigger Setting Descriptor UUID. 

```

#### ESP_GATT_UUID_GAP_DEVICE_NAME


```


ESP_GATT_UUID_GAP_DEVICE_NAME
GAP Device Name UUID. 

```

#### ESP_GATT_UUID_GAP_ICON


```


ESP_GATT_UUID_GAP_ICON
GAP Icon UUID. 

```

#### ESP_GATT_UUID_GAP_PREF_CONN_PARAM


```


ESP_GATT_UUID_GAP_PREF_CONN_PARAM
GAP Preferred Connection Parameters UUID. 

```

#### ESP_GATT_UUID_GAP_CENTRAL_ADDR_RESOL


```


ESP_GATT_UUID_GAP_CENTRAL_ADDR_RESOL
GAP Central Address Resolution UUID. 

```

#### ESP_GATT_UUID_GATT_SRV_CHGD


```


ESP_GATT_UUID_GATT_SRV_CHGD
GATT Service Changed UUID. 

```

#### ESP_GATT_UUID_ALERT_LEVEL


```


ESP_GATT_UUID_ALERT_LEVEL
Alert Level UUID. 

```

#### ESP_GATT_UUID_TX_POWER_LEVEL


```


ESP_GATT_UUID_TX_POWER_LEVEL
TX Power Level UUID. 

```

#### ESP_GATT_UUID_CURRENT_TIME


```


ESP_GATT_UUID_CURRENT_TIME
Current Time UUID. 

```

#### ESP_GATT_UUID_LOCAL_TIME_INFO


```


ESP_GATT_UUID_LOCAL_TIME_INFO
Local Time Info UUID. 

```

#### ESP_GATT_UUID_REF_TIME_INFO


```


ESP_GATT_UUID_REF_TIME_INFO
Reference Time Information UUID. 

```

#### ESP_GATT_UUID_NW_STATUS


```


ESP_GATT_UUID_NW_STATUS
Network Availability Status UUID. 

```

#### ESP_GATT_UUID_NW_TRIGGER


```


ESP_GATT_UUID_NW_TRIGGER
Network Availability Trigger UUID. 

```

#### ESP_GATT_UUID_ALERT_STATUS


```


ESP_GATT_UUID_ALERT_STATUS
Alert Status UUID. 

```

#### ESP_GATT_UUID_RINGER_CP


```


ESP_GATT_UUID_RINGER_CP
Ringer Control Point UUID. 

```

#### ESP_GATT_UUID_RINGER_SETTING


```


ESP_GATT_UUID_RINGER_SETTING
Ringer Setting UUID. 

```

#### ESP_GATT_UUID_GM_MEASUREMENT


```


ESP_GATT_UUID_GM_MEASUREMENT
Glucose Measurement Characteristic UUID. 

```

#### ESP_GATT_UUID_GM_CONTEXT


```


ESP_GATT_UUID_GM_CONTEXT
Glucose Measurement Context Characteristic UUID. 

```

#### ESP_GATT_UUID_GM_CONTROL_POINT


```


ESP_GATT_UUID_GM_CONTROL_POINT
Glucose Control Point Characteristic UUID. 

```

#### ESP_GATT_UUID_GM_FEATURE


```


ESP_GATT_UUID_GM_FEATURE
Glucose Feature Characteristic UUID. 

```

#### ESP_GATT_UUID_SYSTEM_ID


```


ESP_GATT_UUID_SYSTEM_ID
System ID Characteristic UUID. 

```

#### ESP_GATT_UUID_MODEL_NUMBER_STR


```


ESP_GATT_UUID_MODEL_NUMBER_STR
Model Number String Characteristic UUID. 

```

#### ESP_GATT_UUID_SERIAL_NUMBER_STR


```


ESP_GATT_UUID_SERIAL_NUMBER_STR
Serial Number String Characteristic UUID. 

```

#### ESP_GATT_UUID_FW_VERSION_STR


```


ESP_GATT_UUID_FW_VERSION_STR
Firmware Revision String Characteristic UUID. 

```

#### ESP_GATT_UUID_HW_VERSION_STR


```


ESP_GATT_UUID_HW_VERSION_STR
Hardware Revision String Characteristic UUID. 

```

#### ESP_GATT_UUID_SW_VERSION_STR


```


ESP_GATT_UUID_SW_VERSION_STR
Software Revision String Characteristic UUID. 

```

#### ESP_GATT_UUID_MANU_NAME


```


ESP_GATT_UUID_MANU_NAME
Manufacturer Name String Characteristic UUID. 

```

#### ESP_GATT_UUID_IEEE_DATA


```


ESP_GATT_UUID_IEEE_DATA
IEEE 11073-20601 Regulatory Certification Data List Characteristic UUID. 

```

#### ESP_GATT_UUID_PNP_ID


```


ESP_GATT_UUID_PNP_ID
PnP ID Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_INFORMATION


```


ESP_GATT_UUID_HID_INFORMATION
HID Information Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_REPORT_MAP


```


ESP_GATT_UUID_HID_REPORT_MAP
HID Report Map Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_CONTROL_POINT


```


ESP_GATT_UUID_HID_CONTROL_POINT
HID Control Point Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_REPORT


```


ESP_GATT_UUID_HID_REPORT
HID Report Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_PROTO_MODE


```


ESP_GATT_UUID_HID_PROTO_MODE
HID Protocol Mode Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_BT_KB_INPUT


```


ESP_GATT_UUID_HID_BT_KB_INPUT
HID Bluetooth Keyboard Input Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_BT_KB_OUTPUT


```


ESP_GATT_UUID_HID_BT_KB_OUTPUT
HID Bluetooth Keyboard Output Characteristic UUID. 

```

#### ESP_GATT_UUID_HID_BT_MOUSE_INPUT


```


ESP_GATT_UUID_HID_BT_MOUSE_INPUT
HID Bluetooth Mouse Input Characteristic UUID. 

```

#### ESP_GATT_HEART_RATE_MEAS


```


ESP_GATT_HEART_RATE_MEAS
Heart Rate Measurement Characteristic UUID. 

```

#### ESP_GATT_BODY_SENSOR_LOCATION


```


ESP_GATT_BODY_SENSOR_LOCATION
Body Sensor Location Characteristic UUID. 

```

#### ESP_GATT_HEART_RATE_CNTL_POINT


```


ESP_GATT_HEART_RATE_CNTL_POINT
Heart Rate Control Point Characteristic UUID. 

```

#### ESP_GATT_UUID_BATTERY_LEVEL


```


ESP_GATT_UUID_BATTERY_LEVEL
Battery Level Characteristic UUID. 

```

#### ESP_GATT_UUID_SC_CONTROL_POINT


```


ESP_GATT_UUID_SC_CONTROL_POINT
Sensor Control Point Characteristic UUID. 

```

#### ESP_GATT_UUID_SENSOR_LOCATION


```


ESP_GATT_UUID_SENSOR_LOCATION
Sensor Location Characteristic UUID. 

```

#### ESP_GATT_UUID_RSC_MEASUREMENT


```


ESP_GATT_UUID_RSC_MEASUREMENT
RSC Measurement Characteristic UUID. 

```

#### ESP_GATT_UUID_RSC_FEATURE


```


ESP_GATT_UUID_RSC_FEATURE
RSC Feature Characteristic UUID. 

```

#### ESP_GATT_UUID_CSC_MEASUREMENT


```


ESP_GATT_UUID_CSC_MEASUREMENT
CSC Measurement Characteristic UUID. 

```

#### ESP_GATT_UUID_CSC_FEATURE


```


ESP_GATT_UUID_CSC_FEATURE
CSC Feature Characteristic UUID. 

```

#### ESP_GATT_UUID_SCAN_INT_WINDOW


```


ESP_GATT_UUID_SCAN_INT_WINDOW
Scan Interval Window Characteristic UUID. 

```

#### ESP_GATT_UUID_SCAN_REFRESH


```


ESP_GATT_UUID_SCAN_REFRESH
Scan Refresh UUID. 

```

#### ESP_GATT_PERM_READ


```


ESP_GATT_PERM_READ
Permission to read the attribute. Corresponds to BTA_GATT_PERM_READ. 

```

#### ESP_GATT_PERM_READ_ENCRYPTED


```


ESP_GATT_PERM_READ_ENCRYPTED
Permission to read the attribute with encryption. Corresponds to BTA_GATT_PERM_READ_ENCRYPTED. 

```

#### ESP_GATT_PERM_READ_ENC_MITM


```


ESP_GATT_PERM_READ_ENC_MITM
Permission to read the attribute with encrypted MITM (Man In The Middle) protection. Corresponds to BTA_GATT_PERM_READ_ENC_MITM. 

```

#### ESP_GATT_PERM_WRITE


```


ESP_GATT_PERM_WRITE
Permission to write to the attribute. Corresponds to BTA_GATT_PERM_WRITE. 

```

#### ESP_GATT_PERM_WRITE_ENCRYPTED


```


ESP_GATT_PERM_WRITE_ENCRYPTED
Permission to write to the attribute with encryption. Corresponds to BTA_GATT_PERM_WRITE_ENCRYPTED. 

```

#### ESP_GATT_PERM_WRITE_ENC_MITM


```


ESP_GATT_PERM_WRITE_ENC_MITM
Permission to write to the attribute with encrypted MITM protection. Corresponds to BTA_GATT_PERM_WRITE_ENC_MITM. 

```

#### ESP_GATT_PERM_WRITE_SIGNED


```


ESP_GATT_PERM_WRITE_SIGNED
Permission for signed writes to the attribute. Corresponds to BTA_GATT_PERM_WRITE_SIGNED. 

```

#### ESP_GATT_PERM_WRITE_SIGNED_MITM


```


ESP_GATT_PERM_WRITE_SIGNED_MITM
Permission for signed writes to the attribute with MITM protection. Corresponds to BTA_GATT_PERM_WRITE_SIGNED_MITM. 

```

#### ESP_GATT_PERM_READ_AUTHORIZATION


```


ESP_GATT_PERM_READ_AUTHORIZATION
Permission to read the attribute with authorization. 

```

#### ESP_GATT_PERM_WRITE_AUTHORIZATION


```


ESP_GATT_PERM_WRITE_AUTHORIZATION
Permission to write to the attribute with authorization. 

```

#### ESP_GATT_PERM_ENCRYPT_KEY_SIZE


```


ESP_GATT_PERM_ENCRYPT_KEY_SIZE(keysize)
Macro to specify minimum encryption key size. 

Parameters

keysize -- The minimum size of the encryption key, in bytes. 




```

#### ESP_GATT_CHAR_PROP_BIT_BROADCAST


```


ESP_GATT_CHAR_PROP_BIT_BROADCAST
Ability to broadcast.Corresponds to BTA_GATT_CHAR_PROP_BIT_BROADCAST. 

```

#### ESP_GATT_CHAR_PROP_BIT_READ


```


ESP_GATT_CHAR_PROP_BIT_READ
Ability to read.Corresponds to BTA_GATT_CHAR_PROP_BIT_READ. 

```

#### ESP_GATT_CHAR_PROP_BIT_WRITE_NR


```


ESP_GATT_CHAR_PROP_BIT_WRITE_NR
Ability to write without response.Corresponds to BTA_GATT_CHAR_PROP_BIT_WRITE_NR. 

```

#### ESP_GATT_CHAR_PROP_BIT_WRITE


```


ESP_GATT_CHAR_PROP_BIT_WRITE
Ability to write.Corresponds to BTA_GATT_CHAR_PROP_BIT_WRITE. 

```

#### ESP_GATT_CHAR_PROP_BIT_NOTIFY


```


ESP_GATT_CHAR_PROP_BIT_NOTIFY
Ability to notify.Corresponds to BTA_GATT_CHAR_PROP_BIT_NOTIFY. 

```

#### ESP_GATT_CHAR_PROP_BIT_INDICATE


```


ESP_GATT_CHAR_PROP_BIT_INDICATE
Ability to indicate.Corresponds to BTA_GATT_CHAR_PROP_BIT_INDICATE. 

```

#### ESP_GATT_CHAR_PROP_BIT_AUTH


```


ESP_GATT_CHAR_PROP_BIT_AUTH
Ability to authenticate.Corresponds to BTA_GATT_CHAR_PROP_BIT_AUTH. 

```

#### ESP_GATT_CHAR_PROP_BIT_EXT_PROP


```


ESP_GATT_CHAR_PROP_BIT_EXT_PROP
Has extended properties.Corresponds to BTA_GATT_CHAR_PROP_BIT_EXT_PROP. 

```

#### ESP_GATT_MAX_ATTR_LEN


```


ESP_GATT_MAX_ATTR_LEN
Defines the maximum length of a GATT attribute. 
This definition specifies the maximum number of bytes that a GATT attribute can hold. As same as GATT_MAX_ATTR_LEN. 

```

#### ESP_GATT_RSP_BY_APP


```


ESP_GATT_RSP_BY_APP
Defines attribute control for GATT operations. 
This module provides definitions for controlling attribute auto responses in GATT operations.
Response to Write/Read operations should be handled by the application. 

```

#### ESP_GATT_AUTO_RSP


```


ESP_GATT_AUTO_RSP
Response to Write/Read operations should be automatically handled by the GATT stack. 

```

#### ESP_GATT_IF_NONE


```


ESP_GATT_IF_NONE
Macro indicating no specific GATT interface. 
No specific application GATT interface. 

```



### Type Definitions


#### esp_gatt_perm_t


```


typedef uint16_t esp_gatt_perm_t
Type to represent GATT attribute permissions. 

```

#### esp_gatt_char_prop_t


```


typedef uint8_t esp_gatt_char_prop_t
Type for characteristic properties bitmask. 

```

#### esp_gatt_if_t


```


typedef uint8_t esp_gatt_if_t
GATT interface type for client applications. 

```



### Enumerations


#### esp_gatt_prep_write_type


```


enum esp_gatt_prep_write_type
Defines the attribute write operation types from the client. 
These values are used to specify the type of write operation in a prepare write sequence. relate to BTA_GATT_PREP_WRITE_xxx in bta/bta_gatt_api.h. 
Values:
ESP_GATT_PREP_WRITE_CANCEL

enumerator ESP_GATT_PREP_WRITE_CANCEL
Prepare write cancel. Corresponds to BTA_GATT_PREP_WRITE_CANCEL. 

ESP_GATT_PREP_WRITE_EXEC

enumerator ESP_GATT_PREP_WRITE_EXEC
Prepare write execute. Corresponds to BTA_GATT_PREP_WRITE_EXEC. 


```

#### esp_gatt_status_t


```


enum esp_gatt_status_t
GATT operation status codes. 
These status codes are used to indicate the result of various GATT operations. relate to BTA_GATT_xxx in bta/bta_gatt_api.h . 
Values:
ESP_GATT_OK

enumerator ESP_GATT_OK
0x0, Operation successful. Corresponds to BTA_GATT_OK. 

ESP_GATT_INVALID_HANDLE

enumerator ESP_GATT_INVALID_HANDLE
0x01, Invalid handle. Corresponds to BTA_GATT_INVALID_HANDLE. 

ESP_GATT_READ_NOT_PERMIT

enumerator ESP_GATT_READ_NOT_PERMIT
0x02, Read operation not permitted. Corresponds to BTA_GATT_READ_NOT_PERMIT. 

ESP_GATT_WRITE_NOT_PERMIT

enumerator ESP_GATT_WRITE_NOT_PERMIT
0x03, Write operation not permitted. Corresponds to BTA_GATT_WRITE_NOT_PERMIT. 

ESP_GATT_INVALID_PDU

enumerator ESP_GATT_INVALID_PDU
0x04, Invalid PDU. Corresponds to BTA_GATT_INVALID_PDU. 

ESP_GATT_INSUF_AUTHENTICATION

enumerator ESP_GATT_INSUF_AUTHENTICATION
0x05, Insufficient authentication. Corresponds to BTA_GATT_INSUF_AUTHENTICATION. 

ESP_GATT_REQ_NOT_SUPPORTED

enumerator ESP_GATT_REQ_NOT_SUPPORTED
0x06, Request not supported. Corresponds to BTA_GATT_REQ_NOT_SUPPORTED. 

ESP_GATT_INVALID_OFFSET

enumerator ESP_GATT_INVALID_OFFSET
0x07, Invalid offset. Corresponds to BTA_GATT_INVALID_OFFSET. 

ESP_GATT_INSUF_AUTHORIZATION

enumerator ESP_GATT_INSUF_AUTHORIZATION
0x08, Insufficient authorization. Corresponds to BTA_GATT_INSUF_AUTHORIZATION. 

ESP_GATT_PREPARE_Q_FULL

enumerator ESP_GATT_PREPARE_Q_FULL
0x09, Prepare queue full. Corresponds to BTA_GATT_PREPARE_Q_FULL. 

ESP_GATT_NOT_FOUND

enumerator ESP_GATT_NOT_FOUND
0x0a, Not found. Corresponds to BTA_GATT_NOT_FOUND. 

ESP_GATT_NOT_LONG

enumerator ESP_GATT_NOT_LONG
0x0b, Not long. Corresponds to BTA_GATT_NOT_LONG. 

ESP_GATT_INSUF_KEY_SIZE

enumerator ESP_GATT_INSUF_KEY_SIZE
0x0c, Insufficient key size. Corresponds to BTA_GATT_INSUF_KEY_SIZE. 

ESP_GATT_INVALID_ATTR_LEN

enumerator ESP_GATT_INVALID_ATTR_LEN
0x0d, Invalid attribute length. Corresponds to BTA_GATT_INVALID_ATTR_LEN. 

ESP_GATT_ERR_UNLIKELY

enumerator ESP_GATT_ERR_UNLIKELY
0x0e, Unlikely error. Corresponds to BTA_GATT_ERR_UNLIKELY. 

ESP_GATT_INSUF_ENCRYPTION

enumerator ESP_GATT_INSUF_ENCRYPTION
0x0f, Insufficient encryption. Corresponds to BTA_GATT_INSUF_ENCRYPTION. 

ESP_GATT_UNSUPPORT_GRP_TYPE

enumerator ESP_GATT_UNSUPPORT_GRP_TYPE
0x10, Unsupported group type. Corresponds to BTA_GATT_UNSUPPORT_GRP_TYPE. 

ESP_GATT_INSUF_RESOURCE

enumerator ESP_GATT_INSUF_RESOURCE
0x11, Insufficient resource. Corresponds to BTA_GATT_INSUF_RESOURCE. 

ESP_GATT_NO_RESOURCES

enumerator ESP_GATT_NO_RESOURCES
0x80, No resources. Corresponds to BTA_GATT_NO_RESOURCES. 

ESP_GATT_INTERNAL_ERROR

enumerator ESP_GATT_INTERNAL_ERROR
0x81, Internal error. Corresponds to BTA_GATT_INTERNAL_ERROR. 

ESP_GATT_WRONG_STATE

enumerator ESP_GATT_WRONG_STATE
0x82, Wrong state. Corresponds to BTA_GATT_WRONG_STATE. 

ESP_GATT_DB_FULL

enumerator ESP_GATT_DB_FULL
0x83, Database full. Corresponds to BTA_GATT_DB_FULL. 

ESP_GATT_BUSY

enumerator ESP_GATT_BUSY
0x84, Busy. Corresponds to BTA_GATT_BUSY. 

ESP_GATT_ERROR

enumerator ESP_GATT_ERROR
0x85, Generic error. Corresponds to BTA_GATT_ERROR. 

ESP_GATT_CMD_STARTED

enumerator ESP_GATT_CMD_STARTED
0x86, Command started. Corresponds to BTA_GATT_CMD_STARTED. 

ESP_GATT_ILLEGAL_PARAMETER

enumerator ESP_GATT_ILLEGAL_PARAMETER
0x87, Illegal parameter. Corresponds to BTA_GATT_ILLEGAL_PARAMETER. 

ESP_GATT_PENDING

enumerator ESP_GATT_PENDING
0x88, Operation pending. Corresponds to BTA_GATT_PENDING. 

ESP_GATT_AUTH_FAIL

enumerator ESP_GATT_AUTH_FAIL
0x89, Authentication failed. Corresponds to BTA_GATT_AUTH_FAIL. 

ESP_GATT_MORE

enumerator ESP_GATT_MORE
0x8a, More data available. Corresponds to BTA_GATT_MORE. 

ESP_GATT_INVALID_CFG

enumerator ESP_GATT_INVALID_CFG
0x8b, Invalid configuration. Corresponds to BTA_GATT_INVALID_CFG. 

ESP_GATT_SERVICE_STARTED

enumerator ESP_GATT_SERVICE_STARTED
0x8c, Service started. Corresponds to BTA_GATT_SERVICE_STARTED. 

ESP_GATT_ENCRYPTED_MITM

enumerator ESP_GATT_ENCRYPTED_MITM
0x0, Encrypted, with MITM protection. Corresponds to BTA_GATT_ENCRYPTED_MITM. 

ESP_GATT_ENCRYPTED_NO_MITM

enumerator ESP_GATT_ENCRYPTED_NO_MITM
0x8d, Encrypted, without MITM protection. Corresponds to BTA_GATT_ENCRYPTED_NO_MITM. 

ESP_GATT_NOT_ENCRYPTED

enumerator ESP_GATT_NOT_ENCRYPTED
0x8e, Not encrypted. Corresponds to BTA_GATT_NOT_ENCRYPTED. 

ESP_GATT_CONGESTED

enumerator ESP_GATT_CONGESTED
0x8f, Congested. Corresponds to BTA_GATT_CONGESTED. 

ESP_GATT_DUP_REG

enumerator ESP_GATT_DUP_REG
0x90, Duplicate registration. Corresponds to BTA_GATT_DUP_REG. 

ESP_GATT_ALREADY_OPEN

enumerator ESP_GATT_ALREADY_OPEN
0x91, Already open. Corresponds to BTA_GATT_ALREADY_OPEN. 

ESP_GATT_CANCEL

enumerator ESP_GATT_CANCEL
0x92, Operation cancelled. Corresponds to BTA_GATT_CANCEL. 

ESP_GATT_STACK_RSP

enumerator ESP_GATT_STACK_RSP
0xe0, Stack response. Corresponds to BTA_GATT_STACK_RSP. 

ESP_GATT_APP_RSP

enumerator ESP_GATT_APP_RSP
0xe1, Application response. Corresponds to BTA_GATT_APP_RSP. 

ESP_GATT_UNKNOWN_ERROR

enumerator ESP_GATT_UNKNOWN_ERROR
0xef, Unknown error. Corresponds to BTA_GATT_UNKNOWN_ERROR. 

ESP_GATT_CCC_CFG_ERR

enumerator ESP_GATT_CCC_CFG_ERR
0xfd, Client Characteristic Configuration Descriptor improperly configured. Corresponds to BTA_GATT_CCC_CFG_ERR. 

ESP_GATT_PRC_IN_PROGRESS

enumerator ESP_GATT_PRC_IN_PROGRESS
0xfe, Procedure already in progress. Corresponds to BTA_GATT_PRC_IN_PROGRESS. 

ESP_GATT_OUT_OF_RANGE

enumerator ESP_GATT_OUT_OF_RANGE
0xff, Attribute value out of range. Corresponds to BTA_GATT_OUT_OF_RANGE. 


```

#### esp_gatt_conn_reason_t


```


enum esp_gatt_conn_reason_t
Enumerates reasons for GATT connection. 
Values:
ESP_GATT_CONN_UNKNOWN

enumerator ESP_GATT_CONN_UNKNOWN
Unknown connection reason. Corresponds to BTA_GATT_CONN_UNKNOWN in bta/bta_gatt_api.h 

ESP_GATT_CONN_L2C_FAILURE

enumerator ESP_GATT_CONN_L2C_FAILURE
General L2CAP failure. Corresponds to BTA_GATT_CONN_L2C_FAILURE in bta/bta_gatt_api.h 

ESP_GATT_CONN_TIMEOUT

enumerator ESP_GATT_CONN_TIMEOUT
Connection timeout. Corresponds to BTA_GATT_CONN_TIMEOUT in bta/bta_gatt_api.h 

ESP_GATT_CONN_TERMINATE_PEER_USER

enumerator ESP_GATT_CONN_TERMINATE_PEER_USER
Connection terminated by peer user. Corresponds to BTA_GATT_CONN_TERMINATE_PEER_USER in bta/bta_gatt_api.h 

ESP_GATT_CONN_TERMINATE_LOCAL_HOST

enumerator ESP_GATT_CONN_TERMINATE_LOCAL_HOST
Connection terminated by local host. Corresponds to BTA_GATT_CONN_TERMINATE_LOCAL_HOST in bta/bta_gatt_api.h 

ESP_GATT_CONN_FAIL_ESTABLISH

enumerator ESP_GATT_CONN_FAIL_ESTABLISH
Failure to establish connection. Corresponds to BTA_GATT_CONN_FAIL_ESTABLISH in bta/bta_gatt_api.h 

ESP_GATT_CONN_LMP_TIMEOUT

enumerator ESP_GATT_CONN_LMP_TIMEOUT
Connection failed due to LMP response timeout. Corresponds to BTA_GATT_CONN_LMP_TIMEOUT in bta/bta_gatt_api.h 

ESP_GATT_CONN_CONN_CANCEL

enumerator ESP_GATT_CONN_CONN_CANCEL
L2CAP connection cancelled. Corresponds to BTA_GATT_CONN_CONN_CANCEL in bta/bta_gatt_api.h 

ESP_GATT_CONN_NONE

enumerator ESP_GATT_CONN_NONE
No connection to cancel. Corresponds to BTA_GATT_CONN_NONE in bta/bta_gatt_api.h 


```

#### esp_gatt_auth_req_t


```


enum esp_gatt_auth_req_t
Defines the GATT authentication request types. 
This enumeration lists the types of authentication requests that can be made. It corresponds to the BTA_GATT_AUTH_REQ_xxx values defined in bta/bta_gatt_api.h. The types include options for no authentication, unauthenticated encryption, authenticated encryption, and both signed versions with and without MITM (Man-In-The-Middle) protection. 
Values:
ESP_GATT_AUTH_REQ_NONE

enumerator ESP_GATT_AUTH_REQ_NONE
No authentication required. Corresponds to BTA_GATT_AUTH_REQ_NONE. 

ESP_GATT_AUTH_REQ_NO_MITM

enumerator ESP_GATT_AUTH_REQ_NO_MITM
Unauthenticated encryption. Corresponds to BTA_GATT_AUTH_REQ_NO_MITM. 

ESP_GATT_AUTH_REQ_MITM

enumerator ESP_GATT_AUTH_REQ_MITM
Authenticated encryption (MITM protection). Corresponds to BTA_GATT_AUTH_REQ_MITM. 

ESP_GATT_AUTH_REQ_SIGNED_NO_MITM

enumerator ESP_GATT_AUTH_REQ_SIGNED_NO_MITM
Signed data, no MITM protection. Corresponds to BTA_GATT_AUTH_REQ_SIGNED_NO_MITM. 

ESP_GATT_AUTH_REQ_SIGNED_MITM

enumerator ESP_GATT_AUTH_REQ_SIGNED_MITM
Signed data with MITM protection. Corresponds to BTA_GATT_AUTH_REQ_SIGNED_MITM. 


```

#### esp_service_source_t


```


enum esp_service_source_t
Enumerates the possible sources of a GATT service discovery. 
This enumeration identifies the source of a GATT service discovery process, indicating whether the service information was obtained from a remote device, from NVS (Non-Volatile Storage) flash, or the source is unknown. 
Values:
ESP_GATT_SERVICE_FROM_REMOTE_DEVICE

enumerator ESP_GATT_SERVICE_FROM_REMOTE_DEVICE
Service information from a remote device. Relates to BTA_GATTC_SERVICE_INFO_FROM_REMOTE_DEVICE. 

ESP_GATT_SERVICE_FROM_NVS_FLASH

enumerator ESP_GATT_SERVICE_FROM_NVS_FLASH
Service information from NVS flash. Relates to BTA_GATTC_SERVICE_INFO_FROM_NVS_FLASH. 

ESP_GATT_SERVICE_FROM_UNKNOWN

enumerator ESP_GATT_SERVICE_FROM_UNKNOWN
Service source is unknown. Relates to BTA_GATTC_SERVICE_INFO_FROM_UNKNOWN. 


```

#### esp_gatt_write_type_t


```


enum esp_gatt_write_type_t
Defines the types of GATT write operations. 
Values:
ESP_GATT_WRITE_TYPE_NO_RSP

enumerator ESP_GATT_WRITE_TYPE_NO_RSP
Write operation where no response is needed. 

ESP_GATT_WRITE_TYPE_RSP

enumerator ESP_GATT_WRITE_TYPE_RSP
Write operation that requires a remote response. 


```

#### esp_gatt_db_attr_type_t


```


enum esp_gatt_db_attr_type_t
Enumerates types of GATT database attributes. 
Values:
ESP_GATT_DB_PRIMARY_SERVICE

enumerator ESP_GATT_DB_PRIMARY_SERVICE
Primary service attribute. 

ESP_GATT_DB_SECONDARY_SERVICE

enumerator ESP_GATT_DB_SECONDARY_SERVICE
Secondary service attribute. 

ESP_GATT_DB_CHARACTERISTIC

enumerator ESP_GATT_DB_CHARACTERISTIC
Characteristic attribute. 

ESP_GATT_DB_DESCRIPTOR

enumerator ESP_GATT_DB_DESCRIPTOR
Descriptor attribute. 

ESP_GATT_DB_INCLUDED_SERVICE

enumerator ESP_GATT_DB_INCLUDED_SERVICE
Included service attribute. 

ESP_GATT_DB_ALL

enumerator ESP_GATT_DB_ALL
All attribute types. 


```