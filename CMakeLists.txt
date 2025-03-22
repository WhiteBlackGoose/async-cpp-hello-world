cmake_minimum_required(VERSION 3.16)
project(AsyncProject LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Enables clangd LSP support
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(folly REQUIRED)

# Add the executable
add_executable(async_app src/main.cpp)

target_link_libraries(async_app PRIVATE
    folly
    glog
    gflags
    double-conversion
    event
    fmt
    Threads::Threads
    atomic
)
