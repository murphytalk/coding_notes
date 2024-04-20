#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

namespace Cxx{

TEST_CASE("Get private data member", "[trick]") {
    /* This is dirty but demonstrates C++ data model.
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

    const int v = 1234;
    Data d(v);
    Data2 *p = reinterpret_cast<Data2*>(&d);
    REQUIRE(p->get_private_data() == v);
}


}
