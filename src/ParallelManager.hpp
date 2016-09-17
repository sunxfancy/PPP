#ifndef PARALLEL_MANAGER_HPP
#define PARALLEL_MANAGER_HPP


#include "Automaton.hpp"
#include "SharedStack.hpp"
#include <map>


class ParallelManager {
public:
    void run() {
        Automaton ato(this);
        // ato.init()
        ato.run();
    }

    void finish(Automaton* other) {
        Automaton* new_ato = new Automaton(*other);
        be_map[&(new_ato->begin_stack)] = new_ato;
    }

private:
    // 并行支持
    std::map< SharedStack<int>*, Automaton* > be_map;

};


#endif /* end of include guard: PARALLEL_MANAGER_HPP */
