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
