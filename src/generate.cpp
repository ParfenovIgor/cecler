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

int findFunctionIndex(std::string name, std::vector <FunctionDescription> lst) {
    for (int i = 0; i < (int)lst.size(); i++) {
        if (name == lst[i].name) {
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
    expression->Generate(out, context);
    int idx = findVariableIndex(identifier, context.variables);
    if (idx == -1) {
        std::cout << "Semantic Error: undeclared variable " << identifier << std::endl;
        exit(1);
    }

    out << "    mov eax, dword [rsp - 4]" << std::endl;
    out << "    mov ebx, dword [rbp - " << (idx + 1) * 4 << "]" << std::endl;
    out << "    mov [ebx], eax" << std::endl;
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
    out << "    jmp .L" << label2 << std::endl;
    out << ".L" << label1 << ":" << std::endl;
    if (block_else) {
        block_else->Generate(out, context);
    }
    out << ".L" << label2 << ":" << std::endl;
}

void While::Generate(std::ostream &out, GenerateContext &context) {
    expression->Generate(out, context);

    int label1 = context.nextLabel;
    context.nextLabel++;
    int label2 = context.nextLabel;
    context.nextLabel++;

    out << ".L" << label1 << ":" << std::endl;
    out << "    mov eax, dword [rsp - 4]" << std::endl;
    out << "    test eax, eax" << std::endl;
    out << "    je .L" << label2 << std::endl;
    
    block->Generate(out, context);
    out << ".L" << label2 << ":" << std::endl;
}

std::vector <std::string> arg_reg = {
    "edi",
    "esi",
    "edx",
    "ecx",
    "r8d",
    "r9d"
};

void FunctionDefinition::Generate(std::ostream &out, GenerateContext &context) {
    context.functions.push_back({name, (int)arguments.size()});
    if (block) {
        out << "global " << name << std::endl;
    }
    else {
        out << "extern " << name << std::endl;
        return;
    }

    out << name << ":" << std::endl;
    out << "    push rbp" << std::endl;
    out << "    mov rbp, rsp" << std::endl;

    if (arguments.size() > arg_reg.size()) {
        std::cout << "Semantic Error: too many arguments in function definition" << std::endl;
        exit(1);
    }
    for (int i = 0; i < (int)arguments.size(); i++) {
        context.variables.push_back(arguments[i]);
        out << "    mov dword [rbp - " << (i + 1) * 4 << "], " << arg_reg[i] << std::endl;
    }
    out << "    sub rsp, " << (int)arguments.size() * 4 << std::endl;

    block->Generate(out, context);
    context.variables.clear();

    out << "    leave" << std::endl;
    out << "    ret" << std::endl;
}

void FunctionCall::Generate(std::ostream &out, GenerateContext &context) {
    int idx = findFunctionIndex(name, context.functions);
    if (idx == -1) {
        std::cout << "Semantic Error: undeclared function " << name << std::endl;
        exit(1);
    }

    if (arguments.size() != context.functions[idx].arg_num) {
        std::cout << "Semantic Error: wrong number of argument, expected " <<
            context.functions[idx].arg_num << " found " <<
            arguments.size() << std::endl;
        exit(1);
    }

    for (int i = 0; i < (int)arguments.size(); i++) {
        arguments[i]->Generate(out, context);
        context.variables.push_back("__var");
        out << "    sub rsp, 4" << std::endl;
    }
    out << "    add rsp, " << (int)arguments.size() * 4 << std::endl;
    for (int i = 0; i < (int)arguments.size(); i++) {
        out << "    mov " << arg_reg[i] << ", dword [rsp - " << (i + 1) * 4 << "]" << std::endl;
    }
    
    out << "    jmp " << name << std::endl;
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
    left->Generate(out, context);
    context.variables.push_back("__var");
    out << "    sub rsp, 4" << std::endl;
    right->Generate(out, context);
    context.variables.pop_back();
    out << "    add rsp, 4" << std::endl;

    out << "    mov eax, dword [rsp - 8]" << std::endl;
    out << "    add eax, dword [rsp - 4]" << std::endl;
    out << "    mov dword [rsp - 4], eax" << std::endl;
}

void Subtraction::Generate(std::ostream &out, GenerateContext &context) {
    left->Generate(out, context);
    context.variables.push_back("__var");
    out << "    sub rsp, 4" << std::endl;
    right->Generate(out, context);
    context.variables.pop_back();
    out << "    add rsp, 4" << std::endl;

    out << "    mov eax, dword [rsp - 8]" << std::endl;
    out << "    sub eax, dword [rsp - 4]" << std::endl;
    out << "    mov dword [rsp - 4], eax" << std::endl;

}

void Multiplication::Generate(std::ostream &out, GenerateContext &context) {
    left->Generate(out, context);
    context.variables.push_back("__var");
    out << "    sub rsp, 4" << std::endl;
    right->Generate(out, context);
    context.variables.pop_back();
    out << "    add rsp, 4" << std::endl;

    out << "    mov eax, dword [rsp - 8]" << std::endl;
    out << "    mul dword [rsp - 4]" << std::endl;
    out << "    mov dword [rsp - 4], eax" << std::endl;
}
