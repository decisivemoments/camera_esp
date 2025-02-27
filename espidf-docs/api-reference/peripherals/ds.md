# Digital Signature (DS)



The Digital Signature (DS) module provides hardware acceleration of signing messages based on RSA. It uses pre-encrypted parameters to calculate a signature. The parameters are encrypted using HMAC as a key-derivation function. In turn, the HMAC uses eFuses as the input key. The whole process happens in hardware so that neither the decryption key for the RSA parameters nor the input key for the HMAC key derivation function can be seen by the software while calculating the signature.


For more detailed information on the hardware involved in the signature calculation and the registers used, see **ESP32-S3 Technical Reference Manual** > **Digital Signature (DS)** [PDF].



## Private Key Parameters


The private key parameters for the RSA signature are stored in flash. To prevent unauthorized access, they are AES-encrypted. The HMAC module is used as a key-derivation function to calculate the AES encryption key for the private key parameters. In turn, the HMAC module uses a key from the eFuses key block which can be read-protected to prevent unauthorized access as well.


Upon signature calculation invocation, the software only specifies which eFuse key to use, the corresponding eFuse key purpose, the location of the encrypted RSA parameters, and the message.




## Key Generation


Both the HMAC key and the RSA private key have to be created and stored before the DS peripheral can be used. This needs to be done in software on the ESP32-S3 or alternatively on a host. For this context, ESP-IDF provides `esp_efuse_write_block()` to set the HMAC key and `esp_hmac_calculate()` to encrypt the private RSA key parameters.


You can find instructions on how to calculate and assemble the private key parameters in **ESP32-S3 Technical Reference Manual** > **Digital Signature (DS)** [PDF].




## Signature Calculation with ESP-IDF


For more detailed information on the workflow and the registers used, see **ESP32-S3 Technical Reference Manual** > **Digital Signature (DS)** [PDF].


Three parameters need to be prepared to calculate the digital signature:


1. The eFuse key block ID which is used as the key for the HMAC
2. The location of the encrypted private key parameters
3. The message to be signed


Since the signature calculation takes some time, there are two possible API versions to use in ESP-IDF. The first one is `esp_ds_sign()` and simply blocks until the calculation is finished. If software needs to do something else during the calculation, `esp_ds_start_sign()` can be called, followed by periodic calls to `esp_ds_is_busy()` to check when the calculation has finished. Once the calculation has finished, `esp_ds_finish_sign()` can be called to get the resulting signature.


The APIs `esp_ds_sign()` and `esp_ds_start_sign()` calculate a plain RSA signature with the help of the DS peripheral. This signature needs to be converted to an appropriate format for further use. For example, the MbedTLS SSL stack supports PKCS#1 format. The API `esp_ds_rsa_sign()` can be used to obtain the signature directly in the PKCS#1 v1.5 format. It internally uses `esp_ds_start_sign()` and converts the signature into PKCS#1 v1.5 format.



Note


This is only the basic DS building block, the message length is fixed. To create signatures of arbitrary messages, the input is normally a hash of the actual message, padded up to the required length. An API to do this is planned in the future.





## Configure the DS Peripheral for a TLS Connection


The DS peripheral on ESP32-S3 chip must be configured before it can be used for a TLS connection. The configuration involves the following steps:


1. Randomly generate a 256-bit value called the `Initialization Vector` (IV).
2. Randomly generate a 256-bit value called the `HMAC_KEY`.
3. Calculate the encrypted private key parameters from the client private key (RSA) and the parameters generated in the above steps.
4. Then burn the 256-bit `HMAC_KEY` on the eFuse, which can only be read by the DS peripheral.


For more details, see **ESP32-S3 Technical Reference Manual** > **Digital Signature (DS)** [PDF].


To configure the DS peripheral for development purposes, you can use the esp-secure-cert-tool.


The encrypted private key parameters obtained after the DS peripheral configuration are then to be kept in flash. Furthermore, they are to be passed to the DS peripheral which makes use of those parameters for the Digital Signature operation. The application then needs to read the DS data from the flash, which has been done through the APIs provided by the esp_secure_cert_mgr component. Please refer to the component/README for more details.


The process of initializing the DS peripheral and then performing the Digital Signature operation is done internally with the help of ESP-TLS. Please refer to Digital Signature with ESP-TLS for more details.


As mentioned in the ESP-TLS documentation, the application only needs to provide the encrypted private key parameters to the esp_tls context (as ds_data), which internally performs all necessary operations for initializing the DS peripheral and then performing the DS operation.




## Example for SSL Mutual Authentication Using DS


The example protocols/mqtt/ssl_ds shows how to use the DS peripheral for mutual authentication. The example uses mqtt_client (Implemented through ESP-MQTT) to connect to broker `test.mosquitto.org` using SSL transport with mutual authentication. The SSL part is internally performed with ESP-TLS. See protocols/mqtt/ssl_ds/README.md for more details.




## API Reference



### Header File


* components/esp_hw_support/include/esp_ds.h
* This header file can be included with:



> ```
> #include "esp_ds.h"
> 
> ```




### Functions


#### esp_ds_sign


```


esp_err_t esp_ds_sign(const void *message, const esp_ds_data_t *data, hmac_key_id_t key_id, void *signature)
Sign the message with a hardware key from specific key slot. The function calculates a plain RSA signature with help of the DS peripheral. The RSA encryption operation is as follows: Z = XY mod M where, Z is the signature, X is the input message, Y and M are the RSA private key parameters. 
This function is a wrapper around esp_ds_finish_sign() and esp_ds_start_sign(), so do not use them in parallel. It blocks until the signing is finished and then returns the signature.

Note
Please see note section of esp_ds_start_sign() for more details about the input parameters.


Parameters

message -- the message to be signed; its length should be (data->rsa_length + 1)*4 bytes, and those bytes must be in little endian format. It is your responsibility to apply your hash function and padding before calling this function, if required. (e.g. message = padding(hash(inputMsg))) 
data -- the encrypted signing key data (AES encrypted RSA key + IV) 
key_id -- the HMAC key ID determining the HMAC key of the HMAC which will be used to decrypt the signing key data 
signature -- the destination of the signature, should be (data->rsa_length + 1)*4 bytes long


Returns

ESP_OK if successful, the signature was written to the parameter signature.
ESP_ERR_INVALID_ARG if one of the parameters is NULL or data->rsa_length is too long or 0
ESP_ERR_HW_CRYPTO_DS_HMAC_FAIL if there was an HMAC failure during retrieval of the decryption key
ESP_ERR_NO_MEM if there hasn't been enough memory to allocate the context object
ESP_ERR_HW_CRYPTO_DS_INVALID_KEY if there's a problem with passing the HMAC key to the DS component
ESP_ERR_HW_CRYPTO_DS_INVALID_DIGEST if the message digest didn't match; the signature is invalid.
ESP_ERR_HW_CRYPTO_DS_INVALID_PADDING if the message padding is incorrect, the signature can be read though since the message digest matches. 





```

#### esp_ds_start_sign


```


esp_err_t esp_ds_start_sign(const void *message, const esp_ds_data_t *data, hmac_key_id_t key_id, esp_ds_context_t **esp_ds_ctx)
Start the signing process. 
This function yields a context object which needs to be passed to esp_ds_finish_sign() to finish the signing process. The function calculates a plain RSA signature with help of the DS peripheral. The RSA encryption operation is as follows: Z = XY mod M where, Z is the signature, X is the input message, Y and M are the RSA private key parameters.

Note
This function locks the HMAC, SHA, AES and RSA components, so the user has to ensure to call esp_ds_finish_sign() in a timely manner. The numbers Y, M, Rb which are a part of esp_ds_data_t should be provided in little endian format and should be of length equal to the RSA private key bit length The message length in bits should also be equal to the RSA private key bit length. No padding is applied to the message automatically, Please ensure the message is appropriate padded before calling the API.


Parameters

message -- the message to be signed; its length should be (data->rsa_length + 1)*4 bytes, and those bytes must be in little endian format. It is your responsibility to apply your hash function and padding before calling this function, if required. (e.g. message = padding(hash(inputMsg))) 
data -- the encrypted signing key data (AES encrypted RSA key + IV) 
key_id -- the HMAC key ID determining the HMAC key of the HMAC which will be used to decrypt the signing key data 
esp_ds_ctx -- the context object which is needed for finishing the signing process later


Returns

ESP_OK if successful, the ds operation was started now and has to be finished with esp_ds_finish_sign()
ESP_ERR_INVALID_ARG if one of the parameters is NULL or data->rsa_length is too long or 0
ESP_ERR_HW_CRYPTO_DS_HMAC_FAIL if there was an HMAC failure during retrieval of the decryption key
ESP_ERR_NO_MEM if there hasn't been enough memory to allocate the context object
ESP_ERR_HW_CRYPTO_DS_INVALID_KEY if there's a problem with passing the HMAC key to the DS component 





```

#### esp_ds_is_busy


```


bool esp_ds_is_busy(void)
Return true if the DS peripheral is busy, otherwise false.

Note
Only valid if esp_ds_start_sign() was called before. 


```

#### esp_ds_finish_sign


```


esp_err_t esp_ds_finish_sign(void *signature, esp_ds_context_t *esp_ds_ctx)
Finish the signing process. 

Parameters

signature -- the destination of the signature, should be (data->rsa_length + 1)*4 bytes long, the resultant signature bytes shall be written in little endian format. 
esp_ds_ctx -- the context object retreived by esp_ds_start_sign()


Returns

ESP_OK if successful, the ds operation has been finished and the result is written to signature.
ESP_ERR_INVALID_ARG if one of the parameters is NULL
ESP_ERR_HW_CRYPTO_DS_INVALID_DIGEST if the message digest didn't match; the signature is invalid. This means that the encrypted RSA key parameters are invalid, indicating that they may have been tampered with or indicating a flash error, etc.
ESP_ERR_HW_CRYPTO_DS_INVALID_PADDING if the message padding is incorrect, the signature can be read though since the message digest matches (see TRM for more details). 





```

#### esp_ds_encrypt_params


```


esp_err_t esp_ds_encrypt_params(esp_ds_data_t *data, const void *iv, const esp_ds_p_data_t *p_data, const void *key)
Encrypt the private key parameters. 
The encryption is a prerequisite step before any signature operation can be done. It is not strictly necessary to use this encryption function, the encryption could also happen on an external device.

Note
The numbers Y, M, Rb which are a part of esp_ds_data_t should be provided in little endian format and should be of length equal to the RSA private key bit length The message length in bits should also be equal to the RSA private key bit length. No padding is applied to the message automatically, Please ensure the message is appropriate padded before calling the API.


Parameters

data -- Output buffer to store encrypted data, suitable for later use generating signatures. 
iv -- Pointer to 16 byte IV buffer, will be copied into 'data'. Should be randomly generated bytes each time. 
p_data -- Pointer to input plaintext key data. The expectation is this data will be deleted after this process is done and 'data' is stored. 
key -- Pointer to 32 bytes of key data. Type determined by key_type parameter. The expectation is the corresponding HMAC key will be stored to efuse and then permanently erased.


Returns

ESP_OK if successful, the ds operation has been finished and the result is written to signature.
ESP_ERR_INVALID_ARG if one of the parameters is NULL or p_data->rsa_length is too long 





```



### Structures


#### esp_digital_signature_data


```


struct esp_digital_signature_data
Encrypted private key data. Recommended to store in flash in this format.

Note
This struct has to match to one from the ROM code! This documentation is mostly taken from there. 


Public Members
rsa_length

esp_digital_signature_length_t rsa_length
RSA LENGTH register parameters (number of words in RSA key & operands, minus one).
This value must match the length field encrypted and stored in 'c', or invalid results will be returned. (The DS peripheral will always use the value in 'c', not this value, so an attacker can't alter the DS peripheral results this way, it will just truncate or extend the message and the resulting signature in software.)

Note
In IDF, the enum type length is the same as of type unsigned, so they can be used interchangably. See the ROM code for the original declaration of struct ets_ds_data_t. 


iv

uint32_t iv[ESP_DS_IV_BIT_LEN / 32]
IV value used to encrypt 'c' 

c

uint8_t c[ESP_DS_C_LEN]
Encrypted Digital Signature parameters. Result of AES-CBC encryption of plaintext values. Includes an encrypted message digest. 



```

#### esp_ds_p_data_t


```


struct esp_ds_p_data_t
Plaintext parameters used by Digital Signature.
This is only used for encrypting the RSA parameters by calling esp_ds_encrypt_params(). Afterwards, the result can be stored in flash or in other persistent memory. The encryption is a prerequisite step before any signature operation can be done.

Note
Y, M, Rb, & M_Prime must all be in little endian format. 


Public Members
Y

uint32_t Y[ESP_DS_SIGNATURE_MAX_BIT_LEN / 32]
RSA exponent. 

M

uint32_t M[ESP_DS_SIGNATURE_MAX_BIT_LEN / 32]
RSA modulus. 

Rb

uint32_t Rb[ESP_DS_SIGNATURE_MAX_BIT_LEN / 32]
RSA r inverse operand. 

M_prime

uint32_t M_prime
RSA M prime operand. 

length

uint32_t length
RSA length in words (32 bit) 



```



### Macros


#### ESP_DS_IV_BIT_LEN


```


ESP_DS_IV_BIT_LEN

```

#### ESP_DS_IV_LEN


```


ESP_DS_IV_LEN

```

#### ESP_DS_SIGNATURE_MAX_BIT_LEN


```


ESP_DS_SIGNATURE_MAX_BIT_LEN

```

#### ESP_DS_SIGNATURE_MD_BIT_LEN


```


ESP_DS_SIGNATURE_MD_BIT_LEN

```

#### ESP_DS_SIGNATURE_M_PRIME_BIT_LEN


```


ESP_DS_SIGNATURE_M_PRIME_BIT_LEN

```

#### ESP_DS_SIGNATURE_L_BIT_LEN


```


ESP_DS_SIGNATURE_L_BIT_LEN

```

#### ESP_DS_SIGNATURE_PADDING_BIT_LEN


```


ESP_DS_SIGNATURE_PADDING_BIT_LEN

```

#### ESP_DS_C_LEN


```


ESP_DS_C_LEN

```



### Type Definitions


#### esp_ds_context_t


```


typedef struct esp_ds_context esp_ds_context_t

```

#### esp_ds_data_t


```


typedef struct esp_digital_signature_data esp_ds_data_t
Encrypted private key data. Recommended to store in flash in this format.

Note
This struct has to match to one from the ROM code! This documentation is mostly taken from there. 


```



### Enumerations


#### esp_digital_signature_length_t


```


enum esp_digital_signature_length_t
Values:
ESP_DS_RSA_1024

enumerator ESP_DS_RSA_1024

ESP_DS_RSA_2048

enumerator ESP_DS_RSA_2048

ESP_DS_RSA_3072

enumerator ESP_DS_RSA_3072

ESP_DS_RSA_4096

enumerator ESP_DS_RSA_4096


```