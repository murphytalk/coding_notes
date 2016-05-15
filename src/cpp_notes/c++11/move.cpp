#include "catch.hpp"
#include <iostream>
namespace Cxx11Test{
    class Rvalue{
    public: 
        int *p;
        Rvalue(){
           p = new int;
           std::cout<<"created p "<<p<<std::endl;
        }
        ~Rvalue(){
           std::cout<<"deleted p "<<p<<std::endl;
           delete p;
        }
        Rvalue(Rvalue&& r){
           std::cout<<"move constructor,move resource p "<<r.p<<std::endl; 
           p = r.p;
           r.p = nullptr;
        }
        Rvalue& operator=(Rvalue&& r){
            std::cout<<"move assigment, move resouce p"<<r.p<<std::endl;
            if(this!=&r){
                std::cout<<"deleted existing p "<<p<<std::endl;
                delete p;
                p = r.p;
                r.p = nullptr;
            }
        }
    };
    
    Rvalue make_rvalue(){
        std::cout<<"make rvalue:";
        return Rvalue();
    }
    
    void accept_rvalue(Rvalue&& r){
        std::cout<<"accepted p "<<r.p<<std::endl;
    }
    
    TEST_CASE("C++ 11 move assigment","[c++11]"){
        std::cout<<"The destination:";
        Rvalue r;
        r = make_rvalue();
    }
    
    TEST_CASE("C++ 11 move constructor","[c++11]"){
        accept_rvalue(Rvalue());
    }
}