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
        
    virtual void newStock(std::shared_ptr<Stock>& stock, const std::string& symbol){
        stock.reset(new Stock(symbol));
    }
public:
    //not thread-safe
    std::shared_ptr<Stock> get(const std::string& symbol)
    {
            std::shared_ptr<Stock> stock;
            
            std::weak_ptr<Stock>& s = _stocks[symbol];
            stock = s.lock();
            if(!stock){
                newStock(stock,symbol);
                //update the weak pointer in map
                s = stock;
            }
            return stock;
    }
        
    int size() const {return _stocks.size();}
};  

class StockFactory2 : public StockFactory
{
public:
   void delStock(Stock* stock){
       if(stock){
           _stocks.erase(stock->symbol);
           std::cout<<"removed "<<stock->symbol<<" from _stocks"<<std::endl;
       }
       delete stock;
   }
protected:
   
    virtual void newStock(std::shared_ptr<Stock>& stock, const std::string& symbol){
        /*
         * StockFactory2::delStock gets called while shared_ptr decides to delete the pointer.
         * Potential problem : 
         * the StockFactory2 pointer this could become invalid if StockFactory2 is freed earlier than Stock - see StockFactory3 for solution
         */
        stock.reset(new Stock(symbol),boost::bind(&StockFactory2::delStock,this,_1));
    }
};

class StockFactory3 : public std::enable_shared_from_this<StockFactory3>, public StockFactory2{
public:
    StockFactory3(){
        std::cout<<"StockFactory3 created"<<std::endl;
    }
    ~StockFactory3(){
        std::cout<<"StockFactory3 destroyed"<<std::endl;
    }
protected:
    virtual void newStock(std::shared_ptr<Stock>& stock, const std::string& symbol){
        //each deleter will hold one count of StockFactory3 via boost::bind
        //this guruantees StockFactory3 won't get freed before shared_ptr<Stock> does
        stock.reset(new Stock(symbol),boost::bind(&StockFactory2::delStock,shared_from_this(),_1));
    }
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
    
TEST_CASE("Solve mem leak by using deleter","[c++11][smartptr]"){
    StockFactory2 factory;
    
    REQUIRE(factory.size()==0);
    std::cout<<"START"<<std::endl;
    {
        auto amazon1 = factory.get(std::string("AMZN"));
        auto amazon2 = factory.get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
    }
    std::cout<<"END"<<std::endl;
    REQUIRE(factory.size()==0);
}

TEST_CASE("Solve possible earlier free of factory","[c++11][smartptr]"){
    std::shared_ptr<StockFactory3> factory(new StockFactory3);
    
    REQUIRE(factory->size()==0);
    std::cout<<"START"<<std::endl;
    {
        auto amazon1 = factory->get(std::string("AMZN"));
        auto amazon2 = factory->get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
        std::cout<<"factory count="<<factory.use_count()<<std::endl;
    }
    std::cout<<"factory count="<<factory.use_count()<<std::endl;
    std::cout<<"END"<<std::endl;
    REQUIRE(factory->size()==0);
}
} 
