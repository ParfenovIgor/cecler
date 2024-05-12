#include <string>
#include <iostream>
#include <cstring>
#include "../include/compile.h"
#include "../include/options.h"

void help() {
    std::cout << "cecler compiler" << std::endl;
    std::cout << "Usage: cecler [options] file..." << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -S                       Compile only; do not assemble or link." << std::endl;
    std::cout << "  -c                       Compile and assemble, but do not link." << std::endl;
    std::cout << "  -o <file>                Place the output into <file>." << std::endl;
    exit(0);
}

int main(int argc, const char **argv) {
    Options options;
    options.assemble = true;
    options.link = true;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-S") == 0) {
            options.assemble = false;
            options.link = false;
        }
        else if (strcmp(argv[i], "-c") == 0) {
            options.link = false;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) help();
            options.filename_out = std::string(argv[i + 1]);
            i++;
        }
        else {
            options.filename_in = std::string(argv[i]);
        }
    }

    if (options.filename_in.empty()) help();
    if (options.filename_out.empty()) options.filename_out = "out";
    compile(options);

    return 0;
}
