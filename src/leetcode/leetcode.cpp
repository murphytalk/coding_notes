#include "catch.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <algorithm>

using namespace std;

namespace LeetCode{
    
/*
  https://leetcode.com/problems/search-for-a-range/

  Given a sorted array of integers, find the starting and ending position of a given target value.

  Your algorithm's runtime complexity must be in the order of O(log n).

  If the target is not found in the array, return [-1, -1].

  For example,
  Given [5, 7, 7, 8, 8, 10] and target value 8, return [3, 4].
*/
class SearchRange {
public:
	vector<int> searchRange(vector<int>& nums, int target) {
		vector<int> range;
		auto low = lower_bound(nums.begin(), nums.end(), target);
		if (low == nums.end() || *low != target) {
			range.push_back(-1);
			range.push_back(-1);
		}
		else {
			range.push_back((int)(low - nums.begin()));
			auto up = upper_bound(low, nums.end(), target);
			range.push_back((int)(up - nums.begin() - 1));
		}
		return range;
	}
};

TEST_CASE("Search Range: no duplication test case", "[leetcode]") {
	vector<int> nums = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	vector<int> r;
	SearchRange s;

	SECTION("too small") {
		r = s.searchRange(nums, -1);
		REQUIRE(r[0] == -1);
		REQUIRE(r[1] == -1);
	}

	SECTION("too big") {
		r = s.searchRange(nums, 100);
		REQUIRE(r[0] == -1);
		REQUIRE(r[1] == -1);
	}

	SECTION("smallest") {
		r = s.searchRange(nums, 0);
		REQUIRE(r[0] == 0);
		REQUIRE(r[1] == 0);
	}

	SECTION("biggest") {
		r = s.searchRange(nums, 11);
		REQUIRE(r[0] == 11);
		REQUIRE(r[1] == 11);
	}

}

TEST_CASE("Search Range: duplication test case", "[leetcode]") {
	vector<int> r;
	SearchRange s;

	SECTION("all same as target") {
		vector<int> nums = { 10,10,10,10,10,10 };
		r = s.searchRange(nums, 10);
		REQUIRE(r[0] ==  0);
		REQUIRE(r[1] ==  5);
	}

	SECTION("partial duplication") {
		vector<int> nums = { 5, 7, 7, 8, 8, 10 };
		r = s.searchRange(nums, 8);
		REQUIRE(r[0] == 3);
		REQUIRE(r[1] == 4);
	}
}


}; //namespace LeetCode

