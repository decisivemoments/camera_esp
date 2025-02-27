# C Support



ESP-IDF is primarily written in C and provides C APIs. Newlib is used as standard C library (the Newlib version can be found in newlib/sbom.yml). In general, all C features supported by the compiler, currently GCC, should be available in ESP-IDF, unless specified in Unsupported C Features below.



## C Version


**GNU dialect of ISO C17** (`--std=gnu17`) is the current default C version in ESP-IDF.


To compile the source code of a certain component using a different language standard, set the desired compiler flag in the component's `CMakeLists.txt` file:



```
idf_component_register( ... )
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu11)

```


If the public header files of the component also need to be compiled with the same language standard, replace the flag `PRIVATE` with `PUBLIC`.




## Unsupported C Features


The following features are not supported in ESP-IDF.



### Nested Function Pointers


The **GNU dialect of ISO C17** supports nested functions. However, they do not work in ESP-IDF when referenced as function pointer because the compiler generates a trampoline on the stack, while the stack is not executable in ESP-IDF. Hence, do not use function pointers to nested functions.