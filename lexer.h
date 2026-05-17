#pragma once
#include <string>
#include <vector>

enum class TokenType {
    NUMBER, IDENT, KEYWORD,
    PLUS, MINUS, STAR, SLASH,
    ASSIGN, EQ, LT,GT,
    LBRACE, RBRACE, SEMI,
    END   // EOF
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& src);