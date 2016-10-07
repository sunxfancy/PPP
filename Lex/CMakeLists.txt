cmake_minimum_required(VERSION 2.8)
project(lex)

SET (CMAKE_BUILD_TYPE Debug)
SET (CMAKE_CXX_COMPILER_ENV_VAR "clang++")
SET (CMAKE_CXX_FLAGS "-std=c++11")
SET (CMAKE_CXX_FLAGS_DEBUG   "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL  "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE  "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(../ExIconv/include ../third-party/cereal-1.1.2)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ExIconv/build)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ExIconv/extlib/lib)

# 核心路径配置
include_directories(include src ../header_libs/cereal-1.1.2)
file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM  source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp)

add_library(lex ${source_files})
install(TARGETS lex DESTINATION bin)
