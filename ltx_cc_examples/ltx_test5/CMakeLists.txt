# CMakeList.txt : CMake project for ltx_test1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
include_directories($ENV{ltx_js_root}/include ../include)
add_executable (ltx_test5 "test.cpp" "test.h")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ltx_test5 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
