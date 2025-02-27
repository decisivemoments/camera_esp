# ESP-BLE-MESH



Note


The current ESP-BLE-MESH v1.1 related code is a preview version, so the Mesh Protocol v1.1 related Structures, MACROs, and APIs involved in the code may be changed.



With various features of ESP-BLE-MESH, users can create a managed flooding mesh network for several scenarios, such as lighting, sensor and etc.


For an ESP32 to join and work on a ESP-BLE-MESH network, it must be provisioned firstly. By provisioning, the ESP32, as an unprovisioned device, will join the ESP-BLE-MESH network and become a ESP-BLE-MESH node, communicating with other nodes within or beyond the radio range.


Apart from ESP-BLE-MESH nodes, inside ESP-BLE-MESH network, there is also ESP32 that works as ESP-BLE-MESH provisioner, which could provision unprovisioned devices into ESP-BLE-MESH nodes and configure the nodes with various features.


For information how to start using ESP32 and ESP-BLE-MESH, please see the Section Getting Started with ESP-BLE-MESH. If you are interested in information on ESP-BLE-MESH architecture, including some details of software implementation, please see Section ESP-BLE-MESH Architecture.



## Application Examples and Demos


Please refer to Sections ESP-BLE-MESH Examples and ESP-BLE-MESH Demo Videos.




## API Reference


ESP-BLE-MESH APIs are divided into the following parts:


* ESP-BLE-MESH Definitions
* ESP-BLE-MESH Core API Reference
* ESP-BLE-MESH Models API Reference
* ESP-BLE-MESH (v1.1) Core API Reference




## ESP-BLE-MESH Definitions


This section contains only one header file, which lists the following items of ESP-BLE-MESH.


* ID of all the models and related message opcodes
* Structs of model, element and Composition Data
* Structs of used by ESP-BLE-MESH Node/Provisioner for provisioning
* Structs used to transmit/receive messages
* Event types and related event parameters



### Header File


* components/bt/esp_ble_mesh/api/esp_ble_mesh_defs.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_defs.h"
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


#### esp_ble_mesh_prov_cb_param_t


```


union esp_ble_mesh_prov_cb_param_t

#include <esp_ble_mesh_defs.h>
BLE Mesh Node/Provisioner callback parameters union. 

Public Members
prov_register_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_prov_register_comp_param prov_register_comp
Event parameter of ESP_BLE_MESH_PROV_REGISTER_COMP_EVT 

node_set_unprov_dev_name_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_set_unprov_dev_name_comp_param node_set_unprov_dev_name_comp
Event parameter of ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT 

node_prov_enable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_prov_enable_comp_param node_prov_enable_comp
Event parameter of ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT 

node_prov_disable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_prov_disable_comp_param node_prov_disable_comp
Event parameter of ESP_BLE_MESH_NODE_PROV_DISABLE_COMP_EVT 

node_prov_link_open

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_link_open_evt_param node_prov_link_open
Event parameter of ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT 

node_prov_link_close

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_link_close_evt_param node_prov_link_close
Event parameter of ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT 

node_prov_output_num

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_output_num_evt_param node_prov_output_num
Event parameter of ESP_BLE_MESH_NODE_PROV_OUTPUT_NUMBER_EVT 

node_prov_output_str

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_output_str_evt_param node_prov_output_str
Event parameter of ESP_BLE_MESH_NODE_PROV_OUTPUT_STRING_EVT 

node_prov_input

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_input_evt_param node_prov_input
Event parameter of ESP_BLE_MESH_NODE_PROV_INPUT_EVT 

node_prov_complete

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provision_complete_evt_param node_prov_complete
Event parameter of ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT 

node_prov_reset

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provision_reset_param node_prov_reset
Event parameter of ESP_BLE_MESH_NODE_PROV_RESET_EVT 

node_prov_set_oob_pub_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_set_oob_pub_key_comp_param node_prov_set_oob_pub_key_comp
Event parameter of ESP_BLE_MESH_NODE_PROV_SET_OOB_PUB_KEY_COMP_EVT 

node_prov_input_num_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_input_number_comp_param node_prov_input_num_comp
Event parameter of ESP_BLE_MESH_NODE_PROV_INPUT_NUM_COMP_EVT 

node_prov_input_str_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_input_string_comp_param node_prov_input_str_comp
Event parameter of ESP_BLE_MESH_NODE_PROV_INPUT_STR_COMP_EVT 

node_proxy_identity_enable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_identity_enable_comp_param node_proxy_identity_enable_comp
Event parameter of ESP_BLE_MESH_NODE_PROXY_IDENTITY_ENABLE_COMP_EVT 

node_proxy_gatt_enable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_gatt_enable_comp_param node_proxy_gatt_enable_comp
Event parameter of ESP_BLE_MESH_NODE_PROXY_GATT_ENABLE_COMP_EVT 

node_proxy_gatt_disable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_gatt_disable_comp_param node_proxy_gatt_disable_comp
Event parameter of ESP_BLE_MESH_NODE_PROXY_GATT_DISABLE_COMP_EVT 

node_private_proxy_identity_enable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_private_identity_enable_comp_param node_private_proxy_identity_enable_comp
Event parameter of ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_ENABLE_COMP_EVT 

node_private_proxy_identity_disable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_private_identity_disable_comp_param node_private_proxy_identity_disable_comp
Event parameter of ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_DISABLE_COMP_EVT 

node_add_net_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_node_add_local_net_key_comp_param node_add_net_key_comp
Event parameter of ESP_BLE_MESH_NODE_ADD_LOCAL_NET_KEY_COMP_EVT 

node_add_app_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_node_add_local_app_key_comp_param node_add_app_key_comp
Event parameter of ESP_BLE_MESH_NODE_ADD_LOCAL_APP_KEY_COMP_EVT 

node_bind_app_key_to_model_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_node_bind_local_mod_app_comp_param node_bind_app_key_to_model_comp
Event parameter of ESP_BLE_MESH_NODE_BIND_APP_KEY_TO_MODEL_COMP_EVT 

provisioner_recv_unprov_adv_pkt

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_recv_unprov_adv_pkt_param provisioner_recv_unprov_adv_pkt
Event parameter of ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT 

provisioner_prov_enable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_enable_comp_param provisioner_prov_enable_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT 

provisioner_prov_disable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_disable_comp_param provisioner_prov_disable_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT 

provisioner_prov_link_open

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_link_open_evt_param provisioner_prov_link_open
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT 

provisioner_prov_read_oob_pub_key

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_read_oob_pub_key_evt_param provisioner_prov_read_oob_pub_key
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_EVT 

provisioner_prov_input

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_input_evt_param provisioner_prov_input
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_INPUT_EVT 

provisioner_prov_output

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_output_evt_param provisioner_prov_output
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_OUTPUT_EVT 

provisioner_prov_link_close

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_link_close_evt_param provisioner_prov_link_close
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT 

provisioner_prov_complete

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_comp_param provisioner_prov_complete
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT 

provisioner_cert_based_prov_start

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_cert_based_prov_start_evt_param provisioner_cert_based_prov_start
Event parameter of ESP_BLE_MESH_PROVISIONER_CERT_BASED_PROV_START_EVT 

recv_provisioner_records_list

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_recv_prov_records_list_evt_param recv_provisioner_records_list
Event parameter of ESP_BLE_MESH_PROVISIONER_RECV_PROV_RECORDS_LIST_EVT 

provisioner_prov_record_recv_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_record_recv_comp_evt_param provisioner_prov_record_recv_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_RECORD_RECV_COMP_EVT 

provisioner_send_records_get

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_send_prov_records_get_evt_param provisioner_send_records_get
Event parameter of ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORDS_GET_EVT 

provisioner_send_record_req

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_send_prov_record_req_evt_param provisioner_send_record_req
Event parameter of ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORD_REQUEST_EVT 

provisioner_send_prov_invite

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_send_prov_invite_evt_param provisioner_send_prov_invite
Event parameter of ESP_BLE_MESH_PROVISIONER_SEND_PROV_INVITE_EVT 

provisioner_send_link_close

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_send_link_close_evt_param provisioner_send_link_close
Event parameter of ESP_BLE_MESH_PROVISIONER_SEND_LINK_CLOSE_EVT 

provisioner_add_unprov_dev_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_add_unprov_dev_comp_param provisioner_add_unprov_dev_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT 

provisioner_prov_dev_with_addr_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_dev_with_addr_comp_param provisioner_prov_dev_with_addr_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_DEV_WITH_ADDR_COMP_EVT 

provisioner_delete_dev_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_delete_dev_comp_param provisioner_delete_dev_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_DELETE_DEV_COMP_EVT 

provisioner_set_dev_uuid_match_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_set_dev_uuid_match_comp_param provisioner_set_dev_uuid_match_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT 

provisioner_set_prov_data_info_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_set_prov_data_info_comp_param provisioner_set_prov_data_info_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_SET_PROV_DATA_INFO_COMP_EVT 

provisioner_set_static_oob_val_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_set_static_oob_val_comp_param provisioner_set_static_oob_val_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_SET_STATIC_OOB_VALUE_COMP_EVT 

provisioner_set_primary_elem_addr_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_set_primary_elem_addr_comp_param provisioner_set_primary_elem_addr_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_SET_PRIMARY_ELEM_ADDR_COMP_EVT 

provisioner_prov_read_oob_pub_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_read_oob_pub_key_comp_param provisioner_prov_read_oob_pub_key_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_COMP_EVT 

provisioner_prov_input_num_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_input_num_comp_param provisioner_prov_input_num_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_INPUT_NUMBER_COMP_EVT 

provisioner_prov_input_str_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_input_str_comp_param provisioner_prov_input_str_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_PROV_INPUT_STRING_COMP_EVT 

provisioner_set_node_name_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_set_node_name_comp_param provisioner_set_node_name_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT 

provisioner_add_app_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_add_local_app_key_comp_param provisioner_add_app_key_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT 

provisioner_update_app_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_update_local_app_key_comp_param provisioner_update_app_key_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_APP_KEY_COMP_EVT 

provisioner_bind_app_key_to_model_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_bind_local_mod_app_comp_param provisioner_bind_app_key_to_model_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT 

provisioner_add_net_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_add_local_net_key_comp_param provisioner_add_net_key_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_NET_KEY_COMP_EVT 

provisioner_update_net_key_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_update_local_net_key_comp_param provisioner_update_net_key_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_NET_KEY_COMP_EVT 

provisioner_store_node_comp_data_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_store_node_comp_data_comp_param provisioner_store_node_comp_data_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_STORE_NODE_COMP_DATA_COMP_EVT 

provisioner_delete_node_with_uuid_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_delete_node_with_uuid_comp_param provisioner_delete_node_with_uuid_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_UUID_COMP_EVT 

provisioner_delete_node_with_addr_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_delete_node_with_addr_comp_param provisioner_delete_node_with_addr_comp
Event parameter of ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_ADDR_COMP_EVT 

err_code

int err_code
Indicate the result of enabling/disabling to receive heartbeat messages by the Provisioner
Indicate the result of setting the heartbeat filter type by the Provisioner
Indicate the result of setting the heartbeat filter address by the Provisioner
Indicate the result of directly erasing settings by the Provisioner
Indicate the result of opening settings with index by the Provisioner
Indicate the result of opening settings with user id by the Provisioner
Indicate the result of closing settings with index by the Provisioner
Indicate the result of closing settings with user id by the Provisioner
Indicate the result of deleting settings with index by the Provisioner
Indicate the result of deleting settings with user id by the Provisioner
Indicate the result of Proxy Client send Solicitation PDU 

enable

bool enable
Indicate enabling or disabling receiving heartbeat messages 

provisioner_enable_heartbeat_recv_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_enable_heartbeat_recv_comp
ESP_BLE_MESH_PROVISIONER_ENABLE_HEARTBEAT_RECV_COMP_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_ENABLE_HEARTBEAT_RECV_COMP_EVT 

type

uint8_t type
Type of the filter used for receiving heartbeat messages 

provisioner_set_heartbeat_filter_type_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_set_heartbeat_filter_type_comp
ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_TYPE_COMP_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_TYPE_COMP_EVT 

op

uint8_t op
Operation (add, remove, clean) 

hb_src

uint16_t hb_src
Heartbeat source address 

hb_dst

uint16_t hb_dst
Heartbeat destination address 

provisioner_set_heartbeat_filter_info_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_set_heartbeat_filter_info_comp
ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_INFO_COMP_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_INFO_COMP_EVT 

init_ttl

uint8_t init_ttl
Heartbeat InitTTL 

rx_ttl

uint8_t rx_ttl
Heartbeat RxTTL 

hops

uint8_t hops
Heartbeat hops (InitTTL - RxTTL + 1) 

feature

uint16_t feature
Bit field of currently active features of the node 

rssi

int8_t rssi
RSSI of the heartbeat message 

provisioner_recv_heartbeat

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_recv_heartbeat
ESP_BLE_MESH_PROVISIONER_RECV_HEARTBEAT_MESSAGE_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_RECV_HEARTBEAT_MESSAGE_EVT 

provisioner_direct_erase_settings_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_direct_erase_settings_comp
ESP_BLE_MESH_PROVISIONER_DIRECT_ERASE_SETTINGS_COMP_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_DIRECT_ERASE_SETTINGS_COMP_EVT 

index

uint8_t index
Index of Provisioner settings 

provisioner_open_settings_with_index_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_open_settings_with_index_comp
ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_INDEX_COMP_EVT. 
Event parameter of ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_INDEX_COMP_EVT 

uid

char uid[ESP_BLE_MESH_SETTINGS_UID_SIZE + 1]
Provisioner settings user id 

provisioner_open_settings_with_uid_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_open_settings_with_uid_comp
ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_UID_COMP_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_UID_COMP_EVT 

provisioner_close_settings_with_index_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_close_settings_with_index_comp
ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_INDEX_COMP_EVT. 
Event parameter of ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_INDEX_COMP_EVT 

provisioner_close_settings_with_uid_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_close_settings_with_uid_comp
ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_UID_COMP_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_UID_COMP_EVT 

provisioner_delete_settings_with_index_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_delete_settings_with_index_comp
ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_INDEX_COMP_EVT. 
Event parameter of ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_INDEX_COMP_EVT 

provisioner_delete_settings_with_uid_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] provisioner_delete_settings_with_uid_comp
ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_UID_COMP_EVT. 
Event parameters of ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_UID_COMP_EVT 

set_fast_prov_info_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_set_fast_prov_info_comp_param set_fast_prov_info_comp
Event parameter of ESP_BLE_MESH_SET_FAST_PROV_INFO_COMP_EVT 

set_fast_prov_action_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_set_fast_prov_action_comp_param set_fast_prov_action_comp
Event parameter of ESP_BLE_MESH_SET_FAST_PROV_ACTION_COMP_EVT 

heartbeat_msg_recv

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_heartbeat_msg_recv_param heartbeat_msg_recv
Event parameter of ESP_BLE_MESH_HEARTBEAT_MESSAGE_RECV_EVT 

lpn_enable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_lpn_enable_comp_param lpn_enable_comp
Event parameter of ESP_BLE_MESH_LPN_ENABLE_COMP_EVT 

lpn_disable_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_lpn_disable_comp_param lpn_disable_comp
Event parameter of ESP_BLE_MESH_LPN_DISABLE_COMP_EVT 

lpn_poll_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_lpn_poll_comp_param lpn_poll_comp
Event parameter of ESP_BLE_MESH_LPN_POLL_COMP_EVT 

lpn_friendship_establish

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_lpn_friendship_establish_param lpn_friendship_establish
Event parameter of ESP_BLE_MESH_LPN_FRIENDSHIP_ESTABLISH_EVT 

lpn_friendship_terminate

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_lpn_friendship_terminate_param lpn_friendship_terminate
Event parameter of ESP_BLE_MESH_LPN_FRIENDSHIP_TERMINATE_EVT 

frnd_friendship_establish

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_friend_friendship_establish_param frnd_friendship_establish
Event parameter of ESP_BLE_MESH_FRIEND_FRIENDSHIP_ESTABLISH_EVT 

frnd_friendship_terminate

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_friend_friendship_terminate_param frnd_friendship_terminate
Event parameter of ESP_BLE_MESH_FRIEND_FRIENDSHIP_TERMINATE_EVT 

proxy_client_recv_adv_pkt

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_recv_adv_pkt_param proxy_client_recv_adv_pkt
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_RECV_ADV_PKT_EVT 

proxy_client_connected

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_connected_param proxy_client_connected
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_CONNECTED_EVT 

proxy_client_disconnected

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_disconnected_param proxy_client_disconnected
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_DISCONNECTED_EVT 

proxy_client_recv_filter_status

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_recv_filter_status_param proxy_client_recv_filter_status
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_RECV_FILTER_STATUS_EVT 

proxy_client_connect_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_connect_comp_param proxy_client_connect_comp
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_CONNECT_COMP_EVT 

proxy_client_disconnect_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_disconnect_comp_param proxy_client_disconnect_comp
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_DISCONNECT_COMP_EVT 

proxy_client_set_filter_type_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_set_filter_type_comp_param proxy_client_set_filter_type_comp
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_SET_FILTER_TYPE_COMP_EVT 

proxy_client_add_filter_addr_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_add_filter_addr_comp_param proxy_client_add_filter_addr_comp
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_ADD_FILTER_ADDR_COMP_EVT 

proxy_client_remove_filter_addr_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_remove_filter_addr_comp_param proxy_client_remove_filter_addr_comp
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_REMOVE_FILTER_ADDR_COMP_EVT 

proxy_client_directed_proxy_set_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_client_directed_proxy_set_param proxy_client_directed_proxy_set_comp
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_DIRECTED_PROXY_SET_COMP_EVT 

proxy_server_connected

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_server_connected_param proxy_server_connected
Event parameter of ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT 

proxy_server_disconnected

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_proxy_server_disconnected_param proxy_server_disconnected
Event parameter of ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT 

net_idx

uint16_t net_idx
Corresponding NetKey Index 

ssrc

uint16_t ssrc
Solicitation SRC 

dst

uint16_t dst
Solicitation DST 

proxy_client_send_solic_pdu_comp

struct esp_ble_mesh_prov_cb_param_t::[anonymous] proxy_client_send_solic_pdu_comp
ESP_BLE_MESH_PROXY_CLIENT_SEND_SOLIC_PDU_COMP_EVT. 
Event parameter of ESP_BLE_MESH_PROXY_CLIENT_SEND_SOLIC_PDU_COMP_EVT 

model_sub_group_addr_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_model_sub_group_addr_comp_param model_sub_group_addr_comp
Event parameters of ESP_BLE_MESH_MODEL_SUBSCRIBE_GROUP_ADDR_COMP_EVT 

model_unsub_group_addr_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_model_unsub_group_addr_comp_param model_unsub_group_addr_comp
Event parameters of ESP_BLE_MESH_MODEL_UNSUBSCRIBE_GROUP_ADDR_COMP_EVT 

deinit_mesh_comp

struct esp_ble_mesh_prov_cb_param_t::ble_mesh_deinit_mesh_comp_param deinit_mesh_comp
Event parameter of ESP_BLE_MESH_DEINIT_MESH_COMP_EVT 


ble_mesh_deinit_mesh_comp_param

struct ble_mesh_deinit_mesh_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_DEINIT_MESH_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of BLE Mesh deinitialization 



ble_mesh_friend_friendship_establish_param

struct ble_mesh_friend_friendship_establish_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_FRIEND_FRIENDSHIP_ESTABLISH_EVT. 

Public Members
lpn_addr

uint16_t lpn_addr
Low Power Node unicast address 



ble_mesh_friend_friendship_terminate_param

struct ble_mesh_friend_friendship_terminate_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_FRIEND_FRIENDSHIP_TERMINATE_EVT. 

Public Types
[anonymous]

enum [anonymous]
This enum value is the reason of friendship termination on the friend node side 
Values:
ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_ESTABLISH_FAIL

enumerator ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_ESTABLISH_FAIL
Friend Offer has been sent, but Friend Offer is not received within 1 second, friendship fails to be established 

ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_POLL_TIMEOUT

enumerator ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_POLL_TIMEOUT
Friendship is established, PollTimeout timer expires and no Friend Poll/Sub Add/Sub Remove is received 

ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_RECV_FRND_REQ

enumerator ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_RECV_FRND_REQ
Receive Friend Request from existing Low Power Node 

ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_RECV_FRND_CLEAR

enumerator ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_RECV_FRND_CLEAR
Receive Friend Clear from other friend node 

ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_DISABLE

enumerator ESP_BLE_MESH_FRND_FRIENDSHIP_TERMINATE_DISABLE
Friend feature disabled or corresponding NetKey is deleted 




Public Members
lpn_addr

uint16_t lpn_addr
Low Power Node unicast address 

reason

enum esp_ble_mesh_prov_cb_param_t::ble_mesh_friend_friendship_terminate_param::[anonymous] reason
This enum value is the reason of friendship termination on the friend node side Friendship terminated reason 



ble_mesh_heartbeat_msg_recv_param

struct ble_mesh_heartbeat_msg_recv_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_HEARTBEAT_MESSAGE_RECV_EVT. 

Public Members
hops

uint8_t hops
Heartbeat hops (InitTTL - RxTTL + 1) 

feature

uint16_t feature
Bit field of currently active features of the node 



ble_mesh_input_evt_param

struct ble_mesh_input_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_INPUT_EVT. 

Public Members
action

esp_ble_mesh_input_action_t action
Action of Input OOB Authentication 

size

uint8_t size
Size of Input OOB Authentication 



ble_mesh_input_number_comp_param

struct ble_mesh_input_number_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_INPUT_NUM_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of inputting number 



ble_mesh_input_string_comp_param

struct ble_mesh_input_string_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_INPUT_STR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of inputting string 



ble_mesh_link_close_evt_param

struct ble_mesh_link_close_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT. 

Public Members
bearer

esp_ble_mesh_prov_bearer_t bearer
Type of the bearer used when device link is closed 

reason

uint8_t reason
Reason of the closed provisioning link 



ble_mesh_link_open_evt_param

struct ble_mesh_link_open_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT. 

Public Members
bearer

esp_ble_mesh_prov_bearer_t bearer
Type of the bearer used when device link is open 



ble_mesh_lpn_disable_comp_param

struct ble_mesh_lpn_disable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_LPN_DISABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of disabling LPN functionality 



ble_mesh_lpn_enable_comp_param

struct ble_mesh_lpn_enable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_LPN_ENABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of enabling LPN functionality 



ble_mesh_lpn_friendship_establish_param

struct ble_mesh_lpn_friendship_establish_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_LPN_FRIENDSHIP_ESTABLISH_EVT. 

Public Members
friend_addr

uint16_t friend_addr
Friend Node unicast address 



ble_mesh_lpn_friendship_terminate_param

struct ble_mesh_lpn_friendship_terminate_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_LPN_FRIENDSHIP_TERMINATE_EVT. 

Public Members
friend_addr

uint16_t friend_addr
Friend Node unicast address 



ble_mesh_lpn_poll_comp_param

struct ble_mesh_lpn_poll_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_LPN_POLL_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of sending Friend Poll 



ble_mesh_model_sub_group_addr_comp_param

struct ble_mesh_model_sub_group_addr_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_MODEL_SUBSCRIBE_GROUP_ADDR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of local model subscribing group address 

element_addr

uint16_t element_addr
Element address 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 

group_addr

uint16_t group_addr
Group Address 



ble_mesh_model_unsub_group_addr_comp_param

struct ble_mesh_model_unsub_group_addr_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_MODEL_UNSUBSCRIBE_GROUP_ADDR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of local model unsubscribing group address 

element_addr

uint16_t element_addr
Element address 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 

group_addr

uint16_t group_addr
Group Address 



ble_mesh_node_add_local_app_key_comp_param

struct ble_mesh_node_add_local_app_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_ADD_LOCAL_APP_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of adding local AppKey by the node 

net_idx

uint16_t net_idx
NetKey Index 

app_idx

uint16_t app_idx
AppKey Index 



ble_mesh_node_add_local_net_key_comp_param

struct ble_mesh_node_add_local_net_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_ADD_LOCAL_NET_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of adding local NetKey by the node 

net_idx

uint16_t net_idx
NetKey Index 



ble_mesh_node_bind_local_mod_app_comp_param

struct ble_mesh_node_bind_local_mod_app_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_BIND_APP_KEY_TO_MODEL_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of binding AppKey with model by the node 

element_addr

uint16_t element_addr
Element address 

app_idx

uint16_t app_idx
AppKey Index 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



ble_mesh_output_num_evt_param

struct ble_mesh_output_num_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_OUTPUT_NUMBER_EVT. 

Public Members
action

esp_ble_mesh_output_action_t action
Action of Output OOB Authentication 

number

uint32_t number
Number of Output OOB Authentication 



ble_mesh_output_str_evt_param

struct ble_mesh_output_str_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_OUTPUT_STRING_EVT. 

Public Members
string

char string[8]
String of Output OOB Authentication 



ble_mesh_prov_disable_comp_param

struct ble_mesh_prov_disable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_DISABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of disabling BLE Mesh device 



ble_mesh_prov_enable_comp_param

struct ble_mesh_prov_enable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of enabling BLE Mesh device 



ble_mesh_prov_register_comp_param

struct ble_mesh_prov_register_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROV_REGISTER_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of BLE Mesh initialization 



ble_mesh_provision_complete_evt_param

struct ble_mesh_provision_complete_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

net_key

uint8_t net_key[16]
NetKey 

addr

uint16_t addr
Primary address 

flags

uint8_t flags
Flags 

iv_index

uint32_t iv_index
IV Index 



ble_mesh_provision_reset_param

struct ble_mesh_provision_reset_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_RESET_EVT. 

ble_mesh_provisioner_add_local_app_key_comp_param

struct ble_mesh_provisioner_add_local_app_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of adding local AppKey by the Provisioner 

net_idx

uint16_t net_idx
NetKey Index 

app_idx

uint16_t app_idx
AppKey Index 



ble_mesh_provisioner_add_local_net_key_comp_param

struct ble_mesh_provisioner_add_local_net_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_NET_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of adding local NetKey by the Provisioner 

net_idx

uint16_t net_idx
NetKey Index 



ble_mesh_provisioner_add_unprov_dev_comp_param

struct ble_mesh_provisioner_add_unprov_dev_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of adding device into queue by the Provisioner 



ble_mesh_provisioner_bind_local_mod_app_comp_param

struct ble_mesh_provisioner_bind_local_mod_app_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of binding AppKey with model by the Provisioner 

element_addr

uint16_t element_addr
Element address 

app_idx

uint16_t app_idx
AppKey Index 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



ble_mesh_provisioner_cert_based_prov_start_evt_param

struct ble_mesh_provisioner_cert_based_prov_start_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_CERT_BASED_PROV_START_EVT. 

Public Members
link_idx

uint16_t link_idx
Index of the provisioning link 



ble_mesh_provisioner_delete_dev_comp_param

struct ble_mesh_provisioner_delete_dev_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_DELETE_DEV_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of deleting device by the Provisioner 



ble_mesh_provisioner_delete_node_with_addr_comp_param

struct ble_mesh_provisioner_delete_node_with_addr_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_ADDR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of deleting node with unicast address by the Provisioner 

unicast_addr

uint16_t unicast_addr
Node unicast address 



ble_mesh_provisioner_delete_node_with_uuid_comp_param

struct ble_mesh_provisioner_delete_node_with_uuid_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_UUID_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of deleting node with uuid by the Provisioner 

uuid

uint8_t uuid[16]
Node device uuid 



ble_mesh_provisioner_link_close_evt_param

struct ble_mesh_provisioner_link_close_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT. 

Public Members
bearer

esp_ble_mesh_prov_bearer_t bearer
Type of the bearer used when Provisioner link is closed 

reason

uint8_t reason
Reason of the closed provisioning link 



ble_mesh_provisioner_link_open_evt_param

struct ble_mesh_provisioner_link_open_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT. 

Public Members
bearer

esp_ble_mesh_prov_bearer_t bearer
Type of the bearer used when Provisioner link is opened 



ble_mesh_provisioner_prov_comp_param

struct ble_mesh_provisioner_prov_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT. 

Public Members
node_idx

uint16_t node_idx
Index of the provisioned device 

device_uuid

esp_ble_mesh_octet16_t device_uuid
Device UUID of the provisioned device 

unicast_addr

uint16_t unicast_addr
Primary address of the provisioned device 

element_num

uint8_t element_num
Element count of the provisioned device 

netkey_idx

uint16_t netkey_idx
NetKey Index of the provisioned device 



ble_mesh_provisioner_prov_dev_with_addr_comp_param

struct ble_mesh_provisioner_prov_dev_with_addr_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_DEV_WITH_ADDR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of Provisioner starting to provision a device 



ble_mesh_provisioner_prov_disable_comp_param

struct ble_mesh_provisioner_prov_disable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of disabling BLE Mesh Provisioner 



ble_mesh_provisioner_prov_enable_comp_param

struct ble_mesh_provisioner_prov_enable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of enabling BLE Mesh Provisioner 



ble_mesh_provisioner_prov_input_evt_param

struct ble_mesh_provisioner_prov_input_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_INPUT_EVT. 

Public Members
method

esp_ble_mesh_oob_method_t method
Method of device Output OOB Authentication 

action

esp_ble_mesh_output_action_t action
Action of device Output OOB Authentication 

size

uint8_t size
Size of device Output OOB Authentication 

link_idx

uint8_t link_idx
Index of the provisioning link 



ble_mesh_provisioner_prov_input_num_comp_param

struct ble_mesh_provisioner_prov_input_num_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_INPUT_NUMBER_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of inputting number by the Provisioner 



ble_mesh_provisioner_prov_input_str_comp_param

struct ble_mesh_provisioner_prov_input_str_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_INPUT_STRING_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of inputting string by the Provisioner 



ble_mesh_provisioner_prov_output_evt_param

struct ble_mesh_provisioner_prov_output_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_OUTPUT_EVT. 

Public Members
method

esp_ble_mesh_oob_method_t method
Method of device Input OOB Authentication 

action

esp_ble_mesh_input_action_t action
Action of device Input OOB Authentication 

size

uint8_t size
Size of device Input OOB Authentication 

link_idx

uint8_t link_idx
Index of the provisioning link 

string

char string[8]
String output by the Provisioner 

number

uint32_t number
Number output by the Provisioner 

[anonymous]

union esp_ble_mesh_prov_cb_param_t::ble_mesh_provisioner_prov_output_evt_param::[anonymous] [anonymous]
Union of output OOB 



ble_mesh_provisioner_prov_read_oob_pub_key_comp_param

struct ble_mesh_provisioner_prov_read_oob_pub_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting OOB Public Key by the Provisioner 



ble_mesh_provisioner_prov_read_oob_pub_key_evt_param

struct ble_mesh_provisioner_prov_read_oob_pub_key_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_EVT. 

Public Members
link_idx

uint8_t link_idx
Index of the provisioning link 



ble_mesh_provisioner_prov_record_recv_comp_evt_param

struct ble_mesh_provisioner_prov_record_recv_comp_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_PROV_RECORD_RECV_COMP_EVT. 

Public Members
status

uint8_t status
Indicates whether or not the request was handled successfully 

link_idx

uint16_t link_idx
Index of the provisioning link 

record_id

uint16_t record_id
Identifies the provisioning record for which the request is made 

frag_offset

uint16_t frag_offset
The starting offset of the requested fragment in the provisioning record data 

total_len

uint16_t total_len
Total length of the provisioning record data stored on the Provisionee 

record

uint8_t *record
Provisioning record data fragment 



ble_mesh_provisioner_recv_prov_records_list_evt_param

struct ble_mesh_provisioner_recv_prov_records_list_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_RECV_PROV_RECORDS_LIST_EVT. 

Public Members
link_idx

uint16_t link_idx
Index of the provisioning link 

len

uint16_t len
Length of message 

msg

uint8_t *msg
Lists the Record IDs of the provisioning records stored on the Provisionee 



ble_mesh_provisioner_recv_unprov_adv_pkt_param

struct ble_mesh_provisioner_recv_unprov_adv_pkt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT. 

Public Members
dev_uuid

uint8_t dev_uuid[16]
Device UUID of the unprovisioned device 

addr

esp_ble_mesh_bd_addr_t addr
Device address of the unprovisioned device 

addr_type

esp_ble_mesh_addr_type_t addr_type
Device address type 

oob_info

uint16_t oob_info
OOB Info of the unprovisioned device 

adv_type

uint8_t adv_type
Advertising type of the unprovisioned device 

bearer

esp_ble_mesh_prov_bearer_t bearer
Bearer of the unprovisioned device 

rssi

int8_t rssi
RSSI of the received advertising packet 



ble_mesh_provisioner_send_link_close_evt_param

struct ble_mesh_provisioner_send_link_close_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SEND_LINK_CLOSE_EVT. 

Public Members
link_idx

uint16_t link_idx
Index of the provisioning link 

err_code

int err_code
Indicate the result of send Link Close message 



ble_mesh_provisioner_send_prov_invite_evt_param

struct ble_mesh_provisioner_send_prov_invite_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SEND_PROV_INVITE_EVT. 

Public Members
link_idx

uint16_t link_idx
Index of the provisioning link 

err_code

int err_code
Indicate the result of send Provisioning Invite message 



ble_mesh_provisioner_send_prov_record_req_evt_param

struct ble_mesh_provisioner_send_prov_record_req_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORD_REQUEST_EVT. 

Public Members
err_code

int err_code
Indicate the result of send Provisioning Record Request message 

link_idx

uint16_t link_idx
Index of the provisioning link 

record_id

uint16_t record_id
Identifies the provisioning record for which the request is made 

frag_offset

uint16_t frag_offset
The starting offset of the requested fragment in the provisioning record data 

max_size

uint16_t max_size
The maximum size of the provisioning record fragment that the Provisioner can receive 



ble_mesh_provisioner_send_prov_records_get_evt_param

struct ble_mesh_provisioner_send_prov_records_get_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORDS_GET_EVT. 

Public Members
err_code

int err_code
Indicate the result of send Provisioning Records List Get message 

link_idx

uint16_t link_idx
Index of the provisioning link 



ble_mesh_provisioner_set_dev_uuid_match_comp_param

struct ble_mesh_provisioner_set_dev_uuid_match_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting Device UUID match value by the Provisioner 



ble_mesh_provisioner_set_node_name_comp_param

struct ble_mesh_provisioner_set_node_name_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting provisioned device name by the Provisioner 

node_index

uint16_t node_index
Index of the provisioned device 



ble_mesh_provisioner_set_primary_elem_addr_comp_param

struct ble_mesh_provisioner_set_primary_elem_addr_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SET_PRIMARY_ELEM_ADDR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting unicast address of primary element by the Provisioner 



ble_mesh_provisioner_set_prov_data_info_comp_param

struct ble_mesh_provisioner_set_prov_data_info_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SET_PROV_DATA_INFO_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting provisioning info by the Provisioner 



ble_mesh_provisioner_set_static_oob_val_comp_param

struct ble_mesh_provisioner_set_static_oob_val_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_SET_STATIC_OOB_VALUE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting static oob value by the Provisioner 



ble_mesh_provisioner_store_node_comp_data_comp_param

struct ble_mesh_provisioner_store_node_comp_data_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_STORE_NODE_COMP_DATA_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of storing node composition data by the Provisioner 

addr

uint16_t addr
Node element address 



ble_mesh_provisioner_update_local_app_key_comp_param

struct ble_mesh_provisioner_update_local_app_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_APP_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of updating local AppKey by the Provisioner 

net_idx

uint16_t net_idx
NetKey Index 

app_idx

uint16_t app_idx
AppKey Index 



ble_mesh_provisioner_update_local_net_key_comp_param

struct ble_mesh_provisioner_update_local_net_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_NET_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of updating local NetKey by the Provisioner 

net_idx

uint16_t net_idx
NetKey Index 



ble_mesh_proxy_client_add_filter_addr_comp_param

struct ble_mesh_proxy_client_add_filter_addr_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_ADD_FILTER_ADDR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of Proxy Client add filter address 

conn_handle

uint8_t conn_handle
Proxy connection handle 

net_idx

uint16_t net_idx
Corresponding NetKey Index 



ble_mesh_proxy_client_connect_comp_param

struct ble_mesh_proxy_client_connect_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_CONNECT_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of Proxy Client connect 

addr

esp_ble_mesh_bd_addr_t addr
Device address of the Proxy Server 

addr_type

esp_ble_mesh_addr_type_t addr_type
Device address type 

net_idx

uint16_t net_idx
Corresponding NetKey Index 



ble_mesh_proxy_client_connected_param

struct ble_mesh_proxy_client_connected_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_CONNECTED_EVT. 

Public Members
addr

esp_ble_mesh_bd_addr_t addr
Device address of the Proxy Server 

addr_type

esp_ble_mesh_addr_type_t addr_type
Device address type 

conn_handle

uint8_t conn_handle
Proxy connection handle 

net_idx

uint16_t net_idx
Corresponding NetKey Index 



ble_mesh_proxy_client_directed_proxy_set_param

struct ble_mesh_proxy_client_directed_proxy_set_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_DIRECTED_PROXY_CONTROL_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of Proxy Client directed proxy control address 

conn_handle

uint8_t conn_handle
Proxy connection handle 

net_idx

uint16_t net_idx
Corresponding NetKey Index 



ble_mesh_proxy_client_disconnect_comp_param

struct ble_mesh_proxy_client_disconnect_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_DISCONNECT_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of Proxy Client disconnect 

conn_handle

uint8_t conn_handle
Proxy connection handle 



ble_mesh_proxy_client_disconnected_param

struct ble_mesh_proxy_client_disconnected_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_DISCONNECTED_EVT. 

Public Members
addr

esp_ble_mesh_bd_addr_t addr
Device address of the Proxy Server 

addr_type

esp_ble_mesh_addr_type_t addr_type
Device address type 

conn_handle

uint8_t conn_handle
Proxy connection handle 

net_idx

uint16_t net_idx
Corresponding NetKey Index 

reason

uint8_t reason
Proxy disconnect reason 



ble_mesh_proxy_client_recv_adv_pkt_param

struct ble_mesh_proxy_client_recv_adv_pkt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_RECV_ADV_PKT_EVT. 

Public Members
addr

esp_ble_mesh_bd_addr_t addr
Device address 

addr_type

esp_ble_mesh_addr_type_t addr_type
Device address type 

net_idx

uint16_t net_idx
Network ID related NetKey Index 

net_id

uint8_t net_id[8]
Network ID contained in the advertising packet 

rssi

int8_t rssi
RSSI of the received advertising packet 



ble_mesh_proxy_client_recv_filter_status_param

struct ble_mesh_proxy_client_recv_filter_status_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_RECV_FILTER_STATUS_EVT. 

Public Members
conn_handle

uint8_t conn_handle
Proxy connection handle 

server_addr

uint16_t server_addr
Proxy Server primary element address 

net_idx

uint16_t net_idx
Corresponding NetKey Index 

filter_type

uint8_t filter_type
Proxy Server filter type(whitelist or blacklist) 

list_size

uint16_t list_size
Number of addresses in the Proxy Server filter list 



ble_mesh_proxy_client_remove_filter_addr_comp_param

struct ble_mesh_proxy_client_remove_filter_addr_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_REMOVE_FILTER_ADDR_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of Proxy Client remove filter address 

conn_handle

uint8_t conn_handle
Proxy connection handle 

net_idx

uint16_t net_idx
Corresponding NetKey Index 



ble_mesh_proxy_client_set_filter_type_comp_param

struct ble_mesh_proxy_client_set_filter_type_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_CLIENT_SET_FILTER_TYPE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of Proxy Client set filter type 

conn_handle

uint8_t conn_handle
Proxy connection handle 

net_idx

uint16_t net_idx
Corresponding NetKey Index 



ble_mesh_proxy_gatt_disable_comp_param

struct ble_mesh_proxy_gatt_disable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROXY_GATT_DISABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of disabling Mesh Proxy Service 



ble_mesh_proxy_gatt_enable_comp_param

struct ble_mesh_proxy_gatt_enable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROXY_GATT_ENABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of enabling Mesh Proxy Service 



ble_mesh_proxy_identity_enable_comp_param

struct ble_mesh_proxy_identity_enable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROXY_IDENTITY_ENABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of enabling Mesh Proxy advertising 



ble_mesh_proxy_private_identity_disable_comp_param

struct ble_mesh_proxy_private_identity_disable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_DISABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of disabling Mesh Proxy private advertising 



ble_mesh_proxy_private_identity_enable_comp_param

struct ble_mesh_proxy_private_identity_enable_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_ENABLE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of enabling Mesh Proxy private advertising 



ble_mesh_proxy_server_connected_param

struct ble_mesh_proxy_server_connected_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT. 

Public Members
conn_handle

uint8_t conn_handle
Proxy connection handle 



ble_mesh_proxy_server_disconnected_param

struct ble_mesh_proxy_server_disconnected_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT. 

Public Members
conn_handle

uint8_t conn_handle
Proxy connection handle 

reason

uint8_t reason
Proxy disconnect reason 



ble_mesh_set_fast_prov_action_comp_param

struct ble_mesh_set_fast_prov_action_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_SET_FAST_PROV_ACTION_COMP_EVT. 

Public Members
status_action

uint8_t status_action
Indicate the result of setting action of fast provisioning 



ble_mesh_set_fast_prov_info_comp_param

struct ble_mesh_set_fast_prov_info_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_SET_FAST_PROV_INFO_COMP_EVT. 

Public Members
status_unicast

uint8_t status_unicast
Indicate the result of setting unicast address range of fast provisioning 

status_net_idx

uint8_t status_net_idx
Indicate the result of setting NetKey Index of fast provisioning 

status_match

uint8_t status_match
Indicate the result of setting matching Device UUID of fast provisioning 



ble_mesh_set_oob_pub_key_comp_param

struct ble_mesh_set_oob_pub_key_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_PROV_SET_OOB_PUB_KEY_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting OOB Public Key 



ble_mesh_set_unprov_dev_name_comp_param

struct ble_mesh_set_unprov_dev_name_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of setting BLE Mesh device name 




```

#### esp_ble_mesh_server_state_value_t


```


union esp_ble_mesh_server_state_value_t

#include <esp_ble_mesh_defs.h>
Server model state value union. 

Public Members
onoff

uint8_t onoff
The value of the Generic OnOff state
The value of the Light LC Light OnOff state 

gen_onoff

struct esp_ble_mesh_server_state_value_t::[anonymous] gen_onoff
The Generic OnOff state 

level

int16_t level
The value of the Generic Level state 

gen_level

struct esp_ble_mesh_server_state_value_t::[anonymous] gen_level
The Generic Level state 

onpowerup

uint8_t onpowerup
The value of the Generic OnPowerUp state 

gen_onpowerup

struct esp_ble_mesh_server_state_value_t::[anonymous] gen_onpowerup
The Generic OnPowerUp state 

power

uint16_t power
The value of the Generic Power Actual state 

gen_power_actual

struct esp_ble_mesh_server_state_value_t::[anonymous] gen_power_actual
The Generic Power Actual state 

lightness

uint16_t lightness
The value of the Light Lightness Actual state
The value of the Light Lightness Linear state
The value of the Light CTL Lightness state
The value of the Light HSL Lightness state
The value of the Light xyL Lightness state 

light_lightness_actual

struct esp_ble_mesh_server_state_value_t::[anonymous] light_lightness_actual
The Light Lightness Actual state 

light_lightness_linear

struct esp_ble_mesh_server_state_value_t::[anonymous] light_lightness_linear
The Light Lightness Linear state 

light_ctl_lightness

struct esp_ble_mesh_server_state_value_t::[anonymous] light_ctl_lightness
The Light CTL Lightness state 

temperature

uint16_t temperature
The value of the Light CTL Temperature state 

delta_uv

int16_t delta_uv
The value of the Light CTL Delta UV state 

light_ctl_temp_delta_uv

struct esp_ble_mesh_server_state_value_t::[anonymous] light_ctl_temp_delta_uv
The Light CTL Temperature & Delta UV states 

hue

uint16_t hue
The value of the Light HSL Hue state 

saturation

uint16_t saturation
The value of the Light HSL Saturation state 

light_hsl

struct esp_ble_mesh_server_state_value_t::[anonymous] light_hsl
The Light HSL composite state 

light_hsl_lightness

struct esp_ble_mesh_server_state_value_t::[anonymous] light_hsl_lightness
The Light HSL Lightness state 

light_hsl_hue

struct esp_ble_mesh_server_state_value_t::[anonymous] light_hsl_hue
The Light HSL Hue state 

light_hsl_saturation

struct esp_ble_mesh_server_state_value_t::[anonymous] light_hsl_saturation
The Light HSL Saturation state 

light_xyl_lightness

struct esp_ble_mesh_server_state_value_t::[anonymous] light_xyl_lightness
The Light xyL Lightness state 

light_lc_light_onoff

struct esp_ble_mesh_server_state_value_t::[anonymous] light_lc_light_onoff
The Light LC Light OnOff state 



```

#### esp_ble_mesh_model_cb_param_t


```


union esp_ble_mesh_model_cb_param_t

#include <esp_ble_mesh_defs.h>
BLE Mesh model callback parameters union. 

Public Members
model_operation

struct esp_ble_mesh_model_cb_param_t::ble_mesh_model_operation_evt_param model_operation
Event parameter of ESP_BLE_MESH_MODEL_OPERATION_EVT 

model_send_comp

struct esp_ble_mesh_model_cb_param_t::ble_mesh_model_send_comp_param model_send_comp
Event parameter of ESP_BLE_MESH_MODEL_SEND_COMP_EVT 

model_publish_comp

struct esp_ble_mesh_model_cb_param_t::ble_mesh_model_publish_comp_param model_publish_comp
Event parameter of ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT 

client_recv_publish_msg

struct esp_ble_mesh_model_cb_param_t::ble_mesh_mod_recv_publish_msg_param client_recv_publish_msg
Event parameter of ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT 

client_send_timeout

struct esp_ble_mesh_model_cb_param_t::ble_mesh_client_model_send_timeout_param client_send_timeout
Event parameter of ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT 

model_publish_update

struct esp_ble_mesh_model_cb_param_t::ble_mesh_model_publish_update_evt_param model_publish_update
Event parameter of ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT 

server_model_update_state

struct esp_ble_mesh_model_cb_param_t::ble_mesh_server_model_update_state_comp_param server_model_update_state
Event parameter of ESP_BLE_MESH_SERVER_MODEL_UPDATE_STATE_COMP_EVT 


ble_mesh_client_model_send_timeout_param

struct ble_mesh_client_model_send_timeout_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT. 

Public Members
opcode

uint32_t opcode
Opcode of the previously sent message 

model

esp_ble_mesh_model_t *model
Pointer to the model which sends the previous message 

ctx

esp_ble_mesh_msg_ctx_t *ctx
Pointer to the context of the previous message 



ble_mesh_mod_recv_publish_msg_param

struct ble_mesh_mod_recv_publish_msg_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT. 

Public Members
opcode

uint32_t opcode
Opcode of the unsolicited received message 

model

esp_ble_mesh_model_t *model
Pointer to the model which receives the message 

ctx

esp_ble_mesh_msg_ctx_t *ctx
Pointer to the context of the message 

length

uint16_t length
Length of the received message 

msg

uint8_t *msg
Value of the received message 



ble_mesh_model_operation_evt_param

struct ble_mesh_model_operation_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_MODEL_OPERATION_EVT. 

Public Members
opcode

uint32_t opcode
Opcode of the received message 

model

esp_ble_mesh_model_t *model
Pointer to the model which receives the message 

ctx

esp_ble_mesh_msg_ctx_t *ctx
Pointer to the context of the received message 

length

uint16_t length
Length of the received message 

msg

uint8_t *msg
Value of the received message 



ble_mesh_model_publish_comp_param

struct ble_mesh_model_publish_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of publishing a message 

model

esp_ble_mesh_model_t *model
Pointer to the model which publishes the message 



ble_mesh_model_publish_update_evt_param

struct ble_mesh_model_publish_update_evt_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT. 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the model which is going to update its publish message 



ble_mesh_model_send_comp_param

struct ble_mesh_model_send_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_MODEL_SEND_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of sending a message 

opcode

uint32_t opcode
Opcode of the message 

model

esp_ble_mesh_model_t *model
Pointer to the model which sends the message 

ctx

esp_ble_mesh_msg_ctx_t *ctx
Context of the message 



ble_mesh_server_model_update_state_comp_param

struct ble_mesh_server_model_update_state_comp_param

#include <esp_ble_mesh_defs.h>
ESP_BLE_MESH_SERVER_MODEL_UPDATE_STATE_COMP_EVT. 

Public Members
err_code

int err_code
Indicate the result of updating server model state 

model

esp_ble_mesh_model_t *model
Pointer to the server model which state value is updated 

type

esp_ble_mesh_server_state_type_t type
Type of the updated server state 




```



### Structures


#### esp_ble_mesh_deinit_param_t


```


struct esp_ble_mesh_deinit_param_t
BLE Mesh deinit parameters 

Public Members
erase_flash

bool erase_flash
Indicate if erasing flash when deinit mesh stack 



```

#### esp_ble_mesh_uar_t


```


struct esp_ble_mesh_uar_t
Format of Unicast Address Range 

Public Members
len_present

uint16_t len_present
Indicate the presence or absence of the RangeLength field 

range_start

uint16_t range_start
15 least significant bits of the starting unicast address 

range_length

uint8_t range_length
Number of addresses in the range (0x02 - 0xFF) 



```

#### esp_ble_mesh_elem_t


```


struct esp_ble_mesh_elem_t
Abstraction that describes a BLE Mesh Element. This structure is associated with struct bt_mesh_elem in mesh_access.h 

Public Members
element_addr

uint16_t element_addr
Element Address, assigned during provisioning. 

location

const uint16_t location
Location Descriptor (GATT Bluetooth Namespace Descriptors) 

sig_model_count

const uint8_t sig_model_count
SIG Model count 

vnd_model_count

const uint8_t vnd_model_count
Vendor Model count 

sig_models

esp_ble_mesh_model_t *sig_models
SIG Models 

vnd_models

esp_ble_mesh_model_t *vnd_models
Vendor Models 



```

#### esp_ble_mesh_model_pub_t


```


struct esp_ble_mesh_model_pub_t
Abstraction that describes a model publication context. This structure is associated with struct bt_mesh_model_pub in mesh_access.h 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the model to which the context belongs. Initialized by the stack. 

publish_addr

uint16_t publish_addr
Publish Address. 

app_idx

uint16_t app_idx
Publish AppKey Index. 

cred

uint16_t cred
Friendship Credentials Flag. 

send_rel

uint16_t send_rel
Force reliable sending (segment acks) 

send_szmic

uint16_t send_szmic
Size of TransMIC when publishing a Segmented Access message 

ttl

uint8_t ttl
Publish Time to Live. 

retransmit

uint8_t retransmit
Retransmit Count & Interval Steps. 

period

uint8_t period
Publish Period. 

period_div

uint8_t period_div
Divisor for the Period. 

fast_period

uint8_t fast_period
Use FastPeriodDivisor 

count

uint8_t count
Retransmissions left. 

period_start

uint32_t period_start
Start of the current period. 

msg

struct net_buf_simple *msg
Publication buffer, containing the publication message. 
This will get correctly created when the publication context has been defined using the ESP_BLE_MESH_MODEL_PUB_DEFINE macro.
ESP_BLE_MESH_MODEL_PUB_DEFINE(name, size); 

update

esp_ble_mesh_cb_t update
Callback used to update publish message. Initialized by the stack. 

timer

struct k_delayed_work timer
Publish Period Timer. Initialized by the stack. 

dev_role

uint8_t dev_role
Role of the device that is going to publish messages 



```

#### esp_ble_mesh_model_op_t


```


struct esp_ble_mesh_model_op_t
Abstraction that describes a model operation context. This structure is associated with struct bt_mesh_model_op in mesh_access.h 

Public Members
opcode

const uint32_t opcode
Message opcode 

min_len

const size_t min_len
Message minimum length 

param_cb

esp_ble_mesh_cb_t param_cb
Callback used to handle message. Initialized by the stack. 



```

#### esp_ble_mesh_model_cbs_t


```


struct esp_ble_mesh_model_cbs_t
Abstraction that describes a model callback structure. This structure is associated with struct bt_mesh_model_cb in mesh_access.h. 

Public Members
init_cb

esp_ble_mesh_cb_t init_cb
Callback used during model initialization. Initialized by the stack. 



```

#### esp_ble_mesh_model


```


struct esp_ble_mesh_model
Abstraction that describes a Mesh Model instance. This structure is associated with struct bt_mesh_model in mesh_access.h 

Public Members
model_id

const uint16_t model_id
16-bit model identifier 

company_id

uint16_t company_id
16-bit company identifier 

model_id

uint16_t model_id
16-bit model identifier 

vnd

struct esp_ble_mesh_model::[anonymous]::[anonymous] vnd
Structure encapsulating a model ID with a company ID 

[anonymous]

union esp_ble_mesh_model::[anonymous] [anonymous]
Model ID 

element_idx

uint8_t element_idx
Internal information, mainly for persistent storage Belongs to Nth element 

model_idx

uint8_t model_idx
Is the Nth model in the element 

flags

uint16_t flags
Information about what has changed 

element

esp_ble_mesh_elem_t *element
The Element to which this Model belongs 

pub

esp_ble_mesh_model_pub_t *const pub
Model Publication 

keys

uint16_t keys[CONFIG_BLE_MESH_MODEL_KEY_COUNT]
AppKey List 

groups

uint16_t groups[CONFIG_BLE_MESH_MODEL_GROUP_COUNT]
Subscription List (group or virtual addresses) 

op

esp_ble_mesh_model_op_t *op
Model operation context 

cb

esp_ble_mesh_model_cbs_t *cb
Model callback structure 

user_data

void *user_data
Model-specific user data 



```

#### esp_ble_mesh_msg_ctx_t


```


struct esp_ble_mesh_msg_ctx_t
Message sending context. This structure is associated with struct bt_mesh_msg_ctx in mesh_access.h 

Public Members
net_idx

uint16_t net_idx
NetKey Index of the subnet through which to send the message. 

app_idx

uint16_t app_idx
AppKey Index for message encryption. 

addr

uint16_t addr
Remote address. 

recv_dst

uint16_t recv_dst
Destination address of a received message. Not used for sending. 

recv_rssi

int8_t recv_rssi
RSSI of a received message. Not used for sending. 

recv_op

uint32_t recv_op
Opcode of a received message. Not used for sending. 

recv_ttl

uint8_t recv_ttl
Received TTL value. Not used for sending. 

recv_cred

uint8_t recv_cred
Security credentials of a received message. Not used for sending. 

recv_tag

uint8_t recv_tag
Tag of a received message. Not used for sending. 

send_rel

uint8_t send_rel
Force sending reliably by using segment acknowledgement. 

send_szmic

uint8_t send_szmic
Size of TransMIC when sending a Segmented Access message. 

send_ttl

uint8_t send_ttl
TTL, or ESP_BLE_MESH_TTL_DEFAULT for default TTL. 

send_cred

uint8_t send_cred
Security credentials used for sending the message 

send_tag

uint8_t send_tag
Tag used for sending the message. 

model

esp_ble_mesh_model_t *model
Model corresponding to the message, no need to be initialized before sending message 

srv_send

bool srv_send
Indicate if the message is sent by a node server model, no need to be initialized before sending message 



```

#### esp_ble_mesh_prov_t


```


struct esp_ble_mesh_prov_t
Provisioning properties & capabilities. This structure is associated with struct bt_mesh_prov in mesh_access.h 

```

#### esp_ble_mesh_comp_t


```


struct esp_ble_mesh_comp_t
Node Composition data context. This structure is associated with struct bt_mesh_comp in mesh_access.h 

Public Members
cid

uint16_t cid
16-bit SIG-assigned company identifier 

pid

uint16_t pid
16-bit vendor-assigned product identifier 

vid

uint16_t vid
16-bit vendor-assigned product version identifier 

element_count

size_t element_count
Element count 

elements

esp_ble_mesh_elem_t *elements
A sequence of elements 



```

#### esp_ble_mesh_unprov_dev_add_t


```


struct esp_ble_mesh_unprov_dev_add_t
Information of the device which is going to be added for provisioning. 

Public Members
addr

esp_ble_mesh_bd_addr_t addr
Device address 

addr_type

esp_ble_mesh_addr_type_t addr_type
Device address type 

uuid

uint8_t uuid[16]
Device UUID 

oob_info

uint16_t oob_info
Device OOB Info ADD_DEV_START_PROV_NOW_FLAG shall not be set if the bearer has both PB-ADV and PB-GATT enabled 

bearer

esp_ble_mesh_prov_bearer_t bearer
Provisioning Bearer 



```

#### esp_ble_mesh_device_delete_t


```


struct esp_ble_mesh_device_delete_t
Information of the device which is going to be deleted. 

Public Members
addr

esp_ble_mesh_bd_addr_t addr
Device address 

addr_type

esp_ble_mesh_addr_type_t addr_type
Device address type 

uuid

uint8_t uuid[16]
Device UUID 

[anonymous]

union esp_ble_mesh_device_delete_t::[anonymous] [anonymous]
Union of Device information 

flag

uint8_t flag
BIT0: device address; BIT1: device UUID 



```

#### esp_ble_mesh_prov_data_info_t


```


struct esp_ble_mesh_prov_data_info_t
Information of the provisioner which is going to be updated. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

flags

uint8_t flags
Flags 

iv_index

uint32_t iv_index
IV Index 

[anonymous]

union esp_ble_mesh_prov_data_info_t::[anonymous] [anonymous]
Provisioning data 

flag

uint8_t flag
BIT0: net_idx; BIT1: flags; BIT2: iv_index 



```

#### esp_ble_mesh_node_t


```


struct esp_ble_mesh_node_t
Information of the provisioned node 

Public Members
addr

esp_ble_mesh_bd_addr_t addr
Node device address 

addr_type

esp_ble_mesh_addr_type_t addr_type
Node device address type 

dev_uuid

uint8_t dev_uuid[16]
Device UUID 

oob_info

uint16_t oob_info
Node OOB information 

unicast_addr

uint16_t unicast_addr
Node unicast address 

element_num

uint8_t element_num
Node element number 

net_idx

uint16_t net_idx
Node NetKey Index 

flags

uint8_t flags
Node key refresh flag and iv update flag 

iv_index

uint32_t iv_index
Node IV Index 

dev_key

uint8_t dev_key[16]
Node device key 

name

char name[ESP_BLE_MESH_NODE_NAME_MAX_LEN + 1]
Node name 

comp_length

uint16_t comp_length
Length of Composition Data 

comp_data

uint8_t *comp_data
Value of Composition Data 



```

#### esp_ble_mesh_fast_prov_info_t


```


struct esp_ble_mesh_fast_prov_info_t
Context of fast provisioning which need to be set. 

Public Members
unicast_min

uint16_t unicast_min
Minimum unicast address used for fast provisioning 

unicast_max

uint16_t unicast_max
Maximum unicast address used for fast provisioning 

net_idx

uint16_t net_idx
Netkey index used for fast provisioning 

flags

uint8_t flags
Flags used for fast provisioning 

iv_index

uint32_t iv_index
IV Index used for fast provisioning 

offset

uint8_t offset
Offset of the UUID to be compared 

match_len

uint8_t match_len
Length of the UUID to be compared 

match_val

uint8_t match_val[16]
Value of UUID to be compared 



```

#### esp_ble_mesh_heartbeat_filter_info_t


```


struct esp_ble_mesh_heartbeat_filter_info_t
Context of Provisioner heartbeat filter information to be set 

Public Members
hb_src

uint16_t hb_src
Heartbeat source address (unicast address) 

hb_dst

uint16_t hb_dst
Heartbeat destination address (unicast address or group address) 



```

#### esp_ble_mesh_client_op_pair_t


```


struct esp_ble_mesh_client_op_pair_t
BLE Mesh client models related definitions. 
Client model Get/Set message opcode and corresponding Status message opcode 

Public Members
cli_op

uint32_t cli_op
The client message opcode 

status_op

uint32_t status_op
The server status opcode corresponding to the client message opcode 



```

#### esp_ble_mesh_client_t


```


struct esp_ble_mesh_client_t
Client Model user data context. 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the client model. Initialized by the stack. 

op_pair_size

uint32_t op_pair_size
Size of the op_pair 

op_pair

const esp_ble_mesh_client_op_pair_t *op_pair
Table containing get/set message opcode and corresponding status message opcode 

publish_status

uint32_t publish_status
Callback used to handle the received unsolicited message. Initialized by the stack. 

internal_data

void *internal_data
Pointer to the internal data of client model 

vendor_data

void *vendor_data
Pointer to the vendor data of client model 

msg_role

uint8_t msg_role
Role of the device (Node/Provisioner) that is going to send messages 



```

#### esp_ble_mesh_client_common_param_t


```


struct esp_ble_mesh_client_common_param_t
Common parameters of the messages sent by Client Model. 

Public Members
opcode

esp_ble_mesh_opcode_t opcode
Message opcode 

model

esp_ble_mesh_model_t *model
Pointer to the client model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
The context used to send message 

msg_timeout

int32_t msg_timeout
Timeout value (ms) to get response to the sent message Note: if using default timeout value in menuconfig, make sure to set this value to 0 

msg_role

uint8_t msg_role
Role of the device - Node/Provisioner 



```

#### esp_ble_mesh_state_transition_t


```


struct esp_ble_mesh_state_transition_t
Parameters of the server model state transition 

Public Functions
BLE_MESH_ATOMIC_DEFINE

BLE_MESH_ATOMIC_DEFINE(flag, ESP_BLE_MESH_SERVER_FLAG_MAX)
Flag used to indicate if the transition timer has been started internally.
If the model which contains esp_ble_mesh_state_transition_t sets "set_auto_rsp" to ESP_BLE_MESH_SERVER_RSP_BY_APP, the handler of the timer shall be initialized by the users.
And users can use this flag to indicate whether the timer is started or not. 



Public Members
just_started

bool just_started
Indicate if the state transition has just started 

trans_time

uint8_t trans_time
State transition time 

remain_time

uint8_t remain_time
Remaining time of state transition 

delay

uint8_t delay
Delay before starting state transition 

quo_tt

uint32_t quo_tt
Duration of each divided transition step 

counter

uint32_t counter
Number of steps which the transition duration is divided 

total_duration

uint32_t total_duration
State transition total duration 

start_timestamp

int64_t start_timestamp
Time when the state transition is started 

timer

struct k_delayed_work timer
Timer used for state transition 



```

#### esp_ble_mesh_last_msg_info_t


```


struct esp_ble_mesh_last_msg_info_t
Parameters of the server model received last same set message. 

Public Members
tid

uint8_t tid
Transaction number of the last message 

src

uint16_t src
Source address of the last message 

dst

uint16_t dst
Destination address of the last message 

timestamp

int64_t timestamp
Time when the last message is received 



```

#### esp_ble_mesh_server_rsp_ctrl_t


```


struct esp_ble_mesh_server_rsp_ctrl_t
Parameters of the Server Model response control 

Public Members
get_auto_rsp

uint8_t get_auto_rsp
BLE Mesh Server Response Option. 

If get_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, then the response of Client Get messages need to be replied by the application;
If get_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, then the response of Client Get messages will be replied by the server models;
If set_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, then the response of Client Set messages need to be replied by the application;
If set_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, then the response of Client Set messages will be replied by the server models;
If status_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, then the response of Server Status messages need to be replied by the application;
If status_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, then the response of Server Status messages will be replied by the server models; Response control for Client Get messages 



set_auto_rsp

uint8_t set_auto_rsp
Response control for Client Set messages 

status_auto_rsp

uint8_t status_auto_rsp
Response control for Server Status messages 



```



### Macros


#### ESP_BLE_MESH_SDU_MAX_LEN


```


ESP_BLE_MESH_SDU_MAX_LEN
< The maximum length of a BLE Mesh message, including Opcode, Payload and TransMIC Length of a short Mesh MIC. 

```

#### ESP_BLE_MESH_MIC_SHORT


```


ESP_BLE_MESH_MIC_SHORT
Length of a long Mesh MIC. 

```

#### ESP_BLE_MESH_MIC_LONG


```


ESP_BLE_MESH_MIC_LONG
The maximum length of a BLE Mesh provisioned node name 

```

#### ESP_BLE_MESH_NODE_NAME_MAX_LEN


```


ESP_BLE_MESH_NODE_NAME_MAX_LEN
The maximum length of a BLE Mesh unprovisioned device name 

```

#### ESP_BLE_MESH_DEVICE_NAME_MAX_LEN


```


ESP_BLE_MESH_DEVICE_NAME_MAX_LEN
The maximum length of settings user id 

```

#### ESP_BLE_MESH_SETTINGS_UID_SIZE


```


ESP_BLE_MESH_SETTINGS_UID_SIZE
The default value of Random Update Interval Steps 

```

#### ESP_BLE_MESH_RAND_UPDATE_INTERVAL_DEFAULT


```


ESP_BLE_MESH_RAND_UPDATE_INTERVAL_DEFAULT
Invalid settings index 

```

#### ESP_BLE_MESH_INVALID_SETTINGS_IDX


```


ESP_BLE_MESH_INVALID_SETTINGS_IDX
Define the BLE Mesh octet 16 bytes size 

```

#### ESP_BLE_MESH_OCTET16_LEN


```


ESP_BLE_MESH_OCTET16_LEN

```

#### ESP_BLE_MESH_OCTET8_LEN


```


ESP_BLE_MESH_OCTET8_LEN

```

#### ESP_BLE_MESH_CID_NVAL


```


ESP_BLE_MESH_CID_NVAL
Special TTL value to request using configured default TTL 

```

#### ESP_BLE_MESH_TTL_DEFAULT


```


ESP_BLE_MESH_TTL_DEFAULT
Maximum allowed TTL value 

```

#### ESP_BLE_MESH_TTL_MAX


```


ESP_BLE_MESH_TTL_MAX

```

#### ESP_BLE_MESH_ADDR_UNASSIGNED


```


ESP_BLE_MESH_ADDR_UNASSIGNED

```

#### ESP_BLE_MESH_ADDR_ALL_NODES


```


ESP_BLE_MESH_ADDR_ALL_NODES

```

#### ESP_BLE_MESH_ADDR_PROXIES


```


ESP_BLE_MESH_ADDR_PROXIES

```

#### ESP_BLE_MESH_ADDR_FRIENDS


```


ESP_BLE_MESH_ADDR_FRIENDS

```

#### ESP_BLE_MESH_ADDR_RELAYS


```


ESP_BLE_MESH_ADDR_RELAYS

```

#### ESP_BLE_MESH_KEY_UNUSED


```


ESP_BLE_MESH_KEY_UNUSED

```

#### ESP_BLE_MESH_KEY_DEV


```


ESP_BLE_MESH_KEY_DEV

```

#### ESP_BLE_MESH_KEY_PRIMARY


```


ESP_BLE_MESH_KEY_PRIMARY

```

#### ESP_BLE_MESH_KEY_ANY


```


ESP_BLE_MESH_KEY_ANY
Internal macros used to initialize array members 

```

#### ESP_BLE_MESH_KEY_UNUSED_ELT_


```


ESP_BLE_MESH_KEY_UNUSED_ELT_(IDX, _)

```

#### ESP_BLE_MESH_ADDR_UNASSIGNED_ELT_


```


ESP_BLE_MESH_ADDR_UNASSIGNED_ELT_(IDX, _)

```

#### ESP_BLE_MESH_MODEL_KEYS_UNUSED


```


ESP_BLE_MESH_MODEL_KEYS_UNUSED

```

#### ESP_BLE_MESH_MODEL_GROUPS_UNASSIGNED


```


ESP_BLE_MESH_MODEL_GROUPS_UNASSIGNED
Primary Network Key index 

```

#### ESP_BLE_MESH_NET_PRIMARY


```


ESP_BLE_MESH_NET_PRIMARY
Relay state value 

```

#### ESP_BLE_MESH_RELAY_DISABLED


```


ESP_BLE_MESH_RELAY_DISABLED

```

#### ESP_BLE_MESH_RELAY_ENABLED


```


ESP_BLE_MESH_RELAY_ENABLED

```

#### ESP_BLE_MESH_RELAY_NOT_SUPPORTED


```


ESP_BLE_MESH_RELAY_NOT_SUPPORTED
Beacon state value 

```

#### ESP_BLE_MESH_BEACON_DISABLED


```


ESP_BLE_MESH_BEACON_DISABLED

```

#### ESP_BLE_MESH_BEACON_ENABLED


```


ESP_BLE_MESH_BEACON_ENABLED

```

#### ESP_BLE_MESH_PRIVATE_BEACON_DISABLE


```


ESP_BLE_MESH_PRIVATE_BEACON_DISABLE

```

#### ESP_BLE_MESH_PRIVATE_BEACON_ENABLE


```


ESP_BLE_MESH_PRIVATE_BEACON_ENABLE
GATT Proxy state value 

```

#### ESP_BLE_MESH_GATT_PROXY_DISABLED


```


ESP_BLE_MESH_GATT_PROXY_DISABLED

```

#### ESP_BLE_MESH_GATT_PROXY_ENABLED


```


ESP_BLE_MESH_GATT_PROXY_ENABLED

```

#### ESP_BLE_MESH_GATT_PROXY_NOT_SUPPORTED


```


ESP_BLE_MESH_GATT_PROXY_NOT_SUPPORTED

```

#### ESP_BLE_MESH_PRIVATE_GATT_PROXY_DISABLED


```


ESP_BLE_MESH_PRIVATE_GATT_PROXY_DISABLED

```

#### ESP_BLE_MESH_PRIVATE_GATT_PROXY_ENABLED


```


ESP_BLE_MESH_PRIVATE_GATT_PROXY_ENABLED

```

#### ESP_BLE_MESH_PRIVATE_GATT_PROXY_NOT_SUPPORTED


```


ESP_BLE_MESH_PRIVATE_GATT_PROXY_NOT_SUPPORTED

```

#### ESP_BLE_MESH_PRIVATE_NODE_IDENTITY_DISABLED


```


ESP_BLE_MESH_PRIVATE_NODE_IDENTITY_DISABLED

```

#### ESP_BLE_MESH_PRIVATE_NODE_IDENTITY_ENABLED


```


ESP_BLE_MESH_PRIVATE_NODE_IDENTITY_ENABLED

```

#### ESP_BLE_MESH_PRIVATE_NODE_IDENTITY_NOT_SUPPORTED


```


ESP_BLE_MESH_PRIVATE_NODE_IDENTITY_NOT_SUPPORTED
Friend state value 

```

#### ESP_BLE_MESH_FRIEND_DISABLED


```


ESP_BLE_MESH_FRIEND_DISABLED

```

#### ESP_BLE_MESH_FRIEND_ENABLED


```


ESP_BLE_MESH_FRIEND_ENABLED

```

#### ESP_BLE_MESH_FRIEND_NOT_SUPPORTED


```


ESP_BLE_MESH_FRIEND_NOT_SUPPORTED
Node identity state value 

```

#### ESP_BLE_MESH_NODE_IDENTITY_STOPPED


```


ESP_BLE_MESH_NODE_IDENTITY_STOPPED

```

#### ESP_BLE_MESH_NODE_IDENTITY_RUNNING


```


ESP_BLE_MESH_NODE_IDENTITY_RUNNING

```

#### ESP_BLE_MESH_NODE_IDENTITY_NOT_SUPPORTED


```


ESP_BLE_MESH_NODE_IDENTITY_NOT_SUPPORTED
Subnet Bridge state value 

```

#### ESP_BLE_MESH_SUBNET_BRIDGE_DISABLED


```


ESP_BLE_MESH_SUBNET_BRIDGE_DISABLED

```

#### ESP_BLE_MESH_SUBNET_BRIDGE_ENABLED


```


ESP_BLE_MESH_SUBNET_BRIDGE_ENABLED
Supported features 

```

#### ESP_BLE_MESH_FEATURE_RELAY


```


ESP_BLE_MESH_FEATURE_RELAY

```

#### ESP_BLE_MESH_FEATURE_PROXY


```


ESP_BLE_MESH_FEATURE_PROXY

```

#### ESP_BLE_MESH_FEATURE_FRIEND


```


ESP_BLE_MESH_FEATURE_FRIEND

```

#### ESP_BLE_MESH_FEATURE_LOW_POWER


```


ESP_BLE_MESH_FEATURE_LOW_POWER

```

#### ESP_BLE_MESH_FEATURE_ALL_SUPPORTED


```


ESP_BLE_MESH_FEATURE_ALL_SUPPORTED

```

#### ESP_BLE_MESH_ADDR_IS_UNICAST


```


ESP_BLE_MESH_ADDR_IS_UNICAST(addr)

```

#### ESP_BLE_MESH_ADDR_IS_GROUP


```


ESP_BLE_MESH_ADDR_IS_GROUP(addr)

```

#### ESP_BLE_MESH_ADDR_IS_VIRTUAL


```


ESP_BLE_MESH_ADDR_IS_VIRTUAL(addr)

```

#### ESP_BLE_MESH_ADDR_IS_RFU


```


ESP_BLE_MESH_ADDR_IS_RFU(addr)

```

#### ESP_BLE_MESH_INVALID_NODE_INDEX


```


ESP_BLE_MESH_INVALID_NODE_INDEX

```

#### ESP_BLE_MESH_PROV_RECORD_MAX_ID


```


ESP_BLE_MESH_PROV_RECORD_MAX_ID

```

#### ESP_BLE_MESH_TRANSMIT


```


ESP_BLE_MESH_TRANSMIT(count, int_ms)
Encode transmission count & interval steps. 

Note
For example, ESP_BLE_MESH_TRANSMIT(2, 20) means that the message will be sent about 90ms(count is 3, step is 1, interval is 30 ms which includes 10ms of advertising interval random delay).


Parameters

count -- Number of retransmissions (first transmission is excluded). 
int_ms -- Interval steps in milliseconds. Must be greater than 0 and a multiple of 10.


Returns
BLE Mesh transmit value that can be used e.g. for the default values of the Configuration Model data. 



```

#### ESP_BLE_MESH_GET_TRANSMIT_COUNT


```


ESP_BLE_MESH_GET_TRANSMIT_COUNT(transmit)
Decode transmit count from a transmit value. 

Parameters

transmit -- Encoded transmit count & interval value.


Returns
Transmission count (actual transmissions equal to N + 1). 



```

#### ESP_BLE_MESH_GET_TRANSMIT_INTERVAL


```


ESP_BLE_MESH_GET_TRANSMIT_INTERVAL(transmit)
Decode transmit interval from a transmit value. 

Parameters

transmit -- Encoded transmit count & interval value.


Returns
Transmission interval in milliseconds. 



```

#### ESP_BLE_MESH_PUBLISH_TRANSMIT


```


ESP_BLE_MESH_PUBLISH_TRANSMIT(count, int_ms)
Encode Publish Retransmit count & interval steps. 

Parameters

count -- Number of retransmissions (first transmission is excluded). 
int_ms -- Interval steps in milliseconds. Must be greater than 0 and a multiple of 50.


Returns
BLE Mesh transmit value that can be used e.g. for the default values of the Configuration Model data. 



```

#### ESP_BLE_MESH_GET_PUBLISH_TRANSMIT_COUNT


```


ESP_BLE_MESH_GET_PUBLISH_TRANSMIT_COUNT(transmit)
Decode Publish Retransmit count from a given value. 

Parameters

transmit -- Encoded Publish Retransmit count & interval value.


Returns
Retransmission count (actual transmissions equal to N + 1). 



```

#### ESP_BLE_MESH_GET_PUBLISH_TRANSMIT_INTERVAL


```


ESP_BLE_MESH_GET_PUBLISH_TRANSMIT_INTERVAL(transmit)
Decode Publish Retransmit interval from a given value. 

Callbacks which are not needed to be initialized by users (set with 0 and will be initialized internally) 

Parameters

transmit -- Encoded Publish Retransmit count & interval value.


Returns
Transmission interval in milliseconds.



```

#### ESP_BLE_MESH_PROV_STATIC_OOB_MAX_LEN


```


ESP_BLE_MESH_PROV_STATIC_OOB_MAX_LEN
Maximum length of string used by Output OOB authentication 

```

#### ESP_BLE_MESH_PROV_OUTPUT_OOB_MAX_LEN


```


ESP_BLE_MESH_PROV_OUTPUT_OOB_MAX_LEN
Maximum length of string used by Output OOB authentication 

```

#### ESP_BLE_MESH_PROV_INPUT_OOB_MAX_LEN


```


ESP_BLE_MESH_PROV_INPUT_OOB_MAX_LEN
Macros used to define message opcode 

```

#### ESP_BLE_MESH_MODEL_OP_1


```


ESP_BLE_MESH_MODEL_OP_1(b0)

```

#### ESP_BLE_MESH_MODEL_OP_2


```


ESP_BLE_MESH_MODEL_OP_2(b0, b1)

```

#### ESP_BLE_MESH_MODEL_OP_3


```


ESP_BLE_MESH_MODEL_OP_3(b0, cid)
This macro is associated with BLE_MESH_MODEL_CB in mesh_access.h 

```

#### ESP_BLE_MESH_SIG_MODEL


```


ESP_BLE_MESH_SIG_MODEL(_id, _op, _pub, _user_data)
This macro is associated with BLE_MESH_MODEL_VND_CB in mesh_access.h 

```

#### ESP_BLE_MESH_VENDOR_MODEL


```


ESP_BLE_MESH_VENDOR_MODEL(_company, _id, _op, _pub, _user_data)

```

#### ESP_BLE_MESH_ELEMENT


```


ESP_BLE_MESH_ELEMENT(_loc, _mods, _vnd_mods)
Helper to define a BLE Mesh element within an array. 
In case the element has no SIG or Vendor models, the helper macro ESP_BLE_MESH_MODEL_NONE can be given instead.

Note
This macro is associated with BLE_MESH_ELEM in mesh_access.h


Parameters

_loc -- Location Descriptor. 
_mods -- Array of SIG models. 
_vnd_mods -- Array of vendor models. 




```

#### ESP_BLE_MESH_PROV


```


ESP_BLE_MESH_PROV(uuid, sta_val, sta_val_len, out_size, out_act, in_size, in_act)

```

#### BT_OCTET32_LEN


```


BT_OCTET32_LEN

```

#### BD_ADDR_LEN


```


BD_ADDR_LEN

```

#### ESP_BLE_MESH_ADDR_TYPE_PUBLIC


```


ESP_BLE_MESH_ADDR_TYPE_PUBLIC

```

#### ESP_BLE_MESH_ADDR_TYPE_RANDOM


```


ESP_BLE_MESH_ADDR_TYPE_RANDOM

```

#### ESP_BLE_MESH_ADDR_TYPE_RPA_PUBLIC


```


ESP_BLE_MESH_ADDR_TYPE_RPA_PUBLIC

```

#### ESP_BLE_MESH_ADDR_TYPE_RPA_RANDOM


```


ESP_BLE_MESH_ADDR_TYPE_RPA_RANDOM

```

#### ESP_BLE_MESH_DIRECTED_FORWARDING_DISABLED


```


ESP_BLE_MESH_DIRECTED_FORWARDING_DISABLED

```

#### ESP_BLE_MESH_DIRECTED_FORWARDING_ENABLED


```


ESP_BLE_MESH_DIRECTED_FORWARDING_ENABLED

```

#### ESP_BLE_MESH_DIRECTED_RELAY_DISABLED


```


ESP_BLE_MESH_DIRECTED_RELAY_DISABLED

```

#### ESP_BLE_MESH_DIRECTED_RELAY_ENABLED


```


ESP_BLE_MESH_DIRECTED_RELAY_ENABLED

```

#### ESP_BLE_MESH_DIRECTED_PROXY_IGNORE


```


ESP_BLE_MESH_DIRECTED_PROXY_IGNORE

```

#### ESP_BLE_MESH_DIRECTED_PROXY_USE_DEFAULT_IGNORE


```


ESP_BLE_MESH_DIRECTED_PROXY_USE_DEFAULT_IGNORE

```

#### ESP_BLE_MESH_DIRECTED_FRIEND_IGNORE


```


ESP_BLE_MESH_DIRECTED_FRIEND_IGNORE

```

#### ESP_BLE_MESH_DIRECTED_PROXY_DISABLED


```


ESP_BLE_MESH_DIRECTED_PROXY_DISABLED

```

#### ESP_BLE_MESH_DIRECTED_PROXY_ENABLED


```


ESP_BLE_MESH_DIRECTED_PROXY_ENABLED

```

#### ESP_BLE_MESH_DIRECTED_PROXY_NOT_SUPPORTED


```


ESP_BLE_MESH_DIRECTED_PROXY_NOT_SUPPORTED

```

#### ESP_BLE_MESH_DIRECTED_PROXY_USE_DEF_DISABLED


```


ESP_BLE_MESH_DIRECTED_PROXY_USE_DEF_DISABLED

```

#### ESP_BLE_MESH_DIRECTED_PROXY_USE_DEF_ENABLED


```


ESP_BLE_MESH_DIRECTED_PROXY_USE_DEF_ENABLED

```

#### ESP_BLE_MESH_DIRECTED_PROXY_USE_DEF_NOT_SUPPORTED


```


ESP_BLE_MESH_DIRECTED_PROXY_USE_DEF_NOT_SUPPORTED

```

#### ESP_BLE_MESH_DIRECTED_FRIEND_DISABLED


```


ESP_BLE_MESH_DIRECTED_FRIEND_DISABLED

```

#### ESP_BLE_MESH_DIRECTED_FRIEND_ENABLED


```


ESP_BLE_MESH_DIRECTED_FRIEND_ENABLED

```

#### ESP_BLE_MESH_DIRECTED_FRIEND_NOT_SUPPORTED


```


ESP_BLE_MESH_DIRECTED_FRIEND_NOT_SUPPORTED

```

#### ESP_BLE_MESH_DIRECTED_PUB_POLICY_FLOODING


```


ESP_BLE_MESH_DIRECTED_PUB_POLICY_FLOODING

```

#### ESP_BLE_MESH_DIRECTED_PUB_POLICY_FORWARD


```


ESP_BLE_MESH_DIRECTED_PUB_POLICY_FORWARD

```

#### ESP_BLE_MESH_PROXY_USE_DIRECTED_DISABLED


```


ESP_BLE_MESH_PROXY_USE_DIRECTED_DISABLED

```

#### ESP_BLE_MESH_PROXY_USE_DIRECTED_ENABLED


```


ESP_BLE_MESH_PROXY_USE_DIRECTED_ENABLED

```

#### ESP_BLE_MESH_FLOODING_CRED


```


ESP_BLE_MESH_FLOODING_CRED

```

#### ESP_BLE_MESH_FRIENDSHIP_CRED


```


ESP_BLE_MESH_FRIENDSHIP_CRED

```

#### ESP_BLE_MESH_DIRECTED_CRED


```


ESP_BLE_MESH_DIRECTED_CRED

```

#### ESP_BLE_MESH_TAG_SEND_SEGMENTED


```


ESP_BLE_MESH_TAG_SEND_SEGMENTED

```

#### ESP_BLE_MESH_TAG_IMMUTABLE_CRED


```


ESP_BLE_MESH_TAG_IMMUTABLE_CRED

```

#### ESP_BLE_MESH_TAG_USE_DIRECTED


```


ESP_BLE_MESH_TAG_USE_DIRECTED

```

#### ESP_BLE_MESH_TAG_RELAY


```


ESP_BLE_MESH_TAG_RELAY

```

#### ESP_BLE_MESH_TAG_FRIENDSHIP


```


ESP_BLE_MESH_TAG_FRIENDSHIP

```

#### ESP_BLE_MESH_SEG_SZMIC_SHORT


```


ESP_BLE_MESH_SEG_SZMIC_SHORT

```

#### ESP_BLE_MESH_SEG_SZMIC_LONG


```


ESP_BLE_MESH_SEG_SZMIC_LONG

```

#### ESP_BLE_MESH_MODEL_PUB_DEFINE


```


ESP_BLE_MESH_MODEL_PUB_DEFINE(_name, _msg_len, _role)
Define a model publication context.

Parameters

_name -- Variable name given to the context. 
_msg_len -- Length of the publication message. 
_role -- Role of the device which contains the model. 




```

#### ESP_BLE_MESH_MODEL_OP


```


ESP_BLE_MESH_MODEL_OP(_opcode, _min_len)
Define a model operation context.

Parameters

_opcode -- Message opcode. 
_min_len -- Message minimum length. 




```

#### ESP_BLE_MESH_MODEL_OP_END


```


ESP_BLE_MESH_MODEL_OP_END
Define the terminator for the model operation table. Each model operation struct array must use this terminator as the end tag of the operation unit. 

```

#### ESP_BLE_MESH_MODEL_NONE


```


ESP_BLE_MESH_MODEL_NONE
Helper to define an empty model array. This structure is associated with BLE_MESH_MODEL_NONE in mesh_access.h 

```

#### ADD_DEV_RM_AFTER_PROV_FLAG


```


ADD_DEV_RM_AFTER_PROV_FLAG
Device will be removed from queue after provisioned successfully 

```

#### ADD_DEV_START_PROV_NOW_FLAG


```


ADD_DEV_START_PROV_NOW_FLAG
Start provisioning device immediately 

```

#### ADD_DEV_FLUSHABLE_DEV_FLAG


```


ADD_DEV_FLUSHABLE_DEV_FLAG
Device can be remove when queue is full and new device is going to added 

```

#### DEL_DEV_ADDR_FLAG


```


DEL_DEV_ADDR_FLAG

```

#### DEL_DEV_UUID_FLAG


```


DEL_DEV_UUID_FLAG

```

#### PROV_DATA_NET_IDX_FLAG


```


PROV_DATA_NET_IDX_FLAG

```

#### PROV_DATA_FLAGS_FLAG


```


PROV_DATA_FLAGS_FLAG

```

#### PROV_DATA_IV_INDEX_FLAG


```


PROV_DATA_IV_INDEX_FLAG

```

#### ESP_BLE_MESH_HEARTBEAT_FILTER_ACCEPTLIST


```


ESP_BLE_MESH_HEARTBEAT_FILTER_ACCEPTLIST

```

#### ESP_BLE_MESH_HEARTBEAT_FILTER_REJECTLIST


```


ESP_BLE_MESH_HEARTBEAT_FILTER_REJECTLIST
Provisioner heartbeat filter operation 

```

#### ESP_BLE_MESH_HEARTBEAT_FILTER_ADD


```


ESP_BLE_MESH_HEARTBEAT_FILTER_ADD

```

#### ESP_BLE_MESH_HEARTBEAT_FILTER_REMOVE


```


ESP_BLE_MESH_HEARTBEAT_FILTER_REMOVE

```

#### ESP_BLE_MESH_MODEL_ID_CONFIG_SRV


```


ESP_BLE_MESH_MODEL_ID_CONFIG_SRV
BLE Mesh models related Model ID and Opcode definitions. 
< Foundation Models 

```

#### ESP_BLE_MESH_MODEL_ID_CONFIG_CLI


```


ESP_BLE_MESH_MODEL_ID_CONFIG_CLI

```

#### ESP_BLE_MESH_MODEL_ID_HEALTH_SRV


```


ESP_BLE_MESH_MODEL_ID_HEALTH_SRV

```

#### ESP_BLE_MESH_MODEL_ID_HEALTH_CLI


```


ESP_BLE_MESH_MODEL_ID_HEALTH_CLI

```

#### ESP_BLE_MESH_MODEL_ID_RPR_SRV


```


ESP_BLE_MESH_MODEL_ID_RPR_SRV

```

#### ESP_BLE_MESH_MODEL_ID_RPR_CLI


```


ESP_BLE_MESH_MODEL_ID_RPR_CLI

```

#### ESP_BLE_MESH_MODEL_ID_DF_SRV


```


ESP_BLE_MESH_MODEL_ID_DF_SRV

```

#### ESP_BLE_MESH_MODEL_ID_DF_CLI


```


ESP_BLE_MESH_MODEL_ID_DF_CLI

```

#### ESP_BLE_MESH_MODEL_ID_BRC_SRV


```


ESP_BLE_MESH_MODEL_ID_BRC_SRV

```

#### ESP_BLE_MESH_MODEL_ID_BRC_CLI


```


ESP_BLE_MESH_MODEL_ID_BRC_CLI

```

#### ESP_BLE_MESH_MODEL_ID_PRB_SRV


```


ESP_BLE_MESH_MODEL_ID_PRB_SRV

```

#### ESP_BLE_MESH_MODEL_ID_PRB_CLI


```


ESP_BLE_MESH_MODEL_ID_PRB_CLI

```

#### ESP_BLE_MESH_MODEL_ID_ODP_SRV


```


ESP_BLE_MESH_MODEL_ID_ODP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_ODP_CLI


```


ESP_BLE_MESH_MODEL_ID_ODP_CLI

```

#### ESP_BLE_MESH_MODEL_ID_SAR_SRV


```


ESP_BLE_MESH_MODEL_ID_SAR_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SAR_CLI


```


ESP_BLE_MESH_MODEL_ID_SAR_CLI

```

#### ESP_BLE_MESH_MODEL_ID_AGG_SRV


```


ESP_BLE_MESH_MODEL_ID_AGG_SRV

```

#### ESP_BLE_MESH_MODEL_ID_AGG_CLI


```


ESP_BLE_MESH_MODEL_ID_AGG_CLI

```

#### ESP_BLE_MESH_MODEL_ID_LCD_SRV


```


ESP_BLE_MESH_MODEL_ID_LCD_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LCD_CLI


```


ESP_BLE_MESH_MODEL_ID_LCD_CLI

```

#### ESP_BLE_MESH_MODEL_ID_SRPL_SRV


```


ESP_BLE_MESH_MODEL_ID_SRPL_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SRPL_CLI


```


ESP_BLE_MESH_MODEL_ID_SRPL_CLI
Models from the Mesh Model Specification 

```

#### ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_CLI

```

#### ESP_BLE_MESH_MODEL_ID_GEN_LEVEL_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_LEVEL_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_LEVEL_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_LEVEL_CLI

```

#### ESP_BLE_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_CLI

```

#### ESP_BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_CLI

```

#### ESP_BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_CLI

```

#### ESP_BLE_MESH_MODEL_ID_GEN_BATTERY_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_BATTERY_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_BATTERY_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_BATTERY_CLI

```

#### ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_CLI

```

#### ESP_BLE_MESH_MODEL_ID_GEN_ADMIN_PROP_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_ADMIN_PROP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_MANUFACTURER_PROP_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_MANUFACTURER_PROP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_USER_PROP_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_USER_PROP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_CLIENT_PROP_SRV


```


ESP_BLE_MESH_MODEL_ID_GEN_CLIENT_PROP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_GEN_PROP_CLI


```


ESP_BLE_MESH_MODEL_ID_GEN_PROP_CLI

```

#### ESP_BLE_MESH_MODEL_ID_SENSOR_SRV


```


ESP_BLE_MESH_MODEL_ID_SENSOR_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SENSOR_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_SENSOR_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SENSOR_CLI


```


ESP_BLE_MESH_MODEL_ID_SENSOR_CLI

```

#### ESP_BLE_MESH_MODEL_ID_TIME_SRV


```


ESP_BLE_MESH_MODEL_ID_TIME_SRV

```

#### ESP_BLE_MESH_MODEL_ID_TIME_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_TIME_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_TIME_CLI


```


ESP_BLE_MESH_MODEL_ID_TIME_CLI

```

#### ESP_BLE_MESH_MODEL_ID_SCENE_SRV


```


ESP_BLE_MESH_MODEL_ID_SCENE_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SCENE_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_SCENE_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SCENE_CLI


```


ESP_BLE_MESH_MODEL_ID_SCENE_CLI

```

#### ESP_BLE_MESH_MODEL_ID_SCHEDULER_SRV


```


ESP_BLE_MESH_MODEL_ID_SCHEDULER_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SCHEDULER_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_SCHEDULER_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_SCHEDULER_CLI


```


ESP_BLE_MESH_MODEL_ID_SCHEDULER_CLI

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI


```


ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_CLI


```


ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_CLI

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_TEMP_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_CTL_TEMP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_CLI


```


ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_CLI

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_XYL_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_XYL_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_XYL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_XYL_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_XYL_CLI


```


ESP_BLE_MESH_MODEL_ID_LIGHT_XYL_CLI

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_LC_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_LC_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_LC_SETUP_SRV


```


ESP_BLE_MESH_MODEL_ID_LIGHT_LC_SETUP_SRV

```

#### ESP_BLE_MESH_MODEL_ID_LIGHT_LC_CLI


```


ESP_BLE_MESH_MODEL_ID_LIGHT_LC_CLI

```

#### ESP_BLE_MESH_MODEL_ID_MBT_SRV


```


ESP_BLE_MESH_MODEL_ID_MBT_SRV

```

#### ESP_BLE_MESH_MODEL_ID_MBT_CLI


```


ESP_BLE_MESH_MODEL_ID_MBT_CLI

```

#### ESP_BLE_MESH_MODEL_OP_BEACON_GET


```


ESP_BLE_MESH_MODEL_OP_BEACON_GET
Config Beacon Get 

```

#### ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET


```


ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET
Config Composition Data Get 

```

#### ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_GET


```


ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_GET
Config Default TTL Get 

```

#### ESP_BLE_MESH_MODEL_OP_GATT_PROXY_GET


```


ESP_BLE_MESH_MODEL_OP_GATT_PROXY_GET
Config GATT Proxy Get 

```

#### ESP_BLE_MESH_MODEL_OP_RELAY_GET


```


ESP_BLE_MESH_MODEL_OP_RELAY_GET
Config Relay Get 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_PUB_GET


```


ESP_BLE_MESH_MODEL_OP_MODEL_PUB_GET
Config Model Publication Get 

```

#### ESP_BLE_MESH_MODEL_OP_FRIEND_GET


```


ESP_BLE_MESH_MODEL_OP_FRIEND_GET
Config Friend Get 

```

#### ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_GET


```


ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_GET
Config Heartbeat Publication Get 

```

#### ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_GET


```


ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_GET
Config Heartbeat Subscription Get 

```

#### ESP_BLE_MESH_MODEL_OP_NET_KEY_GET


```


ESP_BLE_MESH_MODEL_OP_NET_KEY_GET
Config NetKey Get 

```

#### ESP_BLE_MESH_MODEL_OP_APP_KEY_GET


```


ESP_BLE_MESH_MODEL_OP_APP_KEY_GET
Config AppKey Get 

```

#### ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_GET


```


ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_GET
Config Node Identity Get 

```

#### ESP_BLE_MESH_MODEL_OP_SIG_MODEL_SUB_GET


```


ESP_BLE_MESH_MODEL_OP_SIG_MODEL_SUB_GET
Config SIG Model Subscription Get 

```

#### ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_SUB_GET


```


ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_SUB_GET
Config Vendor Model Subscription Get 

```

#### ESP_BLE_MESH_MODEL_OP_SIG_MODEL_APP_GET


```


ESP_BLE_MESH_MODEL_OP_SIG_MODEL_APP_GET
Config SIG Model App Get 

```

#### ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_APP_GET


```


ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_APP_GET
Config Vendor Model App Get 

```

#### ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_GET


```


ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_GET
Config Key Refresh Phase Get 

```

#### ESP_BLE_MESH_MODEL_OP_LPN_POLLTIMEOUT_GET


```


ESP_BLE_MESH_MODEL_OP_LPN_POLLTIMEOUT_GET
Config Low Power Node PollTimeout Get 

```

#### ESP_BLE_MESH_MODEL_OP_NETWORK_TRANSMIT_GET


```


ESP_BLE_MESH_MODEL_OP_NETWORK_TRANSMIT_GET
Config Network Transmit Get 

```

#### ESP_BLE_MESH_MODEL_OP_BEACON_SET


```


ESP_BLE_MESH_MODEL_OP_BEACON_SET
Config Beacon Set 

```

#### ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_SET


```


ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_SET
Config Default TTL Set 

```

#### ESP_BLE_MESH_MODEL_OP_GATT_PROXY_SET


```


ESP_BLE_MESH_MODEL_OP_GATT_PROXY_SET
Config GATT Proxy Set 

```

#### ESP_BLE_MESH_MODEL_OP_RELAY_SET


```


ESP_BLE_MESH_MODEL_OP_RELAY_SET
Config Relay Set 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET


```


ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET
Config Model Publication Set 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD
Config Model Subscription Add 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_ADD


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_ADD
Config Model Subscription Virtual Address Add 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE
Config Model Subscription Delete 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_DELETE


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_DELETE
Config Model Subscription Virtual Address Delete 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_OVERWRITE


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_OVERWRITE
Config Model Subscription Overwrite 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_OVERWRITE


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_OVERWRITE
Config Model Subscription Virtual Address Overwrite 

```

#### ESP_BLE_MESH_MODEL_OP_NET_KEY_ADD


```


ESP_BLE_MESH_MODEL_OP_NET_KEY_ADD
Config NetKey Add 

```

#### ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD


```


ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD
Config AppKey Add 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND


```


ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND
Config Model App Bind 

```

#### ESP_BLE_MESH_MODEL_OP_NODE_RESET


```


ESP_BLE_MESH_MODEL_OP_NODE_RESET
Config Node Reset 

```

#### ESP_BLE_MESH_MODEL_OP_FRIEND_SET


```


ESP_BLE_MESH_MODEL_OP_FRIEND_SET
Config Friend Set 

```

#### ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_SET


```


ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_SET
Config Heartbeat Publication Set 

```

#### ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_SET


```


ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_SET
Config Heartbeat Subscription Set 

```

#### ESP_BLE_MESH_MODEL_OP_NET_KEY_UPDATE


```


ESP_BLE_MESH_MODEL_OP_NET_KEY_UPDATE
Config NetKey Update 

```

#### ESP_BLE_MESH_MODEL_OP_NET_KEY_DELETE


```


ESP_BLE_MESH_MODEL_OP_NET_KEY_DELETE
Config NetKey Delete 

```

#### ESP_BLE_MESH_MODEL_OP_APP_KEY_UPDATE


```


ESP_BLE_MESH_MODEL_OP_APP_KEY_UPDATE
Config AppKey Update 

```

#### ESP_BLE_MESH_MODEL_OP_APP_KEY_DELETE


```


ESP_BLE_MESH_MODEL_OP_APP_KEY_DELETE
Config AppKey Delete 

```

#### ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_SET


```


ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_SET
Config Node Identity Set 

```

#### ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_SET


```


ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_SET
Config Key Refresh Phase Set 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_PUB_VIRTUAL_ADDR_SET


```


ESP_BLE_MESH_MODEL_OP_MODEL_PUB_VIRTUAL_ADDR_SET
Config Model Publication Virtual Address Set 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE_ALL


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE_ALL
Config Model Subscription Delete All 

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_APP_UNBIND


```


ESP_BLE_MESH_MODEL_OP_MODEL_APP_UNBIND
Config Model App Unbind 

```

#### ESP_BLE_MESH_MODEL_OP_NETWORK_TRANSMIT_SET


```


ESP_BLE_MESH_MODEL_OP_NETWORK_TRANSMIT_SET
Config Network Transmit Set 

```

#### ESP_BLE_MESH_MODEL_OP_BEACON_STATUS


```


ESP_BLE_MESH_MODEL_OP_BEACON_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_STATUS


```


ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_STATUS


```


ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GATT_PROXY_STATUS


```


ESP_BLE_MESH_MODEL_OP_GATT_PROXY_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_RELAY_STATUS


```


ESP_BLE_MESH_MODEL_OP_RELAY_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_PUB_STATUS


```


ESP_BLE_MESH_MODEL_OP_MODEL_PUB_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_SUB_STATUS


```


ESP_BLE_MESH_MODEL_OP_MODEL_SUB_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SIG_MODEL_SUB_LIST


```


ESP_BLE_MESH_MODEL_OP_SIG_MODEL_SUB_LIST

```

#### ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_SUB_LIST


```


ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_SUB_LIST

```

#### ESP_BLE_MESH_MODEL_OP_NET_KEY_STATUS


```


ESP_BLE_MESH_MODEL_OP_NET_KEY_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_NET_KEY_LIST


```


ESP_BLE_MESH_MODEL_OP_NET_KEY_LIST

```

#### ESP_BLE_MESH_MODEL_OP_APP_KEY_STATUS


```


ESP_BLE_MESH_MODEL_OP_APP_KEY_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_APP_KEY_LIST


```


ESP_BLE_MESH_MODEL_OP_APP_KEY_LIST

```

#### ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_STATUS


```


ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_MODEL_APP_STATUS


```


ESP_BLE_MESH_MODEL_OP_MODEL_APP_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SIG_MODEL_APP_LIST


```


ESP_BLE_MESH_MODEL_OP_SIG_MODEL_APP_LIST

```

#### ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_APP_LIST


```


ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_APP_LIST

```

#### ESP_BLE_MESH_MODEL_OP_NODE_RESET_STATUS


```


ESP_BLE_MESH_MODEL_OP_NODE_RESET_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_FRIEND_STATUS


```


ESP_BLE_MESH_MODEL_OP_FRIEND_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_STATUS


```


ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_STATUS


```


ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_STATUS


```


ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LPN_POLLTIMEOUT_STATUS


```


ESP_BLE_MESH_MODEL_OP_LPN_POLLTIMEOUT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_NETWORK_TRANSMIT_STATUS


```


ESP_BLE_MESH_MODEL_OP_NETWORK_TRANSMIT_STATUS

```

#### ESP_BLE_MESH_CFG_STATUS_SUCCESS


```


ESP_BLE_MESH_CFG_STATUS_SUCCESS

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_ADDRESS


```


ESP_BLE_MESH_CFG_STATUS_INVALID_ADDRESS

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_MODEL


```


ESP_BLE_MESH_CFG_STATUS_INVALID_MODEL

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_APPKEY


```


ESP_BLE_MESH_CFG_STATUS_INVALID_APPKEY

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_NETKEY


```


ESP_BLE_MESH_CFG_STATUS_INVALID_NETKEY

```

#### ESP_BLE_MESH_CFG_STATUS_INSUFFICIENT_RESOURCES


```


ESP_BLE_MESH_CFG_STATUS_INSUFFICIENT_RESOURCES

```

#### ESP_BLE_MESH_CFG_STATUS_KEY_INDEX_ALREADY_STORED


```


ESP_BLE_MESH_CFG_STATUS_KEY_INDEX_ALREADY_STORED

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_PUBLISH_PARAMETERS


```


ESP_BLE_MESH_CFG_STATUS_INVALID_PUBLISH_PARAMETERS

```

#### ESP_BLE_MESH_CFG_STATUS_NOT_A_SUBSCRIBE_MODEL


```


ESP_BLE_MESH_CFG_STATUS_NOT_A_SUBSCRIBE_MODEL

```

#### ESP_BLE_MESH_CFG_STATUS_STORAGE_FAILURE


```


ESP_BLE_MESH_CFG_STATUS_STORAGE_FAILURE

```

#### ESP_BLE_MESH_CFG_STATUS_FEATURE_NOT_SUPPORTED


```


ESP_BLE_MESH_CFG_STATUS_FEATURE_NOT_SUPPORTED

```

#### ESP_BLE_MESH_CFG_STATUS_CANNOT_UPDATE


```


ESP_BLE_MESH_CFG_STATUS_CANNOT_UPDATE

```

#### ESP_BLE_MESH_CFG_STATUS_CANNOT_REMOVE


```


ESP_BLE_MESH_CFG_STATUS_CANNOT_REMOVE

```

#### ESP_BLE_MESH_CFG_STATUS_CANNOT_BIND


```


ESP_BLE_MESH_CFG_STATUS_CANNOT_BIND

```

#### ESP_BLE_MESH_CFG_STATUS_TEMP_UNABLE_TO_CHANGE_STATE


```


ESP_BLE_MESH_CFG_STATUS_TEMP_UNABLE_TO_CHANGE_STATE

```

#### ESP_BLE_MESH_CFG_STATUS_CANNOT_SET


```


ESP_BLE_MESH_CFG_STATUS_CANNOT_SET

```

#### ESP_BLE_MESH_CFG_STATUS_UNSPECIFIED_ERROR


```


ESP_BLE_MESH_CFG_STATUS_UNSPECIFIED_ERROR

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_BINDING


```


ESP_BLE_MESH_CFG_STATUS_INVALID_BINDING

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_PATH_ENTRY


```


ESP_BLE_MESH_CFG_STATUS_INVALID_PATH_ENTRY

```

#### ESP_BLE_MESH_CFG_STATUS_CANNOT_GET


```


ESP_BLE_MESH_CFG_STATUS_CANNOT_GET

```

#### ESP_BLE_MESH_CFG_STATUS_OBSOLETE_INFO


```


ESP_BLE_MESH_CFG_STATUS_OBSOLETE_INFO

```

#### ESP_BLE_MESH_CFG_STATUS_INVALID_BEARER


```


ESP_BLE_MESH_CFG_STATUS_INVALID_BEARER

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_GET


```


ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_GET
Health Fault Get 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_GET


```


ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_GET
Health Period Get 

```

#### ESP_BLE_MESH_MODEL_OP_ATTENTION_GET


```


ESP_BLE_MESH_MODEL_OP_ATTENTION_GET
Health Attention Get 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR


```


ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR
Health Fault Clear 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR_UNACK


```


ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR_UNACK
Health Fault Clear Unacknowledged 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST


```


ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST
Health Fault Test 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST_UNACK


```


ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST_UNACK
Health Fault Test Unacknowledged 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET


```


ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET
Health Period Set 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET_UNACK
Health Period Set Unacknowledged 

```

#### ESP_BLE_MESH_MODEL_OP_ATTENTION_SET


```


ESP_BLE_MESH_MODEL_OP_ATTENTION_SET
Health Attention Set 

```

#### ESP_BLE_MESH_MODEL_OP_ATTENTION_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_ATTENTION_SET_UNACK
Health Attention Set Unacknowledged 

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_CURRENT_STATUS


```


ESP_BLE_MESH_MODEL_OP_HEALTH_CURRENT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_STATUS


```


ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_STATUS


```


ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_ATTENTION_STATUS


```


ESP_BLE_MESH_MODEL_OP_ATTENTION_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS
Generic Level Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET_UNACK
Generic Default Transition Time Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_STATUS
Generic Power OnOff Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_STATUS
Generic Power OnOff Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET_UNACK
Generic Power Level Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_LAST_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_LAST_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_LAST_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_LAST_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_STATUS
Generic Power Level Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET_UNACK
Generic Battery Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_BATTERY_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_BATTERY_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_BATTERY_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_BATTERY_STATUS
Generic Location Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_STATUS
Generic Location Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET_UNACK
Generic Manufacturer Property Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTIES_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTIES_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTIES_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTIES_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_STATUS
Generic Admin Property Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_STATUS
Generic User Property Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET


```


ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_STATUS
Generic Client Property Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_GET


```


ESP_BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_GET

```

#### ESP_BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_STATUS


```


ESP_BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS


```


ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_GET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_GET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_STATUS


```


ESP_BLE_MESH_MODEL_OP_SENSOR_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_COLUMN_GET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_COLUMN_GET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_COLUMN_STATUS


```


ESP_BLE_MESH_MODEL_OP_SENSOR_COLUMN_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_GET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_GET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_STATUS


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_STATUS
Sensor Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_GET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_GET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_STATUS


```


ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_GET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_GET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_GET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_GET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_STATUS


```


ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_TIME_GET


```


ESP_BLE_MESH_MODEL_OP_TIME_GET

```

#### ESP_BLE_MESH_MODEL_OP_TIME_SET


```


ESP_BLE_MESH_MODEL_OP_TIME_SET

```

#### ESP_BLE_MESH_MODEL_OP_TIME_STATUS


```


ESP_BLE_MESH_MODEL_OP_TIME_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_TIME_ROLE_GET


```


ESP_BLE_MESH_MODEL_OP_TIME_ROLE_GET

```

#### ESP_BLE_MESH_MODEL_OP_TIME_ROLE_SET


```


ESP_BLE_MESH_MODEL_OP_TIME_ROLE_SET

```

#### ESP_BLE_MESH_MODEL_OP_TIME_ROLE_STATUS


```


ESP_BLE_MESH_MODEL_OP_TIME_ROLE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_TIME_ZONE_GET


```


ESP_BLE_MESH_MODEL_OP_TIME_ZONE_GET

```

#### ESP_BLE_MESH_MODEL_OP_TIME_ZONE_SET


```


ESP_BLE_MESH_MODEL_OP_TIME_ZONE_SET

```

#### ESP_BLE_MESH_MODEL_OP_TIME_ZONE_STATUS


```


ESP_BLE_MESH_MODEL_OP_TIME_ZONE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_GET


```


ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_GET

```

#### ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_SET


```


ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_SET

```

#### ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_STATUS


```


ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_STATUS
Scene Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_GET


```


ESP_BLE_MESH_MODEL_OP_SCENE_GET

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_RECALL


```


ESP_BLE_MESH_MODEL_OP_SCENE_RECALL

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_RECALL_UNACK


```


ESP_BLE_MESH_MODEL_OP_SCENE_RECALL_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_STATUS


```


ESP_BLE_MESH_MODEL_OP_SCENE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_REGISTER_GET


```


ESP_BLE_MESH_MODEL_OP_SCENE_REGISTER_GET

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_REGISTER_STATUS


```


ESP_BLE_MESH_MODEL_OP_SCENE_REGISTER_STATUS
Scene Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_STORE


```


ESP_BLE_MESH_MODEL_OP_SCENE_STORE

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_STORE_UNACK


```


ESP_BLE_MESH_MODEL_OP_SCENE_STORE_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_DELETE


```


ESP_BLE_MESH_MODEL_OP_SCENE_DELETE

```

#### ESP_BLE_MESH_MODEL_OP_SCENE_DELETE_UNACK


```


ESP_BLE_MESH_MODEL_OP_SCENE_DELETE_UNACK
Scheduler Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_GET


```


ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_GET

```

#### ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_STATUS


```


ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SCHEDULER_GET


```


ESP_BLE_MESH_MODEL_OP_SCHEDULER_GET

```

#### ESP_BLE_MESH_MODEL_OP_SCHEDULER_STATUS


```


ESP_BLE_MESH_MODEL_OP_SCHEDULER_STATUS
Scheduler Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET


```


ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET

```

#### ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_STATUS
Light Lightness Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET_UNACK
Light CTL Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_STATUS
Light CTL Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK
Light HSL Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_STATUS
Light HSL Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET_UNACK
Light xyL Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_STATUS
Light xyL Setup Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET_UNACK
Light Control Message Opcode 

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_GET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_GET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET_UNACK


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_STATUS


```


ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_STATUS

```

#### ESP_BLE_MESH_MODEL_STATUS_SUCCESS


```


ESP_BLE_MESH_MODEL_STATUS_SUCCESS

```

#### ESP_BLE_MESH_MODEL_STATUS_CANNOT_SET_RANGE_MIN


```


ESP_BLE_MESH_MODEL_STATUS_CANNOT_SET_RANGE_MIN

```

#### ESP_BLE_MESH_MODEL_STATUS_CANNOT_SET_RANGE_MAX


```


ESP_BLE_MESH_MODEL_STATUS_CANNOT_SET_RANGE_MAX

```

#### ESP_BLE_MESH_SERVER_RSP_BY_APP


```


ESP_BLE_MESH_SERVER_RSP_BY_APP
Response need to be sent in the application 

```

#### ESP_BLE_MESH_SERVER_AUTO_RSP


```


ESP_BLE_MESH_SERVER_AUTO_RSP
Response will be sent internally 

```



### Type Definitions


#### esp_ble_mesh_octet16_t


```


typedef uint8_t esp_ble_mesh_octet16_t[ESP_BLE_MESH_OCTET16_LEN]
Define the BLE Mesh octet 8 bytes size 

```

#### esp_ble_mesh_octet8_t


```


typedef uint8_t esp_ble_mesh_octet8_t[ESP_BLE_MESH_OCTET8_LEN]
Invalid Company ID 

```

#### esp_ble_mesh_cb_t


```


typedef uint32_t esp_ble_mesh_cb_t

```

#### UINT8


```


typedef uint8_t UINT8

```

#### UINT16


```


typedef uint16_t UINT16

```

#### UINT32


```


typedef uint32_t UINT32

```

#### UINT64


```


typedef uint64_t UINT64

```

#### BT_OCTET32


```


typedef UINT8 BT_OCTET32[BT_OCTET32_LEN]

```

#### BD_ADDR


```


typedef uint8_t BD_ADDR[BD_ADDR_LEN]

```

#### esp_ble_mesh_bd_addr_t


```


typedef uint8_t esp_ble_mesh_bd_addr_t[BD_ADDR_LEN]

```

#### esp_ble_mesh_addr_type_t


```


typedef uint8_t esp_ble_mesh_addr_type_t
BLE device address type. 

```

#### esp_ble_mesh_model_t


```


typedef struct esp_ble_mesh_model esp_ble_mesh_model_t

```

#### esp_ble_mesh_dev_add_flag_t


```


typedef uint8_t esp_ble_mesh_dev_add_flag_t

```

#### esp_ble_mesh_opcode_config_client_get_t


```


typedef uint32_t esp_ble_mesh_opcode_config_client_get_t
esp_ble_mesh_opcode_config_client_get_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by esp_ble_mesh_config_client_get_state. The following opcodes will only be used in the esp_ble_mesh_config_client_get_state function. 

```

#### esp_ble_mesh_opcode_config_client_set_t


```


typedef uint32_t esp_ble_mesh_opcode_config_client_set_t
esp_ble_mesh_opcode_config_client_set_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by esp_ble_mesh_config_client_set_state. The following opcodes will only be used in the esp_ble_mesh_config_client_set_state function. 

```

#### esp_ble_mesh_opcode_config_status_t


```


typedef uint32_t esp_ble_mesh_opcode_config_status_t
esp_ble_mesh_opcode_config_status_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by the Config Model messages The following opcodes are used by the BLE Mesh Config Server Model internally to respond to the Config Client Model's request messages. 

```

#### esp_ble_mesh_cfg_status_t


```


typedef uint8_t esp_ble_mesh_cfg_status_t
This typedef is only used to indicate the status code contained in some of the Configuration Server Model status message. 

```

#### esp_ble_mesh_opcode_health_client_get_t


```


typedef uint32_t esp_ble_mesh_opcode_health_client_get_t
esp_ble_mesh_opcode_health_client_get_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by esp_ble_mesh_health_client_get_state. The following opcodes will only be used in the esp_ble_mesh_health_client_get_state function. 

```

#### esp_ble_mesh_opcode_health_client_set_t


```


typedef uint32_t esp_ble_mesh_opcode_health_client_set_t
esp_ble_mesh_opcode_health_client_set_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by esp_ble_mesh_health_client_set_state. The following opcodes will only be used in the esp_ble_mesh_health_client_set_state function. 

```

#### esp_ble_mesh_health_model_status_t


```


typedef uint32_t esp_ble_mesh_health_model_status_t
esp_ble_mesh_health_model_status_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by the Health Model messages. The following opcodes are used by the BLE Mesh Health Server Model internally to respond to the Health Client Model's request messages. 

```

#### esp_ble_mesh_generic_message_opcode_t


```


typedef uint32_t esp_ble_mesh_generic_message_opcode_t
esp_ble_mesh_generic_message_opcode_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by functions esp_ble_mesh_generic_client_get_state & esp_ble_mesh_generic_client_set_state. Generic OnOff Message Opcode 

```

#### esp_ble_mesh_sensor_message_opcode_t


```


typedef uint32_t esp_ble_mesh_sensor_message_opcode_t
esp_ble_mesh_sensor_message_opcode_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by functions esp_ble_mesh_sensor_client_get_state & esp_ble_mesh_sensor_client_set_state. Sensor Message Opcode 

```

#### esp_ble_mesh_time_scene_message_opcode_t


```


typedef uint32_t esp_ble_mesh_time_scene_message_opcode_t
esp_ble_mesh_time_scene_message_opcode_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by functions esp_ble_mesh_time_scene_client_get_state & esp_ble_mesh_time_scene_client_set_state. Time Message Opcode 

```

#### esp_ble_mesh_light_message_opcode_t


```


typedef uint32_t esp_ble_mesh_light_message_opcode_t
esp_ble_mesh_light_message_opcode_t belongs to esp_ble_mesh_opcode_t, this typedef is only used to locate the opcodes used by functions esp_ble_mesh_light_client_get_state & esp_ble_mesh_light_client_set_state. Light Lightness Message Opcode 

```

#### esp_ble_mesh_opcode_t


```


typedef uint32_t esp_ble_mesh_opcode_t
End of defines of esp_ble_mesh_opcode_t 

```

#### esp_ble_mesh_model_status_t


```


typedef uint8_t esp_ble_mesh_model_status_t
This typedef is only used to indicate the status code contained in some of the server models (e.g. Generic Server Model) status message. 

```



### Enumerations


#### esp_ble_mesh_cb_type_t


```


enum esp_ble_mesh_cb_type_t
Values:
ESP_BLE_MESH_TYPE_PROV_CB

enumerator ESP_BLE_MESH_TYPE_PROV_CB

ESP_BLE_MESH_TYPE_OUTPUT_NUM_CB

enumerator ESP_BLE_MESH_TYPE_OUTPUT_NUM_CB

ESP_BLE_MESH_TYPE_OUTPUT_STR_CB

enumerator ESP_BLE_MESH_TYPE_OUTPUT_STR_CB

ESP_BLE_MESH_TYPE_INTPUT_CB

enumerator ESP_BLE_MESH_TYPE_INTPUT_CB

ESP_BLE_MESH_TYPE_LINK_OPEN_CB

enumerator ESP_BLE_MESH_TYPE_LINK_OPEN_CB

ESP_BLE_MESH_TYPE_LINK_CLOSE_CB

enumerator ESP_BLE_MESH_TYPE_LINK_CLOSE_CB

ESP_BLE_MESH_TYPE_COMPLETE_CB

enumerator ESP_BLE_MESH_TYPE_COMPLETE_CB

ESP_BLE_MESH_TYPE_RESET_CB

enumerator ESP_BLE_MESH_TYPE_RESET_CB


```

#### esp_ble_mesh_oob_method_t


```


enum esp_ble_mesh_oob_method_t
Values:
ESP_BLE_MESH_NO_OOB

enumerator ESP_BLE_MESH_NO_OOB

ESP_BLE_MESH_STATIC_OOB

enumerator ESP_BLE_MESH_STATIC_OOB

ESP_BLE_MESH_OUTPUT_OOB

enumerator ESP_BLE_MESH_OUTPUT_OOB

ESP_BLE_MESH_INPUT_OOB

enumerator ESP_BLE_MESH_INPUT_OOB


```

#### esp_ble_mesh_output_action_t


```


enum esp_ble_mesh_output_action_t
Values:
ESP_BLE_MESH_NO_OUTPUT

enumerator ESP_BLE_MESH_NO_OUTPUT

ESP_BLE_MESH_BLINK

enumerator ESP_BLE_MESH_BLINK

ESP_BLE_MESH_BEEP

enumerator ESP_BLE_MESH_BEEP

ESP_BLE_MESH_VIBRATE

enumerator ESP_BLE_MESH_VIBRATE

ESP_BLE_MESH_DISPLAY_NUMBER

enumerator ESP_BLE_MESH_DISPLAY_NUMBER

ESP_BLE_MESH_DISPLAY_STRING

enumerator ESP_BLE_MESH_DISPLAY_STRING


```

#### esp_ble_mesh_input_action_t


```


enum esp_ble_mesh_input_action_t
Values:
ESP_BLE_MESH_NO_INPUT

enumerator ESP_BLE_MESH_NO_INPUT

ESP_BLE_MESH_PUSH

enumerator ESP_BLE_MESH_PUSH

ESP_BLE_MESH_TWIST

enumerator ESP_BLE_MESH_TWIST

ESP_BLE_MESH_ENTER_NUMBER

enumerator ESP_BLE_MESH_ENTER_NUMBER

ESP_BLE_MESH_ENTER_STRING

enumerator ESP_BLE_MESH_ENTER_STRING


```

#### esp_ble_mesh_prov_bearer_t


```


enum esp_ble_mesh_prov_bearer_t
Values:
ESP_BLE_MESH_PROV_ADV

enumerator ESP_BLE_MESH_PROV_ADV

ESP_BLE_MESH_PROV_GATT

enumerator ESP_BLE_MESH_PROV_GATT


```

#### esp_ble_mesh_prov_oob_info_t


```


enum esp_ble_mesh_prov_oob_info_t
Values:
ESP_BLE_MESH_PROV_OOB_OTHER

enumerator ESP_BLE_MESH_PROV_OOB_OTHER

ESP_BLE_MESH_PROV_OOB_URI

enumerator ESP_BLE_MESH_PROV_OOB_URI

ESP_BLE_MESH_PROV_OOB_2D_CODE

enumerator ESP_BLE_MESH_PROV_OOB_2D_CODE

ESP_BLE_MESH_PROV_OOB_BAR_CODE

enumerator ESP_BLE_MESH_PROV_OOB_BAR_CODE

ESP_BLE_MESH_PROV_OOB_NFC

enumerator ESP_BLE_MESH_PROV_OOB_NFC

ESP_BLE_MESH_PROV_OOB_NUMBER

enumerator ESP_BLE_MESH_PROV_OOB_NUMBER

ESP_BLE_MESH_PROV_OOB_STRING

enumerator ESP_BLE_MESH_PROV_OOB_STRING

ESP_BLE_MESH_PROV_CERT_BASED

enumerator ESP_BLE_MESH_PROV_CERT_BASED

ESP_BLE_MESH_PROV_RECORDS

enumerator ESP_BLE_MESH_PROV_RECORDS

ESP_BLE_MESH_PROV_OOB_ON_BOX

enumerator ESP_BLE_MESH_PROV_OOB_ON_BOX

ESP_BLE_MESH_PROV_OOB_IN_BOX

enumerator ESP_BLE_MESH_PROV_OOB_IN_BOX

ESP_BLE_MESH_PROV_OOB_ON_PAPER

enumerator ESP_BLE_MESH_PROV_OOB_ON_PAPER

ESP_BLE_MESH_PROV_OOB_IN_MANUAL

enumerator ESP_BLE_MESH_PROV_OOB_IN_MANUAL

ESP_BLE_MESH_PROV_OOB_ON_DEV

enumerator ESP_BLE_MESH_PROV_OOB_ON_DEV


```

#### esp_ble_mesh_dev_role_t


```


enum esp_ble_mesh_dev_role_t
Values:
ROLE_NODE

enumerator ROLE_NODE

ROLE_PROVISIONER

enumerator ROLE_PROVISIONER

ROLE_FAST_PROV

enumerator ROLE_FAST_PROV


```

#### esp_ble_mesh_fast_prov_action_t


```


enum esp_ble_mesh_fast_prov_action_t
Values:
FAST_PROV_ACT_NONE

enumerator FAST_PROV_ACT_NONE

FAST_PROV_ACT_ENTER

enumerator FAST_PROV_ACT_ENTER

FAST_PROV_ACT_SUSPEND

enumerator FAST_PROV_ACT_SUSPEND

FAST_PROV_ACT_EXIT

enumerator FAST_PROV_ACT_EXIT

FAST_PROV_ACT_MAX

enumerator FAST_PROV_ACT_MAX


```

#### esp_ble_mesh_proxy_filter_type_t


```


enum esp_ble_mesh_proxy_filter_type_t
Values:
PROXY_FILTER_WHITELIST

enumerator PROXY_FILTER_WHITELIST

PROXY_FILTER_BLACKLIST

enumerator PROXY_FILTER_BLACKLIST


```

#### esp_ble_mesh_prov_cb_event_t


```


enum esp_ble_mesh_prov_cb_event_t
Values:
ESP_BLE_MESH_PROV_REGISTER_COMP_EVT

enumerator ESP_BLE_MESH_PROV_REGISTER_COMP_EVT
Initialize BLE Mesh provisioning capabilities and internal data information completion event 

ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT

enumerator ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT
Set the unprovisioned device name completion event 

ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT
Enable node provisioning functionality completion event 

ESP_BLE_MESH_NODE_PROV_DISABLE_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROV_DISABLE_COMP_EVT
Disable node provisioning functionality completion event 

ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT

enumerator ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT
Establish a BLE Mesh link event 

ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT

enumerator ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT
Close a BLE Mesh link event 

ESP_BLE_MESH_NODE_PROV_OOB_PUB_KEY_EVT

enumerator ESP_BLE_MESH_NODE_PROV_OOB_PUB_KEY_EVT
Generate Node input OOB public key event 

ESP_BLE_MESH_NODE_PROV_OUTPUT_NUMBER_EVT

enumerator ESP_BLE_MESH_NODE_PROV_OUTPUT_NUMBER_EVT
Generate Node Output Number event 

ESP_BLE_MESH_NODE_PROV_OUTPUT_STRING_EVT

enumerator ESP_BLE_MESH_NODE_PROV_OUTPUT_STRING_EVT
Generate Node Output String event 

ESP_BLE_MESH_NODE_PROV_INPUT_EVT

enumerator ESP_BLE_MESH_NODE_PROV_INPUT_EVT
Event requiring the user to input a number or string 

ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT

enumerator ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT
Provisioning done event 

ESP_BLE_MESH_NODE_PROV_RESET_EVT

enumerator ESP_BLE_MESH_NODE_PROV_RESET_EVT
Provisioning reset event 

ESP_BLE_MESH_NODE_PROV_SET_OOB_PUB_KEY_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROV_SET_OOB_PUB_KEY_COMP_EVT
Node set oob public key completion event 

ESP_BLE_MESH_NODE_PROV_INPUT_NUMBER_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROV_INPUT_NUMBER_COMP_EVT
Node input number completion event 

ESP_BLE_MESH_NODE_PROV_INPUT_STRING_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROV_INPUT_STRING_COMP_EVT
Node input string completion event 

ESP_BLE_MESH_NODE_PROXY_IDENTITY_ENABLE_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROXY_IDENTITY_ENABLE_COMP_EVT
Enable BLE Mesh Proxy Identity advertising completion event 

ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_ENABLE_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_ENABLE_COMP_EVT
Enable BLE Mesh Private Proxy Identity advertising completion event 

ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_DISABLE_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PRIVATE_PROXY_IDENTITY_DISABLE_COMP_EVT
Disable BLE Mesh Private Proxy Identity advertising completion event 

ESP_BLE_MESH_NODE_PROXY_GATT_ENABLE_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROXY_GATT_ENABLE_COMP_EVT
Enable BLE Mesh GATT Proxy Service completion event 

ESP_BLE_MESH_NODE_PROXY_GATT_DISABLE_COMP_EVT

enumerator ESP_BLE_MESH_NODE_PROXY_GATT_DISABLE_COMP_EVT
Disable BLE Mesh GATT Proxy Service completion event 

ESP_BLE_MESH_NODE_ADD_LOCAL_NET_KEY_COMP_EVT

enumerator ESP_BLE_MESH_NODE_ADD_LOCAL_NET_KEY_COMP_EVT
Node add NetKey locally completion event 

ESP_BLE_MESH_NODE_ADD_LOCAL_APP_KEY_COMP_EVT

enumerator ESP_BLE_MESH_NODE_ADD_LOCAL_APP_KEY_COMP_EVT
Node add AppKey locally completion event 

ESP_BLE_MESH_NODE_BIND_APP_KEY_TO_MODEL_COMP_EVT

enumerator ESP_BLE_MESH_NODE_BIND_APP_KEY_TO_MODEL_COMP_EVT
Node bind AppKey to model locally completion event 

ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT
Provisioner enable provisioning functionality completion event 

ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT
Provisioner disable provisioning functionality completion event 

ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT

enumerator ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
Provisioner receives unprovisioned device beacon event 

ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_EVT
Provisioner read unprovisioned device OOB public key event 

ESP_BLE_MESH_PROVISIONER_PROV_INPUT_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_INPUT_EVT
Provisioner input value for provisioning procedure event 

ESP_BLE_MESH_PROVISIONER_PROV_OUTPUT_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_OUTPUT_EVT
Provisioner output value for provisioning procedure event 

ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT
Provisioner establish a BLE Mesh link event 

ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT
Provisioner close a BLE Mesh link event 

ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT
Provisioner provisioning done event 

ESP_BLE_MESH_PROVISIONER_CERT_BASED_PROV_START_EVT

enumerator ESP_BLE_MESH_PROVISIONER_CERT_BASED_PROV_START_EVT
Provisioner initiate a certificate based provisioning 

ESP_BLE_MESH_PROVISIONER_RECV_PROV_RECORDS_LIST_EVT

enumerator ESP_BLE_MESH_PROVISIONER_RECV_PROV_RECORDS_LIST_EVT
Provisioner receive provisioning records list event 

ESP_BLE_MESH_PROVISIONER_PROV_RECORD_RECV_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_RECORD_RECV_COMP_EVT
Provisioner receive provisioning record complete event 

ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORDS_GET_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORDS_GET_EVT
Provisioner send provisioning records get to device event 

ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORD_REQUEST_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SEND_PROV_RECORD_REQUEST_EVT
Provisioner send provisioning record request to device event 

ESP_BLE_MESH_PROVISIONER_SEND_PROV_INVITE_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SEND_PROV_INVITE_EVT
Provisioner send provisioning invite to device event 

ESP_BLE_MESH_PROVISIONER_SEND_LINK_CLOSE_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SEND_LINK_CLOSE_EVT
Provisioner send link close to device event 

ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT
Provisioner add a device to the list which contains devices that are waiting/going to be provisioned completion event 

ESP_BLE_MESH_PROVISIONER_PROV_DEV_WITH_ADDR_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_DEV_WITH_ADDR_COMP_EVT
Provisioner start to provision an unprovisioned device completion event 

ESP_BLE_MESH_PROVISIONER_DELETE_DEV_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_DELETE_DEV_COMP_EVT
Provisioner delete a device from the list, close provisioning link with the device completion event 

ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT
Provisioner set the value to be compared with part of the unprovisioned device UUID completion event 

ESP_BLE_MESH_PROVISIONER_SET_PROV_DATA_INFO_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SET_PROV_DATA_INFO_COMP_EVT
Provisioner set net_idx/flags/iv_index used for provisioning completion event 

ESP_BLE_MESH_PROVISIONER_SET_STATIC_OOB_VALUE_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SET_STATIC_OOB_VALUE_COMP_EVT
Provisioner set static oob value used for provisioning completion event 

ESP_BLE_MESH_PROVISIONER_SET_PRIMARY_ELEM_ADDR_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SET_PRIMARY_ELEM_ADDR_COMP_EVT
Provisioner set unicast address of primary element completion event 

ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_READ_OOB_PUB_KEY_COMP_EVT
Provisioner read unprovisioned device OOB public key completion event 

ESP_BLE_MESH_PROVISIONER_PROV_INPUT_NUMBER_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_INPUT_NUMBER_COMP_EVT
Provisioner input number completion event 

ESP_BLE_MESH_PROVISIONER_PROV_INPUT_STRING_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_PROV_INPUT_STRING_COMP_EVT
Provisioner input string completion event 

ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT
Provisioner set node name completion event 

ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT
Provisioner add local app key completion event 

ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_APP_KEY_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_APP_KEY_COMP_EVT
Provisioner update local app key completion event 

ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT
Provisioner bind local model with local app key completion event 

ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_NET_KEY_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_NET_KEY_COMP_EVT
Provisioner add local network key completion event 

ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_NET_KEY_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_UPDATE_LOCAL_NET_KEY_COMP_EVT
Provisioner update local network key completion event 

ESP_BLE_MESH_PROVISIONER_STORE_NODE_COMP_DATA_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_STORE_NODE_COMP_DATA_COMP_EVT
Provisioner store node composition data completion event 

ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_UUID_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_UUID_COMP_EVT
Provisioner delete node with uuid completion event 

ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_ADDR_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_DELETE_NODE_WITH_ADDR_COMP_EVT
Provisioner delete node with unicast address completion event 

ESP_BLE_MESH_PROVISIONER_ENABLE_HEARTBEAT_RECV_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_ENABLE_HEARTBEAT_RECV_COMP_EVT
Provisioner start to receive heartbeat message completion event 

ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_TYPE_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_TYPE_COMP_EVT
Provisioner set the heartbeat filter type completion event 

ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_INFO_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_SET_HEARTBEAT_FILTER_INFO_COMP_EVT
Provisioner set the heartbeat filter information completion event 

ESP_BLE_MESH_PROVISIONER_RECV_HEARTBEAT_MESSAGE_EVT

enumerator ESP_BLE_MESH_PROVISIONER_RECV_HEARTBEAT_MESSAGE_EVT
Provisioner receive heartbeat message event 

ESP_BLE_MESH_PROVISIONER_DIRECT_ERASE_SETTINGS_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_DIRECT_ERASE_SETTINGS_COMP_EVT
Provisioner directly erase settings completion event 

ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_INDEX_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_INDEX_COMP_EVT
Provisioner open settings with index completion event 

ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_UID_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_OPEN_SETTINGS_WITH_UID_COMP_EVT
Provisioner open settings with user id completion event 

ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_INDEX_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_INDEX_COMP_EVT
Provisioner close settings with index completion event 

ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_UID_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_CLOSE_SETTINGS_WITH_UID_COMP_EVT
Provisioner close settings with user id completion event 

ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_INDEX_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_INDEX_COMP_EVT
Provisioner delete settings with index completion event 

ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_UID_COMP_EVT

enumerator ESP_BLE_MESH_PROVISIONER_DELETE_SETTINGS_WITH_UID_COMP_EVT
Provisioner delete settings with user id completion event 

ESP_BLE_MESH_SET_FAST_PROV_INFO_COMP_EVT

enumerator ESP_BLE_MESH_SET_FAST_PROV_INFO_COMP_EVT
Set fast provisioning information (e.g. unicast address range, net_idx, etc.) completion event 

ESP_BLE_MESH_SET_FAST_PROV_ACTION_COMP_EVT

enumerator ESP_BLE_MESH_SET_FAST_PROV_ACTION_COMP_EVT
Set fast provisioning action completion event 

ESP_BLE_MESH_HEARTBEAT_MESSAGE_RECV_EVT

enumerator ESP_BLE_MESH_HEARTBEAT_MESSAGE_RECV_EVT
Receive Heartbeat message event 

ESP_BLE_MESH_LPN_ENABLE_COMP_EVT

enumerator ESP_BLE_MESH_LPN_ENABLE_COMP_EVT
Enable Low Power Node completion event 

ESP_BLE_MESH_LPN_DISABLE_COMP_EVT

enumerator ESP_BLE_MESH_LPN_DISABLE_COMP_EVT
Disable Low Power Node completion event 

ESP_BLE_MESH_LPN_POLL_COMP_EVT

enumerator ESP_BLE_MESH_LPN_POLL_COMP_EVT
Low Power Node send Friend Poll completion event 

ESP_BLE_MESH_LPN_FRIENDSHIP_ESTABLISH_EVT

enumerator ESP_BLE_MESH_LPN_FRIENDSHIP_ESTABLISH_EVT
Low Power Node establishes friendship event 

ESP_BLE_MESH_LPN_FRIENDSHIP_TERMINATE_EVT

enumerator ESP_BLE_MESH_LPN_FRIENDSHIP_TERMINATE_EVT
Low Power Node terminates friendship event 

ESP_BLE_MESH_FRIEND_FRIENDSHIP_ESTABLISH_EVT

enumerator ESP_BLE_MESH_FRIEND_FRIENDSHIP_ESTABLISH_EVT
Friend Node establishes friendship event 

ESP_BLE_MESH_FRIEND_FRIENDSHIP_TERMINATE_EVT

enumerator ESP_BLE_MESH_FRIEND_FRIENDSHIP_TERMINATE_EVT
Friend Node terminates friendship event 

ESP_BLE_MESH_PROXY_CLIENT_RECV_ADV_PKT_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_RECV_ADV_PKT_EVT
Proxy Client receives Network ID advertising packet event 

ESP_BLE_MESH_PROXY_CLIENT_CONNECTED_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_CONNECTED_EVT
Proxy Client establishes connection successfully event 

ESP_BLE_MESH_PROXY_CLIENT_DISCONNECTED_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_DISCONNECTED_EVT
Proxy Client terminates connection successfully event 

ESP_BLE_MESH_PROXY_CLIENT_RECV_FILTER_STATUS_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_RECV_FILTER_STATUS_EVT
Proxy Client receives Proxy Filter Status event 

ESP_BLE_MESH_PROXY_CLIENT_CONNECT_COMP_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_CONNECT_COMP_EVT
Proxy Client connect completion event 

ESP_BLE_MESH_PROXY_CLIENT_DISCONNECT_COMP_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_DISCONNECT_COMP_EVT
Proxy Client disconnect completion event 

ESP_BLE_MESH_PROXY_CLIENT_SET_FILTER_TYPE_COMP_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_SET_FILTER_TYPE_COMP_EVT
Proxy Client set filter type completion event 

ESP_BLE_MESH_PROXY_CLIENT_ADD_FILTER_ADDR_COMP_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_ADD_FILTER_ADDR_COMP_EVT
Proxy Client add filter address completion event 

ESP_BLE_MESH_PROXY_CLIENT_REMOVE_FILTER_ADDR_COMP_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_REMOVE_FILTER_ADDR_COMP_EVT
Proxy Client remove filter address completion event 

ESP_BLE_MESH_PROXY_CLIENT_DIRECTED_PROXY_SET_COMP_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_DIRECTED_PROXY_SET_COMP_EVT
Proxy Client directed proxy set completion event 

ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT

enumerator ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT
Proxy Server establishes connection successfully event 

ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT

enumerator ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT
Proxy Server terminates connection successfully event 

ESP_BLE_MESH_PROXY_CLIENT_SEND_SOLIC_PDU_COMP_EVT

enumerator ESP_BLE_MESH_PROXY_CLIENT_SEND_SOLIC_PDU_COMP_EVT
Proxy Client send Solicitation PDU completion event 

ESP_BLE_MESH_MODEL_SUBSCRIBE_GROUP_ADDR_COMP_EVT

enumerator ESP_BLE_MESH_MODEL_SUBSCRIBE_GROUP_ADDR_COMP_EVT
Local model subscribes group address completion event 

ESP_BLE_MESH_MODEL_UNSUBSCRIBE_GROUP_ADDR_COMP_EVT

enumerator ESP_BLE_MESH_MODEL_UNSUBSCRIBE_GROUP_ADDR_COMP_EVT
Local model unsubscribes group address completion event 

ESP_BLE_MESH_DEINIT_MESH_COMP_EVT

enumerator ESP_BLE_MESH_DEINIT_MESH_COMP_EVT
De-initialize BLE Mesh stack completion event 

ESP_BLE_MESH_PROV_EVT_MAX

enumerator ESP_BLE_MESH_PROV_EVT_MAX


```

#### [anonymous]


```


enum [anonymous]
BLE Mesh server models related definitions. 
This enum value is the flag of transition timer operation 
Values:
ESP_BLE_MESH_SERVER_TRANS_TIMER_START

enumerator ESP_BLE_MESH_SERVER_TRANS_TIMER_START

ESP_BLE_MESH_SERVER_FLAG_MAX

enumerator ESP_BLE_MESH_SERVER_FLAG_MAX


```

#### esp_ble_mesh_server_state_type_t


```


enum esp_ble_mesh_server_state_type_t
This enum value is the type of server model states 
Values:
ESP_BLE_MESH_GENERIC_ONOFF_STATE

enumerator ESP_BLE_MESH_GENERIC_ONOFF_STATE

ESP_BLE_MESH_GENERIC_LEVEL_STATE

enumerator ESP_BLE_MESH_GENERIC_LEVEL_STATE

ESP_BLE_MESH_GENERIC_ONPOWERUP_STATE

enumerator ESP_BLE_MESH_GENERIC_ONPOWERUP_STATE

ESP_BLE_MESH_GENERIC_POWER_ACTUAL_STATE

enumerator ESP_BLE_MESH_GENERIC_POWER_ACTUAL_STATE

ESP_BLE_MESH_LIGHT_LIGHTNESS_ACTUAL_STATE

enumerator ESP_BLE_MESH_LIGHT_LIGHTNESS_ACTUAL_STATE

ESP_BLE_MESH_LIGHT_LIGHTNESS_LINEAR_STATE

enumerator ESP_BLE_MESH_LIGHT_LIGHTNESS_LINEAR_STATE

ESP_BLE_MESH_LIGHT_CTL_LIGHTNESS_STATE

enumerator ESP_BLE_MESH_LIGHT_CTL_LIGHTNESS_STATE

ESP_BLE_MESH_LIGHT_CTL_TEMP_DELTA_UV_STATE

enumerator ESP_BLE_MESH_LIGHT_CTL_TEMP_DELTA_UV_STATE

ESP_BLE_MESH_LIGHT_HSL_STATE

enumerator ESP_BLE_MESH_LIGHT_HSL_STATE

ESP_BLE_MESH_LIGHT_HSL_LIGHTNESS_STATE

enumerator ESP_BLE_MESH_LIGHT_HSL_LIGHTNESS_STATE

ESP_BLE_MESH_LIGHT_HSL_HUE_STATE

enumerator ESP_BLE_MESH_LIGHT_HSL_HUE_STATE

ESP_BLE_MESH_LIGHT_HSL_SATURATION_STATE

enumerator ESP_BLE_MESH_LIGHT_HSL_SATURATION_STATE

ESP_BLE_MESH_LIGHT_XYL_LIGHTNESS_STATE

enumerator ESP_BLE_MESH_LIGHT_XYL_LIGHTNESS_STATE

ESP_BLE_MESH_LIGHT_LC_LIGHT_ONOFF_STATE

enumerator ESP_BLE_MESH_LIGHT_LC_LIGHT_ONOFF_STATE

ESP_BLE_MESH_SERVER_MODEL_STATE_MAX

enumerator ESP_BLE_MESH_SERVER_MODEL_STATE_MAX


```

#### esp_ble_mesh_model_cb_event_t


```


enum esp_ble_mesh_model_cb_event_t
Values:
ESP_BLE_MESH_MODEL_OPERATION_EVT

enumerator ESP_BLE_MESH_MODEL_OPERATION_EVT
User-defined models receive messages from peer devices (e.g. get, set, status, etc) event 

ESP_BLE_MESH_MODEL_SEND_COMP_EVT

enumerator ESP_BLE_MESH_MODEL_SEND_COMP_EVT
User-defined models send messages completion event 

ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT

enumerator ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT
User-defined models publish messages completion event 

ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT

enumerator ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT
User-defined client models receive publish messages event 

ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT
Timeout event for the user-defined client models that failed to receive response from peer server models 

ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT

enumerator ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT
When a model is configured to publish messages periodically, this event will occur during every publish period 

ESP_BLE_MESH_SERVER_MODEL_UPDATE_STATE_COMP_EVT

enumerator ESP_BLE_MESH_SERVER_MODEL_UPDATE_STATE_COMP_EVT
Server models update state value completion event 

ESP_BLE_MESH_MODEL_EVT_MAX

enumerator ESP_BLE_MESH_MODEL_EVT_MAX


```




## ESP-BLE-MESH Core API Reference


This section contains ESP-BLE-MESH Core related APIs, which can be used to initialize ESP-BLE-MESH stack, provision, send/publish messages, etc.


This API reference covers six components:


* ESP-BLE-MESH Stack Initialization
* Reading of Local Data Information
* Low Power Operation (Updating)
* Send/Publish Messages, add Local AppKey, etc.
* ESP-BLE-MESH Node/Provisioner Provisioning
* ESP-BLE-MESH GATT Proxy Server



### ESP-BLE-MESH Stack Initialization




### Header File


* components/bt/esp_ble_mesh/api/core/include/esp_ble_mesh_common_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_common_api.h"
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


#### esp_ble_mesh_init


```


esp_err_t esp_ble_mesh_init(esp_ble_mesh_prov_t *prov, esp_ble_mesh_comp_t *comp)
Initialize BLE Mesh module. This API initializes provisioning capabilities and composition data information. 

Note
After calling this API, the device needs to call esp_ble_mesh_prov_enable() to enable provisioning functionality again.


Parameters

prov -- [in] Pointer to the device provisioning capabilities. This pointer must remain valid during the lifetime of the BLE Mesh device. 
comp -- [in] Pointer to the device composition data information. This pointer must remain valid during the lifetime of the BLE Mesh device.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_deinit


```


esp_err_t esp_ble_mesh_deinit(esp_ble_mesh_deinit_param_t *param)
De-initialize BLE Mesh module. 

Note

This function shall be invoked after esp_ble_mesh_client_model_deinit().
This function is strictly forbidden to run in any BTC Task Context (e.g. registered Mesh Event Callback).




Parameters
param -- [in] Pointer to the structure of BLE Mesh deinit parameters.

Returns
ESP_OK on success or error code otherwise. 



```



### Reading of Local Data Information




### Header File


* components/bt/esp_ble_mesh/api/core/include/esp_ble_mesh_local_data_operation_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_local_data_operation_api.h"
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


#### esp_ble_mesh_get_model_publish_period


```


int32_t esp_ble_mesh_get_model_publish_period(esp_ble_mesh_model_t *model)
Get the model publish period, the unit is ms. 

Parameters
model -- [in] Model instance pointer.

Returns
Publish period value on success, 0 or (negative) error code from errno.h on failure. 



```

#### esp_ble_mesh_get_primary_element_address


```


uint16_t esp_ble_mesh_get_primary_element_address(void)
Get the address of the primary element. 

Returns
Address of the primary element on success, or ESP_BLE_MESH_ADDR_UNASSIGNED on failure which means the device has not been provisioned. 



```

#### esp_ble_mesh_is_model_subscribed_to_group


```


uint16_t *esp_ble_mesh_is_model_subscribed_to_group(esp_ble_mesh_model_t *model, uint16_t group_addr)
Check if the model has subscribed to the given group address. Note: E.g., once a status message is received and the destination address is a group address, the model uses this API to check if it is successfully subscribed to the given group address. 

Parameters

model -- [in] Pointer to the model. 
group_addr -- [in] Group address.


Returns
Pointer to the group address within the Subscription List of the model on success, or NULL on failure which means the model has not subscribed to the given group address. Note: With the pointer to the group address returned, you can reset the group address to 0x0000 in order to unsubscribe the model from the group. 



```

#### esp_ble_mesh_find_element


```


esp_ble_mesh_elem_t *esp_ble_mesh_find_element(uint16_t element_addr)
Find the BLE Mesh element pointer via the element address. 

Parameters
element_addr -- [in] Element address.

Returns
Pointer to the element on success, or NULL on failure. 



```

#### esp_ble_mesh_get_element_count


```


uint8_t esp_ble_mesh_get_element_count(void)
Get the number of elements that have been registered. 

Returns
Number of elements. 



```

#### esp_ble_mesh_find_vendor_model


```


esp_ble_mesh_model_t *esp_ble_mesh_find_vendor_model(const esp_ble_mesh_elem_t *element, uint16_t company_id, uint16_t model_id)
Find the Vendor specific model with the given element, the company ID and the Vendor Model ID. 

Parameters

element -- [in] Element to which the model belongs. 
company_id -- [in] A 16-bit company identifier assigned by the Bluetooth SIG. 
model_id -- [in] A 16-bit vendor-assigned model identifier.


Returns
Pointer to the Vendor Model on success, or NULL on failure which means the Vendor Model is not found. 



```

#### esp_ble_mesh_find_sig_model


```


esp_ble_mesh_model_t *esp_ble_mesh_find_sig_model(const esp_ble_mesh_elem_t *element, uint16_t model_id)
Find the SIG model with the given element and Model id. 

Parameters

element -- [in] Element to which the model belongs. 
model_id -- [in] SIG model identifier.


Returns
Pointer to the SIG Model on success, or NULL on failure which means the SIG Model is not found. 



```

#### esp_ble_mesh_get_composition_data


```


const esp_ble_mesh_comp_t *esp_ble_mesh_get_composition_data(void)
Get the Composition data which has been registered. 

Returns
Pointer to the Composition data on success, or NULL on failure which means the Composition data is not initialized. 



```

#### esp_ble_mesh_model_subscribe_group_addr


```


esp_err_t esp_ble_mesh_model_subscribe_group_addr(uint16_t element_addr, uint16_t company_id, uint16_t model_id, uint16_t group_addr)
A local model of node or Provisioner subscribes a group address. 

Note
This function shall not be invoked before node is provisioned or Provisioner is enabled.


Parameters

element_addr -- [in] Unicast address of the element to which the model belongs. 
company_id -- [in] A 16-bit company identifier. 
model_id -- [in] A 16-bit model identifier. 
group_addr -- [in] The group address to be subscribed.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_model_unsubscribe_group_addr


```


esp_err_t esp_ble_mesh_model_unsubscribe_group_addr(uint16_t element_addr, uint16_t company_id, uint16_t model_id, uint16_t group_addr)
A local model of node or Provisioner unsubscribes a group address. 

Note
This function shall not be invoked before node is provisioned or Provisioner is enabled.


Parameters

element_addr -- [in] Unicast address of the element to which the model belongs. 
company_id -- [in] A 16-bit company identifier. 
model_id -- [in] A 16-bit model identifier. 
group_addr -- [in] The subscribed group address.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_get_local_net_key


```


const uint8_t *esp_ble_mesh_node_get_local_net_key(uint16_t net_idx)
This function is called by Node to get the local NetKey. 

Parameters
net_idx -- [in] NetKey index.

Returns
NetKey on success, or NULL on failure. 



```

#### esp_ble_mesh_node_get_local_app_key


```


const uint8_t *esp_ble_mesh_node_get_local_app_key(uint16_t app_idx)
This function is called by Node to get the local AppKey. 

Parameters
app_idx -- [in] AppKey index.

Returns
AppKey on success, or NULL on failure. 



```

#### esp_ble_mesh_node_add_local_net_key


```


esp_err_t esp_ble_mesh_node_add_local_net_key(const uint8_t net_key[16], uint16_t net_idx)
This function is called by Node to add a local NetKey. 

Note
This function can only be called after the device is provisioned.


Parameters

net_key -- [in] NetKey to be added. 
net_idx -- [in] NetKey Index.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_add_local_app_key


```


esp_err_t esp_ble_mesh_node_add_local_app_key(const uint8_t app_key[16], uint16_t net_idx, uint16_t app_idx)
This function is called by Node to add a local AppKey. 

Note
The net_idx must be an existing one. This function can only be called after the device is provisioned.


Parameters

app_key -- [in] AppKey to be added. 
net_idx -- [in] NetKey Index. 
app_idx -- [in] AppKey Index.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_bind_app_key_to_local_model


```


esp_err_t esp_ble_mesh_node_bind_app_key_to_local_model(uint16_t element_addr, uint16_t company_id, uint16_t model_id, uint16_t app_idx)
This function is called by Node to bind AppKey to model locally. 

Note
If going to bind app_key with local vendor model, the company_id shall be set to 0xFFFF. This function can only be called after the device is provisioned.


Parameters

element_addr -- [in] Node local element address 
company_id -- [in] Node local company id 
model_id -- [in] Node local model id 
app_idx -- [in] Node local appkey index


Returns
ESP_OK on success or error code otherwise. 



```



### Low Power Operation (Updating)




### Header File


* components/bt/esp_ble_mesh/api/core/include/esp_ble_mesh_low_power_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_low_power_api.h"
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


#### esp_ble_mesh_lpn_enable


```


esp_err_t esp_ble_mesh_lpn_enable(void)
Enable BLE Mesh device LPN functionality. 

Note
This API enables LPN functionality. Once called, the proper Friend Request will be sent.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_lpn_disable


```


esp_err_t esp_ble_mesh_lpn_disable(bool force)
Disable BLE Mesh device LPN functionality. 

Parameters
force -- [in] when disabling LPN functionality, use this flag to indicate whether directly clear corresponding information or just send friend clear to disable it if friendship has already been established.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_lpn_poll


```


esp_err_t esp_ble_mesh_lpn_poll(void)
LPN tries to poll messages from the Friend Node. 

Note
The Friend Poll message is sent by a Low Power node to ask the Friend node to send a message that it has stored for the Low Power node. Users can call this API to send Friend Poll message manually. If this API is not invoked, the bottom layer of the Low Power node will send Friend Poll before the PollTimeout timer expires. If the corresponding Friend Update is received and MD is set to 0, which means there are no messages for the Low Power node, then the Low Power node will stop scanning.


Returns
ESP_OK on success or error code otherwise. 



```



### Send/Publish Messages, Add Local AppKey, Etc.




### Header File


* components/bt/esp_ble_mesh/api/core/include/esp_ble_mesh_networking_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_networking_api.h"
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


#### esp_ble_mesh_register_custom_model_callback


```


esp_err_t esp_ble_mesh_register_custom_model_callback(esp_ble_mesh_model_cb_t callback)
Register BLE Mesh callback for user-defined models' operations. This callback can report the following events generated for the user-defined models: 

Call back the messages received by user-defined client and server models to the application layer;
If users call esp_ble_mesh_server/client_model_send, this callback notifies the application layer of the send_complete event;
If user-defined client model sends a message that requires response, and the response message is received after the timer expires, the response message will be reported to the application layer as published by a peer device;
If the user-defined client model fails to receive the response message during a specified period of time, a timeout event will be reported to the application layer.



Note
The client models (i.e. Config Client model, Health Client model, Generic Client models, Sensor Client model, Scene Client model and Lighting Client models) that have been realized internally have their specific register functions. For example, esp_ble_mesh_register_config_client_callback is the register function for Config Client Model.


Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_model_msg_opcode_init


```


esp_err_t esp_ble_mesh_model_msg_opcode_init(uint8_t *data, uint32_t opcode)
Add the message opcode to the beginning of the model message before sending or publishing the model message. 

Note
This API is only used to set the opcode of the message.


Parameters

data -- [in] Pointer to the message data. 
opcode -- [in] The message opcode.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_client_model_init


```


esp_err_t esp_ble_mesh_client_model_init(esp_ble_mesh_model_t *model)
Initialize the user-defined client model. All user-defined client models shall call this function to initialize the client model internal data. Node: Before calling this API, the op_pair_size and op_pair variables within the user_data(defined using esp_ble_mesh_client_t_) of the client model need to be initialized. 

Parameters
model -- [in] BLE Mesh Client model to which the message belongs.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_client_model_deinit


```


esp_err_t esp_ble_mesh_client_model_deinit(esp_ble_mesh_model_t *model)
De-initialize the user-defined client model. 

Note
This function shall be invoked before esp_ble_mesh_deinit() is called.


Parameters
model -- [in] Pointer of the Client model.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_server_model_send_msg


```


esp_err_t esp_ble_mesh_server_model_send_msg(esp_ble_mesh_model_t *model, esp_ble_mesh_msg_ctx_t *ctx, uint32_t opcode, uint16_t length, uint8_t *data)
Send server model messages(such as server model status messages). 

Parameters

model -- [in] BLE Mesh Server Model to which the message belongs. 
ctx -- [in] Message context, includes keys, TTL, etc. 
opcode -- [in] Message opcode. 
length -- [in] Message length (exclude the message opcode). 
data -- [in] Parameters of Access Payload (exclude the message opcode) to be sent.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_client_model_send_msg


```


esp_err_t esp_ble_mesh_client_model_send_msg(esp_ble_mesh_model_t *model, esp_ble_mesh_msg_ctx_t *ctx, uint32_t opcode, uint16_t length, uint8_t *data, int32_t msg_timeout, bool need_rsp, esp_ble_mesh_dev_role_t device_role)
Send client model message (such as model get, set, etc). 

Parameters

model -- [in] BLE Mesh Client Model to which the message belongs. 
ctx -- [in] Message context, includes keys, TTL, etc. 
opcode -- [in] Message opcode. 
length -- [in] Message length (exclude the message opcode). 
data -- [in] Parameters of the Access Payload (exclude the message opcode) to be sent. 
msg_timeout -- [in] Time to get response to the message (in milliseconds). 
need_rsp -- [in] TRUE if the opcode requires the peer device to reply, FALSE otherwise. 
device_role -- [in] Role of the device (Node/Provisioner) that sends the message.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_model_publish


```


esp_err_t esp_ble_mesh_model_publish(esp_ble_mesh_model_t *model, uint32_t opcode, uint16_t length, uint8_t *data, esp_ble_mesh_dev_role_t device_role)
Send a model publication message. 

Note
Before calling this function, the user needs to ensure that the model publication message (esp_ble_mesh_model_pub_t::msg) contains a valid message to be sent. And if users want to update the publishing message, this API should be called in ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT with the message updated.


Parameters

model -- [in] Mesh (client) Model publishing the message. 
opcode -- [in] Message opcode. 
length -- [in] Message length (exclude the message opcode). 
data -- [in] Parameters of the Access Payload (exclude the message opcode) to be sent. 
device_role -- [in] Role of the device (node/provisioner) publishing the message of the type esp_ble_mesh_dev_role_t.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_server_model_update_state


```


esp_err_t esp_ble_mesh_server_model_update_state(esp_ble_mesh_model_t *model, esp_ble_mesh_server_state_type_t type, esp_ble_mesh_server_state_value_t *value)
Update a server model state value. If the model publication state is set properly (e.g. publish address is set to a valid address), it will publish corresponding status message. 

Note
Currently this API is used to update bound state value, not for all server model states.


Parameters

model -- [in] Server model which is going to update the state. 
type -- [in] Server model state type. 
value -- [in] Server model state value.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_local_reset


```


esp_err_t esp_ble_mesh_node_local_reset(void)
Reset the provisioning procedure of the local BLE Mesh node. 

Note
All provisioning information in this node will be deleted and the node needs to be re-provisioned. The API function esp_ble_mesh_node_prov_enable() needs to be called to start a new provisioning procedure.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_set_node_name


```


esp_err_t esp_ble_mesh_provisioner_set_node_name(uint16_t index, const char *name)
This function is called to set the node (provisioned device) name. 

Note
index is obtained from the parameters of ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT.


Parameters

index -- [in] Index of the node in the node queue. 
name -- [in] Name (end by '\0') to be set for the node.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_get_node_name


```


const char *esp_ble_mesh_provisioner_get_node_name(uint16_t index)
This function is called to get the node (provisioned device) name. 

Note
index is obtained from the parameters of ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT.


Parameters
index -- [in] Index of the node in the node queue.

Returns
Node name on success, or NULL on failure. 



```

#### esp_ble_mesh_provisioner_get_node_index


```


uint16_t esp_ble_mesh_provisioner_get_node_index(const char *name)
This function is called to get the node (provisioned device) index. 

Parameters
name -- [in] Name of the node (end by '\0').

Returns
Node index on success, or an invalid value (0xFFFF) on failure. 



```

#### esp_ble_mesh_provisioner_store_node_comp_data


```


esp_err_t esp_ble_mesh_provisioner_store_node_comp_data(uint16_t unicast_addr, uint8_t *data, uint16_t length)
This function is called to store the Composition Data of the node. 

Parameters

unicast_addr -- [in] Element address of the node 
data -- [in] Pointer of Composition Data 
length -- [in] Length of Composition Data


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_get_node_with_uuid


```


esp_ble_mesh_node_t *esp_ble_mesh_provisioner_get_node_with_uuid(const uint8_t uuid[16])
This function is called to get the provisioned node information with the node device uuid. 

Parameters
uuid -- [in] Device UUID of the node

Returns
Pointer of the node info struct or NULL on failure. 



```

#### esp_ble_mesh_provisioner_get_node_with_addr


```


esp_ble_mesh_node_t *esp_ble_mesh_provisioner_get_node_with_addr(uint16_t unicast_addr)
This function is called to get the provisioned node information with the node unicast address. 

Parameters
unicast_addr -- [in] Unicast address of the node

Returns
Pointer of the node info struct or NULL on failure. 



```

#### esp_ble_mesh_provisioner_get_node_with_name


```


esp_ble_mesh_node_t *esp_ble_mesh_provisioner_get_node_with_name(const char *name)
This function is called to get the provisioned node information with the node name. 

Parameters
name -- [in] Name of the node (end by '\0').

Returns
Pointer of the node info struct or NULL on failure. 



```

#### esp_ble_mesh_provisioner_get_prov_node_count


```


uint16_t esp_ble_mesh_provisioner_get_prov_node_count(void)
This function is called by Provisioner to get provisioned node count. 

Returns
Number of the provisioned nodes. 



```

#### esp_ble_mesh_provisioner_get_node_table_entry


```


const esp_ble_mesh_node_t **esp_ble_mesh_provisioner_get_node_table_entry(void)
This function is called by Provisioner to get the entry of the node table. 

Note
After invoking the function to get the entry of nodes, users can use the "for" loop combined with the macro CONFIG_BLE_MESH_MAX_PROV_NODES to get each node's information. Before trying to read the node's information, users need to check if the node exists, i.e. if the *(esp_ble_mesh_node_t **node) is NULL. For example: ``` const esp_ble_mesh_node_t **entry = esp_ble_mesh_provisioner_get_node_table_entry(); for (int i = 0; i < CONFIG_BLE_MESH_MAX_PROV_NODES; i++) { const esp_ble_mesh_node_t *node = entry[i]; if (node) { ...... } } ```


Returns
Pointer to the start of the node table. 



```

#### esp_ble_mesh_provisioner_delete_node_with_uuid


```


esp_err_t esp_ble_mesh_provisioner_delete_node_with_uuid(const uint8_t uuid[16])
This function is called to delete the provisioned node information with the node device uuid. 

Parameters
uuid -- [in] Device UUID of the node

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_delete_node_with_addr


```


esp_err_t esp_ble_mesh_provisioner_delete_node_with_addr(uint16_t unicast_addr)
This function is called to delete the provisioned node information with the node unicast address. 

Parameters
unicast_addr -- [in] Unicast address of the node

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_add_local_app_key


```


esp_err_t esp_ble_mesh_provisioner_add_local_app_key(const uint8_t app_key[16], uint16_t net_idx, uint16_t app_idx)
This function is called to add a local AppKey for Provisioner. 

Note
app_key: If set to NULL, app_key will be generated internally. net_idx: Should be an existing one. app_idx: If it is going to be generated internally, it should be set to 0xFFFF, and the new app_idx will be reported via an event.


Parameters

app_key -- [in] The app key to be set for the local BLE Mesh stack. 
net_idx -- [in] The network key index. 
app_idx -- [in] The app key index.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_update_local_app_key


```


esp_err_t esp_ble_mesh_provisioner_update_local_app_key(const uint8_t app_key[16], uint16_t net_idx, uint16_t app_idx)
This function is used to update a local AppKey for Provisioner. 

Parameters

app_key -- [in] Value of the AppKey. 
net_idx -- [in] Corresponding NetKey Index. 
app_idx -- [in] The AppKey Index


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_get_local_app_key


```


const uint8_t *esp_ble_mesh_provisioner_get_local_app_key(uint16_t net_idx, uint16_t app_idx)
This function is called by Provisioner to get the local app key value. 

Parameters

net_idx -- [in] Network key index. 
app_idx -- [in] Application key index.


Returns
App key on success, or NULL on failure. 



```

#### esp_ble_mesh_provisioner_bind_app_key_to_local_model


```


esp_err_t esp_ble_mesh_provisioner_bind_app_key_to_local_model(uint16_t element_addr, uint16_t app_idx, uint16_t model_id, uint16_t company_id)
This function is called by Provisioner to bind own model with proper app key. 

Note
company_id: If going to bind app_key with local vendor model, company_id should be set to 0xFFFF.


Parameters

element_addr -- [in] Provisioner local element address 
app_idx -- [in] Provisioner local appkey index 
model_id -- [in] Provisioner local model id 
company_id -- [in] Provisioner local company id


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_add_local_net_key


```


esp_err_t esp_ble_mesh_provisioner_add_local_net_key(const uint8_t net_key[16], uint16_t net_idx)
This function is called by Provisioner to add local network key. 

Note
net_key: If set to NULL, net_key will be generated internally. net_idx: If it is going to be generated internally, it should be set to 0xFFFF, and the new net_idx will be reported via an event.


Parameters

net_key -- [in] The network key to be added to the Provisioner local BLE Mesh stack. 
net_idx -- [in] The network key index.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_update_local_net_key


```


esp_err_t esp_ble_mesh_provisioner_update_local_net_key(const uint8_t net_key[16], uint16_t net_idx)
This function is called by Provisioner to update a local network key. 

Parameters

net_key -- [in] Value of the NetKey. 
net_idx -- [in] The NetKey Index.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_get_local_net_key


```


const uint8_t *esp_ble_mesh_provisioner_get_local_net_key(uint16_t net_idx)
This function is called by Provisioner to get the local network key value. 

Parameters
net_idx -- [in] Network key index.

Returns
Network key on success, or NULL on failure. 



```

#### esp_ble_mesh_provisioner_recv_heartbeat


```


esp_err_t esp_ble_mesh_provisioner_recv_heartbeat(bool enable)
This function is called by Provisioner to enable or disable receiving heartbeat messages. 

Note
If enabling receiving heartbeat message successfully, the filter will be an empty rejectlist by default, which means all heartbeat messages received by the Provisioner will be reported to the application layer.


Parameters
enable -- [in] Enable or disable receiving heartbeat messages.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_set_heartbeat_filter_type


```


esp_err_t esp_ble_mesh_provisioner_set_heartbeat_filter_type(uint8_t type)
This function is called by Provisioner to set the heartbeat filter type. 

Note
1. If the filter type is not the same with the current value, then all the filter entries will be cleaned.
If the previous type is rejectlist, and changed to acceptlist, then the filter will be an empty acceptlist, which means no heartbeat messages will be reported. Users need to add SRC or DST into the filter entry, then heartbeat messages from the SRC or to the DST will be reported.




Parameters
type -- [in] Heartbeat filter type (acceptlist or rejectlist).

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_set_heartbeat_filter_info


```


esp_err_t esp_ble_mesh_provisioner_set_heartbeat_filter_info(uint8_t op, esp_ble_mesh_heartbeat_filter_info_t *info)
This function is called by Provisioner to add or remove a heartbeat filter entry. 

If the operation is "REMOVE", the "hb_src" can be set to the SRC (can only be a unicast address) of heartbeat messages, and the "hb_dst" can be set to the DST (unicast address or group address), at least one of them needs to be set.
The filter entry with the same SRC or DST will be removed.





Note
1. If the operation is "ADD", the "hb_src" can be set to the SRC (can only be a unicast address) of heartbeat messages, and the "hb_dst" can be set to the DST (unicast address or group address), at least one of them needs to be set.
If only one of them is set, the filter entry will only use the configured SRC or DST to filter heartbeat messages.
If both of them are set, the SRC and DST will both be used to decide if a heartbeat message will be handled.
If SRC or DST already exists in some filter entry, then the corresponding entry will be cleaned firstly, then a new entry will be allocated to store the information.




Parameters

op -- [in] Add or REMOVE 
info -- [in] Heartbeat filter entry information, including: hb_src - Heartbeat source address; hb_dst - Heartbeat destination address;


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_direct_erase_settings


```


esp_err_t esp_ble_mesh_provisioner_direct_erase_settings(void)
This function is called by Provisioner to directly erase the mesh information from nvs namespace. 

Note
This function can be invoked when the mesh stack is not initialized or has been de-initialized.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_open_settings_with_index


```


esp_err_t esp_ble_mesh_provisioner_open_settings_with_index(uint8_t index)
This function is called by Provisioner to open a nvs namespace for storing mesh information. 

Note
Before open another nvs namespace, the previously opened nvs namespace must be closed firstly.


Parameters
index -- [in] Settings index.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_open_settings_with_uid


```


esp_err_t esp_ble_mesh_provisioner_open_settings_with_uid(const char *uid)
This function is called by Provisioner to open a nvs namespace for storing mesh information. 

Note
Before open another nvs namespace, the previously opened nvs namespace must be closed firstly.


Parameters
uid -- [in] Settings user id.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_close_settings_with_index


```


esp_err_t esp_ble_mesh_provisioner_close_settings_with_index(uint8_t index, bool erase)
This function is called by Provisioner to close a nvs namespace which is opened previously for storing mesh information. 

Note
1. Before closing the nvs namespace, it must be open.
When the function is invoked, the Provisioner functionality will be disabled firstly, and: a) If the "erase" flag is set to false, the mesh information will be cleaned (e.g. removing NetKey, AppKey, nodes, etc) from the mesh stack. b) If the "erase" flag is set to true, the mesh information stored in the nvs namespace will also be erased besides been cleaned from the mesh stack.
If Provisioner tries to work properly again, we can invoke the open function to open a new nvs namespace or a previously added one, and restore the mesh information from it if not erased.
The working process shall be as following: a) Open settings A b) Start to provision and control nodes c) Close settings A d) Open settings B e) Start to provision and control other nodes f) Close settings B g) ......




Parameters

index -- [in] Settings index. 
erase -- [in] Indicate if erasing mesh information.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_close_settings_with_uid


```


esp_err_t esp_ble_mesh_provisioner_close_settings_with_uid(const char *uid, bool erase)
This function is called by Provisioner to close a nvs namespace which is opened previously for storing mesh information. 

Note
1. Before closing the nvs namespace, it must be open.
When the function is invoked, the Provisioner functionality will be disabled firstly, and: a) If the "erase" flag is set to false, the mesh information will be cleaned (e.g. removing NetKey, AppKey, nodes, etc) from the mesh stack. b) If the "erase" flag is set to true, the mesh information stored in the nvs namespace will also be erased besides been cleaned from the mesh stack.
If Provisioner tries to work properly again, we can invoke the open function to open a new nvs namespace or a previously added one, and restore the mesh information from it if not erased.
The working process shall be as following: a) Open settings A b) Start to provision and control nodes c) Close settings A d) Open settings B e) Start to provision and control other nodes f) Close settings B g) ......




Parameters

uid -- [in] Settings user id. 
erase -- [in] Indicate if erasing mesh information.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_delete_settings_with_index


```


esp_err_t esp_ble_mesh_provisioner_delete_settings_with_index(uint8_t index)
This function is called by Provisioner to erase the mesh information and settings user id from a nvs namespace. 

Note
When this function is called, the nvs namespace must not be open. This function is used to erase the mesh information and settings user id which are not used currently.


Parameters
index -- [in] Settings index.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_delete_settings_with_uid


```


esp_err_t esp_ble_mesh_provisioner_delete_settings_with_uid(const char *uid)
This function is called by Provisioner to erase the mesh information and settings user id from a nvs namespace. 

Note
When this function is called, the nvs namespace must not be open. This function is used to erase the mesh information and settings user id which are not used currently.


Parameters
uid -- [in] Settings user id.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_get_settings_uid


```


const char *esp_ble_mesh_provisioner_get_settings_uid(uint8_t index)
This function is called by Provisioner to get settings user id. 

Parameters
index -- [in] Settings index.

Returns
Setting user id on success or NULL on failure. 



```

#### esp_ble_mesh_provisioner_get_settings_index


```


uint8_t esp_ble_mesh_provisioner_get_settings_index(const char *uid)
This function is called by Provisioner to get settings index. 

Parameters
uid -- [in] Settings user id.

Returns
Settings index. 



```

#### esp_ble_mesh_provisioner_get_free_settings_count


```


uint8_t esp_ble_mesh_provisioner_get_free_settings_count(void)
This function is called by Provisioner to get the number of free settings user id. 

Returns
Number of free settings user id. 



```

#### esp_ble_mesh_get_fast_prov_app_key


```


const uint8_t *esp_ble_mesh_get_fast_prov_app_key(uint16_t net_idx, uint16_t app_idx)
This function is called to get fast provisioning application key. 

Parameters

net_idx -- [in] Network key index. 
app_idx -- [in] Application key index.


Returns
Application key on success, or NULL on failure. 



```



### Type Definitions


#### esp_ble_mesh_model_cb_t


```


typedef void (*esp_ble_mesh_model_cb_t)(esp_ble_mesh_model_cb_event_t event, esp_ble_mesh_model_cb_param_t *param)
: event, event code of user-defined model events; param, parameters of user-defined model events 

```



### ESP-BLE-MESH Node/Provisioner Provisioning




### Header File


* components/bt/esp_ble_mesh/api/core/include/esp_ble_mesh_provisioning_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_provisioning_api.h"
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


#### esp_ble_mesh_register_prov_callback


```


esp_err_t esp_ble_mesh_register_prov_callback(esp_ble_mesh_prov_cb_t callback)
Register BLE Mesh provisioning callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_is_provisioned


```


bool esp_ble_mesh_node_is_provisioned(void)
Check if a device has been provisioned. 

Returns
TRUE if the device is provisioned, FALSE if the device is unprovisioned. 



```

#### esp_ble_mesh_node_prov_enable


```


esp_err_t esp_ble_mesh_node_prov_enable(esp_ble_mesh_prov_bearer_t bearers)
Enable specific provisioning bearers to get the device ready for provisioning. 

Note
PB-ADV: send unprovisioned device beacon. PB-GATT: send connectable advertising packets.


Parameters
bearers -- Bit-wise OR of provisioning bearers.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_prov_disable


```


esp_err_t esp_ble_mesh_node_prov_disable(esp_ble_mesh_prov_bearer_t bearers)
Disable specific provisioning bearers to make a device inaccessible for provisioning. 

Parameters
bearers -- Bit-wise OR of provisioning bearers.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_set_oob_pub_key


```


esp_err_t esp_ble_mesh_node_set_oob_pub_key(uint8_t pub_key_x[32], uint8_t pub_key_y[32], uint8_t private_key[32])
Unprovisioned device set own oob public key & private key pair. 

Note
In order to avoid suffering brute-forcing attack (CVE-2020-26559). The Bluetooth SIG recommends that potentially vulnerable mesh provisioners use an out-of-band mechanism to exchange the public keys. So as an unprovisioned device, it should use this function to input the Public Key exchanged through the out-of-band mechanism.


Parameters

pub_key_x -- [in] Unprovisioned device's Public Key X 
pub_key_y -- [in] Unprovisioned device's Public Key Y 
private_key -- [in] Unprovisioned device's Private Key


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_input_number


```


esp_err_t esp_ble_mesh_node_input_number(uint32_t number)
Provide provisioning input OOB number. 

Note
This is intended to be called if the user has received ESP_BLE_MESH_NODE_PROV_INPUT_EVT with ESP_BLE_MESH_ENTER_NUMBER as the action.


Parameters
number -- [in] Number input by device.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_node_input_string


```


esp_err_t esp_ble_mesh_node_input_string(const char *string)
Provide provisioning input OOB string. 

Note
This is intended to be called if the user has received ESP_BLE_MESH_NODE_PROV_INPUT_EVT with ESP_BLE_MESH_ENTER_STRING as the action.


Parameters
string -- [in] String input by device.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_set_unprovisioned_device_name


```


esp_err_t esp_ble_mesh_set_unprovisioned_device_name(const char *name)
Using this function, an unprovisioned device can set its own device name, which will be broadcasted in its advertising data. 

Note
This API applicable to PB-GATT mode only by setting the name to the scan response data, it doesn't apply to PB-ADV mode.


Parameters
name -- [in] Unprovisioned device name

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_read_oob_pub_key


```


esp_err_t esp_ble_mesh_provisioner_read_oob_pub_key(uint8_t link_idx, uint8_t pub_key_x[32], uint8_t pub_key_y[32])
Provisioner inputs unprovisioned device's oob public key. 

Note
In order to avoid suffering brute-forcing attack (CVE-2020-26559). The Bluetooth SIG recommends that potentially vulnerable mesh provisioners use an out-of-band mechanism to exchange the public keys.


Parameters

link_idx -- [in] The provisioning link index 
pub_key_x -- [in] Unprovisioned device's Public Key X 
pub_key_y -- [in] Unprovisioned device's Public Key Y


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_input_string


```


esp_err_t esp_ble_mesh_provisioner_input_string(const char *string, uint8_t link_idx)
Provide provisioning input OOB string. 
          This is intended to be called after the esp_ble_mesh_prov_t prov_input_num
          callback has been called with ESP_BLE_MESH_ENTER_STRING as the action.




Parameters

string -- [in] String input by Provisioner. 
link_idx -- [in] The provisioning link index.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_input_number


```


esp_err_t esp_ble_mesh_provisioner_input_number(uint32_t number, uint8_t link_idx)
Provide provisioning input OOB number. 
          This is intended to be called after the esp_ble_mesh_prov_t prov_input_num
          callback has been called with ESP_BLE_MESH_ENTER_NUMBER as the action.




Parameters

number -- [in] Number input by Provisioner. 
link_idx -- [in] The provisioning link index.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_prov_enable


```


esp_err_t esp_ble_mesh_provisioner_prov_enable(esp_ble_mesh_prov_bearer_t bearers)
Enable one or more provisioning bearers. 

Note
PB-ADV: Enable BLE scan. PB-GATT: Initialize corresponding BLE Mesh Proxy info.


Parameters
bearers -- [in] Bit-wise OR of provisioning bearers.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_prov_disable


```


esp_err_t esp_ble_mesh_provisioner_prov_disable(esp_ble_mesh_prov_bearer_t bearers)
Disable one or more provisioning bearers. 

Note
PB-ADV: Disable BLE scan. PB-GATT: Break any existing BLE Mesh Provisioning connections.


Parameters
bearers -- [in] Bit-wise OR of provisioning bearers.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_add_unprov_dev


```


esp_err_t esp_ble_mesh_provisioner_add_unprov_dev(esp_ble_mesh_unprov_dev_add_t *add_dev, esp_ble_mesh_dev_add_flag_t flags)
Add unprovisioned device info to the unprov_dev queue. 

Note
: 1. Currently address type only supports public address and static random address.
If device UUID and/or device address as well as address type already exist in the device queue, but the bearer is different from the existing one, add operation will also be successful and it will update the provision bearer supported by the device.
For example, if the Provisioner wants to add an unprovisioned device info before receiving its unprovisioned device beacon or Mesh Provisioning advertising packets, the Provisioner can use this API to add the device info with each one or both of device UUID and device address added. When the Provisioner gets the device's advertising packets, it will start provisioning the device internally.
In this situation, the Provisioner can set bearers with each one or both of ESP_BLE_MESH_PROV_ADV and ESP_BLE_MESH_PROV_GATT enabled, and cannot set flags with ADD_DEV_START_PROV_NOW_FLAG enabled.


Another example is when the Provisioner receives the unprovisioned device's beacon or Mesh Provisioning advertising packets, the advertising packets will be reported on to the application layer using the callback registered by the function esp_ble_mesh_register_prov_callback. And in the callback, the Provisioner can call this API to start provisioning the device.
If the Provisioner uses PB-ADV to provision, either one or both of device UUID and device address can be added, bearers shall be set with ESP_BLE_MESH_PROV_ADV enabled and the flags shall be set with ADD_DEV_START_PROV_NOW_FLAG enabled.
If the Provisioner uses PB-GATT to provision, both the device UUID and device address need to be added, bearers shall be set with ESP_BLE_MESH_PROV_GATT enabled, and the flags shall be set with ADD_DEV_START_PROV_NOW_FLAG enabled.
If the Provisioner just wants to store the unprovisioned device info when receiving its advertising packets and start to provision it the next time (e.g. after receiving its advertising packets again), then it can add the device info with either one or both of device UUID and device address included. Bearers can be set with either one or both of ESP_BLE_MESH_PROV_ADV and ESP_BLE_MESH_PROV_GATT enabled (recommend to enable the bearer which will receive its advertising packets, because if the other bearer is enabled, the Provisioner is not aware if the device supports the bearer), and flags cannot be set with ADD_DEV_START_PROV_NOW_FLAG enabled.
Note: ESP_BLE_MESH_PROV_ADV, ESP_BLE_MESH_PROV_GATT and ADD_DEV_START_PROV_NOW_FLAG can not be enabled at the same time. 






Parameters

add_dev -- [in] Pointer to a struct containing the device information 
flags -- [in] Flags indicate several operations on the device information
Remove device information from queue after device has been provisioned (BIT0)
Start provisioning immediately after device is added to queue (BIT1)
Device can be removed if device queue is full (BIT2)




Returns
ESP_OK on success or error code otherwise.



```

#### esp_ble_mesh_provisioner_prov_device_with_addr


```


esp_err_t esp_ble_mesh_provisioner_prov_device_with_addr(const uint8_t uuid[16], esp_ble_mesh_bd_addr_t addr, esp_ble_mesh_addr_type_t addr_type, esp_ble_mesh_prov_bearer_t bearer, uint16_t oob_info, uint16_t unicast_addr)
Provision an unprovisioned device and assign a fixed unicast address for it in advance. 

Note
: 1. Currently address type only supports public address and static random address.
Bearer must be equal to ESP_BLE_MESH_PROV_ADV or ESP_BLE_MESH_PROV_GATT, since Provisioner will start to provision a device immediately once this function is invoked. And the input bearer must be identical with the one within the parameters of the ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT event.
If this function is used by a Provisioner to provision devices, the application should take care of the assigned unicast address and avoid overlap of the unicast addresses of different nodes.
Recommend to use only one of the functions "esp_ble_mesh_provisioner_add_unprov_dev" and "esp_ble_mesh_provisioner_prov_device_with_addr" by a Provisioner. 




Parameters

uuid -- [in] Device UUID of the unprovisioned device 
addr -- [in] Device address of the unprovisioned device 
addr_type -- [in] Device address type of the unprovisioned device 
bearer -- [in] Provisioning bearer going to be used by Provisioner 
oob_info -- [in] OOB info of the unprovisioned device 
unicast_addr -- [in] Unicast address going to be allocated for the unprovisioned device


Returns
Zero on success or (negative) error code otherwise.



```

#### esp_ble_mesh_provisioner_delete_dev


```


esp_err_t esp_ble_mesh_provisioner_delete_dev(esp_ble_mesh_device_delete_t *del_dev)
Delete device from queue, and reset current provisioning link with the device. 

Note
If the device is in the queue, remove it from the queue; if the device is being provisioned, terminate the provisioning procedure. Either one of the device address or device UUID can be used as input.


Parameters
del_dev -- [in] Pointer to a struct containing the device information.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_set_dev_uuid_match


```


esp_err_t esp_ble_mesh_provisioner_set_dev_uuid_match(const uint8_t *match_val, uint8_t match_len, uint8_t offset, bool prov_after_match)
This function is called by Provisioner to set the part of the device UUID to be compared before starting to provision. 

Parameters

match_val -- [in] Value to be compared with the part of the device UUID. 
match_len -- [in] Length of the compared match value. 
offset -- [in] Offset of the device UUID to be compared (based on zero). 
prov_after_match -- [in] Flag used to indicate whether provisioner should start to provision the device immediately if the part of the UUID matches.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_set_prov_data_info


```


esp_err_t esp_ble_mesh_provisioner_set_prov_data_info(esp_ble_mesh_prov_data_info_t *prov_data_info)
This function is called by Provisioner to set provisioning data information before starting to provision. 

Parameters
prov_data_info -- [in] Pointer to a struct containing net_idx or flags or iv_index.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_set_static_oob_value


```


esp_err_t esp_ble_mesh_provisioner_set_static_oob_value(const uint8_t *value, uint8_t length)
This function is called by Provisioner to set static oob value used for provisioning. 

AuthValues selected using a cryptographically secure random or pseudorandom number generator and having the maximum permitted entropy (128-bits) will be most difficult to brute-force. AuthValues with reduced entropy or generated in a predictable manner will not grant the same level of protection against this vulnerability. Selecting a new AuthValue with each provisioning attempt can also make it more difficult to launch a brute-force attack by requiring the attacker to restart the search with each provisioning attempt (CVE-2020-26556).

Note
The Bluetooth SIG recommends that mesh implementations enforce a randomly selected AuthValue using all of the available bits, where permitted by the implementation. A large entropy helps ensure that a brute-force of the AuthValue, even a static AuthValue, cannot normally be completed in a reasonable time (CVE-2020-26557).


Parameters

value -- [in] Pointer to the static oob value. 
length -- [in] Length of the static oob value.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_provisioner_set_primary_elem_addr


```


esp_err_t esp_ble_mesh_provisioner_set_primary_elem_addr(uint16_t addr)
This function is called by Provisioner to set own Primary element address. 

Note
This API must be invoked when BLE Mesh initialization is completed successfully, and can be invoked before Provisioner functionality is enabled. Once this API is invoked successfully, the prov_unicast_addr value in the struct esp_ble_mesh_prov_t will be ignored, and Provisioner will use this address as its own primary element address. And if the unicast address going to assigned for the next unprovisioned device is smaller than the input address + element number of Provisioner, then the address for the next unprovisioned device will be recalculated internally.


Parameters
addr -- [in] Unicast address of the Primary element of Provisioner.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_set_fast_prov_info


```


esp_err_t esp_ble_mesh_set_fast_prov_info(esp_ble_mesh_fast_prov_info_t *fast_prov_info)
This function is called to set provisioning data information before starting fast provisioning. 

Parameters
fast_prov_info -- [in] Pointer to a struct containing unicast address range, net_idx, etc.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_set_fast_prov_action


```


esp_err_t esp_ble_mesh_set_fast_prov_action(esp_ble_mesh_fast_prov_action_t action)
This function is called to start/suspend/exit fast provisioning. 

Parameters
action -- [in] fast provisioning action (i.e. enter, suspend, exit).

Returns
ESP_OK on success or error code otherwise. 



```



### Type Definitions


#### esp_ble_mesh_prov_cb_t


```


typedef void (*esp_ble_mesh_prov_cb_t)(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t *param)
: event, event code of provisioning events; param, parameters of provisioning events 

```

#### esp_ble_mesh_prov_adv_cb_t


```


typedef void (*esp_ble_mesh_prov_adv_cb_t)(const esp_ble_mesh_bd_addr_t addr, const esp_ble_mesh_addr_type_t addr_type, const uint8_t adv_type, const uint8_t *dev_uuid, uint16_t oob_info, esp_ble_mesh_prov_bearer_t bearer)
Callback for Provisioner that received advertising packets from unprovisioned devices which are not in the unprovisioned device queue. 
Report on the unprovisioned device beacon and mesh provisioning service adv data to application.

Param addr
[in] Pointer to the unprovisioned device address. 

Param addr_type
[in] Unprovisioned device address type. 

Param adv_type
[in] Adv packet type(ADV_IND or ADV_NONCONN_IND). 

Param dev_uuid
[in] Unprovisioned device UUID pointer. 

Param oob_info
[in] OOB information of the unprovisioned device. 

Param bearer
[in] Adv packet received from PB-GATT or PB-ADV bearer. 



```



### ESP-BLE-MESH GATT Proxy Server




### Header File


* components/bt/esp_ble_mesh/api/core/include/esp_ble_mesh_proxy_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_proxy_api.h"
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


#### esp_ble_mesh_proxy_identity_enable


```


esp_err_t esp_ble_mesh_proxy_identity_enable(void)
Enable advertising with Node Identity. 

Note
This API requires that GATT Proxy support be enabled. Once called, each subnet starts advertising using Node Identity for the next 60 seconds, and after 60s Network ID will be advertised. Under normal conditions, the BLE Mesh Proxy Node Identity and Network ID advertising will be enabled automatically by BLE Mesh stack after the device is provisioned.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_gatt_enable


```


esp_err_t esp_ble_mesh_proxy_gatt_enable(void)
Enable BLE Mesh GATT Proxy Service. 

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_gatt_disable


```


esp_err_t esp_ble_mesh_proxy_gatt_disable(void)
Disconnect the BLE Mesh GATT Proxy connection if there is any, and disable the BLE Mesh GATT Proxy Service. 

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_private_proxy_identity_enable


```


esp_err_t esp_ble_mesh_private_proxy_identity_enable(void)
Enable advertising with Private Node Identity. 

Note
This API requires that GATT Proxy support be enabled. Once called, each subnet starts advertising using Private Node Identity for the next 60 seconds, and after 60s Private Network ID will be advertised. Under normal conditions, the BLE Mesh Proxy Node Identity, Network ID advertising, Proxy Private Node Identity and Private Network ID advertising will be enabled automatically by BLE Mesh stack after the device is provisioned.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_private_proxy_identity_disable


```


esp_err_t esp_ble_mesh_private_proxy_identity_disable(void)
Disable advertising with Private Node Identity. 

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_client_connect


```


esp_err_t esp_ble_mesh_proxy_client_connect(esp_ble_mesh_bd_addr_t addr, esp_ble_mesh_addr_type_t addr_type, uint16_t net_idx)
Proxy Client creates a connection with the Proxy Server. 

Parameters

addr -- [in] Device address of the Proxy Server. 
addr_type -- [in] Device address type(public or static random). 
net_idx -- [in] NetKey Index related with Network ID in the Mesh Proxy advertising packet.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_client_disconnect


```


esp_err_t esp_ble_mesh_proxy_client_disconnect(uint8_t conn_handle)
Proxy Client terminates a connection with the Proxy Server. 

Parameters
conn_handle -- [in] Proxy connection handle.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_client_set_filter_type


```


esp_err_t esp_ble_mesh_proxy_client_set_filter_type(uint8_t conn_handle, uint16_t net_idx, esp_ble_mesh_proxy_filter_type_t filter_type)
Proxy Client sets the filter type of the Proxy Server. 

Parameters

conn_handle -- [in] Proxy connection handle. 
net_idx -- [in] Corresponding NetKey Index. 
filter_type -- [in] whitelist or blacklist.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_client_add_filter_addr


```


esp_err_t esp_ble_mesh_proxy_client_add_filter_addr(uint8_t conn_handle, uint16_t net_idx, uint16_t *addr, uint16_t addr_num)
Proxy Client adds address to the Proxy Server filter list. 

Parameters

conn_handle -- [in] Proxy connection handle. 
net_idx -- [in] Corresponding NetKey Index. 
addr -- [in] Pointer to the filter address. 
addr_num -- [in] Number of the filter address.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_client_remove_filter_addr


```


esp_err_t esp_ble_mesh_proxy_client_remove_filter_addr(uint8_t conn_handle, uint16_t net_idx, uint16_t *addr, uint16_t addr_num)
Proxy Client removes address from the Proxy Server filter list. 

Parameters

conn_handle -- [in] Proxy connection handle. 
net_idx -- [in] Corresponding NetKey Index. 
addr -- [in] Pointer to the filter address. 
addr_num -- [in] Number of the filter address.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_client_directed_proxy_set


```


esp_err_t esp_ble_mesh_proxy_client_directed_proxy_set(uint8_t conn_handle, uint16_t net_idx, uint8_t use_directed)
Proxy Client sets whether or not the Directed Proxy Server uses directed forwarding for Directed Proxy Client messages. 

Parameters

conn_handle -- [in] Proxy connection handle. 
net_idx -- [in] Corresponding NetKey Index. 
use_directed -- [in] Whether or not to send message by directed forwarding.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_proxy_client_send_solic_pdu


```


esp_err_t esp_ble_mesh_proxy_client_send_solic_pdu(uint8_t net_idx, uint16_t ssrc, uint16_t dst)
Proxy Client sends Solicitation PDU. 

Parameters

net_idx -- [in] Corresponding NetKey Index. 
ssrc -- [in] Solicitation SRC, shall be one of its element address. 
dst -- [in] Solicitation DST (TBD).


Returns
ESP_OK on success or error code otherwise. 



```



### Macros


#### ESP_BLE_MESH_PROXY_CLI_DIRECTED_FORWARDING_ENABLE


```


ESP_BLE_MESH_PROXY_CLI_DIRECTED_FORWARDING_ENABLE

```

#### ESP_BLE_MESH_PROXY_CLI_DIRECTED_FORWARDING_DISABLE


```


ESP_BLE_MESH_PROXY_CLI_DIRECTED_FORWARDING_DISABLE

```




## ESP-BLE-MESH Models API Reference


This section contains ESP-BLE-MESH Model related APIs, event types, event parameters, etc.


There are six categories of models:


* Configuration Client/Server Models
* Health Client/Server Models
* Generic Client/Server Models
* Sensor Client/Server Models
* Time and Scenes Client/Server Models
* Lighting Client/Server Models



Note


Definitions related to Server Models are being updated, and will be released soon.




### Configuration Client/Server Models




### Header File


* components/bt/esp_ble_mesh/api/models/include/esp_ble_mesh_config_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_config_model_api.h"
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


#### esp_ble_mesh_register_config_client_callback


```


esp_err_t esp_ble_mesh_register_config_client_callback(esp_ble_mesh_cfg_client_cb_t callback)
Register BLE Mesh Config Client Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_config_server_callback


```


esp_err_t esp_ble_mesh_register_config_server_callback(esp_ble_mesh_cfg_server_cb_t callback)
Register BLE Mesh Config Server Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_config_client_get_state


```


esp_err_t esp_ble_mesh_config_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_cfg_client_get_state_t *get_state)
Get the value of Config Server Model states using the Config Client Model get messages. 

Note
If you want to find the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_opcode_config_client_get_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
get_state -- [in] Pointer to a union, each kind of opcode corresponds to one structure inside. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_config_client_set_state


```


esp_err_t esp_ble_mesh_config_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_cfg_client_set_state_t *set_state)
Set the value of the Configuration Server Model states using the Config Client Model set messages. 

Note
If you want to find the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_opcode_config_client_set_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
set_state -- [in] Pointer to a union, each kind of opcode corresponds to one structure inside. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_cfg_client_get_state_t


```


union esp_ble_mesh_cfg_client_get_state_t

#include <esp_ble_mesh_config_model_api.h>
For ESP_BLE_MESH_MODEL_OP_BEACON_GET ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_GET ESP_BLE_MESH_MODEL_OP_GATT_PROXY_GET ESP_BLE_MESH_MODEL_OP_RELAY_GET ESP_BLE_MESH_MODEL_OP_MODEL_PUB_GET ESP_BLE_MESH_MODEL_OP_FRIEND_GET ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_GET ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_GET the get_state parameter in the esp_ble_mesh_config_client_get_state function should not be set to NULL. 

Public Members
model_pub_get

esp_ble_mesh_cfg_model_pub_get_t model_pub_get
For ESP_BLE_MESH_MODEL_OP_MODEL_PUB_GET. 

comp_data_get

esp_ble_mesh_cfg_composition_data_get_t comp_data_get
For ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET. 

sig_model_sub_get

esp_ble_mesh_cfg_sig_model_sub_get_t sig_model_sub_get
For ESP_BLE_MESH_MODEL_OP_SIG_MODEL_SUB_GET 

vnd_model_sub_get

esp_ble_mesh_cfg_vnd_model_sub_get_t vnd_model_sub_get
For ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_SUB_GET 

app_key_get

esp_ble_mesh_cfg_app_key_get_t app_key_get
For ESP_BLE_MESH_MODEL_OP_APP_KEY_GET. 

node_identity_get

esp_ble_mesh_cfg_node_identity_get_t node_identity_get
For ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_GET. 

sig_model_app_get

esp_ble_mesh_cfg_sig_model_app_get_t sig_model_app_get
For ESP_BLE_MESH_MODEL_OP_SIG_MODEL_APP_GET 

vnd_model_app_get

esp_ble_mesh_cfg_vnd_model_app_get_t vnd_model_app_get
For ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_APP_GET 

kr_phase_get

esp_ble_mesh_cfg_kr_phase_get_t kr_phase_get
For ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_GET 

lpn_pollto_get

esp_ble_mesh_cfg_lpn_polltimeout_get_t lpn_pollto_get
For ESP_BLE_MESH_MODEL_OP_LPN_POLLTIMEOUT_GET 



```

#### esp_ble_mesh_cfg_client_set_state_t


```


union esp_ble_mesh_cfg_client_set_state_t

#include <esp_ble_mesh_config_model_api.h>
For ESP_BLE_MESH_MODEL_OP_BEACON_SET ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_SET ESP_BLE_MESH_MODEL_OP_GATT_PROXY_SET ESP_BLE_MESH_MODEL_OP_RELAY_SET ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_ADD ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_DELETE ESP_BLE_MESH_MODEL_OP_MODEL_SUB_OVERWRITE ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_OVERWRITE ESP_BLE_MESH_MODEL_OP_NET_KEY_ADD ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND ESP_BLE_MESH_MODEL_OP_NODE_RESET ESP_BLE_MESH_MODEL_OP_FRIEND_SET ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_SET ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_SET the set_state parameter in the esp_ble_mesh_config_client_set_state function should not be set to NULL. 

Public Members
beacon_set

esp_ble_mesh_cfg_beacon_set_t beacon_set
For ESP_BLE_MESH_MODEL_OP_BEACON_SET 

default_ttl_set

esp_ble_mesh_cfg_default_ttl_set_t default_ttl_set
For ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_SET 

friend_set

esp_ble_mesh_cfg_friend_set_t friend_set
For ESP_BLE_MESH_MODEL_OP_FRIEND_SET 

gatt_proxy_set

esp_ble_mesh_cfg_gatt_proxy_set_t gatt_proxy_set
For ESP_BLE_MESH_MODEL_OP_GATT_PROXY_SET 

relay_set

esp_ble_mesh_cfg_relay_set_t relay_set
For ESP_BLE_MESH_MODEL_OP_RELAY_SET 

net_key_add

esp_ble_mesh_cfg_net_key_add_t net_key_add
For ESP_BLE_MESH_MODEL_OP_NET_KEY_ADD 

app_key_add

esp_ble_mesh_cfg_app_key_add_t app_key_add
For ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD 

model_app_bind

esp_ble_mesh_cfg_model_app_bind_t model_app_bind
For ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND 

model_pub_set

esp_ble_mesh_cfg_model_pub_set_t model_pub_set
For ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET 

model_sub_add

esp_ble_mesh_cfg_model_sub_add_t model_sub_add
For ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD 

model_sub_delete

esp_ble_mesh_cfg_model_sub_delete_t model_sub_delete
For ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE 

model_sub_overwrite

esp_ble_mesh_cfg_model_sub_overwrite_t model_sub_overwrite
For ESP_BLE_MESH_MODEL_OP_MODEL_SUB_OVERWRITE 

model_sub_va_add

esp_ble_mesh_cfg_model_sub_va_add_t model_sub_va_add
For ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_ADD 

model_sub_va_delete

esp_ble_mesh_cfg_model_sub_va_delete_t model_sub_va_delete
For ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_DELETE 

model_sub_va_overwrite

esp_ble_mesh_cfg_model_sub_va_overwrite_t model_sub_va_overwrite
For ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_OVERWRITE 

heartbeat_pub_set

esp_ble_mesh_cfg_heartbeat_pub_set_t heartbeat_pub_set
For ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_SET 

heartbeat_sub_set

esp_ble_mesh_cfg_heartbeat_sub_set_t heartbeat_sub_set
For ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_SET 

model_pub_va_set

esp_ble_mesh_cfg_model_pub_va_set_t model_pub_va_set
For ESP_BLE_MESH_MODEL_OP_MODEL_PUB_VIRTUAL_ADDR_SET 

model_sub_delete_all

esp_ble_mesh_cfg_model_sub_delete_all_t model_sub_delete_all
For ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE_ALL 

net_key_update

esp_ble_mesh_cfg_net_key_update_t net_key_update
For ESP_BLE_MESH_MODEL_OP_NET_KEY_UPDATE 

net_key_delete

esp_ble_mesh_cfg_net_key_delete_t net_key_delete
For ESP_BLE_MESH_MODEL_OP_NET_KEY_DELETE 

app_key_update

esp_ble_mesh_cfg_app_key_update_t app_key_update
For ESP_BLE_MESH_MODEL_OP_APP_KEY_UPDATE 

app_key_delete

esp_ble_mesh_cfg_app_key_delete_t app_key_delete
For ESP_BLE_MESH_MODEL_OP_APP_KEY_DELETE 

node_identity_set

esp_ble_mesh_cfg_node_identity_set_t node_identity_set
For ESP_BLE_MESH_MODEL_OP_NODE_IDENTITY_SET 

model_app_unbind

esp_ble_mesh_cfg_model_app_unbind_t model_app_unbind
For ESP_BLE_MESH_MODEL_OP_MODEL_APP_UNBIND 

kr_phase_set

esp_ble_mesh_cfg_kr_phase_set_t kr_phase_set
For ESP_BLE_MESH_MODEL_OP_KEY_REFRESH_PHASE_SET 

net_transmit_set

esp_ble_mesh_cfg_net_transmit_set_t net_transmit_set
For ESP_BLE_MESH_MODEL_OP_NETWORK_TRANSMIT_SET 



```

#### esp_ble_mesh_cfg_client_common_cb_param_t


```


union esp_ble_mesh_cfg_client_common_cb_param_t

#include <esp_ble_mesh_config_model_api.h>
Configuration Client Model received message union. 

Public Members
beacon_status

esp_ble_mesh_cfg_beacon_status_cb_t beacon_status
The beacon status value 

comp_data_status

esp_ble_mesh_cfg_comp_data_status_cb_t comp_data_status
The composition data status value 

default_ttl_status

esp_ble_mesh_cfg_default_ttl_status_cb_t default_ttl_status
The default_ttl status value 

gatt_proxy_status

esp_ble_mesh_cfg_gatt_proxy_status_cb_t gatt_proxy_status
The gatt_proxy status value 

relay_status

esp_ble_mesh_cfg_relay_status_cb_t relay_status
The relay status value 

model_pub_status

esp_ble_mesh_cfg_model_pub_status_cb_t model_pub_status
The model publication status value 

model_sub_status

esp_ble_mesh_cfg_model_sub_status_cb_t model_sub_status
The model subscription status value 

netkey_status

esp_ble_mesh_cfg_net_key_status_cb_t netkey_status
The netkey status value 

appkey_status

esp_ble_mesh_cfg_app_key_status_cb_t appkey_status
The appkey status value 

model_app_status

esp_ble_mesh_cfg_mod_app_status_cb_t model_app_status
The model app status value 

friend_status

esp_ble_mesh_cfg_friend_status_cb_t friend_status
The friend status value 

heartbeat_pub_status

esp_ble_mesh_cfg_hb_pub_status_cb_t heartbeat_pub_status
The heartbeat publication status value 

heartbeat_sub_status

esp_ble_mesh_cfg_hb_sub_status_cb_t heartbeat_sub_status
The heartbeat subscription status value 

net_transmit_status

esp_ble_mesh_cfg_net_trans_status_cb_t net_transmit_status
The network transmit status value 

model_sub_list

esp_ble_mesh_cfg_model_sub_list_cb_t model_sub_list
The model subscription list value 

netkey_list

esp_ble_mesh_cfg_net_key_list_cb_t netkey_list
The network key index list value 

appkey_list

esp_ble_mesh_cfg_app_key_list_cb_t appkey_list
The application key index list value 

node_identity_status

esp_ble_mesh_cfg_node_id_status_cb_t node_identity_status
The node identity status value 

model_app_list

esp_ble_mesh_cfg_model_app_list_cb_t model_app_list
The model application key index list value 

kr_phase_status

esp_ble_mesh_cfg_kr_phase_status_cb_t kr_phase_status
The key refresh phase status value 

lpn_timeout_status

esp_ble_mesh_cfg_lpn_pollto_status_cb_t lpn_timeout_status
The low power node poll timeout status value 



```

#### esp_ble_mesh_cfg_server_state_change_t


```


union esp_ble_mesh_cfg_server_state_change_t

#include <esp_ble_mesh_config_model_api.h>
Configuration Server model state change value union. 

Public Members
mod_pub_set

esp_ble_mesh_state_change_cfg_mod_pub_set_t mod_pub_set
The recv_op in ctx can be used to decide which state is changed. Config Model Publication Set 

mod_pub_va_set

esp_ble_mesh_state_change_cfg_mod_pub_va_set_t mod_pub_va_set
Config Model Publication Virtual Address Set 

mod_sub_add

esp_ble_mesh_state_change_cfg_model_sub_add_t mod_sub_add
Config Model Subscription Add 

mod_sub_delete

esp_ble_mesh_state_change_cfg_model_sub_delete_t mod_sub_delete
Config Model Subscription Delete 

netkey_add

esp_ble_mesh_state_change_cfg_netkey_add_t netkey_add
Config NetKey Add 

netkey_update

esp_ble_mesh_state_change_cfg_netkey_update_t netkey_update
Config NetKey Update 

netkey_delete

esp_ble_mesh_state_change_cfg_netkey_delete_t netkey_delete
Config NetKey Delete 

appkey_add

esp_ble_mesh_state_change_cfg_appkey_add_t appkey_add
Config AppKey Add 

appkey_update

esp_ble_mesh_state_change_cfg_appkey_update_t appkey_update
Config AppKey Update 

appkey_delete

esp_ble_mesh_state_change_cfg_appkey_delete_t appkey_delete
Config AppKey Delete 

mod_app_bind

esp_ble_mesh_state_change_cfg_model_app_bind_t mod_app_bind
Config Model App Bind 

mod_app_unbind

esp_ble_mesh_state_change_cfg_model_app_unbind_t mod_app_unbind
Config Model App Unbind 

kr_phase_set

esp_ble_mesh_state_change_cfg_kr_phase_set_t kr_phase_set
Config Key Refresh Phase Set 



```

#### esp_ble_mesh_cfg_server_cb_value_t


```


union esp_ble_mesh_cfg_server_cb_value_t

#include <esp_ble_mesh_config_model_api.h>
Configuration Server model callback value union. 

Public Members
state_change

esp_ble_mesh_cfg_server_state_change_t state_change
ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_cfg_srv_t


```


struct esp_ble_mesh_cfg_srv_t
Configuration Server Model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Configuration Server Model 

net_transmit

uint8_t net_transmit
Network Transmit state 

relay

uint8_t relay
Relay Mode state 

relay_retransmit

uint8_t relay_retransmit
Relay Retransmit state 

beacon

uint8_t beacon
Secure Network Beacon state 

gatt_proxy

uint8_t gatt_proxy
GATT Proxy state 

friend_state

uint8_t friend_state
Friend state 

default_ttl

uint8_t default_ttl
Default TTL 

timer

struct k_delayed_work timer
Heartbeat Publication timer 

dst

uint16_t dst
Destination address for Heartbeat messages 

count

uint16_t count
Number of Heartbeat messages to be sent
Number of Heartbeat messages received 

period

uint8_t period
Period for sending Heartbeat messages 

ttl

uint8_t ttl
TTL to be used when sending Heartbeat messages 

feature

uint16_t feature
Bit field indicating features that trigger Heartbeat messages when changed 

net_idx

uint16_t net_idx
NetKey Index used by Heartbeat Publication 

heartbeat_pub

struct esp_ble_mesh_cfg_srv_t::[anonymous] heartbeat_pub
Heartbeat Publication 

expiry

int64_t expiry
Timestamp when Heartbeat subscription period is expired 

src

uint16_t src
Source address for Heartbeat messages 

min_hops

uint8_t min_hops
Minimum hops when receiving Heartbeat messages 

max_hops

uint8_t max_hops
Maximum hops when receiving Heartbeat messages 

heartbeat_recv_cb

esp_ble_mesh_cb_t heartbeat_recv_cb
Optional heartbeat subscription tracking function 

heartbeat_sub

struct esp_ble_mesh_cfg_srv_t::[anonymous] heartbeat_sub
Heartbeat Subscription 



```

#### esp_ble_mesh_cfg_composition_data_get_t


```


struct esp_ble_mesh_cfg_composition_data_get_t
Parameters of Config Composition Data Get. 

Public Members
page

uint8_t page
Page number of the Composition Data. 



```

#### esp_ble_mesh_cfg_model_pub_get_t


```


struct esp_ble_mesh_cfg_model_pub_get_t
Parameters of Config Model Publication Get. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_sig_model_sub_get_t


```


struct esp_ble_mesh_cfg_sig_model_sub_get_t
Parameters of Config SIG Model Subscription Get. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_id

uint16_t model_id
The model id 



```

#### esp_ble_mesh_cfg_vnd_model_sub_get_t


```


struct esp_ble_mesh_cfg_vnd_model_sub_get_t
Parameters of Config Vendor Model Subscription Get. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_app_key_get_t


```


struct esp_ble_mesh_cfg_app_key_get_t
Parameters of Config AppKey Get. 

Public Members
net_idx

uint16_t net_idx
The network key index 



```

#### esp_ble_mesh_cfg_node_identity_get_t


```


struct esp_ble_mesh_cfg_node_identity_get_t
Parameters of Config Node Identity Get. 

Public Members
net_idx

uint16_t net_idx
The network key index 



```

#### esp_ble_mesh_cfg_sig_model_app_get_t


```


struct esp_ble_mesh_cfg_sig_model_app_get_t
Parameters of Config SIG Model App Get. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_id

uint16_t model_id
The model id 



```

#### esp_ble_mesh_cfg_vnd_model_app_get_t


```


struct esp_ble_mesh_cfg_vnd_model_app_get_t
Parameters of Config Vendor Model App Get. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_kr_phase_get_t


```


struct esp_ble_mesh_cfg_kr_phase_get_t
Parameters of Config Key Refresh Phase Get. 

Public Members
net_idx

uint16_t net_idx
The network key index 



```

#### esp_ble_mesh_cfg_lpn_polltimeout_get_t


```


struct esp_ble_mesh_cfg_lpn_polltimeout_get_t
Parameters of Config Low Power Node PollTimeout Get. 

Public Members
lpn_addr

uint16_t lpn_addr
The unicast address of the Low Power node 



```

#### esp_ble_mesh_cfg_beacon_set_t


```


struct esp_ble_mesh_cfg_beacon_set_t
Parameters of Config Beacon Set. 

Public Members
beacon

uint8_t beacon
New Secure Network Beacon state 



```

#### esp_ble_mesh_cfg_default_ttl_set_t


```


struct esp_ble_mesh_cfg_default_ttl_set_t
Parameters of Config Default TTL Set. 

Public Members
ttl

uint8_t ttl
The default TTL state value 



```

#### esp_ble_mesh_cfg_friend_set_t


```


struct esp_ble_mesh_cfg_friend_set_t
Parameters of Config Friend Set. 

Public Members
friend_state

uint8_t friend_state
The friend state value 



```

#### esp_ble_mesh_cfg_gatt_proxy_set_t


```


struct esp_ble_mesh_cfg_gatt_proxy_set_t
Parameters of Config GATT Proxy Set. 

Public Members
gatt_proxy

uint8_t gatt_proxy
The GATT Proxy state value 



```

#### esp_ble_mesh_cfg_relay_set_t


```


struct esp_ble_mesh_cfg_relay_set_t
Parameters of Config Relay Set. 

Public Members
relay

uint8_t relay
The relay value 

relay_retransmit

uint8_t relay_retransmit
The relay retransmit value 



```

#### esp_ble_mesh_cfg_net_key_add_t


```


struct esp_ble_mesh_cfg_net_key_add_t
Parameters of Config NetKey Add. 

Public Members
net_idx

uint16_t net_idx
The network key index 

net_key

uint8_t net_key[16]
The network key value 



```

#### esp_ble_mesh_cfg_app_key_add_t


```


struct esp_ble_mesh_cfg_app_key_add_t
Parameters of Config AppKey Add. 

Public Members
net_idx

uint16_t net_idx
The network key index 

app_idx

uint16_t app_idx
The app key index 

app_key

uint8_t app_key[16]
The app key value 



```

#### esp_ble_mesh_cfg_model_app_bind_t


```


struct esp_ble_mesh_cfg_model_app_bind_t
Parameters of Config Model App Bind. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_app_idx

uint16_t model_app_idx
Index of the app key to bind with the model 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_pub_set_t


```


struct esp_ble_mesh_cfg_model_pub_set_t
Parameters of Config Model Publication Set. 

Public Members
element_addr

uint16_t element_addr
The element address 

publish_addr

uint16_t publish_addr
Value of the publish address 

publish_app_idx

uint16_t publish_app_idx
Index of the application key 

cred_flag

bool cred_flag
Value of the Friendship Credential Flag 

publish_ttl

uint8_t publish_ttl
Default TTL value for the publishing messages 

publish_period

uint8_t publish_period
Period for periodic status publishing 

publish_retransmit

uint8_t publish_retransmit
Number of retransmissions and number of 50-millisecond steps between retransmissions 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_sub_add_t


```


struct esp_ble_mesh_cfg_model_sub_add_t
Parameters of Config Model Subscription Add. 

Public Members
element_addr

uint16_t element_addr
The element address 

sub_addr

uint16_t sub_addr
The address to be added to the Subscription List 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_sub_delete_t


```


struct esp_ble_mesh_cfg_model_sub_delete_t
Parameters of Config Model Subscription Delete. 

Public Members
element_addr

uint16_t element_addr
The element address 

sub_addr

uint16_t sub_addr
The address to be removed from the Subscription List 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_sub_overwrite_t


```


struct esp_ble_mesh_cfg_model_sub_overwrite_t
Parameters of Config Model Subscription Overwrite. 

Public Members
element_addr

uint16_t element_addr
The element address 

sub_addr

uint16_t sub_addr
The address to be added to the Subscription List 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_sub_va_add_t


```


struct esp_ble_mesh_cfg_model_sub_va_add_t
Parameters of Config Model Subscription Virtual Address Add. 

Public Members
element_addr

uint16_t element_addr
The element address 

label_uuid

uint8_t label_uuid[16]
The Label UUID of the virtual address to be added to the Subscription List 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_sub_va_delete_t


```


struct esp_ble_mesh_cfg_model_sub_va_delete_t
Parameters of Config Model Subscription Virtual Address Delete. 

Public Members
element_addr

uint16_t element_addr
The element address 

label_uuid

uint8_t label_uuid[16]
The Label UUID of the virtual address to be removed from the Subscription List 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_sub_va_overwrite_t


```


struct esp_ble_mesh_cfg_model_sub_va_overwrite_t
Parameters of Config Model Subscription Virtual Address Overwrite. 

Public Members
element_addr

uint16_t element_addr
The element address 

label_uuid

uint8_t label_uuid[16]
The Label UUID of the virtual address to be added to the Subscription List 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_pub_va_set_t


```


struct esp_ble_mesh_cfg_model_pub_va_set_t
Parameters of Config Model Publication Virtual Address Set. 

Public Members
element_addr

uint16_t element_addr
The element address 

label_uuid

uint8_t label_uuid[16]
Value of the Label UUID publish address 

publish_app_idx

uint16_t publish_app_idx
Index of the application key 

cred_flag

bool cred_flag
Value of the Friendship Credential Flag 

publish_ttl

uint8_t publish_ttl
Default TTL value for the publishing messages 

publish_period

uint8_t publish_period
Period for periodic status publishing 

publish_retransmit

uint8_t publish_retransmit
Number of retransmissions and number of 50-millisecond steps between retransmissions 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_model_sub_delete_all_t


```


struct esp_ble_mesh_cfg_model_sub_delete_all_t
Parameters of Config Model Subscription Delete All. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_net_key_update_t


```


struct esp_ble_mesh_cfg_net_key_update_t
Parameters of Config NetKey Update. 

Public Members
net_idx

uint16_t net_idx
The network key index 

net_key

uint8_t net_key[16]
The network key value 



```

#### esp_ble_mesh_cfg_net_key_delete_t


```


struct esp_ble_mesh_cfg_net_key_delete_t
Parameters of Config NetKey Delete. 

Public Members
net_idx

uint16_t net_idx
The network key index 



```

#### esp_ble_mesh_cfg_app_key_update_t


```


struct esp_ble_mesh_cfg_app_key_update_t
Parameters of Config AppKey Update. 

Public Members
net_idx

uint16_t net_idx
The network key index 

app_idx

uint16_t app_idx
The app key index 

app_key

uint8_t app_key[16]
The app key value 



```

#### esp_ble_mesh_cfg_app_key_delete_t


```


struct esp_ble_mesh_cfg_app_key_delete_t
Parameters of Config AppKey Delete. 

Public Members
net_idx

uint16_t net_idx
The network key index 

app_idx

uint16_t app_idx
The app key index 



```

#### esp_ble_mesh_cfg_node_identity_set_t


```


struct esp_ble_mesh_cfg_node_identity_set_t
Parameters of Config Node Identity Set. 

Public Members
net_idx

uint16_t net_idx
The network key index 

identity

uint8_t identity
New Node Identity state 



```

#### esp_ble_mesh_cfg_model_app_unbind_t


```


struct esp_ble_mesh_cfg_model_app_unbind_t
Parameters of Config Model App Unbind. 

Public Members
element_addr

uint16_t element_addr
The element address 

model_app_idx

uint16_t model_app_idx
Index of the app key to bind with the model 

model_id

uint16_t model_id
The model id 

company_id

uint16_t company_id
The company id, if not a vendor model, shall set to 0xFFFF 



```

#### esp_ble_mesh_cfg_kr_phase_set_t


```


struct esp_ble_mesh_cfg_kr_phase_set_t
Parameters of Config Key Refresh Phase Set. 

Public Members
net_idx

uint16_t net_idx
The network key index 

transition

uint8_t transition
New Key Refresh Phase Transition 



```

#### esp_ble_mesh_cfg_net_transmit_set_t


```


struct esp_ble_mesh_cfg_net_transmit_set_t
Parameters of Config Network Transmit Set. 

Public Members
net_transmit

uint8_t net_transmit
Network Transmit State 



```

#### esp_ble_mesh_cfg_heartbeat_pub_set_t


```


struct esp_ble_mesh_cfg_heartbeat_pub_set_t
Parameters of Config Model Heartbeat Publication Set. 

Public Members
dst

uint16_t dst
Destination address for Heartbeat messages 

count

uint8_t count
Number of Heartbeat messages to be sent 

period

uint8_t period
Period for sending Heartbeat messages 

ttl

uint8_t ttl
TTL to be used when sending Heartbeat messages 

feature

uint16_t feature
Bit field indicating features that trigger Heartbeat messages when changed 

net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_cfg_heartbeat_sub_set_t


```


struct esp_ble_mesh_cfg_heartbeat_sub_set_t
Parameters of Config Model Heartbeat Subscription Set. 

Public Members
src

uint16_t src
Source address for Heartbeat messages 

dst

uint16_t dst
Destination address for Heartbeat messages 

period

uint8_t period
Period for receiving Heartbeat messages 



```

#### esp_ble_mesh_cfg_beacon_status_cb_t


```


struct esp_ble_mesh_cfg_beacon_status_cb_t
Parameter of Config Beacon Status 

Public Members
beacon

uint8_t beacon
Secure Network Beacon state value 



```

#### esp_ble_mesh_cfg_comp_data_status_cb_t


```


struct esp_ble_mesh_cfg_comp_data_status_cb_t
Parameters of Config Composition Data Status 

Public Members
page

uint8_t page
Page number of the Composition Data 

composition_data

struct net_buf_simple *composition_data
Pointer to Composition Data for the identified page 



```

#### esp_ble_mesh_cfg_default_ttl_status_cb_t


```


struct esp_ble_mesh_cfg_default_ttl_status_cb_t
Parameter of Config Default TTL Status 

Public Members
default_ttl

uint8_t default_ttl
Default TTL state value 



```

#### esp_ble_mesh_cfg_gatt_proxy_status_cb_t


```


struct esp_ble_mesh_cfg_gatt_proxy_status_cb_t
Parameter of Config GATT Proxy Status 

Public Members
gatt_proxy

uint8_t gatt_proxy
GATT Proxy state value 



```

#### esp_ble_mesh_cfg_relay_status_cb_t


```


struct esp_ble_mesh_cfg_relay_status_cb_t
Parameters of Config Relay Status 

Public Members
relay

uint8_t relay
Relay state value 

retransmit

uint8_t retransmit
Relay retransmit value(number of retransmissions and number of 10-millisecond steps between retransmissions) 



```

#### esp_ble_mesh_cfg_model_pub_status_cb_t


```


struct esp_ble_mesh_cfg_model_pub_status_cb_t
Parameters of Config Model Publication Status 

Public Members
status

uint8_t status
Status Code for the request message 

element_addr

uint16_t element_addr
Address of the element 

publish_addr

uint16_t publish_addr
Value of the publish address 

app_idx

uint16_t app_idx
Index of the application key 

cred_flag

bool cred_flag
Value of the Friendship Credential Flag 

ttl

uint8_t ttl
Default TTL value for the outgoing messages 

period

uint8_t period
Period for periodic status publishing 

transmit

uint8_t transmit
Number of retransmissions and number of 50-millisecond steps between retransmissions 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_cfg_model_sub_status_cb_t


```


struct esp_ble_mesh_cfg_model_sub_status_cb_t
Parameters of Config Model Subscription Status 

Public Members
status

uint8_t status
Status Code for the request message 

element_addr

uint16_t element_addr
Address of the element 

sub_addr

uint16_t sub_addr
Value of the address 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_cfg_net_key_status_cb_t


```


struct esp_ble_mesh_cfg_net_key_status_cb_t
Parameters of Config NetKey Status 

Public Members
status

uint8_t status
Status Code for the request message 

net_idx

uint16_t net_idx
Index of the NetKey 



```

#### esp_ble_mesh_cfg_app_key_status_cb_t


```


struct esp_ble_mesh_cfg_app_key_status_cb_t
Parameters of Config AppKey Status 

Public Members
status

uint8_t status
Status Code for the request message 

net_idx

uint16_t net_idx
Index of the NetKey 

app_idx

uint16_t app_idx
Index of the application key 



```

#### esp_ble_mesh_cfg_mod_app_status_cb_t


```


struct esp_ble_mesh_cfg_mod_app_status_cb_t
Parameters of Config Model App Status 

Public Members
status

uint8_t status
Status Code for the request message 

element_addr

uint16_t element_addr
Address of the element 

app_idx

uint16_t app_idx
Index of the application key 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_cfg_friend_status_cb_t


```


struct esp_ble_mesh_cfg_friend_status_cb_t
Parameter of Config Friend Status 

Public Members
friend_state

uint8_t friend_state
Friend state value 



```

#### esp_ble_mesh_cfg_hb_pub_status_cb_t


```


struct esp_ble_mesh_cfg_hb_pub_status_cb_t
Parameters of Config Heartbeat Publication Status 

Public Members
status

uint8_t status
Status Code for the request message 

dst

uint16_t dst
Destination address for Heartbeat messages 

count

uint8_t count
Number of Heartbeat messages remaining to be sent 

period

uint8_t period
Period for sending Heartbeat messages 

ttl

uint8_t ttl
TTL to be used when sending Heartbeat messages 

features

uint16_t features
Features that trigger Heartbeat messages when changed 

net_idx

uint16_t net_idx
Index of the NetKey 



```

#### esp_ble_mesh_cfg_hb_sub_status_cb_t


```


struct esp_ble_mesh_cfg_hb_sub_status_cb_t
Parameters of Config Heartbeat Subscription Status 

Public Members
status

uint8_t status
Status Code for the request message 

src

uint16_t src
Source address for Heartbeat messages 

dst

uint16_t dst
Destination address for Heartbeat messages 

period

uint8_t period
Remaining Period for processing Heartbeat messages 

count

uint8_t count
Number of Heartbeat messages received 

min_hops

uint8_t min_hops
Minimum hops when receiving Heartbeat messages 

max_hops

uint8_t max_hops
Maximum hops when receiving Heartbeat messages 



```

#### esp_ble_mesh_cfg_net_trans_status_cb_t


```


struct esp_ble_mesh_cfg_net_trans_status_cb_t
Parameters of Config Network Transmit Status 

Public Members
net_trans_count

uint8_t net_trans_count
Number of transmissions for each Network PDU originating from the node 

net_trans_step

uint8_t net_trans_step
Maximum hops when receiving Heartbeat messages 



```

#### esp_ble_mesh_cfg_model_sub_list_cb_t


```


struct esp_ble_mesh_cfg_model_sub_list_cb_t
Parameters of Config SIG/Vendor Subscription List 

Public Members
status

uint8_t status
Status Code for the request message 

element_addr

uint16_t element_addr
Address of the element 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 

sub_addr

struct net_buf_simple *sub_addr
A block of all addresses from the Subscription List 



```

#### esp_ble_mesh_cfg_net_key_list_cb_t


```


struct esp_ble_mesh_cfg_net_key_list_cb_t
Parameter of Config NetKey List 

Public Members
net_idx

struct net_buf_simple *net_idx
A list of NetKey Indexes known to the node 



```

#### esp_ble_mesh_cfg_app_key_list_cb_t


```


struct esp_ble_mesh_cfg_app_key_list_cb_t
Parameters of Config AppKey List 

Public Members
status

uint8_t status
Status Code for the request message 

net_idx

uint16_t net_idx
NetKey Index of the NetKey that the AppKeys are bound to 

app_idx

struct net_buf_simple *app_idx
A list of AppKey indexes that are bound to the NetKey identified by NetKeyIndex 



```

#### esp_ble_mesh_cfg_node_id_status_cb_t


```


struct esp_ble_mesh_cfg_node_id_status_cb_t
Parameters of Config Node Identity Status 

Public Members
status

uint8_t status
Status Code for the request message 

net_idx

uint16_t net_idx
Index of the NetKey 

identity

uint8_t identity
Node Identity state 



```

#### esp_ble_mesh_cfg_model_app_list_cb_t


```


struct esp_ble_mesh_cfg_model_app_list_cb_t
Parameters of Config SIG/Vendor Model App List 

Public Members
status

uint8_t status
Status Code for the request message 

element_addr

uint16_t element_addr
Address of the element 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 

app_idx

struct net_buf_simple *app_idx
All AppKey indexes bound to the Model 



```

#### esp_ble_mesh_cfg_kr_phase_status_cb_t


```


struct esp_ble_mesh_cfg_kr_phase_status_cb_t
Parameters of Config Key Refresh Phase Status 

Public Members
status

uint8_t status
Status Code for the request message 

net_idx

uint16_t net_idx
Index of the NetKey 

phase

uint8_t phase
Key Refresh Phase state 



```

#### esp_ble_mesh_cfg_lpn_pollto_status_cb_t


```


struct esp_ble_mesh_cfg_lpn_pollto_status_cb_t
Parameters of Config Low Power Node PollTimeout Status 

Public Members
lpn_addr

uint16_t lpn_addr
The unicast address of the Low Power node 

poll_timeout

int32_t poll_timeout
The current value of the PollTimeout timer of the Low Power node 



```

#### esp_ble_mesh_cfg_client_cb_param_t


```


struct esp_ble_mesh_cfg_client_cb_param_t
Configuration Client Model callback parameters 

Public Members
error_code

int error_code
Appropriate error code 

params

esp_ble_mesh_client_common_param_t *params
The client common parameters 

status_cb

esp_ble_mesh_cfg_client_common_cb_param_t status_cb
The config status message callback values 



```

#### esp_ble_mesh_state_change_cfg_mod_pub_set_t


```


struct esp_ble_mesh_state_change_cfg_mod_pub_set_t
Configuration Server model related context. 
Parameters of Config Model Publication Set 

Public Members
element_addr

uint16_t element_addr
Element Address 

pub_addr

uint16_t pub_addr
Publish Address 

app_idx

uint16_t app_idx
AppKey Index 

cred_flag

bool cred_flag
Friendship Credential Flag 

pub_ttl

uint8_t pub_ttl
Publish TTL 

pub_period

uint8_t pub_period
Publish Period 

pub_retransmit

uint8_t pub_retransmit
Publish Retransmit 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_state_change_cfg_mod_pub_va_set_t


```


struct esp_ble_mesh_state_change_cfg_mod_pub_va_set_t
Parameters of Config Model Publication Virtual Address Set 

Public Members
element_addr

uint16_t element_addr
Element Address 

label_uuid

uint8_t label_uuid[16]
Label UUID 

app_idx

uint16_t app_idx
AppKey Index 

cred_flag

bool cred_flag
Friendship Credential Flag 

pub_ttl

uint8_t pub_ttl
Publish TTL 

pub_period

uint8_t pub_period
Publish Period 

pub_retransmit

uint8_t pub_retransmit
Publish Retransmit 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_state_change_cfg_model_sub_add_t


```


struct esp_ble_mesh_state_change_cfg_model_sub_add_t
Parameters of Config Model Subscription Add 

Public Members
element_addr

uint16_t element_addr
Element Address 

sub_addr

uint16_t sub_addr
Subscription Address 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_state_change_cfg_model_sub_delete_t


```


struct esp_ble_mesh_state_change_cfg_model_sub_delete_t
Parameters of Config Model Subscription Delete 

Public Members
element_addr

uint16_t element_addr
Element Address 

sub_addr

uint16_t sub_addr
Subscription Address 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_state_change_cfg_netkey_add_t


```


struct esp_ble_mesh_state_change_cfg_netkey_add_t
Parameters of Config NetKey Add 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

net_key

uint8_t net_key[16]
NetKey 



```

#### esp_ble_mesh_state_change_cfg_netkey_update_t


```


struct esp_ble_mesh_state_change_cfg_netkey_update_t
Parameters of Config NetKey Update 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

net_key

uint8_t net_key[16]
NetKey 



```

#### esp_ble_mesh_state_change_cfg_netkey_delete_t


```


struct esp_ble_mesh_state_change_cfg_netkey_delete_t
Parameter of Config NetKey Delete 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_state_change_cfg_appkey_add_t


```


struct esp_ble_mesh_state_change_cfg_appkey_add_t
Parameters of Config AppKey Add 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

app_idx

uint16_t app_idx
AppKey Index 

app_key

uint8_t app_key[16]
AppKey 



```

#### esp_ble_mesh_state_change_cfg_appkey_update_t


```


struct esp_ble_mesh_state_change_cfg_appkey_update_t
Parameters of Config AppKey Update 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

app_idx

uint16_t app_idx
AppKey Index 

app_key

uint8_t app_key[16]
AppKey 



```

#### esp_ble_mesh_state_change_cfg_appkey_delete_t


```


struct esp_ble_mesh_state_change_cfg_appkey_delete_t
Parameters of Config AppKey Delete 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

app_idx

uint16_t app_idx
AppKey Index 



```

#### esp_ble_mesh_state_change_cfg_model_app_bind_t


```


struct esp_ble_mesh_state_change_cfg_model_app_bind_t
Parameters of Config Model App Bind 

Public Members
element_addr

uint16_t element_addr
Element Address 

app_idx

uint16_t app_idx
AppKey Index 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_state_change_cfg_model_app_unbind_t


```


struct esp_ble_mesh_state_change_cfg_model_app_unbind_t
Parameters of Config Model App Unbind 

Public Members
element_addr

uint16_t element_addr
Element Address 

app_idx

uint16_t app_idx
AppKey Index 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_state_change_cfg_kr_phase_set_t


```


struct esp_ble_mesh_state_change_cfg_kr_phase_set_t
Parameters of Config Key Refresh Phase Set 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

kr_phase

uint8_t kr_phase
New Key Refresh Phase Transition 



```

#### esp_ble_mesh_cfg_server_cb_param_t


```


struct esp_ble_mesh_cfg_server_cb_param_t
Configuration Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_cfg_server_cb_value_t value
Value of the received configuration messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_CFG_SRV


```


ESP_BLE_MESH_MODEL_CFG_SRV(srv_data)
Define a new Config Server Model. 

Note
The Config Server Model can only be included by a Primary Element.


Parameters

srv_data -- Pointer to a unique Config Server Model user_data.


Returns
New Config Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_CFG_CLI


```


ESP_BLE_MESH_MODEL_CFG_CLI(cli_data)
Define a new Config Client Model. 

Note
The Config Client Model can only be included by a Primary Element.


Parameters

cli_data -- Pointer to a unique struct esp_ble_mesh_client_t.


Returns
New Config Client Model instance. 



```



### Type Definitions


#### esp_ble_mesh_cfg_client_cb_t


```


typedef void (*esp_ble_mesh_cfg_client_cb_t)(esp_ble_mesh_cfg_client_cb_event_t event, esp_ble_mesh_cfg_client_cb_param_t *param)
Bluetooth Mesh Config Client and Server Model functions. 
Configuration Client Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_cfg_server_cb_t


```


typedef void (*esp_ble_mesh_cfg_server_cb_t)(esp_ble_mesh_cfg_server_cb_event_t event, esp_ble_mesh_cfg_server_cb_param_t *param)
Configuration Server Model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_cfg_client_cb_event_t


```


enum esp_ble_mesh_cfg_client_cb_event_t
This enum value is the event of Configuration Client Model 
Values:
ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT

enumerator ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT

ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT

enumerator ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT

ESP_BLE_MESH_CFG_CLIENT_PUBLISH_EVT

enumerator ESP_BLE_MESH_CFG_CLIENT_PUBLISH_EVT

ESP_BLE_MESH_CFG_CLIENT_TIMEOUT_EVT

enumerator ESP_BLE_MESH_CFG_CLIENT_TIMEOUT_EVT

ESP_BLE_MESH_CFG_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_CFG_CLIENT_EVT_MAX


```

#### esp_ble_mesh_cfg_server_cb_event_t


```


enum esp_ble_mesh_cfg_server_cb_event_t
This enum value is the event of Configuration Server model 
Values:
ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_CFG_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_CFG_SERVER_EVT_MAX


```



### Health Client/Server Models




### Header File


* components/bt/esp_ble_mesh/api/models/include/esp_ble_mesh_health_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_health_model_api.h"
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


#### esp_ble_mesh_register_health_client_callback


```


esp_err_t esp_ble_mesh_register_health_client_callback(esp_ble_mesh_health_client_cb_t callback)
Register BLE Mesh Health Model callback, the callback will report Health Client & Server Model events. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_health_server_callback


```


esp_err_t esp_ble_mesh_register_health_server_callback(esp_ble_mesh_health_server_cb_t callback)
Register BLE Mesh Health Server Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_health_client_get_state


```


esp_err_t esp_ble_mesh_health_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_health_client_get_state_t *get_state)
This function is called to get the Health Server states using the Health Client Model get messages. 

Note
If you want to find the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_opcode_health_client_get_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
get_state -- [in] Pointer to a union, each kind of opcode corresponds to one structure inside. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_health_client_set_state


```


esp_err_t esp_ble_mesh_health_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_health_client_set_state_t *set_state)
This function is called to set the Health Server states using the Health Client Model set messages. 

Note
If you want to find the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_opcode_health_client_set_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
set_state -- [in] Pointer to a union, each kind of opcode corresponds to one structure inside. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_health_server_fault_update


```


esp_err_t esp_ble_mesh_health_server_fault_update(esp_ble_mesh_elem_t *element)
This function is called by the Health Server Model to update the context of its Health Current status. 

Parameters
element -- [in] The element to which the Health Server Model belongs.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_health_client_get_state_t


```


union esp_ble_mesh_health_client_get_state_t

#include <esp_ble_mesh_health_model_api.h>
For ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_GET ESP_BLE_MESH_MODEL_OP_ATTENTION_GET ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_GET the get_state parameter in the esp_ble_mesh_health_client_get_state function should not be set to NULL. 

Public Members
fault_get

esp_ble_mesh_health_fault_get_t fault_get
For ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_GET. 



```

#### esp_ble_mesh_health_client_set_state_t


```


union esp_ble_mesh_health_client_set_state_t

#include <esp_ble_mesh_health_model_api.h>
For ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR_UNACK ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST_UNACK ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET_UNACK ESP_BLE_MESH_MODEL_OP_ATTENTION_SET ESP_BLE_MESH_MODEL_OP_ATTENTION_SET_UNACK the set_state parameter in the esp_ble_mesh_health_client_set_state function should not be set to NULL. 

Public Members
attention_set

esp_ble_mesh_health_attention_set_t attention_set
For ESP_BLE_MESH_MODEL_OP_ATTENTION_SET or ESP_BLE_MESH_MODEL_OP_ATTENTION_SET_UNACK. 

period_set

esp_ble_mesh_health_period_set_t period_set
For ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET or ESP_BLE_MESH_MODEL_OP_HEALTH_PERIOD_SET_UNACK. 

fault_test

esp_ble_mesh_health_fault_test_t fault_test
For ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST or ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_TEST_UNACK. 

fault_clear

esp_ble_mesh_health_fault_clear_t fault_clear
For ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR or ESP_BLE_MESH_MODEL_OP_HEALTH_FAULT_CLEAR_UNACK. 



```

#### esp_ble_mesh_health_client_common_cb_param_t


```


union esp_ble_mesh_health_client_common_cb_param_t

#include <esp_ble_mesh_health_model_api.h>
Health Client Model received message union. 

Public Members
current_status

esp_ble_mesh_health_current_status_cb_t current_status
The health current status value 

fault_status

esp_ble_mesh_health_fault_status_cb_t fault_status
The health fault status value 

period_status

esp_ble_mesh_health_period_status_cb_t period_status
The health period status value 

attention_status

esp_ble_mesh_health_attention_status_cb_t attention_status
The health attention status value 



```

#### esp_ble_mesh_health_server_cb_param_t


```


union esp_ble_mesh_health_server_cb_param_t

#include <esp_ble_mesh_health_model_api.h>
Health Server Model callback parameters union. 

Public Members
fault_update_comp

esp_ble_mesh_health_fault_update_comp_cb_t fault_update_comp
ESP_BLE_MESH_HEALTH_SERVER_FAULT_UPDATE_COMP_EVT 

fault_clear

esp_ble_mesh_health_fault_clear_cb_t fault_clear
ESP_BLE_MESH_HEALTH_SERVER_FAULT_CLEAR_EVT 

fault_test

esp_ble_mesh_health_fault_test_cb_t fault_test
ESP_BLE_MESH_HEALTH_SERVER_FAULT_TEST_EVT 

attention_on

esp_ble_mesh_health_attention_on_cb_t attention_on
ESP_BLE_MESH_HEALTH_SERVER_ATTENTION_ON_EVT 

attention_off

esp_ble_mesh_health_attention_off_cb_t attention_off
ESP_BLE_MESH_HEALTH_SERVER_ATTENTION_OFF_EVT 



```



### Structures


#### esp_ble_mesh_health_srv_cb_t


```


struct esp_ble_mesh_health_srv_cb_t
ESP BLE Mesh Health Server callback 

Public Members
fault_clear

esp_ble_mesh_cb_t fault_clear
Clear health registered faults. Initialized by the stack. 

fault_test

esp_ble_mesh_cb_t fault_test
Run a specific health test. Initialized by the stack. 

attention_on

esp_ble_mesh_cb_t attention_on
Health attention on callback. Initialized by the stack. 

attention_off

esp_ble_mesh_cb_t attention_off
Health attention off callback. Initialized by the stack. 



```

#### esp_ble_mesh_health_test_t


```


struct esp_ble_mesh_health_test_t
ESP BLE Mesh Health Server test Context 

Public Members
id_count

uint8_t id_count
Number of Health self-test ID 

test_ids

const uint8_t *test_ids
Array of Health self-test IDs 

company_id

uint16_t company_id
Company ID used to identify the Health Fault state 

prev_test_id

uint8_t prev_test_id
Current test ID of the health fault test 

current_faults

uint8_t current_faults[ESP_BLE_MESH_HEALTH_FAULT_ARRAY_SIZE]
Array of current faults 

registered_faults

uint8_t registered_faults[ESP_BLE_MESH_HEALTH_FAULT_ARRAY_SIZE]
Array of registered faults 



```

#### esp_ble_mesh_health_srv_t


```


struct esp_ble_mesh_health_srv_t
ESP BLE Mesh Health Server Model Context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Health Server Model 

health_cb

esp_ble_mesh_health_srv_cb_t health_cb
Health callback struct 

attention_timer

struct k_delayed_work attention_timer
Attention Timer state 

attention_timer_start

bool attention_timer_start
Attention Timer start flag 

health_test

esp_ble_mesh_health_test_t health_test
Health Server fault test 



```

#### esp_ble_mesh_health_fault_get_t


```


struct esp_ble_mesh_health_fault_get_t
Parameter of Health Fault Get 

Public Members
company_id

uint16_t company_id
Bluetooth assigned 16-bit Company ID 



```

#### esp_ble_mesh_health_attention_set_t


```


struct esp_ble_mesh_health_attention_set_t
Parameter of Health Attention Set 

Public Members
attention

uint8_t attention
Value of the Attention Timer state 



```

#### esp_ble_mesh_health_period_set_t


```


struct esp_ble_mesh_health_period_set_t
Parameter of Health Period Set 

Public Members
fast_period_divisor

uint8_t fast_period_divisor
Divider for the Publish Period 



```

#### esp_ble_mesh_health_fault_test_t


```


struct esp_ble_mesh_health_fault_test_t
Parameter of Health Fault Test 

Public Members
company_id

uint16_t company_id
Bluetooth assigned 16-bit Company ID 

test_id

uint8_t test_id
ID of a specific test to be performed 



```

#### esp_ble_mesh_health_fault_clear_t


```


struct esp_ble_mesh_health_fault_clear_t
Parameter of Health Fault Clear 

Public Members
company_id

uint16_t company_id
Bluetooth assigned 16-bit Company ID 



```

#### esp_ble_mesh_health_current_status_cb_t


```


struct esp_ble_mesh_health_current_status_cb_t
Parameters of Health Current Status 

Public Members
test_id

uint8_t test_id
ID of a most recently performed test 

company_id

uint16_t company_id
Bluetooth assigned 16-bit Company ID 

fault_array

struct net_buf_simple *fault_array
FaultArray field contains a sequence of 1-octet fault values 



```

#### esp_ble_mesh_health_fault_status_cb_t


```


struct esp_ble_mesh_health_fault_status_cb_t
Parameters of Health Fault Status 

Public Members
test_id

uint8_t test_id
ID of a most recently performed test 

company_id

uint16_t company_id
Bluetooth assigned 16-bit Company ID 

fault_array

struct net_buf_simple *fault_array
FaultArray field contains a sequence of 1-octet fault values 



```

#### esp_ble_mesh_health_period_status_cb_t


```


struct esp_ble_mesh_health_period_status_cb_t
Parameter of Health Period Status 

Public Members
fast_period_divisor

uint8_t fast_period_divisor
Divider for the Publish Period 



```

#### esp_ble_mesh_health_attention_status_cb_t


```


struct esp_ble_mesh_health_attention_status_cb_t
Parameter of Health Attention Status 

Public Members
attention

uint8_t attention
Value of the Attention Timer state 



```

#### esp_ble_mesh_health_client_cb_param_t


```


struct esp_ble_mesh_health_client_cb_param_t
Health Client Model callback parameters 

Public Members
error_code

int error_code
Appropriate error code 

params

esp_ble_mesh_client_common_param_t *params
The client common parameters. 

status_cb

esp_ble_mesh_health_client_common_cb_param_t status_cb
The health message status callback values 



```

#### esp_ble_mesh_health_fault_update_comp_cb_t


```


struct esp_ble_mesh_health_fault_update_comp_cb_t
Parameter of publishing Health Current Status completion event 

Public Members
error_code

int error_code
The result of publishing Health Current Status 

element

esp_ble_mesh_elem_t *element
Pointer to the element which contains the Health Server Model 



```

#### esp_ble_mesh_health_fault_clear_cb_t


```


struct esp_ble_mesh_health_fault_clear_cb_t
Parameters of Health Fault Clear event 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Health Server Model 

company_id

uint16_t company_id
Bluetooth assigned 16-bit Company ID 



```

#### esp_ble_mesh_health_fault_test_cb_t


```


struct esp_ble_mesh_health_fault_test_cb_t
Parameters of Health Fault Test event 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Health Server Model 

test_id

uint8_t test_id
ID of a specific test to be performed 

company_id

uint16_t company_id
Bluetooth assigned 16-bit Company ID 



```

#### esp_ble_mesh_health_attention_on_cb_t


```


struct esp_ble_mesh_health_attention_on_cb_t
Parameter of Health Attention On event 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Health Server Model 

time

uint8_t time
Duration of attention timer on (in seconds) 



```

#### esp_ble_mesh_health_attention_off_cb_t


```


struct esp_ble_mesh_health_attention_off_cb_t
Parameter of Health Attention Off event 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Health Server Model 



```



### Macros


#### ESP_BLE_MESH_MODEL_HEALTH_SRV


```


ESP_BLE_MESH_MODEL_HEALTH_SRV(srv, pub)
Define a new Health Server Model. 

Note
The Health Server Model can only be included by a Primary Element.


Parameters

srv -- Pointer to the unique struct esp_ble_mesh_health_srv_t. 
pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t.


Returns
New Health Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_HEALTH_CLI


```


ESP_BLE_MESH_MODEL_HEALTH_CLI(cli_data)
Define a new Health Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Health Client Model.


Parameters

cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Health Client Model instance. 



```

#### ESP_BLE_MESH_HEALTH_PUB_DEFINE


```


ESP_BLE_MESH_HEALTH_PUB_DEFINE(_name, _max, _role)
A helper to define a health publication context

Parameters

_name -- Name given to the publication context variable. 
_max -- Maximum number of faults the element can have. 
_role -- Role of the device which contains the model. 




```

#### ESP_BLE_MESH_HEALTH_STANDARD_TEST


```


ESP_BLE_MESH_HEALTH_STANDARD_TEST
SIG identifier of Health Fault Test. 0x01 ~ 0xFF: Vendor Specific Test. 

```

#### ESP_BLE_MESH_NO_FAULT


```


ESP_BLE_MESH_NO_FAULT
Fault values of Health Fault Test. 0x33 ~ 0x7F: Reserved for Future Use. 0x80 ~ 0xFF: Vendor Specific Warning/Error. 

```

#### ESP_BLE_MESH_BATTERY_LOW_WARNING


```


ESP_BLE_MESH_BATTERY_LOW_WARNING

```

#### ESP_BLE_MESH_BATTERY_LOW_ERROR


```


ESP_BLE_MESH_BATTERY_LOW_ERROR

```

#### ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_LOW_WARNING


```


ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_LOW_WARNING

```

#### ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_LOW_ERROR


```


ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_LOW_ERROR

```

#### ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_HIGH_WARNING


```


ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_HIGH_WARNING

```

#### ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_HIGH_ERROR


```


ESP_BLE_MESH_SUPPLY_VOLTAGE_TOO_HIGH_ERROR

```

#### ESP_BLE_MESH_POWER_SUPPLY_INTERRUPTED_WARNING


```


ESP_BLE_MESH_POWER_SUPPLY_INTERRUPTED_WARNING

```

#### ESP_BLE_MESH_POWER_SUPPLY_INTERRUPTED_ERROR


```


ESP_BLE_MESH_POWER_SUPPLY_INTERRUPTED_ERROR

```

#### ESP_BLE_MESH_NO_LOAD_WARNING


```


ESP_BLE_MESH_NO_LOAD_WARNING

```

#### ESP_BLE_MESH_NO_LOAD_ERROR


```


ESP_BLE_MESH_NO_LOAD_ERROR

```

#### ESP_BLE_MESH_OVERLOAD_WARNING


```


ESP_BLE_MESH_OVERLOAD_WARNING

```

#### ESP_BLE_MESH_OVERLOAD_ERROR


```


ESP_BLE_MESH_OVERLOAD_ERROR

```

#### ESP_BLE_MESH_OVERHEAT_WARNING


```


ESP_BLE_MESH_OVERHEAT_WARNING

```

#### ESP_BLE_MESH_OVERHEAT_ERROR


```


ESP_BLE_MESH_OVERHEAT_ERROR

```

#### ESP_BLE_MESH_CONDENSATION_WARNING


```


ESP_BLE_MESH_CONDENSATION_WARNING

```

#### ESP_BLE_MESH_CONDENSATION_ERROR


```


ESP_BLE_MESH_CONDENSATION_ERROR

```

#### ESP_BLE_MESH_VIBRATION_WARNING


```


ESP_BLE_MESH_VIBRATION_WARNING

```

#### ESP_BLE_MESH_VIBRATION_ERROR


```


ESP_BLE_MESH_VIBRATION_ERROR

```

#### ESP_BLE_MESH_CONFIGURATION_WARNING


```


ESP_BLE_MESH_CONFIGURATION_WARNING

```

#### ESP_BLE_MESH_CONFIGURATION_ERROR


```


ESP_BLE_MESH_CONFIGURATION_ERROR

```

#### ESP_BLE_MESH_ELEMENT_NOT_CALIBRATED_WARNING


```


ESP_BLE_MESH_ELEMENT_NOT_CALIBRATED_WARNING

```

#### ESP_BLE_MESH_ELEMENT_NOT_CALIBRATED_ERROR


```


ESP_BLE_MESH_ELEMENT_NOT_CALIBRATED_ERROR

```

#### ESP_BLE_MESH_MEMORY_WARNING


```


ESP_BLE_MESH_MEMORY_WARNING

```

#### ESP_BLE_MESH_MEMORY_ERROR


```


ESP_BLE_MESH_MEMORY_ERROR

```

#### ESP_BLE_MESH_SELF_TEST_WARNING


```


ESP_BLE_MESH_SELF_TEST_WARNING

```

#### ESP_BLE_MESH_SELF_TEST_ERROR


```


ESP_BLE_MESH_SELF_TEST_ERROR

```

#### ESP_BLE_MESH_INPUT_TOO_LOW_WARNING


```


ESP_BLE_MESH_INPUT_TOO_LOW_WARNING

```

#### ESP_BLE_MESH_INPUT_TOO_LOW_ERROR


```


ESP_BLE_MESH_INPUT_TOO_LOW_ERROR

```

#### ESP_BLE_MESH_INPUT_TOO_HIGH_WARNING


```


ESP_BLE_MESH_INPUT_TOO_HIGH_WARNING

```

#### ESP_BLE_MESH_INPUT_TOO_HIGH_ERROR


```


ESP_BLE_MESH_INPUT_TOO_HIGH_ERROR

```

#### ESP_BLE_MESH_INPUT_NO_CHANGE_WARNING


```


ESP_BLE_MESH_INPUT_NO_CHANGE_WARNING

```

#### ESP_BLE_MESH_INPUT_NO_CHANGE_ERROR


```


ESP_BLE_MESH_INPUT_NO_CHANGE_ERROR

```

#### ESP_BLE_MESH_ACTUATOR_BLOCKED_WARNING


```


ESP_BLE_MESH_ACTUATOR_BLOCKED_WARNING

```

#### ESP_BLE_MESH_ACTUATOR_BLOCKED_ERROR


```


ESP_BLE_MESH_ACTUATOR_BLOCKED_ERROR

```

#### ESP_BLE_MESH_HOUSING_OPENED_WARNING


```


ESP_BLE_MESH_HOUSING_OPENED_WARNING

```

#### ESP_BLE_MESH_HOUSING_OPENED_ERROR


```


ESP_BLE_MESH_HOUSING_OPENED_ERROR

```

#### ESP_BLE_MESH_TAMPER_WARNING


```


ESP_BLE_MESH_TAMPER_WARNING

```

#### ESP_BLE_MESH_TAMPER_ERROR


```


ESP_BLE_MESH_TAMPER_ERROR

```

#### ESP_BLE_MESH_DEVICE_MOVED_WARNING


```


ESP_BLE_MESH_DEVICE_MOVED_WARNING

```

#### ESP_BLE_MESH_DEVICE_MOVED_ERROR


```


ESP_BLE_MESH_DEVICE_MOVED_ERROR

```

#### ESP_BLE_MESH_DEVICE_DROPPED_WARNING


```


ESP_BLE_MESH_DEVICE_DROPPED_WARNING

```

#### ESP_BLE_MESH_DEVICE_DROPPED_ERROR


```


ESP_BLE_MESH_DEVICE_DROPPED_ERROR

```

#### ESP_BLE_MESH_OVERFLOW_WARNING


```


ESP_BLE_MESH_OVERFLOW_WARNING

```

#### ESP_BLE_MESH_OVERFLOW_ERROR


```


ESP_BLE_MESH_OVERFLOW_ERROR

```

#### ESP_BLE_MESH_EMPTY_WARNING


```


ESP_BLE_MESH_EMPTY_WARNING

```

#### ESP_BLE_MESH_EMPTY_ERROR


```


ESP_BLE_MESH_EMPTY_ERROR

```

#### ESP_BLE_MESH_INTERNAL_BUS_WARNING


```


ESP_BLE_MESH_INTERNAL_BUS_WARNING

```

#### ESP_BLE_MESH_INTERNAL_BUS_ERROR


```


ESP_BLE_MESH_INTERNAL_BUS_ERROR

```

#### ESP_BLE_MESH_MECHANISM_JAMMED_WARNING


```


ESP_BLE_MESH_MECHANISM_JAMMED_WARNING

```

#### ESP_BLE_MESH_MECHANISM_JAMMED_ERROR


```


ESP_BLE_MESH_MECHANISM_JAMMED_ERROR

```

#### ESP_BLE_MESH_HEALTH_FAULT_ARRAY_SIZE


```


ESP_BLE_MESH_HEALTH_FAULT_ARRAY_SIZE

```



### Type Definitions


#### esp_ble_mesh_health_client_cb_t


```


typedef void (*esp_ble_mesh_health_client_cb_t)(esp_ble_mesh_health_client_cb_event_t event, esp_ble_mesh_health_client_cb_param_t *param)
Bluetooth Mesh Health Client and Server Model function. 
Health Client Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_health_server_cb_t


```


typedef void (*esp_ble_mesh_health_server_cb_t)(esp_ble_mesh_health_server_cb_event_t event, esp_ble_mesh_health_server_cb_param_t *param)
Health Server Model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_health_client_cb_event_t


```


enum esp_ble_mesh_health_client_cb_event_t
This enum value is the event of Health Client Model 
Values:
ESP_BLE_MESH_HEALTH_CLIENT_GET_STATE_EVT

enumerator ESP_BLE_MESH_HEALTH_CLIENT_GET_STATE_EVT

ESP_BLE_MESH_HEALTH_CLIENT_SET_STATE_EVT

enumerator ESP_BLE_MESH_HEALTH_CLIENT_SET_STATE_EVT

ESP_BLE_MESH_HEALTH_CLIENT_PUBLISH_EVT

enumerator ESP_BLE_MESH_HEALTH_CLIENT_PUBLISH_EVT

ESP_BLE_MESH_HEALTH_CLIENT_TIMEOUT_EVT

enumerator ESP_BLE_MESH_HEALTH_CLIENT_TIMEOUT_EVT

ESP_BLE_MESH_HEALTH_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_HEALTH_CLIENT_EVT_MAX


```

#### esp_ble_mesh_health_server_cb_event_t


```


enum esp_ble_mesh_health_server_cb_event_t
This enum value is the event of Health Server Model 
Values:
ESP_BLE_MESH_HEALTH_SERVER_FAULT_UPDATE_COMP_EVT

enumerator ESP_BLE_MESH_HEALTH_SERVER_FAULT_UPDATE_COMP_EVT

ESP_BLE_MESH_HEALTH_SERVER_FAULT_CLEAR_EVT

enumerator ESP_BLE_MESH_HEALTH_SERVER_FAULT_CLEAR_EVT

ESP_BLE_MESH_HEALTH_SERVER_FAULT_TEST_EVT

enumerator ESP_BLE_MESH_HEALTH_SERVER_FAULT_TEST_EVT

ESP_BLE_MESH_HEALTH_SERVER_ATTENTION_ON_EVT

enumerator ESP_BLE_MESH_HEALTH_SERVER_ATTENTION_ON_EVT

ESP_BLE_MESH_HEALTH_SERVER_ATTENTION_OFF_EVT

enumerator ESP_BLE_MESH_HEALTH_SERVER_ATTENTION_OFF_EVT

ESP_BLE_MESH_HEALTH_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_HEALTH_SERVER_EVT_MAX


```



### Generic Client/Server Models




### Header File


* components/bt/esp_ble_mesh/api/models/include/esp_ble_mesh_generic_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_generic_model_api.h"
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


#### esp_ble_mesh_register_generic_client_callback


```


esp_err_t esp_ble_mesh_register_generic_client_callback(esp_ble_mesh_generic_client_cb_t callback)
Register BLE Mesh Generic Client Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_generic_client_get_state


```


esp_err_t esp_ble_mesh_generic_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_generic_client_get_state_t *get_state)
Get the value of Generic Server Model states using the Generic Client Model get messages. 

Note
If you want to find the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_generic_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
get_state -- [in] Pointer to generic get message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_generic_client_set_state


```


esp_err_t esp_ble_mesh_generic_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_generic_client_set_state_t *set_state)
Set the value of Generic Server Model states using the Generic Client Model set messages. 

Note
If you want to find the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_generic_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
set_state -- [in] Pointer to generic set message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_generic_server_callback


```


esp_err_t esp_ble_mesh_register_generic_server_callback(esp_ble_mesh_generic_server_cb_t callback)
Register BLE Mesh Generic Server Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_generic_client_get_state_t


```


union esp_ble_mesh_generic_client_get_state_t

#include <esp_ble_mesh_generic_model_api.h>
Generic Client Model get message union. 

Public Members
user_property_get

esp_ble_mesh_gen_user_property_get_t user_property_get
For ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_GET 

admin_property_get

esp_ble_mesh_gen_admin_property_get_t admin_property_get
For ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_GET 

manufacturer_property_get

esp_ble_mesh_gen_manufacturer_property_get_t manufacturer_property_get
For ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET 

client_properties_get

esp_ble_mesh_gen_client_properties_get_t client_properties_get
For ESP_BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_GET 



```

#### esp_ble_mesh_generic_client_set_state_t


```


union esp_ble_mesh_generic_client_set_state_t

#include <esp_ble_mesh_generic_model_api.h>
Generic Client Model set message union. 

Public Members
onoff_set

esp_ble_mesh_gen_onoff_set_t onoff_set
For ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET & ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK 

level_set

esp_ble_mesh_gen_level_set_t level_set
For ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET & ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK 

delta_set

esp_ble_mesh_gen_delta_set_t delta_set
For ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET & ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET_UNACK 

move_set

esp_ble_mesh_gen_move_set_t move_set
For ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET & ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET_UNACK 

def_trans_time_set

esp_ble_mesh_gen_def_trans_time_set_t def_trans_time_set
For ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET & ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET_UNACK 

power_set

esp_ble_mesh_gen_onpowerup_set_t power_set
For ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET & ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET_UNACK 

power_level_set

esp_ble_mesh_gen_power_level_set_t power_level_set
For ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET & ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET_UNACK 

power_default_set

esp_ble_mesh_gen_power_default_set_t power_default_set
For ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET & ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET_UNACK 

power_range_set

esp_ble_mesh_gen_power_range_set_t power_range_set
For ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET & ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET_UNACK 

loc_global_set

esp_ble_mesh_gen_loc_global_set_t loc_global_set
For ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET & ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET_UNACK 

loc_local_set

esp_ble_mesh_gen_loc_local_set_t loc_local_set
For ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET & ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET_UNACK 

user_property_set

esp_ble_mesh_gen_user_property_set_t user_property_set
For ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET & ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET_UNACK 

admin_property_set

esp_ble_mesh_gen_admin_property_set_t admin_property_set
For ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET & ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET_UNACK 

manufacturer_property_set

esp_ble_mesh_gen_manufacturer_property_set_t manufacturer_property_set
For ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET & ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET_UNACK 



```

#### esp_ble_mesh_gen_client_status_cb_t


```


union esp_ble_mesh_gen_client_status_cb_t

#include <esp_ble_mesh_generic_model_api.h>
Generic Client Model received message union. 

Public Members
onoff_status

esp_ble_mesh_gen_onoff_status_cb_t onoff_status
For ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS 

level_status

esp_ble_mesh_gen_level_status_cb_t level_status
For ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS 

def_trans_time_status

esp_ble_mesh_gen_def_trans_time_status_cb_t def_trans_time_status
For ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_STATUS 

onpowerup_status

esp_ble_mesh_gen_onpowerup_status_cb_t onpowerup_status
For ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_STATUS 

power_level_status

esp_ble_mesh_gen_power_level_status_cb_t power_level_status
For ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_STATUS 

power_last_status

esp_ble_mesh_gen_power_last_status_cb_t power_last_status
For ESP_BLE_MESH_MODEL_OP_GEN_POWER_LAST_STATUS 

power_default_status

esp_ble_mesh_gen_power_default_status_cb_t power_default_status
For ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_STATUS 

power_range_status

esp_ble_mesh_gen_power_range_status_cb_t power_range_status
For ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_STATUS 

battery_status

esp_ble_mesh_gen_battery_status_cb_t battery_status
For ESP_BLE_MESH_MODEL_OP_GEN_BATTERY_STATUS 

location_global_status

esp_ble_mesh_gen_loc_global_status_cb_t location_global_status
For ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_STATUS 

location_local_status

esp_ble_mesh_gen_loc_local_status_cb_t location_local_status
ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_STATUS 

user_properties_status

esp_ble_mesh_gen_user_properties_status_cb_t user_properties_status
ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_STATUS 

user_property_status

esp_ble_mesh_gen_user_property_status_cb_t user_property_status
ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_STATUS 

admin_properties_status

esp_ble_mesh_gen_admin_properties_status_cb_t admin_properties_status
ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_STATUS 

admin_property_status

esp_ble_mesh_gen_admin_property_status_cb_t admin_property_status
ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_STATUS 

manufacturer_properties_status

esp_ble_mesh_gen_manufacturer_properties_status_cb_t manufacturer_properties_status
ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTIES_STATUS 

manufacturer_property_status

esp_ble_mesh_gen_manufacturer_property_status_cb_t manufacturer_property_status
ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_STATUS 

client_properties_status

esp_ble_mesh_gen_client_properties_status_cb_t client_properties_status
ESP_BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_STATUS 



```

#### esp_ble_mesh_generic_server_state_change_t


```


union esp_ble_mesh_generic_server_state_change_t

#include <esp_ble_mesh_generic_model_api.h>
Generic Server Model state change value union. 

Public Members
onoff_set

esp_ble_mesh_state_change_gen_onoff_set_t onoff_set
The recv_op in ctx can be used to decide which state is changed. Generic OnOff Set 

level_set

esp_ble_mesh_state_change_gen_level_set_t level_set
Generic Level Set 

delta_set

esp_ble_mesh_state_change_gen_delta_set_t delta_set
Generic Delta Set 

move_set

esp_ble_mesh_state_change_gen_move_set_t move_set
Generic Move Set 

def_trans_time_set

esp_ble_mesh_state_change_gen_def_trans_time_set_t def_trans_time_set
Generic Default Transition Time Set 

onpowerup_set

esp_ble_mesh_state_change_gen_onpowerup_set_t onpowerup_set
Generic OnPowerUp Set 

power_level_set

esp_ble_mesh_state_change_gen_power_level_set_t power_level_set
Generic Power Level Set 

power_default_set

esp_ble_mesh_state_change_gen_power_default_set_t power_default_set
Generic Power Default Set 

power_range_set

esp_ble_mesh_state_change_gen_power_range_set_t power_range_set
Generic Power Range Set 

loc_global_set

esp_ble_mesh_state_change_gen_loc_global_set_t loc_global_set
Generic Location Global Set 

loc_local_set

esp_ble_mesh_state_change_gen_loc_local_set_t loc_local_set
Generic Location Local Set 

user_property_set

esp_ble_mesh_state_change_gen_user_property_set_t user_property_set
Generic User Property Set 

admin_property_set

esp_ble_mesh_state_change_gen_admin_property_set_t admin_property_set
Generic Admin Property Set 

manu_property_set

esp_ble_mesh_state_change_gen_manu_property_set_t manu_property_set
Generic Manufacturer Property Set 



```

#### esp_ble_mesh_generic_server_recv_get_msg_t


```


union esp_ble_mesh_generic_server_recv_get_msg_t

#include <esp_ble_mesh_generic_model_api.h>
Generic Server Model received get message union. 

Public Members
user_property

esp_ble_mesh_server_recv_gen_user_property_get_t user_property
Generic User Property Get 

admin_property

esp_ble_mesh_server_recv_gen_admin_property_get_t admin_property
Generic Admin Property Get 

manu_property

esp_ble_mesh_server_recv_gen_manufacturer_property_get_t manu_property
Generic Manufacturer Property Get 

client_properties

esp_ble_mesh_server_recv_gen_client_properties_get_t client_properties
Generic Client Properties Get 



```

#### esp_ble_mesh_generic_server_recv_set_msg_t


```


union esp_ble_mesh_generic_server_recv_set_msg_t

#include <esp_ble_mesh_generic_model_api.h>
Generic Server Model received set message union. 

Public Members
onoff

esp_ble_mesh_server_recv_gen_onoff_set_t onoff
Generic OnOff Set/Generic OnOff Set Unack 

level

esp_ble_mesh_server_recv_gen_level_set_t level
Generic Level Set/Generic Level Set Unack 

delta

esp_ble_mesh_server_recv_gen_delta_set_t delta
Generic Delta Set/Generic Delta Set Unack 

move

esp_ble_mesh_server_recv_gen_move_set_t move
Generic Move Set/Generic Move Set Unack 

def_trans_time

esp_ble_mesh_server_recv_gen_def_trans_time_set_t def_trans_time
Generic Default Transition Time Set/Generic Default Transition Time Set Unack 

onpowerup

esp_ble_mesh_server_recv_gen_onpowerup_set_t onpowerup
Generic OnPowerUp Set/Generic OnPowerUp Set Unack 

power_level

esp_ble_mesh_server_recv_gen_power_level_set_t power_level
Generic Power Level Set/Generic Power Level Set Unack 

power_default

esp_ble_mesh_server_recv_gen_power_default_set_t power_default
Generic Power Default Set/Generic Power Default Set Unack 

power_range

esp_ble_mesh_server_recv_gen_power_range_set_t power_range
Generic Power Range Set/Generic Power Range Set Unack 

location_global

esp_ble_mesh_server_recv_gen_loc_global_set_t location_global
Generic Location Global Set/Generic Location Global Set Unack 

location_local

esp_ble_mesh_server_recv_gen_loc_local_set_t location_local
Generic Location Local Set/Generic Location Local Set Unack 

user_property

esp_ble_mesh_server_recv_gen_user_property_set_t user_property
Generic User Property Set/Generic User Property Set Unack 

admin_property

esp_ble_mesh_server_recv_gen_admin_property_set_t admin_property
Generic Admin Property Set/Generic Admin Property Set Unack 

manu_property

esp_ble_mesh_server_recv_gen_manufacturer_property_set_t manu_property
Generic Manufacturer Property Set/Generic Manufacturer Property Set Unack 



```

#### esp_ble_mesh_generic_server_cb_value_t


```


union esp_ble_mesh_generic_server_cb_value_t

#include <esp_ble_mesh_generic_model_api.h>
Generic Server Model callback value union. 

Public Members
state_change

esp_ble_mesh_generic_server_state_change_t state_change
ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT 

get

esp_ble_mesh_generic_server_recv_get_msg_t get
ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT 

set

esp_ble_mesh_generic_server_recv_set_msg_t set
ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT 



```



### Structures


#### esp_ble_mesh_gen_onoff_set_t


```


struct esp_ble_mesh_gen_onoff_set_t
Bluetooth Mesh Generic Client Model Get and Set parameters structure. 
Parameters of Generic OnOff Set. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

onoff

uint8_t onoff
Target value of Generic OnOff state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_gen_level_set_t


```


struct esp_ble_mesh_gen_level_set_t
Parameters of Generic Level Set. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

level

int16_t level
Target value of Generic Level state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_gen_delta_set_t


```


struct esp_ble_mesh_gen_delta_set_t
Parameters of Generic Delta Set. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

level

int32_t level
Delta change of Generic Level state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_gen_move_set_t


```


struct esp_ble_mesh_gen_move_set_t
Parameters of Generic Move Set. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

delta_level

int16_t delta_level
Delta Level step to calculate Move speed for Generic Level state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_gen_def_trans_time_set_t


```


struct esp_ble_mesh_gen_def_trans_time_set_t
Parameter of Generic Default Transition Time Set. 

Public Members
trans_time

uint8_t trans_time
The value of the Generic Default Transition Time state 



```

#### esp_ble_mesh_gen_onpowerup_set_t


```


struct esp_ble_mesh_gen_onpowerup_set_t
Parameter of Generic OnPowerUp Set. 

Public Members
onpowerup

uint8_t onpowerup
The value of the Generic OnPowerUp state 



```

#### esp_ble_mesh_gen_power_level_set_t


```


struct esp_ble_mesh_gen_power_level_set_t
Parameters of Generic Power Level Set. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

power

uint16_t power
Target value of Generic Power Actual state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_gen_power_default_set_t


```


struct esp_ble_mesh_gen_power_default_set_t
Parameter of Generic Power Default Set. 

Public Members
power

uint16_t power
The value of the Generic Power Default state 



```

#### esp_ble_mesh_gen_power_range_set_t


```


struct esp_ble_mesh_gen_power_range_set_t
Parameters of Generic Power Range Set. 

Public Members
range_min

uint16_t range_min
Value of Range Min field of Generic Power Range state 

range_max

uint16_t range_max
Value of Range Max field of Generic Power Range state 



```

#### esp_ble_mesh_gen_loc_global_set_t


```


struct esp_ble_mesh_gen_loc_global_set_t
Parameters of Generic Location Global Set. 

Public Members
global_latitude

int32_t global_latitude
Global Coordinates (Latitude) 

global_longitude

int32_t global_longitude
Global Coordinates (Longitude) 

global_altitude

int16_t global_altitude
Global Altitude 



```

#### esp_ble_mesh_gen_loc_local_set_t


```


struct esp_ble_mesh_gen_loc_local_set_t
Parameters of Generic Location Local Set. 

Public Members
local_north

int16_t local_north
Local Coordinates (North) 

local_east

int16_t local_east
Local Coordinates (East) 

local_altitude

int16_t local_altitude
Local Altitude 

floor_number

uint8_t floor_number
Floor Number 

uncertainty

uint16_t uncertainty
Uncertainty 



```

#### esp_ble_mesh_gen_user_property_get_t


```


struct esp_ble_mesh_gen_user_property_get_t
Parameter of Generic User Property Get. 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic User Property 



```

#### esp_ble_mesh_gen_user_property_set_t


```


struct esp_ble_mesh_gen_user_property_set_t
Parameters of Generic User Property Set. 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic User Property 

property_value

struct net_buf_simple *property_value
Raw value for the User Property 



```

#### esp_ble_mesh_gen_admin_property_get_t


```


struct esp_ble_mesh_gen_admin_property_get_t
Parameter of Generic Admin Property Get. 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Admin Property 



```

#### esp_ble_mesh_gen_admin_property_set_t


```


struct esp_ble_mesh_gen_admin_property_set_t
Parameters of Generic Admin Property Set. 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Admin Property 

user_access

uint8_t user_access
Enumeration indicating user access 

property_value

struct net_buf_simple *property_value
Raw value for the Admin Property 



```

#### esp_ble_mesh_gen_manufacturer_property_get_t


```


struct esp_ble_mesh_gen_manufacturer_property_get_t
Parameter of Generic Manufacturer Property Get. 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Manufacturer Property 



```

#### esp_ble_mesh_gen_manufacturer_property_set_t


```


struct esp_ble_mesh_gen_manufacturer_property_set_t
Parameters of Generic Manufacturer Property Set. 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Manufacturer Property 

user_access

uint8_t user_access
Enumeration indicating user access 



```

#### esp_ble_mesh_gen_client_properties_get_t


```


struct esp_ble_mesh_gen_client_properties_get_t
Parameter of Generic Client Properties Get. 

Public Members
property_id

uint16_t property_id
A starting Client Property ID present within an element 



```

#### esp_ble_mesh_gen_onoff_status_cb_t


```


struct esp_ble_mesh_gen_onoff_status_cb_t
Bluetooth Mesh Generic Client Model Get and Set callback parameters structure. 
Parameters of Generic OnOff Status. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_onoff

uint8_t present_onoff
Current value of Generic OnOff state 

target_onoff

uint8_t target_onoff
Target value of Generic OnOff state (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_gen_level_status_cb_t


```


struct esp_ble_mesh_gen_level_status_cb_t
Parameters of Generic Level Status. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_level

int16_t present_level
Current value of Generic Level state 

target_level

int16_t target_level
Target value of the Generic Level state (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_gen_def_trans_time_status_cb_t


```


struct esp_ble_mesh_gen_def_trans_time_status_cb_t
Parameter of Generic Default Transition Time Status. 

Public Members
trans_time

uint8_t trans_time
The value of the Generic Default Transition Time state 



```

#### esp_ble_mesh_gen_onpowerup_status_cb_t


```


struct esp_ble_mesh_gen_onpowerup_status_cb_t
Parameter of Generic OnPowerUp Status. 

Public Members
onpowerup

uint8_t onpowerup
The value of the Generic OnPowerUp state 



```

#### esp_ble_mesh_gen_power_level_status_cb_t


```


struct esp_ble_mesh_gen_power_level_status_cb_t
Parameters of Generic Power Level Status. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_power

uint16_t present_power
Current value of Generic Power Actual state 

target_power

uint16_t target_power
Target value of Generic Power Actual state (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_gen_power_last_status_cb_t


```


struct esp_ble_mesh_gen_power_last_status_cb_t
Parameter of Generic Power Last Status. 

Public Members
power

uint16_t power
The value of the Generic Power Last state 



```

#### esp_ble_mesh_gen_power_default_status_cb_t


```


struct esp_ble_mesh_gen_power_default_status_cb_t
Parameter of Generic Power Default Status. 

Public Members
power

uint16_t power
The value of the Generic Default Last state 



```

#### esp_ble_mesh_gen_power_range_status_cb_t


```


struct esp_ble_mesh_gen_power_range_status_cb_t
Parameters of Generic Power Range Status. 

Public Members
status_code

uint8_t status_code
Status Code for the request message 

range_min

uint16_t range_min
Value of Range Min field of Generic Power Range state 

range_max

uint16_t range_max
Value of Range Max field of Generic Power Range state 



```

#### esp_ble_mesh_gen_battery_status_cb_t


```


struct esp_ble_mesh_gen_battery_status_cb_t
Parameters of Generic Battery Status. 

Public Members
battery_level

uint32_t battery_level
Value of Generic Battery Level state 

time_to_discharge

uint32_t time_to_discharge
Value of Generic Battery Time to Discharge state 

time_to_charge

uint32_t time_to_charge
Value of Generic Battery Time to Charge state 

flags

uint32_t flags
Value of Generic Battery Flags state 



```

#### esp_ble_mesh_gen_loc_global_status_cb_t


```


struct esp_ble_mesh_gen_loc_global_status_cb_t
Parameters of Generic Location Global Status. 

Public Members
global_latitude

int32_t global_latitude
Global Coordinates (Latitude) 

global_longitude

int32_t global_longitude
Global Coordinates (Longitude) 

global_altitude

int16_t global_altitude
Global Altitude 



```

#### esp_ble_mesh_gen_loc_local_status_cb_t


```


struct esp_ble_mesh_gen_loc_local_status_cb_t
Parameters of Generic Location Local Status. 

Public Members
local_north

int16_t local_north
Local Coordinates (North) 

local_east

int16_t local_east
Local Coordinates (East) 

local_altitude

int16_t local_altitude
Local Altitude 

floor_number

uint8_t floor_number
Floor Number 

uncertainty

uint16_t uncertainty
Uncertainty 



```

#### esp_ble_mesh_gen_user_properties_status_cb_t


```


struct esp_ble_mesh_gen_user_properties_status_cb_t
Parameter of Generic User Properties Status. 

Public Members
property_ids

struct net_buf_simple *property_ids
Buffer contains a sequence of N User Property IDs 



```

#### esp_ble_mesh_gen_user_property_status_cb_t


```


struct esp_ble_mesh_gen_user_property_status_cb_t
Parameters of Generic User Property Status. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property ID identifying a Generic User Property 

user_access

uint8_t user_access
Enumeration indicating user access (optional) 

property_value

struct net_buf_simple *property_value
Raw value for the User Property (C.1) 



```

#### esp_ble_mesh_gen_admin_properties_status_cb_t


```


struct esp_ble_mesh_gen_admin_properties_status_cb_t
Parameter of Generic Admin Properties Status. 

Public Members
property_ids

struct net_buf_simple *property_ids
Buffer contains a sequence of N Admin Property IDs 



```

#### esp_ble_mesh_gen_admin_property_status_cb_t


```


struct esp_ble_mesh_gen_admin_property_status_cb_t
Parameters of Generic Admin Property Status. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property ID identifying a Generic Admin Property 

user_access

uint8_t user_access
Enumeration indicating user access (optional) 

property_value

struct net_buf_simple *property_value
Raw value for the Admin Property (C.1) 



```

#### esp_ble_mesh_gen_manufacturer_properties_status_cb_t


```


struct esp_ble_mesh_gen_manufacturer_properties_status_cb_t
Parameter of Generic Manufacturer Properties Status. 

Public Members
property_ids

struct net_buf_simple *property_ids
Buffer contains a sequence of N Manufacturer Property IDs 



```

#### esp_ble_mesh_gen_manufacturer_property_status_cb_t


```


struct esp_ble_mesh_gen_manufacturer_property_status_cb_t
Parameters of Generic Manufacturer Property Status. 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property ID identifying a Generic Manufacturer Property 

user_access

uint8_t user_access
Enumeration indicating user access (optional) 

property_value

struct net_buf_simple *property_value
Raw value for the Manufacturer Property (C.1) 



```

#### esp_ble_mesh_gen_client_properties_status_cb_t


```


struct esp_ble_mesh_gen_client_properties_status_cb_t
Parameter of Generic Client Properties Status. 

Public Members
property_ids

struct net_buf_simple *property_ids
Buffer contains a sequence of N Client Property IDs 



```

#### esp_ble_mesh_generic_client_cb_param_t


```


struct esp_ble_mesh_generic_client_cb_param_t
Generic Client Model callback parameters 

Public Members
error_code

int error_code
Appropriate error code 

params

esp_ble_mesh_client_common_param_t *params
The client common parameters. 

status_cb

esp_ble_mesh_gen_client_status_cb_t status_cb
The generic status message callback values 



```

#### esp_ble_mesh_gen_onoff_state_t


```


struct esp_ble_mesh_gen_onoff_state_t
Parameters of Generic OnOff state 

Public Members
onoff

uint8_t onoff
The present value of the Generic OnOff state 

target_onoff

uint8_t target_onoff
The target value of the Generic OnOff state 



```

#### esp_ble_mesh_gen_onoff_srv_t


```


struct esp_ble_mesh_gen_onoff_srv_t
User data of Generic OnOff Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic OnOff Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_onoff_state_t state
Parameters of the Generic OnOff state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 



```

#### esp_ble_mesh_gen_level_state_t


```


struct esp_ble_mesh_gen_level_state_t
Parameters of Generic Level state 

Public Members
level

int16_t level
The present value of the Generic Level state 

target_level

int16_t target_level
The target value of the Generic Level state 

last_level

int16_t last_level
When a new transaction starts, level should be set to last_last, and use "level + incoming delta" to calculate the target level. In another word, "last_level" is used to record "level" of the last transaction, and "last_delta" is used to record the previously received delta_level value. The last value of the Generic Level state 

last_delta

int32_t last_delta
The last delta change of the Generic Level state 

move_start

bool move_start
Indicate if the transition of the Generic Level state has been started 

positive

bool positive
Indicate if the transition is positive or negative 



```

#### esp_ble_mesh_gen_level_srv_t


```


struct esp_ble_mesh_gen_level_srv_t
User data of Generic Level Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Level Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_level_state_t state
Parameters of the Generic Level state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_level

int32_t tt_delta_level
Delta change value of level state transition 



```

#### esp_ble_mesh_gen_def_trans_time_state_t


```


struct esp_ble_mesh_gen_def_trans_time_state_t
Parameter of Generic Default Transition Time state 

Public Members
trans_time

uint8_t trans_time
The value of the Generic Default Transition Time state 



```

#### esp_ble_mesh_gen_def_trans_time_srv_t


```


struct esp_ble_mesh_gen_def_trans_time_srv_t
User data of Generic Default Transition Time Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Default Transition Time Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_def_trans_time_state_t state
Parameters of the Generic Default Transition Time state 



```

#### esp_ble_mesh_gen_onpowerup_state_t


```


struct esp_ble_mesh_gen_onpowerup_state_t
Parameter of Generic OnPowerUp state 

Public Members
onpowerup

uint8_t onpowerup
The value of the Generic OnPowerUp state 



```

#### esp_ble_mesh_gen_power_onoff_srv_t


```


struct esp_ble_mesh_gen_power_onoff_srv_t
User data of Generic Power OnOff Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Power OnOff Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_onpowerup_state_t *state
Parameters of the Generic OnPowerUp state 



```

#### esp_ble_mesh_gen_power_onoff_setup_srv_t


```


struct esp_ble_mesh_gen_power_onoff_setup_srv_t
User data of Generic Power OnOff Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Power OnOff Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_onpowerup_state_t *state
Parameters of the Generic OnPowerUp state 



```

#### esp_ble_mesh_gen_power_level_state_t


```


struct esp_ble_mesh_gen_power_level_state_t
Parameters of Generic Power Level state 

Public Members
power_actual

uint16_t power_actual
The present value of the Generic Power Actual state 

target_power_actual

uint16_t target_power_actual
The target value of the Generic Power Actual state 

power_last

uint16_t power_last
The value of the Generic Power Last state 

power_default

uint16_t power_default
The value of the Generic Power Default state 

status_code

uint8_t status_code
The status code of setting Generic Power Range state 

power_range_min

uint16_t power_range_min
The minimum value of the Generic Power Range state 

power_range_max

uint16_t power_range_max
The maximum value of the Generic Power Range state 



```

#### esp_ble_mesh_gen_power_level_srv_t


```


struct esp_ble_mesh_gen_power_level_srv_t
User data of Generic Power Level Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Power Level Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_power_level_state_t *state
Parameters of the Generic Power Level state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_level

int32_t tt_delta_level
Delta change value of level state transition 



```

#### esp_ble_mesh_gen_power_level_setup_srv_t


```


struct esp_ble_mesh_gen_power_level_setup_srv_t
User data of Generic Power Level Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Power Level Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_power_level_state_t *state
Parameters of the Generic Power Level state 



```

#### esp_ble_mesh_gen_battery_state_t


```


struct esp_ble_mesh_gen_battery_state_t
Parameters of Generic Battery state 

Public Members
battery_level

uint32_t battery_level
The value of the Generic Battery Level state 

time_to_discharge

uint32_t time_to_discharge
The value of the Generic Battery Time to Discharge state 

time_to_charge

uint32_t time_to_charge
The value of the Generic Battery Time to Charge state 

battery_flags

uint32_t battery_flags
The value of the Generic Battery Flags state 



```

#### esp_ble_mesh_gen_battery_srv_t


```


struct esp_ble_mesh_gen_battery_srv_t
User data of Generic Battery Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Battery Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_battery_state_t state
Parameters of the Generic Battery state 



```

#### esp_ble_mesh_gen_location_state_t


```


struct esp_ble_mesh_gen_location_state_t
Parameters of Generic Location state 

Public Members
global_latitude

int32_t global_latitude
The value of the Global Latitude field 

global_longitude

int32_t global_longitude
The value of the Global Longitude field 

global_altitude

int16_t global_altitude
The value of the Global Altitude field 

local_north

int16_t local_north
The value of the Local North field 

local_east

int16_t local_east
The value of the Local East field 

local_altitude

int16_t local_altitude
The value of the Local Altitude field 

floor_number

uint8_t floor_number
The value of the Floor Number field 

uncertainty

uint16_t uncertainty
The value of the Uncertainty field 



```

#### esp_ble_mesh_gen_location_srv_t


```


struct esp_ble_mesh_gen_location_srv_t
User data of Generic Location Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Location Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_location_state_t *state
Parameters of the Generic Location state 



```

#### esp_ble_mesh_gen_location_setup_srv_t


```


struct esp_ble_mesh_gen_location_setup_srv_t
User data of Generic Location Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Location Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_gen_location_state_t *state
Parameters of the Generic Location state 



```

#### esp_ble_mesh_generic_property_t


```


struct esp_ble_mesh_generic_property_t
Parameters of Generic Property states 

Public Members
id

uint16_t id
The value of User/Admin/Manufacturer Property ID 

user_access

uint8_t user_access
The value of User Access field 

admin_access

uint8_t admin_access
The value of Admin Access field 

manu_access

uint8_t manu_access
The value of Manufacturer Access field 

val

struct net_buf_simple *val
The value of User/Admin/Manufacturer Property 



```

#### esp_ble_mesh_gen_user_prop_srv_t


```


struct esp_ble_mesh_gen_user_prop_srv_t
User data of Generic User Property Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic User Property Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

property_count

uint8_t property_count
Generic User Property count 

properties

esp_ble_mesh_generic_property_t *properties
Parameters of the Generic User Property state 



```

#### esp_ble_mesh_gen_admin_prop_srv_t


```


struct esp_ble_mesh_gen_admin_prop_srv_t
User data of Generic Admin Property Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Admin Property Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

property_count

uint8_t property_count
Generic Admin Property count 

properties

esp_ble_mesh_generic_property_t *properties
Parameters of the Generic Admin Property state 



```

#### esp_ble_mesh_gen_manu_prop_srv_t


```


struct esp_ble_mesh_gen_manu_prop_srv_t
User data of Generic Manufacturer Property Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Manufacturer Property Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

property_count

uint8_t property_count
Generic Manufacturer Property count 

properties

esp_ble_mesh_generic_property_t *properties
Parameters of the Generic Manufacturer Property state 



```

#### esp_ble_mesh_gen_client_prop_srv_t


```


struct esp_ble_mesh_gen_client_prop_srv_t
User data of Generic Client Property Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Generic Client Property Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

id_count

uint8_t id_count
Generic Client Property ID count 

property_ids

uint16_t *property_ids
Parameters of the Generic Client Property state 



```

#### esp_ble_mesh_state_change_gen_onoff_set_t


```


struct esp_ble_mesh_state_change_gen_onoff_set_t
Parameter of Generic OnOff Set state change event 

Public Members
onoff

uint8_t onoff
The value of Generic OnOff state 



```

#### esp_ble_mesh_state_change_gen_level_set_t


```


struct esp_ble_mesh_state_change_gen_level_set_t
Parameter of Generic Level Set state change event 

Public Members
level

int16_t level
The value of Generic Level state 



```

#### esp_ble_mesh_state_change_gen_delta_set_t


```


struct esp_ble_mesh_state_change_gen_delta_set_t
Parameter of Generic Delta Set state change event 

Public Members
level

int16_t level
The value of Generic Level state 



```

#### esp_ble_mesh_state_change_gen_move_set_t


```


struct esp_ble_mesh_state_change_gen_move_set_t
Parameter of Generic Move Set state change event 

Public Members
level

int16_t level
The value of Generic Level state 



```

#### esp_ble_mesh_state_change_gen_def_trans_time_set_t


```


struct esp_ble_mesh_state_change_gen_def_trans_time_set_t
Parameter of Generic Default Transition Time Set state change event 

Public Members
trans_time

uint8_t trans_time
The value of Generic Default Transition Time state 



```

#### esp_ble_mesh_state_change_gen_onpowerup_set_t


```


struct esp_ble_mesh_state_change_gen_onpowerup_set_t
Parameter of Generic OnPowerUp Set state change event 

Public Members
onpowerup

uint8_t onpowerup
The value of Generic OnPowerUp state 



```

#### esp_ble_mesh_state_change_gen_power_level_set_t


```


struct esp_ble_mesh_state_change_gen_power_level_set_t
Parameter of Generic Power Level Set state change event 

Public Members
power

uint16_t power
The value of Generic Power Actual state 



```

#### esp_ble_mesh_state_change_gen_power_default_set_t


```


struct esp_ble_mesh_state_change_gen_power_default_set_t
Parameter of Generic Power Default Set state change event 

Public Members
power

uint16_t power
The value of Generic Power Default state 



```

#### esp_ble_mesh_state_change_gen_power_range_set_t


```


struct esp_ble_mesh_state_change_gen_power_range_set_t
Parameters of Generic Power Range Set state change event 

Public Members
range_min

uint16_t range_min
The minimum value of Generic Power Range state 

range_max

uint16_t range_max
The maximum value of Generic Power Range state 



```

#### esp_ble_mesh_state_change_gen_loc_global_set_t


```


struct esp_ble_mesh_state_change_gen_loc_global_set_t
Parameters of Generic Location Global Set state change event 

Public Members
latitude

int32_t latitude
The Global Latitude value of Generic Location state 

longitude

int32_t longitude
The Global Longitude value of Generic Location state 

altitude

int16_t altitude
The Global Altitude value of Generic Location state 



```

#### esp_ble_mesh_state_change_gen_loc_local_set_t


```


struct esp_ble_mesh_state_change_gen_loc_local_set_t
Parameters of Generic Location Local Set state change event 

Public Members
north

int16_t north
The Local North value of Generic Location state 

east

int16_t east
The Local East value of Generic Location state 

altitude

int16_t altitude
The Local Altitude value of Generic Location state 

floor_number

uint8_t floor_number
The Floor Number value of Generic Location state 

uncertainty

uint16_t uncertainty
The Uncertainty value of Generic Location state 



```

#### esp_ble_mesh_state_change_gen_user_property_set_t


```


struct esp_ble_mesh_state_change_gen_user_property_set_t
Parameters of Generic User Property Set state change event 

Public Members
id

uint16_t id
The property id of Generic User Property state 

value

struct net_buf_simple *value
The property value of Generic User Property state 



```

#### esp_ble_mesh_state_change_gen_admin_property_set_t


```


struct esp_ble_mesh_state_change_gen_admin_property_set_t
Parameters of Generic Admin Property Set state change event 

Public Members
id

uint16_t id
The property id of Generic Admin Property state 

access

uint8_t access
The property access of Generic Admin Property state 

value

struct net_buf_simple *value
The property value of Generic Admin Property state 



```

#### esp_ble_mesh_state_change_gen_manu_property_set_t


```


struct esp_ble_mesh_state_change_gen_manu_property_set_t
Parameters of Generic Manufacturer Property Set state change event 

Public Members
id

uint16_t id
The property id of Generic Manufacturer Property state 

access

uint8_t access
The property value of Generic Manufacturer Property state 



```

#### esp_ble_mesh_server_recv_gen_user_property_get_t


```


struct esp_ble_mesh_server_recv_gen_user_property_get_t
Context of the received Generic User Property Get message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic User Property 



```

#### esp_ble_mesh_server_recv_gen_admin_property_get_t


```


struct esp_ble_mesh_server_recv_gen_admin_property_get_t
Context of the received Generic Admin Property Get message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Admin Property 



```

#### esp_ble_mesh_server_recv_gen_manufacturer_property_get_t


```


struct esp_ble_mesh_server_recv_gen_manufacturer_property_get_t
Context of the received Generic Manufacturer Property message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Manufacturer Property 



```

#### esp_ble_mesh_server_recv_gen_client_properties_get_t


```


struct esp_ble_mesh_server_recv_gen_client_properties_get_t
Context of the received Generic Client Properties Get message 

Public Members
property_id

uint16_t property_id
A starting Client Property ID present within an element 



```

#### esp_ble_mesh_server_recv_gen_onoff_set_t


```


struct esp_ble_mesh_server_recv_gen_onoff_set_t
Context of the received Generic OnOff Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

onoff

uint8_t onoff
Target value of Generic OnOff state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_gen_level_set_t


```


struct esp_ble_mesh_server_recv_gen_level_set_t
Context of the received Generic Level Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

level

int16_t level
Target value of Generic Level state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_gen_delta_set_t


```


struct esp_ble_mesh_server_recv_gen_delta_set_t
Context of the received Generic Delta Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

delta_level

int32_t delta_level
Delta change of Generic Level state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_gen_move_set_t


```


struct esp_ble_mesh_server_recv_gen_move_set_t
Context of the received Generic Move Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

delta_level

int16_t delta_level
Delta Level step to calculate Move speed for Generic Level state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_gen_def_trans_time_set_t


```


struct esp_ble_mesh_server_recv_gen_def_trans_time_set_t
Context of the received Generic Default Transition Time Set message 

Public Members
trans_time

uint8_t trans_time
The value of the Generic Default Transition Time state 



```

#### esp_ble_mesh_server_recv_gen_onpowerup_set_t


```


struct esp_ble_mesh_server_recv_gen_onpowerup_set_t
Context of the received Generic OnPowerUp Set message 

Public Members
onpowerup

uint8_t onpowerup
The value of the Generic OnPowerUp state 



```

#### esp_ble_mesh_server_recv_gen_power_level_set_t


```


struct esp_ble_mesh_server_recv_gen_power_level_set_t
Context of the received Generic Power Level Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

power

uint16_t power
Target value of Generic Power Actual state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_gen_power_default_set_t


```


struct esp_ble_mesh_server_recv_gen_power_default_set_t
Context of the received Generic Power Default Set message 

Public Members
power

uint16_t power
The value of the Generic Power Default state 



```

#### esp_ble_mesh_server_recv_gen_power_range_set_t


```


struct esp_ble_mesh_server_recv_gen_power_range_set_t
Context of the received Generic Power Range Set message 

Public Members
range_min

uint16_t range_min
Value of Range Min field of Generic Power Range state 

range_max

uint16_t range_max
Value of Range Max field of Generic Power Range state 



```

#### esp_ble_mesh_server_recv_gen_loc_global_set_t


```


struct esp_ble_mesh_server_recv_gen_loc_global_set_t
Context of the received Generic Location Global Set message 

Public Members
global_latitude

int32_t global_latitude
Global Coordinates (Latitude) 

global_longitude

int32_t global_longitude
Global Coordinates (Longitude) 

global_altitude

int16_t global_altitude
Global Altitude 



```

#### esp_ble_mesh_server_recv_gen_loc_local_set_t


```


struct esp_ble_mesh_server_recv_gen_loc_local_set_t
Context of the received Generic Location Local Set message 

Public Members
local_north

int16_t local_north
Local Coordinates (North) 

local_east

int16_t local_east
Local Coordinates (East) 

local_altitude

int16_t local_altitude
Local Altitude 

floor_number

uint8_t floor_number
Floor Number 

uncertainty

uint16_t uncertainty
Uncertainty 



```

#### esp_ble_mesh_server_recv_gen_user_property_set_t


```


struct esp_ble_mesh_server_recv_gen_user_property_set_t
Context of the received Generic User Property Set message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic User Property 

property_value

struct net_buf_simple *property_value
Raw value for the User Property 



```

#### esp_ble_mesh_server_recv_gen_admin_property_set_t


```


struct esp_ble_mesh_server_recv_gen_admin_property_set_t
Context of the received Generic Admin Property Set message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Admin Property 

user_access

uint8_t user_access
Enumeration indicating user access 

property_value

struct net_buf_simple *property_value
Raw value for the Admin Property 



```

#### esp_ble_mesh_server_recv_gen_manufacturer_property_set_t


```


struct esp_ble_mesh_server_recv_gen_manufacturer_property_set_t
Context of the received Generic Manufacturer Property Set message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Generic Manufacturer Property 

user_access

uint8_t user_access
Enumeration indicating user access 



```

#### esp_ble_mesh_generic_server_cb_param_t


```


struct esp_ble_mesh_generic_server_cb_param_t
Generic Server Model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Generic Server Models 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received messages 

value

esp_ble_mesh_generic_server_cb_value_t value
Value of the received Generic Messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_GEN_ONOFF_CLI


```


ESP_BLE_MESH_MODEL_GEN_ONOFF_CLI(cli_pub, cli_data)
Define a new Generic OnOff Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic OnOff Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic OnOff Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_LEVEL_CLI


```


ESP_BLE_MESH_MODEL_GEN_LEVEL_CLI(cli_pub, cli_data)
Define a new Generic Level Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic Level Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic Level Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_DEF_TRANS_TIME_CLI


```


ESP_BLE_MESH_MODEL_GEN_DEF_TRANS_TIME_CLI(cli_pub, cli_data)
Define a new Generic Default Transition Time Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic Default Transition Time Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic Default Transition Time Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_POWER_ONOFF_CLI


```


ESP_BLE_MESH_MODEL_GEN_POWER_ONOFF_CLI(cli_pub, cli_data)
Define a new Generic Power OnOff Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic Power OnOff Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic Power OnOff Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_POWER_LEVEL_CLI


```


ESP_BLE_MESH_MODEL_GEN_POWER_LEVEL_CLI(cli_pub, cli_data)
Define a new Generic Power Level Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic Power Level Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic Power Level Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_BATTERY_CLI


```


ESP_BLE_MESH_MODEL_GEN_BATTERY_CLI(cli_pub, cli_data)
Define a new Generic Battery Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic Battery Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic Battery Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_LOCATION_CLI


```


ESP_BLE_MESH_MODEL_GEN_LOCATION_CLI(cli_pub, cli_data)
Define a new Generic Location Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic Location Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic Location Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_PROPERTY_CLI


```


ESP_BLE_MESH_MODEL_GEN_PROPERTY_CLI(cli_pub, cli_data)
Define a new Generic Property Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Generic Property Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Generic Location Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_ONOFF_SRV


```


ESP_BLE_MESH_MODEL_GEN_ONOFF_SRV(srv_pub, srv_data)
Generic Server Models related context. 
Define a new Generic OnOff Server Model.

Note
1. The Generic OnOff Server Model is a root model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_onoff_srv_t.


Returns
New Generic OnOff Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_LEVEL_SRV


```


ESP_BLE_MESH_MODEL_GEN_LEVEL_SRV(srv_pub, srv_data)
Define a new Generic Level Server Model. 

Note
1. The Generic Level Server Model is a root model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_level_srv_t.


Returns
New Generic Level Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_DEF_TRANS_TIME_SRV


```


ESP_BLE_MESH_MODEL_GEN_DEF_TRANS_TIME_SRV(srv_pub, srv_data)
Define a new Generic Default Transition Time Server Model. 

Note
1. The Generic Default Transition Time Server Model is a root model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_def_trans_time_srv_t.


Returns
New Generic Default Transition Time Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_POWER_ONOFF_SRV


```


ESP_BLE_MESH_MODEL_GEN_POWER_ONOFF_SRV(srv_pub, srv_data)
Define a new Generic Power OnOff Server Model. 

Note
1. The Generic Power OnOff Server model extends the Generic OnOff Server model. When this model is present on an element, the corresponding Generic Power OnOff Setup Server model shall also be present.
This model may be used to represent a variety of devices that do not fit any of the model descriptions that have been defined but support the generic properties of On/Off.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_power_onoff_srv_t.


Returns
New Generic Power OnOff Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_POWER_ONOFF_SETUP_SRV


```


ESP_BLE_MESH_MODEL_GEN_POWER_ONOFF_SETUP_SRV(srv_pub, srv_data)
Define a new Generic Power OnOff Setup Server Model. 

Note
1. The Generic Power OnOff Setup Server model extends the Generic Power OnOff Server model and the Generic Default Transition Time Server model.
This model shall support model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_power_onoff_setup_srv_t.


Returns
New Generic Power OnOff Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_POWER_LEVEL_SRV


```


ESP_BLE_MESH_MODEL_GEN_POWER_LEVEL_SRV(srv_pub, srv_data)
Define a new Generic Power Level Server Model. 

Note
1. The Generic Power Level Server model extends the Generic Power OnOff Server model and the Generic Level Server model. When this model is present on an Element, the corresponding Generic Power Level Setup Server model shall also be present.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_power_level_srv_t.


Returns
New Generic Power Level Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_POWER_LEVEL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_GEN_POWER_LEVEL_SETUP_SRV(srv_pub, srv_data)
Define a new Generic Power Level Setup Server Model. 

Note
1. The Generic Power Level Setup Server model extends the Generic Power Level Server model and the Generic Power OnOff Setup Server model.
This model shall support model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_power_level_setup_srv_t.


Returns
New Generic Power Level Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_BATTERY_SRV


```


ESP_BLE_MESH_MODEL_GEN_BATTERY_SRV(srv_pub, srv_data)
Define a new Generic Battery Server Model. 

Note
1. The Generic Battery Server Model is a root model.
This model shall support model publication and model subscription.
The model may be used to represent an element that is powered by a battery.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_battery_srv_t.


Returns
New Generic Battery Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_LOCATION_SRV


```


ESP_BLE_MESH_MODEL_GEN_LOCATION_SRV(srv_pub, srv_data)
Define a new Generic Location Server Model. 

Note
1. The Generic Location Server model is a root model. When this model is present on an Element, the corresponding Generic Location Setup Server model shall also be present.
This model shall support model publication and model subscription.
The model may be used to represent an element that knows its location (global or local).




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_location_srv_t.


Returns
New Generic Location Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_LOCATION_SETUP_SRV


```


ESP_BLE_MESH_MODEL_GEN_LOCATION_SETUP_SRV(srv_pub, srv_data)
Define a new Generic Location Setup Server Model. 

Note
1. The Generic Location Setup Server model extends the Generic Location Server model.
This model shall support model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_location_setup_srv_t.


Returns
New Generic Location Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_USER_PROP_SRV


```


ESP_BLE_MESH_MODEL_GEN_USER_PROP_SRV(srv_pub, srv_data)
Define a new Generic User Property Server Model. 

Note
1. The Generic User Property Server model is a root model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_user_prop_srv_t.


Returns
New Generic User Property Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_ADMIN_PROP_SRV


```


ESP_BLE_MESH_MODEL_GEN_ADMIN_PROP_SRV(srv_pub, srv_data)
Define a new Generic Admin Property Server Model. 

Note
1. The Generic Admin Property Server model extends the Generic User Property Server model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_admin_prop_srv_t.


Returns
New Generic Admin Property Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_MANUFACTURER_PROP_SRV


```


ESP_BLE_MESH_MODEL_GEN_MANUFACTURER_PROP_SRV(srv_pub, srv_data)
Define a new Generic Manufacturer Property Server Model. 

Note
1. The Generic Manufacturer Property Server model extends the Generic User Property Server model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_manu_prop_srv_t.


Returns
New Generic Manufacturer Property Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_GEN_CLIENT_PROP_SRV


```


ESP_BLE_MESH_MODEL_GEN_CLIENT_PROP_SRV(srv_pub, srv_data)
Define a new Generic User Property Server Model. 

Note
1. The Generic Client Property Server model is a root model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_gen_client_prop_srv_t.


Returns
New Generic Client Property Server Model instance. 



```



### Type Definitions


#### esp_ble_mesh_generic_client_cb_t


```


typedef void (*esp_ble_mesh_generic_client_cb_t)(esp_ble_mesh_generic_client_cb_event_t event, esp_ble_mesh_generic_client_cb_param_t *param)
Bluetooth Mesh Generic Client Model function. 
Generic Client Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_generic_server_cb_t


```


typedef void (*esp_ble_mesh_generic_server_cb_t)(esp_ble_mesh_generic_server_cb_event_t event, esp_ble_mesh_generic_server_cb_param_t *param)
Bluetooth Mesh Generic Server Model function. 
Generic Server Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_generic_client_cb_event_t


```


enum esp_ble_mesh_generic_client_cb_event_t
This enum value is the event of Generic Client Model 
Values:
ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT

enumerator ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT

ESP_BLE_MESH_GENERIC_CLIENT_SET_STATE_EVT

enumerator ESP_BLE_MESH_GENERIC_CLIENT_SET_STATE_EVT

ESP_BLE_MESH_GENERIC_CLIENT_PUBLISH_EVT

enumerator ESP_BLE_MESH_GENERIC_CLIENT_PUBLISH_EVT

ESP_BLE_MESH_GENERIC_CLIENT_TIMEOUT_EVT

enumerator ESP_BLE_MESH_GENERIC_CLIENT_TIMEOUT_EVT

ESP_BLE_MESH_GENERIC_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_GENERIC_CLIENT_EVT_MAX


```

#### esp_ble_mesh_gen_user_prop_access_t


```


enum esp_ble_mesh_gen_user_prop_access_t
This enum value is the access value of Generic User Property 
Values:
ESP_BLE_MESH_GEN_USER_ACCESS_PROHIBIT

enumerator ESP_BLE_MESH_GEN_USER_ACCESS_PROHIBIT

ESP_BLE_MESH_GEN_USER_ACCESS_READ

enumerator ESP_BLE_MESH_GEN_USER_ACCESS_READ

ESP_BLE_MESH_GEN_USER_ACCESS_WRITE

enumerator ESP_BLE_MESH_GEN_USER_ACCESS_WRITE

ESP_BLE_MESH_GEN_USER_ACCESS_READ_WRITE

enumerator ESP_BLE_MESH_GEN_USER_ACCESS_READ_WRITE


```

#### esp_ble_mesh_gen_admin_prop_access_t


```


enum esp_ble_mesh_gen_admin_prop_access_t
This enum value is the access value of Generic Admin Property 
Values:
ESP_BLE_MESH_GEN_ADMIN_NOT_USER_PROP

enumerator ESP_BLE_MESH_GEN_ADMIN_NOT_USER_PROP

ESP_BLE_MESH_GEN_ADMIN_ACCESS_READ

enumerator ESP_BLE_MESH_GEN_ADMIN_ACCESS_READ

ESP_BLE_MESH_GEN_ADMIN_ACCESS_WRITE

enumerator ESP_BLE_MESH_GEN_ADMIN_ACCESS_WRITE

ESP_BLE_MESH_GEN_ADMIN_ACCESS_READ_WRITE

enumerator ESP_BLE_MESH_GEN_ADMIN_ACCESS_READ_WRITE


```

#### esp_ble_mesh_gen_manu_prop_access_t


```


enum esp_ble_mesh_gen_manu_prop_access_t
This enum value is the access value of Generic Manufacturer Property 
Values:
ESP_BLE_MESH_GEN_MANU_NOT_USER_PROP

enumerator ESP_BLE_MESH_GEN_MANU_NOT_USER_PROP

ESP_BLE_MESH_GEN_MANU_ACCESS_READ

enumerator ESP_BLE_MESH_GEN_MANU_ACCESS_READ


```

#### esp_ble_mesh_generic_server_cb_event_t


```


enum esp_ble_mesh_generic_server_cb_event_t
This enum value is the event of Generic Server Model 
Values:
ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT

When get_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, no event will be callback to the application layer when Generic Get messages are received.
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, this event will be callback to the application layer when Generic Set/Set Unack messages are received. 



ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT

enumerator ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT
When get_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Generic Get messages are received. 

ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT

enumerator ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Generic Set/Set Unack messages are received. 

ESP_BLE_MESH_GENERIC_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_GENERIC_SERVER_EVT_MAX


```



### Sensor Client/Server Models




### Header File


* components/bt/esp_ble_mesh/api/models/include/esp_ble_mesh_sensor_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_sensor_model_api.h"
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


#### esp_ble_mesh_register_sensor_client_callback


```


esp_err_t esp_ble_mesh_register_sensor_client_callback(esp_ble_mesh_sensor_client_cb_t callback)
Register BLE Mesh Sensor Client Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_sensor_client_get_state


```


esp_err_t esp_ble_mesh_sensor_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_sensor_client_get_state_t *get_state)
Get the value of Sensor Server Model states using the Sensor Client Model get messages. 

Note
If you want to know the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_sensor_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
get_state -- [in] Pointer to sensor get message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_sensor_client_set_state


```


esp_err_t esp_ble_mesh_sensor_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_sensor_client_set_state_t *set_state)
Set the value of Sensor Server Model states using the Sensor Client Model set messages. 

Note
If you want to know the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_sensor_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
set_state -- [in] Pointer to sensor set message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_sensor_server_callback


```


esp_err_t esp_ble_mesh_register_sensor_server_callback(esp_ble_mesh_sensor_server_cb_t callback)
Register BLE Mesh Sensor Server Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_sensor_client_get_state_t


```


union esp_ble_mesh_sensor_client_get_state_t

#include <esp_ble_mesh_sensor_model_api.h>
Sensor Client Model get message union. 

Public Members
descriptor_get

esp_ble_mesh_sensor_descriptor_get_t descriptor_get
For ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET 

cadence_get

esp_ble_mesh_sensor_cadence_get_t cadence_get
For ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_GET 

settings_get

esp_ble_mesh_sensor_settings_get_t settings_get
For ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_GET 

setting_get

esp_ble_mesh_sensor_setting_get_t setting_get
For ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_GET 

sensor_get

esp_ble_mesh_sensor_get_t sensor_get
For ESP_BLE_MESH_MODEL_OP_SENSOR_GET 

column_get

esp_ble_mesh_sensor_column_get_t column_get
For ESP_BLE_MESH_MODEL_OP_SENSOR_COLUMN_GET 

series_get

esp_ble_mesh_sensor_series_get_t series_get
For ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_GET 



```

#### esp_ble_mesh_sensor_client_set_state_t


```


union esp_ble_mesh_sensor_client_set_state_t

#include <esp_ble_mesh_sensor_model_api.h>
Sensor Client Model set message union. 

Public Members
cadence_set

esp_ble_mesh_sensor_cadence_set_t cadence_set
For ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET & ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK 

setting_set

esp_ble_mesh_sensor_setting_set_t setting_set
For ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET & ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK 



```

#### esp_ble_mesh_sensor_client_status_cb_t


```


union esp_ble_mesh_sensor_client_status_cb_t

#include <esp_ble_mesh_sensor_model_api.h>
Sensor Client Model received message union. 

Public Members
descriptor_status

esp_ble_mesh_sensor_descriptor_status_cb_t descriptor_status
For ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS 

cadence_status

esp_ble_mesh_sensor_cadence_status_cb_t cadence_status
For ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_STATUS 

settings_status

esp_ble_mesh_sensor_settings_status_cb_t settings_status
For ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS 

setting_status

esp_ble_mesh_sensor_setting_status_cb_t setting_status
For ESP_BLE_MESH_MODEL_OP_SENSOR_SETTING_STATUS 

sensor_status

esp_ble_mesh_sensor_status_cb_t sensor_status
For ESP_BLE_MESH_MODEL_OP_SENSOR_STATUS 

column_status

esp_ble_mesh_sensor_column_status_cb_t column_status
For ESP_BLE_MESH_MODEL_OP_SENSOR_COLUMN_STATUS 

series_status

esp_ble_mesh_sensor_series_status_cb_t series_status
For ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_STATUS 



```

#### esp_ble_mesh_sensor_server_state_change_t


```


union esp_ble_mesh_sensor_server_state_change_t

#include <esp_ble_mesh_sensor_model_api.h>
Sensor Server Model state change value union. 

Public Members
sensor_cadence_set

esp_ble_mesh_state_change_sensor_cadence_set_t sensor_cadence_set
The recv_op in ctx can be used to decide which state is changed. Sensor Cadence Set 

sensor_setting_set

esp_ble_mesh_state_change_sensor_setting_set_t sensor_setting_set
Sensor Setting Set 



```

#### esp_ble_mesh_sensor_server_recv_get_msg_t


```


union esp_ble_mesh_sensor_server_recv_get_msg_t

#include <esp_ble_mesh_sensor_model_api.h>
Sensor Server Model received get message union. 

Public Members
sensor_descriptor

esp_ble_mesh_server_recv_sensor_descriptor_get_t sensor_descriptor
Sensor Descriptor Get 

sensor_cadence

esp_ble_mesh_server_recv_sensor_cadence_get_t sensor_cadence
Sensor Cadence Get 

sensor_settings

esp_ble_mesh_server_recv_sensor_settings_get_t sensor_settings
Sensor Settings Get 

sensor_setting

esp_ble_mesh_server_recv_sensor_setting_get_t sensor_setting
Sensor Setting Get 

sensor_data

esp_ble_mesh_server_recv_sensor_get_t sensor_data
Sensor Get 

sensor_column

esp_ble_mesh_server_recv_sensor_column_get_t sensor_column
Sensor Column Get 

sensor_series

esp_ble_mesh_server_recv_sensor_series_get_t sensor_series
Sensor Series Get 



```

#### esp_ble_mesh_sensor_server_recv_set_msg_t


```


union esp_ble_mesh_sensor_server_recv_set_msg_t

#include <esp_ble_mesh_sensor_model_api.h>
Sensor Server Model received set message union. 

Public Members
sensor_cadence

esp_ble_mesh_server_recv_sensor_cadence_set_t sensor_cadence
Sensor Cadence Set 

sensor_setting

esp_ble_mesh_server_recv_sensor_setting_set_t sensor_setting
Sensor Setting Set 



```

#### esp_ble_mesh_sensor_server_cb_value_t


```


union esp_ble_mesh_sensor_server_cb_value_t

#include <esp_ble_mesh_sensor_model_api.h>
Sensor Server Model callback value union. 

Public Members
state_change

esp_ble_mesh_sensor_server_state_change_t state_change
ESP_BLE_MESH_SENSOR_SERVER_STATE_CHANGE_EVT 

get

esp_ble_mesh_sensor_server_recv_get_msg_t get
ESP_BLE_MESH_SENSOR_SERVER_RECV_GET_MSG_EVT 

set

esp_ble_mesh_sensor_server_recv_set_msg_t set
ESP_BLE_MESH_SENSOR_SERVER_RECV_SET_MSG_EVT 



```



### Structures


#### esp_ble_mesh_sensor_descriptor_get_t


```


struct esp_ble_mesh_sensor_descriptor_get_t
Bluetooth Mesh Sensor Client Model Get and Set parameters structure. 
Parameters of Sensor Descriptor Get 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property ID of a sensor (optional) 



```

#### esp_ble_mesh_sensor_cadence_get_t


```


struct esp_ble_mesh_sensor_cadence_get_t
Parameter of Sensor Cadence Get 

Public Members
property_id

uint16_t property_id
Property ID of a sensor 



```

#### esp_ble_mesh_sensor_cadence_set_t


```


struct esp_ble_mesh_sensor_cadence_set_t
Parameters of Sensor Cadence Set 

Public Members
property_id

uint16_t property_id
Property ID for the sensor 

fast_cadence_period_divisor

uint8_t fast_cadence_period_divisor
Divisor for the publish period 

status_trigger_type

uint8_t status_trigger_type
The unit and format of the Status Trigger Delta fields 

status_trigger_delta_down

struct net_buf_simple *status_trigger_delta_down
Delta down value that triggers a status message 

status_trigger_delta_up

struct net_buf_simple *status_trigger_delta_up
Delta up value that triggers a status message 

status_min_interval

uint8_t status_min_interval
Minimum interval between two consecutive Status messages 

fast_cadence_low

struct net_buf_simple *fast_cadence_low
Low value for the fast cadence range 

fast_cadence_high

struct net_buf_simple *fast_cadence_high
Fast value for the fast cadence range 



```

#### esp_ble_mesh_sensor_settings_get_t


```


struct esp_ble_mesh_sensor_settings_get_t
Parameter of Sensor Settings Get 

Public Members
sensor_property_id

uint16_t sensor_property_id
Property ID of a sensor 



```

#### esp_ble_mesh_sensor_setting_get_t


```


struct esp_ble_mesh_sensor_setting_get_t
Parameters of Sensor Setting Get 

Public Members
sensor_property_id

uint16_t sensor_property_id
Property ID of a sensor 

sensor_setting_property_id

uint16_t sensor_setting_property_id
Setting ID identifying a setting within a sensor 



```

#### esp_ble_mesh_sensor_setting_set_t


```


struct esp_ble_mesh_sensor_setting_set_t
Parameters of Sensor Setting Set 

Public Members
sensor_property_id

uint16_t sensor_property_id
Property ID identifying a sensor 

sensor_setting_property_id

uint16_t sensor_setting_property_id
Setting ID identifying a setting within a sensor 

sensor_setting_raw

struct net_buf_simple *sensor_setting_raw
Raw value for the setting 



```

#### esp_ble_mesh_sensor_get_t


```


struct esp_ble_mesh_sensor_get_t
Parameters of Sensor Get 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property ID for the sensor (optional) 



```

#### esp_ble_mesh_sensor_column_get_t


```


struct esp_ble_mesh_sensor_column_get_t
Parameters of Sensor Column Get 

Public Members
property_id

uint16_t property_id
Property identifying a sensor 

raw_value_x

struct net_buf_simple *raw_value_x
Raw value identifying a column 



```

#### esp_ble_mesh_sensor_series_get_t


```


struct esp_ble_mesh_sensor_series_get_t
Parameters of Sensor Series Get 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property identifying a sensor 

raw_value_x1

struct net_buf_simple *raw_value_x1
Raw value identifying a starting column (optional) 

raw_value_x2

struct net_buf_simple *raw_value_x2
Raw value identifying an ending column (C.1) 



```

#### esp_ble_mesh_sensor_descriptor_status_cb_t


```


struct esp_ble_mesh_sensor_descriptor_status_cb_t
Bluetooth Mesh Sensor Client Model Get and Set callback parameters structure. 
Parameter of Sensor Descriptor Status 

Public Members
descriptor

struct net_buf_simple *descriptor
Sequence of 8-octet sensor descriptors (optional) 



```

#### esp_ble_mesh_sensor_cadence_status_cb_t


```


struct esp_ble_mesh_sensor_cadence_status_cb_t
Parameters of Sensor Cadence Status 

Public Members
property_id

uint16_t property_id
Property for the sensor 

sensor_cadence_value

struct net_buf_simple *sensor_cadence_value
Value of sensor cadence state 



```

#### esp_ble_mesh_sensor_settings_status_cb_t


```


struct esp_ble_mesh_sensor_settings_status_cb_t
Parameters of Sensor Settings Status 

Public Members
sensor_property_id

uint16_t sensor_property_id
Property ID identifying a sensor 

sensor_setting_property_ids

struct net_buf_simple *sensor_setting_property_ids
A sequence of N sensor setting property IDs (optional) 



```

#### esp_ble_mesh_sensor_setting_status_cb_t


```


struct esp_ble_mesh_sensor_setting_status_cb_t
Parameters of Sensor Setting Status 

Public Members
op_en

bool op_en
Indicate id optional parameters are included 

sensor_property_id

uint16_t sensor_property_id
Property ID identifying a sensor 

sensor_setting_property_id

uint16_t sensor_setting_property_id
Setting ID identifying a setting within a sensor 

sensor_setting_access

uint8_t sensor_setting_access
Read/Write access rights for the setting (optional) 

sensor_setting_raw

struct net_buf_simple *sensor_setting_raw
Raw value for the setting 



```

#### esp_ble_mesh_sensor_status_cb_t


```


struct esp_ble_mesh_sensor_status_cb_t
Parameter of Sensor Status 

Public Members
marshalled_sensor_data

struct net_buf_simple *marshalled_sensor_data
Value of sensor data state (optional) 



```

#### esp_ble_mesh_sensor_column_status_cb_t


```


struct esp_ble_mesh_sensor_column_status_cb_t
Parameters of Sensor Column Status 

Public Members
property_id

uint16_t property_id
Property identifying a sensor and the Y axis 

sensor_column_value

struct net_buf_simple *sensor_column_value
Left values of sensor column status 



```

#### esp_ble_mesh_sensor_series_status_cb_t


```


struct esp_ble_mesh_sensor_series_status_cb_t
Parameters of Sensor Series Status 

Public Members
property_id

uint16_t property_id
Property identifying a sensor and the Y axis 

sensor_series_value

struct net_buf_simple *sensor_series_value
Left values of sensor series status 



```

#### esp_ble_mesh_sensor_client_cb_param_t


```


struct esp_ble_mesh_sensor_client_cb_param_t
Sensor Client Model callback parameters 

Public Members
error_code

int error_code
0: success, otherwise failure. For the error code values please refer to errno.h file. A negative sign is added to the standard error codes in errno.h. 

params

esp_ble_mesh_client_common_param_t *params
The client common parameters. 

status_cb

esp_ble_mesh_sensor_client_status_cb_t status_cb
The sensor status message callback values 



```

#### esp_ble_mesh_sensor_descriptor_t


```


struct esp_ble_mesh_sensor_descriptor_t
Parameters of Sensor Descriptor state 

Public Members
positive_tolerance

uint32_t positive_tolerance
The value of Sensor Positive Tolerance field 

negative_tolerance

uint32_t negative_tolerance
The value of Sensor Negative Tolerance field 

sampling_function

uint32_t sampling_function
The value of Sensor Sampling Function field 

measure_period

uint8_t measure_period
The value of Sensor Measurement Period field 

update_interval

uint8_t update_interval
The value of Sensor Update Interval field 



```

#### esp_ble_mesh_sensor_setting_t


```


struct esp_ble_mesh_sensor_setting_t
Parameters of Sensor Setting state 

Public Members
property_id

uint16_t property_id
The value of Sensor Setting Property ID field 

access

uint8_t access
The value of Sensor Setting Access field 

raw

struct net_buf_simple *raw
The value of Sensor Setting Raw field 



```

#### esp_ble_mesh_sensor_cadence_t


```


struct esp_ble_mesh_sensor_cadence_t
Parameters of Sensor Cadence state 

Public Members
period_divisor

uint8_t period_divisor
The value of Fast Cadence Period Divisor field 

trigger_type

uint8_t trigger_type
The value of Status Trigger Type field 

trigger_delta_down

struct net_buf_simple *trigger_delta_down
Note: The parameter "size" in trigger_delta_down, trigger_delta_up, fast_cadence_low & fast_cadence_high indicates the exact length of these four parameters, and they are associated with the Sensor Property ID. Users need to initialize the "size" precisely. The value of Status Trigger Delta Down field 

trigger_delta_up

struct net_buf_simple *trigger_delta_up
The value of Status Trigger Delta Up field 

min_interval

uint8_t min_interval
The value of Status Min Interval field 

fast_cadence_low

struct net_buf_simple *fast_cadence_low
The value of Fast Cadence Low field 

fast_cadence_high

struct net_buf_simple *fast_cadence_high
The value of Fast Cadence High field 



```

#### esp_ble_mesh_sensor_data_t


```


struct esp_ble_mesh_sensor_data_t
Parameters of Sensor Data state 

Public Members
format

uint8_t format
Format A: The Length field is a 1-based uint4 value (valid range 0x0–0xF, representing range of 1 – 16). Format B: The Length field is a 1-based uint7 value (valid range 0x0–0x7F, representing range of 1 – 127). The value 0x7F represents a length of zero. The value of the Sensor Data format 

length

uint8_t length
The value of the Sensor Data length 

raw_value

struct net_buf_simple *raw_value
The value of Sensor Data raw value 



```

#### esp_ble_mesh_sensor_series_column_t


```


struct esp_ble_mesh_sensor_series_column_t
Parameters of Sensor Series Column state 

Public Members
raw_value_x

struct net_buf_simple *raw_value_x
The value of Sensor Raw Value X field 

column_width

struct net_buf_simple *column_width
The value of Sensor Column Width field 

raw_value_y

struct net_buf_simple *raw_value_y
The value of Sensor Raw Value Y field 



```

#### esp_ble_mesh_sensor_state_t


```


struct esp_ble_mesh_sensor_state_t
Parameters of Sensor states 

Public Members
sensor_property_id

uint16_t sensor_property_id
The value of Sensor Property ID field 

descriptor

esp_ble_mesh_sensor_descriptor_t descriptor
Parameters of the Sensor Descriptor state 

setting_count

const uint8_t setting_count
Multiple Sensor Setting states may be present for each sensor. The Sensor Setting Property ID values shall be unique for each Sensor Property ID that identifies a sensor within an element. 

settings

esp_ble_mesh_sensor_setting_t *settings
Parameters of the Sensor Setting state 

cadence

esp_ble_mesh_sensor_cadence_t *cadence
The Sensor Cadence state may be not supported by sensors based on device properties referencing "non-scalar characteristics" such as "histograms" or "composite characteristics". Parameters of the Sensor Cadence state 

sensor_data

esp_ble_mesh_sensor_data_t sensor_data
Parameters of the Sensor Data state 

series_column

esp_ble_mesh_sensor_series_column_t series_column
Parameters of the Sensor Series Column state 



```

#### esp_ble_mesh_sensor_srv_t


```


struct esp_ble_mesh_sensor_srv_t
User data of Sensor Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Sensor Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state_count

const uint8_t state_count
Sensor state count 

states

esp_ble_mesh_sensor_state_t *states
Parameters of the Sensor states 



```

#### esp_ble_mesh_sensor_setup_srv_t


```


struct esp_ble_mesh_sensor_setup_srv_t
User data of Sensor Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Sensor Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state_count

const uint8_t state_count
Sensor state count 

states

esp_ble_mesh_sensor_state_t *states
Parameters of the Sensor states 



```

#### esp_ble_mesh_state_change_sensor_cadence_set_t


```


struct esp_ble_mesh_state_change_sensor_cadence_set_t
Parameters of Sensor Cadence Set state change event 

Public Members
property_id

uint16_t property_id
The value of Sensor Property ID state 

period_divisor

uint8_t period_divisor
The value of Fast Cadence Period Divisor state 

trigger_type

uint8_t trigger_type
The value of Status Trigger Type state 

trigger_delta_down

struct net_buf_simple *trigger_delta_down
The value of Status Trigger Delta Down state 

trigger_delta_up

struct net_buf_simple *trigger_delta_up
The value of Status Trigger Delta Up state 

min_interval

uint8_t min_interval
The value of Status Min Interval state 

fast_cadence_low

struct net_buf_simple *fast_cadence_low
The value of Fast Cadence Low state 

fast_cadence_high

struct net_buf_simple *fast_cadence_high
The value of Fast Cadence High state 



```

#### esp_ble_mesh_state_change_sensor_setting_set_t


```


struct esp_ble_mesh_state_change_sensor_setting_set_t
Parameters of Sensor Setting Set state change event 

Public Members
property_id

uint16_t property_id
The value of Sensor Property ID state 

setting_property_id

uint16_t setting_property_id
The value of Sensor Setting Property ID state 

setting_value

struct net_buf_simple *setting_value
The value of Sensor Property Value state 



```

#### esp_ble_mesh_server_recv_sensor_descriptor_get_t


```


struct esp_ble_mesh_server_recv_sensor_descriptor_get_t
Context of the received Sensor Descriptor Get message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property ID of a sensor (optional) 



```

#### esp_ble_mesh_server_recv_sensor_cadence_get_t


```


struct esp_ble_mesh_server_recv_sensor_cadence_get_t
Context of the received Sensor Cadence Get message 

Public Members
property_id

uint16_t property_id
Property ID of a sensor 



```

#### esp_ble_mesh_server_recv_sensor_settings_get_t


```


struct esp_ble_mesh_server_recv_sensor_settings_get_t
Context of the received Sensor Settings Get message 

Public Members
property_id

uint16_t property_id
Property ID of a sensor 



```

#### esp_ble_mesh_server_recv_sensor_setting_get_t


```


struct esp_ble_mesh_server_recv_sensor_setting_get_t
Context of the received Sensor Setting Get message 

Public Members
property_id

uint16_t property_id
Property ID of a sensor 

setting_property_id

uint16_t setting_property_id
Setting ID identifying a setting within a sensor 



```

#### esp_ble_mesh_server_recv_sensor_get_t


```


struct esp_ble_mesh_server_recv_sensor_get_t
Context of the received Sensor Get message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property ID for the sensor (optional) 



```

#### esp_ble_mesh_server_recv_sensor_column_get_t


```


struct esp_ble_mesh_server_recv_sensor_column_get_t
Context of the received Sensor Column Get message 

Public Members
property_id

uint16_t property_id
Property identifying a sensor 

raw_value_x

struct net_buf_simple *raw_value_x
Raw value identifying a column 



```

#### esp_ble_mesh_server_recv_sensor_series_get_t


```


struct esp_ble_mesh_server_recv_sensor_series_get_t
Context of the received Sensor Series Get message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

property_id

uint16_t property_id
Property identifying a sensor 

raw_value

struct net_buf_simple *raw_value
Raw value containing X1 and X2 (optional) 



```

#### esp_ble_mesh_server_recv_sensor_cadence_set_t


```


struct esp_ble_mesh_server_recv_sensor_cadence_set_t
Context of the received Sensor Cadence Set message 

Public Members
property_id

uint16_t property_id
Property ID for the sensor 

cadence

struct net_buf_simple *cadence
Value of Sensor Cadence state 



```

#### esp_ble_mesh_server_recv_sensor_setting_set_t


```


struct esp_ble_mesh_server_recv_sensor_setting_set_t
Context of the received Sensor Setting Set message 

Public Members
property_id

uint16_t property_id
Property ID identifying a sensor 

setting_property_id

uint16_t setting_property_id
Setting ID identifying a setting within a sensor 

setting_raw

struct net_buf_simple *setting_raw
Raw value for the setting 



```

#### esp_ble_mesh_sensor_server_cb_param_t


```


struct esp_ble_mesh_sensor_server_cb_param_t
Sensor Server Model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Sensor Server Models 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received messages 

value

esp_ble_mesh_sensor_server_cb_value_t value
Value of the received Sensor Messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_SENSOR_CLI


```


ESP_BLE_MESH_MODEL_SENSOR_CLI(cli_pub, cli_data)
Define a new Sensor Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Sensor Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Sensor Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_SENSOR_SRV


```


ESP_BLE_MESH_MODEL_SENSOR_SRV(srv_pub, srv_data)
Sensor Server Models related context. 
Define a new Sensor Server Model.

Note
1. The Sensor Server model is a root model. When this model is present on an element, the corresponding Sensor Setup Server model shall also be present.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_sensor_srv_t.


Returns
New Sensor Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_SENSOR_SETUP_SRV


```


ESP_BLE_MESH_MODEL_SENSOR_SETUP_SRV(srv_pub, srv_data)
Define a new Sensor Setup Server Model. 

Note
1. The Sensor Setup Server model extends the Sensor Server model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_sensor_setup_srv_t.


Returns
New Sensor Setup Server Model instance. 



```

#### ESP_BLE_MESH_INVALID_SENSOR_PROPERTY_ID


```


ESP_BLE_MESH_INVALID_SENSOR_PROPERTY_ID
Invalid Sensor Property ID 

```

#### ESP_BLE_MESH_SENSOR_PROPERTY_ID_LEN


```


ESP_BLE_MESH_SENSOR_PROPERTY_ID_LEN
Length of Sensor Property ID 

```

#### ESP_BLE_MESH_SENSOR_DESCRIPTOR_LEN


```


ESP_BLE_MESH_SENSOR_DESCRIPTOR_LEN
Length of Sensor Descriptor state 

```

#### ESP_BLE_MESH_SENSOR_UNSPECIFIED_POS_TOLERANCE


```


ESP_BLE_MESH_SENSOR_UNSPECIFIED_POS_TOLERANCE
Unspecified Sensor Positive Tolerance 

```

#### ESP_BLE_MESH_SENSOR_UNSPECIFIED_NEG_TOLERANCE


```


ESP_BLE_MESH_SENSOR_UNSPECIFIED_NEG_TOLERANCE
Unspecified Sensor Negative Tolerance 

```

#### ESP_BLE_MESH_SENSOR_NOT_APPL_MEASURE_PERIOD


```


ESP_BLE_MESH_SENSOR_NOT_APPL_MEASURE_PERIOD
Not applicable Sensor Measurement Period 

```

#### ESP_BLE_MESH_SENSOR_NOT_APPL_UPDATE_INTERVAL


```


ESP_BLE_MESH_SENSOR_NOT_APPL_UPDATE_INTERVAL
Not applicable Sensor Update Interval 

```

#### ESP_BLE_MESH_INVALID_SENSOR_SETTING_PROPERTY_ID


```


ESP_BLE_MESH_INVALID_SENSOR_SETTING_PROPERTY_ID
Invalid Sensor Setting Property ID 

```

#### ESP_BLE_MESH_SENSOR_SETTING_PROPERTY_ID_LEN


```


ESP_BLE_MESH_SENSOR_SETTING_PROPERTY_ID_LEN
Length of Sensor Setting Property ID 

```

#### ESP_BLE_MESH_SENSOR_SETTING_ACCESS_LEN


```


ESP_BLE_MESH_SENSOR_SETTING_ACCESS_LEN
Length of Sensor Setting Access 

```

#### ESP_BLE_MESH_SENSOR_SETTING_ACCESS_READ


```


ESP_BLE_MESH_SENSOR_SETTING_ACCESS_READ
Sensor Setting Access - Read 

```

#### ESP_BLE_MESH_SENSOR_SETTING_ACCESS_READ_WRITE


```


ESP_BLE_MESH_SENSOR_SETTING_ACCESS_READ_WRITE
Sensor Setting Access - Read & Write 

```

#### ESP_BLE_MESH_SENSOR_DIVISOR_TRIGGER_TYPE_LEN


```


ESP_BLE_MESH_SENSOR_DIVISOR_TRIGGER_TYPE_LEN
Length of Sensor Divisor Trigger Type 

```

#### ESP_BLE_MESH_SENSOR_STATUS_MIN_INTERVAL_LEN


```


ESP_BLE_MESH_SENSOR_STATUS_MIN_INTERVAL_LEN
Length of Sensor Status Min Interval 

```

#### ESP_BLE_MESH_SENSOR_PERIOD_DIVISOR_MAX_VALUE


```


ESP_BLE_MESH_SENSOR_PERIOD_DIVISOR_MAX_VALUE
Maximum value of Sensor Period Divisor 

```

#### ESP_BLE_MESH_SENSOR_STATUS_MIN_INTERVAL_MAX


```


ESP_BLE_MESH_SENSOR_STATUS_MIN_INTERVAL_MAX
Maximum value of Sensor Status Min Interval 

```

#### ESP_BLE_MESH_SENSOR_STATUS_TRIGGER_TYPE_CHAR


```


ESP_BLE_MESH_SENSOR_STATUS_TRIGGER_TYPE_CHAR
Sensor Status Trigger Type - Format Type of the characteristic that the Sensor Property ID state references 

```

#### ESP_BLE_MESH_SENSOR_STATUS_TRIGGER_TYPE_UINT16


```


ESP_BLE_MESH_SENSOR_STATUS_TRIGGER_TYPE_UINT16
Sensor Status Trigger Type - Format Type "uint16" 

```

#### ESP_BLE_MESH_SENSOR_DATA_FORMAT_A


```


ESP_BLE_MESH_SENSOR_DATA_FORMAT_A
Sensor Data Format A 

```

#### ESP_BLE_MESH_SENSOR_DATA_FORMAT_B


```


ESP_BLE_MESH_SENSOR_DATA_FORMAT_B
Sensor Data Format B 

```

#### ESP_BLE_MESH_SENSOR_DATA_FORMAT_A_MPID_LEN


```


ESP_BLE_MESH_SENSOR_DATA_FORMAT_A_MPID_LEN
MPID length of Sensor Data Format A 

```

#### ESP_BLE_MESH_SENSOR_DATA_FORMAT_B_MPID_LEN


```


ESP_BLE_MESH_SENSOR_DATA_FORMAT_B_MPID_LEN
MPID length of Sensor Data Format B 

```

#### ESP_BLE_MESH_SENSOR_DATA_ZERO_LEN


```


ESP_BLE_MESH_SENSOR_DATA_ZERO_LEN
Zero length of Sensor Data.
Note: The Length field is a 1-based uint7 value (valid range 0x0–0x7F, representing range of 1–127). The value 0x7F represents a length of zero. 

```

#### ESP_BLE_MESH_GET_SENSOR_DATA_FORMAT


```


ESP_BLE_MESH_GET_SENSOR_DATA_FORMAT(_data)
Get format of the sensor data. 

Note
Multiple sensor data may be concatenated. Make sure the _data pointer is updated before getting the format of the corresponding sensor data.


Parameters

_data -- Pointer to the start of the sensor data.


Returns
Format of the sensor data. 



```

#### ESP_BLE_MESH_GET_SENSOR_DATA_LENGTH


```


ESP_BLE_MESH_GET_SENSOR_DATA_LENGTH(_data, _fmt)
Get length of the sensor data. 

Note
Multiple sensor data may be concatenated. Make sure the _data pointer is updated before getting the length of the corresponding sensor data.


Parameters

_data -- Pointer to the start of the sensor data. 
_fmt -- Format of the sensor data.


Returns
Length (zero-based) of the sensor data. 



```

#### ESP_BLE_MESH_GET_SENSOR_DATA_PROPERTY_ID


```


ESP_BLE_MESH_GET_SENSOR_DATA_PROPERTY_ID(_data, _fmt)
Get Sensor Property ID of the sensor data. 

Note
Multiple sensor data may be concatenated. Make sure the _data pointer is updated before getting Sensor Property ID of the corresponding sensor data.


Parameters

_data -- Pointer to the start of the sensor data. 
_fmt -- Format of the sensor data.


Returns
Sensor Property ID of the sensor data. 



```

#### ESP_BLE_MESH_SENSOR_DATA_FORMAT_A_MPID


```


ESP_BLE_MESH_SENSOR_DATA_FORMAT_A_MPID(_len, _id)
Generate a MPID value for sensor data with Format A. 

Note
1. The Format field is 0b0 and indicates that Format A is used.
The Length field is a 1-based uint4 value (valid range 0x0–0xF, representing range of 1–16).
The Property ID is an 11-bit bit field representing 11 LSb of a Property ID.
This format may be used for Property Values that are not longer than 16 octets and for Property IDs less than 0x0800.




Parameters

_len -- Length of Sensor Raw value. 
_id -- Sensor Property ID.


Returns
2-octet MPID value for sensor data with Format A. 



```

#### ESP_BLE_MESH_SENSOR_DATA_FORMAT_B_MPID


```


ESP_BLE_MESH_SENSOR_DATA_FORMAT_B_MPID(_len, _id)
Generate a MPID value for sensor data with Format B. 

Note
1. The Format field is 0b1 and indicates Format B is used.
The Length field is a 1-based uint7 value (valid range 0x0–0x7F, representing range of 1–127). The value 0x7F represents a length of zero.
The Property ID is a 16-bit bit field representing a Property ID.
This format may be used for Property Values not longer than 128 octets and for any Property IDs. Property values longer than 128 octets are not supported by the Sensor Status message.
Exclude the generated 1-octet value, the 2-octet Sensor Property ID




Parameters

_len -- Length of Sensor Raw value. 
_id -- Sensor Property ID.


Returns
3-octet MPID value for sensor data with Format B. 



```



### Type Definitions


#### esp_ble_mesh_sensor_client_cb_t


```


typedef void (*esp_ble_mesh_sensor_client_cb_t)(esp_ble_mesh_sensor_client_cb_event_t event, esp_ble_mesh_sensor_client_cb_param_t *param)
Bluetooth Mesh Sensor Client Model function. 
Sensor Client Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_sensor_server_cb_t


```


typedef void (*esp_ble_mesh_sensor_server_cb_t)(esp_ble_mesh_sensor_server_cb_event_t event, esp_ble_mesh_sensor_server_cb_param_t *param)
Bluetooth Mesh Sensor Server Model function. 
Sensor Server Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_sensor_client_cb_event_t


```


enum esp_ble_mesh_sensor_client_cb_event_t
This enum value is the event of Sensor Client Model 
Values:
ESP_BLE_MESH_SENSOR_CLIENT_GET_STATE_EVT

enumerator ESP_BLE_MESH_SENSOR_CLIENT_GET_STATE_EVT

ESP_BLE_MESH_SENSOR_CLIENT_SET_STATE_EVT

enumerator ESP_BLE_MESH_SENSOR_CLIENT_SET_STATE_EVT

ESP_BLE_MESH_SENSOR_CLIENT_PUBLISH_EVT

enumerator ESP_BLE_MESH_SENSOR_CLIENT_PUBLISH_EVT

ESP_BLE_MESH_SENSOR_CLIENT_TIMEOUT_EVT

enumerator ESP_BLE_MESH_SENSOR_CLIENT_TIMEOUT_EVT

ESP_BLE_MESH_SENSOR_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_SENSOR_CLIENT_EVT_MAX


```

#### esp_ble_mesh_sensor_sample_func


```


enum esp_ble_mesh_sensor_sample_func
This enum value is value of Sensor Sampling Function 
Values:
ESP_BLE_MESH_SAMPLE_FUNC_UNSPECIFIED

enumerator ESP_BLE_MESH_SAMPLE_FUNC_UNSPECIFIED

ESP_BLE_MESH_SAMPLE_FUNC_INSTANTANEOUS

enumerator ESP_BLE_MESH_SAMPLE_FUNC_INSTANTANEOUS

ESP_BLE_MESH_SAMPLE_FUNC_ARITHMETIC_MEAN

enumerator ESP_BLE_MESH_SAMPLE_FUNC_ARITHMETIC_MEAN

ESP_BLE_MESH_SAMPLE_FUNC_RMS

enumerator ESP_BLE_MESH_SAMPLE_FUNC_RMS

ESP_BLE_MESH_SAMPLE_FUNC_MAXIMUM

enumerator ESP_BLE_MESH_SAMPLE_FUNC_MAXIMUM

ESP_BLE_MESH_SAMPLE_FUNC_MINIMUM

enumerator ESP_BLE_MESH_SAMPLE_FUNC_MINIMUM

ESP_BLE_MESH_SAMPLE_FUNC_ACCUMULATED

enumerator ESP_BLE_MESH_SAMPLE_FUNC_ACCUMULATED

ESP_BLE_MESH_SAMPLE_FUNC_COUNT

enumerator ESP_BLE_MESH_SAMPLE_FUNC_COUNT


```

#### esp_ble_mesh_sensor_server_cb_event_t


```


enum esp_ble_mesh_sensor_server_cb_event_t
This enum value is the event of Sensor Server Model 
Values:
ESP_BLE_MESH_SENSOR_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_SENSOR_SERVER_STATE_CHANGE_EVT

When get_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, no event will be callback to the application layer when Sensor Get messages are received.
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, this event will be callback to the application layer when Sensor Set/Set Unack messages are received. 



ESP_BLE_MESH_SENSOR_SERVER_RECV_GET_MSG_EVT

enumerator ESP_BLE_MESH_SENSOR_SERVER_RECV_GET_MSG_EVT
When get_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Sensor Get messages are received. 

ESP_BLE_MESH_SENSOR_SERVER_RECV_SET_MSG_EVT

enumerator ESP_BLE_MESH_SENSOR_SERVER_RECV_SET_MSG_EVT
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Sensor Set/Set Unack messages are received. 

ESP_BLE_MESH_SENSOR_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_SENSOR_SERVER_EVT_MAX


```



### Time and Scenes Client/Server Models




### Header File


* components/bt/esp_ble_mesh/api/models/include/esp_ble_mesh_time_scene_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_time_scene_model_api.h"
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


#### esp_ble_mesh_register_time_scene_client_callback


```


esp_err_t esp_ble_mesh_register_time_scene_client_callback(esp_ble_mesh_time_scene_client_cb_t callback)
Register BLE Mesh Time Scene Client Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_time_scene_client_get_state


```


esp_err_t esp_ble_mesh_time_scene_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_time_scene_client_get_state_t *get_state)
Get the value of Time Scene Server Model states using the Time Scene Client Model get messages. 

Note
If you want to know the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_time_scene_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
get_state -- [in] Pointer to time scene get message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_time_scene_client_set_state


```


esp_err_t esp_ble_mesh_time_scene_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_time_scene_client_set_state_t *set_state)
Set the value of Time Scene Server Model states using the Time Scene Client Model set messages. 

Note
If you want to know the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_time_scene_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
set_state -- [in] Pointer to time scene set message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_time_scene_server_callback


```


esp_err_t esp_ble_mesh_register_time_scene_server_callback(esp_ble_mesh_time_scene_server_cb_t callback)
Register BLE Mesh Time and Scenes Server Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_time_scene_client_get_state_t


```


union esp_ble_mesh_time_scene_client_get_state_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Client Model get message union. 

Public Members
scheduler_act_get

esp_ble_mesh_scheduler_act_get_t scheduler_act_get
For ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_GET 



```

#### esp_ble_mesh_time_scene_client_set_state_t


```


union esp_ble_mesh_time_scene_client_set_state_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Client Model set message union. 

Public Members
time_set

esp_ble_mesh_time_set_t time_set
For ESP_BLE_MESH_MODEL_OP_TIME_SET 

time_zone_set

esp_ble_mesh_time_zone_set_t time_zone_set
For ESP_BLE_MESH_MODEL_OP_TIME_ZONE_SET 

tai_utc_delta_set

esp_ble_mesh_tai_utc_delta_set_t tai_utc_delta_set
For ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_SET 

time_role_set

esp_ble_mesh_time_role_set_t time_role_set
For ESP_BLE_MESH_MODEL_OP_TIME_ROLE_SET 

scene_store

esp_ble_mesh_scene_store_t scene_store
For ESP_BLE_MESH_MODEL_OP_SCENE_STORE & ESP_BLE_MESH_MODEL_OP_SCENE_STORE_UNACK 

scene_recall

esp_ble_mesh_scene_recall_t scene_recall
For ESP_BLE_MESH_MODEL_OP_SCENE_RECALL & ESP_BLE_MESH_MODEL_OP_SCENE_RECALL_UNACK 

scene_delete

esp_ble_mesh_scene_delete_t scene_delete
For ESP_BLE_MESH_MODEL_OP_SCENE_DELETE & ESP_BLE_MESH_MODEL_OP_SCENE_DELETE_UNACK 

scheduler_act_set

esp_ble_mesh_scheduler_act_set_t scheduler_act_set
For ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET & ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET_UNACK 



```

#### esp_ble_mesh_time_scene_client_status_cb_t


```


union esp_ble_mesh_time_scene_client_status_cb_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Client Model received message union. 

Public Members
time_status

esp_ble_mesh_time_status_cb_t time_status
For ESP_BLE_MESH_MODEL_OP_TIME_STATUS 

time_zone_status

esp_ble_mesh_time_zone_status_cb_t time_zone_status
For ESP_BLE_MESH_MODEL_OP_TIME_ZONE_STATUS 

tai_utc_delta_status

esp_ble_mesh_tai_utc_delta_status_cb_t tai_utc_delta_status
For ESP_BLE_MESH_MODEL_OP_TAI_UTC_DELTA_STATUS 

time_role_status

esp_ble_mesh_time_role_status_cb_t time_role_status
For ESP_BLE_MESH_MODEL_OP_TIME_ROLE_STATUS 

scene_status

esp_ble_mesh_scene_status_cb_t scene_status
For ESP_BLE_MESH_MODEL_OP_SCENE_STATUS 

scene_register_status

esp_ble_mesh_scene_register_status_cb_t scene_register_status
For ESP_BLE_MESH_MODEL_OP_SCENE_REGISTER_STATUS 

scheduler_status

esp_ble_mesh_scheduler_status_cb_t scheduler_status
For ESP_BLE_MESH_MODEL_OP_SCHEDULER_STATUS 

scheduler_act_status

esp_ble_mesh_scheduler_act_status_cb_t scheduler_act_status
For ESP_BLE_MESH_MODEL_OP_SCHEDULER_ACT_STATUS 



```

#### esp_ble_mesh_time_scene_server_state_change_t


```


union esp_ble_mesh_time_scene_server_state_change_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Server Model state change value union. 

Public Members
time_set

esp_ble_mesh_state_change_time_set_t time_set
The recv_op in ctx can be used to decide which state is changed. Time Set 

time_status

esp_ble_mesh_state_change_time_status_t time_status
Time Status 

time_zone_set

esp_ble_mesh_state_change_time_zone_set_t time_zone_set
Time Zone Set 

tai_utc_delta_set

esp_ble_mesh_state_change_tai_utc_delta_set_t tai_utc_delta_set
TAI UTC Delta Set 

time_role_set

esp_ble_mesh_state_change_time_role_set_t time_role_set
Time Role Set 

scene_store

esp_ble_mesh_state_change_scene_store_t scene_store
Scene Store 

scene_recall

esp_ble_mesh_state_change_scene_recall_t scene_recall
Scene Recall 

scene_delete

esp_ble_mesh_state_change_scene_delete_t scene_delete
Scene Delete 

scheduler_act_set

esp_ble_mesh_state_change_scheduler_act_set_t scheduler_act_set
Scheduler Action Set 



```

#### esp_ble_mesh_time_scene_server_recv_get_msg_t


```


union esp_ble_mesh_time_scene_server_recv_get_msg_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Server Model received get message union. 

Public Members
scheduler_act

esp_ble_mesh_server_recv_scheduler_act_get_t scheduler_act
Scheduler Action Get 



```

#### esp_ble_mesh_time_scene_server_recv_set_msg_t


```


union esp_ble_mesh_time_scene_server_recv_set_msg_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Server Model received set message union. 

Public Members
time

esp_ble_mesh_server_recv_time_set_t time
Time Set 

time_zone

esp_ble_mesh_server_recv_time_zone_set_t time_zone
Time Zone Set 

tai_utc_delta

esp_ble_mesh_server_recv_tai_utc_delta_set_t tai_utc_delta
TAI-UTC Delta Set 

time_role

esp_ble_mesh_server_recv_time_role_set_t time_role
Time Role Set 

scene_store

esp_ble_mesh_server_recv_scene_store_t scene_store
Scene Store/Scene Store Unack 

scene_recall

esp_ble_mesh_server_recv_scene_recall_t scene_recall
Scene Recall/Scene Recall Unack 

scene_delete

esp_ble_mesh_server_recv_scene_delete_t scene_delete
Scene Delete/Scene Delete Unack 

scheduler_act

esp_ble_mesh_server_recv_scheduler_act_set_t scheduler_act
Scheduler Action Set/Scheduler Action Set Unack 



```

#### esp_ble_mesh_time_scene_server_recv_status_msg_t


```


union esp_ble_mesh_time_scene_server_recv_status_msg_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Server Model received status message union. 

Public Members
time_status

esp_ble_mesh_server_recv_time_status_t time_status
Time Status 



```

#### esp_ble_mesh_time_scene_server_cb_value_t


```


union esp_ble_mesh_time_scene_server_cb_value_t

#include <esp_ble_mesh_time_scene_model_api.h>
Time Scene Server Model callback value union. 

Public Members
state_change

esp_ble_mesh_time_scene_server_state_change_t state_change
ESP_BLE_MESH_TIME_SCENE_SERVER_STATE_CHANGE_EVT 

get

esp_ble_mesh_time_scene_server_recv_get_msg_t get
ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_GET_MSG_EVT 

set

esp_ble_mesh_time_scene_server_recv_set_msg_t set
ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_SET_MSG_EVT 

status

esp_ble_mesh_time_scene_server_recv_status_msg_t status
ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_STATUS_MSG_EVT 



```



### Structures


#### esp_ble_mesh_time_set_t


```


struct esp_ble_mesh_time_set_t
Bluetooth Mesh Time Scene Client Model Get and Set parameters structure. 
Parameters of Time Set 

Public Members
tai_seconds

uint8_t tai_seconds[5]
The current TAI time in seconds 

sub_second

uint8_t sub_second
The sub-second time in units of 1/256 second 

uncertainty

uint8_t uncertainty
The estimated uncertainty in 10-millisecond steps 

time_authority

uint16_t time_authority
0 = No Time Authority, 1 = Time Authority 

tai_utc_delta

uint16_t tai_utc_delta
Current difference between TAI and UTC in seconds 

time_zone_offset

uint8_t time_zone_offset
The local time zone offset in 15-minute increments 



```

#### esp_ble_mesh_time_zone_set_t


```


struct esp_ble_mesh_time_zone_set_t
Parameters of Time Zone Set 

Public Members
time_zone_offset_new

uint8_t time_zone_offset_new
Upcoming local time zone offset 

tai_zone_change

uint8_t tai_zone_change[5]
TAI Seconds time of the upcoming Time Zone Offset change 



```

#### esp_ble_mesh_tai_utc_delta_set_t


```


struct esp_ble_mesh_tai_utc_delta_set_t
Parameters of TAI-UTC Delta Set 

Public Members
tai_utc_delta_new

uint16_t tai_utc_delta_new
Upcoming difference between TAI and UTC in seconds 

padding

uint16_t padding
Always 0b0. Other values are Prohibited. 

tai_delta_change

uint8_t tai_delta_change[5]
TAI Seconds time of the upcoming TAI-UTC Delta change 



```

#### esp_ble_mesh_time_role_set_t


```


struct esp_ble_mesh_time_role_set_t
Parameter of Time Role Set 

Public Members
time_role

uint8_t time_role
The Time Role for the element 



```

#### esp_ble_mesh_scene_store_t


```


struct esp_ble_mesh_scene_store_t
Parameter of Scene Store 

Public Members
scene_number

uint16_t scene_number
The number of scenes to be stored 



```

#### esp_ble_mesh_scene_recall_t


```


struct esp_ble_mesh_scene_recall_t
Parameters of Scene Recall 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

scene_number

uint16_t scene_number
The number of scenes to be recalled 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_scene_delete_t


```


struct esp_ble_mesh_scene_delete_t
Parameter of Scene Delete 

Public Members
scene_number

uint16_t scene_number
The number of scenes to be deleted 



```

#### esp_ble_mesh_scheduler_act_get_t


```


struct esp_ble_mesh_scheduler_act_get_t
Parameter of Scheduler Action Get 

Public Members
index

uint8_t index
Index of the Schedule Register entry to get 



```

#### esp_ble_mesh_scheduler_act_set_t


```


struct esp_ble_mesh_scheduler_act_set_t
Parameters of Scheduler Action Set 

Public Members
index

uint64_t index
Index of the Schedule Register entry to set 

year

uint64_t year
Scheduled year for the action 

month

uint64_t month
Scheduled month for the action 

day

uint64_t day
Scheduled day of the month for the action 

hour

uint64_t hour
Scheduled hour for the action 

minute

uint64_t minute
Scheduled minute for the action 

second

uint64_t second
Scheduled second for the action 

day_of_week

uint64_t day_of_week
Schedule days of the week for the action 

action

uint64_t action
Action to be performed at the scheduled time 

trans_time

uint64_t trans_time
Transition time for this action 

scene_number

uint16_t scene_number
Transition time for this action 



```

#### esp_ble_mesh_time_status_cb_t


```


struct esp_ble_mesh_time_status_cb_t
Bluetooth Mesh Time Scene Client Model Get and Set callback parameters structure. 
Parameters of Time Status 

Public Members
tai_seconds

uint8_t tai_seconds[5]
The current TAI time in seconds 

sub_second

uint8_t sub_second
The sub-second time in units of 1/256 second 

uncertainty

uint8_t uncertainty
The estimated uncertainty in 10-millisecond steps 

time_authority

uint16_t time_authority
0 = No Time Authority, 1 = Time Authority 

tai_utc_delta

uint16_t tai_utc_delta
Current difference between TAI and UTC in seconds 

time_zone_offset

uint8_t time_zone_offset
The local time zone offset in 15-minute increments 



```

#### esp_ble_mesh_time_zone_status_cb_t


```


struct esp_ble_mesh_time_zone_status_cb_t
Parameters of Time Zone Status 

Public Members
time_zone_offset_curr

uint8_t time_zone_offset_curr
Current local time zone offset 

time_zone_offset_new

uint8_t time_zone_offset_new
Upcoming local time zone offset 

tai_zone_change

uint8_t tai_zone_change[5]
TAI Seconds time of the upcoming Time Zone Offset change 



```

#### esp_ble_mesh_tai_utc_delta_status_cb_t


```


struct esp_ble_mesh_tai_utc_delta_status_cb_t
Parameters of TAI-UTC Delta Status 

Public Members
tai_utc_delta_curr

uint16_t tai_utc_delta_curr
Current difference between TAI and UTC in seconds 

padding_1

uint16_t padding_1
Always 0b0. Other values are Prohibited. 

tai_utc_delta_new

uint16_t tai_utc_delta_new
Upcoming difference between TAI and UTC in seconds 

padding_2

uint16_t padding_2
Always 0b0. Other values are Prohibited. 

tai_delta_change

uint8_t tai_delta_change[5]
TAI Seconds time of the upcoming TAI-UTC Delta change 



```

#### esp_ble_mesh_time_role_status_cb_t


```


struct esp_ble_mesh_time_role_status_cb_t
Parameter of Time Role Status 

Public Members
time_role

uint8_t time_role
The Time Role for the element 



```

#### esp_ble_mesh_scene_status_cb_t


```


struct esp_ble_mesh_scene_status_cb_t
Parameters of Scene Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

status_code

uint8_t status_code
Status code of the last operation 

current_scene

uint16_t current_scene
Scene Number of the current scene 

target_scene

uint16_t target_scene
Scene Number of the target scene (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_scene_register_status_cb_t


```


struct esp_ble_mesh_scene_register_status_cb_t
Parameters of Scene Register Status 

Public Members
status_code

uint8_t status_code
Status code for the previous operation 

current_scene

uint16_t current_scene
Scene Number of the current scene 

scenes

struct net_buf_simple *scenes
A list of scenes stored within an element 



```

#### esp_ble_mesh_scheduler_status_cb_t


```


struct esp_ble_mesh_scheduler_status_cb_t
Parameter of Scheduler Status 

Public Members
schedules

uint16_t schedules
Bit field indicating defined Actions in the Schedule Register 



```

#### esp_ble_mesh_scheduler_act_status_cb_t


```


struct esp_ble_mesh_scheduler_act_status_cb_t
Parameters of Scheduler Action Status 

Public Members
index

uint64_t index
Enumerates (selects) a Schedule Register entry 

year

uint64_t year
Scheduled year for the action 

month

uint64_t month
Scheduled month for the action 

day

uint64_t day
Scheduled day of the month for the action 

hour

uint64_t hour
Scheduled hour for the action 

minute

uint64_t minute
Scheduled minute for the action 

second

uint64_t second
Scheduled second for the action 

day_of_week

uint64_t day_of_week
Schedule days of the week for the action 

action

uint64_t action
Action to be performed at the scheduled time 

trans_time

uint64_t trans_time
Transition time for this action 

scene_number

uint16_t scene_number
Transition time for this action 



```

#### esp_ble_mesh_time_scene_client_cb_param_t


```


struct esp_ble_mesh_time_scene_client_cb_param_t
Time Scene Client Model callback parameters 

Public Members
error_code

int error_code
Appropriate error code 

params

esp_ble_mesh_client_common_param_t *params
The client common parameters. 

status_cb

esp_ble_mesh_time_scene_client_status_cb_t status_cb
The scene status message callback values 



```

#### esp_ble_mesh_time_state_t


```


struct esp_ble_mesh_time_state_t
Parameters of Time state 

Public Members
tai_seconds

uint8_t tai_seconds[5]
The value of the TAI Seconds state 

subsecond

uint8_t subsecond
The value of the Subsecond field 

uncertainty

uint8_t uncertainty
The value of the Uncertainty field 

time_zone_offset_curr

uint8_t time_zone_offset_curr
The value of the Time Zone Offset Current field 

time_zone_offset_new

uint8_t time_zone_offset_new
The value of the Time Zone Offset New state 

tai_zone_change

uint8_t tai_zone_change[5]
The value of the TAI of Zone Change field 

time_authority

uint16_t time_authority
The value of the Time Authority bit 

tai_utc_delta_curr

uint16_t tai_utc_delta_curr
The value of the TAI-UTC Delta Current state 

tai_utc_delta_new

uint16_t tai_utc_delta_new
The value of the TAI-UTC Delta New state 

tai_delta_change

uint8_t tai_delta_change[5]
The value of the TAI of Delta Change field 

time

struct esp_ble_mesh_time_state_t::[anonymous] time
Parameters of the Time state 

time_role

uint8_t time_role
The value of the Time Role state 



```

#### esp_ble_mesh_time_srv_t


```


struct esp_ble_mesh_time_srv_t
User data of Time Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Time Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_time_state_t *state
Parameters of the Time state 



```

#### esp_ble_mesh_time_setup_srv_t


```


struct esp_ble_mesh_time_setup_srv_t
User data of Time Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Time Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_time_state_t *state
Parameters of the Time state 



```

#### esp_ble_mesh_scene_register_t


```


struct esp_ble_mesh_scene_register_t

Scene Store is an operation of storing values of a present state of an element.
The structure and meaning of the stored state is determined by a model. States to be stored are specified by each model.
The Scene Store operation shall persistently store all values of all states marked as Stored with Scene for all models present on all elements of a node.
If a model is extending another model, the extending model shall determine the Stored with Scene behavior of that model. Parameters of Scene Register state 



Public Members
scene_number

uint16_t scene_number
The value of the Scene Number 

scene_type

uint8_t scene_type
The value of the Scene Type 

scene_value

struct net_buf_simple *scene_value
Scene value may use a union to represent later, the union contains structures of all the model states which can be stored in a scene. The value of the Scene Value 



```

#### esp_ble_mesh_scenes_state_t


```


struct esp_ble_mesh_scenes_state_t
Parameters of Scenes state.
Scenes serve as memory banks for storage of states (e.g., a power level or a light level/color). Values of states of an element can be stored as a scene and can be recalled later from the scene memory.
A scene is represented by a Scene Number, which is a 16-bit non-zero, mesh-wide value. (There can be a maximum of 65535 scenes in a mesh network.) The meaning of a scene, as well as the state storage container associated with it, are determined by a model.
The Scenes state change may start numerous parallel model transitions. In that case, each individual model handles the transition internally.
The scene transition is defined as a group of individual model transitions started by a Scene Recall operation. The scene transition is in progress when at least one transition from the group of individual model transitions is in progress. 

Public Members
scene_count

const uint16_t scene_count
The Scenes state's scene count 

scenes

esp_ble_mesh_scene_register_t *scenes
Parameters of the Scenes state 

current_scene

uint16_t current_scene
The Current Scene state is a 16-bit value that contains either the Scene Number of the currently active scene or a value of 0x0000 when no scene is active.
When a Scene Store operation or a Scene Recall operation completes with success, the Current Scene state value shall be to the Scene Number used during that operation.
When the Current Scene Number is deleted from a Scene Register state as a result of Scene Delete operation, the Current Scene state shall be set to 0x0000.
When any of the element's state that is marked as “Stored with Scene” has changed not as a result of a Scene Recall operation, the value of the Current Scene state shall be set to 0x0000.
When a scene transition is in progress, the value of the Current Scene state shall be set to 0x0000. The value of the Current Scene state 

target_scene

uint16_t target_scene
The Target Scene state is a 16-bit value that contains the target Scene Number when a scene transition is in progress.
When the scene transition is in progress and the target Scene Number is deleted from a Scene Register state as a result of Scene Delete operation, the Target Scene state shall be set to 0x0000.
When the scene transition is in progress and a new Scene Number is stored in the Scene Register as a result of Scene Store operation, the Target Scene state shall be set to the new Scene Number.
When the scene transition is not in progress, the value of the Target Scene state shall be set to 0x0000. The value of the Target Scene state 

status_code

uint8_t status_code
The status code of the last scene operation 

in_progress

bool in_progress
Indicate if the scene transition is in progress 



```

#### esp_ble_mesh_scene_srv_t


```


struct esp_ble_mesh_scene_srv_t
User data of Scene Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Scene Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_scenes_state_t *state
Parameters of the Scenes state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 



```

#### esp_ble_mesh_scene_setup_srv_t


```


struct esp_ble_mesh_scene_setup_srv_t
User data of Scene Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Scene Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_scenes_state_t *state
Parameters of the Scenes state 



```

#### esp_ble_mesh_schedule_register_t


```


struct esp_ble_mesh_schedule_register_t
Parameters of Scheduler Register state 

Public Members
in_use

bool in_use
Indicate if the registered schedule is in use 

year

uint64_t year
The value of Scheduled year for the action 

month

uint64_t month
The value of Scheduled month for the action 

day

uint64_t day
The value of Scheduled day of the month for the action 

hour

uint64_t hour
The value of Scheduled hour for the action 

minute

uint64_t minute
The value of Scheduled minute for the action 

second

uint64_t second
The value of Scheduled second for the action 

day_of_week

uint64_t day_of_week
The value of Schedule days of the week for the action 

action

uint64_t action
The value of Action to be performed at the scheduled time 

trans_time

uint64_t trans_time
The value of Transition time for this action 

scene_number

uint16_t scene_number
The value of Scene Number to be used for some actions 



```

#### esp_ble_mesh_scheduler_state_t


```


struct esp_ble_mesh_scheduler_state_t
Parameters of Scheduler state 

Public Members
schedule_count

const uint8_t schedule_count
Scheduler count 

schedules

esp_ble_mesh_schedule_register_t *schedules
Up to 16 scheduled entries 



```

#### esp_ble_mesh_scheduler_srv_t


```


struct esp_ble_mesh_scheduler_srv_t
User data of Scheduler Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Scheduler Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_scheduler_state_t *state
Parameters of the Scheduler state 



```

#### esp_ble_mesh_scheduler_setup_srv_t


```


struct esp_ble_mesh_scheduler_setup_srv_t
User data of Scheduler Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Scheduler Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_scheduler_state_t *state
Parameters of the Scheduler state 



```

#### esp_ble_mesh_state_change_time_set_t


```


struct esp_ble_mesh_state_change_time_set_t
Parameters of Time Set state change event 

Public Members
tai_seconds

uint8_t tai_seconds[5]
The current TAI time in seconds 

subsecond

uint8_t subsecond
The sub-second time in units of 1/256 second 

uncertainty

uint8_t uncertainty
The estimated uncertainty in 10-millisecond steps 

time_authority

uint16_t time_authority
0 = No Time Authority, 1 = Time Authority 

tai_utc_delta_curr

uint16_t tai_utc_delta_curr
Current difference between TAI and UTC in seconds 

time_zone_offset_curr

uint8_t time_zone_offset_curr
The local time zone offset in 15-minute increments 



```

#### esp_ble_mesh_state_change_time_status_t


```


struct esp_ble_mesh_state_change_time_status_t
Parameters of Time Status state change event 

Public Members
tai_seconds

uint8_t tai_seconds[5]
The current TAI time in seconds 

subsecond

uint8_t subsecond
The sub-second time in units of 1/256 second 

uncertainty

uint8_t uncertainty
The estimated uncertainty in 10-millisecond steps 

time_authority

uint16_t time_authority
0 = No Time Authority, 1 = Time Authority 

tai_utc_delta_curr

uint16_t tai_utc_delta_curr
Current difference between TAI and UTC in seconds 

time_zone_offset_curr

uint8_t time_zone_offset_curr
The local time zone offset in 15-minute increments 



```

#### esp_ble_mesh_state_change_time_zone_set_t


```


struct esp_ble_mesh_state_change_time_zone_set_t
Parameters of Time Zone Set state change event 

Public Members
time_zone_offset_new

uint8_t time_zone_offset_new
Upcoming local time zone offset 

tai_zone_change

uint8_t tai_zone_change[5]
TAI Seconds time of the upcoming Time Zone Offset change 



```

#### esp_ble_mesh_state_change_tai_utc_delta_set_t


```


struct esp_ble_mesh_state_change_tai_utc_delta_set_t
Parameters of TAI UTC Delta Set state change event 

Public Members
tai_utc_delta_new

uint16_t tai_utc_delta_new
Upcoming difference between TAI and UTC in seconds 

tai_delta_change

uint8_t tai_delta_change[5]
TAI Seconds time of the upcoming TAI-UTC Delta change 



```

#### esp_ble_mesh_state_change_time_role_set_t


```


struct esp_ble_mesh_state_change_time_role_set_t
Parameter of Time Role Set state change event 

Public Members
time_role

uint8_t time_role
The Time Role for the element 



```

#### esp_ble_mesh_state_change_scene_store_t


```


struct esp_ble_mesh_state_change_scene_store_t
Parameter of Scene Store state change event 

Public Members
scene_number

uint16_t scene_number
The number of scenes to be stored 



```

#### esp_ble_mesh_state_change_scene_recall_t


```


struct esp_ble_mesh_state_change_scene_recall_t
Parameter of Scene Recall state change event 

Public Members
scene_number

uint16_t scene_number
The number of scenes to be recalled 



```

#### esp_ble_mesh_state_change_scene_delete_t


```


struct esp_ble_mesh_state_change_scene_delete_t
Parameter of Scene Delete state change event 

Public Members
scene_number

uint16_t scene_number
The number of scenes to be deleted 



```

#### esp_ble_mesh_state_change_scheduler_act_set_t


```


struct esp_ble_mesh_state_change_scheduler_act_set_t
Parameter of Scheduler Action Set state change event 

Public Members
index

uint64_t index
Index of the Schedule Register entry to set 

year

uint64_t year
Scheduled year for the action 

month

uint64_t month
Scheduled month for the action 

day

uint64_t day
Scheduled day of the month for the action 

hour

uint64_t hour
Scheduled hour for the action 

minute

uint64_t minute
Scheduled minute for the action 

second

uint64_t second
Scheduled second for the action 

day_of_week

uint64_t day_of_week
Schedule days of the week for the action 

action

uint64_t action
Action to be performed at the scheduled time 

trans_time

uint64_t trans_time
Transition time for this action 

scene_number

uint16_t scene_number
Scene number to be used for some actions 



```

#### esp_ble_mesh_server_recv_scheduler_act_get_t


```


struct esp_ble_mesh_server_recv_scheduler_act_get_t
Context of the received Scheduler Action Get message 

Public Members
index

uint8_t index
Index of the Schedule Register entry to get 



```

#### esp_ble_mesh_server_recv_time_set_t


```


struct esp_ble_mesh_server_recv_time_set_t
Context of the received Time Set message 

Public Members
tai_seconds

uint8_t tai_seconds[5]
The current TAI time in seconds 

subsecond

uint8_t subsecond
The sub-second time in units of 1/256 second 

uncertainty

uint8_t uncertainty
The estimated uncertainty in 10-millisecond steps 

time_authority

uint16_t time_authority
0 = No Time Authority, 1 = Time Authority 

tai_utc_delta

uint16_t tai_utc_delta
Current difference between TAI and UTC in seconds 

time_zone_offset

uint8_t time_zone_offset
The local time zone offset in 15-minute increments 



```

#### esp_ble_mesh_server_recv_time_zone_set_t


```


struct esp_ble_mesh_server_recv_time_zone_set_t
Context of the received Time Zone Set message 

Public Members
time_zone_offset_new

uint8_t time_zone_offset_new
Upcoming local time zone offset 

tai_zone_change

uint8_t tai_zone_change[5]
TAI Seconds time of the upcoming Time Zone Offset change 



```

#### esp_ble_mesh_server_recv_tai_utc_delta_set_t


```


struct esp_ble_mesh_server_recv_tai_utc_delta_set_t
Context of the received TAI UTC Delta Set message 

Public Members
tai_utc_delta_new

uint16_t tai_utc_delta_new
Upcoming difference between TAI and UTC in seconds 

padding

uint16_t padding
Always 0b0. Other values are Prohibited. 

tai_delta_change

uint8_t tai_delta_change[5]
TAI Seconds time of the upcoming TAI-UTC Delta change 



```

#### esp_ble_mesh_server_recv_time_role_set_t


```


struct esp_ble_mesh_server_recv_time_role_set_t
Context of the received Time Role Set message 

Public Members
time_role

uint8_t time_role
The Time Role for the element 



```

#### esp_ble_mesh_server_recv_scene_store_t


```


struct esp_ble_mesh_server_recv_scene_store_t
Context of the received Scene Store message 

Public Members
scene_number

uint16_t scene_number
The number of scenes to be stored 



```

#### esp_ble_mesh_server_recv_scene_recall_t


```


struct esp_ble_mesh_server_recv_scene_recall_t
Context of the received Scene Recall message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

scene_number

uint16_t scene_number
The number of scenes to be recalled 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_scene_delete_t


```


struct esp_ble_mesh_server_recv_scene_delete_t
Context of the received Scene Delete message 

Public Members
scene_number

uint16_t scene_number
The number of scenes to be deleted 



```

#### esp_ble_mesh_server_recv_scheduler_act_set_t


```


struct esp_ble_mesh_server_recv_scheduler_act_set_t
Context of the received Scheduler Action Set message 

Public Members
index

uint64_t index
Index of the Schedule Register entry to set 

year

uint64_t year
Scheduled year for the action 

month

uint64_t month
Scheduled month for the action 

day

uint64_t day
Scheduled day of the month for the action 

hour

uint64_t hour
Scheduled hour for the action 

minute

uint64_t minute
Scheduled minute for the action 

second

uint64_t second
Scheduled second for the action 

day_of_week

uint64_t day_of_week
Schedule days of the week for the action 

action

uint64_t action
Action to be performed at the scheduled time 

trans_time

uint64_t trans_time
Transition time for this action 

scene_number

uint16_t scene_number
Scene number to be used for some actions 



```

#### esp_ble_mesh_server_recv_time_status_t


```


struct esp_ble_mesh_server_recv_time_status_t
Context of the received Time Status message 

Public Members
tai_seconds

uint8_t tai_seconds[5]
The current TAI time in seconds 

subsecond

uint8_t subsecond
The sub-second time in units of 1/256 second 

uncertainty

uint8_t uncertainty
The estimated uncertainty in 10-millisecond steps 

time_authority

uint16_t time_authority
0 = No Time Authority, 1 = Time Authority 

tai_utc_delta

uint16_t tai_utc_delta
Current difference between TAI and UTC in seconds 

time_zone_offset

uint8_t time_zone_offset
The local time zone offset in 15-minute increments 



```

#### esp_ble_mesh_time_scene_server_cb_param_t


```


struct esp_ble_mesh_time_scene_server_cb_param_t
Time Scene Server Model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Time and Scenes Server Models 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received messages 

value

esp_ble_mesh_time_scene_server_cb_value_t value
Value of the received Time and Scenes Messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_TIME_CLI


```


ESP_BLE_MESH_MODEL_TIME_CLI(cli_pub, cli_data)
Define a new Time Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Time Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Time Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_SCENE_CLI


```


ESP_BLE_MESH_MODEL_SCENE_CLI(cli_pub, cli_data)
Define a new Scene Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Scene Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Scene Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_SCHEDULER_CLI


```


ESP_BLE_MESH_MODEL_SCHEDULER_CLI(cli_pub, cli_data)
Define a new Scheduler Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Scheduler Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Scheduler Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_TIME_SRV


```


ESP_BLE_MESH_MODEL_TIME_SRV(srv_pub, srv_data)
Time Scene Server Models related context. 
Define a new Time Server Model.

Note
1. The Time Server model is a root model. When this model is present on an Element, the corresponding Time Setup Server model shall also be present.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_time_srv_t.


Returns
New Time Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_TIME_SETUP_SRV


```


ESP_BLE_MESH_MODEL_TIME_SETUP_SRV(srv_data)
Define a new Time Setup Server Model. 

Note
1. The Time Setup Server model extends the Time Server model. Time is sensitive information that is propagated across a mesh network.
Only an authorized Time Client should be allowed to change the Time and Time Role states. A dedicated application key Bluetooth SIG Proprietary should be used on the Time Setup Server to restrict access to the server to only authorized Time Clients.
This model does not support subscribing nor publishing.




Parameters

srv_data -- Pointer to the unique struct esp_ble_mesh_time_setup_srv_t.


Returns
New Time Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_SCENE_SRV


```


ESP_BLE_MESH_MODEL_SCENE_SRV(srv_pub, srv_data)
Define a new Scene Server Model. 

Note
1. The Scene Server model is a root model. When this model is present on an Element, the corresponding Scene Setup Server model shall also be present.
This model shall support model publication and model subscription.
The model may be present only on the Primary element of a node.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_scene_srv_t.


Returns
New Scene Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_SCENE_SETUP_SRV


```


ESP_BLE_MESH_MODEL_SCENE_SETUP_SRV(srv_pub, srv_data)
Define a new Scene Setup Server Model. 

Note
1. The Scene Setup Server model extends the Scene Server model and the Generic Default Transition Time Server model.
This model shall support model subscription.
The model may be present only on the Primary element of a node.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_scene_setup_srv_t.


Returns
New Scene Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_SCHEDULER_SRV


```


ESP_BLE_MESH_MODEL_SCHEDULER_SRV(srv_pub, srv_data)
Define a new Scheduler Server Model. 

Note
1. The Scheduler Server model extends the Scene Server model. When this model is present on an Element, the corresponding Scheduler Setup Server model shall also be present.
This model shall support model publication and model subscription.
The model may be present only on the Primary element of a node.
The model requires the Time Server model shall be present on the element.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_scheduler_srv_t.


Returns
New Scheduler Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_SCHEDULER_SETUP_SRV


```


ESP_BLE_MESH_MODEL_SCHEDULER_SETUP_SRV(srv_pub, srv_data)
Define a new Scheduler Setup Server Model. 

Note
1. The Scheduler Setup Server model extends the Scheduler Server and the Scene Setup Server models.
This model shall support model subscription.
The model may be present only on the Primary element of a node.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_scheduler_setup_srv_t.


Returns
New Scheduler Setup Server Model instance. 



```

#### ESP_BLE_MESH_UNKNOWN_TAI_SECONDS


```


ESP_BLE_MESH_UNKNOWN_TAI_SECONDS
Unknown TAI Seconds 

```

#### ESP_BLE_MESH_UNKNOWN_TAI_ZONE_CHANGE


```


ESP_BLE_MESH_UNKNOWN_TAI_ZONE_CHANGE
Unknown TAI of Zone Change 

```

#### ESP_BLE_MESH_UNKNOWN_TAI_DELTA_CHANGE


```


ESP_BLE_MESH_UNKNOWN_TAI_DELTA_CHANGE
Unknown TAI of Delta Change 

```

#### ESP_BLE_MESH_TAI_UTC_DELTA_MAX_VALUE


```


ESP_BLE_MESH_TAI_UTC_DELTA_MAX_VALUE
Maximum TAI-UTC Delta value 

```

#### ESP_BLE_MESH_TAI_SECONDS_LEN


```


ESP_BLE_MESH_TAI_SECONDS_LEN
Length of TAI Seconds 

```

#### ESP_BLE_MESH_TAI_OF_ZONE_CHANGE_LEN


```


ESP_BLE_MESH_TAI_OF_ZONE_CHANGE_LEN
Length of TAI of Zone Change 

```

#### ESP_BLE_MESH_TAI_OF_DELTA_CHANGE_LEN


```


ESP_BLE_MESH_TAI_OF_DELTA_CHANGE_LEN
Length of TAI of Delta Change 

```

#### ESP_BLE_MESH_INVALID_SCENE_NUMBER


```


ESP_BLE_MESH_INVALID_SCENE_NUMBER
Invalid Scene Number 

```

#### ESP_BLE_MESH_SCENE_NUMBER_LEN


```


ESP_BLE_MESH_SCENE_NUMBER_LEN
Length of the Scene Number 

```

#### ESP_BLE_MESH_SCHEDULE_YEAR_ANY_YEAR


```


ESP_BLE_MESH_SCHEDULE_YEAR_ANY_YEAR
Any year of the Scheduled year 

```

#### ESP_BLE_MESH_SCHEDULE_DAY_ANY_DAY


```


ESP_BLE_MESH_SCHEDULE_DAY_ANY_DAY
Any day of the Scheduled day 

```

#### ESP_BLE_MESH_SCHEDULE_HOUR_ANY_HOUR


```


ESP_BLE_MESH_SCHEDULE_HOUR_ANY_HOUR
Any hour of the Scheduled hour 

```

#### ESP_BLE_MESH_SCHEDULE_HOUR_ONCE_A_DAY


```


ESP_BLE_MESH_SCHEDULE_HOUR_ONCE_A_DAY
Any hour of the Scheduled Day 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_ANY_OF_HOUR


```


ESP_BLE_MESH_SCHEDULE_SEC_ANY_OF_HOUR
Any minute of the Scheduled hour 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_EVERY_15_MIN


```


ESP_BLE_MESH_SCHEDULE_SEC_EVERY_15_MIN
Every 15 minutes of the Scheduled hour 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_EVERY_20_MIN


```


ESP_BLE_MESH_SCHEDULE_SEC_EVERY_20_MIN
Every 20 minutes of the Scheduled hour 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_ONCE_AN_HOUR


```


ESP_BLE_MESH_SCHEDULE_SEC_ONCE_AN_HOUR
Once of the Scheduled hour 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_ANY_OF_MIN


```


ESP_BLE_MESH_SCHEDULE_SEC_ANY_OF_MIN
Any second of the Scheduled minute 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_EVERY_15_SEC


```


ESP_BLE_MESH_SCHEDULE_SEC_EVERY_15_SEC
Every 15 seconds of the Scheduled minute 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_EVERY_20_SEC


```


ESP_BLE_MESH_SCHEDULE_SEC_EVERY_20_SEC
Every 20 seconds of the Scheduled minute 

```

#### ESP_BLE_MESH_SCHEDULE_SEC_ONCE_AN_MIN


```


ESP_BLE_MESH_SCHEDULE_SEC_ONCE_AN_MIN
Once of the Scheduled minute 

```

#### ESP_BLE_MESH_SCHEDULE_ACT_TURN_OFF


```


ESP_BLE_MESH_SCHEDULE_ACT_TURN_OFF
Scheduled Action - Turn Off 

```

#### ESP_BLE_MESH_SCHEDULE_ACT_TURN_ON


```


ESP_BLE_MESH_SCHEDULE_ACT_TURN_ON
Scheduled Action - Turn On 

```

#### ESP_BLE_MESH_SCHEDULE_ACT_SCENE_RECALL


```


ESP_BLE_MESH_SCHEDULE_ACT_SCENE_RECALL
Scheduled Action - Scene Recall 

```

#### ESP_BLE_MESH_SCHEDULE_ACT_NO_ACTION


```


ESP_BLE_MESH_SCHEDULE_ACT_NO_ACTION
Scheduled Action - No Action 

```

#### ESP_BLE_MESH_SCHEDULE_SCENE_NO_SCENE


```


ESP_BLE_MESH_SCHEDULE_SCENE_NO_SCENE
Scheduled Scene - No Scene 

```

#### ESP_BLE_MESH_SCHEDULE_ENTRY_MAX_INDEX


```


ESP_BLE_MESH_SCHEDULE_ENTRY_MAX_INDEX
Maximum number of Scheduled entries 

```

#### ESP_BLE_MESH_TIME_NONE


```


ESP_BLE_MESH_TIME_NONE
Time Role - None 

```

#### ESP_BLE_MESH_TIME_AUTHORITY


```


ESP_BLE_MESH_TIME_AUTHORITY
Time Role - Mesh Time Authority 

```

#### ESP_BLE_MESH_TIME_RELAY


```


ESP_BLE_MESH_TIME_RELAY
Time Role - Mesh Time Relay 

```

#### ESP_BLE_MESH_TIME_CLIENT


```


ESP_BLE_MESH_TIME_CLIENT
Time Role - Mesh Time Client 

```

#### ESP_BLE_MESH_SCENE_SUCCESS


```


ESP_BLE_MESH_SCENE_SUCCESS
Scene operation - Success 

```

#### ESP_BLE_MESH_SCENE_REG_FULL


```


ESP_BLE_MESH_SCENE_REG_FULL
Scene operation - Scene Register Full 

```

#### ESP_BLE_MESH_SCENE_NOT_FOUND


```


ESP_BLE_MESH_SCENE_NOT_FOUND
Scene operation - Scene Not Found 

```



### Type Definitions


#### esp_ble_mesh_time_scene_client_cb_t


```


typedef void (*esp_ble_mesh_time_scene_client_cb_t)(esp_ble_mesh_time_scene_client_cb_event_t event, esp_ble_mesh_time_scene_client_cb_param_t *param)
Bluetooth Mesh Time Scene Client Model function. 
Time Scene Client Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_time_scene_server_cb_t


```


typedef void (*esp_ble_mesh_time_scene_server_cb_t)(esp_ble_mesh_time_scene_server_cb_event_t event, esp_ble_mesh_time_scene_server_cb_param_t *param)
Bluetooth Mesh Time and Scenes Server Model function. 
Time Scene Server Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_time_scene_client_cb_event_t


```


enum esp_ble_mesh_time_scene_client_cb_event_t
This enum value is the event of Time Scene Client Model 
Values:
ESP_BLE_MESH_TIME_SCENE_CLIENT_GET_STATE_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_CLIENT_GET_STATE_EVT

ESP_BLE_MESH_TIME_SCENE_CLIENT_SET_STATE_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_CLIENT_SET_STATE_EVT

ESP_BLE_MESH_TIME_SCENE_CLIENT_PUBLISH_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_CLIENT_PUBLISH_EVT

ESP_BLE_MESH_TIME_SCENE_CLIENT_TIMEOUT_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_CLIENT_TIMEOUT_EVT

ESP_BLE_MESH_TIME_SCENE_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_TIME_SCENE_CLIENT_EVT_MAX


```

#### esp_ble_mesh_time_scene_server_cb_event_t


```


enum esp_ble_mesh_time_scene_server_cb_event_t
This enum value is the event of Time Scene Server Model 
Values:
ESP_BLE_MESH_TIME_SCENE_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_SERVER_STATE_CHANGE_EVT

When get_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, no event will be callback to the application layer when Time Scene Get messages are received.
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, this event will be callback to the application layer when Time Scene Set/Set Unack messages are received. 



ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_GET_MSG_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_GET_MSG_EVT
When get_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Time Scene Get messages are received. 

ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_SET_MSG_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_SET_MSG_EVT
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Time Scene Set/Set Unack messages are received. 

ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_STATUS_MSG_EVT

enumerator ESP_BLE_MESH_TIME_SCENE_SERVER_RECV_STATUS_MSG_EVT
When status_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when TIme Status message is received. 

ESP_BLE_MESH_TIME_SCENE_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_TIME_SCENE_SERVER_EVT_MAX


```



### Lighting Client/Server Models




### Header File


* components/bt/esp_ble_mesh/api/models/include/esp_ble_mesh_lighting_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_lighting_model_api.h"
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


#### esp_ble_mesh_register_light_client_callback


```


esp_err_t esp_ble_mesh_register_light_client_callback(esp_ble_mesh_light_client_cb_t callback)
Register BLE Mesh Light Client Model callback. 

Parameters
callback -- [in] pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_light_client_get_state


```


esp_err_t esp_ble_mesh_light_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_light_client_get_state_t *get_state)
Get the value of Light Server Model states using the Light Client Model get messages. 

Note
If you want to know the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_light_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
get_state -- [in] Pointer of light get message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_light_client_set_state


```


esp_err_t esp_ble_mesh_light_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_light_client_set_state_t *set_state)
Set the value of Light Server Model states using the Light Client Model set messages. 

Note
If you want to know the opcodes and corresponding meanings accepted by this API, please refer to esp_ble_mesh_light_message_opcode_t in esp_ble_mesh_defs.h


Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
set_state -- [in] Pointer of light set message value. Shall not be set to NULL.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_lighting_server_callback


```


esp_err_t esp_ble_mesh_register_lighting_server_callback(esp_ble_mesh_lighting_server_cb_t callback)
Register BLE Mesh Lighting Server Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_light_client_get_state_t


```


union esp_ble_mesh_light_client_get_state_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Client Model get message union. 

Public Members
lc_property_get

esp_ble_mesh_light_lc_property_get_t lc_property_get
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_GET 



```

#### esp_ble_mesh_light_client_set_state_t


```


union esp_ble_mesh_light_client_set_state_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Client Model set message union. 

Public Members
lightness_set

esp_ble_mesh_light_lightness_set_t lightness_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET_UNACK 

lightness_linear_set

esp_ble_mesh_light_lightness_linear_set_t lightness_linear_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET_UNACK 

lightness_default_set

esp_ble_mesh_light_lightness_default_set_t lightness_default_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK 

lightness_range_set

esp_ble_mesh_light_lightness_range_set_t lightness_range_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET_UNACK 

ctl_set

esp_ble_mesh_light_ctl_set_t ctl_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_SET_UNACK 

ctl_temperature_set

esp_ble_mesh_light_ctl_temperature_set_t ctl_temperature_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET_UNACK 

ctl_temperature_range_set

esp_ble_mesh_light_ctl_temperature_range_set_t ctl_temperature_range_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK 

ctl_default_set

esp_ble_mesh_light_ctl_default_set_t ctl_default_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET_UNACK 

hsl_set

esp_ble_mesh_light_hsl_set_t hsl_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SET_UNACK 

hsl_hue_set

esp_ble_mesh_light_hsl_hue_set_t hsl_hue_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET_UNACK 

hsl_saturation_set

esp_ble_mesh_light_hsl_saturation_set_t hsl_saturation_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET_UNACK 

hsl_default_set

esp_ble_mesh_light_hsl_default_set_t hsl_default_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET_UNACK 

hsl_range_set

esp_ble_mesh_light_hsl_range_set_t hsl_range_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET_UNACK 

xyl_set

esp_ble_mesh_light_xyl_set_t xyl_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_SET_UNACK 

xyl_default_set

esp_ble_mesh_light_xyl_default_set_t xyl_default_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET_UNACK 

xyl_range_set

esp_ble_mesh_light_xyl_range_set_t xyl_range_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET_UNACK 

lc_mode_set

esp_ble_mesh_light_lc_mode_set_t lc_mode_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET_UNACK 

lc_om_set

esp_ble_mesh_light_lc_om_set_t lc_om_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET_UNACK 

lc_light_onoff_set

esp_ble_mesh_light_lc_light_onoff_set_t lc_light_onoff_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET_UNACK 

lc_property_set

esp_ble_mesh_light_lc_property_set_t lc_property_set
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET & ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET_UNACK 



```

#### esp_ble_mesh_light_client_status_cb_t


```


union esp_ble_mesh_light_client_status_cb_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Client Model received message union. 

Public Members
lightness_status

esp_ble_mesh_light_lightness_status_cb_t lightness_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_STATUS 

lightness_linear_status

esp_ble_mesh_light_lightness_linear_status_cb_t lightness_linear_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_STATUS 

lightness_last_status

esp_ble_mesh_light_lightness_last_status_cb_t lightness_last_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_STATUS 

lightness_default_status

esp_ble_mesh_light_lightness_default_status_cb_t lightness_default_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_STATUS 

lightness_range_status

esp_ble_mesh_light_lightness_range_status_cb_t lightness_range_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_STATUS 

ctl_status

esp_ble_mesh_light_ctl_status_cb_t ctl_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_STATUS 

ctl_temperature_status

esp_ble_mesh_light_ctl_temperature_status_cb_t ctl_temperature_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_STATUS 

ctl_temperature_range_status

esp_ble_mesh_light_ctl_temperature_range_status_cb_t ctl_temperature_range_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_STATUS 

ctl_default_status

esp_ble_mesh_light_ctl_default_status_cb_t ctl_default_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_STATUS 

hsl_status

esp_ble_mesh_light_hsl_status_cb_t hsl_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_STATUS 

hsl_target_status

esp_ble_mesh_light_hsl_target_status_cb_t hsl_target_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_STATUS 

hsl_hue_status

esp_ble_mesh_light_hsl_hue_status_cb_t hsl_hue_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_STATUS 

hsl_saturation_status

esp_ble_mesh_light_hsl_saturation_status_cb_t hsl_saturation_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_STATUS 

hsl_default_status

esp_ble_mesh_light_hsl_default_status_cb_t hsl_default_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_STATUS 

hsl_range_status

esp_ble_mesh_light_hsl_range_status_cb_t hsl_range_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_STATUS 

xyl_status

esp_ble_mesh_light_xyl_status_cb_t xyl_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_STATUS 

xyl_target_status

esp_ble_mesh_light_xyl_target_status_cb_t xyl_target_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_STATUS 

xyl_default_status

esp_ble_mesh_light_xyl_default_status_cb_t xyl_default_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_STATUS 

xyl_range_status

esp_ble_mesh_light_xyl_range_status_cb_t xyl_range_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_STATUS 

lc_mode_status

esp_ble_mesh_light_lc_mode_status_cb_t lc_mode_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_STATUS 

lc_om_status

esp_ble_mesh_light_lc_om_status_cb_t lc_om_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_OM_STATUS 

lc_light_onoff_status

esp_ble_mesh_light_lc_light_onoff_status_cb_t lc_light_onoff_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_STATUS 

lc_property_status

esp_ble_mesh_light_lc_property_status_cb_t lc_property_status
For ESP_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_STATUS 



```

#### esp_ble_mesh_lighting_server_state_change_t


```


union esp_ble_mesh_lighting_server_state_change_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Server Model state change value union. 

Public Members
lightness_set

esp_ble_mesh_state_change_light_lightness_set_t lightness_set
The recv_op in ctx can be used to decide which state is changed. Light Lightness Set 

lightness_linear_set

esp_ble_mesh_state_change_light_lightness_linear_set_t lightness_linear_set
Light Lightness Linear Set 

lightness_default_set

esp_ble_mesh_state_change_light_lightness_default_set_t lightness_default_set
Light Lightness Default Set 

lightness_range_set

esp_ble_mesh_state_change_light_lightness_range_set_t lightness_range_set
Light Lightness Range Set 

ctl_set

esp_ble_mesh_state_change_light_ctl_set_t ctl_set
Light CTL Set 

ctl_temp_set

esp_ble_mesh_state_change_light_ctl_temperature_set_t ctl_temp_set
Light CTL Temperature Set 

ctl_temp_range_set

esp_ble_mesh_state_change_light_ctl_temperature_range_set_t ctl_temp_range_set
Light CTL Temperature Range Set 

ctl_default_set

esp_ble_mesh_state_change_light_ctl_default_set_t ctl_default_set
Light CTL Default Set 

hsl_set

esp_ble_mesh_state_change_light_hsl_set_t hsl_set
Light HSL Set 

hsl_hue_set

esp_ble_mesh_state_change_light_hsl_hue_set_t hsl_hue_set
Light HSL Hue Set 

hsl_saturation_set

esp_ble_mesh_state_change_light_hsl_saturation_set_t hsl_saturation_set
Light HSL Saturation Set 

hsl_default_set

esp_ble_mesh_state_change_light_hsl_default_set_t hsl_default_set
Light HSL Default Set 

hsl_range_set

esp_ble_mesh_state_change_light_hsl_range_set_t hsl_range_set
Light HSL Range Set 

xyl_set

esp_ble_mesh_state_change_light_xyl_set_t xyl_set
Light xyL Set 

xyl_default_set

esp_ble_mesh_state_change_light_xyl_default_set_t xyl_default_set
Light xyL Default Set 

xyl_range_set

esp_ble_mesh_state_change_light_xyl_range_set_t xyl_range_set
Light xyL Range Set 

lc_mode_set

esp_ble_mesh_state_change_light_lc_mode_set_t lc_mode_set
Light LC Mode Set 

lc_om_set

esp_ble_mesh_state_change_light_lc_om_set_t lc_om_set
Light LC Occupancy Mode Set 

lc_light_onoff_set

esp_ble_mesh_state_change_light_lc_light_onoff_set_t lc_light_onoff_set
Light LC Light OnOff Set 

lc_property_set

esp_ble_mesh_state_change_light_lc_property_set_t lc_property_set
Light LC Property Set 

sensor_status

esp_ble_mesh_state_change_sensor_status_t sensor_status
Sensor Status 



```

#### esp_ble_mesh_lighting_server_recv_get_msg_t


```


union esp_ble_mesh_lighting_server_recv_get_msg_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Server Model received get message union. 

Public Members
lc_property

esp_ble_mesh_server_recv_light_lc_property_get_t lc_property
Light LC Property Get 



```

#### esp_ble_mesh_lighting_server_recv_set_msg_t


```


union esp_ble_mesh_lighting_server_recv_set_msg_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Server Model received set message union. 

Public Members
lightness

esp_ble_mesh_server_recv_light_lightness_set_t lightness
Light Lightness Set/Light Lightness Set Unack 

lightness_linear

esp_ble_mesh_server_recv_light_lightness_linear_set_t lightness_linear
Light Lightness Linear Set/Light Lightness Linear Set Unack 

lightness_default

esp_ble_mesh_server_recv_light_lightness_default_set_t lightness_default
Light Lightness Default Set/Light Lightness Default Set Unack 

lightness_range

esp_ble_mesh_server_recv_light_lightness_range_set_t lightness_range
Light Lightness Range Set/Light Lightness Range Set Unack 

ctl

esp_ble_mesh_server_recv_light_ctl_set_t ctl
Light CTL Set/Light CTL Set Unack 

ctl_temp

esp_ble_mesh_server_recv_light_ctl_temperature_set_t ctl_temp
Light CTL Temperature Set/Light CTL Temperature Set Unack 

ctl_temp_range

esp_ble_mesh_server_recv_light_ctl_temperature_range_set_t ctl_temp_range
Light CTL Temperature Range Set/Light CTL Temperature Range Set Unack 

ctl_default

esp_ble_mesh_server_recv_light_ctl_default_set_t ctl_default
Light CTL Default Set/Light CTL Default Set Unack 

hsl

esp_ble_mesh_server_recv_light_hsl_set_t hsl
Light HSL Set/Light HSL Set Unack 

hsl_hue

esp_ble_mesh_server_recv_light_hsl_hue_set_t hsl_hue
Light HSL Hue Set/Light HSL Hue Set Unack 

hsl_saturation

esp_ble_mesh_server_recv_light_hsl_saturation_set_t hsl_saturation
Light HSL Saturation Set/Light HSL Saturation Set Unack 

hsl_default

esp_ble_mesh_server_recv_light_hsl_default_set_t hsl_default
Light HSL Default Set/Light HSL Default Set Unack 

hsl_range

esp_ble_mesh_server_recv_light_hsl_range_set_t hsl_range
Light HSL Range Set/Light HSL Range Set Unack 

xyl

esp_ble_mesh_server_recv_light_xyl_set_t xyl
Light xyL Set/Light xyL Set Unack 

xyl_default

esp_ble_mesh_server_recv_light_xyl_default_set_t xyl_default
Light xyL Default Set/Light xyL Default Set Unack 

xyl_range

esp_ble_mesh_server_recv_light_xyl_range_set_t xyl_range
Light xyL Range Set/Light xyL Range Set Unack 

lc_mode

esp_ble_mesh_server_recv_light_lc_mode_set_t lc_mode
Light LC Mode Set/Light LC Mode Set Unack 

lc_om

esp_ble_mesh_server_recv_light_lc_om_set_t lc_om
Light LC OM Set/Light LC OM Set Unack 

lc_light_onoff

esp_ble_mesh_server_recv_light_lc_light_onoff_set_t lc_light_onoff
Light LC Light OnOff Set/Light LC Light OnOff Set Unack 

lc_property

esp_ble_mesh_server_recv_light_lc_property_set_t lc_property
Light LC Property Set/Light LC Property Set Unack 



```

#### esp_ble_mesh_lighting_server_recv_status_msg_t


```


union esp_ble_mesh_lighting_server_recv_status_msg_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Server Model received status message union. 

Public Members
sensor_status

esp_ble_mesh_server_recv_sensor_status_t sensor_status
Sensor Status 



```

#### esp_ble_mesh_lighting_server_cb_value_t


```


union esp_ble_mesh_lighting_server_cb_value_t

#include <esp_ble_mesh_lighting_model_api.h>
Lighting Server Model callback value union. 

Public Members
state_change

esp_ble_mesh_lighting_server_state_change_t state_change
ESP_BLE_MESH_LIGHTING_SERVER_STATE_CHANGE_EVT 

get

esp_ble_mesh_lighting_server_recv_get_msg_t get
ESP_BLE_MESH_LIGHTING_SERVER_RECV_GET_MSG_EVT 

set

esp_ble_mesh_lighting_server_recv_set_msg_t set
ESP_BLE_MESH_LIGHTING_SERVER_RECV_SET_MSG_EVT 

status

esp_ble_mesh_lighting_server_recv_status_msg_t status
ESP_BLE_MESH_LIGHTING_SERVER_RECV_STATUS_MSG_EVT 



```



### Structures


#### esp_ble_mesh_light_lightness_set_t


```


struct esp_ble_mesh_light_lightness_set_t
Bluetooth Mesh Light Lightness Client Model Get and Set parameters structure. 
Parameters of Light Lightness Set 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

lightness

uint16_t lightness
Target value of light lightness actual state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_lightness_linear_set_t


```


struct esp_ble_mesh_light_lightness_linear_set_t
Parameters of Light Lightness Linear Set 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

lightness

uint16_t lightness
Target value of light lightness linear state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_lightness_default_set_t


```


struct esp_ble_mesh_light_lightness_default_set_t
Parameter of Light Lightness Default Set 

Public Members
lightness

uint16_t lightness
The value of the Light Lightness Default state 



```

#### esp_ble_mesh_light_lightness_range_set_t


```


struct esp_ble_mesh_light_lightness_range_set_t
Parameters of Light Lightness Range Set 

Public Members
range_min

uint16_t range_min
Value of range min field of light lightness range state 

range_max

uint16_t range_max
Value of range max field of light lightness range state 



```

#### esp_ble_mesh_light_ctl_set_t


```


struct esp_ble_mesh_light_ctl_set_t
Parameters of Light CTL Set 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

ctl_lightness

uint16_t ctl_lightness
Target value of light ctl lightness state 

ctl_temperature

uint16_t ctl_temperature
Target value of light ctl temperature state 

ctl_delta_uv

int16_t ctl_delta_uv
Target value of light ctl delta UV state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_ctl_temperature_set_t


```


struct esp_ble_mesh_light_ctl_temperature_set_t
Parameters of Light CTL Temperature Set 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

ctl_temperature

uint16_t ctl_temperature
Target value of light ctl temperature state 

ctl_delta_uv

int16_t ctl_delta_uv
Target value of light ctl delta UV state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_ctl_temperature_range_set_t


```


struct esp_ble_mesh_light_ctl_temperature_range_set_t
Parameters of Light CTL Temperature Range Set 

Public Members
range_min

uint16_t range_min
Value of temperature range min field of light ctl temperature range state 

range_max

uint16_t range_max
Value of temperature range max field of light ctl temperature range state 



```

#### esp_ble_mesh_light_ctl_default_set_t


```


struct esp_ble_mesh_light_ctl_default_set_t
Parameters of Light CTL Default Set 

Public Members
lightness

uint16_t lightness
Value of light lightness default state 

temperature

uint16_t temperature
Value of light temperature default state 

delta_uv

int16_t delta_uv
Value of light delta UV default state 



```

#### esp_ble_mesh_light_hsl_set_t


```


struct esp_ble_mesh_light_hsl_set_t
Parameters of Light HSL Set 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

hsl_lightness

uint16_t hsl_lightness
Target value of light hsl lightness state 

hsl_hue

uint16_t hsl_hue
Target value of light hsl hue state 

hsl_saturation

uint16_t hsl_saturation
Target value of light hsl saturation state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_hsl_hue_set_t


```


struct esp_ble_mesh_light_hsl_hue_set_t
Parameters of Light HSL Hue Set 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

hue

uint16_t hue
Target value of light hsl hue state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_hsl_saturation_set_t


```


struct esp_ble_mesh_light_hsl_saturation_set_t
Parameters of Light HSL Saturation Set 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

saturation

uint16_t saturation
Target value of light hsl hue state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_hsl_default_set_t


```


struct esp_ble_mesh_light_hsl_default_set_t
Parameters of Light HSL Default Set 

Public Members
lightness

uint16_t lightness
Value of light lightness default state 

hue

uint16_t hue
Value of light hue default state 

saturation

uint16_t saturation
Value of light saturation default state 



```

#### esp_ble_mesh_light_hsl_range_set_t


```


struct esp_ble_mesh_light_hsl_range_set_t
Parameters of Light HSL Range Set 

Public Members
hue_range_min

uint16_t hue_range_min
Value of hue range min field of light hsl hue range state 

hue_range_max

uint16_t hue_range_max
Value of hue range max field of light hsl hue range state 

saturation_range_min

uint16_t saturation_range_min
Value of saturation range min field of light hsl saturation range state 

saturation_range_max

uint16_t saturation_range_max
Value of saturation range max field of light hsl saturation range state 



```

#### esp_ble_mesh_light_xyl_set_t


```


struct esp_ble_mesh_light_xyl_set_t
Parameters of Light xyL Set 

Public Members
op_en

bool op_en
Indicate whether optional parameters included 

xyl_lightness

uint16_t xyl_lightness
The target value of the Light xyL Lightness state 

xyl_x

uint16_t xyl_x
The target value of the Light xyL x state 

xyl_y

uint16_t xyl_y
The target value of the Light xyL y state 

tid

uint8_t tid
Transaction Identifier 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_xyl_default_set_t


```


struct esp_ble_mesh_light_xyl_default_set_t
Parameters of Light xyL Default Set 

Public Members
lightness

uint16_t lightness
The value of the Light Lightness Default state 

xyl_x

uint16_t xyl_x
The value of the Light xyL x Default state 

xyl_y

uint16_t xyl_y
The value of the Light xyL y Default state 



```

#### esp_ble_mesh_light_xyl_range_set_t


```


struct esp_ble_mesh_light_xyl_range_set_t
Parameters of Light xyL Range Set 

Public Members
xyl_x_range_min

uint16_t xyl_x_range_min
The value of the xyL x Range Min field of the Light xyL x Range state 

xyl_x_range_max

uint16_t xyl_x_range_max
The value of the xyL x Range Max field of the Light xyL x Range state 

xyl_y_range_min

uint16_t xyl_y_range_min
The value of the xyL y Range Min field of the Light xyL y Range state 

xyl_y_range_max

uint16_t xyl_y_range_max
The value of the xyL y Range Max field of the Light xyL y Range state 



```

#### esp_ble_mesh_light_lc_mode_set_t


```


struct esp_ble_mesh_light_lc_mode_set_t
Parameter of Light LC Mode Set 

Public Members
mode

uint8_t mode
The target value of the Light LC Mode state 



```

#### esp_ble_mesh_light_lc_om_set_t


```


struct esp_ble_mesh_light_lc_om_set_t
Parameter of Light LC OM Set 

Public Members
mode

uint8_t mode
The target value of the Light LC Occupancy Mode state 



```

#### esp_ble_mesh_light_lc_light_onoff_set_t


```


struct esp_ble_mesh_light_lc_light_onoff_set_t
Parameters of Light LC Light OnOff Set 

Public Members
op_en

bool op_en
Indicate whether optional parameters included 

light_onoff

uint8_t light_onoff
The target value of the Light LC Light OnOff state 

tid

uint8_t tid
Transaction Identifier 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_light_lc_property_get_t


```


struct esp_ble_mesh_light_lc_property_get_t
Parameter of Light LC Property Get 

Public Members
property_id

uint16_t property_id
Property ID identifying a Light LC Property 



```

#### esp_ble_mesh_light_lc_property_set_t


```


struct esp_ble_mesh_light_lc_property_set_t
Parameters of Light LC Property Set 

Public Members
property_id

uint16_t property_id
Property ID identifying a Light LC Property 

property_value

struct net_buf_simple *property_value
Raw value for the Light LC Property 



```

#### esp_ble_mesh_light_lightness_status_cb_t


```


struct esp_ble_mesh_light_lightness_status_cb_t
Bluetooth Mesh Light Lightness Client Model Get and Set callback parameters structure. 
Parameters of Light Lightness Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_lightness

uint16_t present_lightness
Current value of light lightness actual state 

target_lightness

uint16_t target_lightness
Target value of light lightness actual state (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_light_lightness_linear_status_cb_t


```


struct esp_ble_mesh_light_lightness_linear_status_cb_t
Parameters of Light Lightness Linear Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_lightness

uint16_t present_lightness
Current value of light lightness linear state 

target_lightness

uint16_t target_lightness
Target value of light lightness linear state (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_light_lightness_last_status_cb_t


```


struct esp_ble_mesh_light_lightness_last_status_cb_t
Parameter of Light Lightness Last Status 

Public Members
lightness

uint16_t lightness
The value of the Light Lightness Last state 



```

#### esp_ble_mesh_light_lightness_default_status_cb_t


```


struct esp_ble_mesh_light_lightness_default_status_cb_t
Parameter of Light Lightness Default Status 

Public Members
lightness

uint16_t lightness
The value of the Light Lightness default State 



```

#### esp_ble_mesh_light_lightness_range_status_cb_t


```


struct esp_ble_mesh_light_lightness_range_status_cb_t
Parameters of Light Lightness Range Status 

Public Members
status_code

uint8_t status_code
Status Code for the request message 

range_min

uint16_t range_min
Value of range min field of light lightness range state 

range_max

uint16_t range_max
Value of range max field of light lightness range state 



```

#### esp_ble_mesh_light_ctl_status_cb_t


```


struct esp_ble_mesh_light_ctl_status_cb_t
Parameters of Light CTL Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_ctl_lightness

uint16_t present_ctl_lightness
Current value of light ctl lightness state 

present_ctl_temperature

uint16_t present_ctl_temperature
Current value of light ctl temperature state 

target_ctl_lightness

uint16_t target_ctl_lightness
Target value of light ctl lightness state (optional) 

target_ctl_temperature

uint16_t target_ctl_temperature
Target value of light ctl temperature state (C.1) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_light_ctl_temperature_status_cb_t


```


struct esp_ble_mesh_light_ctl_temperature_status_cb_t
Parameters of Light CTL Temperature Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_ctl_temperature

uint16_t present_ctl_temperature
Current value of light ctl temperature state 

present_ctl_delta_uv

uint16_t present_ctl_delta_uv
Current value of light ctl delta UV state 

target_ctl_temperature

uint16_t target_ctl_temperature
Target value of light ctl temperature state (optional) 

target_ctl_delta_uv

uint16_t target_ctl_delta_uv
Target value of light ctl delta UV state (C.1) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_light_ctl_temperature_range_status_cb_t


```


struct esp_ble_mesh_light_ctl_temperature_range_status_cb_t
Parameters of Light CTL Temperature Range Status 

Public Members
status_code

uint8_t status_code
Status code for the request message 

range_min

uint16_t range_min
Value of temperature range min field of light ctl temperature range state 

range_max

uint16_t range_max
Value of temperature range max field of light ctl temperature range state 



```

#### esp_ble_mesh_light_ctl_default_status_cb_t


```


struct esp_ble_mesh_light_ctl_default_status_cb_t
Parameters of Light CTL Default Status 

Public Members
lightness

uint16_t lightness
Value of light lightness default state 

temperature

uint16_t temperature
Value of light temperature default state 

delta_uv

int16_t delta_uv
Value of light delta UV default state 



```

#### esp_ble_mesh_light_hsl_status_cb_t


```


struct esp_ble_mesh_light_hsl_status_cb_t
Parameters of Light HSL Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

hsl_lightness

uint16_t hsl_lightness
Current value of light hsl lightness state 

hsl_hue

uint16_t hsl_hue
Current value of light hsl hue state 

hsl_saturation

uint16_t hsl_saturation
Current value of light hsl saturation state 

remain_time

uint8_t remain_time
Time to complete state transition (optional) 



```

#### esp_ble_mesh_light_hsl_target_status_cb_t


```


struct esp_ble_mesh_light_hsl_target_status_cb_t
Parameters of Light HSL Target Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

hsl_lightness_target

uint16_t hsl_lightness_target
Target value of light hsl lightness state 

hsl_hue_target

uint16_t hsl_hue_target
Target value of light hsl hue state 

hsl_saturation_target

uint16_t hsl_saturation_target
Target value of light hsl saturation state 

remain_time

uint8_t remain_time
Time to complete state transition (optional) 



```

#### esp_ble_mesh_light_hsl_hue_status_cb_t


```


struct esp_ble_mesh_light_hsl_hue_status_cb_t
Parameters of Light HSL Hue Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_hue

uint16_t present_hue
Current value of light hsl hue state 

target_hue

uint16_t target_hue
Target value of light hsl hue state (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_light_hsl_saturation_status_cb_t


```


struct esp_ble_mesh_light_hsl_saturation_status_cb_t
Parameters of Light HSL Saturation Status 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

present_saturation

uint16_t present_saturation
Current value of light hsl saturation state 

target_saturation

uint16_t target_saturation
Target value of light hsl saturation state (optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_light_hsl_default_status_cb_t


```


struct esp_ble_mesh_light_hsl_default_status_cb_t
Parameters of Light HSL Default Status 

Public Members
lightness

uint16_t lightness
Value of light lightness default state 

hue

uint16_t hue
Value of light hue default state 

saturation

uint16_t saturation
Value of light saturation default state 



```

#### esp_ble_mesh_light_hsl_range_status_cb_t


```


struct esp_ble_mesh_light_hsl_range_status_cb_t
Parameters of Light HSL Range Status 

Public Members
status_code

uint8_t status_code
Status code for the request message 

hue_range_min

uint16_t hue_range_min
Value of hue range min field of light hsl hue range state 

hue_range_max

uint16_t hue_range_max
Value of hue range max field of light hsl hue range state 

saturation_range_min

uint16_t saturation_range_min
Value of saturation range min field of light hsl saturation range state 

saturation_range_max

uint16_t saturation_range_max
Value of saturation range max field of light hsl saturation range state 



```

#### esp_ble_mesh_light_xyl_status_cb_t


```


struct esp_ble_mesh_light_xyl_status_cb_t
Parameters of Light xyL Status 

Public Members
op_en

bool op_en
Indicate whether optional parameters included 

xyl_lightness

uint16_t xyl_lightness
The present value of the Light xyL Lightness state 

xyl_x

uint16_t xyl_x
The present value of the Light xyL x state 

xyl_y

uint16_t xyl_y
The present value of the Light xyL y state 

remain_time

uint8_t remain_time
Time to complete state transition (optional) 



```

#### esp_ble_mesh_light_xyl_target_status_cb_t


```


struct esp_ble_mesh_light_xyl_target_status_cb_t
Parameters of Light xyL Target Status 

Public Members
op_en

bool op_en
Indicate whether optional parameters included 

target_xyl_lightness

uint16_t target_xyl_lightness
The target value of the Light xyL Lightness state 

target_xyl_x

uint16_t target_xyl_x
The target value of the Light xyL x state 

target_xyl_y

uint16_t target_xyl_y
The target value of the Light xyL y state 

remain_time

uint8_t remain_time
Time to complete state transition (optional) 



```

#### esp_ble_mesh_light_xyl_default_status_cb_t


```


struct esp_ble_mesh_light_xyl_default_status_cb_t
Parameters of Light xyL Default Status 

Public Members
lightness

uint16_t lightness
The value of the Light Lightness Default state 

xyl_x

uint16_t xyl_x
The value of the Light xyL x Default state 

xyl_y

uint16_t xyl_y
The value of the Light xyL y Default state 



```

#### esp_ble_mesh_light_xyl_range_status_cb_t


```


struct esp_ble_mesh_light_xyl_range_status_cb_t
Parameters of Light xyL Range Status 

Public Members
status_code

uint8_t status_code
Status Code for the requesting message 

xyl_x_range_min

uint16_t xyl_x_range_min
The value of the xyL x Range Min field of the Light xyL x Range state 

xyl_x_range_max

uint16_t xyl_x_range_max
The value of the xyL x Range Max field of the Light xyL x Range state 

xyl_y_range_min

uint16_t xyl_y_range_min
The value of the xyL y Range Min field of the Light xyL y Range state 

xyl_y_range_max

uint16_t xyl_y_range_max
The value of the xyL y Range Max field of the Light xyL y Range state 



```

#### esp_ble_mesh_light_lc_mode_status_cb_t


```


struct esp_ble_mesh_light_lc_mode_status_cb_t
Parameter of Light LC Mode Status 

Public Members
mode

uint8_t mode
The present value of the Light LC Mode state 



```

#### esp_ble_mesh_light_lc_om_status_cb_t


```


struct esp_ble_mesh_light_lc_om_status_cb_t
Parameter of Light LC OM Status 

Public Members
mode

uint8_t mode
The present value of the Light LC Occupancy Mode state 



```

#### esp_ble_mesh_light_lc_light_onoff_status_cb_t


```


struct esp_ble_mesh_light_lc_light_onoff_status_cb_t
Parameters of Light LC Light OnOff Status 

Public Members
op_en

bool op_en
Indicate whether optional parameters included 

present_light_onoff

uint8_t present_light_onoff
The present value of the Light LC Light OnOff state 

target_light_onoff

uint8_t target_light_onoff
The target value of the Light LC Light OnOff state (Optional) 

remain_time

uint8_t remain_time
Time to complete state transition (C.1) 



```

#### esp_ble_mesh_light_lc_property_status_cb_t


```


struct esp_ble_mesh_light_lc_property_status_cb_t
Parameters of Light LC Property Status 

Public Members
property_id

uint16_t property_id
Property ID identifying a Light LC Property 

property_value

struct net_buf_simple *property_value
Raw value for the Light LC Property 



```

#### esp_ble_mesh_light_client_cb_param_t


```


struct esp_ble_mesh_light_client_cb_param_t
Lighting Client Model callback parameters 

Public Members
error_code

int error_code
Appropriate error code 

params

esp_ble_mesh_client_common_param_t *params
The client common parameters. 

status_cb

esp_ble_mesh_light_client_status_cb_t status_cb
The light status message callback values 



```

#### esp_ble_mesh_light_lightness_state_t


```


struct esp_ble_mesh_light_lightness_state_t
Parameters of Light Lightness state 

Public Members
lightness_linear

uint16_t lightness_linear
The present value of Light Lightness Linear state 

target_lightness_linear

uint16_t target_lightness_linear
The target value of Light Lightness Linear state 

lightness_actual

uint16_t lightness_actual
The present value of Light Lightness Actual state 

target_lightness_actual

uint16_t target_lightness_actual
The target value of Light Lightness Actual state 

lightness_last

uint16_t lightness_last
The value of Light Lightness Last state 

lightness_default

uint16_t lightness_default
The value of Light Lightness Default state 

status_code

uint8_t status_code
The status code of setting Light Lightness Range state 

lightness_range_min

uint16_t lightness_range_min
The minimum value of Light Lightness Range state 

lightness_range_max

uint16_t lightness_range_max
The maximum value of Light Lightness Range state 



```

#### esp_ble_mesh_light_lightness_srv_t


```


struct esp_ble_mesh_light_lightness_srv_t
User data of Light Lightness Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting Lightness Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_lightness_state_t *state
Parameters of the Light Lightness state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

actual_transition

esp_ble_mesh_state_transition_t actual_transition
Parameters of state transition 

linear_transition

esp_ble_mesh_state_transition_t linear_transition
Parameters of state transition 

tt_delta_lightness_actual

int32_t tt_delta_lightness_actual
Delta change value of lightness actual state transition 

tt_delta_lightness_linear

int32_t tt_delta_lightness_linear
Delta change value of lightness linear state transition 



```

#### esp_ble_mesh_light_lightness_setup_srv_t


```


struct esp_ble_mesh_light_lightness_setup_srv_t
User data of Light Lightness Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting Lightness Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_lightness_state_t *state
Parameters of the Light Lightness state 



```

#### esp_ble_mesh_light_ctl_state_t


```


struct esp_ble_mesh_light_ctl_state_t
Parameters of Light CTL state 

Public Members
lightness

uint16_t lightness
The present value of Light CTL Lightness state 

target_lightness

uint16_t target_lightness
The target value of Light CTL Lightness state 

temperature

uint16_t temperature
The present value of Light CTL Temperature state 

target_temperature

uint16_t target_temperature
The target value of Light CTL Temperature state 

delta_uv

int16_t delta_uv
The present value of Light CTL Delta UV state 

target_delta_uv

int16_t target_delta_uv
The target value of Light CTL Delta UV state 

status_code

uint8_t status_code
The statue code of setting Light CTL Temperature Range state 

temperature_range_min

uint16_t temperature_range_min
The minimum value of Light CTL Temperature Range state 

temperature_range_max

uint16_t temperature_range_max
The maximum value of Light CTL Temperature Range state 

lightness_default

uint16_t lightness_default
The value of Light Lightness Default state 

temperature_default

uint16_t temperature_default
The value of Light CTL Temperature Default state 

delta_uv_default

int16_t delta_uv_default
The value of Light CTL Delta UV Default state 



```

#### esp_ble_mesh_light_ctl_srv_t


```


struct esp_ble_mesh_light_ctl_srv_t
User data of Light CTL Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting CTL Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_ctl_state_t *state
Parameters of the Light CTL state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_lightness

int32_t tt_delta_lightness
Delta change value of lightness state transition 

tt_delta_temperature

int32_t tt_delta_temperature
Delta change value of temperature state transition 

tt_delta_delta_uv

int32_t tt_delta_delta_uv
Delta change value of delta uv state transition 



```

#### esp_ble_mesh_light_ctl_setup_srv_t


```


struct esp_ble_mesh_light_ctl_setup_srv_t
User data of Light CTL Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting CTL Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_ctl_state_t *state
Parameters of the Light CTL state 



```

#### esp_ble_mesh_light_ctl_temp_srv_t


```


struct esp_ble_mesh_light_ctl_temp_srv_t
User data of Light CTL Temperature Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting CTL Temperature Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_ctl_state_t *state
Parameters of the Light CTL state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_temperature

int32_t tt_delta_temperature
Delta change value of temperature state transition 

tt_delta_delta_uv

int32_t tt_delta_delta_uv
Delta change value of delta uv state transition 



```

#### esp_ble_mesh_light_hsl_state_t


```


struct esp_ble_mesh_light_hsl_state_t
Parameters of Light HSL state 

Public Members
lightness

uint16_t lightness
The present value of Light HSL Lightness state 

target_lightness

uint16_t target_lightness
The target value of Light HSL Lightness state 

hue

uint16_t hue
The present value of Light HSL Hue state 

target_hue

uint16_t target_hue
The target value of Light HSL Hue state 

saturation

uint16_t saturation
The present value of Light HSL Saturation state 

target_saturation

uint16_t target_saturation
The target value of Light HSL Saturation state 

lightness_default

uint16_t lightness_default
The value of Light Lightness Default state 

hue_default

uint16_t hue_default
The value of Light HSL Hue Default state 

saturation_default

uint16_t saturation_default
The value of Light HSL Saturation Default state 

status_code

uint8_t status_code
The status code of setting Light HSL Hue & Saturation Range state 

hue_range_min

uint16_t hue_range_min
The minimum value of Light HSL Hue Range state 

hue_range_max

uint16_t hue_range_max
The maximum value of Light HSL Hue Range state 

saturation_range_min

uint16_t saturation_range_min
The minimum value of Light HSL Saturation state 

saturation_range_max

uint16_t saturation_range_max
The maximum value of Light HSL Saturation state 



```

#### esp_ble_mesh_light_hsl_srv_t


```


struct esp_ble_mesh_light_hsl_srv_t
User data of Light HSL Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting HSL Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_hsl_state_t *state
Parameters of the Light HSL state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_lightness

int32_t tt_delta_lightness
Delta change value of lightness state transition 

tt_delta_hue

int32_t tt_delta_hue
Delta change value of hue state transition 

tt_delta_saturation

int32_t tt_delta_saturation
Delta change value of saturation state transition 



```

#### esp_ble_mesh_light_hsl_setup_srv_t


```


struct esp_ble_mesh_light_hsl_setup_srv_t
User data of Light HSL Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting HSL Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_hsl_state_t *state
Parameters of the Light HSL state 



```

#### esp_ble_mesh_light_hsl_hue_srv_t


```


struct esp_ble_mesh_light_hsl_hue_srv_t
User data of Light HSL Hue Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting HSL Hue Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_hsl_state_t *state
Parameters of the Light HSL state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_hue

int32_t tt_delta_hue
Delta change value of hue state transition 



```

#### esp_ble_mesh_light_hsl_sat_srv_t


```


struct esp_ble_mesh_light_hsl_sat_srv_t
User data of Light HSL Saturation Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting HSL Saturation Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_hsl_state_t *state
Parameters of the Light HSL state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_saturation

int32_t tt_delta_saturation
Delta change value of saturation state transition 



```

#### esp_ble_mesh_light_xyl_state_t


```


struct esp_ble_mesh_light_xyl_state_t
Parameters of Light xyL state 

Public Members
lightness

uint16_t lightness
The present value of Light xyL Lightness state 

target_lightness

uint16_t target_lightness
The target value of Light xyL Lightness state 

x

uint16_t x
The present value of Light xyL x state 

target_x

uint16_t target_x
The target value of Light xyL x state 

y

uint16_t y
The present value of Light xyL y state 

target_y

uint16_t target_y
The target value of Light xyL y state 

lightness_default

uint16_t lightness_default
The value of Light Lightness Default state 

x_default

uint16_t x_default
The value of Light xyL x Default state 

y_default

uint16_t y_default
The value of Light xyL y Default state 

status_code

uint8_t status_code
The status code of setting Light xyL x & y Range state 

x_range_min

uint16_t x_range_min
The minimum value of Light xyL x Range state 

x_range_max

uint16_t x_range_max
The maximum value of Light xyL x Range state 

y_range_min

uint16_t y_range_min
The minimum value of Light xyL y Range state 

y_range_max

uint16_t y_range_max
The maximum value of Light xyL y Range state 



```

#### esp_ble_mesh_light_xyl_srv_t


```


struct esp_ble_mesh_light_xyl_srv_t
User data of Light xyL Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting xyL Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_xyl_state_t *state
Parameters of the Light xyL state 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 

tt_delta_lightness

int32_t tt_delta_lightness
Delta change value of lightness state transition 

tt_delta_x

int32_t tt_delta_x
Delta change value of x state transition 

tt_delta_y

int32_t tt_delta_y
Delta change value of y state transition 



```

#### esp_ble_mesh_light_xyl_setup_srv_t


```


struct esp_ble_mesh_light_xyl_setup_srv_t
User data of Light xyL Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting xyL Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

state

esp_ble_mesh_light_xyl_state_t *state
Parameters of the Light xyL state 



```

#### esp_ble_mesh_light_lc_state_t


```


struct esp_ble_mesh_light_lc_state_t
Parameters of Light LC states 

Public Members
mode

uint32_t mode
0b0 The controller is turned off.
The binding with the Light Lightness state is disabled. 0b1 The controller is turned on.
The binding with the Light Lightness state is enabled. The value of Light LC Mode state 



occupancy_mode

uint32_t occupancy_mode
The value of Light LC Occupancy Mode state 

light_onoff

uint32_t light_onoff
The present value of Light LC Light OnOff state 

target_light_onoff

uint32_t target_light_onoff
The target value of Light LC Light OnOff state 

occupancy

uint32_t occupancy
The value of Light LC Occupancy state 

ambient_luxlevel

uint32_t ambient_luxlevel
The value of Light LC Ambient LuxLevel state 

linear_output

uint16_t linear_output

Light LC Linear Output = max((Lightness Out)^2/65535, Regulator Output)
If the Light LC Mode state is set to 0b1, the binding is enabled and upon a change of the Light LC Linear Output state, the following operation shall be performed: Light Lightness Linear = Light LC Linear Output
If the Light LC Mode state is set to 0b0, the binding is disabled (i.e., upon a change of the Light LC Linear Output state, no operation on the Light Lightness Linear state is performed). The value of Light LC Linear Output state 





```

#### esp_ble_mesh_light_lc_property_state_t


```


struct esp_ble_mesh_light_lc_property_state_t
Parameters of Light Property states. The Light LC Property states are read / write states that determine the configuration of a Light Lightness Controller. Each state is represented by a device property and is controlled by Light LC Property messages. 

Public Members
time_occupancy_delay

uint32_t time_occupancy_delay
A timing state that determines the delay for changing the Light LC Occupancy state upon receiving a Sensor Status message from an occupancy sensor. The value of Light LC Time Occupancy Delay state 

time_fade_on

uint32_t time_fade_on
A timing state that determines the time the controlled lights fade to the level determined by the Light LC Lightness On state. The value of Light LC Time Fade On state 

time_run_on

uint32_t time_run_on
A timing state that determines the time the controlled lights stay at the level determined by the Light LC Lightness On state. The value of Light LC Time Run On state 

time_fade

uint32_t time_fade
A timing state that determines the time the controlled lights fade from the level determined by the Light LC Lightness On state to the level determined by the Light Lightness Prolong state. The value of Light LC Time Fade state 

time_prolong

uint32_t time_prolong
A timing state that determines the time the controlled lights stay at the level determined by the Light LC Lightness Prolong state. The value of Light LC Time Prolong state 

time_fade_standby_auto

uint32_t time_fade_standby_auto
A timing state that determines the time the controlled lights fade from the level determined by the Light LC Lightness Prolong state to the level determined by the Light LC Lightness Standby state when the transition is automatic. The value of Light LC Time Fade Standby Auto state 

time_fade_standby_manual

uint32_t time_fade_standby_manual
A timing state that determines the time the controlled lights fade from the level determined by the Light LC Lightness Prolong state to the level determined by the Light LC Lightness Standby state when the transition is triggered by a change in the Light LC Light OnOff state. The value of Light LC Time Fade Standby Manual state 

lightness_on

uint16_t lightness_on
A lightness state that determines the perceptive light lightness at the Occupancy and Run internal controller states. The value of Light LC Lightness On state 

lightness_prolong

uint16_t lightness_prolong
A lightness state that determines the light lightness at the Prolong internal controller state. The value of Light LC Lightness Prolong state 

lightness_standby

uint16_t lightness_standby
A lightness state that determines the light lightness at the Standby internal controller state. The value of Light LC Lightness Standby state 

ambient_luxlevel_on

uint16_t ambient_luxlevel_on
A uint16 state representing the Ambient LuxLevel level that determines if the controller transitions from the Light Control Standby state. The value of Light LC Ambient LuxLevel On state 

ambient_luxlevel_prolong

uint16_t ambient_luxlevel_prolong
A uint16 state representing the required Ambient LuxLevel level in the Prolong state. The value of Light LC Ambient LuxLevel Prolong state 

ambient_luxlevel_standby

uint16_t ambient_luxlevel_standby
A uint16 state representing the required Ambient LuxLevel level in the Standby state. The value of Light LC Ambient LuxLevel Standby state 

regulator_kiu

float regulator_kiu
A float32 state representing the integral coefficient that determines the integral part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is less than LuxLevel Out. Valid range: 0.0 ~ 1000.0. The default value is 250.0. The value of Light LC Regulator Kiu state 

regulator_kid

float regulator_kid
A float32 state representing the integral coefficient that determines the integral part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is greater than or equal to the value of the LuxLevel Out state. Valid range: 0.0 ~ 1000.0. The default value is 25.0. The value of Light LC Regulator Kid state 

regulator_kpu

float regulator_kpu
A float32 state representing the proportional coefficient that determines the proportional part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is less than the value of the LuxLevel Out state. Valid range: 0.0 ~ 1000.0. The default value is 80.0. The value of Light LC Regulator Kpu state 

regulator_kpd

float regulator_kpd
A float32 state representing the proportional coefficient that determines the proportional part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is greater than or equal to the value of the LuxLevel Out state. Valid range: 0.0 ~ 1000.0. The default value is 80.0. The value of Light LC Regulator Kpd state 

regulator_accuracy

int8_t regulator_accuracy
A int8 state representing the percentage accuracy of the Light LC PI Feedback Regulator. Valid range: 0.0 ~ 100.0. The default value is 2.0. The value of Light LC Regulator Accuracy state 

set_occupancy_to_1_delay

uint32_t set_occupancy_to_1_delay
If the message Raw field contains a Raw Value for the Time Since Motion Sensed device property, which represents a value less than or equal to the value of the Light LC Occupancy Delay state, it shall delay setting the Light LC Occupancy state to 0b1 by the difference between the value of the Light LC Occupancy Delay state and the received Time Since Motion value. The value of the difference between value of the Light LC Occupancy Delay state and the received Time Since Motion value 



```

#### esp_ble_mesh_light_lc_state_machine_t


```


struct esp_ble_mesh_light_lc_state_machine_t
Parameters of Light LC state machine 

Public Members
fade_on

uint8_t fade_on
The value of transition time of Light LC Time Fade On 

fade

uint8_t fade
The value of transition time of Light LC Time Fade 

fade_standby_auto

uint8_t fade_standby_auto
The value of transition time of Light LC Time Fade Standby Auto 

fade_standby_manual

uint8_t fade_standby_manual
The value of transition time of Light LC Time Fade Standby Manual 

trans_time

struct esp_ble_mesh_light_lc_state_machine_t::[anonymous] trans_time
The Fade On, Fade, Fade Standby Auto, and Fade Standby Manual states are transition states that define the transition of the Lightness Out and LuxLevel Out states. This transition can be started as a result of the Light LC State Machine change or as a result of receiving the Light LC Light OnOff Set or Light LC Light Set Unacknowledged message. The value of transition time 

state

esp_ble_mesh_lc_state_t state
The value of Light LC state machine state 

timer

struct k_delayed_work timer
Timer of Light LC state machine 



```

#### esp_ble_mesh_light_control_t


```


struct esp_ble_mesh_light_control_t
Parameters of Light Lightness controller 

Public Members
state

esp_ble_mesh_light_lc_state_t state
Parameters of Light LC state 

prop_state

esp_ble_mesh_light_lc_property_state_t prop_state
Parameters of Light LC Property state 

state_machine

esp_ble_mesh_light_lc_state_machine_t state_machine
Parameters of Light LC state machine 



```

#### esp_ble_mesh_light_lc_srv_t


```


struct esp_ble_mesh_light_lc_srv_t
User data of Light LC Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting LC Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

lc

esp_ble_mesh_light_control_t *lc
Parameters of the Light controller 

last

esp_ble_mesh_last_msg_info_t last
Parameters of the last received set message 

transition

esp_ble_mesh_state_transition_t transition
Parameters of state transition 



```

#### esp_ble_mesh_light_lc_setup_srv_t


```


struct esp_ble_mesh_light_lc_setup_srv_t
User data of Light LC Setup Server Model 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the Lighting LC Setup Server Model. Initialized internally. 

rsp_ctrl

esp_ble_mesh_server_rsp_ctrl_t rsp_ctrl
Response control of the server model received messages 

lc

esp_ble_mesh_light_control_t *lc
Parameters of the Light controller 



```

#### esp_ble_mesh_state_change_light_lightness_set_t


```


struct esp_ble_mesh_state_change_light_lightness_set_t
Parameter of Light Lightness Actual state change event 

Public Members
lightness

uint16_t lightness
The value of Light Lightness Actual state 



```

#### esp_ble_mesh_state_change_light_lightness_linear_set_t


```


struct esp_ble_mesh_state_change_light_lightness_linear_set_t
Parameter of Light Lightness Linear state change event 

Public Members
lightness

uint16_t lightness
The value of Light Lightness Linear state 



```

#### esp_ble_mesh_state_change_light_lightness_default_set_t


```


struct esp_ble_mesh_state_change_light_lightness_default_set_t
Parameter of Light Lightness Default state change event 

Public Members
lightness

uint16_t lightness
The value of Light Lightness Default state 



```

#### esp_ble_mesh_state_change_light_lightness_range_set_t


```


struct esp_ble_mesh_state_change_light_lightness_range_set_t
Parameters of Light Lightness Range state change event 

Public Members
range_min

uint16_t range_min
The minimum value of Light Lightness Range state 

range_max

uint16_t range_max
The maximum value of Light Lightness Range state 



```

#### esp_ble_mesh_state_change_light_ctl_set_t


```


struct esp_ble_mesh_state_change_light_ctl_set_t
Parameters of Light CTL state change event 

Public Members
lightness

uint16_t lightness
The value of Light CTL Lightness state 

temperature

uint16_t temperature
The value of Light CTL Temperature state 

delta_uv

int16_t delta_uv
The value of Light CTL Delta UV state 



```

#### esp_ble_mesh_state_change_light_ctl_temperature_set_t


```


struct esp_ble_mesh_state_change_light_ctl_temperature_set_t
Parameters of Light CTL Temperature state change event 

Public Members
temperature

uint16_t temperature
The value of Light CTL Temperature state 

delta_uv

int16_t delta_uv
The value of Light CTL Delta UV state 



```

#### esp_ble_mesh_state_change_light_ctl_temperature_range_set_t


```


struct esp_ble_mesh_state_change_light_ctl_temperature_range_set_t
Parameters of Light CTL Temperature Range state change event 

Public Members
range_min

uint16_t range_min
The minimum value of Light CTL Temperature Range state 

range_max

uint16_t range_max
The maximum value of Light CTL Temperature Range state 



```

#### esp_ble_mesh_state_change_light_ctl_default_set_t


```


struct esp_ble_mesh_state_change_light_ctl_default_set_t
Parameters of Light CTL Default state change event 

Public Members
lightness

uint16_t lightness
The value of Light Lightness Default state 

temperature

uint16_t temperature
The value of Light CTL Temperature Default state 

delta_uv

int16_t delta_uv
The value of Light CTL Delta UV Default state 



```

#### esp_ble_mesh_state_change_light_hsl_set_t


```


struct esp_ble_mesh_state_change_light_hsl_set_t
Parameters of Light HSL state change event 

Public Members
lightness

uint16_t lightness
The value of Light HSL Lightness state 

hue

uint16_t hue
The value of Light HSL Hue state 

saturation

uint16_t saturation
The value of Light HSL Saturation state 



```

#### esp_ble_mesh_state_change_light_hsl_hue_set_t


```


struct esp_ble_mesh_state_change_light_hsl_hue_set_t
Parameter of Light HSL Hue state change event 

Public Members
hue

uint16_t hue
The value of Light HSL Hue state 



```

#### esp_ble_mesh_state_change_light_hsl_saturation_set_t


```


struct esp_ble_mesh_state_change_light_hsl_saturation_set_t
Parameter of Light HSL Saturation state change event 

Public Members
saturation

uint16_t saturation
The value of Light HSL Saturation state 



```

#### esp_ble_mesh_state_change_light_hsl_default_set_t


```


struct esp_ble_mesh_state_change_light_hsl_default_set_t
Parameters of Light HSL Default state change event 

Public Members
lightness

uint16_t lightness
The value of Light HSL Lightness Default state 

hue

uint16_t hue
The value of Light HSL Hue Default state 

saturation

uint16_t saturation
The value of Light HSL Saturation Default state 



```

#### esp_ble_mesh_state_change_light_hsl_range_set_t


```


struct esp_ble_mesh_state_change_light_hsl_range_set_t
Parameters of Light HSL Range state change event 

Public Members
hue_range_min

uint16_t hue_range_min
The minimum hue value of Light HSL Range state 

hue_range_max

uint16_t hue_range_max
The maximum hue value of Light HSL Range state 

saturation_range_min

uint16_t saturation_range_min
The minimum saturation value of Light HSL Range state 

saturation_range_max

uint16_t saturation_range_max
The maximum saturation value of Light HSL Range state 



```

#### esp_ble_mesh_state_change_light_xyl_set_t


```


struct esp_ble_mesh_state_change_light_xyl_set_t
Parameters of Light xyL state change event 

Public Members
lightness

uint16_t lightness
The value of Light xyL Lightness state 

x

uint16_t x
The value of Light xyL x state 

y

uint16_t y
The value of Light xyL y state 



```

#### esp_ble_mesh_state_change_light_xyl_default_set_t


```


struct esp_ble_mesh_state_change_light_xyl_default_set_t
Parameters of Light xyL Default state change event 

Public Members
lightness

uint16_t lightness
The value of Light Lightness Default state 

x

uint16_t x
The value of Light xyL x Default state 

y

uint16_t y
The value of Light xyL y Default state 



```

#### esp_ble_mesh_state_change_light_xyl_range_set_t


```


struct esp_ble_mesh_state_change_light_xyl_range_set_t
Parameters of Light xyL Range state change event 

Public Members
x_range_min

uint16_t x_range_min
The minimum value of Light xyL x Range state 

x_range_max

uint16_t x_range_max
The maximum value of Light xyL x Range state 

y_range_min

uint16_t y_range_min
The minimum value of Light xyL y Range state 

y_range_max

uint16_t y_range_max
The maximum value of Light xyL y Range state 



```

#### esp_ble_mesh_state_change_light_lc_mode_set_t


```


struct esp_ble_mesh_state_change_light_lc_mode_set_t
Parameter of Light LC Mode state change event 

Public Members
mode

uint8_t mode
The value of Light LC Mode state 



```

#### esp_ble_mesh_state_change_light_lc_om_set_t


```


struct esp_ble_mesh_state_change_light_lc_om_set_t
Parameter of Light LC Occupancy Mode state change event 

Public Members
mode

uint8_t mode
The value of Light LC Occupancy Mode state 



```

#### esp_ble_mesh_state_change_light_lc_light_onoff_set_t


```


struct esp_ble_mesh_state_change_light_lc_light_onoff_set_t
Parameter of Light LC Light OnOff state change event 

Public Members
onoff

uint8_t onoff
The value of Light LC Light OnOff state 



```

#### esp_ble_mesh_state_change_light_lc_property_set_t


```


struct esp_ble_mesh_state_change_light_lc_property_set_t
Parameters of Light LC Property state change event 

Public Members
property_id

uint16_t property_id
The property id of Light LC Property state 

property_value

struct net_buf_simple *property_value
The property value of Light LC Property state 



```

#### esp_ble_mesh_state_change_sensor_status_t


```


struct esp_ble_mesh_state_change_sensor_status_t
Parameters of Sensor Status state change event 

Public Members
property_id

uint16_t property_id
The value of Sensor Property ID 

occupancy

uint8_t occupancy
The value of Light LC Occupancy state 

set_occupancy_to_1_delay

uint32_t set_occupancy_to_1_delay
The value of Light LC Set Occupancy to 1 Delay state 

ambient_luxlevel

uint32_t ambient_luxlevel
The value of Light LC Ambient Luxlevel state 

state

union esp_ble_mesh_state_change_sensor_status_t::[anonymous] state
Parameters of Sensor Status related state 



```

#### esp_ble_mesh_server_recv_light_lc_property_get_t


```


struct esp_ble_mesh_server_recv_light_lc_property_get_t
Context of the received Light LC Property Get message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Light LC Property 



```

#### esp_ble_mesh_server_recv_light_lightness_set_t


```


struct esp_ble_mesh_server_recv_light_lightness_set_t
Context of the received Light Lightness Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

lightness

uint16_t lightness
Target value of light lightness actual state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_lightness_linear_set_t


```


struct esp_ble_mesh_server_recv_light_lightness_linear_set_t
Context of the received Light Lightness Linear Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

lightness

uint16_t lightness
Target value of light lightness linear state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_lightness_default_set_t


```


struct esp_ble_mesh_server_recv_light_lightness_default_set_t
Context of the received Light Lightness Default Set message 

Public Members
lightness

uint16_t lightness
The value of the Light Lightness Default state 



```

#### esp_ble_mesh_server_recv_light_lightness_range_set_t


```


struct esp_ble_mesh_server_recv_light_lightness_range_set_t
Context of the received Light Lightness Range Set message 

Public Members
range_min

uint16_t range_min
Value of range min field of light lightness range state 

range_max

uint16_t range_max
Value of range max field of light lightness range state 



```

#### esp_ble_mesh_server_recv_light_ctl_set_t


```


struct esp_ble_mesh_server_recv_light_ctl_set_t
Context of the received Light CTL Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

lightness

uint16_t lightness
Target value of light ctl lightness state 

temperature

uint16_t temperature
Target value of light ctl temperature state 

delta_uv

int16_t delta_uv
Target value of light ctl delta UV state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_ctl_temperature_set_t


```


struct esp_ble_mesh_server_recv_light_ctl_temperature_set_t
Context of the received Light CTL Temperature Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

temperature

uint16_t temperature
Target value of light ctl temperature state 

delta_uv

int16_t delta_uv
Target value of light ctl delta UV state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_ctl_temperature_range_set_t


```


struct esp_ble_mesh_server_recv_light_ctl_temperature_range_set_t
Context of the received Light CTL Temperature Range Set message 

Public Members
range_min

uint16_t range_min
Value of temperature range min field of light ctl temperature range state 

range_max

uint16_t range_max
Value of temperature range max field of light ctl temperature range state 



```

#### esp_ble_mesh_server_recv_light_ctl_default_set_t


```


struct esp_ble_mesh_server_recv_light_ctl_default_set_t
Context of the received Light CTL Default Set message 

Public Members
lightness

uint16_t lightness
Value of light lightness default state 

temperature

uint16_t temperature
Value of light temperature default state 

delta_uv

int16_t delta_uv
Value of light delta UV default state 



```

#### esp_ble_mesh_server_recv_light_hsl_set_t


```


struct esp_ble_mesh_server_recv_light_hsl_set_t
Context of the received Light HSL Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

lightness

uint16_t lightness
Target value of light hsl lightness state 

hue

uint16_t hue
Target value of light hsl hue state 

saturation

uint16_t saturation
Target value of light hsl saturation state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_hsl_hue_set_t


```


struct esp_ble_mesh_server_recv_light_hsl_hue_set_t
Context of the received Light HSL Hue Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

hue

uint16_t hue
Target value of light hsl hue state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_hsl_saturation_set_t


```


struct esp_ble_mesh_server_recv_light_hsl_saturation_set_t
Context of the received Light HSL Saturation Set message 

Public Members
op_en

bool op_en
Indicate if optional parameters are included 

saturation

uint16_t saturation
Target value of light hsl hue state 

tid

uint8_t tid
Transaction ID 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_hsl_default_set_t


```


struct esp_ble_mesh_server_recv_light_hsl_default_set_t
Context of the received Light HSL Default Set message 

Public Members
lightness

uint16_t lightness
Value of light lightness default state 

hue

uint16_t hue
Value of light hue default state 

saturation

uint16_t saturation
Value of light saturation default state 



```

#### esp_ble_mesh_server_recv_light_hsl_range_set_t


```


struct esp_ble_mesh_server_recv_light_hsl_range_set_t
Context of the received Light HSL Range Set message 

Public Members
hue_range_min

uint16_t hue_range_min
Value of hue range min field of light hsl hue range state 

hue_range_max

uint16_t hue_range_max
Value of hue range max field of light hsl hue range state 

saturation_range_min

uint16_t saturation_range_min
Value of saturation range min field of light hsl saturation range state 

saturation_range_max

uint16_t saturation_range_max
Value of saturation range max field of light hsl saturation range state 



```

#### esp_ble_mesh_server_recv_light_xyl_set_t


```


struct esp_ble_mesh_server_recv_light_xyl_set_t
Context of the received Light xyL Set message 

Public Members
op_en

bool op_en
Indicate whether optional parameters included 

lightness

uint16_t lightness
The target value of the Light xyL Lightness state 

x

uint16_t x
The target value of the Light xyL x state 

y

uint16_t y
The target value of the Light xyL y state 

tid

uint8_t tid
Transaction Identifier 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_xyl_default_set_t


```


struct esp_ble_mesh_server_recv_light_xyl_default_set_t
Context of the received Light xyL Default Set message 

Public Members
lightness

uint16_t lightness
The value of the Light Lightness Default state 

x

uint16_t x
The value of the Light xyL x Default state 

y

uint16_t y
The value of the Light xyL y Default state 



```

#### esp_ble_mesh_server_recv_light_xyl_range_set_t


```


struct esp_ble_mesh_server_recv_light_xyl_range_set_t
Context of the received Light xyl Range Set message 

Public Members
x_range_min

uint16_t x_range_min
The value of the xyL x Range Min field of the Light xyL x Range state 

x_range_max

uint16_t x_range_max
The value of the xyL x Range Max field of the Light xyL x Range state 

y_range_min

uint16_t y_range_min
The value of the xyL y Range Min field of the Light xyL y Range state 

y_range_max

uint16_t y_range_max
The value of the xyL y Range Max field of the Light xyL y Range state 



```

#### esp_ble_mesh_server_recv_light_lc_mode_set_t


```


struct esp_ble_mesh_server_recv_light_lc_mode_set_t
Context of the received Light LC Mode Set message 

Public Members
mode

uint8_t mode
The target value of the Light LC Mode state 



```

#### esp_ble_mesh_server_recv_light_lc_om_set_t


```


struct esp_ble_mesh_server_recv_light_lc_om_set_t
Context of the received Light OM Set message 

Public Members
mode

uint8_t mode
The target value of the Light LC Occupancy Mode state 



```

#### esp_ble_mesh_server_recv_light_lc_light_onoff_set_t


```


struct esp_ble_mesh_server_recv_light_lc_light_onoff_set_t
Context of the received Light LC Light OnOff Set message 

Public Members
op_en

bool op_en
Indicate whether optional parameters included 

light_onoff

uint8_t light_onoff
The target value of the Light LC Light OnOff state 

tid

uint8_t tid
Transaction Identifier 

trans_time

uint8_t trans_time
Time to complete state transition (optional) 

delay

uint8_t delay
Indicate message execution delay (C.1) 



```

#### esp_ble_mesh_server_recv_light_lc_property_set_t


```


struct esp_ble_mesh_server_recv_light_lc_property_set_t
Context of the received Light LC Property Set message 

Public Members
property_id

uint16_t property_id
Property ID identifying a Light LC Property 

property_value

struct net_buf_simple *property_value
Raw value for the Light LC Property 



```

#### esp_ble_mesh_server_recv_sensor_status_t


```


struct esp_ble_mesh_server_recv_sensor_status_t
Context of the received Sensor Status message 

Public Members
data

struct net_buf_simple *data
Value of sensor data state (optional) 



```

#### esp_ble_mesh_lighting_server_cb_param_t


```


struct esp_ble_mesh_lighting_server_cb_param_t
Lighting Server Model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Lighting Server Models 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received messages 

value

esp_ble_mesh_lighting_server_cb_value_t value
Value of the received Lighting Messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_CLI


```


ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_CLI(cli_pub, cli_data)
Define a new Light Lightness Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Light Lightness Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Light Lightness Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_CTL_CLI


```


ESP_BLE_MESH_MODEL_LIGHT_CTL_CLI(cli_pub, cli_data)
Define a new Light CTL Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Light CTL Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Light CTL Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_HSL_CLI


```


ESP_BLE_MESH_MODEL_LIGHT_HSL_CLI(cli_pub, cli_data)
Define a new Light HSL Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Light HSL Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Light HSL Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_XYL_CLI


```


ESP_BLE_MESH_MODEL_LIGHT_XYL_CLI(cli_pub, cli_data)
Define a new Light xyL Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Light xyL Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Light xyL Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_LC_CLI


```


ESP_BLE_MESH_MODEL_LIGHT_LC_CLI(cli_pub, cli_data)
Define a new Light LC Client Model. 

Note
This API needs to be called for each element on which the application needs to have a Light LC Client Model.


Parameters

cli_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
cli_data -- Pointer to the unique struct esp_ble_mesh_client_t.


Returns
New Light LC Client Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_SRV(srv_pub, srv_data)
Lighting Server Models related context. 
Define a new Light Lightness Server Model.

Note
1. The Light Lightness Server model extends the Generic Power OnOff Server model and the Generic Level Server model. When this model is present on an Element, the corresponding Light Lightness Setup Server model shall also be present.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_lightness_srv_t.


Returns
New Light Lightness Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_SETUP_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_SETUP_SRV(srv_pub, srv_data)
Define a new Light Lightness Setup Server Model. 

Note
1. The Light Lightness Setup Server model extends the Light Lightness Server model and the Generic Power OnOff Setup Server model.
This model shall support model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_lightness_setup_srv_t.


Returns
New Light Lightness Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_CTL_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_CTL_SRV(srv_pub, srv_data)
Define a new Light CTL Server Model. 

Note
1. The Light CTL Server model extends the Light Lightness Server model. When this model is present on an Element, the corresponding Light CTL Temperature Server model and the corresponding Light CTL Setup Server model shall also be present.
This model shall support model publication and model subscription.
The model requires two elements: the main element and the Temperature element. The Temperature element contains the corresponding Light CTL Temperature Server model and an instance of a Generic Level state bound to the Light CTL Temperature state on the Temperature element. The Light CTL Temperature state on the Temperature element is bound to the Light CTL state on the main element.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_ctl_srv_t.


Returns
New Light CTL Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_CTL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_CTL_SETUP_SRV(srv_pub, srv_data)
Define a new Light CTL Setup Server Model. 

Note
1. The Light CTL Setup Server model extends the Light CTL Server and the Light Lightness Setup Server.
This model shall support model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_ctl_setup_srv_t.


Returns
New Light CTL Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_CTL_TEMP_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_CTL_TEMP_SRV(srv_pub, srv_data)
Define a new Light CTL Temperature Server Model. 

Note
1. The Light CTL Temperature Server model extends the Generic Level Server model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_ctl_temp_srv_t.


Returns
New Light CTL Temperature Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_HSL_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_HSL_SRV(srv_pub, srv_data)
Define a new Light HSL Server Model. 

Note
1. The Light HSL Server model extends the Light Lightness Server model. When this model is present on an Element, the corresponding Light HSL Hue Server model and the corresponding Light HSL Saturation Server model and the corresponding Light HSL Setup Server model shall also be present.
This model shall support model publication and model subscription.
The model requires three elements: the main element and the Hue element and the Saturation element. The Hue element contains the corresponding Light HSL Hue Server model and an instance of a Generic Level state bound to the Light HSL Hue state on the Hue element. The Saturation element contains the corresponding Light HSL Saturation Server model and an instance of a Generic Level state bound to the Light HSL Saturation state on the Saturation element. The Light HSL Hue state on the Hue element is bound to the Light HSL state on the main element and the Light HSL Saturation state on the Saturation element is bound to the Light HSL state on the main element.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_hsl_srv_t.


Returns
New Light HSL Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_HSL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_HSL_SETUP_SRV(srv_pub, srv_data)
Define a new Light HSL Setup Server Model. 

Note
1. The Light HSL Setup Server model extends the Light HSL Server and the Light Lightness Setup Server.
This model shall support model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_hsl_setup_srv_t.


Returns
New Light HSL Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_HSL_HUE_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_HSL_HUE_SRV(srv_pub, srv_data)
Define a new Light HSL Hue Server Model. 

Note
1. The Light HSL Hue Server model extends the Generic Level Server model. This model is associated with the Light HSL Server model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_hsl_hue_srv_t.


Returns
New Light HSL Hue Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_HSL_SAT_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_HSL_SAT_SRV(srv_pub, srv_data)
Define a new Light HSL Saturation Server Model. 

Note
1. The Light HSL Saturation Server model extends the Generic Level Server model. This model is associated with the Light HSL Server model.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_hsl_sat_srv_t.


Returns
New Light HSL Saturation Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_XYL_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_XYL_SRV(srv_pub, srv_data)
Define a new Light xyL Server Model. 

Note
1. The Light xyL Server model extends the Light Lightness Server model. When this model is present on an Element, the corresponding Light xyL Setup Server model shall also be present.
This model shall support model publication and model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_xyl_srv_t.


Returns
New Light xyL Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_XYL_SETUP_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_XYL_SETUP_SRV(srv_pub, srv_data)
Define a new Light xyL Setup Server Model. 

Note
1. The Light xyL Setup Server model extends the Light xyL Server and the Light Lightness Setup Server.
This model shall support model subscription.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_xyl_setup_srv_t.


Returns
New Light xyL Setup Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_LC_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_LC_SRV(srv_pub, srv_data)
Define a new Light LC Server Model. 

Note
1. The Light LC (Lightness Control) Server model extends the Light Lightness Server model and the Generic OnOff Server model. When this model is present on an Element, the corresponding Light LC Setup Server model shall also be present.
This model shall support model publication and model subscription.
This model may be used to represent an element that is a client to a Sensor Server model and controls the Light Lightness Actual state via defined state bindings.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_lc_srv_t.


Returns
New Light LC Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_LIGHT_LC_SETUP_SRV


```


ESP_BLE_MESH_MODEL_LIGHT_LC_SETUP_SRV(srv_pub, srv_data)
Define a new Light LC Setup Server Model. 

Note
1. The Light LC (Lightness Control) Setup model extends the Light LC Server model.
This model shall support model publication and model subscription.
This model may be used to configure setup parameters for the Light LC Server model.




Parameters

srv_pub -- Pointer to the unique struct esp_ble_mesh_model_pub_t. 
srv_data -- Pointer to the unique struct esp_ble_mesh_light_lc_setup_srv_t.


Returns
New Light LC Setup Server Model instance. 



```



### Type Definitions


#### esp_ble_mesh_light_client_cb_t


```


typedef void (*esp_ble_mesh_light_client_cb_t)(esp_ble_mesh_light_client_cb_event_t event, esp_ble_mesh_light_client_cb_param_t *param)
Bluetooth Mesh Light Client Model function. 
Lighting Client Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_lighting_server_cb_t


```


typedef void (*esp_ble_mesh_lighting_server_cb_t)(esp_ble_mesh_lighting_server_cb_event_t event, esp_ble_mesh_lighting_server_cb_param_t *param)
Bluetooth Mesh Lighting Server Model function. 
Lighting Server Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_light_client_cb_event_t


```


enum esp_ble_mesh_light_client_cb_event_t
This enum value is the event of Lighting Client Model 
Values:
ESP_BLE_MESH_LIGHT_CLIENT_GET_STATE_EVT

enumerator ESP_BLE_MESH_LIGHT_CLIENT_GET_STATE_EVT

ESP_BLE_MESH_LIGHT_CLIENT_SET_STATE_EVT

enumerator ESP_BLE_MESH_LIGHT_CLIENT_SET_STATE_EVT

ESP_BLE_MESH_LIGHT_CLIENT_PUBLISH_EVT

enumerator ESP_BLE_MESH_LIGHT_CLIENT_PUBLISH_EVT

ESP_BLE_MESH_LIGHT_CLIENT_TIMEOUT_EVT

enumerator ESP_BLE_MESH_LIGHT_CLIENT_TIMEOUT_EVT

ESP_BLE_MESH_LIGHT_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_LIGHT_CLIENT_EVT_MAX


```

#### esp_ble_mesh_lc_state_t


```


enum esp_ble_mesh_lc_state_t
This enum value is the Light LC State Machine states 
Values:
ESP_BLE_MESH_LC_OFF

enumerator ESP_BLE_MESH_LC_OFF

ESP_BLE_MESH_LC_STANDBY

enumerator ESP_BLE_MESH_LC_STANDBY

ESP_BLE_MESH_LC_FADE_ON

enumerator ESP_BLE_MESH_LC_FADE_ON

ESP_BLE_MESH_LC_RUN

enumerator ESP_BLE_MESH_LC_RUN

ESP_BLE_MESH_LC_FADE

enumerator ESP_BLE_MESH_LC_FADE

ESP_BLE_MESH_LC_PROLONG

enumerator ESP_BLE_MESH_LC_PROLONG

ESP_BLE_MESH_LC_FADE_STANDBY_AUTO

enumerator ESP_BLE_MESH_LC_FADE_STANDBY_AUTO

ESP_BLE_MESH_LC_FADE_STANDBY_MANUAL

enumerator ESP_BLE_MESH_LC_FADE_STANDBY_MANUAL


```

#### esp_ble_mesh_lighting_server_cb_event_t


```


enum esp_ble_mesh_lighting_server_cb_event_t
This enum value is the event of Lighting Server Model 
Values:
ESP_BLE_MESH_LIGHTING_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_LIGHTING_SERVER_STATE_CHANGE_EVT

When get_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, no event will be callback to the application layer when Lighting Get messages are received.
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_AUTO_RSP, this event will be callback to the application layer when Lighting Set/Set Unack messages are received. 



ESP_BLE_MESH_LIGHTING_SERVER_RECV_GET_MSG_EVT

enumerator ESP_BLE_MESH_LIGHTING_SERVER_RECV_GET_MSG_EVT
When get_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Lighting Get messages are received. 

ESP_BLE_MESH_LIGHTING_SERVER_RECV_SET_MSG_EVT

enumerator ESP_BLE_MESH_LIGHTING_SERVER_RECV_SET_MSG_EVT
When set_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Lighting Set/Set Unack messages are received. 

ESP_BLE_MESH_LIGHTING_SERVER_RECV_STATUS_MSG_EVT

enumerator ESP_BLE_MESH_LIGHTING_SERVER_RECV_STATUS_MSG_EVT
When status_auto_rsp is set to ESP_BLE_MESH_SERVER_RSP_BY_APP, this event will be callback to the application layer when Sensor Status message is received. 

ESP_BLE_MESH_LIGHTING_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_LIGHTING_SERVER_EVT_MAX


```




## ESP-BLE-MESH (v1.1) Core API Reference



Note


This section is a preview version, so the related structures, macros, and APIs may be changed.



This section contains ESP-BLE-MESH v1.1 Core related APIs, event types, event parameters, etc.


This API reference covers 10 components:


* Remote Provisioning
* Directed Forwarding
* Subnet Bridge Configuration
* Mesh Private Beacon
* On-Demand Private Proxy
* SAR Configuration
* Solicitation PDU RPL Configuration
* Opcodes Aggregator
* Large Composition Data
* Composition and Metadata



### Remote Provisioning




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_rpr_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_rpr_model_api.h"
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


#### esp_ble_mesh_register_rpr_client_callback


```


esp_err_t esp_ble_mesh_register_rpr_client_callback(esp_ble_mesh_rpr_client_cb_t callback)
Register BLE Mesh Remote Provisioning Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_rpr_client_send


```


esp_err_t esp_ble_mesh_rpr_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_rpr_client_msg_t *msg)
Get the value of Remote Provisioning Server model state with the corresponding get message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to Remote Provisioning Client message.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_rpr_client_action


```


esp_err_t esp_ble_mesh_rpr_client_action(esp_ble_mesh_rpr_client_act_type_t type, esp_ble_mesh_rpr_client_act_param_t *param)
Remote Provisioning Client model perform related actions, e.g. start remote provisioning. 

Parameters

type -- [in] Type of the action to be performed. 
param -- [in] Parameters of the action to be performed.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_rpr_server_callback


```


esp_err_t esp_ble_mesh_register_rpr_server_callback(esp_ble_mesh_rpr_server_cb_t callback)
Register BLE Mesh Remote Provisioning Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_rpr_client_msg_t


```


union esp_ble_mesh_rpr_client_msg_t

#include <esp_ble_mesh_rpr_model_api.h>
Remote Provisioning Client model message union. 

Public Members
scan_start

esp_ble_mesh_rpr_scan_start_t scan_start
For ESP_BLE_MESH_MODEL_OP_RPR_SCAN_START 

ext_scan_start

esp_ble_mesh_rpr_ext_scan_start_t ext_scan_start
For ESP_BLE_MESH_MODEL_OP_RPR_EXT_SCAN_START 

link_open

esp_ble_mesh_rpr_link_open_t link_open
For ESP_BLE_MESH_MODEL_OP_RPR_LINK_OPEN 

link_close

esp_ble_mesh_rpr_link_close_t link_close
For ESP_BLE_MESH_MODEL_OP_RPR_LINK_CLOSE 



```

#### esp_ble_mesh_rpr_client_act_param_t


```


union esp_ble_mesh_rpr_client_act_param_t

#include <esp_ble_mesh_rpr_model_api.h>
Remote Provisioning Client model action union. 

Public Members
start_rpr

esp_ble_mesh_rpr_client_start_rpr_t start_rpr
Start remote provisioning 



```

#### esp_ble_mesh_rpr_client_recv_cb_t


```


union esp_ble_mesh_rpr_client_recv_cb_t

#include <esp_ble_mesh_rpr_model_api.h>
Remote Provisioning Client model received message union. 

Public Members
scan_caps_status

esp_ble_mesh_rpr_scan_caps_status_t scan_caps_status
For ESP_BLE_MESH_MODEL_OP_RPR_SCAN_CAPS_STATUS 

scan_status

esp_ble_mesh_rpr_scan_status_t scan_status
For ESP_BLE_MESH_MODEL_OP_RPR_SCAN_STATUS 

scan_report

esp_ble_mesh_rpr_scan_report_t scan_report
For ESP_BLE_MESH_MODEL_OP_RPR_SCAN_REPORT 

ext_scan_report

esp_ble_mesh_rpr_ext_scan_report_t ext_scan_report
For ESP_BLE_MESH_MODEL_OP_RPR_EXT_SCAN_REPORT 

link_status

esp_ble_mesh_rpr_link_status_t link_status
For ESP_BLE_MESH_MODEL_OP_RPR_LINK_STATUS 

link_report

esp_ble_mesh_rpr_link_report_t link_report
For ESP_BLE_MESH_MODEL_OP_RPR_LINK_REPORT 



```

#### esp_ble_mesh_rpr_client_cb_param_t


```


union esp_ble_mesh_rpr_client_cb_param_t

#include <esp_ble_mesh_rpr_model_api.h>
Remote Provisioning Client model callback parameters 

Public Members
err_code

int err_code
Result of sending a message
Result of starting remote provisioning 

params

esp_ble_mesh_client_common_param_t *params
Client common parameters 

send

struct esp_ble_mesh_rpr_client_cb_param_t::[anonymous] send
Event parameters of sending messages Event parameters of sending messages 

val

esp_ble_mesh_rpr_client_recv_cb_t val
Parameters of received status message 

recv

struct esp_ble_mesh_rpr_client_cb_param_t::[anonymous] recv
Event parameters of receiving messages Event parameters of receiving messages 

sub_evt

esp_ble_mesh_rpr_client_act_evt_t sub_evt
Event type of the performed action 

model

esp_ble_mesh_model_t *model
Pointer of Remote Provisioning Client 

rpr_srv_addr

uint16_t rpr_srv_addr
Unicast address of Remote Provisioning Server 

start_rpr_comp

struct esp_ble_mesh_rpr_client_cb_param_t::[anonymous]::[anonymous] start_rpr_comp
ESP_BLE_MESH_START_RPR_COMP_SUB_EVT. 
Event parameter of ESP_BLE_MESH_START_RPR_COMP_SUB_EVT 

act

struct esp_ble_mesh_rpr_client_cb_param_t::[anonymous] act
Event parameters of performed actions Event parameters of performed actions 

link_open

struct esp_ble_mesh_rpr_client_cb_param_t::[anonymous] link_open
ESP_BLE_MESH_RPR_CLIENT_LINK_OPEN_EVT. 
Event parameters of ESP_BLE_MESH_RPR_CLIENT_LINK_OPEN_EVT 

reason

uint8_t reason
Reason of closing provisioning link 

link_close

struct esp_ble_mesh_rpr_client_cb_param_t::[anonymous] link_close
ESP_BLE_MESH_RPR_CLIENT_LINK_CLOSE_EVT. 
Event parameters of ESP_BLE_MESH_RPR_CLIENT_LINK_CLOSE_EVT 

nppi

uint8_t nppi
NPPI Procedure 

index

uint16_t index
Index of the provisioned node 

uuid

uint8_t uuid[16]
Device UUID 

unicast_addr

uint16_t unicast_addr
Primary element address 

element_num

uint8_t element_num
Element number 

net_idx

uint16_t net_idx
NetKey Index 

prov

struct esp_ble_mesh_rpr_client_cb_param_t::[anonymous] prov
ESP_BLE_MESH_RPR_CLIENT_PROV_COMP_EVT. 
Event parameters of ESP_BLE_MESH_RPR_CLIENT_PROV_COMP_EVT 



```

#### esp_ble_mesh_rpr_server_cb_param_t


```


union esp_ble_mesh_rpr_server_cb_param_t

#include <esp_ble_mesh_rpr_model_api.h>
Remote Provisioning Server model related context. 
Remote Provisioning Server model callback value union 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

scan_items_limit

uint8_t scan_items_limit
Maximum number of scanned items to be reported 

timeout

uint8_t timeout
Time limit for a scan (in seconds)
Time limit for extended scan (in seconds)
Time limit for opening a link (in seconds) 

uuid

uint8_t uuid[16]
Device UUID (All ZERO if not present)
Device UUID (ZERO if not present) 

net_idx

uint16_t net_idx
NetKey Index used by Remote Provisioning Client 

rpr_cli_addr

uint16_t rpr_cli_addr
Unicast address of Remote Provisioning Client 

scan_start

struct esp_ble_mesh_rpr_server_cb_param_t::[anonymous] scan_start
ESP_BLE_MESH_RPR_SERVER_SCAN_START_EVT. 

scan_stop

struct esp_ble_mesh_rpr_server_cb_param_t::[anonymous] scan_stop
ESP_BLE_MESH_RPR_SERVER_SCAN_STOP_EVT. 

ad_type_filter_count

uint8_t ad_type_filter_count
Number of AD Types in the ADTypeFilter field 

ad_type_filter

uint8_t *ad_type_filter
List of AD Types to be reported 

index

uint8_t index
Index of the extended scan instance 

ext_scan_start

struct esp_ble_mesh_rpr_server_cb_param_t::[anonymous] ext_scan_start
ESP_BLE_MESH_RPR_SERVER_EXT_SCAN_START_EVT. 

ext_scan_stop

struct esp_ble_mesh_rpr_server_cb_param_t::[anonymous] ext_scan_stop
ESP_BLE_MESH_RPR_SERVER_EXT_SCAN_STOP_EVT. 

status

uint8_t status
Status of Link Open procedure 

nppi

uint8_t nppi
Node Provisioning Protocol Interface
Provisioning bearer link close reason code 

link_open

struct esp_ble_mesh_rpr_server_cb_param_t::[anonymous] link_open
ESP_BLE_MESH_RPR_SERVER_LINK_OPEN_EVT. 

close_by_device

bool close_by_device
Indicate if the link is closed by the Unprovisioned Device 

reason

uint8_t reason
Provisioning bearer link close reason code 

link_close

struct esp_ble_mesh_rpr_server_cb_param_t::[anonymous] link_close
ESP_BLE_MESH_RPR_SERVER_LINK_CLOSE_EVT. 

prov_comp

struct esp_ble_mesh_rpr_server_cb_param_t::[anonymous] prov_comp
ESP_BLE_MESH_RPR_SERVER_PROV_COMP_EVT. TODO: Duplicate with Link Close event? 



```



### Structures


#### esp_ble_mesh_rpr_scan_start_t


```


struct esp_ble_mesh_rpr_scan_start_t
Remote Provisioning Server model context Parameters of Remote Provisioning Scan Start 

Public Members
scan_items_limit

uint8_t scan_items_limit
Maximum number of scanned items to be reported 

timeout

uint8_t timeout
Time limit for a scan (in seconds) 

uuid_en

bool uuid_en
Indicate if Device UUID is present 

uuid

uint8_t uuid[16]
Device UUID (Optional) 



```

#### esp_ble_mesh_rpr_ext_scan_start_t


```


struct esp_ble_mesh_rpr_ext_scan_start_t
Parameters of Remote Provisioning Extended Scan Start 

Public Members
ad_type_filter_count

uint8_t ad_type_filter_count
Number of AD Types in the ADTypeFilter field 

ad_type_filter

uint8_t ad_type_filter[16]
List of AD Types to be reported. Minimum is 1, maximum is 16 

uuid_en

bool uuid_en
Indicate if Device UUID is present 

uuid

uint8_t uuid[16]
Device UUID (Optional) 

timeout

uint8_t timeout
Time limit for a scan (in seconds) (C.1) 



```

#### esp_ble_mesh_rpr_link_open_t


```


struct esp_ble_mesh_rpr_link_open_t
Parameters of Remote Provisioning Link Open 

Public Members
uuid_en

bool uuid_en
Indicate if Device UUID is present 

uuid

uint8_t uuid[16]
Device UUID (Optional) 

timeout_en

bool timeout_en
Indicate if Link open timeout is present 

timeout

uint8_t timeout
Link open timeout in seconds (C.1) 

nppi

uint8_t nppi
Node Provisioning Protocol Interface (C.2) 



```

#### esp_ble_mesh_rpr_link_close_t


```


struct esp_ble_mesh_rpr_link_close_t
Parameters of Remote Provisioning Link Close 

Public Members
reason

uint8_t reason
Provisioning bearer link close reason code 



```

#### esp_ble_mesh_rpr_client_start_rpr_t


```


struct esp_ble_mesh_rpr_client_start_rpr_t
Parameters of starting remote provisioning 

Public Members
model

esp_ble_mesh_model_t *model
Pointer of Remote Provisioning Client 

rpr_srv_addr

uint16_t rpr_srv_addr
Unicast address of Remote Provisioning Server 



```

#### esp_ble_mesh_rpr_scan_caps_status_t


```


struct esp_ble_mesh_rpr_scan_caps_status_t
Parameters of Remote Provisioning Scan Capabilities Status 

Public Members
max_scan_items

uint8_t max_scan_items
The maximum number of UUIDs that can be reported during scanning 

active_scan

uint8_t active_scan
Indication if active scan is supported 



```

#### esp_ble_mesh_rpr_scan_status_t


```


struct esp_ble_mesh_rpr_scan_status_t
Parameters of Remote Provisioning Scan Status 

Public Members
status

uint8_t status
Status for the requesting message 

rpr_scanning

uint8_t rpr_scanning
The Remote Provisioning Scan state value 

scan_items_limit

uint8_t scan_items_limit
Maximum number of scanned items to be reported 

timeout

uint8_t timeout
Time limit for a scan (in seconds) 



```

#### esp_ble_mesh_rpr_scan_report_t


```


struct esp_ble_mesh_rpr_scan_report_t
Parameters of Remote Provisioning Scan Report 

Public Members
rssi

int8_t rssi
An indication of received signal strength measured in dBm 

uuid

uint8_t uuid[16]
Device UUID 

oob_info

uint16_t oob_info
OOB information 

uri_hash

uint32_t uri_hash
URI Hash (Optional) 



```

#### esp_ble_mesh_rpr_ext_scan_report_t


```


struct esp_ble_mesh_rpr_ext_scan_report_t
Parameters of Remote Provisioning Extended Scan Report 

Public Members
status

uint8_t status
Status for the requesting message 

uuid

uint8_t uuid[16]
Device UUID 

oob_info_en

bool oob_info_en
Indicate if OOB Information is present 

oob_info

uint16_t oob_info
OOB Information (Optional) 

adv_structures

struct net_buf_simple *adv_structures
Concatenated list of AD Structures (C.1) 



```

#### esp_ble_mesh_rpr_link_status_t


```


struct esp_ble_mesh_rpr_link_status_t
Parameters of Remote Provisioning Link Status 

Public Members
status

uint8_t status
Status for the requesting message 

rpr_state

uint8_t rpr_state
Remote Provisioning Link state 



```

#### esp_ble_mesh_rpr_link_report_t


```


struct esp_ble_mesh_rpr_link_report_t
Parameters of Remote Provisioning Link Report 

Public Members
status

uint8_t status
Status of the provisioning bearer or the NPPI 

rpr_state

uint8_t rpr_state
Remote Provisioning Link state 

reason_en

bool reason_en
Indicate if Link close Reason code is present 

reason

uint8_t reason
Link close Reason code (Optional) 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_RPR_SCAN_CAPS_GET


```


ESP_BLE_MESH_MODEL_OP_RPR_SCAN_CAPS_GET

```

#### ESP_BLE_MESH_MODEL_OP_RPR_SCAN_CAPS_STATUS


```


ESP_BLE_MESH_MODEL_OP_RPR_SCAN_CAPS_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_RPR_SCAN_GET


```


ESP_BLE_MESH_MODEL_OP_RPR_SCAN_GET

```

#### ESP_BLE_MESH_MODEL_OP_RPR_SCAN_START


```


ESP_BLE_MESH_MODEL_OP_RPR_SCAN_START

```

#### ESP_BLE_MESH_MODEL_OP_RPR_SCAN_STOP


```


ESP_BLE_MESH_MODEL_OP_RPR_SCAN_STOP

```

#### ESP_BLE_MESH_MODEL_OP_RPR_SCAN_STATUS


```


ESP_BLE_MESH_MODEL_OP_RPR_SCAN_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_RPR_SCAN_REPORT


```


ESP_BLE_MESH_MODEL_OP_RPR_SCAN_REPORT

```

#### ESP_BLE_MESH_MODEL_OP_RPR_EXT_SCAN_START


```


ESP_BLE_MESH_MODEL_OP_RPR_EXT_SCAN_START

```

#### ESP_BLE_MESH_MODEL_OP_RPR_EXT_SCAN_REPORT


```


ESP_BLE_MESH_MODEL_OP_RPR_EXT_SCAN_REPORT

```

#### ESP_BLE_MESH_MODEL_OP_RPR_LINK_GET


```


ESP_BLE_MESH_MODEL_OP_RPR_LINK_GET

```

#### ESP_BLE_MESH_MODEL_OP_RPR_LINK_OPEN


```


ESP_BLE_MESH_MODEL_OP_RPR_LINK_OPEN

```

#### ESP_BLE_MESH_MODEL_OP_RPR_LINK_CLOSE


```


ESP_BLE_MESH_MODEL_OP_RPR_LINK_CLOSE

```

#### ESP_BLE_MESH_MODEL_OP_RPR_LINK_STATUS


```


ESP_BLE_MESH_MODEL_OP_RPR_LINK_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_RPR_LINK_REPORT


```


ESP_BLE_MESH_MODEL_OP_RPR_LINK_REPORT

```

#### ESP_BLE_MESH_MODEL_OP_RPR_PDU_SEND


```


ESP_BLE_MESH_MODEL_OP_RPR_PDU_SEND

```

#### ESP_BLE_MESH_MODEL_OP_RPR_PDU_OUTBOUND_REPORT


```


ESP_BLE_MESH_MODEL_OP_RPR_PDU_OUTBOUND_REPORT

```

#### ESP_BLE_MESH_MODEL_OP_RPR_PDU_REPORT


```


ESP_BLE_MESH_MODEL_OP_RPR_PDU_REPORT

```

#### ESP_BLE_MESH_RPR_SRV_MAX_SCANNED_ITEMS_MIN


```


ESP_BLE_MESH_RPR_SRV_MAX_SCANNED_ITEMS_MIN

```

#### ESP_BLE_MESH_RPR_NOT_SUPPORT_ACTIVE_SCAN


```


ESP_BLE_MESH_RPR_NOT_SUPPORT_ACTIVE_SCAN

```

#### ESP_BLE_MESH_RPR_SUPPORT_ACTIVE_SCAN


```


ESP_BLE_MESH_RPR_SUPPORT_ACTIVE_SCAN

```

#### ESP_BLE_MESH_RPR_SCAN_IDLE


```


ESP_BLE_MESH_RPR_SCAN_IDLE

```

#### ESP_BLE_MESH_RPR_SCAN_MULTIPLE_DEVICE


```


ESP_BLE_MESH_RPR_SCAN_MULTIPLE_DEVICE

```

#### ESP_BLE_MESH_RPR_SCAN_SINGLE_DEVICE


```


ESP_BLE_MESH_RPR_SCAN_SINGLE_DEVICE

```

#### ESP_BLE_MESH_RPR_SCAN_NOT_IN_PROGRESS


```


ESP_BLE_MESH_RPR_SCAN_NOT_IN_PROGRESS

```

#### ESP_BLE_MESH_RPR_PROHIBIT_SCAN_TIMEOUT


```


ESP_BLE_MESH_RPR_PROHIBIT_SCAN_TIMEOUT

```

#### ESP_BLE_MESH_RPR_EXT_SCAN_TIMEOUT_MIN


```


ESP_BLE_MESH_RPR_EXT_SCAN_TIMEOUT_MIN

```

#### ESP_BLE_MESH_RPR_EXT_SCAN_TIMEOUT_MAX


```


ESP_BLE_MESH_RPR_EXT_SCAN_TIMEOUT_MAX

```

#### ESP_BLE_MESH_RPR_AD_TYPE_FILTER_CNT_MIN


```


ESP_BLE_MESH_RPR_AD_TYPE_FILTER_CNT_MIN

```

#### ESP_BLE_MESH_RPR_AD_TYPE_FILTER_CNT_MAX


```


ESP_BLE_MESH_RPR_AD_TYPE_FILTER_CNT_MAX

```

#### ESP_BLE_MESH_RPR_LINK_OPEN_TIMEOUT_MIN


```


ESP_BLE_MESH_RPR_LINK_OPEN_TIMEOUT_MIN

```

#### ESP_BLE_MESH_RPR_LINK_OPEN_TIMEOUT_MAX


```


ESP_BLE_MESH_RPR_LINK_OPEN_TIMEOUT_MAX

```

#### ESP_BLE_MESH_RPR_LINK_TIMEOUT_DEFAULT


```


ESP_BLE_MESH_RPR_LINK_TIMEOUT_DEFAULT

```

#### ESP_BLE_MESH_RPR_REASON_SUCCESS


```


ESP_BLE_MESH_RPR_REASON_SUCCESS

```

#### ESP_BLE_MESH_RPR_REASON_FAIL


```


ESP_BLE_MESH_RPR_REASON_FAIL

```

#### ESP_BLE_MESH_RPR_LINK_IDLE


```


ESP_BLE_MESH_RPR_LINK_IDLE

```

#### ESP_BLE_MESH_RPR_LINK_OPENING


```


ESP_BLE_MESH_RPR_LINK_OPENING

```

#### ESP_BLE_MESH_RPR_LINK_ACTIVE


```


ESP_BLE_MESH_RPR_LINK_ACTIVE

```

#### ESP_BLE_MESH_RPR_OUTBOUND_PACKET_TRANSFER


```


ESP_BLE_MESH_RPR_OUTBOUND_PACKET_TRANSFER

```

#### ESP_BLE_MESH_RPR_LINK_CLOSING


```


ESP_BLE_MESH_RPR_LINK_CLOSING

```

#### ESP_BLE_MESH_RPR_STATUS_SUCCESS


```


ESP_BLE_MESH_RPR_STATUS_SUCCESS

```

#### ESP_BLE_MESH_RPR_STATUS_SCANNING_CANNOT_START


```


ESP_BLE_MESH_RPR_STATUS_SCANNING_CANNOT_START

```

#### ESP_BLE_MESH_RPR_STATUS_INVALID_STATE


```


ESP_BLE_MESH_RPR_STATUS_INVALID_STATE

```

#### ESP_BLE_MESH_RPR_STATUS_LIMITED_RESOURCES


```


ESP_BLE_MESH_RPR_STATUS_LIMITED_RESOURCES

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_CANNOT_OPEN


```


ESP_BLE_MESH_RPR_STATUS_LINK_CANNOT_OPEN

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_OPEN_FAILED


```


ESP_BLE_MESH_RPR_STATUS_LINK_OPEN_FAILED

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_BY_DEVICE


```


ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_BY_DEVICE

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_BY_SERVER


```


ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_BY_SERVER

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_BY_CLIENT


```


ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_BY_CLIENT

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_AS_CANNOT_RECEIVE_PDU


```


ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_AS_CANNOT_RECEIVE_PDU

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_AS_CANNOT_SEND_PDU


```


ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_AS_CANNOT_SEND_PDU

```

#### ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_AS_CANNOT_DELIVER_PDU_REPORT


```


ESP_BLE_MESH_RPR_STATUS_LINK_CLOSED_AS_CANNOT_DELIVER_PDU_REPORT

```

#### ESP_BLE_MESH_MODEL_RPR_SRV


```


ESP_BLE_MESH_MODEL_RPR_SRV(srv_data)
Define a new Remote Provisioning Server model. 

Note
If supported, the model shall be supported by a primary element and may be supported by any secondary element.


Parameters

srv_data -- Pointer to a unique Remote Provisioning Server model user_data.


Returns
New Remote Provisioning Server model instance. 



```

#### ESP_BLE_MESH_MODEL_RPR_CLI


```


ESP_BLE_MESH_MODEL_RPR_CLI(cli_data)
Define a new Remote Provisioning Client model. 

Note
If supported, the model shall be supported by a primary element and may be supported by any secondary element.


Parameters

cli_data -- Pointer to a unique Remote Provisioning Client model user_data.


Returns
New Remote Provisioning Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_rpr_client_cb_t


```


typedef void (*esp_ble_mesh_rpr_client_cb_t)(esp_ble_mesh_rpr_client_cb_event_t event, esp_ble_mesh_rpr_client_cb_param_t *param)
Remote Provisioning client and server model functions. 
Remote Provisioning Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_rpr_server_cb_t


```


typedef void (*esp_ble_mesh_rpr_server_cb_t)(esp_ble_mesh_rpr_server_cb_event_t event, esp_ble_mesh_rpr_server_cb_param_t *param)
Remote Provisioning Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_rpr_client_act_type_t


```


enum esp_ble_mesh_rpr_client_act_type_t
This enum value is the action of Remote Provisioning Client model 
Values:
ESP_BLE_MESH_RPR_CLIENT_ACT_START_RPR

enumerator ESP_BLE_MESH_RPR_CLIENT_ACT_START_RPR

ESP_BLE_MESH_RPR_CLIENT_ACT_MAX

enumerator ESP_BLE_MESH_RPR_CLIENT_ACT_MAX


```

#### esp_ble_mesh_rpr_client_act_evt_t


```


enum esp_ble_mesh_rpr_client_act_evt_t
This enum value is the event type of the performed action 
Values:
ESP_BLE_MESH_START_RPR_COMP_SUB_EVT

enumerator ESP_BLE_MESH_START_RPR_COMP_SUB_EVT


```

#### esp_ble_mesh_rpr_client_cb_event_t


```


enum esp_ble_mesh_rpr_client_cb_event_t
This enum value is the event of Remote Provisioning Client model 
Values:
ESP_BLE_MESH_RPR_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_RPR_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_RPR_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_RPR_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_RPR_CLIENT_ACT_COMP_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_ACT_COMP_EVT

ESP_BLE_MESH_RPR_CLIENT_LINK_OPEN_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_LINK_OPEN_EVT

ESP_BLE_MESH_RPR_CLIENT_LINK_CLOSE_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_LINK_CLOSE_EVT

ESP_BLE_MESH_RPR_CLIENT_PROV_COMP_EVT

enumerator ESP_BLE_MESH_RPR_CLIENT_PROV_COMP_EVT

ESP_BLE_MESH_RPR_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_RPR_CLIENT_EVT_MAX


```

#### esp_ble_mesh_rpr_server_cb_event_t


```


enum esp_ble_mesh_rpr_server_cb_event_t
This enum value is the event of Remote Provisioning Server model 
Values:
ESP_BLE_MESH_RPR_SERVER_SCAN_START_EVT

enumerator ESP_BLE_MESH_RPR_SERVER_SCAN_START_EVT

ESP_BLE_MESH_RPR_SERVER_SCAN_STOP_EVT

enumerator ESP_BLE_MESH_RPR_SERVER_SCAN_STOP_EVT

ESP_BLE_MESH_RPR_SERVER_EXT_SCAN_START_EVT

enumerator ESP_BLE_MESH_RPR_SERVER_EXT_SCAN_START_EVT

ESP_BLE_MESH_RPR_SERVER_EXT_SCAN_STOP_EVT

enumerator ESP_BLE_MESH_RPR_SERVER_EXT_SCAN_STOP_EVT

ESP_BLE_MESH_RPR_SERVER_LINK_OPEN_EVT

enumerator ESP_BLE_MESH_RPR_SERVER_LINK_OPEN_EVT

ESP_BLE_MESH_RPR_SERVER_LINK_CLOSE_EVT

enumerator ESP_BLE_MESH_RPR_SERVER_LINK_CLOSE_EVT

ESP_BLE_MESH_RPR_SERVER_PROV_COMP_EVT

enumerator ESP_BLE_MESH_RPR_SERVER_PROV_COMP_EVT

ESP_BLE_MESH_RPR_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_RPR_SERVER_EVT_MAX


```



### Directed Forwarding




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_df_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_df_model_api.h"
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


#### esp_ble_mesh_register_df_client_callback


```


esp_err_t esp_ble_mesh_register_df_client_callback(esp_ble_mesh_df_client_cb_t callback)
Register BLE Mesh Directed Forwarding Configuration Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_df_server_callback


```


esp_err_t esp_ble_mesh_register_df_server_callback(esp_ble_mesh_df_server_cb_t callback)
Register BLE Mesh Directed Forwarding Configuration Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_df_client_get_state


```


esp_err_t esp_ble_mesh_df_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_df_client_get_t *get)
Get the value of Directed Forwarding Configuration Server model state with the corresponding get message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
get -- [in] Pointer to a union, each kind of opcode corresponds to one structure inside.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_df_client_set_state


```


esp_err_t esp_ble_mesh_df_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_df_client_set_t *set)
Set the value of Directed Forwarding Configuration Server model state with the corresponding set message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
set -- [in] Pointer to a union, each kind of opcode corresponds to one structure inside.


Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_df_client_get_t


```


union esp_ble_mesh_df_client_get_t

#include <esp_ble_mesh_df_model_api.h>
Directed Forwarding Configuration Client model get message union. 

Public Members
directed_control_get

esp_ble_mesh_directed_control_get_t directed_control_get
For ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_GET 

path_metric_get

esp_ble_mesh_path_metric_get_t path_metric_get
For ESP_BLE_MESH_MODEL_OP_PATH_METRIC_GET 

disc_table_caps_get

esp_ble_mesh_discovery_table_caps_get_t disc_table_caps_get
For ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_GET 

forwarding_table_entries_cnt_get

esp_ble_mesh_forwarding_table_entries_cnt_get_t forwarding_table_entries_cnt_get
For ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_CNT_GET 

forwarding_table_entries_get

esp_ble_mesh_forwarding_table_entries_get_t forwarding_table_entries_get
For ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_GET 

forwarding_table_deps_get

esp_ble_mesh_forwarding_table_deps_get_t forwarding_table_deps_get
For ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_GET 

wanted_lanes_get

esp_ble_mesh_wanted_lanes_get_t wanted_lanes_get
For ESP_BLE_MESH_MODEL_OP_WANTED_LANES_GET 

two_way_path_get

esp_ble_mesh_two_way_path_get_t two_way_path_get
For ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_GET 

path_echo_interval_get

esp_ble_mesh_path_echo_interval_get_t path_echo_interval_get
For ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_GET 

directed_pub_policy_get

esp_ble_mesh_directed_publish_policy_get_t directed_pub_policy_get
For ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_GET 



```

#### esp_ble_mesh_df_client_set_t


```


union esp_ble_mesh_df_client_set_t

#include <esp_ble_mesh_df_model_api.h>
Directed Forwarding Configuration Client model set message union. 

Public Members
directed_control_set

esp_ble_mesh_directed_control_set_t directed_control_set
For ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_SET 

path_metric_set

esp_ble_mesh_path_metric_set_t path_metric_set
For ESP_BLE_MESH_MODEL_OP_PATH_METRIC_SET 

disc_table_caps_set

esp_ble_mesh_discovery_table_caps_set_t disc_table_caps_set
For ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_SET 

forwarding_table_add

esp_ble_mesh_forwarding_table_add_t forwarding_table_add
For ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ADD 

forwarding_table_del

esp_ble_mesh_forwarding_table_delete_t forwarding_table_del
For ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEL 

forwarding_table_deps_add

esp_ble_mesh_forwarding_table_deps_add_t forwarding_table_deps_add
For ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_ADD 

forwarding_table_deps_del

esp_ble_mesh_forwarding_table_deps_delete_t forwarding_table_deps_del
For ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_DEL 

wanted_lanes_set

esp_ble_mesh_wanted_lanes_set_t wanted_lanes_set
For ESP_BLE_MESH_MODEL_OP_WANTED_LANES_SET 

two_way_path_set

esp_ble_mesh_two_way_path_set_t two_way_path_set
For ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_SET 

path_echo_interval_set

esp_ble_mesh_path_echo_interval_set_t path_echo_interval_set
For ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_SET 

directed_net_transmit_set

esp_ble_mesh_directed_net_transmit_set_t directed_net_transmit_set
For ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_SET 

directed_relay_retransmit_set

esp_ble_mesh_directed_relay_retransmit_set_t directed_relay_retransmit_set
For ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_SET 

rssi_threshold_set

esp_ble_mesh_rssi_threshold_set_t rssi_threshold_set
For ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_SET 

directed_pub_policy_set

esp_ble_mesh_directed_publish_policy_set_t directed_pub_policy_set
For ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_SET 

path_disc_timing_ctl_set

esp_ble_mesh_path_discovery_timing_ctl_set_t path_disc_timing_ctl_set
For ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_SET 

directed_ctl_net_transmit_set

esp_ble_mesh_directed_ctl_net_transmit_set_t directed_ctl_net_transmit_set
For ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_SET 

directed_ctl_relay_retransmit_set

esp_ble_mesh_directed_ctl_relay_retransmit_set_t directed_ctl_relay_retransmit_set
For ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_SET 



```

#### esp_ble_mesh_df_client_recv_cb_t


```


union esp_ble_mesh_df_client_recv_cb_t

#include <esp_ble_mesh_df_model_api.h>
Directed Forwarding Configuration Client model received message union. 

Public Members
directed_control_status

esp_ble_mesh_directed_control_status_t directed_control_status
ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_STATUS 

path_metric_status

esp_ble_mesh_path_metric_status_t path_metric_status
ESP_BLE_MESH_MODEL_OP_PATH_METRIC_STATUS 

disc_table_caps_status

esp_ble_mesh_discovery_table_caps_status_t disc_table_caps_status
ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_STATUS 

forwarding_table_status

esp_ble_mesh_forwarding_table_status_t forwarding_table_status
ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_STATUS 

forwarding_table_deps_status

esp_ble_mesh_forwarding_table_deps_status_t forwarding_table_deps_status
ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_STATUS 

forwarding_table_entries_cnt_status

esp_ble_mesh_forwarding_table_entries_cnt_status_t forwarding_table_entries_cnt_status
ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_CNT_STATUS 

forwarding_table_entries_status

esp_ble_mesh_forwarding_table_entries_status_t forwarding_table_entries_status
ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_STATUS 

forwarding_table_deps_get_status

esp_ble_mesh_forwarding_table_deps_get_status_t forwarding_table_deps_get_status
ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_GET_STATUS 

wanted_lanes_status

esp_ble_mesh_wanted_lanes_status_t wanted_lanes_status
ESP_BLE_MESH_MODEL_OP_WANTED_LANES_STATUS 

two_way_path_status

esp_ble_mesh_two_way_path_status_t two_way_path_status
ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_STATUS 

path_echo_interval_status

esp_ble_mesh_path_echo_interval_status_t path_echo_interval_status
ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_STATUS 

directed_net_transmit_status

esp_ble_mesh_directed_net_transmit_status_t directed_net_transmit_status
ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_STATUS 

directed_relay_retransmit_status

esp_ble_mesh_directed_relay_retransmit_status_t directed_relay_retransmit_status
ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_STATUS 

rssi_threshold_status

esp_ble_mesh_rssi_threshold_status_t rssi_threshold_status
ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_STATUS 

directed_paths_status

esp_ble_mesh_directed_paths_status_t directed_paths_status
ESP_BLE_MESH_MODEL_OP_DIRECTED_PATHS_STATUS 

directed_pub_policy_status

esp_ble_mesh_directed_pub_policy_status_t directed_pub_policy_status
ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_STATUS 

path_disc_timing_ctl_status

esp_ble_mesh_path_disc_timing_ctl_status_cb_t path_disc_timing_ctl_status
ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_STATUS 

directed_ctl_net_transmit_status

esp_ble_mesh_directed_ctl_net_transmit_status_t directed_ctl_net_transmit_status
ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_STATUS 

directed_ctl_relay_retransmit_status

esp_ble_mesh_directed_ctl_relay_retransmit_status_t directed_ctl_relay_retransmit_status
ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_STATUS 



```

#### esp_ble_mesh_df_server_state_change_t


```


union esp_ble_mesh_df_server_state_change_t

#include <esp_ble_mesh_df_model_api.h>
Directed Forwarding Configuration Server model related context. 
Directed Forwarding Configuration Server model state change value union 

Public Members
dummy

uint8_t dummy
Event not used currently 



```

#### esp_ble_mesh_df_server_cb_value_t


```


union esp_ble_mesh_df_server_cb_value_t

#include <esp_ble_mesh_df_model_api.h>
Directed Forwarding Configuration Server model callback value union. 

Public Members
state_change

esp_ble_mesh_df_server_state_change_t state_change
For ESP_BLE_MESH_DF_SERVER_STATE_CHANGE_EVT 

table_change

esp_ble_mesh_df_server_table_change_t table_change
For ESP_BLE_MESH_DF_SERVER_TABLE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_df_srv_t


```


struct esp_ble_mesh_df_srv_t
Directed Forwarding Configuration Server model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Directed Forwarding Configuration Server model 

directed_net_transmit

uint8_t directed_net_transmit
Directed Network Transmit state 

directed_relay_retransmit

uint8_t directed_relay_retransmit
Directed Relay Retransmit state 

default_rssi_threshold

int8_t default_rssi_threshold
Default RSSI Threshold state 

rssi_margin

uint8_t rssi_margin
RSSI Margin state 

directed_node_paths

uint16_t directed_node_paths
Directed Node Paths state 

directed_relay_paths

uint16_t directed_relay_paths
Directed Relay Paths state 

directed_proxy_paths

uint16_t directed_proxy_paths
Directed Proxy Paths state 

directed_friend_paths

uint16_t directed_friend_paths
Directed Friend Paths state 

path_monitor_interval

uint16_t path_monitor_interval
Path Monitoring Interval state 

path_disc_retry_interval

uint16_t path_disc_retry_interval
Path Discovery Retry Interval state 

path_disc_interval

uint8_t path_disc_interval
Path Discovery Interval state 

lane_disc_guard_interval

uint8_t lane_disc_guard_interval
Lane Discovery Guard Interval state 

directed_ctl_net_transmit

uint8_t directed_ctl_net_transmit
Directed Control Network Transmit state 

directed_ctl_relay_retransmit

uint8_t directed_ctl_relay_retransmit
Directed Control Relay Retransmit state 



```

#### esp_ble_mesh_directed_control_get_t


```


struct esp_ble_mesh_directed_control_get_t
Parameters of Directed Control Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_directed_control_set_t


```


struct esp_ble_mesh_directed_control_set_t
Parameters of Directed Control Set. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

directed_forwarding

uint8_t directed_forwarding
New Directed Forwarding state 

directed_relay

uint8_t directed_relay
New Directed Relay state 

directed_proxy

uint8_t directed_proxy
New Directed Proxy state 

directed_proxy_use_default

uint8_t directed_proxy_use_default
New Directed Proxy Use Directed Default state or value to ignore 

directed_friend

uint8_t directed_friend
New Directed Friend state or value to ignore 



```

#### esp_ble_mesh_path_metric_get_t


```


struct esp_ble_mesh_path_metric_get_t
Parameters of Path Metric Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_path_metric_set_t


```


struct esp_ble_mesh_path_metric_set_t
Parameters of Path Metric Set. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

path_metric_type

uint8_t path_metric_type
New Path Metric Type state 

path_lifetime

uint8_t path_lifetime
New Path Lifetime state 



```

#### esp_ble_mesh_discovery_table_caps_get_t


```


struct esp_ble_mesh_discovery_table_caps_get_t
Parameters of Discovery Table Capabilities Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_discovery_table_caps_set_t


```


struct esp_ble_mesh_discovery_table_caps_set_t
Parameters of Discovery Table Capabilities Set. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

max_concurr_init

uint8_t max_concurr_init
New Max Concurrent Init state 



```

#### esp_ble_mesh_forwarding_table_add_t


```


struct esp_ble_mesh_forwarding_table_add_t
Parameters of Forwarding Table Add. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

unicast_dst

uint16_t unicast_dst
Indicates whether or not the destination of the path is a unicast address 

bw_path_validated

uint16_t bw_path_validated
Indicates whether or not the backward path has been validated 

path_origin

esp_ble_mesh_uar_t path_origin
Unicast address range of the Path Origin 

path_target

esp_ble_mesh_uar_t path_target
Unicast address range of the Path Target 

multicast_dst

uint16_t multicast_dst
Multicast destination address 

[anonymous]

union esp_ble_mesh_forwarding_table_add_t::[anonymous] [anonymous]
Path target address 

bearer_twd_path_origin

uint16_t bearer_twd_path_origin
Index of the bearer toward the Path Origin 

bearer_twd_path_target

uint16_t bearer_twd_path_target
Index of the bearer toward the Path Target 



```

#### esp_ble_mesh_forwarding_table_delete_t


```


struct esp_ble_mesh_forwarding_table_delete_t
Parameters of Forwarding Table Delete. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 



```

#### esp_ble_mesh_forwarding_table_deps_add_t


```


struct esp_ble_mesh_forwarding_table_deps_add_t
Parameters of Forwarding Table Dependents Add. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 

dep_origin_uar_list_size

uint8_t dep_origin_uar_list_size
Number of entries in the Dependent_Origin_Unicast_Addr_Range_List field 

dep_target_uar_list_size

uint8_t dep_target_uar_list_size
Number of entries in the Dependent_Target_Unicast_Addr_Range_List field 

dep_origin_uar_list

esp_ble_mesh_uar_t *dep_origin_uar_list
List of the unicast address ranges of the dependent nodes of the Path Origin 

dep_target_uar_list

esp_ble_mesh_uar_t *dep_target_uar_list
List of the unicast address ranges of the dependent nodes of the Path Target 



```

#### esp_ble_mesh_forwarding_table_deps_delete_t


```


struct esp_ble_mesh_forwarding_table_deps_delete_t
Parameters of Forwarding Table Dependents Delete. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 

dep_origin_list_size

uint8_t dep_origin_list_size
Number of entries in the Dependent_Origin_List field 

dep_target_list_size

uint8_t dep_target_list_size
Number of entries in the Dependent_Target_List field 

dep_origin_list

uint16_t *dep_origin_list
List of the primary element addresses of the dependent nodes of the Path Origin 

dep_target_list

uint16_t *dep_target_list
List of the primary element addresses of the dependent nodes of the Path Target 



```

#### esp_ble_mesh_forwarding_table_entries_cnt_get_t


```


struct esp_ble_mesh_forwarding_table_entries_cnt_get_t
Parameters of Forwarding Table Entries Count Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_forwarding_table_entries_get_t


```


struct esp_ble_mesh_forwarding_table_entries_get_t
Parameters of Forwarding Table Entries Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

filter_mask

uint16_t filter_mask
Filter to be applied to the Forwarding Table entries 

start_index

uint16_t start_index
Start offset to read in units of Forwarding Table entries 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 

include_id

bool include_id
Indicate whether or not the Forwarding Table Update Identifier is present 

update_id

uint16_t update_id
Last saved Forwarding Table Update Identifier (Optional) 



```

#### esp_ble_mesh_forwarding_table_deps_get_t


```


struct esp_ble_mesh_forwarding_table_deps_get_t
Parameters of Forwarding Table Dependents Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

dep_list_mask

uint16_t dep_list_mask
Filter applied to the lists of unicast address ranges for dependent nodes 

fixed_path_flag

uint16_t fixed_path_flag
Indicate whether or not to return the unicast address ranges of dependent nodes in a fixed path entry 

start_index

uint16_t start_index
Start offset in units of unicast address ranges 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 

include_id

bool include_id
Indicate whether or not the Forwarding Table Update Identifier is present 

update_id

uint16_t update_id
Last saved Forwarding Table Update Identifier (Optional) 



```

#### esp_ble_mesh_wanted_lanes_get_t


```


struct esp_ble_mesh_wanted_lanes_get_t
Parameters of Wanted Lanes Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_wanted_lanes_set_t


```


struct esp_ble_mesh_wanted_lanes_set_t
Parameters of Wanted Lanes Set. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

wanted_lanes

uint8_t wanted_lanes
New Wanted Lanes state 



```

#### esp_ble_mesh_two_way_path_get_t


```


struct esp_ble_mesh_two_way_path_get_t
Parameters of Two Way Path Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_two_way_path_set_t


```


struct esp_ble_mesh_two_way_path_set_t
Parameters of Two Way Path Set. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

two_way_path

uint8_t two_way_path
Two way path flag 



```

#### esp_ble_mesh_path_echo_interval_get_t


```


struct esp_ble_mesh_path_echo_interval_get_t
Parameters of Path Echo Interval Get. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 



```

#### esp_ble_mesh_path_echo_interval_set_t


```


struct esp_ble_mesh_path_echo_interval_set_t
Parameters of Path Echo Interval Set. 

Public Members
net_idx

uint16_t net_idx
NetKey Index 

unicast_echo_interval

uint8_t unicast_echo_interval
New Unicast Echo Interval state or indication of no state change 

multicast_echo_interval

uint8_t multicast_echo_interval
New Multicast Echo Interval state or indication of no state change 



```

#### esp_ble_mesh_directed_net_transmit_set_t


```


struct esp_ble_mesh_directed_net_transmit_set_t
Parameters of Directed Network Transmit Set. 

Public Members
net_transmit

uint8_t net_transmit
New Directed Network Transmit state 



```

#### esp_ble_mesh_directed_relay_retransmit_set_t


```


struct esp_ble_mesh_directed_relay_retransmit_set_t
Parameters of Directed Relay Retransmit Set. 

Public Members
relay_retransmit

uint8_t relay_retransmit
New Directed Relay Retransmit state 



```

#### esp_ble_mesh_rssi_threshold_set_t


```


struct esp_ble_mesh_rssi_threshold_set_t
Parameters of RSSI Threshold Set. 

Public Members
rssi_margin

uint8_t rssi_margin
New RSSI Margin state 



```

#### esp_ble_mesh_directed_publish_policy_get_t


```


struct esp_ble_mesh_directed_publish_policy_get_t
Parameters of Directed Publish Policy Get. 

Public Members
element_addr

uint16_t element_addr
Address of the element 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_directed_publish_policy_set_t


```


struct esp_ble_mesh_directed_publish_policy_set_t
Parameters of Directed Publish Policy Set. 

Public Members
direct_pub_policy

uint8_t direct_pub_policy
New Directed Publish Policy state 

element_addr

uint16_t element_addr
Address of the element 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_path_discovery_timing_ctl_set_t


```


struct esp_ble_mesh_path_discovery_timing_ctl_set_t
Parameters of Path Discovery Timing Control Set. 

Public Members
path_monitor_interval

uint16_t path_monitor_interval
New Path Monitoring Interval state 

path_disc_retry_interval

uint16_t path_disc_retry_interval
New Path Discovery Retry Interval state 

path_disc_interval

uint8_t path_disc_interval
New Path Discovery Interval state 

lane_disc_guard_interval

uint8_t lane_disc_guard_interval
New Lane Discovery Guard Interval state 



```

#### esp_ble_mesh_directed_ctl_net_transmit_set_t


```


struct esp_ble_mesh_directed_ctl_net_transmit_set_t
Parameters of Directed Control Network Transmit Set. 

Public Members
net_transmit

uint8_t net_transmit
New Directed Control Network Transmit Count state 



```

#### esp_ble_mesh_directed_ctl_relay_retransmit_set_t


```


struct esp_ble_mesh_directed_ctl_relay_retransmit_set_t
Parameters of Directed Control Relay Retransmit Set. 

Public Members
relay_retransmit

uint8_t relay_retransmit
New Directed Control Relay Retransmit Count state 



```

#### esp_ble_mesh_directed_control_status_t


```


struct esp_ble_mesh_directed_control_status_t
Parameters of Directed Control Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

directed_forwarding

uint8_t directed_forwarding
Current Directed Forwarding state 

directed_relay

uint8_t directed_relay
Current Directed Relay state 

directed_proxy

uint8_t directed_proxy
Current Directed Proxy state 

directed_proxy_use_default

uint8_t directed_proxy_use_default
Current Directed Proxy Use Directed Default state or 0xFF 

directed_friend

uint8_t directed_friend
Current Directed Friend state 



```

#### esp_ble_mesh_path_metric_status_t


```


struct esp_ble_mesh_path_metric_status_t
Parameters of Path Metric Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

path_metric_type

uint8_t path_metric_type
Current Path Metric Type state 

path_lifetime

uint8_t path_lifetime
Current Path Lifetime state 



```

#### esp_ble_mesh_discovery_table_caps_status_t


```


struct esp_ble_mesh_discovery_table_caps_status_t
Parameters of Discovery Table Capabilities Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

max_concurr_init

uint8_t max_concurr_init
Current Max Concurrent Init state 

max_disc_entries

uint8_t max_disc_entries
Max Discovery Table Entries Count state 



```

#### esp_ble_mesh_forwarding_table_status_t


```


struct esp_ble_mesh_forwarding_table_status_t
Parameters of Forwarding Table Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 



```

#### esp_ble_mesh_forwarding_table_deps_status_t


```


struct esp_ble_mesh_forwarding_table_deps_status_t
Parameters of Forwarding Table Dependent Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 



```

#### esp_ble_mesh_forwarding_table_entries_cnt_status_t


```


struct esp_ble_mesh_forwarding_table_entries_cnt_status_t
Parameters of Forwarding Table Entries Count Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

update_id

uint16_t update_id
Current Forwarding Table Update Identifier state 

fixed_entry_cnt

uint16_t fixed_entry_cnt
Number of fixed path entries in the Forwarding Table 

non_fixed_entry_cnt

uint16_t non_fixed_entry_cnt
Number of non-fixed path entries in the Forwarding Table 



```

#### esp_ble_mesh_forwarding_table_entry_t


```


struct esp_ble_mesh_forwarding_table_entry_t
Parameters of Forwarding Table Entry. 

Public Members
fixed_path_flag

uint16_t fixed_path_flag
Indicates whether the table entry is a fixed path entry or a non-fixed path entry 

unicast_dst_flag

uint16_t unicast_dst_flag
Indicates whether or not the destination of the path is a unicast address 

bw_path_validated_flag

uint16_t bw_path_validated_flag
Indicates whether or not the backward path has been validated 

bearer_twd_path_origin_ind

uint16_t bearer_twd_path_origin_ind
Indicates the presence or absence of the Bearer_Toward_Path_Origin field 

bearer_twd_path_target_ind

uint16_t bearer_twd_path_target_ind
Indicates the presence or absence of the Bearer_Toward_Path_Target field 

dep_origin_list_size_ind

uint16_t dep_origin_list_size_ind
Indicates the size of the Dependent_Origin_List field 

dep_target_list_size_ind

uint16_t dep_target_list_size_ind
Indicates the size of the Dependent_Target_List field 

lane_counter

uint8_t lane_counter
Number of lanes in the path 

path_remaining_time

uint16_t path_remaining_time
Path lifetime remaining 

path_origin_forward_number

uint8_t path_origin_forward_number
Forwarding number of the Path Origin 

path_origin

esp_ble_mesh_uar_t path_origin
Path Origin unicast address range 

dep_origin_list_size

uint16_t dep_origin_list_size
Current number of entries in the list of dependent nodes of the Path Origin 

bearer_twd_path_origin

uint16_t bearer_twd_path_origin
Index of the bearer toward the Path Origin 

path_target

esp_ble_mesh_uar_t path_target
Path Target unicast address range 

multicast_dst

uint16_t multicast_dst
Multicast destination address 

dep_target_list_size

uint16_t dep_target_list_size
Current number of entries in the list of dependent nodes of the Path Target 

bearer_twd_path_target

uint16_t bearer_twd_path_target
Index of the bearer toward the Path Target 



```

#### esp_ble_mesh_forwarding_table_entries_status_t


```


struct esp_ble_mesh_forwarding_table_entries_status_t
Parameters of Forwarding Table Entries Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

filter_mask

uint16_t filter_mask
Filter applied to the Forwarding Table entries 

start_index

uint16_t start_index
Start offset in units of Forwarding Table entries 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 

include_id

bool include_id
Indicate whether or not the Forwarding Table Update Identifier is present 

update_id

uint16_t update_id
Current Forwarding Table Update Identifier state 

entry_list_size

uint8_t entry_list_size
Current number of entries in the list of Forwarding Table entries 

entry_list

esp_ble_mesh_forwarding_table_entry_t *entry_list
List of Forwarding Table entries 



```

#### esp_ble_mesh_forwarding_table_deps_get_status_t


```


struct esp_ble_mesh_forwarding_table_deps_get_status_t
Parameters of Forwarding Table Dependents Get Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

dep_list_mask

uint16_t dep_list_mask
Filter applied to the lists of unicast address ranges for dependent nodes 

fixed_path_flag

uint16_t fixed_path_flag
Flag indicating whether or not to return the unicast address ranges of dependent nodes in a fixed path entry 

start_index

uint16_t start_index
Start offset in units of unicast address ranges 

path_origin

uint16_t path_origin
Primary element address of the Path Origin 

dst

uint16_t dst
Destination address 

include_id

bool include_id
Indicate whether or not the Forwarding Table Update Identifier is present 

update_id

uint16_t update_id
Current Forwarding Table Update Identifier state 

dep_origin_uar_list_size

uint8_t dep_origin_uar_list_size
Number of unicast address ranges in the Dependent_Origin_Unicast_Addr_Range_List field 

dep_target_uar_list_size

uint8_t dep_target_uar_list_size
Number of unicast address ranges in the Dependent_Target_Unicast_Addr_Range_List field 

dep_origin_uar_list

esp_ble_mesh_uar_t *dep_origin_uar_list
List of unicast address ranges of dependent nodes of the Path Origin 

dep_target_uar_list

esp_ble_mesh_uar_t *dep_target_uar_list
List of unicast address ranges of dependent nodes of the Path Target 



```

#### esp_ble_mesh_wanted_lanes_status_t


```


struct esp_ble_mesh_wanted_lanes_status_t
Parameters of Wanted Lanes Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

wanted_lanes

uint8_t wanted_lanes
Current Wanted Lanes state 



```

#### esp_ble_mesh_two_way_path_status_t


```


struct esp_ble_mesh_two_way_path_status_t
Parameters of Two Way Path Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

two_way_path

uint8_t two_way_path
Current Two Way Path state 



```

#### esp_ble_mesh_path_echo_interval_status_t


```


struct esp_ble_mesh_path_echo_interval_status_t
Parameters of Path Echo Interval Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

net_idx

uint16_t net_idx
NetKey Index 

unicast_echo_interval

uint8_t unicast_echo_interval
Current Unicast Echo Interval state 

multicast_echo_interval

uint8_t multicast_echo_interval
Current Multicast Echo Interval state 



```

#### esp_ble_mesh_directed_net_transmit_status_t


```


struct esp_ble_mesh_directed_net_transmit_status_t
Parameters of Directed Network Transmit Status. 

Public Members
net_transmit

uint8_t net_transmit
Current Directed Network Transmit state 



```

#### esp_ble_mesh_directed_relay_retransmit_status_t


```


struct esp_ble_mesh_directed_relay_retransmit_status_t
Parameters of Directed Relay Retransmit Status. 

Public Members
relay_retransmit

uint8_t relay_retransmit
Current Directed Relay Retransmit state 



```

#### esp_ble_mesh_rssi_threshold_status_t


```


struct esp_ble_mesh_rssi_threshold_status_t
Parameters of RSSI Threshold Status. 

Public Members
default_rssi_threshold

uint8_t default_rssi_threshold
Default RSSI Threshold state 

rssi_margin

uint8_t rssi_margin
Current RSSI Margin state 



```

#### esp_ble_mesh_directed_paths_status_t


```


struct esp_ble_mesh_directed_paths_status_t
Parameters of Directed Paths Status. 

Public Members
directed_node_paths

uint16_t directed_node_paths
Directed Node Paths state 

directed_relay_paths

uint16_t directed_relay_paths
Directed Relay Paths state 

directed_proxy_paths

uint16_t directed_proxy_paths
Directed Proxy Paths state 

directed_friend_paths

uint16_t directed_friend_paths
Directed Friend Paths state 



```

#### esp_ble_mesh_directed_pub_policy_status_t


```


struct esp_ble_mesh_directed_pub_policy_status_t
Parameters of Directed Publish Policy Status. 

Public Members
status

uint8_t status
Status code for the requesting message 

directed_pub_policy

uint8_t directed_pub_policy
Current Directed Publish Policy state 

element_addr

uint16_t element_addr
Address of the element 

company_id

uint16_t company_id
Company ID 

model_id

uint16_t model_id
Model ID 



```

#### esp_ble_mesh_path_disc_timing_ctl_status_cb_t


```


struct esp_ble_mesh_path_disc_timing_ctl_status_cb_t
Parameters of Path Discovery Timing Control Status. 

Public Members
path_monitor_interval

uint16_t path_monitor_interval
Current Path Monitoring Interval state 

path_disc_retry_interval

uint16_t path_disc_retry_interval
Current Path Discovery Retry Interval state 

path_disc_interval

uint8_t path_disc_interval
Current Path Discovery Interval state 

lane_disc_guard_interval

uint8_t lane_disc_guard_interval
Current Lane Discovery Guard Interval state 



```

#### esp_ble_mesh_directed_ctl_net_transmit_status_t


```


struct esp_ble_mesh_directed_ctl_net_transmit_status_t
Parameters of Directed Control Network Transmit Status. 

Public Members
net_transmit

uint8_t net_transmit
Current Directed Control Network Transmit state 



```

#### esp_ble_mesh_directed_ctl_relay_retransmit_status_t


```


struct esp_ble_mesh_directed_ctl_relay_retransmit_status_t
Parameters of Directed Control Relay Retransmit Status. 

Public Members
relay_retransmit

uint8_t relay_retransmit
Current Directed Control Relay Retransmit state 



```

#### esp_ble_mesh_df_client_send_cb_t


```


struct esp_ble_mesh_df_client_send_cb_t
Result of sending Directed Forwarding Configuration Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_df_client_cb_param_t


```


struct esp_ble_mesh_df_client_cb_param_t
Directed Forwarding Configuration Client model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
Client common parameters, used by all events. 

send

esp_ble_mesh_df_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_df_client_recv_cb_t recv
Parameters of received status message 

[anonymous]

union esp_ble_mesh_df_client_cb_param_t::[anonymous] [anonymous]
Union of DF Client callback 



```

#### esp_ble_mesh_df_server_table_change_t


```


struct esp_ble_mesh_df_server_table_change_t
Parameters of directed forwarding table entry change 

Public Members
action

esp_ble_mesh_df_table_action_t action
Action of directed forwarding table 

path_origin

esp_ble_mesh_uar_t path_origin
Primary element address of the Path Origin 

path_target

esp_ble_mesh_uar_t path_target
Primary element address of the Path Target 

dep_origin_data

esp_ble_mesh_uar_t *dep_origin_data
List of the primary element addresses of the dependent nodes of the Path Origin 

dep_origin_num

uint32_t dep_origin_num
Number of entries in the Dependent_Origin_List field of the message 

dep_target_data

esp_ble_mesh_uar_t *dep_target_data
List of the primary element addresses of the dependent nodes of the Path Target 

dep_target_num

uint32_t dep_target_num
Number of entries in the Dependent_Target_List field of the message 

fixed_path

uint8_t fixed_path
Indicates whether the table entry is a fixed path entry or a non-fixed path entry 

bw_path_validate

uint8_t bw_path_validate
Indicates whether or not the backward path has been validated 

path_not_ready

uint8_t path_not_ready
Flag indicating whether or not the path is ready for use 

forward_number

uint8_t forward_number
Forwarding number of the Path Origin; If the entry is associated with a fixed path, the value is 0 

lane_counter

uint8_t lane_counter
Number of lanes discovered; if the entry is associated with a fixed path, the value is 1. 

df_table_entry_add_remove

struct esp_ble_mesh_df_server_table_change_t::[anonymous]::[anonymous] df_table_entry_add_remove
Structure of directed forwarding table add and remove Structure of directed forwarding table add and remove 

dummy

uint8_t dummy
Event not used currently 

df_table_entry_change

struct esp_ble_mesh_df_server_table_change_t::[anonymous]::[anonymous] df_table_entry_change
Structure of directed forwarding table entry change Directed forwarding table entry change 

df_table_info

union esp_ble_mesh_df_server_table_change_t::[anonymous] df_table_info
Union of directed forwarding table information Directed forwarding table information 



```

#### esp_ble_mesh_df_server_cb_param_t


```


struct esp_ble_mesh_df_server_cb_param_t
Directed Forwarding Configuration Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_df_server_cb_value_t value
Value of the received configuration messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_GET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_GET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_SET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_SET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_STATUS


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CONTROL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_PATH_METRIC_GET


```


ESP_BLE_MESH_MODEL_OP_PATH_METRIC_GET

```

#### ESP_BLE_MESH_MODEL_OP_PATH_METRIC_SET


```


ESP_BLE_MESH_MODEL_OP_PATH_METRIC_SET

```

#### ESP_BLE_MESH_MODEL_OP_PATH_METRIC_STATUS


```


ESP_BLE_MESH_MODEL_OP_PATH_METRIC_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_GET


```


ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_GET

```

#### ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_SET


```


ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_SET

```

#### ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_STATUS


```


ESP_BLE_MESH_MODEL_OP_DISCOVERY_TABLE_CAPS_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ADD


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ADD

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEL


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEL

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_STATUS


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_ADD


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_ADD

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_DEL


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_DEL

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_STATUS


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_GET


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_GET

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_GET_STATUS


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_DEPS_GET_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_CNT_GET


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_CNT_GET

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_CNT_STATUS


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_CNT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_GET


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_GET

```

#### ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_STATUS


```


ESP_BLE_MESH_MODEL_OP_FORWARDING_TABLE_ENTRIES_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_WANTED_LANES_GET


```


ESP_BLE_MESH_MODEL_OP_WANTED_LANES_GET

```

#### ESP_BLE_MESH_MODEL_OP_WANTED_LANES_SET


```


ESP_BLE_MESH_MODEL_OP_WANTED_LANES_SET

```

#### ESP_BLE_MESH_MODEL_OP_WANTED_LANES_STATUS


```


ESP_BLE_MESH_MODEL_OP_WANTED_LANES_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_GET


```


ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_GET

```

#### ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_SET


```


ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_SET

```

#### ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_STATUS


```


ESP_BLE_MESH_MODEL_OP_TWO_WAY_PATH_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_GET


```


ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_GET

```

#### ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_SET


```


ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_SET

```

#### ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_STATUS


```


ESP_BLE_MESH_MODEL_OP_PATH_ECHO_INTERVAL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_GET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_GET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_SET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_SET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_STATUS


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_NET_TRANSMIT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_GET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_GET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_SET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_SET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_STATUS


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_RELAY_RETRANSMIT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_GET


```


ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_GET

```

#### ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_SET


```


ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_SET

```

#### ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_STATUS


```


ESP_BLE_MESH_MODEL_OP_RSSI_THRESHOLD_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_PATHS_GET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_PATHS_GET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_PATHS_STATUS


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_PATHS_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_GET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_GET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_SET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_SET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_STATUS


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_PUB_POLICY_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_GET


```


ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_GET

```

#### ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_SET


```


ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_SET

```

#### ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_STATUS


```


ESP_BLE_MESH_MODEL_OP_PATH_DISCOVERY_TIMING_CTL_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_GET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_GET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_SET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_SET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_STATUS


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_NET_TRANSMIT_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_GET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_GET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_SET


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_SET

```

#### ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_STATUS


```


ESP_BLE_MESH_MODEL_OP_DIRECTED_CTL_RELAY_RETRANSMIT_STATUS

```

#### ESP_BLE_MESH_PATH_DISC_INTERVAL_5_SEC


```


ESP_BLE_MESH_PATH_DISC_INTERVAL_5_SEC

```

#### ESP_BLE_MESH_PATH_DISC_INTERVAL_30_SEC


```


ESP_BLE_MESH_PATH_DISC_INTERVAL_30_SEC

```

#### ESP_BLE_MESH_LANE_DISC_GUARD_INTERVAL_2_SEC


```


ESP_BLE_MESH_LANE_DISC_GUARD_INTERVAL_2_SEC

```

#### ESP_BLE_MESH_LANE_DISC_GUARD_INTERVAL_10_SEC


```


ESP_BLE_MESH_LANE_DISC_GUARD_INTERVAL_10_SEC

```

#### ESP_BLE_MESH_DIRECTED_PUB_POLICY_MANAGED_FLOODING


```


ESP_BLE_MESH_DIRECTED_PUB_POLICY_MANAGED_FLOODING

```

#### ESP_BLE_MESH_DIRECTED_PUB_POLICY_DIRECTED_FORWARDING


```


ESP_BLE_MESH_DIRECTED_PUB_POLICY_DIRECTED_FORWARDING

```

#### ESP_BLE_MESH_GET_FILTER_MASK


```


ESP_BLE_MESH_GET_FILTER_MASK(fp, nfp, pom, dm)

```

#### ESP_BLE_MESH_MODEL_DF_SRV


```


ESP_BLE_MESH_MODEL_DF_SRV(srv_data)
Define a new Directed Forwarding Configuration Server model. 

Note
If supported, the model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

srv_data -- Pointer to a unique Directed Forwarding Configuration Server model user_data.


Returns
New Directed Forwarding Configuration Server model instance. 



```

#### ESP_BLE_MESH_MODEL_DF_CLI


```


ESP_BLE_MESH_MODEL_DF_CLI(cli_data)
Define a new Directed Forwarding Configuration Client model. 

Note
If supported, the model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

cli_data -- Pointer to a unique Directed Forwarding Configuration Client model user_data.


Returns
New Directed Forwarding Configuration Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_df_client_cb_t


```


typedef void (*esp_ble_mesh_df_client_cb_t)(esp_ble_mesh_df_client_cb_event_t event, esp_ble_mesh_df_client_cb_param_t *param)
Bluetooth Mesh Directed Forwarding Configuration client and server model functions. 
Directed Forwarding Configuration Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_df_server_cb_t


```


typedef void (*esp_ble_mesh_df_server_cb_t)(esp_ble_mesh_df_server_cb_event_t event, esp_ble_mesh_df_server_cb_param_t *param)
Directed Forwarding Configuration Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_df_client_cb_event_t


```


enum esp_ble_mesh_df_client_cb_event_t
This enum value is the event of Directed Forwarding Configuration Client model 
Values:
ESP_BLE_MESH_DF_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_DF_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_DF_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_DF_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_DF_CLIENT_RECV_GET_RSP_EVT

enumerator ESP_BLE_MESH_DF_CLIENT_RECV_GET_RSP_EVT

ESP_BLE_MESH_DF_CLIENT_RECV_SET_RSP_EVT

enumerator ESP_BLE_MESH_DF_CLIENT_RECV_SET_RSP_EVT

ESP_BLE_MESH_DF_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_DF_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_DF_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_DF_CLIENT_EVT_MAX


```

#### esp_ble_mesh_df_table_action_t


```


enum esp_ble_mesh_df_table_action_t
Values:
ESP_BLE_MESH_DF_TABLE_ACT_EMPTY

enumerator ESP_BLE_MESH_DF_TABLE_ACT_EMPTY

ESP_BLE_MESH_DF_TABLE_ADD

enumerator ESP_BLE_MESH_DF_TABLE_ADD

ESP_BLE_MESH_DF_TABLE_REMOVE

enumerator ESP_BLE_MESH_DF_TABLE_REMOVE

ESP_BLE_MESH_DF_TABLE_ENTRY_CHANGE

enumerator ESP_BLE_MESH_DF_TABLE_ENTRY_CHANGE

ESP_BLE_MESH_DF_TABLE_ACT_MAX_LIMIT

enumerator ESP_BLE_MESH_DF_TABLE_ACT_MAX_LIMIT


```

#### esp_ble_mesh_df_server_cb_event_t


```


enum esp_ble_mesh_df_server_cb_event_t
This enum value is the event of Directed Forwarding Configuration Server model 
Values:
ESP_BLE_MESH_DF_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_DF_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_DF_SERVER_TABLE_CHANGE_EVT

enumerator ESP_BLE_MESH_DF_SERVER_TABLE_CHANGE_EVT

ESP_BLE_MESH_DF_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_DF_SERVER_EVT_MAX


```



### Subnet Bridge Configuration




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_brc_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_brc_model_api.h"
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


#### esp_ble_mesh_register_brc_client_callback


```


esp_err_t esp_ble_mesh_register_brc_client_callback(esp_ble_mesh_brc_client_cb_t callback)
Register BLE Mesh Bridge Configuration Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_brc_server_callback


```


esp_err_t esp_ble_mesh_register_brc_server_callback(esp_ble_mesh_brc_server_cb_t callback)
Register BLE Mesh Bridge Configuration Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_brc_client_send


```


esp_err_t esp_ble_mesh_brc_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_brc_client_msg_t *msg)
Get/Set the value of Bridge Configuration Server model state with the corresponding message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to Bridge Configuration Client message.


Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_brc_client_msg_t


```


union esp_ble_mesh_brc_client_msg_t

#include <esp_ble_mesh_brc_model_api.h>
Bridge Configuration Client model message union. 

Public Members
bridged_subnets_get

esp_ble_mesh_bridged_subnets_get_t bridged_subnets_get
For ESP_BLE_MESH_MODEL_OP_BRIDGED_SUBNETS_GET 

bridging_table_get

esp_ble_mesh_bridging_table_get_t bridging_table_get
For ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_GET 

subnet_bridge_set

esp_ble_mesh_subnet_bridge_set_t subnet_bridge_set
For ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_SET 

bridging_table_add

esp_ble_mesh_bridging_table_add_t bridging_table_add
For ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_ADD 

bridging_table_remove

esp_ble_mesh_bridging_table_remove_t bridging_table_remove
For ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_REMOVE 



```

#### esp_ble_mesh_brc_client_recv_cb_t


```


union esp_ble_mesh_brc_client_recv_cb_t

#include <esp_ble_mesh_brc_model_api.h>
Bridge Configuration Client model received message union. 

Public Members
subnet_bridge_status

esp_ble_mesh_subnet_bridge_status_t subnet_bridge_status
ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_STATUS 

bridging_table_status

esp_ble_mesh_bridging_table_status_t bridging_table_status
ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_STATUS 

bridged_subnets_list

esp_ble_mesh_bridged_subnets_list_t bridged_subnets_list
ESP_BLE_MESH_MODEL_OP_BRIDGED_SUBNETS_LIST 

bridging_table_list

esp_ble_mesh_bridging_table_list_t bridging_table_list
ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_LIST 

bridging_table_size_status

esp_ble_mesh_bridging_table_size_status_t bridging_table_size_status
ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_SIZE_STATUS 



```

#### esp_ble_mesh_brc_server_state_change_t


```


union esp_ble_mesh_brc_server_state_change_t

#include <esp_ble_mesh_brc_model_api.h>
Bridge Configuration Server model state change value union. 

Public Members
bridging_table_add

esp_ble_mesh_state_change_bridging_table_add_t bridging_table_add
The recv_op in ctx can be used to decide which state is changed. Bridging Table Add 

bridging_table_remove

esp_ble_mesh_state_change_bridging_table_remove_t bridging_table_remove
Bridging Table Remove 



```

#### esp_ble_mesh_brc_server_cb_value_t


```


union esp_ble_mesh_brc_server_cb_value_t

#include <esp_ble_mesh_brc_model_api.h>
Bridge Configuration Server model callback value union. 

Public Members
state_change

esp_ble_mesh_brc_server_state_change_t state_change
For ESP_BLE_MESH_BRC_SERVER_STATE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_subnet_bridge_table_t


```


struct esp_ble_mesh_subnet_bridge_table_t
Parameters of subnet bridge table 

Public Members
bridge_direction

uint8_t bridge_direction
Allowed directions for the bridged traffic 

bridge_net_idx

uint8_t bridge_net_idx[3]
Two NetKey Indexes are packed into three octets 

bridge_addr_1

uint16_t bridge_addr_1
Address of the node in the first subnet 

bridge_addr_2

uint16_t bridge_addr_2
Address of the node in the second subnet 



```

#### esp_ble_mesh_subnet_bridge_set_t


```


struct esp_ble_mesh_subnet_bridge_set_t
Parameters of Subnet Bridge Set 

Public Members
subnet_bridge

uint8_t subnet_bridge
New Subnet Bridge state 



```

#### esp_ble_mesh_bridging_table_add_t


```


struct esp_ble_mesh_bridging_table_add_t
Parameters of Bridging Table Add 

Public Members
bridge_direction

uint8_t bridge_direction
Allowed directions for the bridged traffic 

bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of the first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 

bridge_addr_1

uint16_t bridge_addr_1
Address of the node in the first subnet 

bridge_addr_2

uint16_t bridge_addr_2
Address of the node in the second subnet 



```

#### esp_ble_mesh_bridging_table_remove_t


```


struct esp_ble_mesh_bridging_table_remove_t
Parameters of Bridging Table Remove 

Public Members
bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of the first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 

bridge_addr_1

uint16_t bridge_addr_1
Address of the node in the first subnet 

bridge_addr_2

uint16_t bridge_addr_2
Address of the node in the second subnet 



```

#### esp_ble_mesh_bridged_subnets_get_t


```


struct esp_ble_mesh_bridged_subnets_get_t
Parameters of Bridged Subnets Get 

Public Members
bridge_filter

uint16_t bridge_filter
Filter to be applied when reporting the set of pairs of NetKey Indexes 

bridge_net_idx

uint16_t bridge_net_idx
NetKey Index of any of the subnets 

bridge_start_idx

uint8_t bridge_start_idx
Start offset in units of Bridging Table state entries 



```

#### esp_ble_mesh_bridging_table_get_t


```


struct esp_ble_mesh_bridging_table_get_t
Parameters of Bridging Table Get 

Public Members
bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 

bridge_start_idx

uint16_t bridge_start_idx
Start offset in units of Bridging Table state entries 



```

#### esp_ble_mesh_subnet_bridge_status_t


```


struct esp_ble_mesh_subnet_bridge_status_t
Parameters of Subnet Bridge Status 

Public Members
subnet_bridge

uint8_t subnet_bridge
Current Subnet Bridge state 



```

#### esp_ble_mesh_bridging_table_status_t


```


struct esp_ble_mesh_bridging_table_status_t
Parameters of Bridging Table Status 

Public Members
status

uint8_t status
Status Code for the requesting message 

bridge_direction

uint8_t bridge_direction
Allowed directions for the bridged traffic 

bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of the first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 

bridge_addr_1

uint16_t bridge_addr_1
Address of the node in the first subnet 

bridge_addr_2

uint16_t bridge_addr_2
Address of the node in the second subnet 



```

#### esp_ble_mesh_bridge_net_idx_pair_entry_t


```


struct esp_ble_mesh_bridge_net_idx_pair_entry_t
Bridged_Subnets_List entry format 

Public Members
bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of the first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 



```

#### esp_ble_mesh_bridged_subnets_list_t


```


struct esp_ble_mesh_bridged_subnets_list_t
Parameters of Bridged Subnets List 

Public Members
bridge_filter

uint16_t bridge_filter
Filter applied to the set of pairs of NetKey Indexes 

bridge_net_idx

uint16_t bridge_net_idx
NetKey Index used for filtering or ignored 

bridge_start_idx

uint8_t bridge_start_idx
Start offset in units of bridges 

bridged_entry_list_size

uint8_t bridged_entry_list_size
Num of pairs of NetKey Indexes 

net_idx_pair

esp_ble_mesh_bridge_net_idx_pair_entry_t *net_idx_pair
Filtered set of N pairs of NetKey Indexes 



```

#### esp_ble_mesh_bridged_addr_list_entry_t


```


struct esp_ble_mesh_bridged_addr_list_entry_t
Bridged_Addresses_List entry format 

Public Members
bridge_direction

uint8_t bridge_direction
Allowed directions for bridged traffic 

bridge_addr_1

uint16_t bridge_addr_1
Address of the node in the first subnet 

bridge_addr_2

uint16_t bridge_addr_2
Address of the node in the second subnet 



```

#### esp_ble_mesh_bridging_table_list_t


```


struct esp_ble_mesh_bridging_table_list_t
Parameters of Bridging Table List 

Public Members
status

uint8_t status
Status Code for the requesting message 

bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of the first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 

bridge_start_idx

uint16_t bridge_start_idx
Start offset in units of Bridging Table state entries 

bridged_addr_list_size

uint16_t bridged_addr_list_size
Num of pairs of entry 

bridged_addr_list

esp_ble_mesh_bridged_addr_list_entry_t *bridged_addr_list
List of bridged addresses and allowed traffic directions 



```

#### esp_ble_mesh_bridging_table_size_status_t


```


struct esp_ble_mesh_bridging_table_size_status_t
Parameters of Bridging Table Size Status 

Public Members
bridging_table_size

uint16_t bridging_table_size
Bridging Table Size state 



```

#### esp_ble_mesh_brc_client_send_cb_t


```


struct esp_ble_mesh_brc_client_send_cb_t
Result of sending Bridge Configuration Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_brc_client_cb_param_t


```


struct esp_ble_mesh_brc_client_cb_param_t
Bridge Configuration Client model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
Client common parameters, used by all events. 

send

esp_ble_mesh_brc_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_brc_client_recv_cb_t recv
Parameters of received status message 

[anonymous]

union esp_ble_mesh_brc_client_cb_param_t::[anonymous] [anonymous]
Union of Bridge Configuration Client callback 



```

#### esp_ble_mesh_state_change_bridging_table_add_t


```


struct esp_ble_mesh_state_change_bridging_table_add_t
Bridge Configuration Server model related context. 
Parameters of Bridging Table Add 

Public Members
bridge_direction

uint8_t bridge_direction
Allowed directions for the bridged traffic 

bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of the first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 

bridge_addr_1

uint16_t bridge_addr_1
Address of the node in the first subnet 

bridge_addr_2

uint16_t bridge_addr_2
Address of the node in the second subnet 



```

#### esp_ble_mesh_state_change_bridging_table_remove_t


```


struct esp_ble_mesh_state_change_bridging_table_remove_t
Parameters of Bridging Table Remove 

Public Members
bridge_net_idx_1

uint16_t bridge_net_idx_1
NetKey Index of the first subnet 

bridge_net_idx_2

uint16_t bridge_net_idx_2
NetKey Index of the second subnet 

bridge_addr_1

uint16_t bridge_addr_1
Address of the node in the first subnet 

bridge_addr_2

uint16_t bridge_addr_2
Address of the node in the second subnet 



```

#### esp_ble_mesh_brc_server_cb_param_t


```


struct esp_ble_mesh_brc_server_cb_param_t
Bridge Configuration Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_brc_server_cb_value_t value
Value of the received configuration messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_GET


```


ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_GET

```

#### ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_SET


```


ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_SET

```

#### ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_STATUS


```


ESP_BLE_MESH_MODEL_OP_SUBNET_BRIDGE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_ADD


```


ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_ADD

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_REMOVE


```


ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_REMOVE

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_STATUS


```


ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGED_SUBNETS_GET


```


ESP_BLE_MESH_MODEL_OP_BRIDGED_SUBNETS_GET

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGED_SUBNETS_LIST


```


ESP_BLE_MESH_MODEL_OP_BRIDGED_SUBNETS_LIST

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_GET


```


ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_GET

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_LIST


```


ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_LIST

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_SIZE_GET


```


ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_SIZE_GET

```

#### ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_SIZE_STATUS


```


ESP_BLE_MESH_MODEL_OP_BRIDGING_TABLE_SIZE_STATUS

```

#### ESP_BLE_MESH_MODEL_BRC_SRV


```


ESP_BLE_MESH_MODEL_BRC_SRV(srv_data)
Define a new Bridge Configuration Server model. 

Note
If supported, the model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

srv_data -- Pointer to a unique Bridge Configuration Server model user_data.


Returns
New Bridge Configuration Server model instance. 



```

#### ESP_BLE_MESH_MODEL_BRC_CLI


```


ESP_BLE_MESH_MODEL_BRC_CLI(cli_data)
Define a new Bridge Configuration Client model. 

Note
If supported, the model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

cli_data -- Pointer to a unique Bridge Configuration Client model user_data.


Returns
New Bridge Configuration Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_brc_client_cb_t


```


typedef void (*esp_ble_mesh_brc_client_cb_t)(esp_ble_mesh_brc_client_cb_event_t event, esp_ble_mesh_brc_client_cb_param_t *param)
Bluetooth Mesh Bridge Configuration client and server model functions. 
Bridge Configuration Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_brc_server_cb_t


```


typedef void (*esp_ble_mesh_brc_server_cb_t)(esp_ble_mesh_brc_server_cb_event_t event, esp_ble_mesh_brc_server_cb_param_t *param)
Bridge Configuration Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_brc_client_cb_event_t


```


enum esp_ble_mesh_brc_client_cb_event_t
This enum value is the event of Bridge Configuration Client model 
Values:
ESP_BLE_MESH_BRC_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_BRC_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_BRC_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_BRC_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_BRC_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_BRC_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_BRC_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_BRC_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_BRC_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_BRC_CLIENT_EVT_MAX


```

#### esp_ble_mesh_brc_server_cb_event_t


```


enum esp_ble_mesh_brc_server_cb_event_t
This enum value is the event of Bridge Configuration Server model 
Values:
ESP_BLE_MESH_BRC_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_BRC_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_BRC_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_BRC_SERVER_EVT_MAX


```



### Mesh Private Beacon




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_prb_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_prb_model_api.h"
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


#### esp_ble_mesh_register_prb_client_callback


```


esp_err_t esp_ble_mesh_register_prb_client_callback(esp_ble_mesh_prb_client_cb_t callback)
Register BLE Mesh Private Beacon Client Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_prb_server_callback


```


esp_err_t esp_ble_mesh_register_prb_server_callback(esp_ble_mesh_prb_server_cb_t callback)
Register BLE Mesh Private Beacon Server Model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_prb_client_send


```


esp_err_t esp_ble_mesh_prb_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_prb_client_msg_t *msg)
Get/Set the value of Private Beacon Server Model states using the corresponding messages of Private Beacon Client Model. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to Mesh Private Beacon Client message.


Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_prb_client_msg_t


```


union esp_ble_mesh_prb_client_msg_t

#include <esp_ble_mesh_prb_model_api.h>
Mesh Private Beacon Client model message union. 

Public Members
priv_beacon_set

esp_ble_mesh_priv_beacon_set_t priv_beacon_set
ESP_BLE_MESH_MODEL_OP_PRIV_BEACON_SET. 

priv_gatt_proxy_set

esp_ble_mesh_priv_gatt_proxy_set_t priv_gatt_proxy_set
ESP_BLE_MESH_MODEL_OP_PRIV_GATT_PROXY_SET. 

priv_node_id_get

esp_ble_mesh_priv_node_id_get_t priv_node_id_get
ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_GET. 

priv_node_id_set

esp_ble_mesh_priv_node_id_set_t priv_node_id_set
ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_SET. 



```

#### esp_ble_mesh_prb_client_recv_cb_t


```


union esp_ble_mesh_prb_client_recv_cb_t

#include <esp_ble_mesh_prb_model_api.h>
Private Beacon Client Model received message union. 

Public Members
priv_beacon_status

esp_ble_mesh_priv_beacon_status_cb_t priv_beacon_status
The private beacon status value 

priv_gatt_proxy_status

esp_ble_mesh_priv_gatt_proxy_status_cb_t priv_gatt_proxy_status
The private gatt proxy status value 

priv_node_id_status

esp_ble_mesh_priv_node_identity_status_cb_t priv_node_id_status
The private node identity status value 



```

#### esp_ble_mesh_prb_server_state_change_t


```


union esp_ble_mesh_prb_server_state_change_t

#include <esp_ble_mesh_prb_model_api.h>
Mesh Private Beacon Server model state change value union. 

Public Members
priv_beacon_set

esp_ble_mesh_state_change_priv_beacon_set_t priv_beacon_set
The recv_op in ctx can be used to decide which state is changed. Private Beacon Set 

priv_gatt_proxy_set

esp_ble_mesh_state_change_priv_gatt_proxy_set_t priv_gatt_proxy_set
Private GATT Proxy Set 

priv_node_id_set

esp_ble_mesh_state_change_priv_node_id_set_t priv_node_id_set
Private Node Identity Set 



```

#### esp_ble_mesh_prb_server_cb_value_t


```


union esp_ble_mesh_prb_server_cb_value_t

#include <esp_ble_mesh_prb_model_api.h>
Private Beacon Server model callback value union. 

Public Members
state_change

esp_ble_mesh_prb_server_state_change_t state_change
ESP_BLE_MESH_PRB_SERVER_STATE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_prb_srv_t


```


struct esp_ble_mesh_prb_srv_t
Private Beacon Server Model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Private Beacon Server Model 

private_beacon

uint8_t private_beacon
Value of Private Beacon state 

random_update_interval

uint8_t random_update_interval
Value of Random Update Interval Steps state 

private_gatt_proxy

uint8_t private_gatt_proxy
Value of Private GATT Proxy state 

update_timer

struct k_delayed_work update_timer
Timer for update the random field of private beacon 



```

#### esp_ble_mesh_priv_beacon_set_t


```


struct esp_ble_mesh_priv_beacon_set_t
Parameter of private Beacon Set 

Public Members
private_beacon

uint8_t private_beacon
New Private Beacon state 

is_effect

bool is_effect
Decide if update_interval exists 

update_interval

uint8_t update_interval
New Random Update Interval Steps state 



```

#### esp_ble_mesh_priv_gatt_proxy_set_t


```


struct esp_ble_mesh_priv_gatt_proxy_set_t
Parameter of Private GATT Proxy Set 

Public Members
private_gatt_proxy

uint8_t private_gatt_proxy
New Private GATT Proxy state 



```

#### esp_ble_mesh_priv_node_id_get_t


```


struct esp_ble_mesh_priv_node_id_get_t
Parameter of Private node identity Get 

Public Members
net_idx

uint16_t net_idx
Index of the NetKey 



```

#### esp_ble_mesh_priv_node_id_set_t


```


struct esp_ble_mesh_priv_node_id_set_t
Parameter of Private node identity Set 

Public Members
net_idx

uint16_t net_idx
Index of the NetKey 

private_node_id

uint8_t private_node_id
New Private Node Identity state 



```

#### esp_ble_mesh_priv_beacon_status_cb_t


```


struct esp_ble_mesh_priv_beacon_status_cb_t
Parameter of Private Beacon Status 

Public Members
private_beacon

uint8_t private_beacon
Current value of the Private Beacon state 

update_interval

uint8_t update_interval
Current value of the Random Update Interval Steps state 



```

#### esp_ble_mesh_priv_gatt_proxy_status_cb_t


```


struct esp_ble_mesh_priv_gatt_proxy_status_cb_t
Parameter of Private GATT Proxy Status 

Public Members
private_gatt_proxy

uint8_t private_gatt_proxy
Private GATT Proxy state 



```

#### esp_ble_mesh_priv_node_identity_status_cb_t


```


struct esp_ble_mesh_priv_node_identity_status_cb_t
Parameters of Private Node Identity Status 

Public Members
status

uint8_t status
Status Code for the requesting message 

net_idx

uint16_t net_idx
Index of the NetKey 

private_node_id

uint8_t private_node_id
Private Node Identity state 



```

#### esp_ble_mesh_prb_client_send_cb_t


```


struct esp_ble_mesh_prb_client_send_cb_t
Result of sending Bridge Configuration Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_prb_client_cb_param_t


```


struct esp_ble_mesh_prb_client_cb_param_t
Mesh Private Beacon Client Model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
The client common parameters. 

send

esp_ble_mesh_prb_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_prb_client_recv_cb_t recv
The private beacon message status callback values 

[anonymous]

union esp_ble_mesh_prb_client_cb_param_t::[anonymous] [anonymous]
Union of Private Beacon Client callback 



```

#### esp_ble_mesh_state_change_priv_beacon_set_t


```


struct esp_ble_mesh_state_change_priv_beacon_set_t
Mesh Private Beacon Server model related context. 
Parameters of Private Beacon Set. 

Public Members
private_beacon

uint8_t private_beacon
Private Beacon state 

is_effect

bool is_effect
Decide whether update_interval effect 

update_interval

uint8_t update_interval
Random Update Interval Steps state 



```

#### esp_ble_mesh_state_change_priv_gatt_proxy_set_t


```


struct esp_ble_mesh_state_change_priv_gatt_proxy_set_t
Parameters of Private GATT Proxy Set. 

Public Members
private_gatt_proxy

uint8_t private_gatt_proxy
Private GATT Proxy state 



```

#### esp_ble_mesh_state_change_priv_node_id_set_t


```


struct esp_ble_mesh_state_change_priv_node_id_set_t
Parameters of Private Node Identity Set. 

Public Members
net_idx

uint16_t net_idx
Index of the NetKey 

private_node_id

uint8_t private_node_id
Private Node Identity state 



```

#### esp_ble_mesh_prb_server_cb_param_t


```


struct esp_ble_mesh_prb_server_cb_param_t
Private Beacon Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_prb_server_cb_value_t value
Value of the received private beacon messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_PRIV_BEACON_GET


```


ESP_BLE_MESH_MODEL_OP_PRIV_BEACON_GET

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_BEACON_SET


```


ESP_BLE_MESH_MODEL_OP_PRIV_BEACON_SET

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_BEACON_STATUS


```


ESP_BLE_MESH_MODEL_OP_PRIV_BEACON_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_GATT_PROXY_GET


```


ESP_BLE_MESH_MODEL_OP_PRIV_GATT_PROXY_GET

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_GATT_PROXY_SET


```


ESP_BLE_MESH_MODEL_OP_PRIV_GATT_PROXY_SET

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_GATT_PROXY_STATUS


```


ESP_BLE_MESH_MODEL_OP_PRIV_GATT_PROXY_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_GET


```


ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_GET

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_SET


```


ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_SET

```

#### ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_STATUS


```


ESP_BLE_MESH_MODEL_OP_PRIV_NODE_IDENTITY_STATUS

```

#### ESP_BLE_MESH_MODEL_PRB_SRV


```


ESP_BLE_MESH_MODEL_PRB_SRV(srv_data)
Define a new Private Beacon Server Model. 

Note
The Private Beacon Server Model can only be included by a Primary Element.


Parameters

srv_data -- Pointer to a unique Private Beacon Server Model user_data.


Returns
New Private Beacon Server Model instance. 



```

#### ESP_BLE_MESH_MODEL_PRB_CLI


```


ESP_BLE_MESH_MODEL_PRB_CLI(cli_data)
Define a new Private Beacon Client Model. 

Note
The Private Beacon Client Model can only be included by a Primary Element.


Parameters

cli_data -- Pointer to a unique struct esp_ble_mesh_client_t.


Returns
New Private Beacon Client Model instance. 



```



### Type Definitions


#### esp_ble_mesh_prb_client_cb_t


```


typedef void (*esp_ble_mesh_prb_client_cb_t)(esp_ble_mesh_prb_client_cb_event_t event, esp_ble_mesh_prb_client_cb_param_t *param)
Bluetooth Mesh Private Beacon Client and Server Model functions. 
Private Beacon Client Model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_prb_server_cb_t


```


typedef void (*esp_ble_mesh_prb_server_cb_t)(esp_ble_mesh_prb_server_cb_event_t event, esp_ble_mesh_prb_server_cb_param_t *param)
Private Beacon Server Model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_prb_client_cb_event_t


```


enum esp_ble_mesh_prb_client_cb_event_t
This enum value is the event of Private Beacon Client Model 
Values:
ESP_BLE_MESH_PRB_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_PRB_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_PRB_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_PRB_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_PRB_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_PRB_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_PRB_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_PRB_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_PRB_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_PRB_CLIENT_EVT_MAX


```

#### esp_ble_mesh_prb_server_cb_event_t


```


enum esp_ble_mesh_prb_server_cb_event_t
This enum value is the event of Private Beacon Server model 
Values:
ESP_BLE_MESH_PRB_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_PRB_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_PRB_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_PRB_SERVER_EVT_MAX


```



### On-Demand Private Proxy




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_odp_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_odp_model_api.h"
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


#### esp_ble_mesh_register_odp_client_callback


```


esp_err_t esp_ble_mesh_register_odp_client_callback(esp_ble_mesh_odp_client_cb_t callback)
Register BLE Mesh On-Demand Private Proxy Config Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_odp_client_send


```


esp_err_t esp_ble_mesh_odp_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_odp_client_msg_t *msg)
Get the value of On-Demand Private Proxy Config Server model state with the corresponding get message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to On-Demand Private Proxy Config Client message.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_odp_server_callback


```


esp_err_t esp_ble_mesh_register_odp_server_callback(esp_ble_mesh_odp_server_cb_t callback)
Register BLE Mesh On-Demand Private Proxy Config Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_odp_client_msg_t


```


union esp_ble_mesh_odp_client_msg_t

#include <esp_ble_mesh_odp_model_api.h>
On-Demand Private Proxy Client model message union. 

Public Members
od_priv_proxy_set

esp_ble_mesh_od_priv_proxy_set_t od_priv_proxy_set
For ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_SET 



```

#### esp_ble_mesh_odp_client_recv_cb_t


```


union esp_ble_mesh_odp_client_recv_cb_t

#include <esp_ble_mesh_odp_model_api.h>
On-Demand Private Proxy Client model received message union. 

Public Members
od_priv_proxy_status

esp_ble_mesh_od_priv_proxy_status_t od_priv_proxy_status
For ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_STATUS 



```

#### esp_ble_mesh_odp_server_state_change_t


```


union esp_ble_mesh_odp_server_state_change_t

#include <esp_ble_mesh_odp_model_api.h>
On-Demand Private Proxy Config Server model related context. 
On-Demand Private Proxy Config Server model state change value union 

Public Members
dummy

uint8_t dummy
Event not used currently 



```

#### esp_ble_mesh_odp_server_cb_value_t


```


union esp_ble_mesh_odp_server_cb_value_t

#include <esp_ble_mesh_odp_model_api.h>
On-Demand Private Proxy Config Server model callback value union. 

Public Members
state_change

esp_ble_mesh_odp_server_state_change_t state_change
For ESP_BLE_MESH_ODP_SERVER_STATE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_odp_srv_t


```


struct esp_ble_mesh_odp_srv_t
On-Demand Private Proxy Config Server model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to On-Demand Private Proxy Config Server model 

on_demand_private_gatt_proxy

uint8_t on_demand_private_gatt_proxy
Duration in seconds of the interval during which advertising with Private Network Identity type is enabled after receiving a Solicitation PDU or after a client disconnect. Note: Binding with the Private GATT Proxy state. 



```

#### esp_ble_mesh_od_priv_proxy_set_t


```


struct esp_ble_mesh_od_priv_proxy_set_t
Parameter of On-Demand Private Proxy Set 

Public Members
gatt_proxy

uint8_t gatt_proxy
On-Demand Private GATT Proxy 



```

#### esp_ble_mesh_od_priv_proxy_status_t


```


struct esp_ble_mesh_od_priv_proxy_status_t
Parameter of On-Demand Private Proxy Status 

Public Members
gatt_proxy

uint8_t gatt_proxy
On-Demand Private GATT Proxy 



```

#### esp_ble_mesh_odp_client_send_cb_t


```


struct esp_ble_mesh_odp_client_send_cb_t
Result of sending On-Demand Private Proxy Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_odp_client_cb_param_t


```


struct esp_ble_mesh_odp_client_cb_param_t
On-Demand Private Proxy Config Client model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
Client common parameters, used by all events 

send

esp_ble_mesh_odp_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_odp_client_recv_cb_t recv
Parameters of received status message 

[anonymous]

union esp_ble_mesh_odp_client_cb_param_t::[anonymous] [anonymous]
Union of ODP Client callback 



```

#### esp_ble_mesh_odp_server_cb_param_t


```


struct esp_ble_mesh_odp_server_cb_param_t
On-Demand Private Proxy Config Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_odp_server_cb_value_t value
Value of the received configuration messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_GET


```


ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_GET

```

#### ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_SET


```


ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_SET

```

#### ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_STATUS


```


ESP_BLE_MESH_MODEL_OP_OD_PRIV_PROXY_STATUS

```

#### ESP_BLE_MESH_MODEL_ODP_SRV


```


ESP_BLE_MESH_MODEL_ODP_SRV(srv_data)
Define a new On-Demand Private Proxy Config Server model. 

Note
The On-Demand Private Proxy Server model is used to represent the ability to enable advertising with Private Network Identity type of a node. This model extends the Mesh Private Beacon Server model. When this model is present on an element, the corresponding Solicitation PDU RPL Configuration Server model shall also be present. The model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

srv_data -- Pointer to a unique On-Demand Private Proxy Config Server model user_data.


Returns
New On-Demand Private Proxy Config Server model instance. 



```

#### ESP_BLE_MESH_MODEL_ODP_CLI


```


ESP_BLE_MESH_MODEL_ODP_CLI(cli_data)
Define a new On-Demand Private Proxy Config Client model. 

Note
The model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

cli_data -- Pointer to a unique On-Demand Private Proxy Config Client model user_data.


Returns
New On-Demand Private Proxy Config Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_odp_client_cb_t


```


typedef void (*esp_ble_mesh_odp_client_cb_t)(esp_ble_mesh_odp_client_cb_event_t event, esp_ble_mesh_odp_client_cb_param_t *param)
Bluetooth Mesh On-Demand Private Proxy Config client and server model functions. 
On-Demand Private Proxy Config Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_odp_server_cb_t


```


typedef void (*esp_ble_mesh_odp_server_cb_t)(esp_ble_mesh_odp_server_cb_event_t event, esp_ble_mesh_odp_server_cb_param_t *param)
On-Demand Private Proxy Config Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_odp_client_cb_event_t


```


enum esp_ble_mesh_odp_client_cb_event_t
This enum value is the event of On-Demand Private Proxy Config Client model 
Values:
ESP_BLE_MESH_ODP_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_ODP_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_ODP_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_ODP_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_ODP_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_ODP_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_ODP_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_ODP_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_ODP_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_ODP_CLIENT_EVT_MAX


```

#### esp_ble_mesh_odp_server_cb_event_t


```


enum esp_ble_mesh_odp_server_cb_event_t
This enum value is the event of On-Demand Private Proxy Config Server model 
Values:
ESP_BLE_MESH_ODP_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_ODP_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_ODP_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_ODP_SERVER_EVT_MAX


```



### SAR Configuration




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_sar_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_sar_model_api.h"
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


#### esp_ble_mesh_register_sar_client_callback


```


esp_err_t esp_ble_mesh_register_sar_client_callback(esp_ble_mesh_sar_client_cb_t callback)
Register BLE Mesh SAR Configuration Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_sar_client_send


```


esp_err_t esp_ble_mesh_sar_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_sar_client_msg_t *msg)
Get the value of SAR Configuration Server model state with the corresponding get message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to SAR Configuration Client message.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_sar_server_callback


```


esp_err_t esp_ble_mesh_register_sar_server_callback(esp_ble_mesh_sar_server_cb_t callback)
Register BLE Mesh SAR Configuration Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_sar_client_msg_t


```


union esp_ble_mesh_sar_client_msg_t

#include <esp_ble_mesh_sar_model_api.h>
SAR Configuration Client model message union. 

Public Members
sar_transmitter_set

esp_ble_mesh_sar_transmitter_set_t sar_transmitter_set
For ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_SET 

sar_receiver_set

esp_ble_mesh_sar_receiver_set_t sar_receiver_set
For ESP_BLE_MESH_MODEL_OP_SAR_RECEIVER_SET 



```

#### esp_ble_mesh_sar_client_recv_cb_t


```


union esp_ble_mesh_sar_client_recv_cb_t

#include <esp_ble_mesh_sar_model_api.h>
SAR Configuration Client model received message union. 

Public Members
sar_transmitter_status

esp_ble_mesh_sar_transmitter_status_t sar_transmitter_status
For ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_STATUS 

sar_receiver_status

esp_ble_mesh_sar_receiver_status_t sar_receiver_status
For ESP_BLE_MESH_MODEL_OP_SAR_RECEIVE_STATUS 



```

#### esp_ble_mesh_sar_server_state_change_t


```


union esp_ble_mesh_sar_server_state_change_t

#include <esp_ble_mesh_sar_model_api.h>
SAR Configuration Server model related context. 
SAR Configuration Server model state change value union 

Public Members
dummy

uint8_t dummy
Event not used currently 



```

#### esp_ble_mesh_sar_server_cb_value_t


```


union esp_ble_mesh_sar_server_cb_value_t

#include <esp_ble_mesh_sar_model_api.h>
SAR Configuration Server model callback value union. 

Public Members
state_change

esp_ble_mesh_sar_server_state_change_t state_change
For ESP_BLE_MESH_SAR_SERVER_STATE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_sar_srv_t


```


struct esp_ble_mesh_sar_srv_t
SAR Configuration Server model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to SAR Configuration Server model 



```

#### esp_ble_mesh_sar_transmitter_set_t


```


struct esp_ble_mesh_sar_transmitter_set_t
Parameters of SAR Transmitter Set 

Public Members
sar_segment_interval_step

uint8_t sar_segment_interval_step
SAR Segment Interval Step state value 

sar_unicast_retrans_count

uint8_t sar_unicast_retrans_count
SAR Unicast Retransmissions Count state 

sar_unicast_retrans_without_progress_count

uint8_t sar_unicast_retrans_without_progress_count
SAR Unicast Retransmissions Without Progress Count state 

sar_unicast_retrans_interval_step

uint8_t sar_unicast_retrans_interval_step
SAR Unicast Retransmissions Interval Step state 

sar_unicast_retrans_interval_increment

uint8_t sar_unicast_retrans_interval_increment
SAR Unicast Retransmissions Interval Increment state 

sar_multicast_retrans_count

uint8_t sar_multicast_retrans_count
SAR Multicast Retransmissions Count state 

sar_multicast_retrans_interval_step

uint8_t sar_multicast_retrans_interval_step
SAR Multicast Retransmissions Interval state 



```

#### esp_ble_mesh_sar_receiver_set_t


```


struct esp_ble_mesh_sar_receiver_set_t
Parameters of SAR Receiver Set 

Public Members
sar_segments_threshold

uint8_t sar_segments_threshold
SAR Segments Threshold state 

sar_ack_delay_increment

uint8_t sar_ack_delay_increment
SAR Acknowledgment Delay Increment state 

sar_discard_timeout

uint8_t sar_discard_timeout
SAR Discard Timeout state 

sar_receiver_segment_interval_step

uint8_t sar_receiver_segment_interval_step
SAR Receiver Segment Interval Step state 

sar_ack_retrans_count

uint8_t sar_ack_retrans_count
SAR Acknowledgment Retransmissions Count state 



```

#### esp_ble_mesh_sar_transmitter_status_t


```


struct esp_ble_mesh_sar_transmitter_status_t
Parameters of SAR Transmitter Status 

Public Members
sar_segment_interval_step

uint8_t sar_segment_interval_step
SAR Segment Interval Step state value 

sar_unicast_retrans_count

uint8_t sar_unicast_retrans_count
SAR Unicast Retransmissions Count state 

sar_unicast_retrans_without_progress_count

uint8_t sar_unicast_retrans_without_progress_count
SAR Unicast Retransmissions Without Progress Count state 

sar_unicast_retrans_interval_step

uint8_t sar_unicast_retrans_interval_step
SAR Unicast Retransmissions Interval Step state 

sar_unicast_retrans_interval_increment

uint8_t sar_unicast_retrans_interval_increment
SAR Unicast Retransmissions Interval Increment state 

sar_multicast_retrans_count

uint8_t sar_multicast_retrans_count
SAR Multicast Retransmissions Count state 

sar_multicast_retrans_interval_step

uint8_t sar_multicast_retrans_interval_step
SAR Multicast Retransmissions Interval state 



```

#### esp_ble_mesh_sar_receiver_status_t


```


struct esp_ble_mesh_sar_receiver_status_t
Parameters of SAR Receiver Status 

Public Members
sar_segments_threshold

uint8_t sar_segments_threshold
SAR Segments Threshold state 

sar_ack_delay_increment

uint8_t sar_ack_delay_increment
SAR Acknowledgment Delay Increment state 

sar_discard_timeout

uint8_t sar_discard_timeout
SAR Discard Timeout state 

sar_receiver_segment_interval_step

uint8_t sar_receiver_segment_interval_step
SAR Receiver Segment Interval Step state 

sar_ack_retrans_count

uint8_t sar_ack_retrans_count
SAR Acknowledgment Retransmissions Count state 



```

#### esp_ble_mesh_sar_client_send_cb_t


```


struct esp_ble_mesh_sar_client_send_cb_t
Result of sending SAR Configuration Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_sar_client_cb_param_t


```


struct esp_ble_mesh_sar_client_cb_param_t
SAR Configuration Client model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
Client common parameters, used by all events. 

send

esp_ble_mesh_sar_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_sar_client_recv_cb_t recv
Parameters of received status message 

[anonymous]

union esp_ble_mesh_sar_client_cb_param_t::[anonymous] [anonymous]
Union of SAR Client callback 



```

#### esp_ble_mesh_sar_server_cb_param_t


```


struct esp_ble_mesh_sar_server_cb_param_t
SAR Configuration Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_sar_server_cb_value_t value
Value of the received configuration messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_GET


```


ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_GET

```

#### ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_SET


```


ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_SET

```

#### ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_STATUS


```


ESP_BLE_MESH_MODEL_OP_SAR_TRANSMITTER_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_SAR_RECEIVER_GET


```


ESP_BLE_MESH_MODEL_OP_SAR_RECEIVER_GET

```

#### ESP_BLE_MESH_MODEL_OP_SAR_RECEIVER_SET


```


ESP_BLE_MESH_MODEL_OP_SAR_RECEIVER_SET

```

#### ESP_BLE_MESH_MODEL_OP_SAR_RECEIVER_STATUS


```


ESP_BLE_MESH_MODEL_OP_SAR_RECEIVER_STATUS

```

#### ESP_BLE_MESH_MODEL_SAR_SRV


```


ESP_BLE_MESH_MODEL_SAR_SRV(srv_data)
Define a new SAR Configuration Server model. 

Note
If supported, the model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

srv_data -- Pointer to a unique SAR Configuration Server model user_data.


Returns
New SAR Configuration Server model instance. 



```

#### ESP_BLE_MESH_MODEL_SAR_CLI


```


ESP_BLE_MESH_MODEL_SAR_CLI(cli_data)
Define a new SAR Configuration Client model. 

Note
If supported, the model shall be supported by the primary element and shall not be supported by any secondary elements.


Parameters

cli_data -- Pointer to a unique SAR Configuration Client model user_data.


Returns
New SAR Configuration Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_sar_client_cb_t


```


typedef void (*esp_ble_mesh_sar_client_cb_t)(esp_ble_mesh_sar_client_cb_event_t event, esp_ble_mesh_sar_client_cb_param_t *param)
Bluetooth Mesh SAR Configuration client and server model functions. 
SAR Configuration Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_sar_server_cb_t


```


typedef void (*esp_ble_mesh_sar_server_cb_t)(esp_ble_mesh_sar_server_cb_event_t event, esp_ble_mesh_sar_server_cb_param_t *param)
SAR Configuration Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_sar_client_cb_event_t


```


enum esp_ble_mesh_sar_client_cb_event_t
This enum value is the event of SAR Configuration Client model 
Values:
ESP_BLE_MESH_SAR_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_SAR_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_SAR_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_SAR_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_SAR_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_SAR_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_SAR_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_SAR_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_SAR_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_SAR_CLIENT_EVT_MAX


```

#### esp_ble_mesh_sar_server_cb_event_t


```


enum esp_ble_mesh_sar_server_cb_event_t
This enum value is the event of SAR Configuration Server model 
Values:
ESP_BLE_MESH_SAR_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_SAR_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_SAR_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_SAR_SERVER_EVT_MAX


```



### Solicitation PDU RPL Configuration




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_srpl_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_srpl_model_api.h"
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


#### esp_ble_mesh_register_srpl_client_callback


```


esp_err_t esp_ble_mesh_register_srpl_client_callback(esp_ble_mesh_srpl_client_cb_t callback)
Register BLE Mesh Solicitation PDU RPL Configuration Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_srpl_client_send


```


esp_err_t esp_ble_mesh_srpl_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_srpl_client_msg_t *msg)
Set the value of Solicitation PDU RPL Configuration Server model state with the corresponding set message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to Solicitation PDU RPL Configuration Client message.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_srpl_server_callback


```


esp_err_t esp_ble_mesh_register_srpl_server_callback(esp_ble_mesh_srpl_server_cb_t callback)
Register BLE Mesh Solicitation PDU RPL Configuration Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_srpl_client_msg_t


```


union esp_ble_mesh_srpl_client_msg_t

#include <esp_ble_mesh_srpl_model_api.h>
Solicitation PDU RPL Configuration Client model message union. 

Public Members
srpl_items_clear

esp_ble_mesh_srpl_items_clear_t srpl_items_clear
For ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_CLEAR 



```

#### esp_ble_mesh_srpl_client_recv_cb_t


```


union esp_ble_mesh_srpl_client_recv_cb_t

#include <esp_ble_mesh_srpl_model_api.h>
Solicitation PDU RPL Configuration Client model received message union. 

Public Members
srpl_items_status

esp_ble_mesh_srpl_items_status_t srpl_items_status
For ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_STATUS 



```

#### esp_ble_mesh_srpl_server_state_change_t


```


union esp_ble_mesh_srpl_server_state_change_t

#include <esp_ble_mesh_srpl_model_api.h>
Solicitation PDU RPL Configuration Server model related context. 
Solicitation PDU RPL Configuration Server model state change value union 

Public Members
dummy

uint8_t dummy
Currently this event is not used. 



```

#### esp_ble_mesh_srpl_server_cb_value_t


```


union esp_ble_mesh_srpl_server_cb_value_t

#include <esp_ble_mesh_srpl_model_api.h>
Solicitation PDU RPL Configuration Server model callback value union. 

Public Members
state_change

esp_ble_mesh_srpl_server_state_change_t state_change
ESP_BLE_MESH_SRPL_SERVER_STATE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_srpl_srv_t


```


struct esp_ble_mesh_srpl_srv_t
Solicitation PDU RPL Configuration Server model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Solicitation PDU RPL Configuration Server model 



```

#### esp_ble_mesh_srpl_items_clear_t


```


struct esp_ble_mesh_srpl_items_clear_t
Parameter of Solicitation PDU RPL Items Clear 

Public Members
addr_range

esp_ble_mesh_uar_t addr_range
Unicast address range 



```

#### esp_ble_mesh_srpl_items_status_t


```


struct esp_ble_mesh_srpl_items_status_t
Parameter of Solicitation PDU RPL Items Clear Status 

Public Members
addr_range

esp_ble_mesh_uar_t addr_range
Unicast address range 



```

#### esp_ble_mesh_srpl_client_send_cb_t


```


struct esp_ble_mesh_srpl_client_send_cb_t
Result of sending Solicitation PDU RPL Configuration Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_srpl_client_cb_param_t


```


struct esp_ble_mesh_srpl_client_cb_param_t
Solicitation PDU RPL Configuration Client model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
Client common parameters, used by all events. 

send

esp_ble_mesh_srpl_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_srpl_client_recv_cb_t recv
Parameters of received status message 

[anonymous]

union esp_ble_mesh_srpl_client_cb_param_t::[anonymous] [anonymous]
Union of SRPL Client callback 



```

#### esp_ble_mesh_srpl_server_cb_param_t


```


struct esp_ble_mesh_srpl_server_cb_param_t
Solicitation PDU RPL Configuration Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_srpl_server_cb_value_t value
Value of the received configuration messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_CLEAR


```


ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_CLEAR

```

#### ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_CLEAR_UNACK


```


ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_CLEAR_UNACK

```

#### ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_STATUS


```


ESP_BLE_MESH_MODEL_OP_SRPL_ITEMS_STATUS

```

#### ESP_BLE_MESH_MODEL_SRPL_SRV


```


ESP_BLE_MESH_MODEL_SRPL_SRV(srv_data)
Define a new Solicitation PDU RPL Configuration Server model. 

Note
The Solicitation PDU RPL Configuration Server model extends the On-Demand Private Proxy Server model. If the model is supported, the model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

srv_data -- Pointer to a unique Solicitation PDU RPL Configuration Server model user_data.


Returns
New Solicitation PDU RPL Configuration Server model instance. 



```

#### ESP_BLE_MESH_MODEL_SRPL_CLI


```


ESP_BLE_MESH_MODEL_SRPL_CLI(cli_data)
Define a new Solicitation PDU RPL Configuration Client model. 

Note
If supported, the model shall be supported by the primary element and shall not be supported by any secondary elements.


Parameters

cli_data -- Pointer to a unique Solicitation PDU RPL Configuration Client model user_data.


Returns
New Solicitation PDU RPL Configuration Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_srpl_client_cb_t


```


typedef void (*esp_ble_mesh_srpl_client_cb_t)(esp_ble_mesh_srpl_client_cb_event_t event, esp_ble_mesh_srpl_client_cb_param_t *param)
Bluetooth Mesh Solicitation PDU RPL Configuration client and server model functions. 
Solicitation PDU RPL Configuration Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_srpl_server_cb_t


```


typedef void (*esp_ble_mesh_srpl_server_cb_t)(esp_ble_mesh_srpl_server_cb_event_t event, esp_ble_mesh_srpl_server_cb_param_t *param)
Solicitation PDU RPL Configuration Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_srpl_client_cb_event_t


```


enum esp_ble_mesh_srpl_client_cb_event_t
This enum value is the event of Solicitation PDU RPL Configuration Client model 
Values:
ESP_BLE_MESH_SRPL_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_SRPL_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_SRPL_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_SRPL_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_SRPL_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_SRPL_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_SRPL_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_SRPL_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_SRPL_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_SRPL_CLIENT_EVT_MAX


```

#### esp_ble_mesh_srpl_server_cb_event_t


```


enum esp_ble_mesh_srpl_server_cb_event_t
This enum value is the event of Solicitation PDU RPL Configuration Server model 
Values:
ESP_BLE_MESH_SRPL_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_SRPL_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_SRPL_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_SRPL_SERVER_EVT_MAX


```



### Opcodes Aggregator




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_agg_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_agg_model_api.h"
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


#### esp_ble_mesh_register_agg_client_callback


```


esp_err_t esp_ble_mesh_register_agg_client_callback(esp_ble_mesh_agg_client_cb_t callback)
Register BLE Mesh Opcodes Aggregator Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_agg_client_send


```


esp_err_t esp_ble_mesh_agg_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_agg_client_msg_t *msg)
Set the value of Opcodes Aggregator Server model state with the corresponding set message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to Opcodes Aggregator Client message.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_agg_server_callback


```


esp_err_t esp_ble_mesh_register_agg_server_callback(esp_ble_mesh_agg_server_cb_t callback)
Register BLE Mesh Opcodes Aggregator Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_agg_client_msg_t


```


union esp_ble_mesh_agg_client_msg_t

#include <esp_ble_mesh_agg_model_api.h>
Opcodes Aggregator Client model message union. 

Public Members
agg_sequence

esp_ble_mesh_agg_sequence_t agg_sequence
For ESP_BLE_MESH_MODEL_OP_AGG_SEQUENCE 



```

#### esp_ble_mesh_agg_client_recv_cb_t


```


union esp_ble_mesh_agg_client_recv_cb_t

#include <esp_ble_mesh_agg_model_api.h>
Opcodes Aggregator Client model received message union. 

Public Members
agg_status

esp_ble_mesh_agg_status_t agg_status
For ESP_BLE_MESH_MODEL_OP_AGG_STATUS 



```

#### esp_ble_mesh_agg_server_recv_msg_t


```


union esp_ble_mesh_agg_server_recv_msg_t

#include <esp_ble_mesh_agg_model_api.h>
Opcodes Aggregator Server model related context. 
Opcodes Aggregator Server model received message union 

Public Members
agg_sequence

esp_ble_mesh_agg_sequence_t agg_sequence
For ESP_BLE_MESH_MODEL_OP_AGG_SEQUENCE 



```



### Structures


#### esp_ble_mesh_agg_srv_t


```


struct esp_ble_mesh_agg_srv_t
Opcodes Aggregator Server model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Opcodes Aggregator Server model 



```

#### esp_ble_mesh_agg_item_t


```


struct esp_ble_mesh_agg_item_t
Parameters of Aggregator Item 

Public Members
length_format

uint16_t length_format
0: Length_Short; 1: Length_Long 

length

uint16_t length
Size of Opcode_And_Parameters field 

data

const uint8_t *data
Opcode and parameters 



```

#### esp_ble_mesh_agg_sequence_t


```


struct esp_ble_mesh_agg_sequence_t
Parameters of Opcodes Aggregator Sequence 

Public Members
element_addr

uint16_t element_addr
Element address 

items

struct net_buf_simple *items
List of items with each item represented as an Aggregator Item 



```

#### esp_ble_mesh_agg_status_t


```


struct esp_ble_mesh_agg_status_t
Parameters of Opcodes Aggregator Status 

Public Members
status

uint8_t status
Status of the most recent operation 

element_addr

uint16_t element_addr
Element Address 

items

struct net_buf_simple *items
List of status items with each status item containing an unacknowledged access layer message or empty item (Optional) 



```

#### esp_ble_mesh_agg_client_send_cb_t


```


struct esp_ble_mesh_agg_client_send_cb_t
Result of sending Opcodes Aggregator Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_agg_client_cb_param_t


```


struct esp_ble_mesh_agg_client_cb_param_t
Opcodes Aggregator Client model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
Client common parameters, used by all events 

send

esp_ble_mesh_agg_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_agg_client_recv_cb_t recv
Parameters of received status message 

[anonymous]

union esp_ble_mesh_agg_client_cb_param_t::[anonymous] [anonymous]
Union of AGG Client callback 



```

#### esp_ble_mesh_agg_server_cb_param_t


```


struct esp_ble_mesh_agg_server_cb_param_t
Opcodes Aggregator Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

recv

esp_ble_mesh_agg_server_recv_msg_t recv
Received message callback values 

[anonymous]

union esp_ble_mesh_agg_server_cb_param_t::[anonymous] [anonymous]
Union of AGG Server callback 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_AGG_SEQUENCE


```


ESP_BLE_MESH_MODEL_OP_AGG_SEQUENCE
Defines the Opcodes Aggregator message opcode. 

```

#### ESP_BLE_MESH_MODEL_OP_AGG_STATUS


```


ESP_BLE_MESH_MODEL_OP_AGG_STATUS

```

#### ESP_BLE_MESH_AGG_STATUS_SUCCESS


```


ESP_BLE_MESH_AGG_STATUS_SUCCESS
Defines the status codes for Opcodes Aggregator messages. 

```

#### ESP_BLE_MESH_AGG_STATUS_INVALID_ADDRESS


```


ESP_BLE_MESH_AGG_STATUS_INVALID_ADDRESS

```

#### ESP_BLE_MESH_AGG_STATUS_INVALID_MODEL


```


ESP_BLE_MESH_AGG_STATUS_INVALID_MODEL

```

#### ESP_BLE_MESH_AGG_STATUS_WRONG_ACCESS_KEY


```


ESP_BLE_MESH_AGG_STATUS_WRONG_ACCESS_KEY

```

#### ESP_BLE_MESH_AGG_STATUS_WRONG_OPCODE


```


ESP_BLE_MESH_AGG_STATUS_WRONG_OPCODE

```

#### ESP_BLE_MESH_AGG_STATUS_MSG_NOT_UNDERSTOOD


```


ESP_BLE_MESH_AGG_STATUS_MSG_NOT_UNDERSTOOD

```

#### ESP_BLE_MESH_AGG_ITEM_LENGTH_FORMAT_SHORT


```


ESP_BLE_MESH_AGG_ITEM_LENGTH_FORMAT_SHORT
Values of the Length_Format 

```

#### ESP_BLE_MESH_AGG_ITEM_LENGTH_FORMAT_LONG


```


ESP_BLE_MESH_AGG_ITEM_LENGTH_FORMAT_LONG

```

#### ESP_BLE_MESH_MODEL_AGG_SRV


```


ESP_BLE_MESH_MODEL_AGG_SRV(srv_data)
Define a new Opcodes Aggregator Server model. 

Note
If supported, the Opcodes Aggregator Server model shall be supported by a primary element.


Parameters

srv_data -- Pointer to a unique Opcodes Aggregator Server model user_data.


Returns
New Opcodes Aggregator Server model instance. 



```

#### ESP_BLE_MESH_MODEL_AGG_CLI


```


ESP_BLE_MESH_MODEL_AGG_CLI(cli_data)
Define a new Opcodes Aggregator Client model. 

Note
If supported, the model shall be supported by the primary element and shall not be supported by any secondary elements.


Parameters

cli_data -- Pointer to a unique Opcodes Aggregator Client model user_data.


Returns
New Opcodes Aggregator Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_agg_client_cb_t


```


typedef void (*esp_ble_mesh_agg_client_cb_t)(esp_ble_mesh_agg_client_cb_event_t event, esp_ble_mesh_agg_client_cb_param_t *param)
Bluetooth Mesh Opcodes Aggregator client and server model functions. 
Opcodes Aggregator Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_agg_server_cb_t


```


typedef void (*esp_ble_mesh_agg_server_cb_t)(esp_ble_mesh_agg_server_cb_event_t event, esp_ble_mesh_agg_server_cb_param_t *param)
Opcodes Aggregator Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_agg_client_cb_event_t


```


enum esp_ble_mesh_agg_client_cb_event_t
This enum value is the event of Opcodes Aggregator Client model 
Values:
ESP_BLE_MESH_AGG_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_AGG_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_AGG_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_AGG_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_AGG_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_AGG_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_AGG_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_AGG_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_AGG_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_AGG_CLIENT_EVT_MAX


```

#### esp_ble_mesh_agg_server_cb_event_t


```


enum esp_ble_mesh_agg_server_cb_event_t
This enum value is the event of Opcodes Aggregator Server model 
Values:
ESP_BLE_MESH_AGG_SERVER_RECV_MSG_EVT

enumerator ESP_BLE_MESH_AGG_SERVER_RECV_MSG_EVT

ESP_BLE_MESH_AGG_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_AGG_SERVER_EVT_MAX


```



### Large Composition Data




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_lcd_model_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_lcd_model_api.h"
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


#### esp_ble_mesh_register_lcd_client_callback


```


esp_err_t esp_ble_mesh_register_lcd_client_callback(esp_ble_mesh_lcd_client_cb_t callback)
Register BLE Mesh Large Composition Data Client model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_lcd_client_send


```


esp_err_t esp_ble_mesh_lcd_client_send(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_lcd_client_msg_t *msg)
Get the value of Large Composition Data Server model state with the corresponding get message. 

Parameters

params -- [in] Pointer to BLE Mesh common client parameters. 
msg -- [in] Pointer to Large Composition Data Client message.


Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_register_lcd_server_callback


```


esp_err_t esp_ble_mesh_register_lcd_server_callback(esp_ble_mesh_lcd_server_cb_t callback)
Register BLE Mesh Large Composition Data Server model callback. 

Parameters
callback -- [in] Pointer to the callback function.

Returns
ESP_OK on success or error code otherwise. 



```



### Unions


#### esp_ble_mesh_lcd_client_msg_t


```


union esp_ble_mesh_lcd_client_msg_t

#include <esp_ble_mesh_lcd_model_api.h>
Large Composition Data Client model message union. 

Public Members
large_comp_data_get

esp_ble_mesh_large_comp_data_get_t large_comp_data_get
For ESP_BLE_MESH_MODEL_OP_LARGE_COMP_DATA_GET 

models_metadata_get

esp_ble_mesh_models_metadata_get_t models_metadata_get
For ESP_BLE_MESH_MODEL_OP_MODELS_METADATA_GET 



```

#### esp_ble_mesh_lcd_client_recv_cb_t


```


union esp_ble_mesh_lcd_client_recv_cb_t

#include <esp_ble_mesh_lcd_model_api.h>
Large Composition Data Client model received message union. 

Public Members
large_comp_data_status

esp_ble_mesh_large_comp_data_status_t large_comp_data_status
For ESP_BLE_MESH_MODEL_OP_LARGE_COMP_DATA_STATUS 

models_metadata_status

esp_ble_mesh_models_metadata_status_t models_metadata_status
For ESP_BLE_MESH_MODEL_OP_MODELS_METADATA_STATUS 



```

#### esp_ble_mesh_lcd_server_state_change_t


```


union esp_ble_mesh_lcd_server_state_change_t

#include <esp_ble_mesh_lcd_model_api.h>
Large Composition Data Server model related context. 
Large Composition Data Server model state change value union 

Public Members
dummy

uint8_t dummy
Event not used currently 



```

#### esp_ble_mesh_lcd_server_cb_value_t


```


union esp_ble_mesh_lcd_server_cb_value_t

#include <esp_ble_mesh_lcd_model_api.h>
Large Composition Data Server model callback value union. 

Public Members
state_change

esp_ble_mesh_lcd_server_state_change_t state_change
For ESP_BLE_MESH_LCD_SERVER_STATE_CHANGE_EVT 



```



### Structures


#### esp_ble_mesh_lcd_srv_t


```


struct esp_ble_mesh_lcd_srv_t
Large Composition Data Server model context 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to Large Composition Data Server model 



```

#### esp_ble_mesh_large_comp_data_get_t


```


struct esp_ble_mesh_large_comp_data_get_t
Parameters of Large Composition Data Get 

Public Members
page

uint8_t page
Page number of the Composition Data 

offset

uint16_t offset
Offset within the page 



```

#### esp_ble_mesh_models_metadata_get_t


```


struct esp_ble_mesh_models_metadata_get_t
Parameters of Models Metadata Get 

Public Members
metadata_page

uint8_t metadata_page
Page number of the Models Metadata 

offset

uint16_t offset
Offset within the page 



```

#### esp_ble_mesh_large_comp_data_status_t


```


struct esp_ble_mesh_large_comp_data_status_t
Parameters of Large Composition Data Status 

Public Members
page

uint8_t page
Page number of the Composition Data 

offset

uint16_t offset
Offset within the page 

total_size

uint16_t total_size
Total size of the page 

data

struct net_buf_simple *data
Composition Data for the identified portion of the page 



```

#### esp_ble_mesh_models_metadata_status_t


```


struct esp_ble_mesh_models_metadata_status_t
Parameters of Models Metadata Data Status 

Public Members
metadata_page

uint8_t metadata_page
Page number of the Models Metadata 

offset

uint16_t offset
Offset within the page 

total_size

uint16_t total_size
Total size of the page 

data

struct net_buf_simple *data
Models Metadata for the identified portion of the page 



```

#### esp_ble_mesh_lcd_client_send_cb_t


```


struct esp_ble_mesh_lcd_client_send_cb_t
Result of sending Large Composition Data Client messages 

Public Members
err_code

int err_code
Result of sending a message 



```

#### esp_ble_mesh_lcd_client_cb_param_t


```


struct esp_ble_mesh_lcd_client_cb_param_t
Large Composition Data Client model callback parameters 

Public Members
params

esp_ble_mesh_client_common_param_t *params
Client common parameters, used by all events. 

send

esp_ble_mesh_lcd_client_send_cb_t send
Result of sending a message 

recv

esp_ble_mesh_lcd_client_recv_cb_t recv
Parameters of received status message 

[anonymous]

union esp_ble_mesh_lcd_client_cb_param_t::[anonymous] [anonymous]
Union of LCD Client callback 



```

#### esp_ble_mesh_lcd_server_cb_param_t


```


struct esp_ble_mesh_lcd_server_cb_param_t
Large Composition Data Server model callback parameters 

Public Members
model

esp_ble_mesh_model_t *model
Pointer to the server model structure 

ctx

esp_ble_mesh_msg_ctx_t ctx
Context of the received message 

value

esp_ble_mesh_lcd_server_cb_value_t value
Value of the received configuration messages 



```



### Macros


#### ESP_BLE_MESH_MODEL_OP_LARGE_COMP_DATA_GET


```


ESP_BLE_MESH_MODEL_OP_LARGE_COMP_DATA_GET

```

#### ESP_BLE_MESH_MODEL_OP_LARGE_COMP_DATA_STATUS


```


ESP_BLE_MESH_MODEL_OP_LARGE_COMP_DATA_STATUS

```

#### ESP_BLE_MESH_MODEL_OP_MODELS_METADATA_GET


```


ESP_BLE_MESH_MODEL_OP_MODELS_METADATA_GET

```

#### ESP_BLE_MESH_MODEL_OP_MODELS_METADATA_STATUS


```


ESP_BLE_MESH_MODEL_OP_MODELS_METADATA_STATUS

```

#### ESP_BLE_MESH_MODEL_LCD_SRV


```


ESP_BLE_MESH_MODEL_LCD_SRV(srv_data)
Define a new Large Composition Data Server model. 

Note
If supported, the model shall be supported by a primary element and shall not be supported by any secondary elements.


Parameters

srv_data -- Pointer to a unique Large Composition Data Server model user_data.


Returns
New Large Composition Data Server model instance. 



```

#### ESP_BLE_MESH_MODEL_LCD_CLI


```


ESP_BLE_MESH_MODEL_LCD_CLI(cli_data)
Define a new Large Composition Data Client model. 

Note
If supported, the model shall be supported by the primary element and shall not be supported by any secondary elements.


Parameters

cli_data -- Pointer to a unique Large Composition Data Client model user_data.


Returns
New Large Composition Data Client model instance. 



```



### Type Definitions


#### esp_ble_mesh_lcd_client_cb_t


```


typedef void (*esp_ble_mesh_lcd_client_cb_t)(esp_ble_mesh_lcd_client_cb_event_t event, esp_ble_mesh_lcd_client_cb_param_t *param)
Large Composition Data client and server model functions. 
Large Composition Data Client model callback function type 

Param event
Event type 

Param param
Pointer to callback parameter 



```

#### esp_ble_mesh_lcd_server_cb_t


```


typedef void (*esp_ble_mesh_lcd_server_cb_t)(esp_ble_mesh_lcd_server_cb_event_t event, esp_ble_mesh_lcd_server_cb_param_t *param)
Large Composition Data Server model callback function type. 

Param event
Event type 

Param param
Pointer to callback parameter 



```



### Enumerations


#### esp_ble_mesh_lcd_client_cb_event_t


```


enum esp_ble_mesh_lcd_client_cb_event_t
This enum value is the event of Large Composition Data Client model 
Values:
ESP_BLE_MESH_LCD_CLIENT_SEND_COMP_EVT

enumerator ESP_BLE_MESH_LCD_CLIENT_SEND_COMP_EVT

ESP_BLE_MESH_LCD_CLIENT_SEND_TIMEOUT_EVT

enumerator ESP_BLE_MESH_LCD_CLIENT_SEND_TIMEOUT_EVT

ESP_BLE_MESH_LCD_CLIENT_RECV_RSP_EVT

enumerator ESP_BLE_MESH_LCD_CLIENT_RECV_RSP_EVT

ESP_BLE_MESH_LCD_CLIENT_RECV_PUB_EVT

enumerator ESP_BLE_MESH_LCD_CLIENT_RECV_PUB_EVT

ESP_BLE_MESH_LCD_CLIENT_EVT_MAX

enumerator ESP_BLE_MESH_LCD_CLIENT_EVT_MAX


```

#### esp_ble_mesh_lcd_server_cb_event_t


```


enum esp_ble_mesh_lcd_server_cb_event_t
This enum value is the event of Large Composition Data Server model 
Values:
ESP_BLE_MESH_LCD_SERVER_STATE_CHANGE_EVT

enumerator ESP_BLE_MESH_LCD_SERVER_STATE_CHANGE_EVT

ESP_BLE_MESH_LCD_SERVER_EVT_MAX

enumerator ESP_BLE_MESH_LCD_SERVER_EVT_MAX


```



### Composition and Metadata




### Header File


* components/bt/esp_ble_mesh/v1.1/api/core/include/esp_ble_mesh_cm_data_api.h
* This header file can be included with:



> ```
> #include "esp_ble_mesh_cm_data_api.h"
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


#### esp_ble_mesh_comp_1_register


```


esp_err_t esp_ble_mesh_comp_1_register(const esp_ble_mesh_comp_1_t *comp)
Register Composition Data Page 1. 

Parameters
comp -- [in] Pointer to Composition Data Page 1.

Returns
ESP_OK on success or error code otherwise. 



```

#### esp_ble_mesh_models_metadata_register


```


esp_err_t esp_ble_mesh_models_metadata_register(const esp_ble_mesh_models_metadata_t *metadata, uint8_t metadata_page)
Register Models Metadata Page 0 or 128. 

Parameters

metadata -- [in] Pointer to Models Metadata Page 0 or 128. 
metadata_page -- [in] Models Metadata Page number, i.e. 0 or 128.


Returns
ESP_OK on success or error code otherwise. 



```



### Structures


#### esp_ble_mesh_extended_model_item_t


```


struct esp_ble_mesh_extended_model_item_t
Format of Extended Model Item 

Public Members
element_offset

uint8_t element_offset
Element address modifier, in the range -4 to 3. See above. 

model_item_idx

uint8_t model_item_idx
Model Index, in the range 0 to 31
Model index, in the range 0 to 255 

element_offset

int8_t element_offset
Element address modifier, in the range -128 to 127 

long_fmt

struct esp_ble_mesh_extended_model_item_t::[anonymous]::[anonymous] long_fmt
Extended Model Item long format Extended Model Item long format 

[anonymous]

union esp_ble_mesh_extended_model_item_t::[anonymous] [anonymous]
Union of Extended Model Item 



```

#### esp_ble_mesh_model_item_t


```


struct esp_ble_mesh_model_item_t
Format of Model Item 

Public Members
corresponding_present

uint8_t corresponding_present
Corresponding_Group_ID field indicator 

format

uint8_t format
Format of Extended_Model_Items indicator 

extended_items_count

uint8_t extended_items_count
Number of Extended Model Items in the Extended_Model_Items field 

corresponding_group_id

uint8_t corresponding_group_id
Corresponding group identifier 

extended_model_items

esp_ble_mesh_extended_model_item_t *const extended_model_items
List of Extended Model Items 



```

#### esp_ble_mesh_comp_1_elem_t


```


struct esp_ble_mesh_comp_1_elem_t
Format of element of Composition Data Page 1 

Public Members
num_s

const uint8_t num_s
A count of SIG Models Items in this element 

num_v

const uint8_t num_v
A count of Vendor Models Items in this element 

model_items_s

esp_ble_mesh_model_item_t *const model_items_s
A sequence of "num_s" SIG Model Items 

model_items_v

esp_ble_mesh_model_item_t *const model_items_v
A sequence of "num_v" Vendor Model Items 



```

#### esp_ble_mesh_comp_1_t


```


struct esp_ble_mesh_comp_1_t
Format of Composition Data Page 1 

Public Members
element_count

size_t element_count
Element count 

elements

esp_ble_mesh_comp_1_elem_t *elements
A sequence of element descriptions 



```

#### esp_ble_mesh_metadata_entry_t


```


struct esp_ble_mesh_metadata_entry_t
Format of Metadata entry 

Public Members
metadata_len

uint16_t metadata_len
Size of the Metadata field 

metadata_id

uint16_t metadata_id
Bluetooth assigned number for the Metadata Identifier 

metadata

const uint8_t *metadata
Model’s metadata 



```

#### esp_ble_mesh_metadata_item_t


```


struct esp_ble_mesh_metadata_item_t
Format of Metadata item 

Public Members
model_id

uint16_t model_id
Model ID 

company_id

uint16_t company_id
Company ID 

vnd

struct esp_ble_mesh_metadata_item_t::[anonymous]::[anonymous] vnd
Vendor model identifier Vendor model identifier 

[anonymous]

union esp_ble_mesh_metadata_item_t::[anonymous] [anonymous]
Union of model ID 

metadata_entries_num

uint8_t metadata_entries_num
Number of metadata entries 

metadata_entries

esp_ble_mesh_metadata_entry_t *const metadata_entries
List of model’s metadata 



```

#### esp_ble_mesh_metadata_elem_t


```


struct esp_ble_mesh_metadata_elem_t
Format of Metadata element of Models Metadata Page 0/128 

Public Members
items_num_s

const uint8_t items_num_s
Number of metadata items for SIG models in the element 

items_num_v

const uint8_t items_num_v
Number of metadata items for Vendor models in the element 

metadata_items_s

esp_ble_mesh_metadata_item_t *const metadata_items_s
List of metadata items for SIG models in the element 

metadata_items_v

esp_ble_mesh_metadata_item_t *const metadata_items_v
List of metadata items for Vendor models in the element 



```

#### esp_ble_mesh_models_metadata_t


```


struct esp_ble_mesh_models_metadata_t
Format of the Models Metadata Page 0/128 

Public Members
element_count

size_t element_count
Element count 

elements

esp_ble_mesh_metadata_elem_t *elements
List of metadata for models for each element 



```



### Macros


#### ESP_BLE_MESH_MODEL_ITEM_SHORT


```


ESP_BLE_MESH_MODEL_ITEM_SHORT
< Definitions of the format of Extended_Model_Items indicator 

```

#### ESP_BLE_MESH_MODEL_ITEM_LONG


```


ESP_BLE_MESH_MODEL_ITEM_LONG

```