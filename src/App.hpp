//
// Created by sxf on 16-10-24.
//

#pragma once

#include <string>
#include "Automaton.hpp"
#include <dlfcn.h>
#include "ParallelManager.hpp"

class App {
public:
    App(const std::string& path, const std::string& lib, int threads) {
        this->path = path; this->lib = lib; this->threads = threads;
    }
    ~App() {
        if (pm != nullptr) delete pm;
        if (handle != nullptr) dlclose(handle);
    }

    void loadLibrary() {
        handle = dlopen (lib.c_str(), RTLD_LAZY);
        char *error;
        if (!handle) {
            fprintf (stderr, "%s\n", dlerror());
            exit(1);
        }
        dlerror();    /* Clear any existing error */
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

    void run() {
        run_lex();
        run_threads();
    }

    void run_lex() {
        pm->run_lex(path);
    }

    void run_threads() {
        pm->split(threads);
    }

    typedef const char* (*getLexTable_t)();
    typedef const char* (*getLALRTable_t)();
    typedef size_t (*getLexTableSize_t)();
    typedef size_t (*getLALRTableSize_t)();

    ParallelManager* pm = nullptr;
private:
    std::string path;
    std::string lib;
    int threads;

    void *handle;
    getLexTable_t getLexTable;
    getLALRTable_t getLALRTable;
    getLexTableSize_t getLexTableSize;
    getLALRTableSize_t getLALRTableSize;
    AutoCallback getAction;
};


