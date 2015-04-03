cmake_minimum_required(VERSION 2.8)

set(TARGET cpp_notes)

project(${TARGET} CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

include_directories(/usr/local/include)

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
  # -rdynamic
)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

if(APPLE)
   include_directories(~/works/gtest/include)
   link_directories(~/works/gtest)
   set(GTEST_MAIN_LIBRARIES "gtest_main")
   set(GTEST_LIBRARIES "gtest")
else(APPLE)
   find_package(GTest REQUIRED)
endif(APPLE)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++11 -DGTEST_USE_OWN_TR1_TUPLE=1)
else(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++11 -stdlib=libc++ -DGTEST_USE_OWN_TR1_TUPLE=1)
endif(CMAKE_COMPILER_IS_GNUCXX)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

find_package(Boost COMPONENTS timer system filesystem REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#include all .cpp/.h files
file(GLOB_RECURSE the_SRC
	"src/*.h"
	"src/*.cpp"
)

add_executable(${TARGET} ${the_SRC})


if(CMAKE_COMPILER_IS_GNUCXX)
   set(EXTRA_LIB "pthread")
else(CMAKE_COMPILER_IS_GNUCXX)
   set(EXTRA_LIB "c++")
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(${TARGET} ${Boost_LIBRARIES} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${EXTRA_LIB})
