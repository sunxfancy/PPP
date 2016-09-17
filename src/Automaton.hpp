#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include <stack>
#include <map>
#include <vector>
#include "SharedStack.hpp"
#include "LexInterface.h"

class LALRTable;
class ParallelManager;
typedef void* (*AutoCallback)(int bnf_num, const std::vector<void*>& args);

class Automaton {
public:
    Automaton (ParallelManager* pm);
    Automaton (const Automaton&);
    virtual ~Automaton ();

    void init (LALRTable* ptable, AutoCallback func);
    void run ();
    void run_from (int state);

    std::deque<int>& getLRStack() { return LRStack; }
    // begin stack for the map
    SharedStack<int> begin_stack;
private:
    Token* reader(int x, int num);
    void Shift(int x, int t, void* ptr);

    // 三个参数分别是，要规约的语法项id，规约时的父节点
    int Reduce(int x);

    void findStack(int len);

    /* data */
    LALRTable* table;
    AutoCallback function;
    // 词法分析程序
    LexInterface* lex;

    // 分析栈
    std::deque<int> LRStack;
    // 节点栈
    std::deque<int> NodeIntStack;
    std::deque<void*> NodeStack;


    ParallelManager* pm;
};


#endif /* end of include guard: AUTOMATON_HPP */
