cmake_minimum_required(VERSION 3.20)
project(MyKeylight)


set(CMAKE_CXX_STANDARD 14)

add_executable(MyKeylightCLI.cpp Keylight.cpp Keylight.h KeylightDiscover.cpp KeylightCommunication.cpp)
target_link_libraries(MyKeylight PRIVATE curl)