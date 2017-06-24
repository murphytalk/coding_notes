#include "catch.hpp"
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>
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

static std::pair<string,string> lru_cache_test(const char* iff, const char* off){
	ifstream f(Utils::get_data_file_path(iff), ifstream::in);
	ostringstream o;

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

	//read expected output
	ifstream e(Utils::get_data_file_path(off), ifstream::in);
	ostringstream ss;
	ss << e.rdbuf();
	return make_pair(ss.str(), o.str());
}

TEST_CASE("LRU Cache: Test case 1", "[hackerrank]"){
	auto r = lru_cache_test("hackerrank-lru-cache-1-input.txt", "hackerrank-lru-cache-1-output.txt");
	REQUIRE(r.first == r.second);
}

TEST_CASE("LRU Cache: Test case 2", "[hackerrank]"){
	auto r = lru_cache_test("hackerrank-lru-cache-2-input.txt", "hackerrank-lru-cache-2-output.txt");
	REQUIRE(r.first == r.second);
}

}