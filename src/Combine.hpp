#pragma once
#include "SharedStack.hpp"

class Automaton;
class ParallelWorker;

class Combine {
public:
    Combine(ParallelWorker* pw) {
        first = pw;
    }
    ~Combine() {
        
    }

    bool combineOutput(ParallelWorker* pw);
    
private:
    ParallelWorker* first;

};