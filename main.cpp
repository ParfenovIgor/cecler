#include <string>
#include <iostream>
#include <cstring>
#include "lexer.h"

void help() {
    std::cout << "cecler compiler" << std::endl;
    std::cout << "Usage: cecler [options] file..." << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -o <file>                Place the output into <file>." << std::endl;
    exit(0);
}

int main(int argc, const char **argv) {
    std::string filename_in, filename_out;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) help();
            filename_out = std::string(argv[i + 1]);
            i++;
        }
        else {
            filename_in = std::string(argv[i]);
        }
    }

    if (filename_in.empty()) help();
    if (filename_out.empty()) filename_out = "out";
    lexer(filename_in, filename_out);

    return 0;
}
