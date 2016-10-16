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

    void combineOutput(ParallelWorker* pw);
    
private:
    ParallelWorker* first;

};