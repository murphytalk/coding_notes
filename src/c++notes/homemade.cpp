#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <memory>
#include <iterator>
#include <utility>
#include <algorithm>
#include "src/compatible.h"

/* Various home made implementations of STL classes
   This is purely for exercise.
*/
namespace CxxHomemade {

//only char version, no wchar
class string{
    size_t _size;
    size_t _capacity;
    std::unique_ptr<char[]> _data;

    void copy(const char* src) {
        const size_t len = strlen(src);
        const size_t capacity = len<<1; //double the capacity
        if (_capacity < capacity) {
            _capacity = capacity;
        }
        _data.reset(new char[_capacity]);
        _size = len;
        STRCPY(_data.get(), _capacity, src);
    }

    void swap(string& other) {
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
        _data.swap(other._data);
    }
public:
    const static size_t npos = std::numeric_limits<size_t>::max();
    using size_type = size_t;

    string(const char* str = nullptr) {
        _capacity = 0;
        if (str) {
            copy(str);
        }
        else {
            _size = 0;
        }
    }
    string(const string& other) {
        _capacity = 0;
        copy(other.c_str());
    }

    //Copy-and-swap idiom
    //unifying assignment , see https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap
    //section "copy elision and copy-and-swap idiom"
    string& operator = (string temp) {
        //the by-value parameter serves as the temp obj
        //the orgininal resource will go down along with temp after the swap, no need to additionally check for self assigment
        swap(temp);
        return *this;
    }
    string(string&& other) {
        _size = std::move(other._size);
        _capacity = std::move (other._capacity);
        _data = std::move( other._data);
        other._size = other._capacity = 0;
    }
    //http://www.cplusplus.com/reference/iterator/
    //we need a random access iterator
    //http://en.cppreference.com/w/cpp/concept/RandomAccessIterator
    class iterator {
        string* _str;
        char* _current;
    public:
        using value_type = char;
        using reference = char&;
        using iterator_category = std::random_access_iterator_tag;
        using pointer = char*;
        using difference_type = ptrdiff_t;


        iterator(string* str) :_str(str),_current(str==nullptr?nullptr:const_cast<char*>(str->c_str())){}
        iterator(string* str,char *current) :_str(str),_current(current){}

        //===================================================
        //operators to satisfy InputIterator
        friend bool operator == (const iterator& lhs, const iterator& rhs) { return lhs._current== rhs._current; }
        friend bool operator != (const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
        reference operator *() {return *_current;}
        iterator& operator ++() {
            ++_current;
            return *this;
        }
        //===================================================
        //operators to satisfy ForwardIterator

        //postfix ++
        iterator operator ++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }
        //===================================================
        //operators to satisfy BidrectionalIterator
        iterator& operator --() {
            --_current;
            return *this;
        }
        //postfix --
        iterator operator --(int) {
            auto temp = *this;
            --*this;
            return temp;
        }
        //===================================================
        //operators to satisfy RandomAccessIterator
        iterator& operator += (difference_type diff) {
            _current += diff;
            return *this;
        }
        friend iterator operator + (const iterator& lhs, difference_type diff){
            auto temp = lhs;
            temp += diff;
            return temp;
        }
        iterator& operator - (difference_type diff) {
            _current -= diff;
            return *this;
        }
        friend iterator operator -= (const iterator& lhs,difference_type diff) {
            auto temp = lhs;
            temp -= diff;
            return temp;
        }
        friend difference_type operator-(const iterator&lhs,const iterator& rhs) {
            return lhs._current - rhs._current;
        }
        reference operator[](size_t pos) const { return *(*this+pos); }
        friend bool operator <(const iterator& lhs, const iterator& rhs) {
            return lhs._current < rhs._current;
        }
        friend bool operator >(const iterator& lhs, const iterator& rhs) {
            return !(lhs > rhs);
        }
    };

    iterator begin() { return iterator(this); }
    iterator end() { return iterator(this,const_cast<char*>(c_str())+_size);}

    const char* c_str() const { return _data.get(); }
    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }
    char& operator[](size_t pos) { return _data[pos]; }
    char& front() { return _data[0]; }
    const char& front() const { return _data[0]; }
    char& back() { return _data[_size-1]; }
    const char& back() const{ return _data[_size-1]; }

    friend string operator+(const string& lhs,const string& rhs) {
        string temp;
        temp._capacity = lhs.size() + rhs.size() + 1;
        temp._size = temp._capacity;
        temp._data.reset(new char[temp._capacity]);
        STRCPY(temp._data.get(), temp._capacity,lhs.c_str());
        STRCAT(temp._data.get(), temp._capacity,rhs.c_str());
        return temp;
    }

    size_type find(const char* s, size_type pos = 0) const {
        auto p = strstr(_data.get()+pos, s);
        return p == nullptr? npos:  p - c_str();
    }

    friend bool operator==(const string& lhs, const string& rhs) {
        if (&lhs == &rhs) return true;
        if (lhs._size != rhs._size) return false;
        if (lhs._data.get() == rhs._data.get()) return true;
        return strcmp(lhs._data.get(), rhs._data.get()) == 0;
    }

    friend bool operator!=(const string& lhs, const string& rhs) {
        return !(lhs == rhs);
    }
};

//VC++ let us get away with not defining this
const size_t string::npos;

TEST_CASE("string: basic operations", "[homemade][c++idioms]") {
    const char source[] = "1234567890";
    string s(source);

    SECTION("Copy constructor") {
        string s2(s);
        REQUIRE(s.size() == s2.size());
        REQUIRE(s.capacity() == s2.capacity());
        REQUIRE(strcmp(s.c_str(), s2.c_str()) == 0);
    }
    SECTION("Copy assignment"){
        string s2;
        s2 = s;
        REQUIRE(strcmp(s.c_str(), source) == 0); //source preserves
        REQUIRE(s.size() == s2.size());
        REQUIRE(s.capacity() == s2.capacity());
        REQUIRE(strcmp(s.c_str(), s2.c_str()) == 0);
    }
    SECTION("Copy self assignment") {
        s = s;
        REQUIRE(s.size() == strlen(source));
        REQUIRE(strcmp(s.c_str(), source) == 0);
    }
    SECTION("Move assignment") {
        auto s2 = string(source);
        auto s3 = std::move(s2);
        REQUIRE(strcmp(s3.c_str(),source)==0);
        REQUIRE(s2.size() == 0);
        REQUIRE(s2.capacity() == 0);
        REQUIRE(s2.c_str() == nullptr);
    }
    SECTION("c_str()") {
        REQUIRE(strcmp(s.c_str(), source) == 0);
    }
    SECTION("size()") {
        REQUIRE(s.size() == strlen(source));
    }
    SECTION("front()") {
        REQUIRE(s.front() == '1');
    }
    SECTION("back()") {
        REQUIRE(s.back() == '0');
    }
    SECTION("empty()") {
        string em;
        REQUIRE(em.empty());
        REQUIRE(!s.empty());
    }
    SECTION("[]") {
        for (size_t i = 0; i < sizeof(source); ++i)
            REQUIRE(s[i] == source[i]);
    }
    SECTION("string == string") {
        const string s2(source);
        REQUIRE(s == s2);
    }
    SECTION("self == self") {
        REQUIRE(s == s);
    }
    SECTION("string != string") {
        const string s2("what is this?");
        REQUIRE(s != s2);
    }
}

TEST_CASE("string: concat", "[homemade]") {
    const char source[] = "1234567890";
    string s(source);

    const size_t  BUFSIZE = (sizeof(source) << 1) + 1;
    char expected[BUFSIZE];

    SECTION("string + string") {
        STRCPY(expected, BUFSIZE,source);
        STRCAT(expected, BUFSIZE, source);
        REQUIRE(strcmp((s+s).c_str(),expected) == 0);
    }
    SECTION("string + const char") {
        STRCPY(expected, BUFSIZE,source);
        STRCAT(expected, BUFSIZE,source);
        REQUIRE(strcmp((s + source).c_str(),expected) == 0);
    }
    SECTION("const char + string") {
        STRCPY(expected, BUFSIZE,source);
        STRCAT(expected, BUFSIZE,source);
        REQUIRE(strcmp((source + s).c_str(),expected) == 0);
    }
    SECTION("string + char") {
        STRCPY(expected, BUFSIZE,source);
        STRCAT(expected, BUFSIZE,source);
        char *p = const_cast<char*>(source);
        REQUIRE(strcmp((s + p).c_str(),expected) == 0);
    }
}

TEST_CASE("string: iterator", "[homemade]") {
    const char source[] = "1234567890";
    string s(source);
    auto begin  = s.begin();
    auto end = s.end();

    SECTION("iterate via for-range") {
        int i = 0;
        for (char c : s) {
            REQUIRE(c == source[i++]);
        }
    }
    SECTION("i+n") {
        std::iterator_traits<string::iterator>::difference_type diff = 2;
        REQUIRE(*(begin + diff) == '3');
    }
    SECTION("++i"){
        REQUIRE(*(++begin) == '2');
    }
    SECTION("i++"){
        begin++;
        REQUIRE(*begin == '2');
    }
    SECTION("i-n") {
        std::iterator_traits<string::iterator>::difference_type diff = 2;
        REQUIRE(*(end - diff) == '9');
    }
    SECTION("--i"){
        REQUIRE(*(--end) == '0');
    }
    SECTION("i--"){
        end--;
        REQUIRE(*end == '0');
    }
    SECTION("derenference") {
        const char x = 'x';
        *begin = x;
        REQUIRE(s[0] == x);
    }
}
TEST_CASE("string: use with algorithm", "[homemade]") {
    char source[] = "123456789abc";
    string s(source);
#if 0
    SECTION("sort") {
        auto cmp = [](const char&l, const char&r) {return l > r; };
        std::sort(source,source+strlen(source),cmp);
        std::sort(s.begin(), s.end(),cmp);
        REQUIRE(strcmp(source, s.c_str()) == 0);
    }
#endif
    SECTION("heap") {
        std::make_heap(s.begin(), s.end());
        REQUIRE(s.front()=='c');
        std::pop_heap(s.begin(),s.end());
        REQUIRE(s.front() == 'b');
        REQUIRE(s.back() == 'c');
    }
}
TEST_CASE("string: substring", "[homemade]") {
    char source[] = "123456789abc";
    string s(source);

    SECTION("find") {
        REQUIRE(s.find("abc") == 9);
    }
    SECTION("find - no such substr") {
        REQUIRE(s.find("ABC") == string::npos);
    }
    SECTION("find from middle") {
        REQUIRE(s.find("abc",9) == 9);
    }
    SECTION("find from middle - overshot") {
        REQUIRE(s.find("abc",10) == string::npos);
    }
}

}
