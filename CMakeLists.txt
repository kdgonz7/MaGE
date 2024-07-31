cmake_minimum_required(VERSION 3.6)
project(MaGE)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES Mage/*.cpp)
add_executable(nmage Mage/Mage.cpp ${SOURCES})
