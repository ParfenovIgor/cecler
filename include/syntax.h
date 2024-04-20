#pragma once

#include <memory>
#include "../include/token.h"
#include "../include/ast.h"

std::shared_ptr <AST> syntax(TokenStream token_stream);
