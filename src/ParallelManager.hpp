#pragma once

#include "Automaton.hpp"
#include "SharedStack.hpp"
#include "LALRTable.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <strstream>
#include <string>

#include <functional>

class ParallelWorker {
public:
    ParallelWorker(LALRTable* ptable, AutoCallback func) {
        this->ptable = ptable;
        this->func = func;
    }

    void init(int begin, int end, std::vector<Token>& data) {
        this->begin = begin;
        this->end = end;
        this->tokens = &data;
    }

    void run() {
        Automaton ato(this);
        ato.init(ptable, func, *tokens);
        ato.begin = ato.now = begin;
        ato.end = end;
        ato.run();
    }

    void finish(Automaton* ato) {
        Automaton* new_ato = new Automaton(*ato);
        be_map[ato->begin_stack] = new_ato;
    }

    void printAll() {
        printf("\n");
        for (auto p : be_map) {
            printStack(p.first);
            printf("-> ");
            printStack(p.second->getLRStack());
            printf("\n");

            printSymbol(p.second->begin_symbol);
            printf("-> ");
            printSymbol(p.second->SymbolStack);
            printf("\n");
        }
    }

    void printStack(const std::deque<int>& stack) {
        for (auto k : stack)
            printf("%d ", k);
    }

    void printSymbol(const std::deque<int>& stack) {
        for (auto k : stack) {
            if (k == 0) printf("$ ");
            else printf("%s ", ptable->vmap->find(k).c_str());
        }
    }

    long time;
    std::map< std::deque<int>, Automaton* >& getBeginEndMap() { return be_map; }
private:
    // 并行支持
    std::map< std::deque<int>, Automaton* > be_map;


    LALRTable* ptable;
    AutoCallback func;
    std::vector<Token>* tokens;
    int begin, end;
};


namespace std { class thread; }
class ParallelManager {
public:
    ParallelManager(const char* l, size_t ls, const char* p, size_t ps, AutoCallback func);
    void split(int n);
    void run_lex(const std::string& path);
    ParallelWorker* create_worker(int begin, int end);
private:
    LexInterface* lex;
    LALRTable* ptable;
    std::string data;
    std::vector<Token> tokens;
    std::vector<std::thread*> threads;
    AutoCallback func;
    long time_avg, time_fc;
    bool combine(std::vector<ParallelWorker*>& pws);
    void fileReader(const std::string& path);
    void countTime(std::vector<ParallelWorker*>& pws);
};
