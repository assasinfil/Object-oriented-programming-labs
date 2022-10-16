cmake_minimum_required(VERSION 3.17)
project(lab4)

set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(lab4 main.cpp)
target_link_libraries(lab4 ${OpenCV_LIBS} )
file(COPY ${PROJECT_SOURCE_DIR}/test.jpg
        DESTINATION ${PROJECT_BINARY_DIR})