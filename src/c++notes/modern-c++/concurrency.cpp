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

namespace Cxx11Test {

TEST_CASE("concurrency : std::lock","[c++11]"){
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
    
   
    auto assign_lunch_partner = [](Employee &e1, Employee &e2)
    {
	    struct send_mail //use functor to simulate a inner function
		{
    		void operator()(Employee &, Employee &) {
    			// simulate a time-consuming messaging operation
    			std::this_thread::sleep_for(std::chrono::seconds(1));
    		}
		};

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
		send_mail m;
		m(e1, e2);
        m(e2, e1);
    };


	Employee alice("alice"), bob("bob"), christina("christina"), dave("dave");

	// assign in parallel threads because mailing users about lunch assignments
	// takes a long time
	std::vector<std::thread> threads;
	threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
	threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
	threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
	threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));

	for (auto &thread : threads) thread.join();
	LOG << alice.output() << "\n" << bob.output() << "\n"
		<< christina.output() << "\n" << dave.output() << "\n";
}


TEST_CASE("thread : start thread with functor", "[c++11]") {
	struct SayHello {
		void operator()() { LOG << "Hello from functor!" << endl; }
	};

	thread t { SayHello() }; 
	t.join();
}


TEST_CASE("thread : start thread with std::bind", "[c++11]") {
	auto thread_func = [](string const& msg) {LOG << msg << endl; };
	thread t(bind(thread_func,"Hello from std::bind!"));
	t.join();
}

TEST_CASE("concurrency : double-checked locking") {
	const int sample = 123;

	class expensive_data {
	public:
		const int value;
		expensive_data(const int v):value(v) { 
		    std::this_thread::sleep_for(std::chrono::seconds(1));
			LOG << "expensive data " << value << " created" << endl; 
		}
	};

	class lazy_init_with_cache
	{
		mutable std::mutex m;
		mutable std::shared_ptr<const expensive_data> data;

	public:
		//https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-6-double-checked-locking.html
		std::shared_ptr<const expensive_data> get_data() const
		{
			std::shared_ptr<const expensive_data> result =
				std::atomic_load_explicit(&data, std::memory_order_acquire);
			if (!result)
			{
				std::lock_guard<std::mutex> lk(m);
				result = data;
				if (!result)
				{
					result.reset(new expensive_data(sample));
					std::atomic_store_explicit(&data, result, std::memory_order_release);
				}
			}
			return result;
		}
		void invalidate_cache()
		{
			std::lock_guard<std::mutex> lk(m);
			std::shared_ptr<const expensive_data> dummy;
			std::atomic_store_explicit(&data, dummy, std::memory_order_relaxed);
		}
	};

	lazy_init_with_cache cache;

	auto f = [&cache]() {
		LOG << this_thread::get_id() << ":get data " << cache.get_data()->value << endl;
	};

	LOG << "Start 2 threads to get expensive data" << endl;
	thread t1(f);
	thread t2(f);

	t1.join();
	t2.join();
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
