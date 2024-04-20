#include <catch2/catch_test_macros.hpp>
#include <vector>
#include "src/utils/utils.h"

using namespace std;

namespace Algo{

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

/* Find the number of consecutive numbers that can add up to the given summary.
   e.g. for 15 , the answer is 3 because:
   1) 15 = 7 + 8
   2) 15 = 4 + 5 + 6
   3) 15 = 1 + 2 + 3 + 4 + 5

   e.g. for 10, ther answer is 1 : 10 = 1 + 2 + 3 + 4
*/
//#define VERBOSE_find_consecutive_num_sum
static int find_consecutive_num_sum(long long N)
{
	int count = 0;
	long long start, end;
	for (auto parts = 2; parts < N; ++parts) {
        // parts: number of the consecutive numbers
		auto half = parts / 2;
        // the numer in the middle of consecutive numbers
        // e.g. if parts == 3 then
        // middle = (int)(N/3) <= N/3
        // because the numbers are required to be consecutive
        // we extend to each side of middle for ceiling(3/2)=2
        // if add them together the result should be >=N
		auto middle = N / parts;
		if (parts % 2 == 0) {
			start = middle - (half - 1);
			end = middle + half;
		}
		else {
			start = middle - half;
			end = middle + half;
		}
		if (start <= 0) continue;
		long long sum = 0;
#ifdef VERBOSE_find_consecutive_num_sum
		stringstream ss;
#endif
		for (long long i = start; i <= end; ++i) {
			sum += i;
#ifdef VERBOSE_find_consecutive_num_sum
			ss << i <<" ";
#endif
			if (sum > N) break;
		}
		if (sum == N) {
			++count;
#ifdef VERBOSE_find_consecutive_num_sum
			LOG <<"parts="<<parts<<":"<< ss.str() << endl;
#endif
		}
	}
	return count;
}

static int find_consecutive_num_sum_dp(long long N)
{
	int count = 0;
	long long start = 1, end = 1;
	long long sum = 1;

    // a sliding window, window left is the first number of the consecutive numbers while window right is the last
    // intially start from 1 , window size is 1
	while (start <= N / 2){
        // start sould neve exceed half of N as N/2 + (N/2 + 1) > N
		if (sum < N){
            // increase the window right to the next consecutive number
			++end;
			sum += end;
		}
		else if (sum > N){
            // window right strech too far, sum exceeded N, drop the first number and shrink window to the right
			sum -= start;
			++start;
		}else if (sum == N){
			++count;
			sum -= start;
			++start;
		}
	}
	return count;
}


TEST_CASE("consecutive num sum : 15", "[algo]") {
	REQUIRE(find_consecutive_num_sum(15) == 3);
}

TEST_CASE("consecutive num sum : 10", "[algo]") {
	REQUIRE(find_consecutive_num_sum(10) == 1);
}

TEST_CASE("consecutive num sum : 100", "[algo]") {
	REQUIRE(find_consecutive_num_sum(100) == 2);
}

TEST_CASE("consecutive num sum : 125", "[algo]") {
	REQUIRE(find_consecutive_num_sum(125) == 3);
}

TEST_CASE("consecutive num sum dp : 15", "[dp]") {
	REQUIRE(find_consecutive_num_sum_dp(15) == 3);
}

TEST_CASE("consecutive num sum dp : 10", "[dp]") {
	REQUIRE(find_consecutive_num_sum_dp(10) == 1);
}

TEST_CASE("consecutive num sum dp : 100", "[dp]") {
	REQUIRE(find_consecutive_num_sum_dp(100) == 2);
}

TEST_CASE("consecutive num sum dp : 125", "[dp]") {
	REQUIRE(find_consecutive_num_sum_dp(125) == 3);
}

TEST_CASE("consecutive num sum dp : big number", "[dp][algo]") {
	//https://en.wikipedia.org/wiki/Polite_number
	//http://planetmath.org/politenumber
	const long long n = 50000;
	const long long N = n*(n + 1) / 2; //http://planetmath.org/node/31698
	static int n1, n2;
	SECTION("DP") {
		n1 = find_consecutive_num_sum_dp(N);
	}
	SECTION("non-DP") {
		n2 = find_consecutive_num_sum(N);
	}
	SECTION("compare") {
		REQUIRE(n1==n2);
		LOG << "N="<<N<< ",DP solution:" << n1 << ",non-DP solution:" << n2 << endl;
	}
}



}; //namespace DP
