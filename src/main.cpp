#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <strstream>

typedef const char* (*getLexTable_t)();
typedef const char* (*getLALRTable_t)();
typedef const char* (*getAction_t)();

extern "C" {
    getLexTable_t getLexTable;
    getLALRTable_t getLALRTable;
    getAction_t getAction;
}


int main(int argc, char const *argv[]) {
    /* code */
    void *handle;
    char *error;
    handle = dlopen (argv[1], RTLD_LAZY);
    if (!handle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }
    dlerror();    /* Clear any existing error */
    getLexTable = (getLexTable_t)dlsym(handle, "getLexTable");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    getLALRTable = (getLALRTable_t)dlsym(handle, "getLALRTable");

    std::istrstream is_lex(getLexTable());
    std::istrstream is_parser(getLALRTable());

    dlclose(handle);
    return 0;
}
