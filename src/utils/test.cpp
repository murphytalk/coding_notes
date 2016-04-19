#include "catch.hpp"
#include "utils.h"

using namespace Utils;
TEST_CASE("bitset: even numbers under 101 are set", "[utils]") {
	const int bitsize = 101;
	bitset s(bitsize);

	for (uint32_t i = 0;i < bitsize; i += 2) {
		s.set(i);
	}

	SECTION("even numbers are set") {
		for (uint32_t i = 0;i <= bitsize; i += 2) {
			REQUIRE(s.check(i));
		}
	}

	SECTION("check odd numbers are not set") {
		for (uint32_t i = 1;i <= bitsize; i += 2) {
			REQUIRE(!s.check(i));
		}
	}

	SECTION("numbers out of range are not set") {
		REQUIRE(!s.check(-1));
		REQUIRE(!s.check(1000));
	}
}

TEST_CASE("bitset: string", "[utils]") {
	//                       5    0
	const std::string s1 = "1111010";
	bitset t(s1);

	SECTION("check bits") {
		REQUIRE(!t.check(0));
		REQUIRE( t.check(1));
		REQUIRE(!t.check(2));
		REQUIRE( t.check(3));
		REQUIRE( t.check(4));
		REQUIRE( t.check(5));
		REQUIRE( t.check(6));

		REQUIRE(!t.check(7));
		REQUIRE(!t.check(8));
	}

	SECTION("restore to string"){
		std::string s2;
		t.to_str(s2);
		REQUIRE(s1 == s2);
	}
}