#include "Combine.hpp"
#include "Automaton.hpp"
#include "ParallelManager.hpp"
    
bool Combine::combineOutput(ParallelWorker* pw) {
    auto&stack = first->getBeginEndMap().begin()->first;
    auto it = pw->getBeginEndMap().find(stack);
    Automaton* automaton;
    if (it != pw->getBeginEndMap().end()) 
        automaton = it->second;
    else return false;
    return true;
}
    