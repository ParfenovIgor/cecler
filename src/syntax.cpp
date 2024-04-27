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
        if (_statement)
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
    if (token_stream.GetToken().token_type == Token_Semicolon) {
        token_stream.NextToken();
        return nullptr;
    }
    if (token_stream.GetToken().token_type == Token_BraceOpen) {
        return process_block(token_stream);
    }
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
        std::string identifier = token_stream.GetToken().string_value;
        token_stream.NextToken();
        if (token_stream.GetToken().token_type == Token_Assignment) {
            token_stream.NextToken();
            std::shared_ptr <Assignment> _assignment = std::make_shared <Assignment> ();
            _assignment->identifier = identifier;
            _assignment->expression = process_expression(token_stream);
            return _assignment;
        }
        else if (token_stream.GetToken().token_type == Token_Movement) {
            token_stream.NextToken();
            std::shared_ptr <Movement> _movement = std::make_shared <Movement> ();
            _movement->identifier = identifier;
            _movement->expression = process_expression(token_stream);
            return _movement;
        }
        else if (token_stream.GetToken().token_type == Token_ParenthesisOpen) {
            token_stream.NextToken();
            std::shared_ptr <FunctionCall> _function_call = std::make_shared <FunctionCall> ();
            _function_call->name = identifier;

            if (token_stream.GetToken().token_type != Token_ParenthesisClose) {
                _function_call->arguments.push_back(process_expression(token_stream));                
                while (token_stream.GetToken().token_type == Token_Comma) {
                    token_stream.NextToken();
                    if (token_stream.GetToken().token_type == Token_ParenthesisClose) {
                        std::cout << "Syntax Error: expression expected" << std::endl;
                        exit(1);
                    }
                    _function_call->arguments.push_back(process_expression(token_stream));
                }
                if (token_stream.GetToken().token_type != Token_ParenthesisClose) {
                    std::cout << "Syntax Error: ) expected" << std::endl;
                    exit(1);
                }
            }
            token_stream.NextToken();

            return _function_call;
        }
        else {
            std::cout << "Syntax Error: := or <- expected" << std::endl;
            exit(1);
        }
    }
    if (token_stream.GetToken().token_type == Token_If) {
        std::shared_ptr <If> _if = std::make_shared <If> ();
        token_stream.NextToken();
        if (token_stream.GetToken().token_type != Token_ParenthesisOpen) {
            std::cout << "Syntax Error: ( expected" << std::endl;
            exit(1);
        }
        token_stream.NextToken();
        _if->expression = process_expression(token_stream);
        if (token_stream.GetToken().token_type != Token_ParenthesisClose) {
            std::cout << "Syntax Error: ) expected" << std::endl;
            exit(1);
        }
        token_stream.NextToken();
        _if->block_then = process_block(token_stream);
        if (token_stream.GetToken().token_type == Token_Else) {
            token_stream.NextToken();
            _if->block_else = process_block(token_stream);
        }
        else
            _if->block_else = nullptr;
        return _if;
    }
    if (token_stream.GetToken().token_type == Token_While) {
        std::shared_ptr <While> _while = std::make_shared <While> ();
        token_stream.NextToken();
        if (token_stream.GetToken().token_type != Token_ParenthesisOpen) {
            std::cout << "Syntax Error: ( expected" << std::endl;
            exit(1);
        }
        token_stream.NextToken();
        _while->expression = process_expression(token_stream);
        if (token_stream.GetToken().token_type != Token_ParenthesisClose) {
            std::cout << "Syntax Error: ) expected" << std::endl;
            exit(1);
        }
        token_stream.NextToken();
        _while->block = process_block(token_stream);
        return _while;
    }
    if (token_stream.GetToken().token_type == Token_Func) {
        std::shared_ptr <FunctionDefinition> _function_def = std::make_shared <FunctionDefinition> ();
        token_stream.NextToken();
        if (token_stream.GetToken().token_type != Token_Identifier) {
            std::cout << "Syntax Error: identifier expected" << std::endl;
            exit(1);
        }
        _function_def->name = token_stream.GetToken().string_value;
        token_stream.NextToken();

        if (token_stream.GetToken().token_type != Token_ParenthesisOpen) {
            std::cout << "Syntax Error: ( expected" << std::endl;
            exit(1);
        }
        token_stream.NextToken();
        
        if (token_stream.GetToken().token_type == Token_Identifier) {
            _function_def->arguments.push_back(token_stream.GetToken().string_value);
            token_stream.NextToken();
            while (token_stream.GetToken().token_type == Token_Comma) {
                token_stream.NextToken();
                if (token_stream.GetToken().token_type != Token_Identifier) {
                    std::cout << "Syntax Error: identifier expected" << std::endl;
                    exit(1);
                }
                _function_def->arguments.push_back(token_stream.GetToken().string_value);
                token_stream.NextToken();
            }
        }

        if (token_stream.GetToken().token_type != Token_ParenthesisClose) {
            std::cout << "Syntax Error: ) expected" << std::endl;
            exit(1);
        }
        token_stream.NextToken();

        if (token_stream.GetToken().token_type == Token_BraceOpen)
            _function_def->block = process_block(token_stream);
        else
            _function_def->block = nullptr;
        return _function_def;
    }
    std::cout << "Syntax Error: unexpected token" << std::endl;
    exit(1);
}

int operator_priority(TokenType token_type) {
    if (token_type == Token_ParenthesisOpen) return 0;
    if (token_type == Token_ParenthesisClose) return 0;
    if (token_type == Token_Plus) return 1;
    if (token_type == Token_Minus) return 1;
    if (token_type == Token_Mult) return 2;
    return -1;
}

void apply_operation(TokenStream &token_stream,
                     std::vector <std::shared_ptr <Expression>> &exprs,
                     std::vector <TokenType> &opers) {
    if (exprs.size() < 2 || opers.back() == Token_ParenthesisOpen) {
        std::cout << "Syntax Error: incorrect expression" << std::endl;
        exit(1); 
    }
    std::shared_ptr <BinaryOperator> oper;
    if (opers.back() == Token_Plus) oper = std::make_shared <Addition> ();
    if (opers.back() == Token_Minus) oper = std::make_shared <Subtraction> ();
    if (opers.back() == Token_Mult) oper = std::make_shared <Multiplication> ();
    oper->left = exprs[exprs.size() - 2];
    oper->right = exprs[exprs.size() - 1];
    exprs.pop_back();
    exprs.pop_back();
    exprs.push_back(oper);
    opers.pop_back();
}

enum ExprState {
    State_Start,
    State_Expr,
    State_Operator,
    State_ParOpen,
    State_ParClose,
};

// expression
// operator
// (
// )
// 
// start -> expression, (
// expression -> operator, ), finish
// operator -> expression, (
// ( -> expression, (
// ) -> operator, ), finish

std::shared_ptr <Expression> process_expression(TokenStream &token_stream) {
    // foo(x + 3, go(foo(5 + "aba"[4]))) + ~(abacaba - *doo(foo))
    std::vector <std::shared_ptr <Expression>> exprs;
    std::vector <TokenType> opers;
    ExprState state = State_Start;
    int parenthesisLevel = 0;

    while  (state == State_Start ||
            state == State_Expr && (
                operator_priority(token_stream.GetToken().token_type) > 0 || 
                token_stream.GetToken().token_type == Token_ParenthesisClose && parenthesisLevel != 0
            ) ||
            state == State_Operator ||
            state == State_ParOpen ||
            state == State_ParClose && (
                operator_priority(token_stream.GetToken().token_type) > 0 || 
                token_stream.GetToken().token_type == Token_ParenthesisClose && parenthesisLevel != 0
            )
            ){
        if (token_stream.GetToken().token_type == Token_ParenthesisOpen) {
            opers.push_back(Token_ParenthesisOpen);
            token_stream.NextToken();
            parenthesisLevel++;
            state = State_ParOpen;
            continue;
        }

        if (token_stream.GetToken().token_type == Token_ParenthesisClose) {
            while (!opers.empty() && opers.back() != Token_ParenthesisOpen) {
                apply_operation(token_stream, exprs, opers);
            }
            if (opers.empty() || opers.back() != Token_ParenthesisOpen) {
                std::cout << "Syntax Error: incorrect expression" << std::endl;
                exit(1);
            }
            opers.pop_back();
            token_stream.NextToken();
            parenthesisLevel--;
            state = State_ParClose;
            continue;
        }

        if (operator_priority(token_stream.GetToken().token_type) > 0) {
            // All operators are left-associative
            while (!opers.empty() && 
                operator_priority(opers.back()) >= 
                operator_priority(token_stream.GetToken().token_type)) {
                apply_operation(token_stream, exprs, opers);
            }
            opers.push_back(token_stream.GetToken().token_type);
            token_stream.NextToken();
            state = State_Operator;
            continue;
        }

        exprs.push_back(process_primary(token_stream));
        state = State_Expr;
        continue;
    }

    while (!opers.empty()) {
        apply_operation(token_stream, exprs, opers);
    }

    if (exprs.size() != 1) {
        std::cout << "Syntax Error: incorrect expression" << std::endl;
        exit(1);
    }

    return exprs[0];
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
