#include "catch.hpp"
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include "src/utils/utils.h"

using namespace std;

//http://thbecker.net/articles/rvalue_references/section_01.html
//https://msdn.microsoft.com/en-us/library/dd293668.aspx
namespace Cxx11Test{
    class Rvalue{
    public:
		int id;
		string s;
		Rvalue() {
           LOG<<"addr "<<this<< std::endl;
		}
        Rvalue(int i,const char* p):id(i),s(p){
           LOG<<"addr "<<this<<",created id="<<id<<"(" <<s<<")" << std::endl;
        }
        ~Rvalue(){
           LOG<<"addr "<<this<<",deleted id=" << id <<"("<<s<< ")" << std::endl;
        }
		Rvalue(const Rvalue& r) = delete;
        Rvalue(Rvalue&& r){
           LOG<<"move constructor,move resource ("<<r.s<< ") from id="<<r.id<<" to id="<<id << std::endl;
		   id = r.id;
		   r.id = -1;
		   s = std::move(r.s);
        }
		/* without move assignment operator, the following code won't compile
			Rvalue r(100,"Destination");
			Rvalue r = make_rvalue(1,"Source");
		*/
		Rvalue& operator=(Rvalue&& r){
			LOG << "move assignment,move resource (" << r.s << ") from id=" << r.id << " to id=" << id << std::endl;
            if(this!=&r){
				s = std::move(r.s);
            }
			return *this;
        }
    };

    Rvalue make_rvalue(int id,const char*s){
        LOG<<"make rvalue:";
        return Rvalue(id,s);
    }

    void accept_rvalue(Rvalue&& r){
        LOG<<"rvalue ref :accepted id="<<r.id<<"("<<r.s << ")" <<std::endl;
		LOG << "end of accept_rvalue()"<<endl;
    }

    void accept_rvalue(Rvalue& r){
        LOG<<"lvalue ref :accepted id="<<r.id<<"("<<r.s << ")" <<std::endl;
		LOG << "end of accept_rvalue()"<<endl;
    }


	TEST_CASE("C++ 11 move assignment 1", "[c++11]") {
		Rvalue r(200, "Orginal");
		auto v = std::move(r);
		LOG << "orginal addr " << &r << ",s="<<r.s<< endl;
		LOG << "dest addr " << &v << ",s="<<v.s << endl;
	}


    TEST_CASE("C++ 11 move assignment 2","[c++11]"){
        Rvalue r(100,"Destination");
		r = make_rvalue(1,"Source");
    }

    TEST_CASE("C++ 11 rvalue reference","[c++11]"){
		LOG << "START" << endl;
		//the following line won't compile if accept_rvalue is defined as accept_rvalue(Rvalue& r)
        accept_rvalue(Rvalue(1000,"Temp"));
		LOG << "end of calling accept_rvalue()" << endl;
		LOG << "END" << endl;
    }

    TEST_CASE("C++ 11 lvalue reference","[c++11]"){
		LOG << "START" << endl;
        Rvalue r(1000,"Temp");
        accept_rvalue(r);
		LOG << "end of calling accept_rvalue()" << endl;
		LOG << "END" << endl;
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
		//v.push_back(r1);
		v.push_back(std::move(r1));
		v.push_back(std::move(r2));
	}

	TEST_CASE("C++ 11 move - queue", "[c++11]") {
		queue<Rvalue> q;
		Rvalue r(100, "data100");
		//if r is defined as const then the following won't compile
		q.push(std::move(r));
	}

	TEST_CASE("C++ 11 move - default", "[c++11]") {
		class UserDefined {
		public:
			string str;
			UserDefined(const char*s) :str(s) {}
		};

		SECTION("no user defined data member") {
			const char expected[] = "test1";
			UserDefined t1(expected);
			UserDefined copied = t1;
			LOG << "After copied" << endl;
			LOG << "t1:" << t1.str << endl;
			LOG << "copied:" << copied.str << endl;
			REQUIRE(t1.str == expected);
			REQUIRE(copied.str == expected);

			UserDefined moved = std::move(t1);
			LOG << "After moved" << endl;
			LOG << "t1:" << t1.str << endl;
			LOG << "copied:" << copied.str << endl;
			LOG << "moved:" << moved.str << endl;
			REQUIRE(t1.str.empty());
			REQUIRE(copied.str == expected);
			REQUIRE(moved.str == expected);
		}

		SECTION("has user defined data member") {
			class Test {
			public:
				UserDefined u;
				Test(const char*s) :u(s) {}
			};

			const char expected[] = "test2";
			Test u1(expected);

			Test copied = u1;
			LOG << "After copied" << endl;
			LOG << "u1:" << u1.u.str << endl;
			LOG << "copied:" << copied.u.str << endl;
			REQUIRE(u1.u.str == expected);
			REQUIRE(copied.u.str == expected);

			Test moved = std::move(u1);
			LOG << "After moved" << endl;
			LOG << "u1:" << u1.u.str << endl;
			LOG << "copied:" << copied.u.str << endl;
			LOG << "moved:" << moved.u.str << endl;
			REQUIRE(u1.u.str.empty());
			REQUIRE(copied.u.str == expected);
			REQUIRE(moved.u.str == expected);
		}
	}

}
