#include "catch.hpp"
#include <functional>

using namespace std;

namespace Cxx11Test {

static void increment(int& v) { ++v; }
TEST_CASE("std::ref","[c++11]") {
	int i = 0;
	//bind increment to a copy of i and invoke the func
	bind(increment, i)();
	//because it is a copy, i's value is not changed
	REQUIRE(i == 0);

	//now bind increment to ref(i)
	bind(increment, ref(i))();
	//now a ref is stored (only possible because of std::reference_wrapper)
	REQUIRE(i == 1);
}
#if 0
class A {
public:
	int a;
};
class B1 : virtual public A {};
class B2 : virtual public A {};
class C : public B1, public B2 {};

TEST_CASE("inheritance") {
	B1 b1;
	B2 b2;
	C c;
	b1.a = 1;
	b2.a = 1;
	c.a = 1;
}
#endif
}
