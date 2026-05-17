#pragma once
#include "lexer.h"
#include "ast.h"

unique_ptr<ProgramNode> parse(const vector<Token>& tokens);