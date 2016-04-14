cmake_minimum_required(VERSION 2.8)

project( CodingNotes )
enable_testing()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# When other libraries are using a shared version of runtime libraries
option(
  cpp_note_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  OFF)

include(cmake/internal_utils.cmake)
fix_default_compiler_settings_()

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

add_subdirectory( cpp_notes )
add_subdirectory( leetcode )
#add_subdirectory( algo )
