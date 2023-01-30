cmake_minimum_required(VERSION 3.24)
project(SerialTransferCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

add_executable(SerialTransferCpp main.cpp
        src/SerialTransferCpp.cpp
        src/PacketCpp.cpp
        src/SerialTransfer.cpp
        src/Packet.cpp

        test/Test_UARTTXData.cpp
        test/Test_UARTRXData.cpp)

target_include_directories(SerialTransferCpp PUBLIC src)
target_include_directories(SerialTransferCpp PUBLIC test)

target_link_options(SerialTransferCpp PUBLIC --coverage)
target_compile_options(SerialTransferCpp PUBLIC --coverage)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set(BUILD_GMOCK OFF)

enable_testing()

target_link_libraries(SerialTransferCpp PUBLIC GTest::gtest_main)
#target_link_libraries(SerialTransferCpp PUBLIC GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(SerialTransferCpp)