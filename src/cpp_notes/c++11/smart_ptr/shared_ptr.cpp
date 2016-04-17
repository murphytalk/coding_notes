#include "catch.hpp"
#include <memory> //C++ 11 smart pointers
namespace Cxx11Test{

class SharedPtr{
public:
    class CycleB;
    class CycleA{
    public:
        CycleA(){
            INFO("CycleA created");
        }
        ~CycleA(){
            INFO("CycleA destroyed");
        }
        std::shared_ptr<CycleB> b;
    };
    
    class CycleB{
    public:
        CycleB(){
            INFO("CycleB created");
        }
        ~CycleB(){
            INFO("CycleB destroyed");
        }
        std::shared_ptr<CycleA> a;
    };
};

    
TEST_CASE("shared_ptr circular refrence","[c++11][smartptr]"){
    std::shared_ptr<SharedPtr::CycleA> a(new SharedPtr::CycleA);    
    std::shared_ptr<SharedPtr::CycleB> b(new SharedPtr::CycleB);    
   
    REQUIRE(a.use_count() == 1);
    REQUIRE(b.use_count() == 1);
    
    //circular reference
    a->b = b; 
    b->a = a; 
    REQUIRE(a.use_count() == 2);
    REQUIRE(b.use_count() == 2);
    INFO("Both CycleA and CycleB won't be freed!");
}


    
}
