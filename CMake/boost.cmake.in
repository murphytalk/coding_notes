if(WIN32)
  FIND_PATH(P "boost\\config.hpp"
    PATHS
    "$ENV{BOOST_ROOT}"
    "${CMAKE_CURRENT_SOURCE_DIR}\\..\\boost"
    NO_DEFAULT_PATH
    )
  if(P)
    set(BOOST_ROOT ${P})
    # On Windows, VC++ supports auto-linking, it will know which library to link from the boost header file included.
    # Only need to tell VC++ where to find the lib
    link_directories("${BOOST_ROOT}\\stage\\lib")
  else()
    MESSAGE(FATAL_ERROR "** Cannot find boost, please setup environment variable BOOST_ROOT **")
  endif()
endif()
