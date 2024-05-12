#pragma once

#include <vector>

class AST;

void generate(std::shared_ptr <AST> ast, std::ostream &out);

struct FunctionDescription {
    std::string name;
    int arg_num;
};

struct GenerateContext {
    int nextLabel = 0;
    std::vector <std::string> variables;
    std::vector <FunctionDescription> functions;
};
