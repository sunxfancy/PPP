#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "Automaton.hpp"
#include "ParallelManager.hpp"
typedef const char* (*getLexTable_t)();
typedef const char* (*getLALRTable_t)();

getLexTable_t getLexTable;
getLALRTable_t getLALRTable;
AutoCallback getAction;

const char* help_msg = "Please give the input file\n";



int main(int argc, char const *argv[]) {
    /* code */
    void *handle;
    char *error;
    if (argc <= 0 || argc > 3) {
        fprintf (stderr, "%s\n", help_msg);
        return 0;
    }
    if (argc == 1) {
        handle = dlopen ("libparser.so", RTLD_LAZY);
    } else {
        handle = dlopen (argv[2], RTLD_LAZY);
    }

    if (!handle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }
    dlerror();    /* Clear any existing error */
    getLexTable = (getLexTable_t)dlsym(handle, "__getLexTable");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    getLALRTable = (getLALRTable_t)dlsym(handle, "__getLALRTable");

    getAction= (AutoCallback)dlsym(handle, "__ppp_script");

    ParallelManager* pm = new ParallelManager(getLexTable(), getLALRTable(), getAction);
    pm->run_lex(argv[1]);

    if (argc == 3)
        pm->split(atoi(argv[3]));
    else
        pm->split(8);

    dlclose(handle);
    delete pm;
    return 0;
}
