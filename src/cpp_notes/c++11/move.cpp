#include "catch.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

//http://thbecker.net/articles/rvalue_references/section_01.html
//https://msdn.microsoft.com/en-us/library/dd293668.aspx
namespace Cxx11Test{
    class Rvalue{
    public:
		int id;
		string s;
        Rvalue(int i,const char* p):id(i),s(p){
           std::cout<<"created id="<<id<<"(" <<s<<")" << std::endl;
        }
        ~Rvalue(){
           std::cout<<"deleted id=" << id <<"("<<s<< ")" << std::endl;
        }
		Rvalue(const Rvalue& r) = delete;
        Rvalue(Rvalue&& r){
           std::cout<<"move constructor,move resource ("<<r.s<< ") from id="<<r.id<<" to id="<<id << std::endl;
		   id = r.id;
		   s = std::move(r.s);
        }
		/* without move assignment operator, the following code won't compile
			Rvalue r(100,"Destination");
			Rvalue r = make_rvalue(1,"Source");
		*/
		Rvalue& operator=(Rvalue&& r){
			std::cout << "move assignment,move resource (" << r.s << ") from id=" << r.id << " to id=" << id << std::endl;
            if(this!=&r){
				s = std::move(r.s);
            }
			return *this;
        }
    };

    Rvalue make_rvalue(int id,const char*s){
        std::cout<<"make rvalue:";
        return Rvalue(id,s);
    }

    void accept_rvalue(Rvalue&& r){
        std::cout<<"rvalue ref :accepted id="<<r.id<<"("<<r.s << ")" <<std::endl;
		cout << "end of accept_rvalue()"<<endl;
    }

    void accept_rvalue(Rvalue& r){
        std::cout<<"lvalue ref :accepted id="<<r.id<<"("<<r.s << ")" <<std::endl;
		cout << "end of accept_rvalue()"<<endl;
    }


	TEST_CASE("C++ 11 move assignment 1", "[c++11]") {
		Rvalue r(200, "Orginal");
		auto& v = r;
		cout << "orginal addr " << &r << endl;
		cout << "dest addr " << &v << endl;
	}


    TEST_CASE("C++ 11 move assignment 2","[c++11]"){
        std::cout<<"The destination:";
        Rvalue r(100,"Destination");
		r = make_rvalue(1,"Source");
    }

    TEST_CASE("C++ 11 rvalue reference","[c++11]"){
		cout << "START" << endl;
		//the following line won't compile if accept_rvalue is defined as accept_rvalue(Rvalue& r)
        accept_rvalue(Rvalue(1000,"Temp"));
		cout << "end of calling accept_rvalue()" << endl;
		cout << "END" << endl;
    }

    TEST_CASE("C++ 11 lvalue reference","[c++11]"){
		cout << "START" << endl;
        Rvalue r(1000,"Temp");
        accept_rvalue(r);
		cout << "end of calling accept_rvalue()" << endl;
		cout << "END" << endl;
    }
    
	TEST_CASE("C++ 11 move constructor 1", "[c++11]") {
		vector<Rvalue> v;
		//temp object will trigger rvalue move constructor
		v.push_back(Rvalue(1, "data1"));
		v.push_back(Rvalue(2, "data2"));
	}

	TEST_CASE("C++ 11 move constructor 2", "[c++11]") {
		vector<Rvalue> v;

		Rvalue r1(10, "data10");
		Rvalue r2(20, "data20");

		//won't compile if don't use std::move due to miss Rvalue(const RValue&) => needed by vector
		v.push_back(std::move(r1));
		v.push_back(std::move(r2));
	}

	TEST_CASE("C++ 11 move - queue", "[c++11]") {
		queue<Rvalue> q;
		Rvalue r(100, "data100");
		//if r is defined as const then the following won't compile
		q.push(std::move(r));
	}
}
