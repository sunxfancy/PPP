#ifndef PARALLEL_MANAGER_HPP
#define PARALLEL_MANAGER_HPP


#include "Automaton.hpp"
#include "SharedStack.hpp"
#include <map>




class ParallelWorker {
public:
    ParallelWorker(LALRTable* ptable, AutoCallback func) {
        this->ptable = ptable;
        this->func = func;
    }

    void run() {
        Automaton ato(this);
        ato.init(ptable, func);
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
};




class ParallelManager {
public:


};


#endif /* end of include guard: PARALLEL_MANAGER_HPP */
