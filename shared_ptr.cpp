#include <iostream>
#include <gtest/gtest.h>
#include <memory> //C++ 11 smart pointers
namespace BoostTest{

class SharedPtr: public ::testing::Test{
public:
    class CycleB;
    class CycleA{
    public:
        CycleA(){
            std::cout<<"CycleA created"<<std::endl;
        }
        ~CycleA(){
            std::cout<<"CycleA destroyed"<<std::endl;
        }
        std::shared_ptr<CycleB> b;
    };
    
    class CycleB{
    public:
        CycleB(){
            std::cout<<"CycleB created"<<std::endl;
        }
        ~CycleB(){
            std::cout<<"CycleB destroyed"<<std::endl;
        }
        std::shared_ptr<CycleA> a;
    };
};

TEST_F(SharedPtr,cycle){
    std::shared_ptr<CycleA> a(new CycleA);    
    std::shared_ptr<CycleB> b(new CycleB);    
    EXPECT_EQ(a.use_count(),1);
    EXPECT_EQ(b.use_count(),1);
    //cycle reference
    a->b = b; 
    b->a = a; 
    EXPECT_EQ(a.use_count(),2);
    EXPECT_EQ(b.use_count(),2);
    std::cout<<"Both CycleA and CycleB won't be freed!"<<std::endl;
}


    
}
