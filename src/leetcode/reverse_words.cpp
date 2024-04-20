#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <algorithm>

using namespace std;

namespace LeetCode{
/*README START
# <<Reverse words in a string>>

[Leetcode #151](https://leetcode.com/problems/reverse-words-in-a-string/)

An in-place `O(1)` space solution, beats 94.29% of C++ submissions.

 ![Screenshot](img/leetcode/RevserseWordsInString.PNG)
README END

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
    inline void reverseWord(char *s, size_t len) {
        char c;
        size_t i, j;
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
    size_t _reverseWords(char *s) {
        auto len = strlen(s);
        char *start = s;
        char *end = start + len - 1;

        for (; *start == ' '; ++start);
        for (; *end == ' '; --end);

        if (start > end) {
            s[0] = 0;
            return 0;
        }

        auto size = ++end - start;

        reverseWord(start, size);
        char *p1 = start, *p2 = start;
        char *dest = s;
        size = 0;
        size_t n = 0;
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
}
