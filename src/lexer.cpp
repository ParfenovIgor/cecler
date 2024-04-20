#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "../include/lexer.h"
#include "../include/token.h"

bool check(std::string buffer, int &x, std::string word, bool keyword, std::vector <Token> &token_stream, TokenType token_type) {
    int len = word.size();
    int sz = buffer.size();
    if (x + len <= sz && buffer.substr(x, len) == word && (!keyword || x + len == sz || !isalpha(buffer[x + len]))) {
        token_stream.push_back({token_type, "", 0});
        x += len;
        return true;
    }
    else return false;
}

TokenStream lexer(std::string buffer) {
    std::vector <Token> token_stream;

    int sz = buffer.size();
    for (int x = 0; x < sz;) {
        if (x + 2 <= sz && buffer[x] == '/' && buffer[x + 1] == '/') {
            while (x < sz && buffer[x] != '\n') x++;
            if (x < sz) x++;
        }
        else if (buffer[x] == '"') {
            x++;
            std::string str;
            while (x < sz && buffer[x] != '"') {
                str.push_back(buffer[x]);
                x++;
            }
            if (x < sz) x++;
            else {
                std::cout << "Lexer Error: \" expected" << std::endl;
                exit(1);
            }
            token_stream.push_back({Token_ConstString, str, 0});
        }
        else if (isdigit(buffer[x])) {
            std::string str;
            while (x < sz && isdigit(buffer[x])) {
                str.push_back(buffer[x]);
                x++;
            }
            token_stream.push_back({Token_ConstInt, "", atoi(str.c_str())});
        }

        else if (check(buffer, x, "def", true, token_stream, Token_Definition));
        else if (check(buffer, x, "func", true, token_stream, Token_Func));
        else if (check(buffer, x, "if", true, token_stream, Token_If));
        else if (check(buffer, x, "else", true, token_stream, Token_Else));
        else if (check(buffer, x, "while", true, token_stream, Token_While));

        else if (check(buffer, x, "(", false, token_stream, Token_ParenthesisOpen));
        else if (check(buffer, x, ")", false, token_stream, Token_ParenthesisClose));
        else if (check(buffer, x, "{", false, token_stream, Token_BraceOpen));
        else if (check(buffer, x, "}", false, token_stream, Token_BraceClose));
        else if (check(buffer, x, ":=", false, token_stream, Token_Assignment));
        else if (check(buffer, x, "<-", false, token_stream, Token_Movement));
        else if (check(buffer, x, "$", false, token_stream, Token_Dereference));
        else if (check(buffer, x, ",", false, token_stream, Token_Comma));
        else if (check(buffer, x, ";", false, token_stream, Token_Semicolon));
        else if (check(buffer, x, "+", false, token_stream, Token_Plus));
        else if (check(buffer, x, "-", false, token_stream, Token_Minus));
        else if (check(buffer, x, "*", false, token_stream, Token_Mult));
        else if (check(buffer, x, "/", false, token_stream, Token_Div));
        else if (check(buffer, x, "<", false, token_stream, Token_Less));
        else if (check(buffer, x, "=", false, token_stream, Token_Equal));

        else if (isalpha(buffer[x])) {
            std::string str;
            while (x < sz && isalnum(buffer[x])) {
                str.push_back(buffer[x]);
                x++;
            }
            token_stream.push_back({Token_Identifier, str, 0});
        }
        else if (buffer[x] == ' ' || buffer[x] == '\n' || buffer[x] == '\r' || buffer[x] == '\t') x++;
        
        else {
            std::cout << "Lexer Error: unexpected symbol" << ' ' << x << ' ' << buffer[x] << std::endl;
            exit(1);
        }
    }

    token_stream.push_back({Token_Eof, "", 0});

    return TokenStream(token_stream);
}
