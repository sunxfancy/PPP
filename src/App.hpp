//
// Created by sxf on 16-10-24.
//

#pragma once

#include <string>
#include "Automaton.hpp"
//#include <dlfcn.h>
#include "ParallelManager.hpp"
#include <string>
#include <fstream>
#include <streambuf>

void* getAction(int bnf_num, const std::vector<void*>& args);

class App {
public:
    App(const std::string& path, const std::string& lexer, const std::string& parser, int threads) {
        this->path = path; this->lexer = lexer; this->parser = parser; this->threads = threads;
    }
    ~App() {
        if (pm != nullptr) delete pm;
//        if (handle != nullptr) dlclose(handle);
    }


    void LoadFile() {
        if (pm != nullptr) delete pm;
        string lexer_data, parser_data;
        readWholeFile(lexer, lexer_data);
        readWholeFile(parser, parser_data);
        pm = new ParallelManager(lexer_data.c_str(), lexer_data.size(), parser_data.c_str(), parser_data.size(), getAction);
    }
    /*
    void loadLibrary() {
        handle = dlopen (lib.c_str(), RTLD_LAZY);
        char *error;
        if (!handle) {
            fprintf (stderr, "%s\n", dlerror());
            exit(1);
        }
        dlerror();    // Clear any existing error
        getLexTable = (getLexTable_t)dlsym(handle, "__getLexTable");
        getLexTableSize = (getLexTableSize_t)dlsym(handle, "__getLexTableSize");
        getLALRTable = (getLALRTable_t)dlsym(handle, "__getLALRTable");
        getLALRTableSize = (getLALRTableSize_t)dlsym(handle, "__getLALRTableSize");
        getAction= (AutoCallback)dlsym(handle, "__ppp_script");
        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        if (pm != nullptr) delete pm;
        pm = new ParallelManager(getLexTable(), getLexTableSize(), getLALRTable(), getLALRTableSize(), getAction);
    }
    */

    void run() {
        run_lex();
        run_threads();
    }

    void run_lex() {
        pm->run_lex(path);
    }

    void run_threads(int t = -1) {
        if (t == -1)
            pm->split(threads);
        else
            pm->split(t);
    }

    void readWholeFile(const string& path, string& str) {

        std::ifstream t(path);

        t.seekg(0, std::ios::end);
        str.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());
    }

    ParallelManager* pm = nullptr;
private:
    std::string path;
    std::string lexer, parser;

    int threads;

//    void *handle;
//    typedef const char* (*getLexTable_t)();
//    typedef const char* (*getLALRTable_t)();
//    typedef size_t (*getLexTableSize_t)();
//    typedef size_t (*getLALRTableSize_t)();
//    getLexTable_t getLexTable;
//    getLALRTable_t getLALRTable;
//    getLexTableSize_t getLexTableSize;
//    getLALRTableSize_t getLALRTableSize;
//    AutoCallback getAction;
};


