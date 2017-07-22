#include "catch.hpp"
#include <vector>

using namespace std;

namespace DP{

static void fibonacci_recursive(size_t n, vector<int>& result) {
    //if (result.size() < n) result.resize(n);
    if(n<=2){
        result[0] = 0;
        result[1] = 1;
    }
    else {
        fibonacci_recursive(n - 1, result);
        result[n-1]= result[n-2-1]+result[n-1-1] ;
    }
}

static void fibonacci_dp_bottom_up(size_t n, vector<int>& result) {
    result[0] = 0;
    result[1] = 1;
    for (auto i = 2; i < n; ++i) {
        result[i] = result[i - 1] + result[i - 2];
    }
}

TEST_CASE("fibocacci", "[dp]") {
    vector<int> r;
    const vector<int> expected = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144 };
    const auto N = expected.size();
    r.resize(N);
    SECTION("resursive"){
        fibonacci_recursive(N, r);
        REQUIRE(r == expected);
    }
    SECTION("dp bottom up"){
        fibonacci_dp_bottom_up(N, r);
        REQUIRE(r == expected);
    }
}

TEST_CASE("fibonacci : long", "[dp]") {
    static vector<int> r1;
    static vector<int> r2;
    const size_t N = 10000; 
    r1.resize(N);
    r2.resize(N);
    SECTION("recursive") { //stack overflow if N is too big
        fibonacci_recursive(N, r1);
    }
    SECTION("dp bottom up") {
        fibonacci_dp_bottom_up(N, r2);
    }
    SECTION("compare") {
        REQUIRE(r1 == r2);
    }
}


}; //namespace DP
