# Anatomy of Core Dump Image



A core dump file's format can be configured to use the ELF format, or a legacy binary format. The ELF format is recommended for all new designs as it provides more information regarding the software's state at the moment the crash occurs, e.g., CPU registers and memory contents.


The memory state embeds a snapshot of all tasks mapped in the memory space of the program. The CPU state contains register values when the core dump has been generated. The core dump file uses a subset of the ELF structures to register this information.


Loadable ELF segments are used to store the process' memory state, while ELF notes (`ELF.PT_NOTE`) are used to store the process' metadata (e.g., PID, registers, signal etc). In particular, the CPU's status is stored in a note with a special name and type (`CORE`, `NT_PRSTATUS type`).


Here is an overview of the core dump layout:



!Core Dump ELF Image Format



Core Dump ELF Image Format







!Core Dump Binary Image Format



Core Dump Binary Image Format







Note


The format of the image file shown in the above pictures represents the current version of the image and can be changed in future releases.





# Overview of Implementation


The figure below describes some basic aspects related to the implementation of the core dump:



!Core Dump Implementation Overview



Core Dump Implementation Overview







Note


The diagram above hides some details and represents the current implementation of the core dump which can be changed later.