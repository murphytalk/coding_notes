#include "catch.hpp"
#include <string>
#include <iostream>
#include <map>
#include <memory> //C++ 11 smart pointers
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

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


class Stock{
public:
     const std::string symbol;
     Stock(const std::string& sym):symbol(sym){
        std::cout<<"Created stock "<<sym<<std::endl;
     };
     ~Stock(){
        std::cout<<"Destroyed stock "<<symbol<<std::endl;
     }
};

class StockFactory : boost::noncopyable
{
protected:
        std::map<std::string,std::weak_ptr<Stock>> _stocks;
public:
        //not thread-safe
        std::shared_ptr<Stock> get(const std::string& symbol)
        {
            std::shared_ptr<Stock> stock;
            
            std::weak_ptr<Stock>& s = _stocks[symbol];
            stock = s.lock();
            if(!stock){
                stock.reset(new Stock(symbol));
                s = stock;
            }
            return stock;
        }
        
        int size() const {return _stocks.size();}
};        

TEST_CASE("Mem leak : _stocks never get freed","[c++11][smartptr]"){
    StockFactory factory;
    
    REQUIRE(factory.size()==0);
    std::cout<<"START"<<std::endl;
    {
        auto amazon1 = factory.get(std::string("AMZN"));
        auto amazon2 = factory.get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
    }
    std::cout<<"END"<<std::endl;
    REQUIRE(factory.size()==1);
}
    
}
