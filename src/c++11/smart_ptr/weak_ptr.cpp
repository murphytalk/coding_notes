#include <gtest/gtest.h>
#include <memory> //C++ 11 smart pointers
namespace Cxx11Test{

class WeakPtr: public ::testing::Test{
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
        std::weak_ptr<CycleB> b;
        int a_value;
    };
    
    class CycleB{
    public:
        CycleB(){
            std::cout<<"CycleB created"<<std::endl;
        }
        ~CycleB(){
            std::cout<<"CycleB destroyed"<<std::endl;
        }
        std::weak_ptr<CycleA> a;
        int b_value;
    };
};

TEST_F(WeakPtr,cycle){
    std::shared_ptr<CycleA> a(new CycleA);    
    std::shared_ptr<CycleB> b(new CycleB); 
    
    a->a_value = 10;
    b->b_value = 20;
    
    EXPECT_EQ(a.use_count(),1);
    EXPECT_EQ(b.use_count(),1);
    
    //cycle reference
    a->b = b; 
    b->a = a; 
    
    //check the weak pointers
    EXPECT_EQ(a->b.lock()->b_value,20);
    EXPECT_EQ(b->a.lock()->a_value,10);
    
    EXPECT_EQ(a.use_count(),1);
    EXPECT_EQ(b.use_count(),1);
    std::cout<<"Both CycleA and CycleB WILL BE freed!"<<std::endl;
}


    
}
