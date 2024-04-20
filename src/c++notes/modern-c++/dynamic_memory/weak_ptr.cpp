#include <catch2/catch_test_macros.hpp>
#include <memory> //C++ 11 smart pointers
namespace Cxx11Test{

class WeakPtr{
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
        std::weak_ptr<CycleB> b;
        int a_value;
    };
    
    class CycleB{
    public:
        CycleB(){
            INFO("CycleB created");
        }
        ~CycleB(){
            INFO("CycleB destroyed");
        }
        std::weak_ptr<CycleA> a;
        int b_value;
    };
};

TEST_CASE("WeakPtr does not cause circular refrence","[c++11][smartptr]"){
    std::shared_ptr<WeakPtr::CycleA> a(new WeakPtr::CycleA);    
    std::shared_ptr<WeakPtr::CycleB> b(new WeakPtr::CycleB); 
    
    a->a_value = 10;
    b->b_value = 20;
    
    REQUIRE(a.use_count() == 1);
    REQUIRE(b.use_count() == 1);
    
    //cycle reference
    a->b = b; 
    b->a = a; 
    
    //check the weak pointers
    REQUIRE(a->b.lock()->b_value==20);
    REQUIRE(b->a.lock()->a_value==10);
    
    REQUIRE(a.use_count()==1);
    REQUIRE(b.use_count()==1);
    INFO("Both CycleA and CycleB WILL BE freed!");
}


    
}
