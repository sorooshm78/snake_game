cmake_minimum_required(VERSION 3.16)

project(snake_network_game)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Werror -Wall -Wextra")

add_executable(snake main.cpp)
target_link_libraries(snake pthread)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory_leak
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
