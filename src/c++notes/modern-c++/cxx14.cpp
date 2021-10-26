#include "catch.hpp"

namespace Cxx14Test{

/*README START
# Variable Templates

Use together with `constexpr`, we can cast variable to wanted data type at compile time.

*/

namespace{
//<<
template <typename T>
constexpr T pi = T(3.14159265358973238462643383);

static_assert( pi<int> == 3);
static_assert( pi<float> == float(3.14159265358973238462643383));
static_assert( pi<float> != double(3.14159265358973238462643383));
static_assert( pi<double> == double(3.14159265358973238462643383));
//>>
}

/* The template can be speicalized.
*/
namespace{
//<<
template <typename T> constexpr T maxValue = T(10000);

template <> constexpr double maxValue<double> = 2000;
template <> constexpr char maxValue<char> = 'Z';

static_assert( maxValue<int> == 10000);
static_assert( maxValue<double> == 2000);
static_assert( maxValue<char> == 'Z');
//>>    
}

//README END
}