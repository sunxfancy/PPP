cmake_minimum_required(VERSION 2.8)

project(ppp)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(Lex)

# src files and make exe
file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM  source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

include_directories(Lex/include third-party/cereal-1.1.2 src)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_library(pp ${source_files})

add_executable(ppp src/main.cpp)
target_link_libraries(ppp pp lex dl ${CONAN_LIBS})

add_subdirectory(test)
