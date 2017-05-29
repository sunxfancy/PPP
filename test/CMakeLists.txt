enable_testing()
find_package(GTest REQUIRED)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${GTEST_INCLUDE_DIRS})
file(GLOB_RECURSE test_source_files ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable (automaton_test ${test_source_files})

target_link_libraries(automaton_test pp lex ${CONAN_LIBS})

add_test(AutomatonTest automaton_test)

add_custom_target( runtest ALL
        DEPENDS automaton_test)

add_custom_command(TARGET runtest
        POST_BUILD
        COMMAND automaton_test)
