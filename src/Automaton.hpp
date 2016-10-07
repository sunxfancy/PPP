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

    void init (LALRTable* ptable, AutoCallback func);
    void run ();
    void run_from (int state = -1);

    std::deque<int>& getLRStack() { return LRStack; }
    // begin stack for the map
    SharedStack<int> begin_stack;

    int begin;
    int now;
    int end;
private:
    Token* reader();
    Token* reader(int x);
    Token** tokens;
    int max_stack;

    void Shift(int x, Token* t);

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
    std::deque<void*> NodeStack;

    ParallelWorker* pm;
};


#endif /* end of include guard: AUTOMATON_HPP */
