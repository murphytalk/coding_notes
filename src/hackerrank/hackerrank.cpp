#include "catch.hpp"
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>
#include <functional>
#include <deque>
#include "src/utils/utils.h"

using namespace std;

namespace HackerRank {

/*
LRU cache
https://www.hackerrank.com/challenges/abstract-classes-polymorphism

Given an abstract base class Cache with member variables and functions,
write a class LRUCache which extends the class Cache and uses the member functions and variables to implement an LRU cache.

Input Format

First line of input will contain the number of lines containing or commands followed by the capacity of the cache.
The following lines can either contain or commands.
An input line starting with will be followed by a to be found in the cache. An input line starting with will be followed by the and respectively to be inserted/replaced in the cache.
*/

class Cache {
protected:
	struct Node {
		Node* next;
		Node* prev;
		int value;
		int key;
		Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
		Node(int k, int val) :prev(nullptr), next(nullptr), key(k), value(val) {};
	};

	map<int, Node*> mp; //map the key to the node in the linked list
	int cp;  //capacity
	Node* tail; // double linked list tail pointer
	Node* head; // double linked list head pointer
	virtual void set(int, int) = 0; //set function
	virtual int get(int) = 0; //get function
};

class LRUCache : public Cache {
	Node* delete_node(Node *n){ //but not free it
		if (!n) return nullptr;
		Node* prev = n->prev;
		Node* next = n->next;
		if (prev) {
			prev->next = next;
		}
		if (next) {
			next->prev = prev;
		}
		if (n == head) {
			head = next;
		}
		if (n == tail) {
			tail = prev;
		}
		return n;
	}
	void add_to_head(Node *n) {
		if (head) head->prev = n;
		n->prev = nullptr;
		n->next = head;
		head = n;
	}
	void add(int new_key, int new_value, bool evict) {
		if (evict) {
			mp.erase(mp.find(tail->key));
			delete delete_node(tail);
		}
		Node* newest = new Node(new_key, new_value);
		mp.insert(make_pair(new_key, newest));
		add_to_head(newest);
		if (tail == nullptr) tail = newest;
	}
	void move_to_head(Node *same) {
		delete_node(same);
		add_to_head(same);
	}
public:
	LRUCache(int n) {
		tail = nullptr;
		head = nullptr;
		cp = n;
	}
	~LRUCache() {
		for_each(mp.begin(), mp.end(), [](map<int, Node*>::value_type& n) {delete n.second; });
	}
	int get(int key) {
		auto pos = mp.find(key);
		return pos == mp.end() ? -1 : pos->second->value;
	}
	void set(int key, int value) {
		if (cp == 0) return;
		auto it = mp.find(key);
		Node *same = it == mp.end() ? nullptr : it->second;
		if (same == nullptr) {
			//key not cached before
			add(key, value, mp.size() >= cp);
		}
		else {
			same->value = value;
			move_to_head(same);
		}
	}
};

/*
Hacker ranker gives test data in STDIN and checks result in STDOUT.
This helper feeds the input/output downloaded from hacker ranker to a user defined function.
It returs a pair where expected output is the first and our output is the second.
*/
static pair<string, string> hacker_ranker_tester(const char* iff, const char* off, function<void(ifstream&,ostringstream&)> test){
	ifstream f(Utils::get_data_file_path(iff), ifstream::in);
	if (!f) return make_pair("missing file in data folder",iff);

	//read expected output
	ifstream e(Utils::get_data_file_path(off), ifstream::in);
	if (!f) return make_pair("missing file in data folder",off);

	ostringstream ss;
	ss << e.rdbuf();

	ostringstream o;
	test(f,o);

	return make_pair(ss.str(), o.str());
}

static pair<string,string> lru_cache_test(const char* iff, const char* off){
	return hacker_ranker_tester(iff, off, [](ifstream& f, ostringstream& o) {
    	int n, capacity, i;
    	f >> n >> capacity;
    	LRUCache l(capacity);
    	for (i = 0; i<n; i++) {
    		string command;
    		f >> command;
    		if (command == "get") {
    			int key;
    			f >> key;
    			o << l.get(key) << endl;
    		}
    		else if (command == "set") {
    			int key, value;
    			f >> key >> value;
    			l.set(key, value);
    		}
    	}
	});
}

TEST_CASE("LRU Cache", "[hackerrank]"){
	SECTION("Test case 1") {
		auto r = lru_cache_test("hackerrank-lru-cache-1-input.txt", "hackerrank-lru-cache-1-output.txt");
		REQUIRE(r.first == r.second);
	}

	SECTION("Test case 2") {
		auto r = lru_cache_test("hackerrank-lru-cache-2-input.txt", "hackerrank-lru-cache-2-output.txt");
		REQUIRE(r.first == r.second);
	}
}

/* 
Max value in contiguous subarray
https://www.hackerrank.com/challenges/deque-stl

Given a set of arrays of size and an integer , you have to find the maximum integer for each and every contiguous subarray of size for each of the given arrays.

Input Format:
First line of input will contain the number of test cases T. 
For each test case, you will be given the size of array N and the size of subarray to be used K.
This will be followed by the elements of the array Ai.

Sample Input:
2
5 2
3 4 6 3 4
7 4
3 4 5 8 1 4 10

Sample Output:
4 6 6 4
8 8 8 10
*/
static void printKMax(int arr[], int n, int k,ostringstream& out) {
	deque<int> q;
	//slide the window along with the sub-array

	//the first window, only value larger than the last seen value will survive and are sorted in descending order 
	int i;
	for (i = 0; i < k; ++i) {
		while(!q.empty() && arr[q.back()] <= arr[i]) {
			q.pop_back();
		}
		q.push_back(i);
	}
	// the head is the index of the max value
	/* Examples:
	   1) if the first sub-array is [4 1 2 3], then what's left in the queue is [4 3]
	      4 will be evicted as window slides, 1,2 does not even enter the queue but that does not matter as 
		    a) they are smaller than 3 and 
			b) they will be evicted before 3 so they will never have chance

			Note while loop is important otherwise in the above case 1 will survive in the queue while it is smaller than 3
	   2) if the first sub-array is [1 2 3 4], then what's left in the queue is [4]
	*/

	// i is at the end of the first window now
	for (; i < n; ++i) {
		//the head of q is the max value of the previous window
		out << arr[q.front()] << " ";

		//evict those do not below to this window
		while (!q.empty() && q.front() <= i - k) q.pop_front();

		while (!q.empty() && arr[q.back()] <= arr[i]) {
			q.pop_back();
		}

		q.push_back(i);
	}
	out << arr[q.front()] << endl;
}

static pair<string,string> contiguous_subarray_test(const char* iff, const char* off){
	return hacker_ranker_tester(iff, off, [](ifstream& f, ostringstream& o) {
		int t;
		f >> t;
		while (t > 0) {
			int n, k;
			f >> n >> k;
			int i;
			int* arr = new int[n];
			for (i = 0; i < n; i++)
				f >> arr[i];
			printKMax(arr, n, k,o);
			delete[]arr;
			t--;
		}
	});
}

TEST_CASE("Max value in contiguous subarray : Test Case 0", "[hackerrank]") {
	auto r = contiguous_subarray_test("hackerrank-contiguous-subarray-0-input.txt", "hackerrank-contiguous-subarray-0-output.txt");
	REQUIRE(r.first == r.second);
}

TEST_CASE("Max value in contiguous subarray : Test Case 1", "[hackerrank]") {
	auto r = contiguous_subarray_test("hackerrank-contiguous-subarray-1-input.txt", "hackerrank-contiguous-subarray-1-output.txt");
	REQUIRE(r.first == r.second);
}

}