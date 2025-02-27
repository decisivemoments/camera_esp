# ESP x509 Certificate Bundle




## Overview


The ESP x509 Certificate Bundle API provides an easy way to include a bundle of custom x509 root certificates for TLS server verification.



Note


The bundle is currently not available when using WolfSSL.



The bundle comes with the complete list of root certificates from Mozilla's NSS root certificate store. Using the gen_crt_bundle.py python utility, the certificates' subject name and public key are stored in a file and embedded in the ESP32-S3 binary.


When generating the bundle you may choose between:



> * The full root certificate bundle from Mozilla, containing more than 130 certificates. The current bundle was updated Mon Mar 11 15:25:27 2024 GMT.
> * A pre-selected filter list of the name of the most commonly used root certificates, reducing the amount of certificates to around 41 while still having around 90% absolute usage coverage and 99% market share coverage according to SSL certificate authorities statistics.


In addition, it is possible to specify a path to a certificate file or a directory containing certificates which then will be added to the generated bundle.



Note


Trusting all root certificates means the list will have to be updated if any of the certificates are retracted. This includes removing them from `cacrt_all.pem`.





## Configuration


Most configuration is done through menuconfig. CMake generates the bundle according to the configuration and embed it.



> * CONFIG_MBEDTLS_CERTIFICATE_BUNDLE: automatically build and attach the bundle.
> * CONFIG_MBEDTLS_DEFAULT_CERTIFICATE_BUNDLE: decide which certificates to include from the complete root certificate list.
> * CONFIG_MBEDTLS_CUSTOM_CERTIFICATE_BUNDLE_PATH: specify the path of any additional certificates to embed in the bundle.


To enable the bundle when using ESP-TLS simply pass the function pointer to the bundle attach function:



```
esp_tls_cfg_t cfg = {
     .crt_bundle_attach = esp_crt_bundle_attach,
};

```


This is done to avoid embedding the certificate bundle unless activated by the user.


If using mbedTLS directly then the bundle may be activated by directly calling the attach function during the setup process:



```
mbedtls_ssl_config conf;
mbedtls_ssl_config_init(&conf);

esp_crt_bundle_attach(&conf);

```




## Generating the List of Root Certificates


The list of root certificates comes from Mozilla's NSS root certificate store, which can be found here


The list can be downloaded and created by running the script `mk-ca-bundle.pl` that is distributed as a part of curl.


Another alternative would be to download the finished list directly from the curl website: CA certificates extracted from Mozilla


The common certificates bundle were made by selecting the authorities with a market share of more than 1% from w3tech's SSL Survey.


These authorities were then used to pick the names of the certificates for the filter list, `cmn_crt_authorities.csv`, from this list provided by Mozilla.




## Updating the Certificate Bundle


The bundle is embedded into the app and can be updated along with the app by an OTA update. If you want to include a more up-to-date bundle than the bundle currently included in ESP-IDF, then the certificate list can be downloaded from Mozilla as described in Generating the List of Root Certificates.




## Periodic Sync


The bundle is kept updated by periodic sync with the Mozilla's NSS root certificate store. The deprecated certs from the upstream bundle are added to deprecated list (for compatibility reasons) in ESP-IDF minor or patch release. If required, the deprecated certs can be added to the default bundle by enabling CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEPRECATED_LIST. The deprecated certs shall be removed (reset) on the next major ESP-IDF release.




## Application Examples


Simple HTTPS example that uses ESP-TLS to establish a secure socket connection using the certificate bundle with two custom certificates added for verification: protocols/https_x509_bundle.


HTTPS example that uses ESP-TLS and the default bundle: protocols/https_request.


HTTPS example that uses mbedTLS and the default bundle: protocols/https_mbedtls.




## API Reference



### Header File


* components/mbedtls/esp_crt_bundle/include/esp_crt_bundle.h
* This header file can be included with:



> ```
> #include "esp_crt_bundle.h"
> 
> ```
* This header file is a part of the API provided by the `mbedtls` component. To declare that your component depends on `mbedtls`, add the following to your CMakeLists.txt:



> ```
> REQUIRES mbedtls
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES mbedtls
> 
> ```




### Functions


#### esp_crt_bundle_attach


```


esp_err_t esp_crt_bundle_attach(void *conf)
Attach and enable use of a bundle for certificate verification. 
Attach and enable use of a bundle for certificate verification through a verification callback. If no specific bundle has been set through esp_crt_bundle_set() it will default to the bundle defined in menuconfig and embedded in the binary.

Parameters
conf -- [in] The config struct for the SSL connection.

Returns

ESP_OK if adding certificates was successful.
Other if an error occured or an action must be taken by the calling process. 





```

#### esp_crt_bundle_detach


```


void esp_crt_bundle_detach(mbedtls_ssl_config *conf)
Disable and dealloc the certification bundle. 
Removes the certificate verification callback and deallocates used resources

Parameters
conf -- [in] The config struct for the SSL connection. 



```

#### esp_crt_bundle_set


```


esp_err_t esp_crt_bundle_set(const uint8_t *x509_bundle, size_t bundle_size)
Set the default certificate bundle used for verification. 
Overrides the default certificate bundle only in case of successful initialization. In most use cases the bundle should be set through menuconfig. The bundle needs to be sorted by subject name since binary search is used to find certificates.

Parameters

x509_bundle -- [in] A pointer to the certificate bundle.
bundle_size -- [in] Size of the certificate bundle in bytes.


Returns

ESP_OK if adding certificates was successful.
Other if an error occured or an action must be taken by the calling process. 





```