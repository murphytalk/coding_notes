#include "catch.hpp"
#include "src/utils/utils.h"

namespace CxxTemplate{

/* 
 * Curiously recurring template pattern
 * https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */

template <class T> 
struct Base
{
    int mark;
    void interface()
    {
        static_cast<T*>(this)->implementation();
    }

    void job()
    {
        LOG<<"Doing job ...";
        interface();
    }
    
    static void static_func()
    {
        T::static_sub_func();
    }
    
    static void static_job()
    {
        LOG<<"Doing static job ...";
        static_func();
    }
};

struct Derived1 : public Base<Derived1>
{
    void implementation(){
        mark = 1;
        LOG<<"Derived1::interface"<<std::endl;
    }
    
    static void static_sub_func(){
        LOG<<"Derived1::static_sub_func"<<std::endl;
    }
};

struct Derived2 : public Base<Derived2>
{
    void implementation(){
        mark = 2;
        LOG<<"Derived2::interface"<<std::endl;
    }
    
    static void static_sub_func(){
        LOG<<"Derived2::static_sub_func"<<std::endl;
    }
};

//Also called: simulated dynamic binding
TEST_CASE("CRTP:Static polymorphism","[template]"){
    
   Derived1 d1;
   Derived2 d2;
   
   d1.job();
   REQUIRE(d1.mark==1);
   
   d2.job();
   REQUIRE(d2.mark==2);
  
   Derived1::static_job();
   Derived2::static_job();
   
}    
        
    
}
