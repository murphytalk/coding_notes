Collection of my notes on coding and solutions to various small problems, a reminder to myself.

The language used is C++ (C++ 11 features used), the dependencies are:

 1. [boost](http://www.boost.org/).
 1. [Catch](https://github.com/philsquared/Catch) unit test. See Catch's docs on [command line options and arguments](https://github.com/philsquared/Catch/blob/master/docs/command-line.md).


# Problems and solutions

## [Leetcode](https://leetcode.com/)

###  [#49 Group Anagrams](https://leetcode.com/problems/anagrams/) 

Use a hash map to group anagrams, but instead of using a sorted string as key (so the default string hash code function would return same hash code for all subjects of the same anagram, it will work but the run time will be penalized for the sorting of every key), the solution implments a customized hash code function which uses the first 26 prime numbers to represent the 26 lower case letters and calculates the hash code by mulitplying them, to reflect the fact that 1) we only care about different letters 2) ordering does not matter.
 
###  [#151 Reverse words in a string](https://leetcode.com/problems/reverse-words-in-a-string/)

An in-place `O(1)` space solution.

 ![Screenshot](img/leetcode/RevserseWordsInString.PNG)
 


