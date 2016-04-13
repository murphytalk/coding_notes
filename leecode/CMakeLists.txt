set(LEECODE leecode)
project(${LEECODE} CXX)

################################
# Include all .cpp/.h files
################################
file(GLOB_RECURSE LEECODE_SRC
	"src/*.h"
	"src/*.cpp"
)
add_executable(${LEECODE} ${LEECODE_SRC})

if(WIN32)
  TARGET_LINK_LIBRARIES(${LEECODE} optimized ${GTEST_RELEASE_LIB})
  TARGET_LINK_LIBRARIES(${LEECODE} debug ${GTEST_DEBUG_LIB})
  TARGET_LINK_LIBRARIES(${LEECODE} optimized ${GTEST_RELEASE_LIB_MAIN})
  TARGET_LINK_LIBRARIES(${LEECODE} debug ${GTEST_DEBUG_LIB_MAIN})
else(WIN32)
  TARGET_LINK_LIBRARIES(${LEECODE} ${GTEST_BOTH_LIBRARIES})  
endif(WIN32)

target_link_libraries(${LEECODE} ${GTEST_BOTH_LIBRARIES} ${EXTRA_LIB})


