#pragma once

#include <vector>
#include <string>
#include <memory>

class AST {

};

class Statement : public AST {

};

class Expression : public AST {

};

class Block : public Statement {
public:
    std::vector < std::shared_ptr <Statement> > statements;
};

class Definition : public Statement {
public:
    std::vector <std::string> identifiers;
};

class Assignment : public Statement {
public:
    std::string identifier;
    std::shared_ptr <Expression> expression;
};

class Movement : public Statement {
public:
    std::string identifier;
    std::shared_ptr <Expression> expression;
};

class If : public Statement {
public:
    std::shared_ptr <Expression> expression;
    std::shared_ptr <Block> block_then, block_else;
};

class While : public Statement {
public:
    std::shared_ptr <Expression> expression;
    std::shared_ptr <Block> block;
};

class FunctionDefinition : public Statement {
public:
    std::string name;
    std::vector <std::string> arguments;
    std::shared_ptr <Block> block;
};

class FunctionCall : public Statement {
public:
    std::string name;
    std::vector <std::shared_ptr <Expression>> arguments;
};

class Identifier : public Expression {
public:
    std::string identifier;
};

class ConstString : public Expression {
public:
    std::string const_string;
};

class ConstInt : public Expression {
public:
    int const_int;
};

class BinaryOperator : public Expression {
public:
    std::shared_ptr <Expression> left, right;
};

class Addition : public BinaryOperator {

};

class Subtraction : public BinaryOperator {

};

class Multiplication : public BinaryOperator {

};
