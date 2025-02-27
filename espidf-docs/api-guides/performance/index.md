# Performance



ESP-IDF ships with default settings that are designed for a trade-off between performance, resource usage, and available functionality.


These guides describe how to optimize a firmware application for a particular aspect of performance. Usually this involves some trade-off in terms of limiting available functions, or swapping one aspect of performance (such as execution speed) for another (such as RAM usage).



## How to Optimize Performance


1. Decide the performance-critical aspects of your application, such as achieving a particular response time for a certain network operation, meeting a particular startup time limit, or maintaining a certain level of peripheral data throughput.
2. Find a way to measure this performance (some methods are outlined in the guides below).
3. Modify the code and project configuration and compare the new measurement to the old measurement.
4. Repeat step 3 until the performance meets the requirements set out in step 1.




## Guides



* Speed Optimization
	+ Overview
	+ Choose What to Optimize
	+ Measuring Performance
	+ Improving Overall Speed
	+ Targeted Optimizations
	+ Improving Startup Time
	+ Task Priorities
	+ Improving Interrupt Performance
	+ Improving Network Speed
	+ Improving I/O Performance
* Minimizing Binary Size
	+ Measuring Static Sizes
	+ Reducing Overall Size
	+ Bootloader Size
	+ IRAM Binary Size
* Minimizing RAM Usage
	+ Background
	+ Measuring Static Memory Usage
	+ Measuring Dynamic Memory Usage
	+ Reducing Static Memory Usage
	+ Reducing Stack Sizes
	+ Reducing Heap Usage
	+ Optimizing IRAM Usage