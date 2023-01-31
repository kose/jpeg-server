##
set(EXE "jpeg-server")
project(${EXE})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(FLAG_DEBUG -D_DEBUG_)
else()
  set(FLAG_DEBUG)
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
find_package(OpenCV REQUIRED)

#
# M1 or Intel
#
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} ARCH)

#
# M1(arm64:homebrew) or not
#
if (ARCH STREQUAL "arm64")
  include_directories(/opt/homebrew/include)
  link_directories(/opt/homebrew/lib)
  set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wno-deprecated-declarations")
else()
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
  set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wno-deprecated-declarations")
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

file (GLOB_RECURSE SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/jpeg-server.cpp
  )

add_definitions(-DTBB_SUPPRESS_DEPRECATED_MESSAGES ${FLAG_DEBUG} -DHOME_DIR="$ENV{HOME}")
add_executable (${EXE} ${SOURCE_FILES} ${LIBRARY_FILES})

target_link_libraries (${EXE} PRIVATE
  ${OpenCV_LIBS} gflags zmq
)


# end
