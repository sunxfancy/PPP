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
    stack = first->getLRStack();
}

// f is shorter than s
bool suffix(const deque<int>& f, const deque<int>& s) {
    if (f > s) return false;
    for (size_t i = 0; i < f.size(); i++) {
        if (f[i] != s[i]) return false;
    }
    return true;
}

bool Combine::combineOutput(ParallelWorker* pw) {
    auto f = deque<int>(stack.rbegin(), stack.rend());
    auto it = pw->getBeginEndMap().lower_bound(f);
    if (it != pw->getBeginEndMap().end() && it->first == f) {
        stack = it->second->getLRStack();
    } else {
        if (it != pw->getBeginEndMap().end() && suffix(it->first, f)) {
            for (size_t i = 0; i < it->first.size(); i++) {
                stack.pop_back();
            }
            for (auto i : it->second->getLRStack()) {
                stack.push_back(i);
            }
        } else
            return false;
    }
    return true;
}
