#include "catch.hpp"
#include <iostream>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

namespace CxxBoost
{

namespace{

}
struct Test2Arg
{
    Test2Arg(const Test2Arg&) = delete;
    Test2Arg(int v1, int v2):m1(v1),m2(v2){}
    int m1,m2;
    int f1() {return m1;}
    int f2() {return m2;}
};

std::ostream& operator<<(std::ostream& s, Test2Arg& arg)
{
    s<<"Test2Arg["<<arg.m1<<","<<arg.m2<<"]";
    return s;
}

TEST_CASE("in_place with optional", "[boost]") {
    boost::optional<typename CxxBoost::Test2Arg> t1(boost::in_place(100,200));
    std::cout<<*t1<<std::endl;

    //not compil
    //Test2Arg t2(t1);

    boost::optional<typename CxxBoost::Test2Arg> t2;
    try
    {
        t2 = boost::in_place(400,300);
        std::cout<<*t2<<std::endl;
        REQUIRE(t2->f1() == 400);
        REQUIRE(t2->f2() == 300);
    }
    catch(const std::exception& e)
    {
        std::cout<<e.what()<<std::endl;
    }


    boost::optional<typename CxxBoost::Test2Arg> t3;
    t3 = boost::in_place(500,600);
    std::cout<<*t3<<std::endl;
}

}