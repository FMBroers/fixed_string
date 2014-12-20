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

//! \defgroup fixed_string
/**
 * \class fixed_string
 *
 * \ingroup fixed_string
 *
 * \brief string library optimized to only use stack. Length is known at compile-time and cannot be changed. Ever.
 *
 * The development of this library is initiated to increase the usage of C++ on memory constrained devices such as small
 * embedded systems. By creating a heap-free library, programmers don't need to worry about heap deallocation (which could
 * introduce defragmentation of the device's memory). This allows for more predictive realtime behaviour of any program.
 * The downside is that every string has a maximum length, which cannot be changed at any time.
 *
 * \note Attempts at zen rarely work.
 *
 * \author $Author: Martin Broers
 *
 * \version $Revision: 1.0 $
 *
 * \date $Date: 2014/12/20 20:00:00 $
 *
 * \contact Contact: broers.martin@gmail.com
 *
 * Created on: $Date
 */

#ifndef _FIXED_STRING_H_
#define _FIXED_STRING_H_

#include <iostream>
#include <cstring>

#include "defines.hpp"

namespace fixed_string {
//! forward declaration
template<int N> class fixed_string;

/// @brief implementation containing all functions
/// @details
/// Usage: none - all functions are inherited by fixed_string<N>
///
template<>
class fixed_string<0> {
private:
	const int allocated_length;
	char *pBuff;
	char error_char;

	// @TODO dynamic '\0; explain decisions

#if defined(OPTIMIZEFORSPEED)
	int used_length = 0;
#endif

protected:
	//! class iter provides for-loop integration for the types char, char* and fixed_string< 0 >.
	//! This means we can use \code for( char ch : some_char_array ) { \endcode or
	//! \code for( char ch : some_fixed_string ) { \endcode even \code for( char ch : some_char ) { \endcode
	//! everywhere where we want to use it.
	class iter {
	public:
		//! constructor for char*
		iter(const char *c) :
				start(c), last(c + std::strlen(c)) {
		}
		//! constructro for char
		iter(char ch) :
				c(ch), start(&c), last(&this->c + 1) {
		}
		//! constructor for fixed_string< 0 >
		iter(fixed_string<0> f) :
				start(f.c_str()), last(f.c_str() + std::strlen(f.c_str())) {

		}

		//! begin() returns first position of char array, char or fixed_string< 0 >
		const char * begin() const {
			return start;
		}
		//! begin() returns last position of char array, char or fixed_string< 0 >
		const char * end() const {
			return last;
		}

	private:
		//! To iterate through single char, remember in local variable
		char c;
		//! Remember start and last of char array, char or fixed_string< 0 >
		const char *start, *last;

	};

	//! The inherited fixed_strings will always call this constructor
	//! Protected constructor, never allow a fixed_string< 0 > to be
	//! initialized solely without fixed_string< N >
	fixed_string(char * content, int l) :
			allocated_length(l), pBuff(content) {
		//! always assign '\0' to first char
		pBuff[0] = '\0';
#if defined(OPTIMIZEFORSPEED)
		used_length = 0;
#endif
	}

public:
	//! Returns a pointer to the contents of a fixed_string.
	//! It does not allow any changes, hence this is a
	//! read-only function.
	const char * c_str() const {
		return pBuff;
	}

	//! Returns the allocated length of a fixed_string.
	//! Note: this does not reflect the actual length
	//! of the stored string, only maximum length
	//! (or allocated length)
	const int get_allocated_length() const {
		return allocated_length;
	}

	//! Returns the length of the actual string
	//! Note that the memory allocated by the
	//! fixed_string might be larger...
	const int get_used_length() const {
#if defined(OPTIMIZEFORSPEED)
		return used_length;
#else
		for(int i = 0; i < allocated_length; i++)
		if(pBuff[i] == '\0')
		return i;
		// @TODO ERROR
		return NULL;
#endif
	}

	//! Appends single character to string, if within boundaries,
	//! else raise error via error_char, if defined.
	//! if defined, use stl::exceptions
	//! else nothing
	void append(char c) {
#if defined(OPTIMIZEFORSPEED)
		if (valid(used_length)) {
			// stringlen points to position of the last '\0'
			pBuff[used_length++] = c;
			pBuff[used_length] = '\0';
		}
#else
		int i;
		for(i = 0; i < allocated_length; i++)
		if(pBuff[i] == '\0') {
			break;
		}
		if(valid ( i )) {
			pBuff[i] = c;
			pBuff[i+1] = '\0';
		}
#endif
#if defined(CANUSELOCALEXCEPTIONS)
		else {
			// in template class meegeven voor al dan niet afhandelen van exceptions
			/*std::cout << "CANUSELOCALEXCEPTIONS " << std::endl;*/
			// error char deprecated
			error_char = '?';
		}
#endif
#if defined(CANUSESTDEXCEPTIONS)
		// @TODO not tested
		else {
			raise(std::bad_alloc);
		}
#endif
	}

	//! operator+= appends character to this fixed_string
	//! but only if append() this allows, which means
	//! that the allocated memory is larger than the stored
	//! string
	template<class T>
	fixed_string & operator+=(const char ch) {
		append(ch);
		return *this;
	}

	template<class T>
	fixed_string & operator+=(T input) {
		for (char ch : iter(input))
			append(ch);
		return *this;
	}

	template<typename T>
	fixed_string & operator=(T const & rhs) {
		reset();
		return *this += rhs;
	}

	// Need this specialization as this class contains a const attribute.
	fixed_string & operator=(const fixed_string & rhs) {
		reset();
		return *this += rhs;
	}

	template<typename T>
	bool operator==(T const & rhs) const {
		return compare(rhs) == 0;
	}

	template<typename T>
	bool operator!=(T const & rhs) const {
		return compare(rhs) != 0;
	}

	template<typename T>
	bool operator>(T const & rhs) const {
		return compare(rhs) > 0;
	}

	template<typename T>
	bool operator<(T const & rhs) const {
		return compare(rhs) < 0;
	}

	template<typename T>
	bool operator<=(T const & rhs) const {
		return compare(rhs) <= 0;
	}

	template<typename T>
	bool operator>=(T const & rhs) const {
		return compare(rhs) >= 0;
	}

	/*template<typename T, typename T::fixed_string>
	 bool operator==(T const & rhs) const {
	 return (fixed_string<0>)compare(rhs) == 0;
	 }

	 template<typename T, typename T::fixed_string>
	 bool operator!=(T const & rhs) const {
	 return (fixed_string<0>)compare(rhs) != 0;
	 }

	 template<typename T, typename T::fixed_string>
	 bool operator>(T const & rhs) const {
	 return (fixed_string<0>)compare(rhs) > 0;
	 }

	 template<typename T, typename T::fixed_string>
	 bool operator<(T const & rhs) const {
	 return (fixed_string<0>)compare(rhs) < 0;
	 }

	 template<typename T, typename T::fixed_string>
	 bool operator<=(T const & rhs) const {
	 return (fixed_string<0>)compare(rhs) <= 0;
	 }

	 template<typename T, typename T::fixed_string>
	 bool operator>=(T const & rhs) const {
	 return (fixed_string<0>)compare(rhs) >= 0;
	 }*/

	// operator[]
	char & operator[](int n) {
		return valid(n) ? pBuff[n] : error_char;
	}
	// doesn't compile ?
	char operator[](int n) const {
		return valid(n) ? pBuff[n] : '?';
	}

	char * begin() {
		return pBuff;
	}

	char * end() {
#if defined(OPTIMIZEFORSPEED)
		return pBuff + used_length;
#else
		// find the last used char
		for(int i = 0; i < allocated_length; i++)
		if(pBuff[i] == '\0')
		return pBuff + i;
		// @TODO throw some error! - no '\0' detected!!!
		return NULL;
#endif
	}

	const char * begin() const {
		return pBuff;
	}

	const char * end() const {
#if defined(OPTIMIZEFORSPEED)
		return pBuff + used_length;
#else
		// find the last used char
		for(int i = 0; i < allocated_length; i++)
		if(pBuff[i] == '\0')
		return pBuff + i;
		// @TODO throw some error! - no '\0' detected!!!
		return NULL;
#endif
	}

private:
	bool valid(const int pos) const {
		return (pos >= 0 && pos < (allocated_length - 1));
	}

	void reset() {
#if defined(OPTIMIZEFORSPEED)
		used_length = 0;
#endif
		pBuff[0] = '\0';
	}

	// @TODO
	//! Need to implement this method...
	int compare(const int & rhs) const {
		return compare(pBuff) <= 0;
	}

	template<typename T>
	int compare(T const & rhs) const {
		int c = 0;
		if (pBuff == 0 || rhs == 0)
			return 0;
		for (char ch : iter(rhs)) {
			if (pBuff[c] > ch  ) // char in lhs > char of rhs
				return 1;
			if (pBuff[c] < ch  ) // char in rhs > char of lhs
				return -1;
			// @TODO: check this one
			if (pBuff[c] != '\0' && ch == '\0') // rhs shorter
				return 1;
			c++;
		}
		// @TODO: check this one
		if (rhs != '\0' && pBuff[c] != '\0') // lhs shorter
			return -1;
		return 0; // equal in length and all chars same
	}

	/*	int compare(const char rhs) const {
	 // @TODO strlen(rhs)
	 if (pBuff == 0)
	 return 0;

	 if (pBuff[0] > rhs) // char in lhs > char of rhs
	 return 1;
	 if (pBuff[0] < rhs) // char in rhs > char of lhs
	 return -1;
	 if (rhs == '\0' && pBuff[0] != '\0') // rhs shorter
	 return 1;

	 if (rhs != '\0') // lhs shorter
	 return -1;
	 return 0; // equal in length and all chars same
	 }*/

};

/*! \brief The fixed_string library allocates space on the stack to prevent heap allocations.
 *
 *
 *  The fixed_string library utilizes stack allocation to prevent
 *  the usage of heap when using strings in your application. The
 *  library is written to target embedded systems which have very
 *  limited RAM available.
 *
 *  The library is fixed in size, i.e. when constructed, the string
 *  cannot use more or less bytes than indicated at the construction
 *  of a fixed_string object. The string itself (the contents) can
 *  change, but the length cannot be longer than N.
 *
 */
template<int N>
class fixed_string: public fixed_string<0> {

public:

	static const bool is_fixed_string = true;

	//! Empty constructor. It calls the implementation <0> to
	//! construct the object, using the attributes contents
	//! (a char array) and the length, which is indicated with
	//! N (fixed_string<N>).
	fixed_string() :
			fixed_string<0>(contents, length) {
	}

	//! Constructor with single char. It calls the
	//! implementation <0> to construct the object,
	//! using the attributes contents (a char array)
	//! and the length, which is indicated with N
	//! (fixed_string<N>).
	fixed_string(char c) :
			fixed_string<0>(contents, length) {
		fixed_string<0>::append(c);
	}

	//! Copy constructor. It calls the
	//! implementation <0> to construct the object,
	//! using the attributes contents (a char array)
	//! and the length, which is indicated with N
	//! (fixed_string<N>).
	//! This function is unique for every length
	//! of the "other" fixed_string. The compiler
	//! will  construct these extra functions, so
	//! use with care, or machine code can be very
	//! lengthy.
	template<int M>
	fixed_string(const fixed_string<M> & rhs) :
			fixed_string<0>(contents, length) {
		for (char c : iter(rhs)) //iters
			fixed_string<0>::append(c);
	}

	//! Constructor with char pointer. It calls the
	//! implementation <0> to construct the object,
	//! using the attributes contents (a char array)
	//! and the length, which is indicated with N
	//! (fixed_string<N>).
	fixed_string(const char * ch) :
			fixed_string<0>(contents, length) {
		for (char c : iter(ch))
			fixed_string<0>::append(c);
	}

	fixed_string(const std::string & ch) :
		fixed_string<0>(contents, length) {
		for (char c : iter(ch.c_str() ))
			fixed_string<0>::append(c);
	}

	/*	operator fixed_string() const {
	 return (fixed_string<0> ) *this;
	 }
	 */

	//! Assignment operator. This function yields in
	//! unique functions for every fixed_string<M>, so
	//! be carefull using this function, as it will
	//! enlarge your machinecode for every use of the
	//! assignment.
	template<int M>
	fixed_string<0> & operator=(const fixed_string<M> & rhs) {
		return fixed_string<0>::operator=(rhs);
	}

private:
	//! Buffer for the chars stored in the object
	char contents[N + 1];
	//! The  length of the fixed_object.
	static const int length = N + 1;
};

class fixed_string_with_guard: public fixed_string<0> {
public:

	static const bool is_fixed_string = true;

	fixed_string_with_guard(char c) :
			fixed_string<0>(contents + 2, length) {
		init();
		fixed_string<0>::append(c);
	}
	fixed_string_with_guard() :
			fixed_string<0>(contents + 2, length) {
		init();
		fixed_string<0>::append('\0');
	}
	fixed_string_with_guard(char * c) :
			fixed_string<0>(contents + 2, length) {
		init();
		for (char c : iter(c))
			fixed_string<0>::append(c);
	}

	bool check_padding() {
		return (contents[0] == '<' && contents[1] == '[' && contents[18] == ']'
				&& contents[19] == '>');
	}
	void init() {
		for (int i = 0; i < 20; i++)
			contents[i] = '-';
		contents[0] = '<';
		contents[1] = '['; //
		contents[18] = ']';
		contents[19] = '>';
	}

	fixed_string<0> & operator=(const fixed_string_with_guard & rhs) {
		return fixed_string<0>::operator=(rhs);
	}

private:
	char contents[20];
	static const int length = 16;
};
} // namespace fixed_string
#endif
