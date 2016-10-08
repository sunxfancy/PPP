#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <strstream>
#include "Automaton.hpp"

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
    if (argc <= 0 || argc > 2) {
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

    std::istrstream is_lex(getLexTable());
    std::istrstream is_parser(getLALRTable());

    getAction= (AutoCallback)dlsym(handle, "__ppp_script");

    dlclose(handle);
    return 0;
}
