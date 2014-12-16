/*  The MIT License (MIT)
 * Copyright (c) 2014 FMBroers
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/*! \mainpage fixed_string library
 *
 * \section intro_sec Introduction
 *
 * string library optimized to only use stack. Length is known at compile-time and cannot be changed. Ever.
 *
 * The development of this library is initiated to increase the usage of C++ on memory constrained devices
 *  such as small embedded systems. By creating a heap-free library, programmers don't need to worry about
 *  heap deallocation (which could introduce defragmentation of the device's memory). This allows for more
 *  predictive realtime behaviour of any program. The downside is that every string has a maximum length,
 *  which cannot be changed at any time.
 *
 * \section usage_sec Usage
 *
 * \subsection step1 Step 1:
 * To use this library, simply #include "fixed_string.hpp" (or <fixed_string> when you're using this
 * library within your PATH)
 *
 * \subsection step2 Step 2:
 * To create a fixed_string, with a predefined length, use the example below to create a fixed_string with
 * 5 character positions. Keep in mind that within the string, a null-terminator ('\0') must be included,
 * so when you are creating a string with 5 characters as in fixed_string<5>, then you can put five in the
 * string, the allocated length will be equivalent to six characters.
 *
 * This behaviour lines up with the c-functions strlen() and sizeof(): \ref http://en.cppreference.com/w/c/string/byte/strlen
 * \code
 * fixed_string<5> MyNewString("hello");
 * strlen("hello"); // = 5
 * sizeof("hello"); // = 6
 *
 * MyNewString.c_str(); // = "hello"
 * MyNewString.get_allocated_length(); // = 6
 * MyNewString.get_used_length(); // = 5
 * \endcode
 *
 * \subsection constructors
 * The fixed_string has several constructors
 *
 * \code
 * // emtpy constructor
 * fixed_string<N> MyNewString();
 * \endcode
 * \code
 * // character constructor
 * fixed_string<N> MyNewString('c');
 * \endcode
 * \code
 * // std::string constructor
 * fixed_string<N> MyNewString("hello world");
 * \endcode
 * \code
 * // copy constructor
 * fixed_string<N> MyNewString("copythis");
 * fixed_string<N> MyNewString2(MyNewString);
 * \endcode
 *
 * Be aware that each and every constructor needs an integer N which defines the length of the fixed_string. That is not the length of the string stored in the string, but the length which is reserved for the string. If the string copied into the fixed_string is longer than the reserved space for the fixed_string, the fixed_string will only store the (N-1) first characters. The library will throw away the remaining characters.
 *
 * \subsection operators
 *
 * The fixed_string supports two operators:
 *
 * \code
 * operator=
 * operator+=
 * \endcode
 *
 * Both operators will always check whether it has enough space for the assigned string, or it will only attach the first (N-1) characters.
 *
 * \subsection comparisons
 *
 * As with c-style strings, the library provides support for various comparison operators:
 * \code
 * operator==
 * operator!=
 * operator<
 * operator<=
 * operator>
 * operator>=
 * \endcode
 *
 * these operators have boolean as return values.
 *
 * \subsection todo
 * The following is tested:
 * \li
 * The following is not tested:
 * \li compile with other C++ compilers
 *
 *
 */
