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

    combine(pws);

    for (auto* p : pws) {
        delete p;
    }
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
    fileReader(path);
    lex->setData(data.c_str());
    auto* t = lex->Read();
    for (; t->type != 0; t = lex->Read()) {
        printf("%s\n", t->pToken);
        tokens.push_back(*TokenFliter(ptable->vmap, t));
    }
    tokens.push_back(*TokenFliter(ptable->vmap, t));
    printf("\n%s\n", "run lex finished");
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
