#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include "../include/compile.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/syntax.h"
#include "../include/ast.h"

void compile(std::string filename_in, std::string filename_out) {
    std::ifstream fin(filename_in);
    std::stringstream ss;
    ss << fin.rdbuf();
    std::string buffer = ss.str();

    TokenStream token_stream = lexer(buffer);
    std::shared_ptr <AST> ast = syntax(token_stream);
}
