# Controller && VHCI



## Application Example


Check bluetooth/hci folder in ESP-IDF examples, which contains the following application:


* This is a Bluetooth® Low Energy (Bluetooth LE) advertising demo with virtual HCI interface. Send Reset/ADV_PARAM/ADV_DATA/ADV_ENABLE HCI command for Bluetooth Low Energy advertising - bluetooth/hci/controller_vhci_ble_adv.


Please note that ESP32-S3 shares the same bt/include/esp32c3/include/esp_bt.h and bt/controller/esp32c3/bt.c files with ESP32-C3.




## API Reference



### Header File


* components/bt/include/esp32c3/include/esp_bt.h
* This header file can be included with:



> ```
> #include "esp_bt.h"
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


#### esp_ble_tx_power_set


```


esp_err_t esp_ble_tx_power_set(esp_ble_power_type_t power_type, esp_power_level_t power_level)
Set BLE TX power Connection Tx power should only be set after connection created. 

Parameters

power_type -- : The type of which tx power, could set Advertising/Connection/Default and etc 
power_level -- Power level(index) corresponding to absolute value(dbm) 


Returns
ESP_OK - success, other - failed 



```

#### esp_ble_tx_power_get


```


esp_power_level_t esp_ble_tx_power_get(esp_ble_power_type_t power_type)
Get BLE TX power Connection Tx power should only be get after connection created. 

Parameters
power_type -- : The type of which tx power, could set Advertising/Connection/Default and etc 

Returns
>= 0 - Power level, < 0 - Invalid 



```

#### esp_bt_controller_init


```


esp_err_t esp_bt_controller_init(esp_bt_controller_config_t *cfg)
Initialize BT controller to allocate task and other resource. This function should be called only once, before any other BT functions are called. 

Parameters
cfg -- Initial configuration of BT controller. Different from previous version, there's a mode and some connection configuration in "cfg" to configure controller work mode and allocate the resource which is needed. 

Returns
ESP_OK - success, other - failed 



```

#### esp_bt_controller_deinit


```


esp_err_t esp_bt_controller_deinit(void)
De-initialize BT controller to free resource and delete task. You should stop advertising and scanning, as well as disconnect all existing connections before de-initializing BT controller. 
This function should be called only once, after any other BT functions are called. This function is not whole completed, esp_bt_controller_init cannot called after this function. 

Returns
ESP_OK - success, other - failed 



```

#### esp_bt_controller_enable


```


esp_err_t esp_bt_controller_enable(esp_bt_mode_t mode)
Enable BT controller. Due to a known issue, you cannot call esp_bt_controller_enable() a second time to change the controller mode dynamically. To change controller mode, call esp_bt_controller_disable() and then call esp_bt_controller_enable() with the new mode. 

Parameters
mode -- : the mode(BLE/BT/BTDM) to enable. For compatible of API, retain this argument. This mode must be equal as the mode in "cfg" of esp_bt_controller_init(). 

Returns
ESP_OK - success, other - failed 



```

#### esp_bt_controller_disable


```


esp_err_t esp_bt_controller_disable(void)
Disable BT controller. 

Returns
ESP_OK - success, other - failed 



```

#### esp_bt_controller_get_status


```


esp_bt_controller_status_t esp_bt_controller_get_status(void)
Get BT controller is initialised/de-initialised/enabled/disabled. 

Returns
status value 



```

#### esp_bt_get_tx_buf_num


```


uint16_t esp_bt_get_tx_buf_num(void)

```

#### esp_vhci_host_check_send_available


```


bool esp_vhci_host_check_send_available(void)
esp_vhci_host_check_send_available used for check actively if the host can send packet to controller or not. 

Returns
true for ready to send, false means cannot send packet 



```

#### esp_vhci_host_send_packet


```


void esp_vhci_host_send_packet(uint8_t *data, uint16_t len)
esp_vhci_host_send_packet host send packet to controller 
Should not call this function from within a critical section or when the scheduler is suspended.

Parameters

data -- the packet point 
len -- the packet length 




```

#### esp_vhci_host_register_callback


```


esp_err_t esp_vhci_host_register_callback(const esp_vhci_host_callback_t *callback)
esp_vhci_host_register_callback register the vhci reference callback struct defined by vhci_host_callback structure. 

Parameters
callback -- esp_vhci_host_callback type variable 

Returns
ESP_OK - success, ESP_FAIL - failed 



```

#### esp_bt_controller_mem_release


```


esp_err_t esp_bt_controller_mem_release(esp_bt_mode_t mode)
esp_bt_controller_mem_release release the controller memory as per the mode 
This function releases the BSS, data and other sections of the controller to heap. The total size is about 70k bytes.
esp_bt_controller_mem_release(mode) should be called only before esp_bt_controller_init() or after esp_bt_controller_deinit().
Note that once BT controller memory is released, the process cannot be reversed. It means you cannot use the bluetooth mode which you have released by this function.
If your firmware will later upgrade the Bluetooth controller mode (BLE -> BT Classic or disabled -> enabled) then do not call this function.
If the app calls esp_bt_controller_enable(ESP_BT_MODE_BLE) to use BLE only then it is safe to call esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT) at initialization time to free unused BT Classic memory.
If the mode is ESP_BT_MODE_BTDM, then it may be useful to call API esp_bt_mem_release(ESP_BT_MODE_BTDM) instead, which internally calls esp_bt_controller_mem_release(ESP_BT_MODE_BTDM) and additionally releases the BSS and data consumed by the BT/BLE host stack to heap. For more details about usage please refer to the documentation of esp_bt_mem_release() function

Parameters
mode -- : the mode want to release memory 

Returns
ESP_OK - success, other - failed 



```

#### esp_bt_mem_release


```


esp_err_t esp_bt_mem_release(esp_bt_mode_t mode)
esp_bt_mem_release release controller memory and BSS and data section of the BT/BLE host stack as per the mode 
This function first releases controller memory by internally calling esp_bt_controller_mem_release(). Additionally, if the mode is set to ESP_BT_MODE_BTDM, it also releases the BSS and data consumed by the BT/BLE host stack to heap
Note that once BT memory is released, the process cannot be reversed. It means you cannot use the bluetooth mode which you have released by this function.
If your firmware will later upgrade the Bluetooth controller mode (BLE -> BT Classic or disabled -> enabled) then do not call this function.
If you never intend to use bluetooth in a current boot-up cycle, you can call esp_bt_mem_release(ESP_BT_MODE_BTDM) before esp_bt_controller_init or after esp_bt_controller_deinit.
For example, if a user only uses bluetooth for setting the WiFi configuration, and does not use bluetooth in the rest of the product operation". In such cases, after receiving the WiFi configuration, you can disable/deinit bluetooth and release its memory. Below is the sequence of APIs to be called for such scenarios:  esp_bluedroid_disable();
 esp_bluedroid_deinit();
 esp_bt_controller_disable();
 esp_bt_controller_deinit();
 esp_bt_mem_release(ESP_BT_MODE_BTDM);




Parameters
mode -- : the mode whose memory is to be released 

Returns
ESP_OK - success, other - failed 



```

#### esp_bt_sleep_enable


```


esp_err_t esp_bt_sleep_enable(void)
enable bluetooth to enter modem sleep 
Note that this function shall not be invoked before esp_bt_controller_enable()
There are currently two options for bluetooth modem sleep, one is ORIG mode, and another is EVED Mode. EVED Mode is intended for BLE only.
For ORIG mode: Bluetooth modem sleep is enabled in controller start up by default if CONFIG_BTDM_CONTROLLER_MODEM_SLEEP is set and "ORIG mode" is selected. In ORIG modem sleep mode, bluetooth controller will switch off some components and pause to work every now and then, if there is no event to process; and wakeup according to the scheduled interval and resume the work. It can also wakeup earlier upon external request using function "esp_bt_controller_wakeup_request".

Returns

ESP_OK : success
other : failed 





```

#### esp_bt_sleep_disable


```


esp_err_t esp_bt_sleep_disable(void)
disable bluetooth modem sleep 
Note that this function shall not be invoked before esp_bt_controller_enable()
If esp_bt_sleep_disable() is called, bluetooth controller will not be allowed to enter modem sleep;
If ORIG modem sleep mode is in use, if this function is called, bluetooth controller may not immediately wake up if it is dormant then. In this case, esp_bt_controller_wakeup_request() can be used to shorten the time for wakeup.

Returns

ESP_OK : success
other : failed 





```

#### esp_bt_controller_is_sleeping


```


bool esp_bt_controller_is_sleeping(void)
to check whether bluetooth controller is sleeping at the instant, if modem sleep is enabled 
Note that this function shall not be invoked before esp_bt_controller_enable() This function is supposed to be used ORIG mode of modem sleep

Returns
true if in modem sleep state, false otherwise 



```

#### esp_bt_controller_wakeup_request


```


void esp_bt_controller_wakeup_request(void)
request controller to wakeup from sleeping state during sleep mode 
Note that this function shall not be invoked before esp_bt_controller_enable() Note that this function is supposed to be used ORIG mode of modem sleep Note that after this request, bluetooth controller may again enter sleep as long as the modem sleep is enabled
Profiling shows that it takes several milliseconds to wakeup from modem sleep after this request. Generally it takes longer if 32kHz XTAL is used than the main XTAL, due to the lower frequency of the former as the bluetooth low power clock source. 

```

#### esp_bt_h4tl_eif_io_event_notify


```


int esp_bt_h4tl_eif_io_event_notify(int event)
notify bluetooth controller task to process the event upon Tx or Rx done 
Note that this function shall not be invoked before esp_bt_controller_enable() This function can be called in both ISR and non-ISR context 

```

#### esp_wifi_bt_power_domain_on


```


void esp_wifi_bt_power_domain_on(void)
bt Wi-Fi power domain power on 

```

#### esp_wifi_bt_power_domain_off


```


void esp_wifi_bt_power_domain_off(void)
bt Wi-Fi power domain power off 

```

#### esp_bt_get_lpclk_src


```


esp_bt_sleep_clock_t esp_bt_get_lpclk_src(void)
Get the Bluetooth module sleep clock source. 
Note that this function shall not be invoked before esp_bt_controller_init()

Returns
clock source used in Bluetooth low power mode 



```



### Structures


#### esp_bt_hci_tl_t


```


struct esp_bt_hci_tl_t
Controller HCI transport layer function structure This structure shall be registered when HCI transport layer is UART. 

Public Members
_magic

uint32_t _magic
Magic number 

_version

uint32_t _version
Version number of the defined structure 

_reserved

uint32_t _reserved
Reserved for future use 

_open

int (*_open)(void)
HCI transport layer open function 

_close

void (*_close)(void)
HCI transport layer close function 

_finish_transfers

void (*_finish_transfers)(void)
HCI transport layer finish transfers function 

_recv

void (*_recv)(uint8_t *buf, uint32_t len, esp_bt_hci_tl_callback_t callback, void *arg)
HCI transport layer receive function 

_send

void (*_send)(uint8_t *buf, uint32_t len, esp_bt_hci_tl_callback_t callback, void *arg)
HCI transport layer send function 

_flow_off

bool (*_flow_off)(void)
HCI transport layer flow off function 

_flow_on

void (*_flow_on)(void)
HCI transport layer flow on function 



```

#### esp_bt_controller_config_t


```


struct esp_bt_controller_config_t
Controller config options, depend on config mask. Config mask indicate which functions enabled, this means some options or parameters of some functions enabled by config mask. 

Public Members
magic

uint32_t magic
Magic number 

version

uint32_t version
version number of the defined structure 

controller_task_stack_size

uint16_t controller_task_stack_size
Bluetooth controller task stack size 

controller_task_prio

uint8_t controller_task_prio
Bluetooth controller task priority 

controller_task_run_cpu

uint8_t controller_task_run_cpu
CPU num that Bluetooth controller task runs on 

bluetooth_mode

uint8_t bluetooth_mode
Controller mode: BR/EDR, BLE or Dual Mode 

ble_max_act

uint8_t ble_max_act
BLE maximum number of air activities 

sleep_mode

uint8_t sleep_mode
controller sleep mode 

sleep_clock

uint8_t sleep_clock
controller sleep clock 

ble_st_acl_tx_buf_nb

uint8_t ble_st_acl_tx_buf_nb
controller static ACL TX BUFFER number 

ble_hw_cca_check

uint8_t ble_hw_cca_check
controller hardware triggered CCA check 

ble_adv_dup_filt_max

uint16_t ble_adv_dup_filt_max
maximum number of duplicate scan filter 

coex_param_en

bool coex_param_en
deprecated 

ce_len_type

uint8_t ce_len_type
connection event length computation method 

coex_use_hooks

bool coex_use_hooks
deprecated 

hci_tl_type

uint8_t hci_tl_type
HCI transport layer, UART, VHCI, etc 

hci_tl_funcs

esp_bt_hci_tl_t *hci_tl_funcs
hci transport functions used, must be set when hci_tl_type is UART 

txant_dft

uint8_t txant_dft
default Tx antenna 

rxant_dft

uint8_t rxant_dft
default Rx antenna 

txpwr_dft

uint8_t txpwr_dft
default Tx power 

cfg_mask

uint32_t cfg_mask
Configuration mask to set specific options 

scan_duplicate_mode

uint8_t scan_duplicate_mode
scan duplicate mode 

scan_duplicate_type

uint8_t scan_duplicate_type
scan duplicate type 

normal_adv_size

uint16_t normal_adv_size
Normal adv size for scan duplicate 

mesh_adv_size

uint16_t mesh_adv_size
Mesh adv size for scan duplicate 

coex_phy_coded_tx_rx_time_limit

uint8_t coex_phy_coded_tx_rx_time_limit
limit on max tx/rx time in case of connection using CODED-PHY with Wi-Fi coexistence 

hw_target_code

uint32_t hw_target_code
hardware target 

slave_ce_len_min

uint8_t slave_ce_len_min
slave minimum ce length 

hw_recorrect_en

uint8_t hw_recorrect_en
Hardware re-correction enabled 

cca_thresh

uint8_t cca_thresh
cca threshold 

scan_backoff_upperlimitmax

uint16_t scan_backoff_upperlimitmax
scan backoff upperlimitmax value 

dup_list_refresh_period

uint16_t dup_list_refresh_period
duplicate scan list refresh time 

ble_50_feat_supp

bool ble_50_feat_supp
BLE 5.0 feature support 

ble_cca_mode

uint8_t ble_cca_mode
BLE CCA mode 

ble_data_lenth_zero_aux

uint8_t ble_data_lenth_zero_aux
Config ext adv aux option 

ble_chan_ass_en

uint8_t ble_chan_ass_en
BLE channel assessment enable 

ble_ping_en

uint8_t ble_ping_en
BLE ping procedure enable 



```

#### esp_vhci_host_callback


```


struct esp_vhci_host_callback
esp_vhci_host_callback used for vhci call host function to notify what host need to do 

Public Members
notify_host_send_available

void (*notify_host_send_available)(void)
callback used to notify that the host can send packet to controller 

notify_host_recv

int (*notify_host_recv)(uint8_t *data, uint16_t len)
callback used to notify that the controller has a packet to send to the host 



```



### Macros


#### ESP_BT_CTRL_CONFIG_MAGIC_VAL


```


ESP_BT_CTRL_CONFIG_MAGIC_VAL

```

#### ESP_BT_CTRL_CONFIG_VERSION


```


ESP_BT_CTRL_CONFIG_VERSION

```

#### ESP_BT_HCI_TL_MAGIC_VALUE


```


ESP_BT_HCI_TL_MAGIC_VALUE

```

#### ESP_BT_HCI_TL_VERSION


```


ESP_BT_HCI_TL_VERSION

```

#### ESP_BT_HCI_TL_STATUS_OK


```


ESP_BT_HCI_TL_STATUS_OK
HCI_TL Tx/Rx operation status OK 

```

#### BT_CONTROLLER_INIT_CONFIG_DEFAULT


```


BT_CONTROLLER_INIT_CONFIG_DEFAULT()

```



### Type Definitions


#### esp_bt_hci_tl_callback_t


```


typedef void (*esp_bt_hci_tl_callback_t)(void *arg, uint8_t status)
callback function for HCI Transport Layer send/receive operations 

```

#### esp_vhci_host_callback_t


```


typedef struct esp_vhci_host_callback esp_vhci_host_callback_t
esp_vhci_host_callback used for vhci call host function to notify what host need to do 

```



### Enumerations


#### esp_bt_mode_t


```


enum esp_bt_mode_t
Bluetooth mode for controller enable/disable. 
Values:
ESP_BT_MODE_IDLE

enumerator ESP_BT_MODE_IDLE
Bluetooth is not running 

ESP_BT_MODE_BLE

enumerator ESP_BT_MODE_BLE
Run BLE mode 

ESP_BT_MODE_CLASSIC_BT

enumerator ESP_BT_MODE_CLASSIC_BT
Run Classic BT mode 

ESP_BT_MODE_BTDM

enumerator ESP_BT_MODE_BTDM
Run dual mode 


```

#### esp_bt_ctrl_hci_tl_t


```


enum esp_bt_ctrl_hci_tl_t
Type of controller HCI transport layer. 
Values:
ESP_BT_CTRL_HCI_TL_UART

enumerator ESP_BT_CTRL_HCI_TL_UART
HCI UART h4 transport layer 

ESP_BT_CTRL_HCI_TL_VHCI

enumerator ESP_BT_CTRL_HCI_TL_VHCI
VHCI interface 


```

#### esp_ble_ce_len_t


```


enum esp_ble_ce_len_t
type of BLE connection event length computation 
Values:
ESP_BLE_CE_LEN_TYPE_ORIG

enumerator ESP_BLE_CE_LEN_TYPE_ORIG
original 

ESP_BLE_CE_LEN_TYPE_CE

enumerator ESP_BLE_CE_LEN_TYPE_CE
use CE_LEN parameter from HCI commands 

ESP_BLE_CE_LEN_TYPE_SD

enumerator ESP_BLE_CE_LEN_TYPE_SD
Espressif vendor defined 


```

#### esp_bt_sleep_mode_t


```


enum esp_bt_sleep_mode_t
Bluetooth sleep mode. 
Values:
ESP_BT_SLEEP_MODE_NONE

enumerator ESP_BT_SLEEP_MODE_NONE
Bluetooth sleep mode disabled 

ESP_BT_SLEEP_MODE_1

enumerator ESP_BT_SLEEP_MODE_1
Bluetooth sleep mode 1 


```

#### esp_bt_sleep_clock_t


```


enum esp_bt_sleep_clock_t
Bluetooth sleep clock. 
Values:
ESP_BT_SLEEP_CLOCK_NONE

enumerator ESP_BT_SLEEP_CLOCK_NONE
Sleep clock not configured 

ESP_BT_SLEEP_CLOCK_MAIN_XTAL

enumerator ESP_BT_SLEEP_CLOCK_MAIN_XTAL
SoC main crystal 

ESP_BT_SLEEP_CLOCK_EXT_32K_XTAL

enumerator ESP_BT_SLEEP_CLOCK_EXT_32K_XTAL
External 32.768kHz crystal 

ESP_BT_SLEEP_CLOCK_RTC_SLOW

enumerator ESP_BT_SLEEP_CLOCK_RTC_SLOW
Internal 136kHz RC oscillator 

ESP_BT_SLEEP_CLOCK_FPGA_32K

enumerator ESP_BT_SLEEP_CLOCK_FPGA_32K
Hardwired 32KHz clock temporarily used for FPGA 


```

#### [anonymous]


```


enum [anonymous]
antenna index used for bluetooth 
Values:
ESP_BT_ANT_IDX_0

enumerator ESP_BT_ANT_IDX_0
anntena NO 0 

ESP_BT_ANT_IDX_1

enumerator ESP_BT_ANT_IDX_1
anntena NO 1 


```

#### [anonymous]


```


enum [anonymous]
Maximum Tx/Rx time limit on Coded-PHY connection. 
Values:
ESP_BT_COEX_PHY_CODED_TX_RX_TIME_LIMIT_FORCE_DISABLE

enumerator ESP_BT_COEX_PHY_CODED_TX_RX_TIME_LIMIT_FORCE_DISABLE
Disable the limit 

ESP_BT_COEX_PHY_CODED_TX_RX_TIME_LIMIT_FORCE_ENABLE

enumerator ESP_BT_COEX_PHY_CODED_TX_RX_TIME_LIMIT_FORCE_ENABLE
Always Enable the limit 


```

#### esp_bt_controller_status_t


```


enum esp_bt_controller_status_t
Bluetooth controller enable/disable/initialised/de-initialised status. 
Values:
ESP_BT_CONTROLLER_STATUS_IDLE

enumerator ESP_BT_CONTROLLER_STATUS_IDLE

ESP_BT_CONTROLLER_STATUS_INITED

enumerator ESP_BT_CONTROLLER_STATUS_INITED

ESP_BT_CONTROLLER_STATUS_ENABLED

enumerator ESP_BT_CONTROLLER_STATUS_ENABLED

ESP_BT_CONTROLLER_STATUS_NUM

enumerator ESP_BT_CONTROLLER_STATUS_NUM


```

#### esp_ble_power_type_t


```


enum esp_ble_power_type_t
BLE tx power type ESP_BLE_PWR_TYPE_CONN_HDL0-8: for each connection, and only be set after connection completed. when disconnect, the correspond TX power is not effected. ESP_BLE_PWR_TYPE_ADV : for advertising/scan response. ESP_BLE_PWR_TYPE_SCAN : for scan. ESP_BLE_PWR_TYPE_DEFAULT : if each connection's TX power is not set, it will use this default value. if neither in scan mode nor in adv mode, it will use this default value. If none of power type is set, system will use ESP_PWR_LVL_P3 as default for ADV/SCAN/CONN0-9. 
Values:
ESP_BLE_PWR_TYPE_CONN_HDL0

enumerator ESP_BLE_PWR_TYPE_CONN_HDL0
For connection handle 0 

ESP_BLE_PWR_TYPE_CONN_HDL1

enumerator ESP_BLE_PWR_TYPE_CONN_HDL1
For connection handle 1 

ESP_BLE_PWR_TYPE_CONN_HDL2

enumerator ESP_BLE_PWR_TYPE_CONN_HDL2
For connection handle 2 

ESP_BLE_PWR_TYPE_CONN_HDL3

enumerator ESP_BLE_PWR_TYPE_CONN_HDL3
For connection handle 3 

ESP_BLE_PWR_TYPE_CONN_HDL4

enumerator ESP_BLE_PWR_TYPE_CONN_HDL4
For connection handle 4 

ESP_BLE_PWR_TYPE_CONN_HDL5

enumerator ESP_BLE_PWR_TYPE_CONN_HDL5
For connection handle 5 

ESP_BLE_PWR_TYPE_CONN_HDL6

enumerator ESP_BLE_PWR_TYPE_CONN_HDL6
For connection handle 6 

ESP_BLE_PWR_TYPE_CONN_HDL7

enumerator ESP_BLE_PWR_TYPE_CONN_HDL7
For connection handle 7 

ESP_BLE_PWR_TYPE_CONN_HDL8

enumerator ESP_BLE_PWR_TYPE_CONN_HDL8
For connection handle 8 

ESP_BLE_PWR_TYPE_ADV

enumerator ESP_BLE_PWR_TYPE_ADV
For advertising 

ESP_BLE_PWR_TYPE_SCAN

enumerator ESP_BLE_PWR_TYPE_SCAN
For scan 

ESP_BLE_PWR_TYPE_DEFAULT

enumerator ESP_BLE_PWR_TYPE_DEFAULT
For default, if not set other, it will use default value 

ESP_BLE_PWR_TYPE_NUM

enumerator ESP_BLE_PWR_TYPE_NUM
TYPE numbers 


```

#### esp_power_level_t


```


enum esp_power_level_t
Bluetooth TX power level(index), it's just a index corresponding to power(dbm). 
Values:
ESP_PWR_LVL_N24

enumerator ESP_PWR_LVL_N24
Corresponding to -24dbm 

ESP_PWR_LVL_N21

enumerator ESP_PWR_LVL_N21
Corresponding to -21dbm 

ESP_PWR_LVL_N18

enumerator ESP_PWR_LVL_N18
Corresponding to -18dbm 

ESP_PWR_LVL_N15

enumerator ESP_PWR_LVL_N15
Corresponding to -15dbm 

ESP_PWR_LVL_N12

enumerator ESP_PWR_LVL_N12
Corresponding to -12dbm 

ESP_PWR_LVL_N9

enumerator ESP_PWR_LVL_N9
Corresponding to -9dbm 

ESP_PWR_LVL_N6

enumerator ESP_PWR_LVL_N6
Corresponding to -6dbm 

ESP_PWR_LVL_N3

enumerator ESP_PWR_LVL_N3
Corresponding to -3dbm 

ESP_PWR_LVL_N0

enumerator ESP_PWR_LVL_N0
Corresponding to 0dbm 

ESP_PWR_LVL_P3

enumerator ESP_PWR_LVL_P3
Corresponding to +3dbm 

ESP_PWR_LVL_P6

enumerator ESP_PWR_LVL_P6
Corresponding to +6dbm 

ESP_PWR_LVL_P9

enumerator ESP_PWR_LVL_P9
Corresponding to +9dbm 

ESP_PWR_LVL_P12

enumerator ESP_PWR_LVL_P12
Corresponding to +12dbm 

ESP_PWR_LVL_P15

enumerator ESP_PWR_LVL_P15
Corresponding to +15dbm 

ESP_PWR_LVL_P18

enumerator ESP_PWR_LVL_P18
Corresponding to +18dbm 

ESP_PWR_LVL_P21

enumerator ESP_PWR_LVL_P21
Corresponding to +21dbm 

ESP_PWR_LVL_INVALID

enumerator ESP_PWR_LVL_INVALID
Indicates an invalid value 


```