if(WIN32)
  if(NOT DEFINED ENV{BOOST_ROOT})
    message(FATAL_ERROR "BOOST_ROOT environment variable is not defined. Please set BOOST_ROOT to your Boost installation path.")
  else()
    # Set the BOOST_ROOT variable within CMake
    set(BOOST_ROOT $ENV{BOOST_ROOT})
    message(STATUS "Using Boost from ${BOOST_ROOT}")

    # Add the path where Boost CMake modules might be installed
    set(Boost_NO_SYSTEM_PATHS TRUE)
  endif()
endif()
