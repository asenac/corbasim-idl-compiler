cmake_minimum_required(VERSION 2.6)
project(corbasim-idl-compiler CXX)

enable_testing()

set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic")

set(PARSER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/idl-parser)

include_directories(
    ${PARSER_DIR}/src
)

add_subdirectory(${PARSER_DIR})

