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

/*
  python range like sequence generator
  https://www.justsoftwaresolutions.co.uk/cplusplus/generating_sequences.html
*/
template<typename T>
class num_range {
    T _current;
    T _last;
public:
    num_range(T start, T last) :_current(start), _last(last) {}
    num_range(T last) :_current(T()), _last(last) {}

    class iterator {
        num_range* _range;
    public:
        iterator(num_range* range) :_range(range) {}

        iterator& operator++() {
            ++(_range->_current);
            if (_range->_current >= _range->_last) _range = nullptr;
            return *this;
        }
        
        //missing * operator on iterator leads to error
        //Indirection operator ( * ) is applied to a nonpointer value.
        T operator*() const {
            //todo:check range
            return _range->_current;
        }

        //otherwise you will get compile error like:
        //binary '!=': no operator found which takes a left-hand operand of type 'num_range<int>::iterator'
        friend bool operator ==(const iterator& lhs, const iterator& rhs) { return lhs._range == rhs._range; }
        friend bool operator !=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
       
    };
    
    iterator begin() { return iterator(this); }
    iterator end() { return iterator(nullptr); }
    
};

template<typename T>
num_range<T> range(T from, T to) {
    return num_range<T>(from, to);
}


template<typename T>
num_range<T> range(T to) {
    return num_range<T>(T(), to);
}

TEST_CASE("python range like seq generator", "[utils]") {
    SECTION("int range") {
        int k = 0;
        for (int i : range(10)) {
            REQUIRE(i == k++);
        }
    }
}


}