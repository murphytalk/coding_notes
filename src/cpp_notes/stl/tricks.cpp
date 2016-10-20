#include "catch.hpp"
#include <vector>
#include <algorithm>

using namespace std;

namespace CxxSTL{
    TEST_CASE("Efficiently remove one elment from vector","[stl]"){
        vector<int> v1,v2;
        for(int i=0;i<10000;++i){
            v1.push_back(i);
            v2.push_back(i);
        }
        
        SECTION("Naive way"){
            v1.erase(find(v1.begin(),v1.end(),1000));
        }
        SECTION("Smart way"){
            /**
            * if the element order does not matter, 
            * swapping the element to delete with the last one and then pop back could save some element moving
            */ 
            auto it = find(v1.begin(),v1.end(),1000);
            swap(*it,v2.back());
            v2.pop_back();
        }
    }
}
