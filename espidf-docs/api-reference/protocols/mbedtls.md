# Mbed TLS



Mbed TLS is a C library that implements cryptographic primitives, X.509 certificate manipulation and the SSL/TLS and DTLS protocols. Its small code footprint makes it suitable for embedded systems.



Note


ESP-IDF uses a fork of Mbed TLS which includes a few patches (related to hardware routines of certain modules like `bignum (MPI)` and `ECC`) over vanilla Mbed TLS.



Mbed TLS supports SSL 3.0 up to TLS 1.3 and DTLS 1.0 to 1.2 communication by providing the following:


* TCP/IP communication functions: listen, connect, accept, read/write.
* SSL/TLS communication functions: init, handshake, read/write.
* X.509 functions: CRT, CRL and key handling
* Random number generation
* Hashing
* Encryption/decryption


Supported TLS versions include SSL 3.0, TLS 1.0, TLS 1.1, TLS 1.2, and TLS 1.3, but on the latest ESP-IDF, SSL 3.0, TLS 1.0, and TLS 1.1 have been removed from Mbed TLS. Supported DTLS versions include DTLS 1.0, DTLS 1.1, and DTLS 1.2, but on the latest ESP-IDF, DTLS 1.0 has been removed from Mbed TLS.



## Mbed TLS Documentation


For Mbed TLS documentation please refer to the following (upstream) pointers:


* API Reference
* Knowledge Base




## Mbed TLS Support in ESP-IDF


Please find the information about the Mbed TLS versions presented in different branches of ESP-IDF here.



Note


Please refer the Mbed TLS to migrate from Mbed TLS version 2.x to version 3.0 or greater.





## Application Examples


Examples in ESP-IDF use ESP-TLS which provides a simplified API interface for accessing the commonly used TLS functionality.


Refer to the examples protocols/https_server/simple (Simple HTTPS server) and protocols/https_request (Make HTTPS requests) for more information.


If the Mbed TLS API is to be used directly, refer to the example protocols/https_mbedtls.




## Alternatives


ESP-TLS acts as an abstraction layer over the underlying SSL/TLS library and thus has an option to use Mbed TLS or wolfSSL as the underlying library. By default, only Mbed TLS is available and used in ESP-IDF whereas wolfSSL is available publicly at <https://github.com/espressif/esp-wolfSSL> with the upstream submodule pointer.


Please refer to ESP-TLS: Underlying SSL/TLS Library Options docs for more information on this and comparison of Mbed TLS and wolfSSL.




## Important Config Options


Following is a brief list of important config options accessible at `Component Config -> mbedTLS`. The full list of config options can be found here.


* CONFIG_MBEDTLS_SSL_PROTO_TLS1_2: Support for TLS 1.2
* CONFIG_MBEDTLS_SSL_PROTO_TLS1_3: Support for TLS 1.3
* CONFIG_MBEDTLS_CERTIFICATE_BUNDLE: Support for trusted root certificate bundle (more about this: ESP x509 Certificate Bundle)
* CONFIG_MBEDTLS_CLIENT_SSL_SESSION_TICKETS: Support for TLS Session Resumption: Client session tickets
* CONFIG_MBEDTLS_SERVER_SSL_SESSION_TICKETS: Support for TLS Session Resumption: Server session tickets
* CONFIG_MBEDTLS_HARDWARE_SHA: Support for hardware SHA acceleration
* CONFIG_MBEDTLS_HARDWARE_AES: Support for hardware AES acceleration
* CONFIG_MBEDTLS_HARDWARE_MPI: Support for hardware MPI (bignum) acceleration






Note


Mbed TLS v3.0.0 and later support only TLS 1.2 and TLS 1.3 (SSL 3.0, TLS 1.0, TLS 1.1, and DTLS 1.0 are not supported). The support for TLS 1.3 is experimental and only supports the client-side. More information about this can be found out here.





## Performance and Memory Tweaks



### Reducing Heap Usage


The following table shows typical memory usage with different configs when the protocols/https_request example (with Server Validation enabled) was run with Mbed TLS as the SSL/TLS library.








| Mbed TLS Test | Related Configs | Heap Usage (approx.) |
| --- | --- | --- |
| Default | NA | 42196 B |
| Enable SSL Variable Length | CONFIG_MBEDTLS_SSL_VARIABLE_BUFFER_LENGTH | 42120 B |
| Disable Keep Peer Certificate | CONFIG_MBEDTLS_SSL_KEEP_PEER_CERTIFICATE | 38533 B |
| Enable Dynamic TX/RX Buffer | CONFIG_MBEDTLS_DYNAMIC_BUFFER CONFIG_MBEDTLS_DYNAMIC_FREE_CONFIG_DATA CONFIG_MBEDTLS_DYNAMIC_FREE_CA_CERT | 22013 B |



Note


These values are subject to change with change in configuration options and versions of Mbed TLS.





### Reducing Binary Size


Under `Component Config -> mbedTLS`, there are multiple Mbed TLS features which are enabled by default but can be disabled if not needed to save code size. More information can be about this can be found in Minimizing Binary Size docs.