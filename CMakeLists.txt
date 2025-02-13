cmake_minimum_required(VERSION 3.27)
project(concurrency)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)


add_executable(concurrency src/main.cpp)

target_link_libraries(concurrency Boost::system)
