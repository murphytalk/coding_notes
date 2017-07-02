#include "catch.hpp"
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <functional>
#include <chrono>
#include <string>
#include "src/utils/utils.h"

using namespace std;

//classes and functions in anonymous namepsace are invisible to external units
namespace { 

struct Employee {
    Employee(std::string id) : id(id) {}
    std::string id;
    std::vector<std::string> lunch_partners;
    std::mutex m;
    std::string output() const
    {
        std::string ret = "Employee " + id + " has lunch partners: ";
        for( const auto& partner : lunch_partners )
            ret += partner + " ";
        return ret;
    }
};

void send_mail(Employee &, Employee &)
{
    // simulate a time-consuming messaging operation
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void assign_lunch_partner(Employee &e1, Employee &e2)
{
    static std::mutex io_mutex;
    {
        std::lock_guard<std::mutex> lk(io_mutex);
        LOG << e1.id << " and " << e2.id << " are waiting for locks" << std::endl;
    }
 
    // use std::lock to acquire two locks without worrying about 
    // other calls to assign_lunch_partner deadlocking us
    {
        std::lock(e1.m, e2.m);
        std::lock_guard<std::mutex> lk1(e1.m, std::adopt_lock);
        std::lock_guard<std::mutex> lk2(e2.m, std::adopt_lock);
// Equivalent code (if unique_locks are needed, e.g. for condition variables)
//        std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
//        std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
//        std::lock(lk1, lk2);
        {
            std::lock_guard<std::mutex> lk(io_mutex);
            LOG << e1.id << " and " << e2.id << " got locks" << std::endl;
        }
        e1.lunch_partners.push_back(e2.id);
        e2.lunch_partners.push_back(e1.id);
    }
    send_mail(e1, e2);
    send_mail(e2, e1);
}

 

}

namespace Cxx11Test {

TEST_CASE("concurrency : std::lock","[c++11]"){
	Employee alice("alice"), bob("bob"), christina("christina"), dave("dave");

	// assign in parallel threads because mailing users about lunch assignments
	// takes a long time
	std::vector<std::thread> threads;
	threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
	threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
	threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
	threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));

	for (auto &thread : threads) thread.join();
	LOG << alice.output() << '\n' << bob.output() << '\n'
		<< christina.output() << '\n' << dave.output() << '\n';
}


TEST_CASE("thread : start thread with functor", "[c++11]") {
	class SayHello {
	public:
		void operator()() { LOG << "Hello!" << endl; }
	};

	thread t { SayHello() }; 
	t.join();
}

}

//gcc does not have decsent C++17 support yet
#ifdef _MSC_VER
#if _MSC_VER >= 1910 //VC2017

namespace Cxx17Test{ 

TEST_CASE("concurrency : shared mutex","[c++17]") {
	class ThreadSafeCounter {
	public:
		ThreadSafeCounter() = default;

		// Multiple threads/readers can read the counter's value at the same time.
		unsigned int get() const {
			std::shared_lock<std::shared_mutex> lock(mutex_);
			return value_;
		}

		// Only one thread/writer can increment/write the counter's value.
		void increment() {
			std::unique_lock<std::shared_mutex> lock(mutex_);
			value_++;
		}

		// Only one thread/writer can reset/write the counter's value.
		void reset() {
			std::unique_lock<std::shared_mutex> lock(mutex_);
			value_ = 0;
		}

	private:
		mutable std::shared_mutex mutex_;
		unsigned int value_ = 0;
	};

	ThreadSafeCounter counter;

	auto increment_and_print = [&counter]() {
		static mutex io_mutex;
		for (int i = 0; i < 3; i++) {
			counter.increment();
			{
				lock_guard<mutex> lk(io_mutex);
				LOG << std::this_thread::get_id() << ' ' << counter.get() << endl;
			}
		}
	};

	std::thread thread1(increment_and_print);
	std::thread thread2(increment_and_print);

	thread1.join();
	thread2.join();

}

}
#endif
#endif
