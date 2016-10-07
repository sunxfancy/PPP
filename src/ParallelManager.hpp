#ifndef PARALLEL_MANAGER_HPP
#define PARALLEL_MANAGER_HPP


#include "Automaton.hpp"
#include "SharedStack.hpp"
#include <map>
#include <vector>



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

    void finish(Automaton* other) {
        Automaton* new_ato = new Automaton(*other);
        be_map[&(new_ato->begin_stack)] = new_ato;
    }

private:
    // 并行支持
    std::map< SharedStack<int>*, Automaton* > be_map;
    LALRTable* ptable;
    AutoCallback func;
    Token** tokens;
    int begin, end;
};




class ParallelManager {
public:
    ParallelManager() {
        lex = LexInterface::Create();
    }

    void split() {

    }

    void run_lex(const string& path) {

    }
private:
    LexInterface* lex;

    std::vector<Token*> tokens;
};


#endif /* end of include guard: PARALLEL_MANAGER_HPP */
