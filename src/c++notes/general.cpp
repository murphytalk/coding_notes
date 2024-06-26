#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <exception>

namespace Cxx {

class BigAndExpensiveData{
public:
    int v;
    BigAndExpensiveData(int v){
        this->v = v;
        std::cout<<"BigAndExpensiveData created " << v << std::endl;
    }
    BigAndExpensiveData(const BigAndExpensiveData&){
        throw std::runtime_error("Too expensive to be copied");
    }

    ~BigAndExpensiveData(){
        std::cout<<"BigAndExpensiveData destroyed " << v << std::endl;
    }
};

const BigAndExpensiveData return_temp_data_as_const_ref(int v){
    auto data = BigAndExpensiveData(v);
    return data;
}

TEST_CASE("expensive obj throw exception when being copied", "[c++]") {
    auto a1 = BigAndExpensiveData(19);
    REQUIRE_THROWS(
        [&](){
            auto a2 = a1;
        }()
    );
}

TEST_CASE("temp obj returned as const ref is not copied", "[c++]") {
    {
        std::cout<<"before call return_temp_data_as_const_ref()" << std::endl;
        int v = 123;
        REQUIRE_NOTHROW(
            [&](){
                const auto& data = return_temp_data_as_const_ref(v);
                REQUIRE( data.v == v);
                std::cout<<"after call return_temp_data_as_const_ref()" << std::endl;
            }()
        );
    }
    std::cout<<"after scope" << std::endl;
}

class Interface{
public:
    virtual int v() = 0;    
};

template<typename T> class Impl : public Interface{
public:
    T value;
    Impl(T v):value(v){}
    virtual int v(){
        return static_cast<int>(value);
    }
};

TEST_CASE("virtual class interface", "[c++]"){
    Impl<long> a(1000);
    Interface *i = &a;
    REQUIRE(i->v() == 1000);
}


}