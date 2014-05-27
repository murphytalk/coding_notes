cmake_minimum_required(VERSION 2.8)

set(TARGET cpp_notes)

project(${TARGET} CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()


include_directories(/usr/local/include)

if(APPLE)
  add_definitions(-std=c++11 -stdlib=libc++ -O3 -DGTEST_USE_OWN_TR1_TUPLE=1)
else(APPLE)
  add_definitions(-std=c++11 -DGTEST_USE_OWN_TR1_TUPLE=1)

  set(CXX_FLAGS
    -g
    # -DVALGRIND
    # -DMUDUO_STD_STRING
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    #-Wconversion
    -Wno-unused-parameter
    #-Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -Wwrite-strings
    -march=native
    # -MMD
    -rdynamic
    )

  set(CMAKE_CXX_COMPILER "g++")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")

endif(APPLE)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

find_package(Boost COMPONENTS timer system filesystem REQUIRED)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#include all .cpp/.h files
file(GLOB the_SRC
	"*.h"
	"*.cpp"
)

add_executable(${TARGET} ${the_SRC})


if(APPLE)
  target_link_libraries(${TARGET} ${Boost_LIBRARIES} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} c++)
else(APPLE)
  target_link_libraries(${TARGET} ${Boost_LIBRARIES} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
endif(APPLE)

