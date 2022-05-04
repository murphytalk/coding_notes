#include "catch2/catch.hpp"
#include <string>
#include <map>
#include <memory> //C++ 11 smart pointers
#include <functional>
#include <boost/noncopyable.hpp>
#include "src/utils/utils.h"

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

/*README START  

# 
 A stock factory which can return the same Stock object for the same stock symbol
 
 There are several generations of attempts trying to solve problems the previous attempt didn't solve.
 See the test cases below the factory implementations
 
 README END*/

class Stock{
public:
     const std::string symbol;
     Stock(const std::string& sym):symbol(sym){
        LOG<<"Created stock "<<sym<<std::endl;
     };
     ~Stock(){
        LOG<<"Destroyed stock "<<symbol<<std::endl;
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
        
    size_t size() const {return _stocks.size();}
};  

class StockFactory2 : public StockFactory
{
public:
   void delStock(Stock* stock){
       if(stock){
           removeSymbol(stock->symbol);
           LOG<<"removed "<<stock->symbol<<" from _stocks"<<std::endl;
       }
       delete stock;
   }
   
   void removeSymbol(const std::string& symbol){
       _stocks.erase(symbol);
   }
protected:
   
    virtual void newStock(std::shared_ptr<Stock>& stock, const std::string& symbol){
        /*
         * StockFactory2::delStock gets called while shared_ptr decides to delete the pointer.
         * Potential problem : 
         * the StockFactory2 pointer this could become invalid if StockFactory2 is freed earlier than Stock - see StockFactory3 for solution
         */
        stock.reset(new Stock(symbol),std::bind(&StockFactory2::delStock,this,std::placeholders::_1));
    }
};

class StockFactory3 : public std::enable_shared_from_this<StockFactory3>, public StockFactory2{
public:
    StockFactory3(){
        LOG<<"StockFactory3 created"<<std::endl;
    }
    ~StockFactory3(){
        LOG<<"StockFactory3 destroyed"<<std::endl;
    }
protected:
    virtual void newStock(std::shared_ptr<Stock>& stock, const std::string& symbol){
        //each deleter will hold one count of StockFactory3 via std::bind
        //this guruantees StockFactory3 won't get freed before shared_ptr<Stock> does
        stock.reset(new Stock(symbol),std::bind(&StockFactory2::delStock,shared_from_this(),std::placeholders::_1));
    }
};

class StockFactory4 : public std::enable_shared_from_this<StockFactory4>, public StockFactory2{
public:
    StockFactory4(){
        LOG<<"StockFactory4 created"<<std::endl;
    }
    ~StockFactory4(){
        LOG<<"StockFactory4 destroyed"<<std::endl;
    }
protected:
    virtual void newStock(std::shared_ptr<Stock>& stock, const std::string& symbol){
        //each deleter will hold one count of StockFactory3 via std::bind
        //this guruantees StockFactory3 won't get freed before shared_ptr<Stock> does
        stock.reset(new Stock(symbol),std::bind(&StockFactory4::weakDelete,std::weak_ptr<StockFactory4>(shared_from_this()),std::placeholders::_1));
    }
    
    static void weakDelete(const std::weak_ptr<StockFactory4>& weakFactory,Stock *stock){
        std::shared_ptr<StockFactory4> factory(weakFactory.lock());
        if(factory){
            LOG<<"factory not destroyed yet"<<std::endl;
            factory->removeSymbol(stock->symbol);
        }
        else{
            LOG<<"factory already destroyed"<<std::endl;
        }
        delete stock;
    }
};

TEST_CASE("Mem leak : symbols map never get freed","[c++11][smartptr]"){
    StockFactory factory;
    
    REQUIRE(factory.size()==0);
    LOG<<"START"<<std::endl;
    {
        auto amazon1 = factory.get(std::string("AMZN"));
        auto amazon2 = factory.get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
    }
    LOG<<"END"<<std::endl;
    REQUIRE(factory.size()==1); //symbol is still there, althoug it maps an empty pointer
}
    
TEST_CASE("Solve mem leak by using deleter","[c++11][smartptr]"){
    StockFactory2 factory;
    
    REQUIRE(factory.size()==0);
    LOG<<"START"<<std::endl;
    {
        auto amazon1 = factory.get(std::string("AMZN"));
        auto amazon2 = factory.get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
    }
    LOG<<"END"<<std::endl;
    REQUIRE(factory.size()==0); //all symbols are freed too
}

TEST_CASE("Solve possible earlier free of factory: stock destroyed first","[c++11][smartptr]"){
    std::shared_ptr<StockFactory3> factory(new StockFactory3);
    
    REQUIRE(factory->size()==0);
    LOG<<"START"<<std::endl;
    {
        auto amazon1 = factory->get(std::string("AMZN"));
        auto amazon2 = factory->get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
        LOG<<"factory count="<<factory.use_count()<<std::endl;
    }
    LOG<<"factory count="<<factory.use_count()<<std::endl;
    LOG<<"END"<<std::endl;
    REQUIRE(factory->size()==0);
}

TEST_CASE("Solve possible earlier free of factory but factory lives longer than expected: stock destroyed later","[c++11][smartptr]"){
    std::shared_ptr<Stock> amazon1;
    
    LOG<<"START"<<std::endl;
    {
        std::shared_ptr<StockFactory3> factory(new StockFactory3);
        amazon1 = factory->get(std::string("AMZN"));
        auto amazon2 = factory->get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
        LOG<<"factory count="<<factory.use_count()<<std::endl;
    }
    //amazon1 still holds one count of factory, factory still living !!!! 
    LOG<<"END"<<std::endl;
}

TEST_CASE("Does not matter which is freed first: stock destroyed first","[c++11][smartptr]"){
    std::shared_ptr<StockFactory4> factory(new StockFactory4);
    
    REQUIRE(factory->size()==0);
    LOG<<"START"<<std::endl;
    {
        auto amazon1 = factory->get(std::string("AMZN"));
        auto amazon2 = factory->get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
        LOG<<"factory count="<<factory.use_count()<<std::endl;
    }
    LOG<<"factory count="<<factory.use_count()<<std::endl;
    LOG<<"END"<<std::endl;
    REQUIRE(factory->size()==0);
}

TEST_CASE("Does not matter which is freed first: stock destroyed later","[c++11][smartptr]"){
    std::shared_ptr<Stock> amazon1;
    
    LOG<<"START"<<std::endl;
    {
        //factory count is only 1 (from shared this), stock holds weak pointer
        std::shared_ptr<StockFactory4> factory(new StockFactory4); 
        amazon1 = factory->get(std::string("AMZN"));
        auto amazon2 = factory->get(std::string("AMZN"));
        REQUIRE(amazon1.get() == amazon2.get());
        LOG<<"factory count="<<factory.use_count()<<std::endl;
    }
    LOG<<"END"<<std::endl;
}
} 
