#include "catch.hpp"
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <functional>
#include <memory>
#include <iterator>
#include <bitset>
#include <unordered_map>
#include "../utils/utils.h"

using namespace std;

namespace LeetCode{
    
/*
  https://leetcode.com/problems/search-for-a-range/

  Given a sorted array of integers, find the starting and ending position of a given target value.

  Your algorithm's runtime complexity must be in the order of O(log n).

  If the target is not found in the array, return [-1, -1].

  For example,
  Given [5, 7, 7, 8, 8, 10] and target value 8, return [3, 4].
*/
class SearchRange {
public:
	vector<int> searchRange(vector<int>& nums, int target) {
		vector<int> range;
		auto low = lower_bound(nums.begin(), nums.end(), target);
		if (low == nums.end() || *low != target) {
			range.push_back(-1);
			range.push_back(-1);
		}
		else {
			range.push_back((int)distance(nums.begin(),low));
			auto up = upper_bound(low, nums.end(), target);
			range.push_back((int)distance(nums.begin(),up)-1);
		}
		return range;
	}
};

TEST_CASE("Search Range: no duplication test case", "[leetcode]") {
	vector<int> nums = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	vector<int> r;
	SearchRange s;

	SECTION("too small") {
		r = s.searchRange(nums, -1);
		REQUIRE(r[0] == -1);
		REQUIRE(r[1] == -1);
	}

	SECTION("too big") {
		r = s.searchRange(nums, 100);
		REQUIRE(r[0] == -1);
		REQUIRE(r[1] == -1);
	}

	SECTION("smallest") {
		r = s.searchRange(nums, 0);
		REQUIRE(r[0] == 0);
		REQUIRE(r[1] == 0);
	}

	SECTION("biggest") {
		r = s.searchRange(nums, 11);
		REQUIRE(r[0] == 11);
		REQUIRE(r[1] == 11);
	}

}

TEST_CASE("Search Range: duplication test case", "[leetcode]") {
	vector<int> r;
	SearchRange s;

	SECTION("all same as target") {
		vector<int> nums = { 10,10,10,10,10,10 };
		r = s.searchRange(nums, 10);
		REQUIRE(r[0] ==  0);
		REQUIRE(r[1] ==  5);
	}

	SECTION("partial duplication") {
		vector<int> nums = { 5, 7, 7, 8, 8, 10 };
		r = s.searchRange(nums, 8);
		REQUIRE(r[0] == 3);
		REQUIRE(r[1] == 4);
	}
}

/*
https://leetcode.com/problems/add-two-numbers/

You are given two non-empty linked lists representing two non-negative integers. 
The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
*/
class AddTwoNumbers {
public:
	struct ListNode {
		int val;
		unique_ptr<ListNode> next;
		ListNode(int x, ListNode *n = nullptr) : val(x), next(n) {}
		//~ListNode() { LOG << val << endl; }
	};

	inline void next(ListNode*& l, int& v) const {
		if (l == nullptr) {
			v = 0;
		}
		else {
			v = l->val;
			l = l->next.get();
		}
	}

	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) const {
		ListNode *p1 = l1, *p2 = l2;
		ListNode *l = nullptr, *dummy_head = new ListNode(0), *last=dummy_head;
		int v, carry = 0;
		int p1v, p2v;

		while( p1 != nullptr || p2 != nullptr){
			next(p1, p1v);
			next(p2, p2v);
			if ((v = carry + p1v + p2v) > 9) {
				l = new ListNode(v % 10);
				carry = v / 10;
			}
			else {
				l = new ListNode(v);
				carry = 0;
			}
			last->next.reset(l);
			last = l;
		}

		if ( carry > 0) {
			last->next.reset(new ListNode(carry));
		}


		ListNode* ret = dummy_head->next.get();
		dummy_head->next.release();
		delete dummy_head;
		return ret;
	}

	const vector<int> addTwoNumbersAsVec(ListNode* l1, ListNode* l2) const {
		vector<int> result;

		ListNode *l = addTwoNumbers(l1, l2);
		for (ListNode *n = l; n != nullptr; n = n->next.get()){
			result.push_back(n->val);
		}
		delete l;
		return result;
	}
};

TEST_CASE("Add Two Numbers", "[leetcode]") {
	const AddTwoNumbers test;

	SECTION("(2 -> 4 -> 3) + (5 -> 6 -> 4)") {
		AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(2, new AddTwoNumbers::ListNode(4, new AddTwoNumbers::ListNode(3)));
		AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(5, new AddTwoNumbers::ListNode(6, new AddTwoNumbers::ListNode(4)));

		const vector<int> verify{ 7,0,8 };
		REQUIRE(test.addTwoNumbersAsVec(l1, l2) == verify);
	}

	SECTION("(4 -> 3) + (5 ->6)") {
		AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(2, new AddTwoNumbers::ListNode(4, new AddTwoNumbers::ListNode(3)));
		AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(5, new AddTwoNumbers::ListNode(6));

		const vector<int> verify{7,0,4};
		REQUIRE(test.addTwoNumbersAsVec(l1,l2) == verify);
	}

	SECTION("(5) + (5)") {
    	AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(5);
    	AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(5);
    
    	const vector<int> verify{0,1};
    	REQUIRE(test.addTwoNumbersAsVec(l1,l2) == verify);
	}

	SECTION("(1) + (9 -> 9)") {
    	AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(1);
    	AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(9,new AddTwoNumbers::ListNode(9));
    
    	const vector<int> verify{0,0,1};
    	REQUIRE(test.addTwoNumbersAsVec(l1,l2) == verify);
	}

	SECTION("(9 -> 1 -> 6) + (0)") {
		AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(9, new AddTwoNumbers::ListNode(1, new AddTwoNumbers::ListNode(6)));
		AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(0);

		const vector<int> verify{ 9,1,6 };
		REQUIRE(test.addTwoNumbersAsVec(l1, l2) == verify);
	}
};

/*
https://leetcode.com/problems/longest-substring-without-repeating-characters/#/description

Given a string, find the length of the longest substring without repeating characters.

Examples:

Given "abcabcbb", the answer is "abc", which the length is 3.

Given "bbbbb", the answer is "b", with the length of 1.

Given "pwwkew", the answer is "wke", with the length of 3. Note that the answer must be a substring, "pwke" is a subsequence and not a substring.
*/
static int lengthOfLongestSubstring_use_map(string s) {
	if (s.empty()) return 0;

	typedef string::size_type S;
	pair<S, S> prev = { 0,0 };
	pair<S, S> current;
	unordered_map<char, S> ascii;
	#define SUBSTR_LEN(p) (p.second-p.first)

	for (S i = 0; i < s.size(); ++i) {
		auto previous_hit = ascii.find(s[i]);
		if (previous_hit!=ascii.end() && previous_hit->second != string::npos) {
			//repeat char found! 
			if (SUBSTR_LEN(current) > SUBSTR_LEN(prev)) {
				prev = current;
			}

			//the new substr should start from the next char of the previous occurance, see test case "dvdf"
			current.first = previous_hit->second + 1;
			//and all hits before that should be cleared
#if 1
			for (auto& kv : ascii) {
				if (kv.second < previous_hit->second) kv.second = string::npos;
			}
#else
			for (S k = prev.first; k < previous_hit->second && k < current.first; ++k) ascii[s[k]] = string::npos;
#endif
			previous_hit->second = i;
			current.second = i;
		}
		else {
			ascii[s[i]] = i;
			current.second = i;
		}
	}


	S _from, _to;
	if (SUBSTR_LEN(current) > SUBSTR_LEN(prev)) {
		_from = current.first;
		_to = current.second;
	}
	else {
		_from = prev.first;
		_to = prev.second;
	}
	return (int)(_to - _from + 1);
}


static int lengthOfLongestSubstring(string s) {
	if (s.empty()) return 0;

	typedef string::size_type S;
	pair<S, S> prev;
	pair<S, S> current;
	const int SIZE = 255 + 1;
	S ascii[SIZE];
	fill(ascii, ascii + SIZE, string::npos);
	#define SUBSTR_LEN(p) (p.second-p.first)

	S n = s.size();
	for (S i = 0; i < n; ++i) {
		auto& previous_hit = ascii[s[i]];
		if (previous_hit != string::npos) {
			//repeat char found! 
			if (SUBSTR_LEN(current) > SUBSTR_LEN(prev)) {
				prev = current;
			}

			//the new substr should start from the next char of the previous occurance, see test case "dvdf"
			current.first = previous_hit + 1;
			//and all hits before that should be cleared
			replace_if(ascii, ascii + SIZE, [&previous_hit](S s) {return s < previous_hit; }, string::npos);
			previous_hit = i;
			current.second = i;
		}
		else {
			ascii[s[i]] = i;
			current.second = i;
		}
	}


	S _from, _to;
	if (SUBSTR_LEN(current) > SUBSTR_LEN(prev)) {
		_from = current.first;
		_to = current.second;
	}
	else {
		_from = prev.first;
		_to = prev.second;
	}
	return (int)(_to - _from + 1);
}

TEST_CASE("longest substring: empty string", "[leetcode]") {
	REQUIRE(lengthOfLongestSubstring("") == 0);
}

TEST_CASE("longest substring: dvdf", "[leetcode]") {
	REQUIRE(lengthOfLongestSubstring("dvdf") == 3);
}

TEST_CASE("longest substring: 1dvdfab1", "[leetcode]") {
	REQUIRE(lengthOfLongestSubstring_use_map("1dvdfab1") == 6);
}

TEST_CASE("longest substring: abcabcbb", "[leetcode]") {
	REQUIRE(lengthOfLongestSubstring("abcabcbb") == 3);
}

TEST_CASE("longest substring: aa", "[leetcode]") {
	REQUIRE(lengthOfLongestSubstring("aa") == 1);
}

TEST_CASE("longest substring: bbbbbbbbbb", "[leetcode]") {
	REQUIRE(lengthOfLongestSubstring("bbbbbbbbbb") == 1);
}

TEST_CASE("longest substring: pwwkew", "[leetcode]") {
	REQUIRE(lengthOfLongestSubstring("pwwkew") == 3);
}

TEST_CASE("longest substring: long string", "[leetcode]") {
	string file = Utils::get_data_file_path("leetcode-longest-substr.txt");
	string longstr;
    if(!Utils::load_test_data(file.c_str(), [&longstr](string& s){ longstr=s; })){
        INFO("Cannot find test file "<<file);
		INFO("To generate it run: scripts/gen_long_tring.py > " << file);
        REQUIRE(false);
    }
	else {
		static int n1, n2;
		SECTION("Use map") {
			n1=lengthOfLongestSubstring_use_map(longstr);
		}
		SECTION("Use array") {
			n2=lengthOfLongestSubstring(longstr);
		}
		SECTION("Use array == use map") {
			//map performs better : 0.023s vs 0.043s
			REQUIRE(n1==n2);
		}
	}
}

/*
https://leetcode.com/problems/longest-consecutive-sequence/#/description

Given an unsorted array of integers, find the length of the longest consecutive elements sequence.

For example,
Given [100, 4, 200, 1, 3, 2],
The longest consecutive elements sequence is [1, 2, 3, 4]. Return its length: 4.
*/
static int longestConsecutive(vector<int>& nums) {
    if (nums.empty()) return 0;
    auto cmp = greater<int>();//[](int a, int b) {return a>b; };
    make_heap(nums.begin(), nums.end(), cmp);
    int longest = 0;
    int cur = 1;
    int last = nums.front();
    pop_heap(nums.begin(), nums.end(), cmp);
    nums.pop_back();
    while (!nums.empty()) {
        int n = nums.front();
        if (n == last + 1) {
            ++cur;
        }
        else if (n != last) {
            if (cur > longest) longest = cur;
            cur = 1;
        }
        last = n;
        pop_heap(nums.begin(), nums.end(), cmp);
        nums.pop_back();
    }
    return max(cur, longest);
}

static int longestConsecutive_map(vector<int>& nums) {
    unordered_map<int,int> m;
    int ret = 0;
    for(auto & n: nums){

        //it is in the middle of some consecutive sequence OR we can say it is already visited earlier
        //therefore it does not contribute to a longer sequence
        if(m[n]) continue; 

        //we cannot find adjacent sequences to n, therefore it is a single element sequence by itself
        if(m.find(n-1) == m.end() && m.find(n+1) == m.end()){ // 
            ret = max(ret, m[n] = 1);
            continue;
        }

        //found a sequence at n+1
        //you may wonder what if the sequence at n+1 contains element n?
        //It it contains n, when we add the length by 1 using m[n+1]+1, it is wrong, right?
        //However it is not possible, because if sequence at n+1 contains n, m[n] must have been visited earlier
        //we checked that using if(m[n]) continue; here m[n] is not yet visited;
        //therefore sequence m[n+1] is always on right side, we can safely extend the length by 1
        if(m.find(n-1)==m.end()){ 
            //we want to maintain the TWO boundaries of the sequence
            //the new length of the sequence is the original length m[n+1] incremented by 1
            //left boundary m[n] = m[n+1] +1;
            //right boundary m[n+m[n+1]] = m[n+1]+1;
            //why n+m[n+1]? it is equal to m[n+1]+(n+1)-1 
            //meaning the old left boundary n+1 plus the old length m[n+1] minus 1
            //e.g. for sequence 3,4,5,6 m[3] = 4, and right boundary 6 = 3+m[3]-1 (here n+1 == 3);
            int r = m[n] = m[n+m[n+1]] = m[n+1]+1;
            ret = max(ret, r);
            continue;
        }
        
        //this is similar to the above case just extend to the right
        if(m.find(n+1)==m.end()){
            int r = m[n] = m[n-m[n-1]] = m[n-1]+1;
            ret = max(ret,r);
            continue;
        }
        
        //here, we found both sequences at n+1 and n-1, for reasons we explained,
        //the sequences have no overlap.
        //Now, we just need to add the length of current element n (which is 1) to both left and right boundaries
        //the new length will be :  
        //old length of left sequence (m[n-1]) + old length of right sequence (m[n+1]) + 1
        //We also need to mark m[n] as visited, here we can either mark it with 1 or the new length;
        int r = m[n-m[n-1]] = m[n+m[n+1]] = 1+ m[n+1]+ m[n-1];
        m[n] = 1; //basically we just need to mark m[n] as any non-zero number
        // or we can write
        //int r = m[n] = m[n-m[n-1]] = m[n+m[n+1]] = 1+ m[n+1]+ m[n-1];
        ret = max(ret,r);
    }
    return ret;    
}

TEST_CASE("longest consecutive sequence: 100,4,200,1,3,2","[leetcode]") {
    vector<int> nums = { 100,4,200,1,3,2 };
    REQUIRE(longestConsecutive(nums) == 4);
}

TEST_CASE("longest consecutive sequence: 1,2,0,1","[leetcode]") {
    vector<int> nums = { 1,2,0,1 };
    REQUIRE(longestConsecutive(nums) == 3);
}

TEST_CASE("longest consecutive sequence: long seq","[leetcode]") {
    const char f[] = "leetcode-int-seq.txt";
	string file = Utils::get_data_file_path(f);
    vector<int> nums;
    if(!Utils::load_test_data(file.c_str(), [&nums](string& s){ nums.push_back(atoi(s.c_str())); })){
        INFO("Cannot find test file "<<file);
		INFO("To generate it run: scripts/" << f <<" > " << file);
        REQUIRE(false);
    }
    else {
        static int h, m;
        SECTION("Use heap") {
            h = longestConsecutive(nums);
            LOG << "result=" << h << endl;;
        }
        SECTION("Use map") {
            m = longestConsecutive_map(nums);
            LOG << "result=" << m << endl;
        }
        /* 
        map version performs better if there's no consecutive sequence, otherwise heap version performs better
        */
        SECTION("heap == map") {
            REQUIRE(h == m);
        }
    }
}


}; //namespace LeetCode

