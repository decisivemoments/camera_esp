# ESP HTTPS OTA




## Overview


`esp_https_ota` provides simplified APIs to perform firmware upgrades over HTTPS. It is an abstraction layer over the existing OTA APIs.




## Application Example



> ```
> esp_err_t do_firmware_upgrade()
> {
>     esp_http_client_config_t config = {
>         .url = CONFIG_FIRMWARE_UPGRADE_URL,
>         .cert_pem = (char *)server_cert_pem_start,
>     };
>     esp_https_ota_config_t ota_config = {
>         .http_config = &config,
>     };
>     esp_err_t ret = esp_https_ota(&ota_config);
>     if (ret == ESP_OK) {
>         esp_restart();
>     } else {
>         return ESP_FAIL;
>     }
>     return ESP_OK;
> }
> 
> ```




## Server Verification


Please refer to ESP-TLS: TLS Server Verification for more information on server verification. The root certificate in PEM format needs to be provided to the `esp_http_client_config_t::cert_pem` member.



Note


The server-endpoint **root** certificate should be used for verification instead of any intermediate ones from the certificate chain. The reason is that the root certificate has the maximum validity and usually remains the same for a long period of time. Users can also use the `esp_http_client_config_t::crt_bundle_attach` member for verification by the `ESP x509 Certificate Bundle` feature, which covers most of the trusted root certificates.





## Partial Image Download over HTTPS


To use the partial image download feature, enable `partial_http_download` configuration in `esp_https_ota_config_t`. When this configuration is enabled, firmware image will be downloaded in multiple HTTP requests of specified sizes. Maximum content length of each request can be specified by setting `max_http_request_size` to the required value.


This option is useful while fetching image from a service like AWS S3, where mbedTLS Rx buffer size (CONFIG_MBEDTLS_SSL_IN_CONTENT_LEN) can be set to a lower value which is not possible without enabling this configuration.


Default value of mbedTLS Rx buffer size is set to 16 KB. By using `partial_http_download` with `max_http_request_size` of 4 KB, size of mbedTLS Rx buffer can be reduced to 4 KB. With this configuration, memory saving of around 12 KB is expected.




## Signature Verification


For additional security, signature of OTA firmware images can be verified. For more information， please refer to Secure OTA Updates Without Secure Boot.




## Advanced APIs


`esp_https_ota` also provides advanced APIs which can be used if more information and control is needed during the OTA process.


Example that uses advanced ESP_HTTPS_OTA APIs: system/ota/advanced_https_ota.




## OTA Upgrades with Pre-Encrypted Firmware


To perform OTA upgrades with pre-encrypted firmware, please enable CONFIG_ESP_HTTPS_OTA_DECRYPT_CB in component menuconfig.


Example that performs OTA upgrade with pre-encrypted firmware: system/ota/pre_encrypted_ota.




## OTA System Events


ESP HTTPS OTA has various events for which a handler can be triggered by the Event Loop Library when the particular event occurs. The handler has to be registered using `esp_event_handler_register()`. This helps the event handling for ESP HTTPS OTA.


`esp_https_ota_event_t` has all the events which can happen when performing OTA upgrade using ESP HTTPS OTA.



### Event Handler Example



> ```
> /* Event handler for catching system events */
> static void event_handler(void* arg, esp_event_base_t event_base,
>                         int32_t event_id, void* event_data)
> {
>     if (event_base == ESP_HTTPS_OTA_EVENT) {
>         switch (event_id) {
>             case ESP_HTTPS_OTA_START:
>                 ESP_LOGI(TAG, "OTA started");
>                 break;
>             case ESP_HTTPS_OTA_CONNECTED:
>                 ESP_LOGI(TAG, "Connected to server");
>                 break;
>             case ESP_HTTPS_OTA_GET_IMG_DESC:
>                 ESP_LOGI(TAG, "Reading Image Description");
>                 break;
>             case ESP_HTTPS_OTA_VERIFY_CHIP_ID:
>                 ESP_LOGI(TAG, "Verifying chip id of new image: %d", *(esp_chip_id_t *)event_data);
>                 break;
>             case ESP_HTTPS_OTA_DECRYPT_CB:
>                 ESP_LOGI(TAG, "Callback to decrypt function");
>                 break;
>             case ESP_HTTPS_OTA_WRITE_FLASH:
>                 ESP_LOGD(TAG, "Writing to flash: %d written", *(int *)event_data);
>                 break;
>             case ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION:
>                 ESP_LOGI(TAG, "Boot partition updated. Next Partition: %d", *(esp_partition_subtype_t *)event_data);
>                 break;
>             case ESP_HTTPS_OTA_FINISH:
>                 ESP_LOGI(TAG, "OTA finish");
>                 break;
>             case ESP_HTTPS_OTA_ABORT:
>                 ESP_LOGI(TAG, "OTA abort");
>                 break;
>         }
>     }
> }
> 
> ```


Expected data type for different ESP HTTPS OTA events in the system event loop:



> * ESP_HTTPS_OTA_START : `NULL`
> * ESP_HTTPS_OTA_CONNECTED : `NULL`
> * ESP_HTTPS_OTA_GET_IMG_DESC : `NULL`
> * ESP_HTTPS_OTA_VERIFY_CHIP_ID : `esp_chip_id_t`
> * ESP_HTTPS_OTA_DECRYPT_CB : `NULL`
> * ESP_HTTPS_OTA_WRITE_FLASH : `int`
> * ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION : `esp_partition_subtype_t`
> * ESP_HTTPS_OTA_FINISH : `NULL`
> * ESP_HTTPS_OTA_ABORT : `NULL`





## API Reference



### Header File


* components/esp_https_ota/include/esp_https_ota.h
* This header file can be included with:



> ```
> #include "esp_https_ota.h"
> 
> ```
* This header file is a part of the API provided by the `esp_https_ota` component. To declare that your component depends on `esp_https_ota`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_https_ota
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_https_ota
> 
> ```




### Functions


#### esp_https_ota


```


esp_err_t esp_https_ota(const esp_https_ota_config_t *ota_config)
HTTPS OTA Firmware upgrade. 
This function allocates HTTPS OTA Firmware upgrade context, establishes HTTPS connection, reads image data from HTTP stream and writes it to OTA partition and finishes HTTPS OTA Firmware upgrade operation. This API supports URL redirection, but if CA cert of URLs differ then it should be appended to cert_pem member of ota_config->http_config.

Note
This API handles the entire OTA operation, so if this API is being used then no other APIs from esp_https_ota component should be called. If more information and control is needed during the HTTPS OTA process, then one can use esp_https_ota_begin and subsequent APIs. If this API returns successfully, esp_restart() must be called to boot from the new firmware image.


Parameters
ota_config -- [in] pointer to esp_https_ota_config_t structure.

Returns

ESP_OK: OTA data updated, next reboot will use specified partition.
ESP_FAIL: For generic failure.
ESP_ERR_INVALID_ARG: Invalid argument
ESP_ERR_OTA_VALIDATE_FAILED: Invalid app image
ESP_ERR_NO_MEM: Cannot allocate memory for OTA operation.
ESP_ERR_FLASH_OP_TIMEOUT or ESP_ERR_FLASH_OP_FAIL: Flash write failed.
For other return codes, refer OTA documentation in esp-idf's app_update component. 





```

#### esp_https_ota_begin


```


esp_err_t esp_https_ota_begin(const esp_https_ota_config_t *ota_config, esp_https_ota_handle_t *handle)
Start HTTPS OTA Firmware upgrade. 
This function initializes ESP HTTPS OTA context and establishes HTTPS connection. This function must be invoked first. If this function returns successfully, then esp_https_ota_perform should be called to continue with the OTA process and there should be a call to esp_https_ota_finish on completion of OTA operation or on failure in subsequent operations. This API supports URL redirection, but if CA cert of URLs differ then it should be appended to cert_pem member of http_config, which is a part of ota_config. In case of error, this API explicitly sets handle to NULL.

Note
This API is blocking, so setting is_async member of http_config structure will result in an error.


Parameters

ota_config -- [in] pointer to esp_https_ota_config_t structure 
handle -- [out] pointer to an allocated data of type esp_https_ota_handle_t which will be initialised in this function


Returns

ESP_OK: HTTPS OTA Firmware upgrade context initialised and HTTPS connection established
ESP_FAIL: For generic failure.
ESP_ERR_INVALID_ARG: Invalid argument (missing/incorrect config, certificate, etc.)
For other return codes, refer documentation in app_update component and esp_http_client component in esp-idf. 





```

#### esp_https_ota_perform


```


esp_err_t esp_https_ota_perform(esp_https_ota_handle_t https_ota_handle)
Read image data from HTTP stream and write it to OTA partition. 
This function reads image data from HTTP stream and writes it to OTA partition. This function must be called only if esp_https_ota_begin() returns successfully. This function must be called in a loop since it returns after every HTTP read operation thus giving you the flexibility to stop OTA operation midway.

Parameters
https_ota_handle -- [in] pointer to esp_https_ota_handle_t structure

Returns

ESP_ERR_HTTPS_OTA_IN_PROGRESS: OTA update is in progress, call this API again to continue.
ESP_OK: OTA update was successful
ESP_FAIL: OTA update failed
ESP_ERR_INVALID_ARG: Invalid argument
ESP_ERR_INVALID_VERSION: Invalid chip revision in image header
ESP_ERR_OTA_VALIDATE_FAILED: Invalid app image
ESP_ERR_NO_MEM: Cannot allocate memory for OTA operation.
ESP_ERR_FLASH_OP_TIMEOUT or ESP_ERR_FLASH_OP_FAIL: Flash write failed.
For other return codes, refer OTA documentation in esp-idf's app_update component. 





```

#### esp_https_ota_is_complete_data_received


```


bool esp_https_ota_is_complete_data_received(esp_https_ota_handle_t https_ota_handle)
Checks if complete data was received or not. 

Note
This API can be called just before esp_https_ota_finish() to validate if the complete image was indeed received.


Parameters
https_ota_handle -- [in] pointer to esp_https_ota_handle_t structure

Returns

false
true 





```

#### esp_https_ota_finish


```


esp_err_t esp_https_ota_finish(esp_https_ota_handle_t https_ota_handle)
Clean-up HTTPS OTA Firmware upgrade and close HTTPS connection. 
This function closes the HTTP connection and frees the ESP HTTPS OTA context. This function switches the boot partition to the OTA partition containing the new firmware image.

Note
If this API returns successfully, esp_restart() must be called to boot from the new firmware image esp_https_ota_finish should not be called after calling esp_https_ota_abort


Parameters
https_ota_handle -- [in] pointer to esp_https_ota_handle_t structure

Returns

ESP_OK: Clean-up successful
ESP_ERR_INVALID_STATE
ESP_ERR_INVALID_ARG: Invalid argument
ESP_ERR_OTA_VALIDATE_FAILED: Invalid app image 





```

#### esp_https_ota_abort


```


esp_err_t esp_https_ota_abort(esp_https_ota_handle_t https_ota_handle)
Clean-up HTTPS OTA Firmware upgrade and close HTTPS connection. 
This function closes the HTTP connection and frees the ESP HTTPS OTA context.

Note
esp_https_ota_abort should not be called after calling esp_https_ota_finish


Parameters
https_ota_handle -- [in] pointer to esp_https_ota_handle_t structure

Returns

ESP_OK: Clean-up successful
ESP_ERR_INVALID_STATE: Invalid ESP HTTPS OTA state
ESP_FAIL: OTA not started
ESP_ERR_NOT_FOUND: OTA handle not found
ESP_ERR_INVALID_ARG: Invalid argument 





```

#### esp_https_ota_get_img_desc


```


esp_err_t esp_https_ota_get_img_desc(esp_https_ota_handle_t https_ota_handle, esp_app_desc_t *new_app_info)
Reads app description from image header. The app description provides information like the "Firmware version" of the image. 

Note
This API can be called only after esp_https_ota_begin() and before esp_https_ota_perform(). Calling this API is not mandatory.


Parameters

https_ota_handle -- [in] pointer to esp_https_ota_handle_t structure 
new_app_info -- [out] pointer to an allocated esp_app_desc_t structure


Returns

ESP_ERR_INVALID_ARG: Invalid arguments
ESP_ERR_INVALID_STATE: Invalid state to call this API. esp_https_ota_begin() not called yet.
ESP_FAIL: Failed to read image descriptor
ESP_OK: Successfully read image descriptor 





```

#### esp_https_ota_get_image_len_read


```


int esp_https_ota_get_image_len_read(esp_https_ota_handle_t https_ota_handle)
This function returns OTA image data read so far. 

Note
This API should be called only if esp_https_ota_perform() has been called at least once or if esp_https_ota_get_img_desc has been called before.


Parameters
https_ota_handle -- [in] pointer to esp_https_ota_handle_t structure

Returns

-1 On failure
total bytes read so far 





```

#### esp_https_ota_get_image_size


```


int esp_https_ota_get_image_size(esp_https_ota_handle_t https_ota_handle)
This function returns OTA image total size. 

Note
This API should be called after esp_https_ota_begin() has been already called. This can be used to create some sort of progress indication (in combination with esp_https_ota_get_image_len_read())


Parameters
https_ota_handle -- [in] pointer to esp_https_ota_handle_t structure

Returns

-1 On failure or chunked encoding
total bytes of image 





```



### Structures


#### esp_https_ota_config_t


```


struct esp_https_ota_config_t
ESP HTTPS OTA configuration. 

Public Members
http_config

const esp_http_client_config_t *http_config
ESP HTTP client configuration 

http_client_init_cb

http_client_init_cb_t http_client_init_cb
Callback after ESP HTTP client is initialised 

bulk_flash_erase

bool bulk_flash_erase
Erase entire flash partition during initialization. By default flash partition is erased during write operation and in chunk of 4K sector size 

partial_http_download

bool partial_http_download
Enable Firmware image to be downloaded over multiple HTTP requests 

max_http_request_size

int max_http_request_size
Maximum request size for partial HTTP download 

buffer_caps

uint32_t buffer_caps
The memory capability to use when allocating the buffer for OTA update. Default capability is MALLOC_CAP_DEFAULT 



```



### Macros


#### ESP_ERR_HTTPS_OTA_BASE


```


ESP_ERR_HTTPS_OTA_BASE

```

#### ESP_ERR_HTTPS_OTA_IN_PROGRESS


```


ESP_ERR_HTTPS_OTA_IN_PROGRESS

```



### Type Definitions


#### esp_https_ota_handle_t


```


typedef void *esp_https_ota_handle_t

```

#### http_client_init_cb_t


```


typedef esp_err_t (*http_client_init_cb_t)(esp_http_client_handle_t)

```



### Enumerations


#### esp_https_ota_event_t


```


enum esp_https_ota_event_t
Events generated by OTA process. 
Values:
ESP_HTTPS_OTA_START

enumerator ESP_HTTPS_OTA_START
OTA started 

ESP_HTTPS_OTA_CONNECTED

enumerator ESP_HTTPS_OTA_CONNECTED
Connected to server 

ESP_HTTPS_OTA_GET_IMG_DESC

enumerator ESP_HTTPS_OTA_GET_IMG_DESC
Read app description from image header 

ESP_HTTPS_OTA_VERIFY_CHIP_ID

enumerator ESP_HTTPS_OTA_VERIFY_CHIP_ID
Verify chip id of new image 

ESP_HTTPS_OTA_DECRYPT_CB

enumerator ESP_HTTPS_OTA_DECRYPT_CB
Callback to decrypt function 

ESP_HTTPS_OTA_WRITE_FLASH

enumerator ESP_HTTPS_OTA_WRITE_FLASH
Flash write operation 

ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION

enumerator ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION
Boot partition update after successful ota update 

ESP_HTTPS_OTA_FINISH

enumerator ESP_HTTPS_OTA_FINISH
OTA finished 

ESP_HTTPS_OTA_ABORT

enumerator ESP_HTTPS_OTA_ABORT
OTA aborted 


```