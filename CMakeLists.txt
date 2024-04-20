cmake_minimum_required(VERSION 3.28)

set(NOTES notes)
project(${NOTES} CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 20)

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
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
#
# Boost
#
include(CMake/boost.cmake)
find_package(Boost COMPONENTS timer system filesystem date_time REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

Include(FetchContent)

set(BUILD_TESTING OFF)
set(CATCH_INSTALL_DOCS OFF)
set(CATCH_INSTALL_EXTRAS OFF)
FetchContent_Declare(
  Catch2
  URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.4.tar.gz
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/CMake")

if(CMAKE_COMPILER_IS_GNUCXX)
   set(EXTRA_LIB "pthread")
else(CMAKE_COMPILER_IS_GNUCXX)
  if(APPLE)
    set(EXTRA_LIB "c++")
  endif(APPLE)
endif(CMAKE_COMPILER_IS_GNUCXX)
set(EXTRA_LIB "${EXTRA_LIB}")
message(STATUS "Using extra libs: ${EXTRA_LIB}")

add_src_libs_ (${NOTES})
