#pragma once

#include "Automaton.hpp"
#include "SharedStack.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <strstream>
#include <string>

#include <functional>

class ParallelWorker {
public:
    ParallelWorker(LALRTable* ptable, AutoCallback func) {
        this->ptable = ptable;
        this->func = func;
    }

    void init(int begin, int end, Token** data) {
        this->begin = begin;
        this->end = end;
        this->tokens = data;
    }

    void run() {
        Automaton ato(this);
        ato.init(ptable, func, tokens);
        ato.run();
    }

    void finish(Automaton* ato) {
        be_map[&(ato->begin_stack)] = ato->getLRStack();
    }

    // 并行支持
    std::map< SharedStack<int>*, std::deque<int> > be_map;
private:
    LALRTable* ptable;
    AutoCallback func;
    Token** tokens;
    int begin, end;
};


namespace std { class thread; }
class ParallelManager {
public:
    ParallelManager(const char* l, const char* p, AutoCallback func) ;
    void split(int n);
    void run_lex(const std::string& path);

private:
    LexInterface* lex;
    LALRTable* ptable;
    std::string data;
    std::vector<Token*> tokens;
    std::vector<std::thread*> threads;
    AutoCallback func;

    void combine(std::vector<ParallelWorker*>& pws);
    void fileReader(const std::string& path);
};
