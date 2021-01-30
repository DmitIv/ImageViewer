cmake_minimum_required(VERSION 3.0)
set(PROJECT ImageViewer)
project(${PROJECT} C CXX)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-Wall -Wextra -pedantic -O2)

find_package(OpenCV REQUIRED) # should have processing the case if opencv not founded
include_directories(${OpenCV_INCLUDE_DIRS} viewer uinterface)

add_executable(${PROJECT} main.cpp viewer/image_viewer.cpp uinterface/uinterface.cpp)
target_link_libraries(${PROJECT} ${OpenCV_LIBS})