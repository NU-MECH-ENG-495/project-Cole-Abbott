cmake_minimum_required(VERSION 3.14)
project(hello_test)

# GoogleTest requires at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch and configure Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Create the test executable
add_executable(hello_test hellotest.cpp)
target_link_libraries(hello_test GTest::gtest_main)

# Register the test
include(GoogleTest)
gtest_discover_tests(hello_test)