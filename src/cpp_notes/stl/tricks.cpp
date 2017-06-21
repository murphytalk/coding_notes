#include "catch.hpp"
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

namespace CxxSTL{
    TEST_CASE("Efficiently remove one elment from vector","[stl][trick]"){
        vector<int> v1,v2;
        for(int i=0;i<10000;++i){
            v1.push_back(i);
            v2.push_back(i);
        }

        SECTION("Naive way"){
            v1.erase(find(v1.begin(),v1.end(),1000));
        }
        SECTION("Smart way"){
            /**
            * if the element order does not matter,
            * swapping the element to delete with the last one and then pop back could save some element moving
            */
            auto it = find(v1.begin(),v1.end(),1000);
            swap(*it,v2.back());
            v2.pop_back();
        }
    }

	/* This is dirty but demostrates C++ data model.	
	 * Question : given a Data type object instance, how to get its _private_data ?
	*/
	class Data {
	private:
		int _private_data;
	public:
		int public_data;
		std::string str;
		Data(int data) {
			_private_data = data;
		}
	};
	// The solution : construct a class with same data members in same order, but can be visit via public member function
	class Data2 {
	private:
		int _private_data;
	public:
		int public_data;
		std::string str;
		int get_private_data() { return _private_data; }
	};
	
	TEST_CASE("Get private data member", "[trick]") {
		const int v = 1234;
		Data d(v);
		Data2 *p = reinterpret_cast<Data2*>(&d);
		REQUIRE(p->get_private_data() == v);
	}


}
