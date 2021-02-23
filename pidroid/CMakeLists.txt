cmake_minimum_required(VERSION 3.4.1)

file(GLOB HEADER_FILES src/main/cpp/*.h src/main/cpp/*.hpp src/main/cpp/pidroid/*.hpp src/main/cpp/pidroid/commons.hpp)
file(GLOB SOURCE_FILES src/main/cpp/*.cpp  src/main/cpp/pidroid/*.cpp src/main/cpp/pidroid/commons.cpp)

add_library(pidroid SHARED ${SOURCE_FILES} ${HEADER_FILES})
set(CMAKE_VERBOSE_MAKEFILE on)

# include_directories(/home/adrian/Escritorio/AndroidStudioProjects/PiDroid/app/src/main/cpp/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

find_library( # Sets the name of the path variable.
        android-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        android )

target_link_libraries( # Specifies the target library.
                    pidroid
                   ${log-lib}
                   ${android-lib})

target_compile_options(pidroid PRIVATE
        "$<$<CONFIG:RELEASE>:-O3>"
        "$<$<CONFIG:DEBUG>:-O3>"
        )