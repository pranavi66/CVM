#pragma once//to prevent multiple inclusions of this header file
#include <string>
#include <vector>

enum class TokenType {     // Token types
    NUMBER, IDENT, KEYWORD,
    PLUS, MINUS, STAR, SLASH,
    ASSIGN, EQ, LT,GT,
    LBRACE, RBRACE, SEMI,
    END   // EOF
};

struct Token {  // Token structure
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& src);// Tokenizer function declaration

/*using namespace std; is not used because  it creates a global
namespace pollution and can lead to name conflicts,especially 
in larger projects. By not using it, we ensure that we explicitly
specify the namespace for each standard library component, which 
improves code readability and maintainability.*/