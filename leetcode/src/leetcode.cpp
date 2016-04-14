#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;
class No_151 : public ::testing::Test{
private:
	/*use pointer of pointer but not pointer reference
	  so it can be used in c too.
	
	  Before the function is called:

	  pDest     => the start of current dest buffer 
	  pSrcStart => the space before a word
	  *ppSrcEnd => after a word (ususally a space)

	  After the function is called:

	  *ppDest     => after the end of word(plus a space) just copied to the destination buffer

	*/
	inline int copy_word(char** ppDest,char* pSrcStart,char* pSrcEnd) {
		char *pb = *ppDest;
		int n = pSrcEnd - (pSrcStart + 1);
		strncpy(*ppDest, pSrcStart + 1, n);
		pb += n;
		*pb++ = ' ';
		*ppDest = pb;
		return n + 1;
	}

public:
	void reverseWords(string &s) {
		char *start = const_cast<char*>(s.c_str());
		char *end = start + s.size() -1;

		for (; *start == ' '; ++start);
		for (; *end == ' '; --end);

		if (start > end) {
			s.clear();
			return;
		}

		const int size = ++end -start + 1;
		char* buf = new char [size];
		char* pb = buf;
		const char *p1 = const_cast<const char*>(start);
		char *p2 = end;
		char *p = p2;
		int actual_size = 0;

		while (p >= p1) {
			--p;
			if (*p == ' ') {
				//found a word
				actual_size += copy_word(&pb, p, p2);
				//if there are more spaces, skip them
				for (; *(p - 1) == ' '&& p > p1; --p);
				p2 = p;  //both pointed to space
			}
		}
		if (p2 != p) {
			actual_size += copy_word(&pb, p, p2);
		}
		--actual_size;
		s.replace(s.begin(), s.end(), buf, buf + actual_size);
		delete []buf;
	}


	void reverseWords2(char *s) {
		int len = strlen(s);
		char *start = s;
		char *end = start + len - 1;

		for (; *start == ' '; ++start);
		for (; *end == ' '; --end);

		if (start > end) {
			s[0] = 0;
			return;
		}

		int size = ++end - start + 1;
		char* buf = (char*) malloc(size);
		char* pb = buf;
		char *p1 = start;
		char *p2 = end;
		char *p = p2;
		int n;
		int actual_size = 0;

		while (p >= p1) {
			--p;
			if (*p == ' ') {
				//found a word
				actual_size += copy_word(&pb, p, p2);
				//if there are more spaces, skip them
				for (; *(p - 1) == ' '&& p > p1; --p);
				p2 = p;  //both pointed to space
			}
		}
		if (p2 != p) {
			actual_size += copy_word(&pb, p, p2);
		}
		strncpy(s, buf, --actual_size);
		s[actual_size] = 0;
		free(buf);
	}
};

TEST_F(No_151,the_sky_is_blue){
    std::string s1 = "the sky is blue";
    const std::string s2 = "blue is sky the";

    reverseWords(s1);
    EXPECT_EQ(s1,s2);
}

TEST_F(No_151, all_spaces) {
	std::string s1 = "  ";
	const std::string s2 = "";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(No_151, one) {
	std::string s1 = "one";
	const std::string s2 = "one";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(No_151, space_one) {
	std::string s1 = " 1";
	const std::string s2 = "1";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(No_151, a) {
	std::string s1 = "a";
	const std::string s2 = "a";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(No_151, lots_of_space_in_the_middle) {
	std::string s1 = "   a   b ";
	const std::string s2 = "b a";

	reverseWords(s1);
	EXPECT_EQ(s1, s2);
}

//c version
TEST_F(No_151, c_the_sky_is_blue) {
	char s1 [] = "the sky is blue";
	const std::string s2 = "blue is sky the";

	reverseWords2(s1);
	EXPECT_EQ(s1, s2);
}

TEST_F(No_151, c_space_one) {
	char s1 [] = " 1";
	const std::string s2 = "1";

	reverseWords2(s1);
	EXPECT_EQ(s1, s2);
}
