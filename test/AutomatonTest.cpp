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
#include "App.hpp"

using namespace std;

extern timespec diff(timespec start, timespec end);

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
    App app("test.txt", "libparser.so", 0);
    app.loadLibrary();
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





AUTOMATON_TEST (SpeedTest)
{
    timespec time, time1, time2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    App app("test2.txt", "libparser.so", 1);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    time = diff(time1, time2);
    printf("\nInit time: %li:%li ns\n", time.tv_sec, time.tv_nsec);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    app.loadLibrary();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    time = diff(time1, time2);
    printf("\nLoad Library time: %li:%li ns\n", time.tv_sec, time.tv_nsec);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    app.run_lex();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    time = diff(time1, time2);
    printf("\nRun Lex time: %li:%li ns\n", time.tv_sec, time.tv_nsec);


    for (int k = 1; k <= 16; k++) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
//        for (int i = 0; i < 100; ++i)
            app.run_threads(k);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
        printf("%d -> %li:%li ns\n---------------------\n", k, diff(time1, time2).tv_sec, diff(time1, time2).tv_nsec);
    }
}