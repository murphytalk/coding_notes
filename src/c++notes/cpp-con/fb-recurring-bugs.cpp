#include "catch.hpp"
#include <map>
#include <string>

// cppcon | 2017 Curiously Recurring C++ Bugs at Facebook
// https://www.youtube.com/watch?v=3MB2iiCkGxg&t=184s
TEST_CASE("map [] implicityly insert new element", "[stl]") {
	std::map<char, int> occ;
	std::string str("abcdedfg");
	for (char c : str) {
		occ[c]++;
	}
	for (char c : str) {
		REQUIRE(occ[c] == 1);
	}
}
