#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/error.h"

bool check(
    std::string buffer,
    int &x,
    int &line,
    int &position,
    std::string &filename,
    std::string word,
    bool keyword,
    std::vector <Token> &token_stream,
    TokenType token_type) {

    int len = word.size();
    int sz = buffer.size();
    if (x + len <= sz && buffer.substr(x, len) == word && (!keyword || x + len == sz || !isalpha(buffer[x + len]))) {
        token_stream.push_back({token_type, "", 0, line, position, line, position + len - 1, filename});
        x += len;
        position += len;
        return true;
    }
    else return false;
}

std::vector <Token> lexer(std::string filename) {
    std::ifstream fin(filename);
    if (!fin) {
        fprintf(stderr, "Could not open file: %s\n", filename.c_str());
        exit(1);
    }
    std::stringstream ss;
    ss << fin.rdbuf();
    std::string buffer = ss.str();

    std::vector <Token> token_stream;

    int sz = buffer.size();
    int line = 0, position = 0;
    for (int x = 0; x < sz;) {
        if (x + 2 <= sz && buffer[x] == '/' && buffer[x + 1] == '/') {
            while (x < sz && buffer[x] != '\n') x++;
            if (x < sz) x++;
            line++;
            position = 0;
        }
        else if (x + 7 <= sz && buffer.substr(x, 7) == "include") {
            int position_begin = position, line_begin = line;
            while (x < sz && buffer[x] != '"') { 
                if (buffer[x] == '\n') {
                    line++;
                    position = 0;
                }
                else position++;
                x++;
            }
            std::string filename;
            if (x < sz) {
                x++;
                position++;
            }
            else {
                lexerError("\" expected in include", line_begin, position_begin, line, position, filename.c_str());
            }
            while (x < sz && buffer[x] != '"') {
                filename.push_back(buffer[x]);
                if (buffer[x] == '\n') {
                    line++;
                    position = 0;
                }
                else position++;
                x++;
            }
            if (x < sz) {
                x++;
                position++;
            }
            else {
                lexerError("\" expected in include", line_begin, position_begin, line, position, filename.c_str());
            }
            std::vector <Token> _token_stream = lexer(filename);
            for (Token token : _token_stream)
                token_stream.push_back(token);
        }
        else if (buffer[x] == '"') {
            int position_begin = position, line_begin = line;
            position++;
            x++;
            std::string str;
            while (x < sz && buffer[x] != '"') {
                str.push_back(buffer[x]);
                if (buffer[x] == '\n') {
                    line++;
                    position = 0;
                }
                else position++;
                x++;
            }
            if (x < sz) {
                x++;
                position++;
            }
            else {
                lexerError("\" expected in const string", line_begin, position_begin, line, position, filename.c_str());
            }
            token_stream.push_back({Token_ConstString, str, 0, position_begin, line_begin, position - 1, line, filename});
        }
        else if (isdigit(buffer[x])) {
            std::string str;
            int position_begin = position;
            while (x < sz && isdigit(buffer[x])) {
                str.push_back(buffer[x]);
                x++;
                position++;
            }
            token_stream.push_back({Token_ConstInt, "", atoi(str.c_str()), line, position_begin, line, position - 1, filename});
        }

        else if (check(buffer, x, line, position, filename, "def", true, token_stream, Token_Definition));
        else if (check(buffer, x, line, position, filename, "func", true, token_stream, Token_Func));
        else if (check(buffer, x, line, position, filename, "if", true, token_stream, Token_If));
        else if (check(buffer, x, line, position, filename, "else", true, token_stream, Token_Else));
        else if (check(buffer, x, line, position, filename, "while", true, token_stream, Token_While));

        else if (check(buffer, x, line, position, filename, "(", false, token_stream, Token_ParenthesisOpen));
        else if (check(buffer, x, line, position, filename, ")", false, token_stream, Token_ParenthesisClose));
        else if (check(buffer, x, line, position, filename, "{", false, token_stream, Token_BraceOpen));
        else if (check(buffer, x, line, position, filename, "}", false, token_stream, Token_BraceClose));
        else if (check(buffer, x, line, position, filename, ":=", false, token_stream, Token_Assignment));
        else if (check(buffer, x, line, position, filename, "<-", false, token_stream, Token_Movement));
        else if (check(buffer, x, line, position, filename, "$", false, token_stream, Token_Dereference));
        else if (check(buffer, x, line, position, filename, ",", false, token_stream, Token_Comma));
        else if (check(buffer, x, line, position, filename, ";", false, token_stream, Token_Semicolon));
        else if (check(buffer, x, line, position, filename, "+", false, token_stream, Token_Plus));
        else if (check(buffer, x, line, position, filename, "-", false, token_stream, Token_Minus));
        else if (check(buffer, x, line, position, filename, "*", false, token_stream, Token_Mult));
        else if (check(buffer, x, line, position, filename, "/", false, token_stream, Token_Div));
        else if (check(buffer, x, line, position, filename, "<", false, token_stream, Token_Less));
        else if (check(buffer, x, line, position, filename, "=", false, token_stream, Token_Equal));

        else if (isalpha(buffer[x]) || buffer[x] == '_') {
            std::string str;
            int position_begin = position;
            while (x < sz && (isalnum(buffer[x]) || buffer[x] == '_')) {
                str.push_back(buffer[x]);
                x++;
                position++;
            }
            token_stream.push_back({Token_Identifier, str, 0, line, position_begin, line, position - 1, filename});
        }
        else if (buffer[x] == ' ' || buffer[x] == '\r' || buffer[x] == '\t') {
            x++;
            position++;
        }
        else if (buffer[x] == '\n') {
            x++;
            line++;
            position = 0;
        }
        
        else {
            lexerError("unexpected symbol", line, position, line, position, filename.c_str());
        }
    }

    return token_stream;
}
