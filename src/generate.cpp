#include <iostream>
#include "../include/ast.h"
#include "../include/generate.h"

int findVariableIndex(std::string name, std::vector <std::string> lst) {
    for (int i = 0; i < (int)lst.size(); i++) {
        if (name == lst[i]) {
            return i;
        }
    }
    return -1;
}

void generate(std::shared_ptr <AST> ast, std::ostream &out) {
    out << "section .text" << std::endl;
    GenerateContext context;
    ast->Generate(out, context);
}

void Block::Generate(std::ostream &out, GenerateContext &context) {
    for (int i = 0; i < (int)statements.size(); i++) {
        statements[i]->Generate(out, context);
    }
}

void Definition::Generate(std::ostream &out, GenerateContext &context) {
    for (int i = 0; i < (int)identifiers.size(); i++) {
        context.variables.push_back(identifiers[i]);
    }
    out << "    sub rsp, " << (int)identifiers.size() * 4 << std::endl;
}

void Assignment::Generate(std::ostream &out, GenerateContext &context) {
    expression->Generate(out, context);
    int idx = findVariableIndex(identifier, context.variables);
    if (idx == -1) {
        std::cout << "Semantic Error: undeclared variable " << identifier << std::endl;
        exit(1);
    }

    out << "    mov eax, dword [rsp - 4]" << std::endl;
    out << "    mov dword [rbp - " << (idx + 1) * 4 << "], eax" << std::endl;
}

void Movement::Generate(std::ostream &out, GenerateContext &context) {

}

void If::Generate(std::ostream &out, GenerateContext &context) {
    expression->Generate(out, context);

    int label1 = context.nextLabel;
    context.nextLabel++;
    int label2 = context.nextLabel;
    context.nextLabel++;

    out << "    mov eax, dword [rsp - 4]" << std::endl;
    out << "    test eax, eax" << std::endl;
    out << "    je .L" << label1 << std::endl;
    
    block_then->Generate(out, context);
    out << ".L" << label1 << ":" << std::endl;
    if (block_else) {
        block_else->Generate(out, context);
    }
    out << ".L" << label2 << ":" << std::endl;
}

void While::Generate(std::ostream &out, GenerateContext &context) {

}

void FunctionDefinition::Generate(std::ostream &out, GenerateContext &context) {
    out << "global " << name << std::endl;
    out << name << ":" << std::endl;
    out << "    push rbp" << std::endl;
    out << "    mov rbp, rsp" << std::endl;

    block->Generate(out, context);
    context.variables.clear();

    out << "    leave" << std::endl;
    out << "    ret" << std::endl;
}

void FunctionCall::Generate(std::ostream &out, GenerateContext &context) {

}

void Identifier::Generate(std::ostream &out, GenerateContext &context) {
    int idx = findVariableIndex(identifier, context.variables);
    if (idx == -1) {
        std::cout << "Semantic Error: undeclared variable " << identifier << std::endl;
        exit(1);
    }

    out << "    mov eax, dword [rbp - " << (idx + 1) * 4 << "]" << std::endl;
    out << "    mov dword [rsp - 4], eax" << std::endl;
}

void ConstString::Generate(std::ostream &out, GenerateContext &context) {

}

void ConstInt::Generate(std::ostream &out, GenerateContext &context) {
    out << "    mov dword [rsp - 4], " << const_int << std::endl;
}

void Addition::Generate(std::ostream &out, GenerateContext &context) {

}

void Subtraction::Generate(std::ostream &out, GenerateContext &context) {

}

void Multiplication::Generate(std::ostream &out, GenerateContext &context) {

}
