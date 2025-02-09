cmake_minimum_required(VERSION 3.14)
project(your_project_name VERSION 1.0)
# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add source files
add_library(${PROJECT_NAME}
src/source1.cpp
#src/source2.cpp
)
# Add include directories
target_include_directories(${PROJECT_NAME} PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/src/include
)
# Enable testing
enable_testing()
# Add the tests subdirectory
add_subdirectory(tests)