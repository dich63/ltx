# CMakeList.txt : CMake project for ltx_test1, include source and define
# project specific logic here.
#

# Add source to this project's executable.


#CMAKE_CXX_FLAGS_DEBUG:STRING=/MDd /Zi /Ob0 /Od /RTC1
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")


include_directories($ENV{ltx_js_root}/include)
add_executable (ltx_test3 "test.cpp" "test.h")



#set_property(TARGET ltx_test3 PROPERTY
#  MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ltx_test3 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
