#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <iterator>
#include "src/utils/utils.h"

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

namespace{
// dont forget to free
char* copy_str(const std::string& str)
{
    char* p = new char[str.size() + 1];
    strncpy(p, str.c_str(), str.size());
    p[str.size()] = 0;
    return p;
}
}

TEST_CASE("vector emplace_back", "[stl][c++11]") {
	/*
	The following code uses emplace_back to append an object of type President to a std::vector. 
	It demonstrates how emplace_back forwards parameters to the President constructor and shows 
	how using emplace_back avoids the extra copy or move operation required when using push_back.
	*/
	struct PresidentS
	{
		std::string name;
		std::string country;
		int year;
    };
    class President: public PresidentS{
    public:
		President(std::string p_name, std::string p_country, int p_year)
		{
            name = std::move(p_name);
            country = std::move(p_country);
            year = p_year;
			LOG << name<<" is being constructed.\n";
		}
		President(President&& other)
		{
			LOG << name <<" is being moved.\n";
            name = std::move(other.name);
            country = std::move(other.country);
            year = other.year;
		}
		~President() {
			LOG << "President " << name << " is gone" << endl;
		}
		President& operator=(const President& other) = default;
	};

	SECTION("emplace"){
	    std::vector<President> elections;
        elections.reserve(2); //if enough size is not reserved, move might happen
	    elections.emplace_back("Nelson Mandela", "South Africa", 1994);
	    elections.emplace_back("Donald Trump", "the USA", 2016);
    	LOG << "Contents:\n";
	    for (President const& president : elections) {
		    LOG << president.name << " was elected president of "
			    << president.country << " in " << president.year << ".\n";
	    }


    	LOG << "Underlying data:\n";
        int i = 1;
        for(President *p = elections.data() ; i <= elections.size(); ++i, ++p){
		    LOG << p->name << " was elected president of "
			    << p->country << " in " << p->year << ".\n";
        }
    }
    /*
	SECTION("emplace and then copy"){
        PresidentS *p = new PresidentS[2];
        {
    	    std::vector<President> elections;
            elections.reserve(2); //if enough size is not reserved, move might happen
	        elections.emplace_back("Nelson Mandela", "South Africa", 1994);
	        elections.emplace_back("Donald Trump", "the USA", 2016);

            memcpy(p, elections.data(), sizeof(President) * 2);
        }
        LOG << "elections vector has gone away\n";
        President *P = static_cast<President*>(p);
        for(int i = 0; i<2;++i, ++P){
            P->~President();
        }
    }
    */

	SECTION("push"){
	    std::vector<President> reElections;
	    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

	    for (President const& president : reElections) {
		    LOG << president.name << " was re-elected president of "
			    << president.country << " in " << president.year << ".\n";
        }
    }
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
