fixed_string
============

string library optimized to only use stack. Length is known at compile-time and cannot be changed. Ever.

=== Summary ===

The development of this library is initiated to increase the usage of C++ on memory constrained devices such as small embedded systems. By creating a heap-free library, programmers don't need to worry about heap deallocation (which could introduce defragmentation of the device's memory).  This allows for more predictive realtime behaviour of any program. The downside is that every string has a maximum length, which cannot be changed at any time. 

=== usage ===

To compile with google test, run 
make clean && cmake CMakeLists.txt && make && ./runTests 
where main.cpp contains the references to the gtest library (in the example main.cpp gtest files are located in PATH, make sure your PATH is correct and set up properly).

To use the library, just instantiate objects like
fixed_string<10> fs; // Or use any other provided constructor

fs = "hello world"; //
fs.c_str(); // is char* to "hello wor" (as there is no more space in the string)

Use the sanity_check to check whether functions really do not overwrite any bits prior or after the string, by checking its check_padding() function.



