#include "catch.hpp"
#include <bitset>
#include <vector>

using namespace std;

namespace Algo{

/*
Find number of duplicates.
e.g. for input {1,1,1,2,3,4,5,5,6} the result should be 2 : 1 and 5 has duplicates
The integer value range is -1000 ~ 1000
*/
static const int find_duplicates_min = -1000;
static const int find_duplicates_max =  1000;

static int find_duplicates(const vector<int>& nums) {
	const int N = find_duplicates_max - find_duplicates_min + 1;
	//one bitset to check if a given num has already been spot
	bitset<N> seen;
	//another bitset to check if a given num has already been confirmed to have duplicates, 
    //so we won't count it more than once
	bitset<N> duplicates;

	int count = 0;
	for (int n : nums) {
		int bit = n - find_duplicates_min; //to avoid negative bit index
		if (!duplicates[bit]) {
			if (seen[bit]) {
				++count;
				duplicates[bit] = true;
			}
			else seen[bit] = true;
		}
	}

	return count;
}

TEST_CASE("find duplicates: 1,1,1,2,3,4,5,5,6,5,7", "[algo]") {
	const vector<int> nums = { 1,1,1,2,3,4,5,5,6,5,7 };
	REQUIRE(find_duplicates(nums) == 2);
}

TEST_CASE("find duplicates: from min to max with duplicates", "[algo]") {
	vector<int> nums = { -1000,-1000,1000,1000 };
	REQUIRE(find_duplicates(nums) == 2);
}

TEST_CASE("find duplicates: from min to max without duplicates", "[algo]") {
	vector<int> nums = { -1000,0,100,200,1000 };
	REQUIRE(find_duplicates(nums) == 0);
}


}