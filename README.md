fixed_string
============

string library optimized to only use stack. Length is known at compile-time and cannot be changed. Ever.

The development of this library is initiated to increase the usage of C++ on memory constrained devices such as small embedded systems. By creating a heap-free library, programmers don't need to worry about heap deallocation introducing defragmentation of the device's memory. This allows for more predictive realtime behaviour of any program. The downside is that every string has a maximum length, which cannot be changed at any time. 
