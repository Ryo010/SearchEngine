# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.25.1)

# Set Compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Use the Ninja generator
set(CMAKE_GENERATOR "Ninja")

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Export compile commands for tooling (e.g., language servers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Create the main executable for SearchEngine
project(SearchEngine)

find_package(CURL REQUIRED)
message("Found curl!")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
file(GLOB_RECURSE SRC src/*.cpp)
add_executable(SearchEngine ${SRC})


target_link_libraries(SearchEngine PRIVATE CURL::libcurl ws2_32)