/*
* @Author: sxf
* @Date:   2016-10-24 12:02:18
* @Last modified by:   sxf
* @Last modified time: 2016-11-18
*/

#ifdef __APPLE__
#define GTEST_HAS_POSIX_RE 0
#endif

#include <gtest/gtest.h>
#include "App.hpp"
#include "Combine.hpp"

using namespace std;

extern timespec diff(timespec start, timespec end);

#define AUTOMATON_TEST(name) TEST(AutomatonTest_##name, name)

AUTOMATON_TEST (Construction)
{
    App app("test.txt", "", "", 2);
    app.LoadFile();
    ParallelWorker* pw =  app.pm->create_worker(1, 2);
    EXPECT_NE(pw, nullptr);
}

AUTOMATON_TEST (Combine)
{
    App app("t100.txt", "", "", 2);
    app.LoadFile();
    app.run_lex();

    ParallelWorker* pw = app.pm->create_worker(0, 2);
    pw->run();
    pw->printAll();

    ParallelWorker* pw2 = app.pm->create_worker(1, 2);
    pw2->run();
    pw2->printAll();

    Combine comb(pw);
    bool t = comb.combineOutput(pw2);
    EXPECT_EQ(t, true);
    const auto& stack = comb.getOutput();
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack[1], 3);
}

AUTOMATON_TEST (Split)
{
    App app("t100.txt", "", "",  5);
    app.LoadFile();
    app.run_lex();

    ParallelWorker* pw = app.pm->create_worker(0, 5);
    pw->run();
    pw->printAll();

    ParallelWorker* pw2 = app.pm->create_worker(1, 5);
    pw2->run();
    pw2->printAll();
    EXPECT_NE(pw2->getBeginEndMap().size(), 0);
}

AUTOMATON_TEST (Running)
{
    App app("test.txt", "", "",  3);
    app.LoadFile();
    app.run_lex();

    ParallelWorker* pw = app.pm->create_worker(0, 3);
    pw->run();
    pw->printAll();

    ParallelWorker* pw2 = app.pm->create_worker(1, 3);
    pw2->run();
    pw2->printAll();

    ParallelWorker* pw3 = app.pm->create_worker(2, 3);
    pw3->run();
    pw3->printAll();
    EXPECT_NE(pw2->getBeginEndMap().size(), 0);
}


AUTOMATON_TEST (Run5)
{
    App app("t100.txt", "", "", 5);
    app.LoadFile();
    app.run_lex();

    ParallelWorker* pw2 = app.pm->create_worker(1, 3);
    pw2->run();
    pw2->printAll();
}



AUTOMATON_TEST (SpeedTest)
{
    timespec time, time1, time2;
    clock_gettime(CLOCK_REALTIME, &time1);
    App app("MOCK_DATA.json", "lex.cfg.lexsave", "parser.cfg.lrsave", 1);
    clock_gettime(CLOCK_REALTIME, &time2);
    time = diff(time1, time2);
    printf("\nInit time: %li:%li ns\n", time.tv_sec, time.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &time1);
    app.LoadFile();
    clock_gettime(CLOCK_REALTIME, &time2);
    time = diff(time1, time2);
    printf("\nLoad Library time: %li:%li ns\n", time.tv_sec, time.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &time1);
    app.run_lex();
    clock_gettime(CLOCK_REALTIME, &time2);
    time = diff(time1, time2);
    printf("\nRun Lex time: %li:%li ns\n", time.tv_sec, time.tv_nsec);


    for (int k = 1; k <= 4; k++) {
        clock_gettime(CLOCK_REALTIME, &time1);
//        for (int i = 0; i < 100; ++i)
            app.run_threads(k);
        clock_gettime(CLOCK_REALTIME, &time2);
        printf("%d -> %li:%li ns\n---------------------\n", k, diff(time1, time2).tv_sec, diff(time1, time2).tv_nsec);
    }
}
