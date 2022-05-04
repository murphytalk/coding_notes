#include <catch2/catch.hpp>
#include <iostream>

#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

/*README START
# Boost Library
README END*/
namespace CxxBoost
{

namespace{

/*README START
## <<Use `in_place` with `optional`>>

`optional` may or may not hold a valid object, it overrides `bool` operator to indicate if a valid object is being held or not. 
It also overrides operator `.` and `->` so the underlying object can be operated as pointer, if it is valid.

The underlying object held by `optional` can be initialized in-place by using Boost's in place factory. 
`optional` overrides `=` operator which has a in-place factory as right hand argument.

For example, for the following class
*/

//<<
struct Arg1{
    Arg1(int v):value_(v){}
    int value_;
    int value() {return value_;}
};

struct Arg2{
    Arg2(int v):value_(v){}
    int value_;
    int value() {return value_;}
};

template<typename T1, typename T2>
struct Test2Arg
{
    Test2Arg(const Test2Arg&) = delete;
    Test2Arg(T1& v1, T2 v2):m1(v1.value()),m2(v2.value()){}
    int m1,m2;
    int f1() {return m1;}
    int f2() {return m2;}
};
//>>
//README END

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& s, Test2Arg<T1,T2>& arg)
{
    s<<"Test2Arg["<<arg.m1<<","<<arg.m2<<"]";
    return s;
}

std::ostream& operator<<(std::ostream& s, Arg1& arg)
{
    s<<"Arg1["<<arg<<"]";
    return s;
}

std::ostream& operator<<(std::ostream& s, Arg2& arg)
{
    s<<"Arg2["<<arg<<"]";
    return s;
}

}

/*README START

Declare two instances.

README END*/
TEST_CASE("in_place with optional", "[boost]") {
//README START
//<<
    Arg1 a1(100);
    Arg2 a2(200);
//>>
//README END

/*README START
`t1` below is an optional but is initialized with an `Test2Arg` instance in place:
- Note `Test2Arg` does not have copy constructor, so the value is not intialized in temp object and then copied.
- Note `Test2Arg` expects the first parameter of its constructor be non-const reference, but `boost::in_place()` only accepts either const reference
  or by-value as parameter, so we wrap `a1` inside `boost::inplace` to make it a by-value parameter.
*/
//<<
    boost::optional<typename CxxBoost::Test2Arg<Arg1, Arg2>> t1(boost::in_place(boost::ref(a1), a2));
//>>
//README END

    std::cout<<*t1<<std::endl;

/*README START
The following code cannot compile
//<<
    Test2Arg t2(t1);
//>>
README END*/

    boost::optional<typename CxxBoost::Test2Arg<Arg1, Arg2>> t2;
    try
    {
        t2 = boost::in_place(boost::ref(a1),a2);
        std::cout<<*t2<<std::endl;
        REQUIRE(t2->f1() == 100);
        REQUIRE(t2->f2() == 200);
    }
    catch(const std::exception& e)
    {
        std::cout<<e.what()<<std::endl;
    }


    boost::optional<typename CxxBoost::Test2Arg<Arg1, Arg2>> t3;
    t3 = boost::in_place(boost::ref(a1),a2);
    std::cout<<*t3<<std::endl;
}
}