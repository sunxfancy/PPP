/**
* @Author: Sun Xiaofan <sxf>
* @Date:   2016-10-26
* @Email:  sunxfancy@gmail.com
* @Last modified by:   sxf
* @Last modified time: 2016-11-17
* @License: MIT License
*/



#pragma once
#include "SharedStack.hpp"

class Automaton;
class ParallelWorker;

class Combine {
public:
    Combine(ParallelWorker* pw);
    ~Combine() {

    }

    bool combineOutput(ParallelWorker* pw);
    const std::deque<int>& getOutput() const { return stack; }
private:
    const Automaton* first;
    std::deque<int> stack;
};
