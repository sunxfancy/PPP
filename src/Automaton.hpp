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

    std::deque<int>& getLRStack() { return LRStack; }
    // begin stack for the map
    std::deque<int> begin_stack;

    int begin;
    int now;
    int end;
private:
    const Token* reader();
    const Token* reader(int x);
    const std::vector<Token>* tokens;
    int max_stack;

    void Shift(int x, const Token* t);

    int Reduce(int x);
    int Reduce(int size, int Vn, int x);

    void findStack(int len, int shift_size);
    /* data */
    LALRTable* table;
    AutoCallback function;

    // 分析栈
    std::deque<int> LRStack;
    std::deque<void*> NodeStack;

    ParallelWorker* pm;

    int shift_size = 1;
};


#endif /* end of include guard: AUTOMATON_HPP */
