cmake_minimum_required(VERSION 3.0)
project(yanda VERSION 1.0 LANGUAGES CXX)

# Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)

# yanda Library
add_library(yanda INTERFACE)
target_include_directories(yanda
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

## Example App
#add_executable(HXTIO-Example HXTIO-Example.cpp)
#target_link_libraries(HXTIO-Example hxtio)

## OpenCV Example App
#option(HXT_USE_OPENCV "Use OpenCV library" off)
#if(HXT_USE_OPENCV)
#    find_package(OpenCV 3 REQUIRED)
#    add_executable(HXTIO-OpenCV HXTIO-OpenCV.cpp)
#    target_link_libraries(HXTIO-OpenCV hxtio opencv_core opencv_imgcodecs)
#endif()
