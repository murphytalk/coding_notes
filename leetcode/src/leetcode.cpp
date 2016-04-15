#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

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
class GroupAnagrams : public ::testing::Test {
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

		for (auto& i : r) {
			sort(i.begin(), i.end());
		}
		return r;
	}
};

TEST_F(GroupAnagrams, eq1) {
	string s1 = "eat";
	string s2 = "tea";
	string s3 = "ate";
	string s4 = "tan";
	ASSERT_TRUE(eq(s1,s2));
	ASSERT_TRUE(eq(s1, s3));
	ASSERT_TRUE(eq(s2, s3));
	ASSERT_FALSE(eq(s1, s4));
}

TEST_F(GroupAnagrams, eq2) {
	string s1 = "duh";
	string s2 = "ill";
	ASSERT_FALSE(eq(s1, s2));
}


TEST_F(GroupAnagrams, test1) {
	vector<string> s = { "eat", "tea", "tan", "ate", "nat", "bat" };
	auto r = groupAnagrams(s);
	
	set<vector<string>> expected = {
		{ "ate", "eat", "tea" },
		{ "nat","tan" },
		{ "bat" }
	};

	set<vector<string>> result;
	copy(r.begin(), r.end(), inserter(result, result.begin()));
	ASSERT_EQ(result, expected);
}

TEST_F(GroupAnagrams, duplicated) {
	vector<string> s = {
		"hos", "boo", "nay", "deb", "wow", "bop", "bob", "brr", "hey", "rye", "eve", "elf", "pup", "bum", "iva", "lyx",
		"yap", "ugh", "hem", "rod", "aha", "nam", "gap", "yea", "doc", "pen", "job", "dis", "max", "oho", "jed", "lye", 
		"ram", "pup", "qua", "ugh", "mir", "nap", "deb", "hog", "let", "gym", "bye", "lon", "aft", "eel", "sol", "jab"
	};

	set<vector<string>> expected = { 
		{ "sol" },{ "wow" },{ "gap" },{ "hem" },{ "yap" },{ "bum" },{ "ugh","ugh" },{ "aha" },{ "jab" },{ "eve" },{ "bop" },{ "lyx" },{ "jed" },{ "iva" },{ "rod" },{ "boo" },{ "brr" },{ "hog" },{ "nay" },{ "mir" },{ "deb","deb" },{ "aft" },{ "dis" },{ "yea" },{ "hos" },{ "rye" },{ "hey" },{ "doc" },{ "bob" },{ "eel" },{ "pen" },{ "job" },{ "max" },{ "oho" },{ "lye" },{ "ram" },{ "nap" },{ "elf" },{ "qua" },{ "pup","pup" },{ "let" },{ "gym" },{ "nam" },{ "bye" },{ "lon" } 
	};

	auto r = groupAnagrams(s);

	set<vector<string>> result;
	copy(r.begin(), r.end(), inserter(result, result.begin()));
	ASSERT_EQ(result, expected);
}


TEST_F(GroupAnagrams, all_single) {
	vector<string> s = {"cab", "tin", "pew", "duh", "may", "ill", "buy", "bar", "max", "doc"};
	auto r = groupAnagrams(s);

	set<vector<string>> expected = { { "cab" },{ "tin" },{ "pew" },{ "duh" },{ "may" },{ "ill" },{ "buy" },{ "bar" },{ "max" },{ "doc" } };
	set<vector<string>> result;
	copy(r.begin(), r.end(), inserter(result, result.begin()));
	ASSERT_EQ(result, expected);
}


TEST_F(GroupAnagrams, empty) {
	vector<string> s = { "" };
	auto r = groupAnagrams(s);
	ASSERT_EQ(r.size(),1);
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
class ReverseWordsInString : public ::testing::Test{
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

	/* an in-place O(1) space solution
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

TEST_F(ReverseWordsInString,the_sky_is_blue){
    std::string s1 = "the sky is blue";
    const std::string s2 = "blue is sky the";

    reverseWords(s1);
    EXPECT_EQ(s1,s2);
}

TEST_F(ReverseWordsInString, all_spaces) {
	std::string s1 = "  ";
	const std::string s2 = "";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(ReverseWordsInString, one) {
	std::string s1 = "one";
	const std::string s2 = "one";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(ReverseWordsInString, space_one) {
	std::string s1 = " 1";
	const std::string s2 = "1";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(ReverseWordsInString, a) {
	std::string s1 = "a";
	const std::string s2 = "a";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(ReverseWordsInString, lots_of_space_in_the_middle) {
	std::string s1 = "   a   b ";
	const std::string s2 = "b a";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

