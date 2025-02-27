# Building OpenOCD from Sources for MacOS



The following instructions are alternative to downloading binary OpenOCD from Espressif GitHub. To quickly setup the binary OpenOCD, instead of compiling it yourself, backup and proceed to section Setup of OpenOCD.



## Download Sources of OpenOCD


The sources for the ESP32-S3-enabled variant of OpenOCD are available from Espressif GitHub under https://github.com/espressif/openocd-esp32. To download the sources, use the following commands:



```
cd ~/esp
git clone --recursive https://github.com/espressif/openocd-esp32.git

```


The clone of sources should be now saved in `~/esp/openocd-esp32` directory.




## Install Dependencies


Install packages that are required to compile OpenOCD using Homebrew:



```
brew install automake libtool libusb wget gcc@4.9 pkg-config

```




## Build OpenOCD


Proceed with configuring and building OpenOCD:



```
cd ~/esp/openocd-esp32
./bootstrap
./configure
make

```


Optionally you can add `sudo make install` step at the end. Skip it, if you have an existing OpenOCD (from e.g., another development platform), as it may get overwritten.



Note


* Should an error occur, resolve it and try again until the command `make` works.
* Error `Unknown command 'raggedright'` may indicate that the required version of `texinfo` was not installed on your computer or installed but was not linked to your PATH. To resolve this issue make sure `texinfo` is installed and PATH is adjusted prior to the `./bootstrap` by running:



```
brew install texinfo
export PATH=/usr/local/opt/texinfo/bin:$PATH

```
* If there is a submodule problem from OpenOCD, please `cd` to the `openocd-esp32` directory and input `git submodule update --init`.
* If the `./configure` is successfully run, information of enabled JTAG will be printed under `OpenOCD configuration summary`.
* If the information of your device is not shown in the log, use `./configure` to enable it as described in `../openocd-esp32/doc/INSTALL.txt`.
* For details concerning compiling OpenOCD, please refer to `openocd-esp32/README.OSX`.



Once `make` process is successfully completed, the executable of OpenOCD will be saved in `~/esp/openocd-esp32/src/openocd` directory.




## Next Steps


To carry on with debugging environment setup, proceed to section Configuring ESP32-S3 Target.