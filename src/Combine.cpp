/**
* @Author: Sun Xiaofan <sxf>
* @Date:   2016-10-26
* @Email:  sunxfancy@gmail.com
* @Last modified by:   sxf
* @Last modified time: 2016-11-17
* @License: MIT License
*/



#include "Combine.hpp"
#include "Automaton.hpp"
#include "ParallelManager.hpp"

Combine::Combine(ParallelWorker* pw)  {
    first = pw->getBeginEndMap().begin()->second;
}

bool suffix(const deque<int>& f, const deque<int>& s) {
    
}

bool Combine::combineOutput(ParallelWorker* pw) {
    auto f = deque<int>(first->getLRStack().rbegin(), first->getLRStack().rend())
    auto it = pw->getBeginEndMap().lower_bound();
    if (it != pw->getBeginEndMap().end() && it->second == f) {
        first = it->second;
    } else {
        if (it != pw->getBeginEndMap().end() && suffix(f, it->second))
        else return false;
    }
    return true;
}
