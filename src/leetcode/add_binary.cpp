#include "catch2/catch.hpp"
#include <string>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "../utils/utils.h"

using namespace std;

namespace LeetCode{

/*
  https://leetcode.com/problems/add-binary/

  Given two binary strings, return their sum (also a binary string).
  For example,
    a = "11"
    b = "1"
  Return "100".

  There are two implementations below : 1) by bit 2) by byte,
  for big test data 2) is 30x times faster than 1)

 */
class AddBinary {
public:
	virtual string addBinary(string a, string b) {
		const uint32_t n1 = (uint32_t)a.size();
		const uint32_t n2 = (uint32_t)b.size();
		//we might need to carry a 1 from lower bit
		const uint32_t n = max(n1, n2) + 1;
		Utils::bitset result(n);
		auto a_pos = a.rbegin();
		auto b_pos = b.rbegin();
		char aa, bb, add, carry = 0;

		auto _get_str_bit = [](string::reverse_iterator& pos, string::reverse_iterator end, char& v) {
			if (pos == end) {
				v = (char) 0;
			}
			else {
				v = (char) (*pos - '0');
				++pos;
			}
		};

		for (size_t i = 0; i < n; ++i) {
			_get_str_bit(a_pos, a.rend(), aa);
			_get_str_bit(b_pos, b.rend(), bb);

			add = carry + aa + bb;

			switch (add) {
			case 0:
				carry = 0;
				break;
			case 1:
				carry = 0;
				result.set((uint32_t)i);
				break;
			case 2:
				carry = 1;
				break;
			case 3:
				//1 + 1 = 10
				carry = 1;
				result.set((uint32_t)i);
				break;
			}

		}
		return result.to_str();
	}
};

class AddBinary2 : public AddBinary{
public:
	static inline uint8_t step_back(char*& p, const char* start) {
		if (p < start) return 0;
		auto n = p - start;
		if (n < 8) {
			p -= n+1;
			return s2i(p+1,(int)n + 1);
		}
		else {
			p -= 8;
			return s2i(p+1);
		}
	}

	static inline uint8_t s2i(char const *p, int step = 8) {
		uint8_t value = 0;
		uint8_t c;
		for (int i = 0; i < step; ++p,++i) {
			c = *p - '0';
			if (c) value |= (c << (step - 1 - i));
		}
		return value;
	}

	virtual string addBinary(string a, string b) {
		uint32_t n1 = (uint32_t)a.size();
		uint32_t n2 = (uint32_t)b.size();
		//we might need to carry a 1 from lower bit
		const auto n = max(n1, n2) + 1;
		Utils::bitset result(n);

		const auto full_steps = n / 8;

		uint16_t add = 0, carry = 0;
		const char* pa_start = a.c_str();
		const char* pb_start = b.c_str();
		char* pa = const_cast<char*>(pa_start) + n1 - 1;
		char* pb = const_cast<char*>(pb_start) + n2 - 1;
		size_t i = 0;
		for (; i < full_steps; ++i) {
			add = carry + step_back(pa, pa_start) + step_back(pb, pb_start);
			carry = add > 0xff;
			result.set((uint32_t)i, add & 0xff);
		}

		n1 = (uint32_t)(pa - pa_start + 1);
		n2 = (uint32_t)(pb - pb_start + 1);
		add = carry + s2i(const_cast<char*>(pa_start), n1) +s2i(const_cast<char*>(pb_start), n2);
		carry = add > 0xff;
		result.set((uint32_t)i, add & 0xff);

		return result.to_str();
	}
};

TEST_CASE("Add binary: s2i", "[leetcode]") {
	SECTION("1111 => 15") {
		REQUIRE(AddBinary2::s2i("1111", 4) == 15);
	}

	SECTION("111111 => 31") {
		REQUIRE(AddBinary2::s2i("11111", 5) == 31);
	}

	SECTION("11111110 => 254") {
		REQUIRE(AddBinary2::s2i("11111110", 8) == 254);
	}

	SECTION("11111111 => 255") {
		REQUIRE(AddBinary2::s2i("11111111", 8) == 255);
	}
}

TEST_CASE("Add binary: step_back", "[leetcode]") {
	SECTION("1111") {
		char s [] = "1111";
		char *start = s;
		char * p = s + 3;
		auto v = AddBinary2::step_back(p, start);

		REQUIRE(v == 15);
		REQUIRE(start-p == 1);
	}

	SECTION("1111110") {
		char s [] = "1111110";
		char *start = s;
		char * p = s + 6;
		auto v = AddBinary2::step_back(p, start);

		REQUIRE(v == 126);
		REQUIRE(start - p == 1);
	}

	SECTION("11111110") {
		char s [] = "11111110";
		char *start = s;
		char * p = s + 7;
		auto v = AddBinary2::step_back(p, start);

		REQUIRE(v == 254);
		REQUIRE(start - p == 1);
	}

	SECTION("111111110") {
		char s [] = "111111110";
		char *start = s;
		char * p = s + 8;
		auto v = AddBinary2::step_back(p, start);

		REQUIRE(v == 254);
		REQUIRE(start - p == 0);
	}
}

TEST_CASE("Add binary: test cases 1", "[leetcode]") {
	AddBinary  add1;
	AddBinary2 add2;
	SECTION("0 + 0 = 0") {
		REQUIRE(add1.addBinary("0", "0") == "0");
		REQUIRE(add2.addBinary("0", "0") == "0");
	}

	SECTION("0 + 1 = 1") {
		REQUIRE(add1.addBinary("0", "1") == "1");
		REQUIRE(add2.addBinary("0", "1") == "1");
	}

	SECTION("1 + 1 = 10") {
		REQUIRE(add1.addBinary("1", "1") == "10");
		REQUIRE(add2.addBinary("1", "1") == "10");
	}

	SECTION("11 + 1 = 100") {
		REQUIRE(add1.addBinary("11", "1") == "100");
		REQUIRE(add2.addBinary("11", "1") == "100");
	}
}

TEST_CASE("Add binary: test cases 2", "[leetcode]") {
	AddBinary  add1;
	AddBinary2 add2;

	{
		char a [] = "1111";
		char b [] = "1111";
		char r [] = "11110";
		SECTION("(AddBinary )1111 + 1111 = 11110") {
			REQUIRE(add1.addBinary(a, b) == r);
		}

		SECTION("(AddBinary2)1111 + 1111 = 11110") {
			REQUIRE(add2.addBinary(a,b) == r);
		}
	}

	{
		char a [] = "11111110";
		char b [] = "11";
		char r [] = "100000001";
		SECTION("(AddBinary )11111110 + 11 = 100000001") {
			REQUIRE(add1.addBinary(a, b) == r);
		}
		SECTION("(AddBinary2)11111110 + 11 = 100000001") {
			REQUIRE(add2.addBinary(a, b) == r);
		}
	}

	{
		char a [] = "1111111011111110";
		char b [] = "100111";
		char r [] = "1111111100100101";
		SECTION("(AddBinary )1111111011111110 + 100111 = 1111111100100101") {
			REQUIRE(add1.addBinary(a, b) == r);
		}

		SECTION("(AddBinary2)1111111011111110 + 100111 = 1111111100100101") {
			REQUIRE(add2.addBinary(a, b) == r);
		}
	}

}

TEST_CASE("Add binary: big test cases", "[leetcode]") {
    AddBinary  add1;
    AddBinary2 add2;

    string a,b;
	string file = Utils::get_data_file_path("leetcode-add-binary.txt");
    if(!Utils::load_test_data(file.c_str(),
							  [&](string& s){ if(a.size()==0) a=s; else b=s; })){
        INFO("Cannot find test file "<<file);
		INFO("To generate it run: scripts/gen_long_bit_string.py > " << file);
        REQUIRE(false);
    }
    else{
        static string s1,s2;
        SECTION("solution 1"){ //only to find runtime
           s1=add1.addBinary(a,b);
        }
        SECTION("solution 2"){
           s2=add2.addBinary(a,b);
        }
        SECTION("solution 1 == solution 2"){
			REQUIRE(s1 == s2);
        }
     }
}


}
