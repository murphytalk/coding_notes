cmake_minimum_required(VERSION 2.8)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TARGET cpp_notes)
project(${TARGET} CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(NOT ${WIN32})
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
endif()    

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++11 -DGTEST_USE_OWN_TR1_TUPLE=1)
else(CMAKE_COMPILER_IS_GNUCXX)
  if(APPLE)
    add_definitions(-std=c++11 -stdlib=libc++ -DGTEST_USE_OWN_TR1_TUPLE=1)
  endif(APPLE)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(NOT WIN32)
  string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
endif()

################################
# Include all .cpp/.h files
################################
file(GLOB_RECURSE the_SRC
	"src/*.h"
	"src/*.cpp"
)
add_executable(${TARGET} ${the_SRC})

################################
#### FIND REQUIRED PACKAGES ####
################################
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
#
# Google Test
# 
find_package(GTestLocal)
if(NOT ${GTEST_FOUND})
  MESSAGE(FATAL_ERROR "** Google Test Not Found!")
endif()
include_directories(${GTEST_INCLUDE_DIRS})
if(WIN32)
  TARGET_LINK_LIBRARIES(${TARGET} optimized ${GTEST_RELEASE_LIB})
  TARGET_LINK_LIBRARIES(${TARGET} debug ${GTEST_DEBUG_LIB})
  TARGET_LINK_LIBRARIES(${TARGET} optimized ${GTEST_RELEASE_LIB_MAIN})
  TARGET_LINK_LIBRARIES(${TARGET} debug ${GTEST_DEBUG_LIB_MAIN})
else(WIN32)
  TARGET_LINK_LIBRARIES(${TARGET} ${GTEST_BOTH_LIBRARIES})  
endif(WIN32)

#
# Boost
#
include(CMake/boost.cmake.in)
if(WIN32)
    INCLUDE_DIRECTORIES(${BOOST_ROOT})
else(WIN32)
    find_package(Boost COMPONENTS timer system filesystem date_time REQUIRED)
endif(WIN32)



if(CMAKE_COMPILER_IS_GNUCXX)
   set(EXTRA_LIB "pthread")
else(CMAKE_COMPILER_IS_GNUCXX)
  if(APPLE)
    set(EXTRA_LIB "c++")
  endif(APPLE)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(${TARGET} ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES} ${EXTRA_LIB})
