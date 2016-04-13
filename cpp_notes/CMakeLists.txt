set(CPPNOTES cpp_notes)
project(${CPPNOTES} CXX)

################################
# Include all .cpp/.h files
################################
file(GLOB_RECURSE CPP_NOTES_SRC
	"src/*.h"
	"src/*.cpp"
)
add_executable(${CPPNOTES} ${CPP_NOTES_SRC})

if(WIN32)
  TARGET_LINK_LIBRARIES(${CPPNOTES} optimized ${GTEST_RELEASE_LIB})
  TARGET_LINK_LIBRARIES(${CPPNOTES} debug ${GTEST_DEBUG_LIB})
  TARGET_LINK_LIBRARIES(${CPPNOTES} optimized ${GTEST_RELEASE_LIB_MAIN})
  TARGET_LINK_LIBRARIES(${CPPNOTES} debug ${GTEST_DEBUG_LIB_MAIN})
else(WIN32)
  TARGET_LINK_LIBRARIES(${CPPNOTES} ${GTEST_BOTH_LIBRARIES})  
endif(WIN32)

target_link_libraries(${CPPNOTES} ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES} ${EXTRA_LIB})


