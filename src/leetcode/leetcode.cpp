#include "catch.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

namespace LeetCode{
    
/*
  https://leetcode.com/problems/anagrams/

  Given an array of strings, group anagrams together.

  For example, given: ["eat", "tea", "tan", "ate", "nat", "bat"],
  Return:
    [
        ["ate", "eat","tea"],
        ["nat","tan"],
        ["bat"]
    ]

  Note:

    - For the return value, each inner list's elements must follow the lexicographic order.
    - All inputs will be in lower-case.
*/
class GroupAnagrams{
private:
    inline static int char_val(const char& c) {
        return c - 'a';
    }
    inline static size_t hash(const string & key) {
        auto n = key.size();
        if (n == 0) return (size_t) 0;
        size_t code = 1;
        //first 26 prime numbers
        static int primes [] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101 };
        for(char c:key){
            code *= primes[char_val(c)];
        }
        return code;
    }
public:
    inline static bool eq(const string& x, const string& y) {
        if (x.size() != y.size()) {
            return false;
        }
        else {
            return hash(x)==hash(y);
        }
    }
    
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        typedef vector<string> V;
        auto _hash = [](const string & key) {
            return hash(key);
        };
        auto _eq = [](const string& x, const string& y) {
            return eq(x, y);
        };
        sort(strs.begin(), strs.end());
        unordered_map < string, int, decltype(_hash), decltype(_eq)> index(1024,_hash,_eq);
        vector<V> r;
        V item;
        for (const auto& s : strs) {
            string k = s;
            auto pos = index.find(k);
            if (pos == index.end()) {
                r.push_back(item);
                r.back().push_back(s);
                index.insert(make_pair(k, r.size()-1));
            }
            else {
                V& v = r[pos->second];
                v.push_back(s);
            }
        }

        //for (auto& i : r) {
        //    sort(i.begin(), i.end());
        //}
        return r;
    }
};

TEST_CASE("Group anagrams: customized equal","[leetcode]") {
	{
		string s1 = "eat";
		string s2 = "tea";
		string s3 = "ate";
		string s4 = "tan";
		REQUIRE(GroupAnagrams::eq(s1,s2));
		REQUIRE(GroupAnagrams::eq(s1, s3));
		REQUIRE(GroupAnagrams::eq(s2, s3));
		REQUIRE_FALSE(GroupAnagrams::eq(s1, s4));
	}
    {
        string s1 = "duh";
        string s2 = "ill";
        REQUIRE_FALSE(GroupAnagrams::eq(s1, s2));
    }
}


TEST_CASE("Group anagrams: test case 1","[leetcode]") {
    vector<string> s = { "eat", "tea", "tan", "ate", "nat", "bat" };
    GroupAnagrams t;
    auto r = t.groupAnagrams(s);
    
    set<vector<string>> expected = {
        { "ate", "eat", "tea" },
        { "nat","tan" },
        { "bat" }
    };

    set<vector<string>> result;
    copy(r.begin(), r.end(), inserter(result, result.begin()));
    REQUIRE(result == expected);
}

TEST_CASE("Group anagrams: duplicated words test case","[leetcode]") {
    vector<string> s = {
        "hos", "boo", "nay", "deb", "wow", "bop", "bob", "brr", "hey", "rye", "eve", "elf", "pup", "bum", "iva", "lyx",
        "yap", "ugh", "hem", "rod", "aha", "nam", "gap", "yea", "doc", "pen", "job", "dis", "max", "oho", "jed", "lye", 
        "ram", "pup", "qua", "ugh", "mir", "nap", "deb", "hog", "let", "gym", "bye", "lon", "aft", "eel", "sol", "jab"
    };

    set<vector<string>> expected = { 
        { "sol" },{ "wow" },{ "gap" },{ "hem" },{ "yap" },{ "bum" },{ "ugh","ugh" },{ "aha" },{ "jab" },{ "eve" },{ "bop" },
        { "lyx" },{ "jed" },{ "iva" },{ "rod" },{ "boo" },{ "brr" },{ "hog" },{ "nay" },{ "mir" },{ "deb","deb" },{ "aft" },
        { "dis" },{ "yea" },{ "hos" },{ "rye" },{ "hey" },{ "doc" },{ "bob" },{ "eel" },{ "pen" },{ "job" },{ "max" },{ "oho" },
        { "lye" },{ "ram" },{ "nap" },{ "elf" },{ "qua" },{ "pup","pup" },{ "let" },{ "gym" },{ "nam" },{ "bye" },{ "lon" } 
    };

    GroupAnagrams t;
    auto r = t.groupAnagrams(s);
    set<vector<string>> result;
    copy(r.begin(), r.end(), inserter(result, result.begin()));
    REQUIRE(result == expected);
}


TEST_CASE("Group anagrams: all single words test case","[leetcode]") {
    vector<string> s = {"cab", "tin", "pew", "duh", "may", "ill", "buy", "bar", "max", "doc"};
    GroupAnagrams t;
    auto r = t.groupAnagrams(s);

    set<vector<string>> expected = { 
        { "cab" },{ "tin" },{ "pew" },{ "duh" },{ "may" },{ "ill" },
        { "buy" },{ "bar" },{ "max" },{ "doc" } 
    };
    set<vector<string>> result;
    copy(r.begin(), r.end(), inserter(result, result.begin()));
    REQUIRE(result == expected);
}


TEST_CASE("Group anagrams: empty input test case","[leetcode]") {
    vector<string> s = { "" };
    GroupAnagrams t;
    auto r = t.groupAnagrams(s);
    REQUIRE(r.size() == 1);
}


/*
   https://leetcode.com/problems/reverse-words-in-a-string/

   Given an input string, reverse the string word by word.

   For example,
   Given s = "the sky is blue",
   return "blue is sky the".

   Clarification:

   - What constitutes a word?
     A sequence of non-space characters constitutes a word.

   - Could the input string contain leading or trailing spaces?
     Yes. However, your reversed string should not contain leading or trailing spaces.

   - How about multiple spaces between two words?
     Reduce them to a single space in the reversed string.
*/
class ReverseWordsInString{
private:
    inline void reverseWord(char *s, int len) {
        char c;
        int i, j;
        for (i = 0, j = len - 1; i<j; ++i, --j) {
            c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
    }

    /* an in-place O(1) space solution:
     * 
     * Example : the sky is blue
     * 1) reverse the whole string :
     *    eulb si yks eht
     * 2) move from the start of the string
     *    2-1) remember start of a word
     *    2-2) find the end of the word
     *    2-3) reverse the word
     * 3) repeate 2) until the end of the string
     * 
     * also handles extra spaces
    */
    int _reverseWords(char *s) {
        int len = strlen(s);
        char *start = s;
        char *end = start + len - 1;

        for (; *start == ' '; ++start);
        for (; *end == ' '; --end);

        if (start > end) {
            s[0] = 0;
            return 0;
        }

        int size = ++end - start;

        reverseWord(start, size);
        char *p1 = start, *p2 = start;
        char *dest = s;
        size = 0;
        int n = 0;
        while (p2 <= end) {
            ++p2;
            if (*p2 == ' ' || p2 >= end) {
                //p1=>p2-1 is a reversed word
                n = p2 - p1;
                reverseWord(p1, n);
                ++n; //space
                size += n;
                strncpy(dest, p1, n);
                dest += n;

                //accept one space
                ++p2;
                //if there are more spaces, skip them
                for (; *p2 == ' ' && p2 <= end; ++p2);
                p1 = p2;
            }
        }

        s[--size] = 0;
        return size;
    }
public:
    void reverseWords(string &s) {
        char *p = const_cast<char*>(s.c_str());
        s.resize(_reverseWords(p));
    }
};

TEST_CASE("Reverse words in string: test cases","[leetcode]") {
    ReverseWordsInString t;

    SECTION("this sky is blue"){
        std::string s1 = "the sky is blue";
        const std::string s2 = "blue is sky the";
        
        t.reverseWords(s1);
        REQUIRE(s1 == s2);
    }

    SECTION("all spaces"){
        std::string s1 = "  ";
        const std::string s2 = "";

        t.reverseWords(s1);
        REQUIRE(s1 == s2);
    }

    SECTION("one"){
        std::string s1 = "one";
        const std::string s2 = "one";

        t.reverseWords(s1);
        REQUIRE(s1 == s2);
    }

    SECTION("space one") {
        std::string s1 = " 1";
        const std::string s2 = "1";

        t.reverseWords(s1);
        REQUIRE(s1 == s2);
    }

    SECTION("a") {
        std::string s1 = "a";
        const std::string s2 = "a";

        t.reverseWords(s1);
        REQUIRE(s1 == s2);
    }

    SECTION("lots of space in the_middle") {
        std::string s1 = "   a   b ";
        const std::string s2 = "b a";
        
        t.reverseWords(s1);
        REQUIRE(s1 == s2);
    }
}

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
			range.push_back(low - nums.begin());
			auto up = upper_bound(low, nums.end(), target);
			range.push_back(up - nums.begin() - 1);
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

}; //namespace LeetCode
