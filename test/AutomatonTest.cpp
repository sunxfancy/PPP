/*
* @Author: sxf
* @Date:   2016-10-24 12:02:18
* @Last Modified by:   sxf
* @Last Modified time: 2016-10-24 12:02:18
*/

#ifdef __APPLE__
#define GTEST_HAS_POSIX_RE 0
#endif

#include <gtest/gtest.h>
#include <string>
#include "App.hpp"

using namespace std;

#define AUTOMATON_TEST(name) TEST(AutomatonTest_##name, name)

AUTOMATON_TEST (Construction)
{
    App app("test.txt", "libparser.so", 2);
    app.loadLibrary();
    ParallelWorker* pw =  app.pm->create_worker(1, 2);
    EXPECT_NE(pw, nullptr);
}


AUTOMATON_TEST (Running)
{
    App app("test.txt", "libparser.so", 2);
    app.loadLibrary();
    app.run_lex();
    ParallelWorker* pw =  app.pm->create_worker(1, 2);
    pw->run();
    EXPECT_NE(pw->getBeginEndMap().size(), 0);
}
