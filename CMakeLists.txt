cmake_minimum_required(VERSION 2.8)

# Name of the project
project(raytrace)

# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp")

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h")

# Set the executable.
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add easy_profiler from local source (for best results on Mac)
#add_subdirectory("ext/easy_profiler-2.1.0")
#target_link_libraries(raytrace easy_profiler)
#include_directories("ext/easy_profiler-2.1.0/easy_profiler_core/include/easy")

# Add TBB
set(TBB_INCLUDE_DIR "$ENV{TBB_INCLUDE_DIR}")
if(TBB_INCLUDE_DIR)
  include_directories(${TBB_INCLUDE_DIR})
  message(STATUS "TBB environment variable found")
else()
  message(STATUS "TBB environment variable `TBB_INCLUDE_DIR` not found, TBB must be installed with the system")
endif()

# Add GLM
# Get the GLM environment variable. Since GLM is a header-only library, we
# just need to add it to the include directory.
set(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
if(GLM_INCLUDE_DIR)
  include_directories(${GLM_INCLUDE_DIR})
  message(STATUS "GLM environment variable found")
else()
# If the GLM_INCLUDE_DIR environment variable is not set, we assume
# the user has installed GLM properly on their system
  message(STATUS "GLM environment variable `GLM_INCLUDE_DIR` not found, GLM must be installed with the system")
endif()

# OS specific options and libraries
if(WIN32)
  # c++0x is enabled by default.
  # -Wall produces way too many warnings.
  # -pedantic is not supported.
  # Disable warning 4996.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
else()
  # Enable all pedantic warnings.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -pedantic -ltbb -O3")
endif()
