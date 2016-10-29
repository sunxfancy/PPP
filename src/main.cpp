#include <stdio.h>
#include <string>
#include "App.hpp"
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
    std::string path;
    std::string lib;
    int threads;

    if (argc <= 1 || argc > 4) {
        fprintf (stderr, "%s\n", help_msg);
        return 0;
    }
    path = argv[1];
    if (argc == 2) {
        lib = "libparser.so";
    } else {
        lib = argv[2];
    }

    if (argc == 4)
        threads = atoi(argv[3]);
    else
        threads = 8;

    App app(path, lib, threads);
    app.loadLibrary();
    app.run();

    return 0;
}

