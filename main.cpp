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

#include <gtest/gtest.h>
#include "fixed_string.hpp"
#include <chrono>

#include <iostream>

TEST(fixed_string, Constructor_char) {
	// ctor char
	const fixed_string::fixed_string<5> fixed_string_char('1');
	EXPECT_STREQ("1", fixed_string_char.c_str());
	EXPECT_EQ(6u, fixed_string_char.get_allocated_length());
	EXPECT_EQ(1u, fixed_string_char.get_used_length());
}

TEST(fixed_string, Constructor_char_array) {
	// ctor char array - same length
	static fixed_string::fixed_string<strlen("char_array")> fs_char_array_same_length("char_array");
	EXPECT_STREQ("char_array", 								fs_char_array_same_length.c_str());
	EXPECT_EQ(sizeof("char_array"),							fs_char_array_same_length.get_allocated_length());
	EXPECT_EQ(strlen("char_array"),							fs_char_array_same_length.get_used_length());

	/*
	 * @TODO not happy with random use of char* and std::string
	 *  should rewrite this portion of the test...
	 *  either stick to std::string or to char *
	 *  or calculate all expected values manually
	 */


	// ctor char array - smaller length
	static std::string str = "std::string";
	static fixed_string::fixed_string<8> fs_char_array_smaller_length(str);
	EXPECT_STREQ(str.substr( 0 , 8 ).c_str(),				fs_char_array_smaller_length.c_str());
	// Add 1 to account for '\0' because substr doesn't add the null-terminator...
	EXPECT_EQ(sizeof(str.substr( 0 , 8 ).c_str()) + 1,		fs_char_array_smaller_length.get_allocated_length());
	// correct behaviour: (either c_str() or substr() don't work as expected ??? )
	EXPECT_EQ(sizeof("std::str"),							fs_char_array_smaller_length.get_allocated_length());
	EXPECT_EQ(strlen(str.substr( 0 , 8 ).c_str()),	 		fs_char_array_smaller_length.get_used_length());

	// ctor char array - larger length
	static fixed_string::fixed_string<14> fs_char_array_larger_length(str);
	EXPECT_STREQ(str.c_str(), 								fs_char_array_larger_length.c_str());
	EXPECT_EQ(sizeof("std::string") + 3,					fs_char_array_larger_length.get_allocated_length());
	EXPECT_EQ(strlen(str.c_str()),				 			fs_char_array_larger_length.get_used_length());

}

TEST(fixed_string, Constructor_fixed_string) {
	const fixed_string::fixed_string<strlen("fixed_string")> fs_fixed_string_original("fixed_string");

	// constructor same size
	const fixed_string::fixed_string<strlen("fixed_string")> fs_fixed_string_same_length(fs_fixed_string_original);
	EXPECT_STREQ("fixed_string", 								fs_fixed_string_same_length.c_str());
	EXPECT_STREQ(fs_fixed_string_original.c_str(), 				fs_fixed_string_same_length.c_str());
	EXPECT_EQ(fs_fixed_string_original.get_allocated_length(), 	fs_fixed_string_same_length.get_allocated_length());
	EXPECT_EQ(fs_fixed_string_original.get_used_length(), 		fs_fixed_string_same_length.get_used_length());

	// ctor larger <N>
	const fixed_string::fixed_string<15> fs_fixed_string_larger_length(fs_fixed_string_original);
	EXPECT_STREQ("fixed_string", 								fs_fixed_string_larger_length.c_str());
	EXPECT_STREQ(fs_fixed_string_original.c_str(), 				fs_fixed_string_larger_length.c_str());
	EXPECT_EQ(16,											 	fs_fixed_string_larger_length.get_allocated_length());
	EXPECT_EQ(12,										 		fs_fixed_string_larger_length.get_used_length());

	// ctor smaller <N>
    const fixed_string::fixed_string<5> fs_fixed_string_smaller_length(fs_fixed_string_original);
	EXPECT_STREQ("fixed", 										fs_fixed_string_smaller_length.c_str());
	// expect greater than (val1 > val2)
	EXPECT_GT(fs_fixed_string_original.c_str(), 				fs_fixed_string_smaller_length.c_str());
	EXPECT_EQ(6,											 	fs_fixed_string_smaller_length.get_allocated_length());
	EXPECT_EQ(5, 												fs_fixed_string_smaller_length.get_used_length());
}


TEST(fixed_string, Constructor_empty) {

	// empty ctor
	const fixed_string::fixed_string<5> fs_empty_ctor;
	EXPECT_STREQ("", 								fs_empty_ctor.c_str());
	EXPECT_EQ(6, 									fs_empty_ctor.get_allocated_length());
	EXPECT_EQ(0, 									fs_empty_ctor.get_used_length());

	// negative <N>
	// Cannot build! <== negative array
	/*const fixed_string<-5> s6("negative <N>!");
	EXPECT_STREQ("", s6.c_str());
	EXPECT_EQ(1u, s6.get_allocated_length());
	EXPECT_EQ(0u, s6.get_used_length);*/

	// zero <N>
	// Cannot build - no constructor available
	/*const fixed_string<0> s6("zero <N>!");
	EXPECT_STREQ("", s6.c_str());
	EXPECT_EQ(1u, s6.get_allocated_length());
	EXPECT_EQ(0u, s6.get_used_length);*/
}

TEST(fixed_string, Assignment_operator_empty_ctor) {
	fixed_string::fixed_string<5> fs_assignment_char_length_5;
	EXPECT_STREQ("", 								fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(0, 									fs_assignment_char_length_5.get_used_length());

	fs_assignment_char_length_5 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_5.get_used_length());

	fixed_string::fixed_string<5> fs_assignment_char_length_1;
	EXPECT_STREQ("", 								fs_assignment_char_length_1.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(0, 									fs_assignment_char_length_1.get_used_length());

	fs_assignment_char_length_1 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_1.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_1.get_used_length());

	fixed_string::fixed_string<10> fs_assignment_char_array;
	EXPECT_STREQ("", 								fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(0, 									fs_assignment_char_array.get_used_length());

	fs_assignment_char_array = "helloworld";
	EXPECT_STREQ("helloworld", 						fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(10, 									fs_assignment_char_array.get_used_length());

	fixed_string::fixed_string<5> fs_assignment_char_array_boundary_overflow;
	EXPECT_STREQ("", 								fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(0, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	fs_assignment_char_array_boundary_overflow = "helloworld";
	EXPECT_STREQ("hello", 							fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	const fixed_string::fixed_string<strlen("fixed_string")> fs_fixed_string_original("fixed_string");
	fixed_string::fixed_string<12> fs_assignment_fixed_string;
	EXPECT_STREQ("", 								fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(0, 									fs_assignment_fixed_string.get_used_length());

	fs_assignment_fixed_string = fs_fixed_string_original;
	EXPECT_STREQ("fixed_string",					fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(12, 									fs_assignment_fixed_string.get_used_length());

	fixed_string::fixed_string<6> fs_assignment_fixed_string_boundary_overflow;
	EXPECT_STREQ("", 								fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(0, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());

	fs_assignment_fixed_string_boundary_overflow = fs_fixed_string_original;
	EXPECT_STREQ("fixed_",							fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(6, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());
}

TEST(fixed_string, Assignment_operator_char_ctor) {
	fixed_string::fixed_string<5> fs_assignment_char_length_5('b');
	EXPECT_STREQ("b", 								fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_5.get_used_length());

	fs_assignment_char_length_5 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_5.get_used_length());

	fixed_string::fixed_string<5> fs_assignment_char_length_1('c');
	EXPECT_STREQ("c", 								fs_assignment_char_length_1.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_1.get_used_length());

	fs_assignment_char_length_1 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_1.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_1.get_used_length());

	fixed_string::fixed_string<10> fs_assignment_char_array('d');
	EXPECT_STREQ("d", 								fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_array.get_used_length());

	fs_assignment_char_array = "helloworld";
	EXPECT_STREQ("helloworld", 						fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(10, 									fs_assignment_char_array.get_used_length());

	fixed_string::fixed_string<5> fs_assignment_char_array_boundary_overflow('e');
	EXPECT_STREQ("e", 								fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	fs_assignment_char_array_boundary_overflow = "9876598765";
	EXPECT_STREQ("98765", 							fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	const fixed_string::fixed_string<strlen("fixed_string")> fs_fixed_string_original("fixed_string");
	fixed_string::fixed_string<12> fs_assignment_fixed_string('f');
	EXPECT_STREQ("f", 								fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_fixed_string.get_used_length());

	fs_assignment_fixed_string = fs_fixed_string_original;
	EXPECT_STREQ("fixed_string",					fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(12, 									fs_assignment_fixed_string.get_used_length());

	fixed_string::fixed_string<6> fs_assignment_fixed_string_boundary_overflow('g');
	EXPECT_STREQ("g", 								fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());

	fs_assignment_fixed_string_boundary_overflow = fs_fixed_string_original;
	EXPECT_STREQ("fixed_",							fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(6, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());
}

TEST(fixed_string, Assignment_operator_char_array_ctor) {
	fixed_string::fixed_string<5> fs_assignment_char_length_5("hello");
	EXPECT_STREQ("hello",							fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_length_5.get_used_length());

	fs_assignment_char_length_5 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_5.get_used_length());

	fixed_string::fixed_string<5> fs_assignment_char_length_1("hello");
	EXPECT_STREQ("hello",							fs_assignment_char_length_1.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_length_1.get_used_length());

	fs_assignment_char_length_1 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_1.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_1.get_used_length());

	fixed_string::fixed_string<10> fs_assignment_char_array("10char");
	EXPECT_STREQ("10char", 							fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(6, 									fs_assignment_char_array.get_used_length());

	fs_assignment_char_array = "helloworld";
	EXPECT_STREQ("helloworld", 						fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(10, 									fs_assignment_char_array.get_used_length());

	fixed_string::fixed_string<5> fs_assignment_char_array_boundary_overflow("5char");
	EXPECT_STREQ("5char", 							fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	fs_assignment_char_array_boundary_overflow = "helloworld";
	EXPECT_STREQ("hello", 							fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	const fixed_string::fixed_string<strlen("fixed_string")> fs_fixed_string_original("fixed_string");
	fixed_string::fixed_string<12> fs_assignment_fixed_string("helloworld!!");
	EXPECT_STREQ("helloworld!!", 					fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(12, 									fs_assignment_fixed_string.get_used_length());

	fs_assignment_fixed_string = fs_fixed_string_original;
	EXPECT_STREQ("fixed_string",					fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(12, 									fs_assignment_fixed_string.get_used_length());

	fixed_string::fixed_string<6> fs_assignment_fixed_string_boundary_overflow("hello!");
	EXPECT_STREQ("hello!", 							fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(6, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());

	fs_assignment_fixed_string_boundary_overflow = fs_fixed_string_original;
	EXPECT_STREQ("fixed_",							fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(6, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());
}

// this TEST fails ... (Will be deleted when fixed)
TEST(fixed_string, Assignment_operator_array_ctor) {
	const static fixed_string::fixed_string<10> fs_original("1234567890");
	EXPECT_STREQ("1234567890", 						fs_original.c_str());
	fixed_string::fixed_string<10> fs_assignment_char_array(fs_original);
	EXPECT_STREQ("1234567890", 						fs_original.c_str());
	EXPECT_STREQ("1234567890", 						fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(10, 									fs_assignment_char_array.get_used_length());

	EXPECT_STREQ("1234567890", 						fs_original.c_str());
	fs_assignment_char_array = "somerandom";
	EXPECT_STREQ("1234567890", 						fs_original.c_str());
	EXPECT_STREQ("somerandom", 						fs_assignment_char_array.c_str());
}

TEST(fixed_string, Assignment_operator_fixed_string_ctor) {
	const static fixed_string::fixed_string<10> fs_original("1234567890");
	fixed_string::fixed_string<5> fs_assignment_char_length_5(fs_original);
	EXPECT_STREQ("12345", 							fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_length_5.get_used_length());

	fs_assignment_char_length_5 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_5.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_length_5.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_5.get_used_length());

	fixed_string::fixed_string<1> fs_assignment_char_length_1(fs_original);
	EXPECT_STREQ("1", 								fs_assignment_char_length_1.c_str());
	EXPECT_EQ(2, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_1.get_used_length());

	fs_assignment_char_length_1 = 'a';
	EXPECT_STREQ("a", 								fs_assignment_char_length_1.c_str());
	EXPECT_EQ(2, 									fs_assignment_char_length_1.get_allocated_length());
	EXPECT_EQ(1, 									fs_assignment_char_length_1.get_used_length());

	// using new original string, because fs_original has been changed
	// implemented TEST Assignment_operator_array_ctor to TEST and
	// fix problem
	const static fixed_string::fixed_string<10> fs_original2("1234567890");
	EXPECT_STREQ("1234567890", 						fs_original2.c_str());
	fixed_string::fixed_string<10> fs_assignment_char_array(fs_original2);
	EXPECT_STREQ("1234567890", 						fs_original2.c_str());
	EXPECT_STREQ("1234567890", 						fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(10, 									fs_assignment_char_array.get_used_length());

	EXPECT_STREQ("1234567890", 						fs_original2.c_str());
	fs_assignment_char_array = "somerandom";
	EXPECT_STREQ("1234567890", 						fs_original2.c_str());
	EXPECT_STREQ("somerandom", 						fs_assignment_char_array.c_str());
	EXPECT_EQ(11, 									fs_assignment_char_array.get_allocated_length());
	EXPECT_EQ(10, 									fs_assignment_char_array.get_used_length());

	fixed_string::fixed_string<5> fs_assignment_char_array_boundary_overflow(fs_original2);
	EXPECT_STREQ("12345",							fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	fs_assignment_char_array_boundary_overflow = "helloworld";
	EXPECT_STREQ("hello", 							fs_assignment_char_array_boundary_overflow.c_str());
	EXPECT_EQ(6, 									fs_assignment_char_array_boundary_overflow.get_allocated_length());
	EXPECT_EQ(5, 									fs_assignment_char_array_boundary_overflow.get_used_length());

	const fixed_string::fixed_string<strlen("fixed_string")> fs_fixed_string_original("fixed_string");
	fixed_string::fixed_string<12> fs_assignment_fixed_string(fs_original);
	EXPECT_STREQ("1234567890",						fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(10, 									fs_assignment_fixed_string.get_used_length());

	fs_assignment_fixed_string = fs_fixed_string_original;
	EXPECT_STREQ("fixed_string",					fs_assignment_fixed_string.c_str());
	EXPECT_EQ(13, 									fs_assignment_fixed_string.get_allocated_length());
	EXPECT_EQ(12, 									fs_assignment_fixed_string.get_used_length());

	fixed_string::fixed_string<6> fs_assignment_fixed_string_boundary_overflow(fs_original);
	EXPECT_STREQ("123456",							fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(6, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());

	fs_assignment_fixed_string_boundary_overflow = fs_fixed_string_original;
	EXPECT_STREQ("fixed_",							fs_assignment_fixed_string_boundary_overflow.c_str());
	EXPECT_EQ(7, 									fs_assignment_fixed_string_boundary_overflow.get_allocated_length());
	EXPECT_EQ(6, 									fs_assignment_fixed_string_boundary_overflow.get_used_length());
}

TEST(fixed_string, time) {
/*
	volatile int i = 0;
	uint32_t iterations = 100000;


	auto begin = std::chrono::high_resolution_clock::now();

	fixed_string<10> fs;
	for(uint32_t j = 0; j < iterations; ++j)
	{
	    // code to benchmark
		// double assignment
		fs = "";
		// unroll for-loop
		  for(i = 0; i < 10; i++) {
			  fs += 'a';
		  }
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
	std::cout << "fixed_string: " << '\t' << duration << "ns total, average : \t" << duration / iterations << "ns." << std::endl;


	begin = std::chrono::high_resolution_clock::now();
	std::string s;
	for(uint32_t j = 0; j < iterations; ++j)
	{
	    // code to benchmark
		  s = "";
		  for(i = 0; i < 10; i++) {
			  s += 'a';
		  }
	}
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
	std::cout << "std::string: " << '\t' << duration << "ns total, average : \t" << duration / iterations << "ns." << std::endl;
*/


}

//! This TEST tests the following:
//! - <=
//! - <
//! - ==
//! - !=
//! - >=
//! - >
TEST(fixed_string, comparisons) {
	// test 1: <=
	// 'a' <= 'b'
	const fixed_string::fixed_string<5> fs_a('a');
	const fixed_string::fixed_string<5> fs_b('b');
	EXPECT_TRUE(fs_a <= fs_b);
	EXPECT_TRUE(fs_a <= 'b');
	EXPECT_TRUE(fs_a <= "b");
	EXPECT_FALSE(fs_b <= fs_a);

	EXPECT_TRUE(fs_a <= 'a');
	EXPECT_TRUE(fs_a <= "a");

	EXPECT_FALSE(fs_a <= 'A');
	EXPECT_TRUE(fs_a <= "ab");

	// test 2: <
	EXPECT_TRUE(fs_a < fs_b);
	EXPECT_TRUE(fs_a < 'b');
	EXPECT_TRUE(fs_a < "b");
	EXPECT_FALSE(fs_b < fs_a);

	EXPECT_FALSE(fs_a < 'a');
	EXPECT_FALSE(fs_a < "a");

	EXPECT_FALSE(fs_a < 'A');
	EXPECT_TRUE(fs_a < "ab");

	// test 3: ==
	EXPECT_FALSE(fs_a == fs_b);
	EXPECT_FALSE(fs_a == 'b');
	EXPECT_FALSE(fs_a == "b");
	EXPECT_FALSE(fs_b == fs_a);

	EXPECT_TRUE(fs_a == 'a');
	EXPECT_TRUE(fs_a == "a");

	EXPECT_FALSE(fs_a == 'A');
	EXPECT_FALSE(fs_a == "ab");

	// test 4: !=
	EXPECT_TRUE(fs_a != fs_b);
	EXPECT_TRUE(fs_a != 'b');
	EXPECT_TRUE(fs_a != "b");
	EXPECT_TRUE(fs_b != fs_a);

	EXPECT_FALSE(fs_a != 'a');
	EXPECT_FALSE(fs_a != "a");

	EXPECT_TRUE(fs_a != 'A');
	EXPECT_TRUE(fs_a != "ab");
	// test 5: >=
	EXPECT_FALSE(fs_a >= fs_b);
	EXPECT_FALSE(fs_a >= 'b');
	EXPECT_FALSE(fs_a >= "b");
	EXPECT_TRUE(fs_b >= fs_a);

	EXPECT_TRUE(fs_a >= 'a');
	EXPECT_TRUE(fs_a >= "a");

	EXPECT_TRUE(fs_a >= 'A');
	EXPECT_FALSE(fs_a >= "ab");
	// test 6: >
	EXPECT_FALSE(fs_a >= fs_b);
	EXPECT_FALSE(fs_a >= 'b');
	EXPECT_FALSE(fs_a >= "b");
	EXPECT_TRUE(fs_b >= fs_a);

	EXPECT_TRUE(fs_a >= 'a');
	EXPECT_TRUE(fs_a >= "a");

	EXPECT_TRUE(fs_a >= 'A');
	EXPECT_FALSE(fs_a >= "ab");
}


TEST(fixed_string, sanitycheck) {
	fixed_string::fixed_string_with_guard sc('e');
	sc = 'f';
	ASSERT_TRUE(sc.check_padding());
	ASSERT_STREQ("f", sc.c_str());
	sc += "123456789123456789123456789123456789123456789";
	ASSERT_TRUE(sc.check_padding());
	ASSERT_STREQ("f12345678912345", sc.c_str());
}



/*
const char kHelloString[] = "Hello, world!";

// Tests the c'tor that accepts a C string.
TEST(MyString, ConstructorFromCString) {
  const MyString s(kHelloString);
  EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));
  EXPECT_EQ(sizeof(kHelloString)/sizeof(kHelloString[0]) - 1,
            s.Length());
}

// Tests the copy c'tor.
TEST(MyString, CopyConstructor) {
  const MyString s1(kHelloString);
  const MyString s2 = s1;
  EXPECT_EQ(0, strcmp(s2.c_string(), kHelloString));
}

// Tests the Set method.
TEST(MyString, Set) {
  MyString s;

  s.Set(kHelloString);
  EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));

  // Set should work when the input pointer is the same as the one
  // already in the MyString object.
  s.Set(s.c_string());
  EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));

  // Can we set the MyString to NULL?
  s.Set(NULL);
  EXPECT_STREQ(NULL, s.c_string());
}
*/


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

