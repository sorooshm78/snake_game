cmake_minimum_required(VERSION 3.16)

project(snake_network_game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")
add_executable(snake main.cpp)
