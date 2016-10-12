#include "ParallelManager.hpp"
#include <thread>


void thread_task(ParallelWorker* pw) {
    pw->run();
}


ParallelManager::ParallelManager(const char* l, const char* p, AutoCallback func) {
    lex = LexInterface::Create();
    this->func = func;
    lex->loadTable(l);
}

void ParallelManager::split(int n) {
    int size = data.size();
    int psize = size / n;
    for (int i = 0; i < n; ++i) {
        ParallelWorker* pm = new ParallelWorker(ptable, func);
        if (i == n-1)
            pm->init(i*psize, size, tokens.data());
        else
            pm->init(i*psize, (i+1)*psize, tokens.data());
        std::thread* t = new std::thread(thread_task, pm);
        threads.push_back(t);
    }
    for (auto* t : threads) {
        t->join();
        delete t;
    }
}



void ParallelManager::run_lex(const std::string& path) {
    fileReader(path);
    lex->setData(data.c_str());
}

void ParallelManager::fileReader(const std::string& path) {
    std::ifstream t(path, std::ios::binary);

    t.seekg(0, std::ios::end);
    data.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
}
