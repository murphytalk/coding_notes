#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <functional>

using namespace std;

namespace Cxx11Test {

/*README START
# static assert

 `static_assert` is a new C++ keyword, the assertion happens at compile time, if the assertion is true the static assert expression has no effect.
*/

namespace{
//<<
struct MyAlignmentSensitiveStuff{
    uint8_t i;
    uint32_t d;
};

void do_stuff(){
    // if data type of i is changed to something else with a different size,
    // or more member is inserted in the front of it
    // the following won't compile
    static_assert(offsetof(MyAlignmentSensitiveStuff, d) == 4,
                  "Wrong offset of d in MyAlignmentSensitiveStuff");
}
//>>
}
/*
  `static_assert` can also be used to generate <<meaningful compile error>> if an unmatched partial template specialization is detected.
 */
namespace{
//<<
template <typename T> class MyTemplatedClass{
    // if none of the specialized version is matched T's size will be zero
    // and the following guarantees a compiler error with user defined error message
    static_assert(sizeof(T) == 0,"Not sepcialized for this data type");
};
template <typename T> class MyTemplatedClass<T*>{};

MyTemplatedClass<int*> ptr;
// uncomment the following line to get a compile error
//MyTemplatedClass<int> not_specialized;
//>>
}


/*
# constexpr specifier
`constexpr`  declares that it is possible to evaluate the value of the function or variable at compile time.

The following function is evaluabled at compile time when is called with parameter 10. 
Note `static_assert` is used to check that value is evalulated correctly at compile time.
*/
namespace{
//<<
constexpr int factorial(int n)
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}

static_assert(factorial(10) == 3628800);

//>> 
}
//README END




static void increment(int& v) { ++v; }
TEST_CASE("std::ref", "[c++11]") {
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

TEST_CASE("inheritance") {
    class A {
    public:
        int a;
        virtual void func() {}
    };
    class AA : public A {};
    class AAA : public AA {};
    class B1 : virtual public A {};
    class B2 : virtual public A {};
    class C : public B1, public B2 {};
    class BB1 : public A {};
    class BB2 : public A {};
    class CC : public BB1, public BB2 {};
    class D {};

    AA aa;
    AAA aaa;
	B1 b1;
	B2 b2;
	C c;
    D d;
	b1.a = 1;
	b2.a = 1;
	c.a = 1;

    CC cc;
    cc.BB1::a = 1;



    A* paa = &aa;
    A* pb1 = &b1;
    //A* pb2 = &b2; 
    A* pc = &c; pc;
    D* pd = &d; pd;

    paa = &aaa;
    AAA* paaa = static_cast<AAA*>(&aa);
    REQUIRE(paaa != nullptr);
    paaa = dynamic_cast<AAA*>(&aa);
    REQUIRE(paaa == nullptr);


    //won't compile, as B1 is virtual inheritance
    //B1* p1 = static_cast<B1*>(pb1); 
    AA* p2 = static_cast<AA*>(paa); p2;

    //can compile, but is disaster
    AA* p3 = static_cast<AA*>(pb1); 
    REQUIRE(p3 != nullptr);

    //won't compile
    //D* pd = static_cast<D*>(pb1);
    //can cast, but disaster
    D* pd2 = reinterpret_cast<D*>(pb1);
    REQUIRE(pd2 != nullptr);

    //won't compile if A is not polymorphism class : does not have virtual function 
    AA* p4 = dynamic_cast<AA*>(pb1); 
    //dynamic check failed 
    REQUIRE(p4 == nullptr);

    //D* p5 = static_cast<D*>(pc);
}

TEST_CASE("const correctness") {
    int i1 = 10;
    int i2 = 20;
    const int * p1 = &i1;
    p1 = &i2;
    //*p1 = 100;  //won't compile
    int const * p2 = &i2;
    p2 = &i1;
    //*p2 = 100;  //won't compile
    int * const p3 = &i1;
    //p3 = &i2;   //won't compile
    *p3 = 100;
    REQUIRE(i1 == 100);
}

}
