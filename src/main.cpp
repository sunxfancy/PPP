#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "Automaton.hpp"
#include "ParallelManager.hpp"
typedef const char* (*getLexTable_t)();
typedef const char* (*getLALRTable_t)();
typedef size_t (*getLexTableSize_t)();
typedef size_t (*getLALRTableSize_t)();

getLexTable_t getLexTable;
getLALRTable_t getLALRTable;
getLexTableSize_t getLexTableSize;
getLALRTableSize_t getLALRTableSize;
AutoCallback getAction;

const char* help_msg = "Please give the input file\n";

extern
void HexOutput(const char* buf, size_t size)
{
    printf("The Hex output of data:\n");
    for(size_t i=0; i < size; ++i)
    {
        unsigned char c = buf[i]; // must use unsigned char to print >128 value
        if( c< 16) printf("0%x ", c);
        else printf("%x ", c);
        if (i % 4 == 0) printf(" ");
        if (i % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {
    /* code */
    void *handle;
    char *error;
    if (argc <= 1 || argc > 4) {
        fprintf (stderr, "%s\n", help_msg);
        return 0;
    }
    if (argc == 2) {
        handle = dlopen ("./libparser.so", RTLD_LAZY);
    } else {
        handle = dlopen (argv[2], RTLD_LAZY);
    }

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

    ParallelManager* pm = new ParallelManager(getLexTable(), getLexTableSize(), getLALRTable(), getLALRTableSize(), getAction);
    pm->run_lex(argv[1]);

    if (argc == 4)
        pm->split(atoi(argv[3]));
    else
        pm->split(8);

    dlclose(handle);
    delete pm;
    return 0;
}
