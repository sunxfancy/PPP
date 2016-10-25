cmake_minimum_required(VERSION 2.8)

project(ppp)

add_subdirectory(Lex)
add_subdirectory(ExIconv)

# src files and make exe
file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM  source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

include_directories(Lex/include third-party/cereal-1.1.2 src)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ExIconv/extlib/lib)


SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_library(pp ${source_files})

add_executable(ppp src/main.cpp)
target_link_libraries(ppp pp lex exiconv iconv charsetdetect charset dl pthread)

add_subdirectory(test)