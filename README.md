*Generated at Sun Jun  3 10:48:45 2018 by [gen_readme.py](gen_readme.py)*

[![Build Status](https://travis-ci.org/murphytalk/coding_notes.svg?branch=master)](https://travis-ci.org/murphytalk/coding_notes)

Collection of my notes on coding and solutions to various small problems, a reminder to myself.

The language used is C++ (C++ 14 features used), the dependencies are:

 1. [boost](http://www.boost.org/).
 1. [Catch](https://github.com/philsquared/Catch) unit test. See Catch's docs on [command line options and arguments](https://github.com/philsquared/Catch/blob/master/docs/command-line.md).

All examples/tests are tagged, run this to list all tags:

```
./note -t 
```
Run this to list all examples/tests:

```
./note -l
```
This will run all examples/tests whose name start with "Add" and are tagged with "leetcode". `-d y` is to show how much test is used for each test:

```
./notes 'Add*' '[leetcode]'  -d y
```

# C++ Notes
## Modern C++
### static assert

 `static_assert` is a new C++ keyword, the assertion happens at compile time, if the assertion is true the static assert expression has no effect.


```c++
namespace{
    struct MyAlignmentSensitiveStuff{
        uint8_t i;
        uint32_t d;
    };

    void do_stuff(){
        // if data type of i is changed to something else with a different size,
        // or more member is inserted in the front of it
        // the following won't compile
        static_assert(offsetof(MyAlignmentSensitiveStuff, d) == 4,
                      "Wrong offset of d in MyAlignmentSensitiveStuff");
    }
}

```
  `static_assert` can also be used to generate [meaningful compile error](src/c++notes/modern-c++/cxx11.cpp#L32) if an unmatched partial template specialization is detected.

```c++
namespace{
    template <typename T> class MyTemplatedClass{
        // called if none of the specialized version is matched
        // and the following guarantees a compiler error with user defined error message
        static_assert(sizeof(T) == 0,
                      "Not sepcialized for this data type");
    };
    template <typename T> class MyTemplatedClass<T*>{};

    MyTemplatedClass<int*> ptr;
    // uncomment the following line to get a compile error
    //MyTemplatedClass<int> not_specialized;
}

```
# [Leetcode](https://leetcode.com/)
## [Group Anagrams](src/leetcode/group_anagrams.cpp#L15)

[Leetcode #49](https://leetcode.com/problems/anagrams/).

Use a hash map to group anagrams, but instead of using a sorted string as key (so the default string hash code function would return same hash code for all subjects of the same anagram, it will work but the run time will be penalized for the sorting of every key), the solution implments a customized hash code function which uses the first 26 prime numbers to represent the 26 lower case letters and calculates the hash code by mulitplying them, to reflect the fact that 1) we only care about different letters 2) ordering does not matter.

Beats 99.75% of C++ submissions.

 ![Screenshot](img/leetcode/Anagrams.PNG)

## [Reverse words in a string](src/leetcode/reverse_words.cpp#L12)

[Leetcode #151](https://leetcode.com/problems/reverse-words-in-a-string/)

An in-place `O(1)` space solution, beats 94.29% of C++ submissions.

 ![Screenshot](img/leetcode/RevserseWordsInString.PNG)
