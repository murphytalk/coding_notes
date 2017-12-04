#pragma once
#include <cstring>
#include <cstdint>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#if 0
#include <boost/chrono/include.hpp>
#define LOG std::cout<<boost::chrono::system_clock::now()
#else
#include <chrono>
#include "../utils/date.h"
#define LOG std::cout<<date::format("%F %T ", std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()))
#endif
namespace Utils{


typedef bool (*CMP_INT_FUNC)(int,int);
extern CMP_INT_FUNC cmp_int;

/*
  Very similar to std::bitset 

  A buffer whose bit can be set or checked by the bit index. 
  Computational complexity : O(n) = c
  Space complexity : O(n) = n 

  Prerequisites: the maximum number is known.
  
  Use Case 1
  ----------
  Sort integer values: 
    1 - Call set() to set the bit whose index eqauls to the integer value
    2 - Iterate from index 0, picks up bit index where the bit is set. The picked up bit indexes will be the original integer values in ascending order.

  Use Case 2
  ----------
  Use as a unique set of integer values
 */
template<typename T>
class basic_bitset{
private:
    typedef unsigned char BITMAP_DATA_TYPE;
	static const uint8_t N = sizeof(BITMAP_DATA_TYPE) * 8;
	BITMAP_DATA_TYPE* bitmap;
	T bit_size;
	T byte_size;
public:
	basic_bitset(T bits, bool clear_buffer = true){
		bit_size = bits;
        byte_size = 1 + (bits/N);
        bitmap = new BITMAP_DATA_TYPE[byte_size];
		if(clear_buffer){
			clear();
		}
	}

	basic_bitset(const std::string& str) : basic_bitset((T)str.size()) {
		T i = 0;		
		for (auto iter = str.rbegin();iter != str.rend();++iter,++i) {
			if (*iter == '1') set(i);
		}
	}

    ~basic_bitset(){
        delete []bitmap;
    }


	void to_str(std::string& str) {
		T actual_bit_size;
		if (check(bit_size - 1)) {
			actual_bit_size = bit_size;
		}
		else{
			//the left most bit is 0,
			//we should skip it to avoid output a leading zero
			actual_bit_size = bit_size - 1;
		}
		
		str.resize(actual_bit_size,'0');
		int k;
		for (T i = 0;i < actual_bit_size ; ++i) {
			k = actual_bit_size - i - 1 ;
			if(check(k)) str[i] =  '1';
		}
	}

	inline std::string to_str() {
		std::string s;
		to_str(s);
		return s;
	}
	
	void clear(){
        memset(bitmap,0,byte_size);
    }

	void set(T index) {
		if (index<0 || index>=bit_size) return;

		T b, bit;

		b = index / N;
		bit = index%N;

		bitmap[b] |= 1 << bit;
	}

	inline void set(T byteIndex, uint8_t value) {
		bitmap[byteIndex] = value;
	}

    void clear(T index){
		if (index<0 || index>=bit_size) return;

		T b, bit;

		b = index / N;
		bit = index%N;

		bitmap[b] &= ~(1 << bit);
	}

    bool check(T index){
		if (index<0 || index>=bit_size) return false;
		
		T b,bit;

        b   = index/N;
        bit = index%N;

        return (bitmap[b]>>bit)&0x01;
    }
};

typedef basic_bitset<uint32_t> bitset;

/* Gosper's hack - #175 in HAKMEM
*/
template<typename T>
class GosperHack{
private:
	T k,n;
	T limit,set;
public:
	GosperHack(T kk, T nn) : k((T) kk), n((T) nn), limit((1 << n)), set(0){}
	T next(){
		if(set == 0){
			set = (1 << k) - 1;
		}
		else{
			T c = set & -set;
			T r = set + c;
			set = (((r^set) >> 2) / c) | r;
		}
		return (set < limit)?set:0;
	}
};

uint32_t nChooseK(uint32_t n,uint32_t k);
uint32_t comb_index(const uint32_t comb);

/* 
   the data directory is data under this project's root directory 
*/
std::string get_data_file_path(const char* filename);

template<typename LINE>
bool load_test_data(char const *file_name,LINE line_callback,const char comment = '#'){    
    std::ifstream infile(file_name);
    if(!infile.is_open())  return false;

    std::string line;
    while (std::getline(infile, line)){
        if(line[0] != comment){
            line_callback(line);
        }
    }
    
    return true;
}

std::string normalize_path(const std::string what);

}

