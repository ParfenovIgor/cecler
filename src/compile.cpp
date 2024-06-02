#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cassert>
#include "../include/compile.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/syntax.h"
#include "../include/generate.h"
#include "../include/ast.h"

template <std::size_t N>
int execvp(const char* file, const char* const (&argv)[N]) {
    assert((N > 0) && (argv[N - 1] == nullptr));
    return execvp(file, const_cast<char* const*>(argv));
}

void compile(Options options) {
    std::vector <Token> token_vector = lexer(options.filename_in);
    token_vector.push_back({Token_Eof, "", 0, 0, 0, 0, 0});

    std::shared_ptr <AST> ast = syntax(TokenStream(token_vector));

    std::string generate_filename;
    if (options.assemble) generate_filename = ".tmpasm";
    else generate_filename = options.filename_out;
    
    std::ofstream fout(generate_filename);
    generate(ast, fout);

    if (options.assemble) {
        std::string assemble_filename;
        if (options.link) assemble_filename = ".tmpo";
        else assemble_filename = options.filename_out;

        int pid = fork();
        if (pid == -1) {
            std::cout << "Fork failed" << std::endl;
            exit(1);
        }
        if (pid == 0) {
            const char* const args[] = { "nasm", "-f", "elf64", generate_filename.c_str(), "-o", assemble_filename.c_str(), NULL };
            execvp(args[0], args);
            std::cout << "Exec failed" << std::endl;
            exit(1);
        }
        waitpid(pid, NULL, 0);


        if (options.link) {
            std::string link_filename = options.filename_out;

            int pid = fork();
            if (pid == -1) {
                std::cout << "Fork failed" << std::endl;
                exit(1);
            }
            if (pid == 0) {
                const char* const args[] = { "ld", assemble_filename.c_str(), "-o", link_filename.c_str(), NULL };
                execvp(args[0], args);
                std::cout << "Exec failed" << std::endl;
                exit(1);
            }
            waitpid(pid, NULL, 0);
        }
    }
}
