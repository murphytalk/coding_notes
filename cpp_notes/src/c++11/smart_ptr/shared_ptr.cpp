#include <gtest/gtest.h>
#include <memory> //C++ 11 smart pointers
namespace Cxx11Test{

class SharedPtr: public ::testing::Test{
public:
    class CycleB;
    class CycleA{
    public:
        CycleA(){
            printf("CycleA created\n");
        }
        ~CycleA(){
            printf("CycleA destroyed\n");
        }
        std::shared_ptr<CycleB> b;
    };
    
    class CycleB{
    public:
        CycleB(){
            printf("CycleB created\n");
        }
        ~CycleB(){
            printf("CycleB destroyed\n");
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
    printf("Both CycleA and CycleB won't be freed!\n");
}


    
}
