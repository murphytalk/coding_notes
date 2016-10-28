#include "catch.hpp"
#include "utils.h"
#include "blocking_queue.h"
#include <string>
#include <thread>
#include <atomic>

using namespace std;

namespace UtilsTest {

TEST_CASE("bitset: even numbers under 101 are set", "[utils]") {
	const int bitsize = 101;
    Utils::bitset s(bitsize);

	for (uint32_t i = 0; i < bitsize; i += 2) {
		s.set(i);
	}

	SECTION("even numbers are set") {
		for (uint32_t i = 0; i <= bitsize; i += 2) {
			REQUIRE(s.check(i));
		}
	}

	SECTION("check odd numbers are not set") {
		for (uint32_t i = 1; i <= bitsize; i += 2) {
			REQUIRE(!s.check(i));
		}
	}

	SECTION("numbers out of range are not set") {
		//REQUIRE(!s.check(-1));
		REQUIRE(!s.check(1000));
	}

	SECTION("clear bits") {
		for (uint32_t i = 0; i <= bitsize; i += 2) {
			s.clear(i);
		}

		for (uint32_t i = 0; i <= bitsize; i += 2) {
			REQUIRE(!s.check(i));
		}
	}
}

TEST_CASE("bitset: string", "[utils]") {
	//                       5    0
	const std::string s1 = "1111010";
    Utils::bitset t(s1);

	SECTION("check bits") {
		REQUIRE(!t.check(0));
		REQUIRE(t.check(1));
		REQUIRE(!t.check(2));
		REQUIRE(t.check(3));
		REQUIRE(t.check(4));
		REQUIRE(t.check(5));
		REQUIRE(t.check(6));

		REQUIRE(!t.check(7));
		REQUIRE(!t.check(8));
	}

	SECTION("restore to string") {
		REQUIRE(s1 == t.to_str());
	}
}

TEST_CASE("bitset: string should not have leading 0", "[utils]") {
	const std::string s = "10";
    Utils::bitset t(3);
	t.set(1);
	REQUIRE(t.to_str() == s);
}

/**
 Blocking queue test:

 Start 2 publisher threads and 1 consumer thread:

 - publisher 1 publishes publiser_count unique Message objects(tag=a), data starts from seed1;
 - publisher 2 publishes publiser_count unique Message objects(tag=b), data starts from seed2;

 Verify:

 - consumer checks each recevied message:
   if tag is a, then data value should be between [seed1, seed1+publisher_count)
   if tag is b, then data value should be between [seed2, seed2+publisher_count)
 - consumer uses a bitset to save each recieved data value to check if duplicated value is recieved
 - finally checks the total received data number

*/

TEST_CASE("blocking queue", "[utils]") {
    struct Message {
	    const string _tag;
	    const int _data;
	    Message(const string& tag, int data):_tag(tag),_data(data) {}
    };

	const string tag1 = "a";
	const string tag2 = "b";
	const string tag_end = "END";
	const int seed1 = 1;
	const int seed2 = 10000;
	const int publiser_count = 2048;

    Utils::blocking_queue<Message> q;

	auto publisher_func = [&publiser_count,&tag_end,&q](const string& tag, const int seed) {
		for (int i = 0; i < publiser_count; ++i) {
			q.push(Message(tag, seed + i));
		}
		q.push(Message(tag_end, 0)); // signal consumer thread to exit
	};

    atomic<uint32_t> count1 = { 0 }, count2 (0); //atmoic's assign operator is deleted

	auto consumer_func = [
		&q, &tag_end,
		&count1, &count2,
		&tag1, &tag2,
		&seed1, &seed2, &publiser_count]() {

		int end_signals_rcv = 0;
		for (;;) {
			/*
			Binding a temporary object to a reference to const on the stack
			lengthens the lifetime of the temporary to the lifetime of the reference itself
			*/
			const Message& msg = q.pop();
			const string& tag = msg._tag;

			if (tag == tag1) {
				REQUIRE( (msg._data >= seed1 && (msg._data <= seed1 + publiser_count)) );
				++count1;
			}
			else if (tag == tag2) {
				REQUIRE( (msg._data >= seed2 && (msg._data <= seed2 + publiser_count)) );
				++count2;
			}
			else if(tag == tag_end){
				if (++end_signals_rcv == 2) break; //we have 2 publishers 
			}
			else {
				LOG << "Invalid msg tag " << tag << " recieved!";
			}
		}
	};

	{
			thread consumer(consumer_func);
			LOG << "consumer started" << endl;

    		std::this_thread::sleep_for(std::chrono::milliseconds(500));
            REQUIRE(count1 == 0);
            REQUIRE(count2 == 0);

			thread publisher1(publisher_func, tag1, seed1);
			LOG << "publisher1 started" << endl;

    		std::this_thread::sleep_for(std::chrono::milliseconds(500));
            REQUIRE(count1 > 0u);
            REQUIRE(count2 == 0);

			thread publisher2(publisher_func, tag2, seed2);
			LOG << "publisher2 started" << endl;

			publisher1.join();
			LOG << "publisher1 done" << endl;
			publisher2.join();
			LOG << "publisher2 done" << endl;
			consumer.join();
			LOG << "consumer done, count1=" << count1 << ",count2=" << count2 << endl;
	}

	REQUIRE(publiser_count == count1);
	REQUIRE(publiser_count == count2);
}

TEST_CASE("nomralize path - consecutive forward slash", "[utils][file]") {
    auto s = normalize_path("/root/abc//efg");
    REQUIRE(s == "/root/abc/efg");
}

TEST_CASE("nomralize path - tailing slash", "[utils][file]") {
    auto s = normalize_path("/root/abc/efg/");
    REQUIRE(s == "/root/abc/efg");
}

}