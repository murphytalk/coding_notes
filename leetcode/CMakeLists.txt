set(LEETCODE leetcode)
project(${LEETCODE} CXX)

################################
# Include all .cpp/.h files
################################
file(GLOB_RECURSE LEETCODE_SRC
	"src/*.h"
	"src/*.cpp"
)
add_executable(${LEETCODE} ${LEETCODE_SRC})

if(WIN32)
  TARGET_LINK_LIBRARIES(${LEETCODE} optimized ${GTEST_RELEASE_LIB})
  TARGET_LINK_LIBRARIES(${LEETCODE} debug ${GTEST_DEBUG_LIB})
  TARGET_LINK_LIBRARIES(${LEETCODE} optimized ${GTEST_RELEASE_LIB_MAIN})
  TARGET_LINK_LIBRARIES(${LEETCODE} debug ${GTEST_DEBUG_LIB_MAIN})
else(WIN32)
  TARGET_LINK_LIBRARIES(${LEETCODE} ${GTEST_BOTH_LIBRARIES})  
endif(WIN32)

target_link_libraries(${LEETCODE} ${GTEST_BOTH_LIBRARIES} ${EXTRA_LIB})


