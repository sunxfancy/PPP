#include "ParallelManager.hpp"
#include <thread>
#include "Combine.hpp"
#include <sstream>
#include <iostream>
#include "LALRTable.hpp"

using namespace std;

extern
void HexOutput(const char* buf, size_t size);

void thread_task(ParallelWorker* pw) {
    pw->run();
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

void ParallelManager::split(int n) {
    int size = data.size();
    int psize = size / n;
    std::vector<ParallelWorker*> pws;
    for (int i = 0; i < n; ++i) {
        ParallelWorker* pm = new ParallelWorker(ptable, func);
        if (i == n-1)
            pm->init(i*psize, size, tokens.data());
        else
            pm->init(i*psize, (i+1)*psize, tokens.data());
        std::thread* t = new std::thread(thread_task, pm);
        pws.push_back(pm);
        threads.push_back(t);
    }

    for (auto* t : threads) {
        t->join();
        delete t;
    }

    combine(pws);

    for (auto* p : pws) {
        delete p;
    }
}



void ParallelManager::run_lex(const std::string& path) {
    fileReader(path);
    lex->setData(data.c_str());
    for (auto* t = lex->Read(); t->type != -1; t = lex->Read()) {
        tokens.push_back(*t);
    }
}

void ParallelManager::fileReader(const std::string& path) {
    std::ifstream t(path, std::ios::binary);

    t.seekg(0, std::ios::end);
    data.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}

void ParallelManager::combine(std::vector<ParallelWorker*>& pws) {
    auto p = pws.begin();
    Combine comb(*p);
    for (++p; p != pws.end(); ++p) {
        comb.combineOutput(*p);
    }
}
