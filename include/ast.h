#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../include/generate.h"

class AST {
public:
    virtual void Generate(std::ostream &out, GenerateContext &context) = 0;
};

class Statement : public AST {

};

class Expression : public AST {

};

class Block : public Statement {
public:
    std::vector < std::shared_ptr <Statement> > statements;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class Definition : public Statement {
public:
    std::vector <std::string> identifiers;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class Assignment : public Statement {
public:
    std::string identifier;
    std::shared_ptr <Expression> expression;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class Movement : public Statement {
public:
    std::string identifier;
    std::shared_ptr <Expression> expression;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class If : public Statement {
public:
    std::shared_ptr <Expression> expression;
    std::shared_ptr <Block> block_then, block_else;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class While : public Statement {
public:
    std::shared_ptr <Expression> expression;
    std::shared_ptr <Block> block;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class FunctionDefinition : public Statement {
public:
    std::string name;
    std::vector <std::string> arguments;
    std::shared_ptr <Block> block;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class FunctionCall : public Statement {
public:
    std::string name;
    std::vector <std::shared_ptr <Expression>> arguments;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class Identifier : public Expression {
public:
    std::string identifier;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class ConstString : public Expression {
public:
    std::string const_string;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class ConstInt : public Expression {
public:
    int const_int;
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class BinaryOperator : public Expression {
public:
    std::shared_ptr <Expression> left, right;
};

class Addition : public BinaryOperator {
public:
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class Subtraction : public BinaryOperator {
public:
    void Generate(std::ostream &out, GenerateContext &context) override;
};

class Multiplication : public BinaryOperator {
public:
    void Generate(std::ostream &out, GenerateContext &context) override;
};
