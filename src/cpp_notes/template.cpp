#include "catch.hpp"
#include <iostream>

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
        std::cout<<"Doing job ...";
        interface();
    }
    
    static void static_func()
    {
        T::static_sub_func();
    }
    
    static void static_job()
    {
        std::cout<<"Doing static job ...";
        static_func();
    }
 };

struct Derived1 : public Base<Derived1>
{
    void implementation(){
        mark = 1;
        std::cout<<"Derived1::interface"<<std::endl;
    }
    
    static void static_sub_func(){
        std::cout<<"Derived1::static_sub_func"<<std::endl;
    }
};

struct Derived2 : public Base<Derived2>
{
    void implementation(){
        mark = 2;
        std::cout<<"Derived2::interface"<<std::endl;
    }
    
    static void static_sub_func(){
        std::cout<<"Derived2::static_sub_func"<<std::endl;
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
