#include "catch.hpp"
#include <map>
#include <string>
#include <iostream>

namespace CxxSTL{

/*README START
CppCon 2017 [Curiously Recurring C++ Bugs at Facebook](https://www.youtube.com/watch?v=3MB2iiCkGxg&t=184s)

<<bug #1 - Implicitly insert new element>>
README END */
TEST_CASE("map [] implicitly insert new element", "[stl][fb-bug]") {
	std::map<char, int> occ;
	std::string str("abcdedfg");
	for (char c : str) {
        // if c does not exist in map
        // int() gets called and the result (0) is inserted into map
		occ[c]++;
	}
	for (char c : str) {
		REQUIRE(occ[c] == (c=='d'?2:1));
    }
}

namespace{
    int use_settings(const std::map<std::string,int>& settings){
#if 0
        // won't compile:
        // [] is not const because
        // settings could get a new element inserted if "valid_key" does not exist
        // hense break the constness
        //
        // the "solution" to drop const from parameter could be very harmful
        return settings["valid_key"];
#else
        // could throw excepion if key does not exist
        // the other solution is to use find() and handles missing key
        return settings.at("valid_key");
#endif
    }
}

// bug #2
TEST_CASE("map const correctness", "[stl][fb-bug]") {
    std::map<std::string, int> global_settings;
    global_settings["valid_key"] = 1234;
    REQUIRE(use_settings(global_settings) == 1234);
}

//folly::get_default
namespace{
    template <class Map, typename Key>
        typename Map::mapped_type get_default(
            const Map& map,
            const Key& key,
            const typename Map::mapped_type& dflt
            ){
        auto pos = map.find(key);
        return (pos != map.end() ? pos->second : dflt);
    }
}

TEST_CASE("temp obj returned as reference", "[stl][fb-bug]"){
    std::map<std::string, std::string> stuff;
    stuff["valid"] = "abcd";

    std::string dft = "nothing";
    REQUIRE(get_default(stuff, std::string("valid"), dft) == "abcd");
    REQUIRE(get_default(stuff, std::string("crazy"), dft) == dft);
#if 0
    // bad code : reference to temp string "123" will be returned and becomes invalid in the next line
    // actually clang won't compile it saying "cannot bind to a temporary of type"
    // because -fsanitize-address-use-after-scope is turned on by default
    auto& s = get_default(stuff, std::string("crazy"), "123");
    REQUIRE( s == "123");
#endif
}

// bug #3
// do not use volite to synchronize thread, use atom or lock

// bug #4
// std::shared_ptr<T> is not always thread-safe:
// - its internal reference counter is
// - but T depends on user's implementation

}