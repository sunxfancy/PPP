#include "ParallelManager.hpp"
#include <thread>
#include "Combine.hpp"
#include <sstream>
#include <iostream>
#include <cstring>
#include "LALRTable.hpp"

using namespace std;

extern
void HexOutput(const char* buf, size_t size);

extern timespec diff(timespec start, timespec end);

void thread_task(ParallelWorker* pw) {
    timespec time1, time2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    pw->run();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    pw->time = diff(time1, time2).tv_nsec;
}


ParallelManager::ParallelManager(const char* l, size_t ls, const char* p, size_t ps, AutoCallback func) {
    lex = LexInterface::Create();
    this->func = func;
    lex->loadTable(l, ls);
    ptable = new LALRTable();
    string str(p, ps);
	istringstream is(str, ios_base::in | ios_base::binary);
    ptable->Load(is);
}

ParallelWorker* ParallelManager::create_worker(int i, int n) {
    int size = (int) tokens.size();
    int psize = size / n;
    ParallelWorker* pm = new ParallelWorker(ptable, func);
    if (i == n-1)
        pm->init(i*psize, size-1, tokens);
    else
        pm->init(i*psize, (i+1)*psize, tokens);
    return pm;
}

void ParallelManager::split(int n) {
    std::vector<ParallelWorker*> pws;
    timespec time, time1, time2;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (int i = 0; i < n; ++i) {
        ParallelWorker* pm = create_worker(i, n);
        std::thread* t = new std::thread(thread_task, pm);
        pws.push_back(pm);
        threads.push_back(t);
    }

    for (auto* t : threads) {
        t->join();
        delete t;
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    time = diff(time1, time2);
    printf("\nall threads running time: %li:%li ns\n", time.tv_sec, time.tv_nsec);
    threads.clear();

    combine(pws);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    countTime(pws);

    for (auto* p : pws) {
        delete p;
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    time = diff(time1, time2);
    printf("\ncleaning time: %li:%li ns\n", time.tv_sec, time.tv_nsec);
}

void ParallelManager::countTime(std::vector<ParallelWorker*>& pws) {
    for (auto* p : pws) {
        time_avg += p->time;
        printf("%f\t", (float) (p->time / 1000000.0));
    }
    time_avg /= pws.size();

    long ans = 0;
    for (auto* p : pws) {
        long c = abs(p->time - time_avg);
        ans += c*c;
    }
    ans /= pws.size();
    time_fc = ans;

    printf("\navg=%li,\tfc=%li\n", time_avg, time_fc);
}

static
Token* TokenFliter(VMap* vmap, Token* token) {
    int id = vmap->getConst(token->pToken);
    if (id != -1) {
        token->type = id;
    }
    // copy debug line
    if (token->debug_line != NULL) {
        char* line = new char[strlen(token->debug_line)+1];
        token->debug_line = strcpy(line, token->debug_line);
    }
    if (token->pToken != NULL) {
        char* line = new char[strlen(token->pToken)+1];
        token->pToken = strcpy(line, token->pToken);
    }
    return token;
}

void ParallelManager::run_lex(const std::string& path) {
    timespec time1, time2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    fileReader(path);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    timespec time = diff(time1, time2);
    printf("\nfile reading time: %li:%li ns\n", time.tv_sec, time.tv_nsec);
    lex->setData(data.c_str());
    auto* t = lex->Read();
    for (; t->type != 0; t = lex->Read()) {
//        printf("%s\n", t->pToken);
        tokens.push_back(*TokenFliter(ptable->vmap, t));
    }
    tokens.push_back(*TokenFliter(ptable->vmap, t));
    printf("\n%s\n", "run lex finished");
}

void ParallelManager::fileReader(const std::string& path) {
    std::ifstream t(path, std::ios::binary);

    t.seekg(0, std::ios::end);
    data.reserve((unsigned long) t.tellg());
    t.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}

bool ParallelManager::combine(std::vector<ParallelWorker*>& pws) {
    int c = 1;
    for (auto p: pws) {
        printf("part %d:\n", c++);
        p->printAll();
    }

    auto p = pws.begin();
    Combine comb(*p);
    bool ans; int count = 2;
    for (++p; p != pws.end(); ++p, ++count) {
        ans = comb.combineOutput(*p);
        if (!ans) {
            printf("Combine Error! error block: %d\n", count);
            return false;
        }
    }
    printf("Combine All Right!\n");
    return true;
}
