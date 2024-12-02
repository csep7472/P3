cmake_minimum_required(VERSION 3.22)
project(VideoGameWordle)

set(CMAKE_CXX_STANDARD 14)

## Compile flags to match Gradescope test environment
#set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_TOOLCHAIN_FILE "C:/Users/liand/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Include directories
include_directories(src)

# Define the executable target
add_executable(Main
        src/main.cpp
        src/splay.h
        src/api.h
        src/api.cpp
)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Main PRIVATE nlohmann_json::nlohmann_json)
find_package(CURL REQUIRED)
target_link_libraries(Main PRIVATE nlohmann_json::nlohmann_json CURL::libcurl)