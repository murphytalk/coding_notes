#pragma once
#include <cstring>
#include <cstdint>
#include <string>
#include <algorithm>
namespace Utils{

typedef bool (*CMP_INT_FUNC)(int,int);
extern CMP_INT_FUNC cmp_int;

/*
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
	BITMAP_DATA_TYPE* bitmap;
	T bit_size;
	T byte_size;
public:
	basic_bitset(T bits, bool clear_buffer = true){
		bit_size = bits;
        byte_size = 1 + (bits/sizeof(BITMAP_DATA_TYPE));
        bitmap = new BITMAP_DATA_TYPE[byte_size];
		if(clear_buffer){
			clear();
		}
	}


	basic_bitset(const std::string& str) : basic_bitset(str.size()) {
		T i = 0;
		for (auto iter = str.rbegin();iter != str.rend();++iter,++i) {
			if(*iter=='1') set(i);
		}
	}

    ~basic_bitset(){
        delete []bitmap;
    }


	void to_str(std::string& str) {
		str.resize(bit_size);
		//for (T i = bit_size-1; i >=0; --i)
		int k;
		for (T i = 0;i < bit_size; ++i) {
			k = bit_size - i - 1;
			str[i] = check(k) ? '1' : '0';
		}
	}
	
	void clear(){
        memset(bitmap,0,byte_size);
    }

    void set(T v){
		if (v<0 || v>bit_size) return;

        T b,bit;

        b   = v/sizeof(BITMAP_DATA_TYPE);
        bit = v%sizeof(BITMAP_DATA_TYPE);

        bitmap[b]|=1<<bit;
    }

    bool check(T v){
		if (v<0 || v>bit_size) return false;
		
		T b,bit;

        b   = v/sizeof(BITMAP_DATA_TYPE);
        bit = v%sizeof(BITMAP_DATA_TYPE);

        return (bitmap[b]>>bit)&0x01;
    }
};

typedef basic_bitset<uint32_t> bitset;

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
}

