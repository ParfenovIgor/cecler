#include <iostream>
#include "../include/syntax.h"

std::shared_ptr <Block> process_block(TokenStream &token_stream);
std::shared_ptr <Statement> process_statement(TokenStream &token_stream);
std::shared_ptr <Expression> process_expression(TokenStream &token_stream);
std::shared_ptr <Expression> process_primary(TokenStream &token_stream);

std::shared_ptr <AST> syntax(TokenStream token_stream) {
    return process_block(token_stream);
}

std::shared_ptr <Block> process_block(TokenStream &token_stream) {
    std::shared_ptr <Block> _block = std::make_shared <Block> ();
    if (token_stream.GetToken().token_type != Token_BraceOpen) {
        std::cout << "Syntax Error: { expected" << std::endl;
        exit(1);
    }
    token_stream.NextToken();
    while (token_stream.GetToken().token_type != Token_BraceClose &&
           token_stream.GetToken().token_type != Token_Eof) {
        std::shared_ptr <Statement> _statement = process_statement(token_stream);
        _block->statements.push_back(_statement);
    }
    if (token_stream.GetToken().token_type != Token_BraceClose) {
        std::cout << "Syntax Error: } expected" << std::endl;
        exit(1);
    }
    token_stream.NextToken();
    return _block;
}

std::shared_ptr <Statement> process_statement(TokenStream &token_stream) {
    if (token_stream.GetToken().token_type == Token_Definition) {
        std::shared_ptr <Definition> _definition = std::make_shared <Definition> ();
        token_stream.NextToken();
        if (token_stream.GetToken().token_type != Token_Identifier) {
            std::cout << "Syntax Error: identifier expected" << std::endl;
            exit(1);
        }
        _definition->identifiers.push_back(token_stream.GetToken().string_value);
        token_stream.NextToken();
        while (token_stream.GetToken().token_type == Token_Comma) {
            token_stream.NextToken();
            if (token_stream.GetToken().token_type != Token_Identifier) {
                std::cout << "Syntax Error: identifier expected" << std::endl;
                exit(1);
            }
            _definition->identifiers.push_back(token_stream.GetToken().string_value);
            token_stream.NextToken();
        }
        return _definition;
    }
    if (token_stream.GetToken().token_type == Token_Identifier) {
        std::shared_ptr <Assignment> _assignment = std::make_shared <Assignment> ();
        _assignment->identifier = token_stream.GetToken().string_value;
        token_stream.NextToken();
        if (token_stream.GetToken().token_type != Token_Assignment) {
            std::cout << "Syntax Error: := expected" << std::endl;
            exit(1);
        }
        token_stream.NextToken();
        _assignment->expression = process_expression(token_stream);
        return _assignment;
    }
    std::cout << "Syntax Error: unexpected token" << std::endl;
    exit(1);
}

std::shared_ptr <Expression> process_expression(TokenStream &token_stream) {
    // foo(x + 3, go(foo(5 + "aba"[4]))) + ~(abacaba - *doo(foo))
    return process_primary(token_stream);
}

std::shared_ptr <Expression> process_primary(TokenStream &token_stream) {
    if (token_stream.GetToken().token_type == Token_Identifier) {
        std::shared_ptr <Identifier> _identifier = std::make_shared <Identifier> ();
        _identifier->identifier = token_stream.GetToken().string_value;
        token_stream.NextToken();
        return _identifier;
    }
    if (token_stream.GetToken().token_type == Token_ConstString) {
        std::shared_ptr <ConstString> _const_string = std::make_shared <ConstString> ();
        _const_string->const_string = token_stream.GetToken().string_value;
        token_stream.NextToken();
        return _const_string;
    }
    if (token_stream.GetToken().token_type == Token_ConstInt) {
        std::shared_ptr <ConstInt> _const_int = std::make_shared <ConstInt> ();
        _const_int->const_int = token_stream.GetToken().int_value;
        token_stream.NextToken();
        return _const_int;
    }
    std::cout << "Syntax Error: unexpected token" << std::endl;
    exit(1);
}
