cmake_minimum_required(VERSION 3.4.1)

file(GLOB SRC_ESGUI src/main/cpp/esgui/*.cpp)

add_library( native-lib SHARED
             ${SRC_ESGUI}
             src/main/cpp/native-lib.cpp
             src/main/cpp/esgui/button.cpp
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

target_link_libraries( native-lib
                       GLESv2
                       ${log-lib} )