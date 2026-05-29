#pragma once
#include "lexer.h"
#include "ast.h"
using namespace std;

unique_ptr<ProgramNode> parseTradingScript(const vector<Token>& tokens);