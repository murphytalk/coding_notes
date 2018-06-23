cmake_minimum_required(VERSION 3.1)

project( CodingNotes )
enable_testing()

# We need C++14
set (CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# When other libraries are using a shared version of runtime libraries
option(
  force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  ON)

include(CMake/internal_utils.cmake)
fix_default_compiler_settings_()

################################
#### FIND REQUIRED PACKAGES ####
################################
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
#
# Boost
#
include(CMake/boost.cmake.in)
if(WIN32)
    INCLUDE_DIRECTORIES(${BOOST_ROOT})
else(WIN32)
    find_package(Boost COMPONENTS timer system filesystem date_time REQUIRED)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})	
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
   set(EXTRA_LIB "pthread")
else(CMAKE_COMPILER_IS_GNUCXX)
  if(APPLE)
    set(EXTRA_LIB "c++")
  endif(APPLE)
endif(CMAKE_COMPILER_IS_GNUCXX)

set(NOTES notes)
project(${NOTES} CXX)
add_src_libs_ (${NOTES})
