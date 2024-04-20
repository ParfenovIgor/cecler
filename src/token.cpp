#include "../include/token.h"

TokenStream::TokenStream(std::vector <Token> _token_stream)
    : token_stream(_token_stream) {
    pos = 0;
}

Token TokenStream::GetToken() {
    return token_stream[pos];
}

void TokenStream::NextToken() {
    pos++;
}
