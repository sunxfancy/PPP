#ifndef PARALLEL_MANAGER_HPP
#define PARALLEL_MANAGER_HPP


#include "Automaton.hpp"
#include "SharedStack.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <strstream>

class ParallelWorker {
public:
    ParallelWorker(LALRTable* ptable, AutoCallback func) {
        this->ptable = ptable;
        this->func = func;
    }

    void init(int begin, int end, Token** data) {
        this->begin = begin;
        this->end = end;
        this->tokens = data;
    }

    void run() {
        Automaton ato(this);
        ato.init(ptable, func, tokens);
        ato.run();
    }

    void finish(Automaton* other) {
        Automaton* new_ato = new Automaton(*other);
        be_map[&(new_ato->begin_stack)] = new_ato;
    }

private:
    // 并行支持
    std::map< SharedStack<int>*, Automaton* > be_map;
    LALRTable* ptable;
    AutoCallback func;
    Token** tokens;
    int begin, end;
};




class ParallelManager {
public:
    ParallelManager(std::istrstream l, std::istrstream p, AutoCallback func) {
        lex = LexInterface::Create();
        this->func = func;
    }

    void split(int n) {
        int size = data.size();
        for (int i = 0; i < n; ++i) {
            ParallelWorker* pm = new ParallelWorker(ptable, func);
        }
    }

    void run_lex(const std::string& path) {
        fileReader(path);
        lex->Init(data.c_str());

    }

    void fileReader(const std::string& path) {
        std::ifstream t(path, std::ios::binary);

        t.seekg(0, std::ios::end);
        data.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        data.assign((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    }

private:
    LexInterface* lex;
    LALRTable* ptable;
    string data;
    std::vector<Token*> tokens;
    AutoCallback func;
};


#endif /* end of include guard: PARALLEL_MANAGER_HPP */
