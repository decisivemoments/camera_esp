# Downloadable IDF Tools



The ESP-IDF build process relies on a number of tools: cross-compiler toolchains, CMake build system, and others.


Installing the tools using an OS-specific package manager (e.g., apt, yum, brew, etc.) is the preferred method, when the required version of the tool is available. This recommendation is reflected in the Get Started. For example, on Linux and macOS, it is recommended to install CMake using an OS package manager.


However, some of the tools are specific to ESP-IDF and are not available in OS package repositories. Furthermore, different ESP-IDF versions require different tool versions for proper operation. To solve these two problems, ESP-IDF provides a set of scripts that can download and install the correct tool versions and set up the environment accordingly.


The rest of the document refers to these downloadable tools simply as "tools". Other kinds of tools used in ESP-IDF are:


* Python scripts bundled with ESP-IDF such as `idf.py`
* Python packages installed from PyPI


The following sections explain the installation method and provide the list of tools installed on each platform.



Note


This document is provided for advanced users who need to customize their installation, users who wish to understand the installation process, and ESP-IDF developers.


If you are looking for instructions on how to install the tools, see Get Started.




## Tools Metadata File


The list of tools and tool versions required for each platform is located in tools/tools.json. The schema of this file is defined by tools/tools_schema.json.


This file is used by the tools/idf_tools.py script when installing the tools or setting up the environment variables.




## Tools Installation Directory


The `IDF_TOOLS_PATH` environment variable specifies the location where the tools are to be downloaded and installed. If not set, the default location will be `HOME/.espressif` on Linux and macOS, and `%USER_PROFILE%\.espressif` on Windows.


Inside the `IDF_TOOLS_PATH` directory, the tools installation scripts create the following directories and files:


* `dist` — where the archives of the tools are downloaded.
* `tools` — where the tools are extracted. The tools are extracted into subdirectories: `tools/TOOL_NAME/VERSION/`. This arrangement allows different versions of tools to be installed side by side.
* `idf-env.json` — user install options, such as targets and features, are stored in this file. Targets are selected chip targets for which tools are installed and kept up-to-date. Features determine the Python package set which should be installed. These options will be discussed later.
* `python_env` — not related to the tools; virtual Python environments are installed in the sub-directories. Note that the Python environment directory can be placed elsewhere by setting the `IDF_PYTHON_ENV_PATH` environment variable.
* `espidf.constraints.*.txt` — one constraint file for each ESP-IDF release containing Python package version requirements.




## GitHub Assets Mirror


Most of the tools downloaded by the tools installer are GitHub Release Assets, which are files attached to a software release on GitHub.


If GitHub downloads are inaccessible or slow to access, a GitHub assets mirror can be configured.


To use Espressif's download server, set the environment variable `IDF_GITHUB_ASSETS` to `dl.espressif.com/github_assets`, or `dl.espressif.cn/github_assets` for faster download in China. When the install process is downloading a tool from `github.com`, the URL will be rewritten to use this server instead.


Any mirror server can be used provided the URL matches the `github.com` download URL format. For any GitHub asset URL that the install process downloads, it will replace `https://github.com` with `https://${IDF_GITHUB_ASSETS}`.



Note


The Espressif download server currently does not mirror everything from GitHub, but only files attached as Assets to some releases, as well as source archives for some releases.





## `idf_tools.py` Script


The tools/idf_tools.py script bundled with ESP-IDF performs several functions:


* `install`: Download the tool into the `${IDF_TOOLS_PATH}/dist` directory and extract it into `${IDF_TOOLS_PATH}/tools/TOOL_NAME/VERSION`.


The `install` command accepts the list of tools to install in the `TOOL_NAME` or `TOOL_NAME@VERSION` format. If `all` is given, all the tools, including required and optional ones, are installed. If no argument or `required` is given, only the required tools are installed.
* `download`: Similar to `install` but doesn't extract the tools. An optional `--platform` argument may be used to download the tools for the specific platform.
* `export`: Lists the environment variables that need to be set to use the installed tools. For most of the tools, setting the `PATH` environment variable is sufficient, but some tools require extra environment variables.


The environment variables can be listed in either `shell` or `key-value` formats, which can be set using the `--format` parameter:


	+ `export` optional parameters:
	
	
		- `--unset`: Creates a statement that unsets specific global variables and restores the environment to its state before calling `export.{sh/fish}`.
		- `--add_paths_extras`: Adds extra ESP-IDF-related paths of `$PATH` to `${IDF_TOOLS_PATH}/esp-idf.json`, which is used to remove global variables when the active ESP-IDF environment is deactivated. For example, while processing the `export.{sh/fish}` script, if new paths are added to the global variable `$PATH`, this option saves these new paths to the `${IDF_TOOLS_PATH}/esp-idf.json` file.
	+ `shell`: Produces output suitable for evaluation in the shell. For example, produce the following output on Linux and macOS:
	
	
	
	```
	export PATH="/home/user/.espressif/tools/tool/v1.0.0/bin:$PATH"
	
	```
	
	
	Produce the following output on Windows:
	
	
	
	```
	set "PATH=C:\Users\user\.espressif\tools\v1.0.0\bin;%PATH%"
	
	```
	
	
	
	Note
	
	
	Exporting environment variables in Powershell format is not supported at the moment. `key-value` format may be used instead.
	
	
	
	The output of this command may be used to update the environment variables if the shell supports it. For example
	
	
	
	```
	eval $($IDF_PATH/tools/idf_tools.py export)
	
	```
	+ `key-value`: Produces output in the `VARIABLE=VALUE` format that is suitable for parsing by other scripts
	
	
	
	```
	PATH=/home/user/.espressif/tools/tool/v1.0.0:$PATH
	
	```
	
	
	Note that the script consuming this output has to perform expansion of `$VAR` or `%VAR%` patterns found in the output.
* `list`: Lists the known versions of the tools, and indicates which ones are installed.


The following option is available to customize the output.


	+ `--outdated`: Lists only outdated versions of tools installed in `IDF_TOOLS_PATH`.
* `check`: For each tool, checks whether the tool is available in the system path and in `IDF_TOOLS_PATH`.
* `install-python-env`: Creates a Python virtual environment in the `${IDF_TOOLS_PATH}/python_env` directory or directly in the directory set by the `IDF_PYTHON_ENV_PATH` environment variable, and install the required Python packages there.


	+ An optional `--features` argument allows one to specify a comma-separated list of features to be added or removed.
	
	
		1. A feature that begins with `-` will be removed, and features with `+` or without any sign will be added. Example syntax for removing feature `XY` is `--features=-XY`, and for adding feature `XY` is `--features=+XY` or `--features=XY`. If both removing and adding options are provided with the same feature, no operation is performed.
		2. For each feature, a requirements file must exist. For example, feature `XY` is a valid feature if `${IDF_PATH}/tools/requirements/requirements.XY.txt` is an existing file with a list of Python packages to be installed.
		3. There is one mandatory `core` feature ensuring the core functionality of ESP-IDF, e.g., build, flash, monitor, debug in console. There can be an arbitrary number of optional features.
		4. The selected list of features is stored in `idf-env.json`.
		5. The requirement files contain a list of the desired Python packages to be installed and the `espidf.constraints.*.txt` file downloaded from https://dl.espressif.com and stored in `${IDF_TOOLS_PATH}`, which contains the package version requirements for a given ESP-IDF version.
Note


Although **it is not recommended**, the download and use of constraint files can be disabled with the `--no-constraints` argument or setting the `IDF_PYTHON_CHECK_CONSTRAINTS` environment variable to `no`.
* `check-python-dependencies`: Checks if all required Python packages are installed. Packages from `${IDF_PATH}/tools/requirements/requirements.*.txt` files selected by the feature list of `idf-env.json` are checked with the package versions specified in the `espidf.constraints.*.txt` file.



Note


The constraint file is downloaded with the `install-python-env` command. Similar to the `install-python-env` command, the use of constraint files can be disabled with the `--no-constraints` argument or setting the `IDF_PYTHON_CHECK_CONSTRAINTS` environment variable to `no`.
* `uninstall`: Prints and removes tools that are currently not used by the active ESP-IDF version.


	+ `--dry-run`: Prints installed unused tools.
	+ `--remove-archives`: Additionally removes all older versions of previously downloaded installation packages.




## Install Scripts


Shell-specific user-facing installation scripts are provided in the root directory of ESP-IDF repository to facilitate tools installation. These are:


* `install.bat` for Windows Command Prompt
* `install.ps1` for Powershell
* `install.sh` for Bash
* `install.fish` for Fish


Apart from downloading and installing the tools in `IDF_TOOLS_PATH`, these scripts prepare a Python virtual environment, and install the required packages into that environment.


These scripts accept optionally a comma-separated list of chip targets and `--enable-*` arguments for enabling features. These arguments are passed to the `idf_tools.py` script which stores them in `idf-env.json`. Therefore, chip targets and features can be enabled incrementally.


To install tools for all chip targets, run the scripts without any optional arguments using `idf_tools.py install --targets=all`. Similarly, to install Python packages for core ESP-IDF functionality, run `idf_tools.py install-python-env --features=core`.


It is also possible to install tools for specific chip targets. For example, `install.sh esp32` installs tools only for ESP32. See Step 3. Set up the Tools for more examples.


`install.sh --enable-XY` enables feature `XY` (by running `idf_tools.py install-python-env --features=core,XY`).




## Export Scripts


Since the installed tools are not permanently added to the user or system `PATH` environment variable, an extra step is required to use them in the command line. The following scripts modify the environment variables in the current shell to make the correct versions of the tools available:


* `export.bat` for Windows Command Prompt
* `export.ps1` for Powershell
* `export.sh` for Bash
* `export.fish` for Fish



Note


To modify the shell environment in Bash, `export.sh` must be "sourced" by using the `. ./export.sh` command. Please ensure to include the leading dot and space.


`export.sh` may be used with shells other than Bash (such as zsh). However, in this case, it is required to set the `IDF_PATH` environment variable before running the script. When used in Bash, the script guesses the `IDF_PATH` value from its own location.



In addition to calling `idf_tools.py`, these scripts list the directories that have been added to the `PATH`.




## Other Installation Methods


Depending on the environment, more user-friendly wrappers for `idf_tools.py` are provided:


* ESP-IDF Tools Installer can download and install the tools. Internally the installer uses `idf_tools.py`.
* ESP-IDF Eclipse Plugin includes a menu item to set up the tools. Internally the plugin calls `idf_tools.py`.
* VSCode ESP-IDF Extension includes an onboarding flow. This flow helps set up the tools. Although the extension does not rely on `idf_tools.py`, the same installation method is used.




## Custom Installation


Although the methods above are recommended for ESP-IDF users, they are not a must for building ESP-IDF applications. ESP-IDF build system expects that all the necessary tools are installed somewhere, and made available in the `PATH`.




## Uninstall ESP-IDF


Uninstalling ESP-IDF requires removing both the tools and the environment variables that have been configured during the installation.


* Windows users using the Windows ESP-IDF Tools Installer can simply run the uninstall wizard to remove ESP-IDF.
* To remove an installation performed by running the supported install scripts, simply delete the tools installation directory including the downloaded and installed tools. Any environment variables set by the export scripts are not permanent and will not be present after opening a new environment.
* When dealing with a custom installation, in addition to deleting the tools as mentioned above, you may also need to manually revert any changes to environment variables or system paths that were made to accommodate the ESP-IDF tools (e.g., `IDF_PYTHON_ENV_PATH` or `IDF_TOOLS_PATH`). If you manually copied any tools, you would need to track and delete those files manually.
* If you installed any plugins like the ESP-IDF Eclipse Plugin or VSCode ESP-IDF Extension, you should follow the specific uninstallation instructions described in the documentation of those components.



Note


Uninstalling the ESP-IDF tools does not remove any project files or your code. Be mindful of what you are deleting to avoid losing any work. If you are unsure about a step, refer back to the installation instructions.


These instructions assume that the tools were installed following the procedures in this provided document. If you've used a custom installation method, you might need to adapt these instructions accordingly.





## List of ESP-IDF Tools



### xtensa-esp-elf-gdb


GDB for Xtensa


License: GPL-3.0-or-later


More info: https://github.com/espressif/binutils-gdb








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-x86_64-linux-gnu.tar.gz SHA256: 9d68472d4cba5cf8c2b79d94f86f92c828e76a632bd1e6be5e7706e5b304d36e |
| linux-arm64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-aarch64-linux-gnu.tar.gz SHA256: bdabc3217994815fc311c4e16e588b78f6596b5ad4ffa46c80b40e982cfb1e66 |
| linux-armel | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-arm-linux-gnueabi.tar.gz SHA256: d54b8d703ba897b28c627da3d27106a3906dd01ba298778a67064710bc33c76d |
| linux-armhf | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-arm-linux-gnueabihf.tar.gz SHA256: 6187d1dd54e57927f7a7b804ff431fe0a295d5d5638c7654ee2bb7c3e0e84d4b |
| linux-i686 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-i586-linux-gnu.tar.gz SHA256: 64d3bc992ed8fdec383d49e8b803ac494605a38117c8293db8da055037de96b0 |
| macos | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-x86_64-apple-darwin14.tar.gz SHA256: 023e74b3fda793da4bc0509b02de776ee0dad6efaaac17bef5916fb7dc9c26b9 |
| macos-arm64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-aarch64-apple-darwin21.1.tar.gz SHA256: ea757c6bf8c25238f6d2fdcc6bbab25a1b00608a0f9e19b7ddd2f37ddbdc3fb1 |
| win32 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-i686-w64-mingw32.zip SHA256: 322e8d9b700dc32d8158e3dc55fb85ec55de48d0bb7789375ee39a28d5d655e2 |
| win64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/xtensa-esp-elf-gdb-14.2_20240403-x86_64-w64-mingw32.zip SHA256: a27a2fe20f192f8e0a51b8936428b4e1cf8935cfe008ee445cc49f6fc7f6db2e |




### riscv32-esp-elf-gdb


GDB for RISC-V


License: GPL-3.0-or-later


More info: https://github.com/espressif/binutils-gdb








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-x86_64-linux-gnu.tar.gz SHA256: ce004bc0bbd71b246800d2d13b239218b272a38bd528e316f21f1af2db8a4b13 |
| linux-arm64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-aarch64-linux-gnu.tar.gz SHA256: ba10f2866c61410b88c65957274280b1a62e3bed05131654ed9b6758efe18e55 |
| linux-armel | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-arm-linux-gnueabi.tar.gz SHA256: 88539db5d987f28827efac7e26080a2803b9b539342ccd2963ccfdd56d7f08f7 |
| linux-armhf | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-arm-linux-gnueabihf.tar.gz SHA256: b45b9711d6a87d4c2f688a9599ce850ce02f477756e3e797c4a6c1c549127fcb |
| linux-i686 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-i586-linux-gnu.tar.gz SHA256: 0e628ee37438ab6ba05eb889a76d09e50cb98e0020a16b8e2b935c5cf19b4ed2 |
| macos | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-x86_64-apple-darwin14.tar.gz SHA256: 8f6bda832d70dad5860a639d55aba4237bd10cbac9f4822db1eece97357b34a9 |
| macos-arm64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-aarch64-apple-darwin21.1.tar.gz SHA256: d88b6116e86456c8480ce9bc95aed375a35c0d091f1da0a53b86be0e6ef3d320 |
| win32 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-i686-w64-mingw32.zip SHA256: d6e7ce05805b0d8d4dd138ad239b98a1adf8da98941867d60760eb1ae5361730 |
| win64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v14.2_20240403/riscv32-esp-elf-gdb-14.2_20240403-x86_64-w64-mingw32.zip SHA256: 5c9f211dc46daf6b96fad09d709284a0f0186fef8947d9f6edd6bca5b5ad4317 |




### xtensa-esp-elf


Toolchain for 32-bit Xtensa based on GCC


License: GPL-3.0-with-GCC-exception


More info: https://github.com/espressif/crosstool-NG








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-x86_64-linux-gnu.tar.xz SHA256: fcef03d87eac44c0dbee2bbee98443ed2fcf82720dcd8ebfe00640807b0f07c2 |
| linux-arm64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-aarch64-linux-gnu.tar.xz SHA256: cfe55b92b4baeaa4309a948ba65e2adfc2d17a542c64856e36650869b419574a |
| linux-armel | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-arm-linux-gnueabi.tar.xz SHA256: c57a062969ec3d98b02a97cd9240eb31091957788509b60c356b0a6f23032669 |
| linux-armhf | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-arm-linux-gnueabihf.tar.xz SHA256: 1adc660f4d7bcf863f54051c5843719456fabc7203c1d4ccbb855924fda82987 |
| linux-i686 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-i586-linux-gnu.tar.xz SHA256: f9203673aa0c42b041847c86b07e6f5b4aa9c90e6ff03d3cd3146928784447ea |
| macos | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-x86_64-apple-darwin.tar.xz SHA256: 39ee7df749f4ceb93624d73627688d5b86269a7429022f986f2940499936aacd |
| macos-arm64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-aarch64-apple-darwin.tar.xz SHA256: d967e49a64f823e18fbae273efb1b094ac55e2207aa21fd3947c9d59f999f47e |
| win32 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-i686-w64-mingw32_hotfix.zip SHA256: d6b227c50e3c8e21d62502b3140e5ab74a4cb502c2b4169c36238b9858a8fb88 |
| win64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/xtensa-esp-elf-13.2.0_20240530-x86_64-w64-mingw32_hotfix.zip SHA256: 155ee97b531236e6a7c763395c68ca793e55e74d2cb4d38a23057a153e01e7d0 |




### esp-clang


Toolchain for all Espressif chips based on clang


License: Apache-2.0


More info: https://github.com/espressif/llvm-project








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | optional | https://github.com/espressif/llvm-project/releases/download/esp-16.0.0-20230516/llvm-esp-16.0.0-20230516-linux-amd64.tar.xz SHA256: 3dbd8dd290913a93e8941da8a451ecd49f9798cc2d74bb9b63ef5cf5c4fee37f |
| linux-arm64 | optional | https://github.com/espressif/llvm-project/releases/download/esp-16.0.0-20230516/llvm-esp-16.0.0-20230516-linux-arm64.tar.xz SHA256: 4b115af6ddd04a9bffc1908fc05837998ee71d450891d741c446186f2aa9b961 |
| linux-armhf | optional | https://github.com/espressif/llvm-project/releases/download/esp-16.0.0-20230516/llvm-esp-16.0.0-20230516-linux-armhf.tar.xz SHA256: 935082bb0704420c5ca42b35038bba8702135348a50cac454ae2fb55af0b4c32 |
| macos | optional | https://github.com/espressif/llvm-project/releases/download/esp-16.0.0-20230516/llvm-esp-16.0.0-20230516-macos.tar.xz SHA256: d9824acafd3e7b1d17ace084243b82a95bbdcb149a26b085bba487ab3d3716d7 |
| macos-arm64 | optional | https://github.com/espressif/llvm-project/releases/download/esp-16.0.0-20230516/llvm-esp-16.0.0-20230516-macos-arm64.tar.xz SHA256: ed5621396dc3e48413e14e8b6caed8e2993e7f2ab5fca1410081f40c940a1060 |
| win64 | optional | https://github.com/espressif/llvm-project/releases/download/esp-16.0.0-20230516/llvm-esp-16.0.0-20230516-win64.tar.xz SHA256: 598c8241c8bf10fd1be8bd21845307cfc404e127041b4ba4e828350a88692883 |




### riscv32-esp-elf


Toolchain for 32-bit RISC-V based on GCC


License: GPL-3.0-with-GCC-exception


More info: https://github.com/espressif/crosstool-NG








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-x86_64-linux-gnu.tar.xz SHA256: f69a491d2f42f63e119f9077da995f7743ea8e1bf6944166a42a312cf60728a8 |
| linux-arm64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-aarch64-linux-gnu.tar.xz SHA256: 276351b883a53e81b695d858be74114a8b627bbe4fc9c69ef46a7127ab143680 |
| linux-armel | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-arm-linux-gnueabi.tar.xz SHA256: 14890f2a624e70f11da7268347adf25b6c396f42bcd4d8ac3c5bfa4050b7c934 |
| linux-armhf | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-arm-linux-gnueabihf.tar.xz SHA256: b61ca9ceff25986ec1d166a01319bff09639be1d4ee5bf117502ce564fdae7e9 |
| linux-i686 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-i586-linux-gnu.tar.xz SHA256: 12ef50f96deb9040ce360974a4237c64ae0706b0c429b90cecc8ab664cf6dbb4 |
| macos | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-x86_64-apple-darwin.tar.xz SHA256: cfbf5deaba05bf217701c8ceab7396bb0c2ca95ab58e134d4b2e175b86c2fd6c |
| macos-arm64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-aarch64-apple-darwin.tar.xz SHA256: 230628fcf464ca8856c82c55514e40a8919e97fbc5e66b7165ca42c9653d2302 |
| win32 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-i686-w64-mingw32.zip SHA256: 590bfb10576702639825581cc00c445da6e577012840a787137417e80d15f46d |
| win64 | required | https://github.com/espressif/crosstool-NG/releases/download/esp-13.2.0_20240530/riscv32-esp-elf-13.2.0_20240530-x86_64-w64-mingw32.zip SHA256: 413eb9f6adf8fdaf25544d014c850fc09eb38bb93a2fc5ebd107ab1b0de1bb3a |




### esp32ulp-elf


Toolchain for ESP32 ULP coprocessor


License: GPL-3.0-or-later


More info: https://github.com/espressif/binutils-gdb








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-linux-amd64.tar.gz SHA256: d13a808365b78465fa6591636dfbbb9604d9d15a397c3d9cd22626d54828ac2c |
| linux-arm64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-linux-arm64.tar.gz SHA256: ecce0788ce1000e5c669c5adaf2fd5bf7f9bf96dcdbd3555d1d9ce4dcb311038 |
| linux-armel | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-linux-armel.tar.gz SHA256: 7228b01277f7908d72eb659470f82e143c4c66b444538a464290d88ece16130e |
| linux-armhf | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-linux-armhf.tar.gz SHA256: 951b089c66561bc2190a8d57c316dfaef985a778728a7c30e1edcd29fe180016 |
| linux-i686 | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-linux-i686.tar.gz SHA256: df323d40962313168f6feeb2d9471c6010ff23a7896f40244e62991517d9745b |
| macos | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-macos.tar.gz SHA256: b2aeba8eaafdf156e9e30be928dde1f133b00eaf33802d96827ec544ac7c864c |
| macos-arm64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-macos-arm64.tar.gz SHA256: e3a4dfea043e2bce8cd00b3a0b260a59249fa61ca5931bf02f18a3d43c18deb4 |
| win32 | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-win32.zip SHA256: d33b64f49df27dcfa4a24d3af1a5ead77b020f85f33448994c31b98f88e66bb4 |
| win64 | required | https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-win64.zip SHA256: 3a7627008ac92d1580542b95c696449e56aaa1d0881dc3ef5fd5c60afc77a49d |




### cmake


CMake build system


On Linux and macOS, it is recommended to install CMake using the OS-specific package manager (like apt, yum, brew, etc.). However, for convenience it is possible to install CMake using idf_tools.py along with the other tools.


License: BSD-3-Clause


More info: https://github.com/Kitware/CMake








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | optional | https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-linux-x86_64.tar.gz SHA256: 726f88e6598523911e4bce9b059dc20b851aa77f97e4cc5573f4e42775a5c16f |
| linux-arm64 | optional | https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-linux-aarch64.tar.gz SHA256: 50c3b8e9d3a3cde850dd1ea143df9d1ae546cbc5e74dc6d223eefc1979189651 |
| linux-armel | optional | https://dl.espressif.com/dl/cmake/cmake-3.24.0-Linux-armv7l.tar.gz SHA256: 7dc787ef968dfef92491a4f191b8739ff70f8a649608b811c7a737b52481beb0 |
| linux-armhf | optional | https://dl.espressif.com/dl/cmake/cmake-3.24.0-Linux-armv7l.tar.gz SHA256: 7dc787ef968dfef92491a4f191b8739ff70f8a649608b811c7a737b52481beb0 |
| macos | optional | https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-macos-universal.tar.gz SHA256: 3e0cca74a56d9027dabb845a5a26e42ef8e8b33beb1655d6a724187a345145e4 |
| macos-arm64 | optional | https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-macos-universal.tar.gz SHA256: 3e0cca74a56d9027dabb845a5a26e42ef8e8b33beb1655d6a724187a345145e4 |
| win32 | required | https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-windows-x86_64.zip SHA256: b1ad8c2dbf0778e3efcc9fd61cd4a962e5c1af40aabdebee3d5074bcff2e103c |
| win64 | required | https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-windows-x86_64.zip SHA256: b1ad8c2dbf0778e3efcc9fd61cd4a962e5c1af40aabdebee3d5074bcff2e103c |




### openocd-esp32


OpenOCD for ESP32


License: GPL-2.0-only


More info: https://github.com/espressif/openocd-esp32








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-linux-amd64-0.12.0-esp32-20240318.tar.gz SHA256: cf26c5cef4f6b04aa23cd2778675604e5a74a4ce4d8d17b854d05fbcb782d52c |
| linux-arm64 | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-linux-arm64-0.12.0-esp32-20240318.tar.gz SHA256: 9b97a37aa2cab94424a778c25c0b4aa0f90d6ef9cda764a1d9289d061305f4b7 |
| linux-armel | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-linux-armel-0.12.0-esp32-20240318.tar.gz SHA256: b7e82776ec374983807d3389df09c632ad9bc8341f2075690b6b500319dfeaf4 |
| linux-armhf | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-linux-armhf-0.12.0-esp32-20240318.tar.gz SHA256: 16f8f65f12e5ba034d328cda2567d6851a2aceb3c957d577f89401c2e1d3f93a |
| macos | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-macos-0.12.0-esp32-20240318.tar.gz SHA256: b16c3082c94df1079367c44d99f7a8605534cd48aabc18898e46e94a2c8c57e7 |
| macos-arm64 | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-macos-arm64-0.12.0-esp32-20240318.tar.gz SHA256: 534ec925ae6e35e869e4e4e6e4d2c4a1eb081f97ebcc2dd5efdc52d12f4c2f86 |
| win32 | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-win32-0.12.0-esp32-20240318.zip SHA256: d379329eba052435173ab0d69c9b15bc164a6ce489e2a67cd11169d2dabff633 |
| win64 | required | https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20240318/openocd-esp32-win32-0.12.0-esp32-20240318.zip SHA256: d379329eba052435173ab0d69c9b15bc164a6ce489e2a67cd11169d2dabff633 |




### ninja


Ninja build system


On Linux and macOS, it is recommended to install ninja using the OS-specific package manager (like apt, yum, brew, etc.). However, for convenience it is possible to install ninja using idf_tools.py along with the other tools.


License: Apache-2.0


More info: https://github.com/ninja-build/ninja








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | optional | https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip SHA256: b901ba96e486dce377f9a070ed4ef3f79deb45f4ffe2938f8e7ddc69cfb3df77 |
| macos | optional | https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-mac.zip SHA256: 482ecb23c59ae3d4f158029112de172dd96bb0e97549c4b1ca32d8fad11f873e |
| macos-arm64 | optional | https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-mac.zip SHA256: 482ecb23c59ae3d4f158029112de172dd96bb0e97549c4b1ca32d8fad11f873e |
| win64 | required | https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip SHA256: 524b344a1a9a55005eaf868d991e090ab8ce07fa109f1820d40e74642e289abc |




### idf-exe


IDF wrapper tool for Windows


License: Apache-2.0


More info: https://github.com/espressif/idf_py_exe_tool








| Platform | Required | Download |
| --- | --- | --- |
| win32 | required | https://github.com/espressif/idf_py_exe_tool/releases/download/v1.0.3/idf-exe-v1.0.3.zip SHA256: 7c81ef534c562354a5402ab6b90a6eb1cc8473a9f4a7b7a7f93ebbd23b4a2755 |
| win64 | required | https://github.com/espressif/idf_py_exe_tool/releases/download/v1.0.3/idf-exe-v1.0.3.zip SHA256: 7c81ef534c562354a5402ab6b90a6eb1cc8473a9f4a7b7a7f93ebbd23b4a2755 |




### ccache


Ccache (compiler cache)


License: GPL-3.0-or-later


More info: https://github.com/ccache/ccache








| Platform | Required | Download |
| --- | --- | --- |
| win64 | required | https://github.com/ccache/ccache/releases/download/v4.8/ccache-4.8-windows-x86_64.zip SHA256: a2b3bab4bb8318ffc5b3e4074dc25636258bc7e4b51261f7d9bef8127fda8309 |




### dfu-util


dfu-util (Device Firmware Upgrade Utilities)


License: GPL-2.0-only


More info: http://dfu-util.sourceforge.net/








| Platform | Required | Download |
| --- | --- | --- |
| win64 | required | https://dl.espressif.com/dl/dfu-util-0.11-win64.zip SHA256: 652eb94cb1c074c6dbead9e47adb628922aeb198a4d440a346ab32e7a0e9bf64 |




### esp-rom-elfs


ESP ROM ELFs


License: Apache-2.0


More info: https://github.com/espressif/esp-rom-elfs








| Platform | Required | Download |
| --- | --- | --- |
| any | required | https://github.com/espressif/esp-rom-elfs/releases/download/20240305/esp-rom-elfs-20240305.tar.gz SHA256: a26609b415710f0163d785850c769752717004059c129c472e9a0cbd54e0422c |




### qemu-xtensa


QEMU for Xtensa


Some ESP-specific instructions for running QEMU for Xtensa chips are here: https://github.com/espressif/esp-toolchain-docs/blob/main/qemu/esp32/README.md


License: GPL-2.0-only


More info: https://github.com/espressif/qemu








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-xtensa-softmmu-esp_develop_9.0.0_20240606-x86_64-linux-gnu.tar.xz SHA256: 071d117c44a6e9a1bc8664ab63b592d3e17ceb779119dcb46c59571a4a7a88c9 |
| linux-arm64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-xtensa-softmmu-esp_develop_9.0.0_20240606-aarch64-linux-gnu.tar.xz SHA256: 43552f32b303a6820d0d9551903e54fc221aca98ccbd04e5cbccbca881548008 |
| macos | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-xtensa-softmmu-esp_develop_9.0.0_20240606-x86_64-apple-darwin.tar.xz SHA256: 0096734280ce04f558cd9bd72f35db39667f80d44309a35565f2f8c02d1f9cc3 |
| macos-arm64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-xtensa-softmmu-esp_develop_9.0.0_20240606-aarch64-apple-darwin.tar.xz SHA256: fb4ca6be7b1a4dbcf153879cf0582300f974371def0826c0c5b728f12812ad08 |
| win64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-xtensa-softmmu-esp_develop_9.0.0_20240606-x86_64-w64-mingw32.tar.xz SHA256: 281659f7a1d49761ac6f54d0aeb14366cb93c002f21948b847a0e15c0b8f5425 |




### qemu-riscv32


QEMU for RISC-V


Some ESP-specific instructions for running QEMU for RISC-V chips are here: https://github.com/espressif/esp-toolchain-docs/blob/main/qemu/esp32c3/README.md


License: GPL-2.0-only


More info: https://github.com/espressif/qemu








| Platform | Required | Download |
| --- | --- | --- |
| linux-amd64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-riscv32-softmmu-esp_develop_9.0.0_20240606-x86_64-linux-gnu.tar.xz SHA256: 47120e826cfec7180db8cb611a7a4aed2e9b2191c2a739194f8ce085e63cdd8d |
| linux-arm64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-riscv32-softmmu-esp_develop_9.0.0_20240606-aarch64-linux-gnu.tar.xz SHA256: 3b6221a8b1881d2c9b9fa0b0bf8d7065c84153d2a54e429307bde9feae235c27 |
| macos | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-riscv32-softmmu-esp_develop_9.0.0_20240606-x86_64-apple-darwin.tar.xz SHA256: 3afa55d5abea52ccf18d0bc41fe819d568bd4ee1582989b1ee9b1ee4a609a31e |
| macos-arm64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-riscv32-softmmu-esp_develop_9.0.0_20240606-aarch64-apple-darwin.tar.xz SHA256: 69ba5154594fb2922d5490a49ea6b4925c024c6c37f875b42f9885f513e0bcdd |
| win64 | optional | https://github.com/espressif/qemu/releases/download/esp-develop-9.0.0-20240606/qemu-riscv32-softmmu-esp_develop_9.0.0_20240606-x86_64-w64-mingw32.tar.xz SHA256: f49bb5c8f4d6e2cfbf7eeec21eb8ef190a57307778705bc689536ac13bde511c |