#pragma once

#include <vector>
#include <string>

enum TokenType {
    Token_Definition,
    Token_Func,
    Token_If,
    Token_Else,
    Token_While,
    
    Token_ParenthesisOpen,
    Token_ParenthesisClose,
    Token_BraceOpen,
    Token_BraceClose,
    
    Token_Assignment,
    Token_Movement,
    Token_Dereference,
    Token_Comma,
    Token_Semicolon,
    
    Token_Plus,
    Token_Minus,
    Token_Mult,
    Token_Div,
    Token_Less,
    Token_Equal,
    
    Token_Identifier,
    Token_ConstString,
    Token_ConstInt,
    
    Token_Eof,
};

struct Token {
    TokenType token_type;
    std::string string_value;
    int int_value;
};

class TokenStream {
public:
    TokenStream(std::vector <Token> _token_stream);
    Token GetToken();
    void NextToken();

private:
    int pos;
    std::vector <Token> token_stream;
};
