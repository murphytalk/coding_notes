Collection of my coding notes and solutions to various small problems, a reminder to myself.  

The language used is C++ (C++ 11 features used), the dependencies are:

 1. boost.
 1. [Catch](https://github.com/philsquared/Catch) unit test. See catch's docs on [command line options and arguments](https://github.com/philsquared/Catch/blob/master/docs/command-line.md).


# Problems and solutions

## [Leetcode](https://leetcode.com/)

###  [#49 Group Anagrams](https://leetcode.com/problems/anagrams/) 

Use a hash map to group Anagrams, while provides a hash code function to ensure same Anagrams will get same  hash code. The idea is that since only the character but not the ordering matters,  use the first 26 prime numbers to represent the 26 lower case character and multiplication represent the fact that only characters matter but their ordering does not.
 
###  [#151 Reverse words in a string](https://leetcode.com/problems/reverse-words-in-a-string/)  

Implemented an in-place `O(1)` space solution.


