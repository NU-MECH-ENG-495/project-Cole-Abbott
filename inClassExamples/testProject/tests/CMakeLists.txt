# Include FetchContent for downloading GoogleTest
include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Configure GoogleTest
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
# Create test executable
add_executable(unit_tests
unit/test_feature1.cpp
#unit/test_feature2.cpp
)
# Link against GoogleTest and your main library
target_link_libraries(unit_tests
PRIVATE
GTest::gtest_main
${PROJECT_NAME}
)
# Discover and run tests
gtest_discover_tests(unit_tests)
