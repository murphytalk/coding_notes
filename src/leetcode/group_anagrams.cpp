#include "catch.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <algorithm>

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
            auto pos = index.find(s);
            if (pos == index.end()) {
                r.push_back(item);
                r.back().push_back(s);
                index.insert(make_pair(s, (int)r.size()-1));
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

    
}
