# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
cmake_minimum_required(VERSION 3.1.0)

project(move)

# Find librealsense2 installed package
find_package(realsense2 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(CURL REQUIRED)

# Enable C++11
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(stasis stasis.cpp)
target_link_libraries(stasis ${realsense2_LIBRARY} ${CURL_LIBRARY})

add_executable(wasd wasd.cpp)
target_link_libraries(wasd ${CURL_LIBRARY})

# Add the application sources to the target
add_executable(${PROJECT_NAME} main.cpp)

# Link librealsense2 to the target
target_link_libraries(${PROJECT_NAME} ${realsense2_LIBRARY} ${CURL_LIBRARY})
