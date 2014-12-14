

#include <gtest/gtest.h>
#include "fixed_string.hpp"
#include <chrono>

#include <iostream>


TEST(fixed_string, Constructors) {
	// ctor char ptr / array
	const fixed_string<5> s("hoi");
	EXPECT_STREQ("hoi", s.c_str());
	EXPECT_EQ(5u, s.get_allocated_length());
	EXPECT_EQ(3u, s.get_used_length());


	// ctor char
	const fixed_string<5> s1('1');
	EXPECT_STREQ("1", s1.c_str());
	EXPECT_EQ(5u, s1.get_allocated_length());
	EXPECT_EQ(1u, s1.get_used_length());

	// ctor same <N>
	const fixed_string<5> s2(s);
	EXPECT_STREQ("hoi", s2.c_str());
	EXPECT_STREQ(s.c_str(), s2.c_str());
	EXPECT_EQ(5u, s2.get_allocated_length());
	EXPECT_EQ(3u, s2.get_used_length());

	// ctor larger <N>
    const fixed_string<10> s3(s);
	EXPECT_STREQ("hoi", s3.c_str());
	EXPECT_STREQ(s.c_str(), s3.c_str());
	EXPECT_EQ(10u, s3.get_allocated_length());
	EXPECT_EQ(3u, s3.get_used_length());

	// ctor smaller <N>
	// @TODO: length = 1 --> only space for '\0' !!
    const fixed_string<1> s4(s);
	EXPECT_STREQ("", s4.c_str());
	EXPECT_EQ(1u, s4.get_allocated_length());
	EXPECT_EQ(0u, s4.get_used_length());

	// empty ctor
	const fixed_string<5> s5;
	EXPECT_STREQ("", s5.c_str());
	EXPECT_EQ(5u, s5.get_allocated_length());
	EXPECT_EQ(0u, s5.get_used_length());


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

TEST(fixed_string, Operators) {

	fixed_string<5> fs('1');
	fs += "testing";
	EXPECT_LT("1testing", fs.c_str());
	EXPECT_STREQ("1tes", fs.c_str());
	EXPECT_EQ(4u, fs.get_used_length());

	fs = 'n';
	EXPECT_STREQ("n", fs.c_str());
	EXPECT_EQ(1u, fs.get_used_length());

	fs = "1";
	fs += '1';
	EXPECT_STREQ("11", fs.c_str());
	EXPECT_EQ(2u, fs.get_used_length());
	/*
	//fixed_string<6> fs("abcd");
	fixed_string<6> fs2("efgh");
	fixed_string<6> fs3("");


	// @TODO fs = fs + fs2 ... (check if self)
	//fs3 = fs + fs2;
	//fs + fs2;
	//EXPECT_STREQ("abcde", fs3.c_str());
	//EXPECT_EQ(std::strlen("abcde"), fs3.get_used_length());
*/
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


TEST(fixed_string, comparisons) {
	fixed_string<3> fs1('a');
	fixed_string<3> fs11('a');
	fixed_string<3> fs2("a");
	fixed_string<3> fs3('A');
	fixed_string<3> fs4("ab");
	fixed_string<3> fs5("Ab");
	fixed_string<3> fs6("ac");
	fixed_string<3> fs7("AC");
	fixed_string<3> fs8('\0');


	// equal operations

	// is self equal to self
	ASSERT_EQ(fs1 == fs1, 0);
	// self equal to same string
	ASSERT_EQ(0,fs1 == fs11);

	// 'a' == "a" ? <-- is true due to nature of fixed_string
	ASSERT_EQ(0,fs1 == fs2);
	// 'a' != 'A'
	ASSERT_NE(0,fs1 == fs3);
	// 'a' != "ab"
	ASSERT_NE(0,fs1 == fs4);
	//
	//ASSERT_LT(fs1 < fs4, 0);

	ASSERT_EQ(0,fs2 == fs1);
	ASSERT_NE(0,fs3 == fs1);
	ASSERT_NE(0,fs4 == fs1);

	EXPECT_NE(fs1 == fs8, 0);
	EXPECT_NE(fs8 == fs1, 0);

	ASSERT_NE(0,fs1 != fs1);
	ASSERT_NE(0,fs1 != fs11);

	ASSERT_NE(0,fs1 != fs2);
	ASSERT_EQ(0,fs1 != fs3);
	ASSERT_EQ(0,fs1 != fs4);

	ASSERT_NE(0,fs2 != fs1);
	ASSERT_EQ(0,fs3 != fs1);
	ASSERT_EQ(0,fs4 != fs1);

	ASSERT_EQ(0, fs1 < fs1);
	// 'a' < 'A'
	ASSERT_EQ(1, fs1 < fs3);
	ASSERT_EQ(-1, fs3 < fs1);

/*	ASSERT_EQ(1, 'a' == fs1);
	ASSERT_EQ(1, fs1 == 'a');

	ASSERT_EQ(1, fs4 == "ab");
	ASSERT_EQ(1, "ab" == fs4);*/
}

TEST(fixed_string, sanitycheck) {
	fixed_string_with_guard sc('e');
	sc = 'f';
	ASSERT_TRUE(sc.check_padding());
	ASSERT_STREQ("f", sc.c_str());
	sc += "123456789123456789123456789123456789123456789";
	ASSERT_TRUE(sc.check_padding());
	ASSERT_STREQ("f12345678912345", sc.c_str());
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

