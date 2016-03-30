#
# Find Google Test
# https://cmake.org/cmake/help/v3.0/module/FindGTest.html
#
if(WIN32)
   SET(GTEST_FOUND "NO")
   FIND_PATH(P gtest.h
     PATHS
     "$ENV{GTEST}\\googletest\\include\\gtest"
     )
   if(P)
     SET(GTEST_FOUND "YES")
     SET(GTEST_INCLUDE_DIRS "${P}\\..\\") 
     MESSAGE("GTest include path :${GTEST_INCLUDE_DIRS}")
     FIND_LIBRARY(GTEST_RELEASE_LIB NAME gtest
       PATHS
        "$ENV{GTEST}/vc/googlemock/gtest/Release"
       )
     FIND_LIBRARY(GTEST_DEBUG_LIB NAME gtest
       PATHS
        "$ENV{GTEST}/vc/googlemock/gtest/Debug"
       )
     FIND_LIBRARY(GTEST_RELEASE_LIB_MAIN  NAME gtest_main
       PATHS
        "$ENV{GTEST}/vc/googlemock/gtest/Release"
       )
     FIND_LIBRARY(GTEST_DEBUG_LIB_MAIN NAME gtest_main
       PATHS
        "$ENV{GTEST}/vc/googlemock/gtest/Debug"
       )
   ENDIF()
else(WIN32)
   find_package(GTest REQUIRED)
endif(WIN32)
