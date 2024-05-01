#pragma once

#include <vector>

class AST;

void generate(std::shared_ptr <AST> ast, std::ostream &out);

struct GenerateContext {
    int nextLabel = 0;
    std::vector <std::string> variables;
};
