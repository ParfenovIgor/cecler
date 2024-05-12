#pragma once

#include <string>

struct Options {
    std::string filename_in, filename_out;
    bool assemble, link;
};
