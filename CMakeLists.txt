cmake_minimum_required(VERSION 3.18)
project(POS_sem C)

set(CMAKE_C_STANDARD 99)

add_executable(POS_sem main.c)
target_link_libraries(POS_sem pthread)