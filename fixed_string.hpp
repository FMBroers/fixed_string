/*  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#undef CANUSESTDEXCEPTIONS
#define CANUSELOCALEXCEPTIONS

#define OPTIMIZEFORSPEED

#ifdef CANUSESTDEXCEPTIONS
#undef CANUSELOCALEXCEPTIONS
#endif

#ifdef CANUSELOCALEXCEPTIONS
#undef CANUSESTDEXCEPTIONS
#endif

#ifndef _FIXED_STRING_H_
#define _FIXED_STRING_H_

#include <iostream>
#include <cstring>

// forward declaration
template<int N> class fixed_string;

// class iter
// @TODO this is a const-ness class ...

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
	class iter {
	public:
		iter(const char *c) :
				start(c), last(c + std::strlen(c)) {
		}
		iter(const char c) :
				start(&c), last(&c + 1) {
		}
		iter(fixed_string<0> f) {
			start = f.c_str();
			last = f.c_str() + std::strlen(f.c_str());
		}

		//char * begin() { return start; }
		//char * end() { return last; }
		const char * begin() const {
			return start;
		}
		const char * end() const {
			return last;
		}

	private:
		const char *start, *last;
	};

	// The inherited fixed_strings will always call this constructor
	fixed_string(char * content, int l) :
			allocated_length(l), pBuff(content) {
		// always assign '\0' to first char
		pBuff[0] = '\0';
#if defined(OPTIMIZEFORSPEED)
		used_length = 0;
#endif
	}

public:
	// The pBuff ends always with a '\0'
	const char * c_str() const {
		return pBuff;
	}

	const int get_allocated_length() const {
		return allocated_length;
	}

	// @TODO get actual length?
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

	// appends single character to string, if within boundaries,
	// else raise error via error_char, if defined.
	// if defined, use stl::exceptions
	// else nothing
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
			error_char = c;
		}
#endif
#if defined(CANUSESTDEXCEPTIONS)
		// @TODO not tested
		else {
			raise(std::bad_alloc);
		}
#endif
		/*std::cout << "stringlen: " << strundefinglen << std::endl;*/
	}

	template<class T>
	fixed_string & operator+=(const char ch) {
		append(ch);
		return *this;
	}

	template<class T>
	// typename decletype ( pin::begin() )
	// pins.hpp pin_in_from

	// traits uitzoeken
	// isIteratable
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

	// what to return ??
	template<typename T>
	bool operator<=(T const & rhs) const {
		return compare(rhs) <= 0;
	}
	template<typename T>
	bool operator>=(T const & rhs) const {
		return compare(rhs) >= 0;
	}

	// operator[]
	char & operator[](int n) {
		return valid(n) ? pBuff[n] : error_char;
	}
	// doesn't compile ?
	char operator[](int n) const {
		return valid(n) ? pBuff[n] : '\0';
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

	template<typename T /*, typename traits = std::char_traits<char> */>
	int compare(T const & rhs) const {
		int c = 0;
		// @TODO strlen(rhs)
		if (pBuff == 0 || rhs.c_str() == 0)
			return 0;
		for (char ch : iter(pBuff)) {
			if (ch > rhs[c]) // char in lhs > char of rhs
				return 1;
			if (ch < rhs[c]) // char in rhs > char of lhs
				return -1;
			if (rhs[c] == '\0' && ch != '\0') // rhs shorter
				return 1;
			c++;
		}
		if (rhs[c] != '\0') // lhs shorter
			return -1;
		return 0; // equal in length and all chars same
	}

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
		for (char c : rhs) //iters
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
	char contents[N];
	//! The  length of the fixed_object.
	static const int length = N;
};

class fixed_string_with_guard: public fixed_string<0> {
public:
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
		return (contents[0] == '<'
				&& contents[1] == '['
				&& contents[18] == ']'
				&& contents[19] == '>');
	}
	void init() {
		for (int i = 0; i < 20; i++)
			contents[i] = '-';
		contents[0] = '<';
		contents[1] = '[';
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

#endif
