# Security



This guide provides an overview of the overall security features available in various Espressif solutions. It is highly recommended to consider this guide while designing the products with the Espressif platform and the ESP-IDF software stack from the **security** perspective.



## Goals


High level security goals are as follows:


1. Preventing untrustworthy code from being executed
2. Protecting the identity and integrity of the code stored in the off-chip flash memory
3. Securing device identity
4. Secure storage for confidential data
5. Authenticated and encrypted communication from the device




## Platform Security



### Secure Boot


The Secure Boot feature ensures that only authenticated software can execute on the device. The Secure Boot process forms a chain of trust by verifying all **mutable** software entities involved in the Application Startup Flow. Signature verification happens during both boot-up as well as in OTA updates.


Please refer to Secure Boot V2 for detailed documentation about this feature.



Important


It is highly recommended that Secure Boot be enabled on all production devices.




#### Secure Boot Best Practices


* Generate the signing key on a system with a quality source of entropy.
* Always keep the signing key private. A leak of this key will compromise the Secure Boot system.
* Do not allow any third party to observe any aspects of the key generation or signing process using `espsecure.py`. Both processes are vulnerable to timing or other side-channel attacks.
* Ensure that all security eFuses have been correctly programmed, including disabling of the debug interfaces, non-required boot mediums (e.g., UART DL mode), etc.





### Flash Encryption


The Flash Encryption feature helps to encrypt the contents on the off-chip flash memory and thus provides the **confidentiality** aspect to the software or data stored in the flash memory.


Please refer to Flash Encryption for detailed information about this feature.


If ESP32-S3 is connected to an external SPI RAM, the contents written to or read from the SPI RAM will also be encrypted and decrypted respectively (via the MMU's flash cache, provided that FLash Encryption is enabled). This provides an additional safety layer for the data stored in SPI RAM, hence configurations like `CONFIG_MBEDTLS_EXTERNAL_MEM_ALLOC` can be safely enabled in this case.



#### Flash Encryption Best Practices


* It is recommended to use flash Encryption release mode for the production use-cases.
* It is recommended to have a unique flash encryption key per device.
* Enable Secure Boot as an extra layer of protection, and to prevent an attacker from selectively corrupting any part of the flash before boot.





### Device Identity


The Digital Signature peripheral in ESP32-S3 produces hardware-accelerated RSA digital signatures with the assistance of HMAC, without the RSA private key being accessible by software. This allows the private key to be kept secured on the device without anyone other than the device hardware being able to access it.


DS peripheral can help to establish the **Secure Device Identity** to the remote endpoint, e.g., in the case of TLS mutual authentication based on the RSA cipher scheme.


Please refer to the Digital Signature (DS) for detailed documentation.




### Memory Protection


ESP32-S3 supports the **Memory Protection** scheme, either through architecture or special peripheral like PMS, which provides an ability to enforce and monitor permission attributes to memory and, in some cases, peripherals. ESP-IDF application startup code configures the permissions attributes like Read/Write access on data memories and Read/Execute access on instruction memories using the relevant peripheral. If there is any attempt made that breaks these permission attributes, e.g., a write operation to instruction memory region, then a violation interrupt is raised, and it results in system panic.


This feature depends on the config option CONFIG_ESP_SYSTEM_MEMPROT_FEATURE and it is kept enabled by default. Please note that the API for this feature is **private** and used exclusively by ESP-IDF code only.



Note


This feature can help to prevent the possibility of remote code injection due to the existing vulnerabilities in the software.





### Debug Interfaces



#### JTAG


* JTAG interface stays disabled if any of the security features are enabled. Please refer to JTAG with Flash Encryption or Secure Boot for more information.
* JTAG interface can also be disabled in the absence of any other security features using eFuse API.
* ESP32-S3 supports soft disabling the JTAG interface and it can be re-enabled by programming a secret key through HMAC. (HMAC for Enabling JTAG)







#### UART Download Mode


In ESP32-S3, Secure UART Download mode gets activated if any of the security features are enabled.


* Secure UART Download mode can also be enabled by calling `esp_efuse_enable_rom_secure_download_mode()`.
* This mode does not allow any arbitrary code to execute if downloaded through the UART download mode.
* It also limits the available commands in Download mode to update SPI config, e.g., changing baud rate, basic flash write, and the command to return a summary of currently enabled security features (`get_security_info`).
* To disable Download Mode entirely, select the CONFIG_SECURE_UART_ROM_DL_MODE to the recommended option `Permanently disable ROM Download Mode` or call `esp_efuse_disable_rom_download_mode()` at runtime.



Important


In Secure UART Download mode, `esptool.py` can only work with the argument `--no-stub`.







## Network Security



### Wi-Fi


In addition to the traditional security methods (WEP/WPA-TKIP/WPA2-CCMP), Wi-Fi driver in ESP-IDF also supports additional state-of-the-art security protocols. Please refer to the Wi-Fi Security for detailed documentation.




### TLS (Transport Layer Security)


It is recommended to use TLS (Transport Layer Security) in all external communications (e.g., cloud communication, OTA updates) from the ESP device. ESP-IDF supports Mbed TLS as the official TLS stack.


TLS is default integrated in ESP HTTP Client, HTTPS Server and several other components that ship with ESP-IDF.



Note


It is recommended to use the ESP-IDF protocol components in their default configuration, which has been ensured to be secure. Disabling of HTTPS and similar security-critical configurations should be avoided.




#### ESP-TLS Abstraction


ESP-IDF provides an abstraction layer for the most-used TLS functionalities. Hence, it is recommended that an application uses the API exposed by ESP-TLS.


TLS Server Verification section highlights diverse ways in which the identity of server could be established on the device side.




#### ESP Certificate Bundle


The ESP x509 Certificate Bundle API provides an easy way to include a bundle of custom x509 root certificates for TLS server verification. The certificate bundle is the easiest way to verify the identity of almost all standard TLS servers.



Important


It is highly recommended to verify the identity of the server based on X.509 certificates to avoid establishing communication with the **fake** server.





#### Managing Root Certificates


Root Certificates embedded inside the application must be managed carefully. Any update to the root certificate list or the ESP x509 Certificate Bundle can have an impact on the TLS connection with the remote endpoint. This includes a connection to the OTA update server. In some cases, the problem shall be visible on the next OTA update and it may leave device unable to perform OTA updates forever.


Root certificates list update could have following reasons:


* New firmware has different set of remote endpoint(s).
* The existing certificate has expired.
* The certificate has been added or retracted from the upstream certificate bundle.
* The certificate list changed due to market share statistics (`CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEFAULT_CMN` case).


Some guidelines to consider on this topic:


* Please consider enabling OTA rollback and then keep the successful connection to the OTA update server as the checkpoint to cancel the rollback process. This ensures that the newly updated firmware can successfully reach till the OTA update server, otherwise rollback process will go back to the previous firmware on the device.
* If you plan to enable the CONFIG_MBEDTLS_HAVE_TIME_DATE option, then please consider to have the time sync mechanism (SNTP) and sufficient number of trusted certificates in place.






## Product Security



### Secure Provisioning


Secure Provisioning refers to a process of secure on-boarding of the ESP device on to the Wi-Fi network. This mechanism also allows provision of additional custom configuration data during the initial provisioning phase from the provisioning entity, e.g., Smartphone.


ESP-IDF provides various security schemes to establish a secure session between ESP and the provisioning entity, they are highlighted at Security Schemes.


Please refer to the Wi-Fi Provisioning documentation for details and the example code for this feature.



Note


Espressif provides Android and iOS Phone Apps along with their sources, so that it could be easy to further customize them as per the product requirement.





### Secure OTA (Over-the-air) Updates


* OTA Updates must happen over secure transport, e.g., HTTPS.
* ESP-IDF provides a simplified abstraction layer ESP HTTPS OTA for this.
* If Secure Boot is enabled, then the server should host the signed application image.
* If Flash Encryption is enabled, then no additional steps are required on the server side, encryption shall be taken care on the device itself during flash write.
* OTA update Rollback Process can help to switch the application as `active` only after its functionality has been verified.



#### Anti-Rollback Protection


Anti-rollback protection feature ensures that device only executes the application that meets the security version criteria as stored in its eFuse. So even though the application is trusted and signed by legitimate key, it may contain some revoked security feature or credential. Hence, device must reject any such application.


ESP-IDF allows this feature for the application only and it is managed through 2nd stage bootloader. The security version is stored in the device eFuse and it is compared against the application image header during both bootup and over-the-air updates.


Please see more information to enable this feature in the Anti-rollback guide.




#### Encrypted Firmware Distribution


Encrypted firmware distribution during over-the-air updates ensures that the application stays encrypted **in transit** from the server to the the device. This can act as an additional layer of protection on top of the TLS communication during OTA updates and protect the identity of the application.


Please see working example for this documented in OTA Upgrades with Pre-Encrypted Firmware section.





### Secure Storage


Secure storage refers to the application-specific data that can be stored in a secure manner on the device, i.e., off-chip flash memory. This is typically a read-write flash partition and holds device specific configuration data, e.g., Wi-Fi credentials.


ESP-IDF provides the **NVS (Non-volatile Storage)** management component which allows encrypted data partitions. This feature is tied with the platform Flash Encryption feature described earlier.


Please refer to the NVS Encryption for detailed documentation on the working and instructions to enable this feature.



Important


By default, ESP-IDF components writes the device specific data into the default NVS partition, including Wi-Fi credentials too, and it is recommended to protect this data using **NVS Encryption** feature.





### Secure Device Control


ESP-IDF provides capability to control an ESP device over `Wi-Fi/Ethernet + HTTP` or `BLE` in a secure manner using ESP Local Control component.


Please refer to the ESP Local Control for detailed documentation about this feature.





## Security Policy


The ESP-IDF GitHub repository has attached Security Policy Brief.



### Advisories


* Espressif publishes critical Security Advisories, which includes security advisories regarding both hardware and software.
* The specific advisories of the ESP-IDF software components are published through the GitHub repository.




### Software Updates


Critical security issues in the ESP-IDF components, and third-party libraries are fixed as and when we find them or when they are reported to us. Gradually, we make the fixes available in all applicable release branches in ESP-IDF.


Applicable security issues and CVEs for the ESP-IDF components, third-party libraries are mentioned in the ESP-IDF release notes.



Important


We recommend periodically updating to the latest bugfix version of the ESP-IDF release to have all critical security fixes available.