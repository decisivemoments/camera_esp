# Hash-Based Message Authentication Code (HMAC)



Hash-based Message Authentication Code (HMAC) is a secure authentication technique that verifies the authenticity and integrity of a message with a pre-shared key. This module provides hardware acceleration for SHA256-HMAC generation using a key burned into an eFuse block.


For more detailed information on the application workflow and the HMAC calculation process, see **ESP32-S3 Technical Reference Manual** > **HMAC Accelerator (HMAC)** [PDF].



## Generalized Application Scheme


Let there be two parties, A and B. They want to verify the authenticity and integrity of messages sent between each other. Before they can start sending messages, they need to exchange the secret key via a secure channel.


To verify A's messages, B can do the following:


* A calculates the HMAC of the message it wants to send.
* A sends the message and the HMAC to B.
* B calculates the HMAC of the received message itself.
* B checks whether the received and calculated HMACs match.


If they do match, the message is authentic.


However, the HMAC itself is not bound to this use case. It can also be used for challenge-response protocols supporting HMAC or as a key input for further security modules (see below), etc.




## HMAC on ESP32-S3


On ESP32-S3, the HMAC module works with a secret key burnt into the eFuses. This eFuse key can be made completely inaccessible for any resources outside the cryptographic modules, thus avoiding key leakage.


Furthermore, ESP32-S3 has three different application scenarios for its HMAC module:


1. HMAC is generated for software use
2. HMAC is used as a key for the Digital Signature (DS) module
3. HMAC is used for enabling the soft-disabled JTAG interface


The first mode is called **Upstream** mode, while the last two modes are called **Downstream** modes.



### eFuse Keys for HMAC


Six physical eFuse blocks can be used as keys for the HMAC module: block 4 ~ block 9. The enum `hmac_key_id_t` in the API maps them to `HMAC_KEY0` ~ `HMAC_KEY5`.


Each key has a corresponding eFuse parameter **key purpose** determining for which of the three HMAC application scenarios (see below) the key may be used:







| Key Purpose | Application Scenario |
| --- | --- |
| 8 | HMAC generated for software use |
| 7 | HMAC used as a key for the Digital Signature (DS) module |
| 6 | HMAC used for enabling the soft-disabled JTAG interface |
| 5 | HMAC both as a key for the DS module and for enabling JTAG |


This is to prevent the usage of a key for a different function than originally intended.


To calculate an HMAC, the software has to provide the ID of the key block containing the secret key as well as the **key purpose** (see **ESP32-S3 Technical Reference Manual** > **eFuse Controller (eFuse)** [PDF]).


Before the HMAC key calculation, the HMAC module looks up the purpose of the provided key block. The calculation only proceeds if the purpose of the provided key block matches the purpose stored in the eFuses of the key block provided by the ID.




### HMAC Generation for Software


Key purpose value: 8


In this case, the HMAC is given out to the software, e.g., to authenticate a message.


The API to calculate the HMAC is `esp_hmac_calculate()`. The input arguments for the function are the message, message length, and the eFuse key block ID which contains the secret and has the efuse key purpose set to Upstream mode.




### HMAC for Digital Signature


Key purpose values: 7, 5


The HMAC can be used as a key derivation function to decrypt private key parameters which are used by the Digital Signature module. A standard message is used by the hardware in that case. You only need to provide the eFuse key block and purpose on the HMAC side, additional parameters are required for the Digital Signature component in that case.


Neither the key nor the actual HMAC is ever exposed outside the HMAC module and DS component. The calculation of the HMAC and its handover to the DS component happen internally.


For more details, see **ESP32-S3 Technical Reference Manual** > **Digital Signature (DS)** [PDF].




### HMAC for Enabling JTAG


Key purpose values: 6, 5


The third application is using the HMAC as a key to enable JTAG if it was soft-disabled before.


Following is the procedure to re-enable the JTAG:


**Stage 1: Setup**


1. Generate a 256-bit HMAC secret key to use for JTAG re-enable.
2. Write the key to an eFuse block with key purpose HMAC_DOWN_ALL (5) or HMAC_DOWN_JTAG (6). This can be done using the `esp_efuse_write_key()` function in the firmware or using `espefuse.py` from the host.
3. Configure the eFuse key block to be read-protected using the `esp_efuse_set_read_protect()`, so that software cannot read back the value.
4. Burn the `soft JTAG disable` bit/bits on ESP32-S3. This will permanently disable JTAG unless the correct key value is provided by the software.



Note


The API **esp_efuse_write_field_cnt(ESP_EFUSE_SOFT_DIS_JTAG, ESP_EFUSE_SOFT_DIS_JTAG[0]->bit_count)** can be used to burn `soft JTAG disable` bits on ESP32-S3.




Note


If `HARD_DIS_JTAG` eFuse is set, then `SOFT_DIS_JTAG` functionality does not work because JTAG is permanently disabled.



JTAG enables


1. The key to re-enable JTAG is the output of the HMAC-SHA256 function using the secret key in eFuse and 32 `0x00` bytes as the message.
2. Pass this key value when calling the `esp_hmac_jtag_enable()` function from the firmware.
3. To re-disable JTAG in the firmware, reset the system or call `esp_hmac_jtag_disable()`.


End-to-end example of soft disable and re-enable JTAG workflow: security/hmac_soft_jtag.


For more details, see **ESP32-S3 Technical Reference Manual** > **HMAC Accelerator (HMAC)** [PDF].





## Application Outline


The following code is an outline of how to set an eFuse key and then use it to calculate an HMAC for software usage.


We use `esp_efuse_write_key` to set physical key block 4 in the eFuse for the HMAC module together with its purpose. `ESP_EFUSE_KEY_PURPOSE_HMAC_UP` (8) means that this key can only be used for HMAC generation for software usage:



```
#include "esp_efuse.h"

const uint8_t key_data[32] = { ... };

esp_err_t status = esp_efuse_write_key(EFUSE_BLK_KEY4,
                    ESP_EFUSE_KEY_PURPOSE_HMAC_UP,
                    key_data, sizeof(key_data));

if (status == ESP_OK) {
    // written key
} else {
    // writing key failed, maybe written already
}

```


Now we can use the saved key to calculate an HMAC for software usage.



```
#include "esp_hmac.h"

uint8_t hmac[32];

const char *message = "Hello, HMAC!";
const size_t msg_len = 12;

esp_err_t result = esp_hmac_calculate(HMAC_KEY4, message, msg_len, hmac);

if (result == ESP_OK) {
    // HMAC written to hmac now
} else {
    // failure calculating HMAC
}

```




## API Reference



### Header File


* components/esp_hw_support/include/esp_hmac.h
* This header file can be included with:



> ```
> #include "esp_hmac.h"
> 
> ```




### Functions


#### esp_hmac_calculate


```


esp_err_t esp_hmac_calculate(hmac_key_id_t key_id, const void *message, size_t message_len, uint8_t *hmac)
Calculate the HMAC of a given message. 
Calculate the HMAC hmac of a given message message with length message_len. SHA256 is used for the calculation.

Note
Uses the HMAC peripheral in "upstream" mode.


Parameters

key_id -- Determines which of the 6 key blocks in the efuses should be used for the HMAC calcuation. The corresponding purpose field of the key block in the efuse must be set to the HMAC upstream purpose value. 
message -- the message for which to calculate the HMAC 
message_len -- message length return ESP_ERR_INVALID_STATE if unsuccessful 
hmac -- [out] the hmac result; the buffer behind the provided pointer must be a writeable buffer of 32 bytes


Returns

ESP_OK, if the calculation was successful,
ESP_ERR_INVALID_ARG if message or hmac is a nullptr or if key_id out of range
ESP_FAIL, if the hmac calculation failed 





```

#### esp_hmac_jtag_enable


```


esp_err_t esp_hmac_jtag_enable(hmac_key_id_t key_id, const uint8_t *token)
Use HMAC peripheral in Downstream mode to re-enable the JTAG, if it is not permanently disabled by HW. In downstream mode, HMAC calculations performed by peripheral are used internally and not provided back to user. 

Note
Return value of the API does not indicate the JTAG status. 


Parameters

key_id -- Determines which of the 6 key blocks in the efuses should be used for the HMAC calculation. The corresponding purpose field of the key block in the efuse must be set to HMAC downstream purpose.
token -- Pre calculated HMAC value of the 32-byte 0x00 using SHA-256 and the known private HMAC key. The key is already programmed to a eFuse key block. The key block number is provided as the first parameter to this function.


Returns

ESP_OK, if the key_purpose of the key_id matches to HMAC downstread mode, The API returns success even if calculated HMAC does not match with the provided token. However, The JTAG will be re-enabled only if the calculated HMAC value matches with provided token, otherwise JTAG will remain disabled.
ESP_FAIL, if the key_purpose of the key_id is not set to HMAC downstream purpose or JTAG is permanently disabled by EFUSE_HARD_DIS_JTAG eFuse parameter.
ESP_ERR_INVALID_ARG, invalid input arguments





```

#### esp_hmac_jtag_disable


```


esp_err_t esp_hmac_jtag_disable(void)
Disable the JTAG which might be enabled using the HMAC downstream mode. This function just clears the result generated by calling esp_hmac_jtag_enable() API. 

Returns

ESP_OK return ESP_OK after writing the HMAC_SET_INVALIDATE_JTAG_REG with value 1. 





```



### Enumerations


#### hmac_key_id_t


```


enum hmac_key_id_t
The possible efuse keys for the HMAC peripheral 
Values:
HMAC_KEY0

enumerator HMAC_KEY0

HMAC_KEY1

enumerator HMAC_KEY1

HMAC_KEY2

enumerator HMAC_KEY2

HMAC_KEY3

enumerator HMAC_KEY3

HMAC_KEY4

enumerator HMAC_KEY4

HMAC_KEY5

enumerator HMAC_KEY5

HMAC_KEY_MAX

enumerator HMAC_KEY_MAX


```