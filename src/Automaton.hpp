/**
* @Author: Sun Xiaofan <sxf>
* @Date:   2016-10-29
* @Email:  sunxfancy@gmail.com
* @Last modified by:   sxf
* @Last modified time: 2016-11-17
* @License: MIT License
*/



#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include <stack>
#include <map>
#include <vector>
#include "SharedStack.hpp"
#include "LexInterface.h"

class LALRTable;
class ParallelWorker;
typedef void* (*AutoCallback)(int bnf_num, const std::vector<void*>& args);

class Automaton {
public:
    Automaton (ParallelWorker* pm);
    Automaton (const Automaton&);
    virtual ~Automaton ();

    void init (LALRTable* ptable, AutoCallback func, const std::vector<Token>& data);
    void run ();
    void run_from (int state = -1);

    const std::deque<int>& getLRStack() const { return LRStack; }
    // begin stack for the map
    std::deque<int> begin_stack;

    std::deque<int> SymbolStack;
    std::deque<int> begin_symbol;

    int begin;
    int now;
    int end;
private:
    const Token* reader();
    const Token* reader(int x);

    const std::vector<Token>* tokens;

    void Shift(int x, const Token* t);
    int Reduce(int x);

    int Reduce(int size, int Vn, int x);
    void findStack(int x, int Vn);
    /* data */
    LALRTable* table;

    AutoCallback function;
    // 分析栈
    std::deque<int> LRStack;
//    std::deque<void*> NodeStack;

    ParallelWorker* pm;
};


#endif /* end of include guard: AUTOMATON_HPP */
