#include "catch.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>

using namespace std;

namespace CxxSTL{

TEST_CASE("Reverse interator", "[stl]") {
	auto v = vector<int>{110,1,-10,20};

	auto r = v;
	sort(r.begin(), r.end(), greater<int>());

	sort(v.begin(), v.end());
	vector<int> v2;

	SECTION("use make_reverse_iterator"){
		copy(make_reverse_iterator(v.end()), make_reverse_iterator(v.begin()), back_inserter(v2));
		REQUIRE(r == v2);
	}

	SECTION("use rbegin/rend"){
		copy(v.rbegin(), v.rend(), back_inserter(v2));
		REQUIRE(r == v2);
	}

}

TEST_CASE("vector emplace_back", "[stl][c++11]") {
	class President
	{
		std::string name;
		std::string country;
		int year;

		President(std::string p_name, std::string p_country, int p_year)
			: name(std::move(p_name)), country(std::move(p_country)), year(p_year)
		{
			std::cout << "I am being constructed.\n";
		}
		President(President&& other)
			: name(std::move(other.name)), country(std::move(other.country)), year(other.year)
		{
			std::cout << "I am being moved.\n";
		}
		President& operator=(const President& other) = default;
	};
}

TEST_CASE("Efficiently remove one elment from vector","[stl][trick]"){
	vector<int> v;
    for(int i=0;i<10000000;++i){
        v.push_back(i);
    }

	const int to_be_removed = 100;

    SECTION("Naive way"){
        v.erase(find(v.begin(),v.end(),to_be_removed));
		REQUIRE(find(v.begin(), v.end(), to_be_removed) == v.end());
    }

    SECTION("Smart way"){
        /**
        * if the element order does not matter,
        * swapping the element to delete with the last one and then pop back could save some element moving
        */
        auto it = find(v.begin(),v.end(),to_be_removed);
        swap(*it,v.back());
        v.pop_back();
		REQUIRE(find(v.begin(), v.end(), to_be_removed) == v.end());
    }
}

}
