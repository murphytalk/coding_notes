#include "catch.hpp"
#include <memory>
#include <iterator>
#include <algorithm>
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
        strcpy(_data.get(), src);
    }

    void swap(string& other) {
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
        _data.swap(other._data);
    }
public:
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
    string& operator = (string& other) {
        //the orgininal resource will go down along with temp after the swap, no need to additionally check for self assigment
        string temp(other);
        swap(temp);
        return *this;
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
        strcpy(temp._data.get(), lhs.c_str());
        strcat(temp._data.get(), rhs.c_str());
        return temp;
    }
};

TEST_CASE("string: basic operations", "[homemade]") {
    const char source[] = "123456790";
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
        REQUIRE(s.size() == s2.size());
        REQUIRE(s.capacity() == s2.capacity());
        REQUIRE(strcmp(s.c_str(), s2.c_str()) == 0);
    }
    SECTION("Copy self assignment") {
        s = s;
        REQUIRE(s.size() == strlen(source));
        REQUIRE(strcmp(s.c_str(), source) == 0);
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
        for (size_t i = 0; i < strlen(source); ++i)
            REQUIRE(s[i] == source[i]);
    }
}

TEST_CASE("string: concat", "[homemade]") {
    const char source[] = "123456790";
    string s(source);

    #define DEF_EXPECTED std::unique_ptr<char[]> expected (new char[(strlen(source) << 1)+1])

    SECTION("string + string") {
        DEF_EXPECTED;
        strcpy(expected.get(), source);
        strcat(expected.get(), source);
        REQUIRE(strcmp((s + s).c_str(),expected.get()) == 0);
    }
    SECTION("string + const char") {
        DEF_EXPECTED;
        strcpy(expected.get(), source);
        strcat(expected.get(), source);
        REQUIRE(strcmp((s + source).c_str(),expected.get()) == 0);
    }
    SECTION("const char + string") {
        DEF_EXPECTED;
        strcpy(expected.get(), source);
        strcat(expected.get(), source);
        REQUIRE(strcmp((source + s).c_str(),expected.get()) == 0);
    }
    SECTION("string + char") {
        DEF_EXPECTED;
        strcpy(expected.get(), source);
        strcat(expected.get(), source);
        char *p = const_cast<char*>(source);
        REQUIRE(strcmp((s + p).c_str(),expected.get()) == 0);
    }
}

TEST_CASE("string: iterator", "[homemade]") {
    const char source[] = "123456790";
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
TEST_CASE("string: use with algorith", "[homemade]") {
    char source[] = "123456789abc";
    string s(source);

    SECTION("sort") {
        auto cmp = [](const char&l, const char&r) {return l > r; };
        std::sort(source,source+strlen(source),cmp);
        std::sort(s.begin(), s.end(),cmp);
        REQUIRE(strcmp(source, s.c_str()) == 0);
    }

    SECTION("heap") {
        std::make_heap(s.begin(), s.end());
        REQUIRE(s.front()=='c');
        std::pop_heap(s.begin(),s.end());
        REQUIRE(s.front() == 'b');
        REQUIRE(s.back() == 'c');
    }
}
}
