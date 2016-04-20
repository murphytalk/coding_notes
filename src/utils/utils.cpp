#include "utils.h"
namespace Utils{

CMP_INT_FUNC cmp_int = [](int x,int y){return x<y;};


uint32_t nChooseK( uint32_t n, uint32_t k )
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( uint32_t i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

//https://class.coursera.org/algo2-002/forum/thread?thread_id=350#comment-1330
//http://en.wikipedia.org/wiki/Combinatorial_number_system
uint32_t comb_index(const uint32_t comb)
{
	uint32_t value = 0;
	uint32_t k,n;
	for(n= 0,k=1;n<sizeof(uint32_t)*8;++n){
		if((comb>>n)&0x1){
			value+=nChooseK(n,k);
			++k;
		}
	}
	return value;
}

std::string get_data_dir(){
    return "";
}

}
