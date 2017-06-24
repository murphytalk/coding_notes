#include "catch.hpp"
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

namespace CxxSTL {

/*
The first line of the input contains ,where is the number of integers.The next line contains integers separated by a space.
e.g. this input

 4
 1 4 3 2

should generate this output

 2 3 4 1
*/
TEST_CASE("stream iterator demo", "[stl]") {
	istringstream inputs("4\n1 4 3 2\n");
	int N, x;
	inputs >> N;
	vector<int> v;
	v.reserve(N);
	while (inputs >> x) {
		v.push_back(x);
	}
#if 0
	copy(v.rbegin(), v.rend(), ostream_iterator<int>(cout, " "));
	cout << endl;
#else
	ostringstream outputs;
	copy(v.rbegin(), v.rend(), ostream_iterator<int>(outputs, " "));
	REQUIRE(outputs.str() == "2 3 4 1 ");
#endif
}


}