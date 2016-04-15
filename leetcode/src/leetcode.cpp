#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

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
