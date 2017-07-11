#include "catch.hpp"
#include  <type_traits> 
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
        
template<typename D, typename B>
class IsDerivedFromHelper
{
    class No { };
    class Yes { /*No no[2]*/int dummy; }; //make it have different size than No

    static Yes Test(B*); //if D is derived from B, D* would be casted to B*
    static No Test(...); //any other parameter that is not derived from B
public:
    enum { Is = sizeof(Test(static_cast<D*>(nullptr))) == sizeof(Yes) };
};


template <class C, class P>
bool IsDerivedFrom() {
    return IsDerivedFromHelper<C, P>::Is;
}

template <typename Base, typename Derived>
struct is_base_of : public std::conditional_t<IsDerivedFromHelper<Derived,Base>::Is,std::true_type,std::false_type>{};


TEST_CASE("Staticly check inheritence", "[template]") {
    class A {};

    class B : public A {};

    class C {};

    auto a2b = IsDerivedFrom<A, B>();
    auto b2a = IsDerivedFrom<B, A>();
    auto c2a = IsDerivedFrom<C, A>();
    auto a2a = IsDerivedFrom<A, A>();

    LOG << "A is derived from B :" << std::boolalpha << a2b << std::endl;
    LOG << "B is derived from A :" << std::boolalpha << b2a << std::endl;
    LOG << "C is derived from A :" << std::boolalpha << c2a << std::endl;
    LOG << "A is derived from A :" << std::boolalpha << a2a <<  std::endl;

    REQUIRE(!a2b);
    REQUIRE(b2a);
    REQUIRE(!c2a);
    REQUIRE(a2a);

    //our implementation
    REQUIRE(  (is_base_of<A, B>::value) );
    REQUIRE( !(is_base_of<B, A>::value) );
    REQUIRE( !(is_base_of<C, A>::value) );
    REQUIRE( !(is_base_of<A, C>::value) );
    REQUIRE(  (is_base_of<A, A>::value) );

    //STL implementation
    REQUIRE(  (std::is_base_of<A, B>::value) );
    REQUIRE( !(std::is_base_of<B, A>::value) );
    REQUIRE( !(std::is_base_of<C, A>::value) );
    REQUIRE( !(std::is_base_of<A, C>::value) );
    REQUIRE(  (std::is_base_of<A, A>::value) );
}

    
}
