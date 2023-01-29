cmake_minimum_required(VERSION 3.24)
project(SerialTransferCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(SerialTransferCpp main.cpp
        src/SerialTransferCpp.cpp
        src/PacketCpp.cpp)

target_include_directories(SerialTransferCpp PUBLIC src)
